#!/usr/bin/env python3
"""Thunk-aware LEMBALL vtable comparison (via tools/gate.py --vtable)."""

from __future__ import annotations

import re
import struct
import sys
from collections import Counter
from dataclasses import dataclass
from itertools import zip_longest

from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86_const import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
from reccmp.compare import Compare
from reccmp.parser.codebase import DecompCodebase
from reccmp.project.detect import RecCmpProjectException
from reccmp.types import EntityType, ImageId

from .paths import RECOMP_EXE
from .reccmp_compat import direct_jump_target, load_engine


# Original-only, unreferenced constructor fragments. The entries at 0x45a680,
# 0x4695d0 and 0x4697c0 have no incoming rel32 call/jump or relocated pointer
# in LEMBALL.EXE. The two button pairs copy CTextButton's virtual slots, apart
# from their deleting-destructor copies. Keep their stores visible in the gate;
# they have no identifiable source owner to annotate.
ORPHAN_VTABLE_STORES = {
    0x00498950: 0x0045a69f,
    0x00499b08: 0x004695f4,
    0x00499b28: 0x004695ee,
    0x00499bf8: 0x004697e4,
    0x00499c18: 0x004697de,
}


def resolve_jump(image, address: int | None, stop_at=None, max_depth: int = 16) -> int | None:
    """Follow E9 rel32 jumps until a non-jump or stop_at predicate."""
    if address is None:
        return None
    seen: set[int] = set()
    current = address
    for _ in range(max_depth):
        if current in seen:
            break
        seen.add(current)
        if stop_at is not None and stop_at(current):
            break
        destination = direct_jump_target(image, current)
        if destination is None or not image.is_valid_vaddr(destination):
            break
        current = destination
    return current

@dataclass(frozen=True)
class SlotResult:
    offset: int
    raw_orig: int | None
    raw_recomp: int | None
    orig: int | None
    recomp: int | None
    orig_entity: object | None
    recomp_entity: object | None
    folded_match: bool = False
    clone_match: bool = False
    adjuster_match: bool = False

    @property
    def matches(self) -> bool:
        if self.raw_orig == 0 and self.raw_recomp == 0:
            return True
        if self.folded_match or self.clone_match or self.adjuster_match:
            return True
        if self.orig_entity is None or self.recomp_entity is None:
            return False
        return self.orig_entity.recomp_addr == self.recomp_entity.recomp_addr

    @property
    def category(self) -> str:
        if self.raw_orig in (None, 0) or self.raw_recomp in (None, 0):
            return "layout-mismatch"
        if self.orig_entity is None:
            return "unannotated-original"
        if self.recomp_entity is None:
            return "unknown-recompiled"
        return "known-mismatch"


def is_original_clone(image, address: int | None, entity: object | None) -> bool:
    if address is None or entity is None or entity.orig_addr is None:
        return False
    size = entity.size(ImageId.RECOMP)
    if size is None or size <= 0 or size > 4096:
        return False
    if not image.is_valid_vaddr(address + size - 1) or not image.is_valid_vaddr(entity.orig_addr + size - 1):
        return False
    try:
        return image.read(address, size) == image.read(entity.orig_addr, size)
    except (IndexError, ValueError):
        return False


def read_table(engine: Compare, match) -> tuple[list[int], list[int]]:
    recomp_size = match.any_size(ImageId.RECOMP)
    recomp_size -= recomp_size % 4
    orig_size = match.size(ImageId.ORIG)
    if orig_size is None:
        orig_size = recomp_size
    else:
        orig_size -= orig_size % 4

    orig_table = engine.orig_bin.read(match.orig_addr, orig_size)
    recomp_table = engine.recomp_bin.read(match.recomp_addr, recomp_size)
    orig_addrs = [item[0] for item in struct.iter_unpack("<L", orig_table)]
    recomp_addrs = [item[0] for item in struct.iter_unpack("<L", recomp_table)]

    # reccmp derives the rebuilt size from symbol gaps/section contributions.
    # Extra nulls are alignment padding, not virtual slots.
    while len(recomp_addrs) > len(orig_addrs) and recomp_addrs[-1] == 0:
        recomp_addrs.pop()
    while len(orig_addrs) > len(recomp_addrs) and orig_addrs[-1] == 0:
        orig_addrs.pop()
    return orig_addrs, recomp_addrs


