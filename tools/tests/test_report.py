"""Check report parsing and exact/fuzzy metrics together."""

import unittest
from unittest.mock import Mock, patch

from reccmp.compare.db import ReccmpEntity
from reccmp.compare.csv import csv_parse
from reccmp.compare.report import ReccmpComparedEntity, ReccmpStatusReport
from reccmp.types import EntityType

from report import build_report
from lib import ROOT


class ReportTests(unittest.TestCase):
    def test_lemball_catalog_has_unique_non_overlapping_code(self):
        catalog = list(csv_parse(
            (ROOT / "tools/data/original-function-sizes.csv").read_text(encoding="utf-8")
        ))
        self.assertEqual(len(catalog), len({address for address, _ in catalog}))
        previous_end = 0
        for address, values in sorted(catalog):
            self.assertGreater(values["size"], 0)
            self.assertGreaterEqual(address, previous_end)
            previous_end = address + values["size"]

    def test_catalogued_untyped_and_linker_code_remains_in_denominator(self):
        entities = [
            ReccmpEntity(0x401000, 0x501000, {
                "type": EntityType.FUNCTION, "orig_size": 10,
            }),
            ReccmpEntity(0x401020, None, {"orig_size": 20}),
            ReccmpEntity(0x401040, None, {
                "type": EntityType.THUNK, "orig_size": 5,
            }),
            ReccmpEntity(0x401050, 0x501050, {
                "type": EntityType.IMPORT_THUNK, "orig_size": 6,
            }),
            ReccmpEntity(0x402000, None, {
                "type": EntityType.DATA, "orig_size": 4,
            }),
        ]
        engine, modules = Mock(), Mock()
        engine.get_all.return_value = entities
        modules.get_module.return_value = None
        comparisons = ReccmpStatusReport("LEMBALL.EXE")
        comparisons.add_match(ReccmpComparedEntity(
            0x401000, "Exact", 1.0, EntityType.FUNCTION, 0x501000,
        ))
        catalog = {0x401000: 10, 0x401020: 20, 0x401040: 5, 0x401050: 6}
        with patch("report.csv_parse", return_value=catalog.items()):
            result = build_report(engine, comparisons, modules)
        self.assertEqual(result["measures"]["total_functions"], 4)
        self.assertEqual(result["measures"]["total_code"], "41")
        self.assertEqual(result["measures"]["matched_functions"], 1)
        self.assertEqual(result["measures"]["matched_code"], "10")
        self.assertAlmostEqual(result["measures"]["fuzzy_match_percent"], 1000 / 41)
        self.assertEqual(
            [f["fuzzy_match_percent"] for u in result["units"] for f in u["functions"]],
            [100.0, 0.0, 0.0, 0.0],
        )

    def test_inventory_and_progress_ignore_rebuilt_sizes(self):
        comparisons = ReccmpStatusReport("LEMBALL.EXE")
        for address, name, score, recomp, kind, flags in (
            (0x401000, "Exact", 1.0, 0x501000, EntityType.FUNCTION, {}),
            (0x401020, "Equivalent(int)", 0.8, 0x501020, EntityType.FUNCTION, {"is_effective_match": True}),
            (0x401040, "Stub", 1.0, 0x501040, EntityType.FUNCTION, {"is_stub": True}),
            (0x401060, "Unmatched", 1.0, None, EntityType.FUNCTION, {}),
            (0x401070, "Adjuster", 1.0, 0x501070, EntityType.FUNCTION, {}),
            (0x402000, "Data", 1.0, 0x502000, EntityType.DATA, {}),
        ):
            comparisons.add_match(ReccmpComparedEntity(address, name, score, kind, recomp, **flags))
        for rebuilt_size in (1, 1000):
            with self.subTest(rebuilt_size=rebuilt_size):
                entities = [
                    ReccmpEntity(
                        address,
                        recomp,
                        {"type": kind, "name": name, "orig_size": size, "recomp_size": rebuilt_size},
                    )
                    for address, recomp, size, name, kind in (
                        (0x401000, 0x501000, 10, "Exact", EntityType.FUNCTION),
                        (0x401020, 0x501020, 20, "Equivalent", EntityType.FUNCTION),
                        (0x401040, 0x501040, 10, "Equivalent", EntityType.FUNCTION),
                        (0x401060, 0x501060, 10, "Equivalent", EntityType.FUNCTION),
                        (0x401070, 0x501070, 8, "Adjuster", EntityType.VTORDISP),
                        (None, 0x501078, 10, "RecompiledOnly", EntityType.FUNCTION),
                        (0x402000, 0x502000, 4, "Data", EntityType.DATA),
                    )
                ]
                entities.extend((
                    ReccmpEntity(0x401080, 0x501080, {"type": EntityType.FUNCTION, "orig_size": 12}),
                    ReccmpEntity(0x401090, 0x501090, {"type": EntityType.FUNCTION, "orig_size": 10}),
                ))
                engine, modules = Mock(), Mock()
                engine.get_all.return_value = entities

                modules.get_module.side_effect = lambda recompiled_address: (
                    "",
                    "CMakeFiles/LEMBALL.dir/src/Exact.cpp.obj"
                    if recompiled_address == 0x501000 else "mixed.obj",
                )
                catalog = {entity.orig_addr: {} for entity in entities
                           if entity.orig_addr is not None and entity.entity_type != EntityType.DATA}
                with patch("report.csv_parse", return_value=catalog.items()):
                    result = build_report(engine, comparisons, modules)
                functions = [f for u in result["units"] for f in u["functions"]]
                self.assertEqual(
                    [unit["name"] for unit in result["units"]],
                    ["CMakeFiles/LEMBALL.dir/src/Exact.cpp.obj", "mixed.obj"],
                )
                self.assertTrue(all("metadata" not in unit for unit in result["units"]))
                self.assertEqual(len({f["name"] for f in functions}), 7)
                self.assertEqual(functions[1]["metadata"]["demangled_name"], "Equivalent(int)")
                totals = result["measures"]
                self.assertEqual(totals["total_functions"], 7)
                self.assertEqual(totals["matched_functions"], 2)
                self.assertEqual(totals["matched_code"], "18")
                self.assertEqual(totals["total_code"], "80")
                self.assertEqual(totals["matched_code_percent"], 22.5)
                self.assertEqual(totals["fuzzy_match_percent"], 42.5)
                self.assertEqual(
                    [f["fuzzy_match_percent"] for u in result["units"] for f in u["functions"]],
                    [100.0, 80.0, 0.0, 0.0, 100.0, 0.0, 0.0],
                )
                for values in [totals] + [u["measures"] for u in result["units"]]:
                    self.assertFalse(any(k.startswith("complete_") for k in values))
