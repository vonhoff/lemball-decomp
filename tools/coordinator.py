#!/usr/bin/env python3
"""Print fallback singleton decompilation targets."""

import csv
import json
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
ILT_START = 0x00401000
ILT_END = 0x00403891


def ensure_tools(root):
    python = root / ".decomp-venv" / "Scripts" / "python.exe"
    if not python.exists():
        subprocess.run([sys.executable, "-m", "venv", python.parent.parent], check=True)
    healthy = subprocess.run(
        [python, "-c", "import pydantic_core, reccmp"],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    ).returncode == 0
    if not healthy:
        subprocess.run(
            [
                python,
                "-m",
                "pip",
                "install",
                "--disable-pip-version-check",
                "--requirement",
                root / "requirements.txt",
            ],
            check=True,
        )


def rows(path):
    with path.open(newline="", encoding="utf-8") as stream:
        functions = list(csv.DictReader(stream))
    invalid = [row["address"] for row in functions if ILT_START <= int(row["address"], 16) < ILT_END]
    if invalid:
        raise SystemExit(
            f"inventory contains linker ILT entry {invalid[0]}; "
            "retag it in Ghidra and regenerate data/objdiff-functions.csv"
        )
    return functions


def matches(root):
    path = root / "build-msvc420" / "reccmp.json"
    if not path.exists():
        return {}
    return {
        f"0x{int(row['address'], 16):08X}": float(row["matching"]) * 100
        for row in json.loads(path.read_text(encoding="utf-8"))["data"]
        if row.get("type") in (None, 1) and row.get("matching") is not None
    }


def rank(functions, ratios):
    def key(row):
        size = int(row["size"])
        ratio = ratios.get(row["address"], 0)
        return (
            size <= 5,
            ratio == 0,
            size * (100 - ratio),
            size,
            row["address"],
        )

    return sorted(
        (row for row in functions if ratios.get(row["address"], 0) != 100),
        key=key,
    )


def main():
    ensure_tools(ROOT)
    ratios = matches(ROOT)
    targets = rank(rows(ROOT / "data" / "objdiff-functions.csv"), ratios)[:25]
    if not targets:
        print("all functions complete")
        return
    print("fallback singleton targets")
    for row in targets:
        ratio = ratios.get(row["address"])
        print(f"{row['address']} {row['size']}B {'-' if ratio is None else f'{ratio:.2f}%'} {row['unit']} {row['name']}")


if __name__ == "__main__":
    main()
