#!/usr/bin/env python3
"""Run source and compare gates.

  python tools/gate.py                 # smell + annot + layout + decomplint
  python tools/gate.py --path src/Foo  # scoped smell
  python tools/gate.py --names         # // 68K comments in src
  python tools/gate.py --vtable        # vtable comparison
  python tools/gate.py --all           # default + vtable + names
"""

from __future__ import annotations

import argparse

from lib.layout import check_layout
from lib.names import check_names
from lib.reccmp import check_decomplint
from lib.smell import check_smell
from lib.vtable import check_vtable


def main() -> int:
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument(
        "--path", action="append", default=[], dest="paths", help="scoped check paths"
    )
    parser.add_argument(
        "--names", action="store_true", help="68K naming vs source comments"
    )
    parser.add_argument("--vtable", action="store_true", help="vtable slot comparison")
    parser.add_argument(
        "--all", action="store_true", help="run all gates (default + vtable + names)"
    )
    args = parser.parse_args()
    paths = args.paths or None

    if paths and not (args.names or args.vtable or args.all):
        return check_smell(paths=paths)

    if args.names and not args.all:
        return check_names(paths=paths, fail=True)

    if args.vtable and not args.all:
        return check_vtable(no_build=True)

    code = check_smell(paths=paths, annot=True)
    if code != 0:
        return code

    code = check_layout(paths=paths, fail=True)
    if code != 0:
        return code

    code = check_decomplint(paths=paths)
    if code != 0:
        return code

    if args.all or args.names:
        code = check_names(paths=paths, fail=True)
        if code != 0:
            return code

    if args.all or args.vtable:
        code = check_vtable(no_build=True)
        if code != 0:
            return code

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
