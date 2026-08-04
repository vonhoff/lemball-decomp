#!/usr/bin/env python3

import os
import shutil
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build-msvc400"
OUTPUT = BUILD / "LEMBALL.EXE"
RULE = BUILD / "CMakeFiles" / "LEMBALL.dir" / "build.make"
EXPECTED_FLAGS = "/O2 /Ob1 /Oy /G4 /Z7 /c /Fo"


def stale_output(output, inputs):
    return not output.exists() or any(
        path.stat().st_mtime_ns > output.stat().st_mtime_ns for path in inputs
    )


def valid_build_rule(path):
    return path.exists() and EXPECTED_FLAGS in path.read_text(
        encoding="utf-8", errors="replace"
    )


def find_msvc400_root(root, environ=os.environ):
    configured = environ.get("MSVC400_ROOT")
    candidate = Path(configured) if configured else root / "msvc400"
    required = ("bin/CL.EXE", "bin/LINK.EXE", "include", "lib")
    if all((candidate / path).exists() for path in required):
        compiler = subprocess.run(
            [candidate / "bin" / "CL.EXE"],
            capture_output=True,
            text=True,
            check=False,
        )
        if "Compiler Version 10.00." in compiler.stdout + compiler.stderr:
            return candidate
    raise SystemExit(
        f"MSVC 4.00 (compiler version 10.00) not found at {candidate}; "
        "set MSVC400_ROOT to its install directory"
    )


def run(command, cwd=ROOT):
    print(" ".join(map(str, command)))
    subprocess.run(command, cwd=cwd, check=True)


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
    msvc_root = find_msvc400_root(ROOT)
    os.environ["MSVC400_ROOT"] = str(msvc_root)
    os.environ["PATH"] = str(msvc_root / "bin") + os.pathsep + os.environ["PATH"]
    os.environ["INCLUDE"] = str(msvc_root / "include")
    os.environ["LIB"] = str(msvc_root / "lib")

    cmake = find_cmake(ROOT, sys.executable)
    reccmp_python = find_reccmp_python(ROOT)
    run([reccmp_python, ROOT / "tools" / "install_reccmp_compat.py"])
    run([cmake, "--preset", "msvc400"])
    if not valid_build_rule(RULE):
        print("invalid MSVC 4.00 cache; configuring fresh")
        run([cmake, "--fresh", "--preset", "msvc400"])
    if not valid_build_rule(RULE):
        raise SystemExit("MSVC 4.00 compile rule is missing required flags")
    run([cmake, "--build", "--preset", "msvc400"])

    objects = list((BUILD / "CMakeFiles" / "LEMBALL.dir").rglob("*.obj"))
    if not objects:
        raise SystemExit("build produced no object files")
    if stale_output(OUTPUT, objects):
        nmake = shutil.which("nmake")
        print("stale LEMBALL.EXE; forcing relink")
        run(
            [nmake, "/nologo", "/f", r"CMakeFiles\LEMBALL.dir\build.make", "LEMBALL.EXE"],
            BUILD,
        )
    if stale_output(OUTPUT, objects):
        raise SystemExit("LEMBALL.EXE is older than object files")
    print(f"fresh {OUTPUT.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
