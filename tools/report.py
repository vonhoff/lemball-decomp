#!/usr/bin/env python3
"""Generate objdiff progress from raw assembly comparisons."""

import json
from collections import defaultdict

from reccmp.compare.report import serialize_reccmp_report
from reccmp.project.detect import DetectWhat, detect_project
from reccmp.tools.roadmap import ModuleMap

from badges import effective_totals
from lib import RECCMP_JSON, REPORT_JSON, ROOT, load_engine
from lib.extents import original_functions, target_size


def measures(functions, total_units=1):
    """Count exact matches and weight raw scores by code size; empty totals score zero."""
    total_code = sum(int(function["size"]) for function in functions)
    exact = [
        function for function in functions if function["fuzzy_match_percent"] == 100
    ]
    exact_code = sum(int(function["size"]) for function in exact)
    weighted_score = sum(
        function["fuzzy_match_percent"] * int(function["size"])
        for function in functions
    )
    return {
        "total_units": total_units,
        "total_code": str(total_code),
        "matched_code": str(exact_code),
        "fuzzy_match_percent": weighted_score / total_code if total_code else 0.0,
        "matched_code_percent": exact_code / total_code * 100 if total_code else 0.0,
        "total_functions": len(functions),
        "matched_functions": len(exact),
        "matched_functions_percent": len(exact) / len(functions) * 100
        if functions
        else 0.0,
    }


def function_record(entity, comparison):
    """Represent one original function using its raw comparison score."""
    matched = comparison is not None and comparison.is_matched()
    return {
        "name": f"0x{entity.orig_addr:08x}",
        "size": str(target_size(entity)),
        "metadata": {
            "virtual_address": str(entity.orig_addr),
            "demangled_name": comparison.name if matched else entity.name or "",
        },
        "fuzzy_match_percent": comparison.accuracy * 100
        if matched and not comparison.is_stub
        else 0.0,
    }


def group_functions(entities, comparisons, modules):
    """Group function records by their upstream PDB module."""
    groups = defaultdict(list)
    for entity in entities:
        module = (
            modules.get_module(entity.recomp_addr)
            if entity.recomp_addr is not None
            else None
        )
        name = module[1] if module else ""
        groups[name or "Compiler-generated"].append(
            function_record(entity, comparisons.entities.get(entity.orig_addr))
        )
    return groups


def build_report(groups):
    """Assemble ordered units and their code-size-weighted totals."""
    units = []
    for name, functions in sorted(groups.items()):
        functions.sort(
            key=lambda function: int(function["metadata"]["virtual_address"])
        )
        unit = {"name": name, "measures": measures(functions), "functions": functions}
        units.append(unit)
    functions = [function for unit in units for function in unit["functions"]]
    return {"version": 2, "units": units, "measures": measures(functions, len(units))}


def main():
    detect_project(
        project_directory=ROOT,
        search_path=[ROOT / "data"],
        detect_what=DetectWhat.ORIGINAL,
    )
    target, engine = load_engine()
    comparisons = engine.to_report(filename=target.original_path.name)
    modules = ModuleMap(target.recompiled_pdb, engine.recomp_bin)
    groups = group_functions(original_functions(engine), comparisons, modules)
    report = build_report(groups)
    RECCMP_JSON.write_text(
        serialize_reccmp_report(comparisons, diff_included=True), encoding="utf-8"
    )
    REPORT_JSON.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    values = report["measures"]
    effective_code, effective_count = effective_totals(report, comparisons)
    total_code = int(values["total_code"])
    effective_percent = effective_code / total_code * 100 if total_code else 0.0
    print(
        f"Exact: {values['matched_code_percent']:.2f}% code; "
        f"{int(values['matched_code']):,}/{total_code:,} bytes; "
        f"{values['matched_functions']:,}/{values['total_functions']:,} functions"
    )
    print(f"Fuzzy: {values['fuzzy_match_percent']:.2f}% code; "
          "raw assembly similarity weighted by original bytes")
    print(
        f"Effective: {effective_percent:.2f}% code; {effective_code:,}/{total_code:,} bytes; "
        f"{effective_count:,}/{values['total_functions']:,} functions (exact + equivalent)"
    )


if __name__ == "__main__":
    main()
