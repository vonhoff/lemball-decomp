"""Check 68K comments against a derived symbol catalog and reviewed Windows pairs.

No game payload is bundled. A private original can verify the symbol extraction.
See evidence/68k/PROVENANCE.md for provenance and the limits of this check.
"""

from __future__ import annotations

import gzip
import hashlib
import io
import json
import re
import sys
import zlib
from collections import Counter
from pathlib import Path

from .paths import ROOT
from .source import collect_sources, mask_comments_and_strings

CATALOG = ROOT / "evidence/68k/catalog.json.gz"
CATALOG_ERRORS = (OSError, UnicodeError, ValueError, EOFError, zlib.error)
TOKENS = re.compile(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'')
MAC_MARK = re.compile(r"//\s*68K\s+(0x[0-9a-fA-F]+)\s+(\S+)(?:\s+.*)?$")
WINDOWS_MARK = re.compile(
    r"//\s*(?:FUNCTION|STUB|SYNTHETIC|TEMPLATE|LIBRARY):\s*LEMBALL\s+(0x[0-9a-fA-F]+)\b"
)


def region(data: bytes, offset: int, size: int) -> bytes:
    if offset < 0 or size < 0 or offset + size > len(data):
        raise ValueError(f"resource fork range outside input: {offset:#x}+{size:#x}")
    return data[offset:offset + size]


def number(data: bytes, offset: int, size: int) -> int:
    return int.from_bytes(region(data, offset, size), "big")


def code_symbols(resource_id: int, payload: bytes) -> dict[int, str]:
    start = 4 if resource_id == 1 else 12
    end = len(payload) if resource_id == 1 else number(payload, 8, 4)
    if not start <= end <= len(payload):
        raise ValueError(f"CODE {resource_id}: invalid relocation boundary {end:#x}")
    symbols = {}
    terminal = start
    while terminal + 5 < end:
        position = terminal
        terminal += 2
        if payload[position:position + 2] != b"\x4e\x75":  # RTS before MacsBug trailer
            continue
        marker = payload[position + 2]
        name_start = position + 3
        if 0x81 <= marker <= 0x9f:
            length = marker & 0x1f
        elif marker == 0x80:
            length = payload[name_start]
            name_start += 1
        else:
            continue
        name_end = name_start + length
        if not length or name_end > end:
            continue
        name_bytes = payload[name_start:name_end]
        if not re.fullmatch(rb"[A-Za-z_][\x20-\x7e]*", name_bytes):
            continue
        constants = (name_end + 1) & ~1
        if constants + 2 > end:
            continue
        trailer_end = (constants + 2 + number(payload, constants, 2) + 1) & ~1
        if trailer_end > end:
            continue
        address = 0x10000000 + resource_id * 0x00100000 + start
        # Ghidra import replaces template punctuation with underscores.
        symbols[address] = re.sub(r"[^A-Za-z0-9_.$?@]", "_", name_bytes.decode("ascii"))
        start = terminal = trailer_end
    if symbols and start != end:
        raise ValueError(f"CODE {resource_id}: unparsed bytes before relocation boundary")
    return symbols


def read_symbols(fork: bytes) -> dict[int, str]:
    data_base, map_base = number(fork, 0, 4), number(fork, 4, 4)
    data = region(fork, data_base, number(fork, 8, 4))
    resource_map = region(fork, map_base, number(fork, 12, 4))
    type_base = number(resource_map, 24, 2)
    type_count = number(resource_map, type_base, 2) + 1
    symbols, seen_ids = {}, set()
    for index in range(type_count):
        entry = type_base + 2 + index * 8
        if region(resource_map, entry, 4) != b"CODE":
            continue
        count = number(resource_map, entry + 4, 2) + 1
        references = type_base + number(resource_map, entry + 6, 2)
        for item in range(count):
            reference = references + item * 12
            record = region(resource_map, reference, 12)
            resource_id = int.from_bytes(record[:2], "big", signed=True)
            if resource_id == 0:
                continue  # Jump table, not a code segment.
            if resource_id < 0 or resource_id in seen_ids:
                raise ValueError(f"invalid or duplicate CODE resource: {resource_id}")
            seen_ids.add(resource_id)
            offset = int.from_bytes(record[5:8], "big")
            payload = region(data, offset + 4, number(data, offset, 4))
            symbols.update(code_symbols(resource_id, payload))
    if not symbols:
        raise ValueError("no MacsBug symbols found in CODE resources")
    return symbols


