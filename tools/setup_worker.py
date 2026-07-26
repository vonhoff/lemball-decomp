#!/usr/bin/env python3
"""Link one worktree to tools in primary checkout."""

import argparse
import os
import subprocess
from pathlib import Path


def junction(target, source):
    subprocess.run(["cmd.exe", "/d", "/c", "mklink", "/J", str(target), str(source)], check=True)


def primary(root):
    lines = subprocess.check_output(
        ["git", "worktree", "list", "--porcelain"], cwd=root, text=True
    ).splitlines()
    return Path(next(line[9:] for line in lines if line.startswith("worktree ")))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("worktree", type=Path)
    parser.add_argument("--refresh", action="store_true")
    args = parser.parse_args()
    root = args.worktree.resolve()
    tools = primary(root)
    if root == tools:
        raise SystemExit("worktree must not be primary checkout")
    for name in ("msvc420", ".decomp-venv"):
        source, target = tools / name, root / name
        if not source.is_dir():
            raise SystemExit(f"missing {source}")
        if args.refresh and os.path.lexists(target):
            os.rmdir(target)
        if not target.exists():
            junction(target, source)
    if args.refresh:
        target = root / "data" / "LEMBALL.EXE"
        if os.path.lexists(target):
            target.unlink()
    target = root / "reccmp-user.yml"
    target.write_text(
        "targets:\n  LEMBALL:\n    path: " + str(tools / "data" / "LEMBALL.EXE") + "\n",
        encoding="utf-8",
    )


if __name__ == "__main__":
    main()
