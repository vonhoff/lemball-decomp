#!/usr/bin/env python3
"""Audit the LEMBALL decomp: map the current source tree against the
Macintosh blueprint (macintosh-x86-correlations.csv) and the live reccmp report.

Produces:
  * which blueprint classes are present vs missing from the source tree
  * which zero-ratio (unreconstructed) game functions exist and their best-guess
    Mac class + blueprint-shaped rename
  * the legacy ALL-CAPS / non-conforming source files still to address

Usage:
  python tools/reconstruction_audit.py            # summary
  python tools/reconstruction_audit.py --missing  # missing blueprint classes
  python tools/reconstruction_audit.py --zeros    # zero-ratio fns + proposed class/rename
  python tools/reconstruction_audit.py --legacy   # legacy source files
"""

from __future__ import annotations

import argparse
import csv
import json
import re
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / "src"
CORR = ROOT / "data" / "macintosh-x86-correlations.csv"
INV = ROOT / "data" / "objdiff-functions.csv"
RECCMP = ROOT / "build-msvc400" / "reccmp.json"

# Mac mangling:  "<func>__<len><ClassName><sig>"  (e.g. __ct__12CPVGDIBitmapFv)
_MAC = re.compile(r"__(\d+)([A-Za-z_][A-Za-z0-9_]*)")


def mac_class(mangled: str) -> str:
    m = _MAC.search(mangled)
    if not m:
        return ""
    cls = m.group(2)[: int(m.group(1))]
    return cls if cls.startswith("C") else ""


def source_classes() -> set[str]:
    """Every C-prefixed class that is declared or defined in the source tree."""
    decl = re.compile(r"\b(class|struct)\s+(C[A-Za-z0-9_]+)\b")
    defined = re.compile(r"\b(C[A-Za-z0-9_]+)::")
    classes: set[str] = set()
    for path in SRC.rglob("*"):
        if not path.is_file() or path.suffix not in (".cpp", ".h", ".C", ".H"):
            continue
        text = path.read_text(encoding="utf-8", errors="ignore")
        for m in decl.finditer(text):
            classes.add(m.group(2))
        for m in defined.finditer(text):
            classes.add(m.group(1))
        # a file like CDoorAct.cpp implies its own class
        cpl = re.match(r"(C[A-Za-z0-9_]+)", path.stem)
        if cpl:
            classes.add(cpl.group(1))
    return classes


def blueprint():
    """address -> (mac class, mac func) and per-class function list."""
    by_addr: dict[int, tuple[str, str]] = {}
    members: dict[str, list[str]] = defaultdict(list)
    with CORR.open(newline="", encoding="utf-8-sig") as f:
        for row in csv.DictReader(f):
            mn = row.get("mac_mangled_name", "")
            cls = mac_class(mn)
            fn = mn.split("__", 1)[0] if mn else ""
            by_addr[int(row["x86_address"], 16)] = (cls, fn)
            if cls:
                members[cls].append(fn)
    return by_addr, members


def reccmp_impl() -> dict[int, bool]:
    if not RECCMP.is_file():
        return {}
    data = json.loads(RECCMP.read_text(encoding="utf-8"))
    return {int(r["address"], 16): bool(r.get("recomp")) for r in data.get("data", [])}


def zero_ratio_fns():
    """unowned, non-compiler-generated functions with ratio 0 (address, name)."""
    impl = reccmp_impl()
    compiler_gen = set()
    cg = ROOT / "data" / "reccmp-compiler-generated.csv"
    if cg.is_file():
        with cg.open(newline="", encoding="utf-8-sig") as f:
            compiler_gen = {int(r["address"], 16) for r in csv.DictReader(f) if r.get("address")}
    out = []
    with INV.open(newline="", encoding="utf-8-sig") as f:
        for r in csv.DictReader(f):
            a = int(r["address"], 16)
            if a in compiler_gen or r.get("name", "").startswith(("_", "??_")) or r["name"].startswith("FUN_"):
                continue
            # skip owned (blueprint-correlated) addresses
            if a in _own:
                continue
            m = impl.get(a)
            if m is False or a not in impl:
                out.append((a, r["name"]))
    return out


_own: set[int] = set()


def load_owned():
    with CORR.open(newline="", encoding="utf-8-sig") as f:
        for row in csv.DictReader(f):
            _own.add(int(row["x86_address"], 16))


