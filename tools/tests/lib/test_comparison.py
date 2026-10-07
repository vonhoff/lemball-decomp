"""One-hop thunk matching, binary fixtures and upstream result preservation."""

import copy
import struct
import unittest
from types import SimpleNamespace
from unittest.mock import Mock, patch

from reccmp.compare.db import EntityDb, ReccmpMatch
from reccmp.compare.functions import FunctionComparator
from reccmp.compare.report import ReccmpComparedEntity
from reccmp.formats.exceptions import (
    InvalidVirtualAddressError,
    InvalidVirtualReadError,
)
from reccmp.types import EntityType, ImageId

from lib.comparison import (
    ThunkParseAsm,
    additional_effective_matches,
    effective_addresses,
    resolve_jump_thunk,
)

THUNK = b"\xe9" + struct.pack("<i", 16)


def fixture(original="c3", rebuilt=None, thunk=THUNK, opcode="e8"):
    bodies = [
        bytes.fromhex(opcode + "fb2f0000 " + tail)
        for tail in (original, original if rebuilt is None else rebuilt)
    ]
    images = []
    for start, body in zip((0x1000, 0x2000), bodies, strict=True):
        images.append(
            SimpleNamespace(
                read=lambda read_address, size, image_start=start, image_body=body: (
                    image_body
                    if read_address == image_start
                    else thunk
                    if read_address == 0x4000
                    else b"\xc3"
                    if read_address == (0x4015 if image_start == 0x1000 else 0x5000)
                    else b""
                )[:size],
                imagebase=0,
                is_relocated_addr=lambda _address: False,
            )
        )
    lines = Mock()
    lines.find_line_of_recomp_address.return_value = None
    comparator = FunctionComparator(
        EntityDb(), lines, images[0], images[1], Mock(), Mock()
    )
    with comparator.db.batch() as batch:
        for side, address in ((ImageId.ORIG, 0x4015), (ImageId.RECOMP, 0x5000)):
            batch.set(side, address, type=EntityType.FUNCTION, name="Target", size=1)
        batch.match(0x4015, 0x5000)
    match = ReccmpMatch(
        0x1000,
        0x2000,
        {
            "name": "Caller",
            "type": EntityType.FUNCTION,
            "orig_size": len(bodies[0]),
            "recomp_size": len(bodies[1]),
        },
    )
    return comparator, match


def patch_body(image, start, offset, replacement):
    read = image.read

    def patched(address, size):
        data = read(address, size)
        if address == start:
            data = data[:offset] + replacement + data[offset + len(replacement) :]
        return data

    image.read = patched


def compare(comparator, match, **flags):
    raw = comparator.compare_function(match)
    comparison = ReccmpComparedEntity(
        match.orig_addr,
        "Caller",
        raw.match_ratio,
        EntityType.FUNCTION,
        match.recomp_addr,
        **flags,
    )
    engine = SimpleNamespace(
        function_comparator=comparator, get_functions=lambda: [match]
    )
    return additional_effective_matches(engine, {match.orig_addr: comparison})


class EffectiveAddressTests(unittest.TestCase):
    def test_effective_accepts_raw_upstream_and_additional_matches_directly(self):
        comparisons = {
            address: ReccmpComparedEntity(
                address, "Fixture", score, kind, rebuilt, **flags
            )
            for address, score, kind, rebuilt, flags in (
                (1, 1.0, EntityType.FUNCTION, 101, {}),
                (2, 0.8, EntityType.FUNCTION, 102, {"is_effective_match": True}),
                (3, 0.7, EntityType.FUNCTION, 103, {}),
                (4, 0.999999999, EntityType.FUNCTION, 104, {}),
                (5, 1.0, EntityType.FUNCTION, 105, {"is_stub": True}),
                (6, 1.0, EntityType.FUNCTION, None, {}),
                (7, 1.0, EntityType.DATA, 107, {}),
            )
        }
        self.assertEqual(effective_addresses(comparisons), {1, 2})
        self.assertEqual(effective_addresses(comparisons, {3, 5, 6, 7}), {1, 2, 3})


