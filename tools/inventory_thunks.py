#!/usr/bin/env python3
"""Audit uncovered original linker slots against native definitions and saved routes."""

import argparse
import csv
import hashlib
import json
from collections import defaultdict
from dataclasses import replace
from pathlib import Path

from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_IMM
from reccmp.compare import Compare
from reccmp.dir import source_code_search
from reccmp.project.detect import RecCmpProject
from reccmp.types import EntityType, ImageId

from lib import BUILD, ROOT, TARGET_ID
from link_binary import read_jump_target
from recover_thunks import (
    ANNOTATIONS,
    MANIFEST,
    OUTPUT,
    jump_entries,
    load_object,
    native_context,
    native_definition,
    thunk_matches,
)


BODY_NOTES = {
    0x42F590: {
        "evidence": "ECX object: count +0x34, array +0x38, stride 0x1a0; forwards two stack arguments to 0x402c75, tests full EAX, returns 0/1, RET 8. Matches CMoverManager/CMover layout; method name and argument types unresolved.",
        "source": [
            "src/Gameplay/Mechanisms/CMoverManager.h",
            "src/Gameplay/Mechanisms/CMover.h",
        ],
        "next_step": "Find Windows call or pointer evidence for 0x401631/0x42f590 and the predicate 0x402c75/0x42eec0; establish both argument types before adding declarations.",
        "prior_trials": "Reconstruction memory records earlier traversal/predicate trials with collateral Effective losses (attempts 1967 and 10890). Current source lacks these methods. Revisit with new evidence and a full collateral audit.",
    },
    0x42EEC0: {
        "evidence": "XOR EAX,EAX; RET 8. Original caller 0x42f5ae computes ECX from the manager's mover array and forwards two DWORD stack values through 0x402c75. Constant-zero behavior established; method name, argument meanings and source types unresolved.",
        "source": [
            "src/Gameplay/Mechanisms/CMover.h",
            "src/Gameplay/Mechanisms/CMoverManager.h",
        ],
        "next_step": "Resolve identity and both forwarded argument types with the traversal at 0x42f590. Earlier helper-only and pair trials lost Effective bytes (attempts 10891 and 10890); require new evidence and a full collateral audit.",
    },
    0x434F50: {
        "evidence": "Loads ECX from incoming ECX+0x78; null check; nonnull tail jump to 0x402d83/0x44b360. CLemmingAnimsManager has CCDLoadAnim* at +0x78; class attribution remains inferred.",
        "source": [
            "src/GameView/Animation/CLemmingAnimsManager.h",
            "src/GameView/Loading/CCDLoadAnim.h",
        ],
        "next_step": "Identify a Windows caller or pointer owner for 0x40324c/0x434f50; identify the no-op callee before assigning class and method names.",
    },
    0x44B360: {
        "evidence": "Single RET. Original wrapper 0x434f50 loads ECX from object+0x78 and tail-jumps through 0x402d83 when nonnull. CCDLoadAnim ownership is inferred from the wrapper's matching layout; the leaf provides no method identity or complete ABI.",
        "source": [
            "src/GameView/Animation/CLemmingAnimsManager.h",
            "src/GameView/Loading/CCDLoadAnim.h",
        ],
        "next_step": "Identify the wrapper's Windows caller or pointer owner and the method intended on member+0x78; preserve existing CCDLoadAnim mappings.",
    },
    0x4395F0: {
        "evidence": "31-byte scalar deleting wrapper: writes CPrimitive vtable 0x496ca8; flags bit 0 conditionally calls delete at 0x45a790; returns this; RET 4. Existing CPrimitive wrapper is mapped at 0x432350. Separate original retained copy; same native symbol cannot be assigned to both bodies.",
        "source": ["src/Engine/Graphics/Primitives/CPrimitive.h"],
        "next_step": "Establish how MSVC retained a second CPrimitive wrapper and reproduce a distinct native COFF definition. Preserve the existing 0x432350 mapping.",
    },
}


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def decoder():
    disassembler = Cs(CS_ARCH_X86, CS_MODE_32)
    disassembler.detail = True
    disassembler.skipdata = True
    return disassembler


