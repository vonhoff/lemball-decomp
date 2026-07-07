#!/usr/bin/env python3

from __future__ import annotations

import argparse
from pathlib import Path

from project_manifest import find_original_binary


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate reccmp-user.yml for the local original binary."
    )
    parser.add_argument("--version", required=True, help="Version identifier, e.g. LEMBALL")
    parser.add_argument("--target", default="LEMBALL", help="reccmp target name")
    parser.add_argument("--output", default="reccmp-user.yml", help="Output YAML path")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    binary = find_original_binary(args.version)
    if binary is None:
        raise SystemExit(
            f"target binary for {args.version} not found. "
            "Expected orig/LEMBALL/LEMBALL.EXE or a configured alternate path."
        )

    output = Path(args.output)
    output.write_text(
        "targets:\n"
        "  {target}:\n"
        "    path: {binary}\n".format(target=args.target, binary=binary)
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