def catalog_entries(catalog: object) -> tuple[dict[int, str], set[tuple[int, int]]]:
    """Validate the data schema, including duplicate rows and duplicate pairs."""
    if not isinstance(catalog, dict) or set(catalog) != {"format", "resource_sha256", "symbols"}:
        raise ValueError("catalog requires format, resource_sha256, and symbols")
    if type(catalog["format"]) is not int or catalog["format"] != 1:
        raise ValueError("unsupported catalog format")
    digest = catalog["resource_sha256"]
    if not isinstance(digest, str) or not re.fullmatch(r"[0-9a-f]{64}", digest):
        raise ValueError("catalog requires the original resource SHA256")
    if not isinstance(catalog["symbols"], list) or not catalog["symbols"]:
        raise ValueError("catalog contains no symbols")

    def address(value):
        if not isinstance(value, str) or not re.fullmatch(r"[0-9a-f]{8}", value) or int(value, 16) == 0:
            raise ValueError(f"invalid catalog address: {value!r}")
        return int(value, 16)

    symbols, mappings = {}, set()
    for index, row in enumerate(catalog["symbols"], 1):
        if not isinstance(row, list) or len(row) != 3:
            raise ValueError(f"catalog row {index}: expected [Mac address, symbol, Windows addresses]")
        mac, name, windows = row
        mac = address(mac)
        if mac in symbols:
            raise ValueError(f"catalog row {index}: duplicate Mac address {mac:#010x}")
        if not isinstance(name, str) or not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_.$?@]*", name):
            raise ValueError(f"catalog row {index}: invalid symbol")
        if not isinstance(windows, list):
            raise ValueError(f"catalog row {index}: Windows addresses must be a list")
        symbols[mac] = name
        for win in windows:
            pair = mac, address(win)
            if pair in mappings:
                raise ValueError(f"catalog row {index}: duplicate Windows pair")
            mappings.add(pair)
    if not mappings:
        raise ValueError("catalog contains no reviewed Windows pairs")
    return symbols, mappings


def read_catalog(path: Path = CATALOG) -> dict:
    catalog = json.loads(gzip.decompress(path.read_bytes()))
    catalog_entries(catalog)
    return catalog


def canonical_catalog(catalog: dict) -> dict:
    symbols, mappings = catalog_entries(catalog)
    windows = {mac: [] for mac in symbols}
    for mac, win in sorted(mappings):
        windows[mac].append(f"{win:08x}")
    return {"format": 1, "resource_sha256": catalog["resource_sha256"], "symbols": [
        [f"{mac:08x}", name, windows[mac]] for mac, name in sorted(symbols.items())
    ]}


def write_catalog(path: Path, catalog: dict) -> None:
    payload = json.dumps(canonical_catalog(catalog), separators=(",", ":")).encode("ascii")
    packed = io.BytesIO()
    # No timestamp or filename; the same reviewed input produces identical bytes.
    with gzip.GzipFile(filename="", mode="wb", fileobj=packed, compresslevel=9, mtime=0) as stream:
        stream.write(payload)
    path.write_bytes(packed.getvalue())


def verify_resource(catalog: dict, path: Path) -> None:
    fork = path.read_bytes()
    if hashlib.sha256(fork).hexdigest() != catalog["resource_sha256"]:
        raise ValueError("private resource SHA256 differs from the catalog source")
    symbols, _ = catalog_entries(catalog)
    if read_symbols(fork) != symbols:
        raise ValueError("catalog symbols differ from the private resource extraction")


def catalog_command(export: Path | None = None, pack: Path | None = None,
                    catalog_path: Path = CATALOG, resource: Path | None = None) -> int:
    """Export readable rows or pack reviewed rows; never derive mappings from annotations."""
    try:
        catalog = (json.loads(pack.read_text(encoding="utf-8")) if pack is not None
                   else read_catalog(catalog_path))
        catalog = canonical_catalog(catalog)
        if resource is not None:
            verify_resource(catalog, resource)
        if pack is not None:
            write_catalog(catalog_path, catalog)
            print(f"68k: packed reviewed catalog into {catalog_path}")
        elif export is not None:
            header = json.dumps({key: catalog[key] for key in ("format", "resource_sha256")}, indent=2)
            rows = ",\n".join("    " + json.dumps(row) for row in catalog["symbols"])
            export.write_text(header[:-2] + ',\n  "symbols": [\n' + rows + "\n  ]\n}\n",
                              encoding="utf-8", newline="\n")
            print(f"68k: exported readable catalog to {export}")
        else:
            raise ValueError("catalog command requires export or pack")
    except CATALOG_ERRORS as error:
        print(f"68k: {error}", file=sys.stderr)
        return 2
    return 0


