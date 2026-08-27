#!/usr/bin/env python3
"""Rank next decomp targets for maximizing 100% function count.

Reads build-msvc400/report.json (objdiff rules, same as tools/report.py) and
src FUNCTION/STUB annotations. Does not build or run reccmp unless --refresh.

  python tools/targets.py
  python tools/targets.py --refresh
  python tools/targets.py --kind tiny --max-size 5
  python tools/targets.py --kind near
  python tools/targets.py --kind unit
  python tools/targets.py --kind clone
  python tools/targets.py --kind tiny --addrs
"""

from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
from collections import Counter, defaultdict
from dataclasses import dataclass
from pathlib import Path

TOOLS_DIR = Path(__file__).resolve().parent
ROOT = TOOLS_DIR.parent
BUILD = ROOT / "build-msvc400"
REPORT = BUILD / "report.json"
SRC = ROOT / "src"

ANNOT_RE = re.compile(r"//\s*(FUNCTION|STUB):\s*LEMBALL\s+(0x[0-9A-Fa-f]+)")


@dataclass(frozen=True)
class Func:
    addr: int
    name: str
    size: int
    ratio: float
    unit: str
    annot: str | None

    @property
    def method(self) -> str:
        return self.name.split("(")[0].split("::")[-1]


def load_annotations() -> dict[int, str]:
    found: dict[int, str] = {}
    for path in (*SRC.rglob("*.cpp"), *SRC.rglob("*.h")):
        text = path.read_text(encoding="utf-8", errors="replace")
        for match in ANNOT_RE.finditer(text):
            found[int(match.group(2), 16)] = match.group(1).upper()
    return found


def load_report(path: Path) -> list[Func]:
    report = json.loads(path.read_text(encoding="utf-8"))
    annot = load_annotations()
    funcs: list[Func] = []
    for unit in report["units"]:
        for item in unit["functions"]:
            addr = int(item["metadata"]["virtual_address"])
            funcs.append(
                Func(
                    addr=addr,
                    name=item["name"],
                    size=int(item["size"]),
                    ratio=float(item["fuzzy_match_percent"]),
                    unit=unit["name"],
                    annot=annot.get(addr),
                )
            )
    return funcs


def ratio_bucket(ratio: float) -> str:
    if ratio == 100.0:
        return "100"
    if ratio == 0.0:
        return "0"
    if ratio >= 99.0:
        return "99-99.99"
    if ratio >= 95.0:
        return "95-98.99"
    if ratio >= 90.0:
        return "90-94.99"
    if ratio >= 80.0:
        return "80-89.99"
    if ratio >= 50.0:
        return "50-79.99"
    return "0.01-49.99"


def fmt_func(func: Func) -> str:
    kind = func.annot or "UNANN"
    return f"  0x{func.addr:08x}  {func.ratio:6.2f}%  sz={func.size:4d}  {kind:5s}  {func.unit}  {func.name}"


def limited(items: list, limit: int) -> list:
    if limit <= 0:
        return items
    return items[:limit]


def print_funcs(funcs: list[Func], limit: int) -> None:
    shown = limited(funcs, limit)
    for func in shown:
        print(fmt_func(func))
    if limit > 0 and len(funcs) > limit:
        print(f"  ... {len(funcs) - limit} more (raise --limit)")


def stubs(funcs: list[Func]) -> list[Func]:
    return [f for f in funcs if f.annot == "STUB"]


def grind(funcs: list[Func]) -> list[Func]:
    return [f for f in funcs if f.annot == "FUNCTION" and f.ratio < 100.0]


def tiny_stubs(funcs: list[Func], max_size: int) -> list[Func]:
    items = [f for f in stubs(funcs) if f.size <= max_size]
    by_unit = Counter(f.unit for f in items)
    return sorted(items, key=lambda f: (-by_unit[f.unit], f.unit, f.addr))


def near_funcs(funcs: list[Func]) -> list[Func]:
    return sorted(grind(funcs), key=lambda f: (-f.ratio, f.size, f.addr))


