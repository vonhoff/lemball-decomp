"""Source annotations compared against independent catalog evidence."""

import tempfile
import unittest
from pathlib import Path

from lib.source.names import scan


class CatalogNamingTests(unittest.TestCase):
    def setUp(self):
        self.path = (
            Path(self.enterContext(tempfile.TemporaryDirectory())) / "Fixture.cpp"
        )
        self.symbols = {0x10B0F952: "__ct__12CPadToButtonFi"}
        self.mappings = {0x43A250: [0x10B0F952]}

    def scan(self, source):
        self.path.write_text(source, encoding="utf-8")
        return list(scan(self.path, self.symbols, self.mappings))

    def test_nested_class_callback_and_unclosed_declaration(self):
        self.symbols[0x10B0F952] = "Read__Q25Outer5InnerCFPFPCc_i"
        (row,) = self.scan(
            "class Outer final : public Base {\nstruct Inner {\n"
            "// FUNCTION: LEMBALL 0x0043a250\n"
            "int Read(int (*callback)(const char* text)) const;\n};\n};\n"
        )
        self.assertEqual((row["status"], row["signature_status"]), ("match", "match"))
        self.assertEqual(
            row["actual_signature"], "Outer::Inner::Read(int (*)(const char*)) const"
        )
        (row,) = self.scan(
            "// FUNCTION: LEMBALL 0x0043a250\n"
            "int Read(int (*callback)(const char* text);"
        )
        self.assertEqual(row["status"], "unresolved")

    def test_callback_abi_difference_is_compared_not_suppressed(self):
        self.symbols[0x10B0F952] = "__ct__17CVSDebugStreambufFPciPFPc_Uc"
        (row,) = self.scan(
            "// FUNCTION: LEMBALL 0x0043a250\n"
            "CVSDebugStreambuf::CVSDebugStreambuf(char* buffer, int size, int (*callback)(char*)) {}"
        )
        self.assertEqual((row["status"], row["signature_status"]), ("match", "review"))
        self.assertIn("unsigned char (*)(char*)", row["original_signature"])
        self.assertIn("int (*)(char*)", row["actual_signature"])

    def test_changed_annotation_and_source_cannot_forge_expected_symbol(self):
        rows = self.scan(
            "// 68K 0x10b0f952 __ct__5CFakeFi\n// FUNCTION: LEMBALL 0x0043a250\nCFake::CFake(int n) {}"
        )
        self.assertEqual([r["status"] for r in rows], ["mismatch"])
        self.assertEqual(
            rows[-1]["original_signature"], "CPadToButton::CPadToButton(int)"
        )

    def test_parameter_names_ignored_but_arity_signedness_and_reference_checked(self):
        for parameters in ("unsigned int n", "int& n", "int n, int extra", ""):
            with self.subTest(parameters=parameters):
                (row,) = self.scan(
                    f"// FUNCTION: LEMBALL 0x0043a250\nCPadToButton::CPadToButton({parameters}) {{}}"
                )
                self.assertEqual(row["signature_status"], "review")
        self.symbols[0x10B0F952] = "Get__12CPadToButtonCFv"
        (row,) = self.scan(
            "// FUNCTION: LEMBALL 0x0043a250\nint CPadToButton::Get() {}"
        )
        self.assertEqual(row["signature_status"], "review")

    def test_fake_markers_in_strings_and_block_comments_are_ignored(self):
        rows = self.scan(
            'const char* s = "// FUNCTION: LEMBALL 0x0043a250";\n'
            "/* // FUNCTION: LEMBALL 0x0043a250 */\n"
        )
        self.assertEqual(rows, [])

    def test_folded_windows_entry_uses_matching_catalog_candidate(self):
        self.symbols[0x10100004] = "Wrong__6COtherFv"
        self.mappings[0x43A250].insert(0, 0x10100004)
        (row,) = self.scan(
            "// FUNCTION: LEMBALL 0x0043a250\nCPadToButton::CPadToButton(int n) {}"
        )
        self.assertEqual(row["status"], "match")
        self.assertEqual(row["original_signature"], "CPadToButton::CPadToButton(int)")

    def test_windows_review_does_not_allow_other_names_signatures_or_addresses(self):
        self.symbols = {0x1010C30E: "GetCDDir__FPCc"}
        for address, declaration, expected in (
            (
                0x45EDA0,
                "char* CPlatformServices::GetCDDir(const char* file)",
                "windows",
            ),
            (
                0x45EDA0,
                "char* CPlatformServices::GetCdDir(const char* file)",
                "mismatch",
            ),
            (0x45EDA0, "char* CPlatformServices::GetCDDir(char* file)", "mismatch"),
            (
                0x45EDA1,
                "char* CPlatformServices::GetCDDir(const char* file)",
                "mismatch",
            ),
        ):
            with self.subTest(address=address, declaration=declaration):
                self.mappings = {address: [0x1010C30E]}
                (row,) = self.scan(
                    f"// FUNCTION: LEMBALL 0x{address:08x}\n{declaration} {{}}"
                )
                self.assertEqual(row["status"], expected)

    def test_windows_callback_review_requires_zero_arguments(self):
        self.symbols = {0x10B0F952: "OnZoomBox__4CWndFUc"}
        self.mappings = {0x43A500: [0x10B0F952]}
        (row,) = self.scan(
            "// FUNCTION: LEMBALL 0x0043a500\nvoid CWnd::OnDriverChange() {}"
        )
        self.assertEqual(
            (row["status"], row["signature_status"]), ("windows", "review")
        )
        (row,) = self.scan(
            "// FUNCTION: LEMBALL 0x0043a500\nvoid CWnd::OnDriverChange(int value) {}"
        )
        self.assertEqual(row["status"], "mismatch")
