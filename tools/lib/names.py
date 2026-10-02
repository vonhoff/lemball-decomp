#!/usr/bin/env python3
"""Audit C++ names and signatures against independent, reviewed CSV evidence."""

import csv
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

from . import ROOT, TOKENS, collect_sources, mask_comments_and_strings
from .signatures import adjacent_signature, canonical_type, class_ranges, decode_signature

CATALOG = ROOT / "tools/data/mac-symbol-catalog.csv"
WINDOWS_MARK = re.compile(
    r"//\s*(?:FUNCTION|STUB|SYNTHETIC|TEMPLATE|LIBRARY):\s*LEMBALL\s+(0x[0-9a-fA-F]+)\b"
)


def read_catalog(path=CATALOG):
    """Read symbol identities and Windows mappings; reject conflicting evidence."""
    symbols, mappings, unmapped = {}, set(), set()
    with path.open(newline="", encoding="utf-8-sig") as stream:
        rows = csv.reader(stream, strict=True)
        if next(rows, None) != ["mac_address", "symbol", "windows_address"]:
            raise ValueError("catalog requires mac_address,symbol,windows_address")
        for row in rows:
            mac, name, win = row
            for address in (mac,) if not win else (mac, win):
                if not re.fullmatch(r"[0-9a-f]{1,8}", address) or int(address, 16) == 0:
                    raise ValueError(f"catalog line {rows.line_num}: invalid address {address!r}")
            mac, win = int(mac, 16), int(win, 16) if win else None
            if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_.$?@]*", name):
                raise ValueError(f"catalog line {rows.line_num}: invalid symbol")
            if mac in symbols and symbols[mac] != name:
                raise ValueError(f"catalog line {rows.line_num}: conflicting symbols at {mac:#010x}")
            if mac in unmapped or (win is None and mac in symbols):
                raise ValueError(f"catalog line {rows.line_num}: redundant unmapped symbol")
            if win is None:
                unmapped.add(mac)
            elif (mac, win) in mappings:
                raise ValueError(f"catalog line {rows.line_num}: duplicate Windows pair")
            else:
                mappings.add((mac, win))
            symbols[mac] = name
    if not mappings:
        raise ValueError("catalog contains no reviewed Windows pairs")
    by_windows = defaultdict(list)
    for mac, address in sorted(mappings):
        by_windows[address].append(mac)
    return symbols, by_windows


# Exact Windows ABI reviews, not spelling aliases. Keep the catalog spelling
# everywhere else; changing the address, catalog symbol, or source signature
# invalidates the review.
WINDOWS_NAME_REVIEWS = {
    (0x0043A500, "OnZoomBox__4CWndFUc", "CWnd::OnDriverChange()"):
        "LEMBALL.EXE: CWnd vtable+0x5c at 0x0049942c points through "
        "0x00401028 to the zero-argument RET at 0x0043a500; CPVWnd's same "
        "slot points to OnDriverChange at 0x00466340.",
    (0x0045EDA0, "GetCDDir__FPCc", "CPlatformServices::GetCDDir(const char*)"):
        "LEMBALL.EXE: caller 0x00406e60 loads the platform object into ECX "
        "before CALL 0x0045eda0; the callee returns with RET 4 at 0x0045ee61. "
        "Windows uses a member function for the catalog's free function.",
}


def compare_signature(expected, actual):
    """Compare names, parameter types, and constness; ABI differences need review."""
    differences = []
    for part, wanted, found in (
        ("class", expected.owner, actual.owner),
        ("method", expected.method, actual.method),
    ):
        if wanted != found:
            kind = "case" if wanted.lower() == found.lower() else "name"
            differences.append(f"{part}-{kind}")
    status = "match"
    if any(difference.endswith("-name") for difference in differences):
        status = "mismatch"
    elif differences:
        status = "case"

    signature_status = "match"
    if expected.parameters is None:
        signature_status = "unencoded"
    elif actual.parameters is None:
        signature_status = "unresolved"
    else:
        try:
            wanted = tuple(canonical_type(parameter) for parameter in expected.parameters)
            found = tuple(canonical_type(parameter) for parameter in actual.parameters)
            if wanted != found or expected.const != actual.const:
                signature_status = "review"
        except ValueError:
            signature_status = "unresolved"
    return {
        "status": status,
        "differences": differences,
        "signature_status": signature_status,
        "original_signature": expected.display(),
        "actual_signature": actual.display(),
    }


