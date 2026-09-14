"""The legacy build must not silently compare an executable from an older tree."""

import os
from pathlib import Path
import subprocess
import tempfile
import unittest
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

    def test_unchanged_build_does_not_relink(self):
        result, calls = self.run_build(lambda *args, **kwargs: self.success())
        self.assertEqual(result[0], 0)
        self.assertEqual(calls, 1)
        self.assertTrue(self.executable.exists())

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
