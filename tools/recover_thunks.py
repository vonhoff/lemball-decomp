#!/usr/bin/env python3
"""Recover or verify original jump entries and compiler relocation routes."""

import argparse
import hashlib
import json
import os
import subprocess
import sys
from bisect import bisect_left
from dataclasses import replace
from functools import cache
from typing import cast

from reccmp.compare import Compare
from reccmp.compare.diff import RawDiffOutput
from reccmp.compare.ingest import load_cvdump_lines
from reccmp.compare.lines import LinesDb
from reccmp.dir import source_code_search
from reccmp.formats import PEImage
from reccmp.project.detect import RecCmpProject
from reccmp.parser.codebase import DecompCodebase
from reccmp.tools.roadmap import ModuleMap
from reccmp.types import EntityType, ImageId

from link_binary import CoffObject, DIR32, REL32, read_jump_target, thunk_symbol
from lib import BUILD, ROOT, TARGET_ID

MANIFEST = ROOT / "tools/data/linker-thunks.json"
ANNOTATIONS = ROOT / "src/Platform/MSVC/LinkerThunks.h"
OUTPUT = BUILD / "linker-thunks"


def instruction_target(raw: bytes, address: int) -> int:
    """Decode a five-byte branch or immediate pointer instruction."""
    target = int.from_bytes(raw[1:], "little")
    if raw[0] in (0xE8, 0xE9):
        target = (address + 5 + target) & 0xFFFFFFFF
    return target


def thunk_matches(engine, functions, thunk):
    """Check a saved jump entry and its paired destination."""
    entry = functions.get(thunk["address"])
    body = functions.get(thunk["target"])
    comparison = engine.compare_address(thunk["address"])
    return not (
        entry is None
        or body is None
        or comparison is None
        or comparison.is_stub
        or comparison.accuracy != 1
        or engine.orig_bin.read(thunk["address"], 5).hex() != thunk["original_bytes"]
        or read_jump_target(engine.orig_bin, thunk["address"]) != thunk["target"]
        or read_jump_target(engine.recomp_bin, entry.recomp_addr) != body.recomp_addr
    )


def reference_matches(engine, functions, caller, reference):
    """Check that a code or data owner references the saved jump entry."""
    instruction = caller.recomp_addr + reference["offset"]
    pointer = "original_pointer" in reference
    length = 4 if pointer else 5
    original_address = (
        reference["original_pointer"] if pointer else reference["original_instruction"]
    )
    original = engine.orig_bin.read(original_address, length)
    rebuilt = engine.recomp_bin.read(instruction, length)
    if pointer:
        if (
            original_address != caller.orig_addr + reference["offset"]
            or original_address not in engine.orig_bin.relocations
        ):
            return False
        original_target = int.from_bytes(original, "little")
        rebuilt_target = int.from_bytes(rebuilt, "little")
    else:
        original_target = instruction_target(original, original_address)
        rebuilt_target = instruction_target(rebuilt, instruction)
    entry = functions.get(reference["thunk"])
    return not (
        entry is None
        or original.hex() != reference["original_bytes"]
        or (not pointer and rebuilt[0] != original[0])
        or original_target != reference["thunk"]
        or rebuilt_target != entry.recomp_addr
    )


def verify_thunks():
    """Verify saved routes and report failures against the current executable."""
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    target = RecCmpProject.from_directory(BUILD).get(TARGET_ID)
    engine = Compare.from_target(target)
    functions = {function.orig_addr: function for function in engine.get_functions()}
    entities = {
        entity.orig_addr: entity for entity in engine.get_all() if entity.orig_addr
    }
    errors = []
    verified_thunks = 0
    verified_references = 0
    if (
        hashlib.sha256(target.original_path.read_bytes()).hexdigest()
        != manifest["original_sha256"]
    ):
        raise ValueError("Original image differs from the recorded thunk evidence")
    for thunk in manifest["thunks"]:
        if not thunk_matches(engine, functions, thunk):
            errors.append(
                {
                    "thunk": thunk["address"],
                    "reason": "Jump entry or destination mismatch",
                }
            )
        else:
            verified_thunks += 1
    for route in manifest["routes"]:
        caller = entities.get(route["caller"])
        for reference in route["references"]:
            if caller is None:
                errors.append(
                    {"caller": route["caller"], "reason": "Caller is unmatched"}
                )
                continue
            if not reference_matches(engine, functions, caller, reference):
                errors.append(
                    {
                        "caller": route["caller"],
                        "reference": reference.get(
                            "original_pointer", reference.get("original_instruction")
                        ),
                        "reason": "Caller does not reference the evidenced jump entry",
                    }
                )
            else:
                verified_references += 1
    result = {
        "original_sha256": manifest["original_sha256"],
        "rebuilt_sha256": hashlib.sha256(
            target.recompiled_path.read_bytes()
        ).hexdigest(),
        "thunks_verified": verified_thunks,
        "references_verified": verified_references,
        "errors": errors,
    }
    path = OUTPUT / "verification.json"
    path.write_text(json.dumps(result, indent=2) + "\n")
    print(
        f"verified {verified_thunks} jump entries; {verified_references} references; "
        f"{len(errors)} errors; {path}"
    )
    return int(bool(errors))


