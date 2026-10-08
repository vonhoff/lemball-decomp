"""Check relocation overlays and rejection of stale thunk evidence."""

import hashlib
import json
import struct
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

from reccmp.types import EntityType

from inventory_thunks import references
from link_binary import CoffObject, DIR32, REL32, prepare_link, thunk_symbol
from recover_thunks import name_jump_destinations, native_definition, reference_matches


def object_bytes(*, data_kind=DIR32, data_target=2, addend=0):
    code = b"\xe8" + bytes(4) + b"\xc3\xc3\xc3"
    data = b"HEAD" + struct.pack("<I", addend) + b"TAIL"
    sections = b"".join(
        struct.pack("<8sIIIIIIHHI", name, 0, 0, size, raw, rel, 0, 1, 0, flags)
        for name, size, raw, rel, flags in (
            (b".text", 8, 100, 120, 0x60100020),
            (b".rdata", 12, 108, 130, 0x40100040),
        )
    )
    symbols = b"".join(
        struct.pack("<8sIhHBB", name, value, section, kind, 2, 0)
        for name, value, section, kind in (
            (b"_call", 0, 1, 0x20),
            (b"_data", 0, 2, 0),
            (b"_body", 6, 1, 0x20),
            (b"_alias", 7, 1, 0x20),
            (b"_other", 0, 0, 0x20),
        )
    )
    return (
        struct.pack("<HHIIIHH", 0x14C, 2, 0, 140, 5, 0, 0)
        + sections
        + code
        + data
        + struct.pack("<IIH", 1, 2, REL32)
        + struct.pack("<IIH", 4, data_target, data_kind)
        + symbols
        + struct.pack("<I", 4)
        + b"DEBUG"
    )


def routes(obj):
    common = {"object": "native.obj"}
    code = {
        **common,
        "caller": 0x400100,
        "symbol": "_call",
        "size": 6,
        "fingerprint": obj.fingerprint("_call", 6),
        "references": [
            {
                "offset": 0,
                "original_instruction": 0x400100,
                "original_bytes": "e8fb0e0000",
                "thunk": 0x401000,
                "source_symbol": "_body",
            }
        ],
    }
    data = {
        **common,
        "caller": 0x490000,
        "kind": "data",
        "symbol": "_data",
        "size": 12,
        "fingerprint": obj.fingerprint("_data", 12),
        "references": [
            {
                "offset": 4,
                "original_pointer": 0x490004,
                "original_bytes": "00104000",
                "thunk": 0x401000,
                "source_symbol": "_body",
            }
        ],
    }
    return [code, data]


def thunk():
    return {
        "address": 0x401000,
        "target": 0x403000,
        "original_bytes": "e9fb1f0000",
        "symbol": thunk_symbol(0x401000),
        "target_symbol": "_body",
    }


