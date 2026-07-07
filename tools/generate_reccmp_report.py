#!/usr/bin/env python3
# /// script
# requires-python = ">=3.11"
# dependencies = ["reccmp"]
# ///
"""Generate an objdiff-format progress report from reccmp results."""

from __future__ import annotations

import argparse
import json
from pathlib import Path

from reccmp.compare.core import Compare
from reccmp.parser.marker import MarkerType, match_marker
from reccmp.project.detect import RecCmpProject


TARGET_NAME = "LEMBALL"
INCLUDED_CATEGORIES = {"internal", "thunk"}
MANIFEST_PATH = Path("data/manifest.json")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("-o", "--output", default="report.json")
    return parser.parse_args()


def parse_annotations(source_root: Path) -> dict[int, str]:
    addr_to_source: dict[int, str] = {}
    for path in sorted(source_root.rglob("*.cpp")):
        with open(path) as f:
            for line in f:
                marker = match_marker(line)
                if marker and marker.module == TARGET_NAME and marker.type == MarkerType.FUNCTION:
                    addr_to_source[marker.offset] = path.as_posix()
    return addr_to_source


def empty_measures() -> dict[str, float | int]:
    return {
        "fuzzy_match_percent": 0.0,
        "total_code": 0,
        "matched_code": 0,
        "total_data": 0,
        "matched_data": 0,
        "total_functions": 0,
        "matched_functions": 0,
        "complete_code": 0,
        "complete_data": 0,
        "complete_units": 0,
    }


def add_measures(total: dict[str, float | int], unit: dict[str, float | int]) -> None:
    total["fuzzy_match_percent"] += unit["fuzzy_match_percent"] * unit["total_code"]
    total["total_code"] += unit["total_code"]
    total["matched_code"] += unit["matched_code"]
    total["total_functions"] += unit["total_functions"]
    total["matched_functions"] += unit["matched_functions"]
    total["complete_code"] += unit["complete_code"]
    total["complete_units"] += unit["complete_units"]


def calc_percents(measures: dict[str, float | int], total_units: int) -> None:
    total_code = int(measures["total_code"])
    total_functions = int(measures["total_functions"])
    measures["matched_code_percent"] = (
        float(measures["matched_code"]) / total_code * 100.0 if total_code else 0.0
    )
    measures["matched_data_percent"] = 100.0
    measures["matched_functions_percent"] = (
        float(measures["matched_functions"]) / total_functions * 100.0
        if total_functions
        else 0.0
    )
    measures["complete_code_percent"] = (
        float(measures["complete_code"]) / total_code * 100.0 if total_code else 0.0
    )
    measures["complete_data_percent"] = 100.0
    measures["total_units"] = total_units
    measures["total_data"] = 0
    measures["matched_data"] = 0


def unmapped_unit_name(function: dict[str, object]) -> str:
    address = str(function["address"])
    return f"UNMAPPED/{function['category'].upper()}/{address[:4]}"


def load_compare_by_address(target_name: str) -> dict[int, dict[str, object]]:
    project = RecCmpProject.from_directory(Path("."))
    target = project.get(target_name)
    compare = Compare.from_target(target)

    by_address: dict[int, dict[str, object]] = {}
    for diff in compare.compare_all():
        match = compare._db.get_one_match(diff.orig_addr)
        by_address[diff.orig_addr] = {
            "name": diff.name,
            "ratio": diff.effective_ratio,
            "stub": diff.is_stub,
            "size": 0 if match is None else match.any_size(),
        }
    return by_address


