"""Repository-local compatibility fixes for the pinned reccmp 0.1.7 parser.

These fixes correct instruction/data boundaries and relocation recognition.
The existing name replacement and matching criteria remain in use.
The installed package is not modified.
"""

from __future__ import annotations

import logging
import re
import struct
from importlib.metadata import version

from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from reccmp.compare import Compare
from reccmp.compare import functions as function_compare
from reccmp.compare.asm import fixes, parse
from reccmp.compare.asm.instgen import InstructGen, SectionType
from reccmp.compare.pinned_sequences import SequenceMatcherWithPins
from reccmp.formats.exceptions import (
    InvalidVirtualAddressError,
    InvalidVirtualReadError,
)
from reccmp.project.detect import RecCmpProject
from reccmp.types import EntityType, ImageId

from .paths import BUILD


def complete_original_extent(image, start, limit, decoder):
    """Find a closed control-flow extent, or decline uncertain code.

    Never cross the next known entity. All reachable instructions must decode
    and all branch targets must be instruction boundaries inside the bounded span.
    """
    pending = [start]
    instructions = set()
    occupied = set()
    saw_return = False
    while pending:
        address = pending.pop()
        if address in instructions:
            continue
        if not start <= address < limit or address in occupied:
            return None
        try:
            raw = image.read(address, min(15, limit - address))
        except (ValueError, IndexError, InvalidVirtualAddressError, InvalidVirtualReadError):
            return None
        instruction = next(decoder.disasm_lite(raw, address), None)
        if instruction is None:
            return None
        _, size, mnemonic, operand = instruction
        span = set(range(address, address + size))
        if occupied.intersection(span):
            return None
        instructions.add(address)
        occupied.update(span)
        if mnemonic in ("ret", "retf"):
            saw_return = True
            continue
        if mnemonic in ("int3", "hlt", "ud2", "int", "iret", "iretd", "sysenter", "sysexit"):
            return None
        if mnemonic.startswith(("j", "loop")):
            if not re.fullmatch(r"0x[0-9a-f]+", operand):
                return None
            target = int(operand, 16)
            if not start <= target < limit:
                return None
            pending.append(target)
            if mnemonic == "jmp":
                continue
        pending.append(address + size)
    return max(occupied) + 1 - start if saw_return else None


_upstream_relocate_instructions = fixes.relocate_instructions
_upstream_patch_mov_compare_jmp = fixes.patch_mov_compare_jmp
_upstream_patch_compare_jmp = fixes.patch_compare_jmp
_upstream_find_effective_match = fixes.find_effective_match


def _zero_cmp_test(original, rebuilt, index):
    """Accept CMP x, 0 / TEST x, x only with a proved zero and same Jcc."""
    match = re.fullmatch(r"cmp (e(?:ax|bx|cx|dx|si|di|bp)), (e(?:bx|cx|si|di|bp))",
                         original[index])
    if match is None or rebuilt[index] != f"test {match[1]}, {match[1]}":
        return False
    zero = match[2]
    aliases = {"ebx": ("ebx", "bx", "bl", "bh"), "ecx": ("ecx", "cx", "cl", "ch"),
               "esi": ("esi", "si"), "edi": ("edi", "di"), "ebp": ("ebp", "bp")}[zero]
    safe = {"mov", "movsx", "movzx", "lea", "push", "call", "cmp", "test",
            "add", "sub", "and", "or", "xor", "shl", "shr", "sar", "inc",
            "dec", "imul", "cdq", "nop"}
    for assembly in (original, rebuilt):
        for line in reversed(assembly[:index]):
            mnemonic, _, operands = line.partition(" ")
            if line == f"xor {zero}, {zero}":
                break
            if (mnemonic not in safe or (mnemonic == "call" and zero == "ecx")
                    or (operands.partition(", ")[0] in aliases
                        and mnemonic not in ("cmp", "test", "push"))):
                return False
        else:
            return False
    for line, other in zip(original[index + 1:index + 4], rebuilt[index + 1:index + 4]):
        if line != other:
            return False
        mnemonic = line.partition(" ")[0]
        if mnemonic in ("mov", "lea", "nop", "push"):
            continue
        return mnemonic in ("je", "jne", "jl", "jle", "jg", "jge", "ja", "jae", "jb", "jbe")
    return False


