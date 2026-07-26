import argparse
import csv
import difflib
import json
import re
import subprocess
from datetime import datetime, timezone
from pathlib import Path


STATES = {"unseen", "surveyed", "reconstructed", "matching", "matched", "blocked"}
STATUS_FIELDS = [
    "address",
    "state",
    "baseline_percent",
    "match_percent",
    "updated",
    "notes",
]
DEPENDENCY_FIELDS = [
    "from_address",
    "to_address",
    "target_range",
    "status",
    "updated",
    "notes",
]
ADDRESS = re.compile(r"^0x[0-9a-fA-F]{8}$")
CLAIM_ROW = re.compile(r"^\| (text-\d{3}) \|")
ANNOTATION = re.compile(
    r"^\s*// (FUNCTION|STUB|GLOBAL|VTABLE): LEMBALL (0x[0-9a-fA-F]{8})"
)
ASSEMBLY = re.compile(r"\b(?:__asm|_asm)\b|__declspec\s*\(\s*naked\s*\)")


def address(value):
    if not ADDRESS.fullmatch(value):
        raise SystemExit(f"invalid address: {value}")
    return int(value, 16)


def hex_address(value):
    return f"0x{value:08X}"


def paths(root):
    return {
        "inventory": root / "data" / "objdiff-functions.csv",
        "ranges": root / "data" / "work-ranges.csv",
        "status": root / "data" / "function-status",
        "dependencies": root / "data" / "dependencies",
        "claims": root / "CLAIMS.md",
        "abi": root / "claims" / "abi",
    }


def read_inventory(path):
    with path.open(newline="", encoding="utf-8") as stream:
        rows = list(csv.DictReader(stream))
    for row in rows:
        row["address_int"] = address(row["address"])
        row["size_int"] = int(row["size"])
    return sorted(rows, key=lambda row: row["address_int"])


def read_ranges(path):
    with path.open(newline="", encoding="utf-8") as stream:
        rows = list(csv.DictReader(stream))
    for row in rows:
        row["start_int"] = address(row["start"])
        row["end_int"] = address(row["end"])
        row["function_count_int"] = int(row["function_count"])
        row["code_bytes_int"] = int(row["code_bytes"])
    return rows


def write_csv(path, fieldnames, rows):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=fieldnames, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def read_code_claims(path):
    claims = []
    for number, line in enumerate(path.read_text(encoding="utf-8").splitlines()):
        if not CLAIM_ROW.match(line):
            continue
        fields = [field.strip() for field in line.strip().strip("|").split("|")]
        if len(fields) != 5:
            raise SystemExit(f"{path}:{number + 1}: invalid claim row")
        range_id, addresses, functions, owner, claimed = fields
        claims.append(
            {
                "range": range_id,
                "addresses": addresses,
                "functions": functions,
                "owner": owner,
                "claimed": claimed,
                "line": number,
            }
        )
    return claims


def write_claim_lines(path, lines):
    path.write_text(
        "\r\n".join(lines) + "\r\n",
        encoding="utf-8",
        newline="",
    )


def add_code_claim(path, row, owner):
    lines = path.read_text(encoding="utf-8").splitlines()
    marker = "|---|---|---:|---|---|"
    index = lines.index(marker) + 1
    while index < len(lines) and CLAIM_ROW.match(lines[index]):
        index += 1
    lines.insert(
        index,
        f"| {row['id']} | {row['start']}..{row['end']} | "
        f"{row['function_count']} | {owner} | {now().date().isoformat()} |",
    )
    write_claim_lines(path, lines)


def remove_code_claim(path, range_id, owner):
    lines = path.read_text(encoding="utf-8").splitlines()
    claim = next(
        (item for item in read_code_claims(path) if item["range"] == range_id),
        None,
    )
    if not claim:
        raise SystemExit(f"not claimed: {range_id}")
    if claim["owner"] != owner:
        raise SystemExit(f"claim belongs to {claim['owner']}")
    lines.pop(claim["line"])
    write_claim_lines(path, lines)
    print(f"released {range_id}")