def collect_folded_aliases(engine: Compare, codebase: DecompCodebase) -> dict[int, set[int]]:
    aliases: dict[int, set[int]] = {}
    for function in codebase.iter_line_functions():
        if not function.is_folded:
            continue
        recomp_addr = engine._lines_db.find_function(
            function.filename,
            function.line_number,
            function.end_line,
            folded=True,
        )
        if recomp_addr is not None:
            aliases.setdefault(function.offset, set()).add(resolve_jump(engine.recomp_bin, recomp_addr))
    return aliases


def compare_table(engine: Compare, match, folded_aliases: dict[int, set[int]]) -> list[SlotResult]:
    orig_addrs, recomp_addrs = read_table(engine, match)
    slots: list[SlotResult] = []

    def is_function_body(image_id: ImageId, address: int) -> bool:
        entity = engine._db.get(image_id, address)
        return entity is not None and not entity_name(entity).startswith("Thunk of '")

    for index, (raw_orig, raw_recomp) in enumerate(zip_longest(orig_addrs, recomp_addrs)):
        orig = resolve_jump(engine.orig_bin, raw_orig, lambda address: is_function_body(ImageId.ORIG, address))
        recomp = resolve_jump(engine.recomp_bin, raw_recomp, lambda address: is_function_body(ImageId.RECOMP, address))

        orig_entity = None if orig is None else engine._db.get(ImageId.ORIG, orig)
        recomp_entity = None if recomp is None else engine._db.get(ImageId.RECOMP, recomp)
        direct_match = (
            orig_entity is not None
            and recomp_entity is not None
            and orig_entity.recomp_addr == recomp_entity.recomp_addr
        )
        folded_match = not direct_match and recomp is not None and recomp in folded_aliases.get(orig, set())
        clone_match = orig_entity is None and is_original_clone(engine.orig_bin, orig, recomp_entity)
        adjuster_match = not direct_match and is_same_generated_adjuster(engine, orig, recomp)
        slots.append(
            SlotResult(
                offset=index * 4,
                raw_orig=raw_orig,
                raw_recomp=raw_recomp,
                orig=orig,
                recomp=recomp,
                orig_entity=orig_entity,
                recomp_entity=recomp_entity,
                folded_match=folded_match,
                clone_match=clone_match,
                adjuster_match=adjuster_match,
            )
        )
    return slots


def entity_name(entity: object | None) -> str:
    if entity is None:
        return "<unknown>"
    name = entity.best_name()
    return name if name is not None else "<unnamed>"


def is_generated_adjuster(entity: object | None) -> bool:
    if entity is None:
        return False
    if "`vtordisp" in entity_name(entity):
        return True

    symbol = entity.get("symbol")
    if not isinstance(symbol, str):
        return False

    _, separator, encoding = symbol.partition("@@")
    return bool(separator) and encoding.startswith(("W", "$4"))


def decode_this_adjuster(image, address: int | None) -> tuple[int, int] | None:
    """Decode a `this`-adjuster thunk (`sub/add ecx, <offset>; jmp <target>`)."""
    if address is None or not image.is_valid_vaddr(address):
        return None
    try:
        data = image.read(address, 16)
    except (IndexError, ValueError):
        try:
            data = image.read(address, 11)
        except (IndexError, ValueError):
            return None

    adj = None
    jmp_offset = 0

    if len(data) >= 11 and data[0] == 0x81:
        if data[1] == 0xE9:  # sub ecx, imm32
            adj = struct.unpack("<i", data[2:6])[0]
            jmp_offset = 6
        elif data[1] == 0xC1:  # add ecx, imm32
            adj = -struct.unpack("<i", data[2:6])[0]
            jmp_offset = 6
    elif len(data) >= 8 and data[0] == 0x83:
        if data[1] == 0xE9:  # sub ecx, imm8
            adj = struct.unpack("<b", data[2:3])[0]
            jmp_offset = 3
        elif data[1] == 0xC1:  # add ecx, imm8
            adj = -struct.unpack("<b", data[2:3])[0]
            jmp_offset = 3

    if adj is not None and len(data) >= jmp_offset + 5 and data[jmp_offset] == 0xE9:
        disp = struct.unpack("<i", data[jmp_offset + 1 : jmp_offset + 5])[0]
        target = address + jmp_offset + 5 + disp
        return adj, target

    return None


