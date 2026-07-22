#!/usr/bin/env python3
"""Require live-Ghidra and headless exports to describe the same functions."""

from __future__ import annotations

import argparse
import json
from pathlib import Path


def inventory(path: Path) -> tuple[dict[str, object], list[tuple[object, ...]]]:
    manifest = json.loads(path.read_text(encoding="utf-8"))
    functions = [
        (
            int(function["address"], 16),
            function["category"],
            function["name"],
            int(function["size"]),
            bool(function["is_thunk"]),
            bool(function["is_external"]),
        )
        for function in manifest["functions"]
    ]
    if functions != sorted(functions):
        raise SystemExit(f"{path} is not sorted by address")
    header = {
        "version": manifest.get("version"),
        "program": manifest.get("program"),
        "sha256": manifest.get("sha256"),
        "function_count": manifest.get("function_count"),
        "summary": manifest.get("summary"),
    }
    return header, functions


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("live", type=Path)
    parser.add_argument("headless", type=Path)
    args = parser.parse_args()

    live_header, live_functions = inventory(args.live)
    headless_header, headless_functions = inventory(args.headless)
    if live_header != headless_header:
        raise SystemExit("live/headless manifest headers differ")
    if live_functions != headless_functions:
        live_set = set(live_functions)
        headless_set = set(headless_functions)
        raise SystemExit(
            "live/headless function inventories differ: "
            f"live-only={len(live_set - headless_set)}, "
            f"headless-only={len(headless_set - live_set)}"
        )

    reportable = sum(category in {"internal", "thunk"} for _, category, *_ in live_functions)
    print(
        f"Ghidra manifest parity clean: {len(live_functions)} total, "
        f"{reportable} reportable"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