def load_abi_claims(directory):
    claims = []
    for path in sorted(directory.glob("*.json")):
        try:
            claim = json.loads(path.read_text(encoding="utf-8"))
        except (json.JSONDecodeError, OSError) as error:
            raise SystemExit(f"{path}: {error}")
        claim["path"] = path
        claims.append(claim)
    return claims


def now():
    return datetime.now(timezone.utc).replace(microsecond=0)


def git_head(root):
    result = subprocess.run(
        ["git", "rev-parse", "HEAD"],
        cwd=root,
        capture_output=True,
        text=True,
        check=False,
    )
    return result.stdout.strip() if result.returncode == 0 else "unknown"


def write_abi_claim(path, owner, root, anchor):
    claim = {
        "kind": "abi",
        "owner": owner,
        "created": now().isoformat(),
        "base": git_head(root),
        "anchor": anchor,
    }
    path.parent.mkdir(parents=True, exist_ok=True)
    try:
        with path.open("x", encoding="utf-8") as stream:
            json.dump(claim, stream, indent=2)
            stream.write("\n")
    except FileExistsError:
        raise SystemExit(f"already claimed: {path.stem}")
    print(path.relative_to(root))


def take(root, range_id, owner):
    project = paths(root)
    ranges = {row["id"]: row for row in read_ranges(project["ranges"])}
    if range_id not in ranges:
        raise SystemExit(f"unknown range: {range_id}")
    claims = read_code_claims(project["claims"])
    selected = next(
        (claim for claim in claims if claim["range"] == range_id),
        None,
    )
    if selected:
        raise SystemExit(f"already claimed by {selected['owner']}")
    for claim in claims:
        if claim["owner"] == owner:
            raise SystemExit(f"{owner} already holds {claim['range']}")
    snapshot_status(root, range_id)
    add_code_claim(project["claims"], ranges[range_id], owner)
    print(f"claimed {range_id}")


def take_abi(root, anchor, owner):
    normalized = hex_address(address(anchor))
    write_abi_claim(
        paths(root)["abi"] / f"{normalized[2:]}.json",
        owner,
        root,
        normalized,
    )


def release_abi(path, owner):
    if not path.exists():
        raise SystemExit(f"not claimed: {path.stem}")
    claim = json.loads(path.read_text(encoding="utf-8"))
    if claim.get("owner") != owner:
        raise SystemExit(f"claim belongs to {claim.get('owner')}")
    path.unlink()
    print(f"released {path.stem}")


def range_for(ranges, range_id):
    for row in ranges:
        if row["id"] == range_id:
            return row
    raise SystemExit(f"unknown range: {range_id}")


def read_status(path):
    if not path.exists():
        return {}
    with path.open(newline="", encoding="utf-8") as stream:
        return {row["address"]: row for row in csv.DictReader(stream)}


def claimed_range(root, range_id, owner):
    project = paths(root)
    claim = next(
        (
            item
            for item in read_code_claims(project["claims"])
            if item["range"] == range_id
        ),
        None,
    )
    if not claim:
        raise SystemExit(f"range is not claimed: {range_id}")
    if claim["owner"] != owner:
        raise SystemExit(f"claim belongs to {claim['owner']}")
    return range_for(read_ranges(project["ranges"]), range_id)


def functions_in_range(root, range_id):
    project = paths(root)
    selected = range_for(read_ranges(project["ranges"]), range_id)
    return [
        row
        for row in read_inventory(project["inventory"])
        if selected["start_int"] <= row["address_int"] < selected["end_int"]
    ]


def format_percent(value):
    return "" if value is None else f"{value:.6f}"


