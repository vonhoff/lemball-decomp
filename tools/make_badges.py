#!/usr/bin/env python3
"""Export README badges without changing the canonical progress report."""

import json

from lib.project import BUILD, EFFECTIVE_JSON, REPORT_JSON
from lib.progress.metrics import effective_measures
from lib.progress.snapshot import load_progress

BADGES_DIR = BUILD / "badges"


def build_badges(report, accepted):
    """Weight accepted functions by the canonical original sizes."""
    values = report["measures"]
    effective_percent = effective_measures(report, accepted)["matched_code_percent"]
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
    try:
        report, accepted = load_progress(REPORT_JSON, EFFECTIVE_JSON)
    except ValueError as exc:
        raise SystemExit(str(exc)) from exc
    badges = build_badges(report, accepted)
    BADGES_DIR.mkdir(parents=True, exist_ok=True)
    for name, badge in badges.items():
        (BADGES_DIR / f"{name}.json").write_text(
            json.dumps(badge) + "\n", encoding="utf-8"
        )
        print(f"{badge['label']}: {badge['message']}")


if __name__ == "__main__":
    main()
