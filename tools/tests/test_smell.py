"""Annotation detection and narrowly reviewed smell exceptions."""

import contextlib
import io
import json
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from lib.smell import apply_exceptions, check_smell, declaration_has_body, load_exceptions, scan_file


class FunctionBodyTests(unittest.TestCase):
    def test_wrapped_local_constructor_and_prototype_are_not_definitions(self):
        self.assertFalse(declaration_has_body("CVsPoint position(\n x,\n y);\nif (x) {\n}"))
        self.assertFalse(declaration_has_body("void Callback(\n int argument\n);\nvoid Next() {}"))

    def test_multiline_function_and_constructor_still_require_annotations(self):
        self.assertTrue(declaration_has_body("void Function(\n int argument\n)\n{\n}"))
        self.assertTrue(declaration_has_body("CThing::CThing(\n int argument\n) : CBase(argument)\n{\n}"))
        self.assertTrue(declaration_has_body("int CThing::Get() const\n{ return 1; }"))


class ReviewedExceptionTests(unittest.TestCase):
    def setUp(self):
        temp = tempfile.TemporaryDirectory()
        self.addCleanup(temp.cleanup)
        # Match the resolved repository root when Windows TEMP uses an 8.3 alias.
        self.root = Path(temp.name).resolve()
        self.source = self.root / "src" / "Fixture.h"
        self.source.parent.mkdir()
        self.manifest = self.root / "exceptions.json"
        self.code = "*(unsigned int*) &" + "long_member_name_" * 7 + "first = value;"
        self.entry = {
            "path": "src/Fixture.h", "rule": "addr-cast-punning", "code": self.code,
            "count": 1, "reason": "Reviewed packed field copy.",
        }
        for name, value in (("ROOT", self.root), ("EXCEPTIONS", self.manifest)):
            patcher = patch("lib.smell." + name, value)
            patcher.start()
            self.addCleanup(patcher.stop)
        self.write_manifest([self.entry])
        self.source.write_text(self.code + "\n", encoding="utf-8")

    def write_manifest(self, entries):
        self.manifest.write_text(json.dumps({"version": 1, "findings": entries}), encoding="utf-8")

    def scan(self):
        hits, _ = scan_file(self.source, True, False, set())
        return hits

    def test_line_movement_keeps_exact_exception_and_reason(self):
        self.source.write_text("\n\n" + self.code + "\n", encoding="utf-8")
        hits = self.scan()
        unreviewed, stale, reviewed = apply_exceptions(hits, [self.source])
        self.assertEqual((unreviewed, stale), ([], []))
        self.assertEqual(reviewed, [(hits[0], self.entry["reason"])])
        self.assertEqual(hits[0][1], 3)

    def test_edit_after_old_truncation_limit_requires_new_review(self):
        changed = self.code.replace("first", "second")
        self.assertEqual(changed[:100], self.code[:100])
        self.source.write_text(changed + "\n", encoding="utf-8")
        hits = self.scan()
        unreviewed, stale, reviewed = apply_exceptions(hits, [self.source])
        self.assertEqual(unreviewed, hits)
        self.assertEqual(len(stale), 1)
        self.assertEqual(reviewed, [])

    def test_extra_occurrence_exceeds_reviewed_count(self):
        self.source.write_text((self.code + "\n") * 2, encoding="utf-8")
        hits = self.scan()
        unreviewed, stale, reviewed = apply_exceptions(hits, [self.source])
        self.assertEqual(unreviewed, hits[1:])
        self.assertEqual(stale, [])
        self.assertEqual(len(reviewed), 1)

    def test_pointer_adjustment_findings_keep_source_identity(self):
        code = "void* owner = (char*) this - 0x10;"
        self.source.write_text(code + "\n", encoding="utf-8")
        hits = self.scan()
        self.assertIn(("src/Fixture.h", 1, "this-adjust-poke", code), hits)
        self.assertTrue(all(hit[3] == code for hit in hits))

    def test_path_and_rule_must_both_match(self):
        for path, rule in (("src/Other.h", self.entry["rule"]), (self.entry["path"], "offset-poke")):
            with self.subTest(path=path, rule=rule):
                hits = [(path, 1, rule, self.code)]
                unreviewed, stale, reviewed = apply_exceptions(hits, [self.source])
                self.assertEqual(unreviewed, hits)
                self.assertEqual(len(stale), 1)
                self.assertEqual(reviewed, [])

    def test_unused_allowance_is_stale_only_in_scanned_scope(self):
        other = self.root / "src" / "Other.h"
        self.assertEqual(apply_exceptions([], [other]), ([], [], []))
        self.assertEqual(len(apply_exceptions([], [self.source])[1]), 1)
        self.entry["count"] = 2
        self.write_manifest([self.entry])
        self.assertIn("count 1", apply_exceptions(self.scan(), [self.source])[1][0])

    def test_deleted_source_is_stale_in_full_scan(self):
        self.source.unlink()
        unreviewed, stale, reviewed = apply_exceptions([], [], full_scan=True)
        self.assertEqual((unreviewed, reviewed), ([], []))
        self.assertEqual(len(stale), 1)
        self.assertIn("src/Fixture.h: exception-stale", stale[0])

    def test_invalid_entries_and_duplicate_allowances_fail(self):
        for field, values in {
            "reason": [None, "", "  "], "code": [None, ""], "rule": [None, ""],
            "path": ["../Fixture.h", "src/../Fixture.h", "src\\Fixture.h", "src//Fixture.h"],
            "count": [-1, 0, True, "1", 1.5],
        }.items():
            for value in values:
                with self.subTest(field=field, value=value), self.assertRaises(ValueError):
                    self.write_manifest([dict(self.entry, **{field: value})])
                    load_exceptions()
        self.write_manifest([self.entry, self.entry])
        with self.assertRaisesRegex(ValueError, "duplicate exception"):
            load_exceptions()

    def test_invalid_manifest_returns_input_error_without_traceback(self):
        for data in ("{", "[]", "{}", '{"version": true, "findings": []}',
                     '{"version": 2, "findings": []}', '{"version": 1, "findings": [null]}'):
            with self.subTest(data=data):
                self.manifest.write_text(data, encoding="utf-8")
                with contextlib.redirect_stderr(io.StringIO()) as errors:
                    self.assertEqual(check_smell(paths=[self.source]), 2)
                self.assertIn("smell:", errors.getvalue())
        self.manifest.unlink()
        with contextlib.redirect_stderr(io.StringIO()) as errors:
            self.assertEqual(check_smell(paths=[self.source]), 2)
        self.assertIn("cannot read", errors.getvalue())

    def test_verbose_output_exposes_reviewed_code_and_reason(self):
        for verbose in (False, True):
            with self.subTest(verbose=verbose), contextlib.redirect_stdout(io.StringIO()) as output:
                self.assertEqual(check_smell(paths=[self.source], verbose=verbose), 0)
                self.assertIn("1 reviewed exception(s)", output.getvalue())
                self.assertEqual(self.code in output.getvalue(), verbose)
                self.assertEqual(self.entry["reason"] in output.getvalue(), verbose)

    def test_unreviewed_code_still_fails_gate(self):
        self.write_manifest([])
        with contextlib.redirect_stderr(io.StringIO()) as errors:
            self.assertEqual(check_smell(paths=[self.source]), 1)
        self.assertIn(self.code, errors.getvalue())


if __name__ == "__main__":
    unittest.main()
