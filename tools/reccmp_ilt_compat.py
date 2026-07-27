"""Teach reccmp 0.1.6 data comparison about original linker ILT pointers."""

from reccmp.compare.variables import VariableComparator
from reccmp.formats.exceptions import InvalidVirtualReadError


ILT_START = 0x00401000
ILT_LAST_ENTRY = 0x0040388C


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
