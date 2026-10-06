"""Name paired references by original identity and byte offset."""

from collections import Counter
from reccmp.types import EntityType, ImageId

from .vtables import VtableLookup


class PairedReferences:
    def __init__(self, comparator, side):
        self.db, self.side = comparator.db, side
        self.vtables = VtableLookup(comparator, side, lambda *args, **kwargs: None)
        self.entities = {entity.addr(side): entity for entity in self.db.get_matches()}

    @staticmethod
    def identity(entity):
        return f"paired {entity.entity_type.name.lower()} @{entity.orig_addr:08x}"

    def direct(self, address, exact=False):
        entity = self.db.get(self.side, address, exact=exact)
        if entity is None or entity.addr(self.side) not in self.entities:
            return None
        offset = address - entity.addr(self.side)
        if offset == 0:
            return self.identity(entity)
        if entity.entity_type not in (EntityType.DATA, EntityType.OFFSET):
            return None
        extents = [entity.size(side) for side in (ImageId.ORIG, ImageId.RECOMP)]
        if any(size is None or offset >= size for size in extents):
            return None
        return f"{self.identity(entity)}+{offset}"

    def indirect(self, address):
        entity = self.db.get(self.side, address, exact=True)
        if entity is not None and entity.entity_type in (
            EntityType.DATA,
            EntityType.IMPORT,
        ):
            return self.direct(address, exact=True)
        slot = self.vtables.paired_slot(address)
        if slot is None or self.vtables.paired_function(*slot) is None:
            return None
        table, offset = slot
        return f"{self.identity(table)}+{offset}"

    def __call__(self, address, exact=False, indirect=False):
        return self.indirect(address) if indirect else self.direct(address, exact)

    def disambiguate(self, upstream, duplicates):
        def lookup(address, exact=False, indirect=False):
            name = upstream(address, exact=exact, indirect=indirect)
            if name is None:
                return None
            label = name.removeprefix("->")
            if any(label.startswith(base) for base in duplicates):
                return self(address, exact=exact, indirect=indirect)
            return name

        return lookup


def install_unique_lookups(comparator):
    """Disambiguate colliding display names without altering ordinary raw names."""
    counts = Counter(entity.best_name() for entity in comparator.db.get_matches())
    duplicates = {
        name for name, count in counts.items() if name is not None and count > 1
    }
    if not duplicates:
        return
    for side, parser in (
        (ImageId.ORIG, comparator.orig_sanitize),
        (ImageId.RECOMP, comparator.recomp_sanitize),
    ):
        parser.name_lookup = PairedReferences(comparator, side).disambiguate(
            parser.name_lookup, duplicates
        )
