"""Gate rejects source-policy violations and catalog mismatches."""

import contextlib
import io
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import gate


class GateTests(unittest.TestCase):
    def test_source_failures_reach_exit_status(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "Fixture.cpp"
            for owner, body, expected in (
                ("CPadToButton", "", 0),
                ("CPadToButton", "*(int*)((char*)this + 0x34) = n;", 1),
                ("CFake", "", 1),
            ):
                path.write_text(
                    f"// FUNCTION: LEMBALL 0x0043a250\n"
                    f"{owner}::{owner}(int n) {{ {body} }}\n",
                    encoding="utf-8",
                )
                with (
                    self.subTest(owner=owner, body=body),
                    patch("sys.argv", ["gate.py", "--path", str(path)]),
                    contextlib.redirect_stdout(io.StringIO()),
                ):
                    self.assertEqual(gate.main(), expected)