def clone_groups(funcs: list[Func], min_clone: int) -> list[tuple[str, int, list[Func]]]:
    groups: dict[tuple[str, int], list[Func]] = defaultdict(list)
    for func in stubs(funcs):
        groups[(func.method, func.size)].append(func)
    ranked = [(name, size, items) for (name, size), items in groups.items() if len(items) >= min_clone]
    ranked.sort(key=lambda item: (-len(item[2]), item[0], item[1]))
    return ranked


def leftover_units(funcs: list[Func]) -> list[tuple[str, int, int, int, int]]:
    by_unit: dict[str, list[Func]] = defaultdict(list)
    for func in funcs:
        by_unit[func.unit].append(func)
    rows = []
    for name, items in by_unit.items():
        n100 = sum(1 for f in items if f.ratio == 100.0)
        leftover = [f for f in items if f.ratio < 100.0]
        if not leftover or n100 == 0:
            continue
        unmatched_code = sum(f.size for f in leftover)
        rows.append((name, n100, len(items), len(leftover), unmatched_code))
    rows.sort(key=lambda r: (-r[1] / r[2], r[3], r[4], r[0]))
    return rows


def print_snapshot(funcs: list[Func]) -> None:
    n = len(funcs)
    n100 = sum(1 for f in funcs if f.ratio == 100.0)
    n_fn = sum(1 for f in funcs if f.annot == "FUNCTION")
    n_stub = sum(1 for f in funcs if f.annot == "STUB")
    n_unann = sum(1 for f in funcs if f.annot is None)
    n_grind = len(grind(funcs))
    code = sum(f.size for f in funcs)
    matched_code = sum(f.size for f in funcs if f.ratio == 100.0)
    print("=== snapshot ===")
    print(f"  report functions: {n100}/{n} at 100%  ({100.0 * n100 / n:.2f}%)")
    print(f"  matched code:     {matched_code}/{code} bytes  ({100.0 * matched_code / code:.2f}%)")
    print(f"  source FUNCTION:  {n_fn}  (not 100%: {n_grind})")
    print(f"  source STUB:      {n_stub}")
    print(f"  unannotated:      {n_unann}")
    print()
    print("=== FUNCTION ratio buckets ===")
    buckets = Counter(ratio_bucket(f.ratio) for f in funcs if f.annot == "FUNCTION")
    for key in ("100", "99-99.99", "95-98.99", "90-94.99", "80-89.99", "50-79.99", "0.01-49.99", "0"):
        print(f"  {key:12s}  {buckets[key]}")
    print()
    print("=== STUB orig-size histogram ===")
    for lo, hi, label in (
        (1, 1, "1"),
        (2, 3, "2-3"),
        (4, 5, "4-5"),
        (6, 8, "6-8"),
        (9, 16, "9-16"),
        (17, 32, "17-32"),
        (33, 64, "33-64"),
        (65, 10**9, ">64"),
    ):
        group = [f for f in stubs(funcs) if lo <= f.size <= hi]
        print(f"  {label:6s}  n={len(group):4d}  bytes={sum(f.size for f in group):6d}")


def print_tiny(funcs: list[Func], max_size: int, limit: int) -> None:
    items = tiny_stubs(funcs, max_size)
    print(f"=== tiny STUBs (size <= {max_size}) ===")
    print(f"  count={len(items)}  bytes={sum(f.size for f in items)}")
    by_unit = Counter(f.unit for f in items)
    print("  by unit:")
    unit_rows = by_unit.most_common()
    for unit, count in limited(unit_rows, limit):
        bytes_ = sum(f.size for f in items if f.unit == unit)
        print(f"    n={count:3d}  bytes={bytes_:4d}  {unit}")
    if limit > 0 and len(unit_rows) > limit:
        print(f"    ... {len(unit_rows) - limit} more units (raise --limit)")
    print("  functions (by unit, then address):")
    print_funcs(items, limit)


