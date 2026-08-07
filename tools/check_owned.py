#!/usr/bin/env python
"""Check whether LEMBALL addresses are already owned by a source `// FUNCTION` annotation.
Usage: python tools/check_owned.py [0xADDR ...]  |  cat a list of addresses on stdin
Prints each address that is NOT owned (safe to reconstruct), and warns on owned ones.
Also flags addresses already present in reccmp's tracked set.
"""
import json, os, re, sys

pat = re.compile(r"// FUNCTION: LEMBALL 0x([0-9a-fA-F]{8})")

def load_owned():
    owned = {}
    for root, dirs, files in os.walk("src"):
        for fn in files:
            if not fn.lower().endswith((".cpp", ".h")):
                continue
            p = os.path.join(root, fn)
            try:
                txt = open(p, encoding="utf-8", errors="ignore").read()
            except OSError:
                continue
            for m in pat.finditer(txt):
                owned.setdefault(int(m.group(1), 16), []).append(p)
    return owned

def load_tracked():
    try:
        d = json.load(open("build-msvc400/reccmp.json"))["data"]
        return {int(x["address"], 16): x for x in d}
    except OSError:
        return {}

def main():
    args = [a for a in sys.argv[1:] if a]
    if not args:
        return
    owned = load_owned()
    tracked = load_tracked()
    for a in args:
        addr = int(a, 16)
        if addr in owned:
            print(f"OWNED  {a} -> {', '.join(owned[addr])}")
        else:
            tag = "TRACKED" if addr in tracked else "UNOWNED"
            print(f"{tag:8} {a}")

if __name__ == "__main__":
    main()
