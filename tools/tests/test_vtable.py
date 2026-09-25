"""Tests for vtable comparison helpers and adjuster thunk reconciliation."""

import struct
import unittest
from types import SimpleNamespace

from lib.vtable import decode_this_adjuster, unannotated_vtable_stores
from reccmp.types import EntityType


class FakeImage:
    def __init__(self, data: bytes, base: int = 0x1000):
        self.data = data
        self.base = base

    def is_valid_vaddr(self, address: int) -> bool:
        return self.base <= address < self.base + len(self.data)

    def read(self, address: int, size: int) -> bytes:
        offset = address - self.base
        if offset < 0 or offset + size > len(self.data):
            raise IndexError("Address out of bounds")
        return self.data[offset : offset + size]


class ThisAdjusterTests(unittest.TestCase):
    def test_decode_imm32_sub(self):
        # sub ecx, 0x46c (81 e9 6c 04 00 00); jmp 0x2000 (e9 ...)
        addr = 0x1000
        target = 0x2000
        disp = target - (addr + 6 + 5)
        code = (b"\x81\xe9\x6c\x04\x00\x00\xe9" + struct.pack("<i", disp)).ljust(16, b"\x90")
        image = FakeImage(code, base=addr)
        result = decode_this_adjuster(image, addr)
        self.assertIsNotNone(result)
        self.assertEqual(result, (0x46c, target))

    def test_decode_imm8_sub(self):
        # sub ecx, 0x10 (83 e9 10); jmp 0x2000 (e9 ...)
        addr = 0x1000
        target = 0x2000
        disp = target - (addr + 3 + 5)
        code = (b"\x83\xe9\x10\xe9" + struct.pack("<i", disp)).ljust(16, b"\x90")
        image = FakeImage(code, base=addr)
        result = decode_this_adjuster(image, addr)
        self.assertIsNotNone(result)
        self.assertEqual(result, (0x10, target))

    def test_decode_non_adjuster(self):
        addr = 0x1000
        code = b"\x55\x8b\xec\x83\xec\x10\x90\x90\x90\x90\x90\x90"
        image = FakeImage(code, base=addr)
        self.assertIsNone(decode_this_adjuster(image, addr))


class VtableStoreCoverageTests(unittest.TestCase):
    def test_scans_all_code_regions_and_excludes_mapped_tables(self):
        data = bytearray(0x300)
        for instruction, table in ((0x1000, 0x1100), (0x1010, 0x1110)):
            data[instruction - 0x1000:instruction - 0x1000 + 6] = (
                b"\xc7\x00" + struct.pack("<I", table)
            )
            struct.pack_into("<I", data, table - 0x1000, 0x1200)
        image = FakeImage(bytes(data))
        image.relocations = (0x1002, 0x1012)
        image.get_code_regions = lambda: iter((SimpleNamespace(addr=0x1000, data=image.data),))
        matches = [SimpleNamespace(
            type=EntityType.FUNCTION,
            rdiff=SimpleNamespace(orig_inst=[
                (hex(address), "mov dword ptr [eax], <OFFSET1>")
                for address in (0x1000, 0x1010)
            ]),
            name="Constructor",
        )]
        engine = SimpleNamespace(
            orig_bin=image,
            _db=SimpleNamespace(get=lambda *_: None),
            compare_all=lambda: iter(matches),
        )
        self.assertEqual(set(unannotated_vtable_stores(engine, set())), {0x1100, 0x1110})
        self.assertEqual(set(unannotated_vtable_stores(engine, {0x1100})), {0x1110})

    def test_purecall_construction_store_needs_proven_overwrite(self):
        data = bytearray(0x300)
        code = (b"\xc7\x00" + struct.pack("<I", 0x1100)
                + b"\xc7\x40\x04\x01\x00\x00\x00"
                + b"\x8d\x4b\x08"
                + b"\xc7\x00" + struct.pack("<I", 0x1120))
        data[:len(code)] = code
        struct.pack_into("<I", data, 0x100, 0x1200)
        image = FakeImage(bytes(data))
        image.relocations = (0x1002, 0x1012)
        image.get_code_regions = lambda: iter((SimpleNamespace(addr=0x1200, data=b"\xc3"),))
        purecall = SimpleNamespace(best_name=lambda: "__purecall")
        matches = [SimpleNamespace(
            type=EntityType.FUNCTION,
            rdiff=SimpleNamespace(orig_inst=[("0x1000", "mov dword ptr [eax], <OFFSET1>")]),
            name="Constructor",
        )]
        engine = SimpleNamespace(
            orig_bin=image,
            _db=SimpleNamespace(get=lambda _, address: purecall if address == 0x1200 else None),
            compare_all=lambda: iter(matches),
        )
        self.assertEqual(unannotated_vtable_stores(engine, {0x1120}), {})
        self.assertEqual(set(unannotated_vtable_stores(engine, set())), {0x1100})
        image.data = image.data[:0x0d] + b"\xe8\x00\x00\x00\x00" + image.data[0x12:]
        self.assertEqual(set(unannotated_vtable_stores(engine, {0x1120})), {0x1100})
        image.data = bytes(data)
        engine._db = SimpleNamespace(get=lambda _, address: (
            SimpleNamespace(best_name=lambda: "Other") if address == 0x1200 else None
        ))
        self.assertEqual(set(unannotated_vtable_stores(engine, {0x1120})), {0x1100})


if __name__ == "__main__":
    unittest.main()
