"""Shared repository paths, source scanning and upstream reccmp setup."""

import logging
import re
from pathlib import Path

from reccmp.dir import source_code_search

ROOT = Path(__file__).resolve().parents[2]
BUILD = ROOT / "build-msvc400"
SRC = ROOT / "src"
RECCMP_JSON = BUILD / "reccmp.json"
REPORT_JSON = BUILD / "report.json"
EFFECTIVE_JSON = BUILD / "effective.json"


TOKENS = re.compile(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'')

TYPE_DEF = re.compile(
    r"\b(?P<kind>class|struct)\s+(?P<name>\w+)\s*(?:final\s*)?(?::[^;{}]*)?\{"
)


def mask_comments_and_strings(text: str) -> str:
    return TOKENS.sub(lambda m: re.sub(r"[^\n]", " ", m[0]), text)


def delimiter_ends(code: str, opening: str, closing: str) -> dict[int, int]:
    """Map each opening delimiter to its matching closing delimiter."""
    ends, stack = {}, []
    for pos, char in enumerate(code):
        if char == opening:
            stack.append(pos)
        elif char == closing and stack:
            ends[stack.pop()] = pos
    return ends


def collect_sources(paths=None):
    return list(source_code_search([ROOT / path for path in paths or [SRC]]))


def _show_match_diagnostic(record):
    """Mute the known Position/ActivatePosition FOLDED-address diagnostic only."""
    return not (
        record.levelno == logging.WARNING
        and record.msg == "Match (%x, %x) collides with previous staged match"
        and isinstance(record.args, tuple)
        and len(record.args) == 2
        and record.args[0] == 0x0040A830
    )


def load_engine():
    from reccmp.compare import Compare
    from reccmp.project.detect import RecCmpProject

    target = RecCmpProject.from_directory(BUILD).get("LEMBALL")
    logger = logging.getLogger("reccmp.compare.db")
    logger.addFilter(_show_match_diagnostic)
    try:
        return target, Compare.from_target(target)
    finally:
        logger.removeFilter(_show_match_diagnostic)
