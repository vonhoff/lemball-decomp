"""Small binary fixtures for comparison adapters."""

import struct
from types import SimpleNamespace
from unittest.mock import Mock

from reccmp.compare.db import EntityDb, ReccmpMatch
from reccmp.compare.functions import FunctionComparator
from reccmp.types import EntityType, ImageId

THUNK = b"\xe9" + struct.pack("<i", 16)


def fixture(original="c3", rebuilt=None, thunk=THUNK, opcode="e8"):
    bodies = [
        bytes.fromhex(opcode + "fb2f0000 " + tail)
        for tail in (original, original if rebuilt is None else rebuilt)
    ]
    images = []
    for start, body in zip((0x1000, 0x2000), bodies, strict=True):
        images.append(
            SimpleNamespace(
                read=lambda read_address, size, image_start=start, image_body=body: (
                    image_body
                    if read_address == image_start
                    else thunk
                    if read_address == 0x4000
                    else b"\xc3"
                    if read_address == (0x4015 if image_start == 0x1000 else 0x5000)
                    else b""
                )[:size],
                imagebase=0,
                is_relocated_addr=lambda _address: False,
            )
        )
    lines = Mock()
    lines.find_line_of_recomp_address.return_value = None
    comparator = FunctionComparator(
        EntityDb(), lines, images[0], images[1], Mock(), Mock()
    )
    with comparator.db.batch() as batch:
        for side, address in ((ImageId.ORIG, 0x4015), (ImageId.RECOMP, 0x5000)):
            batch.set(side, address, type=EntityType.FUNCTION, name="Target", size=1)
        batch.match(0x4015, 0x5000)
    match = ReccmpMatch(
        0x1000,
        0x2000,
        {
            "name": "Caller",
            "type": EntityType.FUNCTION,
            "orig_size": len(bodies[0]),
            "recomp_size": len(bodies[1]),
        },
    )
    return comparator, match


def patch_body(image, start, offset, replacement):
    read = image.read

    def patched(address, size):
        data = read(address, size)
        if address == start:
            data = data[:offset] + replacement + data[offset + len(replacement) :]
        return data

    image.read = patched
