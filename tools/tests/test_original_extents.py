"""An original function must not be truncated to a shorter rebuilt length."""

import unittest
import struct

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from reccmp.compare.db import ReccmpMatch
from reccmp.compare.functions import FunctionComparator
from reccmp.compare.asm.parse import ParseAsm
from reccmp.types import ImageId

from lib.paths import ORIGINAL_EXE, RECOMP_EXE, RECOMP_PDB
from lib.reccmp import load_engine
from lib.reccmp_compat import complete_original_extent, extend_original_match
from lib.reccmp_compat import BoundedInstructGen
from reccmp.compare.asm.instgen import SectionType


class BytesImage:
    def __init__(self, data, base=0x1000):
        self.data = data
        self.base = base

    def read(self, address, size):
        offset = address - self.base
        if not 0 <= offset < len(self.data):
            raise ValueError("Address is outside the test image")
        return self.data[offset:offset + size]


class OriginalExtentTests(unittest.TestCase):
    def setUp(self):
        self.decoder = Cs(CS_ARCH_X86, CS_MODE_32)

    def extent(self, data, limit=None):
        return complete_original_extent(
            BytesImage(data), 0x1000, 0x1000 + (limit or len(data)), self.decoder
        )

    def test_original_tail_is_included_without_changing_inventory(self):
        image = BytesImage(bytes.fromhex("b8 01000000 40 c3 cc cc"))
        match = ReccmpMatch(0x1000, 0x2000, {"recomp_size": 6, "orig_max_size": 9})
        extended = extend_original_match(match, image, self.decoder)
        self.assertEqual(extended.size(ImageId.ORIG), 7)
        self.assertEqual(extended.size(ImageId.RECOMP), 6)
        self.assertIsNone(match.size(ImageId.ORIG))
        self.assertEqual(match.any_size(), extended.any_size())

    def test_prefix_only_rebuild_cannot_receive_a_false_exact_match(self):
        original = bytes.fromhex("b8 01000000 40 c3")
        comparator = object.__new__(FunctionComparator)
        comparator.orig_bin = BytesImage(original)
        comparator.recomp_bin = BytesImage(original[:5], base=0x2000)
        comparator.orig_sanitize = ParseAsm()
        comparator.recomp_sanitize = ParseAsm()
        comparator._collect_line_annotations = lambda _: []
        comparator._source_ref_of_recomp_addr = lambda _: None
        match = ReccmpMatch(0x1000, 0x2000, {"recomp_size": 5, "orig_max_size": 7})
        self.assertEqual(comparator.compare_function(match).match_ratio, 1.0)
        fixed = comparator.compare_function(
            extend_original_match(match, comparator.orig_bin, self.decoder)
        )
        self.assertLess(fixed.match_ratio, 1.0)
        self.assertFalse(fixed.is_effective_match)

    def test_all_conditional_paths_are_followed(self):
        # The second return is beyond the first return and two padding bytes.
        self.assertEqual(self.extent(bytes.fromhex("74 03 c3 cc cc 40 c3")), 7)

    def test_calls_keep_the_fallthrough_path(self):
        self.assertEqual(self.extent(bytes.fromhex("e8 00000000 40 c3")), 7)

    def test_uncertain_paths_do_not_supply_a_size(self):
        for data in (
            "ff e0",             # indirect dispatch
            "eb 7f",             # branch outside the known bound
            "74 01 b8 00000000 c3",  # branch into an instruction
            "eb fe",             # no return evidence
            "90 0f",             # incomplete instruction
            "74 01 c3 cc",       # one path terminates in an interrupt
        ):
            with self.subTest(data=data):
                self.assertIsNone(self.extent(bytes.fromhex(data)))
        self.assertIsNone(self.extent(bytes.fromhex("90 90 c3"), limit=2))

    def test_explicit_sizes_and_nonextensions_keep_upstream_behavior(self):
        image = BytesImage(bytes.fromhex("90 c3 cc cc"))
        for metadata in (
            {"orig_size": 2, "recomp_size": 1, "orig_max_size": 4},
            {"recomp_size": 3, "orig_max_size": 4},
            {"recomp_size": 1},
            {"recomp_size": 4, "orig_max_size": 4},
        ):
            with self.subTest(metadata=metadata):
                match = ReccmpMatch(0x1000, 0x2000, metadata)
                self.assertIs(extend_original_match(match, image, self.decoder), match)

    def switch(self, near=False):
        data = bytearray(b"\xcc" * 0xa0)
        guard = (bytes.fromhex("3d 01000000 0f87") + struct.pack("<i", 0x40 - 11)
                 if near else bytes.fromhex("83f801 773b"))
        dispatch = (bytes.fromhex("33c9 8a88") + struct.pack("<I", 0x1090)
                    + bytes.fromhex("ff248d") + struct.pack("<I", 0x1080))
        data[:len(guard + dispatch)] = guard + dispatch
        data[0x40] = 0xc3
        data[0x42:0x44] = bytes.fromhex("40 c3")
        data[0x44:0x46] = bytes.fromhex("41 c3")
        struct.pack_into("<II", data, 0x80, 0x1042, 0x1044)
        data[0x90:0x92] = bytes((0, 1))
        return data

    def test_bounded_switch_includes_every_case_and_both_tables(self):
        for near in (False, True):
            with self.subTest(near=near):
                self.assertEqual(self.extent(self.switch(near)), 0x92)
        image = BytesImage(self.switch())
        match = ReccmpMatch(0x1000, 0x2000, {"recomp_size": 0x20, "orig_max_size": 0xa0})
        extended = extend_original_match(match, image, self.decoder)
        self.assertEqual(extended.size(ImageId.ORIG), 0x92)
        self.assertEqual(extended.any_size(), match.any_size())

    def direct_switch(self, near=False):
        data = self.switch(near)
        cursor = 11 if near else 5
        data[cursor:cursor + 15] = b"\xcc" * 15
        data[cursor:cursor + 7] = bytes.fromhex("ff2485") + struct.pack("<I", 0x1080)
        return data

    def test_direct_switch_includes_all_targets_and_table(self):
        for near in (False, True):
            with self.subTest(near=near):
                self.assertEqual(self.extent(self.direct_switch(near)), 0x88)

    def test_proven_table_length_preserves_following_bytes_as_data(self):
        data = self.direct_switch()
        # The upstream instruction reader stops at INT3; use decodable padding
        # so this fixture exercises the table reader as well as the CFG proof.
        data[12:0x40] = b"\x90" * (0x40 - 12)
        data[0x41] = 0x90
        data[0x46:0x80] = b"\x90" * (0x80 - 0x46)
        # Even a following word that looks like a valid code address is not
        # a third entry: CMP EAX,1 proves the two-entry dispatch.
        struct.pack_into("<I", data, 0x88, 0x1042)
        sections = BoundedInstructGen(data, 0x1000).sections
        tables = [s for s in sections if s.type == SectionType.ADDR_TAB]
        self.assertEqual([len(s.contents) for s in tables], [2])
        trailing = [pair for s in sections if s.type == SectionType.DATA_TAB
                    for pair in s.contents if pair[0] >= 0x1088]
        self.assertEqual(trailing, list(enumerate(data[0x88:], 0x1088)))
        data[0x88] ^= 1
        changed = BoundedInstructGen(data, 0x1000).sections
        self.assertNotEqual(sections, changed)

    def test_unproven_control_flow_does_not_supply_table_lengths(self):
        data = self.direct_switch()
        data[0x40:0x42] = bytes.fromhex("ff e0")
        self.assertEqual(BoundedInstructGen(data, 0x1000)._table_bounds, {})

    def switch_with_guard_store(self, near=False):
        data = self.direct_switch(near)
        compare_size, guard_size = (5, 11) if near else (3, 5)
        dispatch = bytes(data[compare_size:guard_size + 7])
        data[compare_size:guard_size + 12] = bytes.fromhex("66 89 5c 24 30") + dispatch
        # Moving JA five bytes forward reduces its displacement by five.
        if near:
            struct.pack_into("<i", data, compare_size + 7, 0x40 - guard_size - 5)
        else:
            data[guard_size + 4] -= 5
        return data

    def test_guard_stack_store_preserves_range_check(self):
        for near in (False, True):
            with self.subTest(near=near):
                self.assertEqual(self.extent(self.switch_with_guard_store(near)), 0x88)

    def test_guard_store_rejects_flag_or_index_changes(self):
        for replacement in ("66 01 5c 24 30", "66 8b 44 24 30", "66 89 c0 90 90",
                            "66 89 5c 85 30", "66 89 1d 24 30"):
            with self.subTest(replacement=replacement):
                data = self.switch_with_guard_store()
                data[3:8] = bytes.fromhex(replacement)
                self.assertIsNone(self.extent(data))

    def test_guard_store_cannot_be_entered_without_comparison(self):
        for offset in (3, 8, 10):
            with self.subTest(offset=offset):
                data = self.switch_with_guard_store()
                data[0x40:0x42] = bytes((0xeb, (offset - 0x42) & 0xff))
                self.assertIsNone(self.extent(data))

    def test_guard_store_keeps_table_bounds_and_target_checks(self):
        self.assertIsNone(self.extent(self.switch_with_guard_store(), limit=0x87))
        for target in (0x0fff, 0x10a0, 0x1080, 0x1004):
            with self.subTest(target=target):
                data = self.switch_with_guard_store()
                struct.pack_into("<I", data, 0x84, target)
                self.assertIsNone(self.extent(data))

    def test_direct_switch_requires_matching_guard_and_index(self):
        for offset, value in ((1, 0xfa), (2, 0xff), (3, 0x7f), (7, 0x8d), (7, 0x45)):
            with self.subTest(offset=offset, value=value):
                data = self.direct_switch()
                data[offset] = value
                self.assertIsNone(self.extent(data))

    def test_direct_switch_requires_complete_table_inside_bound(self):
        self.assertIsNone(self.extent(self.direct_switch(), limit=0x87))
        for pointer in (0x0ffc, 0x109c, 0x1000):
            with self.subTest(pointer=pointer):
                data = self.direct_switch()
                struct.pack_into("<I", data, 8, pointer)
                self.assertIsNone(self.extent(data))

    def test_direct_switch_checks_every_target_and_case_path(self):
        for target in (0x0fff, 0x10a0, 0x1080, 0x1005, 0x1043, 0x1060):
            with self.subTest(target=target):
                data = self.direct_switch()
                data[0x42:0x48] = bytes.fromhex("b8 01000000 c3")
                data[0x60:0x62] = bytes.fromhex("ff e0")
                struct.pack_into("<I", data, 0x84, target)
                self.assertIsNone(self.extent(data))

    def test_direct_switch_external_edge_cannot_bypass_guard(self):
        data = self.direct_switch()
        # Default returns to the indirect jump, bypassing the proven range check.
        data[0x40:0x42] = bytes.fromhex("eb c3")
        self.assertIsNone(self.extent(data))

    def test_switch_requires_unsigned_guard_and_zero_extended_index(self):
        for offset, value in ((3, 0x7f), (2, 0xff), (5, 0x90), (8, 0x89), (16, 0x85)):
            with self.subTest(offset=offset):
                data = self.switch()
                data[offset] = value
                self.assertIsNone(self.extent(data))

    def edx_switch(self):
        data = self.switch()
        data[1] = 0xfa  # CMP EDX,maximum
        data[6] = 0xc0  # XOR EAX,EAX
        data[8] = 0x82  # MOV AL,[EDX+byte_table]
        data[15] = 0x85  # JMP [EAX*4+target_table]
        return data

    def test_edx_switch_includes_complete_tables(self):
        self.assertEqual(self.extent(self.edx_switch()), 0x92)

    def edi_switch(self):
        data = self.edx_switch()
        data[1] = 0xff  # CMP EDI,maximum
        data[8] = 0x87  # MOV AL,[EDI+byte_table]
        return data

    def test_edi_switch_includes_complete_tables(self):
        self.assertEqual(self.extent(self.edi_switch()), 0x92)

    def test_edi_switch_rejects_inconsistent_registers(self):
        for offset, value in ((1, 0xfa), (6, 0xc9), (8, 0x82), (15, 0x8d)):
            with self.subTest(offset=offset):
                data = self.edi_switch()
                data[offset] = value
                self.assertIsNone(self.extent(data))

    def test_edi_switch_keeps_bounds_and_control_flow_checks(self):
        self.assertIsNone(self.extent(self.edi_switch(), limit=0x91))
        for target in (0x0fff, 0x10a0, 0x1080, 0x1090, 0x1005, 0x1043):
            with self.subTest(target=target):
                data = self.edi_switch()
                data[0x42:0x48] = bytes.fromhex("b8 01000000 c3")
                struct.pack_into("<I", data, 0x84, target)
                self.assertIsNone(self.extent(data))
        data = self.edi_switch()
        data[0x40:0x42] = bytes.fromhex("eb c3")
        self.assertIsNone(self.extent(data))

    def test_edx_switch_rejects_inconsistent_registers(self):
        for offset, value in ((1, 0xf8), (6, 0xc9), (8, 0x80), (15, 0x8d)):
            with self.subTest(offset=offset):
                data = self.edx_switch()
                data[offset] = value
                self.assertIsNone(self.extent(data))

    def test_edx_switch_keeps_bounds_and_control_flow_checks(self):
        self.assertIsNone(self.extent(self.edx_switch(), limit=0x91))
        for target in (0x0fff, 0x10a0, 0x1080, 0x1090, 0x1005):
            with self.subTest(target=target):
                data = self.edx_switch()
                struct.pack_into("<I", data, 0x84, target)
                self.assertIsNone(self.extent(data))

    def test_switch_tables_must_be_complete_disjoint_and_within_bound(self):
        for location, pointer in ((9, 0x0ff0), (9, 0x10a0), (16, 0x109c),
                                  (9, 0x1080), (16, 0x1000)):
            with self.subTest(location=location, pointer=pointer):
                data = self.switch()
                struct.pack_into("<I", data, location, pointer)
                self.assertIsNone(self.extent(data))
        self.assertIsNone(self.extent(self.switch(), limit=0x91))

    def test_switch_rejects_targets_outside_code_or_into_dispatch(self):
        for target in (0x0fff, 0x10a0, 0x1080, 0x1090, 0x1005, 0x1043):
            with self.subTest(target=target):
                data = self.switch()
                # Second case overlaps the interior of a multibyte first case.
                data[0x42:0x48] = bytes.fromhex("b8 01000000 c3")
                struct.pack_into("<I", data, 0x84, target)
                self.assertIsNone(self.extent(data))

    def test_switch_requires_closed_case_paths(self):
        data = self.switch()
        data[0x44:0x46] = bytes.fromhex("ff e0")
        self.assertIsNone(self.extent(data))

    def test_external_edge_cannot_bypass_switch_guard(self):
        data = self.switch()
        dispatch = bytes(data[:20])
        data[:0x24] = b"\xcc" * 0x24
        data[:5] = b"\xe9" + struct.pack("<i", 0xa0 - 5)
        data[0x10:0x24] = dispatch
        data[0x14] = 0x2b  # Moved guard still branches to the default at 0x1040.
        # One successor enters the dispatch directly; the other reaches its guard.
        data.extend(b"\x0f\x84" + struct.pack("<i", 0x15 - 0xa6)
                    + b"\xe9" + struct.pack("<i", 0x10 - 0xab))
        self.assertIsNone(complete_original_extent(
            BytesImage(data), 0x1000, 0x10ab, self.decoder
        ))


