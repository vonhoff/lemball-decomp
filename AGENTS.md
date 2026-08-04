# Decompilation worker

## Objective

Make verified decompilation progress while preserving compiler behavior. `/LEMBALL.EXE` is authority.

## Workflow

1. Inspect `git status`, all diffs, and `// STUB:` markers. Finish, commit, or revert unfinished work.
2. Refresh canonical MSVC 4.00 build/reccmp JSON when stale. Keep `exact-baseline.json` immutable.
3. Choose repeated normalized instruction shapes first. Preserve order, branches, stack cleanup, ABI, offsets, and size; normalize only addresses, relocations, and proven member constants.
4. Prefer 100% exemplars and compiler-owned families: constructors, destructors, scalar deletes, vtable restores, accessors, forwarding/adjustment thunks.
5. Inspect every candidate member in original `/LEMBALL.EXE` with Ghidra MCP: bytes, boundaries, ABI, flow, xrefs, owner, and variations. ILT entries are never targets.
6. Recover one minimal C/C++ template from binary evidence and compiler probes. Parameterize only proven variations; never mass-generate from shape alone.
7. Implement the complete viable family, build, compare a representative, then every member. Revise from assembly diffs and binary evidence.
8. Run full reccmp JSON and `--check-baseline` after each batch. Exit `1` is expected for tolerated losses; never accept a new lost address. Keep batches only for verified gains or measured improvement without worsening the lost set.
9. Repeat safe members of the family; then investigate remaining singletons by fewest mismatched bytes, five-byte functions last.
10. Fix issues immediately. If blocked, leave a compiling `// STUB:` marker.

Verified progress: new 100% match, similarity gain, corrected inventory/Ghidra data, compiler-accurate infrastructure, or removed incorrect code.

## Macintosh sibling-build evidence

Japanese Macintosh CODE resources provide terminology and logical ownership, not Windows ABI evidence.

1. Inventory raw resources and symbols before assigning Windows names.
2. Correlate names to `/LEMBALL.EXE` by behavior, constants, strings, callers, tables, subsystem, and signature; record confidence/evidence.
3. Use corroborated Macintosh names only after independent x86 correlation; update Ghidra, source, and reccmp metadata together.
4. Use Macintosh logical class/file ownership while preserving proven Windows physical TUs with exact-position includes/views. No `// MACINTOSH:` comments.
5. Treat Macintosh Toolbox, CFM, filesystem, sound, display, and input code as terminology only unless x86 behavior proves equivalence.
6. Never transfer Macintosh layout, vtables, thunks, calling conventions, RTTI, exceptions, or code generation to Windows. `/LEMBALL.EXE` and VC4.00 probes win.
7. Preserve raw sibling binaries and reproducible symbol inventories; do not normalize away mangled names.

### Macintosh evidence files and checks

- `data/macintosh-68k-symbols.csv`: raw symbols. `data/macintosh-x86-correlations.csv`: reviewed mappings. `data/macintosh-symbol-coverage.csv`: every raw-symbol disposition.
- Run `.decomp-venv\Scripts\python.exe tools\macintosh_naming.py check`; use `summary` and `names` for live reports. `tools/generate_objdiff_report.py` produces the required grouped progress report from reccmp output.
- Coverage key: `(mac_code_file, mac_name_length_offset, mac_mangled_name)`. Every row has one disposition. Accepted mappings require proven x86 address, canonical unit, source path or `unreconstructed`, Windows identity, and behavioral evidence. Inline/merged and platform-specific rows require evidence/reason.

- VSGDI mappings live in `data/macintosh-x86-correlations.csv`; keep platform hooks, DirectDraw adapters, upload helpers, and compiler adjustor thunks Windows-owned until proven. `CPVSurface`/`CPVScrollableSurface`/`CSurface` topology and `0x5A0` layout are Windows-only evidence.

## Constraints

- Do not edit `README.md` unless requested.
- Preserve ABI, layout, annotations, source placement, and link order.
- Use compiler-generated C/C++; use assembly only for proven real ABI/vtordisp thunks after failed C++ probes. Mark it `// ABI-THUNK:` with original bytes, ABI, failed form, and reccmp result. Never embed opcodes or recreate ILT wrappers.
- Repair inventories; never hide entries with denylists. Add ownership only after Ghidra confirms address and source owner. Investigate every datacmp issue.
- Temporary probes belong only in ignored `build-msvc400/`. Run `clang-format -i <paths>` on modified C/C++.
- Use project commands below for verification.

## Binary layout

Original LINK 3.00 ILT: `0x00401000`–`0x00403890`, 2,077 five-byte `E9` entries through `0x0040388C`; real code starts `0x00406160`. Ghidra marks ILT as `linker-ilt`; source wrappers use `// LINKERILT:`. By-name ILTs belong in `data/reccmp-linker-ilts.csv`; standalone compiler-generated symbols in `data/reccmp-compiler-generated.csv`. Keep MSVC 4.00 `/INCREMENTAL:YES`; compare thunk destination identity. Whole-image parity is incomplete across `.text`, resources, CRT, imports, and object order.

`data/reccmp-*.csv` are configured in `reccmp-project.yml` and use `address,name,symbol,type,source`; `objdiff-functions.csv` uses `address,size,name,unit`. Do not replace these inventories with broad filters or duplicated summaries.

## Commands

Focused diagnosis, from `build-msvc400`:

```powershell
..\.decomp-venv\Scripts\reccmp-reccmp.exe --target LEMBALL --verbose 0xADDRESS --print-rec-addr
```

Canonical verification, from repository root:

```powershell
.decomp-venv\Scripts\python.exe tools\lint_reccmp_metadata.py
.decomp-venv\Scripts\python.exe tools\install_reccmp_compat.py
.decomp-venv\Scripts\cmake.exe --fresh --preset msvc400
.decomp-venv\Scripts\cmake.exe --build --preset msvc400 --clean-first
.decomp-venv\Scripts\reccmp-project.exe detect --search-path data
Push-Location build-msvc400
../.decomp-venv/Scripts/reccmp-reccmp.exe --target LEMBALL --json reccmp.json --json-diet
../.decomp-venv/Scripts/reccmp-vtable.exe --target LEMBALL
../.decomp-venv/Scripts/reccmp-decomplint.exe --target LEMBALL
../.decomp-venv/Scripts/reccmp-datacmp.exe --target LEMBALL --no-color
Pop-Location
```

## Completion

Run canonical verification and `git diff --check`; review diff; stage explicit paths; commit/push verified progress; leave worktree clean. Report change and commit/push status in one or two sentences.

## Exact-set audit

`data/exact-baseline.json` is immutable. After rebuilding `build-msvc400/reccmp.json`, compare added/lost addresses; keep measured gains without new lost baseline addresses. Historical checkpoints are not current truth.
