#!/usr/bin/env python3
"""Generate objdiff progress from raw assembly comparisons."""

import csv
import json
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

from reccmp.compare.report import serialize_reccmp_report
from reccmp.project.detect import DetectWhat, detect_project
from reccmp.types import EntityType

from lib import BUILD, RECCMP_JSON, REPORT_JSON, ROADMAP_CSV, ROOT, load_engine


def measures(functions, total_units=1):
    total_code = sum(int(f["size"]) for f in functions)
    matched = [f for f in functions if f["fuzzy_match_percent"] == 100]
    matched_code = sum(int(f["size"]) for f in matched)
    return {
        "total_units": total_units,
        "total_code": str(total_code),
        "matched_code": str(matched_code),
        "fuzzy_match_percent": sum(f["fuzzy_match_percent"] * int(f["size"])
                                   for f in functions) / total_code if total_code else 100.0,
        "matched_code_percent": matched_code / total_code * 100 if total_code else 100.0,
        "total_functions": len(functions),
        "matched_functions": len(matched),
        "matched_functions_percent": len(matched) / len(functions) * 100 if functions else 100.0,
    }


def build_report(roadmap_path, reccmp_path):
    data = json.loads(reccmp_path.read_text(encoding="utf-8"))["data"]
    matches = {int(m["address"], 16): m for m in data if m["type"] == EntityType.FUNCTION}
    groups = defaultdict(list)
    with roadmap_path.open(newline="", encoding="utf-8-sig") as stream:
        for row in csv.DictReader(stream):
            if row["row_type"] != "fun" or not row["orig_addr"]:
                continue
            size = int(row["size"] or "0", 16)
            if not size:
                continue
            address = int(row["orig_addr"], 16)
            match = matches.get(address, {})
            name = row["module"].removeprefix("CMakeFiles/LEMBALL.dir/src/").removesuffix(".obj")
            groups[name or "Compiler-generated"].append({
                "name": f"0x{address:08x}", "size": str(size),
                "metadata": {"virtual_address": str(address),
                             "demangled_name": match.get("name", row["name"])},
                "fuzzy_match_percent": 0.0 if match.get("stub") else match.get("matching", 0.0) * 100,
            })

    units = []
    for name, functions in sorted(groups.items()):
        functions.sort(key=lambda f: int(f["metadata"]["virtual_address"]))
        unit = {"name": name, "measures": measures(functions), "functions": functions}
        source = Path("src") / name
        if (ROOT / source).is_file():
            unit["metadata"] = {"source_path": source.as_posix()}
        units.append(unit)
    return {
        "version": 2, "units": units,
        "measures": measures([f for u in units for f in u["functions"]], len(units)),
    }


def main():
    detect_project(project_directory=ROOT, search_path=[ROOT / "data"],
                   detect_what=DetectWhat.ORIGINAL, build_directory=BUILD)
    target, engine = load_engine()
    comparisons = engine.to_report(filename=target.original_path.name)
    RECCMP_JSON.write_text(serialize_reccmp_report(comparisons, diff_included=True), encoding="utf-8")
    subprocess.run(
        [sys.executable, "-m", "reccmp.tools.roadmap", "--target", "LEMBALL", "--csv", str(ROADMAP_CSV)],
        cwd=BUILD, check=True,
    )
    report = build_report(ROADMAP_CSV, RECCMP_JSON)
    REPORT_JSON.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    values = report["measures"]
    print(f"Normalized-exact: {values['matched_functions']}/{values['total_functions']} functions, "
          f"{values['matched_code']}/{values['total_code']} bytes; "
          f"fuzzy: {values['fuzzy_match_percent']:.2f}%")


if __name__ == "__main__":
    main()
