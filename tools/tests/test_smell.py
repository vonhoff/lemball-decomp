"""Smell and annotation gate."""

import contextlib
import io
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from lib import mask_comments_and_strings
from lib.smell import check_smell, scan_file, unannotated_definitions


class SmellTests(unittest.TestCase):
    @staticmethod
    def scan(text, filename="Fixture.cpp"):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp).resolve()
            source = root / filename
            source.write_text(text, encoding="utf-8")
            with patch("lib.smell.ROOT", root):
                return scan_file(source)

    def test_offset_rules_preserve_specific_hits_and_buffer_exceptions(self):
        text = (
            'auto member = (char*) owner + 0x10;\n'
            'auto buffer = (char*) p_bits + 0x10;\n'
            'auto bytes = (char*) owner + sizeof(Widget);\n'
            'auto indexed = ((int*) owner)[0x58 / 4];\n'
            'auto offsets = (char*) first + offset + (char*) second + offset;\n'
        )
        hits = self.scan(text, "Fixture.h")
        self.assertEqual([(line, rule) for _, line, rule, _ in hits],
                         [(1, 'expr-char-offset'), (4, 'type-erase-index'),
                          (5, 'offset-poke'), (5, 'offset-poke')])

    def test_masking_and_nested_parentheses(self):
        text = (
            'const char* text = "(char*) this - 0x10";\n'
            '/*\nvoid Fake() {}\n(char*) this - 0x10;\n*/\n'
            'void Prototype(void (*callback)(int));\n'
            '// FUNCTION: LEMBALL 0x00401000\n'
            'void Annotated(void (*callback)(int)) {}\n'
            'void Missing(void (*callback)(int)) {}\n'
            'auto value = *((Widget*) (owner + offset)); // trailing comment\n'
            'auto broken = *((Widget*) (owner + offset);\n'
        )
        hits = self.scan(text)
        self.assertEqual([(line, rule) for _, line, rule, _ in hits],
                         [(10, 'cast-deref-offset'), (9, 'no-annotation')])
        self.assertEqual(hits[0][3], 'auto value = *((Widget*) (owner + offset));')

    def test_annotation_comment_block_boundaries(self):
        text = ('// FUNCTION: LEMBALL 0x00401000\n// Label\n\nvoid Annotated() {}\n'
                '// FUNCTION: LEMBALL 0x00401010\n\n// Separate block\nvoid Missing() {}')
        self.assertEqual(
            list(unannotated_definitions(text.splitlines(), mask_comments_and_strings(text))),
            [(8, 'void Missing() {}')],
        )

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
