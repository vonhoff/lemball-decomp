"""Effective badges stay separate from raw canonical matching."""

import copy
import unittest

from reccmp.compare.report import ReccmpComparedEntity, ReccmpStatusReport
from reccmp.types import EntityType

from badges import build_badges
from report import build_report


class BadgeTests(unittest.TestCase):
    def test_empty_report_badges_have_zero_progress(self):
        comparisons = ReccmpStatusReport("LEMBALL.EXE")
        for groups in ({}, {"Empty": []}):
            with self.subTest(groups=groups):
                badges = build_badges(build_report(groups), comparisons)
                self.assertTrue(all(badge["message"] == "0.00%" for badge in badges.values()))

    def test_exact_fuzzy_and_effective_are_distinct(self):
        comparisons = ReccmpStatusReport("LEMBALL.EXE")
        functions = []
        for address, size, score, effective, stub, kind, recomp in (
            (0x401000, 10, 100, True, False, EntityType.FUNCTION, 0x501000),
            (0x401020, 20, 80, True, False, EntityType.FUNCTION, 0x501020),
            (0x401040, 10, 0, True, True, EntityType.FUNCTION, 0x501040),
            (0x401060, 10, 50, False, False, EntityType.FUNCTION, 0x501060),
            (0x401080, 10, 0, True, False, EntityType.FUNCTION, None),
            (0x4010A0, 10, 0, True, False, EntityType.DATA, 0x5010A0),
            (0x4010C0, 10, 0, False, False, EntityType.FUNCTION, None),
        ):
            functions.append(
                {
                    "name": hex(address),
                    "size": str(size),
                    "metadata": {"virtual_address": str(address)},
                    "fuzzy_match_percent": score,
                }
            )
            if address != 0x4010C0:
                comparisons.add_match(
                    ReccmpComparedEntity(
                        address,
                        "Fixture",
                        score / 100,
                        kind,
                        recomp,
                        is_effective_match=effective,
                        is_stub=stub,
                    )
                )
        report = build_report({"Fixture": functions})
        unchanged = copy.deepcopy(report)
        badges = build_badges(report, comparisons)
        self.assertEqual(badges["exact"]["message"], "12.50%")
        self.assertEqual(badges["fuzzy"]["message"], "38.75%")
        self.assertEqual(badges["effective"]["message"], "37.50%")
        self.assertEqual(report, unchanged)
        self.assertEqual(report["measures"]["matched_functions"], 1)
        self.assertTrue(all(badge["schemaVersion"] == 1 for badge in badges.values()))
