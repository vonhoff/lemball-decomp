#!/usr/bin/env python3
"""Check one primary class per file and matching class/filename spelling."""

import re
import sys
from collections import Counter

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
    """Look for a vtable marker in the comments immediately above a type."""
    for line in reversed(text[:type_offset].splitlines()):
        line = line.strip()
        if not line:
            continue
        if not line.startswith("//"):
            break
        if VTABLE_MARK.match(line):
            return True
    return False


def top_level_types(code):
    """Yield complete type definitions, skipping nested classes and structs."""
    ends = brace_ends(code)
    previous_end = 0
    for match in TYPE_DEF.finditer(code):
        closing = ends.get(match.end() - 1)
        if closing is not None and match.start() >= previous_end:
            yield match
            previous_end = closing


def primary_names(path, text, code):
    """Use method owners for .cpp; prefer vtables, then classes or the header stem."""
    if path.suffix == ".cpp":
        owners = {}
        for match in METHOD_DEF.finditer(code):
            owner = match["owner"].split("::")[0]
            owners.setdefault(owner.casefold(), owner)
        return sorted(owners.values(), key=str.casefold)
    types = list(top_level_types(code))
    vtables = {match["name"] for match in types if has_vtable_above(text, match.start())}
    if vtables:
        return sorted(vtables)
    classes = {match["name"] for match in types if match["kind"] == "class"}
    matched = {match["name"] for match in types if match["name"].casefold() == path.stem.casefold()}
    return sorted(matched | classes or {match["name"] for match in types})


def check_layout(paths=None):
    files = collect_sources(paths)
    if not files:
        print("layout: no C++ source files found", file=sys.stderr)
        return 2
    counts = Counter()
    for path in files:
        text = path.read_text(encoding="utf-8")
        primary = primary_names(path, text, mask_comments_and_strings(text))
        relpath = path.resolve().relative_to(ROOT).as_posix()
        detail = None
        if len(primary) > 1:
            status = "multi-class"
            detail = "primary classes: " + ", ".join(primary)
        elif not primary:
            status = "free"
        elif path.stem.casefold() != primary[0].casefold():
            status = "stem-name"
            detail = f"stem {path.stem} != expected {primary[0]} (class)"
        else:
            status = "match"
        counts[status] += 1
        if detail:
            print(f"{relpath}: {status}: {detail}")
    print(f"{len(files)} files: {dict(counts)}")
    return int(bool(counts["multi-class"] or counts["stem-name"]))
