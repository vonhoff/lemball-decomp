"""Teach reccmp 0.1.6 data and vtable comparison about original linker ILTs."""

from reccmp.compare.core import Compare
from reccmp.compare.db import EntityDb
from reccmp.compare.variables import VariableComparator
from reccmp.formats.exceptions import InvalidVirtualReadError
from reccmp.types import ImageId


ILT_START = 0x00401000
ILT_LAST_ENTRY = 0x0040388C
# These original vtable entries are linker thunks to reconstructed virtual
# methods, while manual compatibility tables still need the thunk identities.
VTABLE_METHOD_ILTS = {
    0x00401451,  # VsNetEffStreamCommon::NoopEffStream
    0x00401564,  # VsNetEffStreamCommon::AcceptEffStreamArgument
    0x004026AD,  # VsNetEffStreamCommon::AcceptEffStream
}


def _ilt_destination(comparator, address):
    if address < ILT_START or ILT_LAST_ENTRY < address:
        return None

    try:
        instruction = comparator.orig_bin.read(address, 5)
    except InvalidVirtualReadError:
        return None

    if len(instruction) != 5 or instruction[0] != 0xE9:
        return None

    displacement = int.from_bytes(instruction[1:5], byteorder="little", signed=True)
    return address + 5 + displacement


if not getattr(VariableComparator, "_lemball_ilt_aware", False):
    _reccmp_is_pointer_match = VariableComparator.is_pointer_match

    def _is_pointer_match(self, orig_addr, recomp_addr):
        if _reccmp_is_pointer_match(self, orig_addr, recomp_addr):
            return True

        destination = _ilt_destination(self, orig_addr)
        return destination is not None and self.db.is_match(destination, recomp_addr)

    VariableComparator.is_pointer_match = _is_pointer_match
    VariableComparator._lemball_ilt_aware = True


if not getattr(Compare, "_lemball_ilt_aware_vtables", False):
    _reccmp_compare_vtable = Compare._compare_vtable

    def _compare_vtable(self, match):
        """Compare original ILT slots by their destination method identity."""
        reccmp_get = EntityDb.get

        def _get(db, image_id, address, *, exact=True):
            if image_id == ImageId.ORIG and address in VTABLE_METHOD_ILTS:
                destination = _ilt_destination(self, address)
                if destination is not None:
                    entity = reccmp_get(db, image_id, destination, exact=exact)
                    if entity is not None and entity.recomp_addr is not None:
                        return entity

            return reccmp_get(db, image_id, address, exact=exact)

        EntityDb.get = _get
        try:
            return _reccmp_compare_vtable(self, match)
        finally:
            EntityDb.get = reccmp_get

    Compare._compare_vtable = _compare_vtable
    Compare._lemball_ilt_aware_vtables = True
