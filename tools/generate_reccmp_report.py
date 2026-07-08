#!/usr/bin/env python3
# /// script
# requires-python = ">=3.11"
# dependencies = ["reccmp"]
# ///
"""Generate the strict objdiff-format codegen report for LEMBALL."""

from __future__ import annotations

import json
import struct
from pathlib import Path

from reccmp.compare.core import Compare
from reccmp.compare.report import ReccmpStatusReport
from reccmp.project.detect import RecCmpProject
from reccmp.types import EntityType


TARGET_NAME = "LEMBALL"
INCLUDED_CATEGORIES = {"internal", "thunk"}
MANIFEST_PATH = Path("data/manifest.json")
RECOMPILED_BINARY_PATH = Path("build-msvc420/LEMBALL.EXE")
RECOMPILED_PDB_PATH = Path("build-msvc420/LEMBALL.pdb")
CODEGEN_REPORT_PATH = Path("build/report.json")
TEXT_VIRTUAL_ADDRESS = 0x401000


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
        "total_units": 0,
        "complete_units": 0,
    }


def add_measures(total: dict[str, float | int], unit: dict[str, float | int]) -> None:
    total["fuzzy_match_percent"] += unit["fuzzy_match_percent"] * unit["total_code"]
    total["total_code"] += unit["total_code"]
    total["matched_code"] += unit["matched_code"]
    total["total_data"] += unit["total_data"]
    total["matched_data"] += unit["matched_data"]
    total["total_functions"] += unit["total_functions"]
    total["matched_functions"] += unit["matched_functions"]
    total["complete_code"] += unit["complete_code"]
    total["complete_data"] += unit["complete_data"]
    total["total_units"] += unit["total_units"]
    total["complete_units"] += unit["complete_units"]


def calc_percents(measures: dict[str, float | int]) -> None:
    total_code = int(measures["total_code"])
    total_data = int(measures["total_data"])
    total_functions = int(measures["total_functions"])
    measures["matched_code_percent"] = (
        float(measures["matched_code"]) / total_code * 100.0 if total_code else 100.0
    )
    measures["matched_data_percent"] = (
        float(measures["matched_data"]) / total_data * 100.0 if total_data else 100.0
    )
    measures["matched_functions_percent"] = (
        float(measures["matched_functions"]) / total_functions * 100.0
        if total_functions
        else 100.0
    )
    measures["complete_code_percent"] = (
        float(measures["complete_code"]) / total_code * 100.0 if total_code else 100.0
    )
    measures["complete_data_percent"] = (
        float(measures["complete_data"]) / total_data * 100.0 if total_data else 100.0
    )


def objdiff_u64(value: int | float) -> str:
    return str(int(value))


def objdiff_f32(value: int | float) -> float:
    packed = struct.pack("<f", float(value))
    rounded = struct.unpack("<f", packed)[0]
    for precision in range(1, 10):
        text = format(rounded, f".{precision}g")
        if struct.pack("<f", float(text)) == packed:
            return float(text)
    return float(format(rounded, ".9g"))


def objdiff_measures(measures: dict[str, float | int]) -> dict[str, object]:
    result: dict[str, object] = {}
    fields = (
        ("fuzzy_match_percent", objdiff_f32),
        ("total_code", objdiff_u64),
        ("matched_code", objdiff_u64),
        ("matched_code_percent", objdiff_f32),
        ("total_data", objdiff_u64),
        ("matched_data", objdiff_u64),
        ("matched_data_percent", objdiff_f32),
        ("total_functions", int),
        ("matched_functions", int),
        ("matched_functions_percent", objdiff_f32),
        ("complete_code", objdiff_u64),
        ("complete_code_percent", objdiff_f32),
        ("complete_data", objdiff_u64),
        ("complete_data_percent", objdiff_f32),
        ("total_units", int),
        ("complete_units", int),
    )
    for key, converter in fields:
        value = measures[key]
        if value != 0:
            result[key] = converter(value)
    return result


def objdiff_item(
    name: object,
    size: int,
    fuzzy_match_percent: float,
    virtual_address: int | None = None,
    address: int | None = None,
) -> dict[str, object]:
    item: dict[str, object] = {"name": str(name)}
    if size != 0:
        item["size"] = objdiff_u64(size)
    if fuzzy_match_percent != 0.0:
        item["fuzzy_match_percent"] = objdiff_f32(fuzzy_match_percent)
    metadata: dict[str, object] = {}
    if virtual_address is not None:
        metadata["virtual_address"] = objdiff_u64(virtual_address)
    item["metadata"] = metadata
    if address is not None:
        item["address"] = objdiff_u64(address)
    return item


