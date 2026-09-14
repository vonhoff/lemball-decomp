"""Boundary and fidelity checks for the repository-local reccmp parser fix."""

import struct
import unittest
from unittest.mock import patch

from lib.reccmp_compat import BoundedInstructGen, RelocationAwareParseAsm, table_end_expressions
from reccmp.compare.asm import parse
from reccmp.compare.asm.instgen import InstructGen, SectionType


def dispatch(table: int, register: int = 0) -> bytes:
    return bytes((0xFF, 0x24, 0x85 + register * 8)) + struct.pack("<I", table)


def embedded_table(start: int, value: int = 1) -> bytes:
    return (
        dispatch(start + 8) + b"\x90"
        + struct.pack("<4I", start + 30, start + 24, start + 30, start + 24)
        + b"\xb8" + struct.pack("<I", value) + b"\xc3"
        + b"\xb8\x02\x00\x00\x00\xc3"
    )


def normalized(blob: bytes, start: int) -> list[str]:
    with patch.object(parse, "InstructGen", BoundedInstructGen):
        parser = parse.ParseAsm(addr_test=lambda address: address == start + 8)
        return [text for _, text in parser.parse_asm(blob, start)]


class JumpTableBoundaryTests(unittest.TestCase):
    def test_embedded_table_does_not_include_following_instructions(self):
        start = 0x1000
        sections = BoundedInstructGen(embedded_table(start), start).sections
        tables = [s for s in sections if s.type == SectionType.ADDR_TAB]
        self.assertEqual([len(s.contents) for s in tables], [4])
        table_bytes = {address + i for s in tables for address, _ in s.contents for i in range(4)}
        code_bytes = {
            address + i for s in sections if s.type == SectionType.CODE
            for address, size, _, _ in s.contents for i in range(size)
        }
        self.assertFalse(table_bytes & code_bytes)
        self.assertEqual(table_bytes | code_bytes, set(range(start, start + 36)))

    def test_two_embedded_tables(self):
        start = 0x2000
        blob = (dispatch(start + 8) + b"\x90"
                + struct.pack("<4I", start + 24, start + 31, start + 24, start + 31)
                + dispatch(start + 32, 2) + b"\x90"
                + struct.pack("<2I", start + 40, start + 46)
                + b"\xb8\x01\x00\x00\x00\xc3\xb8\x02\x00\x00\x00\xc3")
        tables = [s for s in BoundedInstructGen(blob, start).sections if s.type == SectionType.ADDR_TAB]
        self.assertEqual([len(s.contents) for s in tables], [4, 2])

    def test_relocated_identical_code_remains_identical(self):
        self.assertEqual(normalized(embedded_table(0x1000), 0x1000),
                         normalized(embedded_table(0x3000), 0x3000))

    def test_instruction_difference_remains_visible(self):
        self.assertNotEqual(normalized(embedded_table(0x1000, 1), 0x1000),
                            normalized(embedded_table(0x3000, 9), 0x3000))

    def test_table_target_difference_remains_visible(self):
        blob = bytearray(embedded_table(0x3000))
        struct.pack_into("<I", blob, 8, 0x3018)
        self.assertNotEqual(normalized(embedded_table(0x1000), 0x1000), normalized(blob, 0x3000))

    def test_unaffected_sections_match_upstream(self):
        start = 0x4000
        fixtures = (
            b"\xb8\x01\x00\x00\x00\xc3",
            dispatch(start + 8) + b"\x90" + struct.pack("<2I", start, start),
            b"\x0f\xb6\x80" + struct.pack("<I", start + 8) + b"\xc3\x00\x01\x02",
        )
        for blob in fixtures:
            with self.subTest(blob=blob.hex()):
                self.assertEqual(BoundedInstructGen(blob, start).sections, InstructGen(blob, start).sections)


