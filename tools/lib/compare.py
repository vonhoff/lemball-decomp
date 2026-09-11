"""Effective-match helpers and PE jump-stub following."""

from __future__ import annotations

import json
import re
import struct
from pathlib import Path

from reccmp.compare.asm.fixes import find_effective_match
from reccmp.compare.pinned_sequences import SequenceMatcherWithPins

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


def asm_head(s: str) -> str:
    return s.split("\t")[0].strip()


def normalize_asm(s: str) -> str:
    s = asm_head(s)
    # Generated MSVC names contain apostrophes themselves (for example,
    # ``Thing::`scalar deleting destructor'``), so the wrapper quotes must be
    # matched greedily rather than stopping at the first apostrophe.
    s = re.sub(r"Thunk of '(.+)' \(THUNK\)$", r"\1 (FUNCTION)", s)
    s = re.sub(r" \(THUNK\)", " (FUNCTION)", s)
    return s


def is_unresolved_symbol(orig_text: str, recomp_text: str) -> bool:
    orig_text = normalize_asm(orig_text)
    recomp_text = normalize_asm(recomp_text)
    parts = RELOCATION.split(orig_text)
    if len(parts) == 1:
        return False
    pattern = "^" + ANNOTATED_SYMBOL.join(re.escape(part) for part in parts) + "$"
    return bool(re.match(pattern, recomp_text))


def is_unresolved_call(orig_text: str, recomp_text: str) -> bool:
    orig_text = asm_head(orig_text)
    recomp_text = asm_head(recomp_text)
    if not re.match(r"call <OFFSET\d+>$", orig_text):
        return False
    return bool(re.match(r"call (?:Thunk of '.+' \(THUNK\)|.+ \(FUNCTION\))$", recomp_text))


def is_unresolved_jmp(orig_text: str, recomp_text: str) -> bool:
    orig_text = asm_head(orig_text)
    recomp_text = asm_head(recomp_text)
    if not re.match(r"jmp -?0x[0-9a-f]+\s*$", orig_text):
        return False
    return bool(re.match(r"jmp (?:Thunk of '.+' \(THUNK\)|.+ \(FUNCTION\))$", recomp_text))


def is_recomp_offset_call(orig_text: str, recomp_text: str) -> bool:
    orig_text = asm_head(orig_text)
    recomp_text = asm_head(recomp_text)
    if not re.match(r"call <OFFSET\d+>", recomp_text):
        return False
    return bool(re.match(r"call .+ \(FUNCTION\)$", orig_text))


def split_vtable_reference(instruction: str) -> tuple[str, str, bool] | None:
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


def load_matches(json_path: Path) -> dict[int, dict]:
    with json_path.open(encoding="utf-8") as f:
        data = json.load(f)["data"]
    return {
        norm_addr(match["address"]): match
        for match in data if match.get("type") in (None, 1)
    }


def resolve_jump(image, address: int | None, stop_at=None, max_depth: int = 16) -> int | None:
    """Follow E9 rel32 jumps until a non-jump or stop_at predicate."""
    if address is None:
        return None
    seen: set[int] = set()
    current = address
    for _ in range(max_depth):
        if current in seen:
            break
        seen.add(current)
        if stop_at is not None and stop_at(current):
            break
        if not image.is_valid_vaddr(current):
            break
        try:
            instruction = image.read(current, 5)
        except (IndexError, ValueError):
            break
        if len(instruction) != 5 or instruction[0] != 0xE9:
            break
        displacement = struct.unpack("<i", instruction[1:])[0]
        destination = current + 5 + displacement
        if not image.is_valid_vaddr(destination):
            break
        current = destination
    return current


def resolve_original_target(image, address: int, decoder, max_depth: int = 32) -> int | None:
    """Follow Capstone-decoded absolute jmp targets."""
    seen: set[int] = set()
    while address not in seen and len(seen) < max_depth:
        seen.add(address)
        try:
            ins = next(decoder.disasm_lite(image.read(address, 15), address), None)
        except (ValueError, IndexError):
            return None
        if ins is None:
            return None
        if ins[2] != "jmp" or not re.fullmatch(r"0x[0-9a-f]+", ins[3]):
            return address
        address = int(ins[3], 16)
    return None
