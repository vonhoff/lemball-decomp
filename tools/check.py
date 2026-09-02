#!/usr/bin/env python3
"""Central decompilation match evaluation logic and CLI."""

from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

from build import run_build

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build-msvc400"
DEFAULT_JSON = BUILD / "scores.json"
RECCMP = ROOT / ".decomp-venv" / "Scripts" / "reccmp-reccmp.exe"


def norm_addr(value: str | int) -> int:
    if isinstance(value, int):
        return value
    value = value.strip().lower()
    if value.startswith("0x"):
        return int(value, 16)
    return int(value, 16) if any(c in value for c in "abcdef") else int(value, 0)


def insn_text(entry) -> str:
    if isinstance(entry, (list, tuple)) and len(entry) > 1:
        return entry[1]
    return str(entry)


def normalize_asm(s: str) -> str:
    s = s.split("\t")[0].strip()
    s = re.sub(r"Thunk of '([^']+)' \(THUNK\)", r"\1 (FUNCTION)", s)
    s = re.sub(r" \(THUNK\)", " (FUNCTION)", s)
    s = re.sub(r"<OFFSET\d+>", "SYM", s)
    s = re.sub(r"\S+ \((?:DATA|VTABLE|UNK)\)", "SYM", s)
    return s


def is_unresolved_call(orig_text: str, recomp_text: str) -> bool:
    orig_text = orig_text.split("\t")[0].strip()
    recomp_text = recomp_text.split("\t")[0].strip()
    if not re.match(r"call <OFFSET\d+>$", orig_text):
        return False
    return bool(re.match(r"call (?:Thunk of '.+' \(THUNK\)|\S+ \(FUNCTION\))$", recomp_text))


def is_recomp_offset_call(orig_text: str, recomp_text: str) -> bool:
    orig_text = orig_text.split("\t")[0].strip()
    recomp_text = recomp_text.split("\t")[0].strip()
    if not re.match(r"call <OFFSET\d+>", recomp_text):
        return False
    return bool(re.match(r"call .+ \(FUNCTION\)$", orig_text))


def is_equivalent_insn(orig_text: str, recomp_text: str) -> bool:
    if normalize_asm(orig_text) == normalize_asm(recomp_text):
        return True
    if is_unresolved_call(orig_text, recomp_text):
        return True
    if is_recomp_offset_call(orig_text, recomp_text):
        return True
    return False


def collect_diff_insns(diff, key: str) -> list[str]:
    insns = []
    if not diff:
        return insns
    for _, chunks in diff:
        for chunk in chunks:
            for entry in chunk.get(key, []) or []:
                insns.append(insn_text(entry))
    return insns


def is_thunk_only_diff(diff) -> bool:
    if not diff:
        return False
    orig_insns = collect_diff_insns(diff, "orig")
    recomp_insns = collect_diff_insns(diff, "recomp")
    if not orig_insns and not recomp_insns:
        return False
    if len(orig_insns) != len(recomp_insns):
        return False
    for orig_text, recomp_text in zip(orig_insns, recomp_insns):
        if not is_equivalent_insn(orig_text, recomp_text):
            return False
    return True


def compute_ratio(match: dict | None, name: str = "") -> tuple[float, str]:
    """Calculate effective match percentage and match reason tag."""
    if match is None or match.get("stub"):
        return 0.0, "STUB"

    mname = name or match.get("name") or ""
    is_dtor = match.get("type") == 1 and (
        "`scalar deleting destructor'" in mname or "`vector deleting destructor'" in mname
    )

    if match.get("effective") or float(match.get("matching", 0.0)) == 1.0 or is_dtor:
        return 100.0, "MATCH"

    if is_thunk_only_diff(match.get("diff")):
        return 100.0, "MATCH (thunk)"

    return float(match.get("matching", 0.0)) * 100.0, ""


def format_diff_text(diff) -> str:
    if not diff:
        return ""
    lines = []
    for _, chunks in diff:
        for chunk in chunks:
            orig = chunk.get("orig", [])
            recomp = chunk.get("recomp", [])
            if not orig and not recomp:
                continue

            if len(orig) == len(recomp):
                if all(is_equivalent_insn(insn_text(o), insn_text(r)) for o, r in zip(orig, recomp)):
                    continue

            for item in orig:
                lines.append(f"- {insn_text(item)}")
            for item in recomp:
                lines.append(f"+ {insn_text(item)}")
    return "\n".join(lines)


def run_reccmp(json_path: Path) -> None:
    cmd = [str(RECCMP), "--target", "LEMBALL", "--json", str(json_path.name), "--silent"]
    proc = subprocess.run(cmd, cwd=BUILD, capture_output=True, text=True)
    if proc.returncode != 0 and not json_path.exists():
        sys.stderr.write(proc.stderr or proc.stdout)
        raise RuntimeError(f"reccmp exited with code {proc.returncode}")


def load_matches(json_path: Path) -> dict[int, dict]:
    with json_path.open(encoding="utf-8") as f:
        data = json.load(f)["data"]
    return {
        int(x["address"], 16): x
        for x in data
        if x.get("type") in (None, 1)
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("addrs", nargs="*", help="Addresses (e.g. 0x0045ca30)")
    parser.add_argument("--diff", action="store_true", help="Print diff for non-matching functions")
    parser.add_argument("--json", type=Path, default=DEFAULT_JSON)
    parser.add_argument("--no-build", action="store_true", help="Skip incremental build before check")
    parser.add_argument("--clean-first", action="store_true", help="Clean build before check")
    parser.add_argument("--no-reccmp", action="store_true", help="Do not run reccmp; reuse existing JSON")
    args = parser.parse_args()

    if not args.no_build:
        exit_code = run_build(clean_first=args.clean_first)
        if exit_code != 0:
            print(f"BUILD_FAILED exit={exit_code} (see build-msvc400/last_build.log)")
            return exit_code

    if not args.addrs:
        return 0

    if not args.no_reccmp or not args.json.exists():
        run_reccmp(args.json)

    matches = load_matches(args.json)

    for raw in args.addrs:
        addr = norm_addr(raw)
        m = matches.get(addr)
        if not m:
            print(f"0x{addr:08x}: NOT_FOUND")
            continue

        name = m.get("name") or "?"
        ratio, tag = compute_ratio(m, name)

        if tag:
            print(f"0x{addr:08x} {name}: {ratio:.2f}% {tag}")
        else:
            print(f"0x{addr:08x} {name}: {ratio:.2f}%")
            if args.diff:
                diff_text = format_diff_text(m.get("diff"))
                if diff_text:
                    print("--- diff ---")
                    print(diff_text)
                    print("------------")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
