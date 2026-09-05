#!/usr/bin/env python3
"""Build LEMBALL and report reccmp matches, accounting for linker-label differences."""

from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

from reccmp.compare.asm.fixes import find_effective_match
from reccmp.compare.pinned_sequences import SequenceMatcherWithPins

from build import run_build

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build-msvc400"
DEFAULT_JSON = BUILD / "scores.json"
RECCMP = ROOT / ".decomp-venv" / "Scripts" / "reccmp-reccmp.exe"
RELOCATION = re.compile(r"<OFFSET\d+>")
ANNOTATED_SYMBOL = r".+? \((?:DATA|VTABLE|UNK|FUNCTION|IMPORT|IMPORT_THUNK|STRING)\)"
BYTE_REGISTER_RE = re.compile(r"\b(?:ah|al|bh|bl|ch|cl|dh|dl)\b")


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
    # Generated MSVC names contain apostrophes themselves (for example,
    # ``Thing::`scalar deleting destructor'``), so the wrapper quotes must be
    # matched greedily rather than stopping at the first apostrophe.
    s = re.sub(r"Thunk of '(.+)' \(THUNK\)$", r"\1 (FUNCTION)", s)
    s = re.sub(r" \(THUNK\)", " (FUNCTION)", s)
    return s


def is_unresolved_symbol(orig_text: str, recomp_text: str) -> bool:
    """Match one relocation to one named symbol."""
    orig_text = normalize_asm(orig_text)
    recomp_text = normalize_asm(recomp_text)
    parts = RELOCATION.split(orig_text)
    if len(parts) == 1:
        return False
    pattern = "^" + ANNOTATED_SYMBOL.join(re.escape(part) for part in parts) + "$"
    return bool(re.match(pattern, recomp_text))


def is_unresolved_call(orig_text: str, recomp_text: str) -> bool:
    orig_text = orig_text.split("\t")[0].strip()
    recomp_text = recomp_text.split("\t")[0].strip()
    if not re.match(r"call <OFFSET\d+>$", orig_text):
        return False
    return bool(re.match(r"call (?:Thunk of '.+' \(THUNK\)|.+ \(FUNCTION\))$", recomp_text))


def is_unresolved_jmp(orig_text: str, recomp_text: str) -> bool:
    """Match an unresolved tail jump to a thunk."""
    orig_text = orig_text.split("\t")[0].strip()
    recomp_text = recomp_text.split("\t")[0].strip()
    if not re.match(r"jmp -?0x[0-9a-f]+\s*$", orig_text):
        return False
    return bool(re.match(r"jmp Thunk of '.+' \(THUNK\)$", recomp_text))


def is_recomp_offset_call(orig_text: str, recomp_text: str) -> bool:
    orig_text = orig_text.split("\t")[0].strip()
    recomp_text = recomp_text.split("\t")[0].strip()
    if not re.match(r"call <OFFSET\d+>", recomp_text):
        return False
    return bool(re.match(r"call .+ \(FUNCTION\)$", orig_text))


def split_vtable_reference(instruction: str) -> tuple[str, str, bool] | None:
    """Return instruction shape, concrete class, and decorated-name status."""
    instruction = normalize_asm(instruction)
    suffix = " (VTABLE)"
    if not instruction.endswith(suffix):
        return None

    body = instruction[: -len(suffix)]
    head, separator, symbol = body.rpartition(", ")
    if not separator:
        head, separator, symbol = body.partition(" ")
    if not separator or not head or not symbol:
        return None

    decorated = re.fullmatch(r"(.+?)::`vftable'(?:\{for `.+?'\})?", symbol)
    if decorated:
        return head, decorated.group(1), True
    if "`vftable'" in symbol:
        return None
    return head, symbol, False


def is_vtable_display_alias(orig_text: str, recomp_text: str) -> bool:
    """Accept a collapsed PDB vtable name against its decorated path name."""
    orig = split_vtable_reference(orig_text)
    recomp = split_vtable_reference(recomp_text)
    if orig is None or recomp is None:
        return False
    orig_head, orig_class, orig_decorated = orig
    recomp_head, recomp_class, recomp_decorated = recomp
    return (
        orig_head == recomp_head
        and orig_class == recomp_class
        and orig_decorated != recomp_decorated
    )


def is_equivalent_insn(orig_text: str, recomp_text: str) -> bool:
    return (
        normalize_asm(orig_text) == normalize_asm(recomp_text)
        or is_vtable_display_alias(orig_text, recomp_text)
        or is_unresolved_symbol(orig_text, recomp_text)
        or is_unresolved_symbol(recomp_text, orig_text)
        or is_unresolved_call(orig_text, recomp_text)
        or is_unresolved_jmp(orig_text, recomp_text)
        or is_recomp_offset_call(orig_text, recomp_text)
    )


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
    return all(
        is_equivalent_insn(orig_text, recomp_text)
        for orig_text, recomp_text in zip(orig_insns, recomp_insns)
    )


