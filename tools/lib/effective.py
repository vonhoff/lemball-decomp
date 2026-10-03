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
        self.local_tables = {}

    def lookup(self, addr, exact=False, indirect=False):
        if not indirect and addr in self.local_tables:
            return self.local_tables[addr]
        return super().lookup(addr, exact=exact, indirect=indirect)

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
        sections = InstructGen(data, start_addr).sections
        self.local_tables = {
            section.contents[0][0]: f"{section.type.name} at +0x{section.contents[0][0] - start_addr:x}"
            for section in sections
            if section.type != SectionType.CODE and section.contents
        }
        asm = super().parse_asm(data, start_addr)
        cursor = start_addr
        instruction_ends = {}
        table_targets = []
        for section in sections:
            for entry in section.contents:
                address = entry[0]
                if section.type == SectionType.CODE:
                    size = entry[1]
                    instruction_ends[address] = address + size
                elif section.type == SectionType.ADDR_TAB:
                    size = 4
                    table_targets.append(entry[1])
                else:
                    size = 1
                if address != cursor:
                    return asm
                cursor += size
        if cursor != start_addr + len(data) or any("<OFFSET" in line for _, line in asm):
            return asm
        if any(target not in instruction_ends for target in table_targets):
            return asm
        for section in sections:
            if section.type != SectionType.CODE or not section.contents:
                continue
            last = section.contents[-1]
            if instruction_ends[last[0]] in self.local_tables and last[2] not in ("ret", "jmp"):
                return asm
            for _, _, mnemonic, operands in section.contents:
                if re.fullmatch(r"(?:call|j\w+|loop\w*)", mnemonic) and operands.startswith("0x"):
                    target = int(operands, 16)
                    if start_addr <= target < cursor and target not in instruction_ends:
                        return asm
        for address, line in asm:
            jump = re.fullmatch(r"(?:j\w+|loop\w*) (-?0x[0-9a-f]+)", line)
            if jump:
                if instruction_ends[address] + int(jump[1], 16) not in instruction_ends:
                    return asm
        # Equal instruction offsets preserve relative branches and return sites.
        # Capture actual operands before FunctionComparator's assertion fixup.
        self.signature = (len(data), [
            (address - start_addr if address is not None else None, line) for address, line in asm
        ])
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
