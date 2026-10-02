"""Byte ownership, padding boundaries, and full upstream assembly coverage."""

import unittest
from pathlib import Path
from tempfile import TemporaryDirectory
from types import SimpleNamespace
from unittest.mock import Mock, patch

from reccmp.compare.asm import instgen
from reccmp.compare.db import EntityDb, ReccmpEntity
from reccmp.compare.functions import FunctionComparator
from reccmp.compare.lines import LinesDb
from reccmp.cvdump.types import CvdumpTypesParser
from reccmp.types import EntityType, ImageId

from lib.extents import (
    FullFunctionComparator,
    function_extents,
    load_target_sizes,
    prepare_function_extents,
    target_size,
)


def comparison_fixture(original, rebuilt, original_size=None):
    database = EntityDb()
    with database.batch() as batch:
        for image, fixture_address, extent_size in ((ImageId.ORIG, 0x401000, original_size),
                                                   (ImageId.RECOMP, 0x501000, len(rebuilt))):
            batch.set(image, fixture_address, type=EntityType.FUNCTION, name="Fixture", size=extent_size)
        batch.match(0x401000, 0x501000)
    original_image = SimpleNamespace(read=lambda address, size: original[:size],
                                     get_relative_addr=lambda address: (1, 0))
    rebuilt_image = SimpleNamespace(read=lambda address, size: rebuilt[:size])
    engine = SimpleNamespace(
        function_comparator=FunctionComparator(database, LinesDb(), original_image,
                                               rebuilt_image, Mock(), CvdumpTypesParser()),
        get_all=database.get_all, orig_bin=original_image, recomp_bin=rebuilt_image,
    )
    return engine, database.get(ImageId.ORIG, 0x401000)


class ExtentTests(unittest.TestCase):
    def test_original_comparison_size_is_required(self):
        entity = ReccmpEntity(0x401000, 0x501000, {"orig_size": 25, "recomp_size": 9})
        self.assertEqual(target_size(entity), 25)
        entity = ReccmpEntity(0x401000, 0x501000, {"recomp_size": 9})
        with self.assertRaisesRegex(
            ValueError, "Missing original function extent at 0x00401000"
        ):
            target_size(entity)

    def test_target_extent_csv_validation(self):
        with TemporaryDirectory() as directory:
            path = Path(directory) / "sizes.csv"
            path.write_text(
                "# Original evidence\naddress,size,evidence\n"
                "0x401019,10,x86\n0x401000,25,x86\n", encoding="utf-8"
            )
            self.assertEqual(load_target_sizes(path), {0x401000: 25, 0x401019: 10})
            for rows, message in (
                ("0x401000,0\n", "Invalid or duplicate target extent"),
                ("0x401000,25\n0x401000,30\n", "Invalid or duplicate target extent"),
                ("0x401010,10\n0x401000,25\n", "Overlapping original function extent"),
            ):
                path.write_text("address,size\n" + rows, encoding="utf-8")
                with self.subTest(rows=rows), self.assertRaisesRegex(ValueError, message):
                    load_target_sizes(path)

    def test_original_suffix_is_compared_when_rebuilt_function_is_shorter(self):
        original = bytes.fromhex("b801000000c3")
        rebuilt = original[:5]
        engine, match = comparison_fixture(original, rebuilt)
        self.assertEqual(engine.function_comparator.compare_function(match).match_ratio, 1)
        with engine.function_comparator.db.batch() as batch:
            batch.set(ImageId.ORIG, 0x401000, size=len(original))
        prepare_function_extents(engine)
        match = engine.function_comparator.db.get(ImageId.ORIG, 0x401000)
        self.assertAlmostEqual(
            engine.function_comparator.compare_function(match).match_ratio, 2 / 3
        )

    def test_int3_does_not_hide_following_function_body(self):
        original = bytes.fromhex("90ccb801000000c3")
        rebuilt = bytes.fromhex("90ccb802000000c3")
        engine, match = comparison_fixture(original, rebuilt, len(original))
        self.assertEqual(engine.function_comparator.compare_function(match).match_ratio, 1)
        previous = instgen.stop_at_int3
        prepare_function_extents(engine)
        result = engine.function_comparator.compare_function(match)
        self.assertEqual(result.match_ratio, 0.75)
        self.assertIs(instgen.stop_at_int3, previous)
        self.assertEqual(function_extents(original, 0x401000), (len(original), len(original)))

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
                    function_extents(bytes.fromhex(blob), 0x401000)[1], expected
                )

    def test_rebuilt_padding_is_removed_before_upstream_comparison(self):
        engine, _ = comparison_fixture(b"\xc3", b"\xc3\x90\x90", 1)
        prepare_function_extents(engine)
        self.assertIsInstance(engine.function_comparator, FullFunctionComparator)
        match = engine.function_comparator.db.get(ImageId.ORIG, 0x401000)
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
                engine, _ = comparison_fixture(blob, b"\xc3", len(blob))
                with self.assertRaisesRegex(ValueError, message):
                    prepare_function_extents(engine)

    def test_switch_table_is_retained(self):
        # CMP EAX,1; JA default; JMP [EAX*4+table]; RET; two target addresses.
        blob = bytes.fromhex("83f8017707ff248510104000c39090900c1040000c104000")
        self.assertEqual(function_extents(blob, 0x401000), (len(blob), len(blob)))

    def test_decoder_hook_restored_when_upstream_comparison_fails(self):
        engine, match = comparison_fixture(b"\xc3", b"\xc3", 1)
        prepare_function_extents(engine)
        previous = instgen.stop_at_int3
        with (
            patch.object(FunctionComparator, "compare_function", side_effect=ValueError("fixture")),
            self.assertRaisesRegex(ValueError, "fixture"),
        ):
            engine.function_comparator.compare_function(match)
        self.assertIs(instgen.stop_at_int3, previous)
