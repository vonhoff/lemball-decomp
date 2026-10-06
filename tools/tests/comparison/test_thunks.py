"""One-hop E9 resolution requires complete reads and a paired destination."""

import struct
import unittest
from types import SimpleNamespace
from unittest.mock import Mock, call

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
                image = SimpleNamespace(read=Mock(side_effect=[thunk, b"\xc3"]))
                self.assertEqual(
                    resolve_jump_thunk(image, {target: "paired"}, address), target
                )
                self.assertEqual(
                    image.read.call_args_list, [call(address, 5), call(target, 1)]
                )

    def test_invalid_fetch_addresses_and_paired_entries_are_not_read(self):
        image = SimpleNamespace(read=Mock())
        for address in (-1, 0xFFFFFFFC, 0x100000000, 0x2000):
            with self.subTest(address=address):
                self.assertIsNone(
                    resolve_jump_thunk(image, {0x2000: "paired"}, address)
                )
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

    def test_empty_or_unreadable_destination_is_unresolved(self):
        for result in (
            b"",
            InvalidVirtualAddressError(0x1005),
            InvalidVirtualReadError(0x1005),
        ):
            with self.subTest(result=result):
                image = SimpleNamespace(
                    read=Mock(side_effect=[b"\xe9\0\0\0\0", result])
                )
                self.assertIsNone(resolve_jump_thunk(image, {0x1005: "paired"}, 0x1000))

    def test_unreadable_thunk_is_unresolved(self):
        for error in (
            InvalidVirtualAddressError(0x1000),
            InvalidVirtualReadError(0x1000),
        ):
            with self.subTest(error=error):
                image = SimpleNamespace(read=Mock(side_effect=error))
                self.assertIsNone(resolve_jump_thunk(image, {0x2000: "paired"}, 0x1000))
