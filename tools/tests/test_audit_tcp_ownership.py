from __future__ import annotations

import importlib.util
import unittest
from pathlib import Path


SCRIPT = Path(__file__).resolve().parents[1] / "audit_tcp_ownership.py"
SPEC = importlib.util.spec_from_file_location("audit_tcp_ownership", SCRIPT)
assert SPEC is not None and SPEC.loader is not None
audit = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(audit)


class TcpOwnershipAuditTests(unittest.TestCase):
    def test_network_source_marker_is_network_owned(self) -> None:
        self.assertTrue(
            audit.is_network_row(
                {
                    "name": "UnhelpfulName",
                    "source_marker": "FUNCTION:C:\\repo\\src\\ENGINE\\NET\\A.CPP:1",
                }
            )
        )

    def test_non_network_row_is_not_network_owned(self) -> None:
        self.assertFalse(
            audit.is_network_row(
                {
                    "name": "GameMode::Tick",
                    "source_marker": "FUNCTION:C:\\repo\\src\\GAME.CPP:1",
                }
            )
        )

    def test_mapped_body_need_not_be_byte_exact(self) -> None:
        self.assertTrue(audit.disposition_is_acceptable("body", "inexact"))
        self.assertFalse(audit.disposition_is_acceptable("body", "unimplemented"))

    def test_every_generated_thunk_must_be_byte_exact(self) -> None:
        self.assertTrue(
            audit.disposition_is_acceptable("compiler-generated thunk", "exact")
        )
        self.assertFalse(
            audit.disposition_is_acceptable("compiler-generated thunk", "inexact")
        )

    def test_role_owner_split(self) -> None:
        self.assertEqual(
            audit.role_owner("composite_construction_bodies"),
            ("body", "VsNetCompositeEffTransportStack"),
        )
        self.assertEqual(
            audit.role_owner("tcp_generated_and_adjusted_entries"),
            ("compiler-generated thunk", "TCP concrete complete owners"),
        )


if __name__ == "__main__":
    unittest.main()
