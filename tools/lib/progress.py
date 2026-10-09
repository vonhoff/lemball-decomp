"""Saved objdiff progress and additional Effective function addresses."""

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
        if function["fuzzy_match_percent"] == 100
        or int(function["metadata"]["virtual_address"]) in accepted
    )
    total = int(report["measures"]["total_code"])
    return matched_code / total * 100 if total else 0.0


def save_progress(report, accepted):
    """Save raw scores and their Effective addresses from the same batch."""
    raw = (json.dumps(report, indent=2) + "\n").encode("utf-8")
    EFFECTIVE_JSON.write_text(
        json.dumps(
            {
                "report_sha256": hashlib.sha256(raw).hexdigest(),
                "addresses": sorted(accepted),
            }
        )
        + "\n",
        encoding="utf-8",
    )
    REPORT_JSON.write_bytes(raw)


def load_progress(*, exact: bool = False) -> tuple[dict[str, Any], set[int]]:
    """Read a saved batch; reject missing or mismatched Effective addresses."""
    try:
        raw = REPORT_JSON.read_bytes()
        report = json.loads(raw)
        accepted = set()
        if not exact:
            snapshot = json.loads(EFFECTIVE_JSON.read_text(encoding="utf-8"))
            if snapshot["report_sha256"] != hashlib.sha256(raw).hexdigest():
                raise ValueError("Effective addresses do not belong to this report")
            addresses = snapshot["addresses"]
            if not isinstance(addresses, list) or any(
                type(address) is not int for address in addresses
            ):
                raise ValueError("Invalid Effective addresses")
            accepted = set(addresses)
        return report, accepted
    except (OSError, ValueError, KeyError, TypeError) as exc:
        raise ValueError(
            f"Cannot read saved progress: {exc}. Run python tools/make_report.py first."
        ) from exc
