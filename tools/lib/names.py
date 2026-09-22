#!/usr/bin/env python3
"""Audit C++ names and signatures against independent, reviewed CSV evidence."""

import json
import re
import sys
from collections import Counter
from pathlib import Path

from .cpp_signatures import adjacent_signature, canonical_type, class_ranges
from .mac_symbols import decode_signature
from .provenance import (
    CATALOG, CATALOG_ERRORS, MAC_MARK, TOKENS, WINDOWS_MARK,
    audit_annotations, catalog_entries, read_catalog, scan_annotations,
)
from .source import (
    collect_sources,
    drop_type_prefix,
    mask_comments_and_strings,
)

ACRONYMS = (
    ("TCPIP", "TcpIp"), ("MRAM", "Mram"), ("GDI", "Gdi"), ("RAM", "Ram"),
    ("CD", "Cd"), ("PV", "Pv"), ("VS", "Vs"), ("AI", "Ai"),
)
# Do not rename source to match 68K for these; see AGENTS.md Naming.
INTENTIONAL = {
    ('', 'GetCdDir', 'TargetPlatformServices', 'GetCdDir'),
    ('CCdLoadAnim', 'Draw', 'CCdLoadAnimDraw', 'Draw'),
    ('CCdLoadAnim', 'Draw', 'CCdLoadAnimProgress', 'Draw'),
    ('CPreviewDrawer::Prims', '<constructor>', 'CPreviewDrawerPrims', '<constructor>'),
    ('CPreviewDrawer::Prims', '<destructor>', 'CPreviewDrawerPrims', '<destructor>'),
    ('CProcess', '<destructor>', 'CBaseProcess', '<destructor>'),
    ('CSuccFailDrawer::Prims', '<constructor>', 'CSuccFailDrawerPrims', '<constructor>'),
    ('CSuccFailDrawer::Prims', '<destructor>', 'CSuccFailDrawerPrims', '<destructor>'),
    ('CWnd', 'OnZoomBox', 'CWnd', 'OnDriverChange'),
}


def decode_symbol(symbol):
    signature = decode_signature(symbol)
    return signature.owner, signature.method


def normalize_word(word):
    for acronym, pascal in ACRONYMS:
        if word.startswith(acronym):
            rest = word[len(acronym):]
            if rest and rest[0].islower():
                rest = rest[0].upper() + rest[1:]
            word = pascal + rest
            break

    def replace_run(match):
        run = match.group(0)
        nxt = word[match.end():match.end() + 1]
        if nxt and nxt.islower():
            return run[0] + run[1:-1].lower() + run[-1]
        return run[0] + run[1:].lower()

    return re.sub(r"[A-Z]{2,}", replace_run, word)


def normalize_segment(segment):
    segment = drop_type_prefix(segment)
    return "".join(normalize_word(part) for part in segment.split("_") if part)


def class_name(name):
    def segment(part):
        if part == "CHAR4":
            return part  # Complete original stream-helper type, not a C prefix.
        if part.startswith("C") and part[1:2].isupper():
            return "C" + "".join(normalize_word(word) for word in part[1:].split("_") if word)
        return normalize_segment(part)
    return "::".join(segment(part) for part in name.split("::"))


def method_name(name):
    if name.startswith(("<", "operator")):
        return name
    internal = name.startswith("_")
    body = "".join(
        n[:1].upper() + n[1:]
        for part in name.split("_")
        if (n := normalize_segment(part))
    )
    return "Internal" + body if internal else body


def method_fold(name):
    if name.startswith(("<", "operator")):
        return name
    internal = name.startswith("_")
    body = "".join(part for part in name.split("_") if part)
    return ("Internal" + body if internal else body).lower()


def comment_blocks(text):
    """Adjacent real line comments; code and blank lines end an annotation block."""
    code = mask_comments_and_strings(text)
    block = []
    for token in TOKENS.finditer(text):
        if not token[0].startswith("//"):
            continue
        if block:
            gap = text[block[-1].end():token.start()]
            if code[block[-1].end():token.start()].strip() or re.search(r"\n[ \t\r]*\n", gap):
                yield block
                block = []
        block.append(token)
    if block:
        yield block


def normalized_type(value, original=False):
    if not original:
        value = re.sub(r"[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*", lambda m: class_name(m[0]), value)
    return canonical_type(value)


def compare_signature(expected, actual, original=False, parameter_error=None):
    wanted_class = expected.owner if original or expected.owner == actual.owner else class_name(expected.owner)
    wanted_method = expected.method if original else method_name(expected.method)
    diffs = []
    if wanted_class != actual.owner:
        diffs.append("class-case" if wanted_class.lower() == actual.owner.lower() else "class-name")
    if wanted_method != actual.method:
        if method_fold(expected.method) != method_fold(actual.method) or "_" in actual.method:
            diffs.append("method-name")
        else:
            diffs.append("method-case")
    key = (class_name(expected.owner), method_name(expected.method), actual.owner, actual.method)
    status = "match"
    if diffs and key in INTENTIONAL and not original:
        status = "intentional"
    elif any(d.endswith("-name") for d in diffs):
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
            wanted = tuple(normalized_type(p, original) for p in expected.parameters)
            actual_types = tuple(normalized_type(p, original) for p in actual.parameters)
            if wanted != actual_types or expected.const != actual.const:
                signature_status = "review"
                detail = f"Mac {expected.display()} -> source {actual.display()}"
        except ValueError as error:
            signature_status, detail = "unresolved", str(error)
    return dict(status=status, wanted_class=wanted_class, wanted_method=wanted_method,
                actual_class=actual.owner, actual_method=actual.method, differences=diffs,
                signature_status=signature_status, signature_detail=detail,
                original_signature=expected.display(), actual_signature=actual.display())


