#!/usr/bin/env python3
"""Compare reconstructed functions at original LEMBALL.EXE addresses."""

import argparse
import sys

from reccmp.compare import Compare
from reccmp.project.detect import RecCmpProject
from reccmp.tools.asmcmp import print_match_oneline, print_match_verbose

from lib.project import BUILD, TARGET_ID
from lib.comparison.matches import additional_effective_matches


def display_comparison(address, comparison, summary, additional=False):
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
    effective_accuracy = 1 if additional else comparison.effective_accuracy
    scores = (
        f"Raw: {comparison.accuracy * 100:.2f}%  "
        f"Effective: {effective_accuracy * 100:.2f}%"
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

    target = RecCmpProject.from_directory(BUILD).get(TARGET_ID)
    engine = Compare.from_target(target)
    comparisons = [engine.compare_address(address) for address in args.addrs]
    additional = additional_effective_matches(
        engine, {c.orig_addr: c for c in comparisons if c is not None}
    )
    for address, comparison in zip(args.addrs, comparisons, strict=True):
        display_comparison(address, comparison, args.summary, address in additional)
    return int(any(comparison is None for comparison in comparisons))


if __name__ == "__main__":
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(errors="backslashreplace")
    if hasattr(sys.stderr, "reconfigure"):
        sys.stderr.reconfigure(errors="backslashreplace")
    raise SystemExit(main())
