"""Check report parsing and exact/fuzzy metrics together."""

import unittest
from unittest.mock import Mock

from reccmp.compare.db import ReccmpEntity
from reccmp.compare.report import ReccmpComparedEntity, ReccmpStatusReport
from reccmp.formats.exceptions import InvalidVirtualAddressError
from reccmp.types import EntityType

from lib.extents import original_functions
from report import build_report, group_functions, measures


class ReportTests(unittest.TestCase):
    def test_empty_reports_have_zero_progress(self):
        for groups in ({}, {"Empty": []}):
            with self.subTest(groups=groups):
                report = build_report(groups)
                self.assertEqual(report["measures"]["total_units"], len(groups))
                for values in [report["measures"]] + [
                    u["measures"] for u in report["units"]
                ]:
                    self.assertEqual(values["total_code"], "0")
                    self.assertEqual(values["matched_code"], "0")
                    self.assertEqual(values["total_functions"], 0)
                    self.assertEqual(values["matched_functions"], 0)
                    self.assertEqual(values["fuzzy_match_percent"], 0.0)
                    self.assertEqual(values["matched_code_percent"], 0.0)
                    self.assertEqual(values["matched_functions_percent"], 0.0)

    def test_zero_code_does_not_claim_matched_bytes(self):
        values = measures([{"size": "0", "fuzzy_match_percent": 100.0}])
        self.assertEqual(values["fuzzy_match_percent"], 0.0)
        self.assertEqual(values["matched_code_percent"], 0.0)
        self.assertEqual(values["matched_functions_percent"], 100.0)

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
                    ReccmpEntity(0x1234, None, {"type": EntityType.FUNCTION, "orig_size": 10}),
                    ReccmpEntity(0x401090, 0x5678, {"type": EntityType.FUNCTION, "orig_size": 10}),
                ))
                engine, modules = Mock(), Mock()
                engine.get_all.return_value = entities

                def check_address(address):
                    if address in (0x1234, 0x5678):
                        raise InvalidVirtualAddressError("Fixture address outside PE sections")
                    return 1, 0

                engine.orig_bin.get_relative_addr.side_effect = check_address
                modules.get_module.side_effect = lambda address: (
                    "",
                    "CMakeFiles/LEMBALL.dir/src/Exact.cpp.obj"
                    if address == 0x501000 else "mixed.obj",
                )
                result = build_report(
                    group_functions(original_functions(engine), comparisons, modules)
                )
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
