import struct
from types import SimpleNamespace
import unittest

from lib.reccmp_compat import incremental_thunks, RelocationAwareParseAsm


class IncrementalThunkTests(unittest.TestCase):
    def image(self, data):
        return SimpleNamespace(get_code_regions=lambda: [SimpleNamespace(addr=0x1000, data=data)])

    def test_stripped_table_requires_padding_and_forward_code_targets(self):
        jump = b"\xe9" + struct.pack("<i", 0x1020 - 0x1005)
        valid = jump + b"\xcc" * 27 + b"\xc3"
        self.assertEqual(incremental_thunks(self.image(valid)), {0x1000: 0x1020})
        for invalid in (jump, jump + b"\x90" * 27 + b"\xc3",
                        b"\x90" + valid, b"\xe9\xfb\xff\xff\xff" + valid[5:],
                        b"\xe9\xff\xff\xff\x7f" + valid[5:]):
            with self.subTest(data=invalid):
                self.assertEqual(incremental_thunks(self.image(invalid)), {})

    def parser(self, names):
        return RelocationAwareParseAsm(
            thunk_targets={0x1000: 0x1020},
            name_lookup=lambda address, **kwargs: names.get(address),
        )

    def test_call_uses_matched_destination_identity(self):
        parser = self.parser({0x1020: "Target (FUNCTION)"})
        self.assertEqual(parser.sanitize((0x2000, 5, "call", "0x1000")),
                         parser.sanitize((0x2000, 5, "call", "0x1020")))

    def test_existing_identity_is_preserved(self):
        parser = self.parser({0x1000: "Existing (FUNCTION)", 0x1020: "Target (FUNCTION)"})
        self.assertEqual(parser.sanitize((0x2000, 5, "call", "0x1000")),
                         ("call", "Existing (FUNCTION)"))

    def test_unknown_destination_and_indirect_calls_are_unchanged(self):
        for names, operand in (({}, "0x1000"), ({0x1020: "Target (FUNCTION)"}, "eax")):
            parser = self.parser(names)
            expected = RelocationAwareParseAsm(name_lookup=lambda address, **kwargs: names.get(address))
            instruction = (0x2000, 5, "call", operand)
            self.assertEqual(parser.sanitize(instruction), expected.sanitize(instruction))

    def test_original_calls_resolve_to_verified_helpers(self):
        from lib.reccmp import load_engine
        _, engine = load_engine()
        image = engine.orig_bin
        self.assertFalse(image.is_debug)
        thunks = incremental_thunks(image)
        parser = engine.function_comparator.orig_sanitize
        for thunk, target, name in (
            (0x4013ca, 0x429e50, "Pt3::InitializeFromAiCoord"),
            (0x4018a7, 0x40c270, "C3DVector::operator="),
            (0x402482, 0x429e80, "ViewData::SetViewActionTuple"),
        ):
            with self.subTest(thunk=hex(thunk)):
                self.assertEqual(thunks[thunk], target)
                self.assertEqual(image.read(thunk, 1), b"\xe9")
                self.assertEqual(parser.sanitize((0x500000, 5, "call", hex(thunk))),
                                 ("call", name + " (FUNCTION)"))
