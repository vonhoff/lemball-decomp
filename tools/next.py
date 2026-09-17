#!/usr/bin/env python3
"""Rank reconstruction targets from report.json and original x86 extents."""

from __future__ import annotations

import argparse
import json
import sys
from collections import Counter, defaultdict
from dataclasses import dataclass, replace
from itertools import pairwise
from pathlib import Path

from lib.paths import ORIGINAL_EXE, REPORT_JSON, TARGETS_CACHE, file_id
from lib.reccmp_compat import complete_original_extent
from lib.source import ANNOT_WITH_ADDR, collect_sources
from report import make_report


@dataclass(frozen=True)
class Func:
    addr: int
    name: str
    size: int
    ratio: float
    unit: str
    annot: str | None
    original_size: int | None = None

    @property
    def method(self) -> str:
        return self.name.split("(")[0].split("::")[-1]


def add_original_evidence(funcs: list[Func], entries: set[int], report_path: Path) -> list[Func]:
    inputs = {
        "original": file_id(ORIGINAL_EXE),
        "report": file_id(report_path),
        "ranking": file_id(Path(__file__)),
        "extent_reader": file_id(Path(__file__).parent / "lib/reccmp_compat.py"),
        "entries": sorted(entries),
    }
    try:
        cache = json.loads(TARGETS_CACHE.read_text(encoding="utf-8"))
        sizes = cache["sizes"] if cache["inputs"] == inputs else {}
    except (OSError, ValueError, KeyError, TypeError):
        sizes = {}
    if not all(str(f.addr) in sizes for f in funcs):
        from capstone import CS_ARCH_X86, CS_MODE_32, Cs
        from reccmp.formats import detect_image

        image = detect_image(ORIGINAL_EXE)
        if image is None:
            raise ValueError("original executable is unavailable")
        decoder = Cs(CS_ARCH_X86, CS_MODE_32)
        entries = sorted(entries | {f.addr for f in funcs})
        limits = dict(pairwise(entries))
        sizes = {
            str(f.addr): complete_original_extent(
                image, f.addr, limits.get(f.addr, f.addr + 65536), decoder
            )
            for f in funcs
        }
        TARGETS_CACHE.parent.mkdir(parents=True, exist_ok=True)
        TARGETS_CACHE.write_text(json.dumps({"inputs": inputs, "sizes": sizes}) + "\n", encoding="utf-8")
    return [replace(f, original_size=sizes[str(f.addr)]) for f in funcs]


def ranked_gain(funcs: list[Func]) -> list[Func]:
    return sorted((f for f in funcs if f.ratio < 100),
                  key=lambda f: (-f.size * f.ratio, -f.size, f.addr))


def scan_annotations() -> tuple[dict[int, str], set[int]]:
    found: dict[int, str] = {}
    entries: set[int] = set()
    for path in collect_sources():
        text = path.read_text(encoding="utf-8", errors="replace")
        for match in ANNOT_WITH_ADDR.finditer(text):
            addr = int(match.group(2), 16)
            entries.add(addr)
            if match.group(1) in ("FUNCTION", "STUB"):
                found[addr] = match.group(1).upper()
    return found, entries


def load_report(path: Path) -> list[Func]:
    report = json.loads(path.read_text(encoding="utf-8"))
    annot, entries = scan_annotations()
    funcs = [
        Func(
            addr=int(item["metadata"]["virtual_address"]),
            name=item["name"],
            size=int(item["size"]),
            ratio=float(item["fuzzy_match_percent"]),
            unit=unit["name"],
            annot=annot.get(int(item["metadata"]["virtual_address"])),
        )
        for unit in report["units"]
        for item in unit["functions"]
    ]
    return add_original_evidence(funcs, entries, path)


def fmt_func(func: Func) -> str:
    kind = func.annot or "UNANN"
    original = str(func.original_size) if func.original_size is not None else '?'
    return (f"  0x{func.addr:08x}  {func.ratio:6.2f}%  orig={original:>5s} rebuilt={func.size:4d} "
            f"{kind:5s}  {func.unit}  {func.name}")


def limited(items: list, limit: int) -> list:
    return items[:limit] if limit > 0 else items


def print_funcs(funcs: list[Func], limit: int) -> None:
    for func in limited(funcs, limit):
        print(fmt_func(func))
    if 0 < limit < len(funcs):
        print(f"  ... {len(funcs) - limit} more (raise --limit)")


def tiny_stubs(funcs: list[Func], max_size: int) -> list[Func]:
    items = [f for f in funcs if f.annot == "STUB" and f.original_size is not None and f.original_size <= max_size]
    by_unit = Counter(f.unit for f in items)
    return sorted(items, key=lambda f: (-by_unit[f.unit], f.unit, f.addr))


def near_funcs(funcs: list[Func]) -> list[Func]:
    return sorted((f for f in funcs if f.annot == "FUNCTION" and f.ratio < 100),
                  key=lambda f: (-f.ratio, f.original_size or float('inf'), f.addr))


def clone_groups(funcs: list[Func], min_clone: int) -> list[tuple[str, int, list[Func]]]:
    groups: dict[tuple[str, int], list[Func]] = defaultdict(list)
    for func in funcs:
        if func.annot == "STUB" and func.original_size is not None:
            groups[(func.method, func.original_size)].append(func)
    return sorted(((name, size, items) for (name, size), items in groups.items() if len(items) >= min_clone),
                  key=lambda item: (-len(item[2]), item[0], item[1]))


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
        rows.append((name, n100, len(items), len(leftover), sum(f.original_size or 0 for f in leftover)))
    rows.sort(key=lambda r: (-r[1] / r[2], r[3], r[4], r[0]))
    return rows


