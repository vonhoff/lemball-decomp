import importlib.util
import tempfile
import unittest
from contextlib import redirect_stdout
from io import StringIO
from pathlib import Path


SCRIPT = Path(__file__).resolve().parents[1] / "lint_reccmp_recommendations.py"
SPEC = importlib.util.spec_from_file_location("lint_reccmp_recommendations", SCRIPT)
linter = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(linter)


class RecommendationLintTest(unittest.TestCase):
    def test_member_and_vtable_coverage(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            source = root / "sample.cpp"
            source.write_text(
                "// VTABLE: TEST 0x1000\n"
                "// SIZE 0x8\n"
                "struct Sample {\n"
                "  virtual ~Sample() {}\n"
                "  virtual void Good(); // vtable+0x04\n"
                "  virtual void Missing();\n"
                "  int good; // 0x04\n"
                "  int missing;\n"
                "};\n"
                "DECOMP_SIZE_ASSERT(Sample, 0x8);\n"
            )
            with redirect_stdout(StringIO()) as output:
                failed = linter.lint(root)

        self.assertTrue(failed)
        self.assertIn("member has no offset", output.getvalue())
        self.assertIn("virtual method has no vtable offset", output.getvalue())


if __name__ == "__main__":
    unittest.main()
