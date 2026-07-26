import argparse
import csv
import json
import msvcrt
import re
from contextlib import contextmanager
from datetime import date
from pathlib import Path


CLAIM_ROW = re.compile(r"^\| (text-\d{3}) \|")


def read_csv(path):
    with path.open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream))


def claims(path):
    result = []
    for line_number, line in enumerate(path.read_text(encoding="utf-8").splitlines()):
        if not CLAIM_ROW.match(line):
            continue
        fields = [field.strip() for field in line.strip("|").split("|")]
        if len(fields) != 5:
            raise SystemExit(f"{path}:{line_number + 1}: invalid claim")
        result.append(dict(zip(("range", "addresses", "functions", "owner", "claimed"), fields)))
    return result


def write_claim(path, selected, owner):
    lines = path.read_text(encoding="utf-8").splitlines()
    marker = "|---|---|---:|---|---|"
    index = lines.index(marker) + 1
    while index < len(lines) and CLAIM_ROW.match(lines[index]):
        index += 1
    lines.insert(
        index,
        f"| {selected['id']} | {selected['start']}..{selected['end']} | "
        f"{selected['function_count']} | {owner} | {date.today().isoformat()} |",
    )
    path.write_text("\r\n".join(lines) + "\r\n", encoding="utf-8", newline="")


@contextmanager
def claim_lock(root):
    path = root / "build-msvc420" / "claims.lock"
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("a+b") as stream:
        if path.stat().st_size == 0:
            stream.write(b"\0")
            stream.flush()
        stream.seek(0)
        msvcrt.locking(stream.fileno(), msvcrt.LK_LOCK, 1)
        try:
            yield
        finally:
            stream.seek(0)
            msvcrt.locking(stream.fileno(), msvcrt.LK_UNLCK, 1)


def matches(root):
    path = root / "build-msvc420" / "reccmp.json"
    if not path.exists():
        return {}
    try:
        rows = json.loads(path.read_text(encoding="utf-8"))["data"]
    except (KeyError, json.JSONDecodeError):
        return {}
    return {
        f"0x{int(row['address'], 16):08X}": float(row["matching"]) * 100
        for row in rows
        if row.get("type") in (None, 1) and row.get("matching") is not None
    }


def functions_for(root, selected):
    start = int(selected["start"], 16)
    end = int(selected["end"], 16)
    return [
        row
        for row in read_csv(root / "data" / "objdiff-functions.csv")
        if start <= int(row["address"], 16) < end
    ]


def notes_for(root, range_id):
    path = root / "data" / "function-status" / f"{range_id}.csv"
    if not path.exists():
        return {}
    return {row["address"]: row.get("notes", "") for row in read_csv(path)}


def choose_range(root, owner):
    ranges = read_csv(root / "data" / "work-ranges.csv")
    claim_path = root / "CLAIMS.md"
    active = claims(claim_path)
    owned = [row for row in active if row["owner"] == owner]
    if len(owned) > 1:
        raise SystemExit(f"{owner} owns multiple ranges")
    if owned:
        return next(row for row in ranges if row["id"] == owned[0]["range"]), False

    claimed = {row["range"] for row in active}
    ratios = matches(root)
    selected = next(
        (
            row
            for row in ranges
            if row["id"] not in claimed
            and any(ratios.get(function["address"]) != 100 for function in functions_for(root, row))
        ),
        None,
    )
    if selected is None:
        raise SystemExit("no unfinished unclaimed range")
    write_claim(claim_path, selected, owner)
    return selected, True


def start(root, owner):
    owner = owner.strip()
    if not owner:
        raise SystemExit("owner is required")
    with claim_lock(root):
        selected, created = choose_range(root, owner)

    ratios = matches(root)
    notes = notes_for(root, selected["id"])
    pending = [
        row
        for row in functions_for(root, selected)
        if ratios.get(row["address"]) != 100
    ]
    action = "claimed" if created else "continuing"
    print(
        f"{action} {selected['id']} {selected['start']}..{selected['end']} "
        f"({len(pending)} unfinished)"
    )
    for row in pending:
        ratio = ratios.get(row["address"])
        percent = "-" if ratio is None else f"{ratio:.2f}%"
        print(f"{row['address']} {percent:>7} {row['unit']} {row['name']}")
        if notes.get(row["address"]):
            print(f"  {notes[row['address']]}")


def main():
    parser = argparse.ArgumentParser(description="Claim or resume one decompilation range")
    parser.add_argument("owner")
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    start(args.root.resolve(), args.owner)


if __name__ == "__main__":
    main()
