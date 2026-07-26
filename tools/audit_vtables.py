#!/usr/bin/env python3
"""Validate vtables while proving original linker-thunk equivalences."""

from __future__ import annotations

import argparse
import json
import logging
import struct
import sys
from pathlib import Path
from typing import Any

from reccmp.compare import Compare
from reccmp.project.detect import (
    RecCmpProjectException,
    argparse_add_project_target_args,
    argparse_parse_project_target,
)
from reccmp.project.logging import (
    argparse_add_logging_args,
    argparse_parse_logging,
)


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_EQUIVALENCES = ROOT / "tools" / "vtable-thunk-equivalences.json"
DEFAULT_MANIFEST = ROOT / "data" / "manifest.json"


def parse_address(value: str) -> int:
    return int(value, 16)


def decode_near_jump(address: int, code: bytes) -> int | None:
    if len(code) != 5 or code[0] != 0xE9:
        return None
    displacement = struct.unpack("<i", code[1:])[0]
    return (address + 5 + displacement) & 0xFFFFFFFF


def validate_proof(
    entry: dict[str, Any],
    manifest_by_address: dict[int, dict[str, Any]],
    read_original: Any,
) -> list[str]:
    errors: list[str] = []
    thunk = parse_address(entry["original_thunk"])
    target = parse_address(entry["target"])
    size = entry["size"]
    expected = bytes.fromhex(entry["instruction_bytes"])
    actual = read_original(thunk, size)
    function = manifest_by_address.get(thunk)

    if size != 5 or len(expected) != size:
        errors.append(f"0x{thunk:08x}: proof must describe one 5-byte near jump")
    if actual != expected:
        errors.append(f"0x{thunk:08x}: original instruction bytes changed")
    if decode_near_jump(thunk, actual) != target:
        errors.append(f"0x{thunk:08x}: near-jump target is not 0x{target:08x}")
    if function is None:
        errors.append(f"0x{thunk:08x}: missing manifest entity")
    else:
        manifest_target = function.get("thunk_target")
        if (
            function.get("category") != "thunk"
            or function.get("is_thunk") is not True
            or function.get("size") != size
            or not isinstance(manifest_target, str)
            or parse_address(manifest_target) != target
        ):
            errors.append(f"0x{thunk:08x}: manifest thunk proof disagrees")
    return errors


def proof_key(entry: dict[str, Any]) -> tuple[int, int]:
    return parse_address(entry["original_vtable"]), entry["slot_offset"]


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Compare vtables with closed, byte-proven linker-thunk equivalences."
    )
    argparse_add_project_target_args(parser)
    argparse_add_logging_args(parser)
    parser.add_argument("--equivalences", type=Path, default=DEFAULT_EQUIVALENCES)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--verbose", "-v", action="store_true")
    args = parser.parse_args()
    argparse_parse_logging(args)
    logging.getLogger("compare").addHandler(logging.NullHandler())

    try:
        target = argparse_parse_project_target(args)
    except RecCmpProjectException as error:
        print(error.args[0], file=sys.stderr)
        return 1

    specification = json.loads(args.equivalences.read_text(encoding="utf-8"))
    manifest = json.loads(args.manifest.read_text(encoding="utf-8"))
    manifest_by_address = {
        parse_address(function["address"]): function
        for function in manifest["functions"]
    }
    entries = specification["entries"]
    proofs: dict[tuple[int, int], dict[str, Any]] = {}
    errors: list[str] = []

    if specification.get("schema_version") != 1:
        errors.append("unsupported equivalence ledger schema")
    for entry in entries:
        key = proof_key(entry)
        if key in proofs:
            errors.append(
                f"duplicate proof for vtable 0x{key[0]:08x} slot +0x{key[1]:x}"
            )
        proofs[key] = entry

    engine = Compare.from_target(target)
    for entry in entries:
        errors.extend(
            validate_proof(entry, manifest_by_address, engine.orig_bin.read)
        )

    entities = list(engine.get_all())
    original_entities = {
        entity.orig_addr: entity
        for entity in entities
        if entity.orig_addr is not None
    }
    rebuilt_entities = {
        entity.recomp_addr: entity
        for entity in entities
        if entity.recomp_addr is not None
    }
    used: set[tuple[int, int]] = set()
    table_count = 0
    failed_tables: set[int] = set()

    for table in engine.get_vtables():
        table_count += 1
        size = table.any_size()
        if size % 4:
            errors.append(f"0x{table.orig_addr:08x}: irregular vtable size {size}")
            size -= size % 4
        original = engine.orig_bin.read(table.orig_addr, size)
        rebuilt = engine.recomp_bin.read(table.recomp_addr, size)
        for offset, (raw_original, raw_rebuilt) in enumerate(
            zip(
                struct.iter_unpack("<I", original),
                struct.iter_unpack("<I", rebuilt),
            )
        ):
            original_address = raw_original[0]
            rebuilt_address = raw_rebuilt[0]
            original_entity = original_entities.get(original_address)
            rebuilt_entity = rebuilt_entities.get(rebuilt_address)
            direct_match = (
                original_entity is not None
                and rebuilt_entity is not None
                and original_entity.recomp_addr == rebuilt_entity.recomp_addr
            )
            if direct_match:
                continue

            slot_offset = offset * 4
            key = (table.orig_addr, slot_offset)
            proof = proofs.get(key)
            equivalent = False
            if proof is not None:
                target = parse_address(proof["target"])
                target_entity = original_entities.get(target)
                equivalent = (
                    original_address == parse_address(proof["original_thunk"])
                    and target_entity is not None
                    and rebuilt_entity is not None
                    and target_entity.recomp_addr == rebuilt_entity.recomp_addr
                )
            if equivalent:
                used.add(key)
                if args.verbose:
                    print(
                        f"proved linker thunk 0x{original_address:08x} at "
                        f"vtable 0x{table.orig_addr:08x}+0x{slot_offset:x}"
                    )
                continue

            failed_tables.add(table.orig_addr)
            errors.append(
                f"0x{table.orig_addr:08x}+0x{slot_offset:x}: "
                f"0x{original_address:08x} != 0x{rebuilt_address:08x}"
            )

    unused = set(proofs) - used
    for vtable, offset in sorted(unused):
        errors.append(f"unused proof for vtable 0x{vtable:08x} slot +0x{offset:x}")

    for function in engine.get_functions():
        if function.name is None or "`vtordisp" not in function.name:
            continue
        comparison = engine.compare_address(function.orig_addr)
        if comparison is None or comparison.ratio < 1.0:
            errors.append(
                f"0x{function.orig_addr:08x}: inexact adjustor thunk {function.name}"
            )

    if errors:
        for error in errors:
            print(error, file=sys.stderr)
        print(
            f"Vtables found: {table_count}. "
            f"Vtables not matching: {len(failed_tables)}. "
            f"Audit issues: {len(errors)}.",
            file=sys.stderr,
        )
        return 1

    print(
        f"Vtables found: {table_count}. 100% match. "
        f"Proven linker thunk equivalences: {len(used)}/{len(proofs)}."
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
