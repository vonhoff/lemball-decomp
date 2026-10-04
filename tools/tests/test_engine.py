"""Known folded diagnostics must not hide unrelated comparison failures."""

import io
import logging
import unittest
from unittest.mock import Mock, patch

from lib import load_engine


class EngineTests(unittest.TestCase):
    def test_folded_warning_only_and_filter_cleanup(self):
        message = "Match (%x, %x) collides with previous staged match"
        for fail in (False, True):
            with self.subTest(fail=fail):
                output = io.StringIO()
                handler = logging.StreamHandler(output)
                logger = logging.getLogger("reccmp.compare.db")
                previous_filters = logger.filters[:]
                logger.addHandler(handler)
                target, engine = Mock(), Mock()

                def create(_target):
                    self.assertIs(_target, target)
                    logger.warning(message, 0x0040A830, 0x00500000)
                    logger.warning(message, 0x00401000, 0x00500000)
                    logger.warning("Different warning at %x", 0x0040A830)
                    logger.error(message, 0x0040A830, 0x00500000)
                    if fail:
                        raise RuntimeError("engine failed")
                    return engine

                try:
                    with (
                        patch(
                            "reccmp.project.detect.RecCmpProject.from_directory"
                        ) as project,
                        patch("reccmp.compare.Compare.from_target", side_effect=create),
                    ):
                        project.return_value.get.return_value = target
                        if fail:
                            with self.assertRaisesRegex(RuntimeError, "engine failed"):
                                load_engine()
                        else:
                            self.assertEqual(load_engine(), (target, engine))
                    self.assertEqual(logger.filters, previous_filters)
                    self.assertEqual(
                        output.getvalue().splitlines(),
                        [
                            "Match (401000, 500000) collides with previous staged match",
                            "Different warning at 40a830",
                            "Match (40a830, 500000) collides with previous staged match",
                        ],
                    )
                finally:
                    logger.removeHandler(handler)
