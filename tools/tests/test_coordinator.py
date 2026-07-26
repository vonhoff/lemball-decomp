import importlib.util
import os
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch


SCRIPT = Path(__file__).resolve().parents[1] / "coordinator.py"
SPEC = importlib.util.spec_from_file_location("coordinator", SCRIPT)
coordinator = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(coordinator)


class CoordinatorTest(unittest.TestCase):
    def test_reads_configured_toolchain_and_writes_target(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            tools = root / "tools"
            (root / coordinator.TOOLCHAIN_CONFIG).write_text(str(tools), encoding="utf-8")
            with patch.dict(os.environ, {"LEMBALL_TOOLCHAIN_ROOT": ""}):
                self.assertEqual(coordinator.toolchain_root(root), tools)
            coordinator.write_reccmp_user(root, tools)
            self.assertIn(str(tools / "LEMBALL.EXE"), (root / "reccmp-user.yml").read_text())


if __name__ == "__main__":
    unittest.main()
