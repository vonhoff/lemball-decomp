"""Audit source signatures against independent symbol catalog evidence."""

import re

from reccmp.cvdump.demangler import msvc_demangle

from .codewarrior import decode_signature
from .signatures import Signature, adjacent_signature, canonical_type, delimiter_ends
from . import TARGET_ID
from .scan import TOKENS, mask_comments_and_strings

WINDOWS_NAME_REVIEWS = {
    (
        0x00471AF0,
        "SysCloseSocket__14CTCPIPRWSocketFv",
        "CTCPIPCommonSocket::SysCloseSocket()",
    ): "LEMBALL.EXE: 0x00471af0 adds 0x128 to ECX and jumps to "
    "CTCPIPCommonSocket::SysCloseSocket at 0x00471a60. Mac 0x1010d04c "
    "is a CTCPIPRWSocket wrapper forwarding to the same common-base method. "
    "The Windows symbol names the defining base of the compiler adjustor.",
    (
        0x0043A500,
        "OnZoomBox__4CWndFUc",
        "CWnd::OnDriverChange()",
    ): "LEMBALL.EXE: CWnd vtable+0x5c at 0x0049942c points through "
    "0x00401028 to the zero-argument RET at 0x0043a500; CPVWnd's same "
    "slot points to OnDriverChange at 0x00466340.",
    (
        0x0045EDA0,
        "GetCDDir__FPCc",
        "CPlatformServices::GetCDDir(const char*)",
    ): "LEMBALL.EXE: caller 0x00406e60 loads the platform object into ECX "
    "before CALL 0x0045eda0; the callee returns with RET 4 at 0x0045ee61. "
    "Windows uses a member function for the catalog's free function.",
}

TYPE_DEF = re.compile(
    r"\b(?:class|struct)\s+(?P<name>\w+)\s*(?:final\s*)?(?::[^;{}]*)?\{"
)

WINDOWS_MARK = re.compile(
    rf"//\s*(?P<kind>FUNCTION|STUB|SYNTHETIC|TEMPLATE|LIBRARY):\s*{re.escape(TARGET_ID)}\s+(?P<address>0x[0-9a-fA-F]+)\b"
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


CANDIDATE_PRIORITY = {
    "match": 0,
    "windows": 1,
    "case": 2,
    "mismatch": 3,
}


def annotated_signature(code, block, token, limit, ranges):
    """Resolve ordinary declarations, explicit SYMBOLs, and compiler descriptors."""
    following = next((item for item in block if item.start() > token.start()), None)
    label = following[0][2:].strip() if following is not None else ""
    if following is not None and WINDOWS_MARK.match(following[0]):
        label = ""
    if "SYMBOL" in token[0] or label.startswith("?"):
        if not label:
            raise ValueError("missing explicit SYMBOL name")
        if not label.startswith("?"):
            return Signature("", label, None)
        declaration = msvc_demangle(label)
        declaration = re.sub(r"`(?:adjustor|vtordisp)\{[^}]*\}'", "", declaration)
        destructor = re.search(
            r"(?P<owner>[\w:]+)::`(?:scalar|vector) deleting (?:dtor|destructor)'",
            declaration,
        )
        if destructor:
            return Signature(destructor["owner"], "<destructor>", None)
        return adjacent_signature(declaration, 0, [])
    if "LIBRARY:" in token[0]:
        if not label:
            raise ValueError("missing library symbol name")
        return Signature("", label, None)
    if "SYNTHETIC:" in token[0]:
        if re.fullmatch(r"\$E\d+", label):
            return Signature("", label, None)
        label = re.sub(r"`(?:adjustor|vtordisp)\{[^}]*\}'", "", label)
        owner, method = label.rsplit("::", 1)
        if method in ("`scalar deleting destructor'", "`vector deleting destructor'"):
            method = "~" + owner.split("::")[-1]
        signature = adjacent_signature(f"{owner}::{method}()", 0, [])
        return Signature(signature.owner, signature.method, None)
    return adjacent_signature(code[:limit], block[-1].end(), ranges)


def scan(path, symbols, by_windows, inferences=None):
    """Attach each Windows annotation to its declaration and catalog candidates."""
    text = path.read_text(encoding="utf-8")
    code = mask_comments_and_strings(text)
    ends = delimiter_ends(code, "{", "}")
    ranges = [
        (opening, ends[opening], match["name"])
        for match in TYPE_DEF.finditer(code)
        if (opening := match.end() - 1) in ends
    ]
    for block, limit in annotation_blocks(text, code):
        for token in block:
            marker = WINDOWS_MARK.match(token[0])
            if marker is None:
                continue
            address = int(marker["address"], 16)
            row = {
                "path": str(path),
                "line": text.count("\n", 0, token.start()) + 1,
                "windows_address": f"0x{address:08x}",
                "kind": marker["kind"],
            }
            candidates = by_windows.get(address)
            try:
                actual = annotated_signature(code, block, token, limit, ranges)
            except ValueError as error:
                yield dict(
                    row,
                    status="unresolved" if candidates else "unmapped",
                    reason=f"{error}; no verified source identity",
                )
                continue
            actual_signature = actual.display()
            row["actual_signature"] = actual_signature
            if not candidates:
                inferred_signature = (inferences or {}).get(address)
                if inferred_signature == actual_signature:
                    yield dict(
                        row, status="inferred", reason="explicit source inference"
                    )
                else:
                    reason = f"{actual_signature}: no Windows catalog mapping or explicit inference"
                    if inferred_signature:
                        reason += f"; recorded inference is {inferred_signature}"
                    yield dict(row, status="unmapped", reason=reason)
                continue
            if actual.method == f"__lemball_jump_{address:08x}":
                yield dict(
                    row,
                    status="synthetic",
                    reason="address-derived linker label; mapped target has no separate C++ declaration",
                )
                continue
            comparisons = []
            for mac in candidates:
                symbol = symbols[mac]
                comparison = compare_signature(decode_signature(symbol), actual)
                if marker["kind"] == "SYNTHETIC":
                    comparison["signature_status"] = "unencoded"
                evidence = WINDOWS_NAME_REVIEWS.get((address, symbol, actual_signature))
                if evidence and comparison["status"] != "match":
                    comparison.update(
                        status="windows",
                        signature_status="review",
                        windows_evidence=evidence,
                    )
                comparisons.append(comparison)
            best = min(
                comparisons,
                key=lambda candidate: (
                    CANDIDATE_PRIORITY[candidate["status"]],
                    candidate["signature_status"] != "match",
                ),
            )
            yield dict(row, **best)
