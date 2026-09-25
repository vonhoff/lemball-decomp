"""The source comment gate keeps functional annotations only."""

import unittest
from pathlib import Path

from lib.comments import invalid_comments


class CommentGateTests(unittest.TestCase):
    def test_documented_annotations_and_layout_notes(self):
        source = (
            "// FUNCTION: LEMBALL 0x00401000 FOLDED\n"
            "// GLOBAL: LEMBALL 0x00402000\n"
            "// CThing::`vbtable'\n"
            "// VTABLE: LEMBALL 0x00403000 CBase's `CSub\n"
            "// SIZE 0x20\nint member; // 0x04\n"
            "virtual void Draw(); // vtable+0x08\n"
            "// clang-format off\n// clang-format on\n"
        )
        self.assertEqual(list(invalid_comments(Path("Fixture.cpp"), source)), [])

    def test_prose_and_malformed_annotations_are_rejected(self):
        source = (
            'const char* text = "// not a comment";\n'
            "// FUNCTION: LEMBALL 0x00401000 extra words\n"
            "// Explanation of the function\n"
            "int value; // explanatory note\n"
        )
        self.assertEqual(len(list(invalid_comments(Path("Fixture.cpp"), source))), 3)
