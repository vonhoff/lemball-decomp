"""Read reccmp results without changing their match scores."""

from __future__ import annotations

import json
from pathlib import Path


def compute_ratio(match: dict | None) -> tuple[float, str]:
    if match is None or match.get("stub"):
        return 0.0, "STUB"
    ratio = float(match.get("matching", 0.0)) * 100.0
    if ratio == 100.0:
        return 100.0, "ASM_EXACT"
    if match.get("effective"):
        return 100.0, "EFFECTIVE"
    return ratio, "PARTIAL"


def load_matches(json_path: Path) -> dict[int, dict]:
    with json_path.open(encoding="utf-8") as stream:
        data = json.load(stream)["data"]
    return {
        int(match["address"], 16): match
        for match in data if match.get("type") in (None, 1)
    }
