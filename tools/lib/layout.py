#!/usr/bin/env python3
"""Check one primary class per file and matching class/filename spelling."""

import re
import sys
from collections import Counter
from pathlib import Path

from . import ROOT, TYPE_DEF, VTABLE_MARK, brace_ends, collect_sources, mask_comments_and_strings

# Out-of-line definitions start in column 0. Indented Class::Call sites are ignored.
METHOD_DEF = re.compile(
    r"^(?:(?P<ret>(?:(?:unsigned|signed|const|volatile|static|inline|virtual)\s+)*"
    r"[A-Za-z_][\w:]*(?:\s*<[^;{}<>]*>)?(?:\s*\*|\s*&)?)\s+)?"
    r"(?P<owner>[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*)::"
    r"(?P<method>~?[A-Za-z_]\w*|operator\s*[^\s(]+)\s*\(",
    re.MULTILINE,
)


def has_vtable_above(text, type_offset):
    for line in reversed(text[:type_offset].splitlines()):
        line = line.strip()
        if not line:
            continue
        if not line.startswith("//"):
            break
        if VTABLE_MARK.match(line):
            return True
    return False


def primary_names(path, text, code):
    if path.suffix == ".cpp":
        owners = {}
        for match in METHOD_DEF.finditer(code):
            owner = match["owner"].split("::")[0]
            owners.setdefault(owner.casefold(), owner)
        return sorted(owners.values(), key=str.casefold)
    ends = brace_ends(code)
    types = [match for match in TYPE_DEF.finditer(code) if match.end() - 1 in ends]
    types = [match for match in types if not any(
        other.end() - 1 < match.start() < ends[other.end() - 1] for other in types)]
    vtables = {match["name"] for match in types if has_vtable_above(text, match.start())}
    if vtables:
        return sorted(vtables)
    classes = {match["name"] for match in types if match["kind"] == "class"}
    matched = {match["name"] for match in types if match["name"].casefold() == path.stem.casefold()}
    return sorted(matched | classes or {match["name"] for match in types})


def scan(path: Path) -> dict:
    text = path.read_text(encoding="utf-8")
    code = mask_comments_and_strings(text)
    primary = primary_names(path, text, code)
    stem = path.stem
    expected = primary[0] if len(primary) == 1 else None

    if len(primary) > 1:
        status = "multi-class"
        detail = "primary classes: " + ", ".join(primary)
    elif expected is not None and stem.casefold() != expected.casefold():
        status = "stem-name"
        detail = f"stem {stem} != expected {expected} (class)"
    elif not primary:
        status = "free"
        detail = "no primary class"
    else:
        status = "match"
        detail = None

    return {"relpath": path.resolve().relative_to(ROOT).as_posix(),
            "primary": primary, "status": status, "detail": detail}


def check_layout(paths=None):
    files = collect_sources(paths)
    if not files:
        print("layout: no C++ source files found", file=sys.stderr)
        return 2
    rows = [scan(path) for path in files]
    failures = [r for r in rows if r["status"] in ("multi-class", "stem-name")]
    for row in failures:
        print(f'{row["relpath"]}: {row["status"]}: {row["detail"]}')
    print(f'{len(files)} files: {dict(Counter(row["status"] for row in rows))}')
    return int(bool(failures))
