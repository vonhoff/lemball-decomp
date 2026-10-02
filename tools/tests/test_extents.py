"""Byte ownership, padding boundaries, and full upstream assembly coverage."""

import unittest
from unittest.mock import Mock, patch

from reccmp.compare.asm import instgen
from reccmp.compare.db import EntityDb
from reccmp.compare.functions import FunctionComparator
from reccmp.compare.lines import LinesDb
from reccmp.cvdump.types import CvdumpTypesParser
from reccmp.types import EntityType, ImageId

from lib.extents import (
    FullFunctionComparator,
    alignment_extent,
    complete_instruction_stream,
    decoded_extent,
    prepare_function_extents,
    validate_extents,
)


def comparison_fixture(original, rebuilt, original_size=None):
    database = EntityDb()
    with database.batch() as batch:
        batch.set(
            ImageId.ORIG,
            0x401000,
            type=EntityType.FUNCTION,
            name="Fixture",
            size=original_size,
        )
        batch.set(
            ImageId.RECOMP,
            0x501000,
            type=EntityType.FUNCTION,
            name="Fixture",
            size=len(rebuilt),
        )
        batch.match(0x401000, 0x501000)
    original_image, rebuilt_image = Mock(), Mock()
    original_image.read.side_effect = lambda address, size: original[:size]
    rebuilt_image.read.side_effect = lambda address, size: rebuilt[:size]
    args = (
        database,
        LinesDb(),
        original_image,
        rebuilt_image,
        Mock(),
        CvdumpTypesParser(),
    )
    return database, args


class ExtentTests(unittest.TestCase):
    def test_original_suffix_is_compared_when_rebuilt_function_is_shorter(self):
        original = bytes.fromhex("b801000000c3")
        rebuilt = original[:5]
        database, args = comparison_fixture(original, rebuilt)
        match = database.get(ImageId.ORIG, 0x401000)
        self.assertEqual(
            FunctionComparator(*args).compare_function(match).match_ratio, 1
        )
        with database.batch() as batch:
            batch.set(ImageId.ORIG, 0x401000, size=len(original))
        match = database.get(ImageId.ORIG, 0x401000)
        self.assertAlmostEqual(
            FullFunctionComparator(*args).compare_function(match).match_ratio, 2 / 3
        )

    def test_int3_does_not_hide_following_function_body(self):
        original = bytes.fromhex("90ccb801000000c3")
        rebuilt = bytes.fromhex("90ccb802000000c3")
        database, args = comparison_fixture(original, rebuilt, len(original))
        match = database.get(ImageId.ORIG, 0x401000)
        self.assertEqual(
            FunctionComparator(*args).compare_function(match).match_ratio, 1
        )
        previous = instgen.stop_at_int3
        result = FullFunctionComparator(*args).compare_function(match)
        self.assertEqual(result.match_ratio, 0.75)
        self.assertIs(instgen.stop_at_int3, previous)
        self.assertEqual(decoded_extent(original, 0x401000), len(original))
        with self.assertRaisesRegex(RuntimeError, "fixture"):
            with complete_instruction_stream():
                raise RuntimeError("fixture")
        self.assertIs(instgen.stop_at_int3, previous)

    def test_padding_must_follow_a_terminal_and_be_untargeted(self):
        for blob, expected in (
            ("c3908d642400cccc", 1),
            ("ffe090cccc", 2),
            ("c390b801000000c3", 8),
            ("eb01c390", 4),  # Direct jump targets the NOP suffix.
            ("e802000000c39090", 8),  # Direct call targets the NOP suffix.
            ("b80100000090", 6),  # No terminating instruction.
            ("c3b8", 2),  # Undecodable suffix cannot be called alignment.
            ("90cc90c3", 4),  # Intentional breakpoint followed by code.
        ):
            with self.subTest(blob=blob):
                self.assertEqual(
                    alignment_extent(bytes.fromhex(blob), 0x401000), expected
                )

    def test_rebuilt_padding_is_removed_before_upstream_comparison(self):
        database, args = comparison_fixture(b"\xc3", b"\xc3\x90\x90", 1)
        engine = Mock()
        engine.function_comparator = FunctionComparator(*args)
        engine.get_all.side_effect = database.get_all
        engine.orig_bin, engine.recomp_bin = args[2:4]
        prepare_function_extents(engine)
        match = database.get(ImageId.ORIG, 0x401000)
        self.assertEqual(match.size(ImageId.RECOMP), 1)
        self.assertEqual(
            engine.function_comparator.compare_function(match).match_ratio, 1
        )

    def test_bad_original_extents_fail_before_comparison(self):
        for blob, message in (
            (b"\xc3\x90", "trailing alignment"),
            (b"\xc3\xb8", "undecoded bytes"),
        ):
            with self.subTest(blob=blob):
                database, args = comparison_fixture(blob, b"\xc3", len(blob))
                engine = Mock()
                engine.function_comparator = FunctionComparator(*args)
                engine.get_all.side_effect = database.get_all
                engine.orig_bin, engine.recomp_bin = args[2:4]
                with self.assertRaisesRegex(ValueError, message):
                    prepare_function_extents(engine)

    def test_adjacent_labels_have_disjoint_ownership(self):
        validate_extents({0x481A42: 16, 0x4819EA: 88})
        with self.assertRaisesRegex(ValueError, "Overlapping original"):
            validate_extents({0x4819EA: 104, 0x481A42: 16})

    def test_switch_table_is_retained(self):
        # CMP EAX,1; JA default; JMP [EAX*4+table]; RET; two target addresses.
        blob = bytes.fromhex("83f8017707ff248510104000c39090900c1040000c104000")
        self.assertEqual(alignment_extent(blob, 0x401000), len(blob))
        self.assertEqual(decoded_extent(blob, 0x401000), len(blob))

    def test_decoder_hook_restored_when_upstream_comparison_fails(self):
        database, args = comparison_fixture(b"\xc3", b"\xc3", 1)
        previous = instgen.stop_at_int3
        with patch.object(
            FunctionComparator, "compare_function", side_effect=ValueError("fixture")
        ):
            with self.assertRaisesRegex(ValueError, "fixture"):
                FullFunctionComparator(*args).compare_function(
                    database.get(ImageId.ORIG, 0x401000)
                )
        self.assertIs(instgen.stop_at_int3, previous)
