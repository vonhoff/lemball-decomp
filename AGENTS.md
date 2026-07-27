# Decompilation worker

## Objective

Make verified decompilation progress while preserving compiler behavior. Treat the original binary as authoritative.

## Workflow

1. Inspect `git status`, every existing diff, and `// STUB:` markers. Continue unfinished work before selecting a new target; complete, commit, or revert it rather than leaving it for the user.
2. If no unfinished work remains, run `python tools\coordinator.py` and select the first viable target.
3. Inspect the target in the original `/LEMBALL.EXE` program with Ghidra MCP. Coordinator addresses are original virtual addresses; pass them unchanged. ILT entries never become targets.
4. Establish raw bytes, disassembly, function boundaries, calling convention, control flow, and xrefs. Use decompiler output only as supporting evidence.
5. Identify the owning source file and implement one target in C or C++.
6. Build and compare the target with verbose reccmp. After a mismatch, use the assembly diff and binary evidence to revise the implementation.
7. If a target is already complete or cannot progress independently, remove speculative edits and continue with the next coordinator target.
8. Fix discovered issues immediately. If a fix cannot be completed in the current run, leave a minimal compiling stub with a `// STUB:` marker for the next run.
9. Stop only for a repository-wide blocker.

Verified progress includes a new 100% match, measurable similarity improvement, corrected inventory or Ghidra data, compiler-accurate infrastructure, or removal of an incorrect implementation.

## Constraints

- Preserve required ABI, layout, annotations, source placement, and link order.
- Use compiler-generated C or C++ except for proven real ABI/vtordisp thunks outside the ILT. Use assembly only after C++ compiler probes fail, and mark it `// ABI-THUNK:` with original bytes, ABI, failed C++ form, and verified reccmp result. Never use embedded opcodes or binary patches.
- Never use assembly or source wrappers to recreate the linker ILT. Analyze five-byte `E9` functions outside the ILT normally.
- Repair incorrect inventory and regenerate its CSV; do not hide entries with denylists or broad filters.
- Use `"/c/Program Files/LLVM/bin/clang-format.exe" -i <paths>` to fix formatting in modified C or C++ files.
- Use only the project commands below for verification. Do not create verification scripts.

## Binary layout

- Original LINK 3.00 incremental link table occupies `0x00401000` through `0x00403890`: 2,077 five-byte `E9` entries through `0x0040388C`, then padding. Real code begins at `0x00406160`.
- Ghidra marks every ILT entry `linker-ilt`; none may retain an `objdiff-unit:*` tag. ILT relocation mappings use `// LINKERILT:` rather than `FUNCTION` or `STUB`; reccmp aliases this marker to non-progress `STUB` only for relocation normalization. Compiler-state shims may remain unannotated only when comments identify slot, destination, and verified regression caused by removal.
- Keep MSVC 4.20 and `/INCREMENTAL:NO` for function-level comparison. `/INCREMENTAL:YES` changes generated thunk calls and lowers useful reccmp accuracy.
- Whole-image parity is deferred: original LINK is 3.00, current LINK is 4.20; original `.rsrc` virtual size is `0x55A4`, current is `0x160`; CRT and imports also differ.

## Commands

Focused diagnosis, from `build-msvc420`:

```powershell
..\.decomp-venv\Scripts\reccmp-reccmp.exe --target LEMBALL --verbose 0xADDRESS --print-rec-addr
```

Canonical verification, from repository root:

```powershell
python tools/build_msvc420.py
.decomp-venv\Scripts\reccmp-project.exe detect --search-path data
Push-Location build-msvc420
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
