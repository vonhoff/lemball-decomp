"""Operand-order equivalence for comparisons whose flags do not escape."""

import re

from reccmp.compare.asm.instgen import SectionType


REVERSED_BRANCH = {
    "ja": "jb", "jb": "ja", "jae": "jbe", "jbe": "jae",
    "jg": "jl", "jl": "jg", "jge": "jle", "jle": "jge",
    "je": "je", "jne": "jne",
}
REGISTER = re.compile(r"e?(?:ax|bx|cx|dx|si|di|sp|bp)|[abcd][lh]")
FLAG_WRITERS = {"cmp", "add", "sub", "neg"}
FLAG_PRESERVERS = {"mov", "movsx", "movzx", "lea", "push", "pop", "nop"}


def _flags_overwritten(instructions, address):
    """Require a full arithmetic-flag write before any other use or exit."""
    visited = set()
    while address in instructions and address not in visited:
        visited.add(address)
        _, size, mnemonic, operands = instructions[address]
        if mnemonic in FLAG_WRITERS:
            return True
        if mnemonic == "jmp" and operands.startswith("0x"):
            address = int(operands, 16)
        elif mnemonic in FLAG_PRESERVERS:
            address += size
        else:
            return False
    return False


def _entry_targets(sections, instructions):
    targets = {target for section in sections if section.type == SectionType.ADDR_TAB
               for _, target in section.contents}
    for _, _, mnemonic, operands in instructions.values():
        if re.fullmatch(r"call|j\w+|loop\w*", mnemonic) and operands.startswith("0x"):
            targets.add(int(operands, 16))
    return targets


def _reversible_pair(instruction, instructions, targets):
    """Accept register CMP/Jcc pairs with one entry and dead outgoing flags."""
    address, size, mnemonic, operands = instruction
    if mnemonic != "cmp":
        return None
    registers = operands.split(", ")
    if len(registers) != 2 or not all(REGISTER.fullmatch(reg) for reg in registers):
        return None
    if registers == sorted(registers):
        return None
    branch = instructions.get(address + size)
    if branch is None or branch[0] in targets or branch[2] not in REVERSED_BRANCH:
        return None
    successors = (branch[0] + branch[1], int(branch[3], 16))
    if not all(_flags_overwritten(instructions, successor) for successor in successors):
        return None
    return registers, branch


def normalize_compare_branches(asm, sections):
    """Normalize proven pairs only; preserve every instruction address and branch target."""
    instructions = {inst[0]: inst for section in sections if section.type == SectionType.CODE
                    for inst in section.contents}
    if any(inst[2] == "jmp" and not inst[3].startswith("0x") for inst in instructions.values()):
        return asm
    targets = _entry_targets(sections, instructions)
    lines = dict(asm)
    replacements = {}
    for address, instruction in instructions.items():
        pair = _reversible_pair(instruction, instructions, targets)
        if pair is None:
            continue
        registers, branch = pair
        replacements[address] = "cmp " + ", ".join(sorted(registers))
        displacement = lines[branch[0]].partition(" ")[2]
        replacements[branch[0]] = REVERSED_BRANCH[branch[2]] + " " + displacement
    return [(address, replacements.get(address, line)) for address, line in asm]
