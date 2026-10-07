"""One-hop E9 resolution requires complete reads and a paired destination."""

import struct
import unittest
from types import SimpleNamespace
from unittest.mock import Mock

from reccmp.formats.exceptions import (
    InvalidVirtualAddressError,
    InvalidVirtualReadError,
)

from lib.comparison.thunks import resolve_jump_thunk


class JumpThunkTests(unittest.TestCase):
    def test_forward_backward_and_wrapped_targets_read_exactly_one_hop(self):
        for address, target in (
            (0x1000, 0x2000),
            (0x2000, 0x1000),
            (0xFFFFFFF0, 0x10),
            (0xFFFFFFFB, 0),
        ):
            with self.subTest(address=address, target=target):
                displacement = (
                    target - address - 5 + 0x80000000
                ) % 0x100000000 - 0x80000000
                thunk = b"\xe9" + struct.pack("<i", displacement)
                image = SimpleNamespace(read=Mock(return_value=thunk))
                self.assertEqual(
                    resolve_jump_thunk(image, {target: "paired"}, address), target
                )
                image.read.assert_called_once_with(address, 5)

    def test_paired_entries_are_not_read(self):
        image = SimpleNamespace(read=Mock())
        self.assertIsNone(resolve_jump_thunk(image, {0x2000: "paired"}, 0x2000))
        image.read.assert_not_called()

    def test_incomplete_or_non_e9_instructions_do_not_read_a_destination(self):
        for data in (
            b"",
            b"\xe9",
            b"\xe9\0\0\0",
            b"\xe8\0\0\0\0",
            b"\xeb\0\x90\x90\x90",
        ):
            with self.subTest(data=data):
                image = SimpleNamespace(read=Mock(return_value=data))
                self.assertIsNone(resolve_jump_thunk(image, {0x1005: "paired"}, 0x1000))
                image.read.assert_called_once_with(0x1000, 5)

    def test_unpaired_destination_is_not_read_or_followed(self):
        image = SimpleNamespace(read=Mock(return_value=b"\xe9\0\0\0\0"))
        self.assertIsNone(resolve_jump_thunk(image, {0x2000: "paired"}, 0x1000))
        image.read.assert_called_once_with(0x1000, 5)

    def test_unreadable_thunk_is_unresolved(self):
        for error in (
            InvalidVirtualAddressError(0x1000),
            InvalidVirtualReadError(0x1000),
        ):
            with self.subTest(error=error):
                image = SimpleNamespace(read=Mock(side_effect=error))
                self.assertIsNone(resolve_jump_thunk(image, {0x2000: "paired"}, 0x1000))
