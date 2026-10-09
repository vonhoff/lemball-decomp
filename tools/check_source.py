#!/usr/bin/env python3
"""Check source policy, annotations, and catalog identities."""

import argparse
import json
from collections import defaultdict
from itertools import groupby
from pathlib import Path

from reccmp import color
from reccmp.dir import source_code_search
from reccmp.tools.decomplint import DecomplintTarget, display_errors, lint_all_targets

from lib import ROOT, TARGET_ID
from lib.codewarrior import decode_signature
from lib.names import read_catalog, scan
from lib.policy import violations


SRC = ROOT / "src"
CATALOG = ROOT / "tools/data/mac-symbol-catalog.csv"


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


def check_names(paths: list[Path | str] | None = None):
    """Check source identities and coverage of all mapped Windows addresses."""
    symbols, mappings = read_catalog(CATALOG)
    thunks = {
        thunk["address"]: thunk["symbol"]
        for thunk in json.loads((CATALOG.parent / "linker-thunks.json").read_text())[
            "thunks"
        ]
    }
    files = collect_sources(paths)
    rows = [row for path in files for row in scan(path, symbols, mappings, thunks)]
    missing, stubs = set(), set()
    if not paths:
        kinds = defaultdict(set)
        for row in rows:
            kinds[int(row["windows_address"], 16)].add(row["kind"])
        missing = mappings.keys() - kinds.keys()
        stubs = {address for address in mappings if kinds.get(address) == {"STUB"}}
        for address in sorted(missing | stubs):
            expected = "; ".join(
                decode_signature(symbols[mac]).display() for mac in mappings[address]
            )
            reason = "missing source annotation" if address in missing else "stub only"
            print(f"catalog: {reason}: {expected} [0x{address:08x}]")
        print(
            f"catalog: source coverage {len(mappings) - len(missing) - len(stubs)}/{len(mappings)} "
            f"mapped Windows addresses; {len(missing)} missing, {len(stubs)} stub-only"
        )
    failures = [row for row in rows if row["status"] != "match"]
    for row in failures:
        print(
            f"{row['path']}:{row['line']}: {row['reason']} [{row['windows_address']}]"
        )
    print(
        f"catalog: {len(rows)} mapped source entries; {len(failures)} identity errors"
    )
    return int(bool(missing or stubs or failures))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--path", action="append", dest="paths")
    args = parser.parse_args()
    if code := check_policy(args.paths):
        return code
    if code := check_annotations(args.paths):
        return code
    return check_names(args.paths)


if __name__ == "__main__":
    raise SystemExit(main())
