#!/usr/bin/env python3
"""Check source policy and reccmp annotations."""

import argparse
from itertools import groupby

from reccmp import color
from reccmp.dir import source_code_search
from reccmp.tools.decomplint import DecomplintTarget, display_errors, lint_all_targets

from lib import ROOT, TARGET_ID
from lib.policy import violations


SRC = ROOT / "src"


def collect_sources(paths=None):
    """Find C/C++ sources under supplied paths or the configured source root."""
    return list(source_code_search([ROOT / path for path in paths or (SRC,)]))


def check_policy(paths=None):
    files = set(collect_sources(paths))
    for path in paths or [SRC]:
        path = ROOT / path
        files.update(
            p
            for p in (path.rglob("*") if path.is_dir() else [path])
            if p.is_file() and p.suffix.lower() in (".inl", ".rc")
        )
    failures = 0
    for path in sorted(files):
        text = path.read_text(encoding="utf-8")
        lines = text.splitlines()
        for line, message in sorted(violations(text)):
            print(f"{path}:{line}: {message}\n  {lines[line - 1].strip()}")
            failures += 1
    print(f"policy: {failures} violations")
    return int(bool(failures))


def check_annotations(paths=None) -> int:
    files = tuple(collect_sources(paths))
    target = DecomplintTarget(files, TARGET_ID, "utf-8")
    alerts = [
        alert
        for alert in lint_all_targets((target,))
        if alert.target in (None, TARGET_ID)
    ]
    alerts.sort(key=lambda alert: str(alert.path).lower())
    for path, errors in groupby(alerts, key=lambda alert: alert.path):
        display_errors(errors, path)
    if alerts:
        print(color.Style.RESET_ALL, end="")
    return int(any(alert.is_error() or alert.is_warning() for alert in alerts))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--path", action="append", dest="paths")
    args = parser.parse_args()
    if code := check_policy(args.paths):
        return code
    return check_annotations(args.paths)


if __name__ == "__main__":
    raise SystemExit(main())
