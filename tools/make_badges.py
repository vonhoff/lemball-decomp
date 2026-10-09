#!/usr/bin/env python3
"""Export README badges without changing the canonical progress report."""

import json

from lib import BUILD
from lib.progress import effective_code_percent, load_progress

BADGES_DIR = BUILD / "badges"


def main():
    try:
        report, accepted = load_progress()
    except ValueError as exc:
        raise SystemExit(str(exc)) from exc
    values = report["measures"]
    BADGES_DIR.mkdir(parents=True, exist_ok=True)
    for name, percent, color in (
        ("exact", values["matched_code_percent"], "informational"),
        ("fuzzy", values["fuzzy_match_percent"], "inactive"),
        ("effective", effective_code_percent(report, accepted), "success"),
    ):
        badge = {
            "schemaVersion": 1,
            "label": name.title(),
            "message": f"{percent:.2f}%",
            "color": color,
        }
        (BADGES_DIR / f"{name}.json").write_text(
            json.dumps(badge) + "\n", encoding="utf-8"
        )
        print(f"{badge['label']}: {badge['message']}")


if __name__ == "__main__":
    main()
