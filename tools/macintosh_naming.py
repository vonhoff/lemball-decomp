#!/usr/bin/env python3
"""Query and validate Macintosh names, classes, and physical Windows owners."""

from __future__ import annotations

import argparse
import csv
import pathlib
import re
import sys
from collections import Counter, defaultdict

ROOT = pathlib.Path(__file__).resolve().parents[1]
SYMBOLS = ROOT / "data/macintosh-68k-symbols.csv"
CORRELATIONS = ROOT / "data/macintosh-x86-correlations.csv"
COVERAGE = ROOT / "data/macintosh-symbol-coverage.csv"
OBJDIFF = ROOT / "data/objdiff-functions.csv"
CLASS_PREFIX = re.compile(r"__(\d+)")
VALID_COVERAGE_CATEGORIES = {
    "accepted_x86_correlation",
    "present_spelling_or_source_analogue",
    "likely_inlined_or_merged",
    "platform_specific",
    "genuinely_missing_or_unresolved",
}


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


def objdiff_name(symbol: str) -> str:
    match = re.match(r"^(.*?)__(\d+)([A-Za-z_][A-Za-z0-9_]*)F", symbol)
    if match:
        function, length, owner = match.groups()
        owner = owner[: int(length)]
        if function == "__ct":
            function = owner
        elif function == "__dt":
            function = f"~{owner}"
        return f"{owner}::{function}"
    match = re.match(r"^(.*?)__F", symbol)
    if match:
        return match.group(1)
    if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", symbol):
        return symbol
    raise ValueError(f"unsupported Macintosh function name: {symbol}")


def rows(path: pathlib.Path) -> list[dict[str, str]]:
    with path.open(encoding="utf-8-sig", newline="") as stream:
        return list(csv.DictReader(stream))


def load() -> tuple[list[dict[str, str]], list[dict[str, str]]]:
    return rows(SYMBOLS), rows(CORRELATIONS)


def module_name(code_file: str) -> str:
    return re.sub(r"^CODE_[0-9]+_", "", code_file).removesuffix(".bin")


def correlation_key(row: dict[str, str]) -> tuple[str, str]:
    return module_name(row["mac_code_file"]), row["mac_mangled_name"]


def symbol_key(row: dict[str, str]) -> tuple[str, str]:
    return module_name(row["code_file"]), row["mangled_name"]


def check(symbols: list[dict[str, str]], correlations: list[dict[str, str]]) -> list[str]:
    errors: list[str] = []
    if mac_class("__ct__11CEnemyGroupFP3CAIP14CObjectManager") != "CEnemyGroup":
        errors.append("MacsBug class parser failed constructor-with-parameters self-check")

    symbol_keys = {row["mangled_name"] for row in symbols}
    symbol_classes = {mac_class(row["mangled_name"]) for row in symbols if mac_class(row["mangled_name"])}
    seen_addresses: dict[int, int] = {}
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


    objdiff_names = {row["address"].lower(): row["name"] for row in rows(OBJDIFF)}
    for row in correlations:
        actual = objdiff_names.get(row["x86_address"].lower())
        expected = objdiff_name(row["mac_mangled_name"])
        if actual is not None and actual != expected:
            errors.append(
                f"{OBJDIFF.relative_to(ROOT)}: {row['x86_address']} must use original readable name "
                f"{expected!r}, not {actual!r}"
            )

    coverage = rows(COVERAGE)
    raw_keys = {(row["code_file"], row["name_length_offset"].lower(), row["mangled_name"]) for row in symbols}
    coverage_keys = [
        (row["mac_code_file"], row["mac_name_length_offset"].lower(), row["mac_mangled_name"])
        for row in coverage
    ]
    if len(coverage_keys) != len(set(coverage_keys)):
        errors.append("Macintosh symbol coverage ledger contains duplicate raw keys")
    if set(coverage_keys) != raw_keys:
        errors.append("Macintosh symbol coverage ledger does not exactly cover the raw inventory")
    accepted = {correlation_key(row): row for row in correlations}
    for line, row in enumerate(coverage, 2):
        where = f"{COVERAGE.relative_to(ROOT)}:{line}"
        category = row["coverage_category"]
        if category not in VALID_COVERAGE_CATEGORIES:
            errors.append(f"{where}: invalid coverage category {category!r}")
            continue
        key = module_name(row["mac_code_file"]), row["mac_mangled_name"]
        correlation = accepted.get(key)
        if category == "accepted_x86_correlation":
            if correlation is None:
                errors.append(f"{where}: accepted disposition has no correlation")
            elif row["x86_address"].lower() != correlation["x86_address"].lower():
                errors.append(f"{where}: x86 address disagrees with accepted correlation")
        elif correlation is not None:
            errors.append(f"{where}: accepted correlation is not represented in coverage ledger")

    return errors