def snapshot_status(root, range_id):
    project = paths(root)
    matches = match_percentages(root)
    if not matches:
        raise SystemExit(
            "missing build-msvc420/reccmp.json; build and run reccmp before claiming"
        )
    status_path = project["status"] / f"{range_id}.csv"
    previous = read_status(status_path)
    timestamp = now().isoformat()
    rows = []
    for function in functions_in_range(root, range_id):
        old = previous.get(function["address"], {})
        current = matches.get(function["address"])
        state = old.get("state", "unseen")
        if current == 100:
            state = "matched"
        elif state == "matched":
            state = "matching"
        rows.append(
            {
                "address": function["address"],
                "state": state,
                "baseline_percent": format_percent(current),
                "match_percent": format_percent(current),
                "updated": old.get("updated") or timestamp,
                "notes": old.get("notes", ""),
            }
        )
    write_csv(status_path, STATUS_FIELDS, rows)


def source_states(root):
    states = {}
    for path in (root / "src").rglob("*"):
        if path.suffix.upper() not in {".CPP", ".H"}:
            continue
        for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
            match = ANNOTATION.match(line)
            if match and match.group(1) in {"FUNCTION", "STUB"}:
                states[hex_address(address(match.group(2)))] = match.group(1).lower()
    return states


def match_percentages(root):
    path = root / "build-msvc420" / "reccmp.json"
    output = root / "build-msvc420" / "LEMBALL.EXE"
    objects = list(
        (root / "build-msvc420" / "CMakeFiles" / "LEMBALL.dir").rglob("*.obj")
    )
    if objects and (
        not output.exists()
        or any(item.stat().st_mtime_ns > output.stat().st_mtime_ns for item in objects)
    ):
        raise SystemExit("stale LEMBALL.EXE; run python tools/build_msvc420.py")
    if output.exists() and (
        not path.exists() or path.stat().st_mtime_ns < output.stat().st_mtime_ns
    ):
        raise SystemExit("stale reccmp.json; rerun reccmp")
    if not path.exists():
        return {}
    try:
        rows = json.loads(path.read_text(encoding="utf-8"))["data"]
    except (KeyError, json.JSONDecodeError, OSError):
        return {}
    return {
        hex_address(int(row["address"], 16)): float(row["matching"]) * 100
        for row in rows
        if row.get("type") in (None, 1)
        and row.get("matching") is not None
    }


def set_status(root, range_id, owner, function_address, state, note):
    project = paths(root)
    claimed_range(root, range_id, owner)
    if state not in STATES:
        raise SystemExit(f"invalid state: {state}")
    if state not in {"unseen", "matched"} and not note.strip():
        raise SystemExit("notes required for resumable non-matched work")

    value = address(function_address)
    functions = {
        row["address"]: row for row in functions_in_range(root, range_id)
    }
    normalized = hex_address(value)
    if normalized not in functions:
        raise SystemExit(f"{normalized} is not in {range_id}")

    status_path = project["status"] / f"{range_id}.csv"
    rows = read_status(status_path)
    if normalized not in rows or "baseline_percent" not in rows[normalized]:
        raise SystemExit("claim baseline missing; release and retake after reccmp")
    current = match_percentages(root).get(normalized)
    if state == "matched" and current != 100:
        raise SystemExit(f"{normalized} is not 100% in reccmp")
    rows[normalized].update(
        {
            "state": state,
            "match_percent": format_percent(current),
            "updated": now().isoformat(),
            "notes": note,
        }
    )
    write_csv(status_path, STATUS_FIELDS, sorted(rows.values(), key=lambda row: row["address"]))
    print(f"{normalized}: {state}")


def owner_range(ranges, function_address):
    value = address(function_address)
    for row in ranges:
        if row["start_int"] <= value < row["end_int"]:
            return row["id"]
    raise SystemExit(f"{hex_address(value)} is outside work ranges")


def read_dependencies(path):
    if not path.exists():
        return {}
    with path.open(newline="", encoding="utf-8") as stream:
        return {
            (row["from_address"], row["to_address"]): row
            for row in csv.DictReader(stream)
        }


