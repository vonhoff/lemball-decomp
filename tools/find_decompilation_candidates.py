#!/usr/bin/env python3
"""Find repeated original-code shapes and cheap unmatched functions."""

import argparse
import csv
import json
import re
from collections import defaultdict
from pathlib import Path

from reccmp.compare.asm.const import JUMP_MNEMONICS
from reccmp.compare.asm.parse import ParseAsm
from reccmp.formats import detect_image


ROOT = Path(__file__).resolve().parents[1]
BINARY = ROOT / "data" / "LEMBALL.EXE"
INVENTORY = ROOT / "data" / "objdiff-functions.csv"
RECCMP = ROOT / "build-msvc420" / "reccmp.json"
BASELINE = ROOT / "build-msvc420" / "exact-baseline.json"
LIMIT = 20
NUMBER = re.compile(r"-?(?:0x[0-9a-f]+|\d+)")


def load_inventory():
    with INVENTORY.open(newline="", encoding="utf-8-sig") as stream:
        return [
            {
                "address": int(row["address"], 16),
                "size": int(row["size"]),
                "name": row["name"],
                "unit": row["unit"],
            }
            for row in csv.DictReader(stream)
        ]


def load_matches():
    rows = json.loads(RECCMP.read_text(encoding="utf-8"))["data"]
    return {
        int(row["address"], 16): row
        for row in rows
        if row.get("type") in (None, 1)
    }


def code_shape(parser, blob, address):
    assembly = parser.parse_asm(blob, address)
    shape = [f"size:{len(blob)}"]
    for index, (instruction_address, text) in enumerate(assembly):
        if instruction_address is None:
            shape.append(text)
            continue
        mnemonic, _, operand_text = text.partition(" ")
        operands = operand_text.split(", ") if operand_text else []
        preserve = mnemonic.startswith("ret") or mnemonic == "enter"
        preserve |= mnemonic in {"add", "sub"} and operands[:1] == ["esp"]
        if mnemonic in JUMP_MNEMONICS and len(operands) == 1 and NUMBER.fullmatch(operands[0]):
            next_address = next(
                (item[0] for item in assembly[index + 1 :] if item[0] is not None),
                address + len(blob),
            )
            target = next_address + int(operands[0], 0)
            operands[0] = (
                f"local+0x{target - address:X}"
                if address <= target < address + len(blob)
                else "target"
            )
        elif not preserve:
            operands = [
                "constant" if "[" not in operand and NUMBER.fullmatch(operand) else operand
                for operand in operands
            ]
        shape.append(f"{mnemonic} {', '.join(operands)}".rstrip())
    return tuple(shape)


def decorate(functions, matches, image, parser):
    decorated = []
    for function in functions:
        row = matches.get(function["address"], {})
        stub = row.get("stub", False)
        ratio = 0.0 if stub else float(row.get("matching") or 0.0)
        decorated.append(
            function
            | {
                "exact": ratio == 1.0,
                "ratio": ratio,
                "stub": stub,
                "shape": code_shape(
                    parser,
                    image.read(function["address"], function["size"]),
                    function["address"],
                ),
            }
        )
    return decorated


def cluster_key(cluster):
    targets = [function for function in cluster if not function["exact"]]
    exemplars = len(cluster) - len(targets)
    five_byte_only = all(function["size"] <= 5 for function in targets)
    return (not exemplars, five_byte_only, -len(targets), min(function["size"] for function in targets))


def target_key(function):
    return (
        function["size"] <= 5,
        function["ratio"] == 0,
        function["size"] * (1 - function["ratio"]),
        function["size"],
        function["address"],
    )


def print_function(function, label):
    state = "STUB" if function["stub"] else f"{function['ratio']:.2%}"
    print(
        f"  {label:8} 0x{function['address']:08X} {function['size']:4}B "
        f"{state:>7} {function['unit']} {function['name']}"
    )


def print_candidates(functions):
    groups = defaultdict(list)
    for function in functions:
        groups[function["shape"]].append(function)
    clusters = [
        cluster
        for cluster in groups.values()
        if len(cluster) >= 2 and any(not function["exact"] for function in cluster)
    ]
    clusters.sort(key=cluster_key)

    unmatched = [function for function in functions if not function["exact"]]
    print(f"inventory: {len(functions)} source-owned functions")
    print(f"exact inventory: {len(functions) - len(unmatched)} source-owned functions")
    print(f"unmatched: {len(unmatched)}")
    print("\nshape clusters")
    for number, cluster in enumerate(clusters[:LIMIT], 1):
        targets = sorted(
            (function for function in cluster if not function["exact"]),
            key=lambda function: function["address"],
        )
        exemplars = sorted(
            (function for function in cluster if function["exact"]),
            key=lambda function: function["address"],
        )
        mnemonics = " ".join(part.split(" ", 1)[0] for part in cluster[0]["shape"][1:])
        print(
            f"\n{number}. {len(targets)} targets, {len(exemplars)} exemplars, "
            f"{cluster[0]['size']}B: {mnemonics}"
        )
        for function in exemplars[:3]:
            print_function(function, "exact")
        for function in targets:
            print_function(function, "target")

    print("\nsingleton fallback")
    clustered = {
        function["address"]
        for cluster in clusters
        for function in cluster
        if not function["exact"]
    }
    singletons = sorted(
        (function for function in unmatched if function["address"] not in clustered),
        key=target_key,
    )
    for function in singletons[:LIMIT]:
        print_function(function, "target")


def save_baseline(addresses):
    BASELINE.write_text(
        json.dumps([f"0x{address:08X}" for address in sorted(addresses)], indent=2) + "\n",
        encoding="utf-8",
    )
    print(f"saved {len(addresses)} exact addresses to {BASELINE}")


def check_baseline(addresses):
    baseline = {int(value, 16) for value in json.loads(BASELINE.read_text(encoding="utf-8"))}
    lost = sorted(baseline - addresses)
    added = sorted(addresses - baseline)
    print(f"exact-set gate: {len(added)} added, {len(lost)} lost")
    for address in lost:
        print(f"  lost 0x{address:08X}")
    for address in added:
        print(f"  added 0x{address:08X}")
    return bool(lost)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    mode = parser.add_mutually_exclusive_group()
    mode.add_argument("--save-baseline", action="store_true")
    mode.add_argument("--check-baseline", action="store_true")
    args = parser.parse_args()

    matches = load_matches()
    addresses = {
        address
        for address, row in matches.items()
        if row.get("matching") == 1 and not row.get("stub", False)
    }
    if args.check_baseline:
        raise SystemExit(check_baseline(addresses))
    if args.save_baseline:
        save_baseline(addresses)
    image = detect_image(BINARY)
    assembly_parser = ParseAsm(addr_test=image.is_valid_vaddr)
    assembly_parser.number_placeholders = False
    print_candidates(decorate(load_inventory(), matches, image, assembly_parser))


if __name__ == "__main__":
    main()
