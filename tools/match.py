#!/usr/bin/env python3
"""Compare reconstructed functions to LEMBALL.EXE at original addresses.

  python tools/match.py 0xADDRESS
  python tools/match.py 0xADDRESS --no-build
"""

from __future__ import annotations

import argparse
from pathlib import Path

from build import run_build
from lib.compare import norm_addr
from lib.reccmp import _load_engine
from reccmp.tools.asmcmp import print_match_verbose


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("addrs", nargs="+", help="Addresses (e.g. 0x0045ca30)")
    parser.add_argument("--no-diff", action="store_true", help="Hide instruction diff")
    parser.add_argument("--no-build", action="store_true", help="Skip incremental build")
    parser.add_argument("--clean-first", action="store_true")
    args = parser.parse_args()

    if not args.no_build:
        exit_code = run_build(clean_first=args.clean_first)
        if exit_code != 0:
            print(f"BUILD_FAILED exit={exit_code} (see build-msvc400/last_build.log)")
            return exit_code

    target, engine = _load_engine()

    for raw in args.addrs:
        addr = norm_addr(raw)
        match = engine.compare_address(addr)
        if match is None:
            print(f"0x{addr:08x}: NOT_FOUND")
            continue

        pct = match.effective_accuracy * 100.0
        if args.no_diff:
            status = "MATCH" if match.is_matched or pct == 100.0 else ("STUB" if match.is_stub else "")
            suffix = f" {status}" if status else ""
            print(f"0x{addr:08x} {match.name}: {pct:.2f}%{suffix}")
        else:
            print_match_verbose(match)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
