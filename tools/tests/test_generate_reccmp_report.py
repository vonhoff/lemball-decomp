from __future__ import annotations

import importlib.util
import json
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

from reccmp.types import EntityType


SCRIPT = Path(__file__).resolve().parents[1] / "generate_reccmp_report.py"
SPEC = importlib.util.spec_from_file_location("generate_reccmp_report", SCRIPT)
assert SPEC is not None and SPEC.loader is not None
reporter = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(reporter)

INVENTORY_SCRIPT = Path(__file__).resolve().parents[1] / "function_inventory.py"
INVENTORY_SPEC = importlib.util.spec_from_file_location(
    "function_inventory_under_test", INVENTORY_SCRIPT
)
assert INVENTORY_SPEC is not None and INVENTORY_SPEC.loader is not None
inventory = importlib.util.module_from_spec(INVENTORY_SPEC)
INVENTORY_SPEC.loader.exec_module(inventory)


def entity(
    address: int,
    name: str,
    accuracy: float,
    *,
    stub: bool = False,
    effective: bool = False,
) -> SimpleNamespace:
    return SimpleNamespace(
        orig_addr=f"0x{address:x}",
        name=name,
        accuracy=accuracy,
        type=EntityType.FUNCTION,
        is_stub=stub,
        is_effective_match=effective,
    )


class ObjdiffReportTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory()
        self.root = Path(self.temporary.name)
        self.manifest = self.root / "manifest.json"
        self.reccmp = self.root / "reccmp.json"
        self.roadmap = self.root / "roadmap.csv"
        self.source_root = self.root / "src"
        self.runtime_symbols = self.root / "runtime-symbols.csv"
        self.source_root.mkdir()
        self.reccmp.write_text("{}", encoding="utf-8")
        self.runtime_symbols.write_text(
            "address,name,symbol,type,manifest_category\n", encoding="utf-8"
        )
        self.roadmap.write_text(
            "row_type,orig_addr,recomp_addr,module,name\n"
            "fun,0x401000,0x501000,CMakeFiles/LEMBALL.dir/src/A.CPP.obj,Exact\n",
            encoding="utf-8",
        )

    def tearDown(self) -> None:
        self.temporary.cleanup()

    def test_roadmap_reader_preserves_unquoted_commas_in_name(self) -> None:
        self.roadmap.write_text(
            "orig_sect_ofs,recomp_sect_ofs,orig_addr,recomp_addr,displacement,"
            "row_type,size,name,module\n"
            "0001:1,0001:2,0x471ba0,0x42e880,-1,vto,0x8,"
            "Body`vtordisp{-4, 0}',CMakeFiles/LEMBALL.dir/src/ENGINE/NET/VSNETTCP.CPP.obj\n",
            encoding="utf-8",
        )
        rows = inventory.read_roadmap_rows(self.roadmap)
        self.assertEqual(rows[0]["name"], "Body`vtordisp{-4, 0}'")
        self.assertEqual(
            rows[0]["module"],
            "CMakeFiles/LEMBALL.dir/src/ENGINE/NET/VSNETTCP.CPP.obj",
        )

    def test_network_ownership_includes_address_range_and_source_markers(self) -> None:
        self.assertTrue(inventory.network_owned(0x00470000, None, []))
        self.assertTrue(
            inventory.network_owned(
                0x00401000,
                None,
                [{"path": Path("src/ENGINE/NET/A.CPP")}],
            )
        )
        self.assertFalse(inventory.network_owned(0x00401000, None, []))

    def test_unimplemented_thunk_inherits_proven_target_module(self) -> None:
        self.write_manifest(
            [
                {
                    "address": "00401000",
                    "name": "NetworkThunk",
                    "size": 5,
                    "category": "thunk",
                    "is_thunk": True,
                    "thunk_target": "00402000",
                },
                {
                    "address": "00402000",
                    "name": "NetworkBody",
                    "size": 10,
                    "category": "internal",
                },
            ]
        )
        self.roadmap.write_text(
            "orig_sect_ofs,recomp_sect_ofs,orig_addr,recomp_addr,displacement,"
            "row_type,size,name,module\n"
            "1:0,1:0,0x00402000,0x00502000,0,fun,10,NetworkBody,"
            "CMakeFiles/LEMBALL.dir/src/ENGINE/NET/OWNER.CPP.obj\n",
            encoding="utf-8",
        )
        native = SimpleNamespace(filename="LEMBALL.EXE", entities={})
        rows, unresolved = inventory.reconcile_inventory(
            self.manifest,
            self.reccmp,
            self.roadmap,
            self.source_root,
            self.runtime_symbols,
            native_report=native,
        )
        thunk = next(row for row in rows if row["address"] == "0x00401000")
        self.assertEqual(thunk["module"], "ENGINE/NET/OWNER.CPP")
        self.assertFalse(unresolved)

    def write_manifest(self, functions: list[dict[str, object]]) -> None:
        self.manifest.write_text(
            json.dumps(
                {
                    "version": "LEMBALL",
                    "program": "LEMBALL.EXE",
                    "functions": functions,
                }
            ),
            encoding="utf-8",
        )

    def test_raw_accuracy_and_manifest_denominator_drive_measures(self) -> None:
        self.write_manifest(
            [
                {"address": "00401000", "name": "Exact", "size": 10, "category": "internal"},
                {"address": "00401010", "name": "EffectiveOnly", "size": 20, "category": "internal"},
                {"address": "00401020", "name": "Stub", "size": 30, "category": "thunk"},
                {"address": "00401030", "name": "Unimplemented", "size": 40, "category": "internal"},
                {"address": "00402000", "name": "Runtime", "size": 50, "category": "runtime"},
                {"address": "00403000", "name": "Import", "size": 60, "category": "import"},
            ]
        )
        native = SimpleNamespace(
            filename="LEMBALL.EXE",
            entities={
                "exact": entity(0x401000, "Exact", 1.0),
                "effective": entity(0x401010, "EffectiveOnly", 0.5, effective=True),
                "stub": entity(0x401020, "Stub", 1.0, stub=True),
            },
        )
        with patch.object(reporter, "deserialize_reccmp_report", return_value=native):
            report = reporter.build_report(
                self.manifest,
                self.reccmp,
                self.roadmap,
                self.source_root,
                self.runtime_symbols,
            )

        measures = report["measures"]
        self.assertEqual(measures["total_functions"], 4)
        self.assertEqual(measures["matched_functions"], 1)
        self.assertEqual(measures["total_code"], "100")
        self.assertEqual(measures["matched_code"], "10")
        self.assertEqual(measures["matched_code_percent"], 10.0)
        self.assertEqual(measures["fuzzy_match_percent"], 20.0)
        self.assertNotIn("matched_data_percent", measures)
        self.assertNotIn("complete_data_percent", measures)
        self.assertNotIn("complete_units", measures)

        items = {
            item["name"]: item
            for unit in report["units"]
            for item in unit["functions"]
        }
        self.assertEqual(items["Exact"]["address"], "0")
        self.assertEqual(items["EffectiveOnly"]["fuzzy_match_percent"], 50.0)
        self.assertNotIn("fuzzy_match_percent", items["Stub"])
        self.assertNotIn("fuzzy_match_percent", items["Unimplemented"])

    def test_aggregate_fuzzy_measure_uses_serialized_function_float32_values(self) -> None:
        functions = [
            {"size": 17, "raw_accuracy": 0.3447037487106323},
            {"size": 31, "raw_accuracy": 0.7123456597328186},
        ]

        result = reporter.measures(functions)
        serialized_ratios = [
            reporter.objdiff_f32(float(function["raw_accuracy"]) * 100.0)
            for function in functions
        ]
        expected = reporter.objdiff_f32(
            sum(
                ratio * int(function["size"])
                for ratio, function in zip(serialized_ratios, functions)
            )
            / sum(int(function["size"]) for function in functions)
        )

        self.assertEqual(result["fuzzy_match_percent"], expected)

    def test_reccmp_function_outside_reportable_manifest_is_rejected(self) -> None:
        self.write_manifest(
            [{"address": "00401000", "name": "Exact", "size": 10, "category": "internal"}]
        )
        native = SimpleNamespace(
            filename="LEMBALL.EXE",
            entities={"outside": entity(0x401100, "Outside", 1.0)},
        )
        with patch.object(reporter, "deserialize_reccmp_report", return_value=native):
            with self.assertRaisesRegex(SystemExit, "missing_manifest_function"):
                reporter.build_report(
                    self.manifest,
                    self.reccmp,
                    self.roadmap,
                    self.source_root,
                    self.runtime_symbols,
                )

    def test_declared_runtime_and_import_entities_are_explicitly_excluded(self) -> None:
        self.write_manifest(
            [
                {"address": "00401000", "name": "Exact", "size": 10, "category": "internal"},
                {"address": "00402000", "name": "Runtime", "size": 20, "category": "runtime"},
                {"address": "00403000", "name": "Import", "size": 5, "category": "import"},
            ]
        )
        self.runtime_symbols.write_text(
            "address,name,symbol,type,manifest_category\n"
            "0x00402000,Runtime,_Runtime,library,runtime\n"
            "0x00403000,Import,_Import,library,import\n",
            encoding="utf-8",
        )
        native = SimpleNamespace(
            filename="LEMBALL.EXE",
            entities={
                "exact": entity(0x401000, "Exact", 1.0),
                "runtime": entity(0x402000, "Runtime", 1.0),
                "import": entity(0x403000, "Import", 1.0),
            },
        )
        with patch.object(reporter, "deserialize_reccmp_report", return_value=native):
            report = reporter.build_report(
                self.manifest,
                self.reccmp,
                self.roadmap,
                self.source_root,
                self.runtime_symbols,
            )
        self.assertEqual(report["measures"]["total_functions"], 1)
        self.assertEqual(report["measures"]["matched_functions"], 1)

    def test_excluded_entity_with_application_marker_is_rejected(self) -> None:
        self.write_manifest(
            [{"address": "00402000", "name": "Runtime", "size": 20, "category": "runtime"}]
        )
        self.runtime_symbols.write_text(
            "address,name,symbol,type,manifest_category\n"
            "0x00402000,Runtime,_Runtime,library,runtime\n",
            encoding="utf-8",
        )
        (self.source_root / "A.CPP").write_text(
            "// FUNCTION: LEMBALL 0x00402000\nvoid Runtime() {}\n",
            encoding="utf-8",
        )
        native = SimpleNamespace(
            filename="LEMBALL.EXE",
            entities={"runtime": entity(0x402000, "Runtime", 1.0)},
        )
        with patch.object(reporter, "deserialize_reccmp_report", return_value=native):
            with self.assertRaisesRegex(SystemExit, "excluded_entity_has_application_marker"):
                reporter.build_report(
                    self.manifest,
                    self.reccmp,
                    self.roadmap,
                    self.source_root,
                    self.runtime_symbols,
                )


if __name__ == "__main__":
    unittest.main()
