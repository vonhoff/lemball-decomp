"""Resolve unnamed absolute-call pointers through verified paired vtable slots."""

from reccmp.compare.functions import create_bin_lookup
from reccmp.types import EntityType, ImageId

from .thunks import resolve_jump_thunk


class VtableLookup:
    def __init__(self, comparator, side, upstream):
        self.db, self.side, self.upstream = comparator.db, side, upstream
        self.other = ImageId.RECOMP if side == ImageId.ORIG else ImageId.ORIG
        self.images = {
            ImageId.ORIG: comparator.orig_bin,
            ImageId.RECOMP: comparator.recomp_bin,
        }
        self.pointers = {
            side: create_bin_lookup(image) for side, image in self.images.items()
        }
        functions = list(self.db.get_matches_by_type(EntityType.FUNCTION))
        self.targets = {
            side: {function.addr(side): function for function in functions}
            for side in self.images
        }
        self.tables = {
            table.addr(side): table
            for table in self.db.get_matches_by_type(EntityType.VTABLE)
        }

    def resolve(self, side, address):
        function = self.targets[side].get(address)
        if function is not None:
            return function
        entity = self.db.get(side, address, exact=True)
        if entity is None or entity.size(side) != 5:
            return None
        target = resolve_jump_thunk(self.images[side], self.targets[side], address)
        return self.targets[side].get(target)

    def paired_slot(self, address):
        entity = self.db.get(self.side, address, exact=False)
        if entity is None or entity.entity_type != EntityType.VTABLE:
            return None
        table = self.tables.get(entity.addr(self.side))
        if table is None:
            return None
        offset = address - table.addr(self.side)
        extents = [table.size(side) or table.max_size(side) for side in self.images]
        if offset % 4 or any(size is None or offset + 4 > size for size in extents):
            return None
        return table, offset

    def paired_function(self, table, offset):
        functions = []
        for side in (self.side, self.other):
            target = self.pointers[side](table.addr(side) + offset)
            function = self.resolve(side, target) if target is not None else None
            if function is None:
                return None
            functions.append(function)
        if functions[0].orig_addr != functions[1].orig_addr:
            return None
        return functions[0]

    def __call__(self, address, exact=False, indirect=False):
        name = self.upstream(address, exact=exact, indirect=indirect)
        if name is not None or not indirect:
            return name
        slot = self.paired_slot(address)
        function = self.paired_function(*slot) if slot is not None else None
        name = function.match_name() if function is not None else None
        return None if name is None else "->" + name


def install_vtable_lookups(comparator):
    """Keep upstream naming; fill unresolved indirect names with paired evidence."""
    for side, parser in (
        (ImageId.ORIG, comparator.orig_sanitize),
        (ImageId.RECOMP, comparator.recomp_sanitize),
    ):
        parser.name_lookup = VtableLookup(comparator, side, parser.name_lookup)
