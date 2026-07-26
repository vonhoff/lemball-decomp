#!/usr/bin/env python3
"""Shared reconciliation logic for Ghidra, reccmp, and source ownership."""

from __future__ import annotations

import csv
import json
import re
from copy import deepcopy
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
NETWORK_ADDRESS_RANGES = (
    (0x0045F3B0, 0x00462E5F),
    (0x0046F210, 0x0047204F),
    (0x004794E0, 0x0047C04F),
)
DEFAULT_OWNERSHIP_OVERRIDES = (
    Path(__file__).resolve().parent / "function-ownership-overrides.json"
)


def source_path_from_module(module: str) -> str:
    prefix = "CMakeFiles/LEMBALL.dir/src/"
    normalized = module.replace("\\", "/")
    if normalized.startswith(prefix):
        normalized = normalized[len(prefix) :]
        if normalized.lower().endswith(".obj"):
            normalized = normalized[:-4]
    return normalized


def read_roadmap_rows(path: Path) -> list[dict[str, str]]:
    """Read roadmap CSV despite pinned reccmp leaving name commas unquoted."""
    with path.open(encoding="utf-8-sig") as stream:
        header_line = stream.readline().rstrip("\r\n")
        header = header_line.split(",")
        if header == [
            "orig_sect_ofs",
            "recomp_sect_ofs",
            "orig_addr",
            "recomp_addr",
            "displacement",
            "row_type",
            "size",
            "name",
            "module",
        ]:
            rows: list[dict[str, str]] = []
            for line_number, line in enumerate(stream, start=2):
                fields = line.rstrip("\r\n").split(",", 7)
                if len(fields) != 8 or "," not in fields[7]:
                    raise SystemExit(f"malformed roadmap row {path}:{line_number}")
                name, module = fields[7].rsplit(",", 1)
                rows.append(dict(zip(header, [*fields[:7], name, module])))
            return rows

    with path.open(newline="", encoding="utf-8-sig") as stream:
        return list(csv.DictReader(stream))


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
    for row in read_roadmap_rows(path):
        if row.get("row_type") not in {"fun", "vto"} or not row.get("orig_addr"):
            continue
        address = int(row["orig_addr"], 16)
        functions[address] = {
            "module": source_path_from_module(row.get("module", "")) or "Unassigned",
            "rebuilt_address": row.get("recomp_addr", ""),
            "roadmap_name": row.get("name", ""),
            "ownership_basis": "roadmap",
        }
    return functions


def load_ownership_overrides(path: Path | None) -> dict[int, dict[str, str]]:
    if path is None or not path.exists():
        return {}
    document = json.loads(path.read_text(encoding="utf-8"))
    if document.get("schema_version") != 1:
        raise SystemExit(f"unsupported ownership override schema: {path}")
    overrides: dict[int, dict[str, str]] = {}
    for index, row in enumerate(document.get("entries", []), start=1):
        address = int(row["address"], 16)
        module = source_path_from_module(row.get("module", ""))
        evidence = row.get("evidence", "").strip()
        if address in overrides:
            raise SystemExit(f"duplicate ownership override {path}: entry {index}")
        if not module.upper().endswith(".CPP") or not evidence:
            raise SystemExit(f"incomplete ownership override {path}: entry {index}")
        overrides[address] = {
            "module": module,
            "rebuilt_address": "",
            "roadmap_name": "",
            "ownership_basis": f"override:{evidence}",
        }
    return overrides


def source_marker_module(
    markers: list[dict[str, object]], source_root: Path
) -> str | None:
    modules = {
        Path(str(marker["path"])).resolve().relative_to(source_root.resolve()).as_posix()
        for marker in markers
        if marker["kind"] != "LIBRARY"
    }
    if len(modules) > 1:
        raise SystemExit(f"conflicting source-marker modules: {sorted(modules)}")
    return next(iter(modules), None)