def set_dependency(root, range_id, owner, from_address, to_address, status, note):
    project = paths(root)
    claimed_range(root, range_id, owner)
    if not note.strip():
        raise SystemExit("dependency notes must state evidence, blocker, or resolution")
    ranges = read_ranges(project["ranges"])
    source = hex_address(address(from_address))
    target = hex_address(address(to_address))
    if owner_range(ranges, source) != range_id:
        raise SystemExit(f"{source} is outside {range_id}")
    target_range = owner_range(ranges, target)
    if target_range == range_id:
        raise SystemExit("dependency is local; record it in function status notes")

    path = project["dependencies"] / f"{range_id}.csv"
    rows = read_dependencies(path)
    key = (source, target)
    if status == "resolved" and key not in rows:
        raise SystemExit("dependency is not recorded")
    rows[key] = {
        "from_address": source,
        "to_address": target,
        "target_range": target_range,
        "status": status,
        "updated": now().isoformat(),
        "notes": note,
    }
    write_csv(
        path,
        DEPENDENCY_FIELDS,
        sorted(rows.values(), key=lambda row: (row["from_address"], row["to_address"])),
    )
    print(f"{source} depends on {target} ({target_range}): {status}")


def list_dependencies(root, range_id=None):
    directory = paths(root)["dependencies"]
    files = [directory / f"{range_id}.csv"] if range_id else sorted(directory.glob("*.csv"))
    count = 0
    for path in files:
        for row in read_dependencies(path).values():
            count += 1
            print(
                f"{path.stem} {row['from_address']} {row['to_address']} "
                f"{row['target_range']} {row['status']} {row['notes']}"
            )
    if not count:
        print("no recorded cross-range dependencies")


def verify_scope(root, range_id, owner):
    project = paths(root)
    claimed_range(root, range_id, owner)
    status_path = project["status"] / f"{range_id}.csv"
    statuses = read_status(status_path)
    if not statuses:
        raise SystemExit("claim baseline missing; release and retake after reccmp")
    current = match_percentages(root)
    if not current:
        raise SystemExit("missing build-msvc420/reccmp.json")

    errors = []
    new_exact = []
    progress_changed = False
    timestamp = now().isoformat()
    for function in functions_in_range(root, range_id):
        row = statuses.get(function["address"])
        if not row:
            errors.append(f"{function['address']}: missing progress row")
            continue
        baseline_text = row.get("baseline_percent", "")
        baseline = float(baseline_text) if baseline_text else None
        ratio = current.get(function["address"])
        latest = format_percent(ratio)
        if row.get("match_percent") != latest:
            row["match_percent"] = latest
            row["updated"] = timestamp
            progress_changed = True
        if baseline is not None and (ratio is None or ratio + 1e-6 < baseline):
            rendered = "absent" if ratio is None else f"{ratio:.6f}%"
            errors.append(
                f"{function['address']}: regressed from {baseline:.6f}% to {rendered}"
            )
        if row.get("state") == "matched" and ratio != 100:
            errors.append(f"{function['address']}: marked matched but not 100%")
        if ratio == 100 and (baseline is None or baseline < 100):
            new_exact.append(function["address"])
            if row.get("state") != "matched":
                errors.append(
                    f"{function['address']}: newly exact function is not marked matched"
                )
    if progress_changed:
        write_csv(
            status_path,
            STATUS_FIELDS,
            sorted(statuses.values(), key=lambda row: row["address"]),
        )
    if not new_exact:
        errors.append("no function became 100% since claim baseline")

    open_dependencies = sum(
        row["status"] == "open"
        for row in read_dependencies(
            project["dependencies"] / f"{range_id}.csv"
        ).values()
    )
    if errors:
        for error in errors:
            print(error)
        raise SystemExit(1)
    print(
        f"{range_id}: clean, {len(new_exact)} new exact, "
        f"{open_dependencies} open cross-range dependencies"
    )


