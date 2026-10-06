"""Absolute-call names require a bounded paired slot and the same matched callee."""

import struct
import unittest
from unittest.mock import Mock

from reccmp.compare.db import EntityDb
from reccmp.compare.functions import FunctionComparator
from reccmp.formats.exceptions import InvalidVirtualAddressError
from reccmp.formats.image import Image
from reccmp.types import EntityType, ImageId

from lib.comparison.vtables import install_vtable_lookups


def fixture():
    db = EntityDb()
    with db.batch() as batch:
        for side, table, target in (
            (ImageId.ORIG, 0x1000, 0x4000),
            (ImageId.RECOMP, 0x2000, 0x5000),
        ):
            batch.set(side, table, type=EntityType.VTABLE, name="Table", size=8)
            batch.set(side, target, type=EntityType.FUNCTION, name="Target", size=1)
        batch.set(ImageId.ORIG, 0x3000, size=5)
        batch.match(0x1000, 0x2000)
        batch.match(0x4000, 0x5000)
    memory = {
        ImageId.ORIG: {
            0x4000: b"\xc3",
            0x1004: struct.pack("<I", 0x3000),
            0x3000: b"\xe9" + struct.pack("<i", 0x4000 - 0x3005),
        },
        ImageId.RECOMP: {0x2004: struct.pack("<I", 0x5000), 0x5000: b"\xc3"},
    }

    def image(side):
        def read(address, size):
            if address not in memory[side]:
                raise InvalidVirtualAddressError(address)
            return memory[side][address][:size]

        return Mock(
            spec=Image, read=read, imagebase=0, is_relocated_addr=lambda _: False
        )

    comparator = FunctionComparator(
        db, Mock(), image(ImageId.ORIG), image(ImageId.RECOMP), Mock(), Mock()
    )
    return comparator, memory