def unmapped_unit_name(function: dict[str, object]) -> str:
    address = str(function["address"])
    return f"UNMAPPED/{function['category'].upper()}/{address[:4]}"


def load_compare_by_address(target_name: str) -> dict[int, dict[str, object]]:
    if not RECOMPILED_BINARY_PATH.exists() or not RECOMPILED_PDB_PATH.exists():
        raise SystemExit(
            "strict codegen report requires the MSVC rebuilt executable and PDB.\n"
            f"Expected {RECOMPILED_BINARY_PATH} and {RECOMPILED_PDB_PATH}.\n"
            "No decomp.dev report is generated without strict codegen inputs."
        )

    project = RecCmpProject.from_directory(Path("."))
    target = project.get(target_name)
    compare = Compare.from_target(target)
    report = ReccmpStatusReport(filename=target.original_path.name)

    by_address: dict[int, dict[str, object]] = {}
    for diff in compare.compare_all():
        if (
            diff.match_type == EntityType.FUNCTION
            and diff.name in target.report_config.ignore_functions
        ):
            continue

        report.add_match(diff)
        match = compare._db.get_one_match(diff.orig_addr)

    for entity in report.entities.values():
        if entity.type != EntityType.FUNCTION:
            continue

        orig_addr = int(entity.orig_addr, 16)
        match = compare._db.get_one_match(orig_addr)
        by_address[orig_addr] = {
            "name": entity.name,
            "ratio": entity.effective_accuracy,
            "stub": entity.is_stub,
            "size": 0 if match is None else match.any_size(),
        }
    return by_address


def build_report() -> dict[str, object]:
    if not MANIFEST_PATH.exists():
        raise SystemExit("data/manifest.json is missing. Run make ghidra-functions first.")

    inventory = json.loads(MANIFEST_PATH.read_text())
    compare_by_address = load_compare_by_address(TARGET_NAME)

    units: dict[str, list[dict[str, object]]] = {}
    for function in inventory["functions"]:
        category = function["category"]
        if category not in INCLUDED_CATEGORIES:
            continue

        address = int(function["address"], 16)
        unit_name = unmapped_unit_name(function)
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
            int(function["size"])
            for function in functions
            if float(function["ratio"]) == 1.0
        )
        matched_functions = sum(1 for function in functions if float(function["ratio"]) == 1.0)
        fuzzy_weight = sum(
            float(function["ratio"]) * 100.0 * int(function["size"]) for function in functions
        )
        fuzzy_match = fuzzy_weight / unit_code if unit_code else 0.0

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
            "complete_code": 0,
            "complete_code_percent": 0.0,
            "complete_data": 0,
            "complete_data_percent": 100.0,
            "total_units": 1,
            "complete_units": 0,
        }

        report_units.append(
            {
                "name": unit_name,
                "measures": objdiff_measures(unit_measures),
                "sections": [
                    objdiff_item(
                        ".text",
                        unit_code,
                        fuzzy_match,
                        virtual_address=TEXT_VIRTUAL_ADDRESS,
                    )
                ],
                "functions": [
                    objdiff_item(
                        function["name"],
                        int(function["size"]),
                        float(function["ratio"]) * 100.0,
                        virtual_address=int(function["address"]),
                        address=int(function["address"]) - TEXT_VIRTUAL_ADDRESS,
                    )
                    for function in functions
                ],
                "metadata": {},
            }
        )

        add_measures(total_measures, unit_measures)

    total_measures["fuzzy_match_percent"] = (
        total_measures["fuzzy_match_percent"] / total_measures["total_code"]
        if total_measures["total_code"]
        else 100.0
    )
    calc_percents(total_measures)

    report: dict[str, object] = {"measures": objdiff_measures(total_measures)}
    if report_units:
        report["units"] = report_units
    report["version"] = 2
    return report


def main() -> int:
    report = build_report()
    output_path = CODEGEN_REPORT_PATH
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(json.dumps(report, indent=2) + "\n")
    measures = report["measures"]
    total_functions = int(measures.get("total_functions", 0))
    matched_functions = int(measures.get("matched_functions", 0))
    matched_functions_percent = float(measures.get("matched_functions_percent", 0.0))
    fuzzy_match_percent = float(measures.get("fuzzy_match_percent", 0.0))
    total_units = int(measures.get("total_units", 0))
    complete_units = int(measures.get("complete_units", 0))
    print(
        f"{total_functions} functions, "
        f"{matched_functions} matched "
        f"({matched_functions_percent:.1f}%), "
        f"fuzzy {fuzzy_match_percent:.1f}%, "
        f"{total_units} units "
        f"({complete_units} complete)"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
