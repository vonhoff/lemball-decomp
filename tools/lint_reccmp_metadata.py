#!/usr/bin/env python3
"""Validate reccmp CSV inventories and source annotation ownership."""

from __future__ import annotations

import csv
import pathlib
import re
import sys
from collections import defaultdict

import macintosh_naming

ROOT = pathlib.Path(__file__).resolve().parents[1]
PROJECT_FILE = ROOT / "reccmp-project.yml"
SOURCE_ROOT = ROOT / "src"
OBJDIFF_FILE = ROOT / "data" / "objdiff-functions.csv"
BUILD_FILE = ROOT / "CMakeLists.txt"
SOURCE_SUFFIXES = {".c", ".cc", ".cpp", ".h", ".hpp"}
ANNOTATION = re.compile(
    r"^\s*//\s+(?:FUNCTION|STUB|SYNTHETIC|TEMPLATE|LIBRARY|LINKERILT|GLOBAL|VTABLE):"
    r"\s+LEMBALL\s+(0x[0-9a-fA-F]+)\b"
)
ALLOWED_TYPES = {
    "function",
    "stub",
    "synthetic",
    "template",
    "library",
    "global",
    "vtable",
}
TYPE_POLICY = {
    "reccmp-source-symbols.csv": {"function", "stub"},
    "reccmp-compiler-generated.csv": {"function", "synthetic", "template"},
    "reccmp-linker-ilts.csv": {"stub"},
    "reccmp-library-symbols.csv": {"library"},
}


def configured_data_sources() -> list[pathlib.Path]:
    paths: list[pathlib.Path] = []
    in_data_sources = False
    for line in PROJECT_FILE.read_text(encoding="utf-8").splitlines():
        if line.strip() == "data-sources:":
            in_data_sources = True
            continue
        if not in_data_sources:
            continue
        match = re.match(r"^\s+-\s+(.+?)\s*$", line)
        if match:
            paths.append(ROOT / match.group(1))
            continue
        if line.strip() and len(line) - len(line.lstrip()) <= 4:
            break
    return paths


def main() -> int:
    errors: list[str] = []
    mac_symbols, correlations = macintosh_naming.load()
    errors.extend(
        f"Macintosh naming tracker: {error}"
        for error in macintosh_naming.check(mac_symbols, correlations)
    )
    configured = configured_data_sources()
    discovered = sorted((ROOT / "data").glob("reccmp-*.csv"))
    if set(configured) != set(discovered):
        missing = sorted(str(path.relative_to(ROOT)) for path in set(discovered) - set(configured))
        stale = sorted(str(path.relative_to(ROOT)) for path in set(configured) - set(discovered))
        if missing:
            errors.append(f"unconfigured CSV inventories: {', '.join(missing)}")
        if stale:
            errors.append(f"missing configured CSV inventories: {', '.join(stale)}")

    csv_addresses: dict[int, list[str]] = defaultdict(list)
    csv_count = 0
    for path in configured:
        if not path.is_file():
            continue
        with path.open(encoding="utf-8", newline="") as stream:
            rows = csv.DictReader(line for line in stream if not line.startswith("#"))
            required = {"address", "name", "symbol", "type", "source"}
            if rows.fieldnames is None or not required.issubset(rows.fieldnames):
                errors.append(f"{path.relative_to(ROOT)}: expected columns {sorted(required)}")
                continue
            for line_number, row in enumerate(rows, 3):
                csv_count += 1
                location = f"{path.relative_to(ROOT)}:{line_number}"
                try:
                    address = int(row["address"], 16)
                except (TypeError, ValueError):
                    errors.append(f"{location}: invalid address {row.get('address')!r}")
                    continue
                csv_addresses[address].append(location)
                entity_type = row["type"].strip().lower()
                if entity_type not in ALLOWED_TYPES:
                    errors.append(f"{location}: unsupported type {entity_type!r}")
                allowed = TYPE_POLICY.get(path.name)
                if allowed is not None and entity_type not in allowed:
                    errors.append(
                        f"{location}: type {entity_type!r} violates {path.name} policy"
                    )
                if not row["name"].strip() and not row["symbol"].strip():
                    errors.append(f"{location}: both name and symbol are empty")
                owner = row["source"].strip()
                if not owner:
                    errors.append(f"{location}: source owner is empty")
                elif owner.startswith("src/") and not (ROOT / owner).is_file():
                    errors.append(f"{location}: source owner does not exist: {owner}")

    for address, locations in sorted(csv_addresses.items()):
        if len(locations) > 1:
            errors.append(f"0x{address:08X}: duplicate CSV ownership: {', '.join(locations)}")

    source_addresses: dict[int, list[str]] = defaultdict(list)
    source_count = 0
    for path in SOURCE_ROOT.rglob("*"):
        if not path.is_file() or path.suffix.lower() not in SOURCE_SUFFIXES:
            continue
        for line_number, line in enumerate(
            path.read_text(encoding="utf-8", errors="ignore").splitlines(), 1
        ):
            match = ANNOTATION.match(line)
            if match is None:
                continue
            source_count += 1
            address = int(match.group(1), 16)
            source_addresses[address].append(f"{path.relative_to(ROOT)}:{line_number}")

    for address, locations in sorted(source_addresses.items()):
        if len(locations) > 1:
            errors.append(f"0x{address:08X}: duplicate source ownership: {', '.join(locations)}")
        if address in csv_addresses:
            errors.append(
                f"0x{address:08X}: CSV/source collision: "
                f"{', '.join(csv_addresses[address] + locations)}"
            )

    with OBJDIFF_FILE.open(encoding="utf-8-sig", newline="") as stream:
        for line_number, row in enumerate(csv.DictReader(stream), 2):
            unit = row["unit"].strip()
            if not unit or not (SOURCE_ROOT / unit).is_file():
                errors.append(
                    f"{OBJDIFF_FILE.relative_to(ROOT)}:{line_number}: "
                    f"objdiff unit does not exist: {unit or '<empty>'}"
                )

    compiled_sources = {
        (ROOT / match.group(1)).resolve()
        for match in re.finditer(r"^\s+(src/\S+\.(?:c|cc|cpp))\s*$", BUILD_FILE.read_text(encoding="utf-8"), re.MULTILINE | re.IGNORECASE)
    }
    source_include = re.compile(r'^\s*#include\s+"([^"]+\.(?:c|cc|cpp))"', re.IGNORECASE)
    for path in SOURCE_ROOT.rglob("*"):
        if not path.is_file() or path.suffix.lower() not in SOURCE_SUFFIXES:
            continue
        for line_number, line in enumerate(path.read_text(encoding="utf-8", errors="ignore").splitlines(), 1):
            match = source_include.match(line)
            if match is None:
                continue
            included = (path.parent / match.group(1)).resolve()
            if included in compiled_sources:
                errors.append(
                    f"{path.relative_to(ROOT)}:{line_number}: included source is also compiled: "
                    f"{included.relative_to(ROOT)}"
                )

    if errors:
        for error in errors:
            print(f"error: {error}", file=sys.stderr)
        return 1

    print(
        f"reccmp metadata: {csv_count} CSV entities, "
        f"{source_count} source annotations, no duplicate addresses"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
