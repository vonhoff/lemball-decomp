#!/usr/bin/env python3
"""Check scalar message codecs and the socket header against original x86."""

import argparse
import hashlib
import json
import random
from dataclasses import dataclass
from pathlib import Path

from reccmp.types import ImageId
from unicorn import UcError

from lib import BUILD
from lib.x86_fixture import HEAP, HEAP_SIZE, X86Fixture, load, put, reference_build

OBJECT = HEAP + 0x1000
DATA = HEAP + 0x2000
OUTPUT = HEAP + 0x4000
HEADER = HEAP + 0x6000
READ_CURSOR = OBJECT + 0x20
WRITE_CURSOR = OBJECT + 0x1C
HEADER_FIELDS = ((0, 4), (4, 4), (8, 2), (10, 2), (12, 2), (14, 1))
FUNCTIONS = {
    ("add", 4): 0x0045EF10,
    ("add", 2): 0x0045EF40,
    ("add", 1): 0x0045EF60,
    ("value", 4): 0x0045EFF0,
    ("get", 4): 0x0045F010,
    ("convert", 4): 0x0045F040,
    ("value", 2): 0x0045F070,
    ("get", 2): 0x0045F090,
    ("convert", 2): 0x0045F0B0,
    ("value", 1): 0x0045F0D0,
    ("get", 1): 0x0045F0F0,
    ("convert", 1): 0x0045F110,
    ("socket_add", 0): 0x0045F790,
    ("socket_get", 0): 0x0045F7F0,
}


@dataclass
class Case:
    label: str
    operation: str
    width: int
    memory: bytearray
    argument: int = 0
    alias: bool = False


def expected_codec(case, *, snapshot_header=False):
    """Preserve native byte-store order, cursor reloads and partial returns."""
    memory = bytearray(case.memory)
    writes = []

    def store(address, size, value):
        offset = address - HEAP
        if not 0 <= offset <= len(memory) - size:
            raise ValueError(f"Codec fixture write outside heap: {address:#x}")
        memory[offset : offset + size] = value.to_bytes(size, "little")
        writes.append((address, size, value))

    def apply(operation, width, argument=0):
        cursor = WRITE_CURSOR if operation == "add" else READ_CURSOR
        result = 0
        if operation == "add":
            for byte in range(width):
                store(
                    load(memory, cursor) + width - byte - 1,
                    1,
                    argument >> (8 * byte) & 0xFF,
                )
        elif operation == "get":
            for byte in range(width):
                value = load(memory, load(memory, cursor) + width - byte - 1, 1)
                store(argument + byte, 1, value)
        elif operation == "value":
            result = int.from_bytes(
                bytes(
                    load(memory, load(memory, cursor) + byte, 1)
                    for byte in range(width)
                ),
                "big",
            )
        elif operation == "convert" and width == 4:
            pointer = load(memory, cursor)
            byte0 = load(memory, pointer, 1)
            byte1 = load(memory, pointer + 1, 1)
            store(pointer, 1, load(memory, pointer + 3, 1))
            pointer = load(memory, cursor)
            store(pointer + 1, 1, load(memory, pointer + 2, 1))
            store(load(memory, cursor) + 2, 1, byte1)
            store(load(memory, cursor) + 3, 1, byte0)
        elif operation == "convert" and width == 2:
            pointer = load(memory, cursor)
            byte1 = load(memory, pointer + 1, 1)
            byte0 = load(memory, pointer, 1)
            store(pointer + 1, 1, byte0)
            store(load(memory, cursor), 1, byte1)
        store(cursor, 4, (load(memory, cursor) + width) & 0xFFFFFFFF)
        return result

    result = 0
    if case.operation == "socket_add":
        initial = bytes(memory)
        for offset, width in HEADER_FIELDS:
            source = initial if snapshot_header else memory
            apply(
                "add", width, load(source, load(source, OBJECT + 0x2C) + offset, width)
            )
    elif case.operation == "socket_get":
        for _, width in HEADER_FIELDS:
            apply("convert", width)
    else:
        result = apply(case.operation, case.width, case.argument)
    return bytes(memory), writes, result


