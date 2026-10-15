#!/usr/bin/env python3
"""Check real MOG directory entries against the original and rebuilt x86 parser."""

import argparse
import hashlib
import json
import struct
from collections import Counter
from dataclasses import dataclass
from pathlib import Path

from reccmp.types import ImageId
from unicorn import UC_HOOK_CODE, UcError
from unicorn.x86_const import (
    UC_X86_REG_EAX,
    UC_X86_REG_ECX,
    UC_X86_REG_EDX,
    UC_X86_REG_EIP,
    UC_X86_REG_ESP,
)

from lib import BUILD, ROOT
from lib.x86_fixture import (
    HEAP,
    HEAP_SIZE,
    STACK,
    STACK_SIZE,
    X86Fixture,
    put,
    reference_build,
)

DIRECTORY_HEADER = struct.Struct("<5I")
DIRECTORY_ENTRY = struct.Struct("<5I16s")
DIRECTORY_KIND = 0x44495243
GET_CHUNK_INFO = 0x0045BFA0
INPUT_DWORD = 0x0045BC40
VS_READ = 0x00462F30
VS_SEEK = 0x00462F80
FREAD = 0x004804E0
FSEEK = 0x00480BD0
FTELL = 0x004809F0
FILE_GLOBAL = 0x004A1D50
OWNER = HEAP + 0x200
FILE = HEAP + 0x800
NODE = HEAP + 0x1000
DATA = HEAP + 0x2000
NODE_SIZE = 56


@dataclass(frozen=True)
class Directory:
    offset: int
    payload_start: int
    table_offset: int
    count: int
    end: int


@dataclass(frozen=True)
class Entry:
    directory: Directory
    index: int
    data_offset: int
    identifier: int
    kind: int
    file_offset: int
    size: int
    name: bytes

    @property
    def offset(self):
        return self.directory.table_offset + self.index * DIRECTORY_ENTRY.size


def read_archive(data):
    """Read fixed-width records; reject inputs outside these valid-file fixtures."""
    directories = {}
    entries = []

    def read_directory(offset, parent_end):
        if (
            offset in directories
            or not 0 <= offset <= len(data) - DIRECTORY_HEADER.size
        ):
            raise ValueError(f"Repeated or out-of-range directory: {offset:#x}")
        kind, size, count, version, table = DIRECTORY_HEADER.unpack_from(data, offset)
        start = offset + DIRECTORY_HEADER.size
        end = offset + 8 + size
        if kind != DIRECTORY_KIND or version != 3:
            raise ValueError(f"Unsupported directory header at {offset:#x}")
        if (
            not start
            <= table
            <= table + count * DIRECTORY_ENTRY.size
            <= end
            <= parent_end
        ):
            raise ValueError(f"Directory extents outside their container: {offset:#x}")
        if table - start > HEAP + HEAP_SIZE - DATA:
            raise ValueError(f"Directory metadata exceeds fixture memory: {offset:#x}")
        directory = Directory(offset, start, table, count, end)
        directories[offset] = directory
        children = []
        for index in range(count):
            entry = Entry(
                directory,
                index,
                *DIRECTORY_ENTRY.unpack_from(
                    data, table + index * DIRECTORY_ENTRY.size
                ),
            )
            if not start <= entry.data_offset < table:
                raise ValueError(
                    f"Metadata pointer outside directory data: {entry.offset:#x}"
                )
            if not entry.file_offset <= entry.file_offset + entry.size <= len(data):
                raise ValueError(f"Payload outside archive: {entry.offset:#x}")
            entries.append(entry)
            if entry.kind == DIRECTORY_KIND:
                children.append(entry)
        for entry in children:
            child = read_directory(entry.file_offset, end)
            if entry.size and child.end - child.offset != entry.size:
                raise ValueError(f"Child directory size disagrees: {entry.offset:#x}")
        return directory

    read_directory(0, len(data))
    if not entries:
        raise ValueError("Archive has no entries to exercise")
    return list(directories.values()), entries


