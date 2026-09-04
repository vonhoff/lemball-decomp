#!/usr/bin/env python3
"""Compare vtables after normalizing transparent MSVC linker artifacts.

The MSVC 4.00 incremental linker routes most vtable entries through five-byte
``jmp rel32`` stubs.  reccmp 0.1.6 compares the raw stub entities, so every
table fails even when both stubs reach matched function bodies.  This wrapper
keeps vtordisp/this-adjusting thunks intact and follows only transparent E9
jumps before applying reccmp's normal entity mapping.

MSVC 4.00 also emits scalar/vector deleting-destructor names through weak
aliases.  Those names may differ even when the actual slot bodies agree, so
same-class aliases are accepted only after a direct codegen-equivalence check.

  python tools/vtable.py
  python tools/vtable.py --no-build
  python tools/vtable.py --no-build --verbose
  python tools/vtable.py --selftest
"""

from __future__ import annotations

import argparse
import logging
import re
import struct
import sys
from collections import Counter
from dataclasses import dataclass
from itertools import zip_longest
from pathlib import Path

from reccmp.compare import Compare
from reccmp.compare.asm.fixes import find_effective_match
from reccmp.compare.db import ReccmpMatch
from reccmp.compare.pinned_sequences import SequenceMatcherWithPins
from reccmp.parser.codebase import DecompCodebase
from reccmp.project.detect import RecCmpProject, RecCmpProjectException
from reccmp.types import EntityType, ImageId

from build import run_build
from check import (
    is_equivalent_insn,
    normalize_asm,
    normalize_copy_tests,
    normalize_multiply_copy_zero,
    normalize_zero_comparisons,
)

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build-msvc400"
MAX_THUNK_DEPTH = 16
DELETING_DESTRUCTOR_RE = re.compile(r"^(?P<class>.+)::`(?P<kind>scalar|vector) deleting destructor'")


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


def resolve_jump(image, address: int | None) -> int | None:
    """Follow a bounded chain of transparent x86 ``jmp rel32`` instructions."""
    if address is None:
        return None

    seen: set[int] = set()
    current = address
    for _ in range(MAX_THUNK_DEPTH):
        if current in seen:
            break
        seen.add(current)
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
    """Recognize an unlabelled original body that exactly copies a mapped body."""
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


def collect_folded_aliases(engine: Compare) -> dict[int, set[int]]:
    """Return every rebuilt body claimed by a source ``FOLDED`` annotation."""
    codebase = DecompCodebase(engine.code_files, engine.target_id, aliases=engine.project_aliases)
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
    for index, (raw_orig, raw_recomp) in enumerate(zip_longest(orig_addrs, recomp_addrs)):
        orig = resolve_jump(engine.orig_bin, raw_orig)
        recomp = resolve_jump(engine.recomp_bin, raw_recomp)

        # reccmp does not currently expose entity lookup as public API.  Keep
        # this one private access isolated so an upstream fix is easy to adopt.
        orig_entity = None if orig is None else engine._db.get(ImageId.ORIG, orig)
        recomp_entity = None if recomp is None else engine._db.get(ImageId.RECOMP, recomp)
        direct_match = (
            orig_entity is not None
            and recomp_entity is not None
            and orig_entity.recomp_addr == recomp_entity.recomp_addr
        )
        folded_match = not direct_match and recomp is not None and recomp in folded_aliases.get(orig, set())
        clone_match = orig_entity is None and is_original_clone(engine.orig_bin, orig, recomp_entity)
        adjuster_match = not direct_match and is_same_generated_adjuster(orig_entity, recomp_entity)
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


def is_same_generated_adjuster(orig_entity: object | None, recomp_entity: object | None) -> bool:
    """Accept duplicate MSVC vtordisp entities only when their full names agree."""
    orig_name = entity_name(orig_entity)
    recomp_name = entity_name(recomp_entity)
    return "`vtordisp" in orig_name and orig_name == recomp_name


