#!/usr/bin/env python3
"""Audit C++ names and signatures against independent, reviewed CSV evidence."""

import csv
import json
import re
import sys
from collections import Counter
from pathlib import Path

from . import ROOT, TOKENS, collect_sources, mask_comments_and_strings
from .signatures import adjacent_signature, canonical_type, class_ranges, decode_signature

CATALOG = ROOT / "tools/data/catalog.csv"
WINDOWS_MARK = re.compile(
    r"//\s*(?:FUNCTION|STUB|SYNTHETIC|TEMPLATE|LIBRARY):\s*LEMBALL\s+(0x[0-9a-fA-F]+)\b"
)


def read_catalog(path=CATALOG):
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
    return symbols, mappings


# Exact Windows ABI reviews, not spelling aliases. Keep the catalog spelling
# everywhere else; changing the address, catalog symbol, or source signature
# invalidates the review. --names-strict still rejects these review items.
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


def compare_signature(expected, actual, parameter_error=None):
    wanted_class = expected.owner
    wanted_method = expected.method
    diffs = []
    if wanted_class != actual.owner:
        diffs.append("class-case" if wanted_class.lower() == actual.owner.lower() else "class-name")
    if wanted_method != actual.method:
        diffs.append("method-case" if wanted_method.lower() == actual.method.lower() else "method-name")
    status = "match"
    if any(d.endswith("-name") for d in diffs):
        status = "mismatch"
    elif diffs:
        status = "case"
    signature_status, detail = "match", None
    if expected.parameters is None:
        signature_status, detail = "unencoded", "MacsBug name contains no parameter-type encoding"
    elif actual.parameters is None:
        signature_status, detail = "unresolved", parameter_error
    else:
        try:
            wanted = tuple(canonical_type(p) for p in expected.parameters)
            actual_types = tuple(canonical_type(p) for p in actual.parameters)
            if wanted != actual_types or expected.const != actual.const:
                signature_status = "review"
                detail = f"Mac {expected.display()} -> source {actual.display()}"
        except ValueError as error:
            signature_status, detail = "unresolved", str(error)
    return {"status": status, "wanted_class": wanted_class, "wanted_method": wanted_method,
                "actual_class": actual.owner, "actual_method": actual.method, "differences": diffs,
                "signature_status": signature_status, "signature_detail": detail,
                "original_signature": expected.display(), "actual_signature": actual.display()}


def scan(path, symbols, mappings):
    order = {"match": 0, "windows": 1, "case": 2, "mismatch": 3, "unresolved": 4}
    by_windows = {}
    for mac, win in sorted(mappings):
        by_windows.setdefault(win, []).append(mac)
    text = path.read_text(encoding="utf-8")
    code = mask_comments_and_strings(text)
    ranges = class_ranges(code)
    rows = []
    blocks, block = [], []
    for token in TOKENS.finditer(text):
        if not token[0].startswith("//"):
            continue
        if block:
            gap = text[block[-1].end():token.start()]
            if code[block[-1].end():token.start()].strip() or re.search(r"\n[ \t\r]*\n", gap):
                blocks.append(block)
                block = []
        block.append(token)
    if block:
        blocks.append(block)
    for block_index, block in enumerate(blocks):
        limit = blocks[block_index + 1][0].start() if block_index + 1 < len(blocks) else len(code)
        windows = [(token, int(mark[1], 16)) for token in block
                   if (mark := WINDOWS_MARK.match(token[0]))]
        for token, win in windows:
            candidates = by_windows.get(win, [])
            row = {"path": str(path), "line": text.count("\n", 0, token.start()) + 1,
                       "windows_address": f"0x{win:08x}"}
            if not candidates:
                rows.append(dict(row, status="unmapped"))
                continue
            if "SYNTHETIC:" in token[0]:
                rows.append(dict(row, status="synthetic", reason="compiler-emitted function; no C++ signature"))
                continue
            try:
                actual, parameter_error = adjacent_signature(code[:limit], block[-1].end(), ranges)
            except ValueError as error:
                rows.append(dict(row, status="unresolved", reason=str(error)))
                continue
            comparisons = []
            for mac in candidates:
                symbol = symbols[mac]
                try:
                    comparison = compare_signature(decode_signature(symbol), actual, parameter_error)
                    evidence = WINDOWS_NAME_REVIEWS.get((win, symbol, actual.display()))
                    if evidence and comparison["status"] != "match":
                        comparison.update(status="windows", signature_status="review",
                                          windows_evidence=evidence)
                except ValueError as error:
                    comparison = {"status": "unresolved", "reason": str(error)}
                comparisons.append(dict(comparison, address_68k=f"0x{mac:08x}", symbol=symbol))
            # Folded code can have several legitimate source identities. Preserve all
            # candidates in JSON and accept a compatible one, never an arbitrary row.
            best = min(comparisons, key=lambda r: (order[r["status"]], r.get("signature_status") != "match"))
            row.update(best)
            row["catalog_candidates"] = comparisons
            rows.append(row)
    return rows


def check_names(paths: list[Path | str] | None = None, strict=False, as_json=False,
                verbose=False, catalog_path=CATALOG):
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
    failures = [r for r in rows if r["status"] in ("mismatch", "unresolved")
                or (strict and r["status"] == "case")
                or (strict and r.get("signature_status") in ("review", "unresolved"))]
    if as_json:
        print(json.dumps({"files": len(files), "entries": len(rows), "counts": counts,
                              "signatures": signatures, "comparisons": rows}, indent=2))
    else:
        for row in rows:
            if row not in failures and row["status"] != "windows" and not (
                    verbose and row.get("signature_status") in ("review", "unresolved")):
                continue
            detail = row.get("reason") or (
                f'{row["original_signature"]} -> {row["actual_signature"]}'
                f' ({", ".join(row["differences"]) or row.get("signature_status", "")})'
            )
            address = row["windows_address"]
            print(f'{row["path"]}:{row["line"]}: {row["status"]}: {detail} [{address}]')
            if row.get("windows_evidence"):
                print(f'  Windows evidence: {row["windows_evidence"]}')
        print(f"names: {len(files)} files, {len(rows)} entries from CSV: {counts}")
        print(f"names: parameter/const comparisons: {signatures}")
        if signatures.get("review") or signatures.get("unresolved"):
            print("names: signature review requires Windows evidence; "
                  "--verbose lists items, --names-strict fails them.")
    if counts.get("unresolved") or (not rows and not paths):
        return 2
    return int(bool(failures))
