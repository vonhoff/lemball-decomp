"""Forward relocation must exclude the moved instruction from its own checks."""

import unittest
from unittest.mock import patch

from lib.reccmp_compat import _upstream_relocate_instructions, relocate_instructions
from reccmp.compare.pinned_sequences import SequenceMatcherWithPins
from reccmp.compare.asm import fixes


class ForwardRelocationTests(unittest.TestCase):
    def compare(self, first, crossed):
        original = [first, *crossed]
        rebuilt = [*crossed, first]
        codes = SequenceMatcherWithPins(original, rebuilt, []).get_opcodes()
        return original, rebuilt, codes

    def test_upstream_counts_moved_instruction_as_its_own_dependency(self):
        original, rebuilt, codes = self.compare("mov eax, 1", ["xor ecx, ecx", "inc ecx"])
        self.assertEqual(_upstream_relocate_instructions(codes, original, rebuilt), set())
        self.assertEqual(relocate_instructions(codes, original, rebuilt), {2})

    def test_effective_match_uses_the_corrected_dependency_check(self):
        original, rebuilt, codes = self.compare("mov eax, 1", ["xor ecx, ecx", "inc ecx"])
        with patch.object(fixes, "relocate_instructions", _upstream_relocate_instructions):
            self.assertFalse(fixes.find_effective_match(codes, original, rebuilt))
        with patch.object(fixes, "relocate_instructions", relocate_instructions):
            self.assertTrue(fixes.find_effective_match(codes, original, rebuilt))

    def test_backward_move_behavior_is_unchanged(self):
        rebuilt, original, _ = self.compare("mov eax, 1", ["xor ecx, ecx", "inc ecx"])
        codes = SequenceMatcherWithPins(original, rebuilt, []).get_opcodes()
        self.assertEqual(relocate_instructions(codes, original, rebuilt),
                         _upstream_relocate_instructions(codes, original, rebuilt))

    def test_rejects_destination_reads_and_source_changes(self):
        for first, crossed in (
            ("mov eax, 1", ["mov ecx, eax", "inc ecx"]),
            ("mov eax, ecx", ["inc ecx", "inc edx"]),
            ("mov eax, 1", ["inc al", "inc ecx"]),
            ("mov eax, 1", ["call helper (FUNCTION)", "inc ecx"]),
            ("mov eax, 1", ["jne 0x10", "inc ecx"]),
            ("mov eax, dword ptr [esi]", ["mov dword ptr [edi], 2", "inc ecx"]),
            ("mov eax, esp", ["push ecx", "inc ecx"]),
            ("mov eax, fs", ["mov fs, cx", "inc ecx"]),
            ("add eax, 1", ["xor ecx, ecx", "inc ecx"]),
        ):
            with self.subTest(first=first, crossed=crossed):
                original, rebuilt, codes = self.compare(first, crossed)
                self.assertEqual(relocate_instructions(codes, original, rebuilt), set())
