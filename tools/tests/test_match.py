"""Upstream comparison display and build failure propagation."""

import contextlib
import io
import unittest
from unittest.mock import Mock, call, patch

from reccmp.compare.report import ReccmpComparedEntity

import match as matching


class MatchTests(unittest.TestCase):
    def test_build_modes_and_comparison_dispatch(self):
        addresses = [0x401000, 0x401020, 0x401040]
        for flags, build_code in (([], 0), (["--no-build"], 0), ([], 7)):
            engine = Mock()
            comparisons = [ReccmpComparedEntity(addresses[0], "Exact", 1.0),
                           ReccmpComparedEntity(addresses[1], "Stub", 1.0, is_stub=True), None]
            engine.compare_address.side_effect = comparisons
            output = io.StringIO()
            with (
                self.subTest(flags=flags, build_code=build_code),
                patch("sys.argv", ["match.py", *map(hex, addresses), *flags]),
                patch.object(matching, "run_build", return_value=build_code) as build,
                patch.object(matching, "load_engine", return_value=(None, engine)) as load,
                patch.object(matching, "print_match_verbose") as diff,
                patch.object(matching, "print_match_oneline") as summary,
                contextlib.redirect_stdout(output),
            ):
                self.assertEqual(matching.main(), build_code)
                if "--no-build" in flags:
                    build.assert_not_called()
                else:
                    build.assert_called_once_with()
                if build_code:
                    load.assert_not_called()
                    engine.compare_address.assert_not_called()
                    diff.assert_not_called()
                    summary.assert_not_called()
                    self.assertEqual(
                        output.getvalue(),
                        "BUILD_FAILED exit=7 (see build-msvc400/last_build.log)\n",
                    )
                else:
                    load.assert_called_once()
                    self.assertEqual(
                        engine.compare_address.call_args_list,
                        [call(address) for address in addresses],
                    )
                    diff.assert_called_once_with(comparisons[0])
                    summary.assert_called_once_with(comparisons[1])
                    self.assertEqual(output.getvalue(), "0x00401040: NOT_FOUND\n")

    def test_invalid_address_prevents_build(self):
        with (
            patch("sys.argv", ["match.py", "invalid"]),
            patch.object(matching, "run_build") as build,
            contextlib.redirect_stderr(io.StringIO()),
            self.assertRaises(SystemExit) as error,
        ):
            matching.main()
        self.assertEqual(error.exception.code, 2)
        build.assert_not_called()
