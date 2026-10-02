"""Raw match status, diff delegation, and build failure propagation."""

import contextlib
import io
import unittest
from unittest.mock import Mock, call, patch

from reccmp.compare.report import ReccmpComparedEntity

import match as matching


class MatchTests(unittest.TestCase):
    def test_comparison_command(self):
        comparisons = [
            ReccmpComparedEntity(0x401000, "Exact", 1.0),
            ReccmpComparedEntity(0x401020, "Effective", 0.8, is_effective_match=True),
            ReccmpComparedEntity(0x401040, "Stub", 1.0, is_stub=True),
            ReccmpComparedEntity(0x401060, "Partial", 0.25),
            ReccmpComparedEntity(0x401080, "ExactEffective", 1.0, is_effective_match=True),
            None,
        ]
        addresses = [0x401000 + index * 0x20 for index in range(len(comparisons))]
        expected = (
            "0x00401000 Exact: 100.00% ASM_EXACT\n"
            "0x00401020 Effective: 80.00% EFFECTIVE\n"
            "0x00401040 Stub: 0.00% STUB\n"
            "0x00401060 Partial: 25.00% PARTIAL\n"
            "0x00401080 ExactEffective: 100.00% ASM_EXACT\n"
            "0x004010a0: NOT_FOUND\n"
        )
        for flags, build_code in (
            ([], 0),
            (["--no-build", "--no-diff"], 0),
            ([], 7),
        ):
            engine = Mock()
            engine.compare_address.side_effect = comparisons
            output = io.StringIO()
            with (
                self.subTest(flags=flags, build_code=build_code),
                patch("sys.argv", ["match.py", *map(hex, addresses), *flags]),
                patch.object(matching, "run_build", return_value=build_code) as build,
                patch.object(matching, "load_engine", return_value=(None, engine)) as load,
                patch.object(matching, "print_match_verbose") as diff,
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
                    self.assertEqual(output.getvalue(), expected)
                    self.assertEqual(
                        diff.call_args_list,
                        [] if "--no-diff" in flags else [call(item) for item in comparisons[:-1]],
                    )

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
