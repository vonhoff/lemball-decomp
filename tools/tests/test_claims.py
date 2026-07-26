import importlib.util
import json
import os
import tempfile
import time
import unittest
from contextlib import redirect_stdout
from io import StringIO
from pathlib import Path


SCRIPT = Path(__file__).resolve().parents[1] / "claims.py"
SPEC = importlib.util.spec_from_file_location("claims", SCRIPT)
claims = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(claims)


class ClaimsTest(unittest.TestCase):
    def setUp(self):
        self.redirect = redirect_stdout(StringIO())
        self.redirect.__enter__()
        self.addCleanup(self.redirect.__exit__, None, None, None)

    def make_project(self):
        temporary = tempfile.TemporaryDirectory()
        root = Path(temporary.name)
        (root / "data").mkdir()
        (root / "claims" / "abi").mkdir(parents=True)
        (root / "data" / "function-status").mkdir()
        (root / "data" / "dependencies").mkdir()
        (root / "build-msvc420").mkdir()
        (root / "data" / "objdiff-functions.csv").write_text(
            "address,size,name,unit\n"
            "0x00401000,4,first,GAME.CPP\n"
            "0x00401004,6,second,GAME.CPP\n"
            "0x0040100A,10,third,LEVEL/LEVELRUN.CPP\n",
            encoding="utf-8",
        )
        self.write_matches(root, {"0x00401000": 50, "0x00401004": 100})
        ranges = [
            {
                "id": "text-001",
                "start": "0x00401000",
                "end": "0x0040100A",
                "function_count": 2,
                "code_bytes": 10,
            },
            {
                "id": "text-002",
                "start": "0x0040100A",
                "end": "0x00401014",
                "function_count": 1,
                "code_bytes": 10,
            },
        ]
        claims.write_csv(
            root / "data" / "work-ranges.csv",
            ["id", "start", "end", "function_count", "code_bytes"],
            ranges,
        )
        (root / "CLAIMS.md").write_text(
            "# Active claims\n\n"
            "| Range | Addresses | Functions | Who | Claimed |\n"
            "|---|---|---:|---|---|\n",
            encoding="utf-8",
        )
        return temporary, root

    def write_matches(self, root, matches):
        data = [
            {
                "address": function_address,
                "matching": ratio / 100,
                "type": 1,
            }
            for function_address, ratio in matches.items()
        ]
        (root / "build-msvc420" / "reccmp.json").write_text(
            json.dumps({"data": data}),
            encoding="utf-8",
        )

    def test_range_catalog_validation(self):
        temporary, root = self.make_project()
        with temporary:
            ranges = claims.read_ranges(root / "data" / "work-ranges.csv")
            self.assertEqual([row["function_count_int"] for row in ranges], [2, 1])
            self.assertEqual(ranges[0]["end"], "0x0040100A")
            claims.validate(root)

    def test_function_annotation_scope(self):
        lines = [
            "// GLOBAL: LEMBALL 0x00490000",
            "int value;",
            "// FUNCTION: LEMBALL 0x00401000",
            "void Function() {}",
            "// VTABLE: LEMBALL 0x00491000",
        ]
        self.assertEqual(claims.annotation_at(lines, 3), "0x00401000")
        self.assertIsNone(claims.annotation_at(lines, 4))

    def test_assembly_patterns_are_forbidden(self):
        self.assertIsNotNone(claims.ASSEMBLY.search("__asm jmp target"))
        self.assertIsNotNone(claims.ASSEMBLY.search("__declspec(naked) void f()"))
        self.assertIsNone(claims.ASSEMBLY.search("return target();"))

    def test_source_states_distinguish_work_already_present(self):
        temporary, root = self.make_project()
        with temporary:
            (root / "src").mkdir()
            (root / "src" / "sample.CPP").write_text(
                "// FUNCTION: LEMBALL 0x00401000\n"
                "// STUB: LEMBALL 0x00401004\n",
                encoding="utf-8",
            )
            self.assertEqual(
                claims.source_states(root),
                {
                    "0x00401000": "function",
                    "0x00401004": "stub",
                },
            )

    def test_one_code_claim_per_owner(self):
        temporary, root = self.make_project()
        with temporary:
            claims.take(root, "text-001", "worker")
            with self.assertRaises(SystemExit):
                claims.take(root, "text-002", "worker")

    def test_status_must_belong_to_claimed_range(self):
        temporary, root = self.make_project()
        with temporary:
            claims.take(root, "text-001", "worker")
            claims.set_status(
                root, "text-001", "worker", "0x00401000", "surveyed", "checked"
            )
            status = claims.read_status(
                root / "data" / "function-status" / "text-001.csv"
            )
            self.assertEqual(status["0x00401000"]["state"], "surveyed")
            self.assertEqual(status["0x00401000"]["baseline_percent"], "50.000000")
            self.assertEqual(status["0x00401000"]["match_percent"], "50.000000")
            with self.assertRaises(SystemExit):
                claims.set_status(
                    root, "text-001", "worker", "0x0040100A", "surveyed", ""
                )

    def test_claim_is_visible_in_markdown(self):
        temporary, root = self.make_project()
        with temporary:
            claims.take(root, "text-001", "worker")
            active = claims.read_code_claims(root / "CLAIMS.md")[0]
            self.assertEqual(active["owner"], "worker")
            claims.validate(root)
            claims.remove_code_claim(root / "CLAIMS.md", "text-001", "worker")
            self.assertEqual(claims.read_code_claims(root / "CLAIMS.md"), [])

    def test_verify_requires_new_exact_without_regression(self):
        temporary, root = self.make_project()
        with temporary:
            claims.take(root, "text-001", "worker")
            self.write_matches(root, {"0x00401000": 100, "0x00401004": 100})
            claims.set_status(
                root, "text-001", "worker", "0x00401000", "matched", ""
            )
            claims.verify_scope(root, "text-001", "worker")

            self.write_matches(root, {"0x00401000": 40, "0x00401004": 100})
            with self.assertRaises(SystemExit):
                claims.verify_scope(root, "text-001", "worker")

    def test_matched_state_requires_exact_reccmp_result(self):
        temporary, root = self.make_project()
        with temporary:
            claims.take(root, "text-001", "worker")
            with self.assertRaises(SystemExit):
                claims.set_status(
                    root, "text-001", "worker", "0x00401000", "matched", ""
                )

    def test_cross_range_dependency_is_resumable(self):
        temporary, root = self.make_project()
        with temporary:
            claims.take(root, "text-001", "worker")
            claims.set_dependency(
                root,
                "text-001",
                "worker",
                "0x00401000",
                "0x0040100A",
                "open",
                "callee signature blocks exact call",
            )
            dependencies = claims.read_dependencies(
                root / "data" / "dependencies" / "text-001.csv"
            )
            row = dependencies[("0x00401000", "0x0040100A")]
            self.assertEqual(row["target_range"], "text-002")
            self.assertEqual(row["status"], "open")
            claims.set_dependency(
                root,
                "text-001",
                "worker",
                "0x00401000",
                "0x0040100A",
                "resolved",
                "merged target correction",
            )
            claims.validate(root)

    def test_comparison_rejects_stale_executable(self):
        temporary, root = self.make_project()
        with temporary:
            output = root / "build-msvc420" / "LEMBALL.EXE"
            obj = (
                root
                / "build-msvc420"
                / "CMakeFiles"
                / "LEMBALL.dir"
                / "GAME.CPP.obj"
            )
            obj.parent.mkdir(parents=True)
            output.write_bytes(b"exe")
            obj.write_bytes(b"obj")
            timestamp = time.time_ns()
            os.utime(output, ns=(timestamp, timestamp))
            os.utime(obj, ns=(timestamp + 2_000_000_000,) * 2)
            with self.assertRaises(SystemExit):
                claims.match_percentages(root)


if __name__ == "__main__":
    unittest.main()
