"""Verify all three conditional waypoint cleanups in the Enemy destructor."""

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
class EnemyCleanupTests(unittest.TestCase):
    def test_patrol_states_release_waypoints_before_their_owner(self):
        _, engine = load_engine()
        matches = {match.orig_addr: match for match in engine.get_all()}
        destructor = matches[0x0041fda0]
        decoder = Cs(CS_ARCH_X86, CS_MODE_32)
        decoder.detail = True
        for image, address, size, delete in (
            (engine.orig_bin, destructor.orig_addr, 144, 0x0045a790),
            (engine.recomp_bin, destructor.recomp_addr, destructor.size(ImageId.RECOMP),
             matches[0x0045a790].recomp_addr),
        ):
            with self.subTest(address=hex(address)):
                instructions = list(decoder.disasm(image.read(address, size), address))
                guards = [i for i in instructions if i.mnemonic == "cmp"]
                self.assertEqual([i.operands[0].mem.disp for i in guards],
                                 [0x12c, 0x138, 0x144])
                self.assertTrue(all(i.operands[0].size == 4 and i.operands[1].imm == 1
                                    for i in guards))
                calls = [i for i in instructions if i.mnemonic == "call"]
                self.assertEqual(len(calls), 7)
                self.assertEqual([i.operands[0].imm for i in calls[:6]], [delete] * 6)
                branches = [i for i in instructions if i.mnemonic == "jne"]
                self.assertEqual(len(branches), 3)
                for index, pointer in enumerate((0x134, 0x140, 0x14c)):
                    branch = branches[index]
                    end = branch.operands[0].imm
                    block = [i for i in instructions if branch.address < i.address < end]
                    loads = [i.operands[1].mem.disp for i in block
                             if i.mnemonic == "mov" and i.operands[1].type == X86_OP_MEM]
                    self.assertEqual(loads, [pointer, 0x10, pointer])
                    self.assertEqual(sum(i.mnemonic == "call" for i in block), 2)
                    self.assertEqual(end, guards[index + 1].address if index < 2
                                     else calls[-1].address - 2)


if __name__ == "__main__":
    unittest.main()
