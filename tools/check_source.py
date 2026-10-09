#!/usr/bin/env python3
"""Check source policy, annotations, and catalog identities."""

import argparse
import csv
import json
from collections import Counter, defaultdict
from itertools import groupby
from pathlib import Path

from reccmp import color
from reccmp.dir import source_code_search
from reccmp.tools.decomplint import DecomplintTarget, display_errors, lint_all_targets

from lib import ROOT, TARGET_ID
from lib.names import scan
from lib.policy import violations
from lib.signatures import Signature


SRC = ROOT / "src"
CATALOG = ROOT / "tools/data/mac-symbol-catalog.csv"


def collect_sources(paths=None):
    """Find C/C++ sources under supplied paths or the configured source root."""
    return list(source_code_search([ROOT / path for path in paths or (SRC,)]))


def read_catalog():
    """Read symbol identities and Windows mappings from the fixed catalog."""
    symbols, by_windows = {}, defaultdict(list)
    with CATALOG.open(newline="", encoding="utf-8-sig") as stream:
        rows = csv.reader(stream)
        next(rows)
        for mac, name, win in rows:
            mac = int(mac, 16)
            symbols[mac] = name
            if win:
                by_windows[int(win, 16)].append(mac)
    for candidates in by_windows.values():
        candidates.sort()
    return symbols, by_windows


def read_inferences():
    """Read explicit source hypotheses and address-derived linker labels."""
    with (CATALOG.parent / "inferred-symbols.csv").open(
        newline="", encoding="utf-8-sig"
    ) as stream:
        inferences = {
            int(row["address"], 16): row["signature"]
            for row in csv.DictReader(stream)
            if row["signature"]
        }
    thunks = json.loads((CATALOG.parent / "linker-thunks.json").read_text())
    for thunk in thunks["thunks"]:
        inferences[thunk["address"]] = Signature("", thunk["symbol"], None).display()
    return inferences


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


def check_names(paths: list[Path | str] | None = None, verbose=False):
    """Require mapped or explicitly inferred names; keep ABI reviews informational."""
    symbols, mappings = read_catalog()
    inferences = read_inferences()
    files = collect_sources(paths)
    rows = [row for path in files for row in scan(path, symbols, mappings, inferences)]
    counts = Counter(row["status"] for row in rows)
    signatures = Counter(
        row["signature_status"] for row in rows if "signature_status" in row
    )
    for row in rows:
        required = row["status"] in (
            "mismatch",
            "case",
            "unresolved",
            "unmapped",
            "windows",
        )
        requested = verbose and (
            row["status"] == "inferred"
            or row.get("signature_status") in ("review", "unresolved")
        )
        if required or requested:
            detail = row.get("reason") or (
                f"{row['original_signature']} -> {row['actual_signature']}"
                f" ({', '.join(row['differences']) or row['signature_status']})"
            )
            print(
                f"{row['path']}:{row['line']}: {row['status']}: {detail} [{row['windows_address']}]"
            )
            if row.get("windows_evidence"):
                print(f"  Windows evidence: {row['windows_evidence']}")
    print(f"names: {len(files)} files, {len(rows)} annotated entries: {dict(counts)}")
    print(
        "names: unmapped kinds: "
        f"{dict(Counter(row['kind'] for row in rows if row['status'] == 'unmapped'))}"
    )
    print(f"names: parameter/const comparisons: {dict(signatures)}")
    if signatures["review"] or signatures["unresolved"]:
        print(
            "names: signature review requires Windows evidence; "
            "check_source.py --verbose lists items."
        )
    if counts["unresolved"]:
        return 2
    return int(bool(counts["mismatch"] or counts["case"] or counts["unmapped"]))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--path", action="append", dest="paths")
    parser.add_argument(
        "-v",
        "--verbose",
        action="store_true",
        help="show catalog review details and signatures",
    )
    args = parser.parse_args()
    if code := check_policy(args.paths):
        return code
    if code := check_annotations(args.paths):
        return code
    return check_names(args.paths, verbose=args.verbose)


if __name__ == "__main__":
    raise SystemExit(main())
