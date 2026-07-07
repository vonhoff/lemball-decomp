#!/usr/bin/env python3

from __future__ import annotations

import argparse
import hashlib
import pathlib
import shutil

ORIGINAL_C2_MD5 = "dcd69f1dd28b02dd03dd7ed02984299a"
PATCHED_C2_MD5 = "e70acde41802ddec06c4263bb357ac30"
C2_SIZE = 549888


def main() -> int:
    parser = argparse.ArgumentParser(
        allow_abbrev=False,
        description="Patch VC 4.20 C2.EXE to silence warning C4786 during decomp builds.",
    )
    parser.add_argument("path", type=pathlib.Path, help="Path to VC 4.20 C2.EXE")
    parser.add_argument("-f", "--force", action="store_true", help="Skip sanity checks")
    args = parser.parse_args()

    if not args.path.is_file():
        parser.error("input is not a file")

    binary = bytearray(args.path.read_bytes())
    md5 = hashlib.md5(binary).hexdigest()

    if not args.force:
        if len(binary) != C2_SIZE:
            parser.error("file size is not correct")
        if md5 not in (ORIGINAL_C2_MD5, PATCHED_C2_MD5):
            parser.error("md5 checksum does not match the known VC 4.20 C2.EXE")

    if md5 == ORIGINAL_C2_MD5:
        shutil.copyfile(args.path, f"{args.path}.BAK")

    for offset, expected in (
        (0x52F07, bytes([0xE8, 0x4F, 0xB3, 0xFE, 0xFF])),
        (0x74832, bytes([0xE8, 0x24, 0x9A, 0xFC, 0xFF])),
    ):
        current = bytes(binary[offset : offset + len(expected)])
        if current not in (expected, bytes([0x90] * len(expected))) and not args.force:
            parser.error(f"unexpected bytes at 0x{offset:08x}")
        binary[offset : offset + len(expected)] = bytes([0x90] * len(expected))

    args.path.write_bytes(binary)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
