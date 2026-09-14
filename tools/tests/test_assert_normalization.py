"""CRT assertion-location normalization without debug-flag assumptions."""

import unittest

from lib.reccmp_compat import normalize_assert_arguments


class AssertNormalizationTests(unittest.TestCase):
    def normalize(self, lines):
        assembly = list(enumerate(lines))
        normalize_assert_arguments(assembly)
        self.assertEqual([address for address, _ in assembly], list(range(len(lines))))
        return [line for _, line in assembly]

    def arguments(self, line="0x72", file="handler.cpp", expression="new_p == 0"):
        return [f"push {line}", f'push "{file}" (STRING)',
                f'push "{expression}" (STRING)', "call __assert (FUNCTION)"]

    def test_file_and_line_normalize_but_expression_is_preserved(self):
        expected = ["push __LINE__", "push __FILE__",
                    'push "new_p == 0" (STRING)', "call __assert (FUNCTION)"]
        self.assertEqual(self.normalize(self.arguments()), expected)
        self.assertEqual(self.normalize(self.arguments("123", "elsewhere.cpp")), expected)
        self.assertNotEqual(self.normalize(self.arguments(expression="new_p != 0")), expected)

    def test_normalization_is_idempotent(self):
        once = self.normalize(self.arguments())
        self.assertEqual(self.normalize(once), once)

    def test_early_calls_do_not_rewrite_tail_instructions(self):
        for prefix in ([], ["push eax"], ["push eax", "push ebx"]):
            lines = prefix + ["call __assert (FUNCTION)", "mov eax, 1", "ret"]
            self.assertEqual(self.normalize(lines), lines)

    def test_unrelated_callees_are_not_assertions(self):
        for call in ("call user_assert (FUNCTION)", "call assert_helper (FUNCTION)",
                     "call __assert + 4 (OFFSET)", "call eax"):
            lines = self.arguments()[:-1] + [call]
            self.assertEqual(self.normalize(lines), lines)

    def test_non_argument_instructions_and_dynamic_locations_are_preserved(self):
        for index, replacement in ((0, "mov eax, 0x72"), (0, "push eax"),
                                   (1, "push eax"), (2, "mov ecx, 1")):
            lines = self.arguments()
            lines[index] = replacement
            self.assertEqual(self.normalize(lines), lines)

    def test_unresolved_literal_pointers_keep_existing_normalization(self):
        lines = ["push 4", "push <OFFSET1>", "push <OFFSET2>", "call _assert (IMPORT)"]
        self.assertEqual(self.normalize(lines),
                         ["push __LINE__", "push __FILE__", "push <OFFSET2>", "call _assert (IMPORT)"])
