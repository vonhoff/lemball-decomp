#!/usr/bin/env python3
"""Link one new worktree to shared local tools."""

import argparse
import os
import shutil
import subprocess
from pathlib import Path


CACHE = Path(r"C:\lemball-tools")


def junction(target, source):
    subprocess.run(["cmd.exe", "/d", "/c", "mklink", "/J", str(target), str(source)], check=True)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("worktree", type=Path)
    root = parser.parse_args().worktree.resolve()
    for name in ("msvc420", ".decomp-venv"):
        source, target = CACHE / name, root / name
        if not source.is_dir():
            raise SystemExit(f"missing {source}")
        if not target.exists():
            junction(target, source)
    source, target = CACHE / "LEMBALL.EXE", root / "data" / "LEMBALL.EXE"
    if not source.is_file():
        raise SystemExit(f"missing {source}")
    if not target.exists():
        try:
            os.link(source, target)
        except OSError:
            shutil.copy2(source, target)


if __name__ == "__main__":
    main()
