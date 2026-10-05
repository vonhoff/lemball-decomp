"""Jump-thunk identity and whole-function comparison boundaries."""

import copy
import struct
import unittest
from types import SimpleNamespace
from unittest.mock import Mock, patch

from reccmp.compare.asm.parse import ParseAsm
from reccmp.compare.db import EntityDb, ReccmpMatch
from reccmp.compare.functions import FunctionComparator
from reccmp.compare.report import ReccmpComparedEntity
from reccmp.formats.exceptions import InvalidVirtualAddressError
from reccmp.types import EntityType, ImageId

from lib.effective import ThunkParseAsm, additional_effective_matches


THUNK = b"\xe9" + struct.pack("<i", 16)


def fixture(original="c3", rebuilt=None, thunk=THUNK, opcode="e8"):
    bodies = [
        bytes.fromhex(opcode + "fb2f0000 " + tail)
        for tail in (original, original if rebuilt is None else rebuilt)
    ]
    images = []
    for start, body in zip((0x1000, 0x2000), bodies, strict=True):
        images.append(
            SimpleNamespace(
                read=lambda read_address, size, image_start=start, image_body=body: (
                    image_body
                    if read_address == image_start
                    else thunk
                    if read_address == 0x4000
                    else b""
                )[:size],
                imagebase=0,
                is_relocated_addr=lambda _address: False,
            )
        )
    lines = Mock()
    lines.find_line_of_recomp_address.return_value = None
    comparator = FunctionComparator(
        EntityDb(), lines, images[0], images[1], Mock(), Mock()
    )
    with comparator.db.batch() as batch:
        for side, address in ((ImageId.ORIG, 0x4015), (ImageId.RECOMP, 0x5000)):
            batch.set(side, address, type=EntityType.FUNCTION, name="Target", size=1)
        batch.match(0x4015, 0x5000)
    match = ReccmpMatch(
        0x1000,
        0x2000,
        {
            "name": "Caller",
            "type": EntityType.FUNCTION,
            "orig_size": len(bodies[0]),
            "recomp_size": len(bodies[1]),
        },
    )
    return comparator, match


def compare(comparator, match, **flags):
    raw = comparator.compare_function(match)
    comparison = ReccmpComparedEntity(
        match.orig_addr,
        "Caller",
        raw.match_ratio,
        EntityType.FUNCTION,
        match.recomp_addr,
        **flags,
    )
    engine = SimpleNamespace(
        function_comparator=comparator, get_functions=lambda: [match]
    )
    return additional_effective_matches(engine, {match.orig_addr: comparison})


def switch_fixture(
    indexed=False,
    targets=None,
    rebuilt_targets=None,
    indices=b"\x00\x01",
    rebuilt_indices=None,
    suffix=b"",
    branch_to_table=False,
    fallthrough=False,
    padding=b"",
    branch_to_padding=False,
):
    table_offset = (39 if indexed else 32) + len(padding)
    targets = targets or ((24, 27) if indexed else (17, 20))
    tails = []
    for start, destinations, index_bytes in (
        (0x1000, targets, indices),
        (
            0x2000,
            targets if rebuilt_targets is None else rebuilt_targets,
            indices if rebuilt_indices is None else rebuilt_indices,
        ),
    ):
        default_offset = table_offset - len(padding) - 6
        branch_target = table_offset if branch_to_table else default_offset
        if branch_to_padding:
            branch_target = table_offset - len(padding)
        displacement = branch_target - 10
        tail = bytes.fromhex("83f80177") + bytes([displacement])
        if indexed:
            tail += bytes.fromhex("0fb680") + struct.pack(
                "<I", start + table_offset + 8
            )
        tail += bytes.fromhex("ff2485") + struct.pack("<I", start + table_offset)
        tail += bytes.fromhex("31c0c3 b801000000c3 b802000000")
        tail += b"\x90" if fallthrough else b"\xc3"
        tail += padding
        tail += b"".join(struct.pack("<I", start + offset) for offset in destinations)
        tails.append((tail + (index_bytes if indexed else b"") + suffix).hex())
    comparator, match = fixture(*tails)
    for start, image in (
        (0x1000, comparator.orig_bin),
        (0x2000, comparator.recomp_bin),
    ):
        image.is_relocated_addr = lambda address, image_start=start: (
            image_start <= address < image_start + 100
        )
    return comparator, match


