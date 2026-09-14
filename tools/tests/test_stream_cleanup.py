"""Verify the local debug stream owns its stream buffer as a member."""

import struct
import unittest

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from reccmp.types import ImageId

from lib.paths import ORIGINAL_EXE, RECOMP_EXE, RECOMP_PDB
from lib.reccmp import load_engine


@unittest.skipUnless(
    all(path.exists() for path in (ORIGINAL_EXE, RECOMP_EXE, RECOMP_PDB)),
    "requires the reference executable and a local build",
)
class StreamCleanupTests(unittest.TestCase):
    def test_virtual_base_tables_match_original_offsets(self):
        _, engine = load_engine()
        matches = {match.orig_addr: match for match in engine.get_all()}
        for original, offsets in ((0x00498898, (0, 0x12c)),
                                  (0x00493000, (0, -0x20)),
                                  (0x00493010, (0, 0x20, 0x40))):
            expected = struct.pack("<" + "i" * len(offsets), *offsets)
            match = matches[original]
            for image, address in ((engine.orig_bin, original),
                                   (engine.recomp_bin, match.recomp_addr)):
                with self.subTest(original=hex(original), address=hex(address)):
                    self.assertEqual(image.read(address, len(expected)), expected)

    def test_member_buffer_precedes_virtual_base_cleanup(self):
        _, engine = load_engine()
        matches = {match.orig_addr: match for match in engine.get_all()}
        destructor = matches[0x00407e80]
        decoder = Cs(CS_ARCH_X86, CS_MODE_32)
        decoder.detail = True
        callees = (0x0045adc0, 0x004584a0, 0x00458440, 0x0045a790)
        for image, address, size, targets, vtable in (
            (engine.orig_bin, destructor.orig_addr, 69, callees, 0x00493020),
            (engine.recomp_bin, destructor.recomp_addr, destructor.size(ImageId.RECOMP),
             tuple(matches[a].recomp_addr for a in callees), matches[0x00493020].recomp_addr),
        ):
            with self.subTest(address=hex(address)):
                instructions = list(decoder.disasm(image.read(address, size), address))
                calls = [i for i in instructions if i.mnemonic == "call"]
                self.assertEqual([i.operands[0].imm for i in calls], list(targets))
                self.assertEqual(instructions[2].op_str, "esi, [ecx - 0x20]")
                self.assertEqual(instructions[3].op_str, "edi, ecx")
                self.assertEqual(instructions[4].op_str, "eax, dword ptr [esi]")
                self.assertEqual(instructions[5].op_str, "ecx, dword ptr [eax + 4]")
                restore = instructions[6]
                self.assertEqual(restore.mnemonic, "mov")
                self.assertEqual(restore.operands[0].mem.disp, -0x20)
                self.assertEqual(restore.operands[1].imm, vtable)
                # EDI is owner + 0x20; the buffer is owner + 4, not a base at zero.
                self.assertEqual(instructions[7].op_str, "ecx, [edi - 0x1c]")
                self.assertEqual(instructions[9].op_str, "ecx, [edi + 0x14c]")
                self.assertEqual(instructions[11].op_str, "ecx, edi")
                self.assertEqual(instructions[13].mnemonic, "test")
                self.assertEqual(instructions[13].op_str, "byte ptr [esp + 0xc], 1")
                self.assertEqual(instructions[14].mnemonic, "je")
                self.assertEqual(instructions[15].op_str, "esi")
                self.assertEqual(instructions[18].op_str, "eax, esi")
                self.assertEqual(instructions[14].operands[0].imm, instructions[18].address)
                self.assertEqual(instructions[-1].mnemonic, "ret")
                self.assertEqual(instructions[-1].op_str, "4")


if __name__ == "__main__":
    unittest.main()