def deleting_destructor_identity(entity: object | None) -> tuple[str, str] | None:
    """Return the owning class and generated deleting-destructor kind."""
    match = DELETING_DESTRUCTOR_RE.match(entity_name(entity))
    if match is None:
        return None
    return match.group("class"), match.group("kind")


def codegen_equivalent(comparison) -> bool:
    """Apply check.py's conservative compiler-entropy rules to a direct pair."""
    orig_raw = [instruction for _, instruction in comparison.diff.orig_inst]
    recomp_raw = [instruction for _, instruction in comparison.diff.recomp_inst]
    if not orig_raw or len(orig_raw) != len(recomp_raw):
        return False

    orig_asm = [normalize_asm(instruction) for instruction in orig_raw]
    recomp_asm = [normalize_asm(instruction) for instruction in recomp_raw]
    for index, (orig_text, recomp_text) in enumerate(zip(orig_raw, recomp_raw)):
        if is_equivalent_insn(orig_text, recomp_text):
            recomp_asm[index] = orig_asm[index]

    normalize_copy_tests(orig_asm)
    normalize_copy_tests(recomp_asm)
    normalize_multiply_copy_zero(orig_asm)
    normalize_multiply_copy_zero(recomp_asm)
    normalize_zero_comparisons(orig_asm, recomp_asm)
    matcher = SequenceMatcherWithPins(orig_asm, recomp_asm, [])
    return matcher.ratio() == 1.0 or find_effective_match(matcher.get_opcodes(), orig_asm, recomp_asm)


