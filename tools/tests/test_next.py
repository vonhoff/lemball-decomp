"""Refreshing a custom report must rank the file just written."""

import contextlib
import io
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

import next as ranking


class RefreshTests(unittest.TestCase):
    def test_refresh_uses_requested_report(self):
        with tempfile.TemporaryDirectory() as directory:
            report = Path(directory) / "custom.json"
            with patch("sys.argv", ["next.py", "--refresh", "--report", str(report), "--kind", "near"]), \
                 patch.object(ranking, "make_report", side_effect=lambda path: path.write_text("{}")) as refresh, \
                 patch.object(ranking, "load_report", return_value=[]) as load, \
                 contextlib.redirect_stdout(io.StringIO()):
                self.assertEqual(ranking.main(), 0)
            refresh.assert_called_once_with(report)
            load.assert_called_once_with(report)
