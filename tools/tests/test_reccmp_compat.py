"""Boundary and fidelity checks for the repository-local reccmp parser fix."""

import struct
import unittest
from difflib import SequenceMatcher
from types import SimpleNamespace
from unittest.mock import patch

from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from lib.reccmp_compat import (
    BoundedInstructGen,
    RelocationAwareParseAsm,
    _upstream_is_operand_swap,
    _upstream_naive_register_replacement,
    _upstream_patch_compare_jmp,
    _upstream_patch_mov_compare_jmp,
    _upstream_relocate_instructions,
    complete_original_extent,
    identical_folded_aliases,
    incremental_thunks,
    is_operand_swap,
    naive_register_replacement,
    normalize_assert_arguments,
    patch_compare_jmp,
    patch_mov_compare_jmp,
    relocate_instructions,
    size_original_match,
    table_end_expressions,
)
from reccmp.compare.asm import fixes, parse
from reccmp.compare.asm.instgen import InstructGen, SectionType
from reccmp.compare.asm.parse import ParseAsm
from reccmp.compare.db import ReccmpMatch
from reccmp.compare.functions import FunctionComparator
from reccmp.compare.pinned_sequences import SequenceMatcherWithPins
from reccmp.types import ImageId


def dispatch(table: int, register: int = 0) -> bytes:
    return bytes((0xFF, 0x24, 0x85 + register * 8)) + struct.pack("<I", table)


def embedded_table(start: int, value: int = 1) -> bytes:
    return (
        dispatch(start + 8) + b"\x90"
        + struct.pack("<4I", start + 30, start + 24, start + 30, start + 24)
        + b"\xb8" + struct.pack("<I", value) + b"\xc3"
        + b"\xb8\x02\x00\x00\x00\xc3"
    )


def normalized(blob: bytes, start: int) -> list[str]:
    with patch.object(parse, "InstructGen", BoundedInstructGen):
        parser = parse.ParseAsm(addr_test=lambda address: address == start + 8)
        return [text for _, text in parser.parse_asm(blob, start)]


class JumpTableBoundaryTests(unittest.TestCase):
    def test_embedded_table_does_not_include_following_instructions(self):
        start = 0x1000
        sections = BoundedInstructGen(embedded_table(start), start).sections
        tables = [s for s in sections if s.type == SectionType.ADDR_TAB]
        self.assertEqual([len(s.contents) for s in tables], [4])
        table_bytes = {address + i for s in tables for address, _ in s.contents for i in range(4)}
        code_bytes = {
            address + i for s in sections if s.type == SectionType.CODE
            for address, size, _, _ in s.contents for i in range(size)
        }
        self.assertFalse(table_bytes & code_bytes)
        self.assertEqual(table_bytes | code_bytes, set(range(start, start + 36)))

    def test_two_embedded_tables(self):
        start = 0x2000
        blob = (dispatch(start + 8) + b"\x90"
                + struct.pack("<4I", start + 24, start + 31, start + 24, start + 31)
                + dispatch(start + 32, 2) + b"\x90"
                + struct.pack("<2I", start + 40, start + 46)
                + b"\xb8\x01\x00\x00\x00\xc3\xb8\x02\x00\x00\x00\xc3")
        tables = [s for s in BoundedInstructGen(blob, start).sections if s.type == SectionType.ADDR_TAB]
        self.assertEqual([len(s.contents) for s in tables], [4, 2])

    def test_relocated_identical_code_remains_identical(self):
        self.assertEqual(normalized(embedded_table(0x1000), 0x1000),
                         normalized(embedded_table(0x3000), 0x3000))

    def test_instruction_difference_remains_visible(self):
        self.assertNotEqual(normalized(embedded_table(0x1000, 1), 0x1000),
                            normalized(embedded_table(0x3000, 9), 0x3000))

    def test_table_target_difference_remains_visible(self):
        blob = bytearray(embedded_table(0x3000))
        struct.pack_into("<I", blob, 8, 0x3018)
        self.assertNotEqual(normalized(embedded_table(0x1000), 0x1000), normalized(blob, 0x3000))

    def test_unaffected_sections_match_upstream(self):
        start = 0x4000
        fixtures = (
            b"\xb8\x01\x00\x00\x00\xc3",
            dispatch(start + 8) + b"\x90" + struct.pack("<2I", start, start),
            b"\x0f\xb6\x80" + struct.pack("<I", start + 8) + b"\xc3\x00\x01\x02",
        )
        for blob in fixtures:
            with self.subTest(blob=blob.hex()):
                self.assertEqual(BoundedInstructGen(blob, start).sections, InstructGen(blob, start).sections)


