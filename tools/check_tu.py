#!/usr/bin/env python3
"""Check translation-unit layout against one-primary-class naming.

python tools/check_tu.py [src/path ...] [--json] [--fail]

Default rule: one primary class per .h/.cpp, file stem equals that class
name (after dropping a leading C/tag/t type prefix). OVERRIDE_STEMS lists
proven original TU basenames where the stem differs from the primary class.
INTENTIONAL lists full-path exceptions. No source is changed.
"""

from __future__ import annotations

import argparse
from collections import Counter
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]

VTABLE = re.compile(r"^\s*//\s*VTABLE:\s+LEMBALL\b", re.M)
TYPE_DEF = re.compile(
    r"\b(?P<kind>class|struct)\s+(?P<name>\w+)\s*(?:final\s*)?(?::[^;{}]*)?\{"
)
# Out-of-line definitions start in column 0. Indented Class::Call sites are ignored.
METHOD_DEF = re.compile(
    r"^(?:(?P<ret>(?:(?:unsigned|signed|const|volatile|static|inline|virtual)\s+)*"
    r"[A-Za-z_][\w:]*(?:\s*<[^;{}<>]*>)?(?:\s*\*|\s*&)?)\s+)?"
    r"(?P<owner>[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*)::"
    r"(?P<method>~?[A-Za-z_]\w*|operator\s*[^\s(]+)\s*\(",
    re.M,
)
RECCMP_MARK = re.compile(
    r"^\s*//\s*(?:FUNCTION|STUB|SYNTHETIC|TEMPLATE|LIBRARY|GLOBAL)\s*:",
    re.M,
)

# File stem -> note. Stem is allowed even when it differs from the primary class.
# Evidence: original _VSRELassert __FILE__ strings.
OVERRIDE_STEMS = {
    "VsGdi": "original VSGDI.CPP; primary Surface",
    "MogLoad": "original MOGLOAD.CPP; primary MogDir",
}

# Relative POSIX paths. Skip the default rule entirely for these files.
INTENTIONAL = {
    "src/Common.h",  # scaffold forward declarations and tiny POD helpers
}


def mask_comments_and_strings(text: str) -> str:
    pattern = r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\''
    return re.sub(pattern, lambda m: re.sub(r"[^\n]", " ", m[0]), text)


def brace_ends(code: str) -> dict[int, int]:
    ends, stack = {}, []
    for pos, char in enumerate(code):
        if char == "{":
            stack.append(pos)
        elif char == "}" and stack:
            ends[stack.pop()] = pos
    return ends


def drop_type_prefix(name: str) -> str:
    for prefix in ("tag", "t", "C"):
        if len(name) > len(prefix) and name.startswith(prefix) and name[len(prefix)].isupper():
            return name[len(prefix) :]
    return name


def class_stem(name: str) -> str:
    return drop_type_prefix(name.split("::")[0])


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
    # CConnect / Connect typedef aliases collapse to one primary.
    by_stem = {}
    for owner in owners:
        by_stem.setdefault(class_stem(owner).casefold(), owner)
    return sorted(by_stem.values(), key=lambda name: class_stem(name).casefold())


def primary_names(path: Path, text: str, code: str, types: list[dict]) -> list[str]:
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


def override_note(stem: str) -> str | None:
    for key, note in OVERRIDE_STEMS.items():
        if stems_equal(stem, key):
            return note
    return None


def rel_posix(path: Path) -> str:
    return path.resolve().relative_to(ROOT).as_posix()


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
    note = override_note(stem)
    expected = None
    evidence = None
    if note is not None:
        expected = stem
        evidence = "override"
    elif len(class_stems) == 1:
        expected = class_stems[0]
        evidence = "class"
    elif len(class_stems) > 1:
        expected = None
        evidence = None
    else:
        expected = None
        evidence = None

    if rel in INTENTIONAL:
        status = "intentional"
        detail = "listed in INTENTIONAL"
    elif len(primary) > 1:
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
        if evidence == "override" and primary and not any(
            stems_equal(stem, class_stem(name)) for name in primary
        ):
            detail = note

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


def main() -> int:
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument("paths", type=Path, nargs="*", default=[ROOT / "src"])
    parser.add_argument("--json", action="store_true")
    parser.add_argument("--fail", action="store_true")
    args = parser.parse_args()
    files = set()
    for path in args.paths:
        if not path.exists():
            parser.error("path does not exist: " + str(path))
        files.update(
            p
            for p in (path.rglob("*") if path.is_dir() else [path])
            if p.suffix in (".cpp", ".h")
        )
    if not files:
        parser.error("no C++ source files found")
    rows = [scan(path) for path in sorted(files)]
    counts = dict(Counter(row["status"] for row in rows))
    failing = {"multi-class", "stem-name"}
    selected = [row for row in rows if row["status"] in failing]
    if args.json:
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
    if args.fail and selected:
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
