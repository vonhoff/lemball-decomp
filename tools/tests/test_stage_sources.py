"""Compiler identity preservation across source moves."""

import json
import tempfile
import unittest
from pathlib import Path

from stage_sources import stage


class StageSourcesTests(unittest.TestCase):
    def test_include_spelling_order_and_incremental_updates(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            source = root / "src/Current"
            source.mkdir(parents=True)
            (root / "cmake").mkdir()
            unit = source / "Widget.cpp"
            unit.write_bytes(b'#include "Widget.h"\r\n#include "Current/Widget.h"\r\nint value;\r\n')
            (source / "Widget.h").write_bytes(b"struct Widget {};\r\n")
            manifest = {
                "Old/Widget.cpp": {
                    "source": "Current/Widget.cpp",
                    "includes": {"Old/Widget.h": ["Widget.h", "Old/Widget.h"]},
                },
                "Old/Widget.h": {"source": "Current/Widget.h"},
            }
            (root / "cmake/source-layout.json").write_text(json.dumps(manifest))
            build = root / "build"
            stage(root, build)
            output = build / "compiler-src/Old/Widget.cpp"
            self.assertEqual(
                output.read_bytes(),
                b'#include "Widget.h"\r\n#include "Old/Widget.h"\r\nint value;\r\n',
            )
            timestamp = output.stat().st_mtime_ns
            stage(root, build)
            self.assertEqual(output.stat().st_mtime_ns, timestamp)
            unit.write_bytes(unit.read_bytes().replace(b"int value", b"int changed"))
            stage(root, build)
            self.assertIn(b"int changed", output.read_bytes())
            self.assertIn("compiler-src/Old/Widget.cpp", (build / "compiler-sources.cmake").read_text())

    def test_unlisted_source_rejected(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root / "src").mkdir()
            (root / "cmake").mkdir()
            (root / "src/New.cpp").write_text("int value;")
            (root / "cmake/source-layout.json").write_text("{}")
            with self.assertRaisesRegex(ValueError, "Source manifest differs"):
                stage(root, root / "build")


if __name__ == "__main__":
    unittest.main()
