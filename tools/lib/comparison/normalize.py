"""Conservative assembly canonicalization for additional Effective matches."""

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


def schedule(items):
    """Sort a run only when every instruction is independent of every other."""
    lines = [line for line, _ in items]
    accesses = [register_access(inst) for _, inst in items]
    # ponytail: independent runs only; add DAG ordering if mixed runs yield matches.
    dependent = any(
        writes_i & (reads_j | writes_j) or reads_i & writes_j
        for j, (reads_j, writes_j) in enumerate(accesses)
        for reads_i, writes_i in accesses[:j]
    )
    return lines if dependent else sorted(lines)


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
    end = start
    for section in InstructGen(bytes(data), start).sections:
        if not section.contents:
            continue
        first = section.contents[0][0]
        if data[end - start : first - start].strip(b"\xcc"):
            return None
        if section.type == SectionType.CODE:
            last, size, _, _ = section.contents[-1]
            end = last + size
            instructions.update(
                (i.address, i)
                for i in decoder.disasm(data[first - start : end - start], first)
            )
            entries.add(first)
        else:
            width = 4 if section.type == SectionType.ADDR_TAB else 1
            end = section.contents[-1][0] + width
            if section.type == SectionType.ADDR_TAB:
                table_targets.update(section.contents)

    if not instructions or data[end - start :].strip(b"\xcc"):
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
            known_zero = None
            operands = inst.op_str.split(", ")
            if len(operands) == 2 and operands[0] in REGISTER_FAMILIES:
                reg, source = operands
                is_zero = source == "0" or source in zero
                if allow_zero_tests and inst.mnemonic == "cmp" and is_zero:
                    text = f"test {reg}, {reg}"
                if reg in GENERAL_REGISTERS and (
                    inst.mnemonic in {"xor", "sub"}
                    and reg == source
                    or inst.mnemonic == "mov"
                    and is_zero
                ):
                    known_zero = reg
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
    pending = []
    for (address, _), line in zip(assembly, lines, strict=True):
        inst = instructions.get(address)
        movable = inst is not None and schedulable(inst)
        if address in entries or not movable:
            result.extend(schedule(pending))
            pending.clear()
        if movable:
            pending.append((line, inst))
        else:
            result.append(line)
    result.extend(schedule(pending))
    # Keep byte displacements unchanged; also reject changed destination indices.
    # Equal displacements can reach different instructions after a size change.
    edges = tuple(
        (positions[source], positions[target])
        for source, target in sorted((branches | table_targets).items())
    )
    return result, edges
