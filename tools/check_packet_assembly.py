#!/usr/bin/env python3
"""Execute original and rebuilt packet assembly against deterministic fixtures."""

import argparse
import hashlib
import json
import random
import struct
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from reccmp.formats.pe import PEImage
from reccmp.project.detect import RecCmpProject
from reccmp.types import ImageId
from unicorn import (
    Uc,
    UcError,
    UC_ARCH_X86,
    UC_MODE_32,
    UC_HOOK_CODE,
    UC_HOOK_MEM_WRITE,
)
from unicorn.x86_const import (
    UC_X86_REG_EAX,
    UC_X86_REG_EBX,
    UC_X86_REG_ECX,
    UC_X86_REG_EDX,
    UC_X86_REG_ESI,
    UC_X86_REG_EDI,
    UC_X86_REG_EBP,
    UC_X86_REG_ESP,
    UC_X86_REG_EIP,
    UC_X86_REG_EFLAGS,
)

from lib import BUILD, TARGET_ID
from lib.compare import Compare

FILL_PACKET = 0x00461470
PACKET_SCRATCH = 0x004A1E28
PACKET_SIZE = 0x004A1E2C
HEADER_SIZE = 16
CASE_COUNT = 2048
HEAP = 0x01000000
HEAP_SIZE = 0x10000
OBJECT = HEAP + 0x1000
SOURCE = HEAP + 0x2000
STACK = 0x02000000
STACK_SIZE = 0x10000
STACK_POINTER = STACK + 0x8000
STOP = 0x03000000
PRESERVED_REGISTERS = (UC_X86_REG_EBX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP)
PRESERVED_VALUES = (0x11223344, 0x55667788, 0x99AABBCC, 0x13579BDF)


def dword(value):
    return struct.pack("<I", value & 0xFFFFFFFF)


def load(memory, address, size=4):
    offset = address - HEAP
    if not 0 <= offset <= len(memory) - size:
        raise ValueError(f"Fixture read outside heap: {address:#x}, size {size}")
    return int.from_bytes(memory[offset : offset + size], "little")


def put(memory, address, value):
    struct.pack_into("<I", memory, address - HEAP, value & 0xFFFFFFFF)


class PacketMachine:
    """Run a real x86 body; record heap writes and reject other side effects."""

    def __init__(self, image, entry, size, scratch, packet_size):
        if not isinstance(image, PEImage):
            raise ValueError("Packet fixtures require Windows x86 PE images")
        self.entry = entry
        self.scratch = scratch
        self.packet_size = packet_size
        self.machine = Uc(UC_ARCH_X86, UC_MODE_32)
        self.writes = []
        self.visited = set()
        machine = self.machine
        image_size = (image.optional_header.size_of_image + 4095) & ~4095
        machine.mem_map(image.imagebase, image_size)
        for section in image.sections:
            machine.mem_write(section.virtual_address, bytes(section.view))
        machine.mem_map(HEAP, HEAP_SIZE)
        machine.mem_map(STACK, STACK_SIZE)
        machine.mem_map(STOP, 4096)
        machine.hook_add(UC_HOOK_MEM_WRITE, self.record_write)
        machine.hook_add(UC_HOOK_CODE, self.record_instruction)
        self.context = machine.context_save()
        self.instructions = {
            instruction.address
            for instruction in Cs(CS_ARCH_X86, CS_MODE_32).disasm(
                image.read(entry, size), entry
            )
            if instruction.mnemonic != "int3"
        }

    def record_instruction(self, _machine, address, _size, _user_data):
        self.visited.add(address)

    def record_write(self, _machine, _access, address, size, value, _user_data):
        if HEAP <= address <= HEAP + HEAP_SIZE - size:
            self.writes.append((address, size, value & ((1 << (8 * size)) - 1)))
        elif not STACK <= address <= STACK + STACK_SIZE - size:
            raise ValueError(
                f"Unexpected write outside fixture heap/stack: {address:#x}"
            )

    def run(self, memory, packet_size):
        machine = self.machine
        machine.context_restore(self.context)
        machine.mem_write(HEAP, bytes(memory))
        machine.mem_write(STACK, bytes(STACK_SIZE))
        machine.mem_write(STACK_POINTER, dword(STOP))
        machine.mem_write(self.scratch, dword(SOURCE))
        machine.mem_write(self.packet_size, dword(packet_size))
        self.writes = []
        for register, value in zip(PRESERVED_REGISTERS, PRESERVED_VALUES, strict=True):
            machine.reg_write(register, value)
        for register, value in (
            (UC_X86_REG_EAX, 0xABCDEF01),
            (UC_X86_REG_ECX, OBJECT),
            (UC_X86_REG_EDX, 0x23456789),
            (UC_X86_REG_ESP, STACK_POINTER),
            (UC_X86_REG_EFLAGS, 0x202),
        ):
            machine.reg_write(register, value)
        machine.emu_start(self.entry, STOP, count=10000)
        if machine.reg_read(UC_X86_REG_EIP) != STOP:
            raise ValueError("Packet assembly did not return within 10000 instructions")
        if machine.reg_read(UC_X86_REG_ESP) != STACK_POINTER + 4:
            raise ValueError("Packet assembly changed the no-argument RET contract")
        if tuple(machine.reg_read(r) for r in PRESERVED_REGISTERS) != PRESERVED_VALUES:
            raise ValueError("Packet assembly clobbered a preserved register")
        return bytes(machine.mem_read(HEAP, HEAP_SIZE)), self.writes


