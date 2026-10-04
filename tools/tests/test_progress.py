"""Batch accounting and saved effective results must agree."""

import json
import tempfile
import unittest
from pathlib import Path

from lib.progress import effective_snapshot, exact_regressions, load_progress
from next import rank_functions


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


class ProgressTests(unittest.TestCase):
    def test_effective_snapshot_rejects_mixed_generations_and_legacy_format(self):
        with tempfile.TemporaryDirectory() as temp:
            report_path = Path(temp) / "report.json"
            effective_path = Path(temp) / "effective.json"
            report = fixture([(0x401000, 200, 80)])
            raw = json.dumps(report).encode()
            report_path.write_bytes(raw)
            effective_path.write_text(
                json.dumps(effective_snapshot(raw, {0x401000}, {}))
            )
            self.assertEqual(
                load_progress(report_path, effective_path), (report, {0x401000})
            )
            report_path.write_bytes(raw + b"\n")
            with self.assertRaisesRegex(ValueError, "Run python tools/report.py"):
                load_progress(report_path, effective_path)
            effective_path.write_text(json.dumps({str(0x401000): ["legacy"]}))
            with self.assertRaisesRegex(ValueError, "do not belong"):
                load_progress(report_path, effective_path)
            effective_path.unlink()
            self.assertEqual(load_progress(report_path), (report, set()))
            with self.assertRaisesRegex(ValueError, "Run python tools/report.py"):
                load_progress(report_path, effective_path)

    def test_exact_audit_detects_offsetting_gains_missing_entries_and_near_exact(self):
        before = fixture([(1, 100, 100), (2, 100, 100), (3, 200, 50)])
        after = fixture([(1, 100, 99.999999), (3, 200, 100)])
        self.assertEqual(set(exact_regressions(before, after)), {1, 2})
        self.assertEqual(exact_regressions(before, before), {})

    def test_ranking_filters_effective_and_size_without_rounding_scores(self):
        report = fixture([(1, 100, 99), (2, 900, 90), (3, 800, 100), (4, 700, 98)])

        def addresses(rows):
            return [int(row["metadata"]["virtual_address"]) for row in rows]

        self.assertEqual(addresses(rank_functions(report, {4})), [1, 2])
        self.assertEqual(addresses(rank_functions(report, {4}, min_size=300)), [2])
        self.assertEqual(addresses(rank_functions(report, sort="size")), [2, 4, 1])
