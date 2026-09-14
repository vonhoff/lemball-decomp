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
    def test_input_changes_during_comparison_or_roadmap_are_not_cached(self):
        for phase in ("comparison", "roadmap"):
            with self.subTest(phase=phase), tempfile.TemporaryDirectory() as directory, ExitStack() as patches:
                build = Path(directory)
                for name, value in {
                    "BUILD": build,
                    "RECCMP_JSON": build / "reccmp.json",
                    "RECCMP_STAMP": build / "stamp.json",
                    "ROADMAP_CSV": build / "roadmap.csv",
                }.items():
                    patches.enter_context(patch.object(reccmp, name, value))
                generation = [1]
                patches.enter_context(patch.object(reccmp, "_stamp", side_effect=lambda: {"generation": generation[0]}))
                target = SimpleNamespace(original_path=build / "original.exe")
                engine = Mock()

                def compare():
                    if phase == "comparison":
                        generation[0] += 1
                    return []

                def write_roadmap(target, engine, path):
                    path.write_text("inventory", encoding="utf-8")
                    if phase == "roadmap":
                        generation[0] += 1

                engine.compare_all.side_effect = compare
                loader = patches.enter_context(patch.object(reccmp, "_load_engine", return_value=(target, engine)))
                patches.enter_context(patch.object(reccmp, "serialize_reccmp_report", return_value="{}"))
                roadmap = patches.enter_context(patch.object(reccmp, "_write_roadmap", side_effect=write_roadmap))
                with self.assertRaisesRegex(RuntimeError, "Comparison inputs changed"):
                    reccmp.run_reccmp(roadmap=True)
                self.assertFalse(reccmp.RECCMP_STAMP.exists())

                engine.compare_all.side_effect = None
                engine.compare_all.return_value = []
                roadmap.side_effect = lambda target, engine, path: path.write_text("current inventory", encoding="utf-8")
                reccmp.run_reccmp(roadmap=True)
                reccmp.run_reccmp(roadmap=True)
                self.assertEqual(loader.call_count, 2)
                self.assertEqual(roadmap.call_count, 2)
                self.assertEqual(reccmp.ROADMAP_CSV.read_text(encoding="utf-8"), "current inventory")

                # A comparison-only refresh must invalidate the previous inventory.
                generation[0] += 1
                reccmp.run_reccmp(roadmap=False)
                self.assertFalse(reccmp.ROADMAP_CSV.exists())
                reccmp.run_reccmp(roadmap=True)
                self.assertEqual(loader.call_count, 4)
                self.assertEqual(roadmap.call_count, 3)

    def test_source_edits_additions_removals_and_moves_refresh_both_outputs(self):
        with tempfile.TemporaryDirectory() as directory, ExitStack() as patches:
            root = Path(directory)
            implementation = root / "reccmp.py"
            compatibility = root / "reccmp_compat.py"
            implementation.write_text("# comparator\n", encoding="utf-8")
            compatibility.write_text("# parser\n", encoding="utf-8")
            patches.enter_context(patch.object(reccmp, "__file__", str(implementation)))
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

            compatibility.write_text("# corrected parser\n", encoding="utf-8")
            check_generation(8)
            implementation.write_text("# corrected comparator\n", encoding="utf-8")
            check_generation(9)
            check_generation(9)
            # Detection can rewrite identical configuration without changing its meaning.
            config.write_text("targets: {LEMBALL: {}}\n", encoding="utf-8")
            os.utime(config, ns=(1_000_000_000, 1_000_000_000))
            check_generation(9)


if __name__ == "__main__":
    unittest.main()
