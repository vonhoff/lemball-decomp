#!/usr/bin/env python3
"""Extract the original executable's resource tree into a Win32 .RES file."""

import argparse
import struct
from pathlib import Path

import pefile


def align4(data):
    return data + b"\0" * (-len(data) & 3)


def identifier(value):
    if isinstance(value, int):
        return struct.pack("<HH", 0xFFFF, value)
    return value.encode("utf-16le") + b"\0\0"


def entry(type_id, name, language, data):
    ids = align4(identifier(type_id) + identifier(name))
    header_size = 8 + len(ids) + 16
    header = struct.pack("<II", len(data), header_size)
    header += ids
    header += struct.pack("<IHHII", 0, 0x1030, language, 0, 0)
    return header + align4(data)


def extract(executable, output):
    pe = pefile.PE(str(executable))
    result = entry(0, 0, 0, b"")
    count = 0
    image = pe.get_memory_mapped_image()
    for type_entry in pe.DIRECTORY_ENTRY_RESOURCE.entries:
        type_id = str(type_entry.name) if type_entry.name is not None else type_entry.struct.Id
        for name_entry in type_entry.directory.entries:
            name = str(name_entry.name) if name_entry.name is not None else name_entry.struct.Id
            for language_entry in name_entry.directory.entries:
                raw = language_entry.data.struct
                data = image[raw.OffsetToData : raw.OffsetToData + raw.Size]
                result += entry(type_id, name, language_entry.struct.Id, data)
                count += 1
    output.write_bytes(result)
    return count


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("executable", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    print(f"wrote {extract(args.executable, args.output)} resources to {args.output}")


if __name__ == "__main__":
    main()
