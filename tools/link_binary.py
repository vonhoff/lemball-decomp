#!/usr/bin/env python3
"""MSVC 4.00 linker adapter for CMake."""

import ctypes
import hashlib
import json
import os
import re
import struct
import subprocess
import sys
from pathlib import Path

from reccmp.formats.exceptions import (
    InvalidVirtualAddressError,
    InvalidVirtualReadError,
)

MSVC_WARNING = re.compile(r"\bwarning\s+[A-Z]*\d+\s*:", re.IGNORECASE)


def read_jump_target(image, address: int) -> int | None:
    """Decode one complete IA-32 E9 with a bounded instruction fetch."""
    try:
        raw = image.read(address, 5)
    except (InvalidVirtualAddressError, InvalidVirtualReadError):
        return None
    if len(raw) != 5 or raw[0] != 0xE9:
        return None
    return (address + 5 + int.from_bytes(raw[1:], "little")) & 0xFFFFFFFF


I386 = 0x14C
REL32 = 0x14
DIR32 = 6
SYMBOL_SIZE = 18
SECTION_SIZE = 40


class CoffObject:
    """Read ordinary MSVC 4.00 objects; retain sections and debug records verbatim."""

    def __init__(self, data: bytes):
        self.data = data
        machine, count, _, self.symbol_start, self.symbol_count, optional, _ = (
            struct.unpack_from("<HHIIIHH", data)
        )
        if machine != I386 or optional:
            raise ValueError("Expected an ordinary i386 COFF object")
        self.sections = [
            struct.unpack_from("<8sIIIIIIHHI", data, 20 + i * SECTION_SIZE)
            for i in range(count)
        ]
        self.string_start = self.symbol_start + self.symbol_count * SYMBOL_SIZE
        string_size = struct.unpack_from("<I", data, self.string_start)[0]
        self.strings = data[self.string_start : self.string_start + string_size]
        self.symbols = {}
        self.weak_defaults = {}
        index = 0
        while index < self.symbol_count:
            name, value, section, kind, storage, aux = struct.unpack_from(
                "<8sIhHBB", data, self.symbol_start + index * SYMBOL_SIZE
            )
            if name[:4] == bytes(4):
                offset = struct.unpack_from("<I", name, 4)[0]
                name = self.strings[offset : self.strings.index(0, offset)]
            else:
                name = name.split(b"\0", 1)[0]
            self.symbols[index] = (name.decode("ascii"), value, section, kind, storage)
            if storage == 105 and aux == 1:
                target, search = struct.unpack_from(
                    "<II", data, self.symbol_start + (index + 1) * SYMBOL_SIZE
                )
                if search in (1, 2, 3):
                    self.weak_defaults[name.decode("ascii")] = target
            index += aux + 1

    def canonical_name(self, name):
        seen = set()
        while name in self.weak_defaults:
            if name in seen:
                raise ValueError("Cyclic COFF weak-external defaults")
            seen.add(name)
            name = self.symbols[self.weak_defaults[name]][0]
        return name

    def publish_static_functions(self):
        """Expose existing private function symbols; retain code and relocations."""
        data = bytearray(self.data)
        for index, (_, _, section, kind, storage) in self.symbols.items():
            if section > 0 and kind == 0x20 and storage == 3:
                data[self.symbol_start + index * SYMBOL_SIZE + 16] = 2
        return bytes(data)

    def definition(self, name):
        matches = [
            value
            for value in self.symbols.values()
            if value[0] == name and value[2] > 0
        ]
        if not matches and name in self.weak_defaults:
            return self.definition(self.canonical_name(name))
        if len(matches) != 1:
            raise ValueError(f"Missing or ambiguous COFF definition: {name}")
        return matches[0]

    def function(self, name):
        symbol = self.definition(name)
        if symbol[3] != 0x20:
            raise ValueError(f"Expected a COFF function: {name}")
        return symbol

    def reference_target(self, name, offset, relocation):
        _, value, section, _, _ = self.definition(name)
        matches = [
            index
            for _, (address, index, kind) in self.relocations(section)
            if address == value + offset and kind == relocation
        ]
        if len(matches) != 1:
            raise ValueError("Missing or ambiguous COFF relocation")
        return self.symbols[matches[0]][0]

    def relocations(self, section):
        descriptor = self.sections[section - 1]
        for i in range(descriptor[7]):
            offset = descriptor[5] + i * 10
            yield offset, struct.unpack_from("<IIH", self.data, offset)

    def fingerprint(self, name, size, *, include_symbols=False):
        _, value, section, _, _ = self.definition(name)
        descriptor = self.sections[section - 1]
        if size <= 0 or value + size > descriptor[3]:
            raise ValueError(f"Symbol extent exceeds COFF section: {name}")
        raw = bytearray(self.data[descriptor[4] + value : descriptor[4] + value + size])
        evidence = bytearray()
        for _, (address, index, kind) in self.relocations(section):
            if value <= address < value + size:
                if kind not in (REL32, DIR32) or address + 4 > value + size:
                    raise ValueError(f"Unsupported relocation: {kind:#x}")
                if include_symbols:
                    evidence += struct.pack("<II", address - value, kind)
                    evidence += raw[address - value : address - value + 4]
                    evidence += self.symbols[index][0].encode("ascii") + b"\0"
                    evidence += (
                        self.canonical_name(self.symbols[index][0]).encode("ascii")
                        + b"\0"
                    )
                raw[address - value : address - value + 4] = bytes(4)
        return hashlib.sha256(raw + evidence).hexdigest()

    def redirect(self, routes, thunks):
        """Change only evidenced relocation symbol indices; reject stale references."""
        data = bytearray(self.data)
        strings = bytearray(self.strings)
        symbols = bytearray()
        added = {}
        for route in routes:
            if self.fingerprint(route["symbol"], route["size"]) != route["fingerprint"]:
                raise ValueError(f"Stale linker-thunk routes: {route['symbol']}")
            _, value, section, _, _ = self.definition(route["symbol"])
            descriptor = self.sections[section - 1]
            for ref in route["references"]:
                thunk = thunks[ref["thunk"]]
                original = bytes.fromhex(ref["original_bytes"])
                pointer = route.get("kind") == "data"
                length = 4 if pointer else 5
                if len(original) != length or not (
                    0 <= ref["offset"] <= route["size"] - length
                ):
                    raise ValueError("Reference exceeds its recorded symbol extent")
                instruction = value + ref["offset"]
                operand = instruction if pointer else instruction + 1
                matches = [
                    (location, index, kind)
                    for location, (address, index, kind) in self.relocations(section)
                    if address == operand
                ]
                if len(matches) != 1:
                    raise ValueError("Missing or ambiguous COFF relocation")
                location, index, kind = matches[0]
                raw_offset = descriptor[4] + instruction
                if pointer:
                    allowed = (
                        kind == DIR32
                        and int.from_bytes(original, "little") == ref["thunk"]
                    )
                else:
                    opcode = original[0]
                    allowed = data[raw_offset] == opcode and (
                        (kind == REL32 and opcode in (0xE8, 0xE9))
                        or (
                            kind == DIR32 and (opcode == 0x68 or 0xB8 <= opcode <= 0xBF)
                        )
                    )
                target_symbols = {thunk["target_symbol"]} | {
                    alias["symbol"] for alias in thunk.get("folded_aliases", [])
                }
                if (
                    not allowed
                    or data[descriptor[4] + operand : descriptor[4] + operand + 4]
                    != bytes(4)
                    or self.symbols[index][0] != ref["source_symbol"]
                    or not target_symbols.intersection(
                        (
                            ref["source_symbol"],
                            self.canonical_name(ref["source_symbol"]),
                        )
                    )
                ):
                    raise ValueError(
                        f"Unverified linker-thunk reference: {route['symbol']}"
                    )
                symbol = thunk["symbol"]
                if symbol not in added:
                    added[symbol] = self.symbol_count + len(added)
                    name = struct.pack("<II", 0, len(strings))
                    strings += symbol.encode("ascii") + b"\0"
                    symbols += struct.pack("<8sIhHBB", name, 0, 0, 0x20, 2, 0)
                struct.pack_into("<I", data, location + 4, added[symbol])
        struct.pack_into("<I", strings, 0, len(strings))
        struct.pack_into("<I", data, 12, self.symbol_count + len(added))
        tail = data[self.string_start + len(self.strings) :]
        return bytes(data[: self.string_start] + symbols + strings + tail)


