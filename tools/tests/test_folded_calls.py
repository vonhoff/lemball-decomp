from types import SimpleNamespace
import unittest
from unittest.mock import patch

from lib.reccmp_compat import identical_folded_aliases, RelocationAwareParseAsm


class FoldedCallTests(unittest.TestCase):
    def test_alias_requires_equal_size_and_identical_bytes(self):
        bodies = {0x2000: b"\xb8\x01\x00\x00\x00\xc3",
                  0x3000: b"\xb8\x01\x00\x00\x00\xc3",
                  0x4000: b"\xb8\x02\x00\x00\x00\xc3",
                  0x5000: b"\xc3"}
        match = SimpleNamespace(recomp_addr=0x2000, size=lambda side: 6)
        engine = SimpleNamespace(
            code_files=[], target_id="LEMBALL", project_aliases={},
            _db=SimpleNamespace(
                get_one_match=lambda original: match,
                get=lambda side, address: SimpleNamespace(size=lambda side: len(bodies[address])),
            ),
            recomp_bin=SimpleNamespace(read=lambda address, size: bodies[address][:size]),
        )
        with patch("reccmp.parser.codebase.DecompCodebase"), patch(
            "lib.vtable.collect_folded_aliases", return_value={0x1000: {0x3000, 0x4000, 0x5000}}
        ):
            self.assertEqual(identical_folded_aliases(engine), {0x3000: 0x2000})

    def test_only_direct_calls_use_alias_identity(self):
        names = {0x2000: "Primary (FUNCTION)", 0x3000: "Folded (FUNCTION)"}
        parser = RelocationAwareParseAsm(
            function_aliases={0x3000: 0x2000},
            addr_test=lambda address: address in names,
            name_lookup=lambda address, **kwargs: names.get(address),
        )
        self.assertEqual(parser.sanitize((0x4000, 5, "call", "0x3000")),
                         ("call", "Primary (FUNCTION)"))
        self.assertEqual(parser.sanitize((0x4000, 5, "push", "0x3000")),
                         ("push", "Folded (FUNCTION)"))
        self.assertEqual(parser.sanitize((0x4000, 2, "call", "eax")), ("call", "eax"))

    def test_shared_primitive_cleanup_is_byte_identical(self):
        from lib.reccmp import load_engine
        _, engine = load_engine()
        match = engine._db.get_one_match(0x439730)
        aliases = identical_folded_aliases(engine)
        other = [address for address, primary in aliases.items()
                 if primary == match.recomp_addr and address != primary]
        self.assertTrue(other)
        for address in other:
            self.assertEqual(engine.recomp_bin.read(address, 7),
                             engine.recomp_bin.read(match.recomp_addr, 7))
        original = bytes(engine.orig_bin.read(0x439730, 7))
        self.assertEqual(original, b"\xc7\x01\xa8\x6c\x49\x00\xc3")