class AdditionalMatchTests(unittest.TestCase):
    def test_calls_and_tail_jumps_normalize_to_the_paired_function(self):
        for opcode in ("e8", "e9"):
            with self.subTest(opcode=opcode):
                self.assertEqual(compare(*fixture(opcode=opcode)), {0x1000})

    def test_reccmp_equivalence_is_used_after_thunk_normalization(self):
        self.assertEqual(compare(*fixture("3bc7 7200 c3", "3bf8 7700 c3")), {0x1000})

    def test_other_instruction_differences_remain_partial(self):
        self.assertEqual(compare(*fixture("b801000000 c3", "b802000000 c3")), set())

    def test_unknown_call_targets_cannot_be_proven_by_placeholder_order(self):
        comparator, match = fixture("31d2 3bc2 c3", "31d2 85c0 c3")
        patch_body(comparator.orig_bin, 0x1000, 0, bytes.fromhex("e8fb4f0000"))
        patch_body(comparator.recomp_bin, 0x2000, 0, bytes.fromhex("e8fb4f0000"))
        self.assertEqual(compare(comparator, match), set())

    def test_zero_comparison_differences_are_left_to_reccmp(self):
        comparator, match = fixture("31d2 3bc2 c3", "31d2 85c0 c3")
        patch_body(comparator.orig_bin, 0x1000, 0, bytes.fromhex("e810300000"))
        raw = comparator.compare_function(match)
        self.assertLess(raw.match_ratio, 1)
        self.assertFalse(raw.is_effective_match)
        self.assertEqual(compare(comparator, match), set())

    def test_upstream_parsers_are_preserved_on_comparison_failure(self):
        comparator, match = fixture("31d2 3bc2 c3", "31d2 85c0 c3")
        parsers = comparator.orig_sanitize, comparator.recomp_sanitize
        with patch.object(
            ThunkParseAsm, "sanitize", side_effect=RuntimeError("decode")
        ):
            with self.assertRaisesRegex(RuntimeError, "decode"):
                compare(comparator, match)
        self.assertEqual(
            (comparator.orig_sanitize, comparator.recomp_sanitize), parsers
        )

    def test_missing_or_non_e9_thunks_do_not_add_matches(self):
        for thunk in (b"", bytes.fromhex("e91000"), bytes.fromhex("e810000000")):
            with self.subTest(thunk=thunk):
                self.assertEqual(compare(*fixture(thunk=thunk)), set())

    def test_exact_upstream_and_stub_results_need_no_binary_recheck(self):
        for accuracy, flags in (
            (1, {}),
            (0.8, {"is_effective_match": True}),
            (1, {"is_stub": True}),
        ):
            with self.subTest(accuracy=accuracy, flags=flags):
                match = SimpleNamespace(orig_addr=0x1000)
                comparison = ReccmpComparedEntity(
                    0x1000, "Caller", accuracy, EntityType.FUNCTION, 0x2000, **flags
                )
                engine = SimpleNamespace(get_functions=lambda entry=match: [entry])
                self.assertEqual(
                    additional_effective_matches(engine, {0x1000: comparison}), set()
                )

    def test_missing_comparisons_need_no_binary_recheck(self):
        engine = SimpleNamespace(
            get_functions=lambda: [SimpleNamespace(orig_addr=0x1000)]
        )
        self.assertEqual(additional_effective_matches(engine, {}), set())

    def test_raw_results_and_upstream_parsers_are_preserved(self):
        comparator, match = fixture()
        raw = comparator.compare_function(match)
        comparison = ReccmpComparedEntity(
            0x1000,
            "Caller",
            raw.match_ratio,
            EntityType.FUNCTION,
            0x2000,
            rdiff=raw.diff,
        )
        saved = copy.deepcopy(comparison)
        parsers = comparator.orig_sanitize, comparator.recomp_sanitize
        engine = SimpleNamespace(
            function_comparator=comparator, get_functions=lambda: [match]
        )
        self.assertEqual(
            additional_effective_matches(engine, {0x1000: comparison}), {0x1000}
        )
        self.assertEqual(comparison, saved)
        self.assertEqual(
            (comparator.orig_sanitize, comparator.recomp_sanitize), parsers
        )

    def test_address_taking_preserves_the_thunk_address(self):
        comparator, _ = fixture()
        parser = ThunkParseAsm(comparator.orig_bin, {0x4015}, comparator.orig_sanitize)
        assembly = parser.parse_asm(bytes.fromhex("e8fb2f0000 c3"), 0x1000)
        self.assertIn("Target", assembly[0][1])
        assembly = parser.parse_asm(bytes.fromhex("b800400000 c3"), 0x1000)
        self.assertEqual(assembly[0][1], "mov eax, 0x4000")


class JumpThunkTests(unittest.TestCase):
    def test_forward_backward_and_wrapped_targets_read_exactly_one_hop(self):
        for address, target in (
            (0x1000, 0x2000),
            (0x2000, 0x1000),
            (0xFFFFFFF0, 0x10),
            (0xFFFFFFFB, 0),
        ):
            with self.subTest(address=address, target=target):
                displacement = (
                    target - address - 5 + 0x80000000
                ) % 0x100000000 - 0x80000000
                thunk = b"\xe9" + struct.pack("<i", displacement)
                image = SimpleNamespace(read=Mock(return_value=thunk))
                self.assertEqual(
                    resolve_jump_thunk(image, {target: "paired"}, address), target
                )
                image.read.assert_called_once_with(address, 5)

    def test_paired_entries_are_not_read(self):
        image = SimpleNamespace(read=Mock())
        self.assertIsNone(resolve_jump_thunk(image, {0x2000: "paired"}, 0x2000))
        image.read.assert_not_called()

    def test_incomplete_or_non_e9_instructions_do_not_read_a_destination(self):
        for data in (
            b"",
            b"\xe9",
            b"\xe9\0\0\0",
            b"\xe8\0\0\0\0",
            b"\xeb\0\x90\x90\x90",
        ):
            with self.subTest(data=data):
                image = SimpleNamespace(read=Mock(return_value=data))
                self.assertIsNone(resolve_jump_thunk(image, {0x1005: "paired"}, 0x1000))
                image.read.assert_called_once_with(0x1000, 5)

    def test_unpaired_destination_is_not_read_or_followed(self):
        image = SimpleNamespace(read=Mock(return_value=b"\xe9\0\0\0\0"))
        self.assertIsNone(resolve_jump_thunk(image, {0x2000: "paired"}, 0x1000))
        image.read.assert_called_once_with(0x1000, 5)

    def test_unreadable_thunk_is_unresolved(self):
        for error in (
            InvalidVirtualAddressError(0x1000),
            InvalidVirtualReadError(0x1000),
        ):
            with self.subTest(error=error):
                image = SimpleNamespace(read=Mock(side_effect=error))
                self.assertIsNone(resolve_jump_thunk(image, {0x2000: "paired"}, 0x1000))