def fixture(entry, index, archive):
    pattern = bytes((index * 17 + byte * 13) & 255 for byte in range(256))
    memory = bytearray(pattern * (HEAP_SIZE // 256))
    directory = entry.directory
    for offset, value in (
        (0x10, directory.table_offset),
        (0x14, directory.payload_start),
        (0x20, entry.index - 1),
        (0x30, DATA),
    ):
        put(memory, OWNER + offset, value)
    metadata = archive[directory.payload_start : directory.table_offset]
    memory[DATA - HEAP : DATA - HEAP + len(metadata)] = metadata
    expected = bytearray(memory)
    for offset, value in (
        (0, DATA + entry.data_offset - directory.payload_start),
        (4, entry.kind),
        (8, entry.identifier),
        (12, entry.file_offset),
        (16, entry.size),
        (0x18, 0),
        (0x20, 0),
        (0x24, 0),
    ):
        put(expected, NODE + offset, value)
    expected[NODE - HEAP + 0x28 : NODE - HEAP + NODE_SIZE] = entry.name
    return memory, bytes(expected)


class ArchiveMachine(X86Fixture):
    """Execute game parsing/file wrappers; model only CRT file operations."""

    def __init__(self, image, entities, image_id, archive):
        super().__init__(image)
        self.addresses = {
            address: entities[address].addr(image_id)
            for address in (
                GET_CHUNK_INFO,
                VS_READ,
                VS_SEEK,
                FREAD,
                FSEEK,
                FTELL,
                FILE_GLOBAL,
            )
        }
        self.archive = archive
        self.events = []
        self.io = []
        self.cursor = 0
        self.machine.hook_add(UC_HOOK_CODE, self.file_call)

    def arguments(self, count):
        address = self.machine.reg_read(UC_X86_REG_ESP) + 4
        return struct.unpack(
            "<" + "I" * count, self.machine.mem_read(address, count * 4)
        )

    def destination(self, address, size):
        if NODE <= address <= NODE + NODE_SIZE - size:
            return ["node", address - NODE]
        if STACK <= address <= STACK + STACK_SIZE - size:
            return ["stack"]
        raise ValueError(f"Unexpected file-read destination: {address:#x}, {size}")

    def return_from_crt(self, value):
        machine = self.machine
        stack = machine.reg_read(UC_X86_REG_ESP)
        target = int.from_bytes(machine.mem_read(stack, 4), "little")
        machine.reg_write(UC_X86_REG_EAX, value & 0xFFFFFFFF)
        machine.reg_write(UC_X86_REG_ECX, 0xCC117755)
        machine.reg_write(UC_X86_REG_EDX, 0xDD226644)
        machine.reg_write(UC_X86_REG_ESP, stack + 4)
        machine.reg_write(UC_X86_REG_EIP, target)

    def file_call(self, _machine, address, _size, _user_data):
        addresses = self.addresses
        if address in (addresses[VS_SEEK], addresses[FSEEK]):
            file, offset, origin = self.arguments(3)
            if file != FILE or origin != 0 or not 0 <= offset < len(self.archive):
                raise ValueError("Unexpected file seek")
            if address == addresses[VS_SEEK]:
                self.events.append(["vsSeek", offset, origin])
                self.io.append(["seek", offset, origin])
            else:
                self.cursor = offset
                self.events.append(["fseek", offset, origin])
                self.return_from_crt(0)
        elif address == addresses[FTELL]:
            if self.arguments(1) != (FILE,):
                raise ValueError("Unexpected file handle")
            self.events.append(["ftell", self.cursor])
            self.return_from_crt(self.cursor)
        elif address == addresses[VS_READ]:
            file, buffer, count = self.arguments(3)
            if file != FILE:
                raise ValueError("Unexpected file handle")
            self.events.append(["vsRead", self.destination(buffer, count), count])
            self.io.append(["read", count])
        elif address == addresses[FREAD]:
            buffer, size, count, file = self.arguments(4)
            if file != FILE or size != 1 or count not in (4, 16):
                raise ValueError("Unexpected fread arguments")
            blob = self.archive[self.cursor : self.cursor + count]
            if len(blob) != count:
                raise ValueError("Short reads are outside these valid-file fixtures")
            self.events.append(
                [
                    "fread",
                    self.cursor,
                    self.destination(buffer, count),
                    size,
                    count,
                    blob.hex(),
                ]
            )
            self.machine.mem_write(buffer, blob)
            self.cursor += count
            self.return_from_crt(count)

    def record_write(self, machine, access, address, size, value, user_data):
        super().record_write(machine, access, address, size, value, user_data)
        if HEAP <= address < HEAP + HEAP_SIZE:
            if not NODE <= address <= NODE + NODE_SIZE - size:
                raise ValueError(
                    f"Unexpected write outside the result node: {address:#x}"
                )
            self.events.append(
                ["store", address - NODE, size, value & ((1 << (size * 8)) - 1)]
            )

    def parse(self, memory):
        self.events = []
        self.io = []
        self.cursor = 0
        return self.run(
            self.addresses[GET_CHUNK_INFO],
            memory,
            OWNER,
            (NODE,),
            ((self.addresses[FILE_GLOBAL], FILE),),
        )


def check(engine, archive):
    directories, entries = read_archive(archive)
    entities = {entity.orig_addr: entity for entity in engine.get_all()}
    machines = [
        ArchiveMachine(image, entities, image_id, archive)
        for image, image_id in (
            (engine.orig_bin, ImageId.ORIG),
            (engine.recomp_bin, ImageId.RECOMP),
        )
    ]
    digest = hashlib.sha256()
    event_count = 0
    for index, entry in enumerate(entries):
        memory, expected = fixture(entry, index, archive)
        wanted_io = [["seek", entry.offset, 0]] + [["read", 4]] * 5 + [["read", 16]]
        try:
            results = [machine.parse(memory) for machine in machines]
            for machine, result in zip(machines, results, strict=True):
                if result.heap != expected:
                    raise ValueError("Decoded node or untouched heap bytes differ")
                if (
                    machine.io != wanted_io
                    or machine.cursor != entry.offset + DIRECTORY_ENTRY.size
                ):
                    raise ValueError("Seek/read boundaries differ")
            if machines[0].events != machines[1].events:
                raise ValueError("Ordered original/rebuilt file and heap events differ")
        except (UcError, ValueError) as error:
            raise ValueError(f"Entry {entry.offset:#x}: {error}") from error
        digest.update(results[0].heap)
        digest.update(json.dumps(machines[0].events, separators=(",", ":")).encode())
        event_count += len(machines[0].events)
    functions = []
    for address in (GET_CHUNK_INFO, INPUT_DWORD, VS_READ, VS_SEEK):
        entity = entities[address]
        instructions = machines[0].instruction_addresses(
            address, entity.size(ImageId.ORIG)
        )
        if instructions - machines[0].visited:
            raise ValueError(f"Original instructions unexercised in {address:#x}")
        functions.append(
            {
                "original": f"0x{address:08x}",
                "rebuilt": f"0x{entity.recomp_addr:08x}",
                "original_instructions_exercised": len(instructions),
            }
        )
    return {
        "archive_sha256": hashlib.sha256(archive).hexdigest(),
        "archive_bytes": len(archive),
        "directories": len(directories),
        "entries": len(entries),
        "native_executions": len(entries) * 2,
        "kind_counts": dict(
            sorted(
                Counter(
                    entry.kind.to_bytes(4, "big").decode("latin1") for entry in entries
                ).items()
            )
        ),
        "zero_sized_directory_entries": sum(
            entry.kind == DIRECTORY_KIND and entry.size == 0 for entry in entries
        ),
        "nonterminated_fixed_names": sum(b"\0" not in entry.name for entry in entries),
        "serialized_entry_bytes": DIRECTORY_ENTRY.size,
        "runtime_node_bytes": NODE_SIZE,
        "heap_bytes_checked_per_execution": HEAP_SIZE,
        "ordered_events_per_image": event_count,
        "digest": digest.hexdigest(),
        "functions": functions,
        "checks": "All input entries; complete heaps, preserved uninitialized index fields, ordered file/heap events, scalar widths, preserved registers and RET 4.",
        "limits": "Game parser/InputDword/vsRead/vsSeek execute; CRT fread/fseek/ftell modeled from archive bytes. Disjoint fixture objects. No short-read, allocator, recursive constructor, resource payload decoder or full-game equivalence claim. Input guards constrain fixtures; they are not original validation behavior.",
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--archive",
        type=Path,
        default=ROOT / "data/PBAIMOG.VSR",
        help="MOG archive (default: data/PBAIMOG.VSR)",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=BUILD / "archive-entry-check.json",
        help="Evidence JSON (default: build-msvc400/archive-entry-check.json)",
    )
    args = parser.parse_args()
    engine, provenance = reference_build()
    result = check(engine, args.archive.read_bytes())
    result.update(provenance)
    args.output.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(
        f"Archive entries: {result['entries']} fixtures, {result['native_executions']} x86 executions; pass."
    )
    print(f"Evidence: {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
