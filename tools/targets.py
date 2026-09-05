#!/usr/bin/env python3
"""Rank decomp targets using original x86 sizes and dependency readiness.

Reads build-msvc400/report.json (objdiff rules, same as tools/report.py) and
src annotations. Sizes come from reachable instructions in data/LEMBALL.EXE;
ambiguous control flow is reported as unknown, never replaced by stub sizes.
Readiness and expected gain are conservative ranking heuristics, not proof
of semantic completeness. Does not build or run reccmp unless --refresh.

  python tools/targets.py
  python tools/targets.py --refresh
  python tools/targets.py --kind tiny --max-size 5
  python tools/targets.py --kind near
  python tools/targets.py --kind gain
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
from dataclasses import dataclass, replace
from pathlib import Path

TOOLS_DIR = Path(__file__).resolve().parent
ROOT = TOOLS_DIR.parent
BUILD = ROOT / "build-msvc400"
REPORT = BUILD / "report.json"
SRC = ROOT / "src"

ANNOT_RE = re.compile(r"//\s*(FUNCTION|STUB):\s*LEMBALL\s+(0x[0-9A-Fa-f]+)")
ENTRY_RE = re.compile(r"//\s*(?:FUNCTION|STUB|TEMPLATE|SYNTHETIC|LIBRARY):\s*LEMBALL\s+(0x[0-9A-Fa-f]+)")


@dataclass(frozen=True)
class Func:
    addr: int
    name: str
    size: int
    ratio: float
    unit: str
    annot: str | None
    original_size: int | None = None
    readiness: float = 0.0
    size_status: str = "unknown"

    @property
    def expected_gain(self) -> float:
        """Ranking heuristic, not a calibrated prediction or equivalence score."""
        return (self.original_size or 0) * (1.0 - self.ratio / 100.0) * self.readiness

    @property
    def method(self) -> str:
        return self.name.split("(")[0].split("::")[-1]


@dataclass(frozen=True)
class OriginalEvidence:
    size: int | None
    callees: tuple[int, ...] = ()
    indirect_calls: int = 0
    reason: str = "reachable x86 span"


def inspect_original(image, address: int, entries: set[int]) -> OriginalEvidence:
    """Follow reachable x86 instructions; fail closed on ambiguous boundaries/tables.

    Size is the entry-to-last-reachable-instruction span, excluding trailing
    alignment. It is never the rebuilt procedure size or a next-symbol estimate.
    """
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32

    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    limit = min((a for a in entries if a > address), default=address + 65536)
    limit = min(limit, address + 65536)
    pending = [address]
    instructions = set()
    occupied = set()
    callees = set()
    indirect = 0

    def unknown(reason):
        return OriginalEvidence(None, tuple(sorted(callees)), indirect, reason)

    while pending:
        current = pending.pop()
        if current in instructions:
            continue
        if not address <= current < limit:
            return unknown("flow crosses known boundary")
        if current in occupied:
            return unknown("branch into instruction")
        try:
            raw = image.read(current, min(15, limit - current))
        except (ValueError, IndexError):
            return unknown("unreadable original bytes")
        ins = next(decoder.disasm_lite(raw, current), None)
        if ins is None:
            return unknown("truncated/invalid instruction")
        _, size, mnemonic, operand = ins
        if any(a in occupied for a in range(current, current + size)):
            return unknown("overlapping instructions")
        instructions.add(current)
        occupied.update(range(current, current + size))
        following = current + size
        if mnemonic.startswith('ret'):
            continue
        if mnemonic in ('int3', 'hlt', 'ud2', 'int', 'iret', 'iretd'):
            return unknown("unsupported terminal instruction")
        if mnemonic == 'call':
            if re.fullmatch(r'0x[0-9a-f]+', operand):
                callees.add(int(operand, 16))
            else:
                indirect += 1
        if mnemonic.startswith('j') or mnemonic.startswith('loop'):
            if not re.fullmatch(r'0x[0-9a-f]+', operand):
                return unknown("indirect jump requires table evidence")
            target = int(operand, 16)
            if not address <= target < limit:
                if mnemonic == 'jmp' and target in entries:
                    callees.add(target)
                    continue
                return unknown("unresolved external branch")
            pending.append(target)
            if mnemonic == 'jmp':
                continue
        pending.append(following)
    return OriginalEvidence(max(occupied) + 1 - address, tuple(sorted(callees)), indirect)


def resolve_original_target(image, address: int) -> int | None:
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32

    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    seen = set()
    while address not in seen and len(seen) < 32:
        seen.add(address)
        try:
            ins = next(decoder.disasm_lite(image.read(address, 15), address), None)
        except (ValueError, IndexError):
            return None
        if ins is None:
            return None
        if ins[2] != 'jmp' or not re.fullmatch(r'0x[0-9a-f]+', ins[3]):
            return address
        address = int(ins[3], 16)
    return None


def add_original_evidence(funcs: list[Func]) -> list[Func]:
    from check import load_original_image

    image = load_original_image()
    if image is None:
        raise ValueError('original executable is unavailable')
    entries = {f.addr for f in funcs}
    for path in (*SRC.rglob('*.h'), *SRC.rglob('*.cpp')):
        entries.update(int(m[1], 16) for m in ENTRY_RE.finditer(path.read_text(encoding='utf-8', errors='replace')))
    by_address = {f.addr: f for f in funcs}
    result = []
    for func in funcs:
        evidence = inspect_original(image, func.addr, entries)
        resolved = [resolve_original_target(image, a) for a in evidence.callees]
        unresolved = sum(a is None for a in resolved)
        dependencies = {a for a in resolved if a is not None}
        dependencies.discard(func.addr)  # recursion is not a separate dependency
        ready = sum(a in by_address and by_address[a].annot != 'STUB' and by_address[a].ratio > 0 for a in dependencies)
        total = len(dependencies) + unresolved + evidence.indirect_calls
        readiness = ready / total if total else 1.0
        result.append(replace(func, original_size=evidence.size, readiness=readiness, size_status=evidence.reason))
    return result


def ranked_gain(funcs: list[Func]) -> list[Func]:
    return sorted((f for f in funcs if f.ratio < 100 and f.original_size is not None),
                  key=lambda f: (-f.expected_gain, -f.readiness, f.original_size, f.addr))


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
    return add_original_evidence(funcs)


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
    original = str(func.original_size) if func.original_size is not None else '?'
    return (f"  0x{func.addr:08x}  {func.ratio:6.2f}%  orig={original:>5s} rebuilt={func.size:4d} "
            f"ready={func.readiness:4.0%} gain~{func.expected_gain:7.1f}  {kind:5s}  {func.unit}  {func.name}")


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
    items = [f for f in stubs(funcs) if f.original_size is not None and f.original_size <= max_size]
    by_unit = Counter(f.unit for f in items)
    return sorted(items, key=lambda f: (-by_unit[f.unit], f.unit, f.addr))


def near_funcs(funcs: list[Func]) -> list[Func]:
    return sorted(grind(funcs), key=lambda f: (-f.ratio, f.original_size or float('inf'), f.addr))


def clone_groups(funcs: list[Func], min_clone: int) -> list[tuple[str, int, list[Func]]]:
    groups: dict[tuple[str, int], list[Func]] = defaultdict(list)
    for func in stubs(funcs):
        if func.original_size is not None:
            groups[(func.method, func.original_size)].append(func)
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
        unmatched_code = sum(f.original_size or 0 for f in leftover)
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
    print(f"  rebuilt weights:  {matched_code}/{code} bytes  ({100.0 * matched_code / code:.2f}%)")
    print(f"  original sizing:  {sum(f.original_size is not None for f in funcs)}/{n} proven reachable spans")
    print("  readiness: known non-STUB callees with positive scores / all observed callees")
    print("  gain~: original size * remaining fraction * readiness (heuristic, not a match score)")
    print("  indirect calls count as unproven; unknown sizes are excluded from tiny/clone/gain lists")
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
        group = [f for f in stubs(funcs) if f.original_size is not None and lo <= f.original_size <= hi]
        print(f"  {label:6s}  n={len(group):4d}  bytes={sum(f.original_size for f in group):6d}")
    print(f"  unknown n={sum(f.original_size is None for f in stubs(funcs)):4d}")


def print_tiny(funcs: list[Func], max_size: int, limit: int) -> None:
    items = tiny_stubs(funcs, max_size)
    print(f"=== tiny STUBs (size <= {max_size}) ===")
    print(f"  count={len(items)}  original_bytes={sum(f.original_size for f in items)}")
    by_unit = Counter(f.unit for f in items)
    print("  by unit:")
    unit_rows = by_unit.most_common()
    for unit, count in limited(unit_rows, limit):
        bytes_ = sum(f.original_size for f in items if f.unit == unit)
        print(f"    n={count:3d}  bytes={bytes_:4d}  {unit}")
    if limit > 0 and len(unit_rows) > limit:
        print(f"    ... {len(unit_rows) - limit} more units (raise --limit)")
    print("  functions (by unit, then address):")
    print_funcs(items, limit)


def print_near(funcs: list[Func], limit: int) -> None:
    items = near_funcs(funcs)
    print("=== FUNCTION not 100% (high ratio first) ===")
    print(f"  count={len(items)}  known_original_bytes={sum(f.original_size or 0 for f in items)}")
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
            f"known_original_bytes={unmatched_code:5d}  {name}"
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
        choices=("all", "tiny", "near", "unit", "clone", "gain"),
        default="all",
        help="which target list to print (default: all)",
    )
    parser.add_argument("--max-size", type=int, default=8, help="tiny STUB size cap (default 8)")
    parser.add_argument("--min-clone", type=int, default=4, help="minimum clone-group size (default 4)")
    parser.add_argument("--limit", type=int, default=40, help="max rows per list; 0 = unlimited")
    parser.add_argument("--addrs", action="store_true", help="print addresses only (requires --kind tiny, near or gain)")
    parser.add_argument("--refresh", action="store_true", help="run tools/report.py before ranking")
    parser.add_argument("--report", type=Path, default=REPORT, help="path to report.json")
    args = parser.parse_args()

    if args.addrs and args.kind not in ("tiny", "near", "gain"):
        parser.error("--addrs requires --kind tiny, --kind near or --kind gain")

    if args.refresh:
        refresh_report()

    if not args.report.exists():
        sys.stderr.write(
            f"missing {args.report}\n"
            "run: python tools/report.py\n"
            "or:  python tools/targets.py --refresh\n"
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
        if args.limit > 0:
            selected = selected[: args.limit]
        print_addrs(selected)
        return 0

    if args.kind == "all":
        print_snapshot(funcs)
        print("\n=== expected matched-byte gain (heuristic) ===")
        print_funcs(ranked_gain(funcs), args.limit)
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
    elif args.kind == "clone":
        print_clones(funcs, args.min_clone, args.limit)
    else:
        print("=== expected matched-byte gain (heuristic) ===")
        print_funcs(ranked_gain(funcs), args.limit)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
