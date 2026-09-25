"""Read reccmp results without changing their match scores."""

from __future__ import annotations

import json
from pathlib import Path


def compute_ratio(match: dict | None) -> float:
    if match is None or match.get("stub"):
        return 0.0
    ratio = float(match.get("matching", 0.0)) * 100.0
    return 100.0 if match.get("effective") else ratio


def load_matches(json_path: Path) -> dict[int, dict]:
    with json_path.open(encoding="utf-8") as stream:
        data = json.load(stream)["data"]
    return {
        int(match["address"], 16): match
        for match in data if match.get("type") in (None, 1)
    }
