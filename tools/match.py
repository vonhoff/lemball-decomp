#!/usr/bin/env python3
"""Compare reconstructed functions at original LEMBALL.EXE addresses."""

import argparse
import sys

from reccmp.tools.asmcmp import print_match_verbose

from build import run_build
from lib import load_engine


def parse_args():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "addrs",
        nargs="+",
        type=lambda value: int(value, 16),
        help="Hex addresses (e.g. 0x0045ca30)",
    )
    parser.add_argument("--no-diff", action="store_true", help="Hide instruction diff")
    parser.add_argument("--no-build", action="store_true", help="Skip incremental build")
    return parser.parse_args()


def print_comparison(address, comparison, show_diff=True):
    """Print raw accuracy and status; delegate instruction diffs to reccmp."""
    if comparison is None:
        print(f"0x{address:08x}: NOT_FOUND")
        return
    percent = 0.0 if comparison.is_stub else comparison.accuracy * 100.0
    if comparison.is_stub:
        status = "STUB"
    elif comparison.accuracy == 1:
        status = "ASM_EXACT"
    elif comparison.is_effective_match:
        status = "EFFECTIVE"
    else:
        status = "PARTIAL"
    print(f"0x{address:08x} {comparison.name}: {percent:.2f}% {status}")
    if show_diff:
        print_match_verbose(comparison)


def main() -> int:
    args = parse_args()
    if not args.no_build:
        code = run_build()
        if code:
            print(f"BUILD_FAILED exit={code} (see build-msvc400/last_build.log)")
            return code

    _, engine = load_engine()
    for address in args.addrs:
        print_comparison(address, engine.compare_address(address), show_diff=not args.no_diff)
    return 0


if __name__ == "__main__":
    sys.stdout.reconfigure(errors="backslashreplace")
    sys.stderr.reconfigure(errors="backslashreplace")
    raise SystemExit(main())