class TableEndComparisonTests(unittest.TestCase):
    def edx_loop(self, begin=0x4000, length=61 * 8):
        return (b"\xba" + struct.pack("<I", begin)
                + bytes.fromhex("8b4708 3902 741a 83c208 46 81fa")
                + struct.pack("<I", begin + length) + b"\x72\xf0")

    def test_edx_table_bound_keeps_base_and_length_not_adjacent_symbol(self):
        rendered = []
        for begin, adjacent in ((0x4000, "debug (DATA)"), (0x8000, "path (DATA)")):
            parser = RelocationAwareParseAsm(
                relocation_sites=(0x1001, 0x1012), addr_test=lambda address: True,
                name_lookup=lambda address, begin=begin, adjacent=adjacent, **kwargs:
                    {begin: "inputs (DATA)", begin + 61 * 8: adjacent}.get(address),
            )
            rendered.append([line for _, line in parser.parse_asm(self.edx_loop(begin), 0x1000)])
        self.assertEqual(*rendered)
        self.assertIn("cmp edx, inputs (DATA) + 0x1e8", rendered[0])

    def test_edx_scan_rejects_missing_relocations_and_changed_flow(self):
        for sites in ((), (0x1001,), (0x1012,)):
            self.assertEqual(table_end_expressions(self.edx_loop(), 0x1000, sites), {})
        for offset, value in ((5, 0x89), (6, 0x57), (8, 0x89), (10, 0x75), (11, 0),
                              (14, 0), (14, 0xff), (15, 0x42), (17, 0xf8), (23, 0xf1)):
            with self.subTest(offset=offset):
                blob = bytearray(self.edx_loop())
                blob[offset] = value
                self.assertEqual(table_end_expressions(blob, 0x1000, (0x1001, 0x1012)), {})
        for length in (0, -8, 7, 0x10008):
            self.assertEqual(table_end_expressions(self.edx_loop(length=length), 0x1000,
                                                  (0x1001, 0x1012)), {})
        self.assertEqual(table_end_expressions(b"\x68" + self.edx_loop(), 0x1000,
                                              (0x1002, 0x1013)), {})

    def loop(self, begin=0x4000, length=0x168):
        return (b"\xb8" + struct.pack("<I", begin)
                + b"\x39\x08\x74\x22\x83\xc0\x08\x46\x3d"
                + struct.pack("<I", begin + length) + b"\x72\xf1")

    def render(self, begin, adjacent_name, length=0x168, base_name="errors (DATA)"):
        def names(address, **kwargs):
            return {begin: base_name, begin + length: adjacent_name}.get(address)
        parser = RelocationAwareParseAsm(
            relocation_sites=(0x1001, 0x100E), name_lookup=names,
            addr_test=lambda address: True,
        )
        return [line for _, line in parser.parse_asm(self.loop(begin, length), 0x1000)]

    def test_adjacent_symbol_does_not_name_table_bound(self):
        original = self.render(0x4000, "next_original (DATA)")
        rebuilt = self.render(0x8000, "next_rebuilt (DATA)")
        self.assertEqual(original, rebuilt)
        self.assertIn("cmp eax, errors (DATA) + 0x168", original)

    def test_different_bounds_and_base_symbols_remain_distinct(self):
        original = self.render(0x4000, "next (DATA)")
        self.assertNotEqual(original, self.render(0x8000, "next (DATA)", length=0x160))
        self.assertNotEqual(original, self.render(0x8000, "next (DATA)", base_name="other (DATA)"))

    def test_requires_both_operand_relocations(self):
        for sites in ((), (0x1001,), (0x100E,)):
            with self.subTest(sites=sites):
                self.assertEqual(table_end_expressions(self.loop(), 0x1000, sites), {})

    def test_declines_changed_control_flow_and_pointer_updates(self):
        for offset, value in ((5, 0x89), (7, 0x75), (8, 0), (9, 0x81),
                              (11, 0), (11, 0xFF), (12, 0x40), (18, 0x73), (19, 0xF0)):
            blob = bytearray(self.loop())
            blob[offset] = value
            with self.subTest(offset=offset, value=value):
                self.assertEqual(table_end_expressions(blob, 0x1000, (0x1001, 0x100E)), {})

    def test_declines_invalid_extent_and_embedded_opcode(self):
        for length in (0, -8, 7, 0x10008):
            with self.subTest(length=length):
                self.assertEqual(table_end_expressions(self.loop(length=length), 0x1000,
                                                      (0x1001, 0x100E)), {})
        # The apparent MOV starts inside a PUSH immediate, not at an instruction.
        blob = b"\x68" + self.loop()
        self.assertEqual(table_end_expressions(blob, 0x1000, (0x1002, 0x100F)), {})


