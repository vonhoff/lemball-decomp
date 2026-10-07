"""Upstream comparison display without build coupling."""

import contextlib
import copy
import io
import unittest
from unittest.mock import Mock, patch

from reccmp.compare.report import ReccmpComparedEntity

import check_function


class CheckFunctionTests(unittest.TestCase):
    def test_raw_exact_counts_without_additional_checks(self):
        comparison = ReccmpComparedEntity(0x401000, "Caller", 1, recomp_addr=0x501000)
        output = io.StringIO()
        with contextlib.redirect_stdout(output):
            check_function.display_comparison(0x401000, comparison, True)
        self.assertIn("Raw: 100.00%  Effective: 100.00%", output.getvalue())
        self.assertNotIn("UNVERIFIED", output.getvalue())

    def test_summary_is_compact_and_missing_addresses_fail(self):
        engine = Mock()
        engine.compare_address.side_effect = [
            ReccmpComparedEntity(0x401000, "Equivalent", 0.8, recomp_addr=0x501000),
            ReccmpComparedEntity(
                0x401020, "Stub", 1.0, recomp_addr=0x501020, is_stub=True
            ),
            None,
        ]
        output = io.StringIO()
        with (
            patch(
                "sys.argv",
                ["check_function.py", "401000", "401020", "401040", "--summary"],
            ),
            patch("check_function.load_engine", return_value=(None, engine)),
            patch("check_function.print_match_verbose") as verbose,
            contextlib.redirect_stdout(output),
        ):
            self.assertEqual(check_function.main(), 1)
            verbose.assert_not_called()
        self.assertEqual(
            output.getvalue().splitlines(),
            [
                "0x00401000 Raw: 80.00%  Effective: 80.00% Equivalent",
                "0x00401020 Raw: 0.00%  Effective: 0.00% STUB Stub",
                "0x00401040: NOT_FOUND",
            ],
        )

    def test_both_scores_display_preserves_raw_comparisons(self):
        comparisons = [
            ReccmpComparedEntity(
                0x401000,
                "Upstream equivalent",
                0.8,
                recomp_addr=0x501000,
                is_effective_match=True,
            ),
            ReccmpComparedEntity(
                0x401020, "Extra equivalent", 0.75, recomp_addr=0x501020
            ),
            ReccmpComparedEntity(0x401040, "Partial", 0.5, recomp_addr=0x501040),
        ]
        unchanged = copy.deepcopy(comparisons)
        engine = Mock()
        engine.compare_address.side_effect = comparisons
        output = io.StringIO()
        with (
            patch(
                "sys.argv",
                ["check_function.py", *[hex(c.orig_addr) for c in comparisons]],
            ),
            patch("check_function.load_engine", return_value=(None, engine)),
            patch("check_function.print_match_verbose") as display,
            contextlib.redirect_stdout(output),
        ):
            self.assertEqual(check_function.main(), 0)
            shown = [call_args.args[0] for call_args in display.call_args_list]
            self.assertEqual([c.is_effective_match for c in shown], [True, False, False])
            self.assertEqual([c.accuracy for c in shown], [0.8, 0.75, 0.5])
            self.assertEqual(comparisons, unchanged)
            self.assertEqual(
                output.getvalue().splitlines(),
                [
                    "Raw: 80.00%  Effective: 100.00%",
                    "Raw: 75.00%  Effective: 75.00%",
                    "Raw: 50.00%  Effective: 50.00%",
                ],
            )
