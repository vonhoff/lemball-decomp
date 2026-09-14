"""Verify the framed-button allocations and their array-delete helpers."""

import unittest

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM
from reccmp.types import ImageId

from lib.paths import ORIGINAL_EXE, RECOMP_EXE, RECOMP_PDB
from lib.reccmp import load_engine


@unittest.skipUnless(
    all(path.exists() for path in (ORIGINAL_EXE, RECOMP_EXE, RECOMP_PDB)),
    "requires the reference executable and a local build",
)
class FramePrimitiveTests(unittest.TestCase):
    def test_input_text_destructor_guards_owned_buffer_delete(self):
        for address, instructions in self.instructions(0x0043a190, 47):
            with self.subTest(address=hex(address)):
                loads = [i.operands[1].mem.disp for i in instructions
                         if i.mnemonic == "mov" and len(i.operands) == 2
                         and i.operands[1].type == X86_OP_MEM]
                self.assertEqual(loads, [0xbc])
                branch = next(index for index, i in enumerate(instructions) if i.mnemonic == "je")
                calls = [index for index, i in enumerate(instructions) if i.mnemonic == "call"]
                self.assertEqual(len(calls), 2)
                self.assertLess(branch, calls[0])
                target = instructions[branch].operands[0].imm
                self.assertGreater(target, instructions[calls[0]].address)
                self.assertLess(target, instructions[calls[1]].address)

    def test_text_alignment_uses_signed_centering_and_original_flags(self):
        for address, instructions in self.instructions(0x00469180, 143):
            with self.subTest(address=hex(address)):
                masks = [i.operands[1].imm for i in instructions if i.mnemonic == "test"
                         and i.operands[1].type == X86_OP_IMM]
                self.assertEqual(masks, [0x10, 4, 0x20, 8])
                self.assertEqual(sum(i.mnemonic == "cdq" for i in instructions), 2)
                shifts = [i for i in instructions if i.mnemonic == "sar"]
                self.assertEqual(len(shifts), 2)
                self.assertTrue(all(i.operands[0].size == 4 and i.operands[1].imm == 1
                                    for i in shifts))

    def test_text_expansion_preserves_product_guard_and_word_bounds(self):
        for address, instructions in self.instructions(0x00469120, 87):
            with self.subTest(address=hex(address)):
                self.assertEqual(sum(i.mnemonic == "imul" for i in instructions), 1)
                comparisons = [i for i in instructions if i.mnemonic == "cmp"]
                self.assertEqual([i.operands[0].mem.disp for i in comparisons], [0xc0, 0xc2])
                self.assertTrue(all(i.operands[0].size == 2 for i in comparisons))
                self.assertEqual(sum(i.mnemonic == "jge" for i in instructions), 2)

    def test_text_button_draw_uses_original_virtual_slots(self):
        for address, instructions in self.instructions(0x00469480, 172):
            with self.subTest(address=hex(address)):
                calls = [i.operands[0] for i in instructions if i.mnemonic == "call"]
                self.assertTrue(all(operand.type == X86_OP_MEM for operand in calls))
                self.assertEqual([operand.mem.disp for operand in calls], [0x38, 0x14, 4])

    def test_text_button_destructor_releases_font_before_array(self):
        _, engine = load_engine()
        unload = next(m for m in engine.get_all() if m.orig_addr == 0x0045d180)
        for (address, instructions), target in zip(
            self.instructions(0x00469440, 55), (unload.orig_addr, unload.recomp_addr),
        ):
            with self.subTest(address=hex(address)):
                calls = [i for i in instructions if i.mnemonic == "call"]
                self.assertEqual(calls[0].operands[0].imm, target)
                self.assertEqual(calls[1].operands[0].type, X86_OP_MEM)
                flags = [i.operands[0].imm for i in instructions
                         if i.mnemonic == "push" and i.operands[0].type == X86_OP_IMM]
                self.assertEqual(flags, [3])

    def test_zrle_destructor_restores_primitive_vtable(self):
        _, engine = load_engine()
        primitive = next(m for m in engine.get_all() if m.orig_addr == 0x00496ca8)
        for (address, instructions), vtable in zip(
            self.instructions(0x00467ba0, 7),
            (primitive.orig_addr, primitive.recomp_addr),
        ):
            with self.subTest(address=hex(address)):
                self.assertEqual([i.mnemonic for i in instructions], ["mov", "ret"])
                destination, value = instructions[0].operands
                self.assertEqual(destination.type, X86_OP_MEM)
                self.assertEqual(instructions[0].reg_name(destination.mem.base), "ecx")
                self.assertEqual(destination.mem.disp, 0)
                self.assertEqual(destination.size, 4)
                self.assertEqual(value.type, X86_OP_IMM)
                self.assertEqual(value.imm, vtable)

    def test_text_constructor_initializes_only_original_fields(self):
        for address, instructions in self.instructions(0x00469a00, 71):
            with self.subTest(address=hex(address)):
                stores = [(i.operands[0].mem.disp, i.operands[0].size)
                          for i in instructions if i.mnemonic == "mov"
                          and i.operands[0].type == X86_OP_MEM]
                self.assertCountEqual(stores, [
                    (0, 4), (0, 4), (0x2c, 4), (0x2c, 4),
                    (0x0c, 2), (0x0e, 2), (0x18, 2), (0x1a, 2),
                    (0x1c, 2), (0x1e, 2), (0x24, 2), (0x26, 2),
                    (0x30, 2), (0x32, 2),
                ])

    def test_text_allocation_has_one_element_cookie(self):
        for address, instructions in self.instructions(0x004693b0, 131):
            with self.subTest(address=hex(address)):
                sizes = [i.operands[0].imm for i in instructions
                         if i.mnemonic == "push" and i.operands[0].type == X86_OP_IMM]
                self.assertEqual(sizes, [0x48])
                cookies = [i.operands[1].imm for i in instructions
                           if i.mnemonic == "mov" and len(i.operands) == 2
                           and i.operands[0].type == X86_OP_MEM
                           and i.operands[0].mem.disp == 0
                           and i.operands[1].type == X86_OP_IMM]
                self.assertEqual(cookies, [1])

    def test_text_delete_helper_handles_array_stride(self):
        for address, instructions in self.instructions(0x00469b80, 98):
            with self.subTest(address=hex(address)):
                tests = [i for i in instructions if i.mnemonic == "test"]
                self.assertEqual(tests[0].operands[1].imm, 2)
                self.assertTrue(any(i.mnemonic == "sub" and len(i.operands) == 2
                                    and i.operands[1].type == X86_OP_IMM
                                    and i.operands[1].imm == 0x44 for i in instructions))
                self.assertEqual([i.operands[0].imm for i in instructions
                                  if i.mnemonic == "ret"], [4, 4])

    def test_resource_destructor_deletes_one_owned_interface(self):
        for address, instructions in self.instructions(0x00468ec0, 54):
            with self.subTest(address=hex(address)):
                flags = [i.operands[0].imm for i in instructions
                         if i.mnemonic == "push" and i.operands[0].type == X86_OP_IMM]
                self.assertEqual(flags, [1])
                loads = [i.operands[1].mem.disp for i in instructions
                         if i.mnemonic == "mov" and len(i.operands) == 2
                         and i.operands[1].type == X86_OP_MEM]
                self.assertEqual(loads[:2], [0x88, 0x90])

    def test_frame_destructor_deletes_both_owned_arrays(self):
        for address, instructions in self.instructions(0x00468c10, 57):
            with self.subTest(address=hex(address)):
                flags = [i.operands[0].imm for i in instructions
                         if i.mnemonic == "push" and i.operands[0].type == X86_OP_IMM]
                self.assertEqual(flags, [3, 3])
                loads = [i.operands[1].mem.disp for i in instructions
                         if i.mnemonic == "mov" and len(i.operands) == 2
                         and i.operands[1].type == X86_OP_MEM]
                # ECX is the hot-area subobject at owner+0x90.
                self.assertEqual(loads[:1], [0x80])
                self.assertIn(0x7c, loads)

    def test_frame_vtables_use_reconstructed_overrides(self):
        _, engine = load_engine()
        matches = {m.orig_addr: m for m in engine.get_all()}
        for table, slot, target in (
            (0x00499838, 0xa8, 0x00468dd0),
            (0x00499838, 0xbc, 0x00468c50),
            (0x00499818, 0, 0x00469900),
            (0x00499928, 0x40, 0x00469990),
            (0x00499928, 0xa8, 0x00468f80),
            (0x00499908, 0, 0x004699a0),
            (0x00499a18, 0xa8, 0x00469530),
            (0x00499a18, 0xbc, 0x00469480),
            (0x004999f8, 0, 0x004699d0),
        ):
            for image, table_address, target_address in (
                (engine.orig_bin, table, target),
                (engine.recomp_bin, matches[table].recomp_addr, matches[target].recomp_addr),
            ):
                with self.subTest(table=hex(table_address), slot=hex(slot)):
                    entry = int.from_bytes(image.read(table_address + slot, 4), "little")
                    self.assertEqual(entry, target_address)

    def instructions(self, original_address, original_size):
        _, engine = load_engine()
        match = next(m for m in engine.get_all() if m.orig_addr == original_address)
        decoder = Cs(CS_ARCH_X86, CS_MODE_32)
        decoder.detail = True
        for image, address, size in (
            (engine.orig_bin, original_address, original_size),
            (engine.recomp_bin, match.recomp_addr, match.size(ImageId.RECOMP)),
        ):
            yield address, list(decoder.disasm(image.read(address, size), address))

    def test_frame_allocations_have_array_cookies(self):
        for address, instructions in self.instructions(0x00468b80, 144):
            with self.subTest(address=hex(address)):
                sizes = [i.operands[0].imm for i in instructions
                         if i.mnemonic == "push" and i.operands[0].type == X86_OP_IMM]
                self.assertEqual(sizes, [0x14, 0x44])
                cookies = [i.operands[1].imm for i in instructions
                           if i.mnemonic == "mov" and len(i.operands) == 2
                           and i.operands[0].type == X86_OP_MEM
                           and i.operands[0].mem.disp == 0
                           and i.operands[1].type == X86_OP_IMM]
                self.assertEqual(cookies, [1, 4])

    def test_both_delete_helpers_handle_arrays(self):
        for helper in (0x00469930, 0x00467bb0):
            for address, instructions in self.instructions(helper, 89):
                with self.subTest(helper=hex(helper), address=hex(address)):
                    tests = [i for i in instructions if i.mnemonic == "test"]
                    self.assertEqual(tests[0].operands[1].imm, 2)
                    self.assertTrue(any(i.mnemonic == "sub" and len(i.operands) == 2
                                        and i.operands[1].type == X86_OP_IMM
                                        and i.operands[1].imm == 0x10 for i in instructions))
                    self.assertEqual([i.operands[0].imm for i in instructions
                                      if i.mnemonic == "ret"], [4, 4])
