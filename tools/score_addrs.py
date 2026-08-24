#!/usr/bin/env python3
"""Score one or more annotated addresses from a diet reccmp JSON (one PDB parse).

Usage (from repo root, after a build):
  .decomp-venv\\Scripts\\reccmp-reccmp.exe --target LEMBALL --silent --json-diet --json build-msvc400/scores.json
  .decomp-venv\\Scripts\\python.exe tools/score_addrs.py 0x0045ca30 0x0045cab0
  .decomp-venv\\Scripts\\python.exe tools/score_addrs.py --json build-msvc400/scores.json 0x0045ca30
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_JSON = ROOT / "build-msvc400" / "scores.json"


def norm_addr(value: str) -> int:
    value = value.strip().lower()
    if value.startswith("0x"):
        return int(value, 16)
    return int(value, 16) if any(c in value for c in "abcdef") else int(value, 0)


def load_functions(path: Path) -> list[dict]:
    data = json.loads(path.read_text(encoding="utf-8"))
    if isinstance(data, dict):
        if isinstance(data.get("data"), list):
            return data["data"]
        if "functions" in data:
            return data["functions"]
        for key, val in data.items():
            if isinstance(val, dict) and "functions" in val:
                return val["functions"]
            if isinstance(val, list):
                return val
    if isinstance(data, list):
        return data
    raise SystemExit(f"unrecognized JSON shape in {path}")


def func_addr(fn: dict) -> int | None:
    for key in ("address", "orig_addr", "addr", "original_addr"):
        if key in fn and fn[key] is not None:
            val = fn[key]
            return int(val) if isinstance(val, int) else norm_addr(str(val))
    meta = fn.get("metadata") or {}
    for key in ("virtual_address", "orig_addr", "address"):
        if key in meta:
            val = meta[key]
            return int(val) if isinstance(val, int) else norm_addr(str(val))
    return None


def func_score(fn: dict) -> float | None:
    for key in ("matching", "similarity", "effective_ratio", "fuzzy_match_percent", "match_percent", "percent"):
        if key in fn and fn[key] is not None:
            return float(fn[key])
    return None

def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("addrs", nargs="+", help="addresses like 0x0045ca30")
    parser.add_argument("--json", type=Path, default=DEFAULT_JSON)
    args = parser.parse_args()

    if not args.json.exists():
        print(f"missing {args.json}; generate with:", file=sys.stderr)
        print(
            "  .decomp-venv\\Scripts\\reccmp-reccmp.exe --target LEMBALL --silent --json-diet --json build-msvc400/scores.json",
            file=sys.stderr,
        )
        return 2

    funcs = load_functions(args.json)
    by_addr = {}
    for fn in funcs:
        addr = func_addr(fn)
        if addr is not None:
            by_addr[addr] = fn

    rc = 0
    for raw in args.addrs:
        addr = norm_addr(raw)
        fn = by_addr.get(addr)
        if not fn:
            print(f"ADDR 0x{addr:08x} NOT_IN_JSON")
            rc = 1
            continue
        score = func_score(fn)
        name = fn.get("name") or fn.get("orig_name") or "?"
        if score is None:
            print(f"ADDR 0x{addr:08x} {name} NO_SCORE")
            rc = 1
        else:
            # normalize to percent display
            pct = score * 100.0 if score <= 1.0 else score
            print(f"ADDR 0x{addr:08x} {name} {pct:.2f}%")
    return rc


if __name__ == "__main__":
    raise SystemExit(main())
