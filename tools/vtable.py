#!/usr/bin/env python3
"""Compare vtables after following MSVC 4.00 linker jmp-rel32 stubs.

Keeps vtordisp/this-adjusting thunks. Accepts opposite scalar/vector deleting
destructor aliases when codegen matches (same rules as check.py).

  python tools/vtable.py --no-build
  python tools/vtable.py --no-build --verbose
  python tools/vtable.py --no-build --annot-strict
"""

from __future__ import annotations

import argparse
import re
import struct
import sys
from collections import Counter
from dataclasses import dataclass
from itertools import zip_longest
from pathlib import Path

from reccmp.compare import Compare
from reccmp.compare.db import ReccmpMatch
from reccmp.parser.codebase import DecompCodebase
from reccmp.project.detect import RecCmpProject, RecCmpProjectException
from reccmp.types import EntityType, ImageId

from build import run_build
from check import is_equivalent_insn, is_codegen_equivalent_diff

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build-msvc400"
MAX_THUNK_DEPTH = 16
DELETING_DESTRUCTOR_RE = re.compile(r"^(?P<class>.+)::`(?P<kind>scalar|vector) deleting destructor'")
NESTED_VTABLE_BASE_RE = re.compile(
    r"^(?P<base>[A-Za-z_][A-Za-z0-9_]*)'s `(?P<via>[A-Za-z_][A-Za-z0-9_]*)$"
)
SIMPLE_CLASS_RE = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")


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
    deleting_dtor_match: bool = False

    @property
    def matches(self) -> bool:
        if self.raw_orig == 0 and self.raw_recomp == 0:
            return True
        if self.folded_match or self.clone_match or self.adjuster_match or self.deleting_dtor_match:
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


def resolve_jump(image, address: int | None, stop_at=None) -> int | None:
    if address is None:
        return None

    seen: set[int] = set()
    current = address
    for _ in range(MAX_THUNK_DEPTH):
        if current in seen:
            break
        seen.add(current)
        if stop_at is not None and stop_at(current):
            break
        if not image.is_valid_vaddr(current):
            break
        try:
            instruction = image.read(current, 5)
        except (IndexError, ValueError):
            break
        if len(instruction) != 5 or instruction[0] != 0xE9:
            break
        displacement = struct.unpack("<i", instruction[1:])[0]
        destination = current + 5 + displacement
        if not image.is_valid_vaddr(destination):
            break
        current = destination
    return current


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


def nested_vtable_symbol(class_name: str, base_class: str | None) -> str | None:
    if base_class is None or SIMPLE_CLASS_RE.fullmatch(class_name) is None:
        return None
    match = NESTED_VTABLE_BASE_RE.fullmatch(base_class)
    if match is None:
        return None
    return f"??_7{class_name}@@6B{match.group('base')}@@{match.group('via')}@@@"


def collect_nested_vtable_matches(engine: Compare, source_vtables, mapped: set[int]) -> list[ReccmpMatch]:
    candidates: dict[str, list[object]] = {}
    for entity in engine._db.unmatched(ImageId.RECOMP):
        if entity.get("type") != EntityType.VTABLE:
            continue
        symbol = entity.get("symbol")
        if symbol is not None:
            candidates.setdefault(symbol, []).append(entity)

    matches: list[ReccmpMatch] = []
    for table in source_vtables:
        if table.offset in mapped:
            continue
        symbol = nested_vtable_symbol(table.name, table.base_class)
        entities = [] if symbol is None else candidates.get(symbol, [])
        if len(entities) != 1:
            continue

        recomp_entity = entities.pop()
        recomp_size = recomp_entity.size(ImageId.RECOMP)
        if recomp_entity.recomp_addr is None or recomp_size is None or recomp_size <= 0:
            continue

        attributes = {
            "type": EntityType.VTABLE,
            "name": table.name,
            "base_class": table.base_class,
            "recomp_size": recomp_size,
        }
        orig_entity = engine._db.get(ImageId.ORIG, table.offset)
        if orig_entity is not None:
            orig_size = orig_entity.size(ImageId.ORIG)
            orig_max_size = orig_entity.max_size(ImageId.ORIG)
            if orig_size is not None:
                attributes["orig_size"] = orig_size
            if orig_max_size is not None:
                attributes["orig_max_size"] = orig_max_size
        matches.append(ReccmpMatch(table.offset, recomp_entity.recomp_addr, attributes))
        mapped.add(table.offset)
    return matches


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
        adjuster_match = not direct_match and is_same_generated_adjuster(
            engine, orig, recomp, orig_entity, recomp_entity
        )
        deleting_dtor_match = not direct_match and is_same_deleting_destructor_alias(
            engine, orig, recomp, orig_entity, recomp_entity
        )
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
                deleting_dtor_match=deleting_dtor_match,
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
    return bool(separator) and (encoding.startswith("W") or encoding.startswith("$4"))


def deleting_destructor_identity(entity: object | None) -> tuple[str, str] | None:
    match = DELETING_DESTRUCTOR_RE.match(entity_name(entity))
    if match is None:
        return None
    return match.group("class"), match.group("kind")


