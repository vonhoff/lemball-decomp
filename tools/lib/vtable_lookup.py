"""Resolve unnamed absolute-call pointers through verified paired vtable slots."""

import struct

from reccmp.compare.functions import create_bin_lookup
from reccmp.formats.exceptions import (
    InvalidVirtualAddressError,
    InvalidVirtualReadError,
)
from reccmp.types import EntityType, ImageId


def install_vtable_lookups(comparator):
    """Keep upstream naming; fill unresolved indirect names with paired evidence."""
    functions = list(comparator.db.get_matches_by_type(EntityType.FUNCTION))
    tables = list(comparator.db.get_matches_by_type(EntityType.VTABLE))
    images = {ImageId.ORIG: comparator.orig_bin, ImageId.RECOMP: comparator.recomp_bin}
    pointers = {side: create_bin_lookup(image) for side, image in images.items()}
    targets = {
        side: {function.addr(side): function for function in functions}
        for side in images
    }

    def resolve(side, address):
        function = targets[side].get(address)
        if function is not None:
            return function
        entity = comparator.db.get(side, address, exact=True)
        if entity is None or entity.size(side) != 5:
            return None
        try:
            code = images[side].read(address, 5)
        except (InvalidVirtualAddressError, InvalidVirtualReadError):
            return None
        if len(code) != 5 or code[0] != 0xE9:
            return None
        target = address + 5 + struct.unpack_from("<i", code, 1)[0]
        return targets[side].get(target)

    def create_lookup(side, upstream):
        paired = {table.addr(side): table for table in tables}
        other = ImageId.RECOMP if side == ImageId.ORIG else ImageId.ORIG

        def lookup(address, exact=False, indirect=False):
            name = upstream(address, exact=exact, indirect=indirect)
            if name is not None or not indirect:
                return name
            entity = comparator.db.get(side, address, exact=False)
            if entity is None or entity.entity_type != EntityType.VTABLE:
                return None
            table = paired.get(entity.addr(side))
            if table is None:
                return None
            offset = address - table.addr(side)
            extents = [table.size(image) or table.max_size(image) for image in images]
            if any(size is None or offset + 4 > size for size in extents) or offset % 4:
                return None
            target = pointers[side](address)
            counterpart = pointers[other](table.addr(other) + offset)
            if target is None or counterpart is None:
                return None
            function = resolve(side, target)
            other_function = resolve(other, counterpart)
            if function is None or other_function is None:
                return None
            if function.orig_addr != other_function.orig_addr:
                return None
            name = function.match_name()
            return None if name is None else "->" + name

        return lookup

    for side, parser in (
        (ImageId.ORIG, comparator.orig_sanitize),
        (ImageId.RECOMP, comparator.recomp_sanitize),
    ):
        parser.name_lookup = create_lookup(side, parser.name_lookup)
