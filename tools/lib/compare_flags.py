"""Operand-order and zero-test equivalence with proven flag lifetimes."""

import re

from reccmp.compare.asm.instgen import InstructGen, SectionType


REVERSED_BRANCH = {
    "ja": "jb",
    "jb": "ja",
    "jae": "jbe",
    "jbe": "jae",
    "jg": "jl",
    "jl": "jg",
    "jge": "jle",
    "jle": "jge",
    "je": "je",
    "jne": "jne",
}
REGISTER = re.compile(r"e?(?:ax|bx|cx|dx|si|di|sp|bp)|[abcd][lh]")
REGISTER32 = re.compile(r"e(?:ax|bx|cx|dx|si|di|sp|bp)")
MEMORY = re.compile(r"(?:byte|word|dword) ptr \[[^\[\],]+]")
FLAG_WRITERS = {"cmp", "add", "sub", "neg"}
LOGICAL_WRITERS = {"and", "or", "xor", "test"}
FLAG_PRESERVERS = {"mov", "movsx", "movzx", "lea", "push", "pop", "nop"}
FLAG_NONREADERS = FLAG_PRESERVERS | LOGICAL_WRITERS | {"shl", "shr", "sar"}


def _flags_overwritten(instructions, address, check_call=None):
    """Require a full arithmetic-flag write before any other use or exit."""
    pending = [(address, False)]
    visited = set()
    while pending:
        address, only_auxiliary = pending.pop()
        if address not in instructions or (address, only_auxiliary) in visited:
            return False
        visited.add((address, only_auxiliary))
        _, size, mnemonic, operands = instructions[address]
        if mnemonic in FLAG_WRITERS:
            continue
        if mnemonic == "call" and operands.startswith("0x"):
            if check_call is None or not check_call(int(operands, 16)):
                return False
            continue
        only_auxiliary |= mnemonic in LOGICAL_WRITERS
        if mnemonic == "jmp" and operands.startswith("0x"):
            successors = (int(operands, 16),)
        elif only_auxiliary and mnemonic in REVERSED_BRANCH:
            successors = address + size, int(operands, 16)
        elif mnemonic in FLAG_NONREADERS:
            successors = (address + size,)
        else:
            return False
        pending.extend((successor, only_auxiliary) for successor in successors)
    return True


def control_flow_targets(sections, instructions):
    """Collect explicit entries that can bypass a preceding instruction."""
    targets = {
        target
        for section in sections
        if section.type == SectionType.ADDR_TAB
        for _, target in section.contents
    }
    for _, _, mnemonic, operands in instructions.values():
        if re.fullmatch(r"call|j\w+|loop\w*", mnemonic) and operands.startswith("0x"):
            targets.add(int(operands, 16))
    return targets


def indirect_jumps_use_tables(instructions, tables):
    """Unknown computed jumps could enter padding or bypass a comparison."""
    for _, _, mnemonic, operands in instructions:
        if mnemonic != "jmp" or operands.startswith("0x"):
            continue
        table = re.fullmatch(
            r"dword ptr \[(?:e[a-z]{2}\*4 \+ )?(0x[0-9a-f]+)]", operands
        )
        if table is None or tables.get(int(table[1], 16)) != SectionType.ADDR_TAB:
            return False
    return True


def prefix_overwrites_flags(data, start):
    """Prove a callee's decoded prefix kills incoming flags without another call."""
    instructions = {
        inst[0]: inst
        for section in InstructGen(data, start).sections
        if section.type == SectionType.CODE
        for inst in section.contents
    }
    return _flags_overwritten(instructions, start)


def _zero_registers_before(instructions, targets):
    """Track XOR-zeroed registers only through contiguous, single-entry MOVs."""
    known = set()
    before = {}
    next_address = None
    for address, size, mnemonic, operands in instructions.values():
        if address != next_address or address in targets:
            known.clear()
        before[address] = known.copy()
        raw = operands.split(", ")
        if len(raw) == 2 and REGISTER32.fullmatch(raw[0]):
            if mnemonic == "xor" and raw[0] == raw[1]:
                known.add(raw[0])
            elif mnemonic == "mov":
                known.discard(raw[0])
            else:
                known.clear()
        elif mnemonic != "nop":
            known.clear()
        next_address = address + size
    return before


def _guarded_pair(
    instruction, instructions, targets, check_call, lines, zero_registers
) -> tuple[list[str], tuple] | None:
    """Accept CMP/TEST and Jcc with one entry and dead outgoing flags."""
    address, size, mnemonic, operands = instruction
    if mnemonic not in ("cmp", "test"):
        return None
    raw = operands.split(", ")
    compared = lines[address].partition(" ")[2].split(", ")
    if len(raw) != 2 or len(compared) != 2:
        return None
    if not any(REGISTER.fullmatch(operand) for operand in raw):
        return None
    if not all(
        REGISTER.fullmatch(operand) or MEMORY.fullmatch(operand) for operand in raw
    ):
        return None
    if mnemonic == "test":
        if raw[0] != raw[1] or not REGISTER32.fullmatch(raw[0]):
            return None
        compared = [compared[0], "0"]
    elif all(REGISTER32.fullmatch(operand) for operand in raw):
        if raw[1] in zero_registers[address]:
            compared = [compared[0], "0"]
        elif raw[0] in zero_registers[address]:
            compared = ["0", compared[1]]
    branch = instructions.get(address + size)
    while branch is not None and branch[2] in FLAG_PRESERVERS:
        if branch[0] in targets:
            return None
        branch = instructions.get(branch[0] + branch[1])
    if branch is None or branch[0] in targets or branch[2] not in REVERSED_BRANCH:
        return None
    successors = (branch[0] + branch[1], int(branch[3], 16))
    if not all(
        _flags_overwritten(instructions, successor, check_call)
        for successor in successors
    ):
        return None
    return compared, branch


def normalize_compare_branches(asm, sections, check_call=None):
    """Normalize proven pairs only; preserve every instruction address and branch target."""
    instructions = {
        inst[0]: inst
        for section in sections
        if section.type == SectionType.CODE
        for inst in section.contents
    }
    tables = {
        section.contents[0][0]: section.type
        for section in sections
        if section.type != SectionType.CODE and section.contents
    }
    if not indirect_jumps_use_tables(instructions.values(), tables):
        return asm
    targets = control_flow_targets(sections, instructions)
    zero_registers = _zero_registers_before(instructions, targets)
    lines = dict(asm)
    replacements = {}
    for address, instruction in instructions.items():
        pair = _guarded_pair(
            instruction, instructions, targets, check_call, lines, zero_registers
        )
        if pair is None:
            continue
        operands, branch = pair
        ordered = sorted(operands)
        replacements[address] = "guarded-cmp " + ", ".join(ordered)
        displacement = lines[branch[0]].partition(" ")[2]
        mnemonic = branch[2] if operands == ordered else REVERSED_BRANCH[branch[2]]
        replacements[branch[0]] = mnemonic + " " + displacement
    return [(address, replacements.get(address, line)) for address, line in asm]