def is_same_generated_adjuster(
    engine: Compare,
    orig: int | None,
    recomp: int | None,
) -> bool:
    orig_thunk = decode_this_adjuster(engine.orig_bin, orig)
    recomp_thunk = decode_this_adjuster(engine.recomp_bin, recomp)
    if orig_thunk is not None and recomp_thunk is not None:
        orig_adj, orig_target = orig_thunk
        recomp_adj, recomp_target = recomp_thunk
        if orig_adj == recomp_adj:
            def is_function_body(image_id: ImageId, address: int) -> bool:
                entity = engine._db.get(image_id, address)
                return entity is not None and not entity_name(entity).startswith("Thunk of '")

            resolved_orig_target = resolve_jump(
                engine.orig_bin, orig_target, lambda a: is_function_body(ImageId.ORIG, a)
            )
            resolved_recomp_target = resolve_jump(
                engine.recomp_bin, recomp_target, lambda a: is_function_body(ImageId.RECOMP, a)
            )
            target_orig_entity = (
                None if resolved_orig_target is None else engine._db.get(ImageId.ORIG, resolved_orig_target)
            )
            target_recomp_entity = (
                None if resolved_recomp_target is None else engine._db.get(ImageId.RECOMP, resolved_recomp_target)
            )
            if target_orig_entity is not None and target_orig_entity.recomp_addr is not None:
                if (
                    target_recomp_entity is not None
                    and target_orig_entity.recomp_addr == target_recomp_entity.recomp_addr
                ):
                    return True
                if target_orig_entity.recomp_addr == resolved_recomp_target:
                    return True

    return False


def format_addr(address: int | None) -> str:
    return "none" if address is None else f"0x{address:08x}"


def unannotated_vtable_stores(engine: Compare, mapped: set[int]) -> dict[int, tuple[int, str]]:
    """Find relocation-backed vptr stores throughout original executable code."""
    image = engine.orig_bin
    relocations = frozenset(image.relocations)
    code_regions = tuple(image.get_code_regions())
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    candidates = {}
    for relocation in sorted(relocations):
        if not any(region.addr <= relocation < region.addr + len(region.data)
                   for region in code_regions):
            continue
        for address in range(relocation - 7, relocation):
            try:
                instruction = next(decoder.disasm(image.read(address, 15), address))
            except (IndexError, StopIteration, ValueError):
                continue
            operands = instruction.operands
            if (instruction.mnemonic != "mov" or len(operands) != 2
                    or operands[0].type != X86_OP_MEM or operands[1].type != X86_OP_IMM
                    or instruction.imm_size != 4
                    or address + instruction.imm_offset != relocation):
                continue
            table = operands[1].imm & 0xffffffff
            if table in mapped or not image.is_valid_vaddr(table):
                continue
            entity = engine._db.get(ImageId.ORIG, table)
            if entity is not None and entity.get("type") != EntityType.VTABLE:
                continue
            try:
                first_slot = int.from_bytes(image.read(table, 4), "little")
            except (IndexError, ValueError):
                continue
            if not any(region.addr <= first_slot < region.addr + len(region.data)
                       for region in code_regions):
                continue
            if transient_purecall_store(image, engine._db, decoder, instruction,
                                        first_slot, mapped, relocations):
                continue
            candidates.setdefault(table, (address, "original executable code"))
    return candidates


def separate_orphan_stores(stores: dict[int, tuple[int, str]]):
    orphans = {
        table: evidence for table, evidence in stores.items()
        if ORPHAN_VTABLE_STORES.get(table) == evidence[0]
    }
    unresolved = {table: evidence for table, evidence in stores.items() if table not in orphans}
    return unresolved, orphans


