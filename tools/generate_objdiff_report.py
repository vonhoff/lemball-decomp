#!/usr/bin/env python3
"""Convert a reccmp JSON report to objdiff report v2."""

import argparse
import csv
import json
import re
import struct
from collections import defaultdict
from pathlib import Path


FUNCTION_NAME = re.compile(r"[A-Za-z_~][A-Za-z0-9_~]*(?:::[A-Za-z_~][A-Za-z0-9_~]*)?")
VAGUE_NAME = re.compile(r"(?:^|_)(?:fun|unknown|method|reserved[0-9a-f]*)(?:_|$)")
MAC_CLASS_PREFIX = re.compile(r"__(\d+)")


def mac_module(code_file):
    name = re.sub(r"^CODE_[0-9]+_", "", code_file).removesuffix(".bin")
    return {
        "Visos (Generic)": "Visos/Generic",
        "Visos (Mac Specific)": "Visos/Windows",
        "views_2d": "views/2d",
    }.get(name, name)


def mac_class(mangled):
    match = MAC_CLASS_PREFIX.search(mangled)
    if match is None:
        return ""
    start = match.end()
    length = int(match.group(1))
    name = mangled[start : start + length]
    if len(name) == length and mangled[start + length : start + length + 1] == "F":
        return name
    return ""


def mac_function(mangled, class_name):
    operators = {"__apl__": "operator+=", "__as__": "operator="}
    for prefix, name in operators.items():
        if mangled.startswith(prefix):
            return name
    if mangled.startswith("__ct__"):
        return class_name
    if mangled.startswith("__dt__"):
        return f"~{class_name}"
    if mangled.startswith("__nw__"):
        return "operator new"
    return mangled.split("__", 1)[0]


def load_owners(path):
    owners = {}
    with path.open(newline="", encoding="utf-8-sig") as stream:
        for row in csv.DictReader(stream):
            address = int(row["x86_address"], 16)
            class_name = mac_class(row["mac_mangled_name"])
            owner = (
                mac_module(row["mac_code_file"]),
                class_name,
                mac_function(row["mac_mangled_name"], class_name),
                row["mac_mangled_name"],
            )
            if address in owners and owners[address] != owner:
                raise SystemExit(f"conflicting owner for 0x{address:08X}")
            owners[address] = owner
    return owners


def load_addresses(path):
    if path is None:
        return set()
    with path.open(newline="", encoding="utf-8-sig") as stream:
        rows = csv.DictReader(line for line in stream if not line.startswith("#"))
        return {int(row["address"], 16) for row in rows}


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


def build_report(inventory_path, reccmp_path, correlations_path, compiler_generated_path=None):
    native = json.loads(reccmp_path.read_text(encoding="utf-8"))
    owners = load_owners(correlations_path)
    compiler_generated = load_addresses(compiler_generated_path)
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
            owner = owners.get(address)
            if owner:
                function["mac_name"] = owner[2]
                group = owner[:2]
            elif address in compiler_generated:
                group = ("Windows", "Compiler-generated")
            else:
                group = ("Windows", "Uncorrelated")
            groups[group].append(function)

    units = []
    for (module, class_name), functions in sorted(groups.items()):
        items = []
        for function in functions:
            item = {
                "name": function.get("mac_name", function["name"]),
                "size": str(function["size"]),
                "metadata": {
                    "virtual_address": str(function["address"]),
                },
            }
            if function["ratio"]:
                item["fuzzy_match_percent"] = f32(function["ratio"])
            items.append(item)
        units.append(
            {
                "name": f"{module}/{class_name}" if class_name else module,
                "measures": measures(functions),
                "sections": [],
                "functions": items,
                "metadata": {"module_name": module},
            }
        )
    functions = [function for group in groups.values() for function in group]
    return {"measures": measures(functions, len(units)), "units": units, "version": 2}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--inventory", type=Path, default=Path("data/objdiff-functions.csv"))
    parser.add_argument("--reccmp", type=Path, default=Path("build-msvc400/reccmp.json"))
    parser.add_argument(
        "--correlations", type=Path, default=Path("data/macintosh-x86-correlations.csv")
    )
    parser.add_argument(
        "--compiler-generated",
        type=Path,
        default=Path("data/reccmp-compiler-generated.csv"),
    )
    parser.add_argument("--output", type=Path, default=Path("build-msvc400/report.json"))
    args = parser.parse_args()
    report = build_report(
        args.inventory, args.reccmp, args.correlations, args.compiler_generated
    )
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    values = report["measures"]
    print(f"{values['matched_functions']}/{values['total_functions']} functions matched")


if __name__ == "__main__":
    main()
