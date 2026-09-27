"""Report exact matches must agree with match.py's reccmp criterion."""

import unittest
from unittest.mock import patch

from report import build_report, measures


class ReportMatchTests(unittest.TestCase):
    def test_report_counts_only_reccmp_effective_matches(self):
        inventory = [
            {"address": 1, "size": 80, "name": "Partial", "module": "partial.obj"},
            {"address": 2, "size": 20, "name": "Effective", "module": "exact.obj"},
        ]
        matches = {
            1: {"matching": 0.75, "diff": [["", [{
                "orig": [[0, "call <OFFSET1>"]],
                "recomp": [[0, "call Example (FUNCTION)"]],
            }]]]},
            2: {"matching": 0.9, "effective": True},
        }
        with patch("report.load_inventory", return_value=inventory), patch(
            "report.load_matches", return_value=matches
        ):
            result = build_report(None, None)

        self.assertEqual(result["measures"]["matched_functions"], 1)
        self.assertEqual(result["measures"]["matched_code"], "20")
        scores = {f["name"]: f["fuzzy_match_percent"]
                  for unit in result["units"] for f in unit["functions"]}
        self.assertEqual(scores, {"Partial": 75.0, "Effective": 100.0})

    def test_fully_linked_counts_whole_units_not_individual_matches(self):
        inventory = [
            {"address": 1, "size": 30, "name": "Exact", "module": "mixed.obj"},
            {"address": 2, "size": 50, "name": "Partial", "module": "mixed.obj"},
            {"address": 3, "size": 20, "name": "Effective", "module": "complete.obj"},
        ]
        matches = {
            1: {"matching": 1.0},
            2: {"matching": 0.5},
            3: {"matching": 0.9, "effective": True},
        }
        with patch("report.load_inventory", return_value=inventory), patch(
            "report.load_matches", return_value=matches
        ):
            result = build_report(None, None)

        totals = result["measures"]
        self.assertEqual(totals["matched_code"], "50")
        self.assertEqual(totals["complete_code"], "20")
        self.assertEqual(totals["complete_code_percent"], 20.0)
        self.assertEqual(totals["complete_units"], 1)
        self.assertEqual(totals["total_units"], 2)
        units = {unit["name"]: unit for unit in result["units"]}
        self.assertFalse(units["mixed"]["metadata"]["complete"])
        self.assertEqual(units["mixed"]["measures"]["complete_code"], "0")
        self.assertTrue(units["complete"]["metadata"]["complete"])
        self.assertEqual(units["complete"]["measures"]["complete_code_percent"], 100.0)
        self.assertEqual(units["mixed"]["sections"], [
            {"name": ".text", "size": "80", "fuzzy_match_percent": 68.75}
        ])

    def test_stub_and_missing_comparison_keep_units_incomplete(self):
        inventory = [
            {"address": 1, "size": 10, "name": "Exact", "module": "stub.obj"},
            {"address": 2, "size": 10, "name": "Stub", "module": "stub.obj"},
            {"address": 3, "size": 10, "name": "Missing", "module": "missing.obj"},
        ]
        matches = {1: {"matching": 1.0}, 2: {"matching": 1.0, "stub": True}}
        with patch("report.load_inventory", return_value=inventory), patch(
            "report.load_matches", return_value=matches
        ):
            result = build_report(None, None)

        self.assertEqual(result["measures"]["total_functions"], 3)
        self.assertEqual(result["measures"]["complete_units"], 0)
        self.assertEqual(result["measures"]["complete_code"], "0")
        self.assertTrue(all(not unit["metadata"]["complete"] for unit in result["units"]))

    def test_empty_and_zero_size_reports_have_finite_linked_percent(self):
        empty = measures([], total_units=0)
        self.assertEqual(empty["complete_units"], 0)
        self.assertEqual(empty["complete_code_percent"], 0.0)
        zero_size = measures([{"size": 0, "ratio": 100.0}])
        self.assertEqual(zero_size["complete_units"], 1)
        self.assertEqual(zero_size["complete_code"], "0")
        self.assertEqual(zero_size["complete_code_percent"], 0.0)


if __name__ == "__main__":
    unittest.main()