def infer_module_ownership(
    manifest_functions: dict[int, dict[str, object]],
    roadmap: dict[int, dict[str, str]],
    source_markers: dict[int, list[dict[str, object]]],
    source_root: Path,
    overrides: dict[int, dict[str, str]],
) -> dict[int, dict[str, str]]:
    """Fill original module ownership from explicit anchors and code layout."""

    mappings = deepcopy(roadmap)
    for address, markers in source_markers.items():
        module = source_marker_module(markers, source_root)
        if module is None:
            continue
        mapping = mappings.get(address)
        if mapping is None or mapping.get("module") == "Unassigned":
            mappings[address] = {
                "module": module,
                "rebuilt_address": "",
                "roadmap_name": "",
                "ownership_basis": "source_marker",
            }

    for address, override in overrides.items():
        function = manifest_functions.get(address)
        if function is None or function.get("category") not in REPORTABLE_CATEGORIES:
            raise SystemExit(f"ownership override has no reportable entity 0x{address:08X}")
        existing = mappings.get(address)
        if existing is not None and existing.get("module") not in {
            "Unassigned",
            override["module"],
        }:
            raise SystemExit(f"ownership override conflicts at 0x{address:08X}")
        mappings[address] = override

    internal_addresses = sorted(
        address
        for address, function in manifest_functions.items()
        if function.get("category") == "internal"
    )
    anchors = [
        address
        for address in internal_addresses
        if mappings.get(address, {}).get("module", "Unassigned") != "Unassigned"
    ]
    if internal_addresses and not anchors:
        return mappings

    anchor_set = set(anchors)
    unresolved_bodies = [
        address for address in internal_addresses if address not in anchor_set
    ]
    for address in unresolved_bodies:
        lower = next((anchor for anchor in reversed(anchors) if anchor < address), None)
        upper = next((anchor for anchor in anchors if anchor > address), None)
        if lower is None or upper is None:
            anchor = upper if lower is None else lower
            assert anchor is not None
            mappings[address] = {
                "module": mappings[anchor]["module"],
                "rebuilt_address": "",
                "roadmap_name": "",
                "ownership_basis": f"layout_one_sided:0x{anchor:08X}",
            }

    bounded = [
        address
        for address in unresolved_bodies
        if mappings.get(address, {}).get("module", "Unassigned") == "Unassigned"
    ]
    by_gap: dict[tuple[int, int], list[int]] = {}
    for address in bounded:
        lower = next(anchor for anchor in reversed(anchors) if anchor < address)
        upper = next(anchor for anchor in anchors if anchor > address)
        by_gap.setdefault((lower, upper), []).append(address)

    for (lower, upper), addresses in by_gap.items():
        lower_module = mappings[lower]["module"]
        upper_module = mappings[upper]["module"]
        if lower_module == upper_module:
            for address in addresses:
                mappings[address] = {
                    "module": lower_module,
                    "rebuilt_address": "",
                    "roadmap_name": "",
                    "ownership_basis": (
                        f"layout_same_flanks:0x{lower:08X}/0x{upper:08X}"
                    ),
                }
            continue

        sequence = [lower, *addresses, upper]
        gaps: list[tuple[int, int, int]] = []
        midpoint = (lower + upper) // 2
        for index, (left, right) in enumerate(zip(sequence, sequence[1:])):
            left_size = int(manifest_functions[left]["size"])
            padding = max(0, right - (left + left_size))
            boundary_distance = abs(((left + right) // 2) - midpoint)
            gaps.append((padding, -boundary_distance, index))
        _, _, split_index = max(gaps)
        split_after = sequence[split_index]
        for address in addresses:
            use_lower = address <= split_after
            mappings[address] = {
                "module": lower_module if use_lower else upper_module,
                "rebuilt_address": "",
                "roadmap_name": "",
                "ownership_basis": (
                    f"layout_gap_{'lower' if use_lower else 'upper'}:"
                    f"0x{lower:08X}/0x{upper:08X}@0x{split_after:08X}"
                ),
            }

    unresolved_thunks = {
        address
        for address, function in manifest_functions.items()
        if function.get("category") == "thunk"
        and mappings.get(address, {}).get("module", "Unassigned") == "Unassigned"
    }
    while unresolved_thunks:
        progress = False
        for address in list(unresolved_thunks):
            function = manifest_functions[address]
            target_value = function.get("thunk_target")
            if not target_value:
                continue
            target = int(str(target_value), 16)
            target_mapping = mappings.get(target)
            if (
                target_mapping is None
                or target_mapping.get("module", "Unassigned") == "Unassigned"
            ):
                continue
            mappings[address] = {
                "module": target_mapping["module"],
                "rebuilt_address": "",
                "roadmap_name": "",
                "ownership_basis": f"thunk_target:0x{target:08X}",
            }
            unresolved_thunks.remove(address)
            progress = True
        if not progress:
            break
    return mappings


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


def network_owned(
    address: int,
    function: dict[str, object] | None,
    markers: list[dict[str, object]],
) -> bool:
    if any(start <= address <= end for start, end in NETWORK_ADDRESS_RANGES):
        return True
    name = "" if function is None else str(function.get("name", "")).casefold()
    if any(token in name for token in ("vsnet", "tcpip", "efftransport", "socket")):
        return True
    return any(
        "/engine/net/" in str(marker["path"]).replace("\\", "/").casefold()
        for marker in markers
    )


def reconcile_inventory(
    manifest_path: Path,
    reccmp_path: Path,
    roadmap_path: Path,
    source_root: Path,
    runtime_symbols_path: Path,
    target: str = "LEMBALL",
    native_report: object | None = None,
    ownership_overrides_path: Path | None = None,
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
    roadmap = infer_module_ownership(
        manifest_functions,
        roadmap,
        source_markers,
        source_root,
        load_ownership_overrides(ownership_overrides_path),
    )

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

        if category in REPORTABLE_CATEGORIES and str(
            mapping.get("module", "Unassigned")
        ) == "Unassigned":
            resolution = (
                "network_entity_unassigned"
                if network_owned(address, function, markers)
                else "reportable_entity_unassigned"
            )
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
            "ownership_basis": str(mapping.get("ownership_basis", "")),
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
        "ownership_basis",
        "rebuilt_address",
        "resolution_reason",
    ]
    with path.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields)
        writer.writeheader()
        writer.writerows(rows)
