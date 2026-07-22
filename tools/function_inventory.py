#!/usr/bin/env python3
"""Shared reconciliation logic for Ghidra, reccmp, and source ownership."""

from __future__ import annotations

import csv
import json
import re
from pathlib import Path

from reccmp.compare.report import deserialize_reccmp_report
from reccmp.types import EntityType


REPORTABLE_CATEGORIES = {"internal", "thunk"}
EXCLUDED_CATEGORIES = {"runtime", "import", "external"}
STRICT_MARKER = re.compile(
    r"^// (?P<kind>FUNCTION|STUB|LIBRARY|SYNTHETIC|TEMPLATE): "
    r"(?P<target>[A-Z0-9_]+) 0x(?P<address>[0-9a-f]{8})(?: .*)?$"
)
SOURCE_SUFFIXES = {".cpp", ".h", ".hpp"}


def source_path_from_module(module: str) -> str:
    prefix = "CMakeFiles/LEMBALL.dir/src/"
    normalized = module.replace("\\", "/")
    if normalized.startswith(prefix):
        normalized = normalized[len(prefix) :]
        if normalized.lower().endswith(".obj"):
            normalized = normalized[:-4]
    return normalized


def load_manifest(path: Path, target: str) -> tuple[dict[str, object], dict[int, dict[str, object]]]:
    manifest = json.loads(path.read_text(encoding="utf-8"))
    if manifest.get("version") != target:
        raise SystemExit(f"{path} is not a {target} manifest")
    functions: dict[int, dict[str, object]] = {}
    for function in manifest.get("functions", []):
        address = int(str(function["address"]), 16)
        if address in functions:
            raise SystemExit(f"duplicate manifest address 0x{address:08X}")
        functions[address] = function
    return manifest, functions


def load_roadmap(path: Path) -> dict[int, dict[str, str]]:
    functions: dict[int, dict[str, str]] = {}
    with path.open(newline="", encoding="utf-8-sig") as stream:
        for row in csv.DictReader(stream):
            if row.get("row_type") != "fun" or not row.get("orig_addr"):
                continue
            address = int(row["orig_addr"], 16)
            functions[address] = {
                "module": source_path_from_module(row.get("module", "")) or "Unassigned",
                "rebuilt_address": row.get("recomp_addr", ""),
                "roadmap_name": row.get("name", ""),
            }
    return functions


def scan_source_markers(source_root: Path, target: str) -> dict[int, list[dict[str, object]]]:
    markers: dict[int, list[dict[str, object]]] = {}
    for path in sorted(source_root.rglob("*")):
        if not path.is_file() or path.suffix.lower() not in SOURCE_SUFFIXES:
            continue
        for line_number, line in enumerate(
            path.read_text(encoding="utf-8-sig").splitlines(), start=1
        ):
            match = STRICT_MARKER.match(line)
            if match is None or match.group("target") != target:
                continue
            address = int(match.group("address"), 16)
            markers.setdefault(address, []).append(
                {
                    "kind": match.group("kind"),
                    "path": path,
                    "line": line_number,
                }
            )
    return markers


def load_runtime_symbols(path: Path) -> dict[int, dict[str, str]]:
    symbols: dict[int, dict[str, str]] = {}
    with path.open(newline="", encoding="utf-8-sig") as stream:
        reader = csv.DictReader(
            line for line in stream if line.strip() and not line.lstrip().startswith("#")
        )
        for row in reader:
            address = int(row["address"], 16)
            if address in symbols:
                raise SystemExit(f"duplicate runtime-symbol address 0x{address:08X}")
            symbols[address] = {key: (value or "").strip() for key, value in row.items()}
    return symbols


def symbol_category(symbol: dict[str, str]) -> str:
    declared = symbol.get("manifest_category", "")
    if declared:
        return declared
    kind = symbol.get("type", "")
    return "runtime" if kind in {"library", "runtime"} else kind


def marker_text(markers: list[dict[str, object]]) -> str:
    return ";".join(
        f"{marker['kind']}:{marker['path']}:{marker['line']}" for marker in markers
    )


def reconcile_inventory(
    manifest_path: Path,
    reccmp_path: Path,
    roadmap_path: Path,
    source_root: Path,
    runtime_symbols_path: Path,
    target: str = "LEMBALL",
    native_report: object | None = None,
) -> tuple[list[dict[str, object]], list[dict[str, object]]]:
    manifest, manifest_functions = load_manifest(manifest_path, target)
    native = (
        deserialize_reccmp_report(reccmp_path.read_text(encoding="utf-8"))
        if native_report is None
        else native_report
    )
    if native.filename.casefold() != str(manifest["program"]).casefold():
        raise SystemExit(
            f"report is for {native.filename}, manifest is for {manifest['program']}"
        )
    matches = {
        int(entity.orig_addr, 16): entity
        for entity in native.entities.values()
        if entity.orig_addr and entity.type in (None, EntityType.FUNCTION)
    }
    roadmap = load_roadmap(roadmap_path)
    source_markers = scan_source_markers(source_root, target)
    runtime_symbols = load_runtime_symbols(runtime_symbols_path)

    rows: list[dict[str, object]] = []
    unresolved: list[dict[str, object]] = []
    for address in sorted(set(manifest_functions) | set(matches)):
        function = manifest_functions.get(address)
        match = matches.get(address)
        mapping = roadmap.get(address, {})
        markers = source_markers.get(address, [])
        category = "" if function is None else str(function.get("category", ""))
        runtime_symbol = runtime_symbols.get(address)
        resolution = ""
        problem = False

        if function is None:
            resolution = "missing_manifest_function"
            problem = match is not None
        elif category in REPORTABLE_CATEGORIES:
            if any(marker["kind"] == "LIBRARY" for marker in markers):
                resolution = "reportable_function_has_library_marker"
                problem = True
            elif match is None:
                resolution = "reportable_unimplemented"
            else:
                resolution = "reportable_reccmp_entity"
        elif category in EXCLUDED_CATEGORIES:
            if match is None:
                resolution = "excluded_without_reccmp_entity"
            elif runtime_symbol is None:
                resolution = "excluded_entity_missing_symbol_declaration"
                problem = True
            elif symbol_category(runtime_symbol) != category:
                resolution = "excluded_entity_category_conflict"
                problem = True
            elif any(marker["kind"] != "LIBRARY" for marker in markers):
                resolution = "excluded_entity_has_application_marker"
                problem = True
            else:
                resolution = f"explicit_{category}_exclusion"
        else:
            resolution = "unknown_manifest_category"
            problem = True

        row = {
            "address": f"0x{address:08X}",
            "name": (
                match.name
                if match is not None
                else "" if function is None else str(function.get("name", ""))
            ),
            "manifest_category": category,
            "thunk_target": "" if function is None else str(function.get("thunk_target", "")),
            "source_marker": marker_text(markers),
            "is_stub": bool(match is not None and match.is_stub),
            "raw_accuracy": (
                "" if match is None else f"{float(match.accuracy):.9g}"
            ),
            "module": str(mapping.get("module", "Unassigned")),
            "rebuilt_address": str(mapping.get("rebuilt_address", "")),
            "resolution_reason": resolution,
        }
        rows.append(row)
        if problem:
            unresolved.append(row)
    return rows, unresolved


def write_inventory_csv(path: Path, rows: list[dict[str, object]]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    fields = [
        "address",
        "name",
        "manifest_category",
        "thunk_target",
        "source_marker",
        "is_stub",
        "raw_accuracy",
        "module",
        "rebuilt_address",
        "resolution_reason",
    ]
    with path.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields)
        writer.writeheader()
        writer.writerows(rows)
