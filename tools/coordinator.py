#!/usr/bin/env python3
"""Print next likely decompilation targets."""

import csv
import json
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


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
        return list(csv.DictReader(stream))


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
    return sorted(
        (row for row in functions if ratios.get(row["address"], 0) != 100),
        key=lambda row: (int(row["size"]) > 5, int(row["size"]), ratios.get(row["address"], 0), row["address"]),
    )


def main():
    ensure_tools(ROOT)
    ratios = matches(ROOT)
    notes = {}
    for path in (ROOT / "data" / "function-status").glob("*.csv"):
        notes.update({row["address"]: row.get("notes", "") for row in rows(path)})
    targets = rank(rows(ROOT / "data" / "objdiff-functions.csv"), ratios)[:10]
    if not targets:
        print("all functions complete")
        return
    print("next targets")
    for row in targets:
        ratio = ratios.get(row["address"])
        print(f"{row['address']} {row['size']}B {'-' if ratio is None else f'{ratio:.2f}%'} {row['unit']} {row['name']}")
        if notes.get(row["address"]):
            print(f"  {notes[row['address']]}")


if __name__ == "__main__":
    main()