def references(engine, addresses):
    """Scan catalogued function extents and every PE base-relocated pointer."""
    found = defaultdict(list)
    disassembler = decoder()
    seen = set()
    entities = {
        entity.orig_addr: entity for entity in engine.get_all() if entity.orig_addr
    }
    extents = {
        entity.orig_addr: entity.size(ImageId.ORIG)
        for entity in entities.values()
        if entity.entity_type == EntityType.FUNCTION and entity.size(ImageId.ORIG)
    }
    regions = [
        range(region.addr, region.addr + len(region.data))
        for region in engine.orig_bin.get_code_regions()
    ]
    with (ROOT / "tools/data/original-symbols.csv").open(encoding="utf-8") as catalog:
        for row in csv.DictReader(line for line in catalog if not line.startswith("#")):
            address = int(row["address"], 16)
            if row["size"] and any(address in region for region in regions):
                extents[address] = int(row["size"], 0)
    for address, size in sorted(extents.items()):
        entity = entities.get(address)
        raw = engine.orig_bin.read(address, size)
        for instruction in disassembler.disasm(raw, address):
            if not instruction.id:
                continue
            for operand in instruction.operands:
                target = (
                    operand.imm & 0xFFFFFFFF if operand.type == X86_OP_IMM else None
                )
                if target not in addresses or (instruction.address, target) in seen:
                    continue
                seen.add((instruction.address, target))
                found[target].append(
                    {
                        "address": instruction.address,
                        "owner": address,
                        "owner_name": entity.get("name") if entity else None,
                        "kind": "code",
                        "bytes": instruction.bytes.hex(),
                        "instruction": f"{instruction.mnemonic} {instruction.op_str}".strip(),
                    }
                )
    for address in sorted(engine.orig_bin.relocations):
        raw = engine.orig_bin.read(address, 4)
        target = int.from_bytes(raw, "little")
        if target in addresses:
            found[target].append(
                {"address": address, "kind": "pointer", "bytes": raw.hex()}
            )
    return found


def native_candidates(engine, functions, modules, publics, reference, body):
    """Describe unrouted native references; never infer a route from candidate order."""
    owner = functions.get(reference.get("owner"))
    if owner is None:
        return {"reason": "Original owner has no paired native body", "candidates": []}
    try:
        definition = native_definition(owner, modules, publics)
    except (ValueError, OSError) as error:
        return {"reason": str(error), "candidates": []}
    candidates = []
    obj = load_object(definition["object"])
    raw = engine.recomp_bin.read(owner.recomp_addr, owner.size(ImageId.RECOMP))
    for instruction in decoder().disasm(raw, owner.recomp_addr):
        if not instruction.id or not any(
            operand.type == X86_OP_IMM and operand.imm & 0xFFFFFFFF == body.recomp_addr
            for operand in instruction.operands
        ):
            continue
        offset = instruction.address - owner.recomp_addr
        candidates.append({"offset": offset, "bytes": instruction.bytes.hex()})
    return {
        "definition": definition,
        "fingerprint": obj.fingerprint(
            definition["symbol"], owner.size(ImageId.RECOMP)
        ),
        "reason": "No native reference to this paired destination"
        if not candidates
        else "Native references exist; no verified instruction alignment and relocation route",
        "candidates": candidates,
        "next_step": "Inspect source and original argument forwarding for this owner; establish a native counterpart before routing."
        if not candidates
        else "Compare original/native control flow and argument setup around these candidates; require exact COFF relocation and instruction correspondence before routing.",
    }


