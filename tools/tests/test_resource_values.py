"""Check scalar resource type tags and full-width payload copies."""

import unittest

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from reccmp.types import ImageId

from lib.paths import ORIGINAL_EXE, RECOMP_EXE, RECOMP_PDB
from lib.reccmp import load_engine


@unittest.skipUnless(
    all(path.exists() for path in (ORIGINAL_EXE, RECOMP_EXE, RECOMP_PDB)),
    "requires the reference executable and a local build",
)
class ResourceValueTests(unittest.TestCase):
    def test_bool_resource_preserves_dword_payload(self):
        _, engine = load_engine()
        matches = {match.orig_addr: match for match in engine.get_all()}
        decoder = Cs(CS_ARCH_X86, CS_MODE_32)
        expected = {
            0x0045e9b0: [
                ("mov", "dword ptr [ecx + 0x40], 0x424f4f4c"),
                ("ret", ""),
            ],
            0x0045e9c0: [
                ("mov", "eax, dword ptr [ecx + 0x38]"),
                ("mov", "edx, dword ptr [eax]"),
                ("mov", "dword ptr [ecx + 0x48], edx"),
                ("ret", ""),
            ],
        }
        for original, operations in expected.items():
            match = matches[original]
            for image, address, size in (
                (engine.orig_bin, original, 8 if original == 0x0045e9b0 else 9),
                (engine.recomp_bin, match.recomp_addr, match.size(ImageId.RECOMP)),
            ):
                with self.subTest(original=hex(original), address=hex(address)):
                    decoded = [(i.mnemonic, i.op_str)
                               for i in decoder.disasm(image.read(address, size), address)]
                    # A BOOL resource stores the complete dword, not a C++ bool conversion.
                    self.assertEqual(decoded, operations)


if __name__ == "__main__":
    unittest.main()
