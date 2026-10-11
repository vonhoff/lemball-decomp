"""Small assembly-normalization fixes for the pinned reccmp 0.1.7."""

from unittest.mock import patch

from reccmp.compare import Compare as BaseCompare
from reccmp.compare.asm.fixes import assert_fixup
from reccmp.compare.asm.instgen import InstructGen, get_disassembler
from reccmp.compare.asm.parse import ParseAsm
from reccmp.types import EntityType, ImageId


class Instructions(InstructGen):
    def _finish_tab_section(self, type_, stuff):
        # Table destinations can shorten section_end after the initial read.
        super()._finish_tab_section(
            type_, [row for row in stuff if row[0] < self.section_end]
        )

    def _get_code_for(self, addr):
        # Interior INT3 is executable; only trailing INT3 bytes are padding.
        instructions = list(
            get_disassembler(self.is_32bit).disasm_lite(
                self.blob[addr - self.start :], addr
            )
        )
        while instructions and instructions[-1][2] == "int3":
            instructions.pop()
        return instructions


class Parser(ParseAsm):
    def __init__(self, parser, db, image_id):
        super().__init__(parser.addr_test, parser.name_lookup, parser.is_32bit)
        self.db = db
        self.image_id = image_id

    def lookup(self, addr, exact=False, indirect=False):
        if not exact and not indirect:
            entity = self.db.get(self.image_id, addr, exact=False)
            while entity is not None and not entity.matched:
                entity = self.db.get(
                    self.image_id, entity.addr(self.image_id) - 1, exact=False
                )
            if entity is not None and entity.entity_type == EntityType.STRING:
                offset = addr - entity.addr(self.image_id)
                if 0 < offset < entity.any_size(self.image_id):
                    return entity.match_name(f"+{offset}")
        return super().lookup(addr, exact, indirect)

    def hex_replace_annotated(self, match):
        addr = int(match.group(1), 16)
        entity = self.db.get(self.image_id, addr - 1, exact=False)
        if entity is not None and entity.matched:
            offset = addr - entity.addr(self.image_id)
            if entity.entity_type == EntityType.FUNCTION and offset < entity.any_size(
                self.image_id
            ):
                return entity.match_name(f"+{offset}")
            if entity.entity_type == EntityType.DATA and offset == entity.max_size(
                ImageId.ORIG
            ) == entity.size(ImageId.RECOMP):
                # A one-past pointer can coincide with an unrelated next global.
                return entity.match_name(f"+{offset}")
        return super().hex_replace_annotated(match)

    def parse_asm(self, data, start_addr):
        with patch("reccmp.compare.asm.parse.InstructGen", Instructions):
            asm = super().parse_asm(data, start_addr)
        # Release CRTs contain assertions even without a PE debug directory.
        assert_fixup(asm)
        return asm


class Compare(BaseCompare):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        comparator = self.function_comparator
        comparator.orig_sanitize = Parser(
            comparator.orig_sanitize, comparator.db, ImageId.ORIG
        )
        comparator.recomp_sanitize = Parser(
            comparator.recomp_sanitize, comparator.db, ImageId.RECOMP
        )
