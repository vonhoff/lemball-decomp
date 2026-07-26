# Decompilation worker

Goal: improve byte matching inside one exclusive original-address range using
MSVC 4.20, Ghidra, and reccmp.

## Start

Coordinator creates each worker with `python tools\create_worker_worktree.py OWNER`
from clean integration checkout. This claims range, commits `CLAIMS.md`, creates
`codex/OWNER` worktree beside repository, prints assigned range. Agent works only
in that worktree; do not run `claims.py OWNER` from worker checkout. Keep claim
until coordinator merges worker commit, verifies integration build, then runs
`python tools\claims.py release OWNER` and commits release.

For single-worker local work only, `python tools\claims.py OWNER` claims or
resumes one range and prints unfinished functions.

Original entry address owns function. Inspect callers and callees anywhere,
but do not edit an out-of-range function. If local match needs such an edit,
record address and evidence in range CSV under `data/function-status/`, then
tell coordinator. Coordinate before changing shared class layouts, inheritance,
vtables, or globals.

## Work

- Use Ghidra MCP for disassembly, decompilation, xrefs, types, vtables, and
  thunks. Treat Ghidra output as evidence, not source.
- Preserve file placement, annotations, calling conventions, class layout,
  and link order when binary evidence requires them.
- `// FUNCTION: LEMBALL 0x...` must directly precede declaration. Put other
  comments above annotation.
- Write compiler-generated C/C++ only. Never use inline or external assembly,
  naked functions, or opcode bytes.
- For five-byte `E9` thunk, write annotated wrapper with matching signature and
  only target call. Rebuilt address may differ. Use existing `/include:` pragma
  pattern only when MAP/PDB proves linker discarded unreferenced wrapper; it
  retains symbol and must not be used to control placement.
- Cross-range target already present is not dependency unless target must
  change.

## Measure

From repository root:

```powershell
python tools/build_msvc420.py
.decomp-venv\Scripts\reccmp-project.exe detect --search-path data
Push-Location build-msvc420
..\.decomp-venv\Scripts\reccmp-reccmp.exe --target LEMBALL --json reccmp.json --json-diet
..\.decomp-venv\Scripts\reccmp-decomplint.exe --target LEMBALL
..\.decomp-venv\Scripts\reccmp-datacmp.exe --target LEMBALL --no-color
Pop-Location
```

100% reccmp result is only completion evidence. Before handoff, update useful
blocker or next-step notes in `data/function-status/<range>.csv`, rerun checks
available in worker worktree, commit source and notes. Coordinator runs full
integration build/reccmp before releasing claim. Never share uncommitted edits
between worker worktrees.
