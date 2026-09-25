"""Independent resource-fork fixtures and annotation provenance failure cases."""

import contextlib
import io
import struct
import tempfile
import unittest
from pathlib import Path

from lib.provenance import (
    audit_annotations,
    catalog_entries,
    check_provenance,
    parse_catalog,
    read_catalog,
    read_symbols,
    scan_annotations,
    scan_windows_annotations,
    verify_resource,
)


def procedure(name, constants=b""):
    encoded = name.encode("ascii")
    marker = bytes([0x80 | len(encoded)]) if len(encoded) <= 31 else bytes([0x80, len(encoded)])
    body = b"\x4e\x71\x4e\x75" + marker + encoded  # NOP; RTS; trailer.
    body += b"\0" * (len(body) % 2)
    body += struct.pack(">H", len(constants)) + constants
    return body + b"\0" * (len(body) % 2)


def code_resource(resource_id, procedures, relocations=b""):
    if resource_id == 1:
        return b"\0" * 4 + procedures
    return b"\0" * 8 + struct.pack(">I", 12 + len(procedures)) + procedures + relocations


def resource_fork(resources):
    data, references = bytearray(), bytearray()
    for resource_id, payload in resources:
        references += struct.pack(">hH", resource_id, 0xffff)
        references += b"\0" + len(data).to_bytes(3, "big") + b"\0" * 4
        data += struct.pack(">I", len(payload)) + payload
    resource_map = bytearray(28)
    resource_map[24:28] = struct.pack(">HH", 28, 38 + len(references))
    resource_map += struct.pack(">H4sHH", 0, b"CODE", len(resources) - 1, 10) + references
    header = struct.pack(">IIII", 256, 256 + len(data), len(data), len(resource_map))
    resource_map[:16] = header
    return header + bytes(240) + data + resource_map


class ResourceSymbolTests(unittest.TestCase):
    def test_segment_addresses_long_symbols_and_import_sanitization(self):
        template = "FindElement__38CList<6CPVWnd,21CListElement<6CPVWnd>>FP6CPVWnd"
        first = procedure("First__Fv")
        fork = resource_fork([
            (0, b"jump table"),
            (1, code_resource(1, first + procedure(template))),
            (6, code_resource(6, procedure("Second__Fv"))),
            (3, code_resource(3, b"\x4e\x75\0\0")),  # Stripped library.
        ])
        self.assertEqual(read_symbols(fork), {
            0x10100004: "First__Fv",
            0x10100004 + len(first): "FindElement__38CList_6CPVWnd_21CListElement_6CPVWnd__FP6CPVWnd",
            0x1060000c: "Second__Fv",
        })

    def test_constants_and_relocations_cannot_invent_functions(self):
        payload = code_resource(6, procedure("Real__Fv", procedure("FakeConstant__Fv")),
                                procedure("FakeRelocation__Fv"))
        self.assertEqual(read_symbols(resource_fork([(6, payload)])), {0x1060000c: "Real__Fv"})

    def test_truncated_resource_map_and_out_of_range_payload_fail(self):
        fork = resource_fork([(1, code_resource(1, procedure("Real__Fv")))])
        with self.assertRaises(ValueError):
            read_symbols(fork[:-1])
        damaged = bytearray(fork)
        struct.pack_into(">I", damaged, 256, 0xffffffff)
        with self.assertRaises(ValueError):
            read_symbols(damaged)

    def test_invalid_relocation_boundary_and_unparsed_named_tail_fail(self):
        payload = code_resource(6, procedure("Real__Fv"))
        with self.assertRaises(ValueError):
            read_symbols(resource_fork([(6, payload[:8] + b"\xff" * 4 + payload[12:])]))
        with self.assertRaises(ValueError):
            read_symbols(resource_fork([(1, code_resource(1, procedure("Real__Fv") + b"tail"))]))

    def test_empty_and_duplicate_code_resources_fail(self):
        payload = code_resource(1, procedure("Real__Fv"))
        for resources in ([(0, b"jump table")], [(1, payload), (1, payload)]):
            with self.subTest(resources=resources), self.assertRaises(ValueError):
                read_symbols(resource_fork(resources))


class AnnotationProvenanceTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.source = self.root / "Fixture.cpp"
        self.symbols = {0x1060000c: "Real__Fv"}
        self.mappings = {(0x1060000c, 0x401000)}

    def scan(self, text):
        self.source.write_text(text, encoding="utf-8")
        return audit_annotations(scan_annotations(self.source), self.symbols, self.mappings)

    def test_real_addresses_and_names_must_belong_to_the_same_mapping(self):
        self.symbols[0x10600020] = "Other__Fv"
        self.mappings.add((0x10600020, 0x402000))
        rows = self.scan("// 68K 0x1060000c Other__Fv\n// FUNCTION: LEMBALL 0x00401000\nvoid F() {}")
        self.assertEqual(rows[0]["status"], "invalid")
        rows = self.scan("// 68K 0x10600020 Real__Fv\n// FUNCTION: LEMBALL 0x00401000\nvoid F() {}")
        self.assertEqual(rows[0]["status"], "invalid")
        rows = self.scan("// 68K 0x1060000c Real__Fv\n// FUNCTION: LEMBALL 0x00402000\nvoid F() {}")
        self.assertEqual(rows[0]["status"], "review")

    def test_unknown_address_and_wrong_symbol_fail_independently(self):
        for annotation in ("0x1060000e Real__Fv", "0x1060000c Invented__Fv"):
            with self.subTest(annotation=annotation):
                self.assertEqual(self.scan(f"// 68K {annotation}\nvoid F();")[0]["status"], "invalid")

    def test_malformed_comment_is_reported_not_silently_skipped(self):
        rows = self.scan("\n// 68K not-an-address Real__Fv\nvoid F();")
        self.assertEqual((rows[0]["status"], rows[0]["line"]), ("invalid", 2))

    def test_strings_and_block_comments_are_not_annotations(self):
        rows = self.scan('const char* text = "// 68K 0x1060000c Invented__Fv";\n'
                         '/*\n// 68K 0x1060000c Invented__Fv\n*/\n')
        self.assertEqual(rows, [])

    def test_declaration_does_not_borrow_later_function_address(self):
        rows = self.scan("// 68K 0x1060000c Real__Fv\nvoid Real();\n"
                         "// FUNCTION: LEMBALL 0x00401000\nvoid Unrelated() {}")
        self.assertEqual(rows[0]["status"], "symbol-only")

    def test_preceding_and_folded_windows_addresses_in_same_block(self):
        rows = self.scan("// FUNCTION: LEMBALL 0x00401000\n// 68K 0x1060000c Real__Fv\n"
                         "// FUNCTION: LEMBALL 0x00402000 FOLDED\nvoid F() {}")
        self.assertEqual(rows[0]["windows_addresses"], (0x401000, 0x402000))
        self.assertEqual(rows[0]["status"], "review")

    def test_synthetic_declaration_ends_before_next_comment_block(self):
        rows = self.scan("// 68K 0x1060000c Real__Fv\n// SYNTHETIC: LEMBALL 0x00401000\n"
                         "// Real::Real\n\n// SYNTHETIC: LEMBALL 0x00402000\n// Real::~Real\n")
        self.assertEqual(rows[0]["windows_addresses"], (0x401000,))
        self.assertEqual(rows[0]["status"], "confirmed")


class CatalogTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.path = self.root / "catalog.csv"
        self.fork = resource_fork([(6, code_resource(6, procedure("Real__Fv")))])
        self.catalog = [(0x1060000c, "Real__Fv", 0x401000)]
        self.header = "mac_address,symbol,windows_address\n"
        self.text = self.header + "1060000c,Real__Fv,401000\n"
        self.path.write_text(self.text, encoding="utf-8")

    def test_plain_csv_reads_addresses_as_integers(self):
        self.assertEqual(read_catalog(self.path), self.catalog)
        self.path.write_text(self.text, encoding="utf-8-sig")
        self.assertEqual(read_catalog(self.path), self.catalog)
        self.assertEqual(catalog_entries(self.catalog),
                         ({0x1060000c: "Real__Fv"}, {(0x1060000c, 0x401000)}))

    def test_invalid_schema_and_addresses_fail(self):
        for text in ("", "mac_address,symbol\n1060000c,Real__Fv\n", self.header,
                     self.header + "1060000c,Real__Fv\n",
                     self.header + "1060000c,Real__Fv,401000,extra\n",
                     self.header + "not-hex,Real__Fv,401000\n",
                     self.header + "1060000c,Real__Fv,0\n",
                     self.header + "1060000c,Real__Fv,100000000\n",
                     self.header + "1060000c,Made up,401000\n"):
            with self.subTest(text=text), self.assertRaises(ValueError):
                parse_catalog(text)
        for row in ([], [0, "Real__Fv", 0x401000], [True, "Real__Fv", 0x401000],
                    [0x1060000c, "Real__Fv", -1], [0x1060000c, "Real__Fv", []]):
            with self.subTest(row=row), self.assertRaises(ValueError):
                catalog_entries([row])

    def test_conflicting_names_and_duplicate_pairs_are_rejected(self):
        for extra in ("1060000c,Real__Fv,401000\n",
                      "1060000c,Wrong__Fv,402000\n",
                      "1060000c,Real__Fv,\n"):
            with self.subTest(extra=extra), self.assertRaises(ValueError):
                parse_catalog(self.text + extra)
        empty = "10600020,MacOnly__Fv,\n"
        with self.assertRaises(ValueError):
            parse_catalog(self.text + empty + empty)

    def test_wrapper_variants_folded_functions_and_mac_only_symbols_are_valid(self):
        rows = parse_catalog(self.text + "1060000c,Real__Fv,402000\n"
                             "10600020,Folded__Fv,401000\n10600040,MacOnly__Fv,\n")
        symbols, pairs = catalog_entries(rows)
        self.assertEqual(len(symbols), 3)
        self.assertEqual(pairs, {(0x1060000c, 0x401000), (0x1060000c, 0x402000),
                                 (0x10600020, 0x401000)})

    def test_private_verification_checks_complete_name_address_dictionary(self):
        private = self.root / "original.rsrc"
        private.write_bytes(self.fork)
        verify_resource(self.catalog, private)
        for altered in ([(0x1060000c, "Invented__Fv", 0x401000)],
                        [(0x1060000e, "Real__Fv", 0x401000)],
                        self.catalog + [(0x10600020, "Extra__Fv", None)]):
            with self.subTest(altered=altered), self.assertRaisesRegex(ValueError, "symbols differ"):
                verify_resource(altered, private)
        # Unrelated container bytes do not change symbol provenance; no whole-file hash needed.
        private.write_bytes(self.fork + b"unrelated container data")
        verify_resource(self.catalog, private)
        private.write_bytes(self.fork.replace(b"Real__Fv", b"Fake__Fv"))
        with self.assertRaisesRegex(ValueError, "symbols differ"):
            verify_resource(self.catalog, private)

    def test_cli_without_original_checks_name_address_and_windows_pair(self):
        source = self.root / "Fixture.cpp"
        source.write_text("// 68K 0x1060000c Real__Fv\n// FUNCTION: LEMBALL 0x00401000\nvoid F() {}")
        with contextlib.redirect_stdout(io.StringIO()), contextlib.redirect_stderr(io.StringIO()):
            self.assertEqual(check_provenance(self.path, [source], strict=True), 0)
            for mac, name, win, status in (("1060000c", "Real__Fv", "00402000", 1),
                                           ("1060000c", "Invented__Fv", "00401000", 1),
                                           ("1060000e", "Real__Fv", "00401000", 1)):
                source.write_text(f"// 68K 0x{mac} {name}\n// FUNCTION: LEMBALL 0x{win}\nvoid F() {{}}")
                self.assertEqual(check_provenance(self.path, [source], strict=True), status)
            source.write_text("void WindowsOnlyFunction() {}")
            self.assertEqual(check_provenance(self.path, [source], strict=True), 0)
            self.path.unlink()
            self.assertEqual(check_provenance(self.path, [source]), 2)

    def test_windows_annotation_alone_resolves_catalog_coverage(self):
        source = self.root / "Fixture.cpp"
        source.write_text('// FUNCTION: LEMBALL 0x00401000\nvoid Real() {}\n'
                          'const char* text = "// FUNCTION: LEMBALL 0x00402000";\n'
                          '/* // FUNCTION: LEMBALL 0x00403000 */\n')
        self.assertEqual(scan_windows_annotations(source), [0x401000])
        output = io.StringIO()
        with contextlib.redirect_stdout(output):
            self.assertEqual(check_provenance(self.path, [source], strict=True), 0)
        self.assertIn("1/1 Windows entries", output.getvalue())

    def test_corrupt_catalog_returns_input_error(self):
        for data in (b"not CSV", b"\xff", b"mac_address,symbol,windows_address\ntruncated"):
            self.path.write_bytes(data)
            with self.subTest(data=data), contextlib.redirect_stderr(io.StringIO()):
                self.assertEqual(check_provenance(self.path), 2)


if __name__ == "__main__":
    unittest.main()