def scan_annotations(path: Path) -> list[dict]:
    text = path.read_text(encoding="utf-8")
    code = mask_comments_and_strings(text)
    comments = [token for token in TOKENS.finditer(text) if token[0].startswith("//")]
    rows = []
    for comment in comments:
        if not re.match(r"//\s*68K\b", comment[0]):
            continue
        row = {"path": str(path), "line": text.count("\n", 0, comment.start()) + 1}
        mark = MAC_MARK.fullmatch(comment[0])
        if not mark:
            rows.append(dict(row, status="invalid", detail="malformed 68K annotation"))
            continue
        # Limit association to this comment block. Never borrow a later function's address.
        before = code[:comment.start()].rstrip()
        following = re.search(r"\S", code[comment.end():])
        end = comment.end() + following.start() if following else len(text)
        start = len(before)
        # Synthetic declarations end at a blank line without emitting C++ code.
        separators = list(re.finditer(r"\n[ \t\r]*\n", text[start:comment.start()]))
        if separators:
            start += separators[-1].end()
        separator = re.search(r"\n[ \t\r]*\n", text[comment.end():end])
        if separator:
            end = comment.end() + separator.start()
        addresses = tuple(dict.fromkeys(
            int(match[1], 16)
            for token in comments if start <= token.start() < end
            if (match := WINDOWS_MARK.match(token[0]))
        ))
        rows.append(dict(row, mac_address=int(mark[1], 16), symbol=mark[2].rstrip(";"),
                         windows_addresses=addresses))
    return rows


def audit_annotations(rows: list[dict], symbols: dict[int, str], mappings: set[tuple[int, int]]) -> list[dict]:
    for row in rows:
        if row.get("status") == "invalid":
            continue
        address, symbol = row["mac_address"], row["symbol"]
        actual = symbols.get(address)
        if actual != symbol:
            row.update(status="invalid", detail=(
                f"{address:#010x} {symbol}: catalog has {actual or 'no symbol at this address'}"
            ))
            continue
        unlisted = [a for a in row["windows_addresses"] if (address, a) not in mappings]
        if unlisted:
            row.update(status="review", detail=(
                f"{address:#010x} {symbol}: Windows pair absent from reviewed catalog: "
                + ", ".join(f"{a:#010x}" for a in unlisted)
            ))
        else:
            row["status"] = "confirmed" if row["windows_addresses"] else "symbol-only"
    return rows


def check_provenance(catalog_path: Path = CATALOG, paths=None, strict=False, verbose=False,
                     resource: Path | None = None) -> int:
    try:
        catalog = read_catalog(catalog_path)
        symbols, mappings = catalog_entries(catalog)
        if resource is not None:
            verify_resource(catalog, resource)
        files = collect_sources(paths)
        if not files:
            raise ValueError("no C++ source files found")
        rows = audit_annotations([row for path in files for row in scan_annotations(path)], symbols, mappings)
        if not rows and not paths:
            raise ValueError("no 68K annotations found")
    except CATALOG_ERRORS as error:
        print(f"68k: {error}", file=sys.stderr)
        return 2
    counts = Counter(row["status"] for row in rows)
    for row in rows:
        if row["status"] == "invalid" or (row["status"] == "review" and (strict or verbose)):
            print(f'{row["path"]}:{row["line"]}: {row["status"]}: {row["detail"]}')
    print(f"68k: {len(symbols)} catalog symbols; {len(rows)} annotations: {dict(counts)}")
    if resource is not None:
        print("68k: private resource SHA256 and complete symbol extraction verified.")
    if counts["review"]:
        print("68k: unlisted Windows pairs need semantic review; destructor variants may be valid.")
    if counts["symbol-only"]:
        print("68k: symbol-only comments have no adjacent Windows address; only symbol provenance checked.")
    return 1 if counts["invalid"] or (strict and counts["review"]) else 0