_vptr_store = re.compile(
    r"mov dword ptr \[(e(?:bx|si|di|bp))(?: \+ (0x[0-9a-f]+|[0-9]+))?\], (.+)"
)


def _dead_vptr_store(original, rebuilt, index):
    left = _vptr_store.fullmatch(original[index])
    right = _vptr_store.fullmatch(rebuilt[index])
    if (left is None or right is None or left.group(1, 2) != right.group(1, 2)
            or not re.fullmatch(r"<OFFSET[0-9]+>", left[3])
            or not right[3].endswith(" (VTABLE)")):
        return False
    base, offset = left[1], int(left[2] or "0", 0)
    for next_index in range(index + 1, min(len(original), index + 4)):
        line = original[next_index]
        if line != rebuilt[next_index]:
            return False
        store = _vptr_store.fullmatch(line)
        if store is not None:
            if store.group(1, 2) == left.group(1, 2):
                return store[3].endswith(" (VTABLE)")
            if store[1] != base or abs(int(store[2] or "0", 0) - offset) < 4 or "[" in store[3]:
                return False
        elif not re.fullmatch(r"lea (e[a-z]+), \[.*\]", line) or line.split(" ", 2)[1].rstrip(",") == base:
            return False
    return False


def find_effective_match(codes, original, rebuilt):
    if _upstream_find_effective_match(codes, original, rebuilt):
        return True
    if len(original) != len(rebuilt):
        return False
    normalized = rebuilt.copy()
    for index, (left, right) in enumerate(zip(original, rebuilt)):
        if left != right and (_zero_cmp_test(original, rebuilt, index)
                              or _dead_vptr_store(original, rebuilt, index)):
            normalized[index] = left
    matcher = SequenceMatcherWithPins(original, normalized, [])
    return _upstream_find_effective_match(matcher.get_opcodes(), original, normalized)


def _register_operand_pair(instruction):
    """Split a two-operand instruction with a register at either end.

    Register endpoints identify the separator without splitting commas inside
    normalized template names or string literals. An x86 CMP/TEST operand swap
    requires a register: memory-memory and immediate-first forms are invalid.
    """
    _, separator, operands = instruction.partition(" ")
    if not separator:
        return None
    for register in fixes.REGISTER_SET:
        if operands.startswith(register + ", "):
            return register, operands[len(register) + 2:]
        if operands.endswith(", " + register):
            return operands[:-len(register) - 2], register
    return None


def is_operand_swap(original, rebuilt):
    if original.partition(" ")[0] != rebuilt.partition(" ")[0]:
        return False
    left = _register_operand_pair(original)
    right = _register_operand_pair(rebuilt)
    return left is not None and right is not None and left != right and left == right[::-1]


def patch_compare_jmp(original, rebuilt, mnemonic):
    fixed = _upstream_patch_compare_jmp(original, rebuilt, mnemonic)
    if fixed and mnemonic == "test":
        index = next(i for i, line in enumerate(original) if line.startswith(mnemonic))
        # TEST is commutative: swapping operands does not reverse ordering flags.
        # Retain upstream eligibility, but never excuse a changed jump.
        if original[index + 1] != rebuilt[index + 1]:
            return set()
    return fixed


