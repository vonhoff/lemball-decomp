# Decompilation worker

Goal: improve byte matching inside one exclusive original-address range using
MSVC 4.20, Ghidra, and reccmp.

## Start

1. Run `python tools\coordinator.py`. It creates missing `.decomp-venv` and
   repairs missing reccmp packages from pinned `requirements.txt`.
2. Pick first viable target it prints. Before editing, state: `Target:
   <address>; <source file>; <binary evidence>; <intended change>.`
3. You own this checkout while working. No other agent may edit or build until
   your commit completes.

Original entry address owns function. Inspect callers and callees anywhere. If
local match needs another edit, leave it for later. Coordinate before changing
shared class layouts, inheritance, vtables, or globals.

## Loop

1. Inspect Ghidra and local source before inferring code. Establish signature,
   calling convention, control flow, xrefs, and relevant bytes.
2. Change one target only. Never use an unverified decompilation as source.
3. Run build and measurements below. Reinspect evidence after each mismatch.
4. If an error is outside target or source of failure is unclear, report exact
   blocker and stop. Do not repair unrelated code.
5. Commit intentional source changes before releasing checkout.

Report only target, evidence, result, and blocker. Keep reasoning private.

## Constraints

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

100% reccmp result is only completion evidence. Before handoff, rerun checks
available in this checkout, verify `git status` contains only intentional
files, then commit source.
