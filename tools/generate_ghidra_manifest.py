#!/usr/bin/env python3
"""Run Ghidra headlessly and export the LEMBALL function manifest."""

from __future__ import annotations

import argparse
import json
import os
import shutil
import subprocess
import tempfile
from pathlib import Path


TARGET_SHA256 = "d6337b58ccaf98df728b1490812cad0f927802d2e2c5fc932d00961f97027f63"
OWNERSHIP_CATEGORIES = {"internal", "runtime", "thunk", "import", "external"}


def find_headless(ghidra_home: Path | None) -> Path:
    names = ("analyzeHeadless.bat", "analyzeHeadless")
    if ghidra_home is not None:
        for relative in (Path("support"), Path("Ghidra/RuntimeScripts/Windows/support")):
            for name in names:
                candidate = ghidra_home / relative / name
                if candidate.is_file():
                    return candidate
    for variable in ("GHIDRA_HOME", "GHIDRA_INSTALL_DIR"):
        value = os.environ.get(variable)
        if value:
            try:
                return find_headless(Path(value))
            except SystemExit:
                pass
    for name in names:
        executable = shutil.which(name)
        if executable:
            return Path(executable)
    raise SystemExit("Ghidra analyzeHeadless was not found; pass --ghidra-home")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--ghidra-home", type=Path)
    parser.add_argument("--project", type=Path, required=True)
    parser.add_argument("--program", default="LEMBALL.EXE")
    parser.add_argument("--target", default="LEMBALL")
    parser.add_argument("--output", type=Path, default=Path("data/manifest.json"))
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    project = args.project.resolve()
    if not project.is_file():
        raise SystemExit(f"Ghidra project does not exist: {project}")
    headless = find_headless(args.ghidra_home)
    script_dir = Path(__file__).with_name("ghidra").resolve()
    output = args.output.resolve()
    output.parent.mkdir(parents=True, exist_ok=True)
    handle, temporary_name = tempfile.mkstemp(
        prefix=f".{output.name}.", suffix=".tmp", dir=output.parent
    )
    os.close(handle)
    temporary = Path(temporary_name)
    command = [
        str(headless), str(project.parent), project.stem,
        "-process", args.program, "-noanalysis", "-readOnly",
        "-scriptPath", str(script_dir),
        "-postScript", "ExportFunctionManifest.java", str(temporary), args.target,
    ]
    try:
        result = subprocess.run(command, check=False)
        if result.returncode != 0:
            return result.returncode
        manifest = json.loads(temporary.read_text(encoding="utf-8"))
        functions = manifest.get("functions", [])
        addresses = [int(function["address"], 16) for function in functions]
        summary = manifest.get("summary", {})
        categories = [function.get("category") for function in functions]
        if (
            manifest.get("version") != args.target
            or manifest.get("program") != args.program
            or str(manifest.get("sha256", "")).casefold() != TARGET_SHA256
            or manifest.get("function_count") != len(functions)
            or summary.get("total") != len(functions)
            or addresses != sorted(addresses)
            or len(addresses) != len(set(addresses))
            or any(category not in OWNERSHIP_CATEGORIES for category in categories)
            or any(int(function.get("size", 0)) <= 0 for function in functions if not function.get("is_external"))
            or any(function.get("is_thunk") and not function.get("thunk_target") for function in functions)
            or any(summary.get(category) != categories.count(category) for category in OWNERSHIP_CATEGORIES)
        ):
            raise SystemExit("Ghidra produced an invalid function manifest")
        temporary.replace(output)
        print(f"Validated and installed {output}")
        return 0
    finally:
        temporary.unlink(missing_ok=True)


if __name__ == "__main__":
    raise SystemExit(main())
