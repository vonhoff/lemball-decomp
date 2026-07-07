#!/usr/bin/env python3

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path

FUNCTION_RE = re.compile(r"^(?:[A-Za-z_~][\w:\s\*&<>]*\s+)?(?:[\*&]\s*)*([A-Za-z_~][\w:]*)\s*\([^;{}]*\)\s*$")
MARKER_RE = re.compile(r"^\s*//\s*FUNCTION:\s*(\w+)\s+(0x[0-9A-Fa-f]+)\s*$")
MANIFEST_PATH = Path("data/manifest.json")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate candidate reccmp annotations from the Ghidra function inventory."
    )
    parser.add_argument("--source-root", default="src")
    parser.add_argument("--output", default="build/annotation-candidates.json")
    return parser.parse_args()


def normalize_name(name: str) -> str:
    return re.sub(r"[^a-z0-9]", "", name.lower())


def build_inventory_index() -> dict[str, list[dict[str, object]]]:
    if not MANIFEST_PATH.exists():
        raise SystemExit("data/manifest.json is missing. Run make ghidra-functions first.")
    inventory = json.loads(MANIFEST_PATH.read_text())
    index: dict[str, list[dict[str, object]]] = {}
    for function in inventory["functions"]:
        key = normalize_name(str(function["name"]))
        index.setdefault(key, []).append(function)
    return index


def source_functions(source_root: Path) -> list[dict[str, object]]:
    out: list[dict[str, object]] = []
    for path in sorted(path for path in source_root.rglob("*") if path.suffix.lower() == ".cpp"):
        lines = path.read_text().splitlines()
        for idx, line in enumerate(lines, 1):
            if line[:1].isspace():
                continue
            marker = lines[idx - 2] if idx > 1 else ""
            signature = line.strip()
            if "(" not in signature:
                continue
            scan_idx = idx
            while "{" not in signature and ";" not in signature and scan_idx < len(lines):
                scan_idx += 1
                signature += " " + lines[scan_idx - 1].strip()
            if "{" not in signature:
                continue
            signature = signature.split("{", 1)[0].strip()
            match = FUNCTION_RE.match(signature)
            if not match:
                continue
            full_name = match.group(1)
            out.append(
                {
                    "path": str(path).replace("\\", "/"),
                    "line": idx,
                    "full_name": full_name,
                    "short_name": full_name.split("::")[-1],
                    "annotated": MARKER_RE.match(marker) is not None,
                }
            )
    return out


def score_candidate(source_name: str, candidate: dict[str, object]) -> tuple[int, int]:
    score = 0
    if not bool(candidate["is_thunk"]):
        score += 10
    if str(candidate["category"]) == "internal":
        score += 5
    if normalize_name(source_name) == normalize_name(str(candidate["name"])):
        score += 20
    return score, int(candidate.get("size", 0))


def main() -> int:
    args = parse_args()
    index = build_inventory_index()
    functions = source_functions(Path(args.source_root))

    candidates = []
    for function in functions:
        key = normalize_name(str(function["short_name"]))
        matches = index.get(key, [])
        ranked = sorted(
            matches,
            key=lambda candidate: score_candidate(str(function["short_name"]), candidate),
            reverse=True,
        )
        candidates.append(
            {
                **function,
                "candidates": [
                    {
                        "address": candidate["address"],
                        "name": candidate["name"],
                        "size": candidate.get("size", 0),
                        "category": candidate["category"],
                        "is_thunk": candidate["is_thunk"],
                    }
                    for candidate in ranked[:8]
                ],
            }
        )

    output_path = Path(args.output)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(json.dumps({"version": "LEMBALL", "functions": candidates}, indent=2) + "\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
