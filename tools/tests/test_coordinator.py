import csv
import importlib.util
import json
import tempfile
import unittest
from contextlib import redirect_stdout
from io import StringIO
from pathlib import Path


SCRIPT = Path(__file__).resolve().parents[1] / "coordinator.py"
SPEC = importlib.util.spec_from_file_location("coordinator", SCRIPT)
coordinator = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(coordinator)


class CoordinatorTest(unittest.TestCase):
    def test_prints_first_unfinished_range(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root / "data").mkdir()
            (root / "build-msvc420").mkdir()
            self.write(root / "data" / "work-ranges.csv", ["id", "start", "end"], [["first", "0x401000", "0x401008"], ["second", "0x401008", "0x40100C"]])
            self.write(root / "data" / "objdiff-functions.csv", ["address", "unit", "name"], [["0x00401000", "GAME.CPP", "done"], ["0x00401004", "GAME.CPP", "next"], ["0x00401008", "LEVEL.CPP", "later"]])
            (root / "build-msvc420" / "reccmp.json").write_text(json.dumps({"data": [{"address": "0x401000", "matching": 1, "type": 1}, {"address": "0x401004", "matching": 0.5, "type": 1}]}), encoding="utf-8")
            with redirect_stdout(StringIO()) as output:
                import sys
                old_argv = sys.argv
                sys.argv = ["coordinator.py", "--root", str(root)]
                try:
                    coordinator.main()
                finally:
                    sys.argv = old_argv
            self.assertIn("next first", output.getvalue())
            self.assertIn("0x00401004", output.getvalue())

    @staticmethod
    def write(path, fields, values):
        with path.open("w", newline="", encoding="utf-8") as stream:
            writer = csv.writer(stream)
            writer.writerow(fields)
            writer.writerows(values)


if __name__ == "__main__":
    unittest.main()
