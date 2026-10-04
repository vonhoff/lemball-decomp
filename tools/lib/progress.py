"""Saved progress accounting; canonical reports contain raw scores only."""

import hashlib
import json
from pathlib import Path
from typing import Any


def functions_by_address(report: dict[str, Any]) -> dict[int, dict[str, Any]]:
    return {
        int(function["metadata"]["virtual_address"]): function
        for unit in report["units"]
        for function in unit["functions"]
    }


def effective_measures(report, accepted):
    functions = functions_by_address(report)
    sizes = [int(f["size"]) for address, f in functions.items() if address in accepted]
    total = int(report["measures"]["total_code"])
    return {
        "matched_code": sum(sizes),
        "matched_functions": len(sizes),
        "matched_code_percent": sum(sizes) / total * 100 if total else 0.0,
    }


def effective_snapshot(report_bytes, accepted, additional):
    """Bind accepted addresses to the exact canonical report that produced them."""
    return {
        "version": 1,
        "report_sha256": hashlib.sha256(report_bytes).hexdigest(),
        "addresses": sorted(accepted),
        "additional": additional,
    }


def load_progress(report_path, effective_path=None) -> tuple[dict[str, Any], set[int]]:
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
    except (OSError, ValueError, KeyError, TypeError, AttributeError) as exc:
        raise ValueError(
            f"Cannot read saved progress: {exc}. Run python tools/report.py first."
        ) from exc


def exact_regressions(
    before: dict[str, Any], after: dict[str, Any]
) -> dict[int, tuple[dict[str, Any], dict[str, Any] | None]]:
    """Include lost inventory entries; equal total counts can hide regressions."""
    current = functions_by_address(after)
    return {
        address: (function, current.get(address))
        for address, function in functions_by_address(before).items()
        if function["fuzzy_match_percent"] == 100
        and (address not in current or current[address]["fuzzy_match_percent"] != 100)
    }
