#!/usr/bin/env python3
"""Recover or verify original jump entries and compiler relocation routes."""

import argparse
import hashlib
import json
import os
import subprocess
import sys
from dataclasses import replace

from reccmp.compare import Compare
from reccmp.compare.csv import csv_parse
from reccmp.dir import source_code_search
from reccmp.project.detect import RecCmpProject
from reccmp.tools.roadmap import ModuleMap
from reccmp.types import ImageId

from link_binary import CoffObject, DIR32, REL32, read_jump_target, thunk_symbol
from lib.project import BUILD, ROOT, TARGET_ID

MANIFEST = ROOT / "tools/data/linker-thunks.json"
ANNOTATIONS = ROOT / "src/Platform/MSVC/LinkerThunks.h"


def verify_thunks():
    manifest = json.loads(MANIFEST.read_text())
    target = RecCmpProject.from_directory(BUILD).get(TARGET_ID)
    engine = Compare.from_target(target)
    functions = {function.orig_addr: function for function in engine.get_functions()}
    errors = []
    verified_thunks = 0
    verified_references = 0
    if hashlib.sha256(target.original_path.read_bytes()).hexdigest() != manifest["original_sha256"]:
        raise ValueError("Original image differs from the recorded thunk evidence")
    for thunk in manifest["thunks"]:
        entry = functions.get(thunk["address"])
        body = functions.get(thunk["target"])
        comparison = engine.compare_address(thunk["address"])
        if (entry is None or body is None or comparison is None or comparison.is_stub
                or comparison.accuracy != 1
                or engine.orig_bin.read(thunk["address"], 5).hex() != thunk["original_bytes"]
                or read_jump_target(engine.orig_bin, thunk["address"]) != thunk["target"]
                or read_jump_target(engine.recomp_bin, entry.recomp_addr) != body.recomp_addr):
            errors.append({"thunk": thunk["address"], "reason": "Jump entry or destination mismatch"})
        else:
            verified_thunks += 1
    for route in manifest["routes"]:
        caller = functions.get(route["caller"])
        for reference in route["references"]:
            if caller is None:
                errors.append({"caller": route["caller"], "reason": "Caller is unmatched"})
                continue
            instruction = caller.recomp_addr + reference["offset"]
            original = engine.orig_bin.read(reference["original_instruction"], 5)
            rebuilt = engine.recomp_bin.read(instruction, 5)
            original_target = int.from_bytes(original[1:], "little")
            rebuilt_target = int.from_bytes(rebuilt[1:], "little")
            if original[0] in (0xE8, 0xE9):
                original_target = (reference["original_instruction"] + 5 + original_target) & 0xFFFFFFFF
                rebuilt_target = (instruction + 5 + rebuilt_target) & 0xFFFFFFFF
            entry = functions.get(reference["thunk"])
            if (entry is None or original.hex() != reference["original_bytes"]
                    or rebuilt[0] != original[0]
                    or original_target != reference["thunk"] or rebuilt_target != entry.recomp_addr):
                errors.append({"caller": route["caller"],
                               "reference": reference["original_instruction"],
                               "reason": "Caller does not reference the evidenced jump entry"})
            else:
                verified_references += 1
    result = {"original_sha256": manifest["original_sha256"],
              "rebuilt_sha256": hashlib.sha256(target.recompiled_path.read_bytes()).hexdigest(),
              "thunks_verified": verified_thunks, "references_verified": verified_references,
              "errors": errors}
    path = BUILD / "linker-thunks/verification.json"
    path.write_text(json.dumps(result, indent=2) + "\n")
    print(f"verified {verified_thunks} jump entries; {verified_references} references; "
          f"{len(errors)} errors; {path}")
    return int(bool(errors))

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    action = parser.add_mutually_exclusive_group()
    action.add_argument("--verify", action="store_true", help="Check the current executable against saved routes")
    action.add_argument(
        "--forwarder", action="append", default=[], type=lambda value: int(value, 16),
        help="Original five-byte tail-jump entry outside the linker table; repeatable",
    )
    args = parser.parse_args()
    if args.verify:
        return verify_thunks()
    forwarders = set(args.forwarder)
    previous = {"routes": [], "thunks": []}
    ranking = subprocess.check_output(
        [sys.executable, str(ROOT / "tools/triage_targets.py"), "--exact", "--limit", "0"],
        text=True,
    )
    addresses = [int(row.split()[0], 16) for row in ranking.splitlines()]
    if MANIFEST.exists():
        # Keep previously selected routes when their callers now match exactly.
        previous = json.loads(MANIFEST.read_text())
        forwarders.update(thunk["address"] for thunk in previous["thunks"]
                          if thunk.get("kind") == "tail-forwarder")
        addresses += [route["caller"] for route in previous["routes"]
                      if route["caller"] not in addresses]
    target = RecCmpProject.from_directory(BUILD).get(TARGET_ID)
    # Analyze a fresh native link of the unmodified compiler objects.
    output = BUILD / "linker-thunks"
    output.mkdir(exist_ok=True)
    build_rules = (BUILD / "CMakeFiles/LEMBALL.dir/build.make").read_text()
    command = next(line.strip().split() for line in build_rules.splitlines()
                   if line.startswith("\tpython ") and "tools/link_binary.py" in line)
    command[0] = sys.executable
    command = ["/OUT:linker-thunks/unrouted.EXE" if arg.upper().startswith("/OUT:")
               else arg for arg in command]
    command.append("/PDB:linker-thunks/unrouted.pdb")
    environment = os.environ.copy()
    environment["LIB"] = str(ROOT / "msvc400/lib")
    environment["INCLUDE"] = str(ROOT / "msvc400/include")
    environment["PATH"] = str(ROOT / "msvc400/bin") + ";" + environment["PATH"]
    linked = subprocess.run(command, cwd=BUILD, env=environment,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    (output / "unrouted-link.log").write_text(linked.stdout, encoding="utf-8")
    if linked.returncode:
        raise RuntimeError(linked.stdout)
    target = replace(target, recompiled_path=output / "unrouted.EXE",
                     recompiled_pdb=output / "unrouted.pdb")
    paths = tuple(path for path in source_code_search(target.source_paths)
                  if path.resolve() != ANNOTATIONS.resolve())
    engine = Compare.from_target(replace(target, source_paths=paths))
    table_entries = set()
    for region in engine.orig_bin.get_code_regions():
        for offset in range(0, len(region.data) - 4, 5):
            if region.data[offset] != 0xE9:
                break
            table_entries.add(region.addr + offset)
    modules = ModuleMap(target.recompiled_pdb, engine.recomp_bin)
    functions = {function.orig_addr: function for function in engine.get_functions()}
    catalog = dict(csv_parse((ROOT / "tools/data/original-function-sizes.csv").read_text()))
    for address in forwarders:
        original = catalog.get(address)
        destination = read_jump_target(engine.orig_bin, address)
        if (address in table_entries or address in functions or original is None
                or original["size"] != 5 or destination not in functions):
            raise ValueError(f"Expected an unpaired five-byte tail jump to a paired body: {address:#x}")
    entries = table_entries | forwarders
    previous_routes = {route["caller"]: route for route in previous["routes"]}
    objects = {}
    public_addresses = {}
    for public in engine.cvdump_analysis.parser.publics:
        if engine.recomp_bin.is_valid_section(public.section):
            public_addresses[public.name] = engine.recomp_bin.get_abs_addr(public.section, public.offset)
    thunks = {}
    routes = []
    unresolved = []
    strong_symbols = None
    for address in addresses:
        function = functions.get(address)
        if function is None or function.get("stub") or not function.get("symbol"):
            continue
        comparison = engine.function_comparator.compare_function(function)
        module = modules.get_module(function.recomp_addr)
        if module is None:
            continue
        object_name = module[1].replace("\\", "/")
        prefix = "linker-thunks/objects/"
        if object_name.startswith(prefix):
            object_name = object_name[len(prefix):]
        if not object_name.startswith("CMakeFiles/LEMBALL.dir/"):
            continue
        try:
            if object_name not in objects:
                objects[object_name] = CoffObject((BUILD / object_name).read_bytes())
            obj = objects[object_name]
            caller_symbol = obj.function(function.get("symbol"))[0]
            if public_addresses.get(caller_symbol) != function.recomp_addr:
                raise ValueError("COFF weak default does not match the linked caller address")
            size = function.size(ImageId.RECOMP)
            fingerprint = obj.fingerprint(caller_symbol, size)
        except (ValueError, OSError, TypeError) as error:
            unresolved.append({"caller": address, "reason": str(error)})
            continue
        references = []
        aligned = []
        old = previous_routes.get(address)
        if (old is not None and old["symbol"] == caller_symbol and old["size"] == size
                and old["fingerprint"] == fingerprint):
            # Retain evidenced positions only while the compiler body is unchanged.
            aligned.extend((ref["original_instruction"], function.recomp_addr + ref["offset"])
                           for ref in old["references"])
        for tag, first, last, rebuilt_first, rebuilt_last in comparison.diff.codes:
            if tag == "equal":
                aligned.extend((int(original[0], 16), int(rebuilt[0], 16))
                               for original, rebuilt in zip(comparison.diff.orig_inst[first:last],
                                   comparison.diff.recomp_inst[rebuilt_first:rebuilt_last], strict=True)
                               if original[0] and rebuilt[0])
            elif tag == "replace" and last - first == rebuilt_last - rebuilt_first == 1:
                # Address-taking remains distinct in comparison; reconstruct the real entry.
                original_addr = comparison.diff.orig_inst[first][0]
                rebuilt_addr = comparison.diff.recomp_inst[rebuilt_first][0]
                if original_addr and rebuilt_addr:
                    aligned.append((int(original_addr, 16), int(rebuilt_addr, 16)))
        seen_offsets = set()
        for original_addr, rebuilt_addr in aligned:
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
            entry = int.from_bytes(original[1:], "little")
            if relative:
                entry = (original_addr + 5 + entry) & 0xFFFFFFFF
            if entry in functions or entry not in entries:
                continue
            destination = read_jump_target(engine.orig_bin, entry)
            callee = functions.get(destination)
            if callee is None or not callee.get("symbol"):
                continue
            rebuilt_target = int.from_bytes(rebuilt[1:], "little")
            if relative:
                rebuilt_target = (rebuilt_addr + 5 + rebuilt_target) & 0xFFFFFFFF
            if rebuilt_target != callee.recomp_addr:
                if read_jump_target(engine.recomp_bin, rebuilt_target) != callee.recomp_addr:
                    continue
            try:
                source_symbol = obj.reference_target(caller_symbol, rebuilt_addr - function.recomp_addr, relocation)
                target_symbol = obj.canonical_name(source_symbol)
                if public_addresses.get(target_symbol) != callee.recomp_addr:
                    # A real vector destructor can override a local weak scalar default.
                    if strong_symbols is None:
                        strong_symbols = set()
                        for path in (BUILD / "CMakeFiles/LEMBALL.dir").rglob("*.obj"):
                            strong_symbols.update(symbol[0] for symbol in CoffObject(path.read_bytes()).symbols.values()
                                                  if symbol[2] > 0 and symbol[3] == 0x20 and symbol[4] == 2)
                    if (public_addresses.get(source_symbol) != callee.recomp_addr
                            or source_symbol not in strong_symbols):
                        raise ValueError("COFF call target does not match the linked callee address")
                    target_symbol = source_symbol
            except ValueError:
                continue
            reference = {"offset": offset, "original_instruction": original_addr,
                         "original_bytes": original.hex(), "thunk": entry,
                         "source_symbol": source_symbol}
            references.append(reference)
            seen_offsets.add(offset)
            thunks[entry] = {"address": entry, "target": destination,
                             "original_bytes": engine.orig_bin.read(entry, 5).hex(),
                             "symbol": thunk_symbol(entry),
                             "target_symbol": target_symbol}
            if entry in forwarders:
                thunks[entry]["kind"] = "tail-forwarder"
        if not references:
            continue
        route = {"caller": address, "object": object_name,
                 "symbol": caller_symbol, "size": size,
                 "fingerprint": fingerprint, "references": references}
        try:
            obj.redirect([route], thunks)
        except (ValueError, KeyError) as error:
            unresolved.append({"caller": address, "reason": str(error)})
            continue
        routes.append(route)
    used = {reference["thunk"] for route in routes for reference in route["references"]}
    if missing := forwarders - used:
        raise ValueError(f"No verified compiler references for selected forwarders: {sorted(missing)}")
    thunks = [thunks[address] for address in sorted(used)]
    routes.sort(key=lambda route: route["caller"])
    manifest = {"version": 2,
                "original_sha256": hashlib.sha256(target.original_path.read_bytes()).hexdigest(),
                "thunks": thunks, "routes": routes}
    MANIFEST.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8", newline="\n")
    lines = ["#ifndef LEMBALL_LINKER_THUNKS_H", "#define LEMBALL_LINKER_THUNKS_H", ""]
    for thunk in thunks:
        lines += [f"// SYNTHETIC: LEMBALL 0x{thunk['address']:08x} SYMBOL",
                  "// " + thunk["symbol"], ""]
    lines += ["#endif", ""]
    ANNOTATIONS.write_text("\n".join(lines), encoding="utf-8", newline="\n")
    output = BUILD / "linker-thunks"
    output.mkdir(exist_ok=True)
    (output / "unresolved.json").write_text(json.dumps(unresolved, indent=2) + "\n")
    print(f"recovered {len(thunks)} thunks; {len(routes)} callers; "
          f"{sum(len(route['references']) for route in routes)} references; "
          f"{len(unresolved)} unresolved callers")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
