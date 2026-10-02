#!/usr/bin/env python3
"""Check source reconstruction and comparison tools."""

import argparse
import subprocess
import sys
import unittest
from pathlib import Path

from reccmp.tools.asmcmp import print_match_verbose
from reccmp.types import EntityType

from lib import SRC, load_engine
from lib.comments import check_comments
from lib.layout import check_layout
from lib.names import check_names
from lib.smell import check_smell


def check_vtable(verbose=False):
    _, engine = load_engine()
    tables = list(engine.compare_all(lambda entity: entity.entity_type == EntityType.VTABLE))
    for table in tables:
        if verbose and table.accuracy < 1:
            print_match_verbose(table)
    exact = sum(table.accuracy == 1 for table in tables)
    print(f"vtables: {exact}/{len(tables)} exact")
    return int(not tables or exact != len(tables))


def check_tool_tests() -> int:
    suite = unittest.defaultTestLoader.discover(str(Path(__file__).parent / 'tests'))
    return int(not unittest.TextTestRunner().run(suite).wasSuccessful())


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--path', action='append', dest='paths')
    parser.add_argument('--names', action='store_true')
    parser.add_argument('--names-strict', action='store_true')
    parser.add_argument('--names-json', action='store_true')
    parser.add_argument('--vtable', action='store_true')
    parser.add_argument('--verbose', '-v', action='store_true')
    parser.add_argument('--tools', action='store_true')
    parser.add_argument('--all', action='store_true', help='also compare vtables')
    args = parser.parse_args()
    def names():
        return check_names(args.paths, strict=args.names_strict,
                           verbose=args.verbose, as_json=args.names_json)
    if not args.all:
        if args.names or args.names_strict or args.names_json:
            return names()
        if args.vtable:
            return check_vtable(args.verbose)
        if args.tools:
            return check_tool_tests()
    checks = [
        lambda: check_comments(args.paths),
        lambda: check_smell(args.paths),
        lambda: check_layout(args.paths),
        lambda: subprocess.run(
            [sys.executable, '-m', 'reccmp.tools.decomplint', '--encoding', 'utf-8',
             '--target', 'LEMBALL', '--warnfail', *(str(path) for path in args.paths or [SRC])], check=False
        ).returncode,
        names,
    ]
    if not args.paths:
        checks.append(check_tool_tests)
    if args.all:
        checks.append(lambda: check_vtable(args.verbose))
    for check in checks:
        if code := check():
            return code
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
