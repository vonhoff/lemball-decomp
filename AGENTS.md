# Range worker

Goal: byte-match one claimed original-address range with MSVC 4.20, Ghidra,
and reccmp. Never grind or edit another range.

## Communication

Always terse. Fragments allowed. Drop filler, pleasantries, repeated facts,
decorative formatting, and long logs. Quote shortest decisive error. Use exact
technical terms; never invent prose abbreviations or use arrows. Progress
update contains candidate address, measured percentage, blocker, next step.
Final contains new exact matches, changed files, checks, unresolved blockers.

1. In cmd, run:
   `call msvc420\bin\VCVARS32.BAT x86`
   `python tools\build_msvc420.py`
   `reccmp-project detect --search-path data`
   `pushd build-msvc420`
   `reccmp-reccmp --target LEMBALL --json reccmp.json --json-diet --silent`
   `popd`
   Never call CMake build directly. Then run
   `python tools\claims.py take <range> <owner>`. Commit `CLAIMS.md` and
   generated baseline status CSV alone.
2. Run `claims.py brief <range>`. In Ghidra MCP inspect decompilation,
   disassembly, callers, callees, references, strings, vtables, and thunks.
   Claims contain at most 16 functions or about 1 KiB. Work one candidate at a
   time; never batch-decompile or survey whole range first.
3. Make at least one previously non-exact function 100%. Continue only when
   next candidate is trivial. A thunk belongs to range containing its entry,
   not target.
4. After each attempt rebuild and refresh reccmp, then run
   `python tools\claims.py status <range> <owner> <address> <state> "<notes>"`.
   Use `matching` for measured partial work and `blocked` only with evidence,
   blocker, and next step. `matched` requires current reccmp result of 100%.
5. Preserve annotations, translation units, link order, compiler constraints,
   and uncertain evidence. Do not perform cleanup unrelated to byte matching.
   Write compiler-generated C/C++ only. Never use `__asm`, `_asm`,
   `__declspec(naked)`, opcode bytes, or external assembly. Assembly-produced
   100% is invalid. Existing assembly is debt, not precedent.
6. Before changing shared class, vtable, inheritance declaration, or global,
   take and commit address-anchored ABI lease.
7. Rebuild and refresh reccmp with commands from step 1. Before handoff run:
   `python tools\claims.py verify <range> <owner>`
   `python tools\claims.py check --base HEAD`
   `python -m unittest discover -s tools\tests`
   In `build-msvc420`, run:
   `reccmp-decomplint --target LEMBALL`
   `reccmp-datacmp --target LEMBALL --no-color`
   Wrapper detects objects newer than executable and forces relink. Never
   compare after failed build.
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
uses ABI lease.

Progress source of truth is `data/function-status/<range>.csv`. Dependency
source of truth is `data/dependencies/<range>.csv`. Commit both with work so
another worker can resume from percentages, evidence, blocker, and next step.

Never edit `data/objdiff-functions.csv` manually; Ghidra export owns it. Never
infer original filenames from address proximity.
