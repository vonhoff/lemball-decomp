#!/usr/bin/env python3
"""Compare source names with adjacent // 68K Metrowerks symbols.

python tools/check_names.py [src/path ...] [--strict] [--json] [--fail]
python tools/check_names.py --inventory path/to/68K.functions.tsv
python tools/check_names.py --inventory path/to/68K.functions.tsv --confirmed path/to/confirmed-mappings.tsv

Expected form: PascalCase, drop C/tag/t prefixes, '_' is a word separator,
leading '_' becomes Internal. INTENTIONAL lists known Mac/Windows divergences.
--strict also reports case-only diffs. No source is changed.
"""

import argparse
from collections import Counter
import csv
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
MARK = re.compile(r"//\s*68K\s+(0x[0-9a-fA-F]+)\s+(\S+)")
FUNCTION = re.compile(
    r"(?:(?P<owner>[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*)\s*::\s*)?"
    r"(?P<method>operator\s*(?:new\b|delete\b|[^\w\s(]+)|~?[A-Za-z_]\w*)\s*\("
)
OPERATORS = {
    "__as": "operator=", "__ls": "operator<<", "__nw": "operatornew",
    "__dl": "operatordelete", "__apl": "operator+=", "__pl": "operator+",
    "__eq": "operator==", "__gt": "operator>", "__ml": "operator*",
}
ACRONYMS = (
    ("TCPIP", "TcpIp"), ("MRAM", "Mram"), ("GDI", "Gdi"), ("RAM", "Ram"),
    ("CD", "Cd"), ("PV", "Pv"), ("VS", "Vs"), ("AI", "Ai"),
)
# Do not rename source to match 68K for these; see AGENTS.md Naming.
INTENTIONAL = {
    ("Wnd", "OnZoomBox", "Wnd", "OnDriverChange"),
    ("PreviewDrawer::Prims", "<constructor>", "PreviewDrawerPrims", "<constructor>"),
    ("PreviewDrawer::Prims", "<destructor>", "PreviewDrawerPrims", "<destructor>"),
    ("SuccFailDrawer::Prims", "<constructor>", "SuccFailDrawerPrims", "<constructor>"),
    ("SuccFailDrawer::Prims", "<destructor>", "SuccFailDrawerPrims", "<destructor>"),
    ("CdLoadAnim", "Draw", "CdLoadAnimDraw", "Draw"),
    ("CdLoadAnim", "Draw", "CdLoadAnimProgress", "Draw"),
    ("Process", "<destructor>", "BaseProcess", "<destructor>"),
    ("", "GetCdDir", "TargetPlatformServices", "GetCdDir"),
}


def decode_symbol(symbol):
    split = re.search(r"__(?=\d|Q\d|F)", symbol)
    if split is None:
        if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", symbol):
            return "", symbol
        raise ValueError("unsupported symbol form")
    method, rest = symbol[:split.start()], symbol[split.end():]
    owners, count = [], 1
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


def normalize_word(word):
    for acronym, pascal in ACRONYMS:
        if word.startswith(acronym):
            rest = word[len(acronym):]
            if rest and rest[0].islower():
                rest = rest[0].upper() + rest[1:]
            word = pascal + rest
            break

    def replace_run(match):
        run = match.group(0)
        nxt = word[match.end():match.end() + 1]
        if nxt and nxt.islower():
            return run[0] + run[1:-1].lower() + run[-1]
        return run[0] + run[1:].lower()

    return re.sub(r"[A-Z]{2,}", replace_run, word)


def normalize_segment(segment):
    for prefix in ("tag", "t", "C"):
        if (len(segment) > len(prefix) and segment.startswith(prefix)
                and segment[len(prefix)].isupper()):
            segment = segment[len(prefix):]
            break
    return "".join(normalize_word(part) for part in segment.split("_") if part)


def class_name(name):
    return "::".join(normalize_segment(part) for part in name.split("::"))


def method_name(name):
    if name.startswith("<") or name.startswith("operator"):
        return name
    internal = name.startswith("_")
    body = "".join(
        n[:1].upper() + n[1:]
        for part in name.split("_") if part
        for n in [normalize_segment(part)] if n
    )
    return "Internal" + body if internal else body


def method_fold(name):
    if name.startswith("<") or name.startswith("operator"):
        return name
    internal = name.startswith("_")
    body = "".join(part for part in name.split("_") if part)
    return ("Internal" + body if internal else body).lower()


def mask_comments_and_strings(text):
    pattern = r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\''
    return re.sub(pattern, lambda m: re.sub(r"[^\n]", " ", m[0]), text)


def class_ranges(code):
    ends, stack, result = {}, [], []
    for pos, char in enumerate(code):
        if char == "{":
            stack.append(pos)
        elif char == "}" and stack:
            ends[stack.pop()] = pos
    for match in re.finditer(r"\b(?:class|struct)\s+(\w+)\s*(?:final\s*)?(?::[^;{}]*)?\{", code):
        opening = match.end() - 1
        if opening in ends:
            result.append((opening, ends[opening], match[1]))
    return result