def thunk_symbol(address):
    return f"__lemball_jump_{address:08x}"


def prepare_runtime(linker, build):
    """Link CRT members with their private function symbols visible to the PDB."""
    manager = linker.parent / "LIB.EXE"
    library = linker.parent.parent / "lib/LIBCMT.LIB"
    members = (
        "aw_map",
        "crt0dat",
        "fflush",
        "ismbbyte",
        "mbctype",
        "output",
        "stdargv",
        "winsig",
        "winxfltr",
    )
    output = build / "build/intel/mt_obj"
    output.mkdir(parents=True, exist_ok=True)
    objects = []
    for member in members:
        name = Path("build/intel/mt_obj") / (member + ".obj")
        path = build / name
        subprocess.run(
            [
                str(manager),
                "/nologo",
                "/extract:" + str(name),
                "/out:" + str(path),
                str(library),
            ],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            check=True,
        )
        obj = CoffObject(path.read_bytes())
        path.write_bytes(obj.publish_static_functions())
        objects.append(str(name))
    runtime = build / "runtime"
    runtime.mkdir(exist_ok=True)
    subprocess.run(
        [
            str(manager),
            "/nologo",
            "/out:" + str(runtime / "LIBCMT.LIB"),
            *["/remove:" + name for name in objects],
            str(library),
            *objects,
        ],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        check=True,
    )
    return runtime


