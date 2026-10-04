"""Independent catalog naming authority, signature parsing, and failure cases."""

import contextlib
import io
import tempfile
import unittest
from pathlib import Path

from lib.names import check_names, read_catalog, scan
from lib.signatures import canonical_type, decode_signature, parameter_type


class SignatureTests(unittest.TestCase):
    def test_catalog_encodings(self):
        cases = {
            "__ct__12CPadToButtonFi": "CPadToButton::CPadToButton(int)",
            "__dt__Q214CPreviewDrawer8tagPRIMSFv": "CPreviewDrawer::tagPRIMS::~tagPRIMS()",
            "Get__6CFixedCFv": "CFixed::Get() const",
            "Callback__FPFPc_Uc": "Callback(unsigned char (*)(char*))",
            "__op7CVector__8CVSPointCFv": "CVSPoint::operator CVector() const",
            "main": "main(?)",
        }
        for symbol, expected in cases.items():
            self.assertEqual(decode_signature(symbol).display(), expected)
        for symbol in ("Wrong__FP", "Callback__FPFPc", "Callback__FPFPc_"):
            with self.subTest(symbol=symbol), self.assertRaises(ValueError):
                decode_signature(symbol)

    def test_cpp_parameter_types(self):
        cases = {
            "int capacity = 42": "int",
            "Widget const* value": "const Widget*",
            "Widget* const value": "Widget*",
            "char buffer[12]": "char*",
            "Namespace::Widget": "Namespace::Widget",
            "unsigned long value": "unsigned long",
            "int (*callback)(char* text)": "int (*)(char*)",
            "void (*outer)(int (*inner)(const char* text))": "void (*)(int (*)(const char*))",
        }
        for source, expected in cases.items():
            self.assertEqual(parameter_type(source), expected)
        self.assertNotEqual(
            canonical_type("unsigned long"), canonical_type("unsigned int")
        )
        for source in (
            "void (__stdcall *callback)(int)",
            "void (Widget::*callback)(int)",
            "void (*callback)(void, int)",
        ):
            with self.assertRaises(ValueError):
                parameter_type(source)


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
        self.assertEqual(rows[-1]["symbol"], self.symbols[0x10B0F952])

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
        self.assertEqual(len(row["catalog_candidates"]), 2)
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

    def test_signature_review_details_do_not_fail_gate(self):
        self.path.write_text(
            "// FUNCTION: LEMBALL 0x0043a250\nCPadToButton::CPadToButton(short n) {}",
            encoding="utf-8",
        )
        for verbose in (False, True):
            output = io.StringIO()
            with contextlib.redirect_stdout(output):
                self.assertEqual(check_names([self.path], verbose=verbose), 0)
            self.assertIn(
                "signature review requires Windows evidence", output.getvalue()
            )
            self.assertEqual(
                "CPadToButton::CPadToButton(short)" in output.getvalue(), verbose
            )