def brief(root, range_id):
    project = paths(root)
    selected = range_for(read_ranges(project["ranges"]), range_id)
    statuses = read_status(project["status"] / f"{range_id}.csv")
    sources = source_states(root)
    matches = match_percentages(root)
    print(
        f"{range_id} {selected['start']}..{selected['end']} "
        f"({selected['function_count']} functions, {selected['code_bytes']} bytes)"
    )
    for row in read_inventory(project["inventory"]):
        if selected["start_int"] <= row["address_int"] < selected["end_int"]:
            state = statuses.get(row["address"], {}).get(
                "state",
                {
                    "function": "implemented",
                    "stub": "stub",
                }.get(sources.get(row["address"]), "unimplemented"),
            )
            ratio = (
                f"{matches[row['address']]:6.2f}%"
                if row["address"] in matches
                else "      -"
            )
            print(
                f"{row['address']} {row['size']:>5} {ratio} {state:<13} "
                f"{row['unit']} {row['name']}"
            )
    open_dependencies = [
        row
        for row in read_dependencies(
            project["dependencies"] / f"{range_id}.csv"
        ).values()
        if row["status"] == "open"
    ]
    if open_dependencies:
        print(f"{len(open_dependencies)} open blocking dependencies")


def list_work(root, available_only):
    project = paths(root)
    claims = {
        claim["range"]: claim for claim in read_code_claims(project["claims"])
    }
    for row in read_ranges(project["ranges"]):
        claim = claims.get(row["id"])
        if available_only and claim:
            continue
        state = f"claimed by {claim['owner']}" if claim else "available"
        print(
            f"{row['id']} {row['start']}..{row['end']} "
            f"{row['function_count']:>3} functions {state}"
        )


def annotation_at(lines, line_number):
    found = None
    for line in lines[: line_number + 1]:
        match = ANNOTATION.match(line)
        if match:
            found = match
    if found and found.group(1) == "FUNCTION":
        return hex_address(address(found.group(2)))
    return None


def git_text(root, revision, path):
    result = subprocess.run(
        ["git", "show", f"{revision}:{path}"],
        cwd=root,
        capture_output=True,
        text=True,
        check=False,
    )
    return result.stdout.splitlines() if result.returncode == 0 else []


def changed_functions(root, base):
    result = subprocess.run(
        ["git", "diff", "--name-only", "--diff-filter=ACDMR", base, "--", "src"],
        cwd=root,
        capture_output=True,
        text=True,
        check=False,
    )
    if result.returncode:
        raise SystemExit(result.stderr.strip() or f"cannot diff {base}")

    changed = set()
    for name in result.stdout.splitlines():
        if Path(name).suffix.upper() not in {".CPP", ".H"}:
            continue
        old = git_text(root, base, name)
        path = root / name
        new = (
            path.read_text(encoding="utf-8", errors="replace").splitlines()
            if path.exists()
            else []
        )
        matcher = difflib.SequenceMatcher(None, old, new, autojunk=False)
        for tag, old_start, old_end, new_start, new_end in matcher.get_opcodes():
            if tag == "equal":
                continue
            for line_number in range(old_start, max(old_end, old_start + 1)):
                function = annotation_at(old, min(line_number, max(len(old) - 1, 0)))
                if function:
                    changed.add(function)
            for line_number in range(new_start, max(new_end, new_start + 1)):
                function = annotation_at(new, min(line_number, max(len(new) - 1, 0)))
                if function:
                    changed.add(function)
    return changed


def added_assembly(root, base):
    result = subprocess.run(
        ["git", "diff", "--unified=0", base, "--", "src"],
        cwd=root,
        capture_output=True,
        text=True,
        check=False,
    )
    if result.returncode:
        raise SystemExit(result.stderr.strip() or f"cannot diff {base}")
    errors = []
    for line in result.stdout.splitlines():
        if line.startswith("+++ b/"):
            suffix = Path(line[6:]).suffix.lower()
            if suffix in {".asm", ".s"}:
                errors.append(f"{line[6:]}: external assembly is forbidden")
        elif line.startswith("+") and not line.startswith("+++") and ASSEMBLY.search(line[1:]):
            errors.append("new inline assembly is forbidden")
    return errors


