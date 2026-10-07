#!/usr/bin/env python3
"""Compare reconstructed functions at original LEMBALL.EXE addresses."""

import argparse
import sys

from reccmp.tools.asmcmp import print_match_oneline, print_match_verbose

from lib.comparison.engine import load_engine


def display_comparison(address, comparison, summary):
    """Show raw and Effective scores without changing the raw result."""
    if comparison is None:
        print(f"0x{address:08x}: NOT_FOUND")
        return
    if comparison.is_stub:
        if summary:
            print(
                f"0x{address:08x} Raw: 0.00%  Effective: 0.00% STUB {comparison.name}"
            )
        else:
            print_match_oneline(comparison)
        return
    scores = (
        f"Raw: {comparison.accuracy * 100:.2f}%  "
        f"Effective: {comparison.effective_accuracy * 100:.2f}%"
    )
    if summary:
        print(f"0x{address:08x} {scores} {comparison.name}")
    else:
        print(scores)
        print_match_verbose(comparison)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "addrs",
        nargs="+",
        type=lambda value: int(value, 16),
        help="Hex addresses (e.g. 0x0045ca30)",
    )
    parser.add_argument(
        "--summary",
        action="store_true",
        help="One row per address; omit assembly diffs",
    )
    args = parser.parse_args()

    _, engine = load_engine()
    comparisons = [engine.compare_address(address) for address in args.addrs]
    for address, comparison in zip(args.addrs, comparisons, strict=True):
        display_comparison(address, comparison, args.summary)
    return int(any(comparison is None for comparison in comparisons))


if __name__ == "__main__":
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(errors="backslashreplace")
    if hasattr(sys.stderr, "reconfigure"):
        sys.stderr.reconfigure(errors="backslashreplace")
    raise SystemExit(main())
