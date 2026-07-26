#!/usr/bin/env python3
"""Convert a reccmp JSON report to objdiff report v2."""

import argparse
import csv
import json
import re
import struct
from collections import defaultdict
from pathlib import Path


FUNCTION_NAME = re.compile(r"[a-z][a-z0-9_]{0,63}")
VAGUE_NAME = re.compile(r"(?:^|_)(?:fun|unknown|method|reserved[0-9a-f]*)(?:_|$)")


def f32(value):
    return struct.unpack("<f", struct.pack("<f", value))[0]


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


def build_report(inventory_path, reccmp_path):
    native = json.loads(reccmp_path.read_text(encoding="utf-8"))
    matches = {
        int(row["address"], 16): row
        for row in native["data"]
        if row.get("type") in (None, 1)
    }
    groups = defaultdict(list)
    seen = set()
    with inventory_path.open(newline="", encoding="utf-8-sig") as stream:
        for row in csv.DictReader(stream):
            address = int(row["address"], 16)
            if address in seen:
                raise SystemExit(f"duplicate inventory address 0x{address:08X}")
            if not FUNCTION_NAME.fullmatch(row["name"]):
                raise SystemExit(f"invalid function name at 0x{address:08X}: {row['name']}")
            if VAGUE_NAME.search(row["name"]):
                raise SystemExit(f"vague function name at 0x{address:08X}: {row['name']}")
            seen.add(address)
            match = matches.get(address)
            ratio = 0.0 if match is None or match.get("stub") else float(match["matching"]) * 100
            function = {
                "name": row["name"],
                "size": int(row["size"]),
                "address": address,
                "ratio": ratio,
            }
            groups[row["unit"]].append(function)

    units = []
    for module, functions in sorted(groups.items()):
        items = []
        for function in functions:
            item = {
                "name": function["name"],
                "size": str(function["size"]),
                "metadata": {"virtual_address": str(function["address"])},
            }
            if function["ratio"]:
                item["fuzzy_match_percent"] = f32(function["ratio"])
            items.append(item)
        units.append(
            {
                "name": module,
                "measures": measures(functions),
                "sections": [],
                "functions": items,
                "metadata": {"source_path": f"src/{module}"},
            }
        )
    functions = [function for group in groups.values() for function in group]
    return {"measures": measures(functions, len(units)), "units": units, "version": 2}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--inventory", type=Path, default=Path("data/objdiff-functions.csv"))
    parser.add_argument("--reccmp", type=Path, default=Path("build-msvc420/reccmp.json"))
    parser.add_argument("--output", type=Path, default=Path("build-msvc420/report.json"))
    args = parser.parse_args()
    report = build_report(args.inventory, args.reccmp)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    values = report["measures"]
    print(f"{values['matched_functions']}/{values['total_functions']} functions matched")


if __name__ == "__main__":
    main()
