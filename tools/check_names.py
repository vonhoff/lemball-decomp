#!/usr/bin/env python3
"""Compare source class/method names with adjacent // 68K Metrowerks symbols.

python tools/check_names.py [src/path ...] [--strict] [--json] [--fail]
python tools/check_names.py --research-root C:/Research
python tools/check_names.py --inventory path/to/68K.functions.tsv

The dropped class C prefix is allowed. Case-only differences are counted and
shown with --strict. Underscores are significant. This checks recorded symbol
comments, not ABI equivalence or the correctness of their x86 associations.
With --inventory or --research-root, the unnormalized inventory symbol is the
expected name; source comments are cross-checked separately. Scaffold naming
rules are not used. Unsupported symbols and comments without an adjacent declaration are reported.
No source is changed. --fail returns 1 for reported differences, 2 for gaps.
"""

import argparse
from collections import Counter
import csv
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
MARK = re.compile(r"//\s*68K\s+(0x[0-9a-fA-F]+)\s+(\S+)")
OPERATORS = {
    "__as": "operator=", "__ls": "operator<<", "__nw": "operatornew",
    "__dl": "operatordelete", "__apl": "operator+=", "__pl": "operator+",
    "__eq": "operator==", "__gt": "operator>", "__ml": "operator*",
}
FUNCTION = re.compile(
    r"(?:(?P<owner>[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*)\s*::\s*)?"
    r"(?P<method>operator\s*(?:new\b|delete\b|[^\w\s(]+)|~?[A-Za-z_]\w*)\s*\("
)


def decode_symbol(symbol):
    """Decode only a function's name and length-prefixed owner, never its ABI."""
    split = re.search(r"__(?=\d|Q\d|F)", symbol)
    if split is None:
        raise ValueError("unsupported symbol form")
    method, rest = symbol[:split.start()], symbol[split.end():]
    owners = []
    count = 1
    if rest.startswith("Q"):
        if len(rest) < 2 or not rest[1].isdigit():
            raise ValueError("unsupported qualified owner")
        count, rest = int(rest[1]), rest[2:]
    if rest.startswith("F"):
        count = 0
    for _ in range(count):
        length = re.match(r"\d+", rest)
        if not length:
            raise ValueError("missing owner length")
        size = int(length[0])
        rest = rest[len(length[0]):]
        if size == 0 or len(rest) < size:
            raise ValueError("invalid owner length")
        owners.append(rest[:size])
        rest = rest[size:]
    if not re.match(r"[CS]*F", rest):
        raise ValueError("unsupported function suffix after owner")
    if method == "__ct":
        method = "<constructor>"
    elif method == "__dt":
        method = "<destructor>"
    elif method.startswith("__"):
        if method not in OPERATORS:
            raise ValueError("unsupported operator " + method)
        method = OPERATORS[method]
    return "::".join(owners), method


def mask_comments_and_strings(text):
    pattern = r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\''
    return re.sub(pattern, lambda m: re.sub(r"[^\n]", " ", m[0]), text)


def class_ranges(code):
    ends, stack = {}, []
    for pos, char in enumerate(code):
        if char == "{":
            stack.append(pos)
        elif char == "}" and stack:
            ends[stack.pop()] = pos
    result = []
    for match in re.finditer(r"\b(?:class|struct)\s+(\w+)\s*(?:final\s*)?(?::[^;{}]*)?\{", code):
        opening = match.end() - 1
        if opening in ends:
            result.append((opening, ends[opening], match[1]))
    return result


def adjacent_name(code, offset, ranges):
    start = offset
    while start < len(code) and code[start].isspace():
        start += 1
    # Stop at the next declaration/body boundary. Never associate a comment
    # after a class with a function in the next class or function body.
    end = re.search(r"[;{}#]", code[start:])
    declaration = code[start:start + end.start()] if end else code[start:]
    match = FUNCTION.search(declaration)
    if not match:
        raise ValueError("no adjacent function declaration")
    owner = match["owner"]
    if not owner:
        enclosing = [r for r in ranges if r[0] < start < r[1]]
        owner = "::".join(r[2] for r in enclosing)
    method = re.sub(r"\s+", "", match["method"])
    if owner and method == owner.split("::")[-1]:
        method = "<constructor>"
    elif owner and method == "~" + owner.split("::")[-1]:
        method = "<destructor>"
    return owner, method


def class_name(name):
    # Only the established C class prefix is ignored. Preserve underscores.
    return "::".join(part[1:] if re.match(r"C[A-Z]", part) else part
                     for part in name.split("::"))