def annotation_blocks(text, code):
    """Yield line-comment blocks and the next block's offset to bound declarations."""
    block = []
    for token in TOKENS.finditer(text):
        if not token[0].startswith("//"):
            continue
        if block:
            start = block[-1].end()
            has_code = code[start:token.start()].strip()
            blank_line = re.search(r"\n[ \t\r]*\n", text[start:token.start()])
            if has_code or blank_line:
                yield block, token.start()
                block = []
        block.append(token)
    if block:
        yield block, len(code)


def compare_catalog_candidates(address, actual, symbols, candidates):
    """Preserve every folded identity; apply Windows reviews only to exact keys."""
    comparisons = []
    actual_signature = actual.display()
    for mac in candidates:
        symbol = symbols[mac]
        try:
            comparison = compare_signature(decode_signature(symbol), actual)
            evidence = WINDOWS_NAME_REVIEWS.get((address, symbol, actual_signature))
            if evidence and comparison["status"] != "match":
                comparison.update(
                    status="windows", signature_status="review", windows_evidence=evidence
                )
        except ValueError as error:
            comparison = {"status": "unresolved", "reason": str(error)}
        comparisons.append(dict(comparison, address_68k=f"0x{mac:08x}", symbol=symbol))
    return comparisons


CANDIDATE_PRIORITY = {"match": 0, "windows": 1, "case": 2, "mismatch": 3, "unresolved": 4}


def scan(path, symbols, by_windows):
    """Attach each Windows annotation to its declaration and catalog candidates."""
    text = path.read_text(encoding="utf-8")
    code = mask_comments_and_strings(text)
    ranges = class_ranges(code)
    for block, limit in annotation_blocks(text, code):
        for token in block:
            marker = WINDOWS_MARK.match(token[0])
            if marker is None:
                continue
            address = int(marker[1], 16)
            row = {"path": str(path), "line": text.count("\n", 0, token.start()) + 1,
                   "windows_address": f"0x{address:08x}"}
            candidates = by_windows.get(address)
            if not candidates:
                yield dict(row, status="unmapped")
                continue
            if "SYNTHETIC:" in token[0]:
                yield dict(row, status="synthetic",
                           reason="compiler-emitted function; no C++ signature")
                continue
            try:
                actual = adjacent_signature(code[:limit], block[-1].end(), ranges)
            except ValueError as error:
                yield dict(row, status="unresolved", reason=str(error))
                continue
            comparisons = compare_catalog_candidates(address, actual, symbols, candidates)
            best = min(comparisons, key=lambda candidate: (
                CANDIDATE_PRIORITY[candidate["status"]],
                candidate.get("signature_status") != "match",
            ))
            yield dict(row, **best, catalog_candidates=comparisons)


def print_review(row):
    """Print one name/signature review and its original Windows evidence."""
    detail = row.get("reason") or (
        f'{row["original_signature"]} -> {row["actual_signature"]}'
        f' ({", ".join(row["differences"]) or row["signature_status"]})'
    )
    print(f'{row["path"]}:{row["line"]}: {row["status"]}: {detail} [{row["windows_address"]}]')
    if row.get("windows_evidence"):
        print(f'  Windows evidence: {row["windows_evidence"]}')


def check_names(paths: list[Path | str] | None = None, verbose=False, catalog_path=CATALOG):
    """Fail on unresolved identities or name mismatches; keep ABI reviews informational."""
    try:
        symbols, mappings = read_catalog(catalog_path)
        files = collect_sources(paths)
        if not files:
            raise ValueError("no C++ source files found")
        rows = [row for path in files for row in scan(path, symbols, mappings)]
    except (OSError, UnicodeError, ValueError, csv.Error) as error:
        print(f"names: {error}", file=sys.stderr)
        return 2
    counts = dict(Counter(row["status"] for row in rows))
    signatures = dict(Counter(row["signature_status"] for row in rows if "signature_status" in row))
    for row in rows:
        required = row["status"] in ("mismatch", "unresolved", "windows")
        requested = verbose and (
            row["status"] == "case" or row.get("signature_status") in ("review", "unresolved")
        )
        if required or requested:
            print_review(row)
    print(f"names: {len(files)} files, {len(rows)} entries from CSV: {counts}")
    print(f"names: parameter/const comparisons: {signatures}")
    if signatures.get("review") or signatures.get("unresolved"):
        print("names: signature review requires Windows evidence; "
              "gate.py --names lists items.")
    if counts.get("unresolved") or (not rows and not paths):
        return 2
    return int(bool(counts.get("mismatch")))