def patch_mov_compare_jmp(original, rebuilt, mnemonic):
    # Keep the upstream eligibility and jump-target checks, but require exact
    # source operands instead of accepting anagrams of the instruction text.
    fixed = _upstream_patch_mov_compare_jmp(original, rebuilt, mnemonic)
    if not fixed:
        return set()
    index = next(i for i, line in enumerate(original) if line.startswith(mnemonic))
    if mnemonic == "test" and original[index + 1] != rebuilt[index + 1]:
        return set()
    left_mov = _register_operand_pair(original[index - 1])
    right_mov = _register_operand_pair(rebuilt[index - 1])
    left_cmp = _register_operand_pair(original[index])
    right_cmp = _register_operand_pair(rebuilt[index])
    if any(pair is None for pair in (left_mov, right_mov, left_cmp, right_cmp)):
        return set()
    if left_mov[0] != right_mov[0] or left_mov[0] not in fixes.REGISTER_SET:
        return set()
    left = tuple(left_mov[1] if operand == left_mov[0] else operand for operand in left_cmp)
    right = tuple(right_mov[1] if operand == right_mov[0] else operand for operand in right_cmp)
    return {index - 1, index, index + 1} if left == right[::-1] else set()


def naive_register_replacement(orig_asm, recomp_asm):
    """Apply the existing register-width check to tokens, never string contents.

    The upstream substring replacement also rewrites symbol fragments (such
    as the 'ax' in 'max') and quoted string literals. Those are program data,
    not registers. No instruction reordering or new equivalence is added.
    """
    widths = {register: "~reg4" for register in fixes.DWORD_REGS}
    widths.update({register: "~reg2" for register in fixes.WORD_REGS})
    widths.update({register: "~reg1" for register in fixes.BYTE_REGS})
    token = re.compile(
        r'"(?:\\.|[^"\\])*"|(?<![\w:@?$])('
        + "|".join(widths) + r')(?![\w:@?$])'
        r'(?!\s+\((?:DATA|FUNCTION|IMPORT|IMPORT_THUNK|VTABLE|OFFSET|STRING|UNK)\))'
    )

    def scrub(line):
        return token.sub(lambda match: widths[match[1]] if match[1] else match[0], line)

    return {index for index, (original, rebuilt) in enumerate(zip(orig_asm, recomp_asm))
            if scrub(original) == scrub(rebuilt)}


_register_tokens = re.compile(r"\b(eax|ax|al|ah|ebx|bx|bl|bh|ecx|cx|cl|ch|edx|dx|dl|dh|esi|si|edi|di|ebp|bp|esp|sp)\b")
_register_families = {
    alias: family
    for family, aliases in (
        ("eax", ("eax", "ax", "al", "ah")),
        ("ebx", ("ebx", "bx", "bl", "bh")),
        ("ecx", ("ecx", "cx", "cl", "ch")),
        ("edx", ("edx", "dx", "dl", "dh")),
        ("esi", ("esi", "si")), ("edi", ("edi", "di")),
        ("ebp", ("ebp", "bp")), ("esp", ("esp", "sp")),
    ) for alias in aliases
}


def relocate_instructions(codes, orig_asm, recomp_asm):
    """Fix the forward-move self-dependency, with conservative safety checks."""
    fixed = _upstream_relocate_instructions(codes, orig_asm, recomp_asm)
    deletes = [i for code, i1, i2, _, _ in codes if code == "delete" for i in range(i1, i2)]
    inserts = [(i1, j) for code, i1, _, j1, j2 in codes if code == "insert" for j in range(j1, j2)]
    transparent = {"mov", "lea", "cmp", "test", "push", "pop", "add", "sub",
                   "inc", "dec", "and", "or", "xor", "shl", "shr", "sar"}
    for destination, j in inserts:
        if j in fixed:
            continue
        line = recomp_asm[j]
        candidates = [i for i in deletes if orig_asm[i] == line]
        if len(candidates) != 1 or sum(recomp_asm[k] == line for _, k in inserts) != 1:
            continue
        i = candidates[0]
        if destination <= i:
            continue
        mnemonic, _, operands = line.partition(" ")
        target, separator, source = operands.partition(", ")
        if mnemonic not in ("mov", "lea") or not separator or target not in fixes.DWORD_REGS:
            continue
        if mnemonic == "mov" and "[" in source:
            continue
        if mnemonic == "mov" and not (
            source in fixes.DWORD_REGS
            or re.fullmatch(r"-?(?:0x[0-9a-f]+|[0-9]+)|<OFFSET[0-9]*>", source)
            or re.search(r" \((?:DATA|VTABLE|UNK|FUNCTION|IMPORT|IMPORT_THUNK|STRING|OFFSET)\)$", source)
        ):
            # Segment/control registers have dependencies outside the GPR set.
            continue
        registers = {_register_families[reg] for reg in _register_tokens.findall(operands)}
        if "esp" in registers:
            continue
        crossed = orig_asm[i + 1:destination]
        if any(
            instruction.partition(" ")[0] not in transparent
            or registers.intersection(_register_families[reg] for reg in _register_tokens.findall(instruction))
            for instruction in crossed
        ):
            continue
        fixed.add(j)
    return fixed


