"""Guard metadata repair needs owner, storage and instruction evidence."""

import struct
import unittest

from reccmp.formats.exceptions import InvalidVirtualReadError
from reccmp.types import EntityType, ImageId

from lib.comparison.guards import repair_guard_symbols
from tests.comparison.fixtures import fixture, patch_body


SYMBOL = "?Run@Widget@@QAEXXZ"
GUARD = "?$S10@?1?" + SYMBOL + "@4EA"


def guard_fixture():
    tails = [
        (
            b"\xf6\x05"
            + struct.pack("<I", slot)
            + b"\x01"
            + b"\x80\x0d"
            + struct.pack("<I", slot)
            + b"\x01\xc3"
        ).hex()
        for slot in (0x6000, 0x8000)
    ]
    comparator, match = fixture(*tails)
    with comparator.db.batch() as batch:
        for side, parent, slot in (
            (ImageId.ORIG, 0x1000, 0x6000),
            (ImageId.RECOMP, 0x2000, 0x8000),
        ):
            batch.set(
                side,
                parent,
                type=EntityType.FUNCTION,
                symbol=SYMBOL,
                size=match.size(side),
            )
            batch.set(side, slot, type=EntityType.DATA, name=GUARD, size=1)
        batch.set(ImageId.ORIG, 0x6000, static_var=True, parent_function=0x1000)
        batch.match(0x1000, 0x2000)
    for image, start, slot in (
        (comparator.orig_bin, 0x1000, 0x6000),
        (comparator.recomp_bin, 0x2000, 0x8000),
    ):
        prior = image.read
        image.read = lambda address, size, prior=prior, slot=slot: (
            b"\0"[:size] if address == slot else prior(address, size)
        )
        image.relocations = {start + 7, start + 14}
    return comparator, match


class GuardMetadataTests(unittest.TestCase):
    def test_binary_witness_repairs_symbol_then_uses_upstream_static_matching(self):
        comparator, _ = guard_fixture()
        self.assertFalse(comparator.db.get(ImageId.ORIG, 0x6000).matched)
        repair_guard_symbols(comparator)
        self.assertEqual(comparator.db.get(ImageId.ORIG, 0x6000).recomp_addr, 0x8000)
        self.assertEqual(comparator.db.get(ImageId.RECOMP, 0x8000).get("symbol"), GUARD)

    def test_names_alone_cannot_repair_guard_metadata(self):
        for side in (ImageId.ORIG, ImageId.RECOMP):
            for mutation in (
                "owner",
                "size",
                "unknown-size",
                "symbol",
                "initial",
                "test-bit",
                "store-bit",
                "opcode",
                "relocation",
                "truncated",
                "missing-owner",
                "wrong-kind",
                "unknown-function",
                "unreadable",
            ):
                with self.subTest(side=side, mutation=mutation):
                    comparator, match = guard_fixture()
                    image, start, slot = (
                        (comparator.orig_bin, 0x1000, 0x6000)
                        if side == ImageId.ORIG
                        else (comparator.recomp_bin, 0x2000, 0x8000)
                    )
                    with comparator.db.batch() as batch:
                        if mutation == "owner":
                            batch.set(side, start, symbol="?Other@Widget@@QAEXXZ")
                        elif mutation in ("size", "unknown-size"):
                            key = "orig_size" if side == ImageId.ORIG else "recomp_size"
                            batch.set(
                                side, slot, **{key: 2 if mutation == "size" else None}
                            )
                        elif mutation == "symbol":
                            batch.set(ImageId.RECOMP, 0x8000, symbol="different")
                        elif mutation == "missing-owner":
                            batch.set(ImageId.ORIG, 0x6000, parent_function=None)
                        elif mutation == "wrong-kind":
                            batch.set(side, slot, type=EntityType.FUNCTION)
                        elif mutation == "unknown-function":
                            key = "orig_size" if side == ImageId.ORIG else "recomp_size"
                            batch.set(side, start, **{key: None})
                    if mutation == "initial":
                        prior = image.read
                        image.read = lambda address, size, prior=prior, slot=slot: (
                            b"\x01" if address == slot else prior(address, size)
                        )
                    elif mutation == "test-bit":
                        patch_body(image, start, 11, b"\x02")
                    elif mutation == "store-bit":
                        patch_body(image, start, 18, b"\x02")
                    elif mutation == "opcode":
                        patch_body(image, start, 5, b"\x80")
                    elif mutation == "relocation":
                        image.relocations = {start + 7}
                    elif mutation == "truncated":
                        prior = image.read
                        image.read = lambda address, size, prior=prior, start=start: (
                            prior(address, size)[:-1]
                            if address == start
                            else prior(address, size)
                        )
                    elif mutation == "unreadable":

                        def inaccessible(address, size):
                            raise InvalidVirtualReadError(address, size)

                        image.read = inaccessible
                    repair_guard_symbols(comparator)
                    self.assertFalse(comparator.db.get(ImageId.ORIG, 0x6000).matched)

    def test_ambiguous_pdb_candidates_are_rejected(self):
        comparator, _ = guard_fixture()
        with comparator.db.batch() as batch:
            batch.set(ImageId.RECOMP, 0x9000, name=GUARD, type=EntityType.DATA, size=1)
        repair_guard_symbols(comparator)
        self.assertFalse(comparator.db.get(ImageId.ORIG, 0x6000).matched)
