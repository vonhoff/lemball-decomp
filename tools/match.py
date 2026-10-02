#!/usr/bin/env python3
"""Compare reconstructed functions at original LEMBALL.EXE addresses."""

import argparse
import sys

from reccmp.tools.asmcmp import print_match_verbose

from build import run_build
from lib import load_engine


def main() -> int:
    sys.stdout.reconfigure(errors="backslashreplace")
    sys.stderr.reconfigure(errors="backslashreplace")
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("addrs", nargs="+", type=lambda value: int(value, 16), help="Hex addresses (e.g. 0x0045ca30)")
    parser.add_argument("--no-diff", action="store_true", help="Hide instruction diff")
    parser.add_argument("--no-build", action="store_true", help="Skip incremental build")
    parser.add_argument("--clean-first", action="store_true")
    args = parser.parse_args()

    if not args.no_build:
        exit_code = run_build(clean_first=args.clean_first)
        if exit_code != 0:
            print(f"BUILD_FAILED exit={exit_code} (see build-msvc400/last_build.log)")
            return exit_code

    _, engine = load_engine()

    for addr in args.addrs:
        match = engine.compare_address(addr)
        if match is None:
            print(f"0x{addr:08x}: NOT_FOUND")
            continue

        pct = 0.0 if match.is_stub else match.accuracy * 100.0
        if match.is_stub:
            status = "STUB"
        elif match.accuracy == 1:
            status = "ASM_EXACT"
        elif match.is_effective_match:
            status = "EFFECTIVE"
        else:
            status = "PARTIAL"
        print(f"0x{addr:08x} {match.name}: {pct:.2f}% {status}")
        if not args.no_diff:
            print_match_verbose(match)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
