#!/usr/bin/env python3
"""Extract resource ids from a MOG v3 archive (pbaimog.vsr) into a manifest header."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

MOG_VERSION = 3
CHUNK_DIRC = 0x44495243
DEFAULT_VSR = ROOT / "data" / "pbaimog.vsr"
DEFAULT_HEADER = ROOT / "src" / "Visos" / "Resources" / "Manifest.h"
DEFAULT_JSON = ROOT / "build-msvc400" / "resource_manifest.json"


def fourcc(value: int) -> str:
    return value.to_bytes(4, "big").decode("latin-1").rstrip()


def read_u32(data: bytes, offset: int) -> tuple[int, int]:
    return struct.unpack_from("<I", data, offset)[0], offset + 4


def sanitize_identifier(*parts: str) -> str:
    tokens: list[str] = []
    for part in parts:
        for segment in re.split(r"[/\\]+", part):
            segment = segment.strip()
            if not segment:
                continue
            cleaned = re.sub(r"[^0-9A-Za-z]+", "_", segment).strip("_").upper()
            if cleaned:
                tokens.append(cleaned)
    name = "_".join(tokens)
    if not name:
        name = "UNNAMED"
    if name[0].isdigit():
        name = f"_{name}"
    return name


def make_define_name(path: str, name: str, type_str: str) -> str:
    path_part = sanitize_identifier(path.strip("/")) if path.strip("/") else ""
    name_part = sanitize_identifier(name)
    type_part = sanitize_identifier(type_str)
    if path_part:
        return f"RES_{path_part}_{name_part}"
    return f"RES_{type_part}_{name_part}"


class MogArchive:
    def __init__(self, path: Path) -> None:
        self.path = path
        self.data = path.read_bytes()

    def u32(self, offset: int) -> tuple[int, int]:
        if offset + 4 > len(self.data):
            raise ValueError(f"{self.path}: unexpected EOF at 0x{offset:x}")
        return read_u32(self.data, offset)

    def read_directory(self, file_offset: int) -> list[dict]:
        # Root dir reads one byte then seeks back to 0 before the header dwords.
        offset = file_offset

        _, offset = self.u32(offset)  # unknown
        _, offset = self.u32(offset)  # unknown
        chunk_count, offset = self.u32(offset)
        version, offset = self.u32(offset)
        if version != MOG_VERSION:
            raise ValueError(f"{self.path}: expected MOG version {MOG_VERSION}, got {version}")
        directory_end, offset = self.u32(offset)
        payload_start = offset
        dir_data = self.data[payload_start:directory_end]

        entries: list[dict] = []
        for index in range(chunk_count):
            # Index rows are 36 bytes each, starting at directory_end.
            entry_offset = directory_end + index * 36
            data_rel, entry_offset = self.u32(entry_offset)
            res_id, entry_offset = self.u32(entry_offset)
            res_type, entry_offset = self.u32(entry_offset)
            file_off, entry_offset = self.u32(entry_offset)
            size, entry_offset = self.u32(entry_offset)

            name = self._name_from_dir_data(dir_data, data_rel - payload_start)

            entries.append(
                {
                    "id": res_id,
                    "type": res_type,
                    "type_str": fourcc(res_type),
                    "name": name,
                    "file_offset": file_off,
                    "size": size,
                }
            )

        return entries

    def _name_from_dir_data(self, dir_data: bytes, offset: int) -> str:
        if offset < 0 or offset >= len(dir_data):
            return ""
        return dir_data[offset:].split(b"\0", 1)[0].decode("latin-1", errors="replace")

    def collect_resources(self, file_offset: int = 0, path_prefix: str = "/") -> list[dict]:
        resources: list[dict] = []
        for entry in self.read_directory(file_offset):
            name = entry["name"]
            if entry["type"] == CHUNK_DIRC:
                child_path = f"{path_prefix.rstrip('/')}/{name}"
                resources.extend(self.collect_resources(entry["file_offset"], child_path))
                continue

            if not name:
                continue

            define_name = make_define_name(path_prefix, name, entry["type_str"])
            resources.append(
                {
                    "id": entry["id"],
                    "id_hex": f"0x{entry['id']:x}",
                    "type": entry["type_str"],
                    "name": name,
                    "path": path_prefix,
                    "define": define_name,
                    "size": entry["size"],
                }
            )

        resources.sort(key=lambda item: item["id"])
        return resources


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def write_json(resources: list[dict], vsr_path: Path, out_path: Path) -> None:
    out_path.parent.mkdir(parents=True, exist_ok=True)
    payload = {
        "source": vsr_path.name,
        "source_path": str(vsr_path.resolve()),
        "sha256": sha256_file(vsr_path),
        "count": len(resources),
        "entries": resources,
    }
    out_path.write_text(json.dumps(payload, indent=2) + "\n", encoding="utf-8")


def write_header(resources: list[dict], vsr_path: Path, out_path: Path) -> None:
    out_path.parent.mkdir(parents=True, exist_ok=True)
    digest = sha256_file(vsr_path)
    lines = [
        "// AUTO-GENERATED by tools/make_manifest.py - do not edit.",
        f"// Source: {vsr_path.name}",
        f"// SHA-256: {digest}",
        f"// Entries: {len(resources)}",
        "",
        "#ifndef LEMBALL_VISOS_RESOURCES_MANIFEST_H",
        "#define LEMBALL_VISOS_RESOURCES_MANIFEST_H",
        "",
    ]

    current_path = None
    for entry in resources:
        if entry["path"] != current_path:
            current_path = entry["path"]
            lines.append(f"// --- {current_path} ---")
        comment = f'{entry["type"]}  "{entry["name"]}"'
        lines.append(f"#define {entry['define']:<48} {entry['id_hex']:<8} // {comment}")

    lines.extend(["", "#endif", ""])
    out_path.write_text("\n".join(lines), encoding="utf-8")


def resolve_vsr(path: Path | None) -> Path:
    if path is not None:
        candidate = path
    else:
        candidate = DEFAULT_VSR
    if not candidate.is_file():
        raise FileNotFoundError(f"VSR not found: {candidate}")
    return candidate


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Generate Manifest.h from pbaimog.vsr")
    parser.add_argument(
        "vsr",
        nargs="?",
        type=Path,
        default=None,
        help=f"MOG archive path (default: {DEFAULT_VSR.relative_to(ROOT)})",
    )
    parser.add_argument(
        "--header",
        type=Path,
        default=DEFAULT_HEADER,
        help=f"Output header path (default: {DEFAULT_HEADER.relative_to(ROOT)})",
    )
    parser.add_argument(
        "--json",
        type=Path,
        default=DEFAULT_JSON,
        help=f"Output JSON path (default: {DEFAULT_JSON.relative_to(ROOT)})",
    )
    parser.add_argument(
        "--header-only",
        action="store_true",
        help="Write only the header file",
    )
    parser.add_argument(
        "--json-only",
        action="store_true",
        help="Write only the JSON manifest",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    try:
        vsr_path = resolve_vsr(args.vsr)
    except FileNotFoundError as exc:
        print(exc, file=sys.stderr)
        print("Place pbaimog.vsr in data/ or pass an explicit path.", file=sys.stderr)
        return 1

    archive = MogArchive(vsr_path)
    resources = archive.collect_resources()

    if not resources:
        print(f"{vsr_path}: no resources found", file=sys.stderr)
        return 1

    write_header_flag = not args.json_only
    write_json_flag = not args.header_only
    if write_header_flag:
        write_header(resources, vsr_path, args.header)
        print(f"Wrote {args.header} ({len(resources)} entries)")
    if write_json_flag:
        write_json(resources, vsr_path, args.json)
        print(f"Wrote {args.json} ({len(resources)} entries)")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
