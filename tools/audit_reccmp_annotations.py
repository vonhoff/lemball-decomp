#!/usr/bin/env python3
"""Reject source functions that reccmp cannot map to LEMBALL.EXE."""

from __future__ import annotations

import argparse
import csv
import re
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
ROADMAP = ROOT / "build" / "reccmp-annotation-audit-roadmap.csv"
RUNTIME_SYMBOLS = ROOT / "data" / "runtime-symbols.csv"
SOURCE_SUFFIXES = {".cpp", ".h", ".hpp"}
EXTERNAL_LIBRARIES = {
    "__purecall",
    "__setjmp3",
    "_memcpy",
    "_fopen",
    "_fclose",
    "_fread",
    "_fflush",
    "_fwrite",
    "_ftell",
    "_fseek",
}
COMPILER_RUNTIME_SYMBOLS = {
    "__rt_probe_read4@4",
    "$CRT_C_Initializer",
    "$CRT_C_Pre-Terminator",
    "__seh_longjmp_unwind@4",
}
LEGACY_MARKER = re.compile(
    r"^\s*/\*\s*(?:FUNCTION|STUB|LIBRARY|SYNTHETIC|TEMPLATE):",
    re.IGNORECASE,
)
STRICT_MARKER = re.compile(
    r"^// (?P<kind>FUNCTION|STUB|LIBRARY|SYNTHETIC|TEMPLATE): "
    r"(?P<target>[A-Z0-9_]+) 0x(?P<address>[0-9a-f]{8})(?: .*)?$"
)


def normalize_module(value: str) -> str:
    return value.replace("\\", "/").strip()


def compiler_generated(symbol: str) -> bool:
    return (
        symbol == "$CRT_CPP_Initializer"
        or "vcall" in symbol.lower()
        or symbol.endswith("::`scalar deleting destructor'")
        or symbol in COMPILER_RUNTIME_SYMBOLS
    )


def generate_roadmap(target: str, roadmap: Path) -> None:
    roadmap.parent.mkdir(parents=True, exist_ok=True)
    subprocess.run(
        ["reccmp-roadmap", "--target", target, "--csv", str(roadmap)],
        cwd=ROOT,
        check=True,
    )


def read_rebuilt_only(roadmap: Path) -> list[dict[str, str]]:
    rows: list[dict[str, str]] = []
    with roadmap.open(newline="", encoding="utf-8-sig") as stream:
        for row in csv.DictReader(stream):
            if row.get("row_type") != "fun":
                continue
            if row.get("orig_addr") or not row.get("recomp_addr"):
                continue
            rows.append(
                {
                    "module": normalize_module(row.get("module", "")),
                    "symbol": row.get("name", "").strip(),
                    "recomp_addr": row.get("recomp_addr", "").strip(),
                }
            )
    return rows


def marker_signature(lines: list[str], marker_index: int) -> str:
    parts: list[str] = []
    for line in lines[marker_index + 1 : marker_index + 25]:
        if STRICT_MARKER.match(line):
            break
        stripped = line.strip()
        if not stripped or stripped.startswith("//"):
            continue
        parts.append(stripped)
        if "{" in stripped or ";" in stripped:
            break
    return " ".join(parts)


def scan_sources(target: str) -> list[str]:
    errors: list[str] = []
    seen_addresses: dict[int, tuple[Path, int]] = {}
    for path in sorted((ROOT / "src").rglob("*")):
        if not path.is_file() or path.suffix.lower() not in SOURCE_SUFFIXES:
            continue
        lines = path.read_text(encoding="utf-8-sig").splitlines()
        anonymous_depth = 0
        brace_depth = 0
        for index, line in enumerate(lines):
            line_number = index + 1
            if LEGACY_MARKER.match(line):
                errors.append(f"{path}:{line_number}: block-style reccmp marker")

            marker = STRICT_MARKER.match(line)
            if marker is not None and marker.group("target") == target:
                address = int(marker.group("address"), 16)
                previous = seen_addresses.get(address)
                if previous is not None:
                    errors.append(
                        f"{path}:{line_number}: duplicate 0x{address:08x}; "
                        f"first at {previous[0]}:{previous[1]}"
                    )
                else:
                    seen_addresses[address] = path, line_number

                signature = marker_signature(lines, index)
                if re.search(r"(?:^|\s)static\s", signature):
                    errors.append(
                        f"{path}:{line_number}: annotated function has static linkage"
                    )
                if anonymous_depth:
                    errors.append(
                        f"{path}:{line_number}: annotated function is in anonymous namespace"
                    )

            code = line.split("//", 1)[0]
            if re.match(r"^\s*namespace\s*\{", code):
                anonymous_depth = brace_depth + 1
            brace_depth += code.count("{") - code.count("}")
            if anonymous_depth and brace_depth < anonymous_depth:
                anonymous_depth = 0
    return errors


def scan_runtime_symbols() -> list[str]:
    errors: list[str] = []
    with RUNTIME_SYMBOLS.open(newline="", encoding="utf-8-sig") as stream:
        reader = csv.DictReader(
            line for line in stream if line.strip() and not line.lstrip().startswith("#")
        )
        seen: set[str] = set()
        for line_number, row in enumerate(reader, start=3):
            name = row.get("name", "").strip()
            symbol = row.get("symbol", "").strip()
            kind = row.get("type", "").strip()
            if kind != "library":
                errors.append(
                    f"{RUNTIME_SYMBOLS}:{line_number}: non-library runtime symbol {name}"
                )
            if name not in EXTERNAL_LIBRARIES:
                errors.append(
                    f"{RUNTIME_SYMBOLS}:{line_number}: non-external runtime symbol {name}"
                )
            if not symbol:
                errors.append(
                    f"{RUNTIME_SYMBOLS}:{line_number}: unresolved external symbol {name}"
                )
            if name in seen:
                errors.append(
                    f"{RUNTIME_SYMBOLS}:{line_number}: duplicate external symbol {name}"
                )
            seen.add(name)
        missing = EXTERNAL_LIBRARIES - seen
        for name in sorted(missing):
            errors.append(f"{RUNTIME_SYMBOLS}: missing external library {name}")
    return errors


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("--target", default="LEMBALL")
    parser.add_argument("--roadmap", type=Path, default=ROADMAP)
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    roadmap = args.roadmap.resolve()
    generate_roadmap(args.target, roadmap)
    rebuilt_only = read_rebuilt_only(roadmap)
    generated = [row for row in rebuilt_only if compiler_generated(row["symbol"])]
    source_authored = [row for row in rebuilt_only if not compiler_generated(row["symbol"])]

    errors = scan_sources(args.target)
    errors.extend(scan_runtime_symbols())
    for row in source_authored:
        errors.append(
            "unmapped source function: "
            f"{row['module']}::{row['symbol']} ({row['recomp_addr']})"
        )

    if errors:
        for error in errors:
            print(error, file=sys.stderr)
        print(
            f"annotation audit failed: {len(source_authored)} source-authored; "
            f"{len(generated)} compiler-generated",
            file=sys.stderr,
        )
        return 1

    print(
        f"annotation audit clean: 0 source-authored; "
        f"{len(generated)} compiler-generated"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
