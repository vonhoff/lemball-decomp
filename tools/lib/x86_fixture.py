"""Shared Windows x86 execution support for behavior fixtures."""

import hashlib
import struct
from dataclasses import dataclass

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from reccmp.formats.pe import PEImage
from reccmp.project.detect import RecCmpProject
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE, UC_HOOK_MEM_WRITE
from unicorn.x86_const import (
    UC_X86_REG_EAX,
    UC_X86_REG_EBX,
    UC_X86_REG_ECX,
    UC_X86_REG_EDX,
    UC_X86_REG_ESI,
    UC_X86_REG_EDI,
    UC_X86_REG_EBP,
    UC_X86_REG_ESP,
    UC_X86_REG_EIP,
    UC_X86_REG_EFLAGS,
)

from . import BUILD, TARGET_ID
from .compare import Compare

HEAP = 0x01000000
HEAP_SIZE = 0x10000
STACK = 0x02000000
STACK_SIZE = 0x10000
STACK_POINTER = STACK + 0x8000
STOP = 0x03000000
PRESERVED_REGISTERS = (UC_X86_REG_EBX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP)
PRESERVED_VALUES = (0x11223344, 0x55667788, 0x99AABBCC, 0x13579BDF)


def dword(value):
    return struct.pack("<I", value & 0xFFFFFFFF)


def load(memory, address, size=4):
    offset = address - HEAP
    if not 0 <= offset <= len(memory) - size:
        raise ValueError(f"Fixture read outside heap: {address:#x}, size {size}")
    return int.from_bytes(memory[offset : offset + size], "little")


def put(memory, address, value):
    struct.pack_into("<I", memory, address - HEAP, value & 0xFFFFFFFF)


def reference_build():
    """Use the existing project reference hash and current PDB mappings."""
    target = RecCmpProject.from_directory(BUILD).get(TARGET_ID)
    original_hash = hashlib.sha256(target.original_path.read_bytes()).hexdigest()
    if original_hash.lower() != target.sha256.lower():
        raise ValueError(
            "Original executable does not match the existing project reference hash"
        )
    provenance = {
        "images": {
            "original": original_hash,
            "rebuilt": hashlib.sha256(target.recompiled_path.read_bytes()).hexdigest(),
        },
        "pdb_sha256": hashlib.sha256(target.recompiled_pdb.read_bytes()).hexdigest(),
    }
    return Compare.from_target(target), provenance


@dataclass
class Execution:
    heap: bytes
    writes: list[tuple[int, int, int]]
    eax: int


class X86Fixture:
    """Execute mapped PE code; check thiscall cleanup and preserved registers."""

    def __init__(self, image):
        if not isinstance(image, PEImage):
            raise ValueError("Native fixtures require Windows x86 PE images")
        self.image = image
        self.machine = Uc(UC_ARCH_X86, UC_MODE_32)
        self.writes = []
        self.visited = set()
        machine = self.machine
        image_size = (image.optional_header.size_of_image + 4095) & ~4095
        machine.mem_map(image.imagebase, image_size)
        for section in image.sections:
            machine.mem_write(section.virtual_address, bytes(section.view))
        machine.mem_map(HEAP, HEAP_SIZE)
        machine.mem_map(STACK, STACK_SIZE)
        machine.mem_map(STOP, 4096)
        machine.hook_add(UC_HOOK_MEM_WRITE, self.record_write)
        machine.hook_add(UC_HOOK_CODE, self.record_instruction)
        self.context = machine.context_save()

    def instruction_addresses(self, entry, size):
        return {
            instruction.address
            for instruction in Cs(CS_ARCH_X86, CS_MODE_32).disasm(
                self.image.read(entry, size), entry
            )
            if instruction.mnemonic != "int3"
        }

    def record_instruction(self, _machine, address, _size, _user_data):
        self.visited.add(address)

    def record_write(self, _machine, _access, address, size, value, _user_data):
        if HEAP <= address <= HEAP + HEAP_SIZE - size:
            self.writes.append((address, size, value & ((1 << (8 * size)) - 1)))
        elif not STACK <= address <= STACK + STACK_SIZE - size:
            raise ValueError(
                f"Unexpected write outside fixture heap/stack: {address:#x}"
            )

    def run(self, entry, memory, this, arguments=(), global_values=()):
        machine = self.machine
        machine.context_restore(self.context)
        machine.mem_write(HEAP, bytes(memory))
        machine.mem_write(STACK, bytes(STACK_SIZE))
        machine.mem_write(STACK_POINTER, dword(STOP) + b"".join(map(dword, arguments)))
        for address, value in global_values:
            machine.mem_write(address, dword(value))
        self.writes = []
        for register, value in zip(PRESERVED_REGISTERS, PRESERVED_VALUES, strict=True):
            machine.reg_write(register, value)
        for register, value in (
            (UC_X86_REG_EAX, 0xABCDEF01),
            (UC_X86_REG_ECX, this),
            (UC_X86_REG_EDX, 0x23456789),
            (UC_X86_REG_ESP, STACK_POINTER),
            (UC_X86_REG_EFLAGS, 0x202),
        ):
            machine.reg_write(register, value)
        machine.emu_start(entry, STOP, count=10000)
        if machine.reg_read(UC_X86_REG_EIP) != STOP:
            raise ValueError("Native fixture did not return within 10000 instructions")
        if machine.reg_read(UC_X86_REG_ESP) != STACK_POINTER + 4 * (len(arguments) + 1):
            raise ValueError("Native fixture changed the thiscall RET contract")
        if tuple(machine.reg_read(r) for r in PRESERVED_REGISTERS) != PRESERVED_VALUES:
            raise ValueError("Native fixture clobbered a preserved register")
        return Execution(
            bytes(machine.mem_read(HEAP, HEAP_SIZE)),
            self.writes,
            machine.reg_read(UC_X86_REG_EAX),
        )
