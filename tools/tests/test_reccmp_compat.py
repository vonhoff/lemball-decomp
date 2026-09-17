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
)
from reccmp.compare.asm import fixes, parse
from reccmp.compare.asm.instgen import InstructGen, SectionType
from reccmp.compare.pinned_sequences import SequenceMatcherWithPins


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
        result = [text for _, text in parser.parse_asm(blob, start)]
    return result


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
            with self.subTest(blob=blob):
                self.assertEqual(
                    [s.type for s in BoundedInstructGen(blob, start).sections],
                    [s.type for s in InstructGen(blob, start).sections],
                )


class PointerComparisonTests(unittest.TestCase):
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

    def test_table_end_comparison_normalizes_to_base_plus_length(self):
        def names(address, **kwargs):
            return {0x4000: "table (DATA)", 0x41e8: "other (DATA)"}.get(address)
        blob = b"\xba" + struct.pack("<I", 0x4000) + b"\x81\xfa" + struct.pack("<I", 0x41e8)
        parser = RelocationAwareParseAsm(
            relocation_sites=(0x1001, 0x1007), name_lookup=names,
            addr_test=lambda address: True,
        )
        lines = [text for _, text in parser.parse_asm(blob, 0x1000)]
        self.assertEqual(lines, ["mov edx, table (DATA)", "cmp edx, table (DATA) + 0x1e8"])

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
                get=lambda _side, address: SimpleNamespace(size=lambda _side: len(bodies[address])),
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



if __name__ == "__main__":
    unittest.main()
