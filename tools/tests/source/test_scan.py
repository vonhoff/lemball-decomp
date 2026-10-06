"""Source discovery and offset-preserving lexical masking."""

import tempfile
import unittest
from pathlib import Path

from lib.source.scan import collect_sources, mask_comments_and_strings


class SourceScanTests(unittest.TestCase):
    def test_source_discovery_includes_files_added_between_checks(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            source = root / "Fixture.cpp"
            source.write_text("", encoding="utf-8")
            self.assertEqual(set(collect_sources([root])), {source})
            header = root / "Fixture.h"
            header.write_text("", encoding="utf-8")
            self.assertEqual(set(collect_sources([root])), {source, header})

    def test_masking_preserves_offsets_and_line_breaks(self):
        text = "int x; // comment\n\"// literal\"; /* block\ncomment */ char c = '/';\n"
        masked = mask_comments_and_strings(text)
        self.assertEqual(len(masked), len(text))
        self.assertEqual(
            [i for i, char in enumerate(masked) if char == "\n"],
            [i for i, char in enumerate(text) if char == "\n"],
        )
        self.assertEqual(masked[:6], "int x;")
        self.assertIn("char c =", masked)
        for token in ("comment", "literal", '"', "'", "//", "/*"):
            self.assertNotIn(token, masked)
