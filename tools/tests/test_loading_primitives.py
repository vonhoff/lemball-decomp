"""Loading-animation needles must retain the original line-rendering primitive."""

import unittest

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from reccmp.types import ImageId

from lib.paths import ORIGINAL_EXE, RECOMP_EXE, RECOMP_PDB
from lib.reccmp import load_engine


@unittest.skipUnless(
    all(path.exists() for path in (ORIGINAL_EXE, RECOMP_EXE, RECOMP_PDB)),
    "requires the reference executable and a local build",
)
class LoadingPrimitiveTests(unittest.TestCase):
    def test_constructor_builds_three_cliprect_needles(self):
        _, engine = load_engine()
        matches = {match.orig_addr: match for match in engine.get_all()}
        constructor = matches[0x0044aa80]
        needle = matches[0x00432a30]
        decoder = Cs(CS_ARCH_X86, CS_MODE_32)

        def calls_to(image, address, size, target):
            count = 0
            for _, _, mnemonic, operand in decoder.disasm_lite(image.read(address, size), address):
                if mnemonic != "call" or not operand.startswith("0x"):
                    continue
                destination = int(operand, 16)
                # Original constructor calls go through linker jump thunks.
                instruction = next(decoder.disasm_lite(image.read(destination, 5), destination), None)
                if instruction and instruction[2] == "jmp" and instruction[3].startswith("0x"):
                    destination = int(instruction[3], 16)
                count += destination == target
            return count

        self.assertEqual(calls_to(engine.orig_bin, 0x0044aa80, 0x2de, 0x00432a30), 3)
        self.assertEqual(calls_to(engine.recomp_bin, constructor.recomp_addr,
                                  constructor.size(ImageId.RECOMP), needle.recomp_addr), 3)
