#!/usr/bin/env python3
"""Produce an objdiff-compatible report from the reconstructed binary.

Runs reccmp against the built LEMBALL, then converts the result to the
objdiff report v2 format. Invoke from the repository root after building:

    .decomp-venv\\Scripts\\python.exe tools\\generate_objdiff_report.py
"""

import argparse
import csv
import json
import shutil
import struct
import subprocess
from collections import defaultdict
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build-msvc400"
MODULE_PREFIX = "CMakeFiles/LEMBALL.dir/src/"
MODULE_SUFFIX = ".obj"


def f32(value):
    return struct.unpack("<f", struct.pack("<f", value))[0]


def unit_name(module):
    if module.startswith("CMakeFiles/LEMBALL.dir/src/"):
        module = module[len("CMakeFiles/LEMBALL.dir/src/") :]
    elif module.startswith("CMakeFiles/LEMBALL.dir/src_old/"):
        module = module[len("CMakeFiles/LEMBALL.dir/src_old/") :]
    elif module.startswith(MODULE_PREFIX):
        module = module[len(MODULE_PREFIX) :]
    if module.endswith(MODULE_SUFFIX):
        module = module[: -len(MODULE_SUFFIX)]
    return module or "Compiler-generated"


def measures(functions, total_units=1):
    total_code = sum(function["size"] for function in functions)
    matched = [function for function in functions if function["ratio"] == 100.0]
    matched_code = sum(function["size"] for function in matched)
    fuzzy = sum(function["ratio"] * function["size"] for function in functions)
    result = {"total_units": total_units}
    if total_code:
        result.update(
            total_code=str(total_code),
            matched_code=str(matched_code),
            fuzzy_match_percent=f32(fuzzy / total_code),
            matched_code_percent=f32(matched_code / total_code * 100),
        )
    if functions:
        result.update(
            total_functions=len(functions),
            matched_functions=len(matched),
            matched_functions_percent=f32(len(matched) / len(functions) * 100),
        )
    return result


def tool(name):
    return shutil.which(name) or str(ROOT / ".decomp-venv" / "Scripts" / f"{name}.exe")


def run_reccmp():
    subprocess.run(
        [tool("reccmp-project"), "detect", "--search-path", str(ROOT / "data")],
        cwd=ROOT,
        check=True,
    )
    subprocess.run(
        [tool("reccmp-reccmp"), "--target", "LEMBALL", "--json", "reccmp.json", "--json-diet", "--silent"],
        cwd=BUILD,
        check=True,
    )
    subprocess.run(
        [tool("reccmp-roadmap"), "--target", "LEMBALL", "--csv", "roadmap.csv"],
        cwd=BUILD,
        check=True,
    )


def load_matches(path):
    with path.open(encoding="utf-8") as stream:
        data = json.load(stream)["data"]
    return {
        int(row["address"], 16): row
        for row in data
        if row.get("type") in (None, 1)
    }


def load_inventory(path):
    with path.open(newline="", encoding="utf-8-sig") as stream:
        for row in csv.DictReader(stream):
            if row["row_type"] != "fun" or not row["orig_addr"]:
                continue
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
        if match is None or match.get("stub"):
            ratio = 0.0
        elif match.get("effective"):
            ratio = 100.0
        else:
            ratio = float(match["matching"]) * 100
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
        functions.sort(key=lambda function: function["address"])
        units.append(
            {
                "name": name,
                "measures": measures(functions),
                "sections": [],
                "functions": [
                    {
                        "name": function["name"],
                        "size": str(function["size"]),
                        "metadata": {"virtual_address": str(function["address"])},
                        "fuzzy_match_percent": f32(function["ratio"]),
                    }
                    for function in functions
                ],
                "metadata": {"module_name": name},
            }
        )

    functions = [function for group in groups.values() for function in group]
    return {
        "measures": measures(functions, len(units)),
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