def inventory(baseline, ghidra_path):
    target = RecCmpProject.from_directory(BUILD).get(TARGET_ID)
    routed = Compare.from_target(target)
    paths = tuple(
        path
        for path in source_code_search(target.source_paths)
        if path.resolve() != ANNOTATIONS.resolve()
    )
    native_target = replace(
        target,
        source_paths=paths,
        recompiled_path=OUTPUT / "unrouted.EXE",
        recompiled_pdb=OUTPUT / "unrouted.pdb",
    )
    native = Compare.from_target(native_target)
    functions = {function.orig_addr: function for function in native.get_functions()}
    routed_functions = {
        function.orig_addr: function for function in routed.get_functions()
    }
    entities = {
        entity.orig_addr: entity for entity in native.get_all() if entity.orig_addr
    }
    modules, publics = native_context(native, native_target)
    current = json.loads(MANIFEST.read_text())
    before = json.loads(baseline.read_text())
    if (
        before["original_sha256"] != digest(target.original_path)
        or current["original_sha256"] != before["original_sha256"]
    ):
        raise ValueError("Manifest original hash differs from the audited image")
    table = jump_entries(native, functions, set())
    scope = table - {thunk["address"] for thunk in before["thunks"]}
    thunks = {thunk["address"]: thunk for thunk in current["thunks"]}
    unpaired_bodies = {
        read_jump_target(native.orig_bin, entry) for entry in scope
    } - functions.keys()
    refs = references(native, scope | unpaired_bodies)
    ghidra = json.loads(ghidra_path.read_text()) if ghidra_path else None
    if ghidra and (
        ghidra["program"] != "LEMBALL.EXE"
        or not all(
            f"{address:08x}" in ghidra["xrefs"] for address in scope | unpaired_bodies
        )
    ):
        raise ValueError("Ghidra snapshot does not cover the audit scope")
    routes = {
        (
            reference.get("original_instruction", reference.get("original_pointer")),
            reference["thunk"],
        ): {
            "owner": route["caller"],
            "object": route["object"],
            "symbol": route["symbol"],
            "offset": reference["offset"],
            "source_symbol": reference["source_symbol"],
        }
        for route in current["routes"]
        for reference in route["references"]
    }
    rows = []
    errors = []
    for address in sorted(scope):
        destination = read_jump_target(native.orig_bin, address)
        body = functions.get(destination)
        row = {
            "address": address,
            "target": destination,
            "original_bytes": native.orig_bin.read(address, 5).hex(),
            "status": "recovered" if address in thunks else "unpaired_body",
            "references": refs[address],
        }
        if ghidra:
            row["ghidra_xrefs"] = ghidra["xrefs"][f"{address:08x}"]
        if body:
            definition = native_definition(body, modules, publics)
            size = body.size(ImageId.RECOMP)
            row.update(
                name=body.get("name"),
                native_definition=definition
                | {
                    "size": size,
                    "fingerprint": load_object(definition["object"]).fingerprint(
                        definition["symbol"], size
                    ),
                },
                verified=address in thunks
                and thunk_matches(routed, routed_functions, thunks[address]),
            )
            if not row["verified"]:
                errors.append(address)
        else:
            entity = entities.get(destination)
            if entity is None or not entity.size(ImageId.ORIG):
                raise ValueError(f"Missing original extent for {destination:#x}")
            raw = native.orig_bin.read(destination, entity.size(ImageId.ORIG))
            instructions = [
                f"{i.address:08x}: {i.mnemonic} {i.op_str}".strip()
                for i in decoder().disasm(raw, destination)
            ]
            note = BODY_NOTES.get(
                destination,
                {
                    "evidence": "Exact body bytes and decoded instructions below. Short return body cannot establish class, method identity or complete prototype. RET 8 proves eight-byte callee cleanup, not member-function identity; RET alone does not establish calling convention.",
                    "next_step": "Find a Windows caller, vtable/pointer owner, or independent symbol mapping; derive argument and return types before adding a source declaration.",
                },
            )
            row.update(
                body_size=len(raw),
                body_bytes=raw.hex(),
                instructions=instructions,
                body_references=refs[destination],
                blocker="No paired native definition with established identity and ABI",
                **note,
            )
            if ghidra:
                row["ghidra_body_xrefs"] = ghidra["xrefs"][f"{destination:08x}"]
        for reference in row["references"]:
            route = routes.get((reference["address"], address))
            reference["route"] = route
            if route is None and body is not None:
                reference["native_evidence"] = native_candidates(
                    native, functions, modules, publics, reference, body
                )
        rows.append(row)
    return {
        "version": 1,
        "original_sha256": digest(target.original_path),
        "native_sha256": digest(native_target.recompiled_path),
        "rebuilt_sha256": digest(target.recompiled_path),
        "baseline_sha256": digest(baseline),
        "manifest_sha256": digest(MANIFEST),
        "ghidra_snapshot_sha256": digest(ghidra_path) if ghidra_path else None,
        "reference_scope": "Immediate operands decoded within catalogued original function extents; all PE base-relocated DWORD pointers; optional full-scope Ghidra xrefs. Static analysis cannot prove an entry unused. Ghidra names/comments are hypotheses; original bytes and source layouts supply evidence.",
        "table_entries": len(table),
        "selected_entries": len(rows),
        "recovered": sum(row["status"] == "recovered" for row in rows),
        "unpaired": sum(row["status"] == "unpaired_body" for row in rows),
        "verification_errors": errors,
        "entries": rows,
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--baseline",
        type=Path,
        default=MANIFEST,
        help="Audit slots uncovered in this manifest; default: current manifest",
    )
    parser.add_argument(
        "--ghidra-xrefs",
        type=Path,
        help="Saved Ghidra bulk xrefs for every selected entry and unpaired body",
    )
    parser.add_argument("--output", type=Path, default=OUTPUT / "table-inventory.json")
    args = parser.parse_args()
    result = inventory(args.baseline, args.ghidra_xrefs)
    args.output.write_text(
        json.dumps(result, indent=2) + "\n", encoding="utf-8", newline="\n"
    )
    print(
        f"{result['selected_entries']} entries: {result['recovered']} recovered, {result['unpaired']} unpaired; {len(result['verification_errors'])} verification errors"
    )
    return bool(result["verification_errors"])


if __name__ == "__main__":
    raise SystemExit(main())