def correlation_integrity(symbols: list[dict[str, str]], correlations: list[dict[str, str]]) -> Counter:
    raw_exact = {
        (module_name(row["code_file"]), row["name_length_offset"], row["mangled_name"])
        for row in symbols
    }
    raw_names = {(module_name(row["code_file"]), row["mangled_name"]) for row in symbols}
    result: Counter = Counter()
    for row in correlations:
        exact = (
            module_name(row["mac_code_file"]),
            row["mac_name_length_offset"],
            row["mac_mangled_name"],
        )
        named = (module_name(row["mac_code_file"]), row["mac_mangled_name"])
        result[
            "exact tuple"
            if exact in raw_exact
            else "name/module with different offset"
            if named in raw_names
            else "no exact raw name/module"
        ] += 1
    return result


def summary(symbols: list[dict[str, str]], correlations: list[dict[str, str]]) -> None:
    mapped_keys = {correlation_key(row) for row in correlations}
    mapped_raw = sum(symbol_key(row) in mapped_keys for row in symbols)
    coverage = rows(COVERAGE)
    target = [
        row for row in coverage
        if module_name(row["mac_code_file"]) != "Visos (Mac Specific)"
        and row["coverage_category"] != "platform_specific"
    ]
    dispositions = Counter(row["coverage_category"] for row in target)
    integrity = correlation_integrity(symbols, correlations)
    print(f"Macintosh symbols: {len(symbols)}")
    print(f"Accepted x86 mappings: {len(correlations)}")
    print(
        "Correlation raw identity: "
        + ", ".join(f"{key}={value}" for key, value in integrity.items())
    )
    print(f"Raw symbols with accepted spelling: {mapped_raw}")
    print(f"Raw symbols without accepted spelling: {len(symbols) - mapped_raw}")
    print(f"Non-Mac-specific target: {len(target)}")
    print(f"  accepted: {dispositions['accepted_x86_correlation']}")
    print(f"  lexical/inlined candidates: {dispositions['present_spelling_or_source_analogue'] + dispositions['likely_inlined_or_merged']}")
    print(f"  unresolved: {dispositions['genuinely_missing_or_unresolved']}")
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


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command")
    sub.add_parser("check", help="validate correlations and coverage")
    sub.add_parser("summary", help="show tracker totals")
    names_parser = sub.add_parser("names", help="list raw Macintosh names and mapping status")
    names_parser.add_argument("--class", dest="class_name")
    names_parser.add_argument("--status", choices=("all", "mapped", "unmapped"), default="unmapped")
    args = parser.parse_args()
    symbols, correlations = load()
    if args.command == "check":
        errors = check(symbols, correlations)
        if errors:
            for error in errors:
                print(f"error: {error}", file=sys.stderr)
            return 1
        print(f"Macintosh naming tracker: {len(symbols)} symbols, {len(correlations)} mappings")
        return 0
    if args.command in (None, "summary"):
        summary(symbols, correlations)
        return 0
    if args.command == "names":
        list_names(args.class_name, args.status, symbols, correlations)
        return 0
    raise AssertionError(args.command)


if __name__ == "__main__":
    raise SystemExit(main())
