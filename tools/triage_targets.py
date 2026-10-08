#!/usr/bin/env python3
"""Rank unfinished functions from the canonical report."""

import argparse
from lib.progress import load_progress


def rank_functions(report, effective=None, min_size=0, sort="score"):
    """Rank unfinished functions without changing the canonical report."""
    effective = effective or set()
    functions = (
        {**function, "unit": unit["name"]}
        for unit in report["units"]
        for function in unit["functions"]
        if function["fuzzy_match_percent"] < 100
        and int(function["metadata"]["virtual_address"]) not in effective
        and int(function["size"]) >= min_size
    )
    return sorted(
        functions,
        key=lambda function: (
            -int(function["size"]) if sort == "size" else 0,
            -function["fuzzy_match_percent"],
            int(function["size"]),
            int(function["metadata"]["virtual_address"]),
        ),
    )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--limit", type=int, default=40, help="rows; 0 = unlimited")
    parser.add_argument(
        "--min-size", type=int, default=0, help="Minimum original bytes"
    )
    parser.add_argument("--sort", choices=("score", "size"), default="score")
    parser.add_argument(
        "--exact", action="store_true", help="Rank by raw comparison scores"
    )
    args = parser.parse_args()
    if args.min_size < 0 or args.limit < 0:
        parser.error("--min-size and --limit must not be negative")
    try:
        report, accepted = load_progress(exact=args.exact)
    except ValueError as exc:
        parser.error(str(exc))
    functions = rank_functions(
        report, None if args.exact else accepted, args.min_size, args.sort
    )
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