def transient_purecall_store(image, database, decoder, instruction, first_slot,
                             mapped: set[int], relocations: frozenset[int]) -> bool:
    """Exclude a pure-virtual construction vptr only when immediately replaced.

    Original examples: 0x496cd8 at 0x432b8a, 0x496d00 at 0x434c0e.
    """
    function = database.get(ImageId.ORIG, first_slot)
    if function is None or function.best_name() != "__purecall":
        return False
    original = instruction.operands[0].mem
    if original.base == 0 or original.index != 0 or original.segment != 0:
        return False
    cursor = instruction.address + instruction.size
    for _ in range(4):
        try:
            current = next(decoder.disasm(image.read(cursor, 15), cursor))
        except (IndexError, StopIteration, ValueError):
            return False
        operands = current.operands
        if (current.mnemonic == "lea" and len(operands) == 2
                and operands[0].type == X86_OP_REG and operands[1].type == X86_OP_MEM
                and operands[0].reg not in (original.base, original.index)):
            cursor += current.size
            continue
        if current.mnemonic != "mov" or len(operands) != 2:
            return False
        if operands[0].type == X86_OP_REG and operands[1].type == X86_OP_REG:
            if operands[0].reg in (original.base, original.index):
                return False
            cursor += current.size
            continue
        if operands[0].type != X86_OP_MEM:
            return False
        destination = operands[0].mem
        if (destination.base != original.base or destination.index != original.index
                or destination.segment != original.segment
                or destination.scale != original.scale):
            return False
        if destination.disp == original.disp:
            return (operands[0].size == 4 and operands[1].type == X86_OP_IMM
                    and current.imm_size == 4
                    and cursor + current.imm_offset in relocations
                    and (operands[1].imm & 0xffffffff) in mapped)
        if operands[1].type not in (X86_OP_IMM, X86_OP_REG):
            return False
        cursor += current.size
    return False


