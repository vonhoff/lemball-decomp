#!/usr/bin/env python3
"""Generate objdiff progress from raw assembly comparisons."""

import json
from collections import defaultdict

from reccmp.compare.csv import csv_parse
from reccmp.compare.report import serialize_reccmp_report
from reccmp.project.detect import DetectWhat, detect_project
from reccmp.tools.roadmap import ModuleMap
from reccmp.types import ImageId

from lib import RECCMP_JSON, REPORT_JSON, ROOT, load_engine
from lib.effective import EFFECTIVE_JSON, additional_effective_matches


def measures(functions, total_units=1):
    """Count exact matches and weight raw scores by original code size."""
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
        "fuzzy_match_percent": weighted_score / total_code,
        "matched_code_percent": exact_code / total_code * 100,
        "total_functions": len(functions),
        "matched_functions": len(exact),
        "matched_functions_percent": len(exact) / len(functions) * 100,
    }


def build_report(engine, comparisons, modules):
    """Map the original code inventory to objdiff functions and PDB units."""
    catalog = dict(csv_parse(
        (ROOT / "tools/data/original-function-sizes.csv").read_text(encoding="utf-8")
    ))
    groups = defaultdict(list)
    for entity in engine.get_all():
        if entity.orig_addr not in catalog:
            continue
        comparison = comparisons.entities.get(entity.orig_addr)
        matched = comparison is not None and comparison.is_matched()
        module = modules.get_module(entity.recomp_addr) if entity.recomp_addr is not None else None
        groups[module[1] if module and module[1] else "Compiler-generated"].append({
            "name": f"0x{entity.orig_addr:08x}",
            "size": str(entity.size(ImageId.ORIG)),
            "metadata": {
                "virtual_address": str(entity.orig_addr),
                "demangled_name": comparison.name if matched else entity.name or "",
            },
            "fuzzy_match_percent": comparison.accuracy * 100
            if matched and not comparison.is_stub else 0.0,
        })
    units = []
    for name, functions in sorted(groups.items()):
        functions.sort(key=lambda function: int(function["metadata"]["virtual_address"]))
        units.append({"name": name, "measures": measures(functions), "functions": functions})
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
    report = build_report(engine, comparisons, modules)
    additional = additional_effective_matches(engine, comparisons.entities)
    reccmp_text = serialize_reccmp_report(comparisons, diff_included=True)
    RECCMP_JSON.write_text(reccmp_text, encoding="utf-8")
    REPORT_JSON.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    EFFECTIVE_JSON.write_text(json.dumps(additional, indent=2) + "\n", encoding="utf-8")
    values = report["measures"]
    print(f"Report: {values['total_functions']:,} functions; "
          f"{values['matched_code_percent']:.2f}% exact; "
          f"{values['fuzzy_match_percent']:.2f}% fuzzy")


if __name__ == "__main__":
    main()
