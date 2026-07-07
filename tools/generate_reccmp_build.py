#!/usr/bin/env python3

from __future__ import annotations

import argparse
from pathlib import Path


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate reccmp-build.yml for a local rebuilt binary."
    )
    parser.add_argument("--target", required=True, help="reccmp target name")
    parser.add_argument("--binary", required=True, help="Path to rebuilt executable")
    parser.add_argument("--pdb", required=True, help="Path to rebuilt PDB")
    parser.add_argument("--project", default=".", help="Project root recorded in the YAML")
    parser.add_argument("--output", default="reccmp-build.yml", help="Output YAML path")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    binary = Path(args.binary).resolve()
    pdb = Path(args.pdb).resolve()
    output = Path(args.output)

    output.write_text(
        "project: {project}\n"
        "targets:\n"
        "  {target}:\n"
        "    path: {binary}\n"
        "    pdb: {pdb}\n".format(
            project=args.project,
            target=args.target,
            binary=binary,
            pdb=pdb,
        )
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
