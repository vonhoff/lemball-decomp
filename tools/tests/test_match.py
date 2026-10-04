"""Upstream comparison display and build failure propagation."""

import contextlib
import copy
import io
import unittest
from unittest.mock import Mock, patch

from reccmp.compare.report import ReccmpComparedEntity

import match as matching


class MatchTests(unittest.TestCase):
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
                ["match.py", "401000", "401020", "401040", "--no-build", "--summary"],
            ),
            patch("match.load_engine", return_value=(None, engine)),
            patch(
                "match.additional_effective_matches", return_value={0x401000: ("Rule",)}
            ),
            patch("match.print_match_verbose") as verbose,
            contextlib.redirect_stdout(output),
        ):
            self.assertEqual(matching.main(), 1)
            verbose.assert_not_called()
        self.assertEqual(
            output.getvalue().splitlines(),
            [
                "0x00401000 Raw: 80.00%  Effective: 100.00% Equivalent",
                "0x00401020 Raw: 0.00%  Effective: 0.00% STUB Stub",
                "0x00401040: NOT_FOUND",
            ],
        )

    def test_failed_build_prevents_comparison(self):
        with (
            patch("sys.argv", ["match.py", "0x401000"]),
            patch("match.run_build", return_value=7),
            patch("match.load_engine") as load,
            contextlib.redirect_stdout(io.StringIO()),
        ):
            self.assertEqual(matching.main(), 7)
            load.assert_not_called()

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
                ["match.py", *[hex(c.orig_addr) for c in comparisons], "--no-build"],
            ),
            patch("match.load_engine", return_value=(None, engine)),
            patch(
                "match.additional_effective_matches", return_value={0x401020: ("Rule",)}
            ),
            patch("match.print_match_verbose") as display,
            contextlib.redirect_stdout(output),
        ):
            self.assertEqual(matching.main(), 0)
            shown = [call_args.args[0] for call_args in display.call_args_list]
            self.assertEqual([c.is_effective_match for c in shown], [True, True, False])
            self.assertEqual([c.accuracy for c in shown], [0.8, 0.75, 0.5])
            self.assertEqual(comparisons, unchanged)
            self.assertEqual(
                output.getvalue().splitlines(),
                [
                    "Raw: 80.00%  Effective: 100.00%",
                    "Raw: 75.00%  Effective: 100.00%",
                    "Raw: 50.00%  Effective: 50.00%",
                ],
            )
