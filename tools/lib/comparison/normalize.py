"""Conservative assembly canonicalization for additional Effective matches."""

import heapq
import re

from capstone import (
    Cs,
    CS_ARCH_X86,
    CS_GRP_INT,
    CS_GRP_IRET,
    CS_GRP_PRIVILEGE,
    CS_GRP_RET,
    CS_MODE_32,
)
from capstone.x86_const import (
    X86_EFLAGS_PRIOR_AF,
    X86_EFLAGS_TEST_AF,
    X86_OP_IMM,
    X86_OP_MEM,
    X86_OP_REG,
)
from reccmp.compare.asm.const import JUMP_MNEMONICS
from reccmp.compare.asm.instgen import InstructGen, SectionType

GENERAL_REGISTERS = {"eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp"}
REGISTER_FAMILIES = {
    alias: family
    for family, aliases in (
        ("eax", ("eax", "ax", "al", "ah")),
        ("ebx", ("ebx", "bx", "bl", "bh")),
        ("ecx", ("ecx", "cx", "cl", "ch")),
        ("edx", ("edx", "dx", "dl", "dh")),
        ("esi", ("esi", "si")),
        ("edi", ("edi", "di")),
        ("ebp", ("ebp", "bp")),
        ("esp", ("esp", "sp")),
    )
    for alias in aliases
}


def number_placeholders(lines, symbols=()):
    """Alpha-rename unknown addresses; preserve repeated/distinct identities."""
    names = {}
    pattern = re.compile(
        "".join(
            re.escape(name) + "|" for name in sorted(symbols, key=len, reverse=True)
        )
        + r"(?P<offset><OFFSET\d+>)"
    )

    def replace(match):
        if match.group("offset") is None:
            return match.group()
        token = match.group()
        return names.setdefault(token, f"<OFFSET{len(names) + 1}>")

    return [pattern.sub(replace, line) for line in lines]


def register_access(inst):
    """Include implicit operands; treat partial registers as their full family."""
    return tuple(
        {REGISTER_FAMILIES.get(inst.reg_name(reg), inst.reg_name(reg)) for reg in regs}
        for regs in inst.regs_access()
    )


def schedulable(inst):
    """Only ordinary register destinations; stores and side effects are barriers."""
    return (
        inst.mnemonic
        in {"mov", "movzx", "movsx", "lea", "add", "sub", "and", "or", "xor"}
        and not any(inst.prefix)
        and len(inst.operands) == 2
        and inst.operands[0].type == X86_OP_REG
        and inst.reg_name(inst.operands[0].reg) in GENERAL_REGISTERS - {"esp"}
        and set.union(*register_access(inst)) <= GENERAL_REGISTERS | {"eflags"}
    )


def schedule(lines, instructions):
    """Canonical topological order, preserving RAW, WAR, WAW and flag hazards."""
    accesses = [register_access(inst) for inst in instructions]
    successors = [[] for _ in lines]
    incoming = [0] * len(lines)
    for j, (reads_j, writes_j) in enumerate(accesses):
        for i, (reads_i, writes_i) in enumerate(accesses[:j]):
            if writes_i & (reads_j | writes_j) or reads_i & writes_j:
                successors[i].append(j)
                incoming[j] += 1
    ready = [(line, i) for i, line in enumerate(lines) if incoming[i] == 0]
    heapq.heapify(ready)
    result = []
    while ready:
        line, i = heapq.heappop(ready)
        result.append(line)
        for j in successors[i]:
            incoming[j] -= 1
            if incoming[j] == 0:
                heapq.heappush(ready, (lines[j], j))
    return result


