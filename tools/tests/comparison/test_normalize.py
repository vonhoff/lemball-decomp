"""Acceptance and rejection cases decoded from real IA-32 instruction bytes."""

import unittest

from reccmp.compare.asm.parse import ParseAsm

from lib.comparison.normalize import normalize, number_placeholders


def canonical(hex_bytes):
    data = bytes.fromhex(hex_bytes)
    parser = ParseAsm()
    return normalize(parser.parse_asm(data, 0x1000), data, 0x1000)


class NormalizationTests(unittest.TestCase):
    def assertEquivalent(self, original, rebuilt):
        left, right = canonical(original), canonical(rebuilt)
        self.assertIsNotNone(left)
        self.assertEqual(left, right)

    def assertDifferent(self, original, rebuilt):
        left, right = canonical(original), canonical(rebuilt)
        self.assertTrue(left is None or right is None or left != right)

    def test_placeholder_numbers_preserve_aliasing_and_named_symbols(self):
        self.assertEqual(
            number_placeholders(
                ["mov eax, [<OFFSET9>]", "mov ebx, [<OFFSET12>]", "push <OFFSET9>"]
            ),
            ["mov eax, [<OFFSET1>]", "mov ebx, [<OFFSET2>]", "push <OFFSET1>"],
        )
        self.assertNotEqual(
            number_placeholders(["push <OFFSET1>", "push <OFFSET1>"]),
            number_placeholders(["push <OFFSET9>", "push <OFFSET12>"]),
        )
        self.assertNotEqual(
            number_placeholders(["call Target"]),
            number_placeholders(["call <OFFSET9>"]),
        )

    def test_placeholder_text_inside_symbols_and_strings_is_preserved(self):
        for name in ("Container<OFFSET9>::Method", "'<OFFSET9>' (STRING)"):
            with self.subTest(name=name):
                self.assertEqual(
                    number_placeholders(["push " + name], {name}), ["push " + name]
                )

    def test_independent_register_and_memory_loads_can_move_both_directions(self):
        self.assertEquivalent("8b03 8b0e c3", "8b0e 8b03 c3")
        self.assertEquivalent("b801000000 b902000000 c3", "b902000000 b801000000 c3")

    def test_register_read_write_and_write_write_hazards(self):
        for left, right in (
            ("8bc3 8bf0", "8bf0 8bc3"),
            ("8bc3 b802000000", "b802000000 8bc3"),
            ("8b03 83c304", "83c304 8b03"),
            ("8bc3 b001", "b001 8bc3"),
            ("8bc3 b401", "b401 8bc3"),
        ):
            with self.subTest(left=left):
                self.assertDifferent(left + " c3", right + " c3")

    def test_stores_stack_calls_and_implicit_instructions_are_barriers(self):
        for barrier in ("890e", "50", "5b", "e800100000", "99", "f7e3", "9f", "fd"):
            with self.subTest(barrier=barrier):
                self.assertDifferent("8b03 " + barrier + " c3", barrier + " 8b03 c3")

    def test_flags_and_segment_overrides_block_reordering(self):
        self.assertDifferent("83c001 83c301 c3", "83c301 83c001 c3")
        self.assertDifferent("648b03 8b0e c3", "8b0e 648b03 c3")
        self.assertDifferent("0f20c0 8b0e c3", "8b0e 0f20c0 c3")

    def test_block_entries_prevent_cross_branch_moves(self):
        self.assertDifferent("7402 8bc3 8bce c3", "7402 8bce 8bc3 c3")

    def test_zero_checks_and_branch_encoding_jitter(self):
        self.assertEquivalent("83f800 7401 90 c3", "85c0 0f8401000000 90 c3")
        self.assertEquivalent("31d2 3bfa 7e01 90 c3", "31d2 85ff 7e01 90 c3")
        self.assertEquivalent(
            "b900000000 3bd9 7501 90 c3", "b900000000 85db 7501 90 c3"
        )

    def test_branch_targets_must_match_instruction_boundaries(self):
        self.assertDifferent("83f800 7401 90 c3", "85c0 7400 90 c3")
        self.assertIsNone(canonical("7401 b801000000 c3"))
        self.assertIsNone(canonical("740f c3"))
        self.assertIsNone(canonical("e800000000 c3"))
        self.assertIsNone(canonical("ffe0"))
        self.assertIsNone(canonical("31d2 cd80 3bc2 c3"))

    def test_truncated_decoding_is_rejected_and_int3_padding_is_allowed(self):
        for code in ("", "b8", "85c0 b8", "85c0 cc 90"):
            with self.subTest(code=code):
                self.assertIsNone(canonical(code))
        self.assertEquivalent("83f800 c3 cccc", "85c0 c3 cc")

    def test_zero_facts_are_killed_by_partial_writes_and_control_flow(self):
        for between in ("b201", "b601", "42", "7400", "e800100000"):
            with self.subTest(between=between):
                self.assertDifferent(
                    "31d2 " + between + " 3bc2 c3", "31d2 " + between + " 85c0 c3"
                )
        # Branch bypasses the zero definition; the merge cannot inherit it.
        self.assertDifferent("7402 31d2 3bc2 c3", "7402 31d2 85c0 c3")
        # Zeroing only DX does not prove EDX zero.
        self.assertDifferent("6631d2 3bc2 c3", "6631d2 85c0 c3")

    def test_abi_preserved_zero_survives_call_but_volatile_zero_does_not(self):
        self.assertEquivalent("31ed e800100000 3bdd c3", "31ed e800100000 85db c3")
        self.assertDifferent("31c9 e800100000 3bd9 c3", "31c9 e800100000 85db c3")

    def test_af_consumers_prevent_cmp_test_normalization(self):
        for consumer in ("9f", "9c", "27", "37"):
            with self.subTest(consumer=consumer):
                self.assertDifferent(
                    "83f800 " + consumer + " c3", "85c0 " + consumer + " c3"
                )

    def test_stack_offsets_arithmetic_width_and_unknown_values_stay_distinct(self):
        for left, right in (
            ("83ec04 8b442408 83c404 c3", "83ec08 8b44240c 83c408 c3"),
            ("8d0439 c3", "03c7 c3"),
            ("66b9ffff c3", "b9ffffffff c3"),
            ("3bc2 c3", "85c0 c3"),
            ("83f801 c3", "85c0 c3"),
        ):
            with self.subTest(left=left):
                self.assertDifferent(left, right)

    def test_jump_tables_preserve_destinations_and_order(self):
        # jmp [eax*4 + 0x100c]; nop; ret; padding; two table entries.
        left = "ff24850c100000 90 c3 909090 07100000 08100000"
        right = "ff24850c100000 90 c3 909090 08100000 07100000"
        self.assertDifferent(left, right)