def scan(path, symbols=None):
    text = path.read_text(encoding="utf-8")
    code = mask_comments_and_strings(text)
    ranges = class_ranges(code)
    rows = []
    for mark in MARK.finditer(text):
        row = {"path": str(path), "line": text.count("\n", 0, mark.start()) + 1,
               "address_68k": mark[1], "symbol": mark[2]}
        try:
            inventory_symbol = None if symbols is None else symbols.get(int(mark[1], 16))
            row["expected_symbol"] = inventory_symbol or mark[2].rstrip(";")
            row["name_evidence"] = "inventory" if inventory_symbol else "source comment"
            expected = decode_symbol(row["expected_symbol"])
            line_end = text.find("\n", mark.end())
            actual = adjacent_name(code, len(text) if line_end < 0 else line_end, ranges)
            row.update(expected_class=expected[0], expected_method=expected[1],
                       actual_class=actual[0], actual_method=actual[1])
            differences = []
            for kind, wanted, found in [("class", expected[0] if expected[0] == actual[0]
                                        else class_name(expected[0]), actual[0]),
                                        ("method", expected[1], actual[1])]:
                if wanted != found:
                    differences.append(kind + ("-case" if wanted.lower() == found.lower() else "-name"))
            row["differences"] = differences
            row["status"] = ("mismatch" if any(d.endswith("-name") for d in differences)
                             else "case" if differences else "match")
        except ValueError as error:
            row.update(status="unresolved", reason=str(error))
        rows.append(row)
    return rows


def read_inventory(path):
    with path.open(encoding="utf-8-sig", newline="") as stream:
        return {int(r["address"], 16): r["name"] for r in csv.DictReader(stream, delimiter="\t")}


def read_research(root):
    mapping = root / "Mapping"
    symbols = read_inventory(mapping / "Lemmings_Paintball__68K_.functions.tsv")
    with (mapping / "confirmed-mappings.tsv").open(encoding="utf-8-sig", newline="") as stream:
        pairs = {(int(r["mac_address"], 16), int(r["windows_address"], 16))
                 for r in csv.DictReader(stream, delimiter="\t")}
    return symbols, pairs


def check_research(rows, symbols, pairs=None):
    sources = {}
    for row in rows:
        path = row["path"]
        if path not in sources:
            sources[path] = Path(path).read_text(encoding="utf-8").splitlines()
        mac = int(row["address_68k"], 16)
        issues = []
        original = symbols.get(mac)
        if original is None:
            issues.append("68K address absent from inventory")
        elif original != row["symbol"].rstrip(";"):
            issues.append("inventory symbol differs: " + original)
        windows = None
        for line in sources[path][row["line"]:]:
            stripped = line.strip()
            if stripped and not stripped.startswith("//"):
                break
            if re.match(r"//\s*68K\s", stripped):
                break
            match = re.search(r"//\s*(?:FUNCTION|STUB|SYNTHETIC|TEMPLATE|LIBRARY):\s+LEMBALL\s+(0x[0-9a-fA-F]+)", line)
            if match:
                windows = int(match[1], 16)
                break
        if pairs is not None and windows is not None and (mac, windows) not in pairs:
            issues.append("address pair absent from confirmed ledger")
        row["address_x86"] = None if windows is None else f"0x{windows:08x}"
        row["evidence_findings"] = issues


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("paths", type=Path, nargs="*", default=[ROOT / "src"])
    parser.add_argument("--strict", action="store_true", help="also report case-only differences")
    parser.add_argument("--json", action="store_true", help="emit all comparisons, including matches")
    parser.add_argument("--fail", action="store_true", help="return nonzero for findings or coverage gaps")
    parser.add_argument("--research-root", type=Path, help="also verify inventory symbols and confirmed address pairs")
    parser.add_argument("--inventory", type=Path, help="use an unnormalized Ghidra 68K function inventory TSV")
    args = parser.parse_args()
    files = set()
    for path in args.paths:
        if not path.exists():
            parser.error("path does not exist: " + str(path))
        files.update(p for p in (path.rglob("*") if path.is_dir() else [path])
                     if p.suffix in (".cpp", ".h"))
    if not files:
        parser.error("no C++ source files found")
    if args.inventory and args.research_root:
        parser.error("choose --inventory or --research-root")
    symbols, pairs = None, None
    try:
        if args.research_root:
            symbols, pairs = read_research(args.research_root)
        elif args.inventory:
            symbols = read_inventory(args.inventory)
    except (OSError, KeyError, ValueError) as error:
        parser.error("cannot read symbol evidence: " + str(error))
    rows = [row for path in sorted(files) for row in scan(path, symbols)]
    if symbols is not None:
        check_research(rows, symbols, pairs)
    counts = dict(Counter(row["status"] for row in rows))
    evidence_count = sum(bool(r.get("evidence_findings")) for r in rows)
    selected = [r for r in rows if r["status"] in ("mismatch", "unresolved")
                or (args.strict and r["status"] == "case") or r.get("evidence_findings")]
    if args.json:
        print(json.dumps({"files": len(files), "annotations": len(rows), "counts": counts,
                          "evidence_findings": evidence_count,
                          "comparisons": rows}, indent=2))
    else:
        for row in selected:
            detail = row.get("reason") or (
                f'{row["expected_class"]}::{row["expected_method"]} -> '
                f'{row["actual_class"]}::{row["actual_method"]} '
                f'({", ".join(row["differences"])})')
            print(f'{row["path"]}:{row["line"]}: {row["status"]}: {detail} [{row["symbol"]}]')
            for issue in row.get("evidence_findings", []):
                print("  evidence: " + issue)
        print(f'{len(files)} files, {len(rows)} annotations: {counts}')
        if symbols is not None:
            print(f'{evidence_count} annotations have inventory/ledger findings')
    if args.fail:
        if counts.get("unresolved") or not rows:
            return 2
        if selected:
            return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
