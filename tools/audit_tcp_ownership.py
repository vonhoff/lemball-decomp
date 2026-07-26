#!/usr/bin/env python3
"""Materialize and validate TCP ownership reconstruction address ledger."""

from __future__ import annotations

import argparse
import csv
import json
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_SPEC = ROOT / "tools" / "tcp-ownership-ledger.json"
DEFAULT_MANIFEST = ROOT / "data" / "manifest.json"
DEFAULT_INVENTORY = ROOT / "build" / "function-inventory.csv"
DEFAULT_OUTPUT = ROOT / "build" / "tcp-ownership-ledger.csv"
FIELDS = [
    "original_address",
    "size",
    "role",
    "owner",
    "body_or_thunk_target",
    "original_vtable_xrefs",
    "rebuilt_symbol",
    "rebuilt_address",
    "annotation_location",
    "byte_match",
    "disposition",
]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("--spec", type=Path, default=DEFAULT_SPEC)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--inventory", type=Path, default=DEFAULT_INVENTORY)
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    parser.add_argument(
        "--allow-incomplete",
        action="store_true",
        help="write diagnostic ledger without enforcing final exactness",
    )
    return parser.parse_args()


def load_inventory(path: Path) -> dict[str, dict[str, str]]:
    with path.open(newline="", encoding="utf-8-sig") as stream:
        return {row["address"][2:].upper(): row for row in csv.DictReader(stream)}


def role_owner(group: str) -> tuple[str, str]:
    if group == "composite_construction_bodies":
        return "body", "VsNetCompositeEffTransportStack"
    if group == "tcp_construction_and_owner_bodies":
        return "body", "TCP construction/complete owners"
    if group == "late_virtual_and_destruction_bodies":
        return "body", "TCP complete owners/shared virtual interfaces"
    if group == "composite_generated_thunks":
        return "compiler-generated thunk", "VsNetCompositeEffTransportStack"
    return "compiler-generated thunk", "TCP concrete complete owners"


def is_network_row(row: dict[str, str]) -> bool:
    marker = row.get("source_marker", "").replace("\\", "/").casefold()
    name = row.get("name", "").casefold()
    return (
        "/engine/net/" in marker
        or "vsnet" in name
        or "tcpip" in name
        or "efftransport" in name
        or "socket" in name
    )


def disposition_is_acceptable(role: str, disposition: str) -> bool:
    if role == "body":
        return disposition in {"exact", "inexact"}
    return disposition == "exact"


def main() -> int:
    args = parse_args()
    spec = json.loads(args.spec.read_text(encoding="utf-8"))
    manifest = json.loads(args.manifest.read_text(encoding="utf-8"))
    manifest_by_address = {
        function["address"].upper(): function for function in manifest["functions"]
    }
    inventory = load_inventory(args.inventory)
    proofs = spec.get("boundary_proofs", {})
    errors: list[str] = []
    rows: list[dict[str, object]] = []
    seen: set[str] = set()

    for group, addresses in spec["entities"].items():
        role, owner = role_owner(group)
        for address in addresses:
            address = address.upper()
            if address in seen:
                errors.append(f"duplicate ledger address {address}")
                continue
            seen.add(address)
            function = manifest_by_address.get(address)
            if function is None:
                errors.append(f"{address}: missing manifest function")
                continue
            mapped = inventory.get(address, {})
            accuracy = mapped.get("raw_accuracy", "")
            module = mapped.get("module", "Unassigned")
            rebuilt_address = mapped.get("rebuilt_address", "")
            if module == "Unassigned":
                disposition = "unassigned"
            elif not rebuilt_address:
                disposition = "unimplemented"
            elif accuracy == "1":
                disposition = "exact"
            else:
                disposition = "inexact"
            proof = proofs.get(address, {})
            target = proof.get("target") or function.get("thunk_target") or ""
            rows.append(
                {
                    "original_address": f"0x{address}",
                    "size": function["size"],
                    "role": role,
                    "owner": owner,
                    "body_or_thunk_target": (
                        "" if role == "body" else f"0x{target}" if target else ""
                    ),
                    "original_vtable_xrefs": ";".join(
                        f"0x{x}" for x in proof.get("original_vtable_xrefs", [])
                    ),
                    "rebuilt_symbol": mapped.get("name", ""),
                    "rebuilt_address": rebuilt_address,
                    "annotation_location": mapped.get("source_marker", ""),
                    "byte_match": accuracy == "1",
                    "disposition": disposition,
                }
            )
            if (
                not args.allow_incomplete
                and not disposition_is_acceptable(role, disposition)
            ):
                errors.append(f"{address}: {disposition}")

    for address, proof in proofs.items():
        function = manifest_by_address.get(address)
        if function is None:
            continue
        if function["size"] != proof["manifest_size"]:
            errors.append(f"{address}: manifest size disagrees with boundary proof")
        if not proof["original_vtable_xrefs"]:
            errors.append(f"{address}: missing original vtable xref proof")
        if proof["source_authored"]:
            errors.append(f"{address}: proven compiler thunk marked source-authored")

    for address, row in inventory.items():
        if is_network_row(row) and row.get("module") == "Unassigned":
            errors.append(f"{address}: network inventory entity remains Unassigned")
        marker = row.get("source_marker", "").replace("\\", "/").casefold()
        if "/engine/net/" in marker and row.get("module") == "Unassigned":
            errors.append(f"{address}: ENGINE/NET marker remains Unassigned")

    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=FIELDS)
        writer.writeheader()
        writer.writerows(rows)

    if errors:
        for error in errors:
            print(error, file=sys.stderr)
        print(f"TCP ownership audit failed: {len(errors)} issue(s)", file=sys.stderr)
        return 1
    if args.allow_incomplete:
        print(f"TCP ownership diagnostic ledger written: {len(rows)} entities")
        return 0
    exact_thunks = sum(
        row["role"] == "compiler-generated thunk"
        and row["disposition"] == "exact"
        for row in rows
    )
    mapped_bodies = sum(
        row["role"] == "body"
        and row["disposition"] in {"exact", "inexact"}
        for row in rows
    )
    print(
        "TCP ownership audit clean: "
        f"{exact_thunks} exact thunks; {mapped_bodies} mapped bodies"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
