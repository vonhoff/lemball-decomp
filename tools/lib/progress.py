"""Saved progress accounting; canonical reports contain raw scores only."""

import hashlib
import json
from pathlib import Path
from typing import Any


def effective_measures(report: dict[str, Any], accepted: set[int]) -> dict[str, Any]:
    matched_code = 0
    matched_count = 0
    for unit in report["units"]:
        for f in unit["functions"]:
            if int(f["metadata"]["virtual_address"]) in accepted:
                matched_code += int(f["size"])
                matched_count += 1
    total = int(report["measures"]["total_code"])
    return {
        "matched_code": matched_code,
        "matched_functions": matched_count,
        "matched_code_percent": (matched_code / total * 100) if total else 0.0,
    }


def effective_snapshot(
    report_bytes: bytes, accepted: set[int], additional: dict[int, Any]
) -> dict[str, Any]:
    """Bind accepted addresses to the exact canonical report that produced them."""
    return {
        "version": 1,
        "report_sha256": hashlib.sha256(report_bytes).hexdigest(),
        "addresses": sorted(accepted),
        "additional": additional,
    }


def load_progress(
    report_path: Path | str, effective_path: Path | str | None = None
) -> tuple[dict[str, Any], set[int]]:
    """Read a saved batch; reject missing, old-format or mismatched sidecars."""
    try:
        raw = Path(report_path).read_bytes()
        report = json.loads(raw)
        accepted = set()
        if effective_path is not None:
            snapshot = json.loads(Path(effective_path).read_bytes())
            if (
                snapshot.get("version") != 1
                or snapshot.get("report_sha256") != hashlib.sha256(raw).hexdigest()
            ):
                raise ValueError("effective results do not belong to this report")
            accepted = {int(address) for address in snapshot["addresses"]}
        return report, accepted
    except (OSError, ValueError, KeyError) as exc:
        raise ValueError(
            f"Cannot read saved progress: {exc}. Run python tools/make_report.py first."
        ) from exc
