"""Repository-local compatibility fixes for the pinned reccmp 0.1.7 parser.

These fixes correct instruction/data boundaries and relocation recognition.
The existing name replacement and matching criteria remain in use.
The installed package is not modified.
"""

from importlib.metadata import version
import struct

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from reccmp.compare.asm import parse
from reccmp.compare.asm.instgen import InstructGen, SectionType


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
