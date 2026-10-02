"""Independent catalog naming authority, signature parsing, and failure cases."""

# Pylint infers an empty list for scan(), ignoring its append calls.
# pylint: disable=unbalanced-tuple-unpacking

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
            '__ct__12CPadToButtonFi': 'CPadToButton::CPadToButton(int)',
            '__dt__Q214CPreviewDrawer8tagPRIMSFv': 'CPreviewDrawer::tagPRIMS::~tagPRIMS()',
            'Get__6CFixedCFv': 'CFixed::Get() const',
            'Callback__FPFPc_Uc': 'Callback(unsigned char (*)(char*))',
            '__op7CVector__8CVSPointCFv': 'CVSPoint::operator CVector() const',
            'main': 'main(?)',
        }
        for symbol, expected in cases.items():
            self.assertEqual(decode_signature(symbol).display(), expected)
        symbols, _ = read_catalog()
        for symbol in symbols.values():
            decode_signature(symbol)
        with self.assertRaises(ValueError):
            decode_signature('Wrong__FP')

    def test_cpp_parameter_types(self):
        cases = {'int capacity = 42': 'int', 'Widget const* value': 'const Widget*',
                 'Widget* const value': 'Widget*', 'char buffer[12]': 'char*',
                 'Namespace::Widget': 'Namespace::Widget', 'unsigned long value': 'unsigned long',
                 'int (*callback)(char* text)': 'int (*)(char*)',
                 'void (*outer)(int (*inner)(const char* text))': 'void (*)(int (*)(const char*))'}
        for source, expected in cases.items():
            self.assertEqual(parameter_type(source), expected)
        self.assertNotEqual(canonical_type('unsigned long'), canonical_type('unsigned int'))
        for source in ('void (__stdcall *callback)(int)', 'void (Widget::*callback)(int)',
                       'void (*callback)(void, int)'):
            with self.assertRaises(ValueError):
                parameter_type(source)


class CatalogTests(unittest.TestCase):
    def test_catalog_preserves_variants_and_rejects_bad_evidence(self):
        header = "mac_address,symbol,windows_address\n"
        row = "1060000c,Real__Fv,401000\n"
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "catalog.csv"
            path.write_text(header + row + "1060000c,Real__Fv,402000\n10600020,MacOnly__Fv,\n",
                            encoding="utf-8")
            self.assertEqual(read_catalog(path), (
                {0x1060000c: "Real__Fv", 0x10600020: "MacOnly__Fv"},
                {(0x1060000c, 0x401000), (0x1060000c, 0x402000)},
            ))
            for invalid in ("mac_address,symbol\n", header, header + row + row,
                            header + row + "1060000c,Wrong__Fv,402000\n",
                            header + row + "1060000c,Real__Fv,\n",
                            header + "1060000c,Made up,401000\n",
                            header + "1060000c,Real__Fv,0\n",
                            header + "1060000c,Real__Fv,100000000\n"):
                path.write_text(invalid, encoding="utf-8")
                with self.subTest(invalid=invalid), self.assertRaises(ValueError):
                    read_catalog(path)
                with contextlib.redirect_stdout(io.StringIO()), contextlib.redirect_stderr(io.StringIO()):
                    self.assertEqual(check_names(catalog_path=path), 2)


