"""The legacy build must not silently compare an executable from an older tree."""

import os
import subprocess
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import build


class LinkFreshnessTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.directory = Path(self.temporary.name)
        self.executable = self.directory / "LEMBALL.EXE"
        self.pdb = self.directory / "LEMBALL.pdb"
        self.object = self.directory / "CMakeFiles" / "LEMBALL.dir" / "src" / "test.obj"
        self.object.parent.mkdir(parents=True)
        for path in (self.executable, self.pdb, self.object):
            path.write_bytes(b"fixture")
            os.utime(path, ns=(10000000000, 10000000000))

    def run_build(self, side_effect):
        with patch.object(build.subprocess, "run", side_effect=side_effect) as run:
            result = build.build_with_link_check(["cmake", "--build"], self.directory, self.directory)
        return result, run.call_count

    def success(self):
        return subprocess.CompletedProcess([], 0, "build output\n")

    def test_new_object_forces_one_relink(self):
        os.utime(self.object, ns=(20000000000, 20000000000))
        calls = []
        def invoke(*args, **kwargs):
            calls.append(None)
            if len(calls) == 2:
                self.assertFalse(self.executable.exists())
                self.executable.write_bytes(b"new linked image")
                os.utime(self.executable, ns=(30000000000, 30000000000))
            return self.success()
        result, count = self.run_build(invoke)
        self.assertEqual(result[0], 0)
        self.assertEqual(count, 2)
        self.assertIn("forcing one relink", result[1])

    def test_missing_pdb_is_failure(self):
        self.pdb.unlink()
        result, calls = self.run_build(lambda *args, **kwargs: self.success())
        self.assertEqual(result[0], 1)
        self.assertEqual(calls, 1)

    def test_failed_build_does_not_delete_existing_executable(self):
        os.utime(self.object, ns=(20000000000, 20000000000))
        result, calls = self.run_build(lambda *args, **kwargs: subprocess.CompletedProcess([], 7, "error"))
        self.assertEqual(result[0], 7)
        self.assertEqual(calls, 1)
        self.assertTrue(self.executable.exists())

    def test_stale_retry_is_failure_not_an_infinite_loop(self):
        os.utime(self.object, ns=(20000000000, 20000000000))
        def invoke(*args, **kwargs):
            self.executable.write_bytes(b"still old")
            os.utime(self.executable, ns=(10000000000, 10000000000))
            return self.success()
        result, calls = self.run_build(invoke)
        self.assertEqual(result[0], 1)
        self.assertEqual(calls, 2)
        self.assertIn("still older", result[1])


class LogFilteringTests(unittest.TestCase):
    def test_compilation_progress_with_error_or_failed_ignored(self):
        ignored = [
            "[ 74%] Building CXX object CMakeFiles/LEMBALL.dir/src/Visos/Messaging/CMessFAILEDConnect.cpp.obj",
            "[ 95%] Building CXX object CMakeFiles/LEMBALL.dir/src/Visos/Target/Graphics/DirectDrawError.cpp.obj",
            "[ 50%] Building CXX object CMakeFiles/LEMBALL.dir/src/Error.cpp.obj",
            "[  1%] Building C object CMakeFiles/LEMBALL.dir/src/failed.c.obj",
            "[10/50] Compiling CXX object CMakeFiles/LEMBALL.dir/src/Error.cpp.obj",
        ]
        for line in ignored:
            with self.subTest(line=line):
                self.assertFalse(build.is_line_of_interest(line))

    def test_bare_compiler_filename_banner_ignored(self):
        ignored = [
            "CMessFAILEDConnect.cpp",
            "DirectDrawError.cpp",
            "Error.cpp",
            "failed.c",
            "LEMBALL.RC",
        ]
        for line in ignored:
            with self.subTest(line=line):
                self.assertFalse(build.is_line_of_interest(line))

    def test_milestones_retained(self):
        retained = [
            "[100%] Linking CXX executable LEMBALL.EXE",
            "[100%] Built target LEMBALL",
            "Link output is stale; forcing one relink after LEMBALL.dir/src/test.obj",
        ]
        for line in retained:
            with self.subTest(line=line):
                self.assertTrue(build.is_line_of_interest(line))

    def test_diagnostics_and_failures_retained(self):
        retained = [
            "src/Visos/Target/Graphics/DirectDrawError.cpp(42) : error C2065: 'foo' : undeclared identifier",
            "LINK : fatal error LNK1181: cannot open input file 'foo.lib'",
            "NMAKE : fatal error U1077: 'cl' : return code '0x2'",
            "FAILED: CMakeFiles/LEMBALL.dir/src/foo.cpp.obj",
            "ninja: build stopped: subcommand failed.",
            "CMake Error at CMakeLists.txt:10 (message):",
            "CMake Warning at CMakeLists.txt:12 (message):",
            "Command line warning D4025 : overriding '/O2' with '/Od'",
            "error: build did not produce both LEMBALL.EXE and LEMBALL.pdb",
        ]
        for line in retained:
            with self.subTest(line=line):
                self.assertTrue(build.is_line_of_interest(line))

