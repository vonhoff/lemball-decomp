"""Saved Effective results must belong to the exact canonical report."""

import json
import tempfile
import unittest
from pathlib import Path

from make_report import effective_snapshot
from lib.progress.snapshot import load_progress


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
        self.assertEqual(self.snapshot["addresses"], [1, 2])
        self.assertEqual(
            load_progress(self.report_path, self.effective_path), (self.report, {1, 2})
        )

    def test_changed_report_bytes_reject_mixed_generations(self):
        self.report_path.write_bytes(self.raw + b"\n")
        with self.assertRaisesRegex(ValueError, "do not belong"):
            load_progress(self.report_path, self.effective_path)

    def test_legacy_and_unsupported_snapshot_versions_are_rejected(self):
        for snapshot in ({"1": ["legacy"]}, {**self.snapshot, "version": 2}):
            with self.subTest(snapshot=snapshot):
                self.effective_path.write_text(json.dumps(snapshot), encoding="utf-8")
                with self.assertRaisesRegex(ValueError, "do not belong"):
                    load_progress(self.report_path, self.effective_path)

    def test_raw_progress_does_not_require_a_sidecar(self):
        self.effective_path.unlink()
        self.assertEqual(load_progress(self.report_path), (self.report, set()))
        with self.assertRaisesRegex(ValueError, "Run python tools/make_report.py"):
            load_progress(self.report_path, self.effective_path)

    def test_prior_acceptance_policy_is_rejected(self):
        for policy in (None, "paired-signatures-v2"):
            self.snapshot["policy"] = policy
            self.effective_path.write_text(json.dumps(self.snapshot), encoding="utf-8")
            with self.assertRaisesRegex(ValueError, "do not belong"):
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
