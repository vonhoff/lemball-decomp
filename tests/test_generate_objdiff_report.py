import importlib.util
import json
import tempfile
import unittest
from pathlib import Path

MODULE_PATH = Path(__file__).parents[1] / "tools" / "generate_objdiff_report.py"
SPEC = importlib.util.spec_from_file_location("generate_objdiff_report", MODULE_PATH)
MODULE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(MODULE)
build_report = MODULE.build_report


class GenerateObjdiffReportTest(unittest.TestCase):
    def test_emits_only_supported_objdiff_metadata(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            inventory = root / "inventory.csv"
            reccmp = root / "reccmp.json"
            correlations = root / "correlations.csv"
            inventory.write_text(
                "address,size,name,unit\n0x00401000,16,WindowsName,Control/Test.cpp\n",
                encoding="utf-8",
            )
            reccmp.write_text(
                json.dumps({"data": [{"address": "0x00401000", "matching": 1.0}]}),
                encoding="utf-8",
            )
            correlations.write_text(
                "x86_address,mac_code_file,mac_mangled_name\n"
                "0x00401000,CODE_1_Arena.bin,MacName__6CArenaF\n",
                encoding="utf-8",
            )

            report = build_report(inventory, reccmp, correlations)
            unit = report["units"][0]
            function = unit["functions"][0]

            self.assertEqual(unit["metadata"], {"module_name": "Arena"})
            self.assertEqual(function["metadata"], {"virtual_address": str(0x00401000)})

    def test_distinguishes_uncorrelated_from_compiler_generated_functions(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            inventory = root / "inventory.csv"
            reccmp = root / "reccmp.json"
            correlations = root / "correlations.csv"
            compiler_generated = root / "compiler-generated.csv"
            inventory.write_text(
                "address,size,name,unit\n"
                "0x00401000,16,UncorrelatedName,Control/Test.cpp\n"
                "0x00401020,8,GeneratedName,Control/Test.cpp\n",
                encoding="utf-8",
            )
            reccmp.write_text(json.dumps({"data": []}), encoding="utf-8")
            correlations.write_text(
                "x86_address,mac_code_file,mac_mangled_name\n", encoding="utf-8"
            )
            compiler_generated.write_text(
                "# Compiler-generated functions without standalone source implementations.\n"
                "address,name,symbol,type,source\n"
                "0x00401020,GeneratedName,,synthetic,src/Control/Test.cpp\n",
                encoding="utf-8",
            )

            report = build_report(inventory, reccmp, correlations, compiler_generated)

            self.assertEqual(
                [unit["name"] for unit in report["units"]],
                ["Windows/Compiler-generated", "Windows/Uncorrelated"],
            )


if __name__ == "__main__":
    unittest.main()
