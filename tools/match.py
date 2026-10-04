#!/usr/bin/env python3
"""Compare reconstructed functions at original LEMBALL.EXE addresses."""

import argparse
import sys
from dataclasses import replace

from reccmp.tools.asmcmp import print_match_oneline, print_match_verbose

from build import run_build
from lib import load_engine
from lib.effective import additional_effective_matches


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "addrs",
        nargs="+",
        type=lambda value: int(value, 16),
        help="Hex addresses (e.g. 0x0045ca30)",
    )
    parser.add_argument(
        "--no-build", action="store_true", help="Skip incremental build"
    )
    parser.add_argument(
        "--summary",
        action="store_true",
        help="One row per address; omit assembly diffs",
    )
    args = parser.parse_args()
    if not args.no_build:
        code = run_build()
        if code:
            print(f"BUILD_FAILED exit={code} (see build-msvc400/last_build.log)")
            return code

    _, engine = load_engine()
    comparisons = [engine.compare_address(address) for address in args.addrs]
    additional = additional_effective_matches(
        engine,
        {
            comparison.orig_addr: comparison
            for comparison in comparisons
            if comparison is not None
        },
    )
    missing = False
    for address, comparison in zip(args.addrs, comparisons, strict=True):
        if comparison is None:
            print(f"0x{address:08x}: NOT_FOUND")
            missing = True
        elif comparison.is_stub:
            if args.summary:
                print(
                    f"0x{address:08x} Raw: 0.00%  Effective: 0.00% STUB {comparison.name}"
                )
            else:
                print_match_oneline(comparison)
        else:
            effective = replace(
                comparison,
                is_effective_match=comparison.is_effective_match
                or address in additional,
            )
            scores = f"Raw: {comparison.accuracy * 100:.2f}%  Effective: {effective.effective_accuracy * 100:.2f}%"
            if args.summary:
                print(f"0x{address:08x} {scores} {comparison.name}")
            else:
                print(scores)
                print_match_verbose(effective)
    return int(missing)


if __name__ == "__main__":
    sys.stdout.reconfigure(errors="backslashreplace")
    sys.stderr.reconfigure(errors="backslashreplace")
    raise SystemExit(main())
