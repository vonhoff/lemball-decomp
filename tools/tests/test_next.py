"""Ranking order, output limits, and report read failures."""

import contextlib
import io
import json
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import next as ranking


class RankingTests(unittest.TestCase):
    def test_ranking_and_limits(self):
        functions = [
            {
                "name": name,
                "size": str(size),
                "fuzzy_match_percent": score,
                "metadata": {"virtual_address": str(address)},
            }
            for name, size, score, address in (
                ("NearLarge", 16, 99, 0x401020),
                ("NearLow", 8, 99, 0x401010),
                ("NearHigh", 8, 99, 0x401030),
                ("GainSmall", 100, 75, 0x402020),
                ("GainLarge", 150, 50, 0x402010),
                ("GainSmallLow", 100, 75, 0x402000),
                ("Exact", 200, 100, 0x403000),
            )
        ]
        report = {"units": [{"name": "Unit", "functions": functions}]}
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "report.json"
            serialized = json.dumps(report)
            path.write_text(serialized, encoding="utf-8")
            for kind, names in (
                (
                    "near",
                    ["NearLow", "NearHigh", "NearLarge", "GainSmallLow", "GainSmall", "GainLarge"],
                ),
                (
                    "gain",
                    ["GainLarge", "GainSmallLow", "GainSmall", "NearLarge", "NearLow", "NearHigh"],
                ),
            ):
                for limit in (1, 0, -1):
                    output = io.StringIO()
                    with (
                        self.subTest(kind=kind, limit=limit),
                        patch.object(ranking, "REPORT_JSON", path),
                        patch("sys.argv", ["next.py", "--kind", kind, "--limit", str(limit)]),
                        contextlib.redirect_stdout(output),
                    ):
                        self.assertEqual(ranking.main(), 0)
                    self.assertEqual(
                        [line.split()[-1] for line in output.getvalue().splitlines()],
                        names[:limit] if limit > 0 else names,
                    )
                    self.assertIn("99.00%" if kind == "near" else "50.00%", output.getvalue())
                    self.assertEqual(path.read_text(encoding="utf-8"), serialized)
        self.assertEqual(ranking.rank_functions({"units": []}, "near"), [])

    def test_unreadable_report(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "report.json"
            for missing in (True, False):
                if not missing:
                    path.write_text("{", encoding="utf-8")
                with (
                    self.subTest(missing=missing),
                    patch.object(ranking, "REPORT_JSON", path),
                    patch("sys.argv", ["next.py"]),
                    self.assertRaisesRegex(SystemExit, "cannot read report:"),
                ):
                    ranking.main()
