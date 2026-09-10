#!/usr/bin/env python3
"""Fail on decomp smells (via tools/gate.py)."""

from __future__ import annotations

import argparse
import json
import re
import sys
from collections import Counter
from pathlib import Path

from .paths import ROOT, SRC
from .source import RECCMP_MARK, SYNTHETIC_MARK, collect_sources

BASELINE = ROOT / "smell.baseline.json"

# Hit: relative path, line, rule, optional code snippet
Hit = tuple[str, int, str, str]

K68_MARK = re.compile(r"^\s*//\s*68K\s+")
SYNTHETIC_DTOR_NAME = re.compile(
    r"^\s*//\s*(?P<class>[A-Za-z_][\w:]*)::`(?:scalar|vector) deleting destructor'\s*$"
)
VBPTR_WALK = re.compile(
    r"\*\(\s*int\s*\*\s*\)\s*\(\s*\*\(\s*int\s*\*\s*\)\s*\(\s*[^;]{1,60}?\+\s*0x40\s*\)\s*\+\s*4\s*\)"
)
THIS_ADJUST = re.compile(r"\(\s*char\s*\*\s*\)\s*this\s*-\s*(?:0x[0-9A-Fa-f]+|\d+)\b")
EXPR_CHAR_OFFSET = re.compile(
    r"\(\s*char\s*\*\s*\)"
    r"(?:\(\s*)?"
    r"\s*"
    r"(?P<expr>this|[A-Za-z_][\w]*(?:\s*(?:->|\.)\s*[A-Za-z_][\w]*|\[[^\]]+\])*)"
    r"\s+[+-]\s+(?:0x[0-9A-Fa-f]+|\d+)\b"
)
MI_DTOR_POKE = re.compile(
    r"\(\s*[A-Za-z_][\w]*\s*\*\s*\)\s*"
    r"\(\s*\(\s*char\s*\*\s*\)[^;]{1,120}?~\s*[A-Za-z_][\w]*\s*\("
)
PTR_CAST = re.compile(
    r"\(\s*(?:unsigned\s+|signed\s+)?(?:char|short|int|long|void|__int16|__int32)\s*\*\s*\)"
)
RAW_CAST_TYPE = (
    r"(?:"
    r"(?:unsigned\s+|signed\s+)(?:char|short|int|long)"
    r"|(?:char|short|int|long|void|__int16|__int32)"
    r"|[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*"
    r")\s*(?:\*\s*)+"
)
RAW_DEREF_CAST = re.compile(
    r"\*\s*\(\s*(?P<type>" + RAW_CAST_TYPE + r")\s*\)\s*(?P<rhs>[^;\n]+)"
)
# *((TYPE) rhs) with balanced outer parens; TYPE is parsed separately.
PAREN_RAW_DEREF_START = re.compile(r"\*\s*\(")
CAST_TYPE_AT = re.compile(r"\(\s*(?P<type>" + RAW_CAST_TYPE + r")\s*\)")
CAST_THEN_ARITH = re.compile(
    PTR_CAST.pattern
    + r"\s*(?:\([^;]{1,80}?\)|[A-Za-z_][\w.]*(?:\[[^\]]{0,40}\])?)"
    + r"(?:\s*[+-]\s*[A-Za-z_][\w]*)*"
    + r"\s*[+-]\s*(?:0x[0-9A-Fa-f]+|\d+)\b"
)
CAST_PAREN_ARITH = re.compile(
    PTR_CAST.pattern + r"\s*\(\s*[^;]{1,80}?\s*[+-]\s*(?:0x[0-9A-Fa-f]+|(?!1\b)\d+)\s*\)"
)
# Cast away a pointer type, then index with a byte offset / element size.
# Bare int* tables like layout[0x58 / 4] are incomplete typing, not this smell.
CAST_BYTE_DIV_INDEX = re.compile(
    r"\(\s*\(\s*"
    + RAW_CAST_TYPE
    + r"\s*\)\s*[^;\n\[\]]{1,80}?"
    + r"\)\s*\[\s*0x[0-9A-Fa-f]+\s*/\s*(?:0x[0-9A-Fa-f]+|[248])\s*\]"
)
NAKED_DATA_OFFSET = re.compile(
    r"(?:->|\.)(?:m_data|m_buffer)\s*\+\s*(?:0x[0-9A-Fa-f]+|\d+)\b"
    r"|(?<![\w.])(?:m_data|m_buffer)\s*\+\s*(?:0x[0-9A-Fa-f]+|\d+)\b"
)
CHAR_VAR_OFFSET = re.compile(
    r"\(\s*char\s*\*\s*\)\s*(?P<expr>this|[A-Za-z_][\w]*)\s*[+-]\s*(?!0x)(?P<off>[A-Za-z_][\w]*)"
)
METHOD_DEF = re.compile(r"^[A-Za-z_][\w:]*::~?[A-Za-z_][\w]*\s*\(")
DTOR_DEF = re.compile(r"^(?P<class>[A-Za-z_][\w:]*)::~[A-Za-z_][\w]*\s*\(")
FREE_DEF = re.compile(r"^(?:static\s+)?(?:[A-Za-z_][\w:*&]*\s+)+\w+\s*\(")
BUFFER_OK = re.compile(r"m_numberBuffer|Bits\b|sz[A-Z]|\bp_bits\b")
SKIP_LEAD = {"if", "while", "for", "switch", "return", "else", "case", "catch", "extern"}
SCALAR_PTR_BASE = re.compile(
    r"(?:(?:unsigned|signed)\s+)?(?:char|short|int|long|__int16|__int32)"
)
RHS_HAS_ARITH = re.compile(r"\s[+-]\s|\b[+-]\s*(?:0x[0-9A-Fa-f]+|\d+|[A-Za-z_])")
RHS_LITERAL_ADDR = re.compile(r"^(?:0x[0-9A-Fa-f]+|\d+)\b")