def snapshot_codec(case):
    """Negative control: cache the cursor and all scalar bytes before writing."""
    memory = bytearray(case.memory)
    cursor_field = WRITE_CURSOR if case.operation == "add" else READ_CURSOR
    cursor = load(memory, cursor_field)
    if case.operation == "add":
        target = cursor
        value = (case.argument & ((1 << (8 * case.width)) - 1)).to_bytes(
            case.width, "big"
        )
    else:
        target = case.argument if case.operation == "get" else cursor
        value = bytes(load(memory, cursor + byte, 1) for byte in range(case.width))[
            ::-1
        ]
    memory[target - HEAP : target - HEAP + case.width] = value
    put(memory, cursor_field, load(memory, cursor_field) + case.width)
    return bytes(memory)


def fixtures():
    random_source = random.Random(0x45EF10)

    def memory():
        result = bytearray(random_source.randbytes(HEAP_SIZE))
        put(result, WRITE_CURSOR, DATA)
        put(result, READ_CURSOR, DATA)
        put(result, OBJECT + 0x2C, HEADER)
        return result

    edges = (
        0,
        1,
        0xFFFFFFFF,
        0x80000000,
        0x7FFFFFFF,
        0x01020304,
        0xAA55AA55,
        0xFF00FF00,
    )
    for operation, width in FUNCTIONS:
        if not width:
            continue
        for index in range(256):
            state = memory()
            value = (
                edges[index] if index < len(edges) else random_source.getrandbits(32)
            )
            if width == 1:
                value = 0xA1B20000 | index
            pointer = DATA + (index % 8) * 16 + index % 4
            put(state, WRITE_CURSOR, pointer)
            put(state, READ_CURSOR, pointer)
            state[pointer - HEAP : pointer - HEAP + width] = (
                value & ((1 << (8 * width)) - 1)
            ).to_bytes(width, "big")
            argument = OUTPUT + index % 4 if operation == "get" else value
            yield Case(f"{operation}{width}/{index}", operation, width, state, argument)

    for width in (1, 2, 4):
        for delta in range(-width, width + 1):
            for index in range(8):
                yield Case(
                    f"get{width}/overlap{delta}/{index}",
                    "get",
                    width,
                    memory(),
                    DATA + delta,
                    True,
                )
        state = memory()
        put(state, WRITE_CURSOR, WRITE_CURSOR)
        value = {1: 0x40, 2: 0xAB20, 4: 0xA5200001}[width]
        yield Case(f"add{width}/cursor", "add", width, state, value, True)
        state = memory()
        state[DATA - HEAP : DATA - HEAP + 4] = (
            b"\x01\x00\x20\x40" if width == 4 else b"\x20\x40\x00\x00"
        )
        if width == 1:
            state[DATA - HEAP] = 0x40
        state[DATA + 0x42 - HEAP] = 0x30
        state[DATA + 0x40 - HEAP] = 0x30
        state[0x3040] = 1
        state[0x3041] = 0
        yield Case(f"get{width}/cursor", "get", width, state, READ_CURSOR, True)
        state = memory()
        put(state, READ_CURSOR, READ_CURSOR)
        state[OBJECT + 3 - HEAP] = 0
        yield Case(f"convert{width}/cursor", "convert", width, state, alias=True)

    for operation in ("socket_add", "socket_get"):
        for index in range(128):
            state = memory()
            if index == 0:
                state[HEADER - HEAP : HEADER - HEAP + 16] = bytes.fromhex(
                    "39305356040302012211443366557fa5"
                )
                state[DATA - HEAP : DATA - HEAP + 16] = bytes.fromhex(
                    "56533039010203041122334455667fa5"
                )
            yield Case(f"{operation}/{index}", operation, 0, state)
    for delta in (-4, -1, 0, 1, 4):
        state = memory()
        put(state, WRITE_CURSOR, HEADER + delta)
        yield Case(f"socket_add/overlap{delta}", "socket_add", 0, state, alias=True)


