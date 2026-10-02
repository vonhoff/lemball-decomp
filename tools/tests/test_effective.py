"""Extra equivalences preserve raw comparisons and reject observable changes."""

import copy
import struct
import unittest
from dataclasses import fields
from types import SimpleNamespace
from unittest.mock import Mock

from reccmp.compare.asm.parse import ParseAsm
from reccmp.compare.db import EntityDb, ReccmpMatch
from reccmp.compare.report import ReccmpComparedEntity, ReccmpStatusReport
from reccmp.types import EntityType

from lib import effective
from lib.effective import EffectiveFunctionComparator, additional_effective_matches
from lib.extents import FullFunctionComparator


def fixture(original, rebuilt):
    original, rebuilt = bytes.fromhex(original), bytes.fromhex(rebuilt)
    lines = Mock()
    lines.find_line_of_recomp_address.return_value = None
    comparator = FullFunctionComparator(
        db=EntityDb(), lines_db=lines,
        orig_bin=SimpleNamespace(
            read=lambda _address, size: original[:size], imagebase=0,
            is_relocated_addr=lambda _address: False,
            get_code_regions=lambda: [], relocations=set(),
        ),
        recomp_bin=SimpleNamespace(
            read=lambda _address, size: rebuilt[:size], imagebase=0,
            is_relocated_addr=lambda _address: False,
            get_code_regions=lambda: [], relocations=set(),
        ),
        report=lambda *_args: None, types=Mock(),
    )
    match = ReccmpMatch(0x1000, 0x2000, {
        "name": "Fixture", "type": EntityType.FUNCTION,
        "orig_size": len(original), "recomp_size": len(rebuilt),
    })
    return comparator, match


def compare_bytes(original, rebuilt):
    upstream, match = fixture(original, rebuilt)
    comparator = EffectiveFunctionComparator(
        **{field.name: getattr(upstream, field.name) for field in fields(upstream)}
    )
    raw = upstream.compare_function(match)
    result = comparator.compare_function(match)
    return raw, result, tuple(sorted(comparator.reasons))


def compare_assembly(original, rebuilt):
    upstream, match = fixture("c3", "c3")
    comparator = EffectiveFunctionComparator(
        **{field.name: getattr(upstream, field.name) for field in fields(upstream)}
    )
    comparator.orig_sanitize.parse_asm = Mock(return_value=list(enumerate(original)))
    comparator.recomp_sanitize.parse_asm = Mock(return_value=list(enumerate(rebuilt)))
    return comparator.compare_function(match)


class ThunkTests(unittest.TestCase):
    def test_incremental_table_requires_padding_and_forward_code_targets(self):
        for displacement, padding, expected in (
            (16, b"\xcc" * 16, {0x1000: 0x1015}),
            (-5, b"\xcc" * 16, {}),
            (16, b"\x90" * 16, {}),
            (0x10000, b"\xcc" * 16, {}),
        ):
            with self.subTest(displacement=displacement, padding=padding):
                region = SimpleNamespace(
                    addr=0x1000, data=b"\xe9" + struct.pack("<i", displacement) + padding + b"\xc3",
                )
                image = SimpleNamespace(get_code_regions=lambda value=region: [value])
                self.assertEqual(effective.incremental_thunks(image), expected)

    def test_direct_indirect_and_function_pointer_thunks_use_verified_targets(self):
        parser = effective.EffectiveParseAsm(
            image=SimpleNamespace(read=lambda _address, _size: b""),
            function_targets={0x2000},
            thunk_targets={0x1000: 0x2000}, indirect_targets={0x3000: 0x2000},
            name_lookup=lambda address, **_kwargs: {0x2000: "Target (FUNCTION)"}.get(address),
        )
        self.assertEqual(parser.lookup(0x1000, exact=True), "Target (FUNCTION)")
        self.assertEqual(parser.lookup(0x3000, indirect=True), "->Target (FUNCTION)")
        self.assertEqual(parser.sanitize((0x4000, 5, "call", "0x1000")), ("call", "Target (FUNCTION)"))
        self.assertEqual(parser.sanitize((0x4000, 5, "mov", "eax, 0x1000")), ("mov", "eax, 0x1000"))
        self.assertIsNone(parser.lookup(0x1004))
        self.assertIsNone(parser.lookup(0x3004, indirect=True))
        self.assertEqual(parser.reasons, {"verified linker thunk target"})

    def test_direct_jump_thunk_requires_e9_and_a_known_function(self):
        for raw, names, expected in (
            (b"\xe9" + struct.pack("<i", 0xffb), {0x2000: "Target (FUNCTION)"}, "Target (FUNCTION)"),
            (b"\xe8" + struct.pack("<i", 0xffb), {0x2000: "Target (FUNCTION)"}, "<OFFSET1>"),
            (b"\xe9" + struct.pack("<i", 0xffb), {}, "<OFFSET1>"),
            (b"\xe9" + struct.pack("<i", 0xffb), {0x2000: "Data (DATA)"}, "<OFFSET1>"),
            (b"\xe9", {0x2000: "Target (FUNCTION)"}, "<OFFSET1>"),
        ):
            with self.subTest(raw=raw, names=names):
                parser = effective.EffectiveParseAsm(
                    image=SimpleNamespace(read=lambda _address, _size, value=raw: value),
                    function_targets={0x2000},
                    thunk_targets={}, indirect_targets={},
                    name_lookup=lambda address, lookup=names, **_kwargs: lookup.get(address),
                )
                self.assertEqual(parser.sanitize((0x4000, 5, "call", "0x1000")), ("call", expected))