def validate(root, base=None):
    project = paths(root)
    inventory = read_inventory(project["inventory"])
    ranges = read_ranges(project["ranges"])
    errors = []
    ids = set()

    for row in ranges:
        if row["id"] in ids:
            errors.append(f"duplicate range id: {row['id']}")
        ids.add(row["id"])
        functions = [
            function
            for function in inventory
            if row["start_int"] <= function["address_int"] < row["end_int"]
        ]
        if row["start_int"] >= row["end_int"]:
            errors.append(f"{row['id']}: empty or reversed")
        if len(functions) != row["function_count_int"]:
            errors.append(f"{row['id']}: function count changed")
        if sum(function["size_int"] for function in functions) != row["code_bytes_int"]:
            errors.append(f"{row['id']}: code byte count changed")

    for function in inventory:
        owners = [
            row["id"]
            for row in ranges
            if row["start_int"] <= function["address_int"] < row["end_int"]
        ]
        if len(owners) != 1:
            errors.append(f"{function['address']}: belongs to {len(owners)} ranges")

    owners = {}
    code_claims = read_code_claims(project["claims"])
    claim_ids = {claim["range"] for claim in code_claims}
    if not claim_ids <= ids:
        errors.append("CLAIMS.md contains unknown range")
    range_map = {row["id"]: row for row in ranges}
    for claim in code_claims:
        row = range_map.get(claim["range"])
        expected_addresses = f"{row['start']}..{row['end']}" if row else ""
        if claim["addresses"] != expected_addresses:
            errors.append(f"{claim['range']}: address text changed")
        if row and claim["functions"] != str(row["function_count_int"]):
            errors.append(f"{claim['range']}: function count text changed")
        owner = claim["owner"]
        if not owner or not claim["claimed"]:
            errors.append(f"{claim['range']}: active claim lacks owner or date")
        if owner in owners:
            errors.append(f"{owner}: holds multiple code ranges")
        owners[owner] = claim["range"]

    if base:
        active = [row for row in ranges if row["id"] in claim_ids]
        for function in changed_functions(root, base):
            value = address(function)
            if not any(row["start_int"] <= value < row["end_int"] for row in active):
                errors.append(f"{function}: changed outside active code claim")
        errors.extend(added_assembly(root, base))

    for claim in load_abi_claims(project["abi"]):
        try:
            address(claim.get("anchor", ""))
        except SystemExit:
            errors.append(f"{claim['path']}: invalid ABI anchor")
        if claim.get("kind") != "abi" or not claim.get("owner"):
            errors.append(f"{claim['path']}: invalid ABI claim")

    inventory_addresses = {row["address"] for row in inventory}
    for status_path in sorted(project["status"].glob("*.csv")):
        if status_path.stem not in ids:
            errors.append(f"{status_path}: unknown range")
            continue
        selected = range_for(ranges, status_path.stem)
        for item in read_status(status_path).values():
            if any(field not in item for field in STATUS_FIELDS):
                errors.append(f"{status_path}: obsolete status schema")
                continue
            try:
                value = address(item["address"])
            except (KeyError, SystemExit):
                errors.append(f"{status_path}: invalid status address")
                continue
            if item.get("address") not in inventory_addresses:
                errors.append(f"{item.get('address')}: absent from inventory")
            if not selected["start_int"] <= value < selected["end_int"]:
                errors.append(f"{item.get('address')}: outside {status_path.stem}")
            if item.get("state") not in STATES:
                errors.append(f"{item.get('address')}: invalid state")
            baseline = item.get("baseline_percent")
            ratio = item.get("match_percent")
            try:
                if baseline:
                    float(baseline)
                if ratio:
                    float(ratio)
            except ValueError:
                errors.append(f"{item.get('address')}: invalid match percentage")

    for dependency_path in sorted(project["dependencies"].glob("*.csv")):
        if dependency_path.stem not in ids:
            errors.append(f"{dependency_path}: unknown source range")
            continue
        for item in read_dependencies(dependency_path).values():
            if any(field not in item for field in DEPENDENCY_FIELDS):
                errors.append(f"{dependency_path}: obsolete dependency schema")
                continue
            try:
                source_range = owner_range(ranges, item["from_address"])
                target_range = owner_range(ranges, item["to_address"])
            except (KeyError, SystemExit):
                errors.append(f"{dependency_path}: invalid dependency address")
                continue
            if source_range != dependency_path.stem:
                errors.append(f"{item['from_address']}: outside {dependency_path.stem}")
            if target_range != item.get("target_range"):
                errors.append(f"{item['to_address']}: wrong target range")
            if source_range == target_range:
                errors.append(f"{item['from_address']}: dependency is not cross-range")
            if item.get("status") not in {"open", "resolved"}:
                errors.append(f"{item['from_address']}: invalid dependency status")
            if not item.get("notes", "").strip():
                errors.append(f"{item['from_address']}: dependency lacks notes")

    if errors:
        for error in errors:
            print(error)
        raise SystemExit(1)
    print(
        f"{len(ranges)} ranges, {len(inventory)} functions, "
        f"{len(owners)} active code claims"
    )


