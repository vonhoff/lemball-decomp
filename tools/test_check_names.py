"""Parser and evidence-association tests for check_names.py."""

import tempfile
from pathlib import Path
import unittest

from check_names import check_research, decode_symbol, scan


class NamingTests(unittest.TestCase):
    def scan_text(self, text, symbols=None):
        directory = tempfile.TemporaryDirectory()
        self.addCleanup(directory.cleanup)
        path = Path(directory.name) / "sample.cpp"
        path.write_text(text, encoding="utf-8")
        return scan(path, symbols)

    def test_exact_underscore_and_removed_class_prefix(self):
        rows = self.scan_text("// 68K 0x1020fdc2 _DrawButton__9CPVButtonFv\n"
                              "// FUNCTION: LEMBALL 0x00467ef0\n"
                              "void PVButton::_DrawButton() {}\n")
        self.assertEqual(rows[0]["status"], "match")

    def test_renamed_method_is_not_hidden_by_case_folding(self):
        rows = self.scan_text("// 68K 0x1020fdc2 _DrawButton__9CPVButtonFv\n"
                              "void PvButton::DrawButtonInternal() {}\n")
        self.assertEqual(rows[0]["differences"], ["class-case", "method-name"])

    def test_constructor_and_assignment_are_distinct(self):
        rows = self.scan_text("// 68K 0x10108342 __as__7CVSRectFRC7CVSRect\n"
                              "VsRect::VsRect(const VsRect& source) {}\n")
        self.assertEqual(rows[0]["expected_method"], "operator=")
        self.assertEqual(rows[0]["actual_method"], "<constructor>")
        self.assertEqual(rows[0]["status"], "mismatch")

    def test_qualified_owner(self):
        self.assertEqual(decode_symbol("__ct__Q214CPreviewDrawer8tagPRIMSFv"),
                         ("CPreviewDrawer::tagPRIMS", "<constructor>"))

    def test_inline_member_and_nested_class(self):
        rows = self.scan_text("class Outer { struct Inner {\n"
                              "// 68K 0x1000 Tick__Q25Outer5InnerFv\n"
                              "void Tick() { const char* s = \"}\"; }\n}; };\n")
        self.assertEqual(rows[0]["actual_class"], "Outer::Inner")
        self.assertEqual(rows[0]["status"], "match")

    def test_existing_c_prefix_is_also_exact(self):
        rows = self.scan_text("// 68K 0x1000 __ct__8CConnectFv\nCConnect::CConnect() {}\n")
        self.assertEqual(rows[0]["status"], "match")

    def test_trailing_comment_does_not_bind_to_next_class(self):
        rows = self.scan_text("class First {};\n// 68K 0x1000 Tick__5FirstFv\n"
                              "class Second { void Tick(); };\n")
        self.assertEqual(rows[0]["status"], "unresolved")

    def test_bad_owner_length_is_unresolved(self):
        rows = self.scan_text("// 68K 0x1000 Tick__99BadFv\nvoid Bad::Tick() {}\n")
        self.assertEqual(rows[0]["status"], "unresolved")

    def test_free_operator(self):
        rows = self.scan_text("// 68K 0x1000 __pl__FRC5FixedT1\n"
                              "Fixed operator+(const Fixed& a, const Fixed& b) {}\n")
        self.assertEqual(rows[0]["actual_method"], "operator+")
        self.assertEqual(rows[0]["status"], "match")

    def test_research_checks_name_and_pair_independently(self):
        rows = self.scan_text("// 68K 0x1000 Tick__5ClockFv\n"
                              "// FUNCTION: LEMBALL 0x00401000\nvoid Clock::Tick() {}\n")
        check_research(rows, {0x1000: "Tock__5ClockFv"}, {(0x1000, 0x402000)})
        self.assertEqual(len(rows[0]["evidence_findings"]), 2)
        check_research(rows, {0x1000: "Tick__5ClockFv"}, {(0x1000, 0x401000)})
        self.assertEqual(rows[0]["evidence_findings"], [])

    def test_inventory_takes_precedence_over_a_renamed_comment(self):
        rows = self.scan_text("// 68K 0x1000 DrawButton__9CPVButtonFv\n"
                              "void PVButton::DrawButton() {}\n",
                              {0x1000: "_DrawButton__9CPVButtonFv"})
        self.assertEqual(rows[0]["expected_method"], "_DrawButton")
        self.assertEqual(rows[0]["name_evidence"], "inventory")
        self.assertEqual(rows[0]["differences"], ["method-name"])


if __name__ == "__main__":
    unittest.main()
