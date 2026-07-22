#!/usr/bin/env python3
"""Reject source functions that reccmp cannot map to LEMBALL.EXE."""

from __future__ import annotations

import argparse
import csv
import re
import subprocess
import sys
from pathlib import Path

try:
    from function_inventory import load_runtime_symbols, symbol_category
except ModuleNotFoundError:
    from tools.function_inventory import load_runtime_symbols, symbol_category


ROOT = Path(__file__).resolve().parents[1]
ROADMAP = ROOT / "build" / "reccmp-annotation-audit-roadmap.csv"
MANIFEST = ROOT / "data" / "manifest.json"
RUNTIME_SYMBOLS = ROOT / "data" / "runtime-symbols.csv"
REBUILT_RUNTIME_SYMBOLS = ROOT / "data" / "rebuilt-runtime-symbols.csv"
SOURCE_SUFFIXES = {".cpp", ".h", ".hpp"}
EXTERNAL_SYMBOLS = {
    "WSAGetLastError": (0x0047B8D0, "import"),
    "__purecall": (0x0047FCA0, "runtime"),
    "_strchr": (0x0047FE00, "runtime"),
    "_WinMainCRTStartup": (0x0047FE20, "runtime"),
    "_strncmp": (0x00480010, "runtime"),
    "__setjmp3": (0x00480198, "runtime"),
    "_memcpy": (0x00480290, "runtime"),
    "_fopen": (0x00480420, "runtime"),
    "_fclose": (0x00480440, "runtime"),
    "_fread": (0x004804E0, "runtime"),
    "_fflush": (0x00480670, "runtime"),
    "_fwrite": (0x00480830, "runtime"),
    "_ftell": (0x004809F0, "runtime"),
    "_fseek": (0x00480BD0, "runtime"),
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


def scan_sources(
    target: str,
    manifest_categories: dict[int, str],
    runtime_addresses: set[int],
) -> list[str]:
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
                category = manifest_categories.get(address)
                if marker.group("kind") == "LIBRARY":
                    valid_address = (
                        category in {"runtime", "import", "external"}
                        and address in runtime_addresses
                    )
                else:
                    valid_address = category in {"internal", "thunk"}
                if not valid_address:
                    errors.append(
                        f"{path}:{line_number}: 0x{address:08x} marker kind "
                        f"{marker.group('kind')} conflicts with manifest category {category!r}"
                    )

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
    expected_names = set(EXTERNAL_SYMBOLS)
    with RUNTIME_SYMBOLS.open(newline="", encoding="utf-8-sig") as stream:
        reader = csv.DictReader(
            line for line in stream if line.strip() and not line.lstrip().startswith("#")
        )
        seen: set[str] = set()
        for line_number, row in enumerate(reader, start=3):
            name = row.get("name", "").strip()
            symbol = row.get("symbol", "").strip()
            kind = row.get("type", "").strip()
            manifest_category = row.get("manifest_category", "").strip()
            address_text = row.get("address", "").strip()
            try:
                address = int(address_text, 16)
            except ValueError:
                address = -1
                errors.append(
                    f"{RUNTIME_SYMBOLS}:{line_number}: invalid address {address_text!r}"
                )
            expected = EXTERNAL_SYMBOLS.get(name)
            if expected is None:
                errors.append(
                    f"{RUNTIME_SYMBOLS}:{line_number}: unknown external symbol {name}"
                )
            elif (
                address != expected[0]
                or kind != "library"
                or symbol_category(
                    {"type": kind, "manifest_category": manifest_category}
                )
                != expected[1]
            ):
                errors.append(
                    f"{RUNTIME_SYMBOLS}:{line_number}: ownership mismatch for {name}"
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
        missing = expected_names - seen
        for name in sorted(missing):
            errors.append(f"{RUNTIME_SYMBOLS}: missing external library {name}")
    return errors


def load_rebuilt_runtime_symbols() -> tuple[set[tuple[str, str]], list[str]]:
    """Load compiler-runtime functions emitted by the pinned MSVC runtime.

    These entries have no source body and reccmp reports them only on the rebuilt
    side.  Keeping them in a separate, exact module/symbol inventory prevents a
    broad name allow-list from hiding application-owned generated functions.
    """
    errors: list[str] = []
    declared: set[tuple[str, str]] = set()
    with REBUILT_RUNTIME_SYMBOLS.open(newline="", encoding="utf-8-sig") as stream:
        reader = csv.DictReader(
            line for line in stream if line.strip() and not line.lstrip().startswith("#")
        )
        for line_number, row in enumerate(reader, start=3):
            module = normalize_module(row.get("module", ""))
            symbol = row.get("symbol", "").strip()
            library = row.get("library", "").strip()
            kind = row.get("type", "").strip()
            reason = row.get("reason", "").strip()
            key = (module.casefold(), symbol)
            if not module.startswith("build/intel/") or "/mt_obj/" not in module:
                errors.append(
                    f"{REBUILT_RUNTIME_SYMBOLS}:{line_number}: invalid runtime module {module!r}"
                )
            if symbol not in COMPILER_RUNTIME_SYMBOLS:
                errors.append(
                    f"{REBUILT_RUNTIME_SYMBOLS}:{line_number}: unknown compiler-runtime symbol {symbol!r}"
                )
            if library != "LIBCMT" or kind != "compiler-runtime" or not reason:
                errors.append(
                    f"{REBUILT_RUNTIME_SYMBOLS}:{line_number}: incomplete ownership for {symbol!r}"
                )
            if key in declared:
                errors.append(
                    f"{REBUILT_RUNTIME_SYMBOLS}:{line_number}: duplicate {module}::{symbol}"
                )
            declared.add(key)
    return declared, errors


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("--target", default="LEMBALL")
    parser.add_argument("--roadmap", type=Path, default=ROADMAP)
    parser.add_argument("--manifest", type=Path, default=MANIFEST)
    return parser.parse_args()


def load_manifest_categories(path: Path, target: str) -> dict[int, str]:
    import json

    manifest = json.loads(path.read_text(encoding="utf-8"))
    if manifest.get("version") != target:
        raise SystemExit(f"{path} is not a {target} manifest")
    return {
        int(function["address"], 16): function["category"]
        for function in manifest["functions"]
    }


def main() -> int:
    args = parse_args()
    roadmap = args.roadmap.resolve()
    generate_roadmap(args.target, roadmap)
    rebuilt_only = read_rebuilt_only(roadmap)
    generated = [row for row in rebuilt_only if compiler_generated(row["symbol"])]
    source_authored = [row for row in rebuilt_only if not compiler_generated(row["symbol"])]

    rebuilt_runtime_symbols, runtime_build_errors = load_rebuilt_runtime_symbols()
    observed_rebuilt_runtime: set[tuple[str, str]] = set()
    unmapped_generated: list[dict[str, str]] = []
    for row in generated:
        key = (row["module"].casefold(), row["symbol"])
        if key in rebuilt_runtime_symbols:
            observed_rebuilt_runtime.add(key)
        else:
            unmapped_generated.append(row)

    manifest_categories = load_manifest_categories(args.manifest.resolve(), args.target)
    runtime_addresses = set(load_runtime_symbols(RUNTIME_SYMBOLS))
    errors = scan_sources(args.target, manifest_categories, runtime_addresses)
    errors.extend(scan_runtime_symbols())
    errors.extend(runtime_build_errors)
    for module, symbol in sorted(rebuilt_runtime_symbols - observed_rebuilt_runtime):
        errors.append(
            f"{REBUILT_RUNTIME_SYMBOLS}: declared runtime symbol not emitted: "
            f"{module}::{symbol}"
        )
    for row in source_authored:
        errors.append(
            "unmapped source function: "
            f"{row['module']}::{row['symbol']} ({row['recomp_addr']})"
        )
    for row in unmapped_generated:
        errors.append(
            "unmapped compiler-generated function: "
            f"{row['module']}::{row['symbol']} ({row['recomp_addr']})"
        )

    if errors:
        for error in errors:
            print(error, file=sys.stderr)
        print(
            f"annotation audit failed: {len(source_authored)} source-authored; "
            f"{len(unmapped_generated)} compiler-generated",
            file=sys.stderr,
        )
        return 1

    print(
        "annotation audit clean: 0 source-authored; 0 compiler-generated; "
        f"{len(observed_rebuilt_runtime)} declared compiler-runtime"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
