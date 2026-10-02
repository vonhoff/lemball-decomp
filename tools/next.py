#!/usr/bin/env python3
"""Rank unfinished functions from the canonical report."""

import argparse
import json

from reccmp.compare.report import deserialize_reccmp_report

from lib import RECCMP_JSON, REPORT_JSON
from lib.effective import EFFECTIVE_JSON, effective_addresses


def rank_functions(report, effective=()):
    """Rank unfinished functions without changing the canonical report."""
    functions = (
        {**function, "unit": unit["name"]}
        for unit in report["units"]
        for function in unit["functions"]
        if function["fuzzy_match_percent"] < 100
        and int(function["metadata"]["virtual_address"]) not in effective
    )
    return sorted(functions, key=lambda function: (
        -function["fuzzy_match_percent"],
        int(function["size"]),
        int(function["metadata"]["virtual_address"]),
    ))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--limit", type=int, default=40, help="rows; 0 = unlimited")
    parser.add_argument("--exact", action="store_true", help="Rank by raw comparison scores")
    args = parser.parse_args()
    try:
        report = json.loads(REPORT_JSON.read_text(encoding="utf-8"))
        accepted = set()
        if not args.exact:
            comparisons = deserialize_reccmp_report(RECCMP_JSON.read_text(encoding="utf-8"))
            additional = {int(address) for address in json.loads(EFFECTIVE_JSON.read_text(encoding="utf-8"))}
            accepted = effective_addresses(comparisons.entities, additional)
    except (OSError, ValueError) as error:
        raise SystemExit(f"cannot read report: {error}") from error
    functions = rank_functions(report, accepted)
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
