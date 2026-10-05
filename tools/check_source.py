#!/usr/bin/env python3
"""Check source policy, annotations, and catalog identities."""

import argparse

from lib.names import check_names
from lib.policy import check_annotations, check_policy


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