def normalize(assembly, data, start, symbols=()):
    """Normalize identities, zero tests and local scheduling.

    Return None on incomplete decoding. Instruction count and table order stay
    fixed. Unknown instructions, calls, stores and block entries stop scheduling.
    Zero facts are local to blocks, with ABI-preserved registers surviving calls.
    """
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = {}
    table_targets = {}
    entries = set()
    covered = bytearray(len(data))
    for section in InstructGen(bytes(data), start).sections:
        if section.type == SectionType.CODE and section.contents:
            first = section.contents[0][0]
            last, size, _, _ = section.contents[-1]
            decoded = list(
                decoder.disasm(data[first - start : last + size - start], first)
            )
            if [
                (i.address, i.size, i.mnemonic, i.op_str) for i in decoded
            ] != section.contents:
                return None
            instructions.update((i.address, i) for i in decoded)
            entries.add(first)
            covered[first - start : last + size - start] = b"\1" * (last + size - first)
        elif section.type == SectionType.ADDR_TAB:
            table_targets.update(section.contents)
            for address, _ in section.contents:
                covered[address - start : address - start + 4] = b"\1" * 4
        elif section.type == SectionType.DATA_TAB:
            for address, _ in section.contents:
                covered[address - start] = 1

    if not instructions or any(
        not seen and byte != 0xCC for seen, byte in zip(covered, data, strict=True)
    ):
        return None

    positions = {
        address: i for i, (address, _) in enumerate(assembly) if address is not None
    }
    targets = set(table_targets.values())
    if not targets <= instructions.keys():
        return None
    branches = {}
    texts = dict(assembly)
    for address, inst in instructions.items():
        if set(inst.groups) & {CS_GRP_INT, CS_GRP_IRET, CS_GRP_PRIVILEGE}:
            return None
        if inst.mnemonic == "jmp" and inst.operands[0].type != X86_OP_IMM:
            operand = inst.operands[0]
            if not (
                operand.type == X86_OP_MEM
                and operand.mem.disp in table_targets
                and operand.mem.base == 0
                and operand.mem.scale == 4
            ):
                return None
        if inst.mnemonic == "call" and inst.operands[0].type == X86_OP_IMM:
            if inst.operands[0].imm in instructions:
                # Local subroutines need interprocedural register/entry analysis.
                return None
        if inst.mnemonic in JUMP_MNEMONICS and inst.operands[0].type == X86_OP_IMM:
            target = inst.operands[0].imm
            if target in instructions:
                branches[address] = target
                targets.add(target)
            elif re.fullmatch(r"\w+ -?0x[0-9a-f]+", texts[address]):
                # Unresolved external or mid-instruction destination.
                return None
    entries.update(targets)
    # TEST leaves AF undefined; CMP with zero clears it. Never hide AF consumers.
    allow_zero_tests = not any(
        i.eflags & (X86_EFLAGS_TEST_AF | X86_EFLAGS_PRIOR_AF)
        or i.mnemonic in {"lahf", "pushf", "pushfd", "aaa", "aas", "daa", "das"}
        for i in instructions.values()
    )
    lines = []
    zero = set()
    for address, text in assembly:
        inst = instructions.get(address)
        if address in entries or inst is None:
            zero.clear()
        if inst is not None:
            operands = inst.operands
            if (
                allow_zero_tests
                and inst.mnemonic == "cmp"
                and operands[0].type == X86_OP_REG
            ):
                left, right = operands
                if (
                    right.type == X86_OP_IMM
                    and right.imm == 0
                    or right.type == X86_OP_REG
                    and inst.reg_name(right.reg) in zero
                ):
                    reg = inst.reg_name(left.reg)
                    text = f"test {reg}, {reg}"
            known_zero = None
            if len(operands) == 2 and operands[0].type == X86_OP_REG:
                left, right = operands
                dest = inst.reg_name(left.reg)
                if dest in GENERAL_REGISTERS and (
                    inst.mnemonic in {"xor", "sub"}
                    and right.type == X86_OP_REG
                    and left.reg == right.reg
                    or inst.mnemonic == "mov"
                    and (
                        right.type == X86_OP_IMM
                        and right.imm == 0
                        or right.type == X86_OP_REG
                        and inst.reg_name(right.reg) in zero
                    )
                ):
                    known_zero = dest
            _, writes = register_access(inst)
            zero.difference_update(writes)
            if known_zero is not None:
                zero.add(known_zero)
            if inst.mnemonic == "call":
                zero.difference_update({"eax", "ecx", "edx"})
            elif inst.mnemonic in JUMP_MNEMONICS or inst.group(CS_GRP_RET):
                zero.clear()
        lines.append(text)

    lines = number_placeholders(lines, symbols)
    result = []
    pending_lines, pending_instructions = [], []

    def flush():
        result.extend(schedule(pending_lines, pending_instructions))
        pending_lines.clear()
        pending_instructions.clear()

    for (address, _), line in zip(assembly, lines, strict=True):
        inst = instructions.get(address)
        if address in entries:
            flush()
        if inst is not None and schedulable(inst):
            pending_lines.append(line)
            pending_instructions.append(inst)
        else:
            flush()
            result.append(line)
    flush()
    # Keep byte displacements unchanged; also reject changed destination indices.
    # Equal displacements can reach different instructions after a size change.
    edges = tuple(
        (positions[source], positions[target])
        for source, target in sorted((branches | table_targets).items())
    )
    return result, edges
