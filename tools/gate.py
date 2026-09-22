#!/usr/bin/env python3
"""Run smell, annotation, 68K provenance, layout, decomplint and tool tests by default."""

from __future__ import annotations

import argparse
import subprocess
import sys
import unittest
from pathlib import Path

from lib.layout import check_layout
from lib.names import check_names
from lib.paths import SRC
from lib.provenance import check_provenance
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
        "--names", action="store_true", help="source names and signatures vs reviewed CSV evidence"
    )
    parser.add_argument("--names-original", action="store_true", help="audit exact original Mac spelling, including prefixes")
    parser.add_argument("--names-strict", action="store_true", help="fail naming case and signature review items")
    parser.add_argument("--names-json", action="store_true", help="emit catalog naming comparisons as JSON")
    parser.add_argument(
        "--68k", dest="provenance", action="store_true",
        help="verify 68K comments using the bundled metadata catalog"
    )
    parser.add_argument(
        "--68k-resource", type=Path, dest="resource",
        help="also verify the catalog against a private original resource fork"
    )
    parser.add_argument("--vtable", action="store_true", help="vtable slot comparison")
    parser.add_argument("--verbose", "-v", action="store_true", help="show verbose output (e.g. for vtable)")
    parser.add_argument("--top", type=int, default=0, help="show the N most frequent unresolved targets and pairs")
    parser.add_argument("--tools", action="store_true", help="comparison-tool regression tests")
    parser.add_argument(
        "--annot-strict", action="store_true", help="strict annotation checks (fail on review items and unmapped vtables)"
    )
    parser.add_argument(
        "--all", action="store_true", help="run all gates (default + vtable + names)"
    )
    args = parser.parse_args()
    paths = args.paths or None

    if (args.provenance or args.resource is not None) and not args.all:
        return check_provenance(paths=paths, strict=args.annot_strict, verbose=args.verbose,
                                resource=args.resource)

    if (args.names or args.names_original or args.names_strict or args.names_json) and not args.all:
        return check_names(paths=paths, fail=True, strict=args.names_strict, original=args.names_original,
                           verbose=args.verbose, as_json=args.names_json)

    if args.vtable and not args.all:
        return check_vtable(no_build=True, verbose=args.verbose, top=args.top, annot_strict=args.annot_strict)

    if args.tools and not args.all:
        return check_tool_tests()

    code = check_smell(paths=paths, annot=True, annot_strict=args.annot_strict)
    if code != 0:
        return code

    code = check_provenance(paths=paths, strict=args.annot_strict, verbose=args.verbose,
                            resource=args.resource)
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
        code = check_names(paths=paths, fail=True, strict=args.names_strict, original=args.names_original,
                           verbose=args.verbose, as_json=args.names_json)
        if code != 0:
            return code

        code = check_vtable(no_build=True, verbose=args.verbose, top=args.top, annot_strict=args.annot_strict)
        if code != 0:
            return code

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