def expected_assembly(memory, packet_size, *, early_sequence=False):
    """Model native forward DWORD/byte copies, not portable memcpy overlap rules."""
    memory = bytearray(memory)
    writes = []
    sequence = load(memory, SOURCE + 12, 2)
    received = load(memory, OBJECT + 16)

    def store(address, size, value):
        offset = address - HEAP
        if not 0 <= offset <= len(memory) - size:
            raise ValueError(f"Fixture write outside heap: {address:#x}, size {size}")
        memory[offset : offset + size] = value.to_bytes(size, "little")
        writes.append((address, size, value))

    def copy(destination, source, size):
        for offset in range(0, size // 4 * 4, 4):
            store(destination + offset, 4, load(memory, source + offset))
        for offset in range(size // 4 * 4, size):
            store(destination + offset, 1, load(memory, source + offset, 1))

    def add(address, value):
        store(address, 4, (load(memory, address) + value) & 0xFFFFFFFF)

    if sequence == 0:
        if received == 0:
            copy(load(memory, OBJECT + 4), SOURCE, packet_size)
            add(OBJECT + 20, packet_size)
        else:
            copy(
                load(memory, OBJECT + 4) + HEADER_SIZE,
                SOURCE + HEADER_SIZE,
                packet_size - HEADER_SIZE,
            )
            add(OBJECT + 20, packet_size - HEADER_SIZE)
    else:
        if received == 0:
            copy(load(memory, OBJECT + 4), SOURCE, HEADER_SIZE)
            add(OBJECT + 20, HEADER_SIZE)
        current_sequence = sequence if early_sequence else load(memory, SOURCE + 12, 2)
        destination = (
            load(memory, OBJECT + 4)
            + HEADER_SIZE
            + (load(memory, OBJECT + 8) * current_sequence & 0xFFFFFFFF)
        ) & 0xFFFFFFFF
        copy(destination, SOURCE + HEADER_SIZE, packet_size - HEADER_SIZE)
        add(OBJECT + 20, packet_size - HEADER_SIZE)
    add(OBJECT + 16, 1)
    return bytes(memory), writes


def fixtures():
    """Four branches plus bounded copies, overlap, and 32-bit arithmetic edges."""
    random_source = random.Random(FILL_PACKET)
    nearby_offsets = (-32, -16, -12, -8, -4, -3, -2, -1, 0, 1, 2, 3, 4, 8, 12, 16, 32)
    for case in range(CASE_COUNT):
        memory = bytearray(random_source.randbytes(HEAP_SIZE))
        branch = case % 4
        sequence = 0 if branch < 2 else random_source.randrange(1, 32)
        received = (
            0
            if branch in (0, 2)
            else (1, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF)[case // 4 % 4]
        )
        size = random_source.randrange(16, 144)
        payload_size = random_source.randrange(1, 17)
        destination = HEAP + 0x8000
        if case % 5 == 0:
            destination = SOURCE + nearby_offsets[case // 5 % len(nearby_offsets)]
            payload_size = 0
        elif case % 7 == 0 and branch >= 2:
            sequence = (2, 4, 16, 256, 32768)[case // 7 % 5]
            payload_size = 0x100000000 // sequence + random_source.randrange(
                0, max(1, 128 // sequence)
            )
        if branch == 2 and case % 17 == 0:
            destination = SOURCE + 4
            payload_size = 16
            sequence = 3
            for offset in (0, 4, 8):
                put(memory, SOURCE + offset, 0x00010001)
        if branch == 0 and case % 11 == 0:
            size = case % 16
        put(memory, OBJECT + 4, destination)
        put(memory, OBJECT + 8, payload_size)
        put(memory, OBJECT + 16, received)
        put(
            memory,
            OBJECT + 20,
            (0, 0xFFFFFFF0, 0xFFFFFFFF, random_source.getrandbits(32))[case // 4 % 4],
        )
        struct.pack_into("<H", memory, SOURCE + 12 - HEAP, sequence)
        yield case, branch, memory, size


def check(engine):
    entities = {entity.orig_addr: entity for entity in engine.get_all()}
    function = entities[FILL_PACKET]
    machines = [
        PacketMachine(
            image,
            function.addr(image_id),
            function.size(image_id),
            entities[PACKET_SCRATCH].addr(image_id),
            entities[PACKET_SIZE].addr(image_id),
        )
        for image, image_id in (
            (engine.orig_bin, ImageId.ORIG),
            (engine.recomp_bin, ImageId.RECOMP),
        )
    ]
    digest = hashlib.sha256()
    branches = [0, 0, 0, 0]
    nearby = wraps = changed_sequences = snapshot_disagreements = 0
    for case, branch, memory, size in fixtures():
        branches[branch] += 1
        sequence = load(memory, SOURCE + 12, 2)
        nearby += abs(load(memory, OBJECT + 4) - SOURCE) < 4096
        wraps += load(memory, OBJECT + 8) * sequence > 0xFFFFFFFF
        try:
            expected = expected_assembly(memory, size)
            actual = [machine.run(memory, size) for machine in machines]
            if actual[0] != expected:
                raise ValueError("Original differs from the native-copy model")
            if actual[1] != actual[0]:
                raise ValueError("Rebuilt differs from original heap or ordered writes")
            if branch == 2 and case % 17 == 0:
                snapshot_disagreements += (
                    expected_assembly(memory, size, early_sequence=True) != expected
                )
        except (UcError, ValueError) as error:
            raise ValueError(f"Case {case}, branch {branch}: {error}") from error
        changed_sequences += bool(
            sequence and branch == 2 and load(expected[0], SOURCE + 12, 2) != sequence
        )
        digest.update(actual[0][0])
        digest.update(repr(actual[0][1]).encode())
    if not snapshot_disagreements:
        raise ValueError(
            "Sequence-reread negative control did not distinguish early capture"
        )
    missing = machines[0].instructions - machines[0].visited
    if missing:
        raise ValueError(f"Original instructions not exercised: {sorted(missing)}")
    return {
        "cases": CASE_COUNT,
        "native_executions": CASE_COUNT * 2,
        "branches": branches,
        "nearby_or_overlapping_buffer_fixtures": nearby,
        "wrapping_product_fixtures": wraps,
        "sequence_changed_during_execution": changed_sequences,
        "early_sequence_snapshot_control_disagreements": snapshot_disagreements,
        "original_instructions_exercised": len(machines[0].instructions),
        "digest": digest.hexdigest(),
        "entries": [f"0x{machine.entry:08x}" for machine in machines],
        "globals": [[f"0x{m.scratch:08x}", f"0x{m.packet_size:08x}"] for m in machines],
        "checks": "Complete 64 KiB heap, ordered write addresses/widths/values, saved registers and no-argument RET. No mocked callees.",
        "limits": "Finite mapped-memory fixtures and bounded packet sizes; no live network, full-game or universal equivalence claim. Overlap checks describe these native copies, not portable C++ memcpy guarantees.",
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--output",
        type=Path,
        default=BUILD / "packet-assembly-check.json",
        help="Evidence JSON (default: build-msvc400/packet-assembly-check.json)",
    )
    args = parser.parse_args()
    target = RecCmpProject.from_directory(BUILD).get(TARGET_ID)
    original_hash = hashlib.sha256(target.original_path.read_bytes()).hexdigest()
    if original_hash.lower() != target.sha256.lower():
        raise ValueError(
            "Original executable does not match the existing project reference hash"
        )
    engine = Compare.from_target(target)
    result = check(engine)
    result["images"] = {
        "original": original_hash,
        "rebuilt": hashlib.sha256(target.recompiled_path.read_bytes()).hexdigest(),
    }
    result["pdb_sha256"] = hashlib.sha256(
        target.recompiled_pdb.read_bytes()
    ).hexdigest()
    args.output.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(
        f"Packet assembly: {result['cases']} fixtures, {result['native_executions']} native executions; pass."
    )
    print(f"Evidence: {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
