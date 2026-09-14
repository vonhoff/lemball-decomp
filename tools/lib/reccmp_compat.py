"""Repository-local compatibility fixes for the pinned reccmp 0.1.7 parser.

These fixes correct instruction/data boundaries and relocation recognition.
The existing name replacement and matching criteria remain in use.
The installed package is not modified.
"""

from importlib.metadata import version
from copy import copy
import struct
import re

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from reccmp.compare.asm import fixes, parse
from reccmp.compare import functions
from reccmp.compare.asm.instgen import InstructGen, SectionType
from reccmp.formats.exceptions import InvalidVirtualAddressError, InvalidVirtualReadError
from reccmp.types import ImageId


def bounded_switch_edges(image, address, start, limit):
    """Recognize the complete range guard and two-level MSVC switch dispatch.

    Accept only CMP EAX,maximum; JA default; XOR ECX,ECX;
    MOV CL,[EAX+byte_table]; JMP [ECX*4+target_table]. Both tables must
    reside inside the known function bound. The caller treats this sequence
    atomically, so another edge into its interior invalidates the proof.
    """
    def read(at, count):
        if count <= 0 or not start <= at < at + count <= limit:
            raise ValueError("Switch data is outside the function bound")
        data = bytes(image.read(at, count))
        if len(data) != count:
            raise ValueError("Switch data is truncated")
        return data

    try:
        raw = bytes(image.read(address, min(32, limit - address)))
        if raw[:2] == b"\x83\xf8" and len(raw) >= 3 and raw[2] < 0x80:
            maximum, cursor = raw[2], 3
        elif raw[:1] == b"\x3d" and len(raw) >= 5:
            maximum, cursor = struct.unpack_from("<I", raw, 1)[0], 5
        else:
            return None
        if maximum > 255:
            return None
        if raw[cursor:cursor + 1] == b"\x77" and len(raw) >= cursor + 2:
            displacement = struct.unpack_from("<b", raw, cursor + 1)[0]
            cursor += 2
        elif raw[cursor:cursor + 2] == b"\x0f\x87" and len(raw) >= cursor + 6:
            displacement = struct.unpack_from("<i", raw, cursor + 2)[0]
            cursor += 6
        else:
            return None
        default = address + cursor + displacement
        if (len(raw) < cursor + 15
                or raw[cursor:cursor + 2] not in (b"\x33\xc9", b"\x31\xc9")
                or raw[cursor + 2:cursor + 4] != b"\x8a\x88"
                or raw[cursor + 8:cursor + 11] != b"\xff\x24\x8d"):
            return None
        byte_table = struct.unpack_from("<I", raw, cursor + 4)[0]
        target_table = struct.unpack_from("<I", raw, cursor + 11)[0]
        indices = read(byte_table, maximum + 1)
        targets = read(target_table, (max(indices) + 1) * 4)
        edges = {default}
        edges.update(struct.unpack_from("<I", targets, index * 4)[0] for index in indices)
        if any(not start <= edge < limit for edge in edges):
            return None
        return cursor + 15, edges, (
            (byte_table, len(indices)), (target_table, len(targets))
        )
    except (ValueError, IndexError, struct.error, InvalidVirtualAddressError, InvalidVirtualReadError):
        return None


def complete_original_extent(image, start, limit, decoder):
    """Find a closed control-flow extent, or decline uncertain code.

    Never cross the next known entity. Indirect jumps require a fully verified
    bounded switch dispatch; all other indirect jumps are declined. Calls
    retain their fallthrough edge. All reachable instructions must decode and
    all branch targets must be instruction boundaries inside the bounded span.
    """
    pending = [start]
    instructions = set()
    occupied = set()
    data_bytes = set()
    saw_return = False
    while pending:
        address = pending.pop()
        if address in instructions:
            continue
        if not start <= address < limit or address in occupied or address in data_bytes:
            return None
        switch = bounded_switch_edges(image, address, start, limit)
        if switch is not None:
            size, edges, tables = switch
            span = set(range(address, address + size))
            table_spans = [set(range(at, at + count)) for at, count in tables]
            if (span.intersection(occupied | data_bytes)
                    or table_spans[0].intersection(table_spans[1])
                    or any(table.intersection(occupied | span | data_bytes) for table in table_spans)):
                return None
            instructions.add(address)
            occupied.update(span)
            data_bytes.update(table_spans[0] | table_spans[1])
            pending.extend(edges)
            continue
        try:
            raw = image.read(address, min(15, limit - address))
        except (ValueError, IndexError, InvalidVirtualAddressError, InvalidVirtualReadError):
            return None
        instruction = next(decoder.disasm_lite(raw, address), None)
        if instruction is None:
            return None
        _, size, mnemonic, operand = instruction
        span = set(range(address, address + size))
        if (occupied | data_bytes).intersection(span):
            return None
        instructions.add(address)
        occupied.update(span)
        if mnemonic in ("ret", "retf"):
            saw_return = True
            continue
        if mnemonic in ("int3", "hlt", "ud2", "int", "iret", "iretd", "sysenter", "sysexit"):
            return None
        if mnemonic.startswith("j") or mnemonic.startswith("loop"):
            if not re.fullmatch(r"0x[0-9a-f]+", operand):
                return None
            target = int(operand, 16)
            if not start <= target < limit:
                return None
            pending.append(target)
            if mnemonic == "jmp":
                continue
        pending.append(address + size)
    return max(occupied | data_bytes) + 1 - start if saw_return else None