class VtableLookupTests(unittest.TestCase):
    def test_named_indirect_target_through_paired_slot(self):
        comparator, _ = fixture()
        self.assertIsNone(comparator.orig_sanitize.lookup(0x1004, indirect=True))
        expected = comparator.recomp_sanitize.lookup(0x2004, indirect=True)
        install_vtable_lookups(comparator)
        self.assertEqual(expected, "->Target (FUNCTION)")
        self.assertEqual(
            comparator.orig_sanitize.lookup(0x1004, indirect=True), expected
        )
        self.assertEqual(
            comparator.orig_sanitize.sanitize(
                (0x8000, 6, "call", "dword ptr [0x1004]")
            ),
            ("call", "dword ptr [->Target (FUNCTION)]"),
        )

    def test_direct_thunk_calls_keep_upstream_identity(self):
        comparator, _ = fixture()
        install_vtable_lookups(comparator)
        self.assertIsNone(comparator.orig_sanitize.lookup(0x3000, exact=True))
        self.assertEqual(
            comparator.orig_sanitize.sanitize((0x8000, 5, "call", "0x3000")),
            ("call", "<OFFSET1>"),
        )

    def test_rebuilt_thunk_uses_the_same_paired_proof(self):
        comparator, memory = fixture()
        with comparator.db.batch() as batch:
            batch.set(ImageId.RECOMP, 0x6000, size=5)
        memory[ImageId.ORIG][0x1004] = struct.pack("<I", 0x4000)
        memory[ImageId.RECOMP][0x2004] = struct.pack("<I", 0x6000)
        memory[ImageId.RECOMP][0x6000] = b"\xe9" + struct.pack("<i", 0x5000 - 0x6005)
        install_vtable_lookups(comparator)
        self.assertEqual(
            comparator.recomp_sanitize.lookup(0x2004, indirect=True),
            comparator.orig_sanitize.lookup(0x1004, indirect=True),
        )

    def test_upstream_variable_name_takes_precedence(self):
        comparator, _ = fixture()
        with comparator.db.batch() as batch:
            batch.set(
                ImageId.ORIG, 0x1004, type=EntityType.DATA, name="Callback", size=4
            )
        install_vtable_lookups(comparator)
        self.assertEqual(
            comparator.orig_sanitize.lookup(0x1004, indirect=True), "Callback (DATA)"
        )

    def test_mismatched_callee_is_unresolved(self):
        comparator, memory = fixture()
        with comparator.db.batch() as batch:
            batch.set(
                ImageId.ORIG, 0x6000, type=EntityType.FUNCTION, name="Target", size=1
            )
            batch.set(
                ImageId.RECOMP, 0x7000, type=EntityType.FUNCTION, name="Target", size=1
            )
            batch.match(0x6000, 0x7000)
        memory[ImageId.RECOMP][0x2004] = struct.pack("<I", 0x7000)
        install_vtable_lookups(comparator)
        self.assertIsNone(comparator.orig_sanitize.lookup(0x1004, indirect=True))

    def test_unpaired_table_is_unresolved(self):
        comparator, memory = fixture()
        with comparator.db.batch() as batch:
            batch.set(
                ImageId.ORIG, 0x9000, type=EntityType.VTABLE, name="Other", size=8
            )
        memory[ImageId.ORIG][0x9004] = memory[ImageId.ORIG][0x1004]
        install_vtable_lookups(comparator)
        self.assertIsNone(comparator.orig_sanitize.lookup(0x9004, indirect=True))

    def test_slot_extent_and_alignment_are_required(self):
        for address, counterpart in ((0x1005, 0x2005), (0x1008, 0x2008)):
            with self.subTest(address=address):
                comparator, memory = fixture()
                memory[ImageId.ORIG][address] = memory[ImageId.ORIG][0x1004]
                memory[ImageId.RECOMP][counterpart] = memory[ImageId.RECOMP][0x2004]
                install_vtable_lookups(comparator)
                self.assertIsNone(
                    comparator.orig_sanitize.lookup(address, indirect=True)
                )

    def test_truncated_slots_and_non_jump_thunks_are_unresolved(self):
        for side, address, code in (
            (ImageId.ORIG, 0x1004, b"\x00"),
            (ImageId.RECOMP, 0x2004, b"\x00"),
            (ImageId.ORIG, 0x3000, b"\xeb\x00\x90\x90\x90"),
        ):
            with self.subTest(address=address):
                comparator, memory = fixture()
                memory[side][address] = code
                install_vtable_lookups(comparator)
                self.assertIsNone(
                    comparator.orig_sanitize.lookup(0x1004, indirect=True)
                )

    def test_unreadable_counterpart_is_unresolved(self):
        comparator, memory = fixture()
        del memory[ImageId.RECOMP][0x2004]
        install_vtable_lookups(comparator)
        self.assertIsNone(comparator.orig_sanitize.lookup(0x1004, indirect=True))

    def test_unreadable_thunk_destination_is_unresolved(self):
        comparator, memory = fixture()
        del memory[ImageId.ORIG][0x4000]
        install_vtable_lookups(comparator)
        self.assertIsNone(comparator.orig_sanitize.lookup(0x1004, indirect=True))

    def test_e9_thunk_target_wraps_at_32_bits(self):
        comparator, memory = fixture()
        address = 0xFFFFFFF0
        with comparator.db.batch() as batch:
            batch.set(ImageId.ORIG, address, size=5)
        memory[ImageId.ORIG][0x1004] = struct.pack("<I", address)
        memory[ImageId.ORIG][address] = b"\xe9" + struct.pack("<i", 0x400B)
        install_vtable_lookups(comparator)
        self.assertEqual(
            comparator.orig_sanitize.lookup(0x1004, indirect=True),
            comparator.recomp_sanitize.lookup(0x2004, indirect=True),
        )

    def test_chained_jumps_are_unresolved(self):
        comparator, memory = fixture()
        with comparator.db.batch() as batch:
            batch.set(ImageId.ORIG, 0x3500, size=5)
        memory[ImageId.ORIG][0x3000] = b"\xe9" + struct.pack("<i", 0x3500 - 0x3005)
        memory[ImageId.ORIG][0x3500] = b"\xe9" + struct.pack("<i", 0x4000 - 0x3505)
        install_vtable_lookups(comparator)
        self.assertIsNone(comparator.orig_sanitize.lookup(0x1004, indirect=True))


if __name__ == "__main__":
    unittest.main()
