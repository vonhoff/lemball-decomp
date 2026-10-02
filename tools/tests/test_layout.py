"""Primary class selection follows type ownership and vtable evidence."""

import unittest
from pathlib import Path

from lib import mask_comments_and_strings
from lib.layout import primary_names


class LayoutTests(unittest.TestCase):
    def test_primary_class_selection(self):
        for filename, source, expected in (
            ('Outer.h', 'class Outer { struct Inner {}; }; struct Helper {};', ['Outer']),
            ('Outer.h', '// VTABLE: LEMBALL 0x00401000\n'
             'struct Owner {}; class Other {};', ['Owner']),
            ('Record.h', 'struct Record {}; struct Helper {};', ['Record']),
            ('Free.h', 'struct First {}; struct Second {};', ['First', 'Second']),
            ('Broken.h', 'class Unclosed { class Complete {};', ['Complete']),
            ('Outer.cpp', 'void Outer::Inner::Run() {}\n'
             'void outer::Other() {}\n  Other::Call();', ['Outer']),
        ):
            with self.subTest(filename=filename, source=source):
                self.assertEqual(primary_names(Path(filename), source,
                                               mask_comments_and_strings(source)), expected)
