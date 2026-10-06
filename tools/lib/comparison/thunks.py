"""Resolve direct IA-32 jump thunks to paired function entries."""

import struct
from collections.abc import Collection

from reccmp.formats.exceptions import (
    InvalidVirtualAddressError,
    InvalidVirtualReadError,
)


def read_jump_target(image, address: int) -> int | None:
    """Decode one complete IA-32 E9 with a bounded instruction fetch."""
    if not 0 <= address <= 0xFFFFFFFB:
        return None
    try:
        raw = image.read(address, 5)
    except (InvalidVirtualAddressError, InvalidVirtualReadError):
        return None
    if len(raw) != 5 or raw[0] != 0xE9:
        return None
    return (address + 5 + struct.unpack_from("<i", raw, 1)[0]) & 0xFFFFFFFF


def resolve_jump_thunk(image, targets: Collection[int], address: int) -> int | None:
    """Resolve one complete E9 to a readable paired function entry."""
    if address in targets:
        return None
    target = read_jump_target(image, address)
    if target is None or target not in targets:
        return None
    try:
        return target if image.read(target, 1) else None
    except (InvalidVirtualAddressError, InvalidVirtualReadError):
        return None
