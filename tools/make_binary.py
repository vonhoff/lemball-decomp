#!/usr/bin/env python3
"""Build LEMBALL with MSVC 4.00; save the full log to last_build.log."""

import argparse
import re
import shutil
import subprocess
from pathlib import Path

from lib import BUILD, ROOT

LOG_PATH = BUILD / "last_build.log"
MSVC_DIAGNOSTIC = re.compile(
    r"\b(?:fatal )?error\s+[A-Z]*\d*\s*:|\bwarning\s+[A-Z]*\d*\s*:|Command line (?:error|warning)\b",
    re.IGNORECASE,
)


def stale_link_inputs(build_dir: Path) -> list[Path]:
    """Find generated linker dependencies newer than the executable."""
    executable = build_dir / "LEMBALL.EXE"
    if not executable.exists():
        return []
    target_dir = build_dir / "CMakeFiles" / "LEMBALL.dir"
    inputs = list(target_dir.rglob("*.obj"))
    inputs.extend(
        (
            build_dir / "LEMBALL.RES",
            target_dir / "build.make",
            target_dir / "objects1.rsp",
        )
    )
    timestamp = executable.stat().st_mtime_ns
    return [
        path for path in inputs if path.exists() and path.stat().st_mtime_ns > timestamp
    ]


def build_with_link_check(
    cmake_args: list[str], build_dir: Path, root: Path
) -> tuple[int, str]:
    """Verify both link artifacts; force at most one relink for stale dependencies."""
    output = ""
    for attempt in range(2):
        proc = subprocess.run(
            cmake_args,
            cwd=root,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            errors="replace",
            check=False,
        )
        output += proc.stdout
        if proc.returncode:
            return proc.returncode, output
        stale = stale_link_inputs(build_dir)
        if not stale:
            if all(
                (build_dir / name).exists() for name in ("LEMBALL.EXE", "LEMBALL.pdb")
            ):
                return 0, output
            return (
                1,
                output
                + "\nerror: build did not produce both LEMBALL.EXE and LEMBALL.pdb\n",
            )
        if attempt == 0:
            output += f"\nLink output is stale; forcing one relink after {stale[0]}\n"
            (build_dir / "LEMBALL.EXE").unlink()
    return (
        1,
        output
        + "\nerror: executable is still older than its link inputs after retry\n",
    )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--clean-first", action="store_true", help="Perform full clean build"
    )
    parser.add_argument(
        "--disable-startup-checks",
        action="store_true",
        help="Disable startup CD and installation checks",
    )
    args = parser.parse_args()

    venv = ROOT / ".decomp-venv/Scripts/cmake.exe"
    cmake = str(venv) if venv.exists() else shutil.which("cmake")
    if not cmake:
        raise SystemExit("cmake not found")
    BUILD.mkdir(parents=True, exist_ok=True)
    startup_checks = "OFF" if args.disable_startup_checks else "ON"
    configured = subprocess.run(
        [
            cmake,
            "--preset",
            "msvc400",
            f"-DLEMBALL_ENFORCE_STARTUP_CHECKS={startup_checks}",
        ],
        cwd=ROOT,
        check=False,
    ).returncode
    if configured:
        return configured

    if args.clean_first:
        for fname in ("LEMBALL.pdb", "LEMBALL.ilk", "LEMBALL.EXE"):
            (BUILD / fname).unlink(missing_ok=True)

    cmake_args = [cmake, "--build", "--preset", "msvc400"]
    if args.clean_first:
        cmake_args.append("--clean-first")

    returncode, output = build_with_link_check(cmake_args, BUILD, ROOT)
    LOG_PATH.write_text(output, encoding="utf-8")
    if returncode:
        print(output)
    else:
        for line in output.splitlines():
            if MSVC_DIAGNOSTIC.search(line):
                print(line)

    print(f"build: exit={returncode}; log={LOG_PATH}")
    return returncode


if __name__ == "__main__":
    raise SystemExit(main())
