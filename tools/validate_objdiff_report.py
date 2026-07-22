#!/usr/bin/env python3
"""Validate an objdiff v2 JSON report with pinned official tooling and local invariants."""

from __future__ import annotations

import argparse
import hashlib
import json
import struct
import subprocess
import urllib.request
from pathlib import Path


OBJDIFF_VERSION = "v3.7.3"
OBJDIFF_ASSET = "objdiff-cli-windows-x86_64.exe"
OBJDIFF_SHA256 = "933bc75fdc526ef7149534b74282efc3e02dc0f1010512accb5463e68f68e07f"
OBJDIFF_URL = (
    f"https://github.com/encounter/objdiff/releases/download/{OBJDIFF_VERSION}/"
    f"{OBJDIFF_ASSET}"
)
DEFAULT_CLI = Path("build") / f"objdiff-cli-{OBJDIFF_VERSION}-windows-x86_64.exe"


def f32(value: float) -> float:
    return struct.unpack("<f", struct.pack("<f", value))[0]


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def ensure_cli(path: Path) -> None:
    if not path.is_file():
        path.parent.mkdir(parents=True, exist_ok=True)
        temporary = path.with_suffix(path.suffix + ".download")
        try:
            urllib.request.urlretrieve(OBJDIFF_URL, temporary)
            if sha256(temporary) != OBJDIFF_SHA256:
                raise SystemExit("downloaded objdiff CLI checksum mismatch")
            temporary.replace(path)
        finally:
            temporary.unlink(missing_ok=True)
    actual = sha256(path)
    if actual != OBJDIFF_SHA256:
        raise SystemExit(
            f"objdiff CLI checksum mismatch: expected {OBJDIFF_SHA256}, got {actual}"
        )


def as_int(value: object, field: str) -> int:
    try:
        result = int(str(value), 10)
    except ValueError as error:
        raise SystemExit(f"{field} is not an objdiff uint64 string") from error
    if result < 0:
        raise SystemExit(f"{field} is negative")
    return result


def validate_measures(measures: dict[str, object], functions: list[dict[str, object]]) -> None:
    forbidden = {"matched_data_percent", "complete_data_percent", "complete_units"}
    present = forbidden.intersection(measures)
    if present:
        raise SystemExit(f"unsupported/inferred measures present: {sorted(present)}")

    sizes = [as_int(function["size"], "function.size") for function in functions]
    ratios = [float(function.get("fuzzy_match_percent", 0.0)) for function in functions]
    total_code = sum(sizes)
    matched_code = sum(size for size, ratio in zip(sizes, ratios) if ratio == 100.0)
    matched_functions = sum(ratio == 100.0 for ratio in ratios)
    expected = {
        "total_code": total_code,
        "matched_code": matched_code,
        "total_functions": len(functions),
        "matched_functions": matched_functions,
    }
    for field, value in expected.items():
        if field.endswith("code"):
            actual = as_int(measures.get(field), field)
        else:
            actual = int(measures.get(field, -1))
        if actual != value:
            raise SystemExit(f"{field} mismatch: expected {value}, got {actual}")

    percentages = {
        "matched_code_percent": matched_code / total_code * 100.0 if total_code else 0.0,
        "matched_functions_percent": matched_functions / len(functions) * 100.0 if functions else 0.0,
        "fuzzy_match_percent": (
            sum(ratio * size for ratio, size in zip(ratios, sizes)) / total_code
            if total_code
            else 0.0
        ),
    }
    for field, value in percentages.items():
        actual = float(measures.get(field, 0.0))
        if f32(actual) != f32(value):
            raise SystemExit(f"{field} mismatch: expected float32 {f32(value)}, got {actual}")


def validate_report(path: Path, cli: Path) -> None:
    report = json.loads(path.read_text(encoding="utf-8"))
    if report.get("version") != 2 or not isinstance(report.get("units"), list):
        raise SystemExit("report is not objdiff report version 2")
    all_functions: list[dict[str, object]] = []
    for unit in report["units"]:
        functions = unit.get("functions", [])
        if not isinstance(functions, list):
            raise SystemExit("unit.functions is not a list")
        for function in functions:
            as_int(function["address"], "function.address")
            as_int(function["metadata"]["virtual_address"], "metadata.virtual_address")
        validate_measures(unit["measures"], functions)
        all_functions.extend(functions)
    validate_measures(report["measures"], all_functions)
    if int(report["measures"].get("total_units", -1)) != len(report["units"]):
        raise SystemExit("total_units does not equal the number of units")

    result = subprocess.run(
        [str(cli), "--no-color", "report", "changes", str(path), str(path)],
        text=True,
        capture_output=True,
        check=False,
    )
    if result.returncode != 0:
        raise SystemExit(f"official objdiff parser rejected report:\n{result.stderr}")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("report", type=Path)
    parser.add_argument("--objdiff-cli", type=Path, default=DEFAULT_CLI)
    args = parser.parse_args()
    ensure_cli(args.objdiff_cli)
    validate_report(args.report, args.objdiff_cli)
    print(f"objdiff {OBJDIFF_VERSION} accepted {args.report}; semantic totals are consistent")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
