import importlib.util
import unittest
from pathlib import Path


SPEC = importlib.util.spec_from_file_location("coordinator", Path(__file__).resolve().parents[1] / "coordinator.py")
coordinator = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(coordinator)


class CoordinatorTest(unittest.TestCase):
    def test_ranks_small_unmatched_functions_first(self):
        functions = [
            {"address": "0x00401000", "size": "20"},
            {"address": "0x00401004", "size": "5"},
            {"address": "0x00401009", "size": "4"},
        ]
        ranked = coordinator.rank(functions, {"0x00401000": 0, "0x00401004": 100})
        self.assertEqual([row["address"] for row in ranked], ["0x00401009", "0x00401000"])


if __name__ == "__main__":
    unittest.main()