def patch_body(image, start, offset, replacement):
    read = image.read

    def patched(address, size):
        data = read(address, size)
        if address == start:
            data = data[:offset] + replacement + data[offset + len(replacement) :]
        return data

    image.read = patched


def callee_fixture(original_prefix, rebuilt_prefix=None):
    original = "3bc7 7200 e8f22f0000 c3"
    rebuilt = "3bf8 7700 e8f22f0000 c3"
    comparator, match = fixture(original, rebuilt)
    for image, target, prefix in (
        (comparator.orig_bin, 0x4015, original_prefix),
        (
            comparator.recomp_bin,
            0x5000,
            original_prefix if rebuilt_prefix is None else rebuilt_prefix,
        ),
    ):
        read = image.read
        image.read = (
            lambda address, size, prior_read=read, callee_address=target, callee_prefix=prefix: (
                bytes.fromhex(callee_prefix)[:size]
                if address == callee_address
                else prior_read(address, size)
            )
        )
    return comparator, match


def comparison_switch_fixture(table_enters_branch=False):
    tails = []
    for start, compare_branch in ((0x1000, "3bc7 7203"), (0x2000, "3bf8 7703")):
        tail = bytes.fromhex("83f801 7711 ff2485") + struct.pack("<I", start + 32)
        tail += bytes.fromhex(compare_branch + " 39c0c3 39c0c3 39c0c3 9090")
        tail += struct.pack(
            "<II", start + (19 if table_enters_branch else 17), start + 27
        )
        tails.append(tail.hex())
    comparator, match = fixture(*tails)
    for image in (comparator.orig_bin, comparator.recomp_bin):
        image.is_relocated_addr = lambda _address: True
    return comparator, match


