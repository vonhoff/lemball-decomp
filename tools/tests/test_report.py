"""Check report parsing and exact/fuzzy metrics together."""

import contextlib
import importlib
import io
import json
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from report import build_report


class ReportTests(unittest.TestCase):
    def test_report(self):
        with tempfile.TemporaryDirectory() as directory:
            roadmap, comparisons = Path(directory) / "roadmap.csv", Path(directory) / "reccmp.json"
            roadmap.write_text(
                "row_type,orig_addr,size,name,module\n"
                "fun,401000,a,Exact,exact.obj\n"
                "fun,401020,14,Equivalent,mixed.obj\n"
                "fun,401040,a,Equivalent,mixed.obj\n"
                "fun,401060,a,Equivalent,mixed.obj\n"
                "fun,401080,0,Zero,mixed.obj\n"
                "dat,402000,4,Data,mixed.obj\n", encoding="utf-8",
            )
            comparisons.write_text(json.dumps({"data": [
                {"address": "0x401000", "type": 1, "matching": 1.0},
                {"address": "0x401020", "type": 1, "name": "Equivalent(int)",
                 "matching": 0.8, "effective": True},
                {"address": "0x401040", "type": 1, "matching": 1.0, "stub": True},
                {"address": "0x402000", "type": 2, "matching": 1.0},
            ]}), encoding="utf-8")
            result = build_report(roadmap, comparisons)
            roadmap.write_text("row_type,orig_addr,size,name,module\n", encoding="utf-8")
            empty = build_report(roadmap, comparisons)
        self.assertEqual(empty["measures"]["total_functions"], 0)
        self.assertEqual(empty["measures"]["fuzzy_match_percent"], 100.0)
        functions = [f for u in result["units"] for f in u["functions"]]
        self.assertEqual(len({f["name"] for f in functions}), 4)
        self.assertEqual(functions[1]["metadata"]["demangled_name"], "Equivalent(int)")
        totals = result["measures"]
        self.assertEqual(totals["total_functions"], 4)
        self.assertEqual(totals["matched_functions"], 1)
        self.assertEqual(totals["matched_code"], "10")
        self.assertEqual(totals["total_code"], "50")
        self.assertEqual(totals["fuzzy_match_percent"], 52.0)
        self.assertEqual([f["fuzzy_match_percent"] for u in result["units"]
                          for f in u["functions"]], [100.0, 80.0, 0.0, 0.0])
        for measures in [totals] + [u["measures"] for u in result["units"]]:
            self.assertFalse(any(k.startswith("complete_") for k in measures))
        self.assertTrue(all("complete" not in u.get("metadata", {}) for u in result["units"]))

    def test_ranking_uses_raw_scores_and_rebuilt_sizes(self):
        ranking = importlib.import_module("next")
        functions = [
            {"name": name, "size": str(size), "fuzzy_match_percent": score,
             "metadata": {"virtual_address": str(address)}}
            for name, size, score, address in
            (("Near", 8, 99, 0x401000), ("Gain", 100, 75, 0x402000), ("Exact", 200, 100, 0x403000))
        ]
        with tempfile.TemporaryDirectory() as directory:
            report = Path(directory) / "report.json"
            report.write_text(json.dumps({"units": [{"name": "Unit", "functions": functions}]}),
                              encoding="utf-8")
            with patch.object(ranking, "REPORT_JSON", report):
                for kind, address, score in (("near", "0x00401000", "99.00%"),
                                             ("gain", "0x00402000", "75.00%")):
                    output = io.StringIO()
                    with (patch("sys.argv", ["next.py", "--kind", kind, "--limit", "1"]),
                          contextlib.redirect_stdout(output)):
                        self.assertEqual(ranking.main(), 0)
                    self.assertEqual(len(output.getvalue().splitlines()), 1)
                    self.assertIn(address, output.getvalue())
                    self.assertIn(score, output.getvalue())
