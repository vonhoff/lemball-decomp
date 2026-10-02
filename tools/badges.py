#!/usr/bin/env python3
"""Export README badges without changing the canonical progress report."""

import json

from reccmp.compare.report import deserialize_reccmp_report

from lib import BUILD, RECCMP_JSON, REPORT_JSON

BADGES_DIR = BUILD / "badges"


def build_badges(report, comparisons):
    """Use the canonical inventory and sizes for all three code percentages."""
    effective_code = 0
    for unit in report["units"]:
        for function in unit["functions"]:
            address = int(function["metadata"]["virtual_address"])
            comparison = comparisons.entities.get(address)
            equivalent = (
                comparison is not None
                and not comparison.is_stub
                and comparison.is_effective_match
            )
            if function["fuzzy_match_percent"] == 100 or equivalent:
                effective_code += int(function["size"])
    values = report["measures"]
    total_code = int(values["total_code"])
    effective_percent = effective_code / total_code * 100 if total_code else 0.0
    return {
        name: {"schemaVersion": 1, "label": label, "message": f"{percent:.2f}%", "color": color}
        for name, label, percent, color in (
            ("exact", "Exact Match", values["matched_code_percent"], "brightgreen"),
            ("fuzzy", "Fuzzy Match", values["fuzzy_match_percent"], "blue"),
            ("effective", "Effective Match", effective_percent, "orange"),
        )
    }


def main():
    report = json.loads(REPORT_JSON.read_text(encoding="utf-8"))
    comparisons = deserialize_reccmp_report(RECCMP_JSON.read_text(encoding="utf-8"))
    badges = build_badges(report, comparisons)
    BADGES_DIR.mkdir(parents=True, exist_ok=True)
    for name, badge in badges.items():
        (BADGES_DIR / f"{name}.json").write_text(json.dumps(badge) + "\n", encoding="utf-8")
        print(f"{badge['label']}: {badge['message']}")


if __name__ == "__main__":
    main()