def strip_line_comment(line: str) -> str:
    in_str = False
    for i, ch in enumerate(line):
        if ch == '"' and (i == 0 or line[i - 1] != "\\"):
            in_str = not in_str
        if not in_str and line.startswith("//", i):
            return line[:i]
    return line


def is_func_def(stripped: str) -> bool:
    if not stripped or stripped.startswith(("#", "//", ":", "*", "}")):
        return False
    if stripped.endswith(";") or stripped.endswith(","):
        return False
    lead = stripped.split(None, 1)
    if lead and lead[0] in SKIP_LEAD:
        return False
    if METHOD_DEF.match(stripped):
        return True
    if "::" in stripped.split("(", 1)[0]:
        return False
    return bool(FREE_DEF.match(stripped))


def body_is_empty(lines: list[str], index: int) -> bool:
    saw_open = False
    for line in lines[index:index + 6]:
        text = line.strip()
        if "{" in text:
            saw_open = True
            after = text.split("{", 1)[1].strip()
            if after.startswith("}"):
                return True
            if after:
                return False
        elif saw_open:
            if text.startswith("}"):
                return True
            if text:
                return False
    return False


def preceding_block(lines: list[str], index: int) -> list[str]:
    block = []
    i = index - 1
    while i >= 0:
        raw = lines[i].strip()
        if raw == "":
            if block:
                break
            i -= 1
            continue
        if raw.startswith("//"):
            block.append(raw)
            i -= 1
            continue
        break
    block.reverse()
    return block


def is_offset_poke(code: str) -> bool:
    if BUFFER_OK.search(code):
        return False
    if CAST_THEN_ARITH.search(code) or CAST_PAREN_ARITH.search(code) or NAKED_DATA_OFFSET.search(code):
        return True
    return any(not BUFFER_OK.search(match.group("expr")) for match in EXPR_CHAR_OFFSET.finditer(code))


def iter_raw_deref_casts(code: str):
    """Yield (type_text, rhs, form) for *(T*)rhs and *((T*) rhs)."""
    for match in RAW_DEREF_CAST.finditer(code):
        yield match.group("type"), match.group("rhs").strip(), "direct"

    for start in PAREN_RAW_DEREF_START.finditer(code):
        cast = CAST_TYPE_AT.match(code, start.end())
        if cast is None:
            continue
        type_text = cast.group("type")
        pos = cast.end()
        depth = 1
        rhs_start = pos
        while pos < len(code) and depth:
            ch = code[pos]
            if ch == "(":
                depth += 1
            elif ch == ")":
                depth -= 1
            pos += 1
        if depth != 0:
            continue
        rhs = code[rhs_start : pos - 1].strip()
        if rhs:
            yield type_text, rhs, "paren"


ADDR_OF = re.compile(r"^\(?\s*&")


