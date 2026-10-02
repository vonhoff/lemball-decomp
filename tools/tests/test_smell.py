"""Smell and annotation gate."""

import contextlib
import io
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from lib.smell import check_smell, scan_file


class SmellTests(unittest.TestCase):
    def test_smell_gate(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp).resolve()
            source = root / 'src/Fixture.cpp'
            source.parent.mkdir()
            source.write_text('void Prototype(\n int arg\n);\n'
                              '// FUNCTION: LEMBALL 0x00401000\nvoid Annotated(int arg) {}\n'
                              'void Missing(\n int arg\n) { return; }\n'
                              'void* owner = (char*) this - 0x10;\n')
            with patch('lib.smell.ROOT', root):
                hits = scan_file(source)
                self.assertEqual([h[2] for h in hits].count('no-annotation'), 1)
                self.assertIn('this-adjust-poke', [h[2] for h in hits])
                with contextlib.redirect_stderr(io.StringIO()), contextlib.redirect_stdout(io.StringIO()):
                    self.assertEqual(check_smell([source]), 1)
                    source.write_text('// FUNCTION: LEMBALL 0x00401000\nvoid Annotated(int arg) {}')
                    self.assertEqual(check_smell([source]), 0)
