"""Shared repository paths and tool resolution."""

from __future__ import annotations

from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
BUILD = ROOT / "build-msvc400"
SRC = ROOT / "src"
DATA = ROOT / "data"
ORIGINAL_EXE = DATA / "LEMBALL.EXE"
RECOMP_EXE = BUILD / "LEMBALL.EXE"
RECOMP_PDB = BUILD / "LEMBALL.pdb"
RECCMP_JSON = BUILD / "reccmp.json"
RECCMP_STAMP = BUILD / "reccmp.stamp.json"
REPORT_JSON = BUILD / "report.json"
ROADMAP_CSV = BUILD / "roadmap.csv"
TARGETS_CACHE = BUILD / "targets-cache.json"


def file_id(path: Path) -> dict | None:
    """mtime+size identity for cache stamps (ponytail: skip full-file hashes)."""
    if not path.exists():
        return None
    st = path.stat()
    return {"path": path.name, "mtime_ns": st.st_mtime_ns, "size": st.st_size}
