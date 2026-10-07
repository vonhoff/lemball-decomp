"""Original-code accounting across canonical reports and Effective badges."""

import copy
import unittest
from unittest.mock import Mock, patch

from reccmp.compare.db import ReccmpEntity
from reccmp.compare.report import ReccmpComparedEntity, ReccmpStatusReport
from reccmp.types import EntityType, ImageId
from reccmp.formats.exceptions import InvalidVirtualReadError

from make_report import build_report, is_catalogued_jump_thunk, measures
from lib.comparison import effective_addresses
from lib.progress import effective_measures


class MakeReportTests(unittest.TestCase):
    def test_catalogued_thunk_requires_complete_e9_to_another_entry(self):
        for instruction, expected in (
            ("e90b000000", True),
            ("e90b0000", False),
            ("e80b000000", False),
            ("e9fbffffff", False),
            ("e90c000000", False),
            ("", False),
        ):
            with self.subTest(instruction=instruction):
                image = Mock()
                image.read.return_value = bytes.fromhex(instruction)
                self.assertEqual(
                    is_catalogued_jump_thunk(
                        image, 0x401000, 5, {0x401000: {}, 0x401010: {}}
                    ),
                    expected,
                )
                image.read.assert_called_once_with(0x401000, 5)

    def test_catalogued_thunk_rejects_wrong_size_and_unreadable_code(self):
        image = Mock()
        self.assertFalse(is_catalogued_jump_thunk(image, 0x401000, 4, {}))
        image.read.assert_not_called()
        image.read.side_effect = InvalidVirtualReadError(0x401000)
        self.assertFalse(is_catalogued_jump_thunk(image, 0x401000, 5, {}))

    def test_catalogued_thunk_wraps_the_ia32_target(self):
        image = Mock()
        image.read.return_value = bytes.fromhex("e91b000000")
        self.assertTrue(is_catalogued_jump_thunk(image, 0xFFFFFFF0, 5, {0x10: {}}))

    def test_only_raw_100_percent_counts_as_exact(self):
        scores = (100, 99.999999999, 80, 0)
        result = measures(
            [{"size": "10", "fuzzy_match_percent": score} for score in scores]
        )
        self.assertEqual(result["matched_functions"], 1)
        self.assertEqual(result["matched_code"], "10")
        self.assertEqual(result["matched_code_percent"], 25)
        self.assertEqual(
            result["fuzzy_match_percent"], sum(score * 10 for score in scores) / 40
        )

    def test_report_excludes_runtime_targets(self):
        entities = []
        comparisons = ReccmpStatusReport("LEMBALL.EXE")
        catalog = {}
        modules_by_address = {
            0x501000: "game.obj",
            0x501010: r"build\intel\mt_obj\fflush.obj",
            0x501020: "game.obj",
        }
        for address, size in ((0x401000, 5), (0x401010, 7), (0x401020, 11)):
            recomp = address + 0x100000
            entities.append(
                ReccmpEntity(
                    address,
                    recomp,
                    {
                        "type": EntityType.FUNCTION,
                        "orig_size": size,
                        "recomp_size": size,
                    },
                )
            )
            catalog[address] = {"size": size}
            comparisons.add_match(
                ReccmpComparedEntity(
                    address, "Fixture", 1.0, EntityType.FUNCTION, recomp
                )
            )

        engine, modules = Mock(), Mock()
        engine.get_all.return_value = entities
        modules.get_module.side_effect = lambda module_address: (
            "",
            modules_by_address[module_address],
        )
        with (
            patch("make_report.csv_parse", return_value=catalog.items()),
            patch("make_report.read_report_exclusions", return_value={0x401000}),
        ):
            result = build_report(engine, comparisons, modules)

        self.assertEqual(result["measures"]["total_functions"], 1)
        self.assertEqual(result["measures"]["total_code"], "11")
        self.assertEqual(result["units"][0]["functions"][0]["name"], "0x00401020")

    def test_original_inventory_raw_scores_and_effective_badges(self):
        entities, comparisons = [], ReccmpStatusReport("LEMBALL.EXE")
        for address, size, kind, score, matched, flags in (
            (0x401000, 10, EntityType.FUNCTION, 1.0, True, {}),
            (
                0x401020,
                20,
                EntityType.FUNCTION,
                0.8,
                True,
                {"is_effective_match": True},
            ),
            (0x401040, 10, EntityType.FUNCTION, 1.0, True, {"is_stub": True}),
            (0x401060, 10, EntityType.FUNCTION, 0.0, False, {}),
            (0x401070, 8, EntityType.VTORDISP, 1.0, True, {}),
            (0x401080, 12, None, None, False, {}),
            (0x401090, 5, EntityType.THUNK, None, False, {}),
            (0x4010A0, 5, EntityType.IMPORT_THUNK, None, True, {}),
            (0x4010B0, 20, EntityType.FUNCTION, 0.5, True, {}),
        ):
            recomp = address + 0x100000 if matched else None
            entities.append(
                ReccmpEntity(
                    address,
                    recomp,
                    {
                        "type": kind,
                        "orig_size": size,
                        "recomp_size": 1000,
                    },
                )
            )
            if score is not None:
                comparisons.add_match(
                    ReccmpComparedEntity(
                        address,
                        "Fixture",
                        score,
                        EntityType.FUNCTION,
                        recomp,
                        **flags,
                    )
                )
        catalog = {
            entity.orig_addr: {"size": entity.size(ImageId.ORIG)} for entity in entities
        }
        entities.extend(
            (
                ReccmpEntity(
                    None, 0x502000, {"type": EntityType.FUNCTION, "recomp_size": 1000}
                ),
                ReccmpEntity(
                    0x403000, None, {"type": EntityType.DATA, "orig_size": 1000}
                ),
            )
        )
        engine, modules = Mock(), Mock()
        engine.get_all.return_value = list(reversed(entities))
        modules.get_module.side_effect = lambda module_address: (
            "",
            "Exact" if module_address == 0x501000 else "Mixed",
        )
        with (
            patch("make_report.csv_parse", return_value=catalog.items()),
            patch("make_report.read_report_exclusions", return_value=set()),
            patch("make_report.is_catalogued_jump_thunk", return_value=False),
        ):
            result = build_report(engine, comparisons, modules)
        totals = result["measures"]
        self.assertEqual(
            [unit["name"] for unit in result["units"]],
            ["Exact", "Mixed", "Unknown"],
        )
        for unit in result["units"]:
            addresses = [
                int(f["metadata"]["virtual_address"]) for f in unit["functions"]
            ]
            self.assertEqual(addresses, sorted(addresses))
        self.assertEqual((totals["total_functions"], totals["total_code"]), (8, "95"))
        self.assertEqual(
            (totals["matched_functions"], totals["matched_code"]), (2, "18")
        )
        self.assertAlmostEqual(totals["matched_code_percent"], 18 / 95 * 100)
        self.assertAlmostEqual(totals["fuzzy_match_percent"], 4400 / 95)
        scores = {
            int(f["metadata"]["virtual_address"]): f["fuzzy_match_percent"]
            for unit in result["units"]
            for f in unit["functions"]
        }
        self.assertEqual(
            scores,
            {
                0x401000: 100,
                0x401020: 80,
                0x401040: 0,
                0x401060: 0,
                0x401070: 100,
                0x401080: 0,
                0x401090: 0,
                0x4010B0: 50,
            },
        )
        for key in (
            "total_functions",
            "total_code",
            "matched_functions",
            "matched_code",
        ):
            self.assertEqual(
                sum(int(unit["measures"][key]) for unit in result["units"]),
                int(totals[key]),
            )
        unchanged = copy.deepcopy((result, comparisons.entities))
        self.assertAlmostEqual(
            effective_measures(
                result,
                effective_addresses(
                    comparisons.entities, {0x401000, 0x401070, 0x4010B0}
                ),
            )["matched_code_percent"],
            58 / 95 * 100,
        )
        self.assertEqual((result, comparisons.entities), unchanged)
