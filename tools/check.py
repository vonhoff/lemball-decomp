#!/usr/bin/env python3
"""Central decompilation match evaluation logic and CLI."""

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

    if is_thunk_only_diff(match.get("diff")):
        return 100.0, "MATCH (thunk)"

    if is_codegen_equivalent_diff(match.get("diff")):
        return 100.0, "MATCH (compiler entropy)"

    return float(match.get("matching", 0.0)) * 100.0, ""


def run_selftest() -> int:
    def make_diff(orig: list[str], recomp: list[str]):
        return [["@@ selftest @@", [{"orig": [["", x] for x in orig], "recomp": [["", x] for x in recomp]}]]]

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

    failed = 0
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
