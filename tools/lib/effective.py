"""Additional equivalence checks used only by the Effective metric."""

import re
import struct
from dataclasses import fields

from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from reccmp.compare.asm.instgen import InstructGen, SectionType
from reccmp.compare.asm.parse import ParseAsm
from reccmp.compare.asm.fixes import DWORD_REGS, find_effective_match
from reccmp.compare.pinned_sequences import SequenceMatcherWithPins
from reccmp.formats.exceptions import InvalidVirtualAddressError, InvalidVirtualReadError
from reccmp.types import EntityType, ImageId

from . import BUILD
from .extents import FullFunctionComparator, complete_instruction_stream

EFFECTIVE_JSON = BUILD / "effective.json"
REGISTER_FAMILIES = {
    alias: family
    for family, aliases in (
        ("eax", ("eax", "ax", "al", "ah")),
        ("ebx", ("ebx", "bx", "bl", "bh")),
        ("ecx", ("ecx", "cx", "cl", "ch")),
        ("edx", ("edx", "dx", "dl", "dh")),
        ("esi", ("esi", "si")), ("edi", ("edi", "di")),
        ("ebp", ("ebp", "bp")), ("esp", ("esp", "sp")),
    )
    for alias in aliases
}
VPTR_STORE = re.compile(
    r"mov dword ptr \[(e(?:bx|si|di|bp))(?: \+ (0x[0-9a-f]+|[0-9]+))?\], (.+)"
)


def incremental_thunks(image):
    """Verify a section-leading E9 table closed by linker padding."""
    thunks = {}
    for region in image.get_code_regions():
        offset, entries = 0, {}
        while offset + 5 <= len(region.data) and region.data[offset] == 0xe9:
            address = region.addr + offset
            entries[address] = address + 5 + struct.unpack_from("<i", region.data, offset + 1)[0]
            offset += 5
        code_start = region.addr + offset + 16
        code_end = region.addr + len(region.data)
        if (entries and region.data[offset:offset + 16] == b"\xcc" * 16
                and all(code_start <= target < code_end for target in entries.values())):
            thunks.update(entries)
    return thunks


class EffectiveParseAsm(ParseAsm):
    """Resolve verified thunk identities in the secondary comparison."""

    def __init__(self, *, image, function_targets, thunk_targets, indirect_targets, **kwargs):
        super().__init__(**kwargs)
        self.image = image
        self.function_targets = function_targets
        self.thunk_targets = thunk_targets
        self.indirect_targets = indirect_targets
        self.reasons = set()

    def parse_asm(self, data, start_addr):
        self.reasons.clear()
        return super().parse_asm(data, start_addr)

    def lookup(self, addr, exact=False, indirect=False):
        name = super().lookup(addr, exact=exact, indirect=indirect)
        if name is not None:
            return name
        target = (self.indirect_targets if indirect else self.thunk_targets).get(addr)
        if target is not None and (name := super().lookup(target, exact=True)) is not None:
            self.reasons.add("verified linker thunk target")
            return "->" + name if indirect else name
        return None

    def sanitize(self, inst):
        _, _, mnemonic, operands = inst
        if mnemonic not in ("call", "jmp") or not re.fullmatch(r"0x[0-9a-f]+", operands):
            return super().sanitize(inst)
        address = int(operands, 16)
        if self.lookup(address, exact=True) is not None:
            return super().sanitize(inst)
        try:
            raw = self.image.read(address, 5)
        except (ValueError, IndexError, InvalidVirtualAddressError, InvalidVirtualReadError):
            return super().sanitize(inst)
        if len(raw) != 5 or raw[0] != 0xe9:
            return super().sanitize(inst)
        target = address + 5 + struct.unpack_from("<i", raw, 1)[0]
        name = super().lookup(target, exact=True)
        if target in self.function_targets and name is not None and name.endswith(" (FUNCTION)"):
            self.reasons.add("verified linker thunk target")
            return mnemonic, self.replace(target, exact=True)
        return super().sanitize(inst)


