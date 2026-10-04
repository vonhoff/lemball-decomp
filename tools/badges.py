#!/usr/bin/env python3
"""Export README badges without changing the canonical progress report."""

import json

from reccmp.compare.report import deserialize_reccmp_report

from lib import BUILD, RECCMP_JSON, REPORT_JSON
from lib.effective import EFFECTIVE_JSON, effective_addresses

BADGES_DIR = BUILD / "badges"


def build_badges(report, comparisons, additional=()):
    """Weight accepted functions by the canonical original sizes."""
    accepted = effective_addresses(comparisons.entities, additional)
    effective_code = sum(
        int(function["size"])
        for unit in report["units"]
        for function in unit["functions"]
        if int(function["metadata"]["virtual_address"]) in accepted
    )
    values = report["measures"]
    total_code = int(values["total_code"])
    effective_percent = effective_code / total_code * 100
    return {
        name: {
            "schemaVersion": 1,
            "label": label,
            "message": f"{percent:.2f}%",
            "color": color,
        }
        for name, label, percent, color in (
            ("exact", "Exact", values["matched_code_percent"], "informational"),
            ("fuzzy", "Fuzzy", values["fuzzy_match_percent"], "inactive"),
            ("effective", "Effective", effective_percent, "success"),
        )
    }


def main():
    report = json.loads(REPORT_JSON.read_text(encoding="utf-8"))
    reccmp_text = RECCMP_JSON.read_text(encoding="utf-8")
    comparisons = deserialize_reccmp_report(reccmp_text)
    additional = {
        int(address)
        for address in json.loads(EFFECTIVE_JSON.read_text(encoding="utf-8"))
    }
    badges = build_badges(report, comparisons, additional)
    BADGES_DIR.mkdir(parents=True, exist_ok=True)
    for name, badge in badges.items():
        (BADGES_DIR / f"{name}.json").write_text(
            json.dumps(badge) + "\n", encoding="utf-8"
        )
        print(f"{badge['label']}: {badge['message']}")


if __name__ == "__main__":
    main()
