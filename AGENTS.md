# Decompilation worker

## Objective

Make verified decompilation progress while preserving compiler behavior. Treat the original binary as authoritative.

## Workflow

1. Inspect `git status`, every existing diff, and `// STUB:` markers. Continue unfinished work before selecting a new target; complete, commit, or revert it rather than leaving it for the user.
2. Refresh the canonical MSVC 4.00 build and reccmp JSON when missing or stale, then run `.decomp-venv\Scripts\python.exe tools\find_decompilation_candidates.py` to print candidates. Treat `exact-baseline.json` as immutable; never save or regenerate it during reconstruction.
3. Use printed repeated normalized instruction shapes before choosing a singleton. Preserve instruction order, branches, stack cleanup, calling convention, object offsets, and function size; normalization covers only concrete addresses, relocations, and per-member constants.
4. Work highest-ranked viable cluster. Prefer existing 100% exemplars and compiler-owned C++ patterns such as constructors, destructors, scalar deletes, vtable restores, accessors, and forwarding or adjustment thunks.
5. Inspect every proposed cluster member in original `/LEMBALL.EXE` with Ghidra MCP. Establish raw bytes, boundaries, ABI, control flow, xrefs, owning source file, and all per-member variations. Use decompiler output only as supporting evidence. ILT entries never become targets.
6. Recover one minimal C or C++ template from binary evidence and compiler probes. Parameterize only proven variations. Never mass-generate speculative bodies from shape similarity alone.
7. Implement complete viable cluster, build once, compare representative member, then compare every member with verbose reccmp. Revise from assembly diffs and binary evidence.
8. Run full reccmp JSON after each batch, then run `.decomp-venv\Scripts\python.exe tools\find_decompilation_candidates.py --check-baseline`. Keep batch only if exact-set gate passes and it adds verified matches or another form of verified progress. Revert speculative or regressing edits.
9. Repeat next viable high-yield cluster. Do not stop after one tiny function while safe members of same proven family remain.
10. When no viable repeated family remains, work first viable entry under miner's `singleton fallback`. It prefers fewest estimated mismatched bytes and investigates five-byte functions last.
11. Fix discovered issues immediately. If fix cannot be completed in current run, leave minimal compiling stub with `// STUB:` marker for next run.
12. Stop only for repository-wide blocker.

Verified progress includes a new 100% match, measurable similarity improvement, corrected inventory or Ghidra data, compiler-accurate infrastructure, or removal of an incorrect implementation.

## Constraints

- Preserve required ABI, layout, annotations, source placement, and link order.
- Use compiler-generated C or C++ except for proven real ABI/vtordisp thunks outside the ILT. Use assembly only after C++ compiler probes fail, and mark it `// ABI-THUNK:` with original bytes, ABI, failed C++ form, and verified reccmp result. Never use embedded opcodes or binary patches.
- Never use assembly or source wrappers to recreate the linker ILT. Analyze five-byte `E9` functions outside the ILT normally.
- Repair incorrect inventory and regenerate its CSV; do not hide entries with denylists or broad filters.
- Add `GLOBAL`, `FUNCTION`, or `STUB` ownership only after confirming original address and source owner in Ghidra. Never add annotations merely to make relocation comparison pass. Investigate every new datacmp issue; direct-function versus original ILT-pointer differences are acceptable only when binary evidence proves same destination and no ILT recreation is introduced.
- Use maintained candidate miner; do not create one-off target-selection or verification scripts. Temporary compiler probes may live only under ignored `build-msvc400/` and must be removed before commit.
- Use `clang-format -i <paths>` from `PATH` to fix formatting in modified C or C++ files.
- Use only project commands below for verification.

## Binary layout

- Original LINK 3.00 incremental link table occupies `0x00401000` through `0x00403890`: 2,077 five-byte `E9` entries through `0x0040388C`, then padding. Real code begins at `0x00406160`.
- Ghidra marks every ILT entry `linker-ilt`; none may retain an `objdiff-unit:*` tag. Source-emitted ILT wrappers use `// LINKERILT:` rather than `FUNCTION` or `STUB`; reccmp aliases this marker to non-progress `STUB` only for relocation normalization. By-name ILT inventory belongs in `data/reccmp-linker-ilts.csv`, while compiler-generated symbols without standalone implementations belong in `data/reccmp-compiler-generated.csv`. Compiler-state shims may remain unannotated only when comments identify slot, destination, and verified regression caused by removal.
- Keep MSVC 4.00 and `/INCREMENTAL:YES`; compare linker thunks by their destination identity rather than recreating them in source.
- Whole-image parity remains incomplete: linker generation is aligned at 3.00, but `.text` size, resources, CRT, imports, and object order still differ.

## Commands

Candidate mining, from repository root:

```powershell
.decomp-venv\Scripts\python.exe tools\find_decompilation_candidates.py
.decomp-venv\Scripts\python.exe tools\find_decompilation_candidates.py --check-baseline
```

Focused diagnosis, from `build-msvc400`:

```powershell
..\.decomp-venv\Scripts\reccmp-reccmp.exe --target LEMBALL --verbose 0xADDRESS --print-rec-addr
```

Canonical verification, from repository root:

```powershell
.decomp-venv\Scripts\python.exe tools\lint_reccmp_metadata.py
python tools/build_msvc400.py
.decomp-venv\Scripts\reccmp-project.exe detect --search-path data
Push-Location build-msvc400
..\.decomp-venv\Scripts\reccmp-reccmp.exe --target LEMBALL --json reccmp.json --json-diet
..\.decomp-venv\Scripts\reccmp-decomplint.exe --target LEMBALL
..\.decomp-venv\Scripts\reccmp-datacmp.exe --target LEMBALL --no-color
Pop-Location
```

## Completion

Before committing:

1. Run the canonical verification commands and `git diff --check`.
2. Review every diff and stage explicit paths.
3. Commit verified progress with a descriptive message and push.
4. Leave the worktree clean.

Report the result in one or two plain-English sentences. State what changed and whether it was committed and pushed. Mention a blocker only if it prevented progress.
