"""Narrow source-policy tripwires and functional comment syntax."""

import re

from reccmp.parser.marker import MarkerType, is_marker_exact, match_marker

from . import ROOT, SRC, TOKENS, collect_sources, mask_comments_and_strings

BYTE = r"(?:(?:const|unsigned|signed)\s+)*(?:char|BYTE)\s*\*"
FUNCTION_POINTER = r"[\w:\s]+\(\s*(?:__\w+\s*)?\*\s*\)\s*\([^;{}]*?\)"
POINTER = rf"(?:[\w:\s]+\*+\s*|{FUNCTION_POINTER})"
LITERAL = r"(?:0[xX][0-9a-fA-F]+|[0-9]+)(?:[uUlL]+)?\b"
RULES = (
    (r"\b(?:__asm__|__asm|_asm|asm|_emit|__emit)\b",
     "assembly: express the operation in C++"),
    (rf"(?:\(\s*(?:{BYTE}|(?:unsigned\s+)?(?:int|long))\s*\)\s*this"
     rf"|reinterpret_cast\s*<\s*{BYTE}\s*>\s*\(\s*this\s*\))\s*[+-]\s*{LITERAL}",
     "this adjustment: use an evidenced base conversion or typed member"),
    (rf"\*\s*(?:\(\s*)?\(\s*{POINTER}\)\s*\(\s*\(\s*{BYTE}\s*\)"
     rf"\s*[\w.>\-]+\s*[+-]\s*{LITERAL}"
     rf"|\*\s*reinterpret_cast\s*<[^;{{}}>]*\*>\s*\(\s*reinterpret_cast\s*<\s*{BYTE}\s*>"
     rf"\s*\(\s*[\w.>\-]+\s*\)\s*[+-]\s*{LITERAL}",
     "raw object access: use a typed member or serialized field"),
    (r"\b(?:__vfptr|__vbptr|__vftable|__vbtable)\b"
     r"|\*\s*\(\s*(?:unsigned\s+)?(?:void|char|short|int|long)\s*\*+\s*\)\s*\(*\s*this\b(?!\s*\)*\s*->)",
     "raw dispatch: use a declared method or evidenced base conversion"),
    (rf"\*\s*\(*\s*\(\s*{POINTER}\)\s*\(*\s*{LITERAL}"
     rf"|\*\s*reinterpret_cast\s*<[^;{{}}>]*\*>\s*\(\s*{LITERAL}"
     rf"|\(\s*{FUNCTION_POINTER}\)\s*\(*\s*{LITERAL}"
     rf"|reinterpret_cast\s*<\s*{FUNCTION_POINTER}\s*>\s*\(\s*{LITERAL}",
     "literal pointer: declare the referenced data or function"),
)
FUNCTIONAL = re.compile(r"// (?:clang-format (?:off|on)|(?:MINIMUM )?SIZE 0x[0-9a-fA-F]+|(?:vtable\+)?0x[0-9a-fA-F]+)")


def violations(text):
    code = mask_comments_and_strings(text)
    for pattern, message in RULES:
        for match in re.finditer(pattern, code):
            yield text.count("\n", 0, match.start()) + 1, message
    previous_line, by_name = 0, False
    for token in TOKENS.finditer(text):
        value = token[0]
        if not value.startswith(("//", "/*")):
            continue
        line = text.count("\n", 0, token.start()) + 1
        standalone = not text[text.rfind("\n", 0, token.start()) + 1:token.start()].strip()
        marker = match_marker(value) if standalone and is_marker_exact(value) else None
        annotation = marker and marker.type != MarkerType.UNKNOWN and (
            marker.extra in (None, "FOLDED", "SYMBOL") or marker.type == MarkerType.VTABLE
            and re.fullmatch(r"[A-Za-z_]\w*(?:'s `[A-Za-z_]\w*)?", marker.extra or ""))
        symbol = (standalone and by_name and line == previous_line + 1
                  and re.fullmatch(r"// (?:\S+|\S+::.+|\"(?:\\.|[^\"\\])*\")", value))
        functional = FUNCTIONAL.fullmatch(value) and (standalone or "clang-format" not in value)
        if not (annotation or symbol or functional):
            yield line, "comment: use a reccmp annotation, symbol, layout note, or format control"
        previous_line = line
        by_name = annotation and marker.type not in (MarkerType.VTABLE, MarkerType.LINE)


def check_policy(paths=None):
    files = set(collect_sources(paths))
    for path in paths or [SRC]:
        path = ROOT / path
        files.update(p for p in (path.rglob("*") if path.is_dir() else [path])
                     if p.is_file() and p.suffix.lower() in (".inl", ".rc"))
    if not files:
        print("policy: no source files found")
        return 2
    failures = 0
    for path in sorted(files):
        text = path.read_text(encoding="utf-8")
        lines = text.splitlines()
        for line, message in sorted(violations(text)):
            print(f"{path}:{line}: {message}\n  {lines[line - 1].strip()}")
            failures += 1
    print(f"policy: {failures} violations")
    return int(bool(failures))
