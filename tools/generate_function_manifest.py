#!/usr/bin/env python3

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate a binary-backed recovered-function manifest from a Ghidra export."
    )
    parser.add_argument("--input", required=True, help="Path to the raw Ghidra export JSON")
    parser.add_argument("--output", required=True, help="Path to write the function manifest")
    return parser.parse_args()


def classify_function(name: str, is_thunk: bool, is_external: bool) -> str:
    if is_external:
        return "external"
    if is_thunk and (
        name.startswith("Ordinal_")
        or "@" in name
        or re.match(r"^[A-Z][A-Za-z0-9_]*$", name) is not None
    ):
        return "import"
    if is_thunk:
        return "thunk"
    if (
        name.startswith("__")
        or name.startswith("_")
        or name.startswith("FID_conflict:")
        or name.startswith("Ordinal_")
        or name in {"entry", "atexit", "exit"}
    ):
        return "runtime"
    return "internal"

def main() -> int:
    args = parse_args()
    input_path = Path(args.input)
    output_path = Path(args.output)
    raw = json.loads(input_path.read_text())

    recovered_functions = []
    summary = {
        "total": 0,
        "internal": 0,
        "runtime": 0,
        "thunk": 0,
        "import": 0,
        "external": 0,
    }

    for function in raw["functions"]:
        address = function["address"].upper()
        category = classify_function(
            function["name"], function["is_thunk"], function["is_external"]
        )
        recovered_functions.append(
            {
                "address": address,
                "name": function["name"],
                "size": int(function.get("size", 0)),
                "category": category,
                "is_thunk": function["is_thunk"],
                "is_external": function["is_external"],
                "provenance": "ghidra",
            }
        )
        summary["total"] += 1
        summary[category] += 1

    payload = {
        "version": "LEMBALL",
        "program": raw["program"],
        "function_count": raw["function_count"],
        "summary": summary,
        "functions": recovered_functions,
    }

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(json.dumps(payload, indent=2) + "\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
