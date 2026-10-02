#!/usr/bin/env python3
"""Export README badges without changing the canonical progress report."""

import json

from reccmp.compare.report import deserialize_reccmp_report

from lib import BUILD, RECCMP_JSON, REPORT_JSON
from lib.effective import EFFECTIVE_JSON, effective_addresses

BADGES_DIR = BUILD / "badges"


def effective_totals(report, comparisons, additional=()):
    """Count exact and equivalent functions once, using canonical original sizes."""
    effective_code = effective_count = 0
    accepted = effective_addresses(comparisons.entities, additional)
    for unit in report["units"]:
        for function in unit["functions"]:
            address = int(function["metadata"]["virtual_address"])
            if function["fuzzy_match_percent"] == 100 or address in accepted:
                effective_code += int(function["size"])
                effective_count += 1
    return effective_code, effective_count


def build_badges(report, comparisons, additional=()):
    """Use the canonical inventory and sizes for all three code percentages."""
    effective_code, _ = effective_totals(report, comparisons, additional)
    values = report["measures"]
    total_code = int(values["total_code"])
    effective_percent = effective_code / total_code * 100 if total_code else 0.0
    return {
        name: {"schemaVersion": 1, "label": label, "message": f"{percent:.2f}%", "color": color}
        for name, label, percent, color in (
            ("exact", "Exact", values["matched_code_percent"], "blue"),
            ("fuzzy", "Fuzzy", values["fuzzy_match_percent"], "lightgrey"),
            ("effective", "Effective", effective_percent, "green"),
        )
    }


def main():
    report = json.loads(REPORT_JSON.read_text(encoding="utf-8"))
    reccmp_text = RECCMP_JSON.read_text(encoding="utf-8")
    comparisons = deserialize_reccmp_report(reccmp_text)
    additional = {int(address): reasons for address, reasons in json.loads(EFFECTIVE_JSON.read_text(encoding="utf-8")).items()}
    badges = build_badges(report, comparisons, additional)
    BADGES_DIR.mkdir(parents=True, exist_ok=True)
    for name, badge in badges.items():
        (BADGES_DIR / f"{name}.json").write_text(json.dumps(badge) + "\n", encoding="utf-8")
        print(f"{badge['label']}: {badge['message']}")


if __name__ == "__main__":
    main()
