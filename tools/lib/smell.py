#!/usr/bin/env python3
"""Fail on decomp smells (via tools/gate.py)."""

from __future__ import annotations

import re
import sys
from pathlib import Path

from . import RECCMP_MARK, ROOT, TOKENS, collect_sources, mask_comments_and_strings

# Hit: relative path, line number, rule, complete source line without trailing comments.
Hit = tuple[str, int, str, str]

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
FREE_DEF = re.compile(r"^(?:static\s+)?(?:[A-Za-z_][\w:*&]*\s+)+\w+\s*\(")
BUFFER_OK = re.compile(r"m_numberBuffer|Bits\b|sz[A-Z]|\bp_bits\b")
SKIP_LEAD = {"if", "while", "for", "switch", "return", "else", "case", "catch", "extern"}
SCALAR_PTR_BASE = re.compile(
    r"(?:(?:unsigned|signed)\s+)?(?:char|short|int|long|__int16|__int32)"
)
RHS_HAS_ARITH = re.compile(r"\s[+-]\s|\b[+-]\s*(?:0x[0-9A-Fa-f]+|\d+|[A-Za-z_])")
RHS_LITERAL_ADDR = re.compile(r"^(?:0x[0-9A-Fa-f]+|\d+)\b")


def is_func_def(stripped: str) -> bool:
    if not stripped or stripped.startswith(("#", "//", ":", "*", "}")):
        return False
    if stripped.endswith((";", ",")):
        return False
    lead = stripped.split(None, 1)
    if lead and lead[0] in SKIP_LEAD:
        return False
    if METHOD_DEF.match(stripped):
        return True
    if "::" in stripped.split("(", 1)[0]:
        return False
    return bool(FREE_DEF.match(stripped))


def declaration_has_body(code: str) -> bool:
    """A wrapped local constructor call or prototype ends in ';', not a body."""
    opening = code.find("(")
    if opening < 0:
        return False
    depth, end = 1, opening + 1
    while end < len(code) and depth:
        depth += (code[end] == "(") - (code[end] == ")")
        end += 1
    if depth:
        return False
    return bool(re.match(r"\s*(?:(?:const|volatile)\s*)*(?:\{|:(?!:))", code[end:]))


def iter_raw_deref_casts(code: str):
    """Yield (type_text, rhs) for *(T*)rhs and *((T*) rhs)."""
    for match in RAW_DEREF_CAST.finditer(code):
        yield match.group("type"), match.group("rhs").strip()

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
            yield type_text, rhs


ADDR_OF = re.compile(r"^\(?\s*&")


def raw_cast_reason(code: str) -> str | None:
    for type_text, rhs in iter_raw_deref_casts(code):
        base_type = re.sub(r"\s*\*\s*", "", type_text).strip()
        scalar_type = SCALAR_PTR_BASE.fullmatch(base_type)
        if scalar_type is None and RHS_HAS_ARITH.search(rhs):
            return "cast-deref-offset"
        if scalar_type is None and RHS_LITERAL_ADDR.match(rhs):
            return "literal-address-cast"
        if ADDR_OF.match(rhs):
            return "addr-cast-punning"
    return None


def scan_file(path: Path) -> list[Hit]:
    lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
    hits: list[Hit] = []
    rel = path.resolve().relative_to(ROOT).as_posix()
    for lineno, raw in enumerate(lines, 1):
        comment = next((token for token in TOKENS.finditer(raw) if token[0].startswith('//')), None)
        code = raw[:comment.start()] if comment else raw
        raw_cast = raw_cast_reason(code)
        if raw_cast is not None:
            hits.append((rel, lineno, raw_cast, code.strip()))
        if VBPTR_WALK.search(code):
            hits.append((rel, lineno, "vbptr-walk", code.strip()))
        if THIS_ADJUST.search(code):
            hits.append((rel, lineno, "this-adjust-poke", code.strip()))
        poked = False
        type_erase = CAST_BYTE_DIV_INDEX.search(code)
        if type_erase is not None:
            hits.append((rel, lineno, "type-erase-index", code.strip()))
            poked = True
        for match in EXPR_CHAR_OFFSET.finditer(code):
            if not BUFFER_OK.search(match.group("expr")):
                hits.append((rel, lineno, "expr-char-offset", code.strip()))
                poked = True
                break
        if MI_DTOR_POKE.search(code):
            hits.append((rel, lineno, "mi-dtor-poke", code.strip()))
            poked = True
        if (not poked and not BUFFER_OK.search(code)
                and (CAST_THEN_ARITH.search(code) or CAST_PAREN_ARITH.search(code)
                     or NAKED_DATA_OFFSET.search(code))):
            hits.append((rel, lineno, "offset-poke", code.strip()))
        for match in CHAR_VAR_OFFSET.finditer(code):
            if BUFFER_OK.search(match.group("expr")):
                continue
            if match.group("off") == "sizeof":
                continue
            hits.append((rel, lineno, "offset-poke", code.strip()))
    if path.suffix.lower() != ".cpp":
        return hits
    masked_lines = mask_comments_and_strings("\n".join(lines)).splitlines()
    for i, raw in enumerate(lines):
        stripped = raw.strip()
        if not is_func_def(stripped) or not declaration_has_body("\n".join(masked_lines[i:])):
            continue
        block = []
        for previous in reversed(lines[:i]):
            previous = previous.strip()
            if not previous:
                if block:
                    break
            elif previous.startswith("//"):
                block.append(previous)
            else:
                break
        if not any(RECCMP_MARK.match(line) for line in block):
            hits.append((rel, i + 1, 'no-annotation', stripped))
    return hits


def check_smell(paths=None) -> int:
    files = collect_sources(paths)
    if not files:
        sys.stderr.write('smell: no C++ source files found\n')
        return 2
    hits = [hit for path in files for hit in scan_file(path)]
    for path, line, rule, code in hits:
        print(f'{path}:{line}: {rule} {code}', file=sys.stderr)
    print(f'smell: {len(hits)} hits')
    return int(bool(hits))