def scan(path, symbols=None, mappings=None, original=False):
    if symbols is None or mappings is None:
        symbols, mappings = catalog_entries(read_catalog())
    by_windows = {}
    for mac, win in sorted(mappings):
        by_windows.setdefault(win, []).append(mac)
    text = path.read_text(encoding="utf-8")
    code = mask_comments_and_strings(text)
    ranges = class_ranges(code)
    rows = []
    # Incorrect annotations cannot override the independent expected name.
    for row in audit_annotations(scan_annotations(path), symbols, mappings):
        if row["status"] in ("invalid", "review"):
            rows.append(dict(row, status="invalid", reason=row["detail"]))
    blocks = list(comment_blocks(text))
    for block_index, block in enumerate(blocks):
        limit = blocks[block_index + 1][0].start() if block_index + 1 < len(blocks) else len(code)
        windows = [(token, int(mark[1], 16)) for token in block
                   if (mark := WINDOWS_MARK.match(token[0]))]
        macs = [(token, int(mark[1], 16)) for token in block
                if (mark := MAC_MARK.fullmatch(token[0]))]
        # Windows entry is the primary lookup, even when no // 68K comment exists.
        entries = [(token, win, by_windows.get(win, [])) for token, win in windows]
        if not windows:
            entries = [(token, None, [mac]) for token, mac in macs if mac in symbols]
        for token, win, candidates in entries:
            row = dict(path=str(path), line=text.count("\n", 0, token.start()) + 1,
                       windows_address=f"0x{win:08x}" if win else None)
            if not candidates:
                rows.append(dict(row, status="unmapped"))
                continue
            if "SYNTHETIC:" in token[0]:
                rows.append(dict(row, status="synthetic", reason="compiler-emitted function; no C++ signature"))
                continue
            try:
                actual, parameter_error = adjacent_signature(code[:limit], block[-1].end(), ranges)
            except ValueError as error:
                # Compiler-emitted functions have no source declaration to compare.
                synthetic = "SYNTHETIC:" in token[0] or (
                    not windows and all(symbols[mac].startswith(("__ct__", "__dt__")) for mac in candidates)
                    and re.match(r"(?:\s*#pragma[^\n]*\n)*\s*(?:class|struct)\b", code[block[-1].end():])
                )
                rows.append(dict(row, status="synthetic" if synthetic else "unresolved", reason=str(error)))
                continue
            comparisons = []
            for mac in candidates:
                symbol = symbols[mac]
                try:
                    comparison = compare_signature(decode_signature(symbol), actual, original, parameter_error)
                except ValueError as error:
                    comparison = dict(status="unresolved", reason=str(error))
                comparisons.append(dict(comparison, address_68k=f"0x{mac:08x}", symbol=symbol))
            # Folded code can have several legitimate source identities. Preserve all
            # candidates in JSON and accept a compatible one, never an arbitrary row.
            order = {"match": 0, "intentional": 1, "case": 2, "mismatch": 3, "unresolved": 4}
            best = min(comparisons, key=lambda r: (order[r["status"]], r.get("signature_status") != "match"))
            row.update(best)
            row["catalog_candidates"] = comparisons
            rows.append(row)
    return rows


def check_names(paths: list[Path | str] | None = None, strict=False, as_json=False,
                fail=True, original=False, verbose=False, catalog_path=CATALOG):
    try:
        symbols, mappings = catalog_entries(read_catalog(catalog_path))
        files = collect_sources(paths)
        if not files:
            raise ValueError("no C++ source files found")
        rows = [row for path in files for row in scan(path, symbols, mappings, original)]
    except CATALOG_ERRORS as error:
        print(f"names: {error}", file=sys.stderr)
        return 2
    counts = dict(Counter(row["status"] for row in rows))
    signatures = dict(Counter(row["signature_status"] for row in rows if "signature_status" in row))
    failures = [r for r in rows if r["status"] in ("mismatch", "invalid", "unresolved")
                or ((strict or original) and r["status"] == "case")
                or (strict and r.get("signature_status") in ("review", "unresolved"))]
    if as_json:
        print(json.dumps(dict(files=len(files), entries=len(rows), counts=counts,
                              signatures=signatures, comparisons=rows), indent=2))
    else:
        for row in rows:
            if row not in failures and not (verbose and row.get("signature_status") in ("review", "unresolved")):
                continue
            detail = row.get("reason") or (
                f'{row["original_signature"]} -> {row["actual_signature"]}'
                f' ({", ".join(row["differences"]) or row.get("signature_status", "")})'
            )
            address = row.get("windows_address") or row.get("address_68k", "")
            print(f'{row["path"]}:{row["line"]}: {row["status"]}: {detail} [{address}]')
        print(f"names: {len(files)} files, {len(rows)} entries from CSV: {counts}")
        print(f"names: parameter/const comparisons: {signatures}")
        if signatures.get("review") or signatures.get("unresolved"):
            print("names: signature review requires Windows evidence; --verbose lists items, --names-strict fails them.")
    if fail:
        if counts.get("unresolved") or (not rows and not paths):
            return 2
        if failures:
            return 1
    return 0