@unittest.skipUnless(
    all(path.exists() for path in (ORIGINAL_EXE, RECOMP_EXE, RECOMP_PDB)),
    "requires the reference executable and a local build",
)
class OriginalExtentBinaryTests(unittest.TestCase):
    def test_mover_process_includes_all_37_action_indices(self):
        _, engine = load_engine()
        extent = complete_original_extent(
            engine.orig_bin, 0x0042eb00, 0x0042eeb0, Cs(CS_ARCH_X86, CS_MODE_32)
        )
        self.assertEqual(extent, 0x3a9)
        self.assertEqual(bytes(engine.orig_bin.read(0x0042ebb5, 3)), bytes.fromhex("83ff24"))
        self.assertEqual(bytes(engine.orig_bin.read(0x0042eea5, 4)), bytes((6, 6, 6, 5)))

    def test_main_display_switch_does_not_consume_following_data(self):
        _, engine = load_engine()
        blob = engine.orig_bin.read(0x00431cd0, 0x1dc)
        tables = [section for section in BoundedInstructGen(blob, 0x00431cd0).sections
                  if section.type == SectionType.ADDR_TAB]
        self.assertEqual([len(table.contents) for table in tables], [6])
        trailing = [pair for section in BoundedInstructGen(blob, 0x00431cd0).sections
                    if section.type == SectionType.DATA_TAB for pair in section.contents]
        self.assertEqual(trailing, list(enumerate(blob[-8:], 0x00431ea4)))

    def test_c2d_draw_lemming_includes_byte_table_after_guard_store(self):
        _, engine = load_engine()
        extent = complete_original_extent(
            engine.orig_bin, 0x0043c200, 0x0043c610, Cs(CS_ARCH_X86, CS_MODE_32)
        )
        # CMP EDX,0x23 proves 36 index bytes at 0x43c5e4.
        self.assertEqual(extent, 0x408)
        self.assertEqual(bytes(engine.orig_bin.read(0x0043c241, 8)),
                         bytes.fromhex("83 fa 23 66 89 44 24 12"))

    def test_c2d_draw_objects_includes_tail_after_guard_store(self):
        _, engine = load_engine()
        extent = complete_original_extent(
            engine.orig_bin, 0x0043f620, 0x0043fce0, Cs(CS_ARCH_X86, CS_MODE_32)
        )
        self.assertEqual(extent, 0x6c0)
        self.assertEqual(bytes(engine.orig_bin.read(0x0043f706, 8)),
                         bytes.fromhex("83 f8 03 66 89 5c 24 30"))
        self.assertEqual(bytes(engine.orig_bin.read(0x0043fcd0, 16)),
                         struct.pack("<4I", 0x43f71b, 0x43f7f3, 0x43f984, 0x43fa5f))

    def test_gun_draw_includes_return_and_direct_switch_table(self):
        _, engine = load_engine()
        extent = complete_original_extent(
            engine.orig_bin, 0x0044d2d0, 0x0044d830, Cs(CS_ARCH_X86, CS_MODE_32)
        )
        self.assertEqual(extent, 0x0044d82c - 0x0044d2d0)
        self.assertEqual(bytes(engine.orig_bin.read(0x0044d817, 5)),
                         bytes.fromhex("83 c4 14 c3 90"))

    def test_network_messages_includes_final_switch_data(self):
        _, engine = load_engine()
        extent = complete_original_extent(
            engine.orig_bin, 0x00454060, 0x00454520, Cs(CS_ARCH_X86, CS_MODE_32)
        )
        # The first switch uses EDX/AL; the two later switches use EAX/CL.
        # The final CMP EAX,0x4d proves 78 bytes at 0x004544d0.
        self.assertEqual(extent, 0x0045451e - 0x00454060)
        self.assertEqual(bytes(engine.orig_bin.read(0x0045451b, 3)), b"\x02" * 3)

    def test_laser_viewdata_includes_return_and_switch_tables(self):
        _, engine = load_engine()
        extent = complete_original_extent(
            engine.orig_bin, 0x00428f90, 0x00429320, Cs(CS_ARCH_X86, CS_MODE_32)
        )
        self.assertEqual(extent, 0x00429314 - 0x00428f90)
        self.assertEqual(bytes(engine.orig_bin.read(0x004292e9, 3)), bytes.fromhex("c20400"))
        self.assertEqual(bytes(engine.orig_bin.read(0x00429300, 20)),
                         bytes([0] + [4] * 16 + [1, 2, 3]))

    def test_bullet_getdata_includes_original_epilogue(self):
        _, engine = load_engine()
        match = next(m for m in engine.get_all() if m.orig_addr == 0x0041AB80)
        extended = extend_original_match(
            match, engine.orig_bin, Cs(CS_ARCH_X86, CS_MODE_32)
        )
        # Verified in the original image and Ghidra: final store, POP ESI,
        # ADD ESP,0x18, RET. None may be cut off at the rebuilt PDB size.
        self.assertEqual(extended.size(ImageId.ORIG), 0xE5)
        self.assertEqual(bytes(engine.orig_bin.read(0x0041AC5D, 8)),
                         bytes.fromhex("89 46 dc 5e 83 c4 18 c3"))
        result = engine.function_comparator.compare_function(match)
        self.assertEqual(result.diff.orig_inst[-1], ("0x41ac64", "ret "))
