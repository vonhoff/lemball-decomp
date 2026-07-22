#!/usr/bin/env python3
"""Convert native reccmp JSON plus a Ghidra inventory to objdiff report v2."""

from __future__ import annotations

import argparse
import csv
import json
import struct
import subprocess
import sys
from collections import defaultdict
from pathlib import Path, PurePosixPath

from reccmp.compare.report import deserialize_reccmp_report
from reccmp.types import EntityType

try:
    from function_inventory import reconcile_inventory, write_inventory_csv
except ModuleNotFoundError:
    from tools.function_inventory import reconcile_inventory, write_inventory_csv


TARGET_NAME = "LEMBALL"
INCLUDED_CATEGORIES = {"internal", "thunk"}
DEFAULT_MANIFEST = Path("data/manifest.json")
DEFAULT_RECCMP_REPORT = Path("build/reccmp-sample.json")
DEFAULT_ROADMAP = Path("build/reccmp-roadmap.csv")
DEFAULT_OUTPUT = Path("build/report.json")
DEFAULT_SOURCE_ROOT = Path("src")
DEFAULT_RUNTIME_SYMBOLS = Path("data/runtime-symbols.csv")
DEFAULT_INVENTORY_OUTPUT = Path("build/function-inventory.csv")
TEXT_VIRTUAL_ADDRESS = 0x401000


def objdiff_u64(value: int | float) -> str:
    return str(int(value))


def objdiff_f32(value: int | float) -> float:
    packed = struct.pack("<f", float(value))
    rounded = struct.unpack("<f", packed)[0]
    for precision in range(1, 10):
        text = format(rounded, f".{precision}g")
        if struct.pack("<f", float(text)) == packed:
            return float(text)
    return float(format(rounded, ".9g"))


def measures(
    functions: list[dict[str, object]], total_units: int = 1
) -> dict[str, object]:
    total_code = sum(int(function["size"]) for function in functions)
    matched = [
        function
        for function in functions
        if float(function["raw_accuracy"]) == 1.0
    ]
    matched_code = sum(int(function["size"]) for function in matched)
    # Report functions carry protobuf float fields.  Aggregate the same
    # float32 values that are serialized on those function records; weighting
    # the original Python doubles can differ by one float32 ULP when objdiff
    # (or our semantic validator) recomputes the unit/report measure.
    serialized_ratios = [
        objdiff_f32(float(function["raw_accuracy"]) * 100.0)
        for function in functions
    ]
    fuzzy_weight = sum(
        ratio * int(function["size"])
        for ratio, function in zip(serialized_ratios, functions)
    )
    result: dict[str, object] = {"total_units": total_units}
    if total_code:
        result.update(
            {
                "fuzzy_match_percent": objdiff_f32(fuzzy_weight / total_code),
                "total_code": objdiff_u64(total_code),
                "matched_code": objdiff_u64(matched_code),
                "matched_code_percent": objdiff_f32(matched_code / total_code * 100.0),
            }
        )
    if functions:
        result.update(
            {
                "total_functions": len(functions),
                "matched_functions": len(matched),
                "matched_functions_percent": objdiff_f32(
                    len(matched) / len(functions) * 100.0
                ),
            }
        )
    return result


def objdiff_item(function: dict[str, object]) -> dict[str, object]:
    address = int(function["address"])
    ratio = float(function["raw_accuracy"]) * 100.0
    item: dict[str, object] = {
        "name": str(function["name"]),
        "size": objdiff_u64(int(function["size"])),
        "metadata": {"virtual_address": objdiff_u64(address)},
        "address": objdiff_u64(address - TEXT_VIRTUAL_ADDRESS),
    }
    if ratio:
        item["fuzzy_match_percent"] = objdiff_f32(ratio)
    return item


def source_path_from_module(module: str) -> str:
    prefix = "CMakeFiles/LEMBALL.dir/src/"
    normalized = module.replace("\\", "/")
    if normalized.startswith(prefix):
        normalized = normalized[len(prefix) :]
        if normalized.lower().endswith(".obj"):
            normalized = normalized[:-4]
    return normalized


def report_unit_identity(module: str) -> tuple[str, str]:
    if module == "Unassigned":
        return "Unassigned", "Unassigned"
    path = PurePosixPath(module)
    parts = path.parts
    if parts[0] == "ENGINE" and len(parts) > 2:
        engine_clusters = {
            "CORE": "Core",
            "DEBUG": "Debug",
            "GDI": "GDI",
            "MEDIA": "Media",
            "NET": "Net",
        }
        cluster_name = f"Engine/{engine_clusters.get(parts[1], parts[1].title())}"
    elif parts[0] in {"FRONTEND", "LEVEL", "RESOURCE", "SHELL"}:
        cluster_name = parts[0].title()
    elif len(parts) == 1 and path.name == "GAME.CPP":
        cluster_name = "Game"
    elif parts[0] == "build":
        cluster_name = "Runtime"
    else:
        cluster_name = "Other"
    return cluster_name, path.name


def source_metadata(module: str) -> dict[str, str]:
    if module == "Unassigned" or not module.upper().endswith(".CPP"):
        return {}
    return {"source_path": f"src/{module}"}


def load_modules(roadmap_path: Path) -> dict[int, str]:
    modules: dict[int, str] = {}
    with roadmap_path.open(newline="", encoding="utf-8-sig") as roadmap_file:
        for row in csv.DictReader(roadmap_file):
            if (
                row["row_type"] != "fun"
                or not row["orig_addr"]
                or not row["module"]
            ):
                continue
            modules[int(row["orig_addr"], 16)] = source_path_from_module(row["module"])
    return modules


