"""Register normalization must not erase changes to literals or identifiers."""

import unittest
from difflib import SequenceMatcher
from unittest.mock import patch

from reccmp.compare.asm import fixes

from lib.reccmp_compat import (
    _upstream_naive_register_replacement,
    naive_register_replacement,
)


class RegisterTokenTests(unittest.TestCase):
    def test_upstream_erases_different_string_literals(self):
        original = ['push "eax" (STRING)']
        rebuilt = ['push "ebx" (STRING)']
        self.assertEqual(_upstream_naive_register_replacement(original, rebuilt), {0})
        self.assertEqual(naive_register_replacement(original, rebuilt), set())

    def test_symbol_fragments_are_not_register_operands(self):
        for original, rebuilt in (
            ("call max (FUNCTION)", "call mbx (FUNCTION)"),
            ("mov eax, get_esi_state (DATA)", "mov ebx, get_edi_state (DATA)"),
            ("mov eax, ns::ax (DATA)", "mov ebx, ns::bx (DATA)"),
            ("call ?ax@@YAXXZ (FUNCTION)", "call ?bx@@YAXXZ (FUNCTION)"),
            ("call eax (FUNCTION)", "call ebx (FUNCTION)"),
            ("mov eax, ebx (DATA)", "mov eax, ecx (DATA)"),
        ):
            with self.subTest(original=original):
                self.assertEqual(naive_register_replacement([original], [rebuilt]), set())

    def test_literal_changes_cannot_receive_effective_match(self):
        for original, rebuilt in (
            ('mov eax, "eax" (STRING)', 'mov eax, "ebx" (STRING)'),
            ('call max (FUNCTION)', 'call mbx (FUNCTION)'),
        ):
            left, right = [original, "ret "], [rebuilt, "ret "]
            codes = SequenceMatcher(None, left, right).get_opcodes()
            with patch.object(fixes, "naive_register_replacement", _upstream_naive_register_replacement):
                self.assertTrue(fixes.find_effective_match(codes, left, right))
            with patch.object(fixes, "naive_register_replacement", naive_register_replacement):
                self.assertFalse(fixes.find_effective_match(codes, left, right))

    def test_register_width_rules_remain_unchanged(self):
        original = ["mov eax, dword ptr [esi + eax*4]", "mov ax, dx", "mov al, ch"]
        rebuilt = ["mov ebx, dword ptr [edi + ebx*4]", "mov bx, cx", "mov bl, dh"]
        self.assertEqual(naive_register_replacement(original, rebuilt), {0, 1, 2})
        self.assertEqual(naive_register_replacement(["mov ax, dx"], ["mov eax, edx"]), set())

    def test_escaped_quotes_do_not_expose_string_contents(self):
        original = [r'push "say \"eax\"" (STRING)', r'push "eax\\" (STRING)']
        rebuilt = [r'push "say \"ebx\"" (STRING)', r'push "ebx\\" (STRING)']
        self.assertEqual(naive_register_replacement(original, rebuilt), set())
        self.assertEqual(naive_register_replacement(original, original), {0, 1})

    def test_no_instruction_reordering_is_added(self):
        self.assertEqual(naive_register_replacement(
            ["mov eax, 1", "xor esi, esi"], ["xor edi, edi", "mov ebx, 1"]
        ), set())


if __name__ == "__main__":
    unittest.main()
