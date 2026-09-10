#!/usr/bin/env python3
"""Produce an objdiff report v2 from reconstructed LEMBALL binary."""

import argparse
import csv
import json
import struct
from collections import defaultdict
from pathlib import Path

from lib.compare import compute_ratio, load_matches
from lib.paths import REPORT_JSON, ROADMAP_CSV
from lib.reccmp import run_reccmp


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


def make_report(output_path: Path = REPORT_JSON) -> dict:
    reccmp_path = run_reccmp(detect=True, roadmap=True)
    report = build_report(ROADMAP_CSV, reccmp_path)
    output_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    values = report["measures"]
    print(f"{values['matched_functions']}/{values['total_functions']} functions matched")
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
