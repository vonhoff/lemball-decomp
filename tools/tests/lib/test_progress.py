"""Effective byte accounting and report-bound saved results."""

import json
import tempfile
import unittest
from pathlib import Path

from reccmp.compare.report import ReccmpComparedEntity
from reccmp.types import EntityType

from lib.progress import effective_addresses, effective_measures, effective_snapshot, load_progress


class ProgressSnapshotTests(unittest.TestCase):
    def setUp(self):
        root = Path(self.enterContext(tempfile.TemporaryDirectory()))
        self.report_path = root / "report.json"
        self.effective_path = root / "effective.json"
        self.report = {"units": []}
        self.raw = json.dumps(self.report).encode()
        self.report_path.write_bytes(self.raw)
        self.snapshot = effective_snapshot(self.raw, {2, 1})
        self.effective_path.write_text(json.dumps(self.snapshot), encoding="utf-8")

    def test_snapshot_round_trip_sorts_addresses(self):
        self.assertEqual(set(self.snapshot), {"report_sha256", "addresses"})
        self.assertEqual(self.snapshot["addresses"], [1, 2])
        self.assertEqual(
            load_progress(self.report_path, self.effective_path), (self.report, {1, 2})
        )

    def test_changed_report_bytes_reject_mixed_generations(self):
        self.report_path.write_bytes(self.raw + b"\n")
        with self.assertRaisesRegex(ValueError, "do not belong"):
            load_progress(self.report_path, self.effective_path)

    def test_snapshot_without_report_hash_is_rejected(self):
        self.effective_path.write_text(json.dumps({"1": ["legacy"]}), encoding="utf-8")
        with self.assertRaisesRegex(ValueError, "Run python tools/make_report.py"):
            load_progress(self.report_path, self.effective_path)

    def test_raw_progress_does_not_require_a_sidecar(self):
        self.effective_path.unlink()
        self.assertEqual(load_progress(self.report_path), (self.report, set()))
        with self.assertRaisesRegex(ValueError, "Run python tools/make_report.py"):
            load_progress(self.report_path, self.effective_path)

    def test_missing_report_and_invalid_json_are_reported(self):
        self.report_path.unlink()
        with self.assertRaisesRegex(ValueError, "Run python tools/make_report.py"):
            load_progress(self.report_path)
        self.report_path.write_bytes(b"{")
        with self.assertRaisesRegex(ValueError, "Run python tools/make_report.py"):
            load_progress(self.report_path)

    def test_missing_accepted_addresses_are_rejected(self):
        del self.snapshot["addresses"]
        self.effective_path.write_text(json.dumps(self.snapshot), encoding="utf-8")
        with self.assertRaisesRegex(ValueError, "Run python tools/make_report.py"):
            load_progress(self.report_path, self.effective_path)


class ProgressMetricTests(unittest.TestCase):
    def test_only_upstream_exact_and_equivalent_implemented_functions_are_accepted(self):
        comparisons = {
            address: ReccmpComparedEntity(address, "Fixture", score, kind, rebuilt, **flags)
            for address, score, kind, rebuilt, flags in (
                (1, 1.0, EntityType.FUNCTION, 101, {}),
                (2, 0.8, EntityType.FUNCTION, 102, {"is_effective_match": True}),
                (3, 0.7, EntityType.FUNCTION, 103, {}),
                (4, 0.999999999, EntityType.FUNCTION, 104, {}),
                (5, 1.0, EntityType.FUNCTION, 105, {"is_stub": True}),
                (6, 1.0, EntityType.FUNCTION, None, {}),
                (7, 1.0, EntityType.DATA, 107, {}),
            )
        }
        self.assertEqual(effective_addresses(comparisons), {1, 2})

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
