"""Binary evidence for the CRT comparisons affected by relocation recognition.

These integration checks use the current PDB mappings, never fixed rebuilt
addresses. They are skipped when the local reference/build inputs are absent.
"""

import struct
import unittest

from lib.paths import ORIGINAL_EXE, RECOMP_EXE, RECOMP_PDB
from lib.reccmp import load_engine


@unittest.skipUnless(
    all(path.exists() for path in (ORIGINAL_EXE, RECOMP_EXE, RECOMP_PDB)),
    "requires the reference executable and a local build",
)
class CrtPointerRelocationTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        _, cls.engine = load_engine()
        cls.matches = {match.orig_addr: match for match in cls.engine.get_all()}

    def sides(self, original):
        match = self.matches[original]
        return ((self.engine.orig_bin, original),
                (self.engine.recomp_bin, match.recomp_addr))

    def pointer(self, image, address):
        self.assertIn(address, image.relocations)
        return struct.unpack("<I", image.read(address, 4))[0]

    def test_code_page_comparison_is_end_of_same_five_entry_table(self):
        tables = []
        for image, address in self.sides(0x004824D0):
            begin = self.pointer(image, address + 0x62)
            end = self.pointer(image, address + 0x76)
            self.assertEqual(end - begin, 5 * 0x30)
            tables.append(bytes(image.read(begin, end - begin)))
        self.assertEqual(*tables)

    def test_io_comparisons_are_end_of_same_64_pointer_array(self):
        for original, begin_offset, end_offset in (
            (0x004829E0, 4, 0x4A),
            (0x00484590, 0xD, 0xBD),
        ):
            for image, address in self.sides(original):
                with self.subTest(function=hex(original), address=hex(address)):
                    begin = self.pointer(image, address + begin_offset)
                    end = self.pointer(image, address + end_offset)
                    self.assertEqual(end - begin, 64 * 4)

    def test_abnormal_termination_compares_same_embedded_unwind_handler(self):
        unwind = self.matches[0x004819A8]
        normalized = []
        for (image, address), unwind_address in zip(
            self.sides(0x00481A52), (unwind.orig_addr, unwind.recomp_addr)
        ):
            target = self.pointer(image, address + 0xC)
            self.assertEqual(target - unwind_address, 0x20)
            code = bytearray(image.read(address, 35))
            code[0xC:0x10] = bytes(4)
            normalized.append(code)
        self.assertEqual(*normalized)
