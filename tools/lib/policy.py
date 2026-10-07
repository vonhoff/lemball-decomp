"""Check C++ source operations and functional comment syntax."""

import re
from collections.abc import Iterator

from reccmp.parser.marker import MarkerType, is_marker_exact, match_marker

from .scan import TOKENS, mask_comments_and_strings

BYTE = r"(?:(?:const|unsigned|signed)\s+)*(?:char|BYTE)\s*\*"
FUNCTION_POINTER = r"[\w:\s]+\(\s*(?:__\w+\s*)?\*\s*\)\s*\([^;{}]*?\)"
POINTER = rf"(?:[\w:\s]+\*+\s*|{FUNCTION_POINTER})"
LITERAL = r"(?:0[xX][0-9a-fA-F]+|[0-9]+)(?:[uUlL]+)?\b"
RULES = (
    (
        r"\b(?:__asm__|__asm|_asm|asm|_emit|__emit)\b",
        "assembly: express the operation in C++",
    ),
    (
        rf"(?:\(\s*(?:{BYTE}|(?:unsigned\s+)?(?:int|long))\s*\)\s*this"
        rf"|reinterpret_cast\s*<\s*{BYTE}\s*>\s*\(\s*this\s*\))\s*[+-]\s*{LITERAL}",
        "this adjustment: use an evidenced base conversion or typed member",
    ),
    (
        rf"\*\s*(?:\(\s*)?\(\s*{POINTER}\)\s*\(\s*\(\s*{BYTE}\s*\)"
        rf"\s*[\w.>\-]+\s*[+-]\s*{LITERAL}"
        rf"|\*\s*reinterpret_cast\s*<[^;{{}}>]*\*>\s*\(\s*reinterpret_cast\s*<\s*{BYTE}\s*>"
        rf"\s*\(\s*[\w.>\-]+\s*\)\s*[+-]\s*{LITERAL}",
        "raw object access: use a typed member or serialized field",
    ),
    (
        r"\b(?:__vfptr|__vbptr|__vftable|__vbtable)\b"
        r"|\*\s*\(\s*(?:unsigned\s+)?(?:void|char|short|int|long)\s*\*+\s*\)\s*\(*\s*this\b(?!\s*\)*\s*->)",
        "raw dispatch: use a declared method or evidenced base conversion",
    ),
    (
        rf"\*\s*\(*\s*\(\s*{POINTER}\)\s*\(*\s*{LITERAL}"
        rf"|\*\s*reinterpret_cast\s*<[^;{{}}>]*\*>\s*\(\s*{LITERAL}"
        rf"|\(\s*{FUNCTION_POINTER}\)\s*\(*\s*{LITERAL}"
        rf"|reinterpret_cast\s*<\s*{FUNCTION_POINTER}\s*>\s*\(\s*{LITERAL}",
        "literal pointer: declare the referenced data or function",
    ),
)
FUNCTIONAL = re.compile(
    r"// (?:clang-format (?:off|on)|(?:MINIMUM )?SIZE 0x[0-9a-fA-F]+|(?:vtable\+)?0x[0-9a-fA-F]+)"
)


def violations(text: str) -> Iterator[tuple[int, str]]:
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
        standalone = not text[
            text.rfind("\n", 0, token.start()) + 1 : token.start()
        ].strip()
        marker = match_marker(value) if standalone and is_marker_exact(value) else None
        annotation = (
            marker
            if marker
            and marker.type != MarkerType.UNKNOWN
            and (
                marker.extra in (None, "FOLDED", "SYMBOL")
                or marker.type == MarkerType.VTABLE
                and re.fullmatch(
                    r"[A-Za-z_]\w*(?:'s `[A-Za-z_]\w*)?", marker.extra or ""
                )
            )
            else None
        )
        symbol = (
            standalone
            and by_name
            and line == previous_line + 1
            and re.fullmatch(r"// (?:\S+|\S+::.+|\"(?:\\.|[^\"\\])*\")", value)
        )
        functional = FUNCTIONAL.fullmatch(value) and (
            standalone or "clang-format" not in value
        )
        if not (annotation or symbol or functional):
            yield (
                line,
                "comment: use a reccmp annotation, symbol, layout note, or format control",
            )
        previous_line = line
        by_name = annotation and annotation.type not in (
            MarkerType.VTABLE,
            MarkerType.LINE,
        )
