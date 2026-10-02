"""Build artifact freshness."""

import os
import subprocess
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import build


class BuildTests(unittest.TestCase):
    def test_link_freshness(self):
        for mode, expected, attempts in (('current', 0, 1), ('relink', 0, 2),
                                         ('stale', 1, 2), ('missing_pdb', 1, 1), ('failed', 7, 1)):
            with self.subTest(mode=mode), tempfile.TemporaryDirectory() as temp:
                directory = Path(temp)
                executable, pdb = directory / 'LEMBALL.EXE', directory / 'LEMBALL.pdb'
                obj = directory / 'CMakeFiles/LEMBALL.dir/test.obj'
                obj.parent.mkdir(parents=True)
                for path in (executable, pdb, obj):
                    path.write_bytes(b'fixture')
                    os.utime(path, ns=(10_000_000_000, 10_000_000_000))
                if mode in ('relink', 'stale', 'failed'):
                    os.utime(obj, ns=(20_000_000_000, 20_000_000_000))
                if mode == 'missing_pdb':
                    pdb.unlink()
                calls = 0

                def invoke(*_args, build_mode=mode, output=executable, **_kwargs):
                    nonlocal calls
                    calls += 1
                    if calls == 2:
                        self.assertFalse(output.exists())
                        output.write_bytes(b'linked')
                        timestamp = 30_000_000_000 if build_mode == 'relink' else 10_000_000_000
                        os.utime(output, ns=(timestamp, timestamp))
                    return subprocess.CompletedProcess([], 7 if build_mode == 'failed' else 0, 'build output')

                with patch.object(build.subprocess, 'run', side_effect=invoke):
                    code, _ = build.build_with_link_check(['cmake', '--build'], directory, directory)
                self.assertEqual((code, calls), (expected, attempts))
                if mode == 'failed':
                    self.assertEqual(executable.read_bytes(), b'fixture')
