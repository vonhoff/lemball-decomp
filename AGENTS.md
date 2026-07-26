# Range worker

Goal: byte-match one claimed original-address range with MSVC 4.20, Ghidra,
and reccmp. Never grind or edit another range.

1. Build, run reccmp to create `build-msvc420/reccmp.json`, then run
   `claims.py take <range> <owner>`. Commit `CLAIMS.md` and generated baseline
   status CSV alone.
2. Run `claims.py brief <range>`. In Ghidra MCP inspect decompilation,
   disassembly, callers, callees, references, strings, vtables, and thunks.
3. Make at least one previously non-exact function 100%, then continue other
   range functions. A thunk belongs to range containing its entry, not target.
4. After each attempt run `claims.py status`. Use `matching` for measured
   partial work and `blocked` only with evidence, blocker, and next step.
   `matched` requires current reccmp result of 100%.
5. Preserve annotations, translation units, link order, compiler constraints,
   and uncertain evidence. Do not perform cleanup unrelated to byte matching.
6. Before changing shared class, vtable, inheritance declaration, or global,
   take and commit address-anchored ABI lease.
7. Rebuild and refresh reccmp after source changes. Before handoff run
   `claims.py verify <range> <owner>`, `claims.py check`, and unit tests.
8. Merge while claims remain active. Release code and ABI claims afterward.

## Cross-range work

Address claim controls edits. Call graph only routes dependencies. Worker may
inspect any callee but must not edit out-of-range body.

When out-of-range correction blocks local match, run:

`claims.py dependency <range> <owner> <caller> <callee> "<evidence and need>"`

Continue other local functions. Coordinator assigns target range to another
worker. Pull merged target change, validate local range, then run
`claims.py resolve-dependency` with resolution evidence. Never duplicate target
body or silently widen claim.

For dependency cycle, coordinator may stage target stub or integrate both
branches. Each worker still edits only owned bodies. Shared type or ABI change
uses ABI lease. `data/function-edges.csv` is advisory Ghidra direct-call data;
virtual and indirect calls may be absent.

Progress source of truth is `data/function-status/<range>.csv`. Dependency
source of truth is `data/dependencies/<range>.csv`. Commit both with work so
another worker can resume from percentages, evidence, blocker, and next step.

Never edit `data/objdiff-functions.csv` or `data/function-edges.csv` manually;
Ghidra exports own them. Never infer original filenames from address proximity.