def zero_registers(generator, decoder):
    """Straight-line zero facts, invalidated at joins, writes, and ABI clobbers."""
    code_sections = [
        section for section in generator.sections
        if section.type == SectionType.CODE and section.contents
    ]
    entries = {
        address for address, kind in generator.confirmed_addrs.items()
        if kind == SectionType.CODE
    }
    entries.update(section.contents[0][0] for section in code_sections)
    tables = {
        section.contents[0][0] for section in generator.sections
        if section.type == SectionType.ADDR_TAB and section.contents
    }
    instructions = []
    code = (instruction for section in code_sections for instruction in section.contents)
    for address, size, mnemonic, operands in code:
        # CMP and TEST differ in AF. Reject functions that can observe it.
        if mnemonic in ("lahf", "pushf", "pushfd", "aaa", "aas", "daa", "das"):
            return {}
        is_branch = mnemonic.startswith(("j", "loop"))
        if (is_branch or mnemonic == "call") and re.fullmatch(r"0x[0-9a-f]+", operands):
            entries.add(int(operands, 16))
        elif is_branch:
            table = re.fullmatch(r"dword ptr \[e\w+\*4 \+ (0x[0-9a-f]+)\]", operands)
            if table is None or int(table[1], 16) not in tables:
                return {}
        offset = address - generator.start
        instructions.append(next(decoder.disasm(generator.blob[offset:offset + size], address)))
    return _zero_register_facts(instructions, entries)


def _zero_register_facts(instructions, entries):
    """Track register values within the validated straight-line instruction spans."""
    known_zero = set()
    zero_before = {}
    for instruction in instructions:
        if instruction.address in entries:
            known_zero.clear()
        zero_before[instruction.address] = known_zero.copy()
        _, writes = instruction.regs_access()
        known_zero.difference_update(
            REGISTER_FAMILIES.get(instruction.reg_name(register)) for register in writes
        )
        if instruction.mnemonic == "call":
            known_zero.difference_update(("eax", "ecx", "edx"))
        elif instruction.mnemonic.startswith(("j", "loop", "ret", "int")):
            known_zero.clear()
        elif instruction.mnemonic == "xor":
            left, _, right = instruction.op_str.partition(", ")
            if left == right and left in DWORD_REGS:
                known_zero.add(left)
    return zero_before


def zero_cmp_test(original, rebuilt, index, original_zero, rebuilt_zero):
    """CMP against a proved zero and TEST of the same register, with the same Jcc."""
    cmp_operands = re.fullmatch(
        r"cmp (e(?:ax|bx|cx|dx|si|di|bp)), (e(?:bx|cx|si|di|bp))", original[index]
    )
    if cmp_operands is None:
        return False
    tested_register, zero_register = cmp_operands.groups()
    if rebuilt[index] != f"test {tested_register}, {tested_register}":
        return False
    if zero_register not in original_zero or zero_register not in rebuilt_zero:
        return False
    for line, other in zip(original[index + 1:index + 4], rebuilt[index + 1:index + 4]):
        if line != other:
            return False
        mnemonic = line.partition(" ")[0]
        if mnemonic in ("mov", "lea", "nop", "push"):
            continue
        return mnemonic in ("je", "jne", "jl", "jle", "jg", "jge", "ja", "jae", "jb", "jbe")
    return False


def dead_vptr_store(original, rebuilt, index):
    """A differing construction vptr overwritten before any read or control transfer."""
    original_store = VPTR_STORE.fullmatch(original[index])
    rebuilt_store = VPTR_STORE.fullmatch(rebuilt[index])
    if original_store is None or rebuilt_store is None:
        return False
    destination = original_store.group(1, 2)
    if (destination != rebuilt_store.group(1, 2)
            or not re.fullmatch(r"<OFFSET[0-9]+>", original_store[3])
            or not rebuilt_store[3].endswith(" (VTABLE)")):
        return False
    base, offset = original_store[1], int(original_store[2] or "0", 0)
    for next_index in range(index + 1, min(len(original), index + 4)):
        line = original[next_index]
        if line != rebuilt[next_index]:
            return False
        store = VPTR_STORE.fullmatch(line)
        if store is None:
            lea = re.fullmatch(r"lea (e[a-z]+), \[.*\]", line)
            if lea is None or lea[1] == base:
                return False
            continue
        if store.group(1, 2) == destination:
            return store[3].endswith(" (VTABLE)")
        if store[1] != base or abs(int(store[2] or "0", 0) - offset) < 4 or "[" in store[3]:
            return False
    return False


