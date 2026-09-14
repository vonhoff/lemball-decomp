"""Pause-window border tile counts retain the original signed word division."""

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
class PauseLayoutTests(unittest.TestCase):
    def test_border_counts_use_signed_word_division(self):
        _, engine = load_engine()
        function = next(m for m in engine.get_all() if m.orig_addr == 0x00444050)
        decoder = Cs(CS_ARCH_X86, CS_MODE_32)
        decoder.detail = True
        for image, address, size in (
            (engine.orig_bin, 0x00444050, 0x627),
            (engine.recomp_bin, function.recomp_addr, function.size(ImageId.RECOMP)),
        ):
            with self.subTest(address=hex(address)):
                instructions = list(decoder.disasm(image.read(address, size), address))
                divisions = [i for i, ins in enumerate(instructions) if ins.mnemonic == "idiv"]
                word_divisions = [i for i in divisions if instructions[i].operands[0].size == 2]
                self.assertEqual(len(word_divisions), 2)
                self.assertEqual(divisions[-2:], word_divisions)
                for i in word_divisions:
                    self.assertEqual(instructions[i - 1].mnemonic, "cwd")
                count_stores = []
                for ins in instructions[word_divisions[-1] + 1:]:
                    if ins.mnemonic == "call":
                        break
                    if (ins.mnemonic == "mov" and ins.operands[0].type == X86_OP_MEM
                            and ins.operands[0].mem.disp in (0x130, 0x132)):
                        count_stores.append((ins.operands[0].mem.disp, ins.operands[0].size))
                self.assertEqual(count_stores, [(0x130, 2), (0x132, 2)])
