#!/usr/bin/env python3
"""Compare reconstructed functions to LEMBALL.EXE at original addresses.

  python tools/match.py 0xADDRESS
  python tools/match.py 0xADDRESS --no-build
"""

from __future__ import annotations

import argparse
from pathlib import Path

from build import run_build
from lib.compare import compute_ratio, format_diff_text, load_matches, norm_addr
from lib.paths import RECCMP_JSON
from lib.reccmp import run_reccmp


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("addrs", nargs="+", help="Addresses (e.g. 0x0045ca30)")
    parser.add_argument("--no-diff", action="store_true", help="Hide instruction diff")
    parser.add_argument("--json", type=Path, default=RECCMP_JSON)
    parser.add_argument("--no-build", action="store_true", help="Skip incremental build")
    parser.add_argument("--clean-first", action="store_true")
    args = parser.parse_args()

    if not args.no_build:
        exit_code = run_build(clean_first=args.clean_first)
        if exit_code != 0:
            print(f"BUILD_FAILED exit={exit_code} (see build-msvc400/last_build.log)")
            return exit_code

    json_path = run_reccmp(args.json)
    matches = load_matches(json_path)

    for raw in args.addrs:
        addr = norm_addr(raw)
        m = matches.get(addr)
        if not m:
            print(f"0x{addr:08x}: NOT_FOUND")
            continue

        name = m.get("name") or "?"
        ratio, tag = compute_ratio(m)
        suffix = f" {tag}" if tag else ""
        print(f"0x{addr:08x} {name}: {ratio:.2f}%{suffix}")
        if not args.no_diff:
            diff_text = format_diff_text(m.get("diff"))
            if diff_text:
                print("--- diff ---")
                print(diff_text)
                print("------------")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
