"""An original function must not be truncated to a shorter rebuilt length."""

import unittest

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


@unittest.skipUnless(
    all(path.exists() for path in (ORIGINAL_EXE, RECOMP_EXE, RECOMP_PDB)),
    "requires the reference executable and a local build",
)
class OriginalExtentBinaryTests(unittest.TestCase):
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
