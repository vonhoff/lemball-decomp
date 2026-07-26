from __future__ import annotations

import importlib.util
import unittest
from pathlib import Path


SCRIPT = Path(__file__).resolve().parents[1] / "audit_vtables.py"
SPEC = importlib.util.spec_from_file_location("audit_vtables", SCRIPT)
assert SPEC is not None and SPEC.loader is not None
audit = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(audit)


class VtableAuditTests(unittest.TestCase):
    def test_decode_forward_near_jump(self) -> None:
        self.assertEqual(
            audit.decode_near_jump(0x401564, bytes.fromhex("e967960000")),
            0x40ABD0,
        )

    def test_decode_backward_near_jump(self) -> None:
        self.assertEqual(
            audit.decode_near_jump(0x500000, bytes.fromhex("e9fbffffff")),
            0x500000,
        )

    def test_reject_non_jump(self) -> None:
        self.assertIsNone(
            audit.decode_near_jump(0x401564, bytes.fromhex("b801000000"))
        )

    def test_manifest_proof_accepts_exact_thunk(self) -> None:
        entry = {
            "original_thunk": "00401564",
            "size": 5,
            "instruction_bytes": "e967960000",
            "target": "0040ABD0",
        }
        manifest = {
            0x401564: {
                "category": "thunk",
                "is_thunk": True,
                "size": 5,
                "thunk_target": "0040ABD0",
            }
        }
        self.assertEqual(
            audit.validate_proof(
                entry,
                manifest,
                lambda _address, _size: bytes.fromhex("e967960000"),
            ),
            [],
        )

    def test_manifest_proof_rejects_wrong_target(self) -> None:
        entry = {
            "original_thunk": "00401564",
            "size": 5,
            "instruction_bytes": "e967960000",
            "target": "0040ABE0",
        }
        manifest = {
            0x401564: {
                "category": "thunk",
                "is_thunk": True,
                "size": 5,
                "thunk_target": "0040ABE0",
            }
        }
        errors = audit.validate_proof(
            entry,
            manifest,
            lambda _address, _size: bytes.fromhex("e967960000"),
        )
        self.assertTrue(any("near-jump target" in error for error in errors))


if __name__ == "__main__":
    unittest.main()
