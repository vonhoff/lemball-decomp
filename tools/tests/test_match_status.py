"""Do not confuse reccmp's address pairing method with an exact code score."""

import unittest

from match import match_status
from reccmp.compare.report import ReccmpComparedEntity


class MatchStatusTests(unittest.TestCase):
    def entity(self, accuracy, **kwargs):
        return ReccmpComparedEntity(0x1000, "test", accuracy,
                                    recomp_addr=0x2000, **kwargs)

    def test_mapped_partial_function_is_not_labeled_match(self):
        entity = self.entity(0.08)
        self.assertTrue(entity.is_matched())
        self.assertEqual(match_status(entity), "")

    def test_stub_takes_precedence_even_for_identical_empty_body(self):
        for accuracy in (0.08, 1.0):
            self.assertEqual(match_status(self.entity(accuracy, is_stub=True)), "STUB")

    def test_exact_and_effective_matches_are_labeled_match(self):
        self.assertEqual(match_status(self.entity(1.0)), "MATCH")
        self.assertEqual(match_status(self.entity(0.9, is_effective_match=True)), "MATCH")