def raw_cast_reason(code: str, audit: bool = False) -> str | None:
    for type_text, rhs, form in iter_raw_deref_casts(code):
        base_type = re.sub(r"\s*\*\s*", "", type_text).strip()
        scalar_type = SCALAR_PTR_BASE.fullmatch(base_type)
        if scalar_type is None and RHS_HAS_ARITH.search(rhs):
            return "cast-deref-offset"
        if scalar_type is None and RHS_LITERAL_ADDR.match(rhs):
            return "literal-address-cast"
        if ADDR_OF.match(rhs):
            return "addr-cast-punning"
        # In audit mode, report every direct cast dereference.
        if audit and form == "direct":
            return "raw-cast"
    return None


def format_hit(hit: Hit) -> str:
    rel, lineno, rule, code = hit
    if code:
        return "%s:%d: %s %s" % (rel, lineno, rule, code)
    return "%s:%d: %s" % (rel, lineno, rule)


def load_baseline() -> Counter[tuple[str, str, str]]:
    try:
        data = json.loads(BASELINE.read_text(encoding="utf-8"))
    except (OSError, ValueError) as error:
        raise ValueError("cannot read %s: %s" % (BASELINE, error))
    if data.get("version") != 1 or not isinstance(data.get("findings"), list):
        raise ValueError("%s must contain version 1 and a findings list" % BASELINE)
    baseline: Counter[tuple[str, str, str]] = Counter()
    for item in data["findings"]:
        try:
            key = (item["path"], item["rule"], item.get("code", ""))
            count = item.get("count", 1)
            reason = item["reason"]
        except (AttributeError, KeyError):
            raise ValueError("invalid finding in %s" % BASELINE)
        if (
            not all(isinstance(value, str) for value in key)
            or not isinstance(count, int)
            or count < 1
            or not isinstance(reason, str)
            or not reason.strip()
        ):
            raise ValueError("invalid finding in %s" % BASELINE)
        baseline[key] += count
    return baseline


def apply_baseline(hits: list[Hit], files: list[Path]) -> tuple[list[Hit], list[str]]:
    baseline = load_baseline()
    remaining = baseline.copy()
    unbaselined: list[Hit] = []
    for hit in hits:
        key = (hit[0], hit[2], hit[3])
        if remaining[key] != 0:
            remaining[key] -= 1
        else:
            unbaselined.append(hit)
    scanned = {path.resolve().relative_to(ROOT).as_posix() for path in files}
    stale = []
    for (path, rule, code), count in sorted(remaining.items()):
        if count != 0 and path in scanned:
            stale.append("%s: baseline-stale %s %s (count %d)" % (path, rule, code, count))
    return unbaselined, stale


def scan_file(
    path: Path,
    annot: bool,
    annot_strict: bool,
    synthetic_destructors: set[str],
    raw_cast_audit: bool = False,
) -> tuple[list[Hit], list[tuple[str, str]]]:
    lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
    hits: list[Hit] = []
    reviews: list[tuple[str, str]] = []
    rel = path.resolve().relative_to(ROOT).as_posix()
    if path.suffix.lower() in {".cpp", ".h", ".c"}:
        for lineno, raw in enumerate(lines, 1):
            code = strip_line_comment(raw)
            raw_cast = raw_cast_reason(code, audit=raw_cast_audit)
            if raw_cast is not None:
                hits.append((rel, lineno, raw_cast, code.strip()[:100]))
            if VBPTR_WALK.search(code):
                hits.append((rel, lineno, "vbptr-walk", ""))
            if THIS_ADJUST.search(code):
                hits.append((rel, lineno, "this-adjust-poke", ""))
            poked = False
            type_erase = CAST_BYTE_DIV_INDEX.search(code)
            if type_erase is not None:
                hits.append((rel, lineno, "type-erase-index", type_erase.group(0).strip()[:100]))
                poked = True
            for match in EXPR_CHAR_OFFSET.finditer(code):
                if not BUFFER_OK.search(match.group("expr")):
                    hits.append((rel, lineno, "expr-char-offset", match.group(0).strip()[:100]))
                    poked = True
                    break
            if MI_DTOR_POKE.search(code):
                hits.append((rel, lineno, "mi-dtor-poke", code.strip()[:100]))
                poked = True
            if not poked and is_offset_poke(code):
                hits.append((rel, lineno, "offset-poke", code.strip()[:100]))
            for match in CHAR_VAR_OFFSET.finditer(code):
                if BUFFER_OK.search(match.group("expr")):
                    continue
                if match.group("off") == "sizeof":
                    continue
                hits.append((rel, lineno, "offset-poke", match.group(0).strip()))
    if path.suffix.lower() != ".cpp":
        return hits, reviews
    for i, raw in enumerate(lines):
        stripped = raw.strip()
        if not is_func_def(stripped):
            continue
        prev = preceding_block(lines, i)
        has_reccmp = any(RECCMP_MARK.match(line) for line in prev)
        has_68k = any(K68_MARK.match(line) for line in prev)
        dtor = DTOR_DEF.match(stripped)
        has_synthetic_dtor = dtor is not None and dtor.group("class") in synthetic_destructors
        if has_reccmp:
            continue
        if annot_strict:
            disposition = "hit"
        elif annot:
            if body_is_empty(lines, i):
                disposition = "review-empty"
            elif has_synthetic_dtor:
                disposition = "review-synthetic"
            else:
                disposition = "hit"
        elif has_68k or body_is_empty(lines, i):
            continue
        else:
            disposition = "hit"
        kind = "incomplete-annotation" if has_68k else "no-annotation"
        record = "%s:%d: %s %s" % (rel, i + 1, kind, stripped[:90])
        if disposition.startswith("review-"):
            reviews.append((disposition, record))
        else:
            hits.append((rel, i + 1, kind, stripped[:90]))
    return hits, reviews


