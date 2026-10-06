"""Engine initialization preserves upstream diagnostics."""

import logging
import unittest
from unittest.mock import Mock, patch

from lib.comparison.engine import load_engine
from lib.project import BUILD, TARGET_ID


class EngineTests(unittest.TestCase):
    def test_load_installs_lookups_without_hiding_upstream_warnings(self):
        target, engine = Mock(), Mock()

        def create_engine(selected_target):
            self.assertIs(selected_target, target)
            logging.getLogger("reccmp.compare.db").warning(
                "Match (%x, %x) collides with previous staged match",
                0x0040A830,
                0x00500000,
            )
            return engine

        with (
            patch("lib.comparison.engine.RecCmpProject.from_directory") as project,
            patch(
                "lib.comparison.engine.Compare.from_target", side_effect=create_engine
            ),
            patch("lib.comparison.engine.install_vtable_lookups") as install,
            patch("lib.comparison.engine.install_unique_lookups") as unique,
            patch("lib.comparison.engine.repair_guard_symbols") as guards,
            self.assertLogs("reccmp.compare.db", level="WARNING") as logs,
        ):
            project.return_value.get.return_value = target
            self.assertEqual(load_engine(), (target, engine))
            project.assert_called_once_with(BUILD)
            project.return_value.get.assert_called_once_with(TARGET_ID)
            install.assert_called_once_with(engine.function_comparator)
            unique.assert_called_once_with(engine.function_comparator)
            guards.assert_called_once_with(engine.function_comparator)
        self.assertEqual(len(logs.records), 1)