class PointerComparisonTests(unittest.TestCase):
    def test_upstream_reproduces_missing_pointer_normalization(self):
        blob = b"\x3d" + struct.pack("<I", 0x4000)
        upstream = parse.ParseAsm(addr_test=lambda address: True)
        self.assertEqual(upstream.parse_asm(blob, 0x1000)[0][1], "cmp eax, 0x4000")

    def render(self, value, sites=(), name_lookup=None, opcode=b"\x3d", start=0x1000):
        parser = RelocationAwareParseAsm(
            relocation_sites=sites, name_lookup=name_lookup,
            addr_test=lambda address: True,
        )
        return [text for _, text in parser.parse_asm(opcode + struct.pack("<I", value), start)]

    def test_relocated_pointer_comparisons_match(self):
        self.assertEqual(self.render(0x4000, (0x1001,)),
                         self.render(0x8000, (0x2001,), start=0x2000))
        self.assertEqual(self.render(0x4000, (0x1001,)), ["cmp eax, <OFFSET1>"])

    def test_equal_value_relocated_elsewhere_does_not_hide_constants(self):
        self.assertNotEqual(self.render(0x4000, (0x9999,)), self.render(0x8000, (0x9999,)))
        self.assertEqual(self.render(0x4000), ["cmp eax, 0x4000"])

    def test_named_pointer_targets_remain_distinct(self):
        def names(address, **kwargs):
            return {0x4000: "first (DATA)", 0x8000: "second (DATA)"}.get(address)
        self.assertNotEqual(self.render(0x4000, (0x1001,), names),
                            self.render(0x8000, (0x1001,), names))

    def test_non_immediate_relocation_does_not_hide_constant(self):
        parser = RelocationAwareParseAsm(relocation_sites=(0x1002,))
        blob = b"\x81\x3d" + struct.pack("<II", 0x4000, 0x8000)
        self.assertEqual(parser.parse_asm(blob, 0x1000)[0][1],
                         "cmp dword ptr [<OFFSET1>], 0x8000")

    def test_named_comparison_counts_toward_later_placeholders(self):
        def names(address, **kwargs):
            return "limit (DATA)" if address == 0x4000 else None
        parser = RelocationAwareParseAsm(relocation_sites=(0x1001,), name_lookup=names,
                                         addr_test=lambda address: True)
        blob = b"\x3d" + struct.pack("<I", 0x4000) + b"\xb8" + struct.pack("<I", 0x5000)
        self.assertEqual([text for _, text in parser.parse_asm(blob, 0x1000)],
                         ["cmp eax, limit (DATA)", "mov eax, <OFFSET2>"])


