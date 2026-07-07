#!/usr/bin/env python3

from __future__ import annotations

import re
from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class SourceInventoryRow:
    file: str
    anchor_address: str
    anchor_function: str
    status: str | None = None


def load_lines(path: Path) -> list[str]:
    if not path.exists():
        return []

    lines: list[str] = []
    for raw_line in path.read_text().splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue
        lines.append(line)
    return lines


def find_original_binary(version: str) -> Path | None:
    candidates = [
        Path("orig") / version / "LEMBALL.EXE",
        Path("../LemBall_Decompilation/LEMBALL/LEMBALL.EXE"),
        Path("../OpenPaintball/reference/LEMBALL.EXE"),
    ]
    for candidate in candidates:
        if candidate.exists():
            return candidate.resolve()
    return None


def load_source_inventory(path: Path) -> list[SourceInventoryRow]:
    rows: list[SourceInventoryRow] = []
    for line in load_lines(path):
        parts = line.split(None, 3)
        if len(parts) < 3:
            continue
        rows.append(
            SourceInventoryRow(
                file=parts[0],
                anchor_address=parts[1],
                anchor_function=parts[2],
                status=parts[3] if len(parts) >= 4 else None,
            )
        )
    return rows


def list_cpp_sources(root: Path) -> list[Path]:
    sources = list(root.rglob("*.CPP"))
    sources.extend(root.rglob("*.cpp"))
    return sorted({path.resolve() for path in sources})


def unit_name_from_source(path: Path) -> str:
    return path.stem.upper()


def load_target_slice_units(path: Path) -> list[str]:
    units: list[str] = []
    for line in load_lines(path):
        parts = re.split(r"\s+", line, maxsplit=3)
        if len(parts) < 3:
            continue
        units.append(parts[0].upper())
    return units
