"""Verify TCP/IP asynchronous-buffer cleanup before window destruction."""

import unittest

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_MEM
from reccmp.types import ImageId

from lib.paths import ORIGINAL_EXE, RECOMP_EXE, RECOMP_PDB
from lib.reccmp import load_engine


@unittest.skipUnless(
    all(path.exists() for path in (ORIGINAL_EXE, RECOMP_EXE, RECOMP_PDB)),
    "requires the reference executable and a local build",
)
class SocketCleanupTests(unittest.TestCase):
    def test_async_buffer_is_conditionally_freed_before_window_cleanup(self):
        _, engine = load_engine()
        matches = {match.orig_addr: match for match in engine.get_all()}
        destructor = matches[0x0046fd70]
        decoder = Cs(CS_ARCH_X86, CS_MODE_32)
        decoder.detail = True
        for image, address, size, delete, window in (
            (engine.orig_bin, destructor.orig_addr, 62, 0x0045a790, 0x0047c140),
            (engine.recomp_bin, destructor.recomp_addr, destructor.size(ImageId.RECOMP),
             matches[0x0045a790].recomp_addr, matches[0x0047c140].recomp_addr),
        ):
            with self.subTest(address=hex(address)):
                instructions = list(decoder.disasm(image.read(address, size), address))
                self.assertTrue(any(i.mnemonic == "lea" and i.op_str == "esi, [ecx - 0x20]"
                                    for i in instructions))
                calls = [i for i in instructions if i.mnemonic == "call"]
                self.assertEqual([i.operands[0].imm for i in calls], [delete, window])
                loads = [i for i in instructions if i.mnemonic == "mov"
                         and i.operands[1].type == X86_OP_MEM
                         and i.operands[1].mem.disp == -0xc]
                self.assertEqual(len(loads), 1)
                load = loads[0]
                self.assertEqual(load.op_str, "eax, dword ptr [ecx - 0xc]")
                index = instructions.index(load)
                self.assertEqual(instructions[index + 1].mnemonic, "test")
                self.assertEqual(instructions[index + 1].op_str, "eax, eax")
                branch = instructions[index + 2]
                self.assertEqual(branch.mnemonic, "je")
                self.assertEqual(instructions[index + 3].mnemonic, "push")
                self.assertEqual(instructions[index + 3].op_str, "eax")
                self.assertEqual(instructions[index + 4].address, calls[0].address)
                cleanup = next(i for i in instructions if i.address == branch.operands[0].imm)
                self.assertEqual(cleanup.mnemonic, "mov")
                self.assertEqual(cleanup.op_str, "ecx, esi")
                self.assertEqual(cleanup.address + cleanup.size, calls[1].address)
                self.assertEqual(instructions[-1].mnemonic, "ret")


if __name__ == "__main__":
    unittest.main()