class OperandSwapTests(unittest.TestCase):
    def test_test_operand_swap_cannot_reverse_branch(self):
        left = ["test eax, ebx", "ja 0x10", "ret "]
        right = ["test ebx, eax", "jb 0x10", "ret "]
        self.assertEqual(_upstream_patch_compare_jmp(left, right, "test"), {0, 1})
        self.assertEqual(patch_compare_jmp(left, right, "test"), set())
        codes = SequenceMatcher(None, left, right).get_opcodes()
        with patch.object(fixes, "patch_compare_jmp", _upstream_patch_compare_jmp):
            self.assertTrue(fixes.find_effective_match(codes, left, right))
        with patch.object(fixes, "patch_compare_jmp", patch_compare_jmp):
            self.assertFalse(fixes.find_effective_match(codes, left, right))

    def test_mov_test_operand_swap_cannot_reverse_branch(self):
        left = ["mov eax, dword ptr [ebp + 4]", "test eax, ecx", "ja 0x10", "ret "]
        right = ["mov eax, ecx", "test eax, dword ptr [ebp + 4]", "jb 0x10", "ret "]
        self.assertEqual(_upstream_patch_mov_compare_jmp(left, right, "test"), {0, 1, 2})
        self.assertEqual(patch_mov_compare_jmp(left, right, "test"), set())
        codes = SequenceMatcher(None, left, right).get_opcodes()
        with patch.object(fixes, "patch_mov_compare_jmp", _upstream_patch_mov_compare_jmp):
            self.assertTrue(fixes.find_effective_match(codes, left, right))
        with patch.object(fixes, "patch_mov_compare_jmp", patch_mov_compare_jmp):
            self.assertFalse(fixes.find_effective_match(codes, left, right))

    def test_existing_test_equality_swaps_remain_valid(self):
        for jump in ("je 0x10", "jne 0x10"):
            self.assertEqual(patch_compare_jmp(
                ["test eax, ebx", jump], ["test ebx, eax", jump], "test"), {0, 1})
            self.assertEqual(patch_mov_compare_jmp(
                ["mov eax, dword ptr [ebp + 4]", "test eax, ecx", jump],
                ["mov eax, ecx", "test eax, dword ptr [ebp + 4]", jump], "test"), {0, 1, 2})

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


class FoldedCallTests(unittest.TestCase):
    def test_alias_requires_equal_size_and_identical_bytes(self):
        bodies = {0x2000: b"\xb8\x01\x00\x00\x00\xc3",
                  0x3000: b"\xb8\x01\x00\x00\x00\xc3",
                  0x4000: b"\xb8\x02\x00\x00\x00\xc3",
                  0x5000: b"\xc3"}
        match = SimpleNamespace(recomp_addr=0x2000, size=lambda side: 6)
        engine = SimpleNamespace(
            code_files=[], target_id="LEMBALL", project_aliases={},
            _db=SimpleNamespace(
                get_one_match=lambda original: match,
                get=lambda side, address: SimpleNamespace(size=lambda side: len(bodies[address])),
            ),
            recomp_bin=SimpleNamespace(read=lambda address, size: bodies[address][:size]),
        )
        with patch("reccmp.parser.codebase.DecompCodebase"), patch(
            "lib.vtable.collect_folded_aliases", return_value={0x1000: {0x3000, 0x4000, 0x5000}}
        ):
            self.assertEqual(identical_folded_aliases(engine), {0x3000: 0x2000})

    def test_only_direct_calls_use_alias_identity(self):
        names = {0x2000: "Primary (FUNCTION)", 0x3000: "Folded (FUNCTION)"}
        parser = RelocationAwareParseAsm(
            function_aliases={0x3000: 0x2000},
            addr_test=lambda address: address in names,
            name_lookup=lambda address, **kwargs: names.get(address),
        )
        self.assertEqual(parser.sanitize((0x4000, 5, "call", "0x3000")),
                         ("call", "Primary (FUNCTION)"))
        self.assertEqual(parser.sanitize((0x4000, 5, "push", "0x3000")),
                         ("push", "Folded (FUNCTION)"))
        self.assertEqual(parser.sanitize((0x4000, 2, "call", "eax")), ("call", "eax"))


