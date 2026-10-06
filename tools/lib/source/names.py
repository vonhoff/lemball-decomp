"""Audit source signatures against independent symbol catalog evidence."""

import re

from .codewarrior import decode_signature
from .signatures import adjacent_signature, canonical_type, class_ranges
from ..project import TARGET_ID, WINDOWS_NAME_REVIEWS
from .scan import TOKENS, mask_comments_and_strings

WINDOWS_MARK = re.compile(
    rf"//\s*(?:FUNCTION|STUB|SYNTHETIC|TEMPLATE|LIBRARY):\s*{re.escape(TARGET_ID)}\s+(0x[0-9a-fA-F]+)\b"
)


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
            wanted = tuple(
                canonical_type(parameter) for parameter in expected.parameters
            )
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
            has_code = code[start : token.start()].strip()
            blank_line = re.search(r"\n[ \t\r]*\n", text[start : token.start()])
            if has_code or blank_line:
                yield block, token.start()
                block = []
        block.append(token)
    if block:
        yield block, len(code)


def compare_catalog_candidates(address, actual, symbols, candidates):
    """Compare folded candidates; apply Windows reviews only to exact keys."""
    actual_signature = actual.display()
    for mac in candidates:
        symbol = symbols[mac]
        comparison = compare_signature(decode_signature(symbol), actual)
        evidence = WINDOWS_NAME_REVIEWS.get((address, symbol, actual_signature))
        if evidence and comparison["status"] != "match":
            comparison.update(
                status="windows", signature_status="review", windows_evidence=evidence
            )
        yield comparison


CANDIDATE_PRIORITY = {
    "match": 0,
    "windows": 1,
    "case": 2,
    "mismatch": 3,
}


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
            row = {
                "path": str(path),
                "line": text.count("\n", 0, token.start()) + 1,
                "windows_address": f"0x{address:08x}",
            }
            candidates = by_windows.get(address)
            if not candidates:
                yield dict(row, status="unmapped")
                continue
            if "SYNTHETIC:" in token[0]:
                yield dict(
                    row,
                    status="synthetic",
                    reason="compiler-emitted function; no C++ signature",
                )
                continue
            try:
                actual = adjacent_signature(code[:limit], block[-1].end(), ranges)
            except ValueError as error:
                yield dict(row, status="unresolved", reason=str(error))
                continue
            best = min(
                compare_catalog_candidates(address, actual, symbols, candidates),
                key=lambda candidate: (
                    CANDIDATE_PRIORITY[candidate["status"]],
                    candidate["signature_status"] != "match",
                ),
            )
            yield dict(row, **best)
