"""Calculate progress from accepted functions and original byte sizes."""

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
