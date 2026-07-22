#!/usr/bin/env python3
"""Join Ghidra, reccmp, source annotations, and rebuilt function ownership."""

from __future__ import annotations

import argparse
from collections import Counter
from pathlib import Path

from function_inventory import reconcile_inventory, write_inventory_csv


ROOT = Path(__file__).resolve().parents[1]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--target", default="LEMBALL")
    parser.add_argument("--manifest", type=Path, default=ROOT / "data" / "manifest.json")
    parser.add_argument("--reccmp-report", type=Path, default=ROOT / "build" / "reccmp-current.json")
    parser.add_argument("--roadmap", type=Path, default=ROOT / "build" / "reccmp-roadmap.csv")
    parser.add_argument("--source-root", type=Path, default=ROOT / "src")
    parser.add_argument("--runtime-symbols", type=Path, default=ROOT / "data" / "runtime-symbols.csv")
    parser.add_argument("--output", type=Path, default=ROOT / "build" / "function-inventory.csv")
    args = parser.parse_args()

    required = (
        args.manifest,
        args.reccmp_report,
        args.roadmap,
        args.source_root,
        args.runtime_symbols,
    )
    for path in required:
        if not path.exists():
            raise SystemExit(f"required input does not exist: {path}")

    rows, unresolved = reconcile_inventory(
        args.manifest,
        args.reccmp_report,
        args.roadmap,
        args.source_root,
        args.runtime_symbols,
        args.target,
    )
    write_inventory_csv(args.output, rows)
    reasons = Counter(str(row["resolution_reason"]) for row in unresolved)
    if unresolved:
        detail = ", ".join(f"{reason}={count}" for reason, count in sorted(reasons.items()))
        print(f"function inventory failed: {len(unresolved)} unresolved ({detail})")
        print(f"ledger: {args.output}")
        return 1
    reportable = sum(row["manifest_category"] in {"internal", "thunk"} for row in rows)
    excluded = sum(str(row["resolution_reason"]).startswith("explicit_") for row in rows)
    print(
        f"function inventory clean: {reportable} reportable; "
        f"{excluded} explicit reccmp exclusions; 0 unresolved"
    )
    print(f"ledger: {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