def link_unrouted(target):
    """Link native compiler objects without applying saved routes."""
    OUTPUT.mkdir(exist_ok=True)
    build_rules = (BUILD / "CMakeFiles/LEMBALL.dir/build.make").read_text()
    command = next(
        line.strip().split()
        for line in build_rules.splitlines()
        if line.startswith("\tpython ") and "tools/link_binary.py" in line
    )
    command[0] = sys.executable
    command = [
        "/OUT:linker-thunks/unrouted.EXE" if arg.upper().startswith("/OUT:") else arg
        for arg in command
    ]
    command.append("/PDB:linker-thunks/unrouted.pdb")
    environment = os.environ.copy()
    environment["LIB"] = str(ROOT / "msvc400/lib")
    environment["INCLUDE"] = str(ROOT / "msvc400/include")
    environment["PATH"] = str(ROOT / "msvc400/bin") + ";" + environment["PATH"]
    with (OUTPUT / "unrouted-link.log").open("w", encoding="utf-8") as log:
        subprocess.run(
            command,
            cwd=BUILD,
            env=environment,
            stdout=log,
            stderr=subprocess.STDOUT,
            check=True,
        )
    return replace(
        target,
        recompiled_path=OUTPUT / "unrouted.EXE",
        recompiled_pdb=OUTPUT / "unrouted.pdb",
    )


def jump_entries(engine, functions, forwarders):
    """Select linker-table entries and validate requested tail forwarders."""
    table_entries = set()
    for region in engine.orig_bin.get_code_regions():
        for offset in range(0, len(region.data) - 4, 5):
            if region.data[offset] != 0xE9:
                break
            table_entries.add(region.addr + offset)
    originals = {entity.orig_addr: entity for entity in engine.get_all()}
    for address in forwarders:
        original = originals.get(address)
        destination = read_jump_target(engine.orig_bin, address)
        if (
            address in table_entries
            or address in functions
            or original is None
            or original.size(ImageId.ORIG) != 5
            or destination not in functions
        ):
            raise ValueError(
                f"Expected an unpaired five-byte tail jump to a paired body: {address:#x}"
            )
    return table_entries | forwarders


def aligned_instructions(diff: RawDiffOutput, route, previous, rebuilt_address):
    """Yield saved positions for unchanged bodies, then diff-aligned positions."""
    if previous is not None and all(
        previous[key] == route[key] for key in ("symbol", "size", "fingerprint")
    ):
        for reference in previous["references"]:
            yield (
                reference["original_instruction"],
                rebuilt_address + reference["offset"],
            )
    for tag, first, last, rebuilt_first, rebuilt_last in diff.codes:
        if tag == "equal" or (
            tag == "replace" and last - first == rebuilt_last - rebuilt_first == 1
        ):
            for original, rebuilt in zip(
                diff.orig_inst[first:last],
                diff.recomp_inst[rebuilt_first:rebuilt_last],
                strict=True,
            ):
                if original[0] and rebuilt[0]:
                    yield int(original[0], 16), int(rebuilt[0], 16)


@cache
def load_object(name):
    """Read each native compiler object once in this process."""
    return CoffObject((BUILD / name).read_bytes())


