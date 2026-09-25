#!/usr/bin/env python3
"""Produce an objdiff report v2 from reconstructed LEMBALL binary."""

import argparse
import csv
import json
import struct
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

from lib.compare import compute_ratio, load_matches
from lib.paths import BUILD, RECCMP_JSON, REPORT_JSON, ROADMAP_CSV, ROOT
from lib.reccmp_compat import load_engine
from reccmp.compare.report import ReccmpStatusReport, serialize_reccmp_report
from reccmp.project.detect import DetectWhat, detect_project
from reccmp.types import EntityType


def f32(value):
    return struct.unpack("<f", struct.pack("<f", value))[0]


def unit_name(module):
    name = module.removeprefix("CMakeFiles/LEMBALL.dir/src/").removesuffix(".obj")
    return name or "Compiler-generated"


def measures(functions, total_units=1):
    total_code = sum(f["size"] for f in functions)
    matched = [f for f in functions if f["ratio"] == 100.0]
    matched_code = sum(f["size"] for f in matched)
    fuzzy = sum(f["ratio"] * f["size"] for f in functions)
    res = {"total_units": total_units}
    if total_code:
        res.update(
            total_code=str(total_code),
            matched_code=str(matched_code),
            fuzzy_match_percent=f32(fuzzy / total_code),
            matched_code_percent=f32(matched_code / total_code * 100),
        )
    if functions:
        res.update(
            total_functions=len(functions),
            matched_functions=len(matched),
            matched_functions_percent=f32(len(matched) / len(functions) * 100),
        )
    return res


def load_inventory(path):
    with path.open(newline="", encoding="utf-8-sig") as stream:
        for row in csv.DictReader(stream):
            if row["row_type"] == "fun" and row["orig_addr"]:
                yield {
                    "address": int(row["orig_addr"], 16),
                    "size": int(row["size"], 16),
                    "name": row["name"],
                    "module": row["module"],
                }


def build_report(roadmap_path, reccmp_path):
    matches = load_matches(reccmp_path)
    groups = defaultdict(list)
    for item in load_inventory(roadmap_path):
        match = matches.get(item["address"])
        ratio, _ = compute_ratio(match)
        groups[unit_name(item["module"])].append(
            {
                "name": item["name"],
                "size": item["size"],
                "address": item["address"],
                "ratio": ratio,
            }
        )

    units = []
    for name, functions in sorted(groups.items()):
        functions.sort(key=lambda f: f["address"])
        units.append(
            {
                "name": name,
                "measures": measures(functions),
                "sections": [],
                "functions": [
                    {
                        "name": f["name"],
                        "size": str(f["size"]),
                        "metadata": {"virtual_address": str(f["address"])},
                        "fuzzy_match_percent": f32(f["ratio"]),
                    }
                    for f in functions
                ],
                "metadata": {"module_name": name},
            }
        )

    all_functions = [f for group in groups.values() for f in group]
    return {
        "measures": measures(all_functions, len(units)),
        "units": units,
        "version": 2,
    }


def run_reccmp(json_path: Path = RECCMP_JSON, *, detect: bool = False, roadmap: bool = False) -> Path:
    out = json_path.resolve()
    BUILD.mkdir(parents=True, exist_ok=True)

    if detect:
        detect_project(
            project_directory=ROOT,
            search_path=[ROOT / "data"],
            detect_what=DetectWhat.ORIGINAL,
            build_directory=BUILD,
        )

    target, engine = load_engine()
    report = ReccmpStatusReport(filename=target.original_path.name)
    for match in engine.compare_all():
        match_type = getattr(match, "type", None)
        if (
            match_type == EntityType.FUNCTION
            and match.name in target.report_config.ignore_functions
        ):
            continue
        report.add_match(match)
    out.write_text(serialize_reccmp_report(report, diff_included=True), encoding="utf-8")

    if roadmap:
        subprocess.run(
            [sys.executable, "-m", "reccmp.tools.roadmap", "--target", "LEMBALL",
             "--csv", str(ROADMAP_CSV)],
            cwd=ROOT,
            check=True,
        )

    return out


def make_report(output_path: Path = REPORT_JSON) -> dict:
    reccmp_path = run_reccmp(detect=True, roadmap=True)
    report = build_report(ROADMAP_CSV, reccmp_path)
    output_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    values = report["measures"]
    matches = load_matches(reccmp_path)
    exact = [item for item in load_inventory(ROADMAP_CSV)
             if (match := matches.get(item["address"]))
             and not match.get("stub") and match.get("matching") == 1.0]
    print(f"assembly-exact (normalized): {len(exact)}/{values['total_functions']} functions, "
          f"{sum(item['size'] for item in exact)}/{values['total_code']} code bytes")
    print(f"effective: {values['matched_functions']}/{values['total_functions']} functions, "
          f"{values['matched_code']}/{values['total_code']} code bytes")
    print(f"wrote {output_path}")
    return report


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=REPORT_JSON)
    args = parser.parse_args()
    make_report(args.output)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
