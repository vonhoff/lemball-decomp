#!/usr/bin/env python3
"""Build wrapper for LEMBALL MSVC 4.00 recompilation.

CL uses /WX (see cmake/msvc400-toolchain.cmake); LINK warnings are detected here.
Warnings from either tool fail the build.
Stdout is filtered; full log is written to build-msvc400/last_build.log.
"""

from __future__ import annotations

import argparse
import ctypes
import os
import re
import shutil
import subprocess
import sys
import time
from pathlib import Path

from lib.paths import BUILD, ROOT

LOG_PATH = BUILD / "last_build.log"
LOG_INTEREST = re.compile(r"warning|error|fatal|failed|built target|linking|\[\s*100%\s*\]", re.IGNORECASE)
MSVC_WARNING = re.compile(r"\bwarning\s+[A-Z]*\d+\s*:", re.IGNORECASE)


def win_short_path(path: str) -> str:
    resolved = str(Path(path).resolve())
    if os.name != "nt" or " " not in resolved:
        return resolved
    get_short = ctypes.windll.kernel32.GetShortPathNameW
    get_short.argtypes = [ctypes.c_wchar_p, ctypes.c_wchar_p, ctypes.c_uint]
    get_short.restype = ctypes.c_uint
    buf = ctypes.create_unicode_buffer(32768)
    if get_short(resolved, buf, 32768) and buf.value and " " not in buf.value:
        return buf.value
    return resolved


def resolve_cmake() -> str:
    venv = ROOT / ".decomp-venv" / "Scripts" / "cmake.exe"
    if venv.exists():
        return win_short_path(str(venv))
    found = shutil.which("cmake")
    if found:
        return win_short_path(found)
    sys.exit("cmake not found")


def cache_cmake_command() -> str | None:
    cache = BUILD / "CMakeCache.txt"
    if not cache.exists():
        return None
    for line in cache.read_text(encoding="utf-8", errors="replace").splitlines():
        if line.startswith("CMAKE_COMMAND:"):
            return line.split("=", 1)[-1].strip().strip('"')
    return None


def handle_link(args: list[str]) -> int:
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
    res = subprocess.run(
        [linker, *link_args],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        errors="replace",
    )
    output = res.stdout or ""
    sys.stdout.write(output)
    warning_count = sum(1 for line in output.splitlines() if MSVC_WARNING.search(line))
    if warning_count:
        sys.stderr.write(f"linker emitted {warning_count} warning(s)\n")
        return 1 if res.returncode == 0 else res.returncode
    if res.returncode == 0 and (out_arg is None or not out_arg.exists()):
        sys.stderr.write(f"linker produced no output: {out_arg}\n")
        return 1
    return res.returncode


def run_build(clean_first: bool = False, extra_args: list[str] | None = None) -> int:
    cmake = resolve_cmake()
    BUILD.mkdir(parents=True, exist_ok=True)

    cached = cache_cmake_command()
    makefile = BUILD / "Makefile"
    toolchain = ROOT / "cmake" / "msvc400-toolchain.cmake"
    need_configure = cached is None or " " in cached or not makefile.exists()
    if (
        not need_configure
        and toolchain.exists()
        and makefile.exists()
        and toolchain.stat().st_mtime > makefile.stat().st_mtime
    ):
        need_configure = True
    if need_configure:
        res = subprocess.run([cmake, "--preset", "msvc400"], cwd=ROOT)
        if res.returncode != 0:
            return res.returncode

    if clean_first:
        for fname in ("LEMBALL.pdb", "LEMBALL.ilk", "LEMBALL.EXE"):
            p = BUILD / fname
            if p.exists():
                try:
                    p.unlink()
                except OSError:
                    pass

    cmake_args = [cmake, "--build", "--preset", "msvc400"]
    if clean_first:
        cmake_args.append("--clean-first")
    if extra_args:
        cmake_args.extend(extra_args)

    start = time.perf_counter()
    proc = subprocess.run(cmake_args, cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors="replace")
    elapsed = time.perf_counter() - start

    output = proc.stdout or ""
    LOG_PATH.write_text(output, encoding="utf-8")
    for line in output.splitlines():
        if LOG_INTEREST.search(line):
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
    parser.add_argument("extra_args", nargs="*", help="Extra arguments passed to cmake --build")
    args = parser.parse_args()

    return run_build(clean_first=args.clean_first, extra_args=args.extra_args)


if __name__ == "__main__":
    raise SystemExit(main())
