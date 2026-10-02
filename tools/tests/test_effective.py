"""Jump-thunk identity and whole-function comparison boundaries."""

import copy
import struct
import unittest
from types import SimpleNamespace
from unittest.mock import Mock, patch

from reccmp.compare.asm.parse import ParseAsm
from reccmp.compare.db import EntityDb, ReccmpMatch
from reccmp.compare.functions import FunctionComparator
from reccmp.compare.report import ReccmpComparedEntity
from reccmp.formats.exceptions import InvalidVirtualAddressError
from reccmp.types import EntityType, ImageId

from lib.effective import ThunkParseAsm, additional_effective_matches


THUNK = b"\xe9" + struct.pack("<i", 16)


def fixture(original="c3", rebuilt=None, thunk=THUNK, opcode="e8"):
    bodies = [bytes.fromhex(opcode + "fb2f0000 " + tail)
              for tail in (original, original if rebuilt is None else rebuilt)]
    images = []
    for start, body in zip((0x1000, 0x2000), bodies):
        images.append(SimpleNamespace(
            read=lambda address, size, start=start, body=body:
                (body if address == start else thunk if address == 0x4000 else b"")[:size],
            imagebase=0, is_relocated_addr=lambda _address: False,
        ))
    lines = Mock()
    lines.find_line_of_recomp_address.return_value = None
    comparator = FunctionComparator(EntityDb(), lines, images[0], images[1], Mock(), Mock())
    with comparator.db.batch() as batch:
        for side, address in ((ImageId.ORIG, 0x4015), (ImageId.RECOMP, 0x5000)):
            batch.set(side, address, type=EntityType.FUNCTION, name="Target", size=1)
        batch.match(0x4015, 0x5000)
    match = ReccmpMatch(0x1000, 0x2000, {
        "name": "Caller", "type": EntityType.FUNCTION,
        "orig_size": len(bodies[0]), "recomp_size": len(bodies[1]),
    })
    return comparator, match


def compare(comparator, match, **flags):
    raw = comparator.compare_function(match)
    comparison = ReccmpComparedEntity(
        match.orig_addr, "Caller", raw.match_ratio, EntityType.FUNCTION, match.recomp_addr,
        **flags,
    )
    engine = SimpleNamespace(function_comparator=comparator, get_functions=lambda: [match])
    return additional_effective_matches(engine, {match.orig_addr: comparison})


