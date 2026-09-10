"""Source scan helpers and reccmp annotation grammar."""

from __future__ import annotations

import re
from pathlib import Path

from .paths import ROOT

CPP_SUFFIXES = {".cpp", ".h", ".c"}
_TYPES = "FUNCTION|STUB|TEMPLATE|SYNTHETIC|LIBRARY|GLOBAL|VTABLE"

RECCMP_MARK = re.compile(rf"^\s*//\s*(?:{_TYPES})\s*:", re.M)
ANNOT_WITH_ADDR = re.compile(rf"//\s*({_TYPES}):\s*LEMBALL\s+(0x[0-9A-Fa-f]+)")
VTABLE_MARK = re.compile(r"^\s*//\s*VTABLE:\s+LEMBALL\b", re.M)
SYNTHETIC_MARK = re.compile(r"^\s*//\s*SYNTHETIC\s*:")


def mask_comments_and_strings(text: str) -> str:
    pattern = r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\''
    return re.sub(pattern, lambda m: re.sub(r"[^\n]", " ", m[0]), text)


def brace_ends(code: str) -> dict[int, int]:
    """Map opening brace index to closing brace index."""
    ends, stack = {}, []
    for pos, char in enumerate(code):
        if char == "{":
            stack.append(pos)
        elif char == "}" and stack:
            ends[stack.pop()] = pos
    return ends


def collect_sources(paths: list[Path | str] | None = None, root: Path = ROOT) -> list[Path]:
    """Collect unique C/C++ source and header files from given paths or root/src."""
    search_paths = [Path(p) for p in paths] if paths else [root / "src"]
    files: set[Path] = set()
    for path in search_paths:
        target = path if path.is_absolute() else root / path
        if not target.exists():
            continue
        if target.is_dir():
            files.update(p for p in target.rglob("*") if p.suffix.lower() in CPP_SUFFIXES)
        elif target.suffix.lower() in CPP_SUFFIXES:
            files.add(target)
    return sorted(files)


def rel_posix(path: Path, root: Path = ROOT) -> str:
    return path.resolve().relative_to(root).as_posix()