def parser():
    result = argparse.ArgumentParser(description="Manage exclusive decompilation work")
    result.add_argument(
        "--root", type=Path, default=Path(__file__).resolve().parents[1]
    )
    commands = result.add_subparsers(dest="command", required=True)

    listing = commands.add_parser("list")
    listing.add_argument("--available", action="store_true")

    brief_command = commands.add_parser("brief")
    brief_command.add_argument("range")

    claim = commands.add_parser("take")
    claim.add_argument("range")
    claim.add_argument("owner")

    claim_abi = commands.add_parser("take-abi")
    claim_abi.add_argument("anchor")
    claim_abi.add_argument("owner")

    release_code = commands.add_parser("release")
    release_code.add_argument("range")
    release_code.add_argument("owner")

    release_abi = commands.add_parser("release-abi")
    release_abi.add_argument("anchor")
    release_abi.add_argument("owner")

    status = commands.add_parser("status")
    status.add_argument("range")
    status.add_argument("owner")
    status.add_argument("address")
    status.add_argument("state", choices=sorted(STATES))
    status.add_argument("notes", nargs="?", default="")

    verify = commands.add_parser("verify")
    verify.add_argument("range")
    verify.add_argument("owner")

    dependency = commands.add_parser("dependency")
    dependency.add_argument("range")
    dependency.add_argument("owner")
    dependency.add_argument("from_address")
    dependency.add_argument("to_address")
    dependency.add_argument("notes")

    resolve = commands.add_parser("resolve-dependency")
    resolve.add_argument("range")
    resolve.add_argument("owner")
    resolve.add_argument("from_address")
    resolve.add_argument("to_address")
    resolve.add_argument("notes")

    dependencies = commands.add_parser("dependencies")
    dependencies.add_argument("range", nargs="?")

    check = commands.add_parser("check")
    check.add_argument("--base")
    return result


def main():
    args = parser().parse_args()
    root = args.root.resolve()
    project = paths(root)
    if args.command == "list":
        list_work(root, args.available)
    elif args.command == "brief":
        brief(root, args.range)
    elif args.command == "take":
        take(root, args.range, args.owner)
    elif args.command == "take-abi":
        take_abi(root, args.anchor, args.owner)
    elif args.command == "release":
        remove_code_claim(project["claims"], args.range, args.owner)
    elif args.command == "release-abi":
        anchor = hex_address(address(args.anchor))
        release_abi(project["abi"] / f"{anchor[2:]}.json", args.owner)
    elif args.command == "status":
        set_status(root, args.range, args.owner, args.address, args.state, args.notes)
    elif args.command == "verify":
        verify_scope(root, args.range, args.owner)
    elif args.command == "dependency":
        set_dependency(
            root,
            args.range,
            args.owner,
            args.from_address,
            args.to_address,
            "open",
            args.notes,
        )
    elif args.command == "resolve-dependency":
        set_dependency(
            root,
            args.range,
            args.owner,
            args.from_address,
            args.to_address,
            "resolved",
            args.notes,
        )
    elif args.command == "dependencies":
        list_dependencies(root, args.range)
    elif args.command == "check":
        validate(root, args.base)


if __name__ == "__main__":
    main()
