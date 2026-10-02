#!/usr/bin/env python3
"""Fail on decomp smells (via tools/gate.py)."""

import re
import sys
from pathlib import Path

from . import RECCMP_MARK, ROOT, TOKENS, collect_sources, mask_comments_and_strings, parenthesis_end

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
    if stripped.split(None, 1)[0] in SKIP_LEAD:
        return False
    if METHOD_DEF.match(stripped):
        return True
    if "::" in stripped.split("(", 1)[0]:
        return False
    return bool(FREE_DEF.match(stripped))


def declaration_has_body(code: str, offset: int = 0) -> bool:
    """A wrapped local constructor call or prototype ends in ';', not a body."""
    end = parenthesis_end(code, code.find("(", offset))
    if end is None:
        return False
    return bool(re.match(r"\s*(?:(?:const|volatile)\s*)*(?:\{|:(?!:))", code[end + 1:]))


def iter_raw_deref_casts(code: str):
    """Yield (type_text, rhs) for *(T*)rhs and *((T*) rhs)."""
    for match in RAW_DEREF_CAST.finditer(code):
        yield match.group("type"), match.group("rhs").strip()

    for start in PAREN_RAW_DEREF_START.finditer(code):
        cast = CAST_TYPE_AT.match(code, start.end())
        if cast is None:
            continue
        end = parenthesis_end(code, start.end() - 1)
        if end is None:
            continue
        rhs = code[cast.end():end].strip()
        if rhs:
            yield cast.group("type"), rhs


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


def line_smells(code: str):
    """Yield specific cast/offset rules before the generic offset fallback."""
    raw_cast = raw_cast_reason(code)
    if raw_cast is not None:
        yield raw_cast
    for pattern, rule in ((VBPTR_WALK, "vbptr-walk"), (THIS_ADJUST, "this-adjust-poke")):
        if pattern.search(code):
            yield rule

    specific_offsets = (
        (CAST_BYTE_DIV_INDEX.search(code), "type-erase-index"),
        (
            any(not BUFFER_OK.search(match["expr"]) for match in EXPR_CHAR_OFFSET.finditer(code)),
            "expr-char-offset",
        ),
        (MI_DTOR_POKE.search(code), "mi-dtor-poke"),
    )
    for matched, rule in specific_offsets:
        if matched:
            yield rule
    if (not any(matched for matched, _ in specific_offsets) and not BUFFER_OK.search(code)
            and (CAST_THEN_ARITH.search(code) or CAST_PAREN_ARITH.search(code)
                 or NAKED_DATA_OFFSET.search(code))):
        yield "offset-poke"
    for match in CHAR_VAR_OFFSET.finditer(code):
        if not BUFFER_OK.search(match["expr"]) and match["off"] != "sizeof":
            yield "offset-poke"


def unannotated_definitions(lines: list[str], masked: str):
    """Find function bodies without a reccmp marker in their preceding comment block."""
    offset, annotated, separated = 0, False, False
    for line, (raw, code) in enumerate(zip(lines, masked.splitlines()), 1):
        start, offset = offset, offset + len(code) + 1
        stripped = raw.strip()
        if stripped.startswith("//"):
            annotated = bool(RECCMP_MARK.match(stripped)) or (annotated and not separated)
            separated = False
        elif not stripped:
            separated = True
        else:
            if not annotated and is_func_def(code.strip()) and declaration_has_body(masked, start):
                yield line, stripped
            annotated = separated = False


def scan_file(path: Path) -> list[Hit]:
    """Scan masked code; retain source text and line numbers for diagnostics."""
    lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
    masked = mask_comments_and_strings("\n".join(lines))
    rel = path.resolve().relative_to(ROOT).as_posix()
    hits: list[Hit] = []
    for lineno, (raw, code) in enumerate(zip(lines, masked.splitlines()), 1):
        comment = next((token for token in TOKENS.finditer(raw) if token[0].startswith('//')), None)
        display = (raw[:comment.start()] if comment else raw).strip()
        hits.extend((rel, lineno, rule, display) for rule in line_smells(code))
    if path.suffix.lower() == ".cpp":
        hits.extend((rel, line, 'no-annotation', code)
                    for line, code in unannotated_definitions(lines, masked))
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