def byte_register_swaps_consistent(orig_asm: list[str], recomp_asm: list[str]) -> bool:
    """Check byte-register substitutions for consistency."""
    forward: dict[str, str] = {}
    reverse: dict[str, str] = {}
    for orig_text, recomp_text in zip(orig_asm, recomp_asm):
        orig_regs = BYTE_REGISTER_RE.findall(orig_text)
        recomp_regs = BYTE_REGISTER_RE.findall(recomp_text)
        if not orig_regs and not recomp_regs:
            continue
        if len(orig_regs) != len(recomp_regs):
            continue
        if BYTE_REGISTER_RE.sub("REG8", orig_text) != BYTE_REGISTER_RE.sub("REG8", recomp_text):
            continue

        for orig_reg, recomp_reg in zip(orig_regs, recomp_regs):
            if forward.get(orig_reg, recomp_reg) != recomp_reg:
                return False
            if reverse.get(recomp_reg, orig_reg) != orig_reg:
                return False
            forward[orig_reg] = recomp_reg
            reverse[recomp_reg] = orig_reg
    return True


def group_asm(chunks) -> tuple[list[str], list[str]]:
    """Build aligned original and rebuilt instruction lists."""
    orig_asm: list[str] = []
    recomp_asm: list[str] = []

    for chunk in chunks:
        both = chunk.get("both", []) or []
        if both:
            for entry in both:
                instruction = normalize_asm(insn_text(entry))
                orig_asm.append(instruction)
                recomp_asm.append(instruction)
            continue

        orig_raw = [insn_text(entry) for entry in chunk.get("orig", []) or []]
        recomp_raw = [insn_text(entry) for entry in chunk.get("recomp", []) or []]
        orig_chunk = [normalize_asm(instruction) for instruction in orig_raw]
        recomp_chunk = [normalize_asm(instruction) for instruction in recomp_raw]

        if len(orig_chunk) == len(recomp_chunk):
            for index, (orig_text, recomp_text) in enumerate(zip(orig_raw, recomp_raw)):
                if is_equivalent_insn(orig_text, recomp_text):
                    recomp_chunk[index] = orig_chunk[index]

        orig_asm.extend(orig_chunk)
        recomp_asm.extend(recomp_chunk)

    return orig_asm, recomp_asm


def is_codegen_equivalent_diff(diff) -> bool:
    """Whether all remaining differences are accepted compiler entropy."""
    if not diff:
        return False

    saw_difference = False
    for _, chunks in diff:
        orig_asm, recomp_asm = group_asm(chunks)
        if not orig_asm and not recomp_asm:
            continue

        saw_difference = True
        if not byte_register_swaps_consistent(orig_asm, recomp_asm):
            return False
        matcher = SequenceMatcherWithPins(orig_asm, recomp_asm, [])
        if matcher.ratio() == 1.0:
            continue

        if not find_effective_match(matcher.get_opcodes(), orig_asm, recomp_asm):
            return False

    return saw_difference


def compute_ratio(match: dict | None) -> tuple[float, str]:
    """Use reccmp's score plus general linker-label/compiler equivalence only."""
    if match is None or match.get("stub"):
        return 0.0, "STUB"
    ratio = float(match.get("matching", 0.0)) * 100.0
    if match.get("effective") or ratio == 100.0:
        return 100.0, "MATCH"
    if is_thunk_only_diff(match.get("diff")):
        return 100.0, "MATCH (thunk)"
    if is_codegen_equivalent_diff(match.get("diff")):
        return 100.0, "MATCH (compiler entropy)"
    return ratio, ""


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

            for item in orig:
                lines.append(f"- {insn_text(item)}")
            for item in recomp:
                lines.append(f"+ {insn_text(item)}")
    return "\n".join(lines)


def run_reccmp(json_path: Path) -> None:
    json_path = json_path.resolve()
    json_path.unlink(missing_ok=True)
    cmd = [str(RECCMP), "--target", "LEMBALL", "--json", str(json_path), "--silent"]
    proc = subprocess.run(cmd, cwd=BUILD, capture_output=True, text=True)
    if proc.returncode != 0:
        sys.stderr.write(proc.stderr or proc.stdout)
        raise RuntimeError(f"reccmp exited with code {proc.returncode}")
    if not json_path.exists():
        raise RuntimeError(f"reccmp produced no JSON: {json_path}")


def load_matches(json_path: Path) -> dict[int, dict]:
    with json_path.open(encoding="utf-8") as f:
        data = json.load(f)["data"]
    return {
        norm_addr(match["address"]): match
        for match in data if match.get("type") in (None, 1)
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("addrs", nargs="*", help="Addresses (e.g. 0x0045ca30)")
    parser.add_argument("--diff", action="store_true", help="Print differences, including effective matches")
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
        ratio, tag = compute_ratio(m)

        suffix = f" {tag}" if tag else ""
        print(f"0x{addr:08x} {name}: {ratio:.2f}%{suffix}")
        if args.diff:
            diff_text = format_diff_text(m.get("diff"))
            if diff_text:
                print("--- diff ---")
                print(diff_text)
                print("------------")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
