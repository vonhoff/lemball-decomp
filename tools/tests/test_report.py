"""Original-code accounting across canonical reports and Effective badges."""

import copy
import unittest
from unittest.mock import Mock, patch

from reccmp.compare.db import ReccmpEntity
from reccmp.compare.csv import csv_parse
from reccmp.compare.report import ReccmpComparedEntity, ReccmpStatusReport
from reccmp.types import EntityType, ImageId

from badges import build_badges
from report import build_report, measures
from lib import ROOT


class ReportTests(unittest.TestCase):
    def test_only_raw_100_percent_counts_as_exact(self):
        scores = (100, 99.999999999, 80, 0)
        result = measures([{"size": "10", "fuzzy_match_percent": score} for score in scores])
        self.assertEqual(result["matched_functions"], 1)
        self.assertEqual(result["matched_code"], "10")
        self.assertEqual(result["matched_code_percent"], 25)
        self.assertEqual(result["fuzzy_match_percent"], sum(score * 10 for score in scores) / 40)

    def test_lemball_catalog_has_unique_non_overlapping_code(self):
        catalog = list(csv_parse(
            (ROOT / "tools/data/original-function-sizes.csv").read_text(encoding="utf-8")
        ))
        self.assertEqual(len(catalog), len({address for address, _ in catalog}))
        previous_end = 0
        for address, values in catalog:
            self.assertGreater(values["size"], 0)
            self.assertGreaterEqual(address, previous_end)
            previous_end = address + values["size"]

    def test_original_inventory_raw_scores_and_effective_badges(self):
        entities, comparisons = [], ReccmpStatusReport("LEMBALL.EXE")
        for address, size, kind, score, matched, flags in (
            (0x401000, 10, EntityType.FUNCTION, 1.0, True, {}),
            (0x401020, 20, EntityType.FUNCTION, 0.8, True, {"is_effective_match": True}),
            (0x401040, 10, EntityType.FUNCTION, 1.0, True, {"is_stub": True}),
            (0x401060, 10, EntityType.FUNCTION, 0.0, False, {}),
            (0x401070, 8, EntityType.VTORDISP, 1.0, True, {}),
            (0x401080, 12, None, None, False, {}),
            (0x401090, 5, EntityType.THUNK, None, False, {}),
            (0x4010a0, 5, EntityType.IMPORT_THUNK, None, True, {}),
            (0x4010b0, 20, EntityType.FUNCTION, 0.5, True, {}),
        ):
            recomp = address + 0x100000 if matched else None
            entities.append(ReccmpEntity(address, recomp, {
                "type": kind, "orig_size": size, "recomp_size": 1000,
            }))
            if score is not None:
                comparisons.add_match(ReccmpComparedEntity(
                    address, "Fixture", score, EntityType.FUNCTION, recomp, **flags,
                ))
        catalog = {entity.orig_addr: {"size": entity.size(ImageId.ORIG)} for entity in entities}
        entities.extend((
            ReccmpEntity(None, 0x502000, {"type": EntityType.FUNCTION, "recomp_size": 1000}),
            ReccmpEntity(0x403000, None, {"type": EntityType.DATA, "orig_size": 1000}),
        ))
        engine, modules = Mock(), Mock()
        engine.get_all.return_value = list(reversed(entities))
        modules.get_module.side_effect = lambda address: ("", "Exact" if address == 0x501000 else "Mixed")
        with patch("report.csv_parse", return_value=catalog.items()):
            result = build_report(engine, comparisons, modules)
        totals = result["measures"]
        self.assertEqual([unit["name"] for unit in result["units"]], ["Compiler-generated", "Exact", "Mixed"])
        for unit in result["units"]:
            addresses = [int(f["metadata"]["virtual_address"]) for f in unit["functions"]]
            self.assertEqual(addresses, sorted(addresses))
        self.assertEqual((totals["total_functions"], totals["total_code"]), (9, "100"))
        self.assertEqual((totals["matched_functions"], totals["matched_code"]), (2, "18"))
        self.assertEqual(totals["matched_code_percent"], 18)
        self.assertEqual(totals["fuzzy_match_percent"], 44)
        scores = {int(f["metadata"]["virtual_address"]): f["fuzzy_match_percent"]
                  for unit in result["units"] for f in unit["functions"]}
        self.assertEqual(scores, {
            0x401000: 100, 0x401020: 80, 0x401040: 0, 0x401060: 0, 0x401070: 100,
            0x401080: 0, 0x401090: 0, 0x4010a0: 0, 0x4010b0: 50,
        })
        for key in ("total_functions", "total_code", "matched_functions", "matched_code"):
            self.assertEqual(sum(int(unit["measures"][key]) for unit in result["units"]), int(totals[key]))
        unchanged = copy.deepcopy((result, comparisons.entities))
        badges = build_badges(result, comparisons, {
            0x401000, 0x401040, 0x401060, 0x401080, 0x401090, 0x4010a0, 0x4010b0,
        })
        self.assertEqual({name: badge["message"] for name, badge in badges.items()}, {
            "exact": "18.00%", "fuzzy": "44.00%", "effective": "58.00%",
        })
        self.assertEqual((result, comparisons.entities), unchanged)
