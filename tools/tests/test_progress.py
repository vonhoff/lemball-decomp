"""Batch accounting and saved progress tests."""

import json
import tempfile
import unittest
from pathlib import Path

from lib.progress import effective_measures, effective_snapshot, load_progress


def fixture(rows):
    total = sum(size for _, size, _ in rows)
    return {
        "measures": {"total_code": str(total)},
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
        ],
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
            with self.assertRaisesRegex(ValueError, "Run python tools/make_report.py"):
                load_progress(report_path, effective_path)
            effective_path.write_text(json.dumps({str(0x401000): ["legacy"]}))
            with self.assertRaisesRegex(ValueError, "do not belong"):
                load_progress(report_path, effective_path)
            effective_path.unlink()
            self.assertEqual(load_progress(report_path), (report, set()))
            with self.assertRaisesRegex(ValueError, "Run python tools/make_report.py"):
                load_progress(report_path, effective_path)

    def test_effective_measures_sums_accepted_function_sizes(self):
        report = fixture([(1, 100, 99), (2, 200, 90), (3, 300, 100)])
        measures = effective_measures(report, {1, 3})
        self.assertEqual(measures["matched_code"], 400)
        self.assertEqual(measures["matched_functions"], 2)
        self.assertAlmostEqual(measures["matched_code_percent"], 400 / 600 * 100)
