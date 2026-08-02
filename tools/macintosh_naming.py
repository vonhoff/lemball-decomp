#!/usr/bin/env python3
"""Query and validate Macintosh names, classes, and physical Windows owners."""

from __future__ import annotations

import argparse
import csv
import json
import pathlib
import re
import sys
from collections import Counter, defaultdict

ROOT = pathlib.Path(__file__).resolve().parents[1]
SYMBOLS = ROOT / "data/macintosh-68k-symbols.csv"
CORRELATIONS = ROOT / "data/macintosh-x86-correlations.csv"
STRUCTURE = ROOT / "data/macintosh-structure.json"
CLASS_PREFIX = re.compile(r"__(\d+)")
VALID_CLASS_STATES = {"planned", "partial", "mapped", "blocked"}
VALID_FILE_STATES = {"planned", "existing", "retained"}


def mac_class(symbol: str) -> str:
    match = CLASS_PREFIX.search(symbol)
    if match is None:
        return ""
    start = match.end()
    length = int(match.group(1))
    name = symbol[start : start + length]
    if len(name) == length and symbol[start + length : start + length + 1] == "F":
        return name
    return ""


def rows(path: pathlib.Path) -> list[dict[str, str]]:
    with path.open(encoding="utf-8-sig", newline="") as stream:
        return list(csv.DictReader(stream))


def load() -> tuple[dict, list[dict[str, str]], list[dict[str, str]]]:
    return json.loads(STRUCTURE.read_text(encoding="utf-8")), rows(SYMBOLS), rows(CORRELATIONS)


def module_name(code_file: str) -> str:
    return re.sub(r"^CODE_[0-9]+_", "", code_file).removesuffix(".bin")


def correlation_key(row: dict[str, str]) -> tuple[str, str]:
    return module_name(row["mac_code_file"]), row["mac_mangled_name"]


def symbol_key(row: dict[str, str]) -> tuple[str, str]:
    return module_name(row["code_file"]), row["mangled_name"]


def check(data: dict, symbols: list[dict[str, str]], correlations: list[dict[str, str]]) -> list[str]:
    errors: list[str] = []
    if data.get("version") != 1:
        errors.append("structure version must be 1")
    if mac_class("__ct__11CEnemyGroupFP3CAIP14CObjectManager") != "CEnemyGroup":
        errors.append("MacsBug class parser failed constructor-with-parameters self-check")

    classes = data.get("classes", [])
    files = data.get("files", [])
    class_names = [item.get("name", "") for item in classes]
    duplicates = [name for name, count in Counter(class_names).items() if count > 1]
    if duplicates:
        errors.append(f"duplicate classes: {', '.join(sorted(duplicates))}")

    for item in classes:
        name = item.get("name", "")
        where = f"class {name or '<unnamed>'}"
        if not name or not item.get("module"):
            errors.append(f"{where}: name and module are required")
        if item.get("state") not in VALID_CLASS_STATES:
            errors.append(f"{where}: invalid state {item.get('state')!r}")
        if not item.get("files") or not item.get("must_contain") or not item.get("must_not_contain"):
            errors.append(f"{where}: files, must_contain, and must_not_contain are required")
        for owner in item.get("files", []):
            if not (ROOT / owner).exists():
                errors.append(f"{where}: missing owner {owner}")

    file_paths = [item.get("path", "") for item in files]
    duplicates = [path for path, count in Counter(file_paths).items() if count > 1]
    if duplicates:
        errors.append(f"duplicate files: {', '.join(sorted(duplicates))}")
    for item in files:
        path = item.get("path", "")
        where = f"file {path or '<unnamed>'}"
        if item.get("state") not in VALID_FILE_STATES:
            errors.append(f"{where}: invalid state {item.get('state')!r}")
        if not item.get("module") or not item.get("must_contain") or not item.get("must_not_contain"):
            errors.append(f"{where}: module, must_contain, and must_not_contain are required")
        if item.get("state") != "planned" and not (ROOT / path).exists():
            errors.append(f"{where}: path does not exist")
        if item.get("state") == "planned" and pathlib.PurePosixPath(path).suffix:
            stem = pathlib.PurePosixPath(path).stem
            if len(stem) > 12:
                errors.append(f"{where}: planned filename stem exceeds 12 characters")
        unknown = sorted(set(item.get("classes", [])) - set(class_names))
        if unknown:
            errors.append(f"{where}: unknown classes {', '.join(unknown)}")

    symbol_keys = {row["mangled_name"] for row in symbols}
    symbol_classes = {mac_class(row["mangled_name"]) for row in symbols if mac_class(row["mangled_name"])}
    seen_addresses: dict[int, int] = {}
    correlated_classes: set[str] = set()
    for line, row in enumerate(correlations, 2):
        where = f"{CORRELATIONS.relative_to(ROOT)}:{line}"
        try:
            address = int(row["x86_address"], 16)
        except ValueError:
            errors.append(f"{where}: invalid x86 address {row['x86_address']!r}")
            continue
        if address in seen_addresses:
            errors.append(f"{where}: duplicate x86 address (first at line {seen_addresses[address]})")
        seen_addresses[address] = line
        name = mac_class(row["mac_mangled_name"])
        if row["mac_mangled_name"] not in symbol_keys and (not name or name not in symbol_classes):
            errors.append(f"{where}: Macintosh symbol family is absent from raw inventory")
        if name:
            correlated_classes.add(name)
    undeclared = sorted(correlated_classes - set(class_names))
    if undeclared:
        errors.append(f"correlated classes absent from structure manifest: {', '.join(undeclared)}")
    return errors


