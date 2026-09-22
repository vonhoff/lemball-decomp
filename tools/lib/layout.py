#!/usr/bin/env python3
"""Check one primary class per file and matching class/filename spelling."""

from __future__ import annotations

import json
import re
import sys
from collections import Counter
from pathlib import Path

from .paths import ROOT
from .source import (
    RECCMP_MARK,
    brace_ends,
    collect_sources,
    mask_comments_and_strings,
    rel_posix,
)
from .source import (
    VTABLE_MARK as VTABLE,
)

TYPE_DEF = re.compile(
    r"\b(?P<kind>class|struct)\s+(?P<name>\w+)\s*(?:final\s*)?(?::[^;{}]*)?\{"
)
# Out-of-line definitions start in column 0. Indented Class::Call sites are ignored.
METHOD_DEF = re.compile(
    r"^(?:(?P<ret>(?:(?:unsigned|signed|const|volatile|static|inline|virtual)\s+)*"
    r"[A-Za-z_][\w:]*(?:\s*<[^;{}<>]*>)?(?:\s*\*|\s*&)?)\s+)?"
    r"(?P<owner>[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*)::"
    r"(?P<method>~?[A-Za-z_]\w*|operator\s*[^\s(]+)\s*\(",
    re.MULTILINE,
)


def class_stem(name: str) -> str:
    return name.split("::")[0]


def stems_equal(left: str, right: str) -> bool:
    return left.casefold() == right.casefold()


def line_of(text: str, offset: int) -> int:
    return text.count("\n", 0, offset) + 1


def has_vtable_above(text: str, type_offset: int) -> bool:
    block_start = text.rfind("\n", 0, type_offset) + 1
    while block_start > 0:
        prev = text.rfind("\n", 0, block_start - 1) + 1
        line = text[prev:block_start]
        stripped = line.strip()
        if not stripped:
            block_start = prev
            continue
        if stripped.startswith("//"):
            if VTABLE.match(stripped):
                return True
            block_start = prev
            continue
        break
    return False


def top_level_types(text: str, code: str) -> list[dict]:
    ends = brace_ends(code)
    ranges = []
    for match in TYPE_DEF.finditer(code):
        opening = match.end() - 1
        if opening not in ends:
            continue
        ranges.append(
            {
                "kind": match["kind"],
                "name": match["name"],
                "start": match.start(),
                "open": opening,
                "end": ends[opening],
                "line": line_of(text, match.start()),
            }
        )
    top = []
    for entry in ranges:
        nested = any(
            other["open"] < entry["start"] < other["end"] for other in ranges if other is not entry
        )
        if nested:
            continue
        entry["vtable"] = has_vtable_above(text, entry["start"])
        top.append(entry)
    return top


def method_owners(code: str) -> list[str]:
    owners = []
    for match in METHOD_DEF.finditer(code):
        owner = match["owner"].split("::")[0]
        if owner:
            owners.append(owner)
    # Repeated definitions of methods belong to one primary class.
    by_stem = {}
    for owner in owners:
        by_stem.setdefault(class_stem(owner).casefold(), owner)
    return sorted(by_stem.values(), key=lambda name: class_stem(name).casefold())


def primary_names(path: Path, _text: str, code: str, types: list[dict]) -> list[str]:
    if path.suffix == ".cpp":
        owners = method_owners(code)
        if owners:
            return owners
        # Local helper structs in a free-function TU are not primary classes.
        return []
    vtable = [entry["name"] for entry in types if entry["vtable"]]
    if vtable:
        return sorted(set(vtable))
    classes = [entry for entry in types if entry["kind"] == "class"]
    matched = [
        entry
        for entry in types
        if stems_equal(entry["name"], path.stem)
        or stems_equal(class_stem(entry["name"]), path.stem)
    ]
    if matched:
        names = {entry["name"] for entry in matched}
        names.update(entry["name"] for entry in classes)
        return sorted(names)
    if classes:
        return sorted({entry["name"] for entry in classes})
    if types:
        return sorted({entry["name"] for entry in types})
    return []


def scan(path: Path) -> dict:
    text = path.read_text(encoding="utf-8")
    code = mask_comments_and_strings(text)
    types = top_level_types(text, code)
    primary = primary_names(path, text, code, types)
    helpers = sorted(
        {entry["name"] for entry in types if entry["name"] not in primary}
    )
    stem = path.stem
    rel = rel_posix(path)
    class_stems = [class_stem(name) for name in primary]
    if len(class_stems) == 1:
        expected, evidence = class_stems[0], "class"
    else:
        expected, evidence = None, None

    if len(primary) > 1:
        status = "multi-class"
        detail = "primary classes: " + ", ".join(primary)
    elif expected is not None and not stems_equal(stem, expected):
        status = "stem-name"
        detail = f"stem {stem} != expected {expected} ({evidence})"
    elif not primary:
        status = "free"
        detail = "no primary class"
        if RECCMP_MARK.search(text):
            detail = "free-function or data TU"
    else:
        status = "match"
        detail = None

    return {
        "path": str(path),
        "relpath": rel,
        "line": 1,
        "stem": stem,
        "primary": primary,
        "helpers": helpers,
        "expected_stem": expected,
        "name_evidence": evidence,
        "status": status,
        "detail": detail,
    }


def check_layout(
    paths: list[Path | str] | None = None,
    fail: bool = True,
    as_json: bool = False,
) -> int:
    files = collect_sources(paths)
    if not files:
        sys.stderr.write("layout: no C++ source files found\n")
        return 2
    rows = [scan(path) for path in files]
    counts = dict(Counter(row["status"] for row in rows))
    failing = {"multi-class", "stem-name"}
    selected = [row for row in rows if row["status"] in failing]
    if as_json:
        print(
            json.dumps(
                {"files": len(files), "counts": counts, "translations": rows},
                indent=2,
            )
        )
    else:
        for row in selected:
            detail = row["detail"] or row["status"]
            print(f'{row["relpath"]}: {row["status"]}: {detail}')
        print(f'{len(files)} files: {counts}')
    if fail and selected:
        return 1
    return 0