def run_comparison(verbose: bool, top: int, annot_strict: bool) -> int:
    try:
        _, engine = load_engine()
    except RecCmpProjectException as error:
        sys.stderr.write(f"vtable: {error}\n")
        return 1

    table_count = 0
    matched_tables = 0
    slot_count = 0
    matched_slots = 0
    equiv = Counter()
    remain = Counter()
    unknown_orig_counts: Counter[int] = Counter()
    known_mismatch_counts: Counter[tuple[int | None, str, int | None, str]] = Counter()
    codebase = DecompCodebase(engine.code_files, engine.target_id, aliases=engine.project_aliases)
    source_vtables = list(codebase.iter_vtables())
    table_matches = list(engine.get_vtables())
    mapped_vtable_addresses = {match.orig_addr for match in table_matches}
    unmapped_vtables = [table for table in source_vtables if table.offset not in mapped_vtable_addresses]
    unannotated_stores, orphan_stores = separate_orphan_stores(
        unannotated_vtable_stores(engine, mapped_vtable_addresses)
    )
    folded_aliases = collect_folded_aliases(engine, codebase)

    for match in table_matches:
        table_count += 1
        slots = compare_table(engine, match, folded_aliases)
        matches = sum(slot.matches for slot in slots)
        slot_count += len(slots)
        matched_slots += matches
        equiv["folded"] += sum(slot.folded_match for slot in slots)
        equiv["clone"] += sum(slot.clone_match for slot in slots)
        equiv["adjuster"] += sum(slot.adjuster_match for slot in slots)
        if matches == len(slots):
            matched_tables += 1
            continue

        if verbose:
            print(
                f"{match.name}: {matches}/{len(slots)} slots "
                f"(orig {format_addr(match.orig_addr)}, recomp {format_addr(match.recomp_addr)})"
            )

        for slot in slots:
            if slot.matches:
                continue
            remain[slot.category] += 1
            if slot.category == "unannotated-original" and slot.orig is not None:
                unknown_orig_counts[slot.orig] += 1
            elif slot.category == "known-mismatch":
                known_mismatch_counts[
                    (
                        slot.orig,
                        entity_name(slot.orig_entity),
                        slot.recomp,
                        entity_name(slot.recomp_entity),
                    )
                ] += 1

            if verbose:
                print(
                    f"  +0x{slot.offset:02x} {slot.category}: "
                    f"orig {format_addr(slot.raw_orig)} -> {format_addr(slot.orig)} "
                    f"[{entity_name(slot.orig_entity)}]; "
                    f"recomp {format_addr(slot.raw_recomp)} -> {format_addr(slot.recomp)} "
                    f"[{entity_name(slot.recomp_entity)}]"
                )

    adjuster_count = 0
    adjuster_problems = 0
    for function in engine.get_functions():
        if not is_generated_adjuster(function):
            continue
        adjuster_count += 1
        result = engine.compare_address(function.orig_addr)
        ratio = getattr(result, "accuracy", 0.0)
        effective = getattr(result, "is_effective_match", False)
        if result is None or (not effective and ratio < 1.0):
            adjuster_problems += 1
            if verbose:
                print(
                    f"adjuster mismatch: {function.name} "
                    f"({format_addr(function.orig_addr)} / {format_addr(function.recomp_addr)})"
                )

    percent = 100.0 * matched_slots / slot_count if slot_count else 0.0
    annotated = len(source_vtables) - len(unmapped_vtables)
    print(
        f"annotated_vtables={matched_tables}/{table_count} "
        f"annotated_slots={matched_slots}/{slot_count} ({percent:.2f}%) "
        f"source_annotations={annotated}/{len(source_vtables)} "
        f"equiv folded={equiv['folded']} clone={equiv['clone']} "
        f"adjuster={equiv['adjuster']} "
        f"remain layout={remain['layout-mismatch']} unannot={remain['unannotated-original']} "
        f"unknown={remain['unknown-recompiled']} mismatch={remain['known-mismatch']} "
        f"adjusters={adjuster_count - adjuster_problems}/{adjuster_count} "
        f"unannotated_stores={len(unannotated_stores)} "
        f"orphan_stores={len(orphan_stores)}"
    )
    if top > 0 and unannotated_stores:
        print("Unannotated vtable store candidates:")
        for table, (address, name) in list(sorted(unannotated_stores.items()))[:top]:
            print(f"  {format_addr(table)} stored at {format_addr(address)} in {name}")
    if top > 0 and orphan_stores:
        print("Audited orphan vtable stores (no identifiable source owner):")
        for table, (address, name) in list(sorted(orphan_stores.items()))[:top]:
            print(f"  {format_addr(table)} stored at {format_addr(address)} in {name}")
    if top > 0 and unmapped_vtables:
        print("Unmapped source vtable annotations:")
        for table in unmapped_vtables[:top]:
            base = "" if table.base_class is None else f" for {table.base_class}"
            print(f"  {format_addr(table.offset)} {table.name}{base}")
    if top > 0 and unknown_orig_counts:
        print("Most frequent unannotated original targets:")
        for address, count in unknown_orig_counts.most_common(top):
            print(f"  {count:4d}x {format_addr(address)}")
    if top > 0 and known_mismatch_counts:
        print("Most frequent known mismatch pairs:")
        for (orig, orig_name, recomp, recomp_name), count in known_mismatch_counts.most_common(top):
            print(
                f"  {count:4d}x {orig_name} ({format_addr(orig)}) -> "
                f"{recomp_name} ({format_addr(recomp)})"
            )
    tables_match = 0 < table_count == matched_tables
    comparisons_pass = tables_match and adjuster_problems == 0
    coverage_pass = not unannotated_stores and (not annot_strict or not unmapped_vtables)
    return 0 if comparisons_pass and coverage_pass else 1


def check_vtable(
    verbose: bool = False,
    top: int = 0,
    annot_strict: bool = False,
) -> int:
    if not RECOMP_EXE.is_file():
        from build import run_build

        result = run_build()
        if result != 0:
            return result

    return run_comparison(verbose, max(top, 0), annot_strict)