def make_thunk_object(thunks):
    """One five-byte E9 section per entry; target addresses remain linker relocations."""
    strings = bytearray(bytes(4))
    symbols = bytearray()

    def symbol(name, section):
        name_field = struct.pack("<II", 0, len(strings))
        strings.extend(name.encode("ascii") + b"\0")
        index = len(symbols) // SYMBOL_SIZE
        symbols.extend(struct.pack("<8sIhHBB", name_field, 0, section, 0x20, 2, 0))
        return index

    sections = bytearray()
    contents = bytearray()
    start = 20 + len(thunks) * SECTION_SIZE
    targets = {}
    for i, thunk in enumerate(thunks):
        symbol(thunk["symbol"], i + 1)
        target = thunk["target_symbol"]
        if target not in targets:
            targets[target] = symbol(target, 0)
        raw = start + len(contents)
        contents += b"\xe9" + bytes(4)
        relocation = start + len(contents)
        contents += struct.pack("<IIH", 1, targets[target], REL32)
        sections += struct.pack(
            "<8sIIIIIIHHI", b".text$lt", 0, 0, 5, raw, relocation, 0, 1, 0, 0x60100020
        )
    struct.pack_into("<I", strings, 0, len(strings))
    header = struct.pack(
        "<HHIIIHH",
        I386,
        len(thunks),
        0,
        start + len(contents),
        len(symbols) // SYMBOL_SIZE,
        0,
        0,
    )
    return bytes(header + sections + contents + symbols + strings)


