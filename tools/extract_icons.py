#!/usr/bin/env python3
"""Recover LEMBALL's icon files and a resource script in the build directory."""

import argparse
import ctypes
import struct
from ctypes import wintypes
from pathlib import Path

from lib import ROOT

# Resource compiler allocation order for the original RT_ICON identifiers.
ICON_IDS = (117, 118, 150, 119, 120)
LANGUAGE = 0x0809


def extract_icons(executable: Path) -> dict[int, bytes]:
    kernel = ctypes.WinDLL("kernel32", use_last_error=True)
    signatures = {
        "LoadLibraryExW": (
            wintypes.HMODULE,
            [wintypes.LPCWSTR, wintypes.HANDLE, wintypes.DWORD],
        ),
        "FindResourceExW": (
            wintypes.HRSRC,
            [wintypes.HMODULE, ctypes.c_void_p, ctypes.c_void_p, wintypes.WORD],
        ),
        "SizeofResource": (wintypes.DWORD, [wintypes.HMODULE, wintypes.HRSRC]),
        "LoadResource": (wintypes.HGLOBAL, [wintypes.HMODULE, wintypes.HRSRC]),
        "LockResource": (ctypes.c_void_p, [wintypes.HGLOBAL]),
        "FreeLibrary": (wintypes.BOOL, [wintypes.HMODULE]),
    }
    for name, (result, arguments) in signatures.items():
        function = getattr(kernel, name)
        function.restype = result
        function.argtypes = arguments

    module = kernel.LoadLibraryExW(
        str(executable.resolve()), None, 0x2
    )  # LOAD_LIBRARY_AS_DATAFILE
    if not module:
        raise ctypes.WinError(ctypes.get_last_error())

    def read_resource(kind: int, identifier: int) -> bytes:
        resource = kernel.FindResourceExW(module, kind, identifier, LANGUAGE)
        if not resource:
            raise ctypes.WinError(ctypes.get_last_error())
        size = kernel.SizeofResource(module, resource)
        loaded = kernel.LoadResource(module, resource)
        if not loaded:
            raise ctypes.WinError(ctypes.get_last_error())
        pointer = kernel.LockResource(loaded)
        if not pointer:
            raise ctypes.WinError(ctypes.get_last_error())
        return ctypes.string_at(pointer, size)

    try:
        icons = {}
        for identifier in ICON_IDS:
            group = read_resource(14, identifier)  # RT_GROUP_ICON
            reserved, kind, count = struct.unpack_from("<HHH", group)
            if reserved != 0 or kind != 1 or count == 0 or len(group) != 6 + count * 14:
                raise ValueError(f"Invalid icon group {identifier}")
            directory = bytearray(group[:6])
            images = bytearray()
            offset = 6 + count * 16
            for entry in struct.iter_unpack("<12sH", group[6:]):
                header, image_id = entry
                image = read_resource(3, image_id)  # RT_ICON
                if len(image) != struct.unpack_from("<I", header, 8)[0]:
                    raise ValueError(f"Invalid icon image {image_id}")
                directory += header + struct.pack("<I", offset)
                images += image
                offset += len(image)
            icons[identifier] = bytes(directory + images)
        return icons
    finally:
        kernel.FreeLibrary(module)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("output", type=Path, help="Generated resource script")
    args = parser.parse_args()
    icons = extract_icons(ROOT / "data/LEMBALL.EXE")
    output = args.output.resolve()
    output.parent.mkdir(parents=True, exist_ok=True)
    source = ROOT / "src/Platform/Windows/LEMBALL.RC"
    lines = [f'#include "{source.as_posix()}"', "LANGUAGE 9, 2"]
    for identifier, data in icons.items():
        filename = output.parent / f"{identifier}.ico"
        filename.write_bytes(data)
        lines.append(f'{identifier} ICON "{filename.as_posix()}"')
    output.write_text("\n".join(lines) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
