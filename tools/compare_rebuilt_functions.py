#!/usr/bin/env python3

from __future__ import annotations

import importlib.util
import json
import shutil
import subprocess
import sys
from pathlib import Path

TARGET_NAME = "LEMBALL"
ORIGINAL_BINARY_PATH = Path("data/LEMBALL.EXE")
MANIFEST_PATH = Path("data/manifest.json")
RECCMP_USER_PATH = Path("reccmp-user.yml")
RECCMP_BUILD_PATH = Path("reccmp-build.yml")
RECOMPILED_BINARY_PATH = Path("build-msvc420/LEMBALL.EXE")
RECOMPILED_PDB_PATH = Path("build-msvc420/LEMBALL.pdb")


def write_reccmp_user_yml() -> None:
    RECCMP_USER_PATH.write_text(
        "targets:\n"
        f"  {TARGET_NAME}:\n"
        f"    path: {ORIGINAL_BINARY_PATH.resolve()}\n"
    )


def write_reccmp_build_yml() -> None:
    RECCMP_BUILD_PATH.write_text(
        "project: .\n"
        "targets:\n"
        f"  {TARGET_NAME}:\n"
        f"    path: {RECOMPILED_BINARY_PATH}\n"
        f"    pdb: {RECOMPILED_PDB_PATH}\n"
    )


def total_reportable_functions() -> int:
    if not MANIFEST_PATH.exists():
        return 0
    manifest = json.loads(MANIFEST_PATH.read_text())
    return sum(
        1 for function in manifest["functions"] if function["category"] in {"internal", "thunk"}
    )


def ensure_reccmp_inputs() -> None:
    if not RECCMP_USER_PATH.exists():
        if not ORIGINAL_BINARY_PATH.exists():
            raise SystemExit(
                f"target binary for {TARGET_NAME} not found.\nExpected data/LEMBALL.EXE."
            )
        write_reccmp_user_yml()

    if RECOMPILED_BINARY_PATH.exists() and RECOMPILED_PDB_PATH.exists():
        write_reccmp_build_yml()

    if not RECCMP_BUILD_PATH.exists():
        raise SystemExit(
            "reccmp-build.yml is missing, and no rebuilt LEMBALL executable/PDB pair was found.\n"
            f"Expected {RECOMPILED_BINARY_PATH} with {RECOMPILED_PDB_PATH}."
        )


def has_total_arg(args: list[str]) -> bool:
    return "--total" in args or "-T" in args


def run_reccmp(args: list[str]) -> int:
    command = ["reccmp-reccmp", "--target", TARGET_NAME]
    if not has_total_arg(args):
        command.extend(["--total", str(total_reportable_functions())])
    command.extend(args)

    reccmp_bin = shutil.which("reccmp-reccmp")
    if reccmp_bin is not None:
        command[0] = reccmp_bin
        return subprocess.run(command, check=False).returncode

    if importlib.util.find_spec("reccmp") is not None:
        return subprocess.run(
            [sys.executable, "-m", "reccmp.reccmp", *command[1:]],
            check=False,
        ).returncode

    raise SystemExit(
        "reccmp is not installed.\n"
        "This project's byte-accurate verify step needs an MSVC-era build lane plus reccmp."
    )


def main() -> int:
    ensure_reccmp_inputs()
    return run_reccmp(sys.argv[1:])


if __name__ == "__main__":
    raise SystemExit(main())