def load_functions(
    manifest_path: Path,
    reccmp_path: Path,
    roadmap_path: Path,
    source_root: Path = DEFAULT_SOURCE_ROOT,
    runtime_symbols_path: Path = DEFAULT_RUNTIME_SYMBOLS,
    inventory_output: Path | None = None,
) -> list[dict[str, object]]:
    native = deserialize_reccmp_report(reccmp_path.read_text(encoding="utf-8"))
    inventory, unresolved = reconcile_inventory(
        manifest_path,
        reccmp_path,
        roadmap_path,
        source_root,
        runtime_symbols_path,
        TARGET_NAME,
        native,
    )
    if inventory_output is not None:
        write_inventory_csv(inventory_output, inventory)
    if unresolved:
        examples = ", ".join(
            f"{row['address']} ({row['resolution_reason']})"
            for row in unresolved[:8]
        )
        suffix = "" if len(unresolved) <= 8 else ", ..."
        raise SystemExit(
            f"{len(unresolved)} unresolved function-inventory entities: "
            f"{examples}{suffix}"
        )

    manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    if manifest.get("version") != TARGET_NAME:
        raise SystemExit(f"{manifest_path} is not a {TARGET_NAME} manifest")

    if native.filename.casefold() != str(manifest["program"]).casefold():
        raise SystemExit(
            f"report is for {native.filename}, manifest is for {manifest['program']}"
        )

    matches = {
        int(entity.orig_addr, 16): entity
        for entity in native.entities.values()
        if entity.orig_addr and entity.type in (None, EntityType.FUNCTION)
    }
    modules = load_modules(roadmap_path)
    functions: list[dict[str, object]] = []
    seen: set[int] = set()
    for entry in manifest["functions"]:
        if entry["category"] not in INCLUDED_CATEGORIES:
            continue
        address = int(entry["address"], 16)
        if address in seen:
            raise SystemExit(f"duplicate manifest address 0x{address:08X}")
        seen.add(address)
        if int(entry["size"]) <= 0:
            raise SystemExit(f"invalid manifest size at 0x{address:08X}")
        match = matches.get(address)
        functions.append(
            {
                "address": address,
                "name": entry["name"] if match is None else match.name,
                "size": int(entry["size"]),
                "raw_accuracy": (
                    0.0
                    if match is None or match.is_stub
                    else float(match.accuracy)
                ),
                "effective_only": bool(
                    match is not None
                    and not match.is_stub
                    and match.is_effective_match
                    and float(match.accuracy) != 1.0
                ),
                "module": modules.get(address, "Unassigned"),
            }
        )
    expected_total = sum(
        entry["category"] in INCLUDED_CATEGORIES
        for entry in manifest["functions"]
    )
    if len(functions) != expected_total:
        raise SystemExit(
            f"report total {len(functions)} does not equal manifest reportable "
            f"total {expected_total}"
        )
    return functions


def build_report(
    manifest_path: Path,
    reccmp_path: Path,
    roadmap_path: Path,
    source_root: Path = DEFAULT_SOURCE_ROOT,
    runtime_symbols_path: Path = DEFAULT_RUNTIME_SYMBOLS,
    inventory_output: Path | None = None,
) -> dict[str, object]:
    functions = load_functions(
        manifest_path,
        reccmp_path,
        roadmap_path,
        source_root,
        runtime_symbols_path,
        inventory_output,
    )
    grouped: dict[str, list[dict[str, object]]] = defaultdict(list)
    for function in functions:
        grouped[str(function["module"])].append(function)

    units = []
    for module, module_functions in sorted(grouped.items()):
        module_name, unit_name = report_unit_identity(module)
        units.append(
            {
                "name": unit_name,
                "measures": measures(module_functions),
                "sections": [],
                "functions": [objdiff_item(function) for function in module_functions],
                "metadata": {
                    "module_name": module_name,
                    **source_metadata(module),
                },
            }
        )
    report_measures = measures(functions, len(units))
    return {"measures": report_measures, "units": units, "version": 2}


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--reccmp-report", type=Path, default=DEFAULT_RECCMP_REPORT)
    parser.add_argument("--roadmap", type=Path, default=DEFAULT_ROADMAP)
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    parser.add_argument("--source-root", type=Path, default=DEFAULT_SOURCE_ROOT)
    parser.add_argument("--runtime-symbols", type=Path, default=DEFAULT_RUNTIME_SYMBOLS)
    parser.add_argument("--inventory-output", type=Path, default=DEFAULT_INVENTORY_OUTPUT)
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    for path in (
        args.manifest,
        args.reccmp_report,
        args.roadmap,
        args.source_root,
        args.runtime_symbols,
    ):
        if not path.exists():
            raise SystemExit(f"required input does not exist: {path}")
    report = build_report(
        args.manifest,
        args.reccmp_report,
        args.roadmap,
        args.source_root,
        args.runtime_symbols,
        args.inventory_output,
    )
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    subprocess.run(
        [
            sys.executable,
            str(Path(__file__).with_name("validate_objdiff_report.py")),
            str(args.output),
        ],
        check=True,
    )
    values = report["measures"]
    effective_only = sum(
        bool(function["effective_only"])
        for function in load_functions(
            args.manifest,
            args.reccmp_report,
            args.roadmap,
            args.source_root,
            args.runtime_symbols,
        )
    )
    print(
        f"{values.get('matched_functions', 0)}/{values.get('total_functions', 0)} "
        f"functions matched; raw fuzzy "
        f"{values.get('fuzzy_match_percent', 0.0):.1f}%; "
        f"{effective_only} additional effective-only matches"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
