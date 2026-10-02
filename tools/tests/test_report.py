"""Check report parsing and exact/fuzzy metrics together."""

import contextlib
import importlib
import io
import json
import tempfile
import unittest
from pathlib import Path
from unittest.mock import Mock, patch

from reccmp.compare.db import ReccmpEntity
from reccmp.compare.report import ReccmpComparedEntity, ReccmpStatusReport
from reccmp.formats.exceptions import InvalidVirtualAddressError
from reccmp.types import EntityType

from report import build_report, group_functions, original_functions


class ReportTests(unittest.TestCase):
    def test_report(self):
        comparisons = ReccmpStatusReport("LEMBALL.EXE")
        for entity in (
            ReccmpComparedEntity(0x401000, "Exact", 1.0, EntityType.FUNCTION, 0x501000),
            ReccmpComparedEntity(
                0x401020,
                "Equivalent(int)",
                0.8,
                EntityType.FUNCTION,
                0x501020,
                is_effective_match=True,
            ),
            ReccmpComparedEntity(
                0x401040, "Stub", 1.0, EntityType.FUNCTION, 0x501040, is_stub=True
            ),
            ReccmpComparedEntity(0x401060, "Unmatched", 1.0, EntityType.FUNCTION),
            ReccmpComparedEntity(0x402000, "Data", 1.0, EntityType.DATA, 0x502000),
        ):
            comparisons.add_match(entity)
        entities = [
            ReccmpEntity(
                address, recomp, {"type": kind, "name": name, "orig_size": 99, "recomp_size": size}
            )
            for address, recomp, size, name, kind in (
                (0x401000, 0x501000, 10, "Exact", EntityType.FUNCTION),
                (0x401020, 0x501020, 20, "Equivalent", EntityType.FUNCTION),
                (0x401040, 0x501040, 10, "Equivalent", EntityType.FUNCTION),
                (0x401060, 0x501060, 10, "Equivalent", EntityType.FUNCTION),
                (None, 0x501070, 10, "RecompiledOnly", EntityType.FUNCTION),
                (0x402000, 0x502000, 4, "Data", EntityType.DATA),
            )
        ]
        entities.append(ReccmpEntity(0x401080, 0x501080, {"type": EntityType.FUNCTION}))
        entities.append(ReccmpEntity(0x1234, None, {"type": EntityType.FUNCTION, "orig_size": 10}))
        entities.append(
            ReccmpEntity(0x401090, 0x5678, {"type": EntityType.FUNCTION, "orig_size": 10})
        )
        engine, modules = Mock(), Mock()
        engine.get_all.return_value = entities

        def check_address(address):
            if address in (0x1234, 0x5678):
                raise InvalidVirtualAddressError("Fixture address outside PE sections")
            return 1, 0

        engine.orig_bin.get_relative_addr.side_effect = check_address
        engine.recomp_bin.get_relative_addr.side_effect = check_address
        modules.get_module.side_effect = lambda address: (
            "",
            "exact.obj" if address == 0x501000 else "mixed.obj",
        )
        result = build_report(group_functions(original_functions(engine), comparisons, modules))
        empty = build_report({})
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
        self.assertEqual(
            [f["fuzzy_match_percent"] for u in result["units"] for f in u["functions"]],
            [100.0, 80.0, 0.0, 0.0],
        )
        for measures in [totals] + [u["measures"] for u in result["units"]]:
            self.assertFalse(any(k.startswith("complete_") for k in measures))
        self.assertTrue(all("complete" not in u.get("metadata", {}) for u in result["units"]))

    def test_ranking_uses_raw_scores_and_rebuilt_sizes(self):
        ranking = importlib.import_module("next")
        functions = [
            {
                "name": name,
                "size": str(size),
                "fuzzy_match_percent": score,
                "metadata": {"virtual_address": str(address)},
            }
            for name, size, score, address in (
                ("Near", 8, 99, 0x401000),
                ("Gain", 100, 75, 0x402000),
                ("Exact", 200, 100, 0x403000),
            )
        ]
        with tempfile.TemporaryDirectory() as directory:
            report = Path(directory) / "report.json"
            report.write_text(
                json.dumps({"units": [{"name": "Unit", "functions": functions}]}), encoding="utf-8"
            )
            with patch.object(ranking, "REPORT_JSON", report):
                for kind, address, score in (
                    ("near", "0x00401000", "99.00%"),
                    ("gain", "0x00402000", "75.00%"),
                ):
                    output = io.StringIO()
                    with (
                        patch("sys.argv", ["next.py", "--kind", kind, "--limit", "1"]),
                        contextlib.redirect_stdout(output),
                    ):
                        self.assertEqual(ranking.main(), 0)
                    self.assertEqual(len(output.getvalue().splitlines()), 1)
                    self.assertIn(address, output.getvalue())
                    self.assertIn(score, output.getvalue())
