"""Shared repository paths, source scanning and upstream reccmp setup."""

import re
from pathlib import Path

from reccmp.dir import source_code_search

ROOT = Path(__file__).resolve().parents[2]
BUILD = ROOT / "build-msvc400"
SRC = ROOT / "src"
RECCMP_JSON = BUILD / "reccmp.json"
REPORT_JSON = BUILD / "report.json"


TOKENS = re.compile(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'')

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
    """Find the closing parenthesis for a known '('; return None if unclosed."""
    depth = 0
    for pos in range(opening, len(code)):
        depth += (code[pos] == "(") - (code[pos] == ")")
        if depth == 0:
            return pos
    return None


def collect_sources(paths=None):
    return list(source_code_search([ROOT / path for path in paths or [SRC]]))


def load_engine():
    from reccmp.compare import Compare
    from reccmp.project.detect import RecCmpProject
    target = RecCmpProject.from_directory(BUILD).get("LEMBALL")
    engine = Compare.from_target(target)
    return target, engine