class IncrementalThunkTests(unittest.TestCase):
    def image(self, data):
        return SimpleNamespace(get_code_regions=lambda: [SimpleNamespace(addr=0x1000, data=data)])

    def test_stripped_table_requires_padding_and_forward_code_targets(self):
        jump = b"\xe9" + struct.pack("<i", 0x1020 - 0x1005)
        valid = jump + b"\xcc" * 27 + b"\xc3"
        self.assertEqual(incremental_thunks(self.image(valid)), {0x1000: 0x1020})
        for invalid in (jump, jump + b"\x90" * 27 + b"\xc3",
                        b"\x90" + valid, b"\xe9\xfb\xff\xff\xff" + valid[5:],
                        b"\xe9\xff\xff\xff\x7f" + valid[5:]):
            with self.subTest(data=invalid):
                self.assertEqual(incremental_thunks(self.image(invalid)), {})

    def parser(self, names):
        return RelocationAwareParseAsm(
            thunk_targets={0x1000: 0x1020},
            name_lookup=lambda address, **kwargs: names.get(address),
        )

    def test_call_uses_matched_destination_identity(self):
        parser = self.parser({0x1020: "Target (FUNCTION)"})
        self.assertEqual(parser.sanitize((0x2000, 5, "call", "0x1000")),
                         parser.sanitize((0x2000, 5, "call", "0x1020")))

    def test_existing_identity_is_preserved(self):
        parser = self.parser({0x1000: "Existing (FUNCTION)", 0x1020: "Target (FUNCTION)"})
        self.assertEqual(parser.sanitize((0x2000, 5, "call", "0x1000")),
                         ("call", "Existing (FUNCTION)"))

    def test_indirect_thunk_requires_known_target_and_preserves_existing_identity(self):
        for names, expected in (({0x1020: "Target (FUNCTION)"}, "->Target (FUNCTION)"),
                                ({0x3000: "Existing", 0x1020: "Target (FUNCTION)"}, "Existing"),
                                ({}, "<OFFSET1>")):
            parser = RelocationAwareParseAsm(
                indirect_thunk_targets={0x3000: 0x1020},
                name_lookup=lambda address, n=names, **kwargs: n.get(address),
            )
            self.assertEqual(parser.indirect_replace(0x3000), expected)
            self.assertTrue(parser.indirect_replace(0x4000).startswith("<OFFSET"))

    def test_unknown_destination_and_indirect_calls_are_unchanged(self):
        for names, operand in (({}, "0x1000"), ({0x1020: "Target (FUNCTION)"}, "eax")):
            parser = self.parser(names)
            expected = RelocationAwareParseAsm(name_lookup=lambda address, n=names, **kwargs: n.get(address))
            instruction = (0x2000, 5, "call", operand)
            self.assertEqual(parser.sanitize(instruction), expected.sanitize(instruction))


class BytesImage:
    def __init__(self, data, base=0x1000):
        self.data = data
        self.base = base

    def read(self, address, size):
        offset = address - self.base
        if not 0 <= offset < len(self.data):
            raise ValueError("Address is outside the test image")
        return self.data[offset:offset + size]


