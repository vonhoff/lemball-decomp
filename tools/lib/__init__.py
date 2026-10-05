"""Shared repository paths, source scanning and upstream reccmp setup."""

import functools
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


def mask_comments_and_strings(text: str) -> str:
    return TOKENS.sub(lambda m: re.sub(r"[^\n]", " ", m[0]), text)


@functools.lru_cache(maxsize=4)
def _find_sources(paths_tuple: tuple[str, ...] | None):
    return list(source_code_search([ROOT / path for path in paths_tuple or (SRC,)]))


def collect_sources(paths=None):
    return _find_sources(tuple(paths) if paths else None)


def load_engine():
    from reccmp.compare import Compare
    from reccmp.project.detect import RecCmpProject
    from .vtable_lookup import install_vtable_lookups

    def mute_folded_diagnostic(record):
        return not (
            record.levelno == logging.WARNING
            and record.msg == "Match (%x, %x) collides with previous staged match"
            and isinstance(record.args, tuple)
            and len(record.args) == 2
            and record.args[0] == 0x0040A830
        )

    target = RecCmpProject.from_directory(BUILD).get("LEMBALL")
    logger = logging.getLogger("reccmp.compare.db")
    logger.addFilter(mute_folded_diagnostic)
    try:
        engine = Compare.from_target(target)
        install_vtable_lookups(engine.function_comparator)
        return target, engine
    finally:
        logger.removeFilter(mute_folded_diagnostic)
