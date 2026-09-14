"""Loading-animation needles must retain the original line-rendering primitive."""

import unittest

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
from reccmp.types import ImageId

from lib.paths import ORIGINAL_EXE, RECOMP_EXE, RECOMP_PDB
from lib.reccmp import load_engine


@unittest.skipUnless(
    all(path.exists() for path in (ORIGINAL_EXE, RECOMP_EXE, RECOMP_PDB)),
    "requires the reference executable and a local build",
)
class LoadingPrimitiveTests(unittest.TestCase):
    def test_destructor_restores_both_callback_vtables_before_cleanup(self):
        _, engine = load_engine()
        matches = {match.orig_addr: match for match in engine.get_all()}
        destructor = matches[0x0044ad60]
        decoder = Cs(CS_ARCH_X86, CS_MODE_32)
        decoder.detail = True
        for image, address, size, tables in (
            (engine.orig_bin, 0x0044ad60, 276, {0x70: 0x00497c8c, 0x74: 0x00497c88}),
            (engine.recomp_bin, destructor.recomp_addr, destructor.size(ImageId.RECOMP),
             {0x70: matches[0x00497c8c].recomp_addr, 0x74: matches[0x00497c88].recomp_addr}),
        ):
            with self.subTest(address=hex(address)):
                stores = {}
                for instruction in decoder.disasm(image.read(address, size), address):
                    if instruction.mnemonic == "call":
                        break
                    operands = instruction.operands
                    if (instruction.mnemonic == "mov" and len(operands) == 2
                            and operands[0].type == X86_OP_MEM and operands[1].type == X86_OP_IMM
                            and operands[0].mem.disp in tables):
                        stores[operands[0].mem.disp] = operands[1].imm
                self.assertEqual(stores, tables)

    def test_centering_subtracts_at_word_width_before_division(self):
        _, engine = load_engine()
        constructor = next(m for m in engine.get_all() if m.orig_addr == 0x0044aa80)
        decoder = Cs(CS_ARCH_X86, CS_MODE_32)
        decoder.detail = True
        for image, address, size in (
            (engine.orig_bin, 0x0044aa80, 0x2de),
            (engine.recomp_bin, constructor.recomp_addr, constructor.size(ImageId.RECOMP)),
        ):
            with self.subTest(address=hex(address)):
                subtractions = []
                for instruction in decoder.disasm(image.read(address, size), address):
                    operands = instruction.operands
                    if (instruction.mnemonic == "sub" and len(operands) == 2
                            and operands[0].type == X86_OP_REG and operands[1].type == X86_OP_MEM
                            and operands[1].mem.disp in (0x48, 0x4a)):
                        subtractions.append((operands[1].mem.disp, operands[0].size, operands[1].size))
                self.assertEqual(sorted(subtractions), [(0x48, 2, 2), (0x4a, 2, 2)])

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
