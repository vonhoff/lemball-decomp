"""MSVC 4.00 linker adapter tests."""

import contextlib
import io
import subprocess
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import link_binary


class LinkBinaryTests(unittest.TestCase):
    def test_response_file_rewritten_and_linker_called(self):
        with tempfile.TemporaryDirectory() as temp:
            temp_dir = Path(temp)
            rsp = temp_dir / "objects.rsp"
            rsp.write_text("foo.obj bar.obj baz.obj", encoding="utf-8")

            with (
                patch("link_binary.win_short_path", side_effect=lambda p: p),
                patch("link_binary.subprocess.run") as mock_run,
                contextlib.redirect_stdout(io.StringIO()),
                contextlib.redirect_stderr(io.StringIO()),
            ):
                mock_run.return_value = subprocess.CompletedProcess(
                    ["link.exe"], 0, stdout="link succeeded"
                )
                code = link_binary.main(["link.exe", f"@{rsp}"])

            self.assertEqual(code, 0)
            self.assertEqual(
                rsp.read_text(encoding="utf-8"), "foo.obj\nbar.obj\nbaz.obj\n"
            )

    def test_linker_warning_fails(self):
        with (
            patch("link_binary.win_short_path", side_effect=lambda p: p),
            patch("link_binary.subprocess.run") as mock_run,
            contextlib.redirect_stdout(io.StringIO()),
            contextlib.redirect_stderr(io.StringIO()),
        ):
            mock_run.return_value = subprocess.CompletedProcess(
                ["link.exe"], 0, stdout="LINK : warning LNK4005: test warning\n"
            )
            code = link_binary.main(["link.exe", "test.obj"])
        self.assertEqual(code, 1)