def check(engine):
    entities = {entity.orig_addr: entity for entity in engine.get_all()}
    machines = [X86Fixture(image) for image in (engine.orig_bin, engine.recomp_bin)]
    image_ids = (ImageId.ORIG, ImageId.RECOMP)
    digest = hashlib.sha256()
    counts = {f"{op}{width}": 0 for op, width in FUNCTIONS}
    controls = {"scalar_snapshot": 0, "header_snapshot": 0}
    aliases = 0
    for case in fixtures():
        counts[f"{case.operation}{case.width}"] += 1
        aliases += case.alias
        entity = entities[FUNCTIONS[case.operation, case.width]]
        arguments = (case.argument,) if case.operation in ("add", "get") else ()
        try:
            expected_heap, expected_writes, expected_value = expected_codec(case)
            results = [
                machine.run(entity.addr(image_id), case.memory, OBJECT, arguments)
                for machine, image_id in zip(machines, image_ids, strict=True)
            ]
            for label, result in zip(("original", "rebuilt"), results, strict=True):
                if (result.heap, result.writes) != (expected_heap, expected_writes):
                    raise ValueError(
                        f"{label} differs from heap or ordered-write contract"
                    )
                if (
                    case.operation == "value"
                    and result.eax & ((1 << (case.width * 8)) - 1) != expected_value
                ):
                    raise ValueError(f"{label} returned the wrong scalar value")
            if case.alias:
                if case.operation == "socket_add":
                    controls["header_snapshot"] += (
                        expected_codec(case, snapshot_header=True)[0] != expected_heap
                    )
                else:
                    controls["scalar_snapshot"] += snapshot_codec(case) != expected_heap
            if case.operation.startswith("socket_"):
                cursor_field = (
                    WRITE_CURSOR if case.operation == "socket_add" else READ_CURSOR
                )
                initial = load(case.memory, cursor_field)
                if (
                    load(expected_heap, cursor_field) != initial + 15
                    or expected_heap[initial + 15 - HEAP]
                    != case.memory[initial + 15 - HEAP]
                ):
                    raise ValueError(
                        "Socket codec changed the unwritten sixteenth byte or cursor extent"
                    )
        except (UcError, ValueError) as error:
            raise ValueError(f"{case.label}: {error}") from error
        digest.update(results[0].heap)
        digest.update(repr(results[0].writes).encode())
        if case.operation == "value":
            digest.update(expected_value.to_bytes(case.width, "little"))
    if not all(controls.values()):
        raise ValueError("Aliasing fixtures did not distinguish both snapshot controls")
    functions = []
    for (operation, width), address in FUNCTIONS.items():
        entity = entities[address]
        instructions = machines[0].instruction_addresses(
            address, entity.size(ImageId.ORIG)
        )
        if instructions - machines[0].visited:
            raise ValueError(
                f"Original {operation}{width} has unexercised instructions"
            )
        functions.append(
            {
                "operation": operation,
                "width": width,
                "original": f"0x{address:08x}",
                "rebuilt": f"0x{entity.recomp_addr:08x}",
                "original_instructions_exercised": len(instructions),
            }
        )
    cases = sum(counts.values())
    return {
        "cases": cases,
        "native_executions": cases * 2,
        "case_counts": counts,
        "controlled_alias_cases": aliases,
        "negative_control_disagreements": controls,
        "functions": functions,
        "digest": digest.hexdigest(),
        "checks": "Complete 64 KiB heaps, ordered heap writes, narrow return values, preserved registers and thiscall RET. All original instructions in the 14 codec/caller bodies. No mocked callees.",
        "limits": "Finite mapped-heap fixtures; no argument-stack aliasing, live networking or universal equivalence claim. Controlled aliases do not establish actual caller reachability.",
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--output",
        type=Path,
        default=BUILD / "message-codec-check.json",
        help="Evidence JSON (default: build-msvc400/message-codec-check.json)",
    )
    args = parser.parse_args()
    engine, provenance = reference_build()
    result = check(engine)
    result.update(provenance)
    args.output.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(
        f"Message codec: {result['cases']} fixtures, {result['native_executions']} x86 executions; pass."
    )
    print(f"Evidence: {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
