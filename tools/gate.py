#!/usr/bin/env python3
"""Run smell, annotation, layout, decomplint and tool tests by default."""

from __future__ import annotations

import argparse
import subprocess
import sys
import unittest
from pathlib import Path

from lib.layout import check_layout
from lib.names import check_names
from lib.paths import SRC
from lib.smell import check_smell
from lib.vtable import check_vtable


def check_decomplint(
    paths: list[Path | str] | None = None,
    target: str = "LEMBALL",
    warnfail: bool = True,
    encoding: str = "utf-8",
) -> int:
    command = [sys.executable, "-m", "reccmp.tools.decomplint", "--encoding", encoding]
    if target is not None:
        command.extend(["--target", target])
    if warnfail:
        command.append("--warnfail")
    command.extend(str(path) for path in (paths or [SRC]))
    return subprocess.run(command, check=False).returncode


def check_tool_tests() -> int:
    suite = unittest.defaultTestLoader.discover(str(Path(__file__).parent / "tests"))
    return 0 if unittest.TextTestRunner().run(suite).wasSuccessful() else 1


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--path", action="append", default=[], dest="paths", help="scoped check paths"
    )
    parser.add_argument(
        "--names", action="store_true", help="68K naming vs source comments"
    )
    parser.add_argument("--vtable", action="store_true", help="vtable slot comparison")
    parser.add_argument("--verbose", "-v", action="store_true", help="show verbose output (e.g. for vtable)")
    parser.add_argument("--top", type=int, default=0, help="show the N most frequent unresolved targets and pairs")
    parser.add_argument("--tools", action="store_true", help="comparison-tool regression tests")
    parser.add_argument(
        "--all", action="store_true", help="run all gates (default + vtable + names)"
    )
    args = parser.parse_args()
    paths = args.paths or None

    if args.names and not args.all:
        return check_names(paths=paths, fail=True)

    if args.vtable and not args.all:
        return check_vtable(no_build=True, verbose=args.verbose, top=args.top)

    if args.tools and not args.all:
        return check_tool_tests()

    code = check_smell(paths=paths, annot=True)
    if code != 0:
        return code

    code = check_layout(paths=paths, fail=True)
    if code != 0:
        return code

    code = check_decomplint(paths=paths)
    if code != 0:
        return code

    if not paths:
        code = check_tool_tests()
        if code != 0:
            return code

    if args.all:
        code = check_names(paths=paths, fail=True)
        if code != 0:
            return code

        code = check_vtable(no_build=True, verbose=args.verbose, top=args.top)
        if code != 0:
            return code

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