class TableEndComparisonTests(unittest.TestCase):
    def edx_loop(self, begin=0x4000, length=61 * 8):
        return (b"\xba" + struct.pack("<I", begin)
                + bytes.fromhex("8b4708 3902 741a 83c208 46 81fa")
                + struct.pack("<I", begin + length) + b"\x72\xf0")

    def test_edx_table_bound_keeps_base_and_length_not_adjacent_symbol(self):
        rendered = []
        for begin, adjacent in ((0x4000, "debug (DATA)"), (0x8000, "path (DATA)")):
            parser = RelocationAwareParseAsm(
                relocation_sites=(0x1001, 0x1012), addr_test=lambda address: True,
                name_lookup=lambda address, begin=begin, adjacent=adjacent, **kwargs:
                    {begin: "inputs (DATA)", begin + 61 * 8: adjacent}.get(address),
            )
            rendered.append([line for _, line in parser.parse_asm(self.edx_loop(begin), 0x1000)])
        self.assertEqual(*rendered)
        self.assertIn("cmp edx, inputs (DATA) + 0x1e8", rendered[0])

    def test_edx_scan_rejects_missing_relocations_and_changed_flow(self):
        for sites in ((), (0x1001,), (0x1012,)):
            self.assertEqual(table_end_expressions(self.edx_loop(), 0x1000, sites), {})
        for offset, value in ((5, 0x89), (6, 0x57), (8, 0x89), (10, 0x75), (11, 0),
                              (14, 0), (14, 0xff), (15, 0x42), (17, 0xf8), (23, 0xf1)):
            with self.subTest(offset=offset):
                blob = bytearray(self.edx_loop())
                blob[offset] = value
                self.assertEqual(table_end_expressions(blob, 0x1000, (0x1001, 0x1012)), {})
        for length in (0, -8, 7, 0x10008):
            self.assertEqual(table_end_expressions(self.edx_loop(length=length), 0x1000,
                                                  (0x1001, 0x1012)), {})
        self.assertEqual(table_end_expressions(b"\x68" + self.edx_loop(), 0x1000,
                                              (0x1002, 0x1013)), {})

    def loop(self, begin=0x4000, length=0x168):
        return (b"\xb8" + struct.pack("<I", begin)
                + b"\x39\x08\x74\x22\x83\xc0\x08\x46\x3d"
                + struct.pack("<I", begin + length) + b"\x72\xf1")

    def render(self, begin, adjacent_name, length=0x168, base_name="errors (DATA)"):
        def names(address, **kwargs):
            return {begin: base_name, begin + length: adjacent_name}.get(address)
        parser = RelocationAwareParseAsm(
            relocation_sites=(0x1001, 0x100E), name_lookup=names,
            addr_test=lambda address: True,
        )
        return [line for _, line in parser.parse_asm(self.loop(begin, length), 0x1000)]

    def test_adjacent_symbol_does_not_name_table_bound(self):
        original = self.render(0x4000, "next_original (DATA)")
        rebuilt = self.render(0x8000, "next_rebuilt (DATA)")
        self.assertEqual(original, rebuilt)
        self.assertIn("cmp eax, errors (DATA) + 0x168", original)

    def test_different_bounds_and_base_symbols_remain_distinct(self):
        original = self.render(0x4000, "next (DATA)")
        self.assertNotEqual(original, self.render(0x8000, "next (DATA)", length=0x160))
        self.assertNotEqual(original, self.render(0x8000, "next (DATA)", base_name="other (DATA)"))

    def test_requires_both_operand_relocations(self):
        for sites in ((), (0x1001,), (0x100E,)):
            with self.subTest(sites=sites):
                self.assertEqual(table_end_expressions(self.loop(), 0x1000, sites), {})

    def test_declines_changed_control_flow_and_pointer_updates(self):
        for offset, value in ((5, 0x89), (7, 0x75), (8, 0), (9, 0x81),
                              (11, 0), (11, 0xFF), (12, 0x40), (18, 0x73), (19, 0xF0)):
            blob = bytearray(self.loop())
            blob[offset] = value
            with self.subTest(offset=offset, value=value):
                self.assertEqual(table_end_expressions(blob, 0x1000, (0x1001, 0x100E)), {})

    def test_declines_invalid_extent_and_embedded_opcode(self):
        for length in (0, -8, 7, 0x10008):
            with self.subTest(length=length):
                self.assertEqual(table_end_expressions(self.loop(length=length), 0x1000,
                                                      (0x1001, 0x100E)), {})
        # The apparent MOV starts inside a PUSH immediate, not at an instruction.
        blob = b"\x68" + self.loop()
        self.assertEqual(table_end_expressions(blob, 0x1000, (0x1002, 0x100F)), {})


class PointerComparisonTests(unittest.TestCase):
    def test_upstream_reproduces_missing_pointer_normalization(self):
        blob = b"\x3d" + struct.pack("<I", 0x4000)
        upstream = parse.ParseAsm(addr_test=lambda address: True)
        self.assertEqual(upstream.parse_asm(blob, 0x1000)[0][1], "cmp eax, 0x4000")

    def render(self, value, sites=(), name_lookup=None, opcode=b"\x3d", start=0x1000):
        parser = RelocationAwareParseAsm(
            relocation_sites=sites, name_lookup=name_lookup,
            addr_test=lambda address: True,
        )
        return [text for _, text in parser.parse_asm(opcode + struct.pack("<I", value), start)]

    def test_relocated_pointer_comparisons_match(self):
        self.assertEqual(self.render(0x4000, (0x1001,)),
                         self.render(0x8000, (0x2001,), start=0x2000))
        self.assertEqual(self.render(0x4000, (0x1001,)), ["cmp eax, <OFFSET1>"])

    def test_equal_value_relocated_elsewhere_does_not_hide_constants(self):
        self.assertNotEqual(self.render(0x4000, (0x9999,)), self.render(0x8000, (0x9999,)))
        self.assertEqual(self.render(0x4000), ["cmp eax, 0x4000"])

    def test_named_pointer_targets_remain_distinct(self):
        def names(address, **kwargs):
            return {0x4000: "first (DATA)", 0x8000: "second (DATA)"}.get(address)
        self.assertNotEqual(self.render(0x4000, (0x1001,), names),
                            self.render(0x8000, (0x1001,), names))

    def test_non_immediate_relocation_does_not_hide_constant(self):
        parser = RelocationAwareParseAsm(relocation_sites=(0x1002,))
        blob = b"\x81\x3d" + struct.pack("<II", 0x4000, 0x8000)
        self.assertEqual(parser.parse_asm(blob, 0x1000)[0][1],
                         "cmp dword ptr [<OFFSET1>], 0x8000")

    def test_named_comparison_counts_toward_later_placeholders(self):
        def names(address, **kwargs):
            return "limit (DATA)" if address == 0x4000 else None
        parser = RelocationAwareParseAsm(relocation_sites=(0x1001,), name_lookup=names,
                                         addr_test=lambda address: True)
        blob = b"\x3d" + struct.pack("<I", 0x4000) + b"\xb8" + struct.pack("<I", 0x5000)
        self.assertEqual([text for _, text in parser.parse_asm(blob, 0x1000)],
                         ["cmp eax, limit (DATA)", "mov eax, <OFFSET2>"])
