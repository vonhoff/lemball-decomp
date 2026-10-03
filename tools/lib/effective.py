"""Direct jump-thunk equivalence for the Effective metric."""

import re
import struct
from dataclasses import dataclass, replace

from reccmp.compare.asm.instgen import InstructGen, SectionType
from reccmp.compare.asm.parse import ParseAsm
from reccmp.compare.functions import FunctionComparator
from reccmp.formats.exceptions import InvalidVirtualAddressError, InvalidVirtualReadError
from reccmp.types import EntityType

from . import BUILD
from .compare_flags import (
    control_flow_targets, indirect_jumps_use_tables, normalize_compare_branches,
    prefix_overwrites_flags,
)

EFFECTIVE_JSON = BUILD / "effective.json"


@dataclass
class AssemblySignature:
    size: int
    instructions: list
    guarded_comparisons: list

    def matches(self, other):
        """Keep strict identity independent of optional comparison proofs."""
        return other is not None and self.size == other.size and (
            self.instructions == other.instructions or self.guarded_comparisons == other.guarded_comparisons
        )


def _relative_instructions(asm, start):
    return [(address - start if address is not None else None, line) for address, line in asm]


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


def _valid_control_flow(sections, ends, start, size):
    """Keep internal transfers on code boundaries and out of table padding."""
    instructions = [inst for section in sections if section.type == SectionType.CODE
                    for inst in section.contents]
    table_targets = {target for section in sections if section.type == SectionType.ADDR_TAB
                     for _, target in section.contents}
    if not table_targets <= ends.keys():
        return False
    targets = control_flow_targets(sections, {inst[0]: inst for inst in instructions}) | {start}
    if any(start <= target < start + size and target not in ends for target in targets):
        return False
    tables = {section.contents[0][0]: section.type for section in sections
              if section.type != SectionType.CODE and section.contents}
    padding = _table_padding(sections, tables)
    if padding is None or padding & targets:
        return False
    return not padding or indirect_jumps_use_tables(instructions, tables)


def _comparison_signature(asm, sections, start, size, check_call):
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
    guarded = normalize_compare_branches(asm, sections, check_call)
    return AssemblySignature(size, _relative_instructions(asm, start), _relative_instructions(guarded, start))


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

    def _thunk_target(self, address):
        if address in self.targets:
            return None
        try:
            raw = self.image.read(address, 5)
        except (InvalidVirtualAddressError, InvalidVirtualReadError):
            return None
        if len(raw) != 5 or raw[0] != 0xe9:
            return None
        target = address + 5 + struct.unpack_from("<i", raw, 1)[0]
        return target if target in self.targets else None

    def _call_overwrites_flags(self, address):
        """Inspect a paired callee, resolving at most one verified E9 thunk."""
        target = self._thunk_target(address) or address
        if target not in self.targets:
            return False
        try:
            data = self.image.read(target, 32)
        except (InvalidVirtualAddressError, InvalidVirtualReadError):
            return False
        return prefix_overwrites_flags(bytes(data), target)

    def sanitize(self, inst):
        _, _, mnemonic, operands = inst
        if mnemonic in ("call", "jmp") and operands.startswith("0x"):
            target = self._thunk_target(int(operands, 16))
            name = self.lookup(target, exact=True) if target is not None else None
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
        self.signature = _comparison_signature(asm, sections, start_addr, len(data), self._call_overwrites_flags)
        return asm


def additional_effective_matches(engine, comparisons):
    upstream: FunctionComparator = engine.function_comparator
    comparator = replace(upstream)
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
        if (comparison is None or comparison.is_stub
                or comparison.accuracy == 1 or comparison.is_effective_match):
            continue
        comparator.compare_function(match)
        if (original.signature is not None and original.signature.matches(rebuilt.signature)
                and (original.used_thunk or rebuilt.used_thunk)):
            matches[match.orig_addr] = ("verified jump thunk target",)
            if original.signature.instructions != rebuilt.signature.instructions:
                matches[match.orig_addr] += ("verified comparison operands and flag lifetimes",)
    return matches


def effective_addresses(comparisons, additional=()):
    """Function addresses counted by the Effective metric."""
    return {
        address for address, comparison in comparisons.items()
        if comparison.is_function() and comparison.is_matched() and not comparison.is_stub
        and (comparison.accuracy == 1 or comparison.is_effective_match or address in additional)
    }
