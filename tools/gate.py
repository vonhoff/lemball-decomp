#!/usr/bin/env python3
"""Check source policy, annotations, and catalog identities."""

import argparse
from itertools import groupby

from colorama import Style
from reccmp.tools.decomplint import DecomplintTarget, display_errors, lint_all_targets
from reccmp.types import EntityType

from lib import collect_sources, load_engine
from lib.names import check_names
from lib.policy import check_policy


def check_vtable():
    _, engine = load_engine()
    tables = list(engine.compare_all(lambda entity: entity.entity_type == EntityType.VTABLE))
    exact = sum(table.accuracy == 1 for table in tables)
    print(f"vtables: {exact}/{len(tables)} exact")
    return int(not tables or exact != len(tables))


def check_annotations(paths=None):
    files = tuple(collect_sources(paths))
    target = DecomplintTarget(files, "LEMBALL", "utf-8")
    alerts = [alert for alert in lint_all_targets((target,)) if alert.target in (None, "LEMBALL")]
    alerts.sort(key=lambda alert: str(alert.path).lower())
    for path, errors in groupby(alerts, key=lambda alert: alert.path):
        display_errors(errors, path)
    if alerts:
        print(Style.RESET_ALL, end="")
    return int(any(alert.is_error() or alert.is_warning() for alert in alerts))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--path", action="append", dest="paths")
    checks = parser.add_mutually_exclusive_group()
    checks.add_argument("--names", action="store_true", help="show catalog review details only")
    checks.add_argument("--vtable", action="store_true", help="compare vtables only")
    args = parser.parse_args()
    if args.vtable and args.paths:
        parser.error("--path applies to source checks, not --vtable")
    if args.vtable:
        return check_vtable()
    if args.names:
        return check_names(args.paths, verbose=True)
    for check in (check_policy, check_annotations, check_names):
        if code := check(args.paths):
            return code
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