@cache
def strong_symbols():
    """Collect strong external function definitions for weak-default overrides."""
    return {
        symbol[0]
        for path in (BUILD / "CMakeFiles/LEMBALL.dir").rglob("*.obj")
        for symbol in CoffObject(path.read_bytes()).symbols.values()
        if symbol[2] > 0 and symbol[3] == 0x20 and symbol[4] == 2
    }


def relocation_symbols(
    obj, caller_symbol, offset, relocation, callee_address, public_addresses
):
    """Resolve a COFF relocation to the linked callee, including strong overrides."""
    source = obj.reference_target(caller_symbol, offset, relocation)
    target = obj.canonical_name(source)
    if public_addresses.get(target) != callee_address:
        if (
            public_addresses.get(source) != callee_address
            or source not in strong_symbols()
        ):
            raise ValueError(
                "COFF call target does not match the linked callee address"
            )
        target = source
    return source, target


def folded_aliases(engine, functions, modules):
    """Require FOLDED annotations, identical full bodies, and native definitions."""
    codebase = DecompCodebase(
        engine.code_files, TARGET_ID, aliases=engine.project_aliases
    )
    lines = LinesDb()
    load_cvdump_lines(engine.cvdump_analysis, lines, engine.recomp_bin)
    lines.add_local_paths(file.path for file in engine.code_files)
    rebuilt = {
        entity.recomp_addr: entity for entity in engine.get_all() if entity.recomp_addr
    }
    groups = {}
    for annotation in codebase.iter_line_functions():
        if not annotation.is_folded or annotation.should_skip():
            continue
        address = lines.find_function(
            annotation.filename,
            annotation.line_number,
            annotation.end_line,
            folded=True,
        )
        entity = rebuilt.get(address)
        body = functions.get(annotation.offset)
        if entity is None or body is None or entity.entity_type != EntityType.FUNCTION:
            continue
        size = body.size(ImageId.RECOMP)
        if not size or entity.size(ImageId.RECOMP) != size:
            continue
        if engine.recomp_bin.read(address, size) != engine.recomp_bin.read(
            body.recomp_addr, size
        ):
            continue
        module = modules.get_module(address)
        if module is None:
            continue
        object_name = module[1].replace("\\", "/")
        if not object_name.startswith("CMakeFiles/LEMBALL.dir/"):
            continue
        try:
            obj = load_object(object_name)
            symbol = obj.function(entity.get("symbol"))[0]
            definition = {
                "object": object_name,
                "symbol": symbol,
                "size": size,
                "fingerprint": obj.fingerprint(symbol, size, include_symbols=True),
            }
        except (ValueError, OSError):
            continue
        groups.setdefault(annotation.offset, {})[address] = definition
    return {
        address: definitions
        for address, definitions in groups.items()
        if len(definitions) > 1 and functions[address].recomp_addr in definitions
    }


def make_thunk(engine, callee, entry, target_symbol, aliases):
    """Choose the paired body and retain evidenced alternate folded symbols."""
    definitions = aliases.get(callee.orig_addr, {})
    thunk = {
        "address": entry,
        "target": callee.orig_addr,
        "original_bytes": engine.orig_bin.read(entry, 5).hex(),
        "symbol": thunk_symbol(entry),
        "target_symbol": definitions[callee.recomp_addr]["symbol"]
        if definitions
        else target_symbol,
    }
    if definitions:
        thunk["folded_aliases"] = sorted(
            definitions.values(), key=lambda definition: definition["symbol"]
        )
    return thunk


def name_jump_destinations(engine, functions, entries):
    """Use decoded E9 destinations for recovery alignment, keeping raw route checks."""
    parser = engine.function_comparator.orig_sanitize
    lookup = parser.name_lookup
    destinations = {
        entry: destination
        for entry in entries
        if (destination := read_jump_target(engine.orig_bin, entry)) in functions
    }

    def lookup_jump(address, exact=False, indirect=False):
        if not indirect and address in destinations:
            return lookup(destinations[address], exact=True)
        return lookup(address, exact=exact, indirect=indirect)

    parser.name_lookup = lookup_jump


def native_context(engine, target):
    """Locate native objects and exact public addresses in the unrouted image."""
    image = cast(PEImage, engine.recomp_bin)
    modules = ModuleMap(target.recompiled_pdb, image)
    public_addresses = {}
    for public in engine.cvdump_analysis.parser.publics:
        if image.is_valid_section(public.section):
            public_addresses[public.name] = image.get_abs_addr(
                public.section, public.offset
            )
    return modules, public_addresses