def print_constraints(label: str, values: list[str]) -> None:
    print(f"{label}:")
    for value in values:
        print(f"  - {value}")


def summary(data: dict, symbols: list[dict[str, str]], correlations: list[dict[str, str]]) -> None:
    states = Counter(item["state"] for item in data["classes"])
    mapped_keys = {correlation_key(row) for row in correlations}
    mapped_raw = sum(symbol_key(row) in mapped_keys for row in symbols)
    print(f"Macintosh symbols: {len(symbols)}")
    print(f"Accepted x86 mappings: {len(correlations)}")
    print(f"Raw symbols with accepted spelling: {mapped_raw}")
    print(f"Raw symbols without accepted spelling: {len(symbols) - mapped_raw}")
    print("Tracked classes: " + ", ".join(f"{key}={value}" for key, value in sorted(states.items())))
    print(f"Tracked physical files: {len(data['files'])}")


def show_class(name: str, data: dict, symbols: list[dict[str, str]], correlations: list[dict[str, str]]) -> int:
    item = next((item for item in data["classes"] if item["name"].lower() == name.lower()), None)
    if item is None:
        print(f"unknown tracked class: {name}", file=sys.stderr)
        return 1
    exact = item["name"]
    class_symbols = [row for row in symbols if mac_class(row["mangled_name"]) == exact]
    mapped = [row for row in correlations if mac_class(row["mac_mangled_name"]) == exact]
    print(f"{exact}: module={item['module']} state={item['state']} mapped={len(mapped)}/{len(class_symbols)}")
    print_constraints("files", item["files"])
    print_constraints("must contain", item["must_contain"])
    print_constraints("must not contain", item["must_not_contain"])
    if mapped:
        print("accepted mappings:")
        for row in mapped:
            print(f"  {row['x86_address']} {row['mac_mangled_name']} [{row['confidence']}]")
    return 0


def show_file(path: str, data: dict) -> int:
    normalized = path.replace("\\", "/")
    item = next((item for item in data["files"] if item["path"].lower() == normalized.lower()), None)
    if item is None:
        owners = [item for item in data["classes"] if normalized in item["files"]]
        if not owners:
            print(f"unknown tracked file: {path}", file=sys.stderr)
            return 1
        print(f"{normalized}: class-owned")
        print_constraints("classes", [item["name"] for item in owners])
        return 0
    print(f"{item['path']}: module={item['module']} state={item['state']}")
    if item.get("classes"):
        print_constraints("classes", item["classes"])
    print_constraints("must contain", item["must_contain"])
    print_constraints("must not contain", item["must_not_contain"])
    return 0


def list_names(class_name: str | None, status: str, symbols: list[dict[str, str]], correlations: list[dict[str, str]]) -> None:
    mapped = {correlation_key(row): row for row in correlations}
    for row in symbols:
        cls = mac_class(row["mangled_name"])
        if class_name and cls.lower() != class_name.lower():
            continue
        accepted = mapped.get(symbol_key(row))
        if status == "mapped" and accepted is None:
            continue
        if status == "unmapped" and accepted is not None:
            continue
        suffix = f" -> {accepted['x86_address']}" if accepted else ""
        print(f"{row['code_file']} {row['name_length_offset']} {row['mangled_name']}{suffix}")


def todo(data: dict, symbols: list[dict[str, str]], correlations: list[dict[str, str]]) -> None:
    totals = Counter(mac_class(row["mangled_name"]) for row in symbols)
    mapped = Counter(mac_class(row["mac_mangled_name"]) for row in correlations)
    for item in data["classes"]:
        if item["state"] == "mapped":
            continue
        name = item["name"]
        print(f"{item['state']:7} {name:24} mappings={mapped[name]}/{totals[name]} module={item['module']}")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command")
    sub.add_parser("check", help="validate correlations and structure constraints")
    sub.add_parser("summary", help="show tracker totals")
    sub.add_parser("todo", help="show partial, planned, and blocked classes")
    class_parser = sub.add_parser("class", help="show one class contract and mappings")
    class_parser.add_argument("name")
    file_parser = sub.add_parser("file", help="show one physical file contract")
    file_parser.add_argument("path")
    names_parser = sub.add_parser("names", help="list raw Macintosh names and mapping status")
    names_parser.add_argument("--class", dest="class_name")
    names_parser.add_argument("--status", choices=("all", "mapped", "unmapped"), default="unmapped")
    args = parser.parse_args()
    data, symbols, correlations = load()
    if args.command == "check":
        errors = check(data, symbols, correlations)
        if errors:
            for error in errors:
                print(f"error: {error}", file=sys.stderr)
            return 1
        print(f"Macintosh naming tracker: {len(symbols)} symbols, {len(correlations)} mappings, {len(data['classes'])} class contracts, {len(data['files'])} file contracts")
        return 0
    if args.command in (None, "summary"):
        summary(data, symbols, correlations)
        return 0
    if args.command == "todo":
        todo(data, symbols, correlations)
        return 0
    if args.command == "class":
        return show_class(args.name, data, symbols, correlations)
    if args.command == "file":
        return show_file(args.path, data)
    if args.command == "names":
        list_names(args.class_name, args.status, symbols, correlations)
        return 0
    raise AssertionError(args.command)


if __name__ == "__main__":
    raise SystemExit(main())
