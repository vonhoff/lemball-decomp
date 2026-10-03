"""Operand-order equivalence for comparisons whose flags do not escape."""

import re

from reccmp.compare.asm.instgen import InstructGen, SectionType


REVERSED_BRANCH = {
    "ja": "jb", "jb": "ja", "jae": "jbe", "jbe": "jae",
    "jg": "jl", "jl": "jg", "jge": "jle", "jle": "jge",
    "je": "je", "jne": "jne",
}
REGISTER = re.compile(r"e?(?:ax|bx|cx|dx|si|di|sp|bp)|[abcd][lh]")
FLAG_WRITERS = {"cmp", "add", "sub", "neg"}
FLAG_PRESERVERS = {"mov", "movsx", "movzx", "lea", "push", "pop", "nop"}


def _flags_overwritten(instructions, address, check_call=None):
    """Require a full arithmetic-flag write before any other use or exit."""
    visited = set()
    while address in instructions and address not in visited:
        visited.add(address)
        _, size, mnemonic, operands = instructions[address]
        if mnemonic in FLAG_WRITERS:
            return True
        if mnemonic == "call" and operands.startswith("0x"):
            return check_call is not None and check_call(int(operands, 16))
        if mnemonic == "jmp" and operands.startswith("0x"):
            address = int(operands, 16)
        elif mnemonic in FLAG_PRESERVERS:
            address += size
        else:
            return False
    return False


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


def _guarded_pair(instruction, instructions, targets, check_call):
    """Accept register CMP/Jcc pairs with one entry and dead outgoing flags."""
    address, size, mnemonic, operands = instruction
    if mnemonic != "cmp":
        return None
    registers = operands.split(", ")
    if len(registers) != 2 or not all(REGISTER.fullmatch(reg) for reg in registers):
        return None
    branch = instructions.get(address + size)
    if branch is None or branch[0] in targets or branch[2] not in REVERSED_BRANCH:
        return None
    successors = (branch[0] + branch[1], int(branch[3], 16))
    if not all(_flags_overwritten(instructions, successor, check_call) for successor in successors):
        return None
    return registers, branch


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
        pair = _guarded_pair(instruction, instructions, targets, check_call)
        if pair is None:
            continue
        registers, branch = pair
        # A distinct marker requires the flag-lifetime proof on both sides.
        replacements[address] = "guarded-cmp " + ", ".join(sorted(registers))
        displacement = lines[branch[0]].partition(" ")[2]
        mnemonic = branch[2] if registers == sorted(registers) else REVERSED_BRANCH[branch[2]]
        replacements[branch[0]] = mnemonic + " " + displacement
    return [(address, replacements.get(address, line)) for address, line in asm]
