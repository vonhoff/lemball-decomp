"""Shared repository paths, source scanning and upstream reccmp setup."""

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
BUILD = ROOT / "build-msvc400"
SRC = ROOT / "src"
RECCMP_JSON = BUILD / "reccmp.json"
REPORT_JSON = BUILD / "report.json"
ROADMAP_CSV = BUILD / "roadmap.csv"


TOKENS = re.compile(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'')

CPP_SUFFIXES = frozenset({".cpp", ".h", ".c"})
RECCMP_MARK = re.compile(r"^\s*//\s*(?:FUNCTION|STUB|TEMPLATE|SYNTHETIC|LIBRARY|GLOBAL|VTABLE)\s*:", re.MULTILINE)
VTABLE_MARK = re.compile(r"^\s*//\s*VTABLE:\s+LEMBALL\b", re.MULTILINE)
TYPE_DEF = re.compile(
    r"\b(?P<kind>class|struct)\s+(?P<name>\w+)\s*(?:final\s*)?(?::[^;{}]*)?\{"
)


def mask_comments_and_strings(text: str) -> str:
    return TOKENS.sub(lambda m: re.sub(r"[^\n]", " ", m[0]), text)


def brace_ends(code: str) -> dict[int, int]:
    """Map opening brace index to closing brace index."""
    ends, stack = {}, []
    for pos, char in enumerate(code):
        if char == "{":
            stack.append(pos)
        elif char == "}" and stack:
            ends[stack.pop()] = pos
    return ends


def parenthesis_end(code: str, opening: int) -> int | None:
    """Find a closing parenthesis in masked code; return None if unclosed."""
    if opening < 0 or opening >= len(code) or code[opening] != "(":
        return None
    depth = 0
    for pos in range(opening, len(code)):
        depth += (code[pos] == "(") - (code[pos] == ")")
        if depth == 0:
            return pos
    return None


def collect_sources(paths=None, suffixes=CPP_SUFFIXES):
    """Collect unique C/C++ source and header files from given paths or src."""
    search_paths = [Path(p) for p in paths] if paths else [SRC]
    files: set[Path] = set()
    for path in search_paths:
        target = path if path.is_absolute() else ROOT / path
        if not target.exists():
            continue
        if target.is_dir():
            files.update(p for p in target.rglob("*") if p.suffix.lower() in suffixes)
        elif target.suffix.lower() in suffixes:
            files.add(target)
    return sorted(files)


def load_engine():
    from reccmp.compare import Compare
    from reccmp.project.detect import RecCmpProject

    target = RecCmpProject.from_directory(BUILD).get("LEMBALL")
    return target, Compare.from_target(target)