def native_definition(callee, modules, public_addresses):
    """Require a real project COFF function at its exact PDB public address."""
    if not callee.get("symbol"):
        raise ValueError("No paired native symbol")
    module = modules.get_module(callee.recomp_addr)
    if module is None:
        raise ValueError("No native object module")
    object_name = module[1].replace("\\", "/")
    if not object_name.startswith("CMakeFiles/LEMBALL.dir/"):
        raise ValueError("Definition is outside the project objects")
    definition = load_object(object_name).function(callee.get("symbol"))
    symbol = definition[0]
    if definition[4] != 2:
        raise ValueError("Definition is not external")
    if public_addresses.get(symbol) != callee.recomp_addr:
        raise ValueError("Definition public address differs from the paired body")
    return {"object": object_name, "symbol": symbol}


def recover_table_entries(
    engine, addresses, functions, entries, thunks, modules, public_addresses, aliases
):
    """Recover real E9 table slots from paired native external definitions."""
    for address in addresses:
        if address not in entries or address in thunks:
            continue
        destination = read_jump_target(engine.orig_bin, address)
        callee = functions.get(destination)
        if callee is None or not callee.get("symbol"):
            continue
        try:
            definition = native_definition(callee, modules, public_addresses)
        except (ValueError, OSError):
            continue
        thunk = make_thunk(engine, callee, address, definition["symbol"], aliases)
        thunk["definition"] = definition
        thunks[address] = thunk


def recover_references(
    engine,
    functions,
    entries,
    public_addresses,
    obj,
    function,
    route,
    previous,
    aliases,
):
    """Recover relocation-backed references to unpaired original jump entries."""
    references = []
    thunks = {}
    caller_symbol = route["symbol"]
    diff = engine.function_comparator.compare_function(function).diff
    seen_offsets = set()
    for original_addr, rebuilt_addr in aligned_instructions(
        diff, route, previous, function.recomp_addr
    ):
        offset = rebuilt_addr - function.recomp_addr
        if offset in seen_offsets:
            continue
        original = engine.orig_bin.read(original_addr, 5)
        rebuilt = engine.recomp_bin.read(rebuilt_addr, 5)
        relative = original[0] in (0xE8, 0xE9)
        pointer = original[0] == 0x68 or 0xB8 <= original[0] <= 0xBF
        if not (relative or pointer) or rebuilt[0] != original[0]:
            continue
        relocation = REL32 if relative else DIR32
        entry = instruction_target(original, original_addr)
        if entry in functions or entry not in entries:
            continue
        destination = cast(int, read_jump_target(engine.orig_bin, entry))
        callee = functions.get(destination)
        if callee is None:
            continue
        rebuilt_target = instruction_target(rebuilt, rebuilt_addr)
        body_addresses = {callee.recomp_addr} | set(aliases.get(destination, {}))
        if (
            rebuilt_target not in body_addresses
            and read_jump_target(engine.recomp_bin, rebuilt_target)
            not in body_addresses
        ):
            continue
        callee_address = (
            rebuilt_target
            if rebuilt_target in body_addresses
            else read_jump_target(engine.recomp_bin, rebuilt_target)
        )
        try:
            source_symbol, target_symbol = relocation_symbols(
                obj,
                caller_symbol,
                offset + 1,
                relocation,
                callee_address,
                public_addresses,
            )
        except ValueError:
            continue
        reference = {
            "offset": offset,
            "original_instruction": original_addr,
            "original_bytes": original.hex(),
            "thunk": entry,
            "source_symbol": source_symbol,
        }
        references.append(reference)
        seen_offsets.add(offset)
        thunks[entry] = make_thunk(engine, callee, entry, target_symbol, aliases)
    return references, thunks


def data_pointer_addresses(owner, original_relocations, size):
    """Bound original pointer fields by both symbol extents and the next symbol."""
    original_size = min(
        size, owner.any_size(ImageId.ORIG), owner.max_size(ImageId.ORIG) or size
    )
    first = bisect_left(original_relocations, owner.orig_addr)
    last = bisect_left(original_relocations, owner.orig_addr + original_size - 3)
    return original_relocations[first:last]


