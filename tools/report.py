#!/usr/bin/env python3
"""Generate objdiff progress from raw assembly comparisons."""

import argparse
import json
from collections import defaultdict
from typing import Any

from reccmp.compare.csv import csv_parse
from reccmp.compare.report import serialize_reccmp_report
from reccmp.project.detect import DetectWhat, detect_project
from reccmp.tools.roadmap import ModuleMap
from reccmp.types import EntityType

from lib import BUILD, EFFECTIVE_JSON, RECCMP_JSON, REPORT_JSON, ROOT, load_engine
from lib.effective import (
    additional_effective_matches,
    effective_addresses,
)
from lib.progress import (
    effective_measures,
    effective_snapshot,
    exact_regressions,
    load_progress,
)

REPORT_EXCLUSIONS = ROOT / "tools/data/report-exclusions.csv"


def measures(functions, total_units=1):
    """Count exact matches and weight raw scores by original code size."""
    total_code = sum(int(function["size"]) for function in functions)
    exact_sizes = [
        int(function["size"])
        for function in functions
        if function["fuzzy_match_percent"] == 100
    ]
    exact_code = sum(exact_sizes)
    return {
        "total_units": total_units,
        "total_code": str(total_code),
        "matched_code": str(exact_code),
        "fuzzy_match_percent": sum(
            f["fuzzy_match_percent"] * int(f["size"]) for f in functions
        )
        / total_code,
        "matched_code_percent": exact_code / total_code * 100,
        "total_functions": len(functions),
        "matched_functions": len(exact_sizes),
        "matched_functions_percent": len(exact_sizes) / len(functions) * 100,
    }


def is_catalogued_jump_thunk(
    image, address: int, size: int, catalog: dict[int, Any]
) -> bool:
    """Recognize a five-byte direct jump that forwards to another inventory entry."""
    if size != 5:
        return False
    instruction = bytes(image.read(address, 5))
    if instruction[0] != 0xE9:
        return False
    target = address + 5 + int.from_bytes(instruction[1:], "little", signed=True)
    return target != address and target in catalog


def read_report_exclusions() -> set[int]:
    """Read unmatched Visual C++ runtime targets excluded from game progress."""
    return {
        address
        for address, _ in csv_parse(REPORT_EXCLUSIONS.read_text(encoding="utf-8"))
    }


def is_visual_cpp_runtime_module(module: str) -> bool:
    """Recognize functions attributed to the linked Visual C++ runtime library."""
    return module.replace("/", "\\").casefold().startswith("build\\intel\\mt_obj\\")


def build_report(engine, comparisons, modules) -> dict[str, Any]:
    """Map the original code inventory to objdiff functions and PDB units."""
    catalog = dict(
        csv_parse(
            (ROOT / "tools/data/original-function-sizes.csv").read_text(
                encoding="utf-8"
            )
        )
    )
    excluded_addresses = read_report_exclusions()
    entities = {entity.orig_addr: entity for entity in engine.get_all()}
    groups = defaultdict(list)
    for address, original in catalog.items():
        if address in excluded_addresses:
            continue
        entity = entities[address]
        comparison = comparisons.entities.get(address)
        unmatched = entity.recomp_addr is None and not (
            comparison and comparison.is_matched()
        )
        if entity.entity_type == EntityType.IMPORT_THUNK or (
            unmatched
            and is_catalogued_jump_thunk(
                engine.orig_bin, address, original["size"], catalog
            )
        ):
            continue
        module = (
            modules.get_module(entity.recomp_addr)[1]
            if entity.recomp_addr
            else "Unknown"
        )
        if is_visual_cpp_runtime_module(module):
            continue
        groups[module].append(
            {
                "name": f"0x{address:08x}",
                "size": str(original["size"]),
                "metadata": {
                    "virtual_address": str(address),
                    "demangled_name": comparison.name
                    if comparison and entity.recomp_addr
                    else entity.name or "",
                },
                "fuzzy_match_percent": comparison.accuracy * 100
                if comparison and not comparison.is_stub
                else 0.0,
            }
        )
    units = [
        {"name": name, "measures": measures(functions), "functions": functions}
        for name, functions in sorted(groups.items())
    ]
    functions = [function for unit in units for function in unit["functions"]]
    return {"version": 2, "units": units, "measures": measures(functions, len(units))}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--check",
        action="store_true",
        help="Snapshot the saved report, regenerate, and fail on any lost exact match",
    )
    args = parser.parse_args()
    baseline = None
    if args.check:
        try:
            baseline, _ = load_progress(REPORT_JSON)
        except ValueError as exc:
            parser.error(str(exc))
        (BUILD / "report-baseline.json").write_bytes(REPORT_JSON.read_bytes())
    detect_project(
        project_directory=ROOT,
        search_path=[ROOT / "data"],
        detect_what=DetectWhat.ORIGINAL,
    )
    target, engine = load_engine()
    comparisons = engine.to_report(filename=target.original_path.name)
    report = build_report(
        engine, comparisons, ModuleMap(target.recompiled_pdb, engine.recomp_bin)
    )
    additional = additional_effective_matches(engine, comparisons.entities)
    accepted = effective_addresses(comparisons.entities, additional)
    RECCMP_JSON.write_text(
        serialize_reccmp_report(comparisons, diff_included=True), encoding="utf-8"
    )
    report_bytes = (json.dumps(report, indent=2) + "\n").encode("utf-8")
    REPORT_JSON.write_bytes(report_bytes)
    EFFECTIVE_JSON.write_text(
        json.dumps(effective_snapshot(report_bytes, accepted, additional), indent=2)
        + "\n",
        encoding="utf-8",
    )
    totals = report["measures"]
    effective = effective_measures(report, accepted)
    print(
        f"Report: {totals['total_functions']:,} functions; "
        f"{totals['matched_code_percent']:.2f}% exact; "
        f"{effective['matched_code_percent']:.2f}% effective; "
        f"{totals['fuzzy_match_percent']:.2f}% fuzzy"
    )
    if baseline is not None:
        previous = baseline["measures"]
        print(
            f"Delta: {totals['matched_functions'] - previous['matched_functions']:+d} exact functions; "
            f"{int(totals['matched_code']) - int(previous['matched_code']):+d} exact bytes; "
            f"{totals['fuzzy_match_percent'] - previous['fuzzy_match_percent']:+.4f} fuzzy points"
        )
        regressions = exact_regressions(baseline, report)
        for address, (old, new) in sorted(regressions.items()):
            score = "MISSING" if new is None else f"{new['fuzzy_match_percent']:.8f}%"
            print(
                f"REGRESSION 0x{address:08x}: 100% -> {score} {old['metadata']['demangled_name']}"
            )
        print(
            f"Exact audit: {len(regressions)} regressions; baseline={BUILD / 'report-baseline.json'}"
        )
        return int(bool(regressions))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
