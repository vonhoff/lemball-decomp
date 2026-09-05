#!/usr/bin/env python3
"""Export the original Ground::IsHit collision masks as grayscale BMPs."""

from __future__ import annotations

import struct
from pathlib import Path


MASKS = {
    "49e1e0_type_0206": (
        0x0003C000, 0x000FF000, 0x003FFC00, 0x00FFFF00,
        0x03FFFFF0, 0x0FFFFFF0, 0x3FFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0x3FFFFFFC, 0x0FFFFFF0, 0x03FFFFC0,
        0x00FFFF00, 0x003FFC00, 0x000FF000, 0x0003C000,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    ),
    "49e260_type_0207": (
        0, 0, 0, 0, 0, 0, 0, 0,
        0x0003C000, 0x000FF000, 0x003FFC00, 0x00FFFF00,
        0x03FFFFC0, 0x0FFFFFF0, 0x3FFFFFFC, 0xFFFFFFFF,
        0xFFFFFFFF, 0x3FFFFFFC, 0x0FFFFFF0, 0x03FFFFC0,
        0x00FFFF00, 0x003FFC00, 0x000FF000, 0x0003C000,
        0, 0, 0, 0, 0, 0, 0, 0,
    ),
    "49e2e0_type_0208_020d": (
        0x0000E000, 0x0001F800, 0x0001FE00, 0x0003FF80,
        0x0007FFE0, 0x0007FFF8, 0x000FFFFE, 0x001FFFFE,
        0x001FFFFF, 0x003FFFFF, 0x007FFFFC, 0x007FFFFC,
        0x00FFFFF8, 0x01FFFFF0, 0x01FFFFF0, 0x03FFFFE0,
        0x07FFFFC0, 0x07FFFFC0, 0x0FFFFF80, 0x1FFFFF00,
        0x1FFFFF00, 0x3FFFFE00, 0x7FFFFC00, 0x7FFFFC00,
        0xFFFFF800, 0x3FFFF000, 0x0FFFF000, 0x03FFF000,
        0x00FFE000, 0x003FC000, 0x000FC000, 0x0003C000,
    ),
    "49e360_type_0202_0209_0214_021a": (
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0x0003C000, 0x000FF000, 0x003FFC00, 0x00FFFF00,
        0x03FFFFC0, 0x0FFFFFF0, 0x3FFFFFFC, 0xFFFFFFFF,
        0xFFFFFFFF, 0x3FFFFFFC, 0x0FFFFFF0, 0x03FFFFC0,
        0x00FFFF00, 0x003FFC00, 0x000FF000, 0x0003C000,
    ),
    "49e3e0_type_020e_020f": (
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0x00018000, 0x0003F000, 0x0007FC00, 0x001FFF00,
        0x003FFFC0, 0x007FFFFF, 0x01FFFFFF, 0x03FFFFFF,
        0x07FFFFFE, 0x0FFFFFFC, 0x1FFFFFF8, 0x3FFFFFF0,
        0x7FFFFFE0, 0xFFFFFFC0, 0xFFFFFF80, 0x3FFFFF00,
        0x0FFFFE00, 0x03FFF880, 0x00FFF000, 0x003FE000,
        0x000FC000, 0x0003C000,
    ),
}


def write_bmp(path: Path, rows: tuple[int, ...], scale: int) -> None:
    width = height = 32 * scale
    row_bytes = (width + 3) & ~3
    pixels = bytearray()
    for row in rows:
        expanded = bytearray()
        for bit in range(32):
            expanded.extend([255 if row & (1 << (31 - bit)) else 0] * scale)
        for _ in range(scale):
            pixels.extend(expanded)
            pixels.extend(b"\0" * (row_bytes - width))

    offset = 14 + 40 + 256 * 4
    dib = struct.pack(
        "<IiiHHIIiiII", 40, width, -height, 1, 8, 0,
        len(pixels), 0, 0, 256, 0
    )
    palette = bytearray()
    for value in range(256):
        palette.extend((value, value, value, 0))
    header = b"BM" + struct.pack("<IHHI", offset + len(pixels), 0, 0, offset)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(header + dib + palette + pixels)


def mirror(rows: tuple[int, ...]) -> tuple[int, ...]:
    return tuple(int(f"{row:032b}"[::-1], 2) for row in rows)


def main() -> None:
    root = Path(__file__).resolve().parents[1] / "artifacts" / "ground_masks"
    for name, rows in MASKS.items():
        write_bmp(root / (name + ".bmp"), rows, 1)
        write_bmp(root / (name + "_10x.bmp"), rows, 10)
    for name in ("49e2e0_type_0208_020d", "49e3e0_type_020e_020f"):
        rows = mirror(MASKS[name])
        write_bmp(root / (name + "_mirrored.bmp"), rows, 1)
        write_bmp(root / (name + "_mirrored_10x.bmp"), rows, 10)


if __name__ == "__main__":
    main()
