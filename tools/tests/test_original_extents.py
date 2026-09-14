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

    def test_switch_requires_unsigned_guard_and_zero_extended_index(self):
        for offset, value in ((3, 0x7f), (2, 0xff), (5, 0x90), (8, 0x89), (16, 0x85)):
            with self.subTest(offset=offset):
                data = self.switch()
                data[offset] = value
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
