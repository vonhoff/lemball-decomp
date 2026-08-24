#!/usr/bin/env python3
"""Build wrapper for LEMBALL MSVC 4.00 recompilation."""

from __future__ import annotations

import argparse
import re
import shutil
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build-msvc400"
LOG_PATH = BUILD / "last_build.log"
CMAKE = shutil.which("cmake") or str(ROOT / ".decomp-venv" / "Scripts" / "cmake.exe")


def handle_link(args: list[str]) -> int:
    """Helper for MSVC 4.0 LINK.EXE response file formatting and output verification."""
    if not args:
        sys.exit("build.py --link requires linker executable and arguments")

    linker = args[0]
    link_args = args[1:]

    for arg in link_args:
        if arg.startswith("@"):
            rsp_path = Path(arg[1:])
            if rsp_path.exists():
                content = rsp_path.read_text(encoding="utf-8", errors="ignore")
                tokens = content.split()
                rsp_path.write_text("\n".join(tokens) + "\n", encoding="utf-8")

    out_arg = next((Path(arg[5:]) for arg in link_args if arg.upper().startswith("/OUT:")), None)
    res = subprocess.run([linker, *link_args])
    if res.returncode == 0 and (out_arg is None or not out_arg.exists()):
        sys.stderr.write(f"linker produced no output: {out_arg}\n")
        return 1
    return res.returncode


def run_build(clean_first: bool = False, fresh: bool = False, extra_args: list[str] | None = None) -> int:
    BUILD.mkdir(parents=True, exist_ok=True)

    if fresh:
        res = subprocess.run([CMAKE, "--fresh", "--preset", "msvc400"], cwd=ROOT)
        if res.returncode != 0:
            return res.returncode

    if not clean_first:
        for fname in ("LEMBALL.pdb", "LEMBALL.ilk", "LEMBALL.EXE"):
            p = BUILD / fname
            if p.exists():
                try:
                    p.unlink()
                except OSError:
                    pass

    cmake_args = [CMAKE, "--build", "--preset", "msvc400"]
    if clean_first:
        cmake_args.append("--clean-first")
    if extra_args:
        cmake_args.extend(extra_args)

    start = time.perf_counter()
    proc = subprocess.run(cmake_args, cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors="replace")
    elapsed = time.perf_counter() - start

    output = proc.stdout or ""
    LOG_PATH.write_text(output, encoding="utf-8")

    filter_re = re.compile(r"warning|error|fatal|failed|built target|linking|\[\s*100%\s*\]", re.IGNORECASE)
    for line in output.splitlines():
        if filter_re.search(line):
            print(line)

    has_exe = (BUILD / "LEMBALL.EXE").exists()
    has_pdb = (BUILD / "LEMBALL.pdb").exists()
    print(f"RESULT exit={proc.returncode} elapsed_s={elapsed:.1f} exe={has_exe} pdb={has_pdb} log={LOG_PATH}")
    return proc.returncode


def main() -> int:
    if len(sys.argv) > 1 and sys.argv[1] == "--link":
        return handle_link(sys.argv[2:])

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--clean-first", action="store_true", help="Perform full clean build")
    parser.add_argument("--fresh", action="store_true", help="Re-run cmake configure before building")
    parser.add_argument("extra_args", nargs="*", help="Extra arguments passed to cmake --build")
    args = parser.parse_args()

    return run_build(clean_first=args.clean_first, fresh=args.fresh, extra_args=args.extra_args)


if __name__ == "__main__":
    raise SystemExit(main())
