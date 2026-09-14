"""Do not confuse reccmp's address pairing method with an exact code score."""

import unittest
import io
from contextlib import redirect_stdout

from match import configure_output, match_status, print_match_verbose
from reccmp.compare.report import ReccmpComparedEntity
from reccmp.compare.diff import RawDiffOutput


class MatchStatusTests(unittest.TestCase):
    def entity(self, accuracy, **kwargs):
        return ReccmpComparedEntity(0x1000, "test", accuracy,
                                    recomp_addr=0x2000, **kwargs)

    def test_mapped_partial_function_is_not_labeled_match(self):
        entity = self.entity(0.08)
        self.assertTrue(entity.is_matched())
        self.assertEqual(match_status(entity), "")

    def test_stub_takes_precedence_even_for_identical_empty_body(self):
        for accuracy in (0.08, 1.0):
            self.assertEqual(match_status(self.entity(accuracy, is_stub=True)), "STUB")

    def test_exact_and_effective_matches_are_labeled_match(self):
        self.assertEqual(match_status(self.entity(1.0)), "MATCH")
        self.assertEqual(match_status(self.entity(0.9, is_effective_match=True)), "MATCH")

    def test_verbose_exact_match_survives_windows_pipe_encoding(self):
        output = io.BytesIO()
        stream = io.TextIOWrapper(output, encoding="cp1252")
        with redirect_stdout(stream), self.assertRaises(UnicodeEncodeError):
            print_match_verbose(self.entity(1.0, rdiff=RawDiffOutput()))
        configure_output(stream)
        with redirect_stdout(stream):
            print_match_verbose(self.entity(1.0, rdiff=RawDiffOutput()))
        stream.flush()
        rendered = output.getvalue().decode("cp1252")
        self.assertIn("100% match", rendered)
        self.assertIn("OK!", rendered)
        self.assertEqual(stream.encoding, "cp1252")

    def test_output_configuration_accepts_capture_streams(self):
        stream = io.StringIO()
        configure_output(stream)
        stream.write("captured")
        self.assertEqual(stream.getvalue(), "captured")
