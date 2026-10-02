#!/usr/bin/env python3
"""Check source reconstruction and comparison tools."""

import argparse
import unittest
from itertools import groupby
from pathlib import Path

from colorama import Style
from reccmp.dir import source_code_search
from reccmp.tools.decomplint import DecomplintTarget, display_errors, lint_all_targets
from reccmp.types import EntityType

from lib import SRC, load_engine
from lib.comments import check_comments
from lib.layout import check_layout
from lib.names import check_names
from lib.smell import check_smell


def check_vtable():
    _, engine = load_engine()
    tables = list(engine.compare_all(lambda entity: entity.entity_type == EntityType.VTABLE))
    exact = sum(table.accuracy == 1 for table in tables)
    print(f"vtables: {exact}/{len(tables)} exact")
    return int(not tables or exact != len(tables))


def check_tool_tests() -> int:
    suite = unittest.defaultTestLoader.discover(str(Path(__file__).parent / "tests"))
    return int(not unittest.TextTestRunner().run(suite).wasSuccessful())


def check_annotations(paths=None):
    files = tuple(source_code_search([Path(path) for path in paths or [SRC]]))
    target = DecomplintTarget(files, "LEMBALL", "utf-8")
    alerts = [alert for alert in lint_all_targets((target,)) if alert.target in (None, "LEMBALL")]
    alerts.sort(key=lambda alert: str(alert.path).lower())
    for path, errors in groupby(alerts, key=lambda alert: alert.path):
        display_errors(errors, path)
    if alerts:
        print(Style.RESET_ALL, end="")
    return int(any(alert.is_error() or alert.is_warning() for alert in alerts))


def check_source(paths=None):
    for check in (check_comments, check_smell, check_layout, check_annotations):
        if code := check(paths):
            return code
    return 0


def parse_args():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--path", action="append", dest="paths")
    checks = parser.add_mutually_exclusive_group()
    checks.add_argument("--names", action="store_true", help="show catalog review details only")
    checks.add_argument("--vtable", action="store_true", help="compare vtables only")
    args = parser.parse_args()
    if args.vtable and args.paths:
        parser.error("--path applies to source checks, not --vtable")
    return args


def main() -> int:
    args = parse_args()
    if args.vtable:
        return check_vtable()
    if args.names:
        return check_names(args.paths, verbose=True)
    if code := check_source(args.paths):
        return code
    if code := check_names(args.paths):
        return code
    return 0 if args.paths else check_tool_tests()


if __name__ == "__main__":
    raise SystemExit(main())