def normalize_assert_arguments(assembly):
    """Normalize verified CRT assertion locations independently of PE debug flags.

    Keep the existing file/line exclusion, but require the actual CRT callee
    and three consecutive argument pushes. Never rewrite an expression or use
    Python's negative indices when the call appears near the start of a body.
    """
    for index, (_, instruction) in enumerate(assembly):
        if index < 3 or not re.fullmatch(
            r"call __?assert \((?:FUNCTION|IMPORT|IMPORT_THUNK)\)", instruction
        ):
            continue
        line, filename, expression = (assembly[index - i][1] for i in (3, 2, 1))
        if not re.fullmatch(r"push (?:0x[0-9a-f]+|[0-9]+|__LINE__)", line):
            continue
        string_push = r'push (?:".*" \(STRING\)|<OFFSET[0-9]*>)'
        if filename != "push __FILE__" and not re.fullmatch(string_push, filename):
            continue
        if not re.fullmatch(string_push, expression):
            continue
        assembly[index - 3] = (assembly[index - 3][0], "push __LINE__")
        assembly[index - 2] = (assembly[index - 2][0], "push __FILE__")


def incremental_thunks(image):
    """Read a section-leading E9 table, even when debug metadata was stripped."""
    result = {}
    for region in image.get_code_regions():
        offset, entries = 0, {}
        while offset + 5 <= len(region.data) and region.data[offset] == 0xe9:
            address = region.addr + offset
            entries[address] = address + 5 + struct.unpack_from("<i", region.data, offset + 1)[0]
            offset += 5
        # Linker padding closes the table; every destination must be later code.
        if (entries and region.data[offset:offset + 16] == b"\xcc" * 16
                and all(region.addr + offset + 16 <= target < region.addr + len(region.data)
                        for target in entries.values())):
            result.update(entries)
    return result


def direct_jump_target(image, address):
    """Return the target of an exact five-byte E9 forwarding body."""
    try:
        raw = image.read(address, 5)
    except (ValueError, IndexError, InvalidVirtualAddressError, InvalidVirtualReadError):
        return None
    if len(raw) != 5 or raw[0] != 0xe9:
        return None
    return address + 5 + struct.unpack_from("<i", raw, 1)[0]


