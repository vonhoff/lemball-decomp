"""Shared reconstruction tools and repository paths."""

from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
BUILD = ROOT / "build-msvc400"
REPORT_JSON = BUILD / "report.json"
RECCMP_JSON = BUILD / "reccmp.json"
TARGET_ID = "LEMBALL"


def thunk_symbol(address):
    return f"__lemball_jump_{address:08x}"
