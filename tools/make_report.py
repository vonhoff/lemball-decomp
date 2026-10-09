#!/usr/bin/env python3
"""Generate objdiff progress from raw assembly comparisons."""

from collections import defaultdict
from pathlib import PureWindowsPath
from typing import Any, cast

from reccmp.compare import Compare
from reccmp.compare.db import ReccmpMatch
from reccmp.formats.pe import PEImage
from reccmp.project.detect import DetectWhat, detect_project
from reccmp.project.detect import RecCmpProject
from reccmp.tools.roadmap import ModuleMap
from reccmp.types import EntityType, ImageId

from lib import BUILD, ROOT, TARGET_ID
from lib.progress import effective_code_percent, save_progress


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


def build_report(engine, modules: ModuleMap) -> tuple[dict[str, Any], set[int]]:
    """Map the original code inventory to objdiff functions and PDB units."""
    code_regions = [
        range(region.addr, region.addr + len(region.data))
        for region in engine.orig_bin.get_code_regions()
    ]
    groups = defaultdict(list)
    accepted = set()
    for entity in engine.get_all():
        address = entity.orig_addr
        size = entity.size(ImageId.ORIG)
        if (
            address is None
            or size is None
            or not any(address in region for region in code_regions)
        ):
            continue
        comparison = (
            engine.function_comparator.compare_function(cast(ReccmpMatch, entity))
            if entity.matched and size and not entity.get("stub")
            else None
        )
        if comparison and comparison.match_ratio != 1 and comparison.is_effective_match:
            accepted.add(address)
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
                "fuzzy_match_percent": comparison.match_ratio * 100
                if comparison
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
    return {
        "version": 2,
        "units": units,
        "measures": measures(functions, len(units)),
    }, accepted


def main() -> int:
    detect_project(
        project_directory=ROOT,
        search_path=[ROOT / "data"],
        detect_what=DetectWhat.ORIGINAL,
    )
    target = RecCmpProject.from_directory(BUILD).get(TARGET_ID)
    engine = Compare.from_target(target)
    report, accepted = build_report(
        engine,
        ModuleMap(target.recompiled_pdb, cast(PEImage, engine.recomp_bin)),
    )
    save_progress(report, accepted)
    totals = report["measures"]
    effective = effective_code_percent(report, accepted)
    print(
        f"Report: {totals['total_functions']:,} functions; "
        f"{totals['matched_code_percent']:.2f}% exact; "
        f"{effective:.2f}% effective; "
        f"{totals['fuzzy_match_percent']:.2f}% fuzzy"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
