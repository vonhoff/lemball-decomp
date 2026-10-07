#!/usr/bin/env python3
"""Recover or verify original jump entries and compiler relocation routes."""

import argparse
import hashlib
import json
import os
import subprocess
import sys
from dataclasses import replace
from functools import cache
from typing import cast

from reccmp.compare import Compare
from reccmp.compare.csv import csv_parse
from reccmp.compare.diff import RawDiffOutput
from reccmp.dir import source_code_search
from reccmp.formats import PEImage
from reccmp.project.detect import RecCmpProject
from reccmp.tools.roadmap import ModuleMap
from reccmp.types import ImageId

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
    """Check that a caller references the saved jump entry."""
    instruction = caller.recomp_addr + reference["offset"]
    original = engine.orig_bin.read(reference["original_instruction"], 5)
    rebuilt = engine.recomp_bin.read(instruction, 5)
    original_target = instruction_target(original, reference["original_instruction"])
    rebuilt_target = instruction_target(rebuilt, instruction)
    entry = functions.get(reference["thunk"])
    return not (
        entry is None
        or original.hex() != reference["original_bytes"]
        or rebuilt[0] != original[0]
        or original_target != reference["thunk"]
        or rebuilt_target != entry.recomp_addr
    )


def verify_thunks():
    """Verify saved routes and report failures against the current executable."""
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    target = RecCmpProject.from_directory(BUILD).get(TARGET_ID)
    engine = Compare.from_target(target)
    functions = {function.orig_addr: function for function in engine.get_functions()}
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
        caller = functions.get(route["caller"])
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
                        "reference": reference["original_instruction"],
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
    catalog = dict(
        csv_parse((ROOT / "tools/data/original-function-sizes.csv").read_text())
    )
    for address in forwarders:
        original = catalog.get(address)
        destination = read_jump_target(engine.orig_bin, address)
        if (
            address in table_entries
            or address in functions
            or original is None
            or original["size"] != 5
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


def recover_references(
    engine, functions, entries, public_addresses, obj, function, route, previous
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
        if (
            rebuilt_target != callee.recomp_addr
            and read_jump_target(engine.recomp_bin, rebuilt_target)
            != callee.recomp_addr
        ):
            continue
        try:
            source_symbol, target_symbol = relocation_symbols(
                obj,
                caller_symbol,
                offset,
                relocation,
                callee.recomp_addr,
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
        thunks[entry] = {
            "address": entry,
            "target": destination,
            "original_bytes": engine.orig_bin.read(entry, 5).hex(),
            "symbol": thunk_symbol(entry),
            "target_symbol": target_symbol,
        }
    return references, thunks


def recover_routes(engine, target, addresses, functions, entries, previous_routes):
    """Recover caller routes that the native COFF objects can redirect."""
    image = cast(PEImage, engine.recomp_bin)
    modules = ModuleMap(target.recompiled_pdb, image)
    public_addresses = {}
    for public in engine.cvdump_analysis.parser.publics:
        if image.is_valid_section(public.section):
            public_addresses[public.name] = image.get_abs_addr(
                public.section, public.offset
            )
    thunks = {}
    routes = []
    unresolved = []
    for address in addresses:
        function = functions.get(address)
        if function is None or function.get("stub") or not function.get("symbol"):
            continue
        module = modules.get_module(function.recomp_addr)
        if module is None:
            continue
        object_name = module[1].replace("\\", "/")
        if not object_name.startswith("CMakeFiles/LEMBALL.dir/"):
            continue
        try:
            obj = load_object(object_name)
            caller_symbol = obj.function(function.get("symbol"))[0]
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
        references, found = recover_references(
            engine,
            functions,
            entries,
            public_addresses,
            obj,
            function,
            route,
            previous_routes.get(address),
        )
        thunks.update(found)
        if not references:
            continue
        route["references"] = references
        try:
            obj.redirect([route], thunks)
        except (ValueError, KeyError) as error:
            unresolved.append({"caller": address, "reason": str(error)})
            continue
        routes.append(route)
    routes.sort(key=lambda caller_route: caller_route["caller"])
    return routes, thunks, unresolved


def write_recovery(target, thunks, routes, unresolved):
    """Save the manifest, matching annotations, and unresolved caller report."""
    manifest = {
        "version": 2,
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
        f"recovered {len(thunks)} thunks; {len(routes)} callers; "
        f"{sum(len(route['references']) for route in routes)} references; "
        f"{len(unresolved)} unresolved callers"
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
        target, [thunks[address] for address in sorted(used)], routes, unresolved
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
