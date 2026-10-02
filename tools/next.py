#!/usr/bin/env python3
"""Rank unfinished functions from the canonical report."""

import argparse
import json
import sys

from lib import REPORT_JSON


def parse_args():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--limit", type=int, default=40, help="rows; 0 = unlimited")
    return parser.parse_args()


def ranking_key(function):
    score = function["fuzzy_match_percent"]
    size = int(function["size"])
    address = int(function["metadata"]["virtual_address"])
    return -score, size, address


def rank_functions(report):
    """Rank unfinished functions without changing the canonical report."""
    functions = [
        {**function, "unit": unit["name"]}
        for unit in report["units"]
        for function in unit["functions"]
        if function["fuzzy_match_percent"] < 100
    ]
    return sorted(functions, key=ranking_key)


def print_functions(functions):
    for function in functions:
        address = int(function["metadata"]["virtual_address"])
        print(
            f"0x{address:08x} {function['fuzzy_match_percent']:6.2f}% "
            f"size={int(function['size']):4d} {function['unit']} "
            f"{function['metadata']['demangled_name']}"
        )


def main() -> int:
    args = parse_args()
    try:
        report = json.loads(REPORT_JSON.read_text(encoding="utf-8"))
    except (OSError, ValueError) as error:
        sys.exit(f"cannot read report: {error}")
    functions = rank_functions(report)
    if args.limit > 0:
        functions = functions[: args.limit]
    print_functions(functions)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
