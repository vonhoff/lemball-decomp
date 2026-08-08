#!/usr/bin/env python
# Rename ClusterScanOps2.cpp function identifiers + MACINTOSH comments to Mac-authenticated names.
# Uses the repo's tools.macintosh_naming.objdiff_name for correct demangling (ctors/dtors/operators).
# Only renames functions whose C-identifier has <=1 reference (definition only) in the tree (safe).
import csv, re, os, sys
sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..") if os.path.basename(os.getcwd()) != "lemball-decomp" else ".")
sys.path.insert(0, ".")
from tools.macintosh_naming import objdiff_name

SRC = r"C:\dev\lemball-decomp\src"
TARGET = os.path.join(SRC, "Control", "ClusterScanOps2.cpp")

rows = list(csv.reader(open(r"C:\dev\lemball-decomp\data\macintosh-x86-correlations.csv")))
x86mac = {int(r[3], 16): r[2] for r in rows[1:] if len(r) >= 5 and r[3].startswith("0x")}

txt = open(TARGET, encoding="utf-8").read()
plan = []
cnt=0
for m in re.finditer(r'// FUNCTION: LEMBALL 0x([0-9a-fA-F]{6,8})\n((?:(?!// FUNCTION)[^\n]*\n)*?)(\w+)\s+__fastcall\s+(\w+)\s*\(', txt):
    a = int(m.group(1), 16)
    cid = m.group(4)
    if a not in x86mac:
        continue
    try:
        nm = objdiff_name(x86mac[a])
    except Exception as e:
        cnt+=1; continue
    # flatten Class::Method to valid C identifier for the function symbol
    flat = nm.replace("::", "_").replace("~", "~")  # destructor ~ stays
    # sanitize to valid C identifier
    flat = re.sub(r'[^A-Za-z0-9_~]', '_', flat)
    plan.append((a, cid, flat, nm))

def count_refs(cname):
    n = 0
    for root, dirs, files in os.walk(SRC):
        for fn in files:
            if not fn.lower().endswith(".cpp"):
                continue
            t = open(os.path.join(root, fn), encoding="utf-8", errors="ignore").read()
            n += len(re.findall(r'\b' + re.escape(cname) + r'\s*\(', t))
    return n

renames, skipped = [], []
for a, cid, flat, nm in plan:
    refs = count_refs(cid)
    if refs <= 1:
        renames.append((a, cid, flat, nm))
    else:
        skipped.append((a, cid, flat, refs))

print(f"PLAN: {len(plan)} Mac-correlated candidates, {len(renames)} safe-to-rename")
for a, cid, flat, nm in renames[:30]:
    print(f"    RENAME 0x{a:x} {cid:<44} -> {flat}   // {nm}")
if skipped:
    print(f"  SKIPPED (external callers) {len(skipped)}:")
    for a, cid, flat, refs in skipped[:8]:
        print(f"    SKIP 0x{a:x} {cid} ({refs} refs)")
with open(r"C:\dev\lemball-decomp\tools\_rename_plan.txt", "w") as f:
    for a, cid, flat, nm in renames:
        f.write(f"{a:=#x}\t{cid}\t{flat}\t{nm}\n")
print(f"\nplan: tools/_rename_plan.txt ({len(renames)} renames)")