def is_same_deleting_destructor_alias(
    engine: Compare,
    orig: int | None,
    recomp: int | None,
    orig_entity: object | None,
    recomp_entity: object | None,
) -> bool:
    """Accept MSVC scalar/vector weak aliases only when their bodies agree."""
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

    recomp_size = recomp_entity.size(ImageId.RECOMP)
    orig_size = orig_entity.size(ImageId.ORIG)
    orig_max_size = orig_entity.max_size(ImageId.ORIG)
    if recomp_size is None or recomp_size <= 0 or recomp_size > 4096:
        return False
    if orig_size is None and orig_max_size is None:
        return False

    attributes = {
        "type": EntityType.FUNCTION,
        "name": f"{orig_identity[0]} deleting-destructor alias",
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
    return comparison.match_ratio == 1.0 or comparison.is_effective_match or codegen_equivalent(comparison)


def format_addr(address: int | None) -> str:
    return "none" if address is None else f"0x{address:08x}"


def comparison_ratio(result) -> float:
    if hasattr(result, "accuracy"):
        return float(result.accuracy)
    return float(result.ratio)


def run_comparison(target_id: str, verbose: bool, top: int) -> int:
    # reccmp emits expected collision warnings while staging folded functions
    # and duplicate MSVC-generated thunks. They are classified below instead.
    logging.getLogger("reccmp.compare").setLevel(logging.ERROR)
    try:
        project = RecCmpProject.from_directory(BUILD)
        engine = Compare.from_target(project.get(target_id))
    except RecCmpProjectException as error:
        sys.stderr.write(f"vtable: {error}\n")
        return 1

    table_count = 0
    exact_tables = 0
    slot_count = 0
    matched_slots = 0
    unannotated_slots = 0
    unknown_recomp_slots = 0
    known_mismatch_slots = 0
    layout_mismatch_slots = 0
    folded_match_slots = 0
    clone_match_slots = 0
    adjuster_match_slots = 0
    deleting_dtor_match_slots = 0
    unknown_orig_targets: set[int] = set()
    unknown_orig_counts: Counter[int] = Counter()
    known_mismatch_counts: Counter[tuple[int | None, str, int | None, str]] = Counter()
    codebase = DecompCodebase(engine.code_files, engine.target_id, aliases=engine.project_aliases)
    source_vtables = list(codebase.iter_vtables())
    table_matches = list(engine.get_vtables())
    mapped_vtable_addresses = {match.orig_addr for match in table_matches}
    unmapped_vtables = [table for table in source_vtables if table.offset not in mapped_vtable_addresses]
    folded_aliases = collect_folded_aliases(engine)

    for match in table_matches:
        table_count += 1
        slots = compare_table(engine, match, folded_aliases)
        matches = sum(slot.matches for slot in slots)
        slot_count += len(slots)
        matched_slots += matches
        folded_match_slots += sum(slot.folded_match for slot in slots)
        clone_match_slots += sum(slot.clone_match for slot in slots)
        adjuster_match_slots += sum(slot.adjuster_match for slot in slots)
        deleting_dtor_match_slots += sum(slot.deleting_dtor_match for slot in slots)
        if matches == len(slots):
            exact_tables += 1
            continue

        if verbose:
            print(
                f"{match.name}: {matches}/{len(slots)} slots "
                f"(orig {format_addr(match.orig_addr)}, recomp {format_addr(match.recomp_addr)})"
            )

        for slot in slots:
            if slot.matches:
                continue
            if slot.category == "layout-mismatch":
                layout_mismatch_slots += 1
            elif slot.category == "unannotated-original":
                unannotated_slots += 1
                if slot.orig is not None:
                    unknown_orig_targets.add(slot.orig)
                    unknown_orig_counts[slot.orig] += 1
            elif slot.category == "unknown-recompiled":
                unknown_recomp_slots += 1
            else:
                known_mismatch_slots += 1
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

    adjuster_problems = 0
    for function in engine.get_functions():
        if function.name is None or "`vtordisp" not in function.name:
            continue
        result = engine.compare_address(function.orig_addr)
        if result is not None and comparison_ratio(result) < 1.0:
            adjuster_problems += 1
            if verbose:
                print(
                    f"adjuster mismatch: {function.name} "
                    f"({format_addr(function.orig_addr)} / {format_addr(function.recomp_addr)})"
                )

    percent = 100.0 * matched_slots / slot_count if slot_count else 0.0
    print(f"Source vtable annotations mapped: {len(source_vtables) - len(unmapped_vtables)}/{len(source_vtables)}.")
    print(f"Thunk-normalized vtables exact: {exact_tables}/{table_count}.")
    print(f"Vtable slots matched: {matched_slots}/{slot_count} ({percent:.2f}%).")
    print(
        "Equivalent slots: "
        f"{folded_match_slots} folded aliases, "
        f"{clone_match_slots} exact original clones, "
        f"{adjuster_match_slots} duplicate named adjusters, "
        f"{deleting_dtor_match_slots} deleting-destructor aliases."
    )
    print(
        "Remaining slots: "
        f"{layout_mismatch_slots} layout/null mismatches, "
        f"{unannotated_slots} unannotated original "
        f"({len(unknown_orig_targets)} unique targets), "
        f"{unknown_recomp_slots} unknown recompiled, "
        f"{known_mismatch_slots} known mismatches."
    )
    print(f"Adjuster thunk mismatches: {adjuster_problems}.")
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
    return 0 if not unmapped_vtables and exact_tables == table_count and adjuster_problems == 0 else 1


class FakeImage:
    def __init__(self, start: int, data: bytes):
        self.start = start
        self.data = data

    def is_valid_vaddr(self, address: int) -> bool:
        return self.start <= address < self.start + len(self.data)

    def read(self, address: int, size: int) -> bytes:
        offset = address - self.start
        if offset < 0:
            raise ValueError(address)
        return self.data[offset : offset + size]


def rel32(source: int, destination: int) -> bytes:
    return b"\xe9" + struct.pack("<i", destination - source - 5)


class FakeEntity:
    def __init__(self, orig_addr: int, size: int):
        self.orig_addr = orig_addr
        self._size = size

    def size(self, image_id: ImageId) -> int:
        assert image_id == ImageId.RECOMP
        return self._size


class FakeNamedEntity:
    def __init__(self, name: str):
        self.name = name

    def best_name(self) -> str:
        return self.name


def run_selftest() -> int:
    start = 0x1000
    data = bytearray(b"\x90" * 0x40)
    data[0:5] = rel32(start, start + 0x10)
    data[0x10:0x15] = rel32(start + 0x10, start + 0x20)
    image = FakeImage(start, bytes(data))

    samples = (
        (start, start + 0x20),
        (start + 0x10, start + 0x20),
        (start + 0x20, start + 0x20),
        (None, None),
    )
    for address, expected in samples:
        actual = resolve_jump(image, address)
        if actual != expected:
            print(f"selftest fail: {format_addr(address)} -> {format_addr(actual)}, expected {format_addr(expected)}")
            return 1

    cyclic = bytearray(b"\x90" * 0x20)
    cyclic[0:5] = rel32(start, start + 0x10)
    cyclic[0x10:0x15] = rel32(start + 0x10, start)
    if resolve_jump(FakeImage(start, bytes(cyclic)), start) != start:
        print("selftest fail: cyclic thunk chain did not stop safely")
        return 1

    clone_data = b"\xc2\x04\x00\x90\xc2\x04\x00\xcc"
    clone_image = FakeImage(start, clone_data)
    if not is_original_clone(clone_image, start + 4, FakeEntity(start, 3)):
        print("selftest fail: exact original clone was not recognized")
        return 1
    if is_original_clone(clone_image, start + 3, FakeEntity(start, 3)):
        print("selftest fail: non-clone original body was accepted")
        return 1

    adjuster_name = "Thing::Method`vtordisp{-4, 0}'"
    if not is_same_generated_adjuster(FakeNamedEntity(adjuster_name), FakeNamedEntity(adjuster_name)):
        print("selftest fail: duplicate named adjuster was not recognized")
        return 1
    if is_same_generated_adjuster(FakeNamedEntity(adjuster_name), FakeNamedEntity("Thing::Method")):
        print("selftest fail: unequal adjusters were accepted")
        return 1

    scalar_dtor = FakeNamedEntity("Thing::`scalar deleting destructor'")
    vector_dtor = FakeNamedEntity("Thing::`vector deleting destructor'(unsigned int)")
    if deleting_destructor_identity(scalar_dtor) != ("Thing", "scalar"):
        print("selftest fail: scalar deleting-destructor identity was not parsed")
        return 1
    if deleting_destructor_identity(vector_dtor) != ("Thing", "vector"):
        print("selftest fail: vector deleting-destructor identity was not parsed")
        return 1
    if deleting_destructor_identity(FakeNamedEntity("Other::~Other")) is not None:
        print("selftest fail: ordinary destructor was accepted as a deleting destructor")
        return 1

    null_slot = SlotResult(0, 0, 0, 0, 0, None, None)
    if not null_slot.matches:
        print("selftest fail: corresponding null slots did not match")
        return 1
    one_sided_null = SlotResult(0, 0, 1, 0, 1, None, None)
    if one_sided_null.matches:
        print("selftest fail: one-sided null slot was accepted")
        return 1
    if one_sided_null.category != "layout-mismatch":
        print("selftest fail: one-sided null slot was not classified as a layout mismatch")
        return 1

    print("vtable selftest: ok")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description="Thunk-aware LEMBALL vtable comparison")
    parser.add_argument("--target", default="LEMBALL", help="reccmp target id (default: LEMBALL)")
    parser.add_argument("--no-build", action="store_true", help="skip incremental build")
    parser.add_argument("--clean-first", action="store_true", help="clean before building")
    parser.add_argument("--verbose", "-v", action="store_true", help="show every mismatching slot")
    parser.add_argument("--top", type=int, default=0, help="show the N most frequent unresolved targets and pairs")
    parser.add_argument("--selftest", action="store_true", help="exercise jump resolution and exit")
    args = parser.parse_args()

    if args.selftest:
        return run_selftest()
    if not args.no_build:
        result = run_build(clean_first=args.clean_first)
        if result != 0:
            return result

    return run_comparison(args.target, args.verbose, max(args.top, 0))


if __name__ == "__main__":
    raise SystemExit(main())