def adjacent_name(code, offset, ranges):
    start = offset
    while start < len(code) and code[start].isspace():
        start += 1
    end = re.search(r"[;{}#]", code[start:])
    declaration = code[start:start + end.start()] if end else code[start:]
    match = FUNCTION.search(declaration)
    if not match:
        raise ValueError("no adjacent function declaration")
    owner = match["owner"]
    if not owner:
        owner = "::".join(r[2] for r in ranges if r[0] < start < r[1])
    method = re.sub(r"\s+", "", match["method"])
    leaf = owner.split("::")[-1] if owner else ""
    if owner and method == leaf:
        method = "<constructor>"
    elif owner and method == "~" + leaf:
        method = "<destructor>"
    return owner, method


def scan(path, symbols=None):
    text = path.read_text(encoding="utf-8")
    code = mask_comments_and_strings(text)
    ranges = class_ranges(code)
    rows = []
    for mark in MARK.finditer(text):
        row = {"path": str(path), "line": text.count("\n", 0, mark.start()) + 1,
               "address_68k": mark[1], "symbol": mark[2]}
        try:
            inventory = None if symbols is None else symbols.get(int(mark[1], 16))
            expected_symbol = inventory or mark[2].rstrip(";")
            expected = decode_symbol(expected_symbol)
            line_end = text.find("\n", mark.end())
            actual = adjacent_name(code, len(text) if line_end < 0 else line_end, ranges)
            wanted_class = expected[0] if expected[0] == actual[0] else class_name(expected[0])
            wanted_method = method_name(expected[1])
            diffs = []
            if wanted_class != actual[0]:
                diffs.append("class-case" if wanted_class.lower() == actual[0].lower() else "class-name")
            if wanted_method != actual[1]:
                if method_fold(expected[1]) != method_fold(actual[1]) or "_" in actual[1]:
                    diffs.append("method-name")
                else:
                    diffs.append("method-case")
            key = (wanted_class, wanted_method, actual[0], actual[1])
            if diffs and key in INTENTIONAL:
                status = "intentional"
            elif any(d.endswith("-name") for d in diffs):
                status = "mismatch"
            elif diffs:
                status = "case"
            else:
                status = "match"
            row.update(wanted_class=wanted_class, wanted_method=wanted_method,
                       actual_class=actual[0], actual_method=actual[1],
                       differences=diffs, status=status,
                       expected_symbol=expected_symbol,
                       name_evidence="inventory" if inventory else "source comment")
        except ValueError as error:
            reason = str(error)
            symbol = row["symbol"].rstrip(";")
            after = text[mark.end():mark.end() + 500]
            synthetic = (reason == "no adjacent function declaration"
                         and symbol.startswith(("__ct__", "__dt__"))
                         and re.search(r"SYNTHETIC:|^\s*(?:class|struct)\s+\w+", after, re.M))
            row.update(status="synthetic" if synthetic else "unresolved", reason=reason)
        rows.append(row)
    return rows


def read_inventory(path):
    with path.open(encoding="utf-8-sig", newline="") as stream:
        return {int(r["address"], 16): r["name"] for r in csv.DictReader(stream, delimiter="\t")}


def read_confirmed(path):
    with path.open(encoding="utf-8-sig", newline="") as stream:
        return {(int(r["mac_address"], 16), int(r["windows_address"], 16))
                for r in csv.DictReader(stream, delimiter="\t")}


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
            match = re.search(
                r"//\s*(?:FUNCTION|STUB|SYNTHETIC|TEMPLATE|LIBRARY):\s+LEMBALL\s+(0x[0-9a-fA-F]+)",
                line)
            if match:
                windows = int(match[1], 16)
                break
        if pairs is not None and windows is not None and (mac, windows) not in pairs:
            issues.append("address pair absent from confirmed ledger")
        row["address_x86"] = None if windows is None else f"0x{windows:08x}"
        row["evidence_findings"] = issues


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("paths", type=Path, nargs="*", default=[ROOT / "src"])
    parser.add_argument("--strict", action="store_true")
    parser.add_argument("--json", action="store_true")
    parser.add_argument("--fail", action="store_true")
    parser.add_argument("--inventory", type=Path)
    parser.add_argument("--confirmed", type=Path,
                        help="confirmed-mappings.tsv (requires --inventory)")
    args = parser.parse_args()
    files = set()
    for path in args.paths:
        if not path.exists():
            parser.error("path does not exist: " + str(path))
        files.update(p for p in (path.rglob("*") if path.is_dir() else [path])
                     if p.suffix in (".cpp", ".h"))
    if not files:
        parser.error("no C++ source files found")
    if args.confirmed and not args.inventory:
        parser.error("--confirmed requires --inventory")
    symbols, pairs = None, None
    try:
        if args.inventory:
            symbols = read_inventory(args.inventory)
        if args.confirmed:
            pairs = read_confirmed(args.confirmed)
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
                          "evidence_findings": evidence_count, "comparisons": rows}, indent=2))
    else:
        for row in selected:
            detail = row.get("reason") or (
                f'{row["wanted_class"]}::{row["wanted_method"]} -> '
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