class ThunkTests(unittest.TestCase):
    def test_direct_calls_and_tail_jumps_reach_the_paired_function(self):
        for opcode in ("e8", "e9"):
            with self.subTest(opcode=opcode):
                comparator, match = fixture(opcode=opcode)
                raw = comparator.compare_function(match)
                self.assertLess(raw.match_ratio, 1)
                self.assertFalse(raw.is_effective_match)
                self.assertEqual(compare(comparator, match), {0x1000: ("verified jump thunk target",)})

    def test_thunk_requires_a_complete_e9_to_a_paired_function(self):
        for thunk in (THUNK[:4], b"\xe8" + THUNK[1:], b"\xe9" + struct.pack("<i", 17)):
            with self.subTest(thunk=thunk):
                self.assertEqual(compare(*fixture(thunk=thunk)), {})
        comparator, match = fixture()
        read = comparator.orig_bin.read
        def invalid(address, size):
            if address == 0x4000:
                raise InvalidVirtualAddressError(address)
            return read(address, size)
        comparator.orig_bin.read = invalid
        self.assertEqual(compare(comparator, match), {})

    def test_function_pointers_and_indirect_calls_keep_their_identity(self):
        for call_first in (True, False):
            parser = ThunkParseAsm(
                SimpleNamespace(read=lambda _address, _size: THUNK), {0x4015},
                ParseAsm(addr_test=lambda _address: True, name_lookup=lambda addr, exact=False, indirect=False: {
                    0x4000: "Wrapper", 0x4015: "Target",
                }.get(addr)),
            )
            call = (0x1000, 5, "call", "0x4000")
            pointer = (0x1005, 5, "mov", "eax, 0x4000")
            results = dict(parser.sanitize(inst) for inst in
                           ((call, pointer) if call_first else (pointer, call)))
            self.assertEqual(results, {"call": "Target", "mov": "eax, Wrapper"})
            self.assertEqual(parser.sanitize((0x100a, 6, "call", "dword ptr [0x4000]")),
                             ("call", "dword ptr [Wrapper]"))
            parser.targets.add(0x4000)
            self.assertEqual(parser.sanitize(call), ("call", "Wrapper"))

    def test_remaining_registers_flags_memory_branches_and_cleanup_must_agree(self):
        for original, rebuilt in (
            ("8bc1 c3", "8bc2 c3"),
            ("31c0 c3", "29c0 c3"),
            ("8b06 890f c3", "890f 8b06 c3"),
            ("85c0 7402 31c0 c3", "85c0 7400 31c0 c3"),
            ("85c0 7400 c3", "85c0 7500 c3"),
            ("e300 c3", "e301 c3"),
            ("c20400", "c20800"),
            ("50 c3", "fff0 c3"),  # Same text, different instruction boundaries.
            ("6a01 6a02 6a03 c3", "6a04 6a02 6a03 c3"),
        ):
            with self.subTest(original=original, rebuilt=rebuilt):
                self.assertEqual(compare(*fixture(original, rebuilt)), {})

    def test_unresolved_operands_cannot_match_by_placeholder_number(self):
        for original, rebuilt in (
            ("e800010000", "e800020000"),
            ("ff1500600000", "ff1500700000"),
            ("a100600000", "a100700000"),
            ("a300600000", "a300700000"),
            ("8b048500600000", "8b048500700000"),
            ("b800600000", "b800700000"),
            ("6800600000", "6800700000"),
        ):
            with self.subTest(original=original):
                comparator, match = fixture(original + " c3", rebuilt + " c3")
                comparator.orig_bin.is_relocated_addr = lambda value: value == 0x6000
                comparator.recomp_bin.is_relocated_addr = lambda value: value == 0x7000
                self.assertEqual(compare(comparator, match), {})

    def test_complete_decoding_and_internal_branch_boundaries_are_required(self):
        for tail in ("0f", "cc 0f", "7401 8b00 c3", "eb00", "e301 c3"):
            with self.subTest(tail=tail):
                self.assertEqual(compare(*fixture(tail)), {})
        self.assertTrue(compare(*fixture("90 ebfd")))

    def test_parser_state_is_per_function(self):
        comparator, match = fixture()
        parser = ThunkParseAsm(comparator.orig_bin, {0x4015}, comparator.orig_sanitize)
        size = match.size(ImageId.ORIG)
        assert size is not None
        parser.parse_asm(comparator.orig_bin.read(0x1000, size), 0x1000)
        self.assertTrue(parser.used_thunk)
        self.assertIsNotNone(parser.signature)
        parser.parse_asm(b"\x0f", 0x1100)
        self.assertFalse(parser.used_thunk)
        self.assertIsNone(parser.signature)

    def test_actual_assertion_arguments_are_compared(self):
        for changed in (False, True):
            comparator, match = fixture("6a01 6a02 6a03 e8f04f0000 c3",
                                        "6a01 6a02 " + ("6a04" if changed else "6a03") + " e8f04f0000 c3")
            with comparator.db.batch() as batch:
                for side, address in ((ImageId.ORIG, 0x6000), (ImageId.RECOMP, 0x7000)):
                    batch.set(side, address, type=EntityType.FUNCTION, name="__assert", size=1)
                batch.match(0x6000, 0x7000)
            with patch("reccmp.compare.functions.has_asserts", return_value=True):
                self.assertEqual(bool(compare(comparator, match)), not changed)

    def test_raw_comparison_and_upstream_parsers_are_preserved(self):
        comparator, match = fixture()
        raw = comparator.compare_function(match)
        comparison = ReccmpComparedEntity(0x1000, "Caller", raw.match_ratio,
                                         EntityType.FUNCTION, 0x2000, rdiff=raw.diff)
        comparisons = {0x1000: comparison}
        saved = copy.deepcopy(comparisons)
        parsers = (comparator.orig_sanitize, comparator.recomp_sanitize)
        engine = SimpleNamespace(function_comparator=comparator, get_functions=lambda: [match])
        self.assertTrue(additional_effective_matches(engine, comparisons))
        self.assertEqual(comparisons, saved)
        self.assertEqual((comparator.orig_sanitize, comparator.recomp_sanitize), parsers)
        for flags in ({"is_stub": True}, {"is_effective_match": True}):
            self.assertEqual(compare(comparator, match, **flags), {})
        comparison.accuracy = 1
        self.assertEqual(additional_effective_matches(engine, comparisons), {})
