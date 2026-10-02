"""Gate routing, ordering, and failure propagation."""

import contextlib
import io
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import DEFAULT, patch

import gate


class GateTests(unittest.TestCase):
    def test_upstream_annotation_checks(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / "Fixture.cpp"
            for target, last_address, expected in (
                ("LEMBALL", "0x00401020", 0),
                ("LEMBALL", "0x00401010", 1),
                ("LEMBALL", "0x00401000", 1),
                ("OTHER", "0x00401000", 0),
            ):
                source.write_text(
                    f"// FUNCTION: {target} 0x00401010\nvoid First() {{}}\n"
                    f"// FUNCTION: {target} {last_address}\nvoid Second() {{}}\n",
                    encoding="utf-8",
                )
                with (
                    self.subTest(target=target, address=last_address),
                    contextlib.redirect_stdout(io.StringIO()),
                ):
                    self.assertEqual(gate.check_annotations([source]), expected)

    def test_modes_and_first_failure(self):
        source = [
            "check_comments",
            "check_smell",
            "check_layout",
            "check_annotations",
            "check_names",
        ]
        cases = (
            ([], None, source + ["check_tool_tests"]),
            (["--path", "Fixture.cpp"], None, source),
            (["--names"], None, ["check_names"]),
            (["--names", "--path", "Fixture.cpp"], None, ["check_names"]),
            (["--vtable"], None, ["check_vtable"]),
            *[([], failure, source[: index + 1]) for index, failure in enumerate(source)],
            ([], "check_tool_tests", source + ["check_tool_tests"]),
            (["--names"], "check_names", ["check_names"]),
            (["--vtable"], "check_vtable", ["check_vtable"]),
        )
        calls = []
        for flags, failure, expected in cases:
            calls.clear()

            def result(name, failure=failure):
                calls.append(name)
                return 7 if name == failure else 0

            with (
                self.subTest(flags=flags, failure=failure),
                patch.object(sys, "argv", ["gate.py", *flags]),
                patch.multiple(
                    gate, **dict.fromkeys(source + ["check_tool_tests", "check_vtable"], DEFAULT)
                ) as checks,
            ):
                for name, check in checks.items():
                    check.side_effect = lambda *args, name=name, result=result, **kwargs: result(
                        name
                    )
                self.assertEqual(gate.main(), 7 if failure else 0)
                self.assertEqual(calls, expected)
                if "check_names" in calls:
                    self.assertEqual(
                        checks["check_names"].call_args.kwargs,
                        {"verbose": True} if "--names" in flags else {},
                    )
                    self.assertEqual(
                        checks["check_names"].call_args.args,
                        (["Fixture.cpp"] if "--path" in flags else None,),
                    )

    def test_conflicting_checks_rejected(self):
        for flags in (["--names", "--vtable"], ["--vtable", "--path", "Fixture.cpp"]):
            with (
                self.subTest(flags=flags),
                patch.object(sys, "argv", ["gate.py", *flags]),
                contextlib.redirect_stderr(io.StringIO()),
                self.assertRaises(SystemExit) as error,
            ):
                gate.main()
            self.assertEqual(error.exception.code, 2)
