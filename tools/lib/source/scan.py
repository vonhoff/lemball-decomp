"""Discover source files and mask C++ comments and literals."""

import re

from reccmp.dir import source_code_search

from ..project import ROOT, SRC

TOKENS = re.compile(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'')


def mask_comments_and_strings(text: str) -> str:
    return TOKENS.sub(lambda m: re.sub(r"[^\n]", " ", m[0]), text)


def collect_sources(paths=None):
    """Find C/C++ sources under supplied paths or the configured source root."""
    return list(source_code_search([ROOT / path for path in paths or (SRC,)]))