def recover_data_references(
    engine,
    functions,
    entries,
    public_addresses,
    obj,
    owner,
    route,
    original_relocations,
    aliases,
):
    """Recover original PE pointers backed by native zero-addend DIR32 relocations."""
    references, thunks = [], {}
    for original_address in data_pointer_addresses(
        owner, original_relocations, route["size"]
    ):
        original = engine.orig_bin.read(original_address, 4)
        entry = int.from_bytes(original, "little")
        if entry not in entries or entry in functions:
            continue
        destination = read_jump_target(engine.orig_bin, entry)
        callee = functions.get(destination)
        if callee is None:
            continue
        offset = original_address - owner.orig_addr
        rebuilt_target = int.from_bytes(
            engine.recomp_bin.read(owner.recomp_addr + offset, 4), "little"
        )
        if rebuilt_target not in {callee.recomp_addr} | set(
            aliases.get(destination, {})
        ):
            continue
        try:
            source_symbol, target_symbol = relocation_symbols(
                obj, route["symbol"], offset, DIR32, rebuilt_target, public_addresses
            )
        except ValueError:
            continue
        references.append(
            {
                "offset": offset,
                "original_pointer": original_address,
                "original_bytes": original.hex(),
                "thunk": entry,
                "source_symbol": source_symbol,
            }
        )
        thunks[entry] = make_thunk(engine, callee, entry, target_symbol, aliases)
    return references, thunks


def recover_routes(engine, target, addresses, functions, entries, previous_routes):
    """Recover code and data routes that the native COFF objects can redirect."""
    modules, public_addresses = native_context(engine, target)
    aliases = folded_aliases(engine, functions, modules)
    name_jump_destinations(engine, functions, entries)
    entities = {
        entity.orig_addr: entity for entity in engine.get_all() if entity.orig_addr
    }
    data_regions = [
        section.virtual_range
        for section in engine.orig_bin.sections
        if section.name in (".rdata", ".data")
    ]
    addresses.update(
        dict.fromkeys(
            address
            for address, entity in entities.items()
            if entity.recomp_addr
            and entity.entity_type in (EntityType.DATA, EntityType.VTABLE)
            and any(address in region for region in data_regions)
        )
    )
    original_relocations = sorted(engine.orig_bin.relocations)
    thunks = {}
    routes = []
    unresolved = []
    for address in addresses:
        function = entities.get(address)
        if function is None or function.get("stub") or not function.get("symbol"):
            continue
        data = function.entity_type in (EntityType.DATA, EntityType.VTABLE)
        if data and not any(
            int.from_bytes(engine.orig_bin.read(pointer, 4), "little") in entries
            for pointer in data_pointer_addresses(
                function, original_relocations, function.any_size(ImageId.RECOMP)
            )
        ):
            continue
        module = modules.get_module(function.recomp_addr)
        if module is None:
            continue
        object_name = module[1].replace("\\", "/")
        if not object_name.startswith("CMakeFiles/LEMBALL.dir/"):
            continue
        try:
            obj = load_object(object_name)
            caller_symbol = (obj.definition if data else obj.function)(
                function.get("symbol")
            )[0]
            if public_addresses.get(caller_symbol) != function.recomp_addr:
                raise ValueError(
                    "COFF weak default does not match the linked caller address"
                )
            size = function.size(ImageId.RECOMP)
            fingerprint = obj.fingerprint(caller_symbol, size)
        except (ValueError, OSError) as error:
            unresolved.append({"caller": address, "reason": str(error)})
            continue
        route = {
            "caller": address,
            "object": object_name,
            "symbol": caller_symbol,
            "size": size,
            "fingerprint": fingerprint,
        }
        if data:
            route["kind"] = "data"
            references, found = recover_data_references(
                engine,
                functions,
                entries,
                public_addresses,
                obj,
                function,
                route,
                original_relocations,
                aliases,
            )
        else:
            references, found = recover_references(
                engine,
                functions,
                entries,
                public_addresses,
                obj,
                function,
                route,
                previous_routes.get(address),
                aliases,
            )
        if not references:
            continue
        route["references"] = references
        try:
            obj.redirect([route], thunks | found)
        except (ValueError, KeyError) as error:
            unresolved.append({"caller": address, "reason": str(error)})
            continue
        thunks.update(found)
        routes.append(route)
    recover_table_entries(
        engine,
        addresses,
        functions,
        entries,
        thunks,
        modules,
        public_addresses,
        aliases,
    )
    routes.sort(key=lambda caller_route: caller_route["caller"])
    return routes, thunks, unresolved


