"""Direct jump-thunk equivalence for the Effective metric."""

import re
import struct

from reccmp.compare.asm.instgen import InstructGen, SectionType
from reccmp.compare.asm.parse import ParseAsm
from reccmp.compare.functions import FunctionComparator
from reccmp.formats.exceptions import InvalidVirtualAddressError, InvalidVirtualReadError
from reccmp.types import EntityType

from . import BUILD
from .compare_flags import normalize_compare_branches

EFFECTIVE_JSON = BUILD / "effective.json"


def _instruction_ends(sections, start, size):
    """Require contiguous decoding of every code and table byte."""
    cursor = start
    ends = {}
    for section in sections:
        width = 4 if section.type == SectionType.ADDR_TAB else 1
        for entry in section.contents:
            address = entry[0]
            if address != cursor:
                return None
            if section.type == SectionType.CODE:
                cursor += entry[1]
                ends[address] = cursor
            else:
                cursor += width
    return ends if cursor == start + size else None


def _is_alignment_instruction(instruction):
    """Recognize the compiler's NOP and register-identity padding forms."""
    _, _, mnemonic, operands = instruction
    if mnemonic == "nop":
        return True
    if mnemonic not in ("mov", "lea"):
        return False
    destination, _, source = operands.partition(", ")
    if not re.fullmatch(r"e(?:ax|bx|cx|dx|si|di|sp|bp)", destination):
        return False
    return source == (destination if mnemonic == "mov" else f"[{destination}]")


def _table_padding(sections, tables):
    """Find alignment after terminal instructions; reject fallthrough into tables."""
    padding = set()
    for section in sections:
        if section.type != SectionType.CODE or not section.contents:
            continue
        last = section.contents[-1]
        if last[0] + last[1] not in tables:
            continue
        index = len(section.contents) - 1
        while index >= 0 and _is_alignment_instruction(section.contents[index]):
            padding.add(section.contents[index][0])
            index -= 1
        if index < 0 or section.contents[index][2] not in ("ret", "jmp"):
            return None
    return padding


def _indirect_jumps_use_tables(instructions, tables):
    """Unknown computed jumps could reach otherwise unused padding."""
    for _, _, mnemonic, operands in instructions:
        if mnemonic != "jmp" or operands.startswith("0x"):
            continue
        table = re.fullmatch(r"dword ptr \[(?:e[a-z]{2}\*4 \+ )?(0x[0-9a-f]+)]", operands)
        if table is None or tables.get(int(table[1], 16)) != SectionType.ADDR_TAB:
            return False
    return True


def _valid_control_flow(sections, ends, start, size):
    """Keep internal transfers on code boundaries and out of table padding."""
    instructions = [inst for section in sections if section.type == SectionType.CODE
                    for inst in section.contents]
    table_targets = {target for section in sections if section.type == SectionType.ADDR_TAB
                     for _, target in section.contents}
    if not table_targets <= ends.keys():
        return False
    targets = table_targets | {start}
    for _, _, mnemonic, operands in instructions:
        if re.fullmatch(r"call|j\w+|loop\w*", mnemonic) and operands.startswith("0x"):
            targets.add(int(operands, 16))
    if any(start <= target < start + size and target not in ends for target in targets):
        return False
    tables = {section.contents[0][0]: section.type for section in sections
              if section.type != SectionType.CODE and section.contents}
    padding = _table_padding(sections, tables)
    if padding is None or padding & targets:
        return False
    return not padding or _indirect_jumps_use_tables(instructions, tables)


def _comparison_signature(asm, sections, start, size):
    """Capture verified operands and offsets before upstream assertion fixups."""
    ends = _instruction_ends(sections, start, size)
    if ends is None or any("<OFFSET" in line for _, line in asm):
        return None
    if not _valid_control_flow(sections, ends, start, size):
        return None
    for address, line in asm:
        jump = re.fullmatch(r"(?:j\w+|loop\w*) (-?0x[0-9a-f]+)", line)
        if jump and ends[address] + int(jump[1], 16) not in ends:
            return None
    asm = normalize_compare_branches(asm, sections)
    return size, [(address - start if address is not None else None, line) for address, line in asm]


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

    def _thunk_name(self, address):
        if address in self.targets:
            return None
        try:
            raw = self.image.read(address, 5)
        except (InvalidVirtualAddressError, InvalidVirtualReadError):
            return None
        if len(raw) != 5 or raw[0] != 0xe9:
            return None
        target = address + 5 + struct.unpack_from("<i", raw, 1)[0]
        return self.lookup(target, exact=True) if target in self.targets else None

    def sanitize(self, inst):
        _, _, mnemonic, operands = inst
        if mnemonic in ("call", "jmp") and operands.startswith("0x"):
            name = self._thunk_name(int(operands, 16))
            if name is not None:
                # E9 changes only EIP; ordinary function pointers retain their identity.
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
        self.signature = _comparison_signature(asm, sections, start_addr, len(data))
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
