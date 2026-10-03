"""Operand-order equivalence for comparisons whose flags do not escape."""

import re

from reccmp.compare.asm.instgen import InstructGen, SectionType


REVERSED_BRANCH = {
    "ja": "jb", "jb": "ja", "jae": "jbe", "jbe": "jae",
    "jg": "jl", "jl": "jg", "jge": "jle", "jle": "jge",
    "je": "je", "jne": "jne",
}
REGISTER = re.compile(r"e?(?:ax|bx|cx|dx|si|di|sp|bp)|[abcd][lh]")
MEMORY = re.compile(r"(?:byte|word|dword) ptr \[[^\[\],]+]")
FLAG_WRITERS = {"cmp", "add", "sub", "neg"}
LOGICAL_WRITERS = {"and", "or", "xor", "test"}
FLAG_PRESERVERS = {"mov", "movsx", "movzx", "lea", "push", "pop", "nop"}
FLAG_NONREADERS = FLAG_PRESERVERS | LOGICAL_WRITERS | {"shl", "shr", "sar"}


def _flag_successors(instruction, only_auxiliary):
    """Follow only transfers that cannot observe the still-unproven flags."""
    address, size, mnemonic, operands = instruction
    if mnemonic == "jmp" and operands.startswith("0x"):
        return (int(operands, 16),)
    if only_auxiliary and mnemonic in REVERSED_BRANCH:
        return address + size, int(operands, 16)
    if mnemonic in FLAG_NONREADERS:
        return (address + size,)
    return ()


def _flags_overwritten(instructions, address, check_call=None):
    """Require a full arithmetic-flag write before any other use or exit."""
    pending = [(address, False)]
    visited = set()
    while pending:
        address, only_auxiliary = pending.pop()
        if address not in instructions or (address, only_auxiliary) in visited:
            return False
        visited.add((address, only_auxiliary))
        instruction = instructions[address]
        _, _, mnemonic, operands = instruction
        if mnemonic in FLAG_WRITERS:
            continue
        if mnemonic == "call" and operands.startswith("0x"):
            if check_call is None or not check_call(int(operands, 16)):
                return False
            continue
        only_auxiliary |= mnemonic in LOGICAL_WRITERS
        successors = _flag_successors(instruction, only_auxiliary)
        if not successors:
            return False
        pending.extend((successor, only_auxiliary) for successor in successors)
    return True


def control_flow_targets(sections, instructions):
    """Collect explicit entries that can bypass a preceding instruction."""
    targets = {target for section in sections if section.type == SectionType.ADDR_TAB
               for _, target in section.contents}
    for _, _, mnemonic, operands in instructions.values():
        if re.fullmatch(r"call|j\w+|loop\w*", mnemonic) and operands.startswith("0x"):
            targets.add(int(operands, 16))
    return targets


def indirect_jumps_use_tables(instructions, tables):
    """Unknown computed jumps could enter padding or bypass a comparison."""
    for _, _, mnemonic, operands in instructions:
        if mnemonic != "jmp" or operands.startswith("0x"):
            continue
        table = re.fullmatch(r"dword ptr \[(?:e[a-z]{2}\*4 \+ )?(0x[0-9a-f]+)]", operands)
        if table is None or tables.get(int(table[1], 16)) != SectionType.ADDR_TAB:
            return False
    return True


def prefix_overwrites_flags(data, start):
    """Prove a callee's decoded prefix kills incoming flags without another call."""
    instructions = {inst[0]: inst for section in InstructGen(data, start).sections
                    if section.type == SectionType.CODE for inst in section.contents}
    return _flags_overwritten(instructions, start)


def _comparison_operands(operands: str, line: str) -> list[str] | None:
    """Accept register/register or register/memory operands without ambiguous commas."""
    raw = operands.split(", ")
    normalized = line.removeprefix("cmp ").split(", ")
    if len(raw) != 2 or len(normalized) != 2:
        return None
    if not any(REGISTER.fullmatch(operand) for operand in raw):
        return None
    if not all(REGISTER.fullmatch(operand) or MEMORY.fullmatch(operand) for operand in raw):
        return None
    return normalized


def _guarded_pair(instruction, instructions, targets, check_call, lines) -> tuple[list[str], tuple] | None:
    """Accept CMP/Jcc pairs with one entry and dead outgoing flags."""
    address, size, mnemonic, operands = instruction
    if mnemonic != "cmp":
        return None
    compared = _comparison_operands(operands, lines[address])
    if compared is None:
        return None
    branch = instructions.get(address + size)
    if branch is None or branch[0] in targets or branch[2] not in REVERSED_BRANCH:
        return None
    successors = (branch[0] + branch[1], int(branch[3], 16))
    if not all(_flags_overwritten(instructions, successor, check_call) for successor in successors):
        return None
    return compared, branch


def normalize_compare_branches(asm, sections, check_call=None):
    """Normalize proven pairs only; preserve every instruction address and branch target."""
    instructions = {inst[0]: inst for section in sections if section.type == SectionType.CODE
                    for inst in section.contents}
    tables = {section.contents[0][0]: section.type for section in sections
              if section.type != SectionType.CODE and section.contents}
    if not indirect_jumps_use_tables(instructions.values(), tables):
        return asm
    targets = control_flow_targets(sections, instructions)
    lines = dict(asm)
    replacements = {}
    for address, instruction in instructions.items():
        pair = _guarded_pair(instruction, instructions, targets, check_call, lines)
        if pair is None:
            continue
        operands: list[str] = pair[0]
        branch = pair[1]
        ordered = sorted(operands)
        # A distinct marker requires the flag-lifetime proof on both sides.
        replacements[address] = "guarded-cmp " + ", ".join(ordered)
        displacement = lines[branch[0]].partition(" ")[2]
        mnemonic = branch[2] if operands == ordered else REVERSED_BRANCH[branch[2]]
        replacements[branch[0]] = mnemonic + " " + displacement
    return [(address, replacements.get(address, line)) for address, line in asm]
