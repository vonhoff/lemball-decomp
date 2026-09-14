"""Source annotations must invalidate cached comparisons and their inventory."""

import os
import tempfile
import unittest
from contextlib import ExitStack
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import Mock, patch

from lib import reccmp


class ReccmpCacheTests(unittest.TestCase):
    def test_source_edits_additions_removals_and_moves_refresh_both_outputs(self):
        with tempfile.TemporaryDirectory() as directory, ExitStack() as patches:
            root = Path(directory)
            source = root / "src"
            source.mkdir()
            header = source / "Unused.h"
            header.write_text("// SYNTHETIC: LEMBALL 0x0045e9d0\n", encoding="utf-8")
            build = root / "build"
            build.mkdir()
            for name, value in {
                "ROOT": root, "SRC": source, "BUILD": build,
                "RECCMP_JSON": build / "reccmp.json",
                "RECCMP_STAMP": build / "stamp.json",
                "ROADMAP_CSV": build / "roadmap.csv",
                "ORIGINAL_EXE": root / "original.exe",
                "RECOMP_EXE": build / "rebuilt.exe",
                "RECOMP_PDB": build / "rebuilt.pdb",
            }.items():
                patches.enter_context(patch.object(reccmp, name, value))
            for binary in (reccmp.ORIGINAL_EXE, reccmp.RECOMP_EXE, reccmp.RECOMP_PDB):
                binary.write_bytes(b"unchanged binary fixture")
            binaries = [reccmp.file_id(path) for path in
                        (reccmp.ORIGINAL_EXE, reccmp.RECOMP_EXE, reccmp.RECOMP_PDB)]
            target = SimpleNamespace(original_path=reccmp.ORIGINAL_EXE)
            engine = Mock()
            engine.compare_all.return_value = []
            loader = patches.enter_context(patch.object(reccmp, "_load_engine",
                                                        return_value=(target, engine)))
            patches.enter_context(patch.object(reccmp, "serialize_reccmp_report", return_value="{}"))
            roadmap = patches.enter_context(patch.object(
                reccmp, "_write_roadmap",
                side_effect=lambda target, engine, path: path.write_text("inventory", encoding="utf-8"),
            ))

            def check_generation(count):
                reccmp.run_reccmp(roadmap=True)
                self.assertEqual(loader.call_count, count)
                self.assertEqual(roadmap.call_count, count)
                self.assertEqual(binaries, [reccmp.file_id(path) for path in
                                           (reccmp.ORIGINAL_EXE, reccmp.RECOMP_EXE, reccmp.RECOMP_PDB)])

            check_generation(1)
            check_generation(1)  # Unchanged inputs still reuse both outputs.
            header.write_text("// Removed the unused annotation.\n", encoding="utf-8")
            check_generation(2)
            extra = source / "Added.h"
            extra.write_text("// FUNCTION: LEMBALL 0x00409d10\n", encoding="utf-8")
            check_generation(3)
            moved = source / "nested" / "Added.h"
            moved.parent.mkdir()
            extra.rename(moved)  # Same filename and metadata, new source path.
            check_generation(4)
            moved.unlink()
            check_generation(5)
            config = root / "reccmp-project.yml"
            config.write_text("targets: {}\n", encoding="utf-8")
            check_generation(6)
            config.write_text("targets: {LEMBALL: {}}\n", encoding="utf-8")
            check_generation(7)
            # Detection can rewrite identical configuration without changing its meaning.
            config.write_text("targets: {LEMBALL: {}}\n", encoding="utf-8")
            os.utime(config, ns=(1_000_000_000, 1_000_000_000))
            check_generation(7)


if __name__ == "__main__":
    unittest.main()
