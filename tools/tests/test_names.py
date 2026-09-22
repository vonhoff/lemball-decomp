"""Independent catalog naming authority, signature parsing, and failure cases."""

import contextlib
import io
import tempfile
import unittest
from pathlib import Path

from lib.cpp_signatures import canonical_type, parameter_type
from lib.mac_symbols import decode_signature
from lib.names import check_names, scan
from lib.provenance import catalog_entries, read_catalog


class MacSignatureTests(unittest.TestCase):
    def test_constructor_and_parameter_type(self):
        signature = decode_signature("__ct__12CPadToButtonFi")
        self.assertEqual(signature.display(), "CPadToButton::CPadToButton(int)")
        self.assertEqual(signature.parameters, ("int",))

    def test_destructor_qualified_owner_and_const_method(self):
        self.assertEqual(decode_signature("__dt__Q214CPreviewDrawer8tagPRIMSFv").display(),
                         "CPreviewDrawer::tagPRIMS::~tagPRIMS()")
        self.assertEqual(decode_signature("Get__6CFixedCFv").display(), "CFixed::Get() const")

    def test_pointer_reference_const_and_unsigned(self):
        self.assertEqual(decode_signature("Test__FPCcRC8CVSPointUlCPi").parameters,
                         ("const char*", "const CVSPoint&", "unsigned long", "int* const"))

    def test_function_pointer_keeps_encoded_return_type(self):
        self.assertEqual(decode_signature("Callback__FPFPc_Uc").parameters,
                         ("unsigned char (*)(char*)",))

    def test_unmangled_name_does_not_invent_parameters(self):
        self.assertIsNone(decode_signature("main").parameters)

    def test_embedded_separator_and_conversion_operator(self):
        self.assertEqual(decode_signature("CSurface__2DMemSet__FPPUcUcsss").owner, "")
        self.assertEqual(decode_signature("__op7CVector__8CVSPointCFv").display(),
                         "CVSPoint::operator CVector() const")

    def test_invalid_encodings_are_not_silently_typed(self):
        for symbol in ("__ct__99MissingFi", "Wrong__Fz", "Wrong__FP", "Wrong__F", "Wrong__Fvi"):
            with self.subTest(symbol=symbol), self.assertRaises(ValueError):
                decode_signature(symbol)

    def test_complete_catalog_decodes_without_guessed_parameters(self):
        symbols, _ = catalog_entries(read_catalog())
        for symbol in symbols.values():
            with self.subTest(symbol=symbol):
                decode_signature(symbol)


class ParameterTests(unittest.TestCase):
    def test_names_defaults_and_unnamed_parameters(self):
        for source in ("int p_arg0", "int capacity", "int", "int capacity = 42"):
            with self.subTest(source=source):
                self.assertEqual(parameter_type(source), "int")

    def test_unsigned_and_const_types_do_not_lose_their_type_name(self):
        cases = {"unsigned int": "unsigned int", "const Widget": "Widget",
                 "Widget const* value": "const Widget*", "Widget* const value": "Widget*",
                 "Widget const& value": "const Widget&", "unsigned long value": "unsigned long",
                 "char buffer[12]": "char*", "Namespace::Widget": "Namespace::Widget"}
        for source, expected in cases.items():
            with self.subTest(source=source):
                self.assertEqual(parameter_type(source), expected)

    def test_windows_integer_width_does_not_make_long_and_int_same_type(self):
        self.assertNotEqual(canonical_type("unsigned long"), canonical_type("unsigned int"))

    def test_complex_declarator_requires_review(self):
        with self.assertRaises(ValueError):
            parameter_type("void (*callback)(int)")


class CatalogNamingTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.path = Path(self.temp.name) / "Fixture.cpp"
        self.symbols = {0x10b0f952: "__ct__12CPadToButtonFi"}
        self.pairs = {(0x10b0f952, 0x43a250)}

    def scan(self, source, **kwargs):
        self.path.write_text(source, encoding="utf-8")
        return scan(self.path, self.symbols, self.pairs, **kwargs)

    def test_windows_address_without_mac_comment_resolves_catalog_name(self):
        row, = self.scan("// FUNCTION: LEMBALL 0x0043a250\nCPadToButton::CPadToButton(int capacity) {}")
        self.assertEqual((row["status"], row["signature_status"]), ("match", "match"))
        self.assertEqual(row["original_signature"], "CPadToButton::CPadToButton(int)")

    def test_changed_annotation_and_source_cannot_forge_expected_symbol(self):
        rows = self.scan("// 68K 0x10b0f952 __ct__5CFakeFi\n// FUNCTION: LEMBALL 0x0043a250\nCFake::CFake(int n) {}")
        self.assertEqual([r["status"] for r in rows], ["invalid", "mismatch"])
        self.assertEqual(rows[-1]["symbol"], self.symbols[0x10b0f952])

    def test_real_symbol_at_wrong_windows_entry_is_rejected(self):
        rows = self.scan("// 68K 0x10b0f952 __ct__12CPadToButtonFi\n// FUNCTION: LEMBALL 0x00401000\nCPadToButton::CPadToButton(int n) {}")
        self.assertEqual([r["status"] for r in rows], ["invalid", "unmapped"])

    def test_parameter_names_ignored_but_arity_signedness_and_reference_checked(self):
        for parameters in ("unsigned int n", "int& n", "int n, int extra", ""):
            with self.subTest(parameters=parameters):
                row, = self.scan(f"// FUNCTION: LEMBALL 0x0043a250\nCPadToButton::CPadToButton({parameters}) {{}}")
                self.assertEqual(row["signature_status"], "review")

    def test_method_constness_checked(self):
        self.symbols[0x10b0f952] = "Get__12CPadToButtonCFv"
        row, = self.scan("// FUNCTION: LEMBALL 0x0043a250\nint CPadToButton::Get() {}")
        self.assertEqual(row["signature_status"], "review")

    def test_fake_markers_in_strings_and_block_comments_are_ignored(self):
        rows = self.scan('const char* s = "// FUNCTION: LEMBALL 0x0043a250";\n'
                         '/* // 68K 0x10b0f952 Fake */\n')
        self.assertEqual(rows, [])

    def test_inline_without_windows_pair_uses_catalog_symbol(self):
        row, = self.scan("class CPadToButton {\n// 68K 0x10b0f952 __ct__12CPadToButtonFi\nCPadToButton(int size);\n};")
        self.assertEqual(row["status"], "match")
        self.assertIsNone(row["windows_address"])

    def test_folded_windows_entry_uses_matching_catalog_candidate(self):
        self.symbols[0x10100004] = "Wrong__5COtherFv"
        self.pairs.add((0x10100004, 0x43a250))
        row, = self.scan("// FUNCTION: LEMBALL 0x0043a250\nCPadToButton::CPadToButton(int n) {}")
        self.assertEqual(row["status"], "match")
        self.assertEqual(len(row["catalog_candidates"]), 2)

    def test_synthetic_comment_cannot_attach_across_blank_line(self):
        rows = self.scan("// SYNTHETIC: LEMBALL 0x0043a250\n// CPadToButton::compiler helper\n\n"
                         "// FUNCTION: LEMBALL 0x00401000\nvoid Unrelated() {}")
        self.assertEqual(rows[0]["status"], "synthetic")
        self.assertEqual(rows[1]["status"], "unmapped")

    def test_original_mode_preserves_c_prefix(self):
        row, = self.scan("// FUNCTION: LEMBALL 0x0043a250\nPadToButton::PadToButton(int n) {}", original=True)
        self.assertEqual(row["status"], "mismatch")
        self.assertEqual(row["wanted_class"], "CPadToButton")

    def test_normal_mode_also_requires_catalog_class_prefix(self):
        row, = self.scan("// FUNCTION: LEMBALL 0x0043a250\nPadToButton::PadToButton(int n) {}")
        self.assertEqual(row["status"], "mismatch")
        self.assertEqual(row["wanted_class"], "CPadToButton")

    def test_strict_signature_review_exit_status(self):
        self.path.write_text("// FUNCTION: LEMBALL 0x0043a250\nCPadToButton::CPadToButton(short n) {}", encoding="utf-8")
        with contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(check_names([self.path], strict=True), 1)
            self.assertEqual(check_names([self.path]), 0)


if __name__ == "__main__":
    unittest.main()