class ThunkTests(unittest.TestCase):
    def test_paired_callee_prefix_can_prove_flags_are_overwritten(self):
        for prefix in ("83ec04 c3", "8b4108 83f803 c3", "39c0 c3"):
            with self.subTest(prefix=prefix):
                self.assertTrue(compare(*callee_fixture(prefix)))

    def test_both_callee_prefixes_must_overwrite_flags_before_observing_them(self):
        for prefix in (
            "c3",
            "11c0 39c0 c3",
            "9c 39c0 c3",
            "40 c3",
            "0f",
            "e800000000 39c0 c3",
            "ebfe",
            "ffe0",
            "90" * 32 + "39c0 c3",
        ):
            for original, rebuilt in ((prefix, "39c0c3"), ("39c0c3", prefix)):
                with self.subTest(original=original, rebuilt=rebuilt):
                    self.assertEqual(compare(*callee_fixture(original, rebuilt)), {})

    def test_unreadable_callee_prefix_cannot_prove_flag_lifetime(self):
        comparator, match = callee_fixture("39c0c3")
        read = comparator.recomp_bin.read

        def unreadable(address, size):
            if address == 0x5000:
                raise InvalidVirtualAddressError(address)
            return read(address, size)

        comparator.recomp_bin.read = unreadable
        self.assertEqual(compare(comparator, match), {})

    def test_switch_dispatch_must_not_enter_the_guarded_branch(self):
        self.assertTrue(compare(*comparison_switch_fixture()))
        self.assertEqual(
            compare(*comparison_switch_fixture(table_enters_branch=True)), {}
        )

    def test_reversed_comparison_requires_matching_condition_and_dead_flags(self):
        for original, rebuilt in (
            ("72", "77"),
            ("76", "73"),
            ("7c", "7f"),
            ("7e", "7d"),
            ("74", "74"),
            ("75", "75"),
        ):
            with self.subTest(branch=original):
                left = "3bc7 " + original + "03 39c0c3 39c0c3"
                right = "3bf8 " + rebuilt + "03 39c0c3 39c0c3"
                self.assertTrue(compare(*fixture(left, right)))
        for left, right in (("663bc7", "663bf8"), ("3ac3", "3ad8")):
            with self.subTest(compare=left):
                self.assertTrue(
                    compare(
                        *fixture(
                            left + " 7203 39c0c3 39c0c3", right + " 7703 39c0c3 39c0c3"
                        )
                    )
                )

    def test_changed_operands_or_branch_targets_are_not_comparison_reversals(self):
        original = "3bc7 7203 39c0c3 39c0c3"
        for rebuilt in (
            "3bf8 7303 39c0c3 39c0c3",  # Wrong unsigned relation.
            "3bf9 7703 39c0c3 39c0c3",  # Different register.
            "3bf8 7700 39c0c3 39c0c3",  # Different valid target.
            "3bf8 7703 39c8c3 39c0c3",
        ):  # Changed flag overwrite.
            with self.subTest(rebuilt=rebuilt):
                self.assertEqual(compare(*fixture(original, rebuilt)), {})

    def test_comparison_flags_must_be_overwritten_on_both_paths(self):
        for observer in (
            "11c0",
            "19c0",
            "9c",
            "9f",
            "0f92c0",
            "7200",
            "40",
            "c3",
            "ff10",
            "e800000000",
        ):
            for taken in (False, True):
                with self.subTest(observer=observer, taken=taken):
                    blocks = ["39c0c3", observer + "39c0c3"]
                    if taken:
                        blocks.reverse()
                    tail = f"{len(bytes.fromhex(blocks[0])):02x} " + " ".join(blocks)
                    self.assertEqual(
                        compare(*fixture("3bc7 72" + tail, "3bf8 77" + tail)), {}
                    )

    def test_comparison_proof_follows_preserving_instructions_and_direct_jumps(self):
        for path in ("89c1 50 5a 8d09 90", "eb00", "663bc0", "d3e0"):
            with self.subTest(path=path):
                tail = " 7200 " + path + " 39c0c3"
                self.assertTrue(
                    compare(
                        *fixture("3bc7" + tail, "3bf8" + tail.replace("7200", "7700"))
                    )
                )
        for path in ("ebfe", "ffe0", "c3"):
            with self.subTest(unproven_path=path):
                self.assertEqual(
                    compare(*fixture("3bc7 7200 " + path, "3bf8 7700 " + path)), {}
                )

    def test_comparison_can_span_flag_preserving_instructions(self):
        for middle in (
            "89c1 50 5a 8d09 90",
            "0fbe01 0fb701",
            "8b0ca8 8a44242c 88442413 8d3451",  # BIteration row setup.
        ):
            with self.subTest(middle=middle):
                self.assertTrue(
                    compare(
                        *fixture(
                            "3954241c " + middle + " 7c03 39c0c3 39c0c3",
                            "3b54241c " + middle + " 7f03 39c0c3 39c0c3",
                        )
                    )
                )

    def test_intervening_flag_uses_and_writes_prevent_comparison_proof(self):
        for middle in ("9f", "9c", "40", "39c0", "85c0", "d3e0", "0f92c0", "eb00"):
            with self.subTest(middle=middle):
                self.assertEqual(
                    compare(
                        *fixture(
                            "3bc7 " + middle + " 7203 39c0c3 39c0c3",
                            "3bf8 " + middle + " 7703 39c0c3 39c0c3",
                        )
                    ),
                    {},
                )

    def test_entry_to_intervening_instruction_cannot_bypass_comparison(self):
        for displacement in (2, 4, 6):
            with self.subTest(displacement=displacement):
                entry = f"eb{displacement:02x} "
                self.assertEqual(
                    compare(
                        *fixture(
                            entry + "3bc7 89c1 8d09 7203 39c0c3 39c0c3",
                            entry + "3bf8 89c1 8d09 7703 39c0c3 39c0c3",
                        )
                    ),
                    {},
                )

    def test_intervening_instruction_changes_are_not_hidden(self):
        self.assertEqual(
            compare(
                *fixture(
                    "3bc7 89c1 7203 39c0c3 39c0c3",
                    "3bf8 89c2 7703 39c0c3 39c0c3",
                )
            ),
            {},
        )

    def test_memory_comparison_keeps_the_same_address_and_width(self):
        for original, rebuilt in (
            ("39442420", "3b442420"),
            ("6639442420", "663b442420"),
            ("38442420", "3a442420"),
        ):
            with self.subTest(original=original):
                self.assertTrue(
                    compare(
                        *fixture(
                            original + " 7203 39c0c3 39c0c3",
                            rebuilt + " 7703 39c0c3 39c0c3",
                        )
                    )
                )
        for rebuilt in ("3b442424", "3b442520", "3b4c2420"):
            with self.subTest(changed_memory_operand=rebuilt):
                self.assertEqual(
                    compare(
                        *fixture(
                            "39442420 7203 39c0c3 39c0c3",
                            rebuilt + " 7703 39c0c3 39c0c3",
                        )
                    ),
                    {},
                )

    def test_xor_zero_register_comparison_can_match_self_test(self):
        for comparison, branch in (("3bfe", "7e"), ("3bf7", "7d")):
            with self.subTest(comparison=comparison):
                self.assertTrue(
                    compare(
                        *fixture(
                            "31f6 8b7934 "
                            + comparison
                            + " "
                            + branch
                            + "03 39c0c3 39c0c3",
                            "31f6 8b7934 85ff 7e03 39c0c3 39c0c3",
                        )
                    )
                )

    def test_zero_comparison_requires_proven_unchanged_register(self):
        for setup in ("89c6", "31f6 89c6", "31f6 66be0100", "31f6 40", "31ff"):
            with self.subTest(setup=setup):
                self.assertEqual(
                    compare(
                        *fixture(
                            setup + " 3bfe 7e03 39c0c3 39c0c3",
                            setup + " 85ff 7e03 39c0c3 39c0c3",
                        )
                    ),
                    {},
                )
        self.assertEqual(
            compare(
                *fixture(
                    "31f6 3bfe 7e03 39c0c3 39c0c3",
                    "31f6 85f6 7e03 39c0c3 39c0c3",
                )
            ),
            {},
        )

    def test_zero_definition_must_dominate_the_comparison(self):
        for displacement in (2, 5):
            with self.subTest(displacement=displacement):
                prefix = f"eb{displacement:02x} 31f6 8b7934 "
                self.assertEqual(
                    compare(
                        *fixture(
                            prefix + "3bfe 7e03 39c0c3 39c0c3",
                            prefix + "85ff 7e03 39c0c3 39c0c3",
                        )
                    ),
                    {},
                )

    def test_zero_test_equivalence_does_not_hide_auxiliary_flag_difference(self):
        for observer in ("9f", "9c", "27"):
            with self.subTest(observer=observer):
                tail = " 7e00 " + observer + " 39c0c3"
                self.assertEqual(
                    compare(*fixture("31f6 3bfe" + tail, "31f6 85ff" + tail)), {}
                )

    def test_logical_writes_leave_auxiliary_flag_live_on_every_successor(self):
        for logical in ("85c0", "83e007", "09c0", "31c0"):
            with self.subTest(logical=logical):
                body = logical + " 7403 39c0c3 39c0c3"
                self.assertTrue(
                    compare(*fixture("3bc7 7200 " + body, "3bf8 7700 " + body))
                )
            for observer in ("9f", "9c", "27", "c3", "7503 39c0c3 9f", "75fe"):
                with self.subTest(logical=logical, observer=observer):
                    body = logical + " " + observer + " 39c0c3"
                    self.assertEqual(
                        compare(*fixture("3bc7 7200 " + body, "3bf8 7700 " + body)), {}
                    )

    def test_shifts_do_not_prove_that_incoming_flags_were_overwritten(self):
        for body in ("d3e0 c3", "d3e0 7200 39c0c3", "d1e0 9f 39c0c3"):
            with self.subTest(body=body):
                self.assertEqual(
                    compare(*fixture("3bc7 7200 " + body, "3bf8 7700 " + body)), {}
                )

    def test_full_arithmetic_writes_end_the_comparison_flag_lifetime(self):
        for writer in ("01c0", "29c0", "f7d8", "39c0"):
            with self.subTest(writer=writer):
                self.assertTrue(
                    compare(
                        *fixture(
                            "3bc7 7200 " + writer + " c3", "3bf8 7700 " + writer + " c3"
                        )
                    )
                )

    def test_comparison_proof_preserves_near_branch_targets(self):
        self.assertTrue(
            compare(
                *fixture(
                    "3bc7 0f8203000000 39c0c3 39c0c3", "3bf8 0f8703000000 39c0c3 39c0c3"
                )
            )
        )
        self.assertEqual(
            compare(
                *fixture(
                    "3bc7 0f8203000000 39c0c3 39c0c3", "3bf8 0f8700000000 39c0c3 39c0c3"
                )
            ),
            {},
        )

    def test_branch_entry_cannot_bypass_the_reversed_comparison(self):
        for entry in ("eb02", "e802000000", "7402"):
            with self.subTest(entry=entry):
                self.assertEqual(
                    compare(
                        *fixture(
                            entry + " 3bc7 7203 39c0c3 39c0c3",
                            entry + " 3bf8 7703 39c0c3 39c0c3",
                        )
                    ),
                    {},
                )

    def test_comparison_proof_does_not_hide_partial_flags_or_unknown_control_flow(self):
        for prefix in ("ffe0", "ff2485aabbccdd"):
            with self.subTest(prefix=prefix):
                self.assertEqual(
                    compare(
                        *fixture(
                            prefix + " 3bc7 7203 39c0c3 39c0c3",
                            prefix + " 3bf8 7703 39c0c3 39c0c3",
                        )
                    ),
                    {},
                )

    def test_direct_calls_and_tail_jumps_reach_the_paired_function(self):
        for opcode in ("e8", "e9"):
            with self.subTest(opcode=opcode):
                comparator, match = fixture(opcode=opcode)
                raw = comparator.compare_function(match)
                self.assertLess(raw.match_ratio, 1)
                self.assertFalse(raw.is_effective_match)
                self.assertEqual(
                    compare(comparator, match),
                    {0x1000: ("verified jump thunk target",)},
                )

    def test_thunk_requires_a_complete_e9_to_a_paired_function(self):
        for thunk in (THUNK[:4], b"\xe8" + THUNK[1:], b"\xe9" + struct.pack("<i", 17)):
            with self.subTest(thunk=thunk):
                self.assertEqual(compare(*fixture(thunk=thunk)), {})
        comparator, match = fixture()
        read = comparator.orig_bin.read

        def invalid(address, size):
            if address == 0x4000:
                raise InvalidVirtualAddressError(address)
            return read(address, size)

        comparator.orig_bin.read = invalid
        self.assertEqual(compare(comparator, match), {})

    def test_function_pointers_and_indirect_calls_keep_their_identity(self):
        for call_first in (True, False):
            parser = ThunkParseAsm(
                SimpleNamespace(read=lambda _address, _size: THUNK),
                {0x4015},
                ParseAsm(
                    addr_test=lambda _address: True,
                    name_lookup=lambda addr, exact=False, indirect=False: {
                        0x4000: "Wrapper",
                        0x4015: "Target",
                    }.get(addr),
                ),
            )
            call = (0x1000, 5, "call", "0x4000")
            pointer = (0x1005, 5, "mov", "eax, 0x4000")
            results = dict(
                parser.sanitize(inst)
                for inst in ((call, pointer) if call_first else (pointer, call))
            )
            self.assertEqual(results, {"call": "Target", "mov": "eax, Wrapper"})
            self.assertEqual(
                parser.sanitize((0x100A, 6, "call", "dword ptr [0x4000]")),
                ("call", "dword ptr [Wrapper]"),
            )
            parser.targets.add(0x4000)
            self.assertEqual(parser.sanitize(call), ("call", "Wrapper"))

    def test_remaining_registers_flags_memory_branches_and_cleanup_must_agree(self):
        for original, rebuilt in (
            ("8bc1 c3", "8bc2 c3"),
            ("31c0 c3", "29c0 c3"),
            ("8b06 890f c3", "890f 8b06 c3"),
            ("85c0 7402 31c0 c3", "85c0 7400 31c0 c3"),
            ("85c0 7400 c3", "85c0 7500 c3"),
            ("e300 c3", "e301 c3"),
            ("c20400", "c20800"),
            ("50 c3", "fff0 c3"),  # Same text, different instruction boundaries.
            ("6a01 6a02 6a03 c3", "6a04 6a02 6a03 c3"),
        ):
            with self.subTest(original=original, rebuilt=rebuilt):
                self.assertEqual(compare(*fixture(original, rebuilt)), {})

    def test_unresolved_operands_cannot_match_by_placeholder_number(self):
        for original, rebuilt in (
            ("e800010000", "e800020000"),
            ("ff1500600000", "ff1500700000"),
            ("a100600000", "a100700000"),
            ("a300600000", "a300700000"),
            ("8b048500600000", "8b048500700000"),
            ("b800600000", "b800700000"),
            ("6800600000", "6800700000"),
        ):
            with self.subTest(original=original):
                comparator, match = fixture(original + " c3", rebuilt + " c3")
                comparator.orig_bin.is_relocated_addr = lambda value: value == 0x6000
                comparator.recomp_bin.is_relocated_addr = lambda value: value == 0x7000
                self.assertEqual(compare(comparator, match), {})

    def test_complete_decoding_and_internal_branch_boundaries_are_required(self):
        for tail in ("0f", "cc 0f", "7401 8b00 c3", "eb00", "e301 c3"):
            with self.subTest(tail=tail):
                self.assertEqual(compare(*fixture(tail)), {})
        self.assertTrue(compare(*fixture("90 ebfd")))

    def test_parser_state_is_per_function(self):
        comparator, match = fixture()
        parser = ThunkParseAsm(comparator.orig_bin, {0x4015}, comparator.orig_sanitize)
        size = match.size(ImageId.ORIG)
        assert size is not None
        parser.parse_asm(comparator.orig_bin.read(0x1000, size), 0x1000)
        self.assertTrue(parser.used_thunk)
        self.assertIsNotNone(parser.signature)
        parser.parse_asm(b"\x0f", 0x1100)
        self.assertFalse(parser.used_thunk)
        self.assertIsNone(parser.signature)

    def test_relocated_switch_tables_preserve_every_target_and_index(self):
        for indexed in (False, True):
            with self.subTest(indexed=indexed):
                comparator, match = switch_fixture(indexed=indexed)
                raw = comparator.compare_function(match)
                self.assertLess(raw.match_ratio, 1)
                self.assertTrue(compare(comparator, match))
        self.assertEqual(compare(*switch_fixture(rebuilt_targets=(20, 17))), {})
        self.assertEqual(
            compare(*switch_fixture(indexed=True, rebuilt_indices=b"\x01\x00")), {}
        )

    def test_switch_targets_must_be_decoded_code_boundaries(self):
        for targets in ((18, 20), (32, 20), (0x5000, 20)):
            with self.subTest(targets=targets):
                self.assertEqual(compare(*switch_fixture(targets=targets)), {})
        self.assertEqual(compare(*switch_fixture(branch_to_table=True)), {})
        self.assertEqual(compare(*switch_fixture(fallthrough=True)), {})

    def test_switch_alignment_padding_must_be_unreachable_and_identical(self):
        for padding in (b"\x90", b"\x90" * 2, b"\x90" * 3, b"\x8b\xff", b"\x8d\x09"):
            with self.subTest(padding=padding):
                self.assertTrue(compare(*switch_fixture(padding=padding)))
                self.assertEqual(
                    compare(*switch_fixture(padding=padding, branch_to_padding=True)),
                    {},
                )
                self.assertEqual(
                    compare(*switch_fixture(padding=padding, targets=(32, 20))), {}
                )
                self.assertEqual(
                    compare(*switch_fixture(padding=padding, fallthrough=True)), {}
                )
        comparator, match = switch_fixture(padding=b"\x90")
        patch_body(comparator.recomp_bin, 0x2000, 32, b"\xcc")
        self.assertEqual(compare(comparator, match), {})
        for padding in (b"\x8b\xc1", b"\x8d\x49\x01"):
            with self.subTest(changed_register=padding):
                self.assertEqual(compare(*switch_fixture(padding=padding)), {})

    def test_unknown_computed_jumps_cannot_prove_padding_unreachable(self):
        comparator, match = switch_fixture(padding=b"\x90")
        patch_body(comparator.orig_bin, 0x1000, 17, b"\xff\xe0\x90")  # jmp eax; nop
        patch_body(comparator.recomp_bin, 0x2000, 17, b"\xff\xe0\x90")
        self.assertEqual(compare(comparator, match), {})

    def test_switch_tables_cannot_hide_partial_entries_or_stale_names(self):
        for suffix in (b"\x00", b"\x00\x00", b"\x00\x00\x00"):
            with self.subTest(suffix=suffix):
                self.assertEqual(compare(*switch_fixture(suffix=suffix)), {})
        comparator, match = switch_fixture()
        parser = ThunkParseAsm(comparator.orig_bin, {0x4015}, comparator.orig_sanitize)
        size = match.size(ImageId.ORIG)
        assert size is not None
        parser.parse_asm(comparator.orig_bin.read(0x1000, size), 0x1000)
        self.assertIsNotNone(parser.signature)
        self.assertTrue(parser.local_tables)
        parser.parse_asm(b"\x0f", 0x1100)
        self.assertEqual(parser.local_tables, {})
        self.assertIsNone(parser.signature)

    def test_actual_assertion_arguments_are_compared(self):
        for changed in (False, True):
            comparator, match = fixture(
                "6a01 6a02 6a03 e8f04f0000 c3",
                "6a01 6a02 " + ("6a04" if changed else "6a03") + " e8f04f0000 c3",
            )
            with comparator.db.batch() as batch:
                for side, address in ((ImageId.ORIG, 0x6000), (ImageId.RECOMP, 0x7000)):
                    batch.set(
                        side, address, type=EntityType.FUNCTION, name="__assert", size=1
                    )
                batch.match(0x6000, 0x7000)
            with patch("reccmp.compare.functions.has_asserts", return_value=True):
                self.assertEqual(bool(compare(comparator, match)), not changed)

    def test_raw_comparison_and_upstream_parsers_are_preserved(self):
        comparator, match = fixture()
        raw = comparator.compare_function(match)
        comparison = ReccmpComparedEntity(
            0x1000,
            "Caller",
            raw.match_ratio,
            EntityType.FUNCTION,
            0x2000,
            rdiff=raw.diff,
        )
        comparisons = {0x1000: comparison}
        saved = copy.deepcopy(comparisons)
        parsers = (comparator.orig_sanitize, comparator.recomp_sanitize)
        engine = SimpleNamespace(
            function_comparator=comparator, get_functions=lambda: [match]
        )
        self.assertTrue(additional_effective_matches(engine, comparisons))
        self.assertEqual(comparisons, saved)
        self.assertEqual(
            (comparator.orig_sanitize, comparator.recomp_sanitize), parsers
        )
        for flags in ({"is_stub": True}, {"is_effective_match": True}):
            self.assertEqual(compare(comparator, match, **flags), {})
        comparison.accuracy = 1
        self.assertEqual(additional_effective_matches(engine, comparisons), {})
