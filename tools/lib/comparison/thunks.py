"""Resolve direct IA-32 jump thunks to paired function entries."""

from collections.abc import Collection

from reccmp.formats.exceptions import (
    InvalidVirtualAddressError,
    InvalidVirtualReadError,
)


def read_jump_target(image, address: int) -> int | None:
    """Decode one complete IA-32 E9 with a bounded instruction fetch."""
    try:
        raw = image.read(address, 5)
    except (InvalidVirtualAddressError, InvalidVirtualReadError):
        return None
    if len(raw) != 5 or raw[0] != 0xE9:
        return None
    return (address + 5 + int.from_bytes(raw[1:], "little")) & 0xFFFFFFFF


def resolve_jump_thunk(image, targets: Collection[int], address: int) -> int | None:
    """Resolve one complete E9 to a paired function entry."""
    if address in targets:
        return None
    target = read_jump_target(image, address)
    return target if target in targets else None