class RelocationOverlayTests(unittest.TestCase):
    def test_inventory_includes_unpaired_catalogued_code_and_relocated_pointers(self):
        code_address = 0x410010
        target = 0x401000
        call = b"\xe8" + struct.pack("<i", target - code_address - 5) + b"\xc3"
        image = SimpleNamespace(
            get_code_regions=lambda: [SimpleNamespace(addr=0x410000, data=bytes(32))],
            relocations={0x490000},
            read=lambda address, size: {
                0x410000: b"\xc3",
                code_address: call,
                0x490000: struct.pack("<I", target),
            }[address][:size],
        )
        owner = SimpleNamespace(
            orig_addr=0x410000,
            entity_type=EntityType.FUNCTION,
            size=lambda image: 1,
            get=lambda key: "paired_owner",
        )
        engine = SimpleNamespace(orig_bin=image, get_all=lambda: [owner])
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "tools/data").mkdir(parents=True)
            (root / "tools/data/original-symbols.csv").write_text(
                "# original extents\naddress,type,size,symbol\n0x00410010,,6,\n"
            )
            with patch("inventory_thunks.ROOT", root):
                found = references(engine, {target})[target]
        self.assertEqual([item["address"] for item in found], [code_address, 0x490000])
        self.assertIsNone(found[0]["owner_name"])
        self.assertEqual(found[1]["kind"], "pointer")

    def test_jump_names_require_exact_direct_entries_with_paired_bodies(self):
        parser = SimpleNamespace(
            name_lookup=lambda address, exact=False, indirect=False: (
                address,
                exact,
                indirect,
            )
        )
        engine = SimpleNamespace(
            function_comparator=SimpleNamespace(orig_sanitize=parser),
            orig_bin=SimpleNamespace(
                read=lambda address, size: bytes.fromhex("e9fb1f0000")
            ),
        )
        name_jump_destinations(engine, {0x403000: object()}, {0x401000, 0x401005})
        self.assertEqual(parser.name_lookup(0x401000), (0x403000, True, False))
        for address in (0x401001, 0x401005, 0x404000):
            self.assertEqual(parser.name_lookup(address), (address, False, False))
        self.assertEqual(
            parser.name_lookup(0x401000, indirect=True), (0x401000, False, True)
        )

    def test_table_definition_requires_external_function_and_exact_public(self):
        class Callee:
            recomp_addr = 0x503000

            def get(self, key):
                return self.symbol if key == "symbol" else None

        callee = Callee()
        callee.symbol = "_body"
        modules = SimpleNamespace(
            get_module=lambda address: ("native", "CMakeFiles/LEMBALL.dir/native.obj")
        )
        publics = {"_body": callee.recomp_addr}
        native = object_bytes()
        with patch("recover_thunks.load_object", return_value=CoffObject(native)):
            self.assertEqual(
                native_definition(callee, modules, publics),
                {"object": "CMakeFiles/LEMBALL.dir/native.obj", "symbol": "_body"},
            )
            for symbol in ("_data", "_other", "_missing"):
                callee.symbol = symbol
                with self.subTest(symbol=symbol), self.assertRaises(ValueError):
                    native_definition(callee, modules, publics)
            callee.symbol = "_body"
            with self.assertRaises(ValueError):
                native_definition(callee, modules, {"_body": callee.recomp_addr + 1})
        local = bytearray(native)
        local[140 + 2 * 18 + 16] = 3
        with patch("recover_thunks.load_object", return_value=CoffObject(local)):
            with self.assertRaises(ValueError):
                native_definition(callee, modules, publics)
        for module in (None, ("runtime", "msvcrt.lib")):
            modules.get_module = lambda address, module=module: module
            with self.subTest(module=module), self.assertRaises(ValueError):
                native_definition(callee, modules, publics)

    def test_pointer_verification_requires_original_relocation_and_routed_target(self):
        reference = routes(CoffObject(object_bytes()))[1]["references"][0]
        original = SimpleNamespace(
            relocations={0x490004},
            read=lambda address, size: bytes.fromhex(reference["original_bytes"]),
        )
        rebuilt = SimpleNamespace(
            read=lambda address, size: struct.pack("<I", 0x500000)
        )
        engine = SimpleNamespace(orig_bin=original, recomp_bin=rebuilt)
        owner = SimpleNamespace(orig_addr=0x490000, recomp_addr=0x510000)
        functions = {0x401000: SimpleNamespace(recomp_addr=0x500000)}
        self.assertTrue(reference_matches(engine, functions, owner, reference))
        original.relocations.clear()
        self.assertFalse(reference_matches(engine, functions, owner, reference))
        original.relocations.add(0x490004)
        functions[0x401000].recomp_addr += 5
        self.assertFalse(reference_matches(engine, functions, owner, reference))
        functions[0x401000].recomp_addr -= 5
        owner.orig_addr += 4
        self.assertFalse(reference_matches(engine, functions, owner, reference))

    def test_code_and_data_change_only_relocation_indices(self):
        native = object_bytes()
        obj = CoffObject(native)
        overlay = obj.redirect(routes(obj), {0x401000: thunk()})
        rebuilt = CoffObject(overlay)
        self.assertEqual(obj.data, native)
        self.assertEqual(native[20:124], overlay[20:124])
        self.assertEqual(native[128:134], overlay[128:134])
        self.assertEqual(
            native[138 : obj.string_start], overlay[138 : obj.string_start]
        )
        self.assertEqual(
            rebuilt.reference_target("_call", 1, REL32), thunk_symbol(0x401000)
        )
        self.assertEqual(
            rebuilt.reference_target("_data", 4, DIR32), thunk_symbol(0x401000)
        )
        self.assertTrue(overlay.endswith(b"DEBUG"))

    def test_rejects_wrong_type_addend_target_and_extent(self):
        for options in ({"data_kind": REL32}, {"addend": 1}, {"data_target": 4}):
            with self.subTest(options=options):
                obj = CoffObject(object_bytes(**options))
                with self.assertRaises(ValueError):
                    obj.redirect(routes(obj), {0x401000: thunk()})
        obj = CoffObject(object_bytes())
        for field, value in (
            ("offset", 9),
            ("original_bytes", "05104000"),
            ("source_symbol", "_other"),
        ):
            with self.subTest(field=field):
                altered = routes(obj)
                altered[1]["references"][0][field] = value
                with self.assertRaises(ValueError):
                    obj.redirect(altered, {0x401000: thunk()})

    def test_rejects_stale_data_and_code(self):
        native = object_bytes()
        saved = routes(CoffObject(native))
        for offset in (100, 108):
            with self.subTest(offset=offset):
                altered = bytearray(native)
                altered[offset] ^= 1
                with self.assertRaisesRegex(ValueError, "Stale"):
                    CoffObject(bytes(altered)).redirect(saved, {0x401000: thunk()})

    def test_folded_guards_include_relocation_symbols(self):
        first = CoffObject(object_bytes())
        second = CoffObject(object_bytes(data_target=4))
        self.assertEqual(
            first.fingerprint("_data", 12), second.fingerprint("_data", 12)
        )
        self.assertNotEqual(
            first.fingerprint("_data", 12, include_symbols=True),
            second.fingerprint("_data", 12, include_symbols=True),
        )

    def test_link_rejects_changed_folded_body_and_preserves_native_object(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            build = root / "build"
            build.mkdir()
            original = root / "data/LEMBALL.EXE"
            original.parent.mkdir()
            original.write_bytes(b"original fixture")
            native = object_bytes(data_target=3)
            path = build / "native.obj"
            path.write_bytes(native)
            obj = CoffObject(native)
            saved = routes(obj)
            saved[1]["references"][0]["source_symbol"] = "_alias"
            entry = thunk()
            with self.assertRaises(ValueError):
                obj.redirect(saved, {entry["address"]: entry})
            entry["folded_aliases"] = [
                {
                    "object": "native.obj",
                    "symbol": symbol,
                    "size": 1,
                    "fingerprint": obj.fingerprint(symbol, 1, include_symbols=True),
                }
                for symbol in ("_body", "_alias")
            ]
            manifest = root / "tools/data/linker-thunks.json"
            manifest.parent.mkdir(parents=True)
            manifest.write_text(
                json.dumps(
                    {
                        "version": 3,
                        "original_sha256": hashlib.sha256(
                            original.read_bytes()
                        ).hexdigest(),
                        "thunks": [entry],
                        "routes": saved,
                    }
                )
            )
            prepare_link(["native.obj"], build, manifest)
            self.assertEqual(path.read_bytes(), native)
            applied = json.loads((build / "linker-thunks/applied.json").read_text())
            self.assertEqual(applied, {"references": 2, "skipped": []})
            altered = bytearray(native)
            altered[107] = 0x90
            path.write_bytes(altered)
            with self.assertRaisesRegex(ValueError, "Stale folded-alias"):
                prepare_link(["native.obj"], build, manifest)
            broken_hash = json.loads(manifest.read_text())
            broken_hash["original_sha256"] = "0" * 64
            manifest.write_text(json.dumps(broken_hash))
            path.write_bytes(native)
            with self.assertRaisesRegex(ValueError, "different original"):
                prepare_link(["native.obj"], build, manifest)


if __name__ == "__main__":
    unittest.main()
