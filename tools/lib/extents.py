"""Original extent validation and x86 alignment boundaries for reccmp."""

from contextlib import contextmanager
from dataclasses import fields

from reccmp.compare.asm import instgen
from reccmp.compare.asm.instgen import InstructGen, SectionType, get_disassembler
from reccmp.compare.csv import csv_parse
from reccmp.compare.functions import FunctionComparator
from reccmp.formats.exceptions import InvalidVirtualAddressError
from reccmp.types import EntityType, ImageId

from lib import ROOT

TARGET_SIZES = ROOT / "tools/data/original-function-sizes.csv"
FUNCTION_TYPES = (EntityType.FUNCTION, EntityType.VTORDISP)
ALIGNMENT_ENCODINGS = {
    ("lea", f"{register}, [{register}]")
    for register in ("eax", "ebx", "ecx", "edx", "esi", "edi", "esp", "ebp")
} | {("mov", "edi, edi"), ("add", "eax, 0")}


def load_target_sizes(path=TARGET_SIZES):
    """Reject ambiguous ownership before upstream CSV ingestion."""
    sizes = {}
    for address, values in csv_parse(path.read_text(encoding="utf-8")):
        size = values.get("size", 0)
        if size <= 0 or address in sizes:
            raise ValueError(f"Invalid or duplicate target extent at 0x{address:08x}")
        sizes[address] = size
    validate_extents(sizes)
    return sizes


def validate_extents(sizes):
    """Each original byte belongs to at most one reported symbol."""
    previous_end = 0
    for address, size in sorted(sizes.items()):
        if address < previous_end:
            raise ValueError(f"Overlapping original function extent at 0x{address:08x}")
        previous_end = address + size


def original_functions(engine):
    """Yield upstream function kinds with original addresses in the PE sections."""
    for entity in engine.get_all():
        if entity.entity_type not in FUNCTION_TYPES or entity.orig_addr is None:
            continue
        try:
            engine.orig_bin.get_relative_addr(entity.orig_addr)
        except InvalidVirtualAddressError:
            continue
        yield entity


def target_size(entity):
    """The upstream comparison and byte weight must share one original extent."""
    size = entity.size(ImageId.ORIG)
    if size is None or size <= 0:
        raise ValueError(
            f"Missing original function extent at 0x{entity.orig_addr:08x}"
        )
    return size


def is_alignment(instruction):
    """MSVC 4.00 alignment encodings; only used after a terminal instruction."""
    mnemonic, operands = instruction[2:]
    return mnemonic in ("nop", "int3") or (mnemonic, operands) in ALIGNMENT_ENCODINGS


def branch_targets(sections):
    """Yield direct control-flow destinations and switch-table entries."""
    for section in sections:
        if section.type == SectionType.ADDR_TAB:
            yield from (target for _, target in section.contents)
        elif section.type == SectionType.CODE:
            for _, _, mnemonic, operands in section.contents:
                if (mnemonic.startswith("j") or mnemonic == "call") and operands.startswith("0x"):
                    yield int(operands, 16)


def alignment_extent(data, address):
    """Remove a fully decoded, untargeted alignment suffix; retain tables and code."""
    with complete_instruction_stream():
        sections = InstructGen(bytes(data), address).sections
    if not sections or sections[-1].type != SectionType.CODE:
        return len(data)
    instructions = sections[-1].contents
    last = len(instructions) - 1
    while last >= 0 and is_alignment(instructions[last]):
        last -= 1
    if last < 0 or instructions[last][2] not in ("ret", "retf", "jmp"):
        return len(data)
    end = instructions[last][0] + instructions[last][1]
    tail = list(get_disassembler().disasm_lite(bytes(data[end - address :]), end))
    if (
        not tail
        or sum(instruction[1] for instruction in tail) != len(data) - (end - address)
        or not all(is_alignment(instruction) for instruction in tail)
    ):
        return len(data)
    if any(end <= target < address + len(data) for target in branch_targets(sections)):
        return len(data)
    return end - address


def _all_instructions(instructions):
    return instructions


@contextmanager
def complete_instruction_stream():
    """Scoped reccmp 0.1.7 workaround for intentional in-function INT3."""
    previous = instgen.stop_at_int3
    instgen.stop_at_int3 = _all_instructions
    try:
        yield
    finally:
        instgen.stop_at_int3 = previous


def decoded_extent(data, address):
    """Byte endpoint actually consumed by the upstream code/table pre-parser."""
    with complete_instruction_stream():
        sections = InstructGen(bytes(data), address).sections
    end = address
    for section in sections:
        if section.contents:
            last = section.contents[-1]
            width = (
                last[1]
                if section.type == SectionType.CODE
                else (4 if section.type == SectionType.ADDR_TAB else 1)
            )
            end = max(end, last[0] + width)
    return end - address


class FullFunctionComparator(FunctionComparator):
    """Keep intentional INT3 inside explicit extents (the original CRT _assert)."""

    def compare_function(self, match):
        # reccmp 0.1.7 stops at *every* INT3, including reachable breakpoint code.
        # Single-threaded commands; restore the upstream decoder hook on all exits.
        with complete_instruction_stream():
            return super().compare_function(match)


def validate_original_functions(engine):
    """Require non-overlapping, fully decoded original code without trailing alignment."""
    functions = list(original_functions(engine))
    validate_extents({entity.orig_addr: target_size(entity) for entity in functions})
    for entity in functions:
        size = target_size(entity)
        data = engine.orig_bin.read(entity.orig_addr, size)
        if alignment_extent(data, entity.orig_addr) != size:
            raise ValueError(
                f"Original extent includes trailing alignment at 0x{entity.orig_addr:08x}"
            )
        if decoded_extent(data, entity.orig_addr) != size:
            raise ValueError(
                f"Original extent contains undecoded bytes at 0x{entity.orig_addr:08x}"
            )
    return functions


def trim_rebuilt_alignment(engine, functions):
    """Keep executable rebuilt bytes; exclude only untargeted alignment suffixes."""
    with engine.function_comparator.db.batch() as batch:
        for entity in functions:
            if entity.recomp_addr is None:
                continue
            rebuilt_size = entity.size(ImageId.RECOMP)
            if rebuilt_size is None:
                continue
            rebuilt_data = engine.recomp_bin.read(entity.recomp_addr, rebuilt_size)
            size = alignment_extent(rebuilt_data, entity.recomp_addr)
            if size != rebuilt_size:
                batch.set(ImageId.RECOMP, entity.recomp_addr, size=size)


def prepare_function_extents(engine):
    """Validate extents, trim rebuilt padding, and retain intentional INT3 in comparisons."""
    functions = validate_original_functions(engine)
    trim_rebuilt_alignment(engine, functions)
    comparator = engine.function_comparator
    engine.function_comparator = FullFunctionComparator(
        **{field.name: getattr(comparator, field.name) for field in fields(comparator)}
    )
