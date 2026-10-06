"""Duplicate display names retain their original paired identities."""

import struct
import unittest

from reccmp.types import EntityType, ImageId

from lib.comparison.references import install_unique_lookups
from tests.comparison.fixtures import fixture, patch_body


class UniqueLookupTests(unittest.TestCase):
    def test_duplicate_globals_keep_matching_pairs_and_offsets(self):
        comparator, _ = fixture()
        with comparator.db.batch() as batch:
            for original, rebuilt in ((0x6000, 0x8000), (0x7000, 0x9000)):
                for side, address in (
                    (ImageId.ORIG, original),
                    (ImageId.RECOMP, rebuilt),
                ):
                    batch.set(side, address, type=EntityType.DATA, name="Value", size=8)
                batch.match(original, rebuilt)
        install_unique_lookups(comparator)
        lookup = comparator.orig_sanitize.lookup
        other = comparator.recomp_sanitize.lookup
        self.assertEqual(lookup(0x6000), other(0x8000))
        self.assertEqual(lookup(0x6004), other(0x8004))
        self.assertNotEqual(lookup(0x6000), other(0x9000))

    def test_duplicate_names_do_not_conflate_different_globals_in_raw_comparison(self):
        comparator, match = fixture("a100600000 c3", "a100900000 c3")
        patch_body(comparator.orig_bin, 0x1000, 1, struct.pack("<i", 0x4015 - 0x1005))
        with comparator.db.batch() as batch:
            for original, rebuilt in ((0x6000, 0x8000), (0x7000, 0x9000)):
                for side, address in (
                    (ImageId.ORIG, original),
                    (ImageId.RECOMP, rebuilt),
                ):
                    batch.set(side, address, type=EntityType.DATA, name="Value", size=4)
                batch.match(original, rebuilt)
        self.assertEqual(comparator.compare_function(match).match_ratio, 1)
        install_unique_lookups(comparator)
        result = comparator.compare_function(match)
        self.assertLess(result.match_ratio, 1)
        self.assertFalse(result.is_effective_match)
