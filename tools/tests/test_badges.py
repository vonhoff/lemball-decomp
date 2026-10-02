"""Effective badges stay separate from raw canonical matching."""

import copy
import unittest

from reccmp.compare.report import ReccmpComparedEntity, ReccmpStatusReport
from reccmp.types import EntityType

from badges import build_badges


class BadgeTests(unittest.TestCase):
    def test_exact_fuzzy_and_effective_are_distinct(self):
        comparisons = ReccmpStatusReport("LEMBALL.EXE")
        functions = []
        for address, size, score, effective, stub, recomp in (
            (0x401000, 10, 100, True, False, 0x501000),
            (0x401020, 20, 80, True, False, 0x501020),
            (0x401040, 10, 0, True, True, 0x501040),
            (0x401060, 10, 50, False, False, 0x501060),
            (0x401080, 10, 0, False, True, None),
            (0x4010C0, 20, 0, False, False, None),
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
                        EntityType.FUNCTION,
                        recomp,
                        is_effective_match=effective,
                        is_stub=stub,
                    )
                )
        report = {
            "units": [{"name": "Fixture", "functions": functions}],
            "measures": {
                "total_code": "80",
                "matched_code_percent": 12.5,
                "fuzzy_match_percent": 38.75,
            },
        }
        unchanged = copy.deepcopy(report)
        badges = build_badges(report, comparisons)
        self.assertEqual(badges["exact"]["message"], "12.50%")
        self.assertEqual(badges["fuzzy"]["message"], "38.75%")
        self.assertEqual(badges["effective"]["message"], "37.50%")
        self.assertEqual(report, unchanged)
        self.assertTrue(all(badge["schemaVersion"] == 1 for badge in badges.values()))
        raw_comparisons = copy.deepcopy(comparisons.entities)
        additional = {0x401000: ("Already exact",), 0x401040: ("Stub",),
                      0x401060: ("Extra equivalence",), 0x401080: ("Unmatched",),
                      0x4010C0: ("No comparison",)}
        extended = build_badges(report, comparisons, additional)
        self.assertEqual(extended["effective"]["message"], "50.00%")
        self.assertEqual(extended["exact"], badges["exact"])
        self.assertEqual(extended["fuzzy"], badges["fuzzy"])
        self.assertEqual(report, unchanged)
        self.assertEqual(comparisons.entities, raw_comparisons)
