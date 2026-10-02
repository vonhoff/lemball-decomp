#!/usr/bin/env python3
"""Rank unfinished functions from the canonical report."""

import argparse
import json

from lib import REPORT_JSON


def rank_functions(report):
    """Rank unfinished functions without changing the canonical report."""
    functions = (
        {**function, "unit": unit["name"]}
        for unit in report["units"]
        for function in unit["functions"]
        if function["fuzzy_match_percent"] < 100
    )
    return sorted(functions, key=lambda function: (
        -function["fuzzy_match_percent"],
        int(function["size"]),
        int(function["metadata"]["virtual_address"]),
    ))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--limit", type=int, default=40, help="rows; 0 = unlimited")
    args = parser.parse_args()
    try:
        report = json.loads(REPORT_JSON.read_text(encoding="utf-8"))
    except (OSError, ValueError) as error:
        raise SystemExit(f"cannot read report: {error}") from error
    functions = rank_functions(report)
    if args.limit > 0:
        functions = functions[: args.limit]
    for function in functions:
        address = int(function["metadata"]["virtual_address"])
        print(
            f"0x{address:08x} {function['fuzzy_match_percent']:6.2f}% "
            f"size={int(function['size']):4d} {function['unit']} "
            f"{function['metadata']['demangled_name']}"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
