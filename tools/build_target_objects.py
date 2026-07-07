#!/usr/bin/env python3

from __future__ import annotations

import argparse
import re
import struct
import subprocess
import tempfile
from dataclasses import dataclass
from pathlib import Path


@dataclass
class Section:
    virtual_address: int
    virtual_size: int
    raw_pointer: int
    raw_size: int


@dataclass
class Slice:
    unit: str
    start_va: int
    end_va: int


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Assemble target-side COFF objects from verified LEMBALL.EXE slices."
    )
    parser.add_argument("--exe", required=True, help="Path to LEMBALL.EXE")
    parser.add_argument("--manifest", required=True, help="Path to slice manifest")
    parser.add_argument("--outdir", required=True, help="Output directory for .obj files")
    parser.add_argument("--assembler", required=True, help="Watcom assembler command")
    parser.add_argument("--unit", help="Optional single unit name to build")
    return parser.parse_args()


def load_pe_image(exe_path: Path) -> tuple[bytes, int, list[Section]]:
    data = exe_path.read_bytes()
    pe_offset = struct.unpack_from("<I", data, 0x3C)[0]
    coff_offset = pe_offset + 4
    section_count = struct.unpack_from("<H", data, coff_offset + 2)[0]
    optional_size = struct.unpack_from("<H", data, coff_offset + 16)[0]
    optional_offset = coff_offset + 20
    image_base = struct.unpack_from("<I", data, optional_offset + 28)[0]
    section_offset = optional_offset + optional_size

    sections: list[Section] = []
    for index in range(section_count):
        offset = section_offset + index * 40
        virtual_size, virtual_address, raw_size, raw_pointer = struct.unpack_from(
            "<IIII", data, offset + 8
        )
        sections.append(
            Section(
                virtual_address=virtual_address,
                virtual_size=virtual_size,
                raw_pointer=raw_pointer,
                raw_size=raw_size,
            )
        )
    return data, image_base, sections


def parse_manifest(manifest_path: Path) -> list[Slice]:
    slices: list[Slice] = []
    for raw_line in manifest_path.read_text().splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue
        parts = re.split(r"\s+", line, maxsplit=3)
        if len(parts) < 3:
            raise SystemExit(f"bad slice manifest line: {raw_line}")
        slices.append(
            Slice(
                unit=parts[0],
                start_va=int(parts[1], 16),
                end_va=int(parts[2], 16),
            )
        )
    return slices


def va_to_file_offset(sections: list[Section], image_base: int, va: int) -> int:
    rva = va - image_base
    for section in sections:
        start = section.virtual_address
        end = section.virtual_address + max(section.virtual_size, section.raw_size)
        if start <= rva < end:
            return section.raw_pointer + (rva - section.virtual_address)
    raise SystemExit(f"VA 0x{va:08x} is not inside any mapped PE section")


def format_byte(value: int) -> str:
    if value <= 0x9:
        return f"{value}h"
    if value <= 0xF or value >= 0xA0:
        return f"0{value:X}h"
    return f"{value:X}h"


def emit_asm(path: Path, unit: str, payload: bytes) -> None:
    symbol = f"{unit}_TARGET"
    lines = [
        ".386",
        '_TEXT segment byte public use32 "CODE"',
        f"public {symbol}",
        f"{symbol} label byte",
    ]
    for offset in range(0, len(payload), 16):
        chunk = payload[offset : offset + 16]
        lines.append("db " + ", ".join(format_byte(byte) for byte in chunk))
    lines.extend(["_TEXT ends", "end", ""])
    path.write_text("\n".join(lines))


def main() -> int:
    args = parse_args()
    exe_path = Path(args.exe)
    outdir = Path(args.outdir)
    manifest_path = Path(args.manifest)

    data, image_base, sections = load_pe_image(exe_path)
    slices = parse_manifest(manifest_path)
    if args.unit:
        slices = [target_slice for target_slice in slices if target_slice.unit == args.unit]
        if not slices:
            raise SystemExit(f"unit {args.unit} not found in {manifest_path}")
    outdir.mkdir(parents=True, exist_ok=True)

    with tempfile.TemporaryDirectory(prefix="lemball-target-asm-") as tmp_name:
        tmpdir = Path(tmp_name)
        for target_slice in slices:
            if target_slice.end_va <= target_slice.start_va:
                raise SystemExit(f"bad slice for {target_slice.unit}: end must be after start")

            start_offset = va_to_file_offset(sections, image_base, target_slice.start_va)
            end_offset = va_to_file_offset(sections, image_base, target_slice.end_va - 1) + 1
            payload = data[start_offset:end_offset]

            asm_path = tmpdir / f"{target_slice.unit}.ASM"
            obj_path = outdir / f"{target_slice.unit}.obj"
            emit_asm(asm_path, target_slice.unit, payload)

            subprocess.run(
                [args.assembler, "-q", f"-fo={obj_path}", str(asm_path)],
                check=True,
            )

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
