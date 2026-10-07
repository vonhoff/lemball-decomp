#!/usr/bin/env python3
"""Generate objdiff progress from raw assembly comparisons."""

import argparse
import hashlib
import json
from collections import defaultdict
from typing import Any, cast

from reccmp.compare import Compare
from reccmp.compare.csv import csv_parse
from reccmp.compare.report import serialize_reccmp_report
from reccmp.formats.pe import PEImage
from reccmp.project.detect import DetectWhat, detect_project
from reccmp.project.detect import RecCmpProject
from reccmp.tools.roadmap import ModuleMap
from reccmp.types import EntityType

from lib import BUILD, EFFECTIVE_JSON, REPORT_JSON, ROOT, TARGET_ID
from lib.progress import effective_code_percent
from link_binary import read_jump_target, thunk_symbol

REPORT_EXCLUSIONS = ROOT / "tools/data/report-exclusions.csv"


def measures(functions, total_units=1):
    """Count exact matches and weight raw scores by original code size."""
    total_code = 0
    exact_code = 0
    exact_count = 0
    fuzzy_weighted = 0.0
    for f in functions:
        size = int(f["size"])
        total_code += size
        score = f["fuzzy_match_percent"]
        fuzzy_weighted += score * size
        if score == 100:
            exact_code += size
            exact_count += 1
    total_functions = len(functions)
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


def build_report(engine, comparisons, modules) -> dict[str, Any]:
    """Map the original code inventory to objdiff functions and PDB units."""
    catalog = dict(
        csv_parse(
            (ROOT / "tools/data/original-function-sizes.csv").read_text(
                encoding="utf-8"
            )
        )
    )
    excluded_addresses = {
        address
        for address, _ in csv_parse(REPORT_EXCLUSIONS.read_text(encoding="utf-8"))
    }
    entities = {entity.orig_addr: entity for entity in engine.get_all()}
    groups = defaultdict(list)
    for address, original in catalog.items():
        if address in excluded_addresses:
            continue
        entity = entities[address]
        comparison = comparisons.entities.get(address)
        if entity.entity_type == EntityType.IMPORT_THUNK:
            continue
        module = (
            modules.get_module(entity.recomp_addr)[1]
            if entity.recomp_addr
            else "Unknown"
        )
        name = (
            comparison.name
            if comparison and entity.recomp_addr
            else entity.name or entity.get("symbol") or ""
        )
        if not name and original["size"] == 5:
            destination = read_jump_target(engine.orig_bin, address)
            if destination != address and destination in catalog:
                name = thunk_symbol(address)
        if name.startswith("__lemball_jump_"):
            module = "Jump thunks"
        groups[module].append(
            {
                "name": f"0x{address:08x}",
                "size": str(original["size"]),
                "metadata": {
                    "virtual_address": str(address),
                    "demangled_name": name,
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


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.parse_args()

    detect_project(
        project_directory=ROOT,
        search_path=[ROOT / "data"],
        detect_what=DetectWhat.ORIGINAL,
    )
    target = RecCmpProject.from_directory(BUILD).get(TARGET_ID)
    engine = Compare.from_target(target)
    comparisons = engine.to_report(filename=target.original_path.name)
    report = build_report(
        engine,
        comparisons,
        ModuleMap(target.recompiled_pdb, cast(PEImage, engine.recomp_bin)),
    )
    accepted = {
        address
        for address, comparison in comparisons.entities.items()
        if comparison.is_function()
        and comparison.is_matched()
        and not comparison.is_stub
        and comparison.effective_accuracy == 1
    }
    (BUILD / "reccmp.json").write_text(
        serialize_reccmp_report(comparisons, diff_included=True), encoding="utf-8"
    )
    report_bytes = (json.dumps(report, indent=2) + "\n").encode("utf-8")
    REPORT_JSON.write_bytes(report_bytes)
    EFFECTIVE_JSON.write_text(
        json.dumps(
            {
                "report_sha256": hashlib.sha256(report_bytes).hexdigest(),
                "addresses": sorted(accepted),
            },
            indent=2,
        )
        + "\n",
        encoding="utf-8",
    )
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
