"""Regression checks for calls hidden by multiline assembly replacements."""

import struct
import unittest
from types import SimpleNamespace

from recover_thunks import recover_references, unique_thunk_calls


def branch(opcode, address, target):
    return bytes([opcode]) + struct.pack("<I", (target - address - 5) & 0xFFFFFFFF)


class Image:
    def __init__(self, instructions):
        self.instructions = instructions

    def read(self, address, length):
        return self.instructions.get(address, b"\x90" * length)[:length]


class CallGroups(unittest.TestCase):
    def setUp(self):
        self.original = Image(
            {
                0x1004: branch(0xE8, 0x1004, 0x2000),
                0x1014: branch(0xE8, 0x1014, 0x2000),
                0x2000: branch(0xE9, 0x2000, 0x3000),
            }
        )
        self.rebuilt = Image(
            {
                0x4008: branch(0xE8, 0x4008, 0x5000),
                0x4020: branch(0xE8, 0x4020, 0x5000),
            }
        )
        self.diff = SimpleNamespace(
            orig_inst=[("0x1004", "call getter"), ("0x1014", "call getter")],
            recomp_inst=[("0x4008", "call getter"), ("0x4020", "call getter")],
            codes=[("replace", 0, 2, 0, 2)],
        )
        self.functions = {0x3000: SimpleNamespace(orig_addr=0x3000, recomp_addr=0x5000)}
        self.entries = {0x2000}
        self.aliases = {}

    def pairs(self):
        return list(
            unique_thunk_calls(
                self.diff,
                self.original,
                self.rebuilt,
                self.functions,
                self.entries,
                self.aliases,
            )
        )

    def test_multiline_replacement_preserves_two_call_occurrences(self):
        self.assertEqual(self.pairs(), [(0x1004, 0x4008), (0x1014, 0x4020)])

    def test_missing_rebuilt_call_is_not_paired(self):
        self.diff.recomp_inst.pop()
        self.assertEqual(self.pairs(), [])

    def test_extra_rebuilt_call_is_not_paired(self):
        self.diff.recomp_inst.append(("0x4030", "call getter"))
        self.rebuilt.instructions[0x4030] = branch(0xE8, 0x4030, 0x5000)
        self.assertEqual(self.pairs(), [])

    def test_distinct_native_thunks_to_same_body_are_ambiguous(self):
        self.entries.add(0x2010)
        self.original.instructions[0x2010] = branch(0xE9, 0x2010, 0x3000)
        self.original.instructions[0x1014] = branch(0xE8, 0x1014, 0x2010)
        self.assertEqual(self.pairs(), [])

    def test_mixed_direct_and_thunk_native_calls_are_ambiguous(self):
        self.original.instructions[0x1014] = branch(0xE8, 0x1014, 0x3000)
        self.assertEqual(self.pairs(), [])

    def test_paired_native_entry_is_not_recovered_as_a_thunk(self):
        self.functions[0x2000] = SimpleNamespace(orig_addr=0x2000, recomp_addr=0x6000)
        self.assertEqual(self.pairs(), [])

    def test_unrelated_rebuilt_callee_is_not_paired(self):
        self.rebuilt.instructions[0x4020] = branch(0xE8, 0x4020, 0x6000)
        self.assertEqual(self.pairs(), [])

    def test_immediate_pointer_is_not_a_call(self):
        self.diff.orig_inst[0] = ("0x1004", "push getter")
        self.original.instructions[0x1004] = b"\x68\x00\x20\x00\x00"
        self.assertEqual(self.pairs(), [])

    def test_shared_rebuilt_body_does_not_choose_an_original_callee(self):
        self.functions[0x3010] = SimpleNamespace(orig_addr=0x3010, recomp_addr=0x5000)
        self.entries.add(0x2010)
        self.original.instructions[0x2010] = branch(0xE9, 0x2010, 0x3010)
        self.original.instructions[0x1014] = branch(0xE8, 0x1014, 0x2010)
        self.assertEqual(self.pairs(), [])

    def test_evidenced_folded_alias_can_receive_a_call(self):
        self.aliases[0x3000] = {0x5000: {}, 0x5010: {}}
        self.rebuilt.instructions[0x4020] = branch(0xE8, 0x4020, 0x5010)
        self.assertEqual(self.pairs(), [(0x1004, 0x4008), (0x1014, 0x4020)])

    def recover(self, obj):
        engine = SimpleNamespace(
            orig_bin=self.original,
            recomp_bin=self.rebuilt,
            function_comparator=SimpleNamespace(
                compare_function=lambda _: SimpleNamespace(diff=self.diff)
            ),
        )
        return recover_references(
            engine,
            self.functions,
            self.entries,
            {"getter": 0x5000},
            obj,
            SimpleNamespace(orig_addr=0x1000, recomp_addr=0x4000),
            {"symbol": "caller"},
            None,
            self.aliases,
        )

    def test_new_pairs_still_require_coff_relocations(self):
        def missing(*_):
            raise ValueError("Missing COFF relocation")

        references, thunks = self.recover(SimpleNamespace(reference_target=missing))
        self.assertEqual((references, thunks), ([], {}))

    def test_unrelated_coff_symbol_is_not_redirected(self):
        references, thunks = self.recover(
            SimpleNamespace(
                reference_target=lambda *_: "unrelated",
                canonical_name=lambda name: name,
            )
        )
        self.assertEqual((references, thunks), ([], {}))

    def test_relocation_validated_calls_keep_native_byte_evidence(self):
        references, thunks = self.recover(
            SimpleNamespace(
                reference_target=lambda *_: "getter", canonical_name=lambda name: name
            )
        )
        self.assertEqual([r["offset"] for r in references], [8, 0x20])
        self.assertEqual(
            [r["original_instruction"] for r in references], [0x1004, 0x1014]
        )
        self.assertEqual([r["thunk"] for r in references], [0x2000, 0x2000])
        self.assertEqual(thunks[0x2000]["target"], 0x3000)
        for reference in references:
            self.assertEqual(
                bytes.fromhex(reference["original_bytes"]),
                self.original.read(reference["original_instruction"], 5),
            )


if __name__ == "__main__":
    unittest.main()
