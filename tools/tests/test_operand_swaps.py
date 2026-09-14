"""Operand swaps must preserve operands, not just their character counts."""

import unittest
from difflib import SequenceMatcher
from unittest.mock import patch

from reccmp.compare.asm import fixes
from lib.reccmp_compat import (
    _upstream_is_operand_swap,
    _upstream_patch_mov_compare_jmp,
    is_operand_swap,
    patch_mov_compare_jmp,
)


class OperandSwapTests(unittest.TestCase):
    def test_upstream_accepts_changed_offsets(self):
        left = "cmp eax, dword ptr [ebx + 0x12]"
        right = "cmp dword ptr [ebx + 0x21], eax"
        self.assertTrue(_upstream_is_operand_swap(left, right))
        self.assertFalse(is_operand_swap(left, right))

    def test_exact_swaps_preserve_embedded_commas(self):
        for operand in ("ebx", "dword ptr [ebx + 0x12]",
                        "dword ptr [Pair<int, short> (DATA)]", '"a, b" (STRING)'):
            with self.subTest(operand=operand):
                self.assertTrue(is_operand_swap("cmp eax, " + operand, "cmp " + operand + ", eax"))
        self.assertFalse(is_operand_swap("cmp eax, ebx", "cmp eax, ebx"))
        self.assertFalse(is_operand_swap("cmp eax, ebx", "test ebx, eax"))

    def test_changed_identifiers_and_literals_are_rejected(self):
        for left, right in (("item12 (DATA)", "item21 (DATA)"),
                            ('"ab" (STRING)', '"ba" (STRING)')):
            self.assertFalse(is_operand_swap("cmp eax, " + left, "cmp " + right + ", eax"))

    def test_mov_compare_checks_exact_sources(self):
        left = ["mov eax, dword ptr [ebp + 0x12]", "cmp eax, ecx", "ja 0x10"]
        right = ["mov eax, ecx", "cmp eax, dword ptr [ebp + 0x21]", "jb 0x10"]
        self.assertEqual(_upstream_patch_mov_compare_jmp(left, right, "cmp"), {0, 1, 2})
        self.assertEqual(patch_mov_compare_jmp(left, right, "cmp"), set())
        right[1] = "cmp eax, dword ptr [ebp + 0x12]"
        self.assertEqual(patch_mov_compare_jmp(left, right, "cmp"), {0, 1, 2})
        right[2] = "jb 0x20"
        self.assertEqual(patch_mov_compare_jmp(left, right, "cmp"), set())

    def test_changed_offset_cannot_receive_effective_match(self):
        left = ["cmp eax, dword ptr [ebx + 0x12]", "ja 0x10", "ret "]
        right = ["cmp dword ptr [ebx + 0x21], eax", "jb 0x10", "ret "]
        codes = SequenceMatcher(None, left, right).get_opcodes()
        with patch.object(fixes, "is_operand_swap", _upstream_is_operand_swap):
            self.assertTrue(fixes.find_effective_match(codes, left, right))
        with patch.object(fixes, "is_operand_swap", is_operand_swap):
            self.assertFalse(fixes.find_effective_match(codes, left, right))

    def test_valid_swap_still_receives_effective_match(self):
        left = ["cmp eax, dword ptr [ebx + 0x12]", "ja 0x10", "ret "]
        right = ["cmp dword ptr [ebx + 0x12], eax", "jb 0x10", "ret "]
        codes = SequenceMatcher(None, left, right).get_opcodes()
        with patch.object(fixes, "is_operand_swap", is_operand_swap):
            self.assertTrue(fixes.find_effective_match(codes, left, right))

    def test_mov_compare_false_match_is_rejected_end_to_end(self):
        left = ["mov eax, dword ptr [ebp + 0x12]", "cmp eax, ecx", "ja 0x10", "ret "]
        right = ["mov eax, ecx", "cmp eax, dword ptr [ebp + 0x21]", "jb 0x10", "ret "]
        codes = SequenceMatcher(None, left, right).get_opcodes()
        with patch.object(fixes, "patch_mov_compare_jmp", _upstream_patch_mov_compare_jmp):
            self.assertTrue(fixes.find_effective_match(codes, left, right))
        with patch.object(fixes, "patch_mov_compare_jmp", patch_mov_compare_jmp):
            self.assertFalse(fixes.find_effective_match(codes, left, right))


if __name__ == "__main__":
    unittest.main()
