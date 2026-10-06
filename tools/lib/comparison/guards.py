"""Repair missing PDB symbols for evidenced MSVC 4 static initialization guards."""

import re
import struct

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from reccmp.compare.match_msvc import match_static_variables
from reccmp.formats.exceptions import (
    InvalidVirtualAddressError,
    InvalidVirtualReadError,
)
from reccmp.types import EntityType, ImageId


GUARD_SYMBOL = re.compile(r"\?\$S[0-9]+@\?1\?(\?.+)@4EA")


def guard_accesses(image, function, slot):
    """Require complete decoding and relocated BYTE TEST/OR bit-one accesses."""
    start, size = function
    if size is None or size <= 0:
        return None
    relocations = getattr(image, "relocations", ())
    if not isinstance(relocations, (set, frozenset)):
        return None
    try:
        data = image.read(start, size)
        initial = image.read(slot, 1)
    except (InvalidVirtualAddressError, InvalidVirtualReadError):
        return None
    if len(data) != size or initial != b"\0":
        return None
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    sites = []
    end = start
    for inst in decoder.disasm(data, start):
        end = inst.address + inst.size
        if inst.disp_size != 4 or inst.disp != slot:
            continue
        raw = bytes(inst.bytes)
        if (
            len(raw) != 7
            or raw[:2] not in (b"\xf6\x05", b"\x80\x0d")
            or raw[-1] != 1
            or inst.address + inst.disp_offset not in relocations
            or struct.unpack_from("<I", raw, 2)[0] != slot
        ):
            return None
        sites.append((inst.address - start, inst.mnemonic))
    if end != start + size or [kind for _, kind in sites] != ["test", "or"]:
        return None
    return sites


def repair_guard_symbols(comparator):
    """Promote a missing scoped PDB symbol only with paired owner/access evidence."""
    db = comparator.db
    rebuilt = list(db.unmatched(ImageId.RECOMP))
    repairs = []
    for original in db.unmatched(ImageId.ORIG):
        name = original.get("name") or ""
        scoped = GUARD_SYMBOL.fullmatch(name)
        if (
            scoped is None
            or original.entity_type != EntityType.DATA
            or not original.get("static_var")
            or original.size(ImageId.ORIG) != 1
        ):
            continue
        parent_address = original.get("parent_function")
        if not isinstance(parent_address, int):
            continue
        parent = db.get(ImageId.ORIG, parent_address, exact=True)
        if (
            parent is None
            or not parent.matched
            or parent.entity_type != EntityType.FUNCTION
            or parent.get("symbol") != scoped[1]
        ):
            continue
        candidates = [entity for entity in rebuilt if entity.get("name") == name]
        if len(candidates) != 1:
            continue
        candidate = candidates[0]
        if (
            candidate.entity_type != EntityType.DATA
            or candidate.size(ImageId.RECOMP) != 1
            or candidate.get("symbol") is not None
        ):
            continue
        original_sites = guard_accesses(
            comparator.orig_bin,
            (parent.orig_addr, parent.size(ImageId.ORIG)),
            original.orig_addr,
        )
        rebuilt_sites = guard_accesses(
            comparator.recomp_bin,
            (parent.recomp_addr, parent.size(ImageId.RECOMP)),
            candidate.recomp_addr,
        )
        if original_sites is not None and original_sites == rebuilt_sites:
            repairs.append((candidate.recomp_addr, name))
    if not repairs:
        return
    with db.batch() as batch:
        for address, name in repairs:
            batch.set(ImageId.RECOMP, address, symbol=name)
    match_static_variables(db)
