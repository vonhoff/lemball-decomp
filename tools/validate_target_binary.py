#!/usr/bin/env python3

from __future__ import annotations

import argparse
import hashlib
import re
from pathlib import Path

TARGET_NAME = "LEMBALL"
ORIGINAL_BINARY_PATH = Path("data/LEMBALL.EXE")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Validate that the original LEMBALL.EXE matches reccmp-project.yml."
    )
    parser.add_argument(
        "--project",
        default="reccmp-project.yml",
        help="Path to reccmp-project.yml",
    )
    return parser.parse_args()


def parse_project_hash(project_path: Path) -> str:
    lines = project_path.read_text().splitlines()
    in_target = False
    for line in lines:
        if re.match(rf"^\s{{2}}{re.escape(TARGET_NAME)}:\s*$", line):
            in_target = True
            continue
        if in_target and re.match(r"^\s{2}[A-Za-z0-9_]+:\s*$", line):
            break
        if in_target:
            match = re.match(r"^\s{6}sha256:\s*([0-9a-fA-F]+)\s*$", line)
            if match:
                return match.group(1).lower()
    raise SystemExit(f"could not find sha256 for target {TARGET_NAME} in {project_path}")


def digest_of(path: Path) -> str:
    digest = hashlib.sha256()
    digest.update(path.read_bytes())
    return digest.hexdigest()


def main() -> int:
    args = parse_args()
    project_path = Path(args.project)
    expected_sha256 = parse_project_hash(project_path)
    if not ORIGINAL_BINARY_PATH.exists():
        raise SystemExit(
            f"target binary for {TARGET_NAME} not found. "
            "Expected data/LEMBALL.EXE."
        )

    actual_sha256 = digest_of(ORIGINAL_BINARY_PATH)
    if actual_sha256 != expected_sha256:
        raise SystemExit(
            f"target binary hash mismatch for {ORIGINAL_BINARY_PATH}: "
            f"expected {expected_sha256}, got {actual_sha256}"
        )

    print(f"{ORIGINAL_BINARY_PATH} sha256={actual_sha256}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
