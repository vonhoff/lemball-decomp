"""Source checks reject source-policy violations and catalog mismatches."""

import contextlib
import io
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import check_source
from check_source import check_names, check_policy, read_catalog


class CheckSourceTests(unittest.TestCase):
    def test_source_failures_reach_exit_status(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "Fixture.cpp"
            for owner, body, expected in (
                ("CPadToButton", "", 0),
                ("CPadToButton", "*(int*)((char*)this + 0x34) = n;", 1),
                ("CFake", "", 1),
            ):
                path.write_text(
                    f"// FUNCTION: LEMBALL 0x0043a250\n"
                    f"{owner}::{owner}(int n) {{ {body} }}\n",
                    encoding="utf-8",
                )
                with (
                    self.subTest(owner=owner, body=body),
                    patch("sys.argv", ["check_source.py", "--path", str(path)]),
                    contextlib.redirect_stdout(io.StringIO()),
                ):
                    self.assertEqual(check_source.main(), expected)

    def test_source_extensions_and_diagnostic_lines(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            for suffix in (".cpp", ".h", ".inl", ".RC"):
                path = root / f"Fixture{suffix}"
                path.write_text('\n"__asm";\n__asm nop;\n', encoding="utf-8")
            with contextlib.redirect_stdout(io.StringIO()) as output:
                self.assertEqual(check_policy([root]), 1)
            self.assertEqual(output.getvalue().count(":3: assembly:"), 4)

    def test_signature_review_details_do_not_fail_gate(self):
        path = Path(self.enterContext(tempfile.TemporaryDirectory())) / "Fixture.cpp"
        path.write_text(
            "// FUNCTION: LEMBALL 0x0043a250\nCPadToButton::CPadToButton(short n) {}",
            encoding="utf-8",
        )
        for verbose in (False, True):
            output = io.StringIO()
            with contextlib.redirect_stdout(output):
                self.assertEqual(check_names([path], verbose=verbose), 0)
            self.assertIn(
                "signature review requires Windows evidence", output.getvalue()
            )
            self.assertEqual(
                "CPadToButton::CPadToButton(short)" in output.getvalue(), verbose
            )


class CatalogTests(unittest.TestCase):
    def test_catalog_preserves_variants_and_unmapped_symbols(self):
        header = "mac_address,symbol,windows_address\n"
        row = "1060000c,Real__Fv,401000\n"
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "mac-symbol-catalog.csv"
            path.write_text(
                header + row + "1060000c,Real__Fv,402000\n10600020,MacOnly__Fv,\n",
                encoding="utf-8",
            )
            self.assertEqual(
                read_catalog(path),
                (
                    {0x1060000C: "Real__Fv", 0x10600020: "MacOnly__Fv"},
                    {0x401000: [0x1060000C], 0x402000: [0x1060000C]},
                ),
            )