def generated_function_codegen_matches(
    engine: Compare,
    orig: int | None,
    recomp: int | None,
    orig_entity: object | None,
    recomp_entity: object | None,
    name: str,
) -> bool:
    if orig is None or recomp is None or orig_entity is None or recomp_entity is None:
        return False

    recomp_size = recomp_entity.size(ImageId.RECOMP)
    orig_size = orig_entity.size(ImageId.ORIG)
    orig_max_size = orig_entity.max_size(ImageId.ORIG)
    if recomp_size is None or recomp_size <= 0 or recomp_size > 4096:
        return False
    if orig_size is None and orig_max_size is None:
        return False

    attributes = {
        "type": EntityType.FUNCTION,
        "name": name,
        "recomp_size": recomp_size,
    }
    if orig_size is not None:
        attributes["orig_size"] = orig_size
    if orig_max_size is not None:
        attributes["orig_max_size"] = orig_max_size

    try:
        comparison = engine.function_comparator.compare_function(ReccmpMatch(orig, recomp, attributes))
    except (AssertionError, IndexError, ValueError):
        return False
    orig_inst = comparison.diff.orig_inst
    recomp_inst = comparison.diff.recomp_inst
    return (
        comparison.match_ratio == 1.0
        or comparison.is_effective_match
        or (
            bool(orig_inst)
            and len(orig_inst) == len(recomp_inst)
            and is_codegen_equivalent_diff([["", [{"orig": orig_inst, "recomp": recomp_inst}]]])
        )
    )


def is_same_generated_adjuster(
    engine: Compare,
    orig: int | None,
    recomp: int | None,
    orig_entity: object | None,
    recomp_entity: object | None,
) -> bool:
    orig_name = entity_name(orig_entity)
    recomp_name = entity_name(recomp_entity)
    return "`vtordisp" in orig_name and orig_name == recomp_name and generated_function_codegen_matches(
        engine,
        orig,
        recomp,
        orig_entity,
        recomp_entity,
        f"{orig_name} duplicate adjuster",
    )


def is_same_deleting_destructor_alias(
    engine: Compare,
    orig: int | None,
    recomp: int | None,
    orig_entity: object | None,
    recomp_entity: object | None,
) -> bool:
    orig_identity = deleting_destructor_identity(orig_entity)
    recomp_identity = deleting_destructor_identity(recomp_entity)
    if (
        orig is None
        or recomp is None
        or orig_identity is None
        or recomp_identity is None
        or orig_identity[0] != recomp_identity[0]
        or orig_identity[1] == recomp_identity[1]
    ):
        return False

    return generated_function_codegen_matches(
        engine,
        orig,
        recomp,
        orig_entity,
        recomp_entity,
        f"{orig_identity[0]} deleting-destructor alias",
    )


def format_addr(address: int | None) -> str:
    return "none" if address is None else f"0x{address:08x}"


def comparison_is_thunk_equivalent(result) -> bool:
    diff = result.result.diff
    if diff is None:
        return False
    orig = [instruction for _, instruction in diff.orig_inst]
    recomp = [instruction for _, instruction in diff.recomp_inst]
    return bool(orig) and len(orig) == len(recomp) and all(
        is_equivalent_insn(orig_text, recomp_text)
        for orig_text, recomp_text in zip(orig, recomp)
    )


def run_comparison(verbose: bool, top: int, annot_strict: bool) -> int:
    try:
        project = RecCmpProject.from_directory(BUILD)
        engine = Compare.from_target(project.get("LEMBALL"))
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
    nested_vtable_matches = collect_nested_vtable_matches(
        engine, source_vtables, mapped_vtable_addresses
    )
    table_matches.extend(nested_vtable_matches)
    unmapped_vtables = [table for table in source_vtables if table.offset not in mapped_vtable_addresses]
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
        equiv["dtor"] += sum(slot.deleting_dtor_match for slot in slots)
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
        if result is None or (
            result.ratio < 1.0 and not comparison_is_thunk_equivalent(result)
        ):
            adjuster_problems += 1
            if verbose:
                print(
                    f"adjuster mismatch: {function.name} "
                    f"({format_addr(function.orig_addr)} / {format_addr(function.recomp_addr)})"
                )

    percent = 100.0 * matched_slots / slot_count if slot_count else 0.0
    annotated = len(source_vtables) - len(unmapped_vtables)
    print(
        f"vtables={matched_tables}/{table_count} slots={matched_slots}/{slot_count} ({percent:.2f}%) "
        f"annot={annotated}/{len(source_vtables)} nested={len(nested_vtable_matches)} "
        f"equiv folded={equiv['folded']} clone={equiv['clone']} "
        f"adjuster={equiv['adjuster']} dtor={equiv['dtor']} "
        f"remain layout={remain['layout-mismatch']} unannot={remain['unannotated-original']} "
        f"unknown={remain['unknown-recompiled']} mismatch={remain['known-mismatch']} "
        f"adjusters={adjuster_count - adjuster_problems}/{adjuster_count}"
    )
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
    comparisons_pass = table_count > 0 and matched_tables == table_count and adjuster_problems == 0
    coverage_pass = not annot_strict or not unmapped_vtables
    return 0 if comparisons_pass and coverage_pass else 1


def main() -> int:
    parser = argparse.ArgumentParser(description="Thunk-aware LEMBALL vtable comparison")
    parser.add_argument("--no-build", action="store_true", help="skip incremental build")
    parser.add_argument("--clean-first", action="store_true", help="clean before building")
    parser.add_argument("--verbose", "-v", action="store_true", help="show every mismatching slot")
    parser.add_argument("--top", type=int, default=0, help="show the N most frequent unresolved targets and pairs")
    parser.add_argument(
        "--annot-strict",
        action="store_true",
        help="also fail when a source VTABLE annotation cannot be paired",
    )
    args = parser.parse_args()

    if not args.no_build:
        result = run_build(clean_first=args.clean_first)
        if result != 0:
            return result

    return run_comparison(args.verbose, max(args.top, 0), args.annot_strict)


if __name__ == "__main__":
    raise SystemExit(main())
