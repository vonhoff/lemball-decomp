import importlib.util
import json
import tempfile
import unittest
from pathlib import Path


SCRIPT = Path(__file__).resolve().parents[1] / "generate_objdiff_report.py"
SPEC = importlib.util.spec_from_file_location("generate_objdiff_report", SCRIPT)
reporter = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(reporter)


class ObjdiffReportTest(unittest.TestCase):
    def test_full_inventory_and_native_matches(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            inventory = root / "functions.csv"
            inventory.write_text(
                "address,size,name,module\n"
                "0x00401000,4,exact,GAME.CPP\n"
                "0x00401004,6,stub,GAME.CPP\n"
                "0x0040100A,10,missing,LEVEL/LEVELRUN.CPP\n",
                encoding="utf-8",
            )
            native = root / "reccmp.json"
            native.write_text(
                json.dumps(
                    {
                        "data": [
                            {"address": "0x401000", "name": "exact", "matching": 1.0, "type": 1},
                            {"address": "0x401004", "name": "stub", "matching": 1.0, "stub": True, "type": 1},
                        ]
                    }
                ),
                encoding="utf-8",
            )

            report = reporter.build_report(inventory, native)

        self.assertEqual(report["version"], 2)
        self.assertEqual(report["measures"]["total_functions"], 3)
        self.assertEqual(report["measures"]["matched_functions"], 1)
        self.assertEqual(report["measures"]["total_code"], "20")
        self.assertEqual(len(report["units"]), 2)
        self.assertEqual(report["units"][0]["metadata"]["module_name"], "Game")
        self.assertNotIn("fuzzy_match_percent", report["units"][0]["functions"][1])


if __name__ == "__main__":
    unittest.main()
