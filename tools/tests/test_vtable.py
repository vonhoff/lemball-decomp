"""Tests for vtable comparison helpers and adjuster thunk reconciliation."""

import struct
import unittest

from lib.vtable import decode_this_adjuster


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


if __name__ == "__main__":
    unittest.main()
