"""Additional Effective matching through reccmp and one-hop IA-32 jump thunks."""

from collections.abc import Collection
from copy import copy

from reccmp.compare.asm.parse import ParseAsm
from reccmp.compare.report import ReccmpComparedEntity
from reccmp.formats.exceptions import (
    InvalidVirtualAddressError,
    InvalidVirtualReadError,
)
from reccmp.types import EntityType, ImageId


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


class ThunkParseAsm(ParseAsm):
    def __init__(self, image, targets, upstream):
        super().__init__(addr_test=upstream.addr_test, name_lookup=upstream.name_lookup)
        self.image, self.targets = image, targets

    def sanitize(self, inst):
        _, _, mnemonic, operands = inst
        if mnemonic in ("call", "jmp") and operands.startswith("0x"):
            target = resolve_jump_thunk(self.image, self.targets, int(operands, 16))
            name = self.lookup(target, exact=True) if target is not None else None
            if name is not None:
                return mnemonic, name
        return super().sanitize(inst)


def additional_effective_matches(
    engine, comparisons: dict[int, ReccmpComparedEntity]
) -> set[int]:
    """Recheck with one-hop E9 resolution using upstream reccmp matching."""
    candidates = [
        match
        for match in engine.get_functions()
        if (comparison := comparisons.get(match.orig_addr)) is not None
        and comparison.is_function()
        and comparison.is_matched()
        and not comparison.is_stub
        and comparison.effective_accuracy != 1
    ]
    if not candidates:
        return set()
    upstream = copy(engine.function_comparator)
    functions = list(upstream.db.get_matches_by_type(EntityType.FUNCTION))
    upstream.orig_sanitize = ThunkParseAsm(
        upstream.orig_bin,
        {function.addr(ImageId.ORIG) for function in functions},
        upstream.orig_sanitize,
    )
    upstream.recomp_sanitize = ThunkParseAsm(
        upstream.recomp_bin,
        {function.addr(ImageId.RECOMP) for function in functions},
        upstream.recomp_sanitize,
    )
    accepted = set()
    for match in candidates:
        result = upstream.compare_function(match)
        if result.match_ratio == 1 or result.is_effective_match:
            accepted.add(match.orig_addr)
    return accepted


def effective_addresses(
    comparisons: dict, additional: Collection[int] = ()
) -> set[int]:
    """Function addresses counted by the Effective metric."""
    return {
        address
        for address, comparison in comparisons.items()
        if comparison.is_function()
        and comparison.is_matched()
        and not comparison.is_stub
        and (comparison.effective_accuracy == 1 or address in additional)
    }
