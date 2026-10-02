"""Direct jump-thunk equivalence for the Effective metric."""

import re
import struct

from reccmp.compare.asm.instgen import InstructGen, SectionType
from reccmp.compare.asm.parse import ParseAsm
from reccmp.compare.functions import FunctionComparator
from reccmp.formats.exceptions import InvalidVirtualAddressError, InvalidVirtualReadError
from reccmp.types import EntityType

from . import BUILD

EFFECTIVE_JSON = BUILD / "effective.json"


class ThunkParseAsm(ParseAsm):
    def __init__(self, image, targets, upstream):
        super().__init__(addr_test=upstream.addr_test, name_lookup=upstream.name_lookup)
        self.image, self.targets = image, targets
        self.used_thunk, self.signature = False, None

    def sanitize(self, inst):
        _, _, mnemonic, operands = inst
        if mnemonic in ("call", "jmp") and operands.startswith("0x"):
            address = int(operands, 16)
            if address not in self.targets:
                try:
                    raw = self.image.read(address, 5)
                except (InvalidVirtualAddressError, InvalidVirtualReadError):
                    raw = b""
                if len(raw) == 5 and raw[0] == 0xe9:
                    target = address + 5 + struct.unpack_from("<i", raw, 1)[0]
                    name = self.lookup(target, exact=True) if target in self.targets else None
                    if name is not None:
                        # E9 changes only EIP. Resolve this transfer locally so
                        # ordinary function pointers retain their own identity.
                        self.used_thunk = True
                        return mnemonic, name
        return super().sanitize(inst)

    def parse_asm(self, data, start_addr):
        self.used_thunk, self.signature = False, None
        data = bytes(data)
        asm = super().parse_asm(data, start_addr)
        cursor = start_addr
        for section in InstructGen(data, start_addr).sections:
            if section.type != SectionType.CODE:
                return asm
            for address, size, _, _ in section.contents:
                if address != cursor:
                    return asm
                cursor += size
        if cursor != start_addr + len(data) or any("<OFFSET" in line for _, line in asm):
            return asm
        addresses = {address for address, _ in asm}
        for index, (_, line) in enumerate(asm):
            jump = re.fullmatch(r"(?:j\w+|loop\w*) (-?0x[0-9a-f]+)", line)
            if jump:
                following = asm[index + 1][0] if index + 1 < len(asm) else cursor
                if following + int(jump[1], 16) not in addresses:
                    return asm
        # Equal instruction offsets preserve relative branches and return sites.
        # Capture actual operands before FunctionComparator's assertion fixup.
        self.signature = (len(data), [(address - start_addr, line) for address, line in asm])
        return asm


def additional_effective_matches(engine, comparisons):
    upstream = engine.function_comparator
    comparator = FunctionComparator(
        upstream.db, upstream.lines_db, upstream.orig_bin, upstream.recomp_bin,
        upstream.report, upstream.types,
    )
    functions = list(upstream.db.get_matches_by_type(EntityType.FUNCTION))
    original = ThunkParseAsm(
        upstream.orig_bin, {entity.orig_addr for entity in functions}, comparator.orig_sanitize,
    )
    rebuilt = ThunkParseAsm(
        upstream.recomp_bin, {entity.recomp_addr for entity in functions}, comparator.recomp_sanitize,
    )
    comparator.orig_sanitize, comparator.recomp_sanitize = original, rebuilt
    matches = {}
    for match in engine.get_functions():
        comparison = comparisons.get(match.orig_addr)
        if (comparison is None or not comparison.is_function() or comparison.is_stub
                or comparison.accuracy == 1 or comparison.is_effective_match):
            continue
        comparator.compare_function(match)
        if (original.signature is not None and original.signature == rebuilt.signature
                and (original.used_thunk or rebuilt.used_thunk)):
            matches[match.orig_addr] = ("verified jump thunk target",)
    return matches


def effective_addresses(comparisons, additional=()):
    """Function addresses counted by the Effective metric."""
    return {
        address for address, comparison in comparisons.items()
        if comparison.is_function() and comparison.is_matched() and not comparison.is_stub
        and (comparison.accuracy == 1 or comparison.is_effective_match or address in additional)
    }
