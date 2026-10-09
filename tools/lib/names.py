"""Check source class and method identities at catalog-mapped Windows addresses."""

import csv
import re
from collections import defaultdict

from reccmp.cvdump.demangler import msvc_demangle

from .codewarrior import decode_signature
from .signatures import Signature, adjacent_signature, delimiter_ends
from . import TARGET_ID
from .scan import TOKENS, mask_comments_and_strings

WINDOWS_NAMES = {
    (
        0x00471AF0,
        "SysCloseSocket__14CTCPIPRWSocketFv",
        "CTCPIPCommonSocket",
        "SysCloseSocket",
    ),
    (
        0x0043A500,
        "OnZoomBox__4CWndFUc",
        "CWnd",
        "OnDriverChange",
    ),
    (
        0x0045EDA0,
        "GetCDDir__FPCc",
        "CPlatformServices",
        "GetCDDir",
    ),
}

TYPE_DEF = re.compile(
    r"\b(?:class|struct)\s+(?P<name>\w+)\s*(?:final\s*)?(?::[^;{}]*)?\{"
)

WINDOWS_MARK = re.compile(
    rf"//\s*(?P<kind>FUNCTION|STUB|SYNTHETIC|TEMPLATE|LIBRARY):\s*{re.escape(TARGET_ID)}\s+(?P<address>0x[0-9a-fA-F]+)\b"
)


def read_catalog(path):
    """Read symbol identities and Windows mappings from the fixed catalog."""
    symbols, by_windows = {}, defaultdict(list)
    with path.open(newline="", encoding="utf-8-sig") as stream:
        rows = csv.reader(stream)
        next(rows)
        for mac, name, win in rows:
            mac = int(mac, 16)
            symbols[mac] = name
            if win:
                by_windows[int(win, 16)].append(mac)
    for candidates in by_windows.values():
        candidates.sort()
    return symbols, by_windows


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


def scan(path, symbols, by_windows, thunks):
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
            candidates = by_windows.get(address)
            if not candidates:
                continue
            row = {
                "path": str(path),
                "line": text.count("\n", 0, token.start()) + 1,
                "windows_address": f"0x{address:08x}",
                "kind": marker["kind"],
            }
            try:
                actual = annotated_signature(code, block, token, limit, ranges)
            except ValueError as error:
                yield dict(
                    row,
                    status="unresolved",
                    reason=str(error),
                )
                continue
            if (
                marker["kind"] == "SYNTHETIC"
                and not actual.owner
                and actual.method == thunks.get(address)
            ):
                yield dict(row, status="match")
                continue
            expected = [decode_signature(symbols[mac]) for mac in candidates]
            matches = any(
                (wanted.owner, wanted.method) == (actual.owner, actual.method)
                or (address, symbols[mac], actual.owner, actual.method) in WINDOWS_NAMES
                for mac, wanted in zip(candidates, expected, strict=True)
            )
            yield dict(
                row,
                status="match" if matches else "mismatch",
                reason=f"{'; '.join(wanted.display() for wanted in expected)} -> {actual.display()}",
            )
