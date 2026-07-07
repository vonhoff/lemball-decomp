#!/usr/bin/env python3

from __future__ import annotations

import pathlib
import subprocess
import sys


def parse_out_path(args: list[str]) -> pathlib.Path | None:
    for arg in args:
        upper = arg.upper()
        if upper.startswith("/OUT:"):
            value = arg[5:]
            if value:
                return pathlib.Path(value)
    return None


def print_candidates(pattern: str) -> None:
    print(f"{pattern} candidates:", file=sys.stderr)
    for path in sorted(pathlib.Path.cwd().rglob(pattern)):
        if path.is_file():
            print(f"  {path}", file=sys.stderr)


def main() -> int:
    if len(sys.argv) < 2:
        print("check_link_output.py: missing linker path", file=sys.stderr)
        return 2

    linker = sys.argv[1]
    args = sys.argv[2:]
    out_path = parse_out_path(args)

    result = subprocess.run([linker, *args], check=False)
    if result.returncode != 0:
        return result.returncode

    if out_path is None:
        print("check_link_output.py: no /OUT: argument was provided", file=sys.stderr)
        return 1

    expected_output = out_path if out_path.is_absolute() else pathlib.Path.cwd() / out_path
    if expected_output.exists():
        return 0

    print(
        f"check_link_output.py: linker returned success but did not produce {expected_output}",
        file=sys.stderr,
    )
    print(f"check_link_output.py: working directory: {pathlib.Path.cwd()}", file=sys.stderr)
    print_candidates("LEMBALL.EXE")
    print_candidates("LEMBALL.pdb")
    print("cwd contents:", file=sys.stderr)
    for path in sorted(pathlib.Path.cwd().iterdir()):
        print(f"  {path}", file=sys.stderr)
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
