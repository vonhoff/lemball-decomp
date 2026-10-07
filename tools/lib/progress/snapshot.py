"""Read Effective results bound to a canonical report hash."""

import hashlib
import json
from pathlib import Path
from typing import Any

EFFECTIVE_POLICY = "reccmp-with-assembly-normalization-v2"


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
                or snapshot.get("policy") != EFFECTIVE_POLICY
                or snapshot.get("report_sha256") != hashlib.sha256(raw).hexdigest()
            ):
                raise ValueError("effective results do not belong to this report")
            accepted = {int(address) for address in snapshot["addresses"]}
        return report, accepted
    except (OSError, ValueError, KeyError) as exc:
        raise ValueError(
            f"Cannot read saved progress: {exc}. Run python tools/make_report.py first."
        ) from exc
