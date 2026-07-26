import importlib.util
import os
import tempfile
import time
import unittest
from pathlib import Path


SCRIPT = Path(__file__).resolve().parents[1] / "build_msvc420.py"
SPEC = importlib.util.spec_from_file_location("build_msvc420", SCRIPT)
build = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(build)


class BuildMsvc420Test(unittest.TestCase):
    def test_project_cmake_wins_over_caller_environment(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            cmake = root / ".decomp-venv" / "Scripts" / "cmake.exe"
            cmake.parent.mkdir(parents=True)
            cmake.write_bytes(b"")
            self.assertEqual(
                build.find_cmake(root, root / "other" / "python.exe"),
                cmake,
            )

    def test_build_rule_requires_matching_flags(self):
        with tempfile.TemporaryDirectory() as temporary:
            rule = Path(temporary) / "build.make"
            rule.write_text("CL.EXE /TP /O2 /Ob1 /Oy /G4 /Z7 /c /FoGAME.obj")
            self.assertTrue(build.valid_build_rule(rule))
            rule.write_text("CL.EXE /Od /GZ")
            self.assertFalse(build.valid_build_rule(rule))

    def test_stale_output(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            output = root / "LEMBALL.EXE"
            obj = root / "GAME.CPP.obj"
            output.write_bytes(b"exe")
            obj.write_bytes(b"obj")
            timestamp = time.time_ns()
            os.utime(output, ns=(timestamp, timestamp))
            os.utime(obj, ns=(timestamp + 2_000_000_000,) * 2)
            self.assertTrue(build.stale_output(output, [obj]))
            os.utime(output, ns=(timestamp + 4_000_000_000,) * 2)
            self.assertFalse(build.stale_output(output, [obj]))


if __name__ == "__main__":
    unittest.main()
