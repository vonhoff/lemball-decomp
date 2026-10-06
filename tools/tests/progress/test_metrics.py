"""Effective counts use accepted addresses and original byte sizes."""

import unittest

from lib.progress.metrics import effective_measures


class ProgressMetricTests(unittest.TestCase):
    def test_effective_measures_sums_accepted_function_sizes(self):
        report = {
            "measures": {"total_code": "600"},
            "units": [
                {
                    "functions": [
                        {"size": "100", "metadata": {"virtual_address": "1"}},
                        {"size": "200", "metadata": {"virtual_address": "2"}},
                    ]
                },
                {
                    "functions": [
                        {"size": "300", "metadata": {"virtual_address": "3"}},
                    ]
                },
            ],
        }
        measures = effective_measures(report, {1, 3, 999})
        self.assertEqual(measures["matched_code"], 400)
        self.assertEqual(measures["matched_functions"], 2)
        self.assertAlmostEqual(measures["matched_code_percent"], 400 / 600 * 100)
        self.assertEqual(effective_measures(report, set())["matched_code"], 0)

    def test_empty_inventory_has_zero_counts_and_percent(self):
        self.assertEqual(
            effective_measures({"measures": {"total_code": "0"}, "units": []}, {1}),
            {"matched_code": 0, "matched_functions": 0, "matched_code_percent": 0.0},
        )
