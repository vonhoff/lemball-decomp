#!/usr/bin/env python3
"""Print next unfinished decompilation range."""

import argparse
import csv
import json
from pathlib import Path


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


def main():
    parser = argparse.ArgumentParser(description="Print next decompilation targets")
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    root = parser.parse_args().root.resolve()
    ratios = matches(root)
    functions = rows(root / "data" / "objdiff-functions.csv")
    for work_range in rows(root / "data" / "work-ranges.csv"):
        start, end = int(work_range["start"], 16), int(work_range["end"], 16)
        status = root / "data" / "function-status" / f"{work_range['id']}.csv"
        notes = {row["address"]: row.get("notes", "") for row in rows(status)} if status.exists() else {}
        pending = [
            row for row in functions
            if start <= int(row["address"], 16) < end and ratios.get(row["address"]) != 100
        ]
        if pending:
            print(f"next {work_range['id']} {work_range['start']}..{work_range['end']}")
            for row in pending:
                ratio = ratios.get(row["address"])
                print(f"{row['address']} {'-' if ratio is None else f'{ratio:.2f}%'} {row['unit']} {row['name']}")
                if notes.get(row["address"]):
                    print(f"  {notes[row['address']]}")
            return
    print("all ranges complete")


if __name__ == "__main__":
    main()
