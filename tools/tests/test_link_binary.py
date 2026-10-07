"""MSVC 4.00 linker adapter tests."""

import contextlib
import copy
import io
import struct
import subprocess
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import link_binary
from link_binary import CoffObject, DIR32, REL32, make_thunk_object, thunk_symbol


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


class LinkerThunkTests(unittest.TestCase):
    def setUp(self):
        self.thunk = {"symbol": thunk_symbol(0x4024E1), "target_symbol": "?DisplayHelp@@YAXXZ"}
        self.raw = make_thunk_object([self.thunk])
        self.obj = CoffObject(self.raw)
        self.entries = {0x403000: {"symbol": "__another_jump_entry",
                                  "target_symbol": self.thunk["target_symbol"]}}
        self.route = {"symbol": self.thunk["symbol"], "size": 5,
                      "fingerprint": self.obj.fingerprint(self.thunk["symbol"], 5),
                      "references": [{"offset": 0, "original_bytes": "e900000000", "thunk": 0x403000,
                                      "source_symbol": self.thunk["target_symbol"]}]}

    def test_entries_have_distinct_addresses_and_symbolic_targets(self):
        other = {"symbol": thunk_symbol(0x403000),
                 "target_symbol": self.thunk["target_symbol"]}
        obj = CoffObject(make_thunk_object([self.thunk, other]))
        self.assertNotEqual(obj.function(self.thunk["symbol"])[2],
                            obj.function(other["symbol"])[2])
        targets = []
        for section in obj.sections:
            self.assertEqual(obj.data[section[4]:section[4] + section[3]], b"\xe9\0\0\0\0")
        for i in (1, 2):
            _, (offset, index, kind) = next(obj.relocations(i))
            self.assertEqual((offset, kind), (1, REL32))
            targets.append(obj.symbols[index][0])
        self.assertEqual(targets, [self.thunk["target_symbol"]] * 2)

    def test_routing_changes_only_relocation_target_and_appends_symbol(self):
        updated = CoffObject(self.obj.redirect([self.route], self.entries))
        location, (offset, index, kind) = next(updated.relocations(1))
        self.assertEqual((offset, kind), (1, REL32))
        self.assertEqual(updated.symbols[index][0], "__another_jump_entry")
        self.assertEqual(updated.fingerprint(self.thunk["symbol"], 5),
                         self.obj.fingerprint(self.thunk["symbol"], 5))
        self.assertEqual(updated.data[20:self.obj.symbol_start][:location - 20 + 4],
                         self.raw[20:self.obj.symbol_start][:location - 20 + 4])

    def test_wrong_callee_opcode_addend_and_stale_function_are_rejected(self):
        wrong_target = copy.deepcopy(self.route)
        wrong_target["references"][0]["source_symbol"] = "?SetGameDefaults@@YAXXZ"
        with self.assertRaisesRegex(ValueError, "Unverified"):
            self.obj.redirect([wrong_target], self.entries)
        wrong_opcode = copy.deepcopy(self.route)
        wrong_opcode["references"][0]["original_bytes"] = "e800000000"
        with self.assertRaisesRegex(ValueError, "Unverified"):
            self.obj.redirect([wrong_opcode], self.entries)
        stale = copy.deepcopy(self.route)
        stale["fingerprint"] = "0" * 64
        with self.assertRaisesRegex(ValueError, "Stale"):
            self.obj.redirect([stale], self.entries)
        addend = bytearray(self.raw)
        addend[self.obj.sections[0][4] + 1] = 1
        with self.assertRaisesRegex(ValueError, "Unverified"):
            CoffObject(bytes(addend)).redirect([self.route], self.entries)

    def test_undefined_duplicate_does_not_hide_the_defined_function(self):
        route = copy.deepcopy(self.route)
        self.entries[0x403000]["symbol"] = self.thunk["symbol"]
        obj = CoffObject(self.obj.redirect([route], self.entries))
        self.assertEqual(obj.function(self.thunk["symbol"])[2], 1)

    def test_non_i386_and_oversized_extents_are_rejected(self):
        wrong_machine = bytearray(self.raw)
        struct.pack_into("<H", wrong_machine, 0, 0x8664)
        with self.assertRaises(ValueError):
            CoffObject(bytes(wrong_machine))
        with self.assertRaises(ValueError):
            self.obj.fingerprint(self.thunk["symbol"], 6)

    def test_callback_address_uses_dir32_and_keeps_a_distinct_entry(self):
        raw = bytearray(self.raw)
        raw[self.obj.sections[0][4]] = 0x68
        relocation, _ = next(self.obj.relocations(1))
        struct.pack_into("<H", raw, relocation + 8, DIR32)
        obj = CoffObject(bytes(raw))
        route = copy.deepcopy(self.route)
        route["fingerprint"] = obj.fingerprint(self.thunk["symbol"], 5)
        route["references"][0]["original_bytes"] = "6800000000"
        updated = CoffObject(obj.redirect([route], self.entries))
        _, (_, index, kind) = next(updated.relocations(1))
        self.assertEqual(kind, DIR32)
        self.assertEqual(updated.symbols[index][0], "__another_jump_entry")
        self.assertEqual(updated.data[updated.sections[0][4]], 0x68)
        self.entries[0x403000]["target_symbol"] = "?OtherCallback@@YAXXZ"
        with self.assertRaisesRegex(ValueError, "Unverified"):
            obj.redirect([route], self.entries)