def print_snapshot(funcs: list[Func]) -> None:
    n = len(funcs)
    n100 = sum(1 for f in funcs if f.ratio == 100.0)
    annotations = Counter(f.annot for f in funcs)
    n_grind = sum(f.annot == "FUNCTION" and f.ratio < 100 for f in funcs)
    code = sum(f.size for f in funcs)
    matched_code = sum(f.size for f in funcs if f.ratio == 100.0)
    print("=== snapshot ===")
    print(f"  report functions: {n100}/{n} at 100%  ({100.0 * n100 / n:.2f}%)")
    print(f"  rebuilt weights:  {matched_code}/{code} bytes  ({100.0 * matched_code / code:.2f}%)")
    print(f"  original sizing:  {sum(f.original_size is not None for f in funcs)}/{n} proven reachable spans")
    print(f"  source FUNCTION:  {annotations['FUNCTION']}  (not 100%: {n_grind})")
    print(f"  source STUB:      {annotations['STUB']}")
    print(f"  unannotated:      {annotations[None]}")


def print_tiny(funcs: list[Func], max_size: int, limit: int) -> None:
    items = tiny_stubs(funcs, max_size)
    print(f"=== tiny STUBs (size <= {max_size}) ===")
    print(f"  count={len(items)}  original_bytes={sum(f.original_size for f in items)}")
    print_funcs(items, limit)


def print_near(funcs: list[Func], limit: int) -> None:
    items = near_funcs(funcs)
    print("=== FUNCTION not 100% (high ratio first) ===")
    print(f"  count={len(items)}  known_original_bytes={sum(f.original_size or 0 for f in items)}")
    print_funcs(items, limit)


def print_units(funcs: list[Func], limit: int) -> None:
    rows = leftover_units(funcs)
    print("=== units with some 100% and leftovers ===")
    for name, n100, total, leftover, unmatched_code in limited(rows, limit):
        pct = 100.0 * n100 / total
        print(
            f"  {pct:5.1f}%  {n100}/{total}  leftover={leftover:3d}  "
            f"known_original_bytes={unmatched_code:5d}  {name}"
        )
    if 0 < limit < len(rows):
        print(f"  ... {len(rows) - limit} more (raise --limit)")


def print_clones(funcs: list[Func], min_clone: int, limit: int) -> None:
    groups = clone_groups(funcs, min_clone)
    print(f"=== STUB clone groups (same method+size, n>={min_clone}) ===")
    print(f"  groups={len(groups)}")
    for name, size, items in limited(groups, limit):
        units = sorted({f.unit for f in items})
        print(f"  n={len(items):3d}  size={size:3d}  {name}  units={len(units)}  e.g. {units[0]}")
    if 0 < limit < len(groups):
        print(f"  ... {len(groups) - limit} more (raise --limit)")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--kind",
        choices=("all", "tiny", "near", "unit", "clone", "gain"),
        default="all",
        help="which target list to print (default: all)",
    )
    parser.add_argument("--max-size", type=int, default=8, help="tiny STUB size cap (default 8)")
    parser.add_argument("--min-clone", type=int, default=4, help="minimum clone-group size (default 4)")
    parser.add_argument("--limit", type=int, default=40, help="max rows per list; 0 = unlimited")
    parser.add_argument("--addrs", action="store_true", help="print addresses only (requires --kind tiny, near or gain)")
    parser.add_argument("--refresh", action="store_true", help="run tools/report.py before ranking")
    parser.add_argument("--report", type=Path, default=REPORT_JSON, help="path to report.json")
    args = parser.parse_args()

    if args.addrs and args.kind not in ("tiny", "near", "gain"):
        parser.error("--addrs requires --kind tiny, --kind near or --kind gain")

    if args.refresh:
        make_report(args.report)

    if not args.report.exists():
        sys.stderr.write(
            f"missing {args.report}\n"
            "run: python tools/report.py\n"
            "or:  python tools/next.py --refresh\n"
        )
        return 1

    try:
        funcs = load_report(args.report)
    except (ValueError, OSError) as error:
        sys.stderr.write(f"cannot rank without original evidence: {error}\n")
        return 1

    if args.addrs:
        selected = (tiny_stubs(funcs, args.max_size) if args.kind == "tiny"
                    else ranked_gain(funcs) if args.kind == "gain" else near_funcs(funcs))
        print(" ".join(f"0x{f.addr:08x}" for f in limited(selected, args.limit)))
        return 0

    if args.kind == "all":
        print_snapshot(funcs)
    if args.kind in ("all", "gain"):
        print("=== largest near matches (rebuilt size x similarity) ===")
        print_funcs(ranked_gain(funcs), args.limit)
    if args.kind in ("all", "tiny"):
        print_tiny(funcs, args.max_size, args.limit)
    if args.kind in ("all", "clone"):
        print_clones(funcs, args.min_clone, args.limit)
    if args.kind in ("all", "near"):
        print_near(funcs, args.limit)
    if args.kind in ("all", "unit"):
        print_units(funcs, args.limit)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