def write_recovery(target, thunks, routes, unresolved):
    """Save the manifest, matching annotations, and unresolved owner report."""
    manifest = {
        "version": 3,
        "original_sha256": hashlib.sha256(
            target.original_path.read_bytes()
        ).hexdigest(),
        "thunks": thunks,
        "routes": routes,
    }
    MANIFEST.write_text(
        json.dumps(manifest, indent=2) + "\n", encoding="utf-8", newline="\n"
    )
    lines = ["#ifndef LEMBALL_LINKER_THUNKS_H", "#define LEMBALL_LINKER_THUNKS_H", ""]
    for thunk in thunks:
        lines += [
            f"// SYNTHETIC: LEMBALL 0x{thunk['address']:08x} SYMBOL",
            "// " + thunk["symbol"],
            "",
        ]
    lines += ["#endif", ""]
    ANNOTATIONS.write_text("\n".join(lines), encoding="utf-8", newline="\n")
    (OUTPUT / "unresolved.json").write_text(json.dumps(unresolved, indent=2) + "\n")
    print(
        f"recovered {len(thunks)} thunks; {len(routes)} owners; "
        f"{sum(len(route['references']) for route in routes)} references; "
        f"{len(unresolved)} unresolved owners"
    )


def recover_thunks(forwarders):
    """Recover evidence from an unrouted link, retaining unchanged saved callers."""
    previous = {"routes": [], "thunks": []}
    ranking = subprocess.check_output(
        [
            sys.executable,
            str(ROOT / "tools/triage_targets.py"),
            "--exact",
            "--limit",
            "0",
        ],
        text=True,
    )
    addresses = dict.fromkeys(int(row.split()[0], 16) for row in ranking.splitlines())
    if MANIFEST.exists():
        previous = json.loads(MANIFEST.read_text())
        forwarders.update(
            thunk["address"]
            for thunk in previous["thunks"]
            if thunk.get("kind") == "tail-forwarder"
        )
        addresses.update(dict.fromkeys(route["caller"] for route in previous["routes"]))
        addresses.update(
            dict.fromkeys(
                thunk["address"]
                for thunk in previous["thunks"]
                if thunk.get("kind") != "tail-forwarder"
            )
        )
    target = link_unrouted(RecCmpProject.from_directory(BUILD).get(TARGET_ID))
    paths = tuple(
        path
        for path in source_code_search(target.source_paths)
        if path.resolve() != ANNOTATIONS.resolve()
    )
    engine = Compare.from_target(replace(target, source_paths=paths))
    functions = {function.orig_addr: function for function in engine.get_functions()}
    entries = jump_entries(engine, functions, forwarders)
    previous_routes = {route["caller"]: route for route in previous["routes"]}
    routes, thunks, unresolved = recover_routes(
        engine, target, addresses, functions, entries, previous_routes
    )
    used = {reference["thunk"] for route in routes for reference in route["references"]}
    if missing := forwarders - used:
        raise ValueError(
            f"No verified compiler references for selected forwarders: {sorted(missing)}"
        )
    for address in forwarders:
        thunks[address]["kind"] = "tail-forwarder"
    write_recovery(
        target, [thunks[address] for address in sorted(thunks)], routes, unresolved
    )
    return 0


def main():
    """Dispatch recovery or verification from the command line."""
    parser = argparse.ArgumentParser(description=__doc__)
    action = parser.add_mutually_exclusive_group()
    action.add_argument(
        "--verify",
        action="store_true",
        help="Check the current executable against saved routes",
    )
    action.add_argument(
        "--forwarder",
        action="append",
        default=[],
        type=lambda value: int(value, 16),
        help="Original five-byte tail-jump entry outside the linker table; repeatable",
    )
    args = parser.parse_args()
    return verify_thunks() if args.verify else recover_thunks(set(args.forwarder))


if __name__ == "__main__":
    raise SystemExit(main())