class CatalogNamingTests(unittest.TestCase):
    def setUp(self):
        # enterContext registers cleanup with unittest.
        with_directory = tempfile.TemporaryDirectory()  # pylint: disable=consider-using-with
        self.path = Path(self.enterContext(with_directory)) / "Fixture.cpp"
        self.symbols = {0x10b0f952: "__ct__12CPadToButtonFi"}
        self.pairs = {(0x10b0f952, 0x43a250)}

    def scan(self, source, **kwargs):
        self.path.write_text(source, encoding="utf-8")
        return scan(self.path, self.symbols, self.pairs, **kwargs)

    def test_callback_abi_difference_is_compared_not_suppressed(self):
        self.symbols[0x10b0f952] = "__ct__17CVSDebugStreambufFPciPFPc_Uc"
        row, = self.scan("// FUNCTION: LEMBALL 0x0043a250\n"
                         "CVSDebugStreambuf::CVSDebugStreambuf(char* buffer, int size, int (*callback)(char*)) {}")
        self.assertEqual((row["status"], row["signature_status"]), ("match", "review"))
        self.assertIn("unsigned char (*)(char*)", row["original_signature"])
        self.assertIn("int (*)(char*)", row["actual_signature"])

    def test_changed_annotation_and_source_cannot_forge_expected_symbol(self):
        rows = self.scan("// 68K 0x10b0f952 __ct__5CFakeFi\n// FUNCTION: LEMBALL 0x0043a250\nCFake::CFake(int n) {}")
        self.assertEqual([r["status"] for r in rows], ["mismatch"])
        self.assertEqual(rows[-1]["symbol"], self.symbols[0x10b0f952])

    def test_parameter_names_ignored_but_arity_signedness_and_reference_checked(self):
        for parameters in ("unsigned int n", "int& n", "int n, int extra", ""):
            with self.subTest(parameters=parameters):
                row, = self.scan(f"// FUNCTION: LEMBALL 0x0043a250\nCPadToButton::CPadToButton({parameters}) {{}}")
                self.assertEqual(row["signature_status"], "review")
        self.symbols[0x10b0f952] = 'Get__12CPadToButtonCFv'
        row, = self.scan('// FUNCTION: LEMBALL 0x0043a250\nint CPadToButton::Get() {}')
        self.assertEqual(row['signature_status'], 'review')

    def test_fake_markers_in_strings_and_block_comments_are_ignored(self):
        rows = self.scan('const char* s = "// FUNCTION: LEMBALL 0x0043a250";\n'
                         '/* // 68K 0x10b0f952 Fake */\n')
        self.assertEqual(rows, [])

    def test_folded_windows_entry_uses_matching_catalog_candidate(self):
        self.symbols[0x10100004] = "Wrong__5COtherFv"
        self.pairs.add((0x10100004, 0x43a250))
        row, = self.scan("// FUNCTION: LEMBALL 0x0043a250\nCPadToButton::CPadToButton(int n) {}")
        self.assertEqual(row["status"], "match")
        self.assertEqual(len(row["catalog_candidates"]), 2)
        self.assertEqual(row["original_signature"], "CPadToButton::CPadToButton(int)")

    def test_synthetic_comment_cannot_attach_across_blank_line(self):
        rows = self.scan("// SYNTHETIC: LEMBALL 0x0043a250\n// CPadToButton::compiler helper\n\n"
                         "// FUNCTION: LEMBALL 0x00401000\nvoid Unrelated() {}")
        self.assertEqual(rows[0]["status"], "synthetic")
        self.assertEqual(rows[1]["status"], "unmapped")

    def test_windows_member_review_keeps_original_catalog_identity(self):
        self.symbols = {0x1010c30e: "GetCDDir__FPCc"}
        self.pairs = {(0x1010c30e, 0x45eda0)}
        row, = self.scan("// FUNCTION: LEMBALL 0x0045eda0\n"
                         "char* CPlatformServices::GetCDDir(const char* file) {}")
        self.assertEqual((row["status"], row["signature_status"]), ("windows", "review"))
        self.assertEqual(row["original_signature"], "GetCDDir(const char*)")
        self.assertIn("RET 4", row["windows_evidence"])

    def test_windows_review_does_not_allow_other_names_signatures_or_addresses(self):
        self.symbols = {0x1010c30e: "GetCDDir__FPCc"}
        for address, declaration in (
                (0x45eda0, "char* CPlatformServices::GetCdDir(const char* file)"),
                (0x45eda0, "char* CPlatformServices::GetCDDir(char* file)"),
                (0x45eda1, "char* CPlatformServices::GetCDDir(const char* file)")):
            with self.subTest(address=address, declaration=declaration):
                self.pairs = {(0x1010c30e, address)}
                row, = self.scan(f"// FUNCTION: LEMBALL 0x{address:08x}\n{declaration} {{}}")
                self.assertEqual(row["status"], "mismatch")

    def test_windows_callback_review_requires_zero_arguments(self):
        self.symbols = {0x10b0f952: "OnZoomBox__4CWndFUc"}
        self.pairs = {(0x10b0f952, 0x43a500)}
        row, = self.scan("// FUNCTION: LEMBALL 0x0043a500\nvoid CWnd::OnDriverChange() {}")
        self.assertEqual((row["status"], row["signature_status"]), ("windows", "review"))
        row, = self.scan("// FUNCTION: LEMBALL 0x0043a500\nvoid CWnd::OnDriverChange(int value) {}")
        self.assertEqual(row["status"], "mismatch")

    def test_strict_signature_review_exit_status(self):
        self.path.write_text("// FUNCTION: LEMBALL 0x0043a250\nCPadToButton::CPadToButton(short n) {}",
                             encoding="utf-8")
        with contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(check_names([self.path], strict=True), 1)
            self.assertEqual(check_names([self.path]), 0)


if __name__ == "__main__":
    unittest.main()
