#!/usr/bin/env python3
"""Generate objdiff progress from raw assembly comparisons."""

from collections import defaultdict
from pathlib import PureWindowsPath
from typing import Any, cast

from reccmp.compare import Compare
from reccmp.compare.db import ReccmpMatch
from reccmp.compare.report import ReccmpComparedEntity, ReccmpStatusReport
from reccmp.formats.pe import PEImage
from reccmp.project.detect import DetectWhat, detect_project
from reccmp.project.detect import RecCmpProject
from reccmp.tools.roadmap import ModuleMap
from reccmp.types import EntityType, ImageId

from lib import BUILD, ROOT, TARGET_ID
from lib.codewarrior import decode_signature
from lib.names import read_catalog
from lib.progress import accepted_functions, effective_code_percent, save_progress


def measures(functions, total_units=1):
    """Count exact matches and weight raw scores by original code size."""
    total_code = 0
    exact_code = 0
    exact_count = 0
    fuzzy_weighted = 0.0
    total_functions = 0
    for f in functions:
        total_functions += 1
        size = int(f["size"])
        total_code += size
        score = f["fuzzy_match_percent"]
        fuzzy_weighted += score * size
        if score == 100:
            exact_code += size
            exact_count += 1
    return {
        "total_units": total_units,
        "total_code": str(total_code),
        "matched_code": str(exact_code),
        "fuzzy_match_percent": (fuzzy_weighted / total_code) if total_code else 0.0,
        "matched_code_percent": (exact_code / total_code * 100) if total_code else 0.0,
        "total_functions": total_functions,
        "matched_functions": exact_count,
        "matched_functions_percent": (
            (exact_count / total_functions * 100) if total_functions else 0.0
        ),
    }


def module_name(path):
    """Group runtime and generated entries; use source stems for other modules."""
    path = PureWindowsPath(path)
    if path.parent == PureWindowsPath("build/intel/mt_obj"):
        return "MSVC Runtime"
    if path in (
        PureWindowsPath("linker-thunks/entries.obj"),
        PureWindowsPath("linker-thunks/terminal-entry.obj"),
    ):
        return "Linker Thunks"
    return PureWindowsPath(path.stem).stem or "Unknown"


def compare_import_thunks(engine: Compare, comparisons: ReccmpStatusReport):
    """Score import jumps omitted by reccmp's default comparison inventory."""
    for entity in engine.get_all():
        address = entity.orig_addr
        if (
            address is None
            or entity.entity_type != EntityType.IMPORT_THUNK
            or address in comparisons.entities
        ):
            continue
        result = (
            engine.function_comparator.compare_function(cast(ReccmpMatch, entity))
            if entity.matched
            else None
        )
        comparisons.add_match(
            ReccmpComparedEntity(
                orig_addr=address,
                recomp_addr=entity.recomp_addr,
                name=entity.best_name() or f"0x{address:08x}",
                type=EntityType.IMPORT_THUNK,
                accuracy=result.match_ratio if result else 0.0,
                is_effective_match=result.is_effective_match if result else False,
                is_stub=entity.get("stub", False),
                is_library=entity.get("library", False),
                rdiff=result.diff if result else None,
            )
        )


def build_report(engine, comparisons, modules: ModuleMap) -> dict[str, Any]:
    """Map the original code inventory to objdiff functions and PDB units."""
    code_regions = [
        range(region.addr, region.addr + len(region.data))
        for region in engine.orig_bin.get_code_regions()
    ]
    groups = defaultdict(list)
    for entity in engine.get_all():
        address = entity.orig_addr
        size = entity.size(ImageId.ORIG)
        if (
            address is None
            or size is None
            or entity.entity_type
            not in (
                None,
                EntityType.FUNCTION,
                EntityType.VTORDISP,
                EntityType.THUNK,
                EntityType.IMPORT_THUNK,
            )
            or not any(address in region for region in code_regions)
        ):
            continue
        comparison = comparisons.entities.get(address)
        if entity.entity_type == EntityType.IMPORT_THUNK:
            unit_name = "Import Thunks"
        else:
            module = (
                modules.get_module(entity.recomp_addr)
                if entity.recomp_addr is not None
                else None
            )
            unit_name = module_name(module[1]) if module is not None else "Unknown"
        groups[unit_name].append(
            {
                "name": f"0x{address:08x}",
                "size": str(size),
                "metadata": {
                    "virtual_address": str(address),
                    "demangled_name": entity.best_name()
                    or entity.get("symbol")
                    or f"0x{address:08x}",
                },
                "fuzzy_match_percent": comparison.accuracy * 100
                if comparison and comparison.is_matched() and not comparison.is_stub
                else 0.0,
            }
        )
    units = [
        {
            "name": name,
            "measures": measures(functions),
            "functions": functions,
        }
        for name, functions in sorted(groups.items())
    ]
    functions = (function for unit in units for function in unit["functions"])
    return {"version": 2, "units": units, "measures": measures(functions, len(units))}


def check_catalog_implementations(comparisons):
    """Require rebuilt, non-stub code for every catalog-mapped Windows address."""
    symbols, mappings = read_catalog(ROOT / "tools/data/mac-symbol-catalog.csv")
    missing = [
        address
        for address in mappings
        if (entity := comparisons.entities.get(address)) is None
        or not entity.is_matched()
        or entity.is_stub
        or entity.type
        not in (
            EntityType.FUNCTION,
            EntityType.VTORDISP,
            EntityType.THUNK,
            EntityType.IMPORT_THUNK,
        )
    ]
    for address in sorted(missing):
        expected = "; ".join(
            decode_signature(symbols[mac]).display() for mac in mappings[address]
        )
        print(f"catalog: missing rebuilt implementation: {expected} [0x{address:08x}]")
    print(
        f"catalog: rebuilt implementations {len(mappings) - len(missing)}/{len(mappings)} "
        "mapped Windows addresses"
    )
    return int(bool(missing))


def main() -> int:
    detect_project(
        project_directory=ROOT,
        search_path=[ROOT / "data"],
        detect_what=DetectWhat.ORIGINAL,
    )
    target = RecCmpProject.from_directory(BUILD).get(TARGET_ID)
    engine = Compare.from_target(target)
    comparisons = engine.to_report(
        filename=target.original_path.name,
        filter_fn=lambda entity: entity.entity_type != EntityType.VTABLE,
    )
    compare_import_thunks(engine, comparisons)
    catalog_status = check_catalog_implementations(comparisons)
    report = build_report(
        engine,
        comparisons,
        ModuleMap(target.recompiled_pdb, cast(PEImage, engine.recomp_bin)),
    )
    accepted = accepted_functions(comparisons)
    save_progress(report, comparisons)
    totals = report["measures"]
    effective = effective_code_percent(report, accepted)
    print(
        f"Report: {totals['total_functions']:,} functions; "
        f"{totals['matched_code_percent']:.2f}% exact; "
        f"{effective:.2f}% effective; "
        f"{totals['fuzzy_match_percent']:.2f}% fuzzy"
    )
    return catalog_status


if __name__ == "__main__":
    raise SystemExit(main())
