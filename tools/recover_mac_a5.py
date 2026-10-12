#!/usr/bin/env python3
"""Recover Paintball's original 68K resource fork and A5 world, read-only.

Dependencies: install with `python -m pip install machfs` (includes macresources).
Only supports the MSF1/tiny installer and 68K startup format on this CD image.
Format evidence: installer CODE5 logical seek at 0x11cc adds 0x2c;
installer CODE4 0x0c selects 'gzip', 0x1230 inflates a raw DEFLATE stream.
Game CODE1 0x10e decodes three DATA0 chunks; 0x212 decodes relocations;
0x286 applies three streams, twice (A5 world, then CODE1).
Synthetic A5 and CODE1 addresses are labels, not a running Mac memory map.
"""

import argparse
import hashlib
import json
from pathlib import Path
import struct
import zlib

import machfs
import macresources


def recover(iso: Path, dest: Path) -> dict:
    dest.mkdir(parents=True, exist_ok=True)
    image = iso.read_bytes()
    volume = machfs.Volume()
    volume.read(image)
    installer = volume["Install Paintball"]
    archive = bytes(installer.data)
    assert archive[:4] == b"MSF1"
    header_size = 0x2C
    logical = archive[header_size:]
    assert logical[:6] == b"tiny\x00\x02"
    assert int.from_bytes(archive[0x18:0x1C], "big") == len(logical)
    count = int.from_bytes(logical[6:10], "big")
    forks = {}
    for index in range(count):
        record = logical[10 + index * 0x5C : 10 + (index + 1) * 0x5C]
        name = record[1 : 1 + record[0]].decode("mac_roman")
        assert name in ["Lemmings Paintball (68K)", "Lemmings Paintball (PPC)"]
        assert record[0x52:0x56] == b"gzip"
        for kind, field in [("data", 0x3A), ("resource", 0x46)]:
            unpacked, packed, offset = struct.unpack_from(">III", record, field)
            if not unpacked:
                assert packed == 0
                continue
            source = logical[offset : offset + packed]
            assert len(source) == packed
            decoder = zlib.decompressobj(-15)
            fork = decoder.decompress(source) + decoder.flush()
            assert decoder.eof and not decoder.unused_data
            assert len(fork) == unpacked
            stem = "68k" if "68K" in name else "ppc"
            path = dest / ("mac-" + stem + "-" + kind + ".bin")
            path.write_bytes(fork)
            forks[name + ":" + kind] = {
                "path": str(path),
                "sha256": hashlib.sha256(fork).hexdigest(),
                "size": len(fork),
                "logical_offset": offset,
                "physical_offset": offset + header_size,
                "packed_size": packed,
            }
    (dest / "mac-installer-data.bin").write_bytes(archive)
    (dest / "mac-installer.rsrc").write_bytes(bytes(installer.rsrc))
    resources = {
        (r.type, r.id): bytes(r)
        for r in macresources.parse_file((dest / "mac-68k-resource.bin").read_bytes())
    }
    for (kind, number), data in resources.items():
        if kind in [b"DATA", b"CODE"]:
            (dest / ("mac-game-%s-%d.bin" % (kind.decode(), number))).write_bytes(data)
    data = resources[b"DATA", 0]
    code = bytearray(resources[b"CODE", 1])
    above, below = struct.unpack_from(">II", resources[b"CODE", 0])
    world = bytearray(below + above)
    source = 4
    chunks = []
    for _ in range(3):
        offset = struct.unpack_from(">i", data, source)[0]
        source += 4
        target = below + offset
        while True:
            op = data[source]
            source += 1
            if op & 128:
                n = (op & 127) + 1
                world[target : target + n] = data[source : source + n]
                source += n
                target += n
            elif op & 64:
                target += (op & 63) + 1
            elif op & 32:
                n = (op & 31) + 2
                world[target : target + n] = bytes([data[source]]) * n
                source += 1
                target += n
            elif op & 16:
                n = (op & 15) + 1
                world[target : target + n] = b"\xff" * n
                target += n
            elif op == 0:
                break
            elif op in [1, 2]:
                target += 4
                prefix = b"\xff\xff" if op == 1 else b"\xff"
                n = 4 - len(prefix)
                world[target : target + 4] = prefix + data[source : source + n]
                source += n
                target += 4
            elif op in [3, 4]:
                world[target : target + 2] = b"\xa9\xf0"
                target += 4 if op == 3 else 3
                n = 2 if op == 3 else 3
                world[target : target + n] = data[source : source + n]
                source += n
                target += n + 1
                world[target] = data[source]
                source += 1
                target += 1
            else:
                raise ValueError((op, source))
        chunks.append({"start": offset, "end": target - below, "source_end": source})
    (dest / "mac-game-A5-unrelocated.bin").write_bytes(world)
    a5, code_base = 0x20000000 + below, 0x10100000
    relocations, streams = [], []
    for name, buffer, origin, own_base in [
        ("A5", world, below, a5),
        ("CODE1", code, 0, code_base),
    ]:
        for kind, base in [("A5", a5), ("CODE1", code_base), (name, own_base)]:
            count = struct.unpack_from(">I", data, source)[0]
            source += 4
            offset = 0
            for _ in range(count):
                x = data[source]
                source += 1
                if x & 128:
                    val = (x << 1) & 255
                    offset += val - 256 if val & 128 else val
                elif x & 64:
                    val = ((x << 8) | data[source]) << 2 & 65535
                    source += 1
                    if val & 32768:
                        val -= 65536
                    offset += val >> 1
                else:
                    val = (
                        (x << 24) | int.from_bytes(data[source : source + 3], "big")
                    ) << 2 & 0xFFFFFFFF
                    source += 3
                    if val & 0x80000000:
                        val -= 0x100000000
                    offset = val >> 1
                position = origin + offset
                raw = struct.unpack_from(">I", buffer, position)[0]
                value = (raw + base) & 0xFFFFFFFF
                struct.pack_into(">I", buffer, position, value)
                relocations.append(
                    dict(
                        target=name,
                        offset=offset,
                        kind=kind,
                        raw=hex(raw),
                        value=hex(value),
                    )
                )
            streams.append(dict(target=name, kind=kind, count=count, source_end=source))
    assert len(world) == below + above
    assert source == len(data)
    assert int.from_bytes(data[:4], "big") == streams[2]["source_end"]
    (dest / "mac-game-A5-relocated.bin").write_bytes(world)
    (dest / "mac-game-CODE-1-relocated.bin").write_bytes(code)
    (dest / "mac-game-relocations.json").write_text(json.dumps(relocations, indent=2))
    evidence = dict(
        iso=str(iso.resolve()),
        iso_sha256=hashlib.sha256(image).hexdigest(),
        installer_data_sha256=hashlib.sha256(archive).hexdigest(),
        installer_resource_sha256=hashlib.sha256(installer.rsrc).hexdigest(),
        forks=forks,
        data0_sha256=hashlib.sha256(data).hexdigest(),
        a5=hex(a5),
        a5_file_index=hex(below),
        code1=hex(code_base),
        chunks=chunks,
        relocation_streams=streams,
        final_source=source,
        data_size=len(data),
        a5_unrelocated_sha256=hashlib.sha256(
            (dest / "mac-game-A5-unrelocated.bin").read_bytes()
        ).hexdigest(),
        a5_relocated_sha256=hashlib.sha256(world).hexdigest(),
        code1_relocated_sha256=hashlib.sha256(code).hexdigest(),
    )
    (dest / "mac-recovery-evidence.json").write_text(json.dumps(evidence, indent=2))
    return evidence


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("iso", type=Path)
    parser.add_argument("dest", type=Path)
    args = parser.parse_args()
    print(json.dumps(recover(args.iso, args.dest), indent=2))
