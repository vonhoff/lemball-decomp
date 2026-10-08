"""Original-byte progress from objdiff and native reccmp reports."""

import hashlib
import json
from typing import Any

from reccmp.compare.report import (
    deserialize_reccmp_report,
    ReccmpReportDeserializeError,
    serialize_reccmp_report,
)

from . import RECCMP_JSON, REPORT_JSON


def accepted_functions(comparisons) -> set[int]:
    """Accept complete non-stub raw or effective function matches."""
    return {
        address
        for address, comparison in comparisons.entities.items()
        if comparison.is_function()
        and comparison.is_matched()
        and not comparison.is_stub
        and comparison.effective_accuracy == 1
    }


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


def save_progress(report, comparisons):
    """Save objdiff totals and compact native results from the same batch."""
    raw = (json.dumps(report, indent=2) + "\n").encode("utf-8")
    native = json.loads(serialize_reccmp_report(comparisons))
    native["report_sha256"] = hashlib.sha256(raw).hexdigest()
    RECCMP_JSON.write_text(
        json.dumps(native, separators=(",", ":")) + "\n", encoding="utf-8"
    )
    REPORT_JSON.write_bytes(raw)


def load_progress(*, exact: bool = False) -> tuple[dict[str, Any], set[int]]:
    """Read a saved batch; reject missing or mismatched native results."""
    try:
        raw = REPORT_JSON.read_bytes()
        report = json.loads(raw)
        accepted = set()
        if not exact:
            native = RECCMP_JSON.read_text(encoding="utf-8")
            snapshot = json.loads(native)
            if snapshot["report_sha256"] != hashlib.sha256(raw).hexdigest():
                raise ValueError("reccmp results do not belong to this report")
            accepted = accepted_functions(deserialize_reccmp_report(native))
        return report, accepted
    except (OSError, ValueError, KeyError, ReccmpReportDeserializeError) as exc:
        raise ValueError(
            f"Cannot read saved progress: {exc}. Run python tools/make_report.py first."
        ) from exc