def build_report() -> dict[str, object]:
    if not MANIFEST_PATH.exists():
        raise SystemExit("data/manifest.json is missing. Run make ghidra-functions first.")

    inventory = json.loads(MANIFEST_PATH.read_text())
    addr_to_source = parse_annotations(Path("src"))
    compare_by_address = load_compare_by_address(TARGET_NAME)

    units: dict[str, list[dict[str, object]]] = {}
    for function in inventory["functions"]:
        category = function["category"]
        if category not in INCLUDED_CATEGORIES:
            continue

        address = int(function["address"], 16)
        source_path = addr_to_source.get(address)
        unit_name = source_path if source_path is not None else unmapped_unit_name(function)
        diff = compare_by_address.get(address)
        ratio = 0.0 if diff is None else float(diff["ratio"])
        is_stub = False if diff is None else bool(diff["stub"])
        size = int(function["size"])
        if diff is not None and int(diff["size"]) > 0:
            size = int(diff["size"])

        units.setdefault(unit_name, []).append(
            {
                "address": address,
                "name": function["name"] if diff is None else str(diff["name"]),
                "size": size,
                "ratio": ratio,
                "stub": is_stub,
                "source_path": source_path,
                "category": category,
            }
        )

    report_units = []
    total_measures = empty_measures()

    for unit_name in sorted(units):
        functions = [function for function in units[unit_name] if not function["stub"]]
        if not functions:
            continue

        unit_code = sum(int(function["size"]) for function in functions)
        matched_code = sum(
            int(function["size"]) for function in functions if float(function["ratio"]) == 1.0
        )
        matched_functions = sum(1 for function in functions if float(function["ratio"]) == 1.0)
        fuzzy_weight = sum(
            float(function["ratio"]) * 100.0 * int(function["size"]) for function in functions
        )
        fuzzy_match = fuzzy_weight / unit_code if unit_code else 0.0
        complete = bool(functions) and all(float(function["ratio"]) == 1.0 for function in functions)

        unit_measures = {
            "fuzzy_match_percent": fuzzy_match,
            "total_code": unit_code,
            "matched_code": matched_code,
            "matched_code_percent": matched_code / unit_code * 100.0 if unit_code else 0.0,
            "total_data": 0,
            "matched_data": 0,
            "matched_data_percent": 100.0,
            "total_functions": len(functions),
            "matched_functions": matched_functions,
            "matched_functions_percent": matched_functions / len(functions) * 100.0,
            "complete_code": unit_code if complete else 0,
            "complete_code_percent": 100.0 if complete else 0.0,
            "complete_data": 0,
            "complete_data_percent": 100.0,
            "total_units": 1,
            "complete_units": 1 if complete else 0,
        }

        unit_metadata = {"complete": complete}
        if functions[0]["source_path"] is not None:
            unit_metadata["source_path"] = functions[0]["source_path"]

        report_units.append(
            {
                "name": unit_name,
                "measures": unit_measures,
                "sections": [
                    {
                        "name": ".text",
                        "size": unit_code,
                        "fuzzy_match_percent": fuzzy_match,
                    }
                ],
                "functions": [
                    {
                        "name": function["name"],
                        "size": int(function["size"]),
                        "fuzzy_match_percent": float(function["ratio"]) * 100.0,
                        "metadata": {
                            "virtual_address": int(function["address"]),
                        },
                    }
                    for function in functions
                ],
                "metadata": unit_metadata,
            }
        )

        add_measures(total_measures, unit_measures)

    total_measures["fuzzy_match_percent"] = (
        total_measures["fuzzy_match_percent"] / total_measures["total_code"]
        if total_measures["total_code"]
        else 0.0
    )
    calc_percents(total_measures, len(report_units))

    return {"version": 2, "measures": total_measures, "units": report_units, "categories": []}


def main() -> int:
    args = parse_args()
    report = build_report()
    output_path = Path(args.output)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(json.dumps(report, indent=2) + "\n")
    measures = report["measures"]
    print(
        f"{measures['total_functions']} functions, "
        f"{measures['matched_functions']} matched "
        f"({measures['matched_functions_percent']:.1f}%), "
        f"fuzzy {measures['fuzzy_match_percent']:.1f}%, "
        f"{measures['total_units']} units "
        f"({measures['complete_units']} complete)"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
