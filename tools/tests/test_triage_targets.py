"""Target selection and ranking tests."""

import unittest

from triage_targets import rank_functions


def fixture(rows):
    return {
        "units": [
            {
                "name": "unit",
                "functions": [
                    {
                        "name": hex(address),
                        "size": str(size),
                        "fuzzy_match_percent": score,
                        "metadata": {
                            "virtual_address": str(address),
                            "demangled_name": "Target",
                        },
                    }
                    for address, size, score in rows
                ],
            }
        ]
    }


class TriageTargetsTests(unittest.TestCase):
    def test_ranking_filters_effective_and_size_without_rounding_scores(self):
        report = fixture([(1, 100, 99), (2, 900, 90), (3, 800, 100), (4, 700, 98)])

        def addresses(rows):
            return [int(row["metadata"]["virtual_address"]) for row in rows]

        self.assertEqual(addresses(rank_functions(report, {4})), [1, 2])
        self.assertEqual(addresses(rank_functions(report, {4}, min_size=300)), [2])
        self.assertEqual(addresses(rank_functions(report, sort="size")), [2, 4, 1])