class RelocationAwareParseAsm(parse.ParseAsm):
    """Recognize pointer operands and arithmetic at verified PE relocation sites."""

    def __init__(self, *, image=None, relocation_sites=(), thunk_targets=None,
                 indirect_thunk_targets=None, **kwargs):
        super().__init__(**kwargs)
        self.image = image
        self.relocation_sites = frozenset(relocation_sites)
        self.thunk_targets = thunk_targets or {}
        self.indirect_thunk_targets = indirect_thunk_targets or {}
        self._decoder = Cs(CS_ARCH_X86, CS_MODE_32)
        self._decoder.detail = True
        self._data: bytes = b""
        self._start: int = 0
        self._table_bases: dict[str, int] = {}
        self._data_targets: set[int] = set()
        self._strlen_addends: set[int] = set()

    def indirect_replace(self, addr):
        target = self.indirect_thunk_targets.get(addr)
        if target is not None and self.lookup(addr, exact=True, indirect=True) is None:
            name = self.lookup(target, exact=True)
            if name is not None:
                return "->" + name
        return super().indirect_replace(addr)

    def replace(self, addr, exact=False):
        target = self.thunk_targets.get(addr)
        if (target is not None and self.lookup(addr, exact=True) is None
                and self.lookup(target, exact=True) is not None):
            return super().replace(target, exact=True)
        return super().replace(addr, exact=exact)

    def parse_asm(self, data, start_addr):
        self._data = bytes(data)
        self._start = start_addr
        self._table_bases = {}
        self._data_targets = set()
        self._strlen_addends = set()
        if self.is_32bit:
            # The upstream parser skips sanitize() for instructions without
            # pointer operands. Inspect decoded code sections for this idiom.
            for section in parse.InstructGen(self._data, start_addr, True).sections:
                if section.type != SectionType.CODE:
                    continue
                for index in range(4, len(section.contents)):
                    window = section.contents[index - 4:index + 1]
                    if (window[-1][2] == "add"
                            and [(item[2], item[3]) for item in window[:-1]] == [
                                ("mov", "ecx, 0xffffffff"),
                                ("sub", "eax, eax"),
                                ("repne scasb", "al, byte ptr es:[edi]"),
                                ("not", "ecx"),
                            ]
                            and all(left[0] + left[1] == right[0]
                                    for left, right in zip(window, window[1:]))):
                        self._strlen_addends.add(window[-1][0])
        assembly = super().parse_asm(data, start_addr)
        normalize_assert_arguments(assembly)
        return assembly

    def sanitize(self, inst):
        address, size, mnemonic, operands = inst
        if self.is_32bit and mnemonic in ("call", "jmp") and re.fullmatch(r"0x[0-9a-f]+", operands):
            target_address = int(operands, 16)
            target = self.thunk_targets.get(target_address)
            if target is None and self.image is not None:
                target = direct_jump_target(self.image, target_address)
            if (target is not None and self.lookup(target_address, exact=True) is None
                    and self.lookup(target, exact=True) is not None):
                return mnemonic, self.replace(target, exact=True)
        if self.is_32bit and mnemonic == "mov":
            reg, _, val = operands.partition(", ")
            if reg in fixes.DWORD_REGS and address + size - 4 in self.relocation_sites:
                try:
                    base = int(val, 16)
                    self._table_bases[reg] = base
                    name = self.lookup(base, exact=True)
                    if name is not None and name.endswith(" (DATA)"):
                        self._data_targets.add(base)
                except ValueError:
                    pass
        elif (self.is_32bit and mnemonic == "add"
              and re.fullmatch(r"ecx, 0x[0-9a-f]+", operands)
              and address + size - 4 in self.relocation_sites
              and address in self._strlen_addends):
            value = int(operands.partition(", ")[2], 16)
            base = value + 1
            if base in self._data_targets:
                offset = address - self._start
                decoded = next(self._decoder.disasm(self._data[offset:offset + size], address))
                if (decoded.imm_size == 4
                        and address + decoded.imm_offset in self.relocation_sites):
                    # NOT ECX includes the scanned terminator. MSVC folds its
                    # subtraction into buffer - 1, which may land in a neighbor.
                    # Retain the exact data target and bias, not that neighbor's name.
                    super().sanitize(inst)
                    return mnemonic, f"ecx, {self.replace(base, exact=True)} - 0x1"
        elif (
            self.is_32bit
            and mnemonic == "cmp"
            and address + size - 4 in self.relocation_sites
        ):
            offset = address - self._start
            decoded = next(
                self._decoder.disasm(self._data[offset : offset + size], address)
            )
            if (
                decoded.imm_size == 4
                and address + decoded.imm_offset in self.relocation_sites
            ):
                # Use the normal pointer-name/placeholder path. The relocation
                # belongs to this operand, not merely to an equal value elsewhere.
                value = int(operands.rpartition(", ")[2], 16)
                mnemonic, sanitized = super().sanitize(inst)
                head, separator, _ = sanitized.rpartition(", ")
                reg = operands.partition(", ")[0]
                if reg in self._table_bases:
                    base = self._table_bases[reg]
                    if 0 < value - base <= 65536:
                        return mnemonic, head + separator + f"{self.replace(base)} + {value - base:#x}"
                return mnemonic, head + separator + self.replace(value)
        return super().sanitize(inst)