class ZeroCompareTests(unittest.TestCase):
    def test_zeroed_callee_saved_register_matches_test_with_raw_diff_intact(self):
        raw, result, reasons = compare_bytes(
            "31ed e800010000 8b5c2404 3bdd 8906 7501 c3 c3",
            "31ed e800010000 8b5c2404 85db 8906 7501 c3 c3",
        )
        self.assertFalse(raw.is_effective_match)
        self.assertTrue(result.is_effective_match)
        self.assertEqual(reasons, ("known-zero CMP versus TEST",))
        self.assertEqual(result.match_ratio, raw.match_ratio)
        self.assertEqual(result.diff, raw.diff)
        self.assertLess(result.match_ratio, 1)

    def test_unproved_zero_or_changed_branch_is_rejected(self):
        cases = (
            ("eb02 31ed 3bc5 7506 b801000000 c3 b802000000 c3",
             "eb02 31ed 85c0 7506 b801000000 c3 b802000000 c3"),
            ("31ed 3bc5 7500 45 ebf9", "31ed 85c0 7500 45 ebf9"),
            ("31ed 66bd0100 3bdd 7501 c3 c3", "31ed 66bd0100 85db 7501 c3 c3"),
            ("31c9 e800010000 3bd9 7501 c3 c3", "31c9 e800010000 85db 7501 c3 c3"),
            ("31ed 3bc5 7501 c3 c3", "31ed 85c9 7501 c3 c3"),
            ("31ed 3bc5 7501 c3 c3", "31ed 85c0 7401 c3 c3"),
            ("31ed 3bc5 7500 ffe2", "31ed 85c0 7500 ffe2"),
        )
        for original, rebuilt in cases:
            with self.subTest(original=original, rebuilt=rebuilt):
                self.assertFalse(compare_bytes(original, rebuilt)[1].is_effective_match)

    def test_auxiliary_flag_observers_are_rejected(self):
        for observer in ("9f", "9c", "27", "37"):
            with self.subTest(observer=observer):
                self.assertFalse(compare_bytes(
                    "31ed 3bc5 7500 " + observer + " c3",
                    "31ed 85c0 7500 " + observer + " c3",
                )[1].is_effective_match)

    def test_remaining_differences_must_pass_upstream_equivalence(self):
        self.assertTrue(compare_bytes(
            "b801000000 31ed 3bdd 7500 c3", "b901000000 31ed 85db 7500 c3",
        )[1].is_effective_match)
        self.assertFalse(compare_bytes(
            "b801000000 31ed 3bdd 7500 c3", "b902000000 31ed 85db 7500 c3",
        )[1].is_effective_match)


