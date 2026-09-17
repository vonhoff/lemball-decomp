#!/usr/bin/env python3
"""Compare reconstructed functions to LEMBALL.EXE at original addresses.

  python tools/match.py 0xADDRESS
  python tools/match.py 0xADDRESS --no-build
"""

from __future__ import annotations

import argparse
import sys

from build import run_build
from lib.reccmp import load_engine
from reccmp.tools.asmcmp import print_match_verbose


def match_status(match) -> str:
    """A mapped address is not an exact match; stubs remain explicitly marked."""
    if match.is_stub:
        return "STUB"
    return "MATCH" if match.effective_accuracy == 1.0 else ""


def configure_output(stream) -> None:
    """Keep the selected encoding; escape characters it cannot represent."""
    if hasattr(stream, "reconfigure"):
        stream.reconfigure(errors="backslashreplace")


def hexadecimal(value: str) -> int:
    return int(value, 16)


def main() -> int:
    configure_output(sys.stdout)
    configure_output(sys.stderr)
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("addrs", nargs="+", type=hexadecimal, help="Hex addresses (e.g. 0x0045ca30)")
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

        pct = match.effective_accuracy * 100.0
        if args.no_diff:
            status = match_status(match)
            suffix = f" {status}" if status else ""
            print(f"0x{addr:08x} {match.name}: {pct:.2f}%{suffix}")
        else:
            print_match_verbose(match)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