def configure_pointer_comparisons(engine) -> None:
    comparator = engine.function_comparator
    for side in ("orig", "recomp"):
        image = getattr(comparator, side + "_bin")
        parser = getattr(comparator, side + "_sanitize")
        image_id = ImageId.ORIG if side == "orig" else ImageId.RECOMP
        thunks = {
            address: target for address, target in incremental_thunks(image).items()
            if (entity := engine._db.get(image_id, target)) is not None
            and (entity.recomp_addr if side == "orig" else entity.orig_addr) is not None
        }
        indirect_thunks = {
            address: thunks[value]
            for address in image.relocations
            if (value := int.from_bytes(image.read(address, 4), "little")) in thunks
        }
        setattr(
            comparator,
            side + "_sanitize",
            RelocationAwareParseAsm(
                image=image,
                relocation_sites=image.relocations,
                thunk_targets=thunks,
                indirect_thunk_targets=indirect_thunks,
                addr_test=parser.addr_test,
                name_lookup=parser.name_lookup,
                is_32bit=parser.is_32bit,
            ),
        )


class BoundedInstructGen(InstructGen):
    """Bound jump tables by discovering target code boundaries first."""

    def _next_section(self, addr: int) -> SectionType | None:
        section_type = super()._next_section(addr)
        if section_type == SectionType.ADDR_TAB:
            cursor = addr
            while cursor + 4 <= self.section_end:
                (target,) = struct.unpack_from("<I", self.blob, cursor - self.start)
                self._insert_confirmed_addr(target, SectionType.CODE)
                cursor += 4
        return section_type


def install_parser_fix() -> None:
    if version("reccmp") != "0.1.7":
        raise RuntimeError("Review the parser compatibility fix before changing reccmp==0.1.7")
    parse.InstructGen = BoundedInstructGen
    fixes.relocate_instructions = relocate_instructions
    fixes.naive_register_replacement = naive_register_replacement
    fixes.is_operand_swap = is_operand_swap
    fixes.patch_compare_jmp = patch_compare_jmp
    fixes.patch_mov_compare_jmp = patch_mov_compare_jmp
    function_compare.find_effective_match = find_effective_match


def match_nested_vtables(db):
    """Preserve the inheritance path lost by MSVC's vtable display names."""
    candidates = {}
    for entity in db.unmatched(ImageId.RECOMP):
        if entity.get("type") == EntityType.VTABLE:
            candidates.setdefault(entity.get("symbol"), []).append(entity.recomp_addr)
    with db.batch() as batch:
        for entity in db.unmatched(ImageId.ORIG):
            if entity.get("type") != EntityType.VTABLE:
                continue
            name, base = entity.get("name", ""), entity.get("base_class", "")
            path = re.fullmatch(r"([A-Za-z_]\w*)'s `([A-Za-z_]\w*)", base or "")
            if path is None or re.fullmatch(r"[A-Za-z_]\w*", name) is None:
                continue
            symbol = f"??_7{name}@@6B{path[1]}@@{path[2]}@@@"
            addresses = candidates.get(symbol, [])
            if len(addresses) == 1:
                batch.match(entity.orig_addr, addresses.pop())


def load_engine() -> tuple[object, Compare]:
    install_parser_fix()
    project = RecCmpProject.from_directory(BUILD)
    target = project.get("LEMBALL")
    logging.getLogger("reccmp").setLevel(logging.WARNING)
    engine = Compare.from_target(target)
    match_nested_vtables(engine._db)
    configure_pointer_comparisons(engine)
    return target, engine