# crude verb->method-name remap, kept Mac/CodeWarrior-style (CamelCase)
_VERBS = {
    "construct": "Construct", "destroy": "Destroy", "delete": "Delete",
    "append": "Add", "remove": "Remove", "emit": "Emit", "advance": "Step",
    "refresh": "Refresh", "dispatch": "Dispatch", "handle": "Handle",
    "load": "Load", "save": "Save", "update": "Update", "set": "Set",
    "get": "Get", "find": "Find", "init": "Initialise", "reset": "Reset",
    "create": "Create", "process": "Process", "copy": "Copy", "decode": "Decode",
    "serialize": "Serialize", "route": "Route", "parse": "Parse", "move": "Move",
    "begin": "Begin", "reload": "Reload", "activate": "Activate", "deactivate": "Deactivate",
}


def _camel(tok: str) -> str:
    return tok[:1].upper() + tok[1:] if tok else tok


def propose_method(name: str) -> str:
    """snake_case Ghidra name -> blueprint-consistent CamelCase method name."""
    parts = name.replace("_", " ").split()
    if not parts:
        return _camel(name)
    verb = parts[0]
    if verb in _VERBS:
        rest = "".join(_camel(t) for t in parts[1:])
        return _VERBS[verb] + rest
    return "".join(_camel(t) for t in parts)


def propose_class(address: int, by_addr, name: str) -> str:
    """Class for a zero fn: (1) sniff the snake_case noun; (2) else bracket the
    address against the nearest blueprint-correlated x86 address and take its class."""
    # (1) noun sniff
    for tok in name.replace("_", " ").split():
        t = tok.lower()
        for stem, cls in (
            ("invs", "CInvisibleSwitch"), ("gmob", "CGlobalGameObject"),
            ("enmy", "CEnemy"), ("coll", "CCollision"), ("plas", "CPlasma"),
            ("boon", "CBalloonPost"), ("lift", "CLift"), ("door", "CDoor"),
            ("rock", "CRock"), ("tram", "CTrampoline"), ("anim", "CAnim"),
            ("pgun", "CPaintGun"), ("mine", "CMine"), ("lemming", "CPlayerLemming"),
            ("sheep", "CSheep"), ("slinky", "CSlinky"), ("balloon", "CBalloon"),
            ("projectile", "CProjectile"), ("move", "CMover"),
        ):
            if t.startswith(stem) or stem.lower() in t:
                return cls
    # (2) address bracket: nearest correlated address's class, within a 4KB window
    best = None; bd = 0x1000
    for ca, entry in by_addr.items():
        clsname = entry[0] if isinstance(entry, tuple) else entry
        if not clsname:
            continue
        d = abs(ca - address)
        if d < bd:
            bd = d; best = clsname
    return best or ""


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--missing", action="store_true", help="list missing blueprint classes")
    ap.add_argument("--zeros", action="store_true", help="list 0% game functions")
    ap.add_argument("--legacy", action="store_true", help="list legacy source files")
    args = ap.parse_args()

    by_addr, members = blueprint()
    src = source_classes()
    load_owned()

    if args.missing:
        miss = sorted(set(members) - src)
        print(f"blueprint classes: {len(members):4d}")
        print(f"source classes:    {len(src):4d}")
        print(f"missing:           {len(miss):4d}")
        for c in miss:
            print(f"  {len(members[c]):3d} fn  {c}")
        return 0

    if args.zeros:
        zeros = zero_ratio_fns()
        print(f"zero-ratio game functions: {len(zeros)}")
        for a, n in sorted(zeros):
            cls = propose_class(a, by_addr, n)
            meth = propose_method(n)
            tag = f"  -> {cls}::{meth}( )" if cls else f"  -> {meth}( )  [class TBD]"
            print(f"  {n}{tag}")
        return 0

    if args.legacy:
        # Legacy files = original sources kept with MS-DOS-style UPPERCASE extension
        # (.CPP/.H/.C). The project's migrated files use lowercase (.cpp/.h). These
        # hold live reconstructed code, so they are flagged for progressive migration
        # (rename to CClass.cpp + move to a class-structured path), never deleted in place.
        legacy = sorted(
            p.relative_to(ROOT).as_posix()
            for p in SRC.rglob("*")
            if p.is_file() and p.suffix in (".CPP", ".H", ".C")
        )
        print(f"legacy source files ({len(legacy)}):")
        for rel in legacy:
            print(f"  {rel}")
        return 0

    # default summary
    miss = sorted(set(members) - src)
    zeros = zero_ratio_fns()
    print(f"source classes present : {len(src)} / {len(members)} blueprint classes")
    print(f"missing blueprint classes: {len(miss)}")
    print(f"zero-ratio game fns     : {len(zeros)}")
    print("run with --missing / --zeros / --legacy for detail")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
