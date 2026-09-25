#!/usr/bin/env python3
"""Extract resource ids from a MOG v3 archive (pbaimog.vsr) into a manifest header."""

from __future__ import annotations

import re
import struct
from pathlib import Path

from .paths import ROOT


MOG_VERSION = 3
CHUNK_DIRC = 0x44495243
DEFAULT_VSR = ROOT / "data" / "pbaimog.vsr"
DEFAULT_HEADER = ROOT / "src" / "Visos" / "Resources" / "Manifest.h"


def fourcc(value: int) -> str:
    return value.to_bytes(4, "big").decode("latin-1").rstrip()


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

    def read_directory(self, file_offset: int) -> list[dict]:
        _, _, chunk_count, version, directory_end = struct.unpack_from(
            "<5I", self.data, file_offset
        )
        if version != MOG_VERSION:
            raise ValueError(f"{self.path}: expected MOG version {MOG_VERSION}, got {version}")
        payload_start = file_offset + 20

        dir_data = self.data[payload_start:directory_end]

        entries: list[dict] = []
        for index in range(chunk_count):
            # Index rows are 36 bytes each, starting at directory_end.
            data_rel, res_id, res_type, file_off, size = struct.unpack_from(
                "<5I", self.data, directory_end + index * 36
            )

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

    @staticmethod
    def _name_from_dir_data(dir_data: bytes, offset: int) -> str:
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


def write_header(resources: list[dict], out_path: Path) -> None:
    out_path.parent.mkdir(parents=True, exist_ok=True)
    lines = [
        "#ifndef LEMBALL_VISOS_RESOURCES_MANIFEST_H",
        "#define LEMBALL_VISOS_RESOURCES_MANIFEST_H",
        "",
        *(f"#define {entry['define']} {entry['id_hex']}" for entry in resources),
        "",
        "#endif",
        "",
    ]
    out_path.write_text("\n".join(lines), encoding="utf-8")


def generate_manifest(vsr_path: Path = DEFAULT_VSR, out_path: Path = DEFAULT_HEADER) -> int:
    if not vsr_path.is_file():
        return 1
    archive = MogArchive(vsr_path)
    resources = archive.collect_resources()
    if not resources:
        return 1
    write_header(resources, out_path)
    return 0