class VptrTests(unittest.TestCase):
    def test_overwritten_vptr_composes_with_upstream_register_equivalence(self):
        original = ["mov eax, 1", "mov dword ptr [esi + 4], eax",
                    "mov dword ptr [esi], <OFFSET1>",
                    "mov dword ptr [esi], Final::`vftable' (VTABLE)"]
        rebuilt = ["mov ecx, 1", "mov dword ptr [esi + 4], ecx",
                   "mov dword ptr [esi], Base::`vftable' (VTABLE)", original[-1]]
        result = compare_assembly(original, rebuilt)
        self.assertTrue(result.is_effective_match)
        self.assertLess(result.match_ratio, 1)
        self.assertEqual([line for _, line in result.diff.recomp_inst], rebuilt)

    def test_overwrite_allows_disjoint_store_and_lea(self):
        original = ["mov dword ptr [edi], <OFFSET6>", "mov dword ptr [edi + 4], eax",
                    "lea eax, [esi + 0x34c]", "mov dword ptr [edi], Final::`vftable' (VTABLE)"]
        rebuilt = original.copy()
        rebuilt[0] = "mov dword ptr [edi], Base::`vftable' (VTABLE)"
        self.assertTrue(compare_assembly(original, rebuilt).is_effective_match)

    def test_observable_changed_or_overlapping_store_is_rejected(self):
        first = "mov dword ptr [esi + 0x70], <OFFSET3>"
        replacement = "mov dword ptr [esi + 0x70], Base::`vftable' (VTABLE)"
        final = "mov dword ptr [esi + 0x70], Final::`vftable' (VTABLE)"
        for middle in (
            "call Inspect (FUNCTION)", "mov eax, dword ptr [esi + 0x70]",
            "mov dword ptr [esi + 0x71], eax", "jmp 0x10", "lea esi, [esi + 4]",
            "mov dword ptr [edi], eax", "mov dword ptr [esi + 4], dword ptr [edi]",
        ):
            with self.subTest(middle=middle):
                self.assertFalse(compare_assembly(
                    [first, middle, final], [replacement, middle, final],
                ).is_effective_match)
        for rebuilt in ([replacement],
                        ["mov dword ptr [esi + 0x74], Base::`vftable' (VTABLE)", final]):
            self.assertFalse(compare_assembly([first, final], rebuilt).is_effective_match)


class EffectivePassTests(unittest.TestCase):
    def test_additional_pass_preserves_engine_reports_and_raw_parsers(self):
        upstream, match = fixture("31ed 3bc5 7500 c3", "31ed 85c0 7500 c3")
        raw = upstream.compare_function(match)
        comparisons = ReccmpStatusReport("LEMBALL.EXE")
        comparisons.add_match(ReccmpComparedEntity(
            match.orig_addr, "Fixture", raw.match_ratio, EntityType.FUNCTION,
            match.recomp_addr, rdiff=raw.diff,
        ))
        engine = SimpleNamespace(function_comparator=upstream, get_functions=lambda: [match])
        unchanged = copy.deepcopy(comparisons.entities)
        parsers = (upstream.orig_sanitize, upstream.recomp_sanitize)
        self.assertEqual(additional_effective_matches(engine, comparisons.entities), {
            match.orig_addr: ("known-zero CMP versus TEST",),
        })
        self.assertEqual(comparisons.entities, unchanged)
        self.assertIs(engine.function_comparator, upstream)
        self.assertEqual((upstream.orig_sanitize, upstream.recomp_sanitize), parsers)
        self.assertIs(type(upstream.orig_sanitize), ParseAsm)

    def test_stub_unmatched_exact_and_upstream_equivalent_are_skipped(self):
        upstream, match = fixture("31ed 3bc5 7500 c3", "31ed 85c0 7500 c3")
        comparisons = ReccmpStatusReport("LEMBALL.EXE")
        engine = SimpleNamespace(function_comparator=upstream, get_functions=lambda: [match])
        for flags in ({"is_stub": True}, {"recomp_addr": None},
                      {"accuracy": 1}, {"is_effective_match": True}, {"type": EntityType.VTABLE}):
            with self.subTest(flags=flags):
                values = dict(orig_addr=match.orig_addr, name="Fixture", accuracy=0.8,
                              type=EntityType.FUNCTION, recomp_addr=match.recomp_addr)
                values.update(flags)
                comparisons.add_match(ReccmpComparedEntity(**values))
                self.assertEqual(additional_effective_matches(engine, comparisons.entities), {})