def prepare_link(objects, build, manifest_path):
    """Produce overlays; never mutate the compiler's object files."""
    manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    if manifest["version"] != 3:
        raise ValueError("Unsupported linker-thunk manifest version")
    thunks = {thunk["address"]: thunk for thunk in manifest["thunks"]}
    for thunk in thunks.values():
        for alias in thunk.get("folded_aliases", []):
            obj = CoffObject((build / alias["object"]).read_bytes())
            if (
                obj.fingerprint(alias["symbol"], alias["size"], include_symbols=True)
                != alias["fingerprint"]
            ):
                raise ValueError(f"Stale folded-alias evidence: {alias['symbol']}")
    root = manifest_path.parents[2]
    original = root / "data/LEMBALL.EXE"
    if hashlib.sha256(original.read_bytes()).hexdigest() != manifest["original_sha256"]:
        raise ValueError("Linker-thunk evidence belongs to a different original image")
    output = build / "linker-thunks"
    output.mkdir(parents=True, exist_ok=True)
    grouped = {}
    for route in manifest["routes"]:
        grouped.setdefault(route["object"].replace("\\", "/").casefold(), []).append(
            route
        )
    linked = []
    applied = 0
    skipped = []
    for name in objects:
        key = name.replace("\\", "/").casefold()
        routes = grouped.get(key, [])
        if not routes:
            linked.append(name)
            continue
        path = build / name
        obj = CoffObject(path.read_bytes())
        current = []
        for route in routes:
            try:
                valid = (
                    obj.fingerprint(route["symbol"], route["size"])
                    == route["fingerprint"]
                )
            except ValueError:
                valid = False
            if valid:
                current.append(route)
            else:
                skipped.append(route["symbol"])
        if current:
            overlay = output / "objects" / name
            overlay.parent.mkdir(parents=True, exist_ok=True)
            overlay.write_bytes(obj.redirect(current, thunks))
            linked.append(str(overlay.relative_to(build)))
            applied += sum(len(route["references"]) for route in current)
        else:
            linked.append(name)
    # A separate final contribution gives LINK's PDB an exact five-byte extent.
    groups = (
        ("entries.obj", manifest["thunks"][:-1]),
        ("terminal-entry.obj", manifest["thunks"][-1:]),
    )
    for name, thunks in groups:
        if thunks:
            thunk_object = output / name
            thunk_object.write_bytes(make_thunk_object(thunks))
            linked.append(str(thunk_object.relative_to(build)))
    (output / "applied.json").write_text(
        json.dumps({"references": applied, "skipped": skipped}, indent=2) + "\n"
    )
    print(
        f"linker thunks: {len(manifest['thunks'])} entries; {applied} references; "
        f"{len(skipped)} stale symbols"
    )
    return linked


def win_short_path(path: str) -> str:
    absp = os.path.abspath(path)
    get_short = ctypes.WinDLL("kernel32")["GetShortPathNameW"]
    get_short.argtypes = [ctypes.c_wchar_p, ctypes.c_wchar_p, ctypes.c_uint]
    get_short.restype = ctypes.c_uint
    buf = ctypes.create_unicode_buffer(32768)
    if get_short(absp, buf, 32768) and buf.value:
        return buf.value
    return absp


def main(args: list[str]) -> int:
    linker, link_args = win_short_path(args[0]), args[1:]
    # LINK 4.00 requires short working-directory and toolchain paths.
    os.chdir(win_short_path(os.getcwd()))
    for env_var in ("LIB", "INCLUDE", "PATH"):
        val = os.environ.get(env_var, "")
        if val:
            os.environ[env_var] = ";".join(
                win_short_path(p) for p in val.split(";") if p
            )

    for arg in link_args:
        if arg.startswith("@"):
            rsp_path = Path(arg[1:])
            content = rsp_path.read_text(encoding="utf-8")
            # LINK 4.00 limits lines to 16383 characters; multiple response files crash it.
            rsp_path.write_text("\n".join(content.split()) + "\n", encoding="utf-8")

    runtime = prepare_runtime(Path(linker), Path.cwd())
    os.environ["LIB"] = win_short_path(str(runtime)) + ";" + os.environ.get("LIB", "")

    manifest = Path(__file__).resolve().parent / "data/linker-thunks.json"
    output = next((arg[5:] for arg in link_args if arg.upper().startswith("/OUT:")), "")
    if manifest.exists() and Path(output).name.upper() == "LEMBALL.EXE":
        for i, arg in enumerate(link_args):
            if arg.startswith("@"):
                objects = Path(arg[1:]).read_text(encoding="utf-8").split()
                linked = prepare_link(objects, Path.cwd(), manifest)
                response = Path("linker-thunks/link.rsp")
                response.write_text("\n".join(linked) + "\n", encoding="utf-8")
                link_args[i] = "@" + str(response)
                break

    res = subprocess.run(
        [linker, *link_args],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        errors="replace",
        check=False,
    )
    output = res.stdout
    sys.stdout.write(output)
    if MSVC_WARNING.search(output):
        sys.stderr.write("linker emitted warnings\n")
        return res.returncode or 1
    return res.returncode


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
