#!/usr/bin/env python3

import os
import shutil
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build-msvc420"
OUTPUT = BUILD / "LEMBALL.EXE"
RULE = BUILD / "CMakeFiles" / "LEMBALL.dir" / "build.make"
EXPECTED_FLAGS = "/TP /O2 /Ob1 /Oy /G4 /Z7 /c /Fo"


def stale_output(output, inputs):
    return not output.exists() or any(
        path.stat().st_mtime_ns > output.stat().st_mtime_ns for path in inputs
    )


def valid_build_rule(path):
    return path.exists() and EXPECTED_FLAGS in path.read_text(
        encoding="utf-8", errors="replace"
    )


def run(command, cwd=ROOT):
    print(" ".join(map(str, command)))
    subprocess.run(command, cwd=cwd, check=True)


def bootstrap_command(vcvars, python, script):
    return (
        f'call "{vcvars}" x86 && '
        f'set "LEMBALL_MSVC420_READY=1" && '
        f'"{python}" "{script}"'
    )


def short_path(path):
    if os.name != "nt" or " " not in str(path):
        return path
    from ctypes import WinDLL, create_unicode_buffer

    buffer = create_unicode_buffer(32768)
    if not WinDLL("kernel32", use_last_error=True).GetShortPathNameW(
        str(path), buffer, len(buffer)
    ):
        raise SystemExit(f"CMake path contains spaces and has no short form: {path}")
    return Path(buffer.value)


def find_cmake(root, python):
    candidates = [
        root / ".decomp-venv" / "Scripts" / "cmake.exe",
        root / ".venv" / "Scripts" / "cmake.exe",
        Path(python).with_name("cmake.exe"),
    ]
    found = shutil.which("cmake")
    if found:
        candidates.append(Path(found))
    for candidate in candidates:
        if candidate.exists():
            return short_path(candidate)
    raise SystemExit("cmake not found; install requirements.txt")


def find_reccmp_python(root):
    candidates = [
        root / ".decomp-venv" / "Scripts" / "python.exe",
        root / ".venv" / "Scripts" / "python.exe",
    ]
    reccmp = shutil.which("reccmp-datacmp")
    if reccmp:
        scripts = Path(reccmp).parent
        candidates.extend([scripts / "python.exe", scripts.parent / "python.exe"])
    candidates.append(Path(sys.executable))

    for candidate in candidates:
        if not candidate.exists():
            continue
        result = subprocess.run(
            [candidate, "-c", "import reccmp"],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
            check=False,
        )
        if result.returncode == 0:
            return candidate
    raise SystemExit("reccmp not found; install requirements.txt")


def main():
    if os.environ.get("LEMBALL_MSVC420_READY") != "1":
        vcvars = ROOT / "msvc420" / "bin" / "VCVARS32.BAT"
        if not vcvars.exists():
            raise SystemExit(f"missing {vcvars.relative_to(ROOT)}")
        command = bootstrap_command(vcvars, sys.executable, Path(__file__).resolve())
        raise SystemExit(
            subprocess.call(f"cmd.exe /d /c {command}", cwd=ROOT)
        )

    cmake = find_cmake(ROOT, sys.executable)
    reccmp_python = find_reccmp_python(ROOT)
    run([reccmp_python, ROOT / "tools" / "install_reccmp_compat.py"])

    run([cmake, "--preset", "msvc420"])
    if not valid_build_rule(RULE):
        print("invalid MSVC 4.20 cache; configuring fresh")
        run([cmake, "--fresh", "--preset", "msvc420"])
    if not valid_build_rule(RULE):
        raise SystemExit("MSVC 4.20 compile rule is missing required flags")
    run([cmake, "--build", "--preset", "msvc420"])

    objects = list((BUILD / "CMakeFiles" / "LEMBALL.dir").rglob("*.obj"))
    if not objects:
        raise SystemExit("build produced no object files")
    if stale_output(OUTPUT, objects):
        nmake = shutil.which("nmake")
        print("stale LEMBALL.EXE; forcing relink")
        run(
            [
                nmake,
                "/nologo",
                "/f",
                r"CMakeFiles\LEMBALL.dir\build.make",
                "LEMBALL.EXE",
            ],
            BUILD,
        )
    if stale_output(OUTPUT, objects):
        raise SystemExit("LEMBALL.EXE is older than object files")
    print(f"fresh {OUTPUT.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