def print_near(funcs: list[Func], limit: int) -> None:
    items = near_funcs(funcs)
    print("=== FUNCTION not 100% (high ratio first) ===")
    print(f"  count={len(items)}  bytes={sum(f.size for f in items)}")
    by_unit = Counter(f.unit for f in items)
    print("  by unit:")
    for unit, count in by_unit.most_common(limit if limit else None):
        print(f"    n={count:3d}  {unit}")
    print("  functions:")
    print_funcs(items, limit)


def print_units(funcs: list[Func], limit: int) -> None:
    rows = leftover_units(funcs)
    print("=== units with some 100% and leftovers ===")
    shown = limited(rows, limit)
    for name, n100, total, leftover, unmatched_code in shown:
        pct = 100.0 * n100 / total
        print(
            f"  {pct:5.1f}%  {n100}/{total}  leftover={leftover:3d}  "
            f"unmatched_code={unmatched_code:5d}  {name}"
        )
    if limit > 0 and len(rows) > limit:
        print(f"  ... {len(rows) - limit} more (raise --limit)")


def print_clones(funcs: list[Func], min_clone: int, limit: int) -> None:
    groups = clone_groups(funcs, min_clone)
    print(f"=== STUB clone groups (same method+size, n>={min_clone}) ===")
    print(f"  groups={len(groups)}")
    shown = limited(groups, limit)
    for name, size, items in shown:
        units = sorted({f.unit for f in items})
        print(f"  n={len(items):3d}  size={size:3d}  {name}  units={len(units)}  e.g. {units[0]}")
    if limit > 0 and len(groups) > limit:
        print(f"  ... {len(groups) - limit} more (raise --limit)")


def print_addrs(funcs: list[Func]) -> None:
    print(" ".join(f"0x{f.addr:08x}" for f in funcs))


def refresh_report() -> None:
    cmd = [sys.executable, str(TOOLS_DIR / "report.py")]
    subprocess.run(cmd, cwd=ROOT, check=True)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--kind",
        choices=("all", "tiny", "near", "unit", "clone"),
        default="all",
        help="which target list to print (default: all)",
    )
    parser.add_argument("--max-size", type=int, default=8, help="tiny STUB size cap (default 8)")
    parser.add_argument("--min-clone", type=int, default=4, help="minimum clone-group size (default 4)")
    parser.add_argument("--limit", type=int, default=40, help="max rows per list; 0 = unlimited")
    parser.add_argument("--addrs", action="store_true", help="print addresses only (requires --kind tiny or near)")
    parser.add_argument("--refresh", action="store_true", help="run tools/report.py before ranking")
    parser.add_argument("--report", type=Path, default=REPORT, help="path to report.json")
    args = parser.parse_args()

    if args.addrs and args.kind not in ("tiny", "near"):
        parser.error("--addrs requires --kind tiny or --kind near")

    if args.refresh:
        refresh_report()

    if not args.report.exists():
        sys.stderr.write(
            f"missing {args.report}\n"
            "run: python tools/report.py\n"
            "or:  python tools/targets.py --refresh\n"
        )
        return 1

    funcs = load_report(args.report)

    if args.addrs:
        selected = tiny_stubs(funcs, args.max_size) if args.kind == "tiny" else near_funcs(funcs)
        if args.limit > 0:
            selected = selected[: args.limit]
        print_addrs(selected)
        return 0

    if args.kind == "all":
        print_snapshot(funcs)
        print()
        print_tiny(funcs, args.max_size, args.limit)
        print()
        print_clones(funcs, args.min_clone, args.limit)
        print()
        print_near(funcs, args.limit)
        print()
        print_units(funcs, args.limit)
        return 0

    if args.kind == "tiny":
        print_tiny(funcs, args.max_size, args.limit)
    elif args.kind == "near":
        print_near(funcs, args.limit)
    elif args.kind == "unit":
        print_units(funcs, args.limit)
    else:
        print_clones(funcs, args.min_clone, args.limit)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
