#!/usr/bin/env python
# Apply rename plan: rename C-identifiers to semantic Class_Method form + REMOVE // MACINTOSH: comments.
import re, os, sys
PLAN = r"C:\dev\lemball-decomp\tools\_rename_plan.txt"
TARGETS = [
    r"C:\dev\lemball-decomp\src\Control\ClusterScanOps2.cpp",
    r"C:\dev\lemball-decomp\src\Control\ClusterScanOps.cpp",
]

plan = []
for line in open(PLAN, encoding="utf-8"):
    line = line.rstrip("\n")
    if not line:
        continue
    addr, cid, flat, nm = line.split("\t")
    plan.append((int(addr, 16), cid, flat, nm))

# Load objdiff name -> also derive semantic short method (last ::segment) for identifier
METHOD_OF = {}
for a, cid, flat, nm in plan:
    METHOD_OF[a] = nm

print(f"plan: {len(plan)} renames")

for TARGET in TARGETS:
    if not os.path.exists(TARGET):
        continue
    txt = open(TARGET, encoding="utf-8").read()
    new = txt
    n_def = 0
    for a, cid, flat, nm in plan:
        # rename definition occurrences
        defn = re.compile(r'\b' + re.escape(cid) + r'(?=\s*\()')
        hits = defn.subn(flat, new)
        new = hits[0]; n_def += hits[1]
    # remove ALL // MACINTOSH: comment lines
    before = len(re.findall(r'(?m)^// MACINTOSH:.*\n', new))
    new = re.sub(r'(?m)^\s*// MACINTOSH:.*\n', '', new)
    open(TARGET, "w", encoding="utf-8", newline="").write(new)
    total = sum(1 for _,cid,_,_ in plan if re.search(r'\b'+re.escape(cid)+r'(?=\s*\()', txt))
    print(f"{os.path.basename(TARGET)}: applied {n_def} identifier renames (planned {total}); removed {before} MACINTOSH lines")
