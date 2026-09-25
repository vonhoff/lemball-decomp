"""Allow only functional source comments."""

from __future__ import annotations

import re
from pathlib import Path

from .paths import ROOT

ANNOTATION = re.compile(
    r"// (FUNCTION|STUB|TEMPLATE|SYNTHETIC|LIBRARY|VTABLE|GLOBAL|STRING|LINE): "
    r"LEMBALL 0x[0-9a-fA-F]+(?: (FOLDED|SYMBOL|[A-Za-z_]\w*(?:'s `[A-Za-z_]\w*)?))?"
)
TOKEN = re.compile(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'')
CLANG_FORMAT = re.compile(r"// clang-format (?:off|on)")
LAYOUT = re.compile(r"// (?:(?:MINIMUM )?SIZE 0x[0-9a-fA-F]+|(?:vtable\+)?0x[0-9a-fA-F]+)")
SUFFIXES = {".c", ".cpp", ".h", ".inl", ".rc"}


def code_files(paths=None):
    roots = [Path(path) for path in paths] if paths else [ROOT / "src"]
    files = set()
    for path in roots:
        path = path if path.is_absolute() else ROOT / path
        if path.is_file() and path.suffix in SUFFIXES:
            files.add(path)
        elif path.is_dir():
            files.update(file for file in path.rglob("*") if file.suffix in SUFFIXES)
    return sorted(files)


def invalid_comments(path, text=None):
    if text is None:
        text = path.read_text(encoding="utf-8")
    previous = None
    for token in TOKEN.finditer(text):
        value = token[0]
        if not value.startswith(("//", "/*")):
            continue
        line = text.count("\n", 0, token.start()) + 1
        prefix = text[text.rfind("\n", 0, token.start()) + 1:token.start()]
        annotation = ANNOTATION.fullmatch(value) if not prefix.strip() else None
        if annotation and annotation[2] not in (None, "FOLDED", "SYMBOL") and annotation[1] != "VTABLE":
            annotation = None
        name = (value.startswith("// ") and previous and previous[0] == line - 1
                and previous[1] and not prefix.strip()
                and re.fullmatch(r"\S+|\S.*::.*", value[3:]))
        layout = LAYOUT.fullmatch(value)
        clang_format = CLANG_FORMAT.fullmatch(value) if not prefix.strip() else None
        if not annotation and not name and not layout and not clang_format:
            yield line, value
        previous = (line, annotation[1] if annotation else None)


def check_comments(paths=None):
    failures = 0
    for path in code_files(paths):
        for line, comment in invalid_comments(path):
            print(f"{path}:{line}: forbidden comment: {comment.splitlines()[0]}")
            failures += 1
    print(f"comments: {failures} forbidden")
    return bool(failures)
