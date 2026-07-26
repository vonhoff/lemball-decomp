#!/usr/bin/env python3

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


def main():
    cmake = Path(sys.executable).with_name("cmake.exe")
    if not cmake.exists():
        found = shutil.which("cmake")
        if not found:
            raise SystemExit("cmake not found")
        cmake = Path(found)

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
        if not nmake:
            raise SystemExit("stale LEMBALL.EXE; run after VCVARS32.BAT")
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
