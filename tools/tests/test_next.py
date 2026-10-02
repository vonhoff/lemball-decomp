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
                "metadata": {"virtual_address": str(address), "demangled_name": name},
            }
            for name, size, score, address in (
                ("Large", 16, 99, 0x401020),
                ("LowAddress", 8, 99, 0x401010),
                ("HighAddress", 8, 99, 0x401030),
                ("Partial", 100, 75, 0x402020),
                ("LowerScore", 150, 50, 0x402010),
                ("Exact", 200, 100, 0x403000),
            )
        ]
        report = {"units": [{"name": "Unit", "functions": functions}]}
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "report.json"
            serialized = json.dumps(report)
            path.write_text(serialized, encoding="utf-8")
            names = ["LowAddress", "HighAddress", "Large", "Partial", "LowerScore"]
            for limit in (None, 1, 0):
                output = io.StringIO()
                flags = [] if limit is None else ["--limit", str(limit)]
                with (
                    self.subTest(limit=limit),
                    patch.object(ranking, "REPORT_JSON", path),
                    patch("sys.argv", ["next.py", *flags]),
                    contextlib.redirect_stdout(output),
                ):
                    self.assertEqual(ranking.main(), 0)
                self.assertEqual(
                    [line.split()[-1] for line in output.getvalue().splitlines()],
                    names[:limit] if limit is not None and limit > 0 else names,
                )
                self.assertIn("99.00%", output.getvalue())
                self.assertEqual(path.read_text(encoding="utf-8"), serialized)
        self.assertEqual(ranking.rank_functions({"units": []}), [])

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
