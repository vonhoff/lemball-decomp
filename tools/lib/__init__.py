"""Shared reconstruction tools and repository paths."""

from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
BUILD = ROOT / "build-msvc400"
REPORT_JSON = BUILD / "report.json"
EFFECTIVE_JSON = BUILD / "effective.json"
TARGET_ID = "LEMBALL"