class EffectiveFunctionComparator(FullFunctionComparator):
    """Secondary comparison used only to classify additional Effective functions."""

    def __init__(self, **kwargs):
        self.reasons = set()
        self.orig_zero = {}
        self.recomp_zero = {}
        super().__init__(**kwargs)

    def __post_init__(self):
        super().__post_init__()
        if self.is_32bit:
            self.orig_sanitize = self._make_parser(ImageId.ORIG, self.orig_bin, self.orig_sanitize)
            self.recomp_sanitize = self._make_parser(ImageId.RECOMP, self.recomp_bin, self.recomp_sanitize)

    def _make_parser(self, image_id, image, upstream_parser):
        """Build thunk identities restricted to paired functions and relocations."""
        function_targets = {
            entity.addr(image_id) for entity in self.db.get_matches_by_type(EntityType.FUNCTION)
        }
        thunk_targets = {
            address: target for address, target in incremental_thunks(image).items()
            if target in function_targets
        }
        indirect_targets = {
            address: thunk_targets[value] for address in getattr(image, "relocations")
            if (value := int.from_bytes(image.read(address, 4), "little")) in thunk_targets
        }
        return EffectiveParseAsm(
            image=image, function_targets=function_targets,
            thunk_targets=thunk_targets, indirect_targets=indirect_targets,
            addr_test=upstream_parser.addr_test, name_lookup=upstream_parser.name_lookup,
            is_32bit=self.is_32bit,
        )

    def compare_function(self, match):
        self.reasons.clear()
        self.orig_zero = {}
        self.recomp_zero = {}
        if self.is_32bit:
            decoder = Cs(CS_ARCH_X86, CS_MODE_32)
            decoder.detail = True
            with complete_instruction_stream():
                self.orig_zero = zero_registers(
                    InstructGen(self.orig_bin.read(match.orig_addr, match.size(ImageId.ORIG)), match.orig_addr),
                    decoder,
                )
                self.recomp_zero = zero_registers(
                    InstructGen(self.recomp_bin.read(match.recomp_addr, match.size(ImageId.RECOMP)), match.recomp_addr),
                    decoder,
                )
        result = super().compare_function(match)
        if self.is_32bit and (result.match_ratio == 1 or result.is_effective_match):
            for parser in (self.orig_sanitize, self.recomp_sanitize):
                self.reasons.update(parser.reasons)
        return result

    def _compare_function_assembly(self, orig, recomp, split_points):
        # noinspection PyProtectedMember
        result = super()._compare_function_assembly(orig, recomp, split_points)
        if not self.is_32bit or result.match_ratio == 1 or result.is_effective_match or len(orig) != len(recomp):
            return result
        original, rebuilt = [line for _, line in orig], [line for _, line in recomp]
        patched = rebuilt.copy()
        reasons = set()
        for index, (left, right) in enumerate(zip(original, rebuilt)):
            if left == right:
                continue
            if dead_vptr_store(original, rebuilt, index):
                reason = "overwritten construction vtable store"
            elif zero_cmp_test(
                original, rebuilt, index,
                self.orig_zero.get(orig[index][0], ()),
                self.recomp_zero.get(recomp[index][0], ()),
            ):
                reason = "known-zero CMP versus TEST"
            else:
                continue
            patched[index] = left
            reasons.add(reason)
        if not reasons:
            return result
        checked = SequenceMatcherWithPins(original, patched, split_points)
        if checked.ratio() == 1 or find_effective_match(checked.get_opcodes(), original, patched):
            result.is_effective_match = True
            self.reasons.update(reasons)
        return result


def additional_effective_matches(engine, comparisons):
    """Return extra function addresses and reasons without editing either report."""
    upstream = engine.function_comparator
    comparator = EffectiveFunctionComparator(
        **{member.name: getattr(upstream, member.name) for member in fields(upstream)}
    )
    matches = {}
    for match in engine.get_functions():
        comparison = comparisons.get(match.orig_addr)
        if (comparison is None or not comparison.is_function() or not comparison.is_matched()
                or comparison.is_stub or comparison.accuracy == 1 or comparison.is_effective_match):
            continue
        result = comparator.compare_function(match)
        if (result.match_ratio == 1 or result.is_effective_match) and comparator.reasons:
            matches[match.orig_addr] = tuple(sorted(comparator.reasons))
    return matches


def effective_addresses(comparisons, additional=()):
    """Function addresses counted by the Effective metric."""
    return {
        address for address, comparison in comparisons.items()
        if comparison.is_function() and comparison.is_matched() and not comparison.is_stub
        and (comparison.accuracy == 1 or comparison.is_effective_match or address in additional)
    }
