#!/usr/bin/env python3
"""Generate objdiff progress from raw assembly comparisons."""

import argparse
import hashlib
from collections.abc import Collection
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

from lib.project import BUILD, EFFECTIVE_JSON, RECCMP_JSON, REPORT_JSON, ROOT, TARGET_ID
from lib.comparison.thunks import read_jump_target
from lib.comparison.matches import additional_effective_matches
from lib.progress.metrics import effective_measures
from lib.progress.snapshot import EFFECTIVE_POLICY

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


def is_catalogued_jump_thunk(
    image, address: int, size: int, catalog: dict[int, Any]
) -> bool:
    """Recognize a five-byte direct jump that forwards to another inventory entry."""
    if size != 5:
        return False
    target = read_jump_target(image, address)
    return target is not None and target != address and target in catalog


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


def effective_addresses(
    comparisons: dict, additional: Collection[int] = ()
) -> set[int]:
    """Function addresses counted by the Effective metric."""
    return {
        address
        for address, comparison in comparisons.items()
        if comparison.is_function()
        and comparison.is_matched()
        and not comparison.is_stub
        and (comparison.effective_accuracy == 1 or address in additional)
    }


def effective_snapshot(report_bytes: bytes, accepted: set[int]) -> dict[str, Any]:
    """Bind accepted addresses to the exact canonical report that produced them."""
    return {
        "version": 1,
        "policy": EFFECTIVE_POLICY,
        "report_sha256": hashlib.sha256(report_bytes).hexdigest(),
        "addresses": sorted(accepted),
    }


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
    additional = additional_effective_matches(engine, comparisons.entities)
    accepted = effective_addresses(comparisons.entities, additional)
    RECCMP_JSON.write_text(
        serialize_reccmp_report(comparisons, diff_included=True), encoding="utf-8"
    )
    report_bytes = (json.dumps(report, indent=2) + "\n").encode("utf-8")
    REPORT_JSON.write_bytes(report_bytes)
    EFFECTIVE_JSON.write_text(
        json.dumps(effective_snapshot(report_bytes, accepted), indent=2) + "\n",
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
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
