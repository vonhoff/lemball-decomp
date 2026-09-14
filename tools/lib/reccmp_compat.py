"""Repository-local compatibility fixes for the pinned reccmp 0.1.7 parser.

These fixes correct instruction/data boundaries and relocation recognition.
The existing name replacement and matching criteria remain in use.
The installed package is not modified.
"""

from importlib.metadata import version
import struct
import re

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from reccmp.compare.asm import fixes, parse
from reccmp.compare.asm.instgen import InstructGen, SectionType

_upstream_relocate_instructions = fixes.relocate_instructions
_register_tokens = re.compile(r"\b(eax|ax|al|ah|ebx|bx|bl|bh|ecx|cx|cl|ch|edx|dx|dl|dh|esi|si|edi|di|ebp|bp|esp|sp)\b")
_register_families = {
    alias: family
    for family, aliases in (
        ("eax", ("eax", "ax", "al", "ah")),
        ("ebx", ("ebx", "bx", "bl", "bh")),
        ("ecx", ("ecx", "cx", "cl", "ch")),
        ("edx", ("edx", "dx", "dl", "dh")),
        ("esi", ("esi", "si")), ("edi", ("edi", "di")),
        ("ebp", ("ebp", "bp")), ("esp", ("esp", "sp")),
    ) for alias in aliases
}


def relocate_instructions(codes, orig_asm, recomp_asm):
    """Fix the forward-move self-dependency, with conservative safety checks."""
    fixed = _upstream_relocate_instructions(codes, orig_asm, recomp_asm)
    deletes = [i for code, i1, i2, _, _ in codes if code == "delete" for i in range(i1, i2)]
    inserts = [(i1, j) for code, i1, _, j1, j2 in codes if code == "insert" for j in range(j1, j2)]
    transparent = {"mov", "lea", "cmp", "test", "push", "pop", "add", "sub",
                   "inc", "dec", "and", "or", "xor", "shl", "shr", "sar"}
    for destination, j in inserts:
        if j in fixed:
            continue
        line = recomp_asm[j]
        candidates = [i for i in deletes if orig_asm[i] == line]
        if len(candidates) != 1 or sum(recomp_asm[k] == line for _, k in inserts) != 1:
            continue
        i = candidates[0]
        if destination <= i:
            continue
        mnemonic, _, operands = line.partition(" ")
        target, separator, source = operands.partition(", ")
        if mnemonic not in ("mov", "lea") or not separator or target not in fixes.DWORD_REGS:
            continue
        if mnemonic == "mov" and "[" in source:
            continue
        if mnemonic == "mov" and not (
            source in fixes.DWORD_REGS
            or re.fullmatch(r"-?(?:0x[0-9a-f]+|[0-9]+)|<OFFSET[0-9]*>", source)
            or re.search(r" \((?:DATA|VTABLE|UNK|FUNCTION|IMPORT|IMPORT_THUNK|STRING|OFFSET)\)$", source)
        ):
            # Segment/control registers have dependencies outside the GPR set.
            continue
        registers = {_register_families[reg] for reg in _register_tokens.findall(operands)}
        if "esp" in registers:
            continue
        crossed = orig_asm[i + 1:destination]
        if any(
            instruction.partition(" ")[0] not in transparent
            or registers.intersection(_register_families[reg] for reg in _register_tokens.findall(instruction))
            for instruction in crossed
        ):
            continue
        fixed.add(j)
    return fixed


class RelocationAwareParseAsm(parse.ParseAsm):
    """Recognize CMP pointer immediates at verified PE relocation sites."""

    def __init__(self, *, relocation_sites=(), **kwargs):
        super().__init__(**kwargs)
        self.relocation_sites = frozenset(relocation_sites)
        self._decoder = Cs(CS_ARCH_X86, CS_MODE_32)
        self._decoder.detail = True

    def parse_asm(self, data, start_addr):
        self._data = bytes(data)
        self._start = start_addr
        return super().parse_asm(data, start_addr)

    def sanitize(self, inst):
        address, size, mnemonic, operands = inst
        if (
            self.is_32bit
            and mnemonic == "cmp"
            and address + size - 4 in self.relocation_sites
        ):
            offset = address - self._start
            decoded = next(
                self._decoder.disasm(self._data[offset : offset + size], address)
            )
            if (
                decoded.imm_size == 4
                and address + decoded.imm_offset in self.relocation_sites
            ):
                # Use the normal pointer-name/placeholder path. The relocation
                # belongs to this operand, not merely to an equal value elsewhere.
                value = int(operands.rpartition(", ")[2], 16)
                mnemonic, sanitized = super().sanitize(inst)
                head, separator, _ = sanitized.rpartition(", ")
                return mnemonic, head + separator + self.replace(value)
        return super().sanitize(inst)


def configure_pointer_comparisons(engine) -> None:
    comparator = engine.function_comparator
    for side in ("orig", "recomp"):
        image = getattr(comparator, side + "_bin")
        parser = getattr(comparator, side + "_sanitize")
        setattr(
            comparator,
            side + "_sanitize",
            RelocationAwareParseAsm(
                relocation_sites=image.relocations,
                addr_test=parser.addr_test,
                name_lookup=parser.name_lookup,
                is_32bit=parser.is_32bit,
            ),
        )


class BoundedInstructGen(InstructGen):
    """Stop address tables at code boundaries discovered from their entries."""

    def _next_section(self, addr: int) -> SectionType | None:
        section_type = super()._next_section(addr)
        if section_type == SectionType.ADDR_TAB:
            # Upstream snapshots read_size before discovering code targets. A
            # target can shorten section_end, leaving instructions in that saved
            # table slice. Discover boundaries first, checking the updated end
            # before each dword; upstream then reads only the bounded table.
            cursor = addr
            while cursor + 4 <= self.section_end:
                (target,) = struct.unpack_from("<I", self.blob, cursor - self.start)
                self._insert_confirmed_addr(target, SectionType.CODE)
                cursor += 4
        return section_type


def install_parser_fix() -> None:
    if version("reccmp") != "0.1.7":
        raise RuntimeError("Review the parser compatibility fix before changing reccmp==0.1.7")
    parse.InstructGen = BoundedInstructGen
    fixes.relocate_instructions = relocate_instructions
