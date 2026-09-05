#!/usr/bin/env python3
"""Central decompilation match evaluation logic and CLI."""

from __future__ import annotations

import argparse
import copy
import json
import re
import subprocess
import sys
from functools import cache
from pathlib import Path

from reccmp.compare.asm.fixes import find_effective_match
from reccmp.compare.asm.instgen import get_disassembler
from reccmp.compare.pinned_sequences import SequenceMatcherWithPins
from reccmp.cvdump import Cvdump
from reccmp.formats import detect_image

from build import run_build

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build-msvc400"
DEFAULT_JSON = BUILD / "scores.json"
RECCMP = ROOT / ".decomp-venv" / "Scripts" / "reccmp-reccmp.exe"
ORIGINAL_EXE = ROOT / "data" / "LEMBALL.EXE"
RECOMPILED_EXE = BUILD / "LEMBALL.EXE"
RECOMPILED_PDB = BUILD / "LEMBALL.PDB"
STRICT_EQUIVALENCE_TARGETS = {
    0x0043CFA0: "C2D::DrawDuplicator",
    0x0043D0F0: "C2D::DrawTimeBonus",
    0x0043D370: "C2D::DrawSheep",
    0x0043D420: "C2D::DrawBall",
    0x0043D4B0: "C2D::DrawKey",
    0x0043D7E0: "C2D::DrawSwitch",
    0x0043D8D0: "C2D::DrawFlag",
    0x0043D950: "C2D::DrawBonus",
    0x004621E0: "BaseNetwork::CtoSRequestConnect",
}
STRICT_EQUIVALENCE_NAMES = frozenset(STRICT_EQUIVALENCE_TARGETS.values())
RELOCATION = re.compile(r"<OFFSET\d+>")
ANNOTATED_SYMBOL = r".+? \((?:DATA|VTABLE|UNK|FUNCTION|IMPORT|IMPORT_THUNK|STRING)\)"
REGISTER = r"(?:eax|ebx|ecx|edx|esi|edi|ebp|esp|ax|bx|cx|dx|si|di|bp|sp)"
DWORD_REGISTERS = {"eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp"}
DWORD_REGISTER_FAMILIES = {
    "eax": {"eax", "ax", "ah", "al"},
    "ebx": {"ebx", "bx", "bh", "bl"},
    "ecx": {"ecx", "cx", "ch", "cl"},
    "edx": {"edx", "dx", "dh", "dl"},
    "esi": {"esi", "si"},
    "edi": {"edi", "di"},
    "ebp": {"ebp", "bp"},
    "esp": {"esp", "sp"},
}
BYTE_REGISTER_RE = re.compile(r"\b(?:ah|al|bh|bl|ch|cl|dh|dl)\b")
REGISTER_RE = re.compile(
    r"\b(?:eax|ebx|ecx|edx|esi|edi|ebp|esp|ax|bx|cx|dx|si|di|bp|sp|ah|al|bh|bl|ch|cl|dh|dl)\b"
)
REGISTER_WIDTH = {
    **{register: 4 for register in ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp")},
    **{register: 2 for register in ("ax", "bx", "cx", "dx", "si", "di", "bp", "sp")},
    **{register: 1 for register in ("ah", "al", "bh", "bl", "ch", "cl", "dh", "dl")},
}
STACK_REFERENCE_RE = re.compile(r"\[esp(?: \+ (0x[0-9a-f]+|[0-9]+))?\]")
WRITES_FIRST_OPERAND = {
    "adc",
    "add",
    "and",
    "dec",
    "inc",
    "lea",
    "mov",
    "movsx",
    "movzx",
    "neg",
    "not",
    "or",
    "pop",
    "rol",
    "ror",
    "sar",
    "sbb",
    "shl",
    "shr",
    "sub",
    "xor",
}


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
    """Match one unresolved relocation against a specifically named symbol.

    This is intentionally pairwise. Replacing every symbol with one generic
    token would also hide real differences such as reads from two distinct
    globals.
    """
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
    """Unresolved original tail-jmp through the incremental-link jump table.

    The original binary jmps into its jump table (Ghidra renders the target as a
    raw relative offset); the recomp resolves the callee through its own table,
    which reccmp prints as a thunk.
    """
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
    if normalize_asm(orig_text) == normalize_asm(recomp_text):
        return True
    if is_vtable_display_alias(orig_text, recomp_text):
        return True
    if is_unresolved_symbol(orig_text, recomp_text):
        return True
    if is_unresolved_symbol(recomp_text, orig_text):
        return True
    if is_unresolved_call(orig_text, recomp_text):
        return True
    if is_unresolved_jmp(orig_text, recomp_text):
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


def split_instruction(instruction: str) -> tuple[str, list[str]]:
    mnemonic, _, operand_text = instruction.partition(" ")
    operands = [operand.strip() for operand in operand_text.split(", ")] if operand_text else []
    return mnemonic, operands


def register_is_known_zero(asm: list[str], before: int, register: str) -> bool:
    """Conservatively trace a full register back to an explicit zeroing idiom."""
    register_re = re.compile(r"\b" + re.escape(register) + r"\b")
    caller_saved = {"eax", "ecx", "edx", "ax", "cx", "dx"}

    for instruction in reversed(asm[:before]):
        mnemonic, operands = split_instruction(instruction)
        if mnemonic.startswith("j") or mnemonic in ("loop", "loope", "loopne", "ret", "retn"):
            return False
        if mnemonic == "call" and register in caller_saved:
            return False
        if not register_re.search(instruction):
            continue
        if mnemonic in ("sub", "xor") and operands == [register, register]:
            return True
        if mnemonic in WRITES_FIRST_OPERAND and operands and operands[0] == register:
            return False
        if mnemonic not in ("cmp", "test", "push"):
            return False

    return False


def zero_compare_equivalent(
    cmp_text: str, test_text: str, cmp_asm: list[str], cmp_index: int
) -> bool:
    cmp_mnemonic, cmp_operands = split_instruction(cmp_text)
    test_mnemonic, test_operands = split_instruction(test_text)
    if cmp_mnemonic != "cmp" or test_mnemonic != "test":
        return False
    if len(cmp_operands) != 2 or len(test_operands) != 2:
        return False
    value, zero_register = cmp_operands
    if not re.fullmatch(REGISTER, zero_register):
        return False
    if test_operands != [value, value]:
        return False
    return register_is_known_zero(cmp_asm, cmp_index, zero_register)


def normalize_zero_comparisons(orig_asm: list[str], recomp_asm: list[str]) -> None:
    """Make proven ``cmp value, zero-register`` / ``test value, value`` pairs equal."""
    if len(orig_asm) != len(recomp_asm):
        return
    for index, (orig_text, recomp_text) in enumerate(zip(orig_asm, recomp_asm)):
        if zero_compare_equivalent(orig_text, recomp_text, orig_asm, index):
            recomp_asm[index] = orig_text
        elif zero_compare_equivalent(recomp_text, orig_text, recomp_asm, index):
            recomp_asm[index] = orig_text


def normalize_swapped_comparison_branches(
    orig_asm: list[str], recomp_asm: list[str]
) -> None:
    """Normalize a nearby compare whose operands and condition are inverted.

    Swapping the two operands of ``cmp`` reverses the ordered condition.  Only
    accept the exact inverse signed/unsigned branch with the same destination;
    at most two identical, explicitly flag-neutral instructions may separate
    the comparison from its flag consumer.
    """
    if len(orig_asm) != len(recomp_asm):
        return

    inverted_condition = {
        "ja": "jb",
        "jae": "jbe",
        "jb": "ja",
        "jbe": "jae",
        "jg": "jl",
        "jge": "jle",
        "jl": "jg",
        "jle": "jge",
    }
    flag_neutral = {"lea", "mov", "movsx", "movzx", "nop"}
    for index in range(len(orig_asm) - 1):
        orig_cmp, orig_operands = split_instruction(orig_asm[index])
        recomp_cmp, recomp_operands = split_instruction(recomp_asm[index])
        if (
            orig_cmp != "cmp"
            or recomp_cmp != "cmp"
            or len(orig_operands) != 2
            or recomp_operands != list(reversed(orig_operands))
            or orig_operands[0] == orig_operands[1]
        ):
            continue

        for branch_index in range(index + 1, min(index + 4, len(orig_asm))):
            intervening = range(index + 1, branch_index)
            if any(
                orig_asm[middle] != recomp_asm[middle]
                or split_instruction(orig_asm[middle])[0] not in flag_neutral
                for middle in intervening
            ):
                break

            orig_branch, orig_target = split_instruction(orig_asm[branch_index])
            recomp_branch, recomp_target = split_instruction(recomp_asm[branch_index])
            if (
                inverted_condition.get(orig_branch) != recomp_branch
                or len(orig_target) != 1
                or recomp_target != orig_target
            ):
                continue

            recomp_asm[index] = orig_asm[index]
            recomp_asm[branch_index] = orig_asm[branch_index]
            break


def normalize_copy_tests(asm: list[str]) -> None:
    """Test the source register when a preceding copy proves it equivalent.

    This lets reccmp's relocation check recognize codegen such as
    ``mov edi, edx; test edi, edi`` versus an earlier ``test edx, edx``.
    Restricting this to an adjacent full-register copy makes the value
    equivalence independent of liveness or alias assumptions.
    """
    for index in range(1, len(asm)):
        test_mnemonic, test_operands = split_instruction(asm[index])
        if test_mnemonic != "test" or len(test_operands) != 2:
            continue
        tested_register = test_operands[0]
        if test_operands[1] != tested_register or tested_register not in DWORD_REGISTERS:
            continue

        mov_mnemonic, mov_operands = split_instruction(asm[index - 1])
        if (
            mov_mnemonic == "mov"
            and len(mov_operands) == 2
            and mov_operands[0] == tested_register
            and mov_operands[1] in DWORD_REGISTERS
        ):
            source_register = mov_operands[1]
            asm[index] = f"test {source_register}, {source_register}"


def normalize_transient_call_argument_registers(orig_asm: list[str], recomp_asm: list[str]) -> None:
    """Canonicalize a caller-saved temporary whose lifetime ends at a call.

    MSVC 4 may choose a different register for the compact sequence
    ``mov reg, source; push reg; mov destination, reg``.  When both streams
    agree on the source and destination and immediately call the same target,
    the call bounds the lifetime of EAX/ECX/EDX and the register choice cannot
    affect subsequent code.
    """
    if len(orig_asm) != len(recomp_asm):
        return

    caller_saved = {"eax", "ecx", "edx"}
    for index in range(len(orig_asm) - 3):
        orig_load, orig_push, orig_store, orig_call = orig_asm[index : index + 4]
        recomp_load, recomp_push, recomp_store, recomp_call = recomp_asm[index : index + 4]

        orig_mnemonic, orig_load_operands = split_instruction(orig_load)
        recomp_mnemonic, recomp_load_operands = split_instruction(recomp_load)
        if (
            orig_mnemonic != "mov"
            or recomp_mnemonic != "mov"
            or len(orig_load_operands) != 2
            or len(recomp_load_operands) != 2
        ):
            continue

        orig_register, orig_source = orig_load_operands
        recomp_register, recomp_source = recomp_load_operands
        role_register_re = re.compile(
            rf"\b(?:{re.escape(orig_register)}|{re.escape(recomp_register)})\b"
        )
        if (
            orig_register == recomp_register
            or orig_register not in caller_saved
            or recomp_register not in caller_saved
            or orig_source != recomp_source
            or role_register_re.search(orig_source)
        ):
            continue
        if orig_push != f"push {orig_register}" or recomp_push != f"push {recomp_register}":
            continue

        orig_store_mnemonic, orig_store_operands = split_instruction(orig_store)
        recomp_store_mnemonic, recomp_store_operands = split_instruction(recomp_store)
        if (
            orig_store_mnemonic != "mov"
            or recomp_store_mnemonic != "mov"
            or len(orig_store_operands) != 2
            or len(recomp_store_operands) != 2
            or orig_store_operands[0] != recomp_store_operands[0]
            or role_register_re.search(orig_store_operands[0])
            or orig_store_operands[1] != orig_register
            or recomp_store_operands[1] != recomp_register
            or orig_call != recomp_call
            or not orig_call.startswith("call ")
        ):
            continue

        recomp_asm[index : index + 3] = orig_asm[index : index + 3]


def normalize_multiply_copy_zero(asm: list[str]) -> None:
    """Canonicalize two equivalent ways to prepare a dividend register pair.

    With three distinct full registers, these sequences leave identical
    register values at the division:

    ``mov A, B; sub B, B; imul A, C; div D``
    ``imul B, C; mov A, B; sub B, B; div D``

    Requiring the immediately following division is important: the two forms
    do not leave identical status flags, but ``div``/``idiv`` does not consume
    them and leaves its own flags undefined.
    """
    for index in range(len(asm) - 3):
        mov_mnemonic, mov_operands = split_instruction(asm[index])
        sub_mnemonic, sub_operands = split_instruction(asm[index + 1])
        imul_mnemonic, imul_operands = split_instruction(asm[index + 2])
        div_mnemonic, div_operands = split_instruction(asm[index + 3])
        if (
            mov_mnemonic != "mov"
            or sub_mnemonic != "sub"
            or imul_mnemonic != "imul"
            or div_mnemonic not in ("div", "idiv")
            or len(mov_operands) != 2
            or len(sub_operands) != 2
            or len(imul_operands) != 2
            or len(div_operands) != 1
        ):
            continue

        product_register, value_register = mov_operands
        multiplier = imul_operands[1]
        if (
            product_register not in DWORD_REGISTERS
            or value_register not in DWORD_REGISTERS
            or multiplier not in DWORD_REGISTERS
            or len({product_register, value_register, multiplier}) != 3
            or sub_operands != [value_register, value_register]
            or imul_operands[0] != product_register
        ):
            continue

        asm[index : index + 3] = [
            f"imul {value_register}, {multiplier}",
            f"mov {product_register}, {value_register}",
            f"sub {value_register}, {value_register}",
        ]


def normalize_divide_scratch_scheduling(orig_asm: list[str], recomp_asm: list[str]) -> None:
    """Align one proven MSVC scratch-register schedule around unsigned division.

    The original may clear EDX before loading EAX, use a soon-overwritten
    constant register as the multiply scratch, and initialize that constant
    just before DIV. The rebuilt stream may initialize the constant earlier,
    use EDX as the scratch, then clear EDX immediately before DIV. Accept the
    reshuffle only when the complete candidate becomes instruction-identical.
    """
    if len(orig_asm) != len(recomp_asm):
        return

    for div_index, div_text in enumerate(orig_asm):
        if div_index < 7 or recomp_asm[div_index] != div_text:
            continue
        div_mnemonic, div_operands = split_instruction(div_text)
        if (
            div_mnemonic != "div"
            or len(div_operands) != 1
            or div_operands[0] not in DWORD_REGISTERS
        ):
            continue

        const_mnemonic, const_operands = split_instruction(orig_asm[div_index - 1])
        if (
            const_mnemonic != "mov"
            or len(const_operands) != 2
            or const_operands[0] not in DWORD_REGISTERS
            or const_operands[0] in ("eax", "edx", div_operands[0])
        ):
            continue
        constant_register, constant_value = const_operands
        try:
            int(constant_value, 0)
        except ValueError:
            continue

        zero_text = "sub edx, edx"
        if (
            orig_asm[div_index - 6] != zero_text
            or recomp_asm[div_index - 1] != zero_text
            or orig_asm.count(zero_text) != 1
            or recomp_asm.count(zero_text) != 1
        ):
            continue
        if (
            orig_asm[div_index - 5] != recomp_asm[div_index - 5]
            or orig_asm[div_index - 4] != recomp_asm[div_index - 4]
        ):
            continue
        load_mnemonic, load_operands = split_instruction(orig_asm[div_index - 5])
        sub_mnemonic, sub_operands = split_instruction(orig_asm[div_index - 4])
        if (
            load_mnemonic != "mov"
            or len(load_operands) != 2
            or load_operands[0] != "eax"
            or sub_mnemonic != "sub"
            or len(sub_operands) != 2
            or sub_operands[0] != "eax"
        ):
            continue

        if (
            orig_asm[div_index - 3]
            != f"lea {constant_register}, [eax + eax*2]"
            or orig_asm[div_index - 2]
            != f"lea eax, [{constant_register} + {constant_register}*4]"
            or recomp_asm[div_index - 3] != "lea edx, [eax + eax*2]"
            or recomp_asm[div_index - 2] != "lea eax, [edx + edx*4]"
        ):
            continue

        constant_text = orig_asm[div_index - 1]
        constant_indices = [
            index
            for index, instruction in enumerate(recomp_asm[: div_index - 5])
            if instruction == constant_text
        ]
        if len(constant_indices) != 1:
            continue
        constant_index = constant_indices[0]
        crossed = recomp_asm[constant_index + 1 : div_index - 5]
        safe = True
        for instruction in crossed:
            mnemonic, operands = split_instruction(instruction)
            if (
                mnemonic not in ("lea", "mov")
                or len(operands) != 2
                or operands[0] not in DWORD_REGISTERS
                or constant_register in REGISTER_RE.findall(instruction)
            ):
                safe = False
                break
        if not safe:
            continue
        if div_index + 1 < len(orig_asm):
            next_mnemonic, _ = split_instruction(orig_asm[div_index + 1])
            if next_mnemonic not in ("cmp", "test"):
                continue

        candidate = list(recomp_asm)
        candidate.pop(div_index - 1)
        candidate.pop(constant_index)
        candidate.insert(div_index - 6, zero_text)
        candidate.insert(div_index - 1, constant_text)
        candidate[div_index - 3] = orig_asm[div_index - 3]
        candidate[div_index - 2] = orig_asm[div_index - 2]
        if candidate == orig_asm:
            recomp_asm[:] = candidate
            return


def normalize_interleaved_stack_offset_chains(
    orig_asm: list[str], recomp_asm: list[str]
) -> None:
    """Align one sequential/interleaved pair of stack-offset calculations.

    The rebuilt schedule may calculate ``(source - offset) << n`` and
    ``(source + offset) << n`` in parallel registers before storing them,
    while the original calculates and stores each result sequentially.  The
    rewrite is limited to distinct dword stack slots, an otherwise-dead
    caller-saved scratch, and a primary result overwritten before any branch
    or use.  Both schedules therefore retain identical memory, flags, and
    observable register state.

    EDX is the only evidenced interleaved scratch.  DIV, IDIV, and CMPXCHG8B
    read it implicitly and therefore invalidate the textual liveness proof.
    Ordinary MSVC 4 cdecl/stdcall/thiscall calls do not take EDX arguments;
    the sole current match was additionally audited to use four
    ``ClipCirclePoint`` calls and one ``DrawClippedCirclePoint`` call with
    stack arguments and ECX as ``this``.
    """
    if len(orig_asm) != len(recomp_asm) or len(orig_asm) < 11:
        return
    if (
        orig_asm[-1] != recomp_asm[-1]
        or split_instruction(orig_asm[-1])[0] not in ("ret", "retn")
    ):
        return

    def stack_offset(operand: str) -> int | None:
        match = re.fullmatch(
            r"dword ptr \[esp(?: \+ (0x[0-9a-f]+|[0-9]+))?\]", operand
        )
        return int(match.group(1), 0) if match and match.group(1) else (0 if match else None)

    def overwritten_before_control_flow(asm: list[str], start: int, register: str) -> bool:
        family = DWORD_REGISTER_FAMILIES[register]
        for instruction in asm[start:]:
            mnemonic, operands = split_instruction(instruction)
            if mnemonic.startswith("j") or mnemonic in ("call", "loop", "loope", "loopne", "ret", "retn"):
                return False
            if not family.intersection(REGISTER_RE.findall(instruction)):
                continue
            return (
                mnemonic == "mov"
                and len(operands) == 2
                and operands[0] == register
                and not family.intersection(REGISTER_RE.findall(operands[1]))
            )
        return False

    candidates = []
    for index in range(1, len(orig_asm) - 9):
        orig_parts = [split_instruction(text) for text in orig_asm[index : index + 8]]
        if [part[0] for part in orig_parts] != [
            "mov",
            "sub",
            "shl",
            "mov",
            "mov",
            "add",
            "shl",
            "mov",
        ] or any(len(part[1]) != 2 for part in orig_parts):
            continue

        primary, source = orig_parts[0][1]
        offset_register = orig_parts[1][1][1]
        shift_count = orig_parts[2][1][1]
        first_destination = orig_parts[3][1][0]
        second_destination = orig_parts[7][1][0]
        recomp_load, recomp_load_operands = split_instruction(recomp_asm[index + 1])
        if recomp_load != "mov" or len(recomp_load_operands) != 2:
            continue
        transient = recomp_load_operands[0]

        if (
            transient != "edx"
            or primary not in DWORD_REGISTERS
            or offset_register not in DWORD_REGISTERS
            or "esp" in (primary, transient, offset_register)
            or len({primary, transient, offset_register}) != 3
        ):
            continue
        try:
            shift_value = int(shift_count, 0)
        except ValueError:
            continue
        if not 1 <= shift_value <= 31:
            continue

        expected_orig = [
            f"mov {primary}, {source}",
            f"sub {primary}, {offset_register}",
            f"shl {primary}, {shift_count}",
            f"mov {first_destination}, {primary}",
            f"mov {primary}, {source}",
            f"add {primary}, {offset_register}",
            f"shl {primary}, {shift_count}",
            f"mov {second_destination}, {primary}",
        ]
        expected_recomp = [
            f"mov {primary}, {source}",
            f"mov {transient}, {source}",
            f"sub {primary}, {offset_register}",
            f"add {transient}, {offset_register}",
            f"shl {primary}, {shift_count}",
            f"shl {transient}, {shift_count}",
            f"mov {first_destination}, {primary}",
            f"mov {second_destination}, {transient}",
        ]
        if (
            orig_asm[index : index + 8] != expected_orig
            or recomp_asm[index : index + 8] != expected_recomp
            or orig_asm[index - 1] != recomp_asm[index - 1]
            or orig_asm[index + 8] != recomp_asm[index + 8]
        ):
            continue

        slots = [
            stack_offset(source),
            stack_offset(first_destination),
            stack_offset(second_destination),
        ]
        if any(slot is None for slot in slots) or any(
            abs(slots[left] - slots[right]) < 4
            for left in range(3)
            for right in range(left + 1, 3)
        ):
            continue

        transient_family = DWORD_REGISTER_FAMILIES[transient]
        suffix = orig_asm[index + 8 :] + recomp_asm[index + 8 :]
        if any(transient_family.intersection(REGISTER_RE.findall(text)) for text in suffix):
            continue
        if any(split_instruction(text)[0] in ("div", "idiv", "cmpxchg8b") for text in suffix):
            continue
        if not overwritten_before_control_flow(orig_asm, index + 8, primary):
            continue
        if not overwritten_before_control_flow(recomp_asm, index + 8, primary):
            continue
        candidates.append(index)

    if len(candidates) == 1:
        index = candidates[0]
        recomp_asm[index : index + 8] = orig_asm[index : index + 8]


def strip_trailing_alignment_nops(orig_asm: list[str], recomp_asm: list[str]) -> None:
    """Discard padding NOPs only when both instruction streams already returned."""
    orig_end = len(orig_asm)
    recomp_end = len(recomp_asm)
    while orig_end > 0 and orig_asm[orig_end - 1] == "nop":
        orig_end -= 1
    while recomp_end > 0 and recomp_asm[recomp_end - 1] == "nop":
        recomp_end -= 1

    if orig_end == len(orig_asm) and recomp_end == len(recomp_asm):
        return
    if orig_end == 0 or recomp_end == 0:
        return
    if not orig_asm[orig_end - 1].startswith("ret") or not recomp_asm[recomp_end - 1].startswith("ret"):
        return

    del orig_asm[orig_end:]
    del recomp_asm[recomp_end:]


def normalize_dead_stack_reservation(orig_asm: list[str], recomp_asm: list[str]) -> None:
    """Remove a clipped, unused original stack reservation.

    reccmp can clip the original balancing epilogue when the rebuilt function
    is shorter.  Accept this only when the two excerpts have equal instruction
    counts, the rebuilt excerpt ends in a return, and every original ESP-based
    access is outside the reserved area and shifts to the rebuilt offset by the
    exact reservation size.
    """
    if len(orig_asm) != len(recomp_asm) or not orig_asm or not recomp_asm:
        return
    match = re.fullmatch(r"sub esp, (0x[0-9a-f]+|[0-9]+)", orig_asm[0])
    if match is None or not recomp_asm[-1].startswith("ret"):
        return
    if orig_asm[-1].startswith("ret") or recomp_asm[0].startswith("sub esp,"):
        return
    reservation = int(match.group(1), 0)
    if reservation <= 0 or reservation % 4 != 0:
        return

    adjusted_orig = orig_asm[1:]
    adjusted_recomp = recomp_asm[:-1]
    for index, instruction in enumerate(adjusted_orig):
        invalid_reference = False

        def adjust_reference(reference: re.Match[str]) -> str:
            nonlocal invalid_reference
            offset_text = reference.group(1)
            offset = int(offset_text, 0) if offset_text is not None else 0
            if offset <= reservation:
                invalid_reference = True
                return reference.group(0)
            adjusted = offset - reservation
            return f"[esp + {hex(adjusted) if adjusted >= 10 else adjusted}]"

        adjusted_orig[index] = STACK_REFERENCE_RE.sub(adjust_reference, instruction)
        if invalid_reference:
            return

    orig_asm[:] = adjusted_orig
    recomp_asm[:] = adjusted_recomp


def byte_register_swaps_consistent(orig_asm: list[str], recomp_asm: list[str]) -> bool:
    """Reject contradictory byte-register substitutions in aligned instructions.

    reccmp's generic effective matcher deliberately erases register identity.
    That is useful for allocation differences, but could otherwise accept a
    definition moved from AL to CL while a later use incorrectly remains AL.
    Keeping a bijection for byte-register-only variants closes that hole while
    leaving unrelated instruction relocation to reccmp.
    """
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


def register_shape(instruction: str) -> str:
    """Erase register identity while retaining operand widths."""
    return REGISTER_RE.sub(
        lambda match: f"REG{REGISTER_WIDTH[match.group(0)]}", instruction
    )


def infer_register_role_map(orig_asm: list[str], recomp_asm: list[str]) -> dict[str, str]:
    """Infer a bijective recomp-to-original register map from aligned evidence.

    Comparison and control-flow instructions are deliberately excluded: their
    operands may be reversed for an equivalent condition, and reccmp already
    validates those swaps.  The resulting map is used only to expose an
    instruction relocation that reccmp can then check with its normal liveness
    safeguards.
    """
    recomp_to_orig: dict[str, str] = {}
    orig_to_recomp: dict[str, str] = {}
    excluded = {"call", "cmp", "pop", "push", "ret", "retn", "test"}

    for orig_text, recomp_text in zip(orig_asm, recomp_asm):
        if orig_text == recomp_text:
            continue
        orig_mnemonic, _ = split_instruction(orig_text)
        recomp_mnemonic, _ = split_instruction(recomp_text)
        if (
            orig_mnemonic != recomp_mnemonic
            or orig_mnemonic in excluded
            or orig_mnemonic.startswith("j")
            or register_shape(orig_text) != register_shape(recomp_text)
        ):
            continue

        orig_regs = REGISTER_RE.findall(orig_text)
        recomp_regs = REGISTER_RE.findall(recomp_text)
        if len(orig_regs) != len(recomp_regs):
            continue

        for orig_reg, recomp_reg in zip(orig_regs, recomp_regs):
            if orig_reg == recomp_reg:
                continue
            if orig_reg not in DWORD_REGISTERS or recomp_reg not in DWORD_REGISTERS:
                continue
            if "esp" in (orig_reg, recomp_reg):
                return {}
            if REGISTER_WIDTH[orig_reg] != REGISTER_WIDTH[recomp_reg]:
                return {}
            if recomp_to_orig.get(recomp_reg, orig_reg) != orig_reg:
                return {}
            if orig_to_recomp.get(orig_reg, recomp_reg) != recomp_reg:
                return {}
            recomp_to_orig[recomp_reg] = orig_reg
            orig_to_recomp[orig_reg] = recomp_reg

    return recomp_to_orig


def is_zero_idiom(instruction: str) -> bool:
    mnemonic, operands = split_instruction(instruction)
    return (
        mnemonic in ("sub", "xor")
        and len(operands) == 2
        and operands[0] == operands[1]
    )


def can_move_zero_across(instructions: list[str], register: str) -> bool:
    """Prove a zero idiom can cross a small independent instruction region."""
    index = 0
    register_re = re.compile(r"\b" + re.escape(register) + r"\b")
    while index < len(instructions):
        instruction = instructions[index]
        if register_re.search(instruction):
            return False
        mnemonic, _ = split_instruction(instruction)
        if mnemonic in ("lea", "mov", "nop", "pop", "push"):
            index += 1
            continue
        if (
            mnemonic in ("cmp", "test")
            and index + 1 < len(instructions)
            and split_instruction(instructions[index + 1])[0].startswith("j")
            and not register_re.search(instructions[index + 1])
        ):
            index += 2
            continue
        return False
    return True


def normalize_safe_zero_relocations(orig_asm: list[str], recomp_asm: list[str]) -> None:
    """Align uniquely paired zero idioms moved across independent instructions."""
    for orig_index, instruction in enumerate(orig_asm):
        if not is_zero_idiom(instruction) or orig_asm.count(instruction) != 1:
            continue
        if recomp_asm.count(instruction) != 1:
            continue
        recomp_index = recomp_asm.index(instruction)
        if recomp_index == orig_index:
            continue
        start = min(orig_index, recomp_index)
        end = max(orig_index, recomp_index)
        crossed = recomp_asm[start:end]
        if recomp_index < orig_index:
            crossed = recomp_asm[start + 1 : end + 1]
        _, operands = split_instruction(instruction)
        if not can_move_zero_across(crossed, operands[0]):
            continue
        recomp_asm.pop(recomp_index)
        recomp_asm.insert(orig_index, instruction)
        if operands[0] not in DWORD_REGISTERS:
            continue
        for index in range(start, end + 1):
            orig_mnemonic, orig_operands = split_instruction(orig_asm[index])
            recomp_mnemonic, recomp_operands = split_instruction(recomp_asm[index])
            if (
                orig_mnemonic != recomp_mnemonic
                or not orig_mnemonic.startswith("j")
                or len(orig_operands) != 1
                or len(recomp_operands) != 1
            ):
                continue
            try:
                displacement_delta = abs(
                    int(orig_operands[0], 0) - int(recomp_operands[0], 0)
                )
            except ValueError:
                continue
            if displacement_delta == 2:
                recomp_asm[index] = orig_asm[index]


def role_relocations_are_safe(
    orig_asm: list[str], recomp_asm: list[str], role_map: dict[str, str]
) -> bool:
    """Reject newly exposed relocations with register or flag dependencies."""
    matcher = SequenceMatcherWithPins(orig_asm, recomp_asm, [])
    deletes = {
        index
        for code, start, end, _, __ in matcher.get_opcodes()
        for index in range(start, end)
        if code == "delete"
    }
    inserts = [
        (orig_dest, index)
        for code, orig_dest, _, start, end in matcher.get_opcodes()
        for index in range(start, end)
        if code == "insert"
    ]

    for orig_dest, recomp_index in inserts:
        instruction = recomp_asm[recomp_index]
        candidates = [index for index in deletes if orig_asm[index] == instruction]
        if not candidates:
            continue
        mnemonic, _ = split_instruction(instruction)
        if mnemonic not in ("lea", "mov", "pop", "push"):
            return False
        used_registers = set(REGISTER_RE.findall(instruction))
        if mnemonic in ("pop", "push"):
            used_registers = {role_map.get(register, register) for register in used_registers}
            used_registers.add("esp")
        safe_candidate = None
        for candidate in candidates:
            start = min(candidate, orig_dest)
            end = max(candidate, orig_dest)
            if mnemonic in ("pop", "push") and any(
                index != candidate
                and split_instruction(orig_asm[index])[0] not in ("lea", "mov", "nop")
                for index in range(start, end)
            ):
                continue
            if all(
                index == candidate
                or not used_registers.intersection(REGISTER_RE.findall(orig_asm[index]))
                for index in range(start, end)
            ):
                safe_candidate = candidate
                break
        if safe_candidate is None:
            return False
        deletes.remove(safe_candidate)
    return True


def role_push_reaches_equivalent_call(
    orig_asm: list[str],
    recomp_asm: list[str],
    index: int,
    role_registers: set[str],
) -> bool:
    """Prove a role-renamed push feeds the same nearby call.

    Permit at most two identical, flag-neutral register setup instructions
    between the push and call.  The setup may not read or write either renamed
    role or ESP, so it cannot alter the pushed value or depend on a different
    stack position.
    """
    for call_index in range(index + 1, index + 4):
        if call_index >= len(orig_asm) or call_index >= len(recomp_asm):
            return False

        orig_text = orig_asm[call_index]
        recomp_text = recomp_asm[call_index]
        orig_mnemonic, orig_operands = split_instruction(orig_text)
        recomp_mnemonic, _ = split_instruction(recomp_text)
        if orig_mnemonic == "call" or recomp_mnemonic == "call":
            return orig_mnemonic == "call" and is_equivalent_insn(
                orig_text, recomp_text
            )
        if orig_text != recomp_text:
            return False
        if orig_mnemonic == "nop":
            continue
        if (
            orig_mnemonic not in ("lea", "mov")
            or len(orig_operands) != 2
            or orig_operands[0] not in DWORD_REGISTERS
            or set(REGISTER_RE.findall(orig_text)).intersection(
                role_registers | {"esp"}
            )
        ):
            return False

    return False


def normalize_saved_register_load_scheduling(
    orig_asm: list[str], recomp_asm: list[str]
) -> None:
    """Align a unique register load moved only across unrelated register saves."""
    for index, orig_text in enumerate(orig_asm):
        if index >= len(recomp_asm) or recomp_asm[index] == orig_text:
            continue
        if orig_asm.count(orig_text) != 1 or recomp_asm.count(orig_text) != 1:
            continue

        mnemonic, operands = split_instruction(orig_text)
        used_registers = set(REGISTER_RE.findall(orig_text))
        if (
            mnemonic not in ("lea", "mov")
            or len(operands) != 2
            or operands[0] not in DWORD_REGISTERS
            or "esp" in used_registers
        ):
            continue

        recomp_index = recomp_asm.index(orig_text)
        if recomp_index <= index:
            continue
        crossed = recomp_asm[index:recomp_index]
        if not crossed:
            continue
        for instruction in crossed:
            crossed_mnemonic, crossed_operands = split_instruction(instruction)
            if (
                crossed_mnemonic != "push"
                or len(crossed_operands) != 1
                or crossed_operands[0] not in DWORD_REGISTERS
                or crossed_operands[0] in used_registers
            ):
                break
        else:
            recomp_asm.insert(index, recomp_asm.pop(recomp_index))


def normalize_role_comparisons_for_exact_match(
    orig_asm: list[str], recomp_asm: list[str], role_map: dict[str, str]
) -> None:
    """Apply role renames to comparisons only when that completes an exact match."""
    if not role_map or len(orig_asm) != len(recomp_asm):
        return

    candidate = list(recomp_asm)
    for index, (orig_text, recomp_text) in enumerate(zip(orig_asm, candidate)):
        mnemonic, _ = split_instruction(recomp_text)
        if mnemonic not in ("cmp", "test"):
            continue
        remapped = REGISTER_RE.sub(
            lambda match: role_map.get(match.group(0), match.group(0)), recomp_text
        )
        if remapped == orig_text:
            candidate[index] = orig_text

    if candidate == orig_asm:
        recomp_asm[:] = candidate


def normalize_exact_full_register_role_rotation(
    orig_asm: list[str], recomp_asm: list[str], role_map: dict[str, str]
) -> None:
    """Canonicalize a complete full-register role permutation only when exact.

    The sole scheduling difference accepted here is an adjacent, disjoint
    register ``xor`` / register ``mov`` commute. Work on a copy so every other
    role, instruction, side effect, and control-flow edge must become identical
    before the caller-visible assembly is changed.
    """
    if (
        not role_map
        or len(orig_asm) != len(recomp_asm)
        or set(role_map) != set(role_map.values())
    ):
        return

    mapped_registers = set(role_map) | set(role_map.values())
    mapped_subregisters = {
        register
        for full_register in mapped_registers
        for register in DWORD_REGISTER_FAMILIES[full_register]
        if register != full_register
    }
    if any(
        register in mapped_subregisters
        for instruction in orig_asm + recomp_asm
        for register in REGISTER_RE.findall(instruction)
    ):
        return

    saved_registers: list[str] = []
    for instruction in orig_asm:
        mnemonic, operands = split_instruction(instruction)
        if (
            mnemonic != "push"
            or len(operands) != 1
            or operands[0] not in DWORD_REGISTERS - {"esp"}
        ):
            break
        saved_registers.append(operands[0])

    restore_start = len(orig_asm)
    if orig_asm and split_instruction(orig_asm[-1])[0] in ("ret", "retn"):
        restore_start -= 1
        while restore_start > 0:
            mnemonic, operands = split_instruction(orig_asm[restore_start - 1])
            if (
                mnemonic != "pop"
                or len(operands) != 1
                or operands[0] not in DWORD_REGISTERS - {"esp"}
            ):
                break
            restore_start -= 1

    restored_registers = [
        split_instruction(instruction)[1][0]
        for instruction in orig_asm[restore_start:-1]
    ]
    if (
        not mapped_registers.issubset(saved_registers)
        or restored_registers != list(reversed(saved_registers))
        or recomp_asm[: len(saved_registers)] != orig_asm[: len(saved_registers)]
        or recomp_asm[restore_start:] != orig_asm[restore_start:]
    ):
        return

    candidate = list(recomp_asm)
    excluded = {"call", "pop", "push", "ret", "retn"}
    for index, (orig_text, recomp_text) in enumerate(zip(orig_asm, candidate)):
        if orig_text == recomp_text:
            used_registers = set(REGISTER_RE.findall(recomp_text))
            if used_registers.intersection(mapped_registers) and not (
                index < len(saved_registers) or index >= restore_start
            ):
                return
            continue
        mnemonic, _ = split_instruction(recomp_text)
        if mnemonic in excluded or mnemonic.startswith("j"):
            continue
        candidate[index] = REGISTER_RE.sub(
            lambda match: role_map.get(match.group(0), match.group(0)), recomp_text
        )

    if candidate != orig_asm:
        differences = [
            index
            for index, (orig_text, recomp_text) in enumerate(zip(orig_asm, candidate))
            if orig_text != recomp_text
        ]
        if len(differences) != 2 or differences[1] != differences[0] + 1:
            return

        first = differences[0]
        if (
            candidate[first] != orig_asm[first + 1]
            or candidate[first + 1] != orig_asm[first]
        ):
            return

        pair = (candidate[first], candidate[first + 1])
        zero_text = next(
            (instruction for instruction in pair if split_instruction(instruction)[0] == "xor"),
            None,
        )
        mov_text = next(
            (instruction for instruction in pair if split_instruction(instruction)[0] == "mov"),
            None,
        )
        if zero_text is None or mov_text is None:
            return

        _, zero_operands = split_instruction(zero_text)
        _, mov_operands = split_instruction(mov_text)
        if (
            len(zero_operands) != 2
            or zero_operands[0] != zero_operands[1]
            or zero_operands[0] not in DWORD_REGISTERS
            or len(mov_operands) != 2
            or any(operand not in DWORD_REGISTERS for operand in mov_operands)
            or len(set(mov_operands)) != 2
            or zero_operands[0] in mov_operands
        ):
            return

        candidate[first], candidate[first + 1] = candidate[first + 1], candidate[first]

    if candidate == orig_asm:
        recomp_asm[:] = candidate


def normalize_register_role_relocations(orig_asm: list[str], recomp_asm: list[str]) -> None:
    """Expose relocations whose instruction also uses a renamed register role."""
    role_map = infer_register_role_map(orig_asm, recomp_asm)
    if not role_map:
        return

    original_recomp = list(recomp_asm)
    orig_zero_idioms = {instruction for instruction in orig_asm if is_zero_idiom(instruction)}
    excluded = {"call", "cmp", "pop", "push", "ret", "retn", "test"}
    for index, (orig_text, recomp_text) in enumerate(zip(orig_asm, recomp_asm)):
        if orig_text == recomp_text:
            continue
        mnemonic, operands = split_instruction(recomp_text)
        if mnemonic == "push":
            _, orig_operands = split_instruction(orig_text)
            remapped = REGISTER_RE.sub(
                lambda match: role_map.get(match.group(0), match.group(0)), recomp_text
            )
            if (
                len(operands) == 1
                and operands[0] in DWORD_REGISTERS
                and len(orig_operands) == 1
                and orig_operands[0] in DWORD_REGISTERS
                and remapped == orig_text
                and role_push_reaches_equivalent_call(
                    orig_asm,
                    recomp_asm,
                    index,
                    {operands[0], orig_operands[0]},
                )
            ):
                recomp_asm[index] = orig_text
            continue
        if mnemonic in excluded or mnemonic.startswith("j"):
            continue
        if (
            mnemonic in ("sub", "xor")
            and len(operands) == 2
            and operands[0] == operands[1]
            and recomp_text in orig_zero_idioms
        ):
            continue
        recomp_asm[index] = REGISTER_RE.sub(
            lambda match: role_map.get(match.group(0), match.group(0)), recomp_text
        )

    normalize_safe_zero_relocations(orig_asm, recomp_asm)
    if not role_relocations_are_safe(orig_asm, recomp_asm, role_map):
        recomp_asm[:] = original_recomp


def group_asm(chunks) -> tuple[list[str], list[str]]:
    """Rebuild the two assembly excerpts represented by one JSON diff group.

    reccmp writes matching lines once under ``both`` and mismatching lines as
    separate ``orig``/``recomp`` arrays. Known target-resolution differences
    are made identical before its compiler-entropy classifier runs.
    """
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

    normalize_copy_tests(orig_asm)
    normalize_copy_tests(recomp_asm)
    normalize_transient_call_argument_registers(orig_asm, recomp_asm)
    normalize_multiply_copy_zero(orig_asm)
    normalize_multiply_copy_zero(recomp_asm)
    normalize_divide_scratch_scheduling(orig_asm, recomp_asm)
    normalize_interleaved_stack_offset_chains(orig_asm, recomp_asm)
    normalize_zero_comparisons(orig_asm, recomp_asm)
    normalize_swapped_comparison_branches(orig_asm, recomp_asm)
    normalize_dead_stack_reservation(orig_asm, recomp_asm)
    role_map = infer_register_role_map(orig_asm, recomp_asm)
    normalize_exact_full_register_role_rotation(orig_asm, recomp_asm, role_map)
    normalize_register_role_relocations(orig_asm, recomp_asm)
    normalize_saved_register_load_scheduling(orig_asm, recomp_asm)
    normalize_role_comparisons_for_exact_match(orig_asm, recomp_asm, role_map)
    strip_trailing_alignment_nops(orig_asm, recomp_asm)
    return orig_asm, recomp_asm


def is_codegen_equivalent_diff(diff) -> bool:
    """Whether all remaining differences are accepted compiler entropy.

    This deliberately delegates register selection, operand-order, and safe
    instruction-relocation decisions to the pinned reccmp implementation. The
    local work is limited to removing target/thunk rendering differences that
    otherwise prevent reccmp from seeing the compiler-only match.
    """
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


def compute_ratio(match: dict | None, name: str = "") -> tuple[float, str]:
    """Calculate effective match percentage and match reason tag.

    ``name`` remains part of the public helper signature for report.py, but no
    generated-function name is evidence of a match by itself.
    """
    if match is None or match.get("stub"):
        return 0.0, "STUB"

    if match.get("effective") or float(match.get("matching", 0.0)) == 1.0:
        return 100.0, "MATCH"

    try:
        strict_address = norm_addr(match.get("address"))
    except (AttributeError, TypeError, ValueError):
        strict_address = None
    strict_claim = (
        strict_address in STRICT_EQUIVALENCE_TARGETS
        or match.get("name") in STRICT_EQUIVALENCE_NAMES
        or name in STRICT_EQUIVALENCE_NAMES
    )
    if strict_claim:
        if (
            is_exact_complete_call_role_rotation_match(match)
            or is_exact_ctos_request_role_swap_match(match)
            or is_exact_complete_adjacent_short_field_alias_match(match)
        ):
            return 100.0, "MATCH (compiler entropy)"
        return float(match.get("matching", 0.0)) * 100.0, ""

    if is_thunk_only_diff(match.get("diff")):
        return 100.0, "MATCH (thunk)"

    if (
        is_codegen_equivalent_diff(match.get("diff"))
        or is_exact_complete_call_role_rotation_match(match)
        or is_exact_ctos_request_role_swap_match(match)
        or is_exact_complete_adjacent_short_field_alias_match(match)
    ):
        return 100.0, "MATCH (compiler entropy)"

    return float(match.get("matching", 0.0)) * 100.0, ""


def is_exact_complete_call_role_rotation(
    orig_asm: list[str],
    recomp_asm: list[str],
    hidden_indices: set[int],
) -> bool:
    """Prove a complete nonvolatile role swap feeding identical calls.

    This deliberately recognizes only the evidenced DrawDuplicator codegen
    shape. The full function, including instructions omitted from grouped JSON
    context, is supplied by the caller. Nothing is changed unless the complete
    recomp sequence is an exact register-role renaming of the original.
    """
    if (
        len(orig_asm) != len(recomp_asm)
        or len(orig_asm) < 2
        or len(hidden_indices) != 1
        or not hidden_indices.issubset(range(len(orig_asm)))
    ):
        return False

    role_map = infer_register_role_map(orig_asm, recomp_asm)
    if (
        len(role_map) != 2
        or set(role_map) != set(role_map.values())
        or any(source == destination for source, destination in role_map.items())
        or not set(role_map).issubset({"ebx", "esi", "edi", "ebp"})
    ):
        return False

    mapped_registers = set(role_map)
    mapped_subregisters = {
        register
        for full_register in mapped_registers
        for register in DWORD_REGISTER_FAMILIES[full_register]
        if register != full_register
    }
    if any(
        register in mapped_subregisters
        for instruction in orig_asm + recomp_asm
        for register in REGISTER_RE.findall(instruction)
    ):
        return False

    hidden_index = next(iter(hidden_indices))
    hidden_orig = orig_asm[hidden_index]
    hidden_recomp = recomp_asm[hidden_index]
    hidden_mnemonic, hidden_operands = split_instruction(hidden_orig)
    if (
        hidden_orig != hidden_recomp
        or hidden_mnemonic != "lea"
        or len(hidden_operands) != 2
        or hidden_operands[0] not in DWORD_REGISTERS
        or set(REGISTER_RE.findall(hidden_orig)).intersection(
            mapped_registers | {"esp"}
        )
    ):
        return False

    call_indices = [
        index
        for index, instruction in enumerate(orig_asm)
        if split_instruction(instruction)[0] == "call"
    ]
    if (
        len(call_indices) != 3
        or any(orig_asm[index] != recomp_asm[index] for index in call_indices)
        or len({orig_asm[index] for index in call_indices}) != 1
    ):
        return False

    role_setup_indices: list[int] = []
    role_push_indices: list[int] = []
    candidate = list(recomp_asm)
    first_call = call_indices[0]

    for index, (orig_text, recomp_text) in enumerate(zip(orig_asm, recomp_asm)):
        if orig_text == recomp_text:
            continue

        remapped = REGISTER_RE.sub(
            lambda match: role_map.get(match.group(0), match.group(0)), recomp_text
        )
        if remapped != orig_text:
            return False
        candidate[index] = remapped

        orig_mnemonic, orig_operands = split_instruction(orig_text)
        recomp_mnemonic, recomp_operands = split_instruction(recomp_text)
        if orig_mnemonic != recomp_mnemonic:
            return False

        if (
            index < first_call
            and orig_mnemonic in ("mov", "sub")
            and len(orig_operands) == 2
            and len(recomp_operands) == 2
            and orig_operands[0] in mapped_registers
            and recomp_operands[0] in mapped_registers
        ):
            role_setup_indices.append(index)
            continue

        if (
            orig_mnemonic == "push"
            and len(orig_operands) == 1
            and len(recomp_operands) == 1
            and orig_operands[0] in mapped_registers
            and recomp_operands[0] in mapped_registers
        ):
            role_push_indices.append(index)
            continue

        return False

    if (
        len(role_setup_indices) != 4
        or role_setup_indices
        != list(range(role_setup_indices[0], role_setup_indices[0] + 4))
        or [split_instruction(orig_asm[index])[0] for index in role_setup_indices]
        != ["mov", "sub", "mov", "sub"]
        or len(role_push_indices) != 6
    ):
        return False

    setup_registers: list[str] = []
    for mov_index, sub_index in (
        role_setup_indices[:2],
        role_setup_indices[2:],
    ):
        orig_mov_operands = split_instruction(orig_asm[mov_index])[1]
        recomp_mov_operands = split_instruction(recomp_asm[mov_index])[1]
        orig_sub_operands = split_instruction(orig_asm[sub_index])[1]
        recomp_sub_operands = split_instruction(recomp_asm[sub_index])[1]
        if (
            orig_mov_operands[0] != orig_sub_operands[0]
            or recomp_mov_operands[0] != recomp_sub_operands[0]
            or orig_mov_operands[1] != recomp_mov_operands[1]
            or orig_sub_operands[1] != recomp_sub_operands[1]
            or not orig_mov_operands[1].startswith("dword ptr [")
            or set(REGISTER_RE.findall(orig_mov_operands[1])).intersection(
                mapped_registers | {"esp"}
            )
        ):
            return False
        setup_registers.append(recomp_mov_operands[0])

    if set(setup_registers) != mapped_registers:
        return False

    first_setup = role_setup_indices[0]
    save_order: list[str] = []
    save_indices: set[int] = set()
    for index in range(first_setup):
        if orig_asm[index] != recomp_asm[index]:
            return False
        mnemonic, operands = split_instruction(orig_asm[index])
        used_registers = set(REGISTER_RE.findall(orig_asm[index]))
        if used_registers.intersection(mapped_registers):
            if (
                mnemonic != "push"
                or len(operands) != 1
                or operands[0] not in mapped_registers
            ):
                return False
        if mnemonic == "push":
            if len(operands) != 1 or operands[0] not in {
                "ebx",
                "esi",
                "edi",
                "ebp",
            }:
                return False
            save_order.append(operands[0])
            save_indices.add(index)

    if save_order != ["ebx", "esi", "edi", "ebp"]:
        return False

    entry_mnemonic, entry_operands = split_instruction(orig_asm[0])
    if (
        orig_asm[0] != recomp_asm[0]
        or entry_mnemonic != "sub"
        or entry_operands != ["esp", "8"]
    ):
        return False

    ret_indices = [
        index
        for index, instruction in enumerate(orig_asm)
        if split_instruction(instruction)[0] in ("ret", "retn")
    ]
    if len(ret_indices) != 3:
        return False

    restore_indices: set[int] = set()
    for ret_index in ret_indices:
        if ret_index < 5:
            return False
        pop_indices = list(range(ret_index - 5, ret_index - 1))
        pop_registers = [
            split_instruction(orig_asm[index])[1]
            for index in pop_indices
            if split_instruction(orig_asm[index])[0] == "pop"
        ]
        if (
            orig_asm[ret_index] != recomp_asm[ret_index]
            or split_instruction(orig_asm[ret_index])[1] != ["4"]
            or orig_asm[ret_index - 1] != "add esp, 8"
            or recomp_asm[ret_index - 1] != "add esp, 8"
            or pop_registers != [["ebp"], ["edi"], ["esi"], ["ebx"]]
            or any(
                orig_asm[index] != recomp_asm[index] for index in pop_indices
            )
        ):
            return False
        restore_indices.update(pop_indices)

    if any(
        split_instruction(instruction)[0] == "pop"
        and index not in restore_indices
        for index, instruction in enumerate(orig_asm)
    ):
        return False

    allowed_identical_role_uses = save_indices | restore_indices
    for index, (orig_text, recomp_text) in enumerate(zip(orig_asm, recomp_asm)):
        if orig_text != recomp_text or index in allowed_identical_role_uses:
            continue
        if set(REGISTER_RE.findall(orig_text)).intersection(mapped_registers):
            return False

    pushes_per_call = {call_index: 0 for call_index in call_indices}
    for push_index in role_push_indices:
        matched_call = None
        for call_index in range(push_index + 1, min(push_index + 4, len(orig_asm))):
            if call_index in call_indices:
                matched_call = call_index
                break

            instruction = candidate[call_index]
            mnemonic, operands = split_instruction(instruction)
            used_registers = set(REGISTER_RE.findall(instruction))
            if mnemonic == "push":
                if len(operands) != 1 or "esp" in used_registers:
                    return False
                continue
            if (
                mnemonic not in ("lea", "mov")
                or len(operands) != 2
                or operands[0] not in DWORD_REGISTERS
                or used_registers.intersection(mapped_registers | {"esp"})
            ):
                return False

        if matched_call is None:
            return False
        pushes_per_call[matched_call] += 1

    if set(pushes_per_call.values()) != {2}:
        return False

    return candidate == orig_asm


def is_exact_ctos_request_role_swap(
    orig_asm: list[str], recomp_asm: list[str]
) -> bool:
    """Prove the complete CtoSRequestConnect ESI/EDI role swap.

    The only accepted schedule difference is the observed parameter load
    moving across ``push edi``. Its stack displacement must consequently grow
    by exactly four bytes. Every instruction after that normalization must be
    an exact ESI/EDI role rename, except physical saves and restores.
    """
    orig_asm = [normalize_asm(instruction) for instruction in orig_asm]
    recomp_asm = [normalize_asm(instruction) for instruction in recomp_asm]
    if len(orig_asm) != 55 or len(recomp_asm) != 55:
        return False

    if orig_asm[:6] != [
        "push ebx",
        "push esi",
        "mov esi, dword ptr [esp + 0xc]",
        "push edi",
        "mov edi, ecx",
        "push esi",
    ] or recomp_asm[:6] != [
        "push ebx",
        "push esi",
        "push edi",
        "mov esi, ecx",
        "mov edi, dword ptr [esp + 0x10]",
        "push edi",
    ]:
        return False

    physical_save_restore = {0, 1, 3, 28, 29, 30, 51, 52, 53}
    if any(
        register in {"si", "di"}
        for instruction in orig_asm + recomp_asm
        for register in REGISTER_RE.findall(instruction)
    ):
        return False

    call_indices = [
        index
        for index, instruction in enumerate(orig_asm)
        if split_instruction(instruction)[0] == "call"
    ]
    if call_indices != [6, 12, 16, 23, 27, 38, 50] or any(
        split_instruction(recomp_asm[index])[0] != "call"
        or not is_equivalent_insn(orig_asm[index], recomp_asm[index])
        for index in call_indices
    ):
        return False

    expected_restore = ["pop edi", "pop esi", "pop ebx", "ret 4"]
    if (
        orig_asm[28:32] != expected_restore
        or recomp_asm[28:32] != expected_restore
        or orig_asm[51:55] != expected_restore
        or recomp_asm[51:55] != expected_restore
    ):
        return False

    candidate = list(recomp_asm)
    candidate.pop(4)
    candidate.insert(2, "mov edi, dword ptr [esp + 0xc]")
    role_map = {"esi": "edi", "edi": "esi"}
    for index, instruction in enumerate(candidate):
        if index in physical_save_restore:
            if instruction != orig_asm[index]:
                return False
            continue
        candidate[index] = REGISTER_RE.sub(
            lambda match: role_map.get(match.group(0), match.group(0)),
            instruction,
        )

    return candidate == orig_asm


def is_exact_adjacent_short_field_alias(
    orig_text: str,
    recomp_text: str,
    orig_pattern: str,
    expected_recomp_text: str,
    orig_base: int,
    recomp_base: int,
    orig_target: int,
    recomp_target: int,
    orig_data: bytes,
    recomp_data: bytes,
    expected_data: bytes,
) -> bool:
    """Prove one named ``short[2]`` second-field relocation alias."""
    orig_text = normalize_asm(orig_text)
    recomp_text = normalize_asm(recomp_text)
    return (
        len(expected_data) == 4
        and re.fullmatch(orig_pattern, orig_text) is not None
        and recomp_text == expected_recomp_text
        and orig_target == orig_base + 2
        and recomp_target == recomp_base + 2
        and orig_data == expected_data
        and recomp_data == expected_data
    )


def is_exact_indexed_static_data_reference(
    orig_text: str,
    recomp_text: str,
    orig_pattern: str,
    expected_recomp_text: str,
    orig_target: int,
    expected_orig_target: int,
    recomp_target: int,
    expected_recomp_target: int,
    orig_data: bytes,
    recomp_data: bytes,
    expected_data: bytes,
) -> bool:
    """Prove one indexed static-data relocation and its complete object."""
    return (
        len(expected_data) > 0
        and re.fullmatch(orig_pattern, normalize_asm(orig_text)) is not None
        and normalize_asm(recomp_text) == expected_recomp_text
        and orig_target == expected_orig_target
        and recomp_target == expected_recomp_target
        and orig_data == expected_data
        and recomp_data == expected_data
    )


@cache
def load_original_image():
    """Load the immutable original image for complete-context validation."""
    return detect_image(ORIGINAL_EXE)


@cache
def load_recompiled_image():
    """Load the current rebuilt image for complete-context validation."""
    return detect_image(RECOMPILED_EXE)


@cache
def load_recompiled_symbol_addresses() -> dict[str, tuple[int, ...]]:
    """Resolve public, global, and function-static PDB symbols to exact VAs."""
    image = detect_image(RECOMPILED_EXE)
    cvdump = Cvdump(str(RECOMPILED_PDB)).publics().globals().symbols().run()
    addresses: dict[str, set[int]] = {}

    def add(name: str, section: int, offset: int) -> None:
        try:
            address = image.get_abs_addr(section, offset)
        except (AssertionError, IndexError, KeyError, LookupError, ValueError):
            return
        addresses.setdefault(name, set()).add(address)

    for entry in cvdump.publics:
        add(entry.name, entry.section, entry.offset)
    for entry in cvdump.globals:
        add(entry.name, entry.section, entry.offset)
    for function in cvdump.symbols:
        for entry in function.static_variables:
            add(entry.name, entry.section, entry.offset)

    return {
        name: tuple(sorted(symbol_addresses))
        for name, symbol_addresses in addresses.items()
    }


def unique_recompiled_symbol_address(name: str) -> int | None:
    """Return one unambiguous PDB address, failing closed on all errors."""
    try:
        addresses = load_recompiled_symbol_addresses().get(name, ())
    except (
        AssertionError,
        IndexError,
        KeyError,
        LookupError,
        OSError,
        RuntimeError,
        ValueError,
    ):
        return None
    return addresses[0] if len(addresses) == 1 else None


def direct_call_reaches_symbol(
    image, instruction, expected_symbol_address: int
) -> bool:
    """Bind one raw direct call, through at most one thunk, to a PDB VA."""
    _, instruction_size, mnemonic, operands = instruction
    if (
        mnemonic != "call"
        or instruction_size != 5
        or re.fullmatch(r"0x[0-9a-f]+", operands) is None
    ):
        return False
    target = int(operands, 16)
    if target == expected_symbol_address:
        return True
    decoded = decode_exact_image_span(image, target, 5)
    return bool(
        decoded is not None
        and len(decoded) == 1
        and decoded[0][2] == "jmp"
        and decoded[0][3] == hex(expected_symbol_address)
    )


def collect_visible_instruction_groups(diff):
    """Return paired visible instructions and addresses from grouped JSON."""
    groups = []
    if not diff:
        return None

    try:
        for _, chunks in diff:
            orig_entries: list[tuple[int, str]] = []
            recomp_entries: list[tuple[int, str]] = []
            for chunk in chunks:
                both = chunk.get("both", []) or []
                if both:
                    for entry in both:
                        if not isinstance(entry, (list, tuple)) or len(entry) < 3:
                            return None
                        orig_entries.append(
                            (norm_addr(entry[0]), normalize_asm(insn_text(entry)))
                        )
                        recomp_entries.append(
                            (norm_addr(entry[2]), normalize_asm(insn_text(entry)))
                        )
                    continue

                for entry in chunk.get("orig", []) or []:
                    if not isinstance(entry, (list, tuple)) or len(entry) < 2:
                        return None
                    orig_entries.append(
                        (norm_addr(entry[0]), normalize_asm(insn_text(entry)))
                    )
                for entry in chunk.get("recomp", []) or []:
                    if not isinstance(entry, (list, tuple)) or len(entry) < 2:
                        return None
                    recomp_entries.append(
                        (norm_addr(entry[0]), normalize_asm(insn_text(entry)))
                    )

            if not orig_entries or len(orig_entries) != len(recomp_entries):
                return None
            groups.append((orig_entries, recomp_entries))
    except (AttributeError, TypeError, ValueError):
        return None

    return groups


def decode_complete_original_function(match: dict):
    """Decode one exactly bounded original function or fail closed."""
    try:
        start = norm_addr(match["address"])
        end = norm_addr(match["_next_orig_addr"])
    except (AttributeError, KeyError, TypeError, ValueError):
        return None

    size = end - start
    if size <= 0 or size > 0x400:
        return None

    try:
        blob = load_original_image().read(start, size)
        decoded = list(get_disassembler().disasm_lite(blob, start))
    except (IndexError, OSError, RuntimeError, ValueError):
        return None

    if not decoded:
        return None
    expected_address = start
    for address, instruction_size, mnemonic, _ in decoded:
        if address != expected_address or instruction_size <= 0 or mnemonic == "int3":
            return None
        expected_address += instruction_size
    if expected_address != end:
        return None

    address_to_index = {
        instruction[0]: index for index, instruction in enumerate(decoded)
    }
    reachable = {0}
    pending = [0]
    while pending:
        index = pending.pop()
        address, _, mnemonic, operands = decoded[index]
        del address
        successors: list[int] = []
        if mnemonic in ("ret", "retn"):
            pass
        elif mnemonic.startswith("j"):
            if not re.fullmatch(r"0x[0-9a-f]+", operands):
                return None
            target_index = address_to_index.get(int(operands, 16))
            if target_index is None:
                return None
            successors.append(target_index)
            if mnemonic != "jmp":
                if index + 1 >= len(decoded):
                    return None
                successors.append(index + 1)
        else:
            if index + 1 >= len(decoded):
                return None
            successors.append(index + 1)

        for successor in successors:
            if successor not in reachable:
                reachable.add(successor)
                pending.append(successor)

    if len(reachable) != len(decoded):
        return None
    return decoded


def decoded_text_matches_json(decoded_text: str, json_text: str) -> bool:
    """Check one original instruction while ignoring rendered local targets."""
    decoded_mnemonic, _ = split_instruction(decoded_text)
    json_mnemonic, _ = split_instruction(json_text)
    if decoded_mnemonic != json_mnemonic:
        return False
    if decoded_mnemonic == "call" or decoded_mnemonic.startswith("j"):
        return True
    return decoded_text == json_text


def is_exact_complete_call_role_rotation_match(match: dict) -> bool:
    """Prove DrawDuplicator from synchronized JSON, PDB, and both PEs."""
    try:
        if (
            norm_addr(match["address"]) != 0x0043CFA0
            or match.get("name") != "C2D::DrawDuplicator"
            or norm_addr(match["_next_orig_addr"]) != 0x0043D070
        ):
            return False
        orig_start = norm_addr(match["address"])
        recomp_start = norm_addr(match["recomp"])
    except (AttributeError, KeyError, TypeError, ValueError):
        return False

    groups = collect_visible_instruction_groups(match.get("diff"))
    if groups is None or tuple(len(group) for group, _ in groups) != (58, 20):
        return False
    try:
        original_image = load_original_image()
        recompiled_image = load_recompiled_image()
    except (IndexError, LookupError, OSError, RuntimeError, ValueError):
        return False
    decoded_orig = decode_exact_image_span(original_image, orig_start, 0xD0)
    decoded_recomp = decode_exact_image_span(
        recompiled_image, recomp_start, 0xD0
    )
    if (
        decoded_orig is None
        or decoded_recomp is None
        or len(decoded_orig) != 79
        or len(decoded_recomp) != 79
    ):
        return False

    orig_index = {
        address: index
        for index, (address, _, _, _) in enumerate(decoded_orig)
    }
    recomp_index = {
        address: index
        for index, (address, _, _, _) in enumerate(decoded_recomp)
    }
    paired_by_index: dict[int, tuple[str, str]] = {}

    for orig_entries, recomp_entries in groups:
        for (orig_address, orig_text), (recomp_address, recomp_text) in zip(
            orig_entries, recomp_entries
        ):
            left_index = orig_index.get(orig_address)
            right_index = recomp_index.get(recomp_address)
            if (
                left_index is None
                or left_index != right_index
                or left_index in paired_by_index
            ):
                return False
            paired_by_index[left_index] = (orig_text, recomp_text)

    expected_visible = tuple(index for index in range(79) if index != 58)
    if tuple(sorted(paired_by_index)) != expected_visible:
        return False

    expected_draw_anim = unique_recompiled_symbol_address(
        "?DrawAnim@LemmingAnimsManager@@QAEXFFKKKPAVRemap@@@Z"
    )
    if expected_draw_anim is None:
        return False

    orig_asm: list[str] = []
    recomp_asm: list[str] = []
    call_indices: list[int] = []
    recomp_call_targets: set[int] = set()
    for index, (orig_instruction, recomp_instruction) in enumerate(
        zip(decoded_orig, decoded_recomp)
    ):
        orig_address, orig_size, orig_mnemonic, orig_operands = orig_instruction
        recomp_address, recomp_size, recomp_mnemonic, recomp_operands = (
            recomp_instruction
        )
        if (
            orig_address - orig_start != recomp_address - recomp_start
            or orig_size != recomp_size
            or orig_mnemonic != recomp_mnemonic
        ):
            return False

        orig_text = f"{orig_mnemonic} {orig_operands}".strip()
        recomp_text = f"{recomp_mnemonic} {recomp_operands}".strip()
        rendered_pair = paired_by_index.get(index)
        if rendered_pair is None:
            if index != 58 or orig_text != recomp_text:
                return False
        elif orig_mnemonic != "call" and not (
            decoded_text_matches_json(orig_text, rendered_pair[0])
            and decoded_text_matches_json(recomp_text, rendered_pair[1])
        ):
            return False

        if orig_mnemonic.startswith("j"):
            if (
                re.fullmatch(r"0x[0-9a-f]+", orig_operands) is None
                or re.fullmatch(r"0x[0-9a-f]+", recomp_operands) is None
            ):
                return False
            orig_target = int(orig_operands, 16) - orig_start
            recomp_target = int(recomp_operands, 16) - recomp_start
            if orig_target != recomp_target:
                return False
            normalized = f"{orig_mnemonic} target_{orig_target:x}"
            orig_asm.append(normalized)
            recomp_asm.append(normalized)
            continue

        if orig_mnemonic == "call":
            if (
                rendered_pair is None
                or re.fullmatch(r"call <OFFSET\d+>", rendered_pair[0])
                is None
                or rendered_pair[1]
                != "call LemmingAnimsManager::DrawAnim (FUNCTION)"
                or orig_operands != "0x4016a9"
                or not direct_call_reaches_symbol(
                    recompiled_image,
                    recomp_instruction,
                    expected_draw_anim,
                )
            ):
                return False
            call_indices.append(index)
            recomp_call_targets.add(int(recomp_operands, 16))
            orig_asm.append("call EXACT_DRAW_ANIM")
            recomp_asm.append("call EXACT_DRAW_ANIM")
            continue

        orig_asm.append(orig_text)
        recomp_asm.append(recomp_text)

    if call_indices != [28, 47, 72] or len(recomp_call_targets) != 1:
        return False
    if not (
        complete_control_flow_is_reachable(decoded_orig, orig_start, {})
        and complete_control_flow_is_reachable(
            decoded_recomp, recomp_start, {}
        )
    ):
        return False

    return is_exact_complete_call_role_rotation(
        orig_asm, recomp_asm, {58}
    )


def is_exact_ctos_request_role_swap_match(match: dict) -> bool:
    """Prove CtoSRequestConnect from synchronized JSON, PDB, and both PEs."""
    try:
        if (
            norm_addr(match["address"]) != 0x004621E0
            or match.get("name") != "BaseNetwork::CtoSRequestConnect"
            or norm_addr(match["_next_orig_addr"]) != 0x00462280
        ):
            return False
        orig_start = norm_addr(match["address"])
        recomp_start = norm_addr(match["recomp"])
        bounded_size = norm_addr(match["_next_orig_addr"]) - orig_start
    except (AttributeError, KeyError, TypeError, ValueError):
        return False
    if bounded_size <= 0 or bounded_size > 0x100:
        return False

    groups = collect_visible_instruction_groups(match.get("diff"))
    if groups is None or len(groups) != 1:
        return False
    orig_entries, recomp_entries = groups[0]
    if len(orig_entries) != 55 or len(recomp_entries) != 55:
        return False

    try:
        original_image = load_original_image()
        recompiled_image = load_recompiled_image()
    except (IndexError, LookupError, OSError, RuntimeError, ValueError):
        return False
    decoded_orig = decode_exact_image_span(original_image, orig_start, 0x96)
    decoded_recomp = decode_exact_image_span(
        recompiled_image, recomp_start, 0x96
    )
    if (
        decoded_orig is None
        or decoded_recomp is None
        or len(decoded_orig) != 55
        or len(decoded_recomp) != 55
    ):
        return False

    if not (
        complete_control_flow_is_reachable(decoded_orig, orig_start, {})
        and complete_control_flow_is_reachable(
            decoded_recomp, recomp_start, {}
        )
    ):
        return False

    orig_addresses = [address for address, _ in orig_entries]
    recomp_addresses = [address for address, _ in recomp_entries]
    decoded_orig_addresses = [
        address for address, _, _, _ in decoded_orig
    ]
    decoded_recomp_addresses = [
        address for address, _, _, _ in decoded_recomp
    ]
    if (
        orig_addresses != decoded_orig_addresses
        or recomp_addresses != decoded_recomp_addresses
    ):
        return False

    orig_prefix_offsets = [address - orig_start for address in orig_addresses[:6]]
    recomp_prefix_offsets = [
        address - recomp_start for address in recomp_addresses[:6]
    ]
    if (
        orig_prefix_offsets != [0, 1, 2, 6, 7, 9]
        or recomp_prefix_offsets != [0, 1, 2, 3, 5, 9]
        or any(
            orig_address - orig_start != recomp_address - recomp_start
            for orig_address, recomp_address in zip(
                orig_addresses[6:], recomp_addresses[6:]
            )
        )
    ):
        return False

    call_specs = {
        6: (
            0x00462180,
            "call BaseNetwork::FindConnection (FUNCTION)",
            "?FindConnection@BaseNetwork@@QAEPAVCConnect@@PAVNetworkAddress@@@Z",
        ),
        12: (
            0x00460CE0,
            "call CConnect::CheckConnectTime (FUNCTION)",
            "?CheckConnectTime@CConnect@@QAEHXZ",
        ),
        16: (
            0x00462040,
            "call BaseNetwork::NewConnect (FUNCTION)",
            "?NewConnect@BaseNetwork@@QAEPAVCConnect@@XZ",
        ),
        27: (
            0x00460FB0,
            "call CConnect::Kill (FUNCTION)",
            "?Kill@CConnect@@QAEXXZ",
        ),
        38: (
            0x00460C60,
            "call CConnect::InitConnect (FUNCTION)",
            "?InitConnect@CConnect@@QAEXPBDPAVNetworkAddress@@F@Z",
        ),
        50: (
            0x00460A50,
            "call Broadcast::Send (FUNCTION)",
            "?Send@Broadcast@@QAEXPAVNetworkAddress@@AAVBroadcastMessage@@@Z",
        ),
    }
    global_specs = {
        19: (
            0x004A1E50,
            "mov eax, dword ptr [g_pMessReqConnect (DATA)]",
            "g_pMessReqConnect",
        ),
        34: (
            0x004A1E50,
            "mov eax, dword ptr [g_pMessReqConnect (DATA)]",
            "g_pMessReqConnect",
        ),
        41: (
            0x004A1E58,
            "mov ecx, dword ptr [g_pMessOKConnect (DATA)]",
            "g_pMessOKConnect",
        ),
        44: (
            0x004A1E58,
            "mov ecx, dword ptr [g_pMessOKConnect (DATA)]",
            "g_pMessOKConnect",
        ),
        46: (
            0x004A1E58,
            "mov ecx, dword ptr [g_pMessOKConnect (DATA)]",
            "g_pMessOKConnect",
        ),
    }

    orig_asm: list[str] = []
    recomp_asm: list[str] = []
    rebuilt_call_targets: set[int] = set()
    rebuilt_global_targets: dict[str, set[int]] = {}
    for index, (
        (orig_address, orig_size, orig_mnemonic, orig_operands),
        (recomp_address, recomp_size, recomp_mnemonic, recomp_operands),
    ) in enumerate(zip(decoded_orig, decoded_recomp)):
        orig_json = orig_entries[index][1]
        recomp_json = recomp_entries[index][1]
        orig_text = f"{orig_mnemonic} {orig_operands}".strip()
        recomp_text = f"{recomp_mnemonic} {recomp_operands}".strip()

        if index not in {2, 3, 4, 5} and (
            orig_address - orig_start != recomp_address - recomp_start
            or orig_size != recomp_size
            or orig_mnemonic != recomp_mnemonic
        ):
            return False

        if orig_mnemonic.startswith("j") or recomp_mnemonic.startswith("j"):
            if (
                orig_mnemonic != recomp_mnemonic
                or re.fullmatch(r"0x[0-9a-f]+", orig_operands) is None
                or re.fullmatch(r"0x[0-9a-f]+", recomp_operands) is None
                or int(orig_operands, 16) - orig_start
                != int(recomp_operands, 16) - recomp_start
                or not decoded_text_matches_json(orig_text, orig_json)
                or not decoded_text_matches_json(recomp_text, recomp_json)
            ):
                return False
            normalized = (
                f"{orig_mnemonic} "
                f"target_{int(orig_operands, 16) - orig_start:x}"
            )
            orig_asm.append(normalized)
            recomp_asm.append(normalized)
            continue

        if index == 23:
            if (
                orig_text != "call dword ptr [eax]"
                or recomp_text != orig_text
                or orig_json != orig_text
                or recomp_json != orig_text
            ):
                return False
            orig_asm.append(orig_text)
            recomp_asm.append(recomp_text)
            continue

        call_spec = call_specs.get(index)
        if call_spec is not None:
            orig_target, rendered_text, pdb_name = call_spec
            symbol_address = unique_recompiled_symbol_address(pdb_name)
            if (
                orig_mnemonic != "call"
                or recomp_mnemonic != "call"
                or orig_operands != hex(orig_target)
                or orig_json != rendered_text
                or recomp_json != rendered_text
                or symbol_address is None
                or not direct_call_reaches_symbol(
                    recompiled_image, decoded_recomp[index], symbol_address
                )
            ):
                return False
            rebuilt_call_targets.add(int(recomp_operands, 16))
            orig_asm.append(rendered_text)
            recomp_asm.append(rendered_text)
            continue

        global_spec = global_specs.get(index)
        if global_spec is not None:
            orig_target, rendered_text, pdb_name = global_spec
            recomp_target = unique_recompiled_symbol_address(pdb_name)
            if (
                orig_text
                != rendered_text.replace(
                    f"[{pdb_name} (DATA)]", f"[{hex(orig_target)}]"
                )
                or recomp_target is None
                or recomp_text
                != rendered_text.replace(
                    f"[{pdb_name} (DATA)]", f"[{hex(recomp_target)}]"
                )
                or orig_json != rendered_text
                or recomp_json != rendered_text
            ):
                return False
            rebuilt_global_targets.setdefault(pdb_name, set()).add(
                recomp_target
            )
            orig_asm.append(rendered_text)
            recomp_asm.append(rendered_text)
            continue

        if orig_text != orig_json or recomp_text != recomp_json:
            return False
        orig_asm.append(orig_text)
        recomp_asm.append(recomp_text)

    if (
        len(rebuilt_call_targets) != len(call_specs)
        or set(rebuilt_global_targets) != {
            "g_pMessReqConnect",
            "g_pMessOKConnect",
        }
        or any(len(targets) != 1 for targets in rebuilt_global_targets.values())
        or next(iter(rebuilt_global_targets["g_pMessReqConnect"]))
        == next(iter(rebuilt_global_targets["g_pMessOKConnect"]))
    ):
        return False

    return is_exact_ctos_request_role_swap(
        orig_asm,
        recomp_asm,
    )


def collect_addressed_function_pairs(diff):
    """Collect paired instructions while separating reccmp jump-table rows."""
    if not diff:
        return None

    records: list[dict] = []
    table_rows: list[tuple[int, int, str]] = []
    table_markers = 0
    try:
        for _, chunks in diff:
            for chunk in chunks:
                both = chunk.get("both", []) or []
                if both:
                    for entry in both:
                        if not isinstance(entry, (list, tuple)) or len(entry) < 3:
                            return None
                        orig_raw = str(entry[0]).strip()
                        recomp_raw = str(entry[2]).strip()
                        instruction = normalize_asm(insn_text(entry))
                        if not orig_raw and not recomp_raw:
                            if instruction != "Jump table:":
                                return None
                            table_markers += 1
                            continue
                        if not orig_raw or not recomp_raw:
                            return None
                        if instruction.startswith("start + "):
                            table_rows.append(
                                (norm_addr(orig_raw), norm_addr(recomp_raw), instruction)
                            )
                            continue
                        records.append(
                            {
                                "orig_address": norm_addr(orig_raw),
                                "orig_text": instruction,
                                "recomp_address": norm_addr(recomp_raw),
                                "recomp_text": instruction,
                                "different": False,
                            }
                        )
                    continue

                orig_entries = chunk.get("orig", []) or []
                recomp_entries = chunk.get("recomp", []) or []
                if not orig_entries or len(orig_entries) != len(recomp_entries):
                    return None
                for orig_entry, recomp_entry in zip(orig_entries, recomp_entries):
                    if (
                        not isinstance(orig_entry, (list, tuple))
                        or len(orig_entry) < 2
                        or not isinstance(recomp_entry, (list, tuple))
                        or len(recomp_entry) < 2
                    ):
                        return None
                    records.append(
                        {
                            "orig_address": norm_addr(orig_entry[0]),
                            "orig_text": normalize_asm(insn_text(orig_entry)),
                            "recomp_address": norm_addr(recomp_entry[0]),
                            "recomp_text": normalize_asm(insn_text(recomp_entry)),
                            "different": True,
                        }
                    )
    except (AttributeError, TypeError, ValueError):
        return None

    return records, table_rows, table_markers


def decode_exact_image_span(image, start: int, size: int):
    """Decode one exact code span without accepting gaps or trailing bytes."""
    if size <= 0 or size > 0x400:
        return None
    try:
        blob = image.read(start, size)
        decoded = list(get_disassembler().disasm_lite(blob, start))
    except (IndexError, OSError, RuntimeError, ValueError):
        return None
    if not decoded:
        return None

    expected_address = start
    for address, instruction_size, mnemonic, _ in decoded:
        if (
            address != expected_address
            or instruction_size <= 0
            or mnemonic in ("int3", "db")
        ):
            return None
        expected_address += instruction_size
    if expected_address != start + size:
        return None
    return decoded


def absolute_instruction_shape(instruction: str) -> str:
    """Hide only image-address constants, preserving all operand structure."""

    def replace_address(match: re.Match) -> str:
        value = int(match.group(0), 16)
        return "<ABS>" if value >= 0x00400000 else match.group(0)

    return re.sub(r"0x[0-9a-f]+", replace_address, instruction)


def direct_word_target(decoded_text: str, rendered_text: str) -> int | None:
    """Extract an absolute ``word ptr`` target with an exact rendered prefix."""
    decoded_text = normalize_asm(decoded_text)
    rendered_text = normalize_asm(rendered_text)
    if "[" not in rendered_text or not rendered_text.endswith("]"):
        return None
    prefix = rendered_text.split("[", 1)[0]
    match = re.fullmatch(re.escape(prefix) + r"\[(0x[0-9a-f]+)\]", decoded_text)
    if match is None:
        return None
    return int(match.group(1), 16)


def complete_control_flow_is_reachable(
    decoded, start: int, indirect_targets: dict[int, list[int]]
) -> bool:
    """Require every decoded instruction to be reachable through exact edges."""
    address_to_index = {
        address: index for index, (address, _, _, _) in enumerate(decoded)
    }
    reachable = {0}
    pending = [0]
    while pending:
        index = pending.pop()
        _, _, mnemonic, operands = decoded[index]
        successors: list[int] = []
        if mnemonic in ("ret", "retn"):
            pass
        elif mnemonic == "jmp" and index in indirect_targets:
            for target in indirect_targets[index]:
                target_index = address_to_index.get(target)
                if target_index is None:
                    return False
                successors.append(target_index)
        elif mnemonic.startswith("j"):
            if not re.fullmatch(r"0x[0-9a-f]+", operands):
                return False
            target_index = address_to_index.get(int(operands, 16))
            if target_index is None:
                return False
            successors.append(target_index)
            if mnemonic != "jmp":
                if index + 1 >= len(decoded):
                    return False
                successors.append(index + 1)
        else:
            if index + 1 >= len(decoded):
                return False
            successors.append(index + 1)

        for successor in successors:
            if successor not in reachable:
                reachable.add(successor)
                pending.append(successor)

    return len(reachable) == len(decoded)


def read_pointer_table_offsets(
    image, table_address: int, count: int, function_start: int
) -> tuple[int, ...] | None:
    try:
        raw = image.read(table_address, count * 4)
    except (IndexError, OSError, RuntimeError, ValueError):
        return None
    if len(raw) != count * 4:
        return None
    return tuple(
        int.from_bytes(raw[index : index + 4], "little") - function_start
        for index in range(0, len(raw), 4)
    )


def is_exact_complete_adjacent_short_field_alias_match(match: dict) -> bool:
    """Prove selected base+2 aliases from both complete executable bodies."""
    candidates = {
        0x0043D0F0: {
            "name": "C2D::DrawTimeBonus",
            "size": 0x37,
            "instruction_count": 16,
            "call_indices": (14,),
            "first_visible_index": 0,
            "json_table_rows": 0,
            "jump_table": (),
            "fields": (
                {
                    "orig_pattern": r"sub ax, word ptr \[<OFFSET\d+>\]",
                    "recomp_text": "sub ax, word ptr [timeBonusOffset[1] (OFFSET)]",
                    "base_text": "sub ax, word ptr [timeBonusOffset (DATA)]",
                    "pdb_name": "timeBonusOffset",
                    "orig_base": 0x00497088,
                    "data": b"\x10\x00\x12\x00",
                    "count": 1,
                },
            ),
        },
        0x0043D370: {
            "name": "C2D::DrawSheep",
            "size": 0x94,
            "instruction_count": 51,
            "call_indices": (28, 45),
            "first_visible_index": 7,
            "json_table_rows": 4,
            "jump_table": (0x32, 0x32, 0x63, 0x8D, 0x32),
            "fields": (
                {
                    "orig_pattern": r"sub si, word ptr \[<OFFSET\d+>\]",
                    "recomp_text": "sub si, word ptr [sheepOffset[1] (OFFSET)]",
                    "base_text": "sub dx, word ptr [sheepOffset (DATA)]",
                    "pdb_name": "sheepOffset",
                    "orig_base": 0x0049706C,
                    "data": b"\x09\x00\x08\x00",
                    "count": 2,
                },
            ),
            "data_refs": (
                {
                    "index": 24,
                    "orig_text": "mov ebx, dword ptr "
                    "[edi*4 + sheepMunchResources (DATA)]",
                    "recomp_text": "mov ebx, dword ptr [edi*4 + sheepMunchResources (DATA)]",
                    "orig_target": 0x0049EF78,
                    "pdb_name": "sheepMunchResources",
                    "data": bytes.fromhex(
                        "a9000000a3000000aa000000a5000000"
                        "ab000000a7000000ac000000a1000000"
                    ),
                },
                {
                    "index": 41,
                    "orig_text": "mov ebx, dword ptr "
                    "[edi*4 + sheepWalkResources (DATA)]",
                    "recomp_text": "mov ebx, dword ptr [edi*4 + sheepWalkResources (DATA)]",
                    "orig_target": 0x0049EF58,
                    "pdb_name": "sheepWalkResources",
                    "data": bytes.fromhex(
                        "a2000000a3000000a4000000a5000000"
                        "a6000000a7000000a8000000a1000000"
                    ),
                },
            ),
        },
        0x0043D420: {
            "name": "C2D::DrawBall",
            "size": 0x88,
            "instruction_count": 49,
            "call_indices": (23, 45),
            "first_visible_index": 3,
            "json_table_rows": 0,
            "jump_table": (),
            "fields": (
                {
                    "orig_pattern": r"sub di, word ptr \[<OFFSET\d+>\]",
                    "recomp_text": "sub di, word ptr [ballOffset[1] (OFFSET)]",
                    "base_text": "sub si, word ptr [ballOffset (DATA)]",
                    "pdb_name": "ballOffset",
                    "orig_base": 0x00497074,
                    "data": b"\x0a\x00\x0f\x00",
                    "count": 1,
                },
                {
                    "orig_pattern": r"sub di, word ptr \[<OFFSET\d+>\]",
                    "recomp_text": "sub di, word ptr [explosionOffset[1] (OFFSET)]",
                    "base_text": "sub si, word ptr [explosionOffset (DATA)]",
                    "pdb_name": "explosionOffset",
                    "orig_base": 0x00497078,
                    "data": b"\x0f\x00\x11\x00",
                    "count": 1,
                },
            ),
        },
        0x0043D4B0: {
            "name": "C2D::DrawKey",
            "size": 0x43,
            "instruction_count": 20,
            "call_indices": (18,),
            "first_visible_index": 2,
            "json_table_rows": 0,
            "jump_table": (),
            "fields": (
                {
                    "orig_pattern": r'sub dx, word ptr \[" " \(STRING\)\]',
                    "recomp_text": "sub dx, word ptr [keyOffset[1] (OFFSET)]",
                    "base_text": "sub ax, word ptr [keyOffset (DATA)]",
                    "pdb_name": "keyOffset",
                    "orig_base": 0x00497054,
                    "data": b"\x08\x00\x20\x00",
                    "count": 1,
                },
            ),
        },
        0x0043D7E0: {
            "name": "C2D::DrawSwitch",
            "size": 0xE4,
            "instruction_count": 99,
            "call_indices": (42, 55, 79, 93),
            "first_visible_index": 0,
            "visible_indices": (
                tuple(range(0, 21))
                + tuple(range(32, 66))
                + tuple(range(69, 99))
            ),
            "json_table_rows": 0,
            "jump_table": (),
            "fields": (
                {
                    "orig_pattern": r"movsx eax, word ptr \[<OFFSET\d+>\]",
                    "recomp_text": "movsx eax, word ptr [switchOffset[1] (OFFSET)]",
                    "base_text": "movsx eax, word ptr [switchOffset (DATA)]",
                    "pdb_name": "switchOffset",
                    "orig_base": 0x0049707C,
                    "data": b"\x05\x00\x19\x00",
                    "count": 1,
                },
            ),
        },
        0x0043D8D0: {
            "name": "C2D::DrawFlag",
            "size": 0x73,
            "instruction_count": 43,
            "call_indices": (26, 39),
            "first_visible_index": 0,
            "json_table_rows": 0,
            "jump_table": (),
            "fields": (
                {
                    "orig_pattern": r"movsx eax, word ptr \[<OFFSET\d+>\]",
                    "recomp_text": "movsx eax, word ptr [flagOffset[1] (OFFSET)]",
                    "base_text": "movsx eax, word ptr [flagOffset (DATA)]",
                    "pdb_name": "flagOffset",
                    "orig_base": 0x00497080,
                    "data": b"\x0f\x00\x1c\x00",
                    "count": 1,
                },
            ),
        },
        0x0043D950: {
            "name": "C2D::DrawBonus",
            "size": 0x35,
            "instruction_count": 15,
            "call_indices": (13,),
            "first_visible_index": 0,
            "json_table_rows": 0,
            "jump_table": (),
            "fields": (
                {
                    "orig_pattern": r"sub ax, word ptr \[<OFFSET\d+>\]",
                    "recomp_text": "sub ax, word ptr [bonusOffset[1] (OFFSET)]",
                    "base_text": "sub ax, word ptr [bonusOffset (DATA)]",
                    "pdb_name": "bonusOffset",
                    "orig_base": 0x00497084,
                    "data": b"\x10\x00\x10\x00",
                    "count": 1,
                },
            ),
        },
    }

    try:
        orig_start = norm_addr(match["address"])
        recomp_start = norm_addr(match["recomp"])
    except (AttributeError, KeyError, TypeError, ValueError):
        return False
    candidate = candidates.get(orig_start)
    if candidate is None or match.get("name") != candidate["name"]:
        return False

    collected = collect_addressed_function_pairs(match.get("diff"))
    if collected is None:
        return False
    records, table_rows, table_markers = collected
    if (
        not records
        or len(table_rows) != candidate["json_table_rows"]
        or table_markers != (1 if table_rows else 0)
    ):
        return False

    orig_addresses = [record["orig_address"] for record in records]
    recomp_addresses = [record["recomp_address"] for record in records]
    if (
        orig_addresses != sorted(orig_addresses)
        or recomp_addresses != sorted(recomp_addresses)
        or len(set(orig_addresses)) != len(orig_addresses)
        or len(set(recomp_addresses)) != len(recomp_addresses)
    ):
        return False
    if any(
        orig_address - orig_start != recomp_address - recomp_start
        for orig_address, recomp_address in zip(orig_addresses, recomp_addresses)
    ) or any(
        orig_address - orig_start != recomp_address - recomp_start
        for orig_address, recomp_address, _ in table_rows
    ):
        return False

    try:
        original_image = load_original_image()
        recompiled_image = load_recompiled_image()
    except (IndexError, LookupError, OSError, RuntimeError, ValueError):
        return False
    decoded_orig = decode_exact_image_span(
        original_image, orig_start, candidate["size"]
    )
    decoded_recomp = decode_exact_image_span(
        recompiled_image, recomp_start, candidate["size"]
    )
    if (
        decoded_orig is None
        or decoded_recomp is None
        or len(decoded_orig) != candidate["instruction_count"]
        or len(decoded_recomp) != candidate["instruction_count"]
    ):
        return False

    orig_index = {
        address: index for index, (address, _, _, _) in enumerate(decoded_orig)
    }
    recomp_index = {
        address: index for index, (address, _, _, _) in enumerate(decoded_recomp)
    }
    paired_by_index: dict[int, dict] = {}
    for record in records:
        left_index = orig_index.get(record["orig_address"])
        right_index = recomp_index.get(record["recomp_address"])
        if (
            left_index is None
            or left_index != right_index
            or left_index in paired_by_index
        ):
            return False
        paired_by_index[left_index] = record

    first_visible = candidate["first_visible_index"]
    visible_indices = candidate.get(
        "visible_indices",
        tuple(range(first_visible, candidate["instruction_count"])),
    )
    if tuple(sorted(paired_by_index)) != visible_indices:
        return False

    expected_call_indices = candidate["call_indices"]
    if (
        tuple(
            index
            for index, (_, _, mnemonic, _) in enumerate(decoded_orig)
            if mnemonic == "call"
        )
        != expected_call_indices
        or tuple(
            index
            for index, (_, _, mnemonic, _) in enumerate(decoded_recomp)
            if mnemonic == "call"
        )
        != expected_call_indices
    ):
        return False
    draw_anim_address = unique_recompiled_symbol_address(
        "?DrawAnim@LemmingAnimsManager@@QAEXFFKKKPAVRemap@@@Z"
    )
    if draw_anim_address is None:
        return False
    data_refs = {
        data_ref["index"]: data_ref
        for data_ref in candidate.get("data_refs", ())
    }
    if len(data_refs) != len(candidate.get("data_refs", ())):
        return False

    alias_records: list[list[dict]] = [[] for _ in candidate["fields"]]
    for record in records:
        if not record["different"]:
            continue
        matching_fields = [
            index
            for index, field in enumerate(candidate["fields"])
            if re.fullmatch(field["orig_pattern"], record["orig_text"])
            and record["recomp_text"] == field["recomp_text"]
        ]
        if len(matching_fields) == 1:
            alias_records[matching_fields[0]].append(record)
            continue
        if matching_fields or not is_equivalent_insn(
            record["orig_text"], record["recomp_text"]
        ):
            return False

    recomp_call_targets: set[int] = set()
    for index, (orig_instruction, recomp_instruction) in enumerate(
        zip(decoded_orig, decoded_recomp)
    ):
        orig_address, orig_size, orig_mnemonic, orig_operands = orig_instruction
        recomp_address, recomp_size, recomp_mnemonic, recomp_operands = (
            recomp_instruction
        )
        if (
            orig_address - orig_start != recomp_address - recomp_start
            or orig_size != recomp_size
            or orig_mnemonic != recomp_mnemonic
        ):
            return False

        orig_text = f"{orig_mnemonic} {orig_operands}".strip()
        recomp_text = f"{recomp_mnemonic} {recomp_operands}".strip()
        record = paired_by_index.get(index)
        if orig_mnemonic == "call":
            if (
                record is None
                or index not in expected_call_indices
                or re.fullmatch(r"call <OFFSET\d+>", record["orig_text"])
                is None
                or record["recomp_text"]
                != "call LemmingAnimsManager::DrawAnim (FUNCTION)"
                or orig_operands != "0x4016a9"
                or not direct_call_reaches_symbol(
                    recompiled_image,
                    recomp_instruction,
                    draw_anim_address,
                )
            ):
                return False
            recomp_call_targets.add(int(recomp_operands, 16))
            continue

        data_ref = data_refs.get(index)
        if data_ref is not None:
            orig_match = re.fullmatch(
                r"mov ebx, dword ptr \[edi\*4 \+ (0x[0-9a-f]+)\]",
                orig_text,
            )
            recomp_match = re.fullmatch(
                r"mov ebx, dword ptr \[edi\*4 \+ (0x[0-9a-f]+)\]",
                recomp_text,
            )
            expected_recomp_target = unique_recompiled_symbol_address(
                data_ref["pdb_name"]
            )
            if (
                record is None
                or record["different"]
                or record["orig_text"] != data_ref["orig_text"]
                or record["recomp_text"] != data_ref["recomp_text"]
                or orig_match is None
                or recomp_match is None
                or int(orig_match.group(1), 16) != data_ref["orig_target"]
                or expected_recomp_target is None
                or int(recomp_match.group(1), 16)
                != expected_recomp_target
            ):
                return False
            try:
                orig_data = bytes(
                    original_image.read(
                        data_ref["orig_target"], len(data_ref["data"])
                    )
                )
                recomp_data = bytes(
                    recompiled_image.read(
                        expected_recomp_target, len(data_ref["data"])
                    )
                )
            except (IndexError, LookupError, OSError, RuntimeError, ValueError):
                return False
            if not is_exact_indexed_static_data_reference(
                record["orig_text"],
                record["recomp_text"],
                re.escape(data_ref["orig_text"]),
                data_ref["recomp_text"],
                int(orig_match.group(1), 16),
                data_ref["orig_target"],
                int(recomp_match.group(1), 16),
                expected_recomp_target,
                orig_data,
                recomp_data,
                data_ref["data"],
            ):
                return False
            continue

        if orig_text == recomp_text:
            continue
        if orig_mnemonic.startswith("j") and re.fullmatch(
            r"0x[0-9a-f]+", orig_operands
        ):
            if not re.fullmatch(r"0x[0-9a-f]+", recomp_operands) or (
                int(orig_operands, 16) - orig_start
                != int(recomp_operands, 16) - recomp_start
            ):
                return False
            continue

        if record is None:
            return False
        if absolute_instruction_shape(orig_text) != absolute_instruction_shape(
            recomp_text
        ):
            return False

    if len(recomp_call_targets) != 1:
        return False

    recomp_bases: list[int] = []
    for field_index, field in enumerate(candidate["fields"]):
        aliases = alias_records[field_index]
        bases = [
            record
            for record in records
            if record["orig_text"] == field["base_text"]
            and record["recomp_text"] == field["base_text"]
        ]
        if len(aliases) != field["count"] or len(bases) != field["count"]:
            return False
        if len({record["orig_text"] for record in aliases}) != 1:
            return False

        orig_base_targets: list[int] = []
        recomp_base_targets: list[int] = []
        for record in bases:
            index = orig_index[record["orig_address"]]
            orig_text = (
                f"{decoded_orig[index][2]} {decoded_orig[index][3]}".strip()
            )
            recomp_text = (
                f"{decoded_recomp[index][2]} {decoded_recomp[index][3]}".strip()
            )
            orig_target = direct_word_target(orig_text, field["base_text"])
            recomp_target = direct_word_target(recomp_text, field["base_text"])
            if orig_target is None or recomp_target is None:
                return False
            orig_base_targets.append(orig_target)
            recomp_base_targets.append(recomp_target)

        if set(orig_base_targets) != {field["orig_base"]} or len(
            set(recomp_base_targets)
        ) != 1:
            return False
        recomp_base = recomp_base_targets[0]
        if recomp_base != unique_recompiled_symbol_address(field["pdb_name"]):
            return False
        recomp_bases.append(recomp_base)
        try:
            orig_data = bytes(original_image.read(field["orig_base"], 4))
            recomp_data = bytes(recompiled_image.read(recomp_base, 4))
        except (IndexError, OSError, RuntimeError, ValueError):
            return False

        for record in aliases:
            index = orig_index[record["orig_address"]]
            orig_text = (
                f"{decoded_orig[index][2]} {decoded_orig[index][3]}".strip()
            )
            recomp_text = (
                f"{decoded_recomp[index][2]} {decoded_recomp[index][3]}".strip()
            )
            orig_target = direct_word_target(orig_text, record["orig_text"])
            recomp_target = direct_word_target(recomp_text, record["recomp_text"])
            if orig_target is None or recomp_target is None:
                return False
            if not is_exact_adjacent_short_field_alias(
                record["orig_text"],
                record["recomp_text"],
                field["orig_pattern"],
                field["recomp_text"],
                field["orig_base"],
                recomp_base,
                orig_target,
                recomp_target,
                orig_data,
                recomp_data,
                field["data"],
            ):
                return False

    if len(set(recomp_bases)) != len(recomp_bases):
        return False

    indirect_orig: dict[int, list[int]] = {}
    indirect_recomp: dict[int, list[int]] = {}
    indirect_indices = [
        index
        for index, (_, _, mnemonic, operands) in enumerate(decoded_orig)
        if mnemonic == "jmp" and not re.fullmatch(r"0x[0-9a-f]+", operands)
    ]
    if candidate["jump_table"]:
        if len(indirect_indices) != 1:
            return False
        index = indirect_indices[0]
        orig_operands = decoded_orig[index][3]
        recomp_operands = decoded_recomp[index][3]
        orig_table_match = re.fullmatch(
            r"dword ptr \[eax\*4 \+ (0x[0-9a-f]+)\]", orig_operands
        )
        recomp_table_match = re.fullmatch(
            r"dword ptr \[eax\*4 \+ (0x[0-9a-f]+)\]", recomp_operands
        )
        if orig_table_match is None or recomp_table_match is None:
            return False
        orig_table = int(orig_table_match.group(1), 16)
        recomp_table = int(recomp_table_match.group(1), 16)
        if (
            orig_table != orig_start + candidate["size"]
            or recomp_table != recomp_start + candidate["size"]
        ):
            return False
        count = len(candidate["jump_table"])
        orig_offsets = read_pointer_table_offsets(
            original_image, orig_table, count, orig_start
        )
        recomp_offsets = read_pointer_table_offsets(
            recompiled_image, recomp_table, count, recomp_start
        )
        if (
            orig_offsets != candidate["jump_table"]
            or recomp_offsets != candidate["jump_table"]
        ):
            return False
        indirect_orig[index] = [orig_start + offset for offset in orig_offsets]
        indirect_recomp[index] = [
            recomp_start + offset for offset in recomp_offsets
        ]
    elif indirect_indices or any(
        mnemonic == "jmp" and not re.fullmatch(r"0x[0-9a-f]+", operands)
        for _, _, mnemonic, operands in decoded_recomp
    ):
        return False

    return complete_control_flow_is_reachable(
        decoded_orig, orig_start, indirect_orig
    ) and complete_control_flow_is_reachable(
        decoded_recomp, recomp_start, indirect_recomp
    )


def run_selftest() -> int:
    def make_diff(orig: list[str], recomp: list[str]):
        return [["@@ selftest @@", [{"orig": [["", x] for x in orig], "recomp": [["", x] for x in recomp]}]]]

    failed = 0
    strict_target_identities = (
        (0x0043CFA0, "C2D::DrawDuplicator"),
        (0x0043D0F0, "C2D::DrawTimeBonus"),
        (0x0043D370, "C2D::DrawSheep"),
        (0x0043D420, "C2D::DrawBall"),
        (0x0043D4B0, "C2D::DrawKey"),
        (0x0043D7E0, "C2D::DrawSwitch"),
        (0x0043D8D0, "C2D::DrawFlag"),
        (0x0043D950, "C2D::DrawBonus"),
        (0x004621E0, "BaseNetwork::CtoSRequestConnect"),
    )
    wrong_target_diff = make_diff(
        ["call <OFFSET1>"],
        ["call CompletelyWrong::Target (FUNCTION)"],
    )
    for address, name in strict_target_identities:
        unsafe_match = {
            "address": hex(address),
            "name": name,
            "matching": 0.25,
            "recomp": "0x1",
            "diff": wrong_target_diff,
        }
        ratio, _ = compute_ratio(unsafe_match, name)
        if ratio == 100.0:
            print(
                "selftest fail: strict candidate accepted an arbitrary call "
                f"target: {name}"
            )
            failed += 1

    for unsafe_match in (
        {
            "address": "0x43cfa0",
            "name": "Unrelated::Function",
            "matching": 0.25,
            "recomp": "0x1",
            "diff": wrong_target_diff,
        },
        {
            "address": "0x43cfa1",
            "name": "C2D::DrawDuplicator",
            "matching": 0.25,
            "recomp": "0x1",
            "diff": wrong_target_diff,
        },
    ):
        ratio, _ = compute_ratio(unsafe_match)
        if ratio == 100.0:
            print(
                "selftest fail: DrawDuplicator identity mutation reached "
                "generic equivalence"
            )
            failed += 1

    wrong_sheep_table_match = {
        "address": "0x43d370",
        "name": "C2D::DrawSheep",
        "matching": 0.25,
        "recomp": "0x1",
        "diff": make_diff(
            ["mov ebx, dword ptr [edi*4 + <OFFSET4>]"],
            [
                "mov ebx, dword ptr "
                "[edi*4 + CompletelyWrong::Table (DATA)]"
            ],
        ),
    }
    ratio, _ = compute_ratio(wrong_sheep_table_match, "C2D::DrawSheep")
    if ratio == 100.0:
        print(
            "selftest fail: DrawSheep accepted an arbitrary resource table"
        )
        failed += 1

    thunk_samples = (
        ("call Thing::Run (FUNCTION)", "call Thunk of 'Thing::Run' (THUNK)", True),
        (
            "jmp Thing::`scalar deleting destructor' (FUNCTION)",
            "jmp Thunk of 'Thing::`scalar deleting destructor'' (THUNK)",
            True,
        ),
        ("call <OFFSET1>", "call Thunk of 'Thing::Run' (THUNK)", True),
        ("call <OFFSET1>", "call _fopen (UNK)", True),
        ("mov dword ptr [esi], <OFFSET2>", "mov dword ptr [esi], Thing::`vftable' (VTABLE)", True),
        (
            "mov dword ptr [esi], Thing (VTABLE)",
            "mov dword ptr [esi], Thing::`vftable'{for `Base'} (VTABLE)",
            True,
        ),
        (
            "mov dword ptr [esi], Thing (VTABLE)",
            "mov dword ptr [edi], Thing::`vftable'{for `Base'} (VTABLE)",
            False,
        ),
        (
            "mov dword ptr [esi], First (VTABLE)",
            "mov dword ptr [esi], Second::`vftable'{for `Base'} (VTABLE)",
            False,
        ),
        (
            "mov dword ptr [esi], Thing::`vftable'{for `Left'} (VTABLE)",
            "mov dword ptr [esi], Thing::`vftable'{for `Right'} (VTABLE)",
            False,
        ),
        ("mov eax, dword ptr [g_left (DATA)]", "mov eax, dword ptr [g_right (DATA)]", False),
        ("call First (FUNCTION)", "call Second (FUNCTION)", False),
    )
    codegen_samples = (
        (
            [
                "test ebp, ebp",
                "jle 0x40",
                "mov eax, dword ptr [esp + 0x34]",
                "sub eax, ebp",
                "shl eax, 2",
                "mov dword ptr [esp + 0x20], eax",
                "mov eax, dword ptr [esp + 0x34]",
                "add eax, ebp",
                "shl eax, 2",
                "mov dword ptr [esp + 0x1c], eax",
                "inc ebx",
                "mov eax, dword ptr [esp + 0x24]",
                "ret",
            ],
            [
                "test ebp, ebp",
                "jle 0x40",
                "mov eax, dword ptr [esp + 0x34]",
                "mov edx, dword ptr [esp + 0x34]",
                "sub eax, ebp",
                "add edx, ebp",
                "shl eax, 2",
                "shl edx, 2",
                "mov dword ptr [esp + 0x20], eax",
                "mov dword ptr [esp + 0x1c], edx",
                "inc ebx",
                "mov eax, dword ptr [esp + 0x24]",
                "ret",
            ],
            True,
        ),
        (
            ["cmp ebx, eax", "jge 0x2c", "call <OFFSET1>"],
            ["cmp eax, ebx", "jle 0x2c", "call Thunk of 'Thing::Run' (THUNK)"],
            True,
        ),
        (
            ["cmp dword ptr [esp + 0x14], ebx", "jg -0x50", "ret"],
            ["cmp ebx, dword ptr [esp + 0x14]", "jl -0x50", "ret"],
            True,
        ),
        (
            ["cmp dword ptr [esi], eax", "jae 0x20", "ret"],
            ["cmp eax, dword ptr [esi]", "jbe 0x20", "ret"],
            True,
        ),
        (
            [
                "cmp dword ptr [esp + 0x14], ebx",
                "lea edx, [eax + eax*2]",
                "mov word ptr [ecx + edx*4 + 8], bp",
                "jg -0x50",
            ],
            [
                "cmp ebx, dword ptr [esp + 0x14]",
                "lea edx, [eax + eax*2]",
                "mov word ptr [ecx + edx*4 + 8], bp",
                "jl -0x50",
            ],
            True,
        ),
        (
            ["cmp dword ptr [esp + 0x14], ebx", "jg -0x50", "ret"],
            ["cmp ebx, dword ptr [esp + 0x14]", "jl -0x40", "ret"],
            False,
        ),
        (
            ["cmp dword ptr [esp + 0x14], ebx", "jg -0x50", "ret"],
            ["cmp ebx, dword ptr [esp + 0x14]", "jle -0x50", "ret"],
            False,
        ),
        (
            ["cmp dword ptr [esp + 0x14], ebx", "jg -0x50", "ret"],
            ["cmp ebx, dword ptr [esp + 0x14]", "jb -0x50", "ret"],
            False,
        ),
        (
            ["cmp dword ptr [esp + 0x14], ebx", "jg -0x50", "ret"],
            ["cmp ebx, dword ptr [esp + 0x18]", "jl -0x50", "ret"],
            False,
        ),
        (
            ["cmp dword ptr [esp + 0x14], ebx", "add ecx, 1", "jg -0x50", "ret"],
            ["cmp ebx, dword ptr [esp + 0x14]", "add ecx, 1", "jl -0x50", "ret"],
            False,
        ),
        (
            ["cmp dword ptr [esp + 0x14], ebx", "call Thing (FUNCTION)", "jg -0x50", "ret"],
            ["cmp ebx, dword ptr [esp + 0x14]", "call Thing (FUNCTION)", "jl -0x50", "ret"],
            False,
        ),
        (
            ["cmp dword ptr [esp + 0x14], ebx", "lea edx, [eax + eax*2]", "jg -0x50", "ret"],
            ["cmp ebx, dword ptr [esp + 0x14]", "lea ecx, [eax + eax*2]", "jl -0x50", "ret"],
            False,
        ),
        (
            ["mov byte ptr [eax + esi + 0x3b0], cl", "call <OFFSET1>"],
            ["mov byte ptr [esi + eax + 0x3b0], cl", "call Thunk of 'Thing::Run' (THUNK)"],
            True,
        ),
        (
            ["push esi", "mov esi, ecx", "mov eax, dword ptr [esi + 4]", "pop esi", "ret"],
            ["push edi", "mov edi, ecx", "mov eax, dword ptr [edi + 4]", "pop edi", "ret"],
            True,
        ),
        (
            [
                "push ebx",
                "push esi",
                "mov esi, dword ptr [ecx]",
                "push edi",
                "push ebp",
                "mov ebx, ecx",
                "xor edi, edi",
                "cmp eax, edi",
                "pop ebp",
                "pop edi",
                "pop esi",
                "pop ebx",
                "ret",
            ],
            [
                "push ebx",
                "push esi",
                "push edi",
                "push ebp",
                "mov esi, dword ptr [ecx]",
                "mov edi, ecx",
                "xor ebx, ebx",
                "cmp eax, ebx",
                "pop ebp",
                "pop edi",
                "pop esi",
                "pop ebx",
                "ret",
            ],
            True,
        ),
        (
            ["xor esi, esi", "cmp ebx, esi", "je 0x2", "ret"],
            ["xor esi, esi", "test ebx, ebx", "je 0x2", "ret"],
            True,
        ),
        (
            ["xor esi, esi", "jne 0x2", "cmp ebx, esi", "je 0x4", "ret"],
            ["xor esi, esi", "jne 0x2", "test ebx, ebx", "je 0x4", "ret"],
            False,
        ),
        (
            [
                "mov edx, dword ptr [ecx + 0x28]",
                "push edi",
                "mov edi, edx",
                "test edi, edi",
                "je 0x2",
            ],
            [
                "mov edx, dword ptr [ecx + 0x28]",
                "test edx, edx",
                "push edi",
                "mov edi, edx",
                "je 0x2",
            ],
            True,
        ),
        (
            ["mov eax, edx", "sub edx, edx", "imul eax, ecx", "div esi"],
            ["imul edx, ecx", "mov eax, edx", "sub edx, edx", "div esi"],
            True,
        ),
        (
            [
                "mov edi, dword ptr [esp + 0x10]",
                "mov ebx, 0x3e8",
                "sub edx, edx",
                "mov eax, dword ptr [edi + 0x24]",
                "sub eax, dword ptr [edi + 0x20]",
                "lea ecx, [eax + eax*2]",
                "lea eax, [ecx + ecx*4]",
                "mov ecx, -1",
                "div ebx",
                "cmp eax, 7",
            ],
            [
                "mov ecx, -1",
                "mov edi, dword ptr [esp + 0x10]",
                "mov ebx, 0x3e8",
                "mov eax, dword ptr [edi + 0x24]",
                "sub eax, dword ptr [edi + 0x20]",
                "lea edx, [eax + eax*2]",
                "lea eax, [edx + edx*4]",
                "sub edx, edx",
                "div ebx",
                "cmp eax, 7",
            ],
            True,
        ),
        (
            [
                "mov edi, dword ptr [esp + 0x10]",
                "mov ebx, 0x3e8",
                "sub edx, edx",
                "mov eax, dword ptr [edi + 0x24]",
                "sub eax, dword ptr [edi + 0x20]",
                "lea ecx, [eax + eax*2]",
                "lea eax, [ecx + ecx*4]",
                "mov ecx, -1",
                "div ebx",
                "cmp eax, 7",
            ],
            [
                "mov ecx, -2",
                "mov edi, dword ptr [esp + 0x10]",
                "mov ebx, 0x3e8",
                "mov eax, dword ptr [edi + 0x24]",
                "sub eax, dword ptr [edi + 0x20]",
                "lea edx, [eax + eax*2]",
                "lea eax, [edx + edx*4]",
                "sub edx, edx",
                "div ebx",
                "cmp eax, 7",
            ],
            False,
        ),
        (
            [
                "mov edi, dword ptr [esp + 0x10]",
                "mov ebx, 0x3e8",
                "sub edx, edx",
                "mov eax, dword ptr [edi + 0x24]",
                "sub eax, dword ptr [edi + 0x20]",
                "lea ecx, [eax + eax*2]",
                "lea eax, [ecx + ecx*4]",
                "mov ecx, -1",
                "div ebx",
                "cmp eax, 7",
            ],
            [
                "mov ecx, -1",
                "mov edi, dword ptr [esp + 0x10]",
                "mov ebx, 0x3e8",
                "mov eax, dword ptr [edi + 0x24]",
                "sub eax, dword ptr [edi + 0x20]",
                "lea edx, [eax + eax*2]",
                "lea eax, [edx + edx*2]",
                "sub edx, edx",
                "div ebx",
                "cmp eax, 7",
            ],
            False,
        ),
        (
            [
                "mov edi, dword ptr [esp + 0x10]",
                "mov ebx, 0x3e8",
                "mov esi, ecx",
                "sub edx, edx",
                "mov eax, dword ptr [edi + 0x24]",
                "sub eax, dword ptr [edi + 0x20]",
                "lea ecx, [eax + eax*2]",
                "lea eax, [ecx + ecx*4]",
                "mov ecx, -1",
                "div ebx",
                "cmp eax, 7",
            ],
            [
                "mov ecx, -1",
                "mov edi, dword ptr [esp + 0x10]",
                "mov ebx, 0x3e8",
                "mov esi, ecx",
                "mov eax, dword ptr [edi + 0x24]",
                "sub eax, dword ptr [edi + 0x20]",
                "lea edx, [eax + eax*2]",
                "lea eax, [edx + edx*4]",
                "sub edx, edx",
                "div ebx",
                "cmp eax, 7",
            ],
            False,
        ),
        (
            [
                "mov edi, dword ptr [esp + 0x10]",
                "mov ebx, 0x3e8",
                "sub edx, edx",
                "mov eax, dword ptr [edi + 0x24]",
                "sub eax, dword ptr [edi + 0x20]",
                "lea ecx, [eax + eax*2]",
                "lea eax, [ecx + ecx*4]",
                "mov ecx, -1",
                "div ebx",
                "cmp eax, 7",
            ],
            [
                "mov ecx, -1",
                "mov edi, dword ptr [esp + 0x10]",
                "mov ebx, 0x3e8",
                "mov eax, dword ptr [edi + 0x24]",
                "sub eax, dword ptr [edi + 0x20]",
                "lea edx, [eax + eax*2]",
                "lea eax, [edx + edx*4]",
                "sub edx, edx",
                "div esi",
                "cmp eax, 7",
            ],
            False,
        ),
        (
            ["mov esi, 1", "cmp ebx, esi", "je 0x2", "ret"],
            ["mov esi, 1", "test ebx, ebx", "je 0x2", "ret"],
            False,
        ),
        (
            ["mov eax, dword ptr [g_left (DATA)]", "ret"],
            ["mov eax, dword ptr [g_right (DATA)]", "ret"],
            False,
        ),
        (
            ["mov al, byte ptr [g_mask (DATA)]", "or byte ptr [g_bits (DATA)], al", "ret", "nop"],
            ["mov cl, byte ptr [g_mask (DATA)]", "or byte ptr [g_bits (DATA)], cl", "ret"],
            True,
        ),
        (
            ["mov al, byte ptr [g_mask (DATA)]", "or byte ptr [g_bits (DATA)], al", "ret"],
            ["mov cl, byte ptr [g_mask (DATA)]", "or byte ptr [g_bits (DATA)], al", "ret"],
            False,
        ),
        (
            ["mov eax, 1", "nop"],
            ["mov eax, 1"],
            False,
        ),
        (
            [
                "sub esp, 4",
                "push esi",
                "mov esi, dword ptr [esp + 0xc]",
                "pop esi",
            ],
            ["push esi", "mov esi, dword ptr [esp + 8]", "pop esi", "ret 4"],
            True,
        ),
        (
            ["sub esp, 4", "mov dword ptr [esp], eax"],
            ["mov dword ptr [esp], eax", "ret"],
            False,
        ),
        (
            [
                "mov esi, dword ptr [ecx + 0x1c]",
                "cmp eax, esi",
                "jge 0x12",
                "imul edx, esi",
                "add edx, dword ptr [ecx + 0x48]",
                "pop esi",
                "mov al, byte ptr [edx + eax]",
                "ret 8",
            ],
            [
                "mov esi, dword ptr [ecx + 0x1c]",
                "cmp esi, eax",
                "jle 0x12",
                "imul esi, edx",
                "add esi, dword ptr [ecx + 0x48]",
                "mov al, byte ptr [esi + eax]",
                "pop esi",
                "ret 8",
            ],
            True,
        ),
        (
            [
                "xor esi, esi",
                "push ebp",
                "mov edi, ecx",
                "mov eax, dword ptr [edi + 0x3c]",
                "mov ecx, dword ptr [eax + esi]",
                "add esi, 4",
            ],
            [
                "mov esi, ecx",
                "push ebp",
                "xor edi, edi",
                "mov eax, dword ptr [esi + 0x3c]",
                "mov ecx, dword ptr [eax + edi]",
                "add edi, 4",
            ],
            True,
        ),
        (
            [
                "xor esi, esi",
                "xor ebx, ebx",
                "cmp dword ptr [edi + 0x18], esi",
                "jle 0x19",
                "mov ecx, dword ptr [eax + esi]",
                "add esi, 4",
                "inc ebx",
                "cmp dword ptr [edi + 0x18], ebx",
            ],
            [
                "xor esi, esi",
                "cmp dword ptr [edi + 0x18], esi",
                "jle 0x1b",
                "xor ebx, ebx",
                "mov ecx, dword ptr [eax + ebx]",
                "add ebx, 4",
                "inc esi",
                "cmp dword ptr [edi + 0x18], esi",
            ],
            True,
        ),
        (
            ["mov eax, ebx", "add ebx, 1", "or edx, ebx", "ret"],
            ["add esi, 1", "mov eax, esi", "or edx, esi", "ret"],
            False,
        ),
        (
            [
                "mov eax, dword ptr [esp + 0x14]",
                "push eax",
                "mov dword ptr [ebp + 0x14], eax",
                "call WSOCK32.dll::Ordinal_11 (IMPORT_THUNK)",
            ],
            [
                "mov ecx, dword ptr [esp + 0x14]",
                "push ecx",
                "mov dword ptr [ebp + 0x14], ecx",
                "call WSOCK32.dll::Ordinal_11 (IMPORT_THUNK)",
            ],
            True,
        ),
        (
            [
                "lea esi, [edi - 4]",
                "mov ebx, dword ptr [esi]",
                "dec ebx",
                "push esi",
                "call operator delete (FUNCTION)",
            ],
            [
                "lea ebx, [edi - 4]",
                "mov esi, dword ptr [ebx]",
                "dec esi",
                "push ebx",
                "call Thunk of 'operator delete' (THUNK)",
            ],
            True,
        ),
        (
            [
                "mov esi, dword ptr [g_buckets (DATA)]",
                "mov ebx, dword ptr [esp + 0x10]",
                "mov ecx, dword ptr [esi]",
                "push ebx",
                "call Bucket::CheckValidPointer (FUNCTION)",
                "add esi, 4",
                "mov ebx, dword ptr [esp + 0x10]",
                "push ebx",
                "mov ecx, dword ptr [g_arena (DATA)]",
                "call Arena::CheckValidPointer (FUNCTION)",
                "ret",
            ],
            [
                "mov ebx, dword ptr [g_buckets (DATA)]",
                "mov esi, dword ptr [esp + 0x10]",
                "mov ecx, dword ptr [ebx]",
                "push esi",
                "call Bucket::CheckValidPointer (FUNCTION)",
                "add ebx, 4",
                "mov esi, dword ptr [esp + 0x10]",
                "push esi",
                "mov ecx, dword ptr [g_arena (DATA)]",
                "call Arena::CheckValidPointer (FUNCTION)",
                "ret",
            ],
            True,
        ),
        (
            [
                "lea ebp, [eax + 4]",
                "mov esi, ebp",
                "add esi, 0x54",
                "call ResZrle::ResZrle (FUNCTION)",
                "mov dword ptr [ebx + 0x78], ebp",
                "pop ebp",
                "pop edi",
                "pop esi",
                "ret",
            ],
            [
                "lea esi, [eax + 4]",
                "mov ebp, esi",
                "add ebp, 0x54",
                "call Thunk of 'ResZrle::ResZrle' (THUNK)",
                "pop ebp",
                "mov dword ptr [ebx + 0x78], esi",
                "pop edi",
                "pop esi",
                "ret",
            ],
            True,
        ),
        (
            [
                "lea ebp, [eax + 4]",
                "mov esi, ebp",
                "add esi, 0x54",
                "mov dword ptr [esp + 4], eax",
                "pop ebp",
                "pop esi",
                "ret",
            ],
            [
                "lea esi, [eax + 4]",
                "mov ebp, esi",
                "add ebp, 0x54",
                "pop ebp",
                "mov dword ptr [esp + 4], eax",
                "pop esi",
                "ret",
            ],
            False,
        ),
        (
            [
                "lea ebp, [eax + 4]",
                "mov esi, ebp",
                "add esi, 0x54",
                "call Thing (FUNCTION)",
                "pop ebp",
                "pop esi",
                "ret",
            ],
            [
                "lea esi, [eax + 4]",
                "mov ebp, esi",
                "add ebp, 0x54",
                "pop ebp",
                "call Thing (FUNCTION)",
                "pop esi",
                "ret",
            ],
            False,
        ),
        (
            ["mov esi, eax", "push esi", "add ebx, esi", "ret"],
            ["mov ebx, eax", "push ebx", "add esi, ebx", "ret"],
            False,
        ),
        (
            ["mov eax, dword ptr [esp + 4]", "push eax", "mov dword ptr [ebp], eax", "ret"],
            ["mov ecx, dword ptr [esp + 4]", "push ecx", "mov dword ptr [ebp], ecx", "ret"],
            False,
        ),
        (
            [
                "mov eax, dword ptr [esp + 4]",
                "push eax",
                "mov dword ptr [ebp], eax",
                "call First (FUNCTION)",
            ],
            [
                "mov ecx, dword ptr [esp + 4]",
                "push ecx",
                "mov dword ptr [ebp], ecx",
                "call Second (FUNCTION)",
            ],
            False,
        ),
        (
            ["xor ebx, ebx", "cmp eax, ecx", "jle 0x19", "ret"],
            ["cmp eax, ecx", "jle 0x1d", "xor ebx, ebx", "ret"],
            False,
        ),
    )

    def make_stack_offset_chains(
        primary: str = "eax",
        transient: str = "edx",
        source: str = "dword ptr [esp + 0x34]",
        first_destination: str = "dword ptr [esp + 0x20]",
        second_destination: str = "dword ptr [esp + 0x1c]",
    ) -> tuple[list[str], list[str]]:
        sequential = [
            f"mov {primary}, {source}",
            f"sub {primary}, ebp",
            f"shl {primary}, 2",
            f"mov {first_destination}, {primary}",
            f"mov {primary}, {source}",
            f"add {primary}, ebp",
            f"shl {primary}, 2",
            f"mov {second_destination}, {primary}",
        ]
        interleaved = [
            f"mov {primary}, {source}",
            f"mov {transient}, {source}",
            f"sub {primary}, ebp",
            f"add {transient}, ebp",
            f"shl {primary}, 2",
            f"shl {transient}, 2",
            f"mov {first_destination}, {primary}",
            f"mov {second_destination}, {transient}",
        ]
        return sequential, interleaved

    def replace_instruction(instructions: list[str], index: int, text: str) -> list[str]:
        replaced = list(instructions)
        replaced[index] = text
        return replaced

    def move_instruction(instructions: list[str], source: int, destination: int) -> list[str]:
        moved = list(instructions)
        instruction = moved.pop(source)
        moved.insert(destination, instruction)
        return moved

    process_role_orig = [
        "push ebx",
        "push esi",
        "push edi",
        "mov ebx, ecx",
        "xor edi, edi",
        "xor esi, esi",
        "cmp dword ptr [ebx + 0x34], edi",
        "jle 0x16",
        "mov ecx, dword ptr [ebx + 0x38]",
        "inc esi",
        "add ecx, edi",
        "add edi, 0x188",
        "mov eax, dword ptr [ecx]",
        "call dword ptr [eax + 0x14]",
        "cmp dword ptr [ebx + 0x34], esi",
        "jg -0x16",
        "pop edi",
        "pop esi",
        "pop ebx",
        "ret",
    ]
    process_role_recomp = [
        "push ebx",
        "push esi",
        "push edi",
        "xor esi, esi",
        "mov edi, ecx",
        "xor ebx, ebx",
        "cmp dword ptr [edi + 0x34], esi",
        "jle 0x16",
        "mov ecx, dword ptr [edi + 0x38]",
        "inc ebx",
        "add ecx, esi",
        "add esi, 0x188",
        "mov eax, dword ptr [ecx]",
        "call dword ptr [eax + 0x14]",
        "cmp dword ptr [edi + 0x34], ebx",
        "jg -0x16",
        "pop edi",
        "pop esi",
        "pop ebx",
        "ret",
    ]

    unsafe_process_role_rotations = (
        (
            "inconsistent register role",
            replace_instruction(process_role_recomp, 9, "inc esi"),
        ),
        (
            "mapped word subregister",
            replace_instruction(process_role_recomp, 5, "xor bx, bx"),
        ),
        (
            "zero moved across a role use",
            move_instruction(process_role_recomp, 3, 10),
        ),
        (
            "zero moved across a memory comparison",
            move_instruction(process_role_recomp, 3, 6),
        ),
        (
            "zero moved across a call",
            move_instruction(process_role_recomp, 3, 14),
        ),
        (
            "zero moved across a stack operation",
            move_instruction(process_role_recomp, 3, 0),
        ),
        (
            "zero moved across arithmetic",
            move_instruction(process_role_recomp, 3, 11),
        ),
        (
            "zero moved across a flag consumer",
            move_instruction(process_role_recomp, 3, 8),
        ),
        (
            "different count member offset",
            replace_instruction(
                process_role_recomp, 6, "cmp dword ptr [edi + 0x30], esi"
            ),
        ),
        (
            "different object stride",
            replace_instruction(process_role_recomp, 11, "add esi, 0x184"),
        ),
        (
            "different virtual slot",
            replace_instruction(
                process_role_recomp, 13, "call dword ptr [eax + 0x10]"
            ),
        ),
        (
            "different branch opcode",
            replace_instruction(process_role_recomp, 7, "jl 0x16"),
        ),
        (
            "different branch target",
            replace_instruction(process_role_recomp, 7, "jle 0x18"),
        ),
        ("different instruction count", process_role_recomp[:-1]),
        (
            "different saved register",
            replace_instruction(process_role_recomp, 0, "push ebp"),
        ),
        (
            "different restored register",
            replace_instruction(process_role_recomp, 16, "pop ebp"),
        ),
        (
            "different return",
            replace_instruction(process_role_recomp, 19, "ret 4"),
        ),
        (
            "wrong role in final comparison",
            replace_instruction(
                process_role_recomp, 14, "cmp dword ptr [edi + 0x34], esi"
            ),
        ),
        ("missing zero definition", process_role_recomp[:5] + process_role_recomp[6:]),
        (
            "different zero definition",
            replace_instruction(process_role_recomp, 5, "sub ebx, ebx"),
        ),
    )

    for description, recomp in unsafe_process_role_rotations:
        if is_codegen_equivalent_diff(make_diff(process_role_orig, recomp)):
            print(f"selftest fail: unsafe full-role rotation accepted: {description}")
            failed += 1

    if not is_codegen_equivalent_diff(make_diff(process_role_orig, process_role_recomp)):
        print("selftest fail: safe exact full-role rotation rejected")
        failed += 1

    duplicator_role_orig = [
        "sub esp, 8",
        "mov dword ptr [esp], ecx",
        "push ebx",
        "mov ecx, dword ptr [esp + 0x10]",
        "push esi",
        "push edi",
        "push ebp",
        "mov eax, dword ptr [ecx + 0x24]",
        "mov ebp, dword ptr [ecx + 0x18]",
        "sub eax, dword ptr [ecx + 0x20]",
        "mov ebx, dword ptr [ecx + 4]",
        "sub ebx, 0x1c",
        "mov esi, dword ptr [ecx + 8]",
        "sub esi, 0x3f",
        "xor edi, edi",
        "cmp word ptr [ecx + 0x1c], di",
        "mov dword ptr [esp + 0x14], eax",
        "je 0xa",
        "mov eax, dword ptr [esp + 0x10]",
        "mov edi, dword ptr [eax + 0x968]",
        "mov eax, dword ptr [esp + 0x10]",
        "push edi",
        "push 0",
        "push 0",
        "push 0x89",
        "mov ecx, dword ptr [eax + 0xa40]",
        "push esi",
        "push ebx",
        "call DRAW_ANIM",
        "cmp ebp, 0x18",
        "je 0xf",
        "cmp ebp, 0x1a",
        "je 0x2f",
        "pop ebp",
        "pop edi",
        "pop esi",
        "pop ebx",
        "add esp, 8",
        "ret 4",
        "mov eax, dword ptr [esp + 0x10]",
        "push edi",
        "push 0",
        "push 0x3f",
        "push 0x89",
        "mov ecx, dword ptr [eax + 0xa40]",
        "push esi",
        "push ebx",
        "call DRAW_ANIM",
        "pop ebp",
        "pop edi",
        "pop esi",
        "pop ebx",
        "add esp, 8",
        "ret 4",
        "mov eax, dword ptr [esp + 0x14]",
        "mov ebp, 0x3e8",
        "sub edx, edx",
        "lea ecx, [eax + eax*2]",
        "lea eax, [ecx + ecx*4]",
        "div ebp",
        "cmp eax, 0x3e",
        "jle 0x5",
        "mov eax, 0x3e",
        "push edi",
        "inc eax",
        "mov edx, dword ptr [esp + 0x14]",
        "push 0",
        "push eax",
        "push 0x89",
        "push esi",
        "mov ecx, dword ptr [edx + 0xa40]",
        "push ebx",
        "call DRAW_ANIM",
        "pop ebp",
        "pop edi",
        "pop esi",
        "pop ebx",
        "add esp, 8",
        "ret 4",
    ]
    physical_save_restore = {2, 4, 35, 36, 50, 51, 75, 76}
    duplicator_role_recomp = [
        (
            instruction
            if index in physical_save_restore
            else REGISTER_RE.sub(
                lambda match: {"ebx": "esi", "esi": "ebx"}.get(
                    match.group(0), match.group(0)
                ),
                instruction,
            )
        )
        for index, instruction in enumerate(duplicator_role_orig)
    ]

    def insert_instruction(
        instructions: list[str], index: int, text: str
    ) -> list[str]:
        inserted = list(instructions)
        inserted.insert(index, text)
        return inserted

    hidden_role_orig = replace_instruction(
        duplicator_role_orig, 58, "lea eax, [ebx + ecx*4]"
    )
    hidden_role_recomp = replace_instruction(
        duplicator_role_recomp, 58, "lea eax, [ebx + ecx*4]"
    )
    opaque_hidden_orig = replace_instruction(duplicator_role_orig, 58, "db 0xff")
    opaque_hidden_recomp = replace_instruction(
        duplicator_role_recomp, 58, "db 0xff"
    )
    long_call_gap_orig = insert_instruction(
        insert_instruction(duplicator_role_orig, 28, "mov ecx, eax"),
        28,
        "mov edx, eax",
    )
    long_call_gap_recomp = insert_instruction(
        insert_instruction(duplicator_role_recomp, 28, "mov ecx, eax"),
        28,
        "mov edx, eax",
    )
    mapped_call_gap_orig = insert_instruction(
        duplicator_role_orig, 28, "mov eax, esi"
    )
    mapped_call_gap_recomp = insert_instruction(
        duplicator_role_recomp, 28, "mov eax, ebx"
    )

    unsafe_duplicator_role_rotations = (
        (
            "inconsistent two-way role map",
            duplicator_role_orig,
            replace_instruction(duplicator_role_recomp, 11, "sub ebx, 0x1c"),
        ),
        (
            "mapped word subregister",
            duplicator_role_orig,
            replace_instruction(duplicator_role_recomp, 11, "sub si, 0x1c"),
        ),
        (
            "esp participates in role map",
            duplicator_role_orig,
            replace_instruction(
                duplicator_role_recomp, 10, "mov esp, dword ptr [ecx + 4]"
            ),
        ),
        (
            "role register in hidden equal context",
            hidden_role_orig,
            hidden_role_recomp,
        ),
        ("opaque hidden context", opaque_hidden_orig, opaque_hidden_recomp),
        (
            "missing mapped register save",
            duplicator_role_orig,
            replace_instruction(duplicator_role_recomp, 2, "push eax"),
        ),
        (
            "duplicate mapped register save",
            duplicator_role_orig,
            insert_instruction(duplicator_role_recomp, 4, "push ebx"),
        ),
        (
            "mapped register use before save",
            insert_instruction(duplicator_role_orig, 2, "mov eax, ebx"),
            insert_instruction(duplicator_role_recomp, 2, "mov eax, ebx"),
        ),
        (
            "wrong restore on one exit",
            duplicator_role_orig,
            replace_instruction(duplicator_role_recomp, 35, "pop ebp"),
        ),
        (
            "wrong stack adjustment on one exit",
            duplicator_role_orig,
            replace_instruction(duplicator_role_recomp, 37, "add esp, 4"),
        ),
        (
            "wrong return immediate on one exit",
            duplicator_role_orig,
            replace_instruction(duplicator_role_recomp, 38, "ret 8"),
        ),
        (
            "more than two instructions before call",
            long_call_gap_orig,
            long_call_gap_recomp,
        ),
        (
            "mapped role used between push and call",
            mapped_call_gap_orig,
            mapped_call_gap_recomp,
        ),
        (
            "stack pop between push and call",
            insert_instruction(duplicator_role_orig, 28, "pop eax"),
            insert_instruction(duplicator_role_recomp, 28, "pop eax"),
        ),
        (
            "memory store between push and call",
            insert_instruction(duplicator_role_orig, 28, "mov dword ptr [eax], ecx"),
            insert_instruction(
                duplicator_role_recomp, 28, "mov dword ptr [eax], ecx"
            ),
        ),
        (
            "flag-setting arithmetic between push and call",
            insert_instruction(duplicator_role_orig, 28, "add eax, 1"),
            insert_instruction(duplicator_role_recomp, 28, "add eax, 1"),
        ),
        (
            "branch between push and call",
            insert_instruction(duplicator_role_orig, 28, "jne 0x4"),
            insert_instruction(duplicator_role_recomp, 28, "jne 0x4"),
        ),
        (
            "different resolved call target",
            duplicator_role_orig,
            replace_instruction(duplicator_role_recomp, 72, "call OTHER_ANIM"),
        ),
        (
            "wrong role in call arguments",
            duplicator_role_orig,
            replace_instruction(duplicator_role_recomp, 71, "push ebx"),
        ),
        (
            "different coordinate member offset",
            duplicator_role_orig,
            replace_instruction(
                duplicator_role_recomp, 10, "mov esi, dword ptr [ecx + 8]"
            ),
        ),
        (
            "different coordinate adjustment",
            duplicator_role_orig,
            replace_instruction(duplicator_role_recomp, 11, "sub esi, 0x1d"),
        ),
        (
            "different branch target",
            duplicator_role_orig,
            replace_instruction(duplicator_role_recomp, 32, "je 0x31"),
        ),
        (
            "genuine instruction scheduling change",
            duplicator_role_orig,
            move_instruction(duplicator_role_recomp, 70, 68),
        ),
        (
            "different instruction count",
            duplicator_role_orig,
            duplicator_role_recomp[:-1],
        ),
    )

    hidden_context_instructions = {
        "lea eax, [ecx + ecx*4]",
        "lea eax, [ebx + ecx*4]",
        "db 0xff",
    }

    for description, orig, recomp in unsafe_duplicator_role_rotations:
        hidden_indices = {
            index
            for index, instruction in enumerate(orig)
            if instruction in hidden_context_instructions
        }
        if is_exact_complete_call_role_rotation(orig, recomp, hidden_indices):
            print(
                "selftest fail: unsafe complete call-role rotation accepted: "
                f"{description}"
            )
            failed += 1

    if not is_exact_complete_call_role_rotation(
        duplicator_role_orig, duplicator_role_recomp, {58}
    ):
        print("selftest fail: safe complete call-role rotation rejected")
        failed += 1

    ctos_role_orig = [
        "push ebx",
        "push esi",
        "mov esi, dword ptr [esp + 0xc]",
        "push edi",
        "mov edi, ecx",
        "push esi",
        "call BaseNetwork::FindConnection (FUNCTION)",
        "test eax, eax",
        "je 0x11",
        "cmp dword ptr [eax + 0x20], 0",
        "jne 0xb",
        "mov ecx, eax",
        "call CConnect::CheckConnectTime (FUNCTION)",
        "test eax, eax",
        "jne 0x6c",
        "mov ecx, edi",
        "call BaseNetwork::NewConnect (FUNCTION)",
        "mov ebx, eax",
        "mov ecx, dword ptr [edi + 0x24]",
        "mov eax, dword ptr [g_pMessReqConnect (DATA)]",
        "mov edx, dword ptr [eax + 0x34]",
        "mov eax, dword ptr [ecx]",
        "push edx",
        "call dword ptr [eax]",
        "cmp ax, 0xffff",
        "jne 0xd",
        "mov ecx, ebx",
        "call CConnect::Kill (FUNCTION)",
        "pop edi",
        "pop esi",
        "pop ebx",
        "ret 4",
        "push eax",
        "push esi",
        "mov eax, dword ptr [g_pMessReqConnect (DATA)]",
        "mov ecx, dword ptr [eax + 0x38]",
        "push ecx",
        "mov ecx, ebx",
        "call CConnect::InitConnect (FUNCTION)",
        "mov ecx, dword ptr [ebx + 4]",
        "mov edx, dword ptr [ecx + 4]",
        "mov ecx, dword ptr [g_pMessOKConnect (DATA)]",
        "mov ax, word ptr [edx + ebx + 0x24]",
        "mov word ptr [ecx + 0x30], ax",
        "mov ecx, dword ptr [g_pMessOKConnect (DATA)]",
        "mov dword ptr [ecx + 0x34], ebx",
        "mov ecx, dword ptr [g_pMessOKConnect (DATA)]",
        "push ecx",
        "push esi",
        "mov ecx, dword ptr [edi + 0x24]",
        "call Broadcast::Send (FUNCTION)",
        "pop edi",
        "pop esi",
        "pop ebx",
        "ret 4",
    ]
    ctos_physical_save_restore = {0, 1, 3, 28, 29, 30, 51, 52, 53}
    ctos_role_recomp = [
        (
            instruction
            if index in ctos_physical_save_restore
            else REGISTER_RE.sub(
                lambda match: {"esi": "edi", "edi": "esi"}.get(
                    match.group(0), match.group(0)
                ),
                instruction,
            )
        )
        for index, instruction in enumerate(ctos_role_orig)
    ]
    ctos_arg_load = ctos_role_recomp.pop(2).replace("0xc", "0x10")
    ctos_role_recomp.insert(4, ctos_arg_load)

    unsafe_ctos_role_swaps = (
        (
            "inconsistent this role",
            replace_instruction(ctos_role_recomp, 15, "mov ecx, edi"),
        ),
        (
            "different broadcast member offset",
            replace_instruction(
                ctos_role_recomp, 18, "mov ecx, dword ptr [esi + 0x28]"
            ),
        ),
        (
            "different request-data member offset",
            replace_instruction(
                ctos_role_recomp, 20, "mov edx, dword ptr [eax + 0x30]"
            ),
        ),
        (
            "different assigned-port store",
            replace_instruction(
                ctos_role_recomp, 43, "mov word ptr [ecx + 0x32], ax"
            ),
        ),
        (
            "different branch opcode",
            replace_instruction(ctos_role_recomp, 10, "je 0xb"),
        ),
        (
            "different branch target",
            replace_instruction(ctos_role_recomp, 14, "jne 0x68"),
        ),
        (
            "different direct call target",
            replace_instruction(
                ctos_role_recomp, 27, "call CConnect::Close (FUNCTION)"
            ),
        ),
        (
            "different virtual call slot",
            replace_instruction(ctos_role_recomp, 23, "call dword ptr [eax + 4]"),
        ),
        (
            "different stack argument offset",
            replace_instruction(
                ctos_role_recomp, 4, "mov edi, dword ptr [esp + 0x14]"
            ),
        ),
        (
            "argument load scheduled past a side effect",
            move_instruction(ctos_role_recomp, 4, 7),
        ),
        (
            "mapped word subregister",
            replace_instruction(ctos_role_recomp, 33, "push di"),
        ),
        (
            "different saved register",
            replace_instruction(ctos_role_recomp, 2, "push ebp"),
        ),
        (
            "different first-exit restore",
            replace_instruction(ctos_role_recomp, 28, "pop ebp"),
        ),
        (
            "different final restore",
            replace_instruction(ctos_role_recomp, 51, "pop ebp"),
        ),
        (
            "different return immediate",
            replace_instruction(ctos_role_recomp, 54, "ret 8"),
        ),
        (
            "wrong connection-address argument",
            replace_instruction(ctos_role_recomp, 33, "push esi"),
        ),
        (
            "different instruction count",
            ctos_role_recomp[:-1],
        ),
    )

    for description, recomp in unsafe_ctos_role_swaps:
        if is_exact_ctos_request_role_swap(ctos_role_orig, recomp):
            print(
                "selftest fail: unsafe CtoSRequestConnect role swap accepted: "
                f"{description}"
            )
            failed += 1

    if not is_exact_ctos_request_role_swap(ctos_role_orig, ctos_role_recomp):
        print("selftest fail: safe CtoSRequestConnect role swap rejected")
        failed += 1

    adjacent_alias_args = {
        "orig_text": "sub si, word ptr [<OFFSET2>]",
        "recomp_text": "sub si, word ptr [sheepOffset[1] (OFFSET)]",
        "orig_pattern": r"sub si, word ptr \[<OFFSET\d+>\]",
        "expected_recomp_text": "sub si, word ptr [sheepOffset[1] (OFFSET)]",
        "orig_base": 0x49706C,
        "recomp_base": 0x464988,
        "orig_target": 0x49706E,
        "recomp_target": 0x46498A,
        "orig_data": b"\x09\x00\x08\x00",
        "recomp_data": b"\x09\x00\x08\x00",
        "expected_data": b"\x09\x00\x08\x00",
    }
    unsafe_adjacent_aliases = (
        ("different original opcode", {"orig_text": "add si, word ptr [<OFFSET2>]"}),
        ("different original register", {"orig_text": "sub di, word ptr [<OFFSET2>]"}),
        ("unresolved dword field", {"orig_text": "sub si, dword ptr [<OFFSET2>]"}),
        (
            "different rebuilt symbol",
            {"recomp_text": "sub si, word ptr [otherOffset[1] (OFFSET)]"},
        ),
        (
            "different rebuilt field index",
            {"recomp_text": "sub si, word ptr [sheepOffset[2] (OFFSET)]"},
        ),
        ("original points at base", {"orig_target": 0x49706C}),
        ("original points four bytes past base", {"orig_target": 0x497070}),
        ("rebuilt points at base", {"recomp_target": 0x464988}),
        ("rebuilt points four bytes past base", {"recomp_target": 0x46498C}),
        ("original data differs", {"orig_data": b"\x09\x00\x07\x00"}),
        ("rebuilt data differs", {"recomp_data": b"\x09\x00\x07\x00"}),
        ("field object is not two shorts", {"expected_data": b"\x09\x00"}),
    )
    for description, changes in unsafe_adjacent_aliases:
        candidate_args = dict(adjacent_alias_args)
        candidate_args.update(changes)
        if is_exact_adjacent_short_field_alias(**candidate_args):
            print(
                "selftest fail: unsafe adjacent-short alias accepted: "
                f"{description}"
            )
            failed += 1

    if not is_exact_adjacent_short_field_alias(**adjacent_alias_args):
        print("selftest fail: safe adjacent-short alias rejected")
        failed += 1

    indexed_data_args = {
        "orig_text": "mov ebx, dword ptr [edi*4 + <OFFSET4>]",
        "recomp_text": "mov ebx, dword ptr [edi*4 + sheepMunchResources (DATA)]",
        "orig_pattern": r"mov ebx, dword ptr \[edi\*4 \+ <OFFSET\d+>\]",
        "expected_recomp_text": "mov ebx, dword ptr [edi*4 + sheepMunchResources (DATA)]",
        "orig_target": 0x49EF78,
        "expected_orig_target": 0x49EF78,
        "recomp_target": 0x46BA18,
        "expected_recomp_target": 0x46BA18,
        "orig_data": bytes.fromhex(
            "a9000000a3000000aa000000a5000000"
            "ab000000a7000000ac000000a1000000"
        ),
        "recomp_data": bytes.fromhex(
            "a9000000a3000000aa000000a5000000"
            "ab000000a7000000ac000000a1000000"
        ),
        "expected_data": bytes.fromhex(
            "a9000000a3000000aa000000a5000000"
            "ab000000a7000000ac000000a1000000"
        ),
    }
    unsafe_indexed_data = (
        (
            "wrong rebuilt PDB address",
            {"recomp_target": 0x46BA38},
        ),
        (
            "arbitrary rebuilt table label",
            {
                "recomp_text": "mov ebx, dword ptr "
                "[edi*4 + CompletelyWrong::Table (DATA)]"
            },
        ),
        ("wrong original table", {"orig_target": 0x49EF58}),
        (
            "wrong rebuilt table bytes",
            {"recomp_data": b"\x00" * 32},
        ),
    )
    for description, changes in unsafe_indexed_data:
        candidate_args = dict(indexed_data_args)
        candidate_args.update(changes)
        if is_exact_indexed_static_data_reference(**candidate_args):
            print(
                "selftest fail: unsafe indexed static-data relocation "
                f"accepted: {description}"
            )
            failed += 1
    if not is_exact_indexed_static_data_reference(**indexed_data_args):
        print("selftest fail: safe indexed static-data relocation rejected")
        failed += 1

    class TinyImage:
        def __init__(self, regions: dict[int, bytes]):
            self.regions = regions

        def read(self, address: int, size: int):
            data = self.regions.get(address)
            if data is None or len(data) < size:
                raise IndexError(address)
            return data[:size]

    def near_jump(source: int, target: int) -> bytes:
        displacement = target - (source + 5)
        return b"\xe9" + displacement.to_bytes(4, "little", signed=True)

    draw_anim_body = 0x42AEA0
    good_thunk = 0x403049
    wrong_thunk = good_thunk + 0x100
    thunk_image = TinyImage(
        {
            good_thunk: near_jump(good_thunk, draw_anim_body),
            wrong_thunk: near_jump(wrong_thunk, draw_anim_body + 0x10),
        }
    )
    if not direct_call_reaches_symbol(
        thunk_image,
        (0x42C54B, 5, "call", hex(good_thunk)),
        draw_anim_body,
    ):
        print("selftest fail: exact rebuilt DrawAnim thunk was rejected")
        failed += 1
    if direct_call_reaches_symbol(
        thunk_image,
        (0x42C54B, 5, "call", hex(wrong_thunk)),
        draw_anim_body,
    ):
        print("selftest fail: patched rebuilt DrawAnim call target was accepted")
        failed += 1

    invalid_image = TinyImage({})
    if decode_exact_image_span(invalid_image, 0xDEAD0000, 5) is not None:
        print("selftest fail: invalid exact image span did not fail closed")
        failed += 1
    if read_pointer_table_offsets(
        invalid_image, 0xDEAD0000, 1, 0x00400000
    ) is not None:
        print("selftest fail: invalid pointer-table span did not fail closed")
        failed += 1

    followon_alias_args = (
        {
            "orig_text": "sub ax, word ptr [<OFFSET1>]",
            "recomp_text": "sub ax, word ptr [timeBonusOffset[1] (OFFSET)]",
            "orig_pattern": r"sub ax, word ptr \[<OFFSET\d+>\]",
            "expected_recomp_text": "sub ax, word ptr [timeBonusOffset[1] (OFFSET)]",
            "orig_base": 0x497088,
            "recomp_base": 0x464980,
            "orig_target": 0x49708A,
            "recomp_target": 0x464982,
            "orig_data": b"\x10\x00\x12\x00",
            "recomp_data": b"\x10\x00\x12\x00",
            "expected_data": b"\x10\x00\x12\x00",
        },
        {
            "orig_text": "sub ax, word ptr [<OFFSET1>]",
            "recomp_text": "sub ax, word ptr [bonusOffset[1] (OFFSET)]",
            "orig_pattern": r"sub ax, word ptr \[<OFFSET\d+>\]",
            "expected_recomp_text": "sub ax, word ptr [bonusOffset[1] (OFFSET)]",
            "orig_base": 0x497084,
            "recomp_base": 0x4649A0,
            "orig_target": 0x497086,
            "recomp_target": 0x4649A2,
            "orig_data": b"\x10\x00\x10\x00",
            "recomp_data": b"\x10\x00\x10\x00",
            "expected_data": b"\x10\x00\x10\x00",
        },
        {
            "orig_text": "movsx eax, word ptr [<OFFSET2>]",
            "recomp_text": "movsx eax, word ptr [flagOffset[1] (OFFSET)]",
            "orig_pattern": r"movsx eax, word ptr \[<OFFSET\d+>\]",
            "expected_recomp_text": "movsx eax, word ptr [flagOffset[1] (OFFSET)]",
            "orig_base": 0x497080,
            "recomp_base": 0x46499C,
            "orig_target": 0x497082,
            "recomp_target": 0x46499E,
            "orig_data": b"\x0f\x00\x1c\x00",
            "recomp_data": b"\x0f\x00\x1c\x00",
            "expected_data": b"\x0f\x00\x1c\x00",
        },
        {
            "orig_text": "movsx eax, word ptr [<OFFSET2>]",
            "recomp_text": "movsx eax, word ptr [switchOffset[1] (OFFSET)]",
            "orig_pattern": r"movsx eax, word ptr \[<OFFSET\d+>\]",
            "expected_recomp_text": "movsx eax, word ptr [switchOffset[1] (OFFSET)]",
            "orig_base": 0x49707C,
            "recomp_base": 0x46599C,
            "orig_target": 0x49707E,
            "recomp_target": 0x46599E,
            "orig_data": b"\x05\x00\x19\x00",
            "recomp_data": b"\x05\x00\x19\x00",
            "expected_data": b"\x05\x00\x19\x00",
        },
    )
    followon_alias_rejections = (
        (
            "time bonus bound to another symbol",
            0,
            {"recomp_text": "sub ax, word ptr [bonusOffset[1] (OFFSET)]"},
        ),
        (
            "time bonus original points four bytes past base",
            0,
            {"orig_target": 0x49708C},
        ),
        (
            "time bonus rebuilt bytes differ",
            0,
            {"recomp_data": b"\x10\x00\x11\x00"},
        ),
        (
            "bonus uses a different field index",
            1,
            {"recomp_text": "sub ax, word ptr [bonusOffset[0] (OFFSET)]"},
        ),
        (
            "bonus rebuilt points at its base",
            1,
            {"recomp_target": 0x4649A0},
        ),
        (
            "bonus original bytes differ",
            1,
            {"orig_data": b"\x10\x00\x0f\x00"},
        ),
        (
            "flag changes sign extension to zero extension",
            2,
            {"orig_text": "movzx eax, word ptr [<OFFSET2>]"},
        ),
        (
            "flag uses another destination register",
            2,
            {"recomp_text": "movsx edx, word ptr [flagOffset[1] (OFFSET)]"},
        ),
        (
            "flag original points four bytes past base",
            2,
            {"orig_target": 0x497084},
        ),
        (
            "flag rebuilt bytes differ",
            2,
            {"recomp_data": b"\x0f\x00\x1b\x00"},
        ),
        (
            "switch changes sign extension to zero extension",
            3,
            {"orig_text": "movzx eax, word ptr [<OFFSET2>]"},
        ),
        (
            "switch uses another destination register",
            3,
            {"recomp_text": "movsx edx, word ptr [switchOffset[1] (OFFSET)]"},
        ),
        (
            "switch binds another offset array",
            3,
            {"recomp_text": "movsx eax, word ptr [flagOffset[1] (OFFSET)]"},
        ),
        (
            "switch uses the base field",
            3,
            {"recomp_text": "movsx eax, word ptr [switchOffset[0] (OFFSET)]"},
        ),
        (
            "switch original points four bytes past base",
            3,
            {"orig_target": 0x497080},
        ),
        (
            "switch rebuilt bytes differ",
            3,
            {"recomp_data": b"\x05\x00\x18\x00"},
        ),
    )
    for description, candidate_index, changes in followon_alias_rejections:
        candidate_args = dict(followon_alias_args[candidate_index])
        candidate_args.update(changes)
        if is_exact_adjacent_short_field_alias(**candidate_args):
            print(
                "selftest fail: unsafe follow-on adjacent alias accepted: "
                f"{description}"
            )
            failed += 1

    for candidate_index, candidate_args in enumerate(followon_alias_args):
        if not is_exact_adjacent_short_field_alias(**candidate_args):
            print(
                "selftest fail: safe follow-on adjacent alias rejected: "
                f"candidate {candidate_index}"
            )
            failed += 1

    sequential_stack_chain, interleaved_stack_chain = make_stack_offset_chains()
    safe_stack_chain_tail = ["inc ebx", "mov eax, dword ptr [esp + 0x24]", "ret"]

    interleaved_stack_chain_rejections = (
        (
            "source aliases the first destination",
            make_stack_offset_chains(source="dword ptr [esp + 0x20]")[0]
            + safe_stack_chain_tail,
            make_stack_offset_chains(source="dword ptr [esp + 0x20]")[1]
            + safe_stack_chain_tail,
        ),
        (
            "transient register remains live",
            sequential_stack_chain + ["add ecx, edx", "ret"],
            interleaved_stack_chain + ["add ecx, edx", "ret"],
        ),
        (
            "transient byte register remains live",
            sequential_stack_chain + ["mov byte ptr [ecx], dl", "ret"],
            interleaved_stack_chain + ["mov byte ptr [ecx], dl", "ret"],
        ),
        (
            "transient edx is an implicit idiv input",
            sequential_stack_chain
            + ["mov eax, dword ptr [esp + 0x24]", "idiv ebx", "ret"],
            interleaved_stack_chain
            + ["mov eax, dword ptr [esp + 0x24]", "idiv ebx", "ret"],
        ),
        (
            "transient edx is an implicit div input",
            sequential_stack_chain
            + ["mov eax, dword ptr [esp + 0x24]", "div ebx", "ret"],
            interleaved_stack_chain
            + ["mov eax, dword ptr [esp + 0x24]", "div ebx", "ret"],
        ),
        (
            "transient edx is an implicit cmpxchg8b input",
            sequential_stack_chain
            + ["mov eax, dword ptr [esp + 0x24]", "cmpxchg8b qword ptr [esi]", "ret"],
            interleaved_stack_chain
            + ["mov eax, dword ptr [esp + 0x24]", "cmpxchg8b qword ptr [esi]", "ret"],
        ),
        (
            "primary register remains live",
            sequential_stack_chain + ["add ecx, eax", "ret"],
            interleaved_stack_chain + ["add ecx, eax", "ret"],
        ),
        (
            "stack destinations are swapped",
            sequential_stack_chain + safe_stack_chain_tail,
            replace_instruction(
                interleaved_stack_chain, 6, "mov dword ptr [esp + 0x1c], eax"
            )[:7]
            + ["mov dword ptr [esp + 0x20], edx"]
            + safe_stack_chain_tail,
        ),
        (
            "second arithmetic operand differs",
            sequential_stack_chain + safe_stack_chain_tail,
            replace_instruction(interleaved_stack_chain, 3, "add edx, esi")
            + safe_stack_chain_tail,
        ),
        (
            "second shift count differs",
            sequential_stack_chain + safe_stack_chain_tail,
            replace_instruction(interleaved_stack_chain, 5, "shl edx, 3")
            + safe_stack_chain_tail,
        ),
        (
            "eax transient reaches the return",
            make_stack_offset_chains(primary="ecx", transient="eax")[0]
            + ["inc ebx", "mov ecx, dword ptr [esp + 0x24]", "ret"],
            make_stack_offset_chains(primary="ecx", transient="eax")[1]
            + ["inc ebx", "mov ecx, dword ptr [esp + 0x24]", "ret"],
        ),
        (
            "ecx transient is outside the evidenced schedule",
            make_stack_offset_chains(transient="ecx")[0] + safe_stack_chain_tail,
            make_stack_offset_chains(transient="ecx")[1] + safe_stack_chain_tail,
        ),
        (
            "transient register is callee-saved",
            make_stack_offset_chains(transient="esi")[0] + safe_stack_chain_tail,
            make_stack_offset_chains(transient="esi")[1] + safe_stack_chain_tail,
        ),
    )

    merged_stack_chain_prefix = [
        "mov edx, dword ptr [g_prior (DATA)]",
        "mov ecx, dword ptr [g_value (DATA)]",
    ]
    merged_stack_chain_rejections = (
        ("full transient register remains live", ["push edx", "ret"]),
        ("transient byte register remains live", ["mov byte ptr [ecx], dh", "ret"]),
        (
            "transient edx is an implicit div input",
            ["mov eax, dword ptr [esp + 0x24]", "div ebx", "ret"],
        ),
        (
            "transient edx is an implicit idiv input",
            ["mov eax, dword ptr [esp + 0x24]", "idiv ebx", "ret"],
        ),
        (
            "transient edx is an implicit cmpxchg8b input",
            ["mov eax, dword ptr [esp + 0x24]", "cmpxchg8b qword ptr [esi]", "ret"],
        ),
    )

    for orig, recomp, expected in thunk_samples:
        actual = is_equivalent_insn(orig, recomp)
        if actual != expected:
            print(f"selftest fail: instruction expected {expected} got {actual}: {orig!r} / {recomp!r}")
            failed += 1

    for orig, recomp, expected in codegen_samples:
        actual = is_codegen_equivalent_diff(make_diff(orig, recomp))
        if actual != expected:
            print(f"selftest fail: codegen expected {expected} got {actual}: {orig!r} / {recomp!r}")
            failed += 1

    stack_chain_prefix = ["test ebp, ebp", "jle 0x40"]
    for description, orig, recomp in interleaved_stack_chain_rejections:
        if is_codegen_equivalent_diff(
            make_diff(stack_chain_prefix + orig, stack_chain_prefix + recomp)
        ):
            print(f"selftest fail: unsafe interleaved stack chain accepted: {description}")
            failed += 1

    for description, suffix in merged_stack_chain_rejections:
        if is_codegen_equivalent_diff(
            make_diff(
                merged_stack_chain_prefix + sequential_stack_chain + suffix,
                merged_stack_chain_prefix + interleaved_stack_chain + suffix,
            )
        ):
            print(f"selftest fail: unsafe merged interleaved stack chain accepted: {description}")
            failed += 1

    if not is_codegen_equivalent_diff(
        make_diff(
            merged_stack_chain_prefix + sequential_stack_chain + safe_stack_chain_tail,
            merged_stack_chain_prefix + interleaved_stack_chain + safe_stack_chain_tail,
        )
    ):
        print("selftest fail: safe interleaved stack chain rejected because of dead prefix use")
        failed += 1

    unsafe_delayed_pushes = (
        (
            ["push ebx", "mov ecx, ebx", "call Thing::Run (FUNCTION)"],
            ["push esi", "mov ecx, ebx", "call Thing::Run (FUNCTION)"],
        ),
        (
            ["push ebx", "mov dword ptr [g_value (DATA)], ecx", "call Thing::Run (FUNCTION)"],
            ["push esi", "mov dword ptr [g_value (DATA)], ecx", "call Thing::Run (FUNCTION)"],
        ),
        (
            ["push ebx", "mov ecx, dword ptr [esp + 8]", "call Thing::Run (FUNCTION)"],
            ["push esi", "mov ecx, dword ptr [esp + 8]", "call Thing::Run (FUNCTION)"],
        ),
        (
            ["push ebx", "mov ecx, dword ptr [g_value (DATA)]", "call First (FUNCTION)"],
            ["push esi", "mov ecx, dword ptr [g_value (DATA)]", "call Second (FUNCTION)"],
        ),
    )
    for orig, recomp in unsafe_delayed_pushes:
        if role_push_reaches_equivalent_call(orig, recomp, 0, {"ebx", "esi"}):
            print(f"selftest fail: unsafe delayed push was normalized: {orig!r} / {recomp!r}")
            failed += 1

    unsafe_load_schedules = (
        (
            ["mov esi, dword ptr [ecx]", "push esi"],
            ["push esi", "mov esi, dword ptr [ecx]"],
        ),
        (
            ["mov esi, dword ptr [esp + 4]", "push edi"],
            ["push edi", "mov esi, dword ptr [esp + 4]"],
        ),
        (
            ["mov esi, dword ptr [ecx]", "call Thing (FUNCTION)"],
            ["call Thing (FUNCTION)", "mov esi, dword ptr [ecx]"],
        ),
    )
    for orig, recomp in unsafe_load_schedules:
        normalized = list(recomp)
        normalize_saved_register_load_scheduling(orig, normalized)
        if normalized == orig:
            print(f"selftest fail: unsafe saved-register load was normalized: {orig!r} / {recomp!r}")
            failed += 1

    partial_comparison_orig = ["cmp eax, edi", "push esi"]
    partial_comparison_recomp = ["cmp eax, ebx", "push ebx"]
    normalized = list(partial_comparison_recomp)
    normalize_role_comparisons_for_exact_match(
        partial_comparison_orig, normalized, {"ebx": "edi"}
    )
    if normalized != partial_comparison_recomp:
        print("selftest fail: partial register-role comparison was normalized")
        failed += 1

    aliased_multiplier = ["mov eax, edx", "sub edx, edx", "imul eax, eax"]
    normalized = list(aliased_multiplier)
    normalize_multiply_copy_zero(normalized)
    if normalized != aliased_multiplier:
        print("selftest fail: unsafe aliased multiply/copy sequence was normalized")
        failed += 1

    flags_live = ["mov eax, edx", "sub edx, edx", "imul eax, ecx", "jo 0x2"]
    normalized = list(flags_live)
    normalize_multiply_copy_zero(normalized)
    if normalized != flags_live:
        print("selftest fail: multiply/copy sequence with live flags was normalized")
        failed += 1

    if DEFAULT_JSON.is_file() and RECOMPILED_EXE.is_file():
        full_matches = load_matches(DEFAULT_JSON)

        live_sheep_match = full_matches.get(0x0043D370)
        if live_sheep_match is None:
            print("selftest fail: live DrawSheep fixture is unavailable")
            failed += 1
        else:
            ratio, _ = compute_ratio(
                live_sheep_match, live_sheep_match["name"]
            )
            if ratio != 100.0:
                print(
                    "selftest fail: live DrawSheep mapped-table candidate "
                    "was rejected"
                )
                failed += 1

        for address, name in strict_target_identities:
            ratio, _ = compute_ratio(full_matches[address], name)
            if ratio != 100.0:
                print(
                    "selftest fail: verified complete candidate was rejected: "
                    f"{name}"
                )
                failed += 1

        def replace_recompiled_annotation(
            match: dict, needle: str, replacement: str
        ) -> int:
            replacements = 0
            for _, chunks in match.get("diff", []):
                for chunk in chunks:
                    for entry in chunk.get("recomp", []) or []:
                        if needle in entry[1]:
                            entry[1] = replacement
                            replacements += 1
            return replacements

        def replace_shared_recompiled_annotation(
            match: dict, needle: str, replacement: str
        ) -> int:
            """Split one rendered-both row to corrupt only its rebuilt side."""
            replacements = 0
            for _, chunks in match.get("diff", []):
                rewritten_chunks = []
                for chunk in chunks:
                    both_entries = chunk.get("both")
                    if not both_entries:
                        rewritten_chunks.append(chunk)
                        continue

                    pending_both = []
                    for entry in both_entries:
                        if len(entry) < 3 or needle not in entry[1]:
                            pending_both.append(entry)
                            continue
                        if pending_both:
                            rewritten_chunks.append({"both": pending_both})
                            pending_both = []
                        rewritten_chunks.append(
                            {
                                "orig": [[entry[0], entry[1]]],
                                "recomp": [[entry[2], replacement]],
                            }
                        )
                        replacements += 1
                    if pending_both:
                        rewritten_chunks.append({"both": pending_both})
                chunks[:] = rewritten_chunks
            return replacements

        class PatchedImage:
            def __init__(self, base, patches: dict[int, bytes]):
                self.base = base
                self.patches = patches

            def read(self, address: int, size: int):
                data = bytearray(self.base.read(address, size))
                end = address + size
                for patch_address, patch in self.patches.items():
                    patch_end = patch_address + len(patch)
                    overlap_start = max(address, patch_address)
                    overlap_end = min(end, patch_end)
                    if overlap_start >= overlap_end:
                        continue
                    data[overlap_start - address : overlap_end - address] = (
                        patch[
                            overlap_start
                            - patch_address : overlap_end
                            - patch_address
                        ]
                    )
                return bytes(data)

        duplicator_match = copy.deepcopy(full_matches[0x0043CFA0])
        duplicator_match["name"] = "Unrelated::Function"
        ratio, _ = compute_ratio(duplicator_match, duplicator_match["name"])
        if ratio == 100.0:
            print(
                "selftest fail: complete DrawDuplicator match accepted after "
                "a name mutation"
            )
            failed += 1

        duplicator_match = copy.deepcopy(full_matches[0x0043CFA0])
        if replace_recompiled_annotation(
            duplicator_match,
            "DrawAnim",
            "call CompletelyWrong::Target (FUNCTION)",
        ) != 3:
            print("selftest fail: DrawDuplicator call fixture is incomplete")
            failed += 1
        else:
            ratio, _ = compute_ratio(
                duplicator_match, duplicator_match["name"]
            )
            if ratio == 100.0:
                print(
                    "selftest fail: complete DrawDuplicator match accepted "
                    "wrong call labels"
                )
                failed += 1

        rebuilt_image = load_recompiled_image()
        time_bonus_match = copy.deepcopy(full_matches[0x0043D0F0])
        time_bonus_start = norm_addr(time_bonus_match["recomp"])
        time_bonus_body = decode_exact_image_span(
            rebuilt_image, time_bonus_start, 0x37
        )
        if time_bonus_body is None or len(time_bonus_body) != 16:
            print("selftest fail: DrawTimeBonus PE fixture is unavailable")
            failed += 1
        else:
            call_address = time_bonus_body[14][0]
            wrong_target = int(time_bonus_body[14][3], 16) + 0x100
            displacement = wrong_target - (call_address + 5)
            patched_image = PatchedImage(
                rebuilt_image,
                {call_address + 1: displacement.to_bytes(4, "little", signed=True)},
            )
            saved_loader = globals()["load_recompiled_image"]
            globals()["load_recompiled_image"] = lambda: patched_image
            try:
                ratio, _ = compute_ratio(
                    time_bonus_match, time_bonus_match["name"]
                )
            finally:
                globals()["load_recompiled_image"] = saved_loader
            if ratio == 100.0:
                print(
                    "selftest fail: DrawTimeBonus accepted a patched "
                    "rebuilt call target"
                )
                failed += 1

            time_bonus_match = copy.deepcopy(full_matches[0x0043D0F0])
            if replace_recompiled_annotation(
                time_bonus_match,
                "DrawAnim",
                "call CompletelyWrong::Target (FUNCTION)",
            ) != 1:
                print("selftest fail: DrawTimeBonus call fixture is incomplete")
                failed += 1
            else:
                ratio, _ = compute_ratio(
                    time_bonus_match, time_bonus_match["name"]
                )
                if ratio == 100.0:
                    print(
                        "selftest fail: DrawTimeBonus accepted a wrong rebuilt "
                        "call label"
                    )
                    failed += 1

        sheep_match = copy.deepcopy(full_matches[0x0043D370])
        sheep_start = norm_addr(sheep_match["recomp"])
        sheep_body = decode_exact_image_span(rebuilt_image, sheep_start, 0x94)
        if sheep_body is None or len(sheep_body) != 51:
            print("selftest fail: DrawSheep PE fixture is unavailable")
            failed += 1
        else:
            table_instruction = sheep_body[24]
            table_bytes = bytes(
                rebuilt_image.read(
                    table_instruction[0] + table_instruction[1] - 4, 4
                )
            )
            wrong_table = int.from_bytes(table_bytes, "little") + 0x20
            patched_image = PatchedImage(
                rebuilt_image,
                {
                    table_instruction[0]
                    + table_instruction[1]
                    - 4: wrong_table.to_bytes(4, "little")
                },
            )
            saved_loader = globals()["load_recompiled_image"]
            globals()["load_recompiled_image"] = lambda: patched_image
            try:
                ratio, _ = compute_ratio(sheep_match, sheep_match["name"])
            finally:
                globals()["load_recompiled_image"] = saved_loader
            if ratio == 100.0:
                print(
                    "selftest fail: DrawSheep accepted a patched rebuilt "
                    "resource table"
                )
                failed += 1

            sheep_match = copy.deepcopy(full_matches[0x0043D370])
            replacements = replace_recompiled_annotation(
                sheep_match,
                "sheepMunchResources",
                "mov ebx, dword ptr "
                "[edi*4 + CompletelyWrong::Table (DATA)]",
            )
            if replacements == 0:
                replacements = replace_shared_recompiled_annotation(
                    sheep_match,
                    "sheepMunchResources",
                    "mov ebx, dword ptr "
                    "[edi*4 + CompletelyWrong::Table (DATA)]",
                )
            if replacements != 1:
                print("selftest fail: DrawSheep table fixture is incomplete")
                failed += 1
            else:
                ratio, _ = compute_ratio(sheep_match, sheep_match["name"])
                if ratio == 100.0:
                    print(
                        "selftest fail: DrawSheep accepted a wrong rebuilt "
                        "resource-table label"
                    )
                    failed += 1

        ctos_match = copy.deepcopy(full_matches[0x004621E0])
        ctos_match["_next_orig_addr"] = 0xDEAD0000
        try:
            ratio, _ = compute_ratio(ctos_match, ctos_match["name"])
        except (IndexError, LookupError):
            print("selftest fail: CtoS invalid span escaped fail-closed handling")
            failed += 1
        else:
            if ratio == 100.0:
                print("selftest fail: CtoS accepted a stale function bound")
                failed += 1

    deleting_dtor = {
        "type": 1,
        "matching": 0.25,
        "name": "Thing::`scalar deleting destructor'",
    }
    ratio, reason = compute_ratio(deleting_dtor, deleting_dtor["name"])
    if ratio != 25.0 or reason:
        print("selftest fail: non-matching deleting destructor was accepted unconditionally")
        failed += 1

    if failed:
        return 1
    print("check selftest: ok")
    return 0


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

    matches = [x for x in data if x.get("type") in (None, 1)]
    function_addresses = sorted(
        {norm_addr(match["address"]) for match in matches if match.get("address")}
    )
    next_address = {
        address: following
        for address, following in zip(function_addresses, function_addresses[1:])
    }
    for match in matches:
        address = norm_addr(match["address"])
        if address in next_address:
            match["_next_orig_addr"] = next_address[address]

    return {norm_addr(match["address"]): match for match in matches}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("addrs", nargs="*", help="Addresses (e.g. 0x0045ca30)")
    parser.add_argument("--diff", action="store_true", help="Print diff for non-matching functions")
    parser.add_argument("--json", type=Path, default=DEFAULT_JSON)
    parser.add_argument("--no-build", action="store_true", help="Skip incremental build before check")
    parser.add_argument("--clean-first", action="store_true", help="Clean build before check")
    parser.add_argument("--no-reccmp", action="store_true", help="Do not run reccmp; reuse existing JSON")
    parser.add_argument("--selftest", action="store_true", help="exercise equivalence classifiers and exit")
    args = parser.parse_args()

    if args.selftest:
        return run_selftest()

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
