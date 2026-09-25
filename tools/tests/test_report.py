"""Report exact matches must agree with match.py's reccmp criterion."""

import unittest
from unittest.mock import patch

from report import build_report


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


if __name__ == "__main__":
    unittest.main()
