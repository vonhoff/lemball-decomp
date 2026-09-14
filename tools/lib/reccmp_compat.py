"""Repository-local compatibility fixes for the pinned reccmp 0.1.7 parser.

These fixes affect instruction/data boundaries, not matching or normalization.
The installed package is not modified.
"""

from importlib.metadata import version
import struct

from reccmp.compare.asm import parse
from reccmp.compare.asm.instgen import InstructGen, SectionType


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
                target, = struct.unpack_from("<I", self.blob, cursor - self.start)
                self._insert_confirmed_addr(target, SectionType.CODE)
                cursor += 4
        return section_type


def install_parser_fix() -> None:
    if version("reccmp") != "0.1.7":
        raise RuntimeError("Review the parser compatibility fix before changing reccmp==0.1.7")
    parse.InstructGen = BoundedInstructGen
