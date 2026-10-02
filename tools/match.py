#!/usr/bin/env python3
"""Compare reconstructed functions at original LEMBALL.EXE addresses."""

import argparse
import sys

from reccmp.tools.asmcmp import print_match_oneline, print_match_verbose

from build import run_build
from lib import load_engine


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("addrs", nargs="+", type=lambda value: int(value, 16),
                        help="Hex addresses (e.g. 0x0045ca30)")
    parser.add_argument("--no-build", action="store_true", help="Skip incremental build")
    args = parser.parse_args()
    if not args.no_build:
        code = run_build()
        if code:
            print(f"BUILD_FAILED exit={code} (see build-msvc400/last_build.log)")
            return code

    _, engine = load_engine()
    for address in args.addrs:
        comparison = engine.compare_address(address)
        if comparison is None:
            print(f"0x{address:08x}: NOT_FOUND")
        elif comparison.is_stub:
            print_match_oneline(comparison)
        else:
            print_match_verbose(comparison)
    return 0


if __name__ == "__main__":
    sys.stdout.reconfigure(errors="backslashreplace")
    sys.stderr.reconfigure(errors="backslashreplace")
    raise SystemExit(main())
