#!/usr/bin/env python3
"""Generate objdiff progress from raw assembly comparisons."""

import json
from collections import defaultdict
from pathlib import Path

from reccmp.compare.csv import csv_parse
from reccmp.compare.report import serialize_reccmp_report
from reccmp.formats.exceptions import InvalidVirtualAddressError
from reccmp.project.detect import DetectWhat, detect_project
from reccmp.tools.roadmap import ModuleMap
from reccmp.types import EntityType, ImageId

from lib import BUILD, RECCMP_JSON, REPORT_JSON, ROOT, load_engine

TARGET_SIZES = ROOT / "tools/data/original-function-sizes.csv"


def load_target_sizes(path=TARGET_SIZES):
    """Load original-binary extents using the upstream CSV parser."""
    sizes = {}
    for address, values in csv_parse(path.read_text(encoding="utf-8")):
        size = values.get("size", 0)
        if size <= 0 or address in sizes:
            raise ValueError(f"Invalid or duplicate target extent at 0x{address:08x}")
        sizes[address] = size
    return sizes


def target_size(entity, sizes):
    """Use an explicit original size or recorded original extent; never the rebuilt size."""
    size = entity.size(ImageId.ORIG)
    if size is None:
        size = sizes.get(entity.orig_addr)
    if size is None or size <= 0:
        raise ValueError(f"Missing original function extent at 0x{entity.orig_addr:08x}")
    return size


def measures(functions, total_units=1):
    """Count exact matches and weight raw scores by code size; empty totals score zero."""
    total_code = sum(int(function["size"]) for function in functions)
    exact = [function for function in functions if function["fuzzy_match_percent"] == 100]
    exact_code = sum(int(function["size"]) for function in exact)
    weighted_score = sum(
        function["fuzzy_match_percent"] * int(function["size"]) for function in functions
    )
    return {
        "total_units": total_units,
        "total_code": str(total_code),
        "matched_code": str(exact_code),
        "fuzzy_match_percent": weighted_score / total_code if total_code else 0.0,
        "matched_code_percent": exact_code / total_code * 100 if total_code else 0.0,
        "total_functions": len(functions),
        "matched_functions": len(exact),
        "matched_functions_percent": len(exact) / len(functions) * 100 if functions else 0.0,
    }


def function_record(entity, comparison, size):
    """Represent one original function using its raw comparison score."""
    name = entity.name or ""
    score = 0.0
    if comparison is not None:
        name = comparison.name
        if not comparison.is_stub:
            score = comparison.accuracy * 100
    return {
        "name": f"0x{entity.orig_addr:08x}",
        "size": str(size),
        "metadata": {
            "virtual_address": str(entity.orig_addr),
            "demangled_name": name,
        },
        "fuzzy_match_percent": score,
    }


def original_functions(engine):
    """Yield upstream function kinds with original addresses in the PE sections."""
    for entity in engine.get_all():
        if (
            entity.entity_type not in (EntityType.FUNCTION, EntityType.VTORDISP)
            or entity.orig_addr is None
        ):
            continue
        try:
            engine.orig_bin.get_relative_addr(entity.orig_addr)
        except InvalidVirtualAddressError:
            continue
        yield entity


def group_functions(entities, comparisons, modules, sizes):
    """Group function records by their upstream PDB module."""
    matches = {
        address: match
        for address, match in comparisons.entities.items()
        if match.type == EntityType.FUNCTION and match.is_matched()
    }
    groups = defaultdict(list)
    for entity in entities:
        module = modules.get_module(entity.recomp_addr) if entity.recomp_addr is not None else None
        name = module[1] if module else ""
        name = name.removeprefix("CMakeFiles/LEMBALL.dir/src/").removesuffix(".obj")
        record = function_record(entity, matches.get(entity.orig_addr), target_size(entity, sizes))
        groups[name or "Compiler-generated"].append(record)
    return groups


def build_report(groups):
    """Assemble ordered units and their code-size-weighted totals."""
    units = []
    for name, functions in sorted(groups.items()):
        functions.sort(key=lambda function: int(function["metadata"]["virtual_address"]))
        unit = {"name": name, "measures": measures(functions), "functions": functions}
        source = Path("src") / name
        if (ROOT / source).is_file():
            unit["metadata"] = {"source_path": source.as_posix()}
        units.append(unit)
    functions = [function for unit in units for function in unit["functions"]]
    return {"version": 2, "units": units, "measures": measures(functions, len(units))}


def main():
    sizes = load_target_sizes()
    detect_project(
        project_directory=ROOT,
        search_path=[ROOT / "data"],
        detect_what=DetectWhat.ORIGINAL,
        build_directory=BUILD,
    )
    target, engine = load_engine()
    comparisons = engine.to_report(filename=target.original_path.name)
    modules = ModuleMap(target.recompiled_pdb, engine.recomp_bin)
    groups = group_functions(original_functions(engine), comparisons, modules, sizes)
    report = build_report(groups)
    RECCMP_JSON.write_text(
        serialize_reccmp_report(comparisons, diff_included=True), encoding="utf-8"
    )
    REPORT_JSON.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    values = report["measures"]
    print(
        f"Normalized-exact: {values['matched_functions']}/{values['total_functions']} functions, "
        f"{values['matched_code']}/{values['total_code']} bytes; "
        f"fuzzy: {values['fuzzy_match_percent']:.2f}%"
    )


if __name__ == "__main__":
    main()
