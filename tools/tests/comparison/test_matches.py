"""One-hop thunk normalization supplements upstream reccmp results."""

import copy
import unittest
from types import SimpleNamespace

from reccmp.compare.report import ReccmpComparedEntity
from reccmp.types import EntityType

from lib.comparison.matches import ThunkParseAsm, additional_effective_matches
from tests.comparison.fixtures import fixture


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


class AdditionalMatchTests(unittest.TestCase):
    def test_calls_and_tail_jumps_normalize_to_the_paired_function(self):
        for opcode in ("e8", "e9"):
            with self.subTest(opcode=opcode):
                self.assertEqual(compare(*fixture(opcode=opcode)), {0x1000})

    def test_reccmp_equivalence_is_used_after_thunk_normalization(self):
        self.assertEqual(compare(*fixture("3bc7 7200 c3", "3bf8 7700 c3")), {0x1000})

    def test_other_instruction_differences_remain_partial(self):
        self.assertEqual(compare(*fixture("b801000000 c3", "b802000000 c3")), set())

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
                engine = SimpleNamespace(get_functions=lambda: [match])
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

    def test_parser_resets_thunk_usage_and_preserves_address_taking(self):
        comparator, _ = fixture()
        parser = ThunkParseAsm(comparator.orig_bin, {0x4015}, comparator.orig_sanitize)
        parser.parse_asm(bytes.fromhex("e8fb2f0000 c3"), 0x1000)
        self.assertTrue(parser.used_thunk)
        parser.parse_asm(bytes.fromhex("b800400000 c3"), 0x1000)
        self.assertFalse(parser.used_thunk)
