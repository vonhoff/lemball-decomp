"""Effective metrics and saved results bound to a canonical report hash."""

import hashlib
import json
from typing import Any

from . import EFFECTIVE_JSON, REPORT_JSON


def effective_code_percent(report: dict[str, Any], accepted: set[int]) -> float:
    """Weight accepted functions by their canonical original sizes."""
    matched_code = sum(
        int(function["size"])
        for unit in report["units"]
        for function in unit["functions"]
        if int(function["metadata"]["virtual_address"]) in accepted
    )
    total = int(report["measures"]["total_code"])
    return matched_code / total * 100 if total else 0.0


def load_progress(*, exact: bool = False) -> tuple[dict[str, Any], set[int]]:
    """Read a saved batch; reject missing or mismatched sidecars."""
    try:
        raw = REPORT_JSON.read_bytes()
        report = json.loads(raw)
        accepted = set()
        if not exact:
            snapshot = json.loads(EFFECTIVE_JSON.read_bytes())
            if snapshot["report_sha256"] != hashlib.sha256(raw).hexdigest():
                raise ValueError("effective results do not belong to this report")
            accepted = {int(address) for address in snapshot["addresses"]}
        return report, accepted
    except (OSError, ValueError, KeyError) as exc:
        raise ValueError(
            f"Cannot read saved progress: {exc}. Run python tools/make_report.py first."
        ) from exc
