#!/usr/bin/env python3

from __future__ import annotations

import argparse
import json
from pathlib import Path

from project_manifest import list_cpp_sources, load_target_slice_units, unit_name_from_source


WATCH_PATTERNS = [
    "*.c",
    "*.cc",
    "*.cp",
    "*.cpp",
    "*.cxx",
    "*.c++",
    "*.h",
    "*.hh",
    "*.hp",
    "*.hpp",
    "*.hxx",
    "*.h++",
    "*.pch",
    "*.pch++",
    "*.inc",
    "*.s",
    "*.S",
    "*.asm",
    "*.py",
    "*.yml",
    "*.yaml",
    "*.txt",
    "*.json",
]

IGNORE_PATTERNS = [
    "build/**/*",
]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate objdiff.json from the current LEMBALL manifests."
    )
    parser.add_argument("--version", required=True, help="Version identifier, e.g. LEMBALL")
    parser.add_argument("--output", required=True, help="Path to write objdiff.json")
    return parser.parse_args()


def build_units(version: str) -> list[dict[str, object]]:
    source_root = Path("LEMBALL")
    slice_manifest_path = Path("manifests") / "target_slices" / f"{version}.txt"
    source_units: dict[str, Path] = {}

    for source_path in list_cpp_sources(source_root):
        source_units[unit_name_from_source(source_path)] = source_path

    target_units = set(load_target_slice_units(slice_manifest_path))
    all_units = sorted(set(source_units) | target_units)
    units: list[dict[str, object]] = []

    for unit_name in all_units:
        unit: dict[str, object] = {
            "name": unit_name,
        }
        if unit_name in target_units:
            unit["target_path"] = f"build/target/{version}/{unit_name}.obj"
        if unit_name in source_units:
            unit["base_path"] = f"build/base/{version}/{unit_name}.obj"
        units.append(unit)

    return units


def build_objdiff_config(version: str) -> dict[str, object]:
    return {
        "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
        "custom_make": "make",
        "custom_args": [f"VERSION={version}"],
        "build_target": True,
        "build_base": True,
        "watch_patterns": WATCH_PATTERNS,
        "ignore_patterns": IGNORE_PATTERNS,
        "units": build_units(version),
        "progress_categories": [
            {"id": "game", "name": "Game"},
            {"id": "sdk", "name": "SDK / Third Party"},
        ],
    }


def main() -> int:
    args = parse_args()
    output_path = Path(args.output)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(json.dumps(build_objdiff_config(args.version), indent=2) + "\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
