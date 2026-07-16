#!/usr/bin/env python3
"""Convert native reccmp JSON plus a Ghidra inventory to objdiff report v2."""

from __future__ import annotations

import argparse
import csv
import json
import struct
from collections import defaultdict
from pathlib import Path, PurePosixPath

from reccmp.compare.report import deserialize_reccmp_report
from reccmp.types import EntityType


TARGET_NAME = "LEMBALL"
INCLUDED_CATEGORIES = {"internal", "thunk"}
DEFAULT_MANIFEST = Path("data/manifest.json")
DEFAULT_RECCMP_REPORT = Path("build/reccmp-sample.json")
DEFAULT_ROADMAP = Path("build/reccmp-roadmap.csv")
DEFAULT_OUTPUT = Path("build/report.json")
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
    functions: list[dict[str, object]], total_units: int = 1,
    complete_units: int | None = None
) -> dict[str, object]:
    total_code = sum(int(function["size"]) for function in functions)
    matched = [function for function in functions if float(function["ratio"]) == 1.0]
    matched_code = sum(int(function["size"]) for function in matched)
    fuzzy_weight = sum(
        float(function["ratio"]) * 100.0 * int(function["size"])
        for function in functions
    )
    result: dict[str, object] = {
        "matched_data_percent": 100.0,
        "complete_data_percent": 100.0,
        "total_units": total_units,
    }
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
    if complete_units is None:
        complete_units = total_units if functions and len(matched) == len(functions) else 0
    if complete_units:
        result["complete_units"] = complete_units
    return result


def objdiff_item(function: dict[str, object]) -> dict[str, object]:
    address = int(function["address"])
    ratio = float(function["ratio"]) * 100.0
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
    module_name = str(path.parent) if str(path.parent) != "." else "Root"
    return module_name, path.name


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
    manifest_path: Path, reccmp_path: Path, roadmap_path: Path
) -> list[dict[str, object]]:
    manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    if manifest.get("version") != TARGET_NAME:
        raise SystemExit(f"{manifest_path} is not a {TARGET_NAME} manifest")

    native = deserialize_reccmp_report(reccmp_path.read_text(encoding="utf-8"))
    if native.filename.casefold() != str(manifest["program"]).casefold():
        raise SystemExit(
            f"report is for {native.filename}, manifest is for {manifest['program']}"
        )

    matches = {
        int(entity.orig_addr, 16): entity
        for entity in native.entities.values()
        if entity.type in (None, EntityType.FUNCTION)
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
        match = matches.get(address)
        if match is not None and match.is_stub:
            continue
        functions.append(
            {
                "address": address,
                "name": entry["name"] if match is None else match.name,
                "size": int(entry["size"]),
                "ratio": (
                    0.0 if match is None else float(match.effective_accuracy)
                ),
                "module": modules.get(address, "Unassigned"),
            }
        )
    return functions


def build_report(
    manifest_path: Path, reccmp_path: Path, roadmap_path: Path
) -> dict[str, object]:
    functions = load_functions(manifest_path, reccmp_path, roadmap_path)
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
                    **(
                        {}
                        if module == "Unassigned"
                        else {"source_path": f"src/{module}"}
                    ),
                },
            }
        )
    complete_units = sum(
        all(float(function["ratio"]) == 1.0 for function in module_functions)
        for module_functions in grouped.values()
    )
    report_measures = measures(functions, len(units), complete_units)
    return {"measures": report_measures, "units": units, "version": 2}


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--reccmp-report", type=Path, default=DEFAULT_RECCMP_REPORT)
    parser.add_argument("--roadmap", type=Path, default=DEFAULT_ROADMAP)
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    for path in (args.manifest, args.reccmp_report, args.roadmap):
        if not path.is_file():
            raise SystemExit(f"required input does not exist: {path}")
    report = build_report(args.manifest, args.reccmp_report, args.roadmap)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    values = report["measures"]
    print(
        f"{values.get('matched_functions', 0)}/{values.get('total_functions', 0)} "
        f"functions matched; fuzzy {values.get('fuzzy_match_percent', 0.0):.1f}%"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