def extend_original_match(match, image, decoder):
    """Extend a guessed original read length only with complete CFG evidence.

    Explicit original sizes, unknown boundaries, and undecodable functions keep
    upstream behavior. This never shortens a read or mutates database/report
    sizes; scoring and instruction normalization are unchanged.
    """
    if match.size(ImageId.ORIG) is not None:
        return match
    maximum = match.max_size(ImageId.ORIG)
    rebuilt = match.size(ImageId.RECOMP)
    if maximum is None or rebuilt is None or maximum <= rebuilt:
        return match
    extent = complete_original_extent(
        image, match.orig_addr, match.orig_addr + min(maximum, 65536), decoder
    )
    if extent is None or extent <= rebuilt:
        return match
    extended = copy(match)
    extended._kvstore = dict(match._kvstore, orig_size=extent)
    return extended


def configure_original_extents(engine):
    comparator = engine.function_comparator
    upstream = comparator.compare_function
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)

    def compare_function(match):
        return upstream(extend_original_match(match, comparator.orig_bin, decoder))

    comparator.compare_function = compare_function

_upstream_relocate_instructions = fixes.relocate_instructions
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


def table_end_expressions(data, start, relocation_sites):
    """Recognize a bounded literal-table scan, not an adjacent global access.

    The accepted sequence initializes EAX with a relocated table pointer,
    compares [EAX] with ECX, exits on equality, advances EAX by a positive
    stride, increments ESI, and compares EAX with a relocated end pointer.
    JB must return to that same key comparison. All other code is untouched.
    """
    expressions = {}
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    for address, _, _, _ in decoder.disasm_lite(data, start):
        offset = address - start
        if offset + 20 > len(data):
            continue
        loop = data[offset:offset + 20]
        if not (
            loop[0] == 0xB8
            and loop[5:8] == b"\x39\x08\x74"
            and 9 + loop[8] >= 20 and loop[8] < 0x80
            and loop[9:11] == b"\x83\xc0"
            and 0 < loop[11] < 0x80
            and loop[12:14] == b"\x46\x3d"
            and loop[18:20] == b"\x72\xf1"
            and start + offset + 1 in relocation_sites
            and start + offset + 14 in relocation_sites
        ):
            continue
        begin = struct.unpack_from("<I", loop, 1)[0]
        end = struct.unpack_from("<I", loop, 14)[0]
        length = end - begin
        if 0 < length <= 65536 and length % loop[11] == 0:
            expressions[start + offset + 13] = (begin, length)
    return expressions


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


class RelocationAwareParseAsm(parse.ParseAsm):
    """Recognize CMP pointer immediates at verified PE relocation sites."""

    def __init__(self, *, relocation_sites=(), **kwargs):
        super().__init__(**kwargs)
        self.relocation_sites = frozenset(relocation_sites)
        self._decoder = Cs(CS_ARCH_X86, CS_MODE_32)
        self._decoder.detail = True

    def parse_asm(self, data, start_addr):
        self._data = bytes(data)
        self._start = start_addr
        self._table_ends = table_end_expressions(self._data, start_addr, self.relocation_sites)
        assembly = super().parse_asm(data, start_addr)
        normalize_assert_arguments(assembly)
        return assembly

    def sanitize(self, inst):
        address, size, mnemonic, operands = inst
        if (
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
                if address in self._table_ends:
                    begin, length = self._table_ends[address]
                    return mnemonic, head + separator + f"{self.replace(begin)} + {length:#x}"
                return mnemonic, head + separator + self.replace(value)
        return super().sanitize(inst)


def configure_pointer_comparisons(engine) -> None:
    comparator = engine.function_comparator
    for side in ("orig", "recomp"):
        image = getattr(comparator, side + "_bin")
        parser = getattr(comparator, side + "_sanitize")
        setattr(
            comparator,
            side + "_sanitize",
            RelocationAwareParseAsm(
                relocation_sites=image.relocations,
                addr_test=parser.addr_test,
                name_lookup=parser.name_lookup,
                is_32bit=parser.is_32bit,
            ),
        )


class BoundedInstructGen(InstructGen):
    """Stop address tables at code boundaries discovered from their entries."""

    def _next_section(self, addr: int) -> SectionType | None:
        section_type = super()._next_section(addr)
        if section_type == SectionType.ADDR_TAB:
            # Upstream snapshots read_size before discovering code targets. A
            # target can shorten section_end, leaving instructions in that saved
            # table slice. Discover boundaries first, checking the updated end
            # before each dword; upstream then reads only the bounded table.
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
    fixes.assert_fixup = normalize_assert_arguments
    functions.assert_fixup = normalize_assert_arguments