def collect_synthetic_destructors(files: list[Path]) -> set[str]:
    classes: set[str] = set()
    for path in files:
        lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
        for index, line in enumerate(lines[:-1]):
            if not SYNTHETIC_MARK.match(line):
                continue
            name = SYNTHETIC_DTOR_NAME.match(lines[index + 1])
            if name is not None:
                classes.add(name.group("class"))
    return classes


def check_smell(
    paths: list[Path | str] | None = None,
    annot: bool = False,
    annot_strict: bool = False,
    raw_cast_audit: bool = False,
) -> int:
    files = collect_sources(paths)
    if not files:
        sys.stderr.write("smell: no C++ source files found\n")
        return 2
    hits: list[Hit] = []
    reviews: list[tuple[str, str]] = []
    synthetic_roots = collect_sources() if paths is None else files
    synthetic_destructors = collect_synthetic_destructors(synthetic_roots)
    for path in files:
        file_hits, file_reviews = scan_file(
            path, annot, annot_strict, synthetic_destructors, raw_cast_audit=raw_cast_audit
        )
        hits.extend(file_hits)
        reviews.extend(file_reviews)
    try:
        hits, stale = apply_baseline(hits, files)
    except ValueError as error:
        sys.stderr.write("smell: %s\n" % error)
        return 2
    if reviews:
        empty_reviews = sum(reason == "review-empty" for reason, _ in reviews)
        synthetic_reviews = sum(reason == "review-synthetic" for reason, _ in reviews)
        sys.stderr.write(
            "smell: review empty=%d synthetic=%d (use --annot-strict)\n"
            % (empty_reviews, synthetic_reviews)
        )
    messages = [format_hit(hit) for hit in hits]
    messages.extend(stale)
    if messages:
        sys.stderr.write("smell: %d hit(s)\n" % len(messages))
        for message in messages:
            sys.stderr.write(message + "\n")
        return 1
    sys.stdout.write("smell: ok\n")
    return 0


def main(argv: list[str] | None = None) -> int:
    if argv is None:
        argv = sys.argv
    parser = argparse.ArgumentParser(description="Decomp smell gate")
    annot_group = parser.add_mutually_exclusive_group()
    annot_group.add_argument(
        "--annot",
        action="store_true",
        help="fail actionable unannotated defs; summarize empty/synthetic for review",
    )
    annot_group.add_argument(
        "--annot-strict",
        action="store_true",
        help="fail every definition lacking a reccmp annotation",
    )
    parser.add_argument(
        "--raw-casts",
        action="store_true",
        help="fail on every direct scalar/pointer dereference cast (audit mode)",
    )
    parser.add_argument("paths", nargs="*", help="files or dirs (default src)")
    args = parser.parse_args(argv[1:])
    return check_smell(
        args.paths,
        annot=args.annot,
        annot_strict=args.annot_strict,
        raw_cast_audit=args.raw_casts,
    )


if __name__ == "__main__":
    sys.exit(main())
