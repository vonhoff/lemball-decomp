#!/usr/bin/env python3
"""Produce an objdiff report v2 from reconstructed LEMBALL binary."""

import argparse
import csv
import json
import re
import shutil
import struct
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

# Add tools dir to path for imports
TOOLS_DIR = Path(__file__).resolve().parent
if str(TOOLS_DIR) not in sys.path:
    sys.path.insert(0, str(TOOLS_DIR))

from check import compute_ratio, is_thunk_only_diff, load_matches, normalize_asm

ROOT = TOOLS_DIR.parent
BUILD = ROOT / "build-msvc400"


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


def tool(name):
    return shutil.which(name) or str(ROOT / ".decomp-venv" / "Scripts" / f"{name}.exe")


def run_reccmp():
    subprocess.run(
        [tool("reccmp-project"), "detect", "--search-path", str(ROOT / "data")],
        cwd=ROOT,
        check=True,
    )
    subprocess.run(
        [tool("reccmp-reccmp"), "--target", "LEMBALL", "--json", "reccmp.json", "--silent"],
        cwd=BUILD,
        check=True,
    )
    subprocess.run(
        [tool("reccmp-roadmap"), "--target", "LEMBALL", "--csv", "roadmap.csv"],
        cwd=BUILD,
        check=True,
    )


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
        ratio, _ = compute_ratio(match, item["name"])
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


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=BUILD / "report.json")
    args = parser.parse_args()
    run_reccmp()
    report = build_report(BUILD / "roadmap.csv", BUILD / "reccmp.json")
    args.output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    values = report["measures"]
    print(f"{values['matched_functions']}/{values['total_functions']} functions matched")
    print(f"wrote {args.output}")


if __name__ == "__main__":
    main()
