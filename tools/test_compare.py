"""Regression checks for bounded one-past data addresses in comparisons."""

import struct
import unittest

from reccmp.compare.asm.parse import ParseAsm
from reccmp.compare.asm.replacement import create_name_lookup
from reccmp.compare.db import EntityDb
from reccmp.types import EntityType, ImageId

from lib.compare import Parser


class ArrayEnds(unittest.TestCase):
    def setUp(self):
        self.db = EntityDb()
        self.bases = {ImageId.ORIG: 0x1000, ImageId.RECOMP: 0x2000}

    def add_array(self, original_size=None, rebuilt_size=4, limit=128, matched=True):
        with self.db.batch() as batch:
            batch.set(
                ImageId.ORIG,
                self.bases[ImageId.ORIG],
                type=EntityType.DATA,
                name="array",
                size=original_size,
                max_size=limit,
            )
            batch.set(
                ImageId.RECOMP,
                self.bases[ImageId.RECOMP],
                type=EntityType.DATA,
                name="array",
                size=rebuilt_size,
            )
            if matched:
                batch.match(self.bases[ImageId.ORIG], self.bases[ImageId.RECOMP])

    def comparison(self, image, offset=4, relocated=True):
        address = self.bases[image] + offset
        lookup = create_name_lookup(self.db, image, lambda _: None, lambda _, n: f"+{n}")
        parser = Parser(
            ParseAsm(lambda value: relocated and value == address, lookup),
            self.db,
            image,
        )
        code = b"\x81\xff" + struct.pack("<I", address)
        return parser.parse_asm(code, 0x4000)[0][1]

    def assert_end(self):
        for image in self.bases:
            with self.subTest(image=image):
                self.assertEqual(self.comparison(image), "cmp edi, array+4 (OFFSET)")

    def test_end_before_next_original_symbol(self):
        self.add_array()
        self.assert_end()

    def test_explicit_matching_extents(self):
        self.add_array(original_size=4)
        self.assert_end()

    def test_adjacent_global_case_remains_supported(self):
        self.add_array(limit=4)
        with self.db.batch() as batch:
            batch.set(ImageId.ORIG, 0x1004, type=EntityType.DATA, name="next_original")
            batch.set(ImageId.RECOMP, 0x2004, type=EntityType.DATA, name="next_rebuilt")
        self.assert_end()

    def test_larger_gap_does_not_override_exact_symbol(self):
        self.add_array()
        with self.db.batch() as batch:
            batch.set(ImageId.ORIG, 0x1040, type=EntityType.DATA, name="other")
            batch.set(ImageId.RECOMP, 0x2004, type=EntityType.DATA, name="other")
            batch.match(0x1040, 0x2004)
        self.assertEqual(self.comparison(ImageId.RECOMP), "cmp edi, other (DATA)")
        self.assertNotEqual(self.comparison(ImageId.ORIG), self.comparison(ImageId.RECOMP))

    def test_conflicting_original_extent_is_not_an_end_match(self):
        self.add_array(original_size=8)
        self.assertNotEqual(self.comparison(ImageId.ORIG), self.comparison(ImageId.RECOMP))
        self.assertEqual(self.comparison(ImageId.RECOMP), "cmp edi, 0x2004")

    def test_conflicting_extent_also_rejects_adjacent_case(self):
        self.add_array(original_size=8, limit=4)
        self.assertEqual(self.comparison(ImageId.RECOMP), "cmp edi, 0x2004")

    def test_end_cannot_exceed_original_upper_bound(self):
        self.add_array(limit=2)
        self.assertEqual(self.comparison(ImageId.ORIG), "cmp edi, 0x1004")

    def test_missing_original_upper_bound_is_not_inferred(self):
        self.add_array(limit=None)
        self.assertEqual(self.comparison(ImageId.ORIG), "cmp edi, 0x1004")

    def test_unknown_rebuilt_extent_is_not_inferred(self):
        self.add_array(rebuilt_size=None)
        self.assertEqual(self.comparison(ImageId.ORIG), "cmp edi, 0x1004")

    def test_unmatched_data_is_not_an_end_match(self):
        self.add_array(matched=False)
        self.assertEqual(self.comparison(ImageId.RECOMP), "cmp edi, 0x2004")

    def test_unrelocated_value_inside_gap_is_not_an_end_match(self):
        self.add_array()
        self.assertEqual(self.comparison(ImageId.ORIG, relocated=False), "cmp edi, 0x1004")

    def test_offset_beyond_end_is_not_normalized(self):
        self.add_array()
        self.assertEqual(self.comparison(ImageId.ORIG, offset=8), "cmp edi, 0x1008")

    def test_array_base_and_interior_still_use_upstream_lookup(self):
        self.add_array(rebuilt_size=8)
        self.assertEqual(self.comparison(ImageId.ORIG, offset=0), "cmp edi, array (DATA)")
        self.assertEqual(self.comparison(ImageId.ORIG, offset=4), "cmp edi, array+4 (OFFSET)")


if __name__ == "__main__":
    unittest.main()
