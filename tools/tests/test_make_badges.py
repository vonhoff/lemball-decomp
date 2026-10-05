"""Badge generation tests."""

import unittest

from make_badges import build_badges


class MakeBadgesTests(unittest.TestCase):
    def test_build_badges_formats_expected_schemas(self):
        report = {
            "measures": {
                "total_code": "1000",
                "matched_code_percent": 25.5,
                "fuzzy_match_percent": 80.0,
            },
            "units": [
                {
                    "name": "unit",
                    "functions": [
                        {
                            "size": "500",
                            "metadata": {"virtual_address": str(0x401000)},
                        },
                        {
                            "size": "500",
                            "metadata": {"virtual_address": str(0x402000)},
                        },
                    ],
                }
            ],
        }
        badges = build_badges(report, {0x401000})
        self.assertEqual(
            badges,
            {
                "exact": {
                    "schemaVersion": 1,
                    "label": "Exact",
                    "message": "25.50%",
                    "color": "informational",
                },
                "fuzzy": {
                    "schemaVersion": 1,
                    "label": "Fuzzy",
                    "message": "80.00%",
                    "color": "inactive",
                },
                "effective": {
                    "schemaVersion": 1,
                    "label": "Effective",
                    "message": "50.00%",
                    "color": "success",
                },
            },
        )