class OriginalExtentTests(unittest.TestCase):
    def setUp(self):
        self.decoder = Cs(CS_ARCH_X86, CS_MODE_32)

    def extent(self, data, limit=None):
        return complete_original_extent(
            BytesImage(data), 0x1000, 0x1000 + (limit or len(data)), self.decoder
        )

    def test_original_tail_is_included_without_changing_inventory(self):
        image = BytesImage(bytes.fromhex("b8 01000000 40 c3 cc cc"))
        match = ReccmpMatch(0x1000, 0x2000, {"recomp_size": 6, "orig_max_size": 9})
        extended = size_original_match(match, image, self.decoder)
        self.assertEqual(extended.size(ImageId.ORIG), 7)
        self.assertEqual(extended.size(ImageId.RECOMP), 6)
        self.assertIsNone(match.size(ImageId.ORIG))
        self.assertEqual(match.any_size(), extended.any_size())

    def test_prefix_only_rebuild_cannot_receive_a_false_exact_match(self):
        original = bytes.fromhex("b8 01000000 40 c3")
        comparator = object.__new__(FunctionComparator)
        comparator.orig_bin = BytesImage(original)
        comparator.recomp_bin = BytesImage(original[:5], base=0x2000)
        comparator.orig_sanitize = ParseAsm()
        comparator.recomp_sanitize = ParseAsm()
        comparator._collect_line_annotations = lambda _: []
        comparator._source_ref_of_recomp_addr = lambda _: None
        match = ReccmpMatch(0x1000, 0x2000, {"recomp_size": 5, "orig_max_size": 7})
        self.assertEqual(comparator.compare_function(match).match_ratio, 1.0)
        fixed = comparator.compare_function(
            size_original_match(match, comparator.orig_bin, self.decoder)
        )
        self.assertLess(fixed.match_ratio, 1.0)
        self.assertFalse(fixed.is_effective_match)

    def test_only_complete_alignment_nops_are_trimmed(self):
        body = bytes.fromhex("b8 01000000 c3")
        for suffix in ("90 " * 10, "8b ff 8d 64 24 00 90 90 90 90"):
            image = BytesImage(body + bytes.fromhex(suffix))
            match = ReccmpMatch(0x1000, 0x2000, {"recomp_size": 8, "orig_max_size": 16})
            sized = size_original_match(match, image, self.decoder)
            self.assertEqual(sized.size(ImageId.ORIG), len(body))
            self.assertIsNone(match.size(ImageId.ORIG))
            self.assertEqual(sized.any_size(), match.any_size())

    def test_trailing_code_and_uncertain_boundaries_are_not_trimmed(self):
        for data, maximum in (
            ("c3 40 " + "90 " * 14, 16),   # Real instruction after the return.
            ("c3 " + "90 " * 14, 15),      # Not a complete alignment boundary.
            ("c3 " + "90 " * 31, 32),      # More than one alignment gap.
            ("c3 " + "90 " * 13 + "8b", 16),  # Truncated instruction/read.
            ("eb fe " + "90 " * 14, 16),   # No closed return path.
        ):
            image = BytesImage(bytes.fromhex(data))
            match = ReccmpMatch(0x1000, 0x2000, {"recomp_size": 8, "orig_max_size": maximum})
            self.assertIs(size_original_match(match, image, self.decoder), match)

    def test_all_conditional_paths_are_followed(self):
        # The second return is beyond the first return and two padding bytes.
        self.assertEqual(self.extent(bytes.fromhex("74 03 c3 cc cc 40 c3")), 7)

    def test_calls_keep_the_fallthrough_path(self):
        self.assertEqual(self.extent(bytes.fromhex("e8 00000000 40 c3")), 7)

    def test_uncertain_paths_do_not_supply_a_size(self):
        for data in (
            "ff e0",             # indirect dispatch
            "eb 7f",             # branch outside the known bound
            "74 01 b8 00000000 c3",  # branch into an instruction
            "eb fe",             # no return evidence
            "90 0f",             # incomplete instruction
            "74 01 c3 cc",       # one path terminates in an interrupt
        ):
            with self.subTest(data=data):
                self.assertIsNone(self.extent(bytes.fromhex(data)))
        self.assertIsNone(self.extent(bytes.fromhex("90 90 c3"), limit=2))

    def test_explicit_sizes_and_nonextensions_keep_upstream_behavior(self):
        image = BytesImage(bytes.fromhex("90 c3 cc cc"))
        for metadata in (
            {"orig_size": 2, "recomp_size": 1, "orig_max_size": 4},
            {"recomp_size": 3, "orig_max_size": 4},
            {"recomp_size": 1},
            {"recomp_size": 4, "orig_max_size": 4},
        ):
            with self.subTest(metadata=metadata):
                match = ReccmpMatch(0x1000, 0x2000, metadata)
                self.assertIs(size_original_match(match, image, self.decoder), match)

    def switch(self, near=False):
        data = bytearray(b"\xcc" * 0xa0)
        guard = (bytes.fromhex("3d 01000000 0f87") + struct.pack("<i", 0x40 - 11)
                 if near else bytes.fromhex("83f801 773b"))
        dispatch = (bytes.fromhex("33c9 8a88") + struct.pack("<I", 0x1090)
                    + bytes.fromhex("ff248d") + struct.pack("<I", 0x1080))
        data[:len(guard + dispatch)] = guard + dispatch
        data[0x40] = 0xc3
        data[0x42:0x44] = bytes.fromhex("40 c3")
        data[0x44:0x46] = bytes.fromhex("41 c3")
        struct.pack_into("<II", data, 0x80, 0x1042, 0x1044)
        data[0x90:0x92] = bytes((0, 1))
        return data

    def test_bounded_switch_includes_every_case_and_both_tables(self):
        for near in (False, True):
            with self.subTest(near=near):
                self.assertEqual(self.extent(self.switch(near)), 0x92)
        image = BytesImage(self.switch())
        match = ReccmpMatch(0x1000, 0x2000, {"recomp_size": 0x20, "orig_max_size": 0xa0})
        extended = size_original_match(match, image, self.decoder)
        self.assertEqual(extended.size(ImageId.ORIG), 0x92)
        self.assertEqual(extended.any_size(), match.any_size())

    def direct_switch(self, near=False):
        data = self.switch(near)
        cursor = 11 if near else 5
        data[cursor:cursor + 15] = b"\xcc" * 15
        data[cursor:cursor + 7] = bytes.fromhex("ff2485") + struct.pack("<I", 0x1080)
        return data

    def test_direct_switch_includes_all_targets_and_table(self):
        for near in (False, True):
            with self.subTest(near=near):
                self.assertEqual(self.extent(self.direct_switch(near)), 0x88)

    def switch_with_guard_store(self, near=False):
        data = self.direct_switch(near)
        compare_size, guard_size = (5, 11) if near else (3, 5)
        dispatch = bytes(data[compare_size:guard_size + 7])
        data[compare_size:guard_size + 12] = bytes.fromhex("66 89 5c 24 30") + dispatch
        # Moving JA five bytes forward reduces its displacement by five.
        if near:
            struct.pack_into("<i", data, compare_size + 7, 0x40 - guard_size - 5)
        else:
            data[guard_size + 4] -= 5
        return data

    def test_guard_stack_store_preserves_range_check(self):
        for near in (False, True):
            with self.subTest(near=near):
                self.assertEqual(self.extent(self.switch_with_guard_store(near)), 0x88)

    def test_guard_store_rejects_flag_or_index_changes(self):
        for replacement in ("66 01 5c 24 30", "66 8b 44 24 30", "66 89 c0 90 90",
                            "66 89 5c 85 30", "66 89 1d 24 30"):
            with self.subTest(replacement=replacement):
                data = self.switch_with_guard_store()
                data[3:8] = bytes.fromhex(replacement)
                self.assertIsNone(self.extent(data))

    def test_guard_store_cannot_be_entered_without_comparison(self):
        for offset in (3, 8, 10):
            with self.subTest(offset=offset):
                data = self.switch_with_guard_store()
                data[0x40:0x42] = bytes((0xeb, (offset - 0x42) & 0xff))
                self.assertIsNone(self.extent(data))

    def test_guard_store_keeps_table_bounds_and_target_checks(self):
        self.assertIsNone(self.extent(self.switch_with_guard_store(), limit=0x87))
        for target in (0x0fff, 0x10a0, 0x1080, 0x1004):
            with self.subTest(target=target):
                data = self.switch_with_guard_store()
                struct.pack_into("<I", data, 0x84, target)
                self.assertIsNone(self.extent(data))

    def test_direct_switch_requires_matching_guard_and_index(self):
        for offset, value in ((1, 0xfa), (2, 0xff), (3, 0x7f), (7, 0x8d), (7, 0x45)):
            with self.subTest(offset=offset, value=value):
                data = self.direct_switch()
                data[offset] = value
                self.assertIsNone(self.extent(data))

    def test_direct_switch_requires_complete_table_inside_bound(self):
        self.assertIsNone(self.extent(self.direct_switch(), limit=0x87))
        for pointer in (0x0ffc, 0x109c, 0x1000):
            with self.subTest(pointer=pointer):
                data = self.direct_switch()
                struct.pack_into("<I", data, 8, pointer)
                self.assertIsNone(self.extent(data))

    def test_direct_switch_checks_every_target_and_case_path(self):
        for target in (0x0fff, 0x10a0, 0x1080, 0x1005, 0x1043, 0x1060):
            with self.subTest(target=target):
                data = self.direct_switch()
                data[0x42:0x48] = bytes.fromhex("b8 01000000 c3")
                data[0x60:0x62] = bytes.fromhex("ff e0")
                struct.pack_into("<I", data, 0x84, target)
                self.assertIsNone(self.extent(data))

    def test_direct_switch_external_edge_cannot_bypass_guard(self):
        data = self.direct_switch()
        # Default returns to the indirect jump, bypassing the proven range check.
        data[0x40:0x42] = bytes.fromhex("eb c3")
        self.assertIsNone(self.extent(data))

    def test_switch_requires_unsigned_guard_and_zero_extended_index(self):
        for offset, value in ((3, 0x7f), (2, 0xff), (5, 0x90), (8, 0x89), (16, 0x85)):
            with self.subTest(offset=offset):
                data = self.switch()
                data[offset] = value
                self.assertIsNone(self.extent(data))

    def indexed_switch(self, register):
        data = self.switch()
        data[1], data[8] = {"ecx": (0xf9, 0x81), "edx": (0xfa, 0x82), "edi": (0xff, 0x87)}[register]
        data[6] = 0xc0  # XOR EAX,EAX
        data[15] = 0x85  # JMP [EAX*4+target_table]
        return data

    def test_only_adjacent_default_entry_extends_the_target_table(self):
        for target, table_end in ((0x1040, 0x108c), (0x1042, 0x1088), (0x90909090, 0x1088)):
            with self.subTest(target=target):
                data = self.indexed_switch("ecx")
                struct.pack_into("<I", data, 9, 0x108c)
                struct.pack_into("<I", data, 0x88, target)
                data[0x8c:0x8e] = bytes((0, 1))
                bounds = {}
                extent = complete_original_extent(
                    BytesImage(data), 0x1000, 0x10a0, self.decoder, table_bounds=bounds
                )
                self.assertEqual(extent, 0x8e)
                self.assertEqual(bounds[0x1080], table_end)

    def test_indexed_switch_includes_complete_tables(self):
        for register in ("ecx", "edx", "edi"):
            with self.subTest(register=register):
                self.assertEqual(self.extent(self.indexed_switch(register)), 0x92)

    def test_indexed_switch_rejects_inconsistent_registers(self):
        for register in ("ecx", "edx", "edi"):
            for offset, value in ((1, 0xf8), (6, 0xc9), (8, 0x80), (15, 0x8d)):
                with self.subTest(register=register, offset=offset):
                    data = self.indexed_switch(register)
                    data[offset] = value
                    self.assertIsNone(self.extent(data))

    def test_indexed_switch_keeps_bounds_and_control_flow_checks(self):
        for register in ("ecx", "edx", "edi"):
            with self.subTest(register=register):
                self.assertIsNone(self.extent(self.indexed_switch(register), limit=0x91))
                for target in (0x0fff, 0x10a0, 0x1080, 0x1090, 0x1005, 0x1043):
                    data = self.indexed_switch(register)
                    data[0x42:0x48] = bytes.fromhex("b8 01000000 c3")
                    struct.pack_into("<I", data, 0x84, target)
                    self.assertIsNone(self.extent(data))
                data = self.indexed_switch(register)
                data[0x40:0x42] = bytes.fromhex("eb c3")
                self.assertIsNone(self.extent(data))

    def test_switch_tables_must_be_complete_disjoint_and_within_bound(self):
        for location, pointer in ((9, 0x0ff0), (9, 0x10a0), (16, 0x109c),
                                  (9, 0x1080), (16, 0x1000)):
            with self.subTest(location=location, pointer=pointer):
                data = self.switch()
                struct.pack_into("<I", data, location, pointer)
                self.assertIsNone(self.extent(data))
        self.assertIsNone(self.extent(self.switch(), limit=0x91))

    def test_switch_rejects_targets_outside_code_or_into_dispatch(self):
        for target in (0x0fff, 0x10a0, 0x1080, 0x1090, 0x1005, 0x1043):
            with self.subTest(target=target):
                data = self.switch()
                # Second case overlaps the interior of a multibyte first case.
                data[0x42:0x48] = bytes.fromhex("b8 01000000 c3")
                struct.pack_into("<I", data, 0x84, target)
                self.assertIsNone(self.extent(data))

    def test_switch_requires_closed_case_paths(self):
        data = self.switch()
        data[0x44:0x46] = bytes.fromhex("ff e0")
        self.assertIsNone(self.extent(data))

    def test_external_edge_cannot_bypass_switch_guard(self):
        data = self.switch()
        dispatch = bytes(data[:20])
        data[:0x24] = b"\xcc" * 0x24
        data[:5] = b"\xe9" + struct.pack("<i", 0xa0 - 5)
        data[0x10:0x24] = dispatch
        data[0x14] = 0x2b  # Moved guard still branches to the default at 0x1040.
        # One successor enters the dispatch directly; the other reaches its guard.
        data.extend(b"\x0f\x84" + struct.pack("<i", 0x15 - 0xa6)
                    + b"\xe9" + struct.pack("<i", 0x10 - 0xab))
        self.assertIsNone(complete_original_extent(
            BytesImage(data), 0x1000, 0x10ab, self.decoder
        ))


if __name__ == "__main__":
    unittest.main()
