import csv
import importlib.util
import json
import tempfile
import unittest
from contextlib import redirect_stdout
from io import StringIO
from pathlib import Path


SCRIPT = Path(__file__).resolve().parents[1] / "claims.py"
SPEC = importlib.util.spec_from_file_location("claims", SCRIPT)
claims = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(claims)


class ClaimsTest(unittest.TestCase):
    def make_project(self):
        temporary = tempfile.TemporaryDirectory()
        root = Path(temporary.name)
        (root / "build-msvc420").mkdir()
        (root / "data" / "function-status").mkdir(parents=True)
        (root / "CLAIMS.md").write_text(
            "# Active claims\n\n"
            "| Range | Addresses | Functions | Who | Claimed |\n"
            "|---|---|---:|---|---|\n",
            encoding="utf-8",
        )
        self.write_csv(
            root / "data" / "work-ranges.csv",
            ["id", "start", "end", "function_count", "code_bytes"],
            [
                ["text-001", "0x00401000", "0x00401008", "2", "8"],
                ["text-002", "0x00401008", "0x0040100C", "1", "4"],
            ],
        )
        self.write_csv(
            root / "data" / "objdiff-functions.csv",
            ["address", "size", "name", "unit"],
            [
                ["0x00401000", "4", "first", "GAME.CPP"],
                ["0x00401004", "4", "second", "GAME.CPP"],
                ["0x00401008", "4", "third", "LEVEL.CPP"],
            ],
        )
        self.write_csv(
            root / "data" / "function-status" / "text-002.csv",
            ["address", "notes"],
            [["0x00401008", "resume here"]],
        )
        (root / "build-msvc420" / "reccmp.json").write_text(
            json.dumps(
                {
                    "data": [
                        {"address": "0x401000", "matching": 1.0, "type": 1},
                        {"address": "0x401004", "matching": 1.0, "type": 1},
                        {"address": "0x401008", "matching": 0.5, "type": 1},
                    ]
                }
            ),
            encoding="utf-8",
        )
        return temporary, root

    @staticmethod
    def write_csv(path, fields, rows):
        with path.open("w", newline="", encoding="utf-8") as stream:
            writer = csv.writer(stream)
            writer.writerow(fields)
            writer.writerows(rows)

    def test_claims_first_unfinished_range_and_resumes_it(self):
        temporary, root = self.make_project()
        with temporary, redirect_stdout(StringIO()) as output:
            claims.start(root, "worker")
            claims.start(root, "worker")
            active = claims.claims(root / "CLAIMS.md")
            self.assertEqual(active[0]["range"], "text-002")
            self.assertEqual(active[0]["owner"], "worker")
            self.assertIn("resume here", output.getvalue())
            self.assertEqual(len(active), 1)

    def test_different_owners_get_disjoint_ranges(self):
        temporary, root = self.make_project()
        with temporary, redirect_stdout(StringIO()):
            (root / "build-msvc420" / "reccmp.json").write_text(
                json.dumps(
                    {
                        "data": [
                            {"address": "0x401000", "matching": 0.5, "type": 1},
                            {"address": "0x401004", "matching": 0.5, "type": 1},
                            {"address": "0x401008", "matching": 0.5, "type": 1},
                        ]
                    }
                ),
                encoding="utf-8",
            )
            claims.start(root, "first")
            claims.start(root, "second")
            active = claims.claims(root / "CLAIMS.md")
            self.assertEqual({row["range"] for row in active}, {"text-001", "text-002"})

    def test_check_and_release(self):
        temporary, root = self.make_project()
        with temporary, redirect_stdout(StringIO()):
            claims.start(root, "worker")
            claims.check(root)
            claims.release(root, "worker")
            self.assertEqual(claims.claims(root / "CLAIMS.md"), [])


if __name__ == "__main__":
    unittest.main()
