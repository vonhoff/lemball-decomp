# Decompilation worker

Goal: maximize verified decompilation progress while preserving correctness.

## Start

1. Inspect `git status` and take responsibility for every existing change. Never stop merely because the worktree is dirty.
2. Run `python tools\coordinator.py`.
3. Select the first viable coordinator target.
4. Before editing, report:

```
Target: <address>; <source>; <binary evidence>; <planned change>
```

## Workflow

Treat the original binary as authoritative.

For every target:

1. Inspect Ghidra before editing.
2. Verify:
    * raw bytes
    * disassembly
    * function boundaries
    * calling convention
    * control flow
    * xrefs
3. Treat decompiler output only as supporting evidence.
4. Modify only C/C++ source.
5. Build and measure.
6. Reinspect binary evidence after every mismatch.
7. Continue while producing verified progress.

Treat all worktree changes as worker-owned. Inspect, verify, complete, revert, or
commit them as appropriate; never leave them for the user to resolve.

## Progress

Progress includes:

* a new 100% reccmp match;
* a measurable improvement toward a match;
* verified inventory corrections;
* verified Ghidra/export corrections;
* reusable compiler-accurate infrastructure;
* cleanup that removes incorrect implementations.

Commit any verified progress that leaves the repository in a better state.

Prefer small, self-contained commits.

## Blockers

If progress stops:

1. Remove speculative edits.
2. Restore a clean worktree.
3. Record exact binary evidence.
4. Continue with the next coordinator target whenever possible.

Stop only when a repository-wide blocker prevents further work.

## Constraints

* Use Ghidra MCP for analysis.
* Treat Ghidra as evidence, not source.
* Reload rebuilt programs before using them.
* Preserve binary-required layout, ABI, annotations, and link order.
* Write compiler-generated C/C++ only.
* Never use:
    * assembly
    * naked functions
    * embedded opcode bytes
    * binary patching
* Handle five-byte `E9` thunks with normal annotated C/C++ wrappers matching the original ABI.
* If inventory data is incorrect, repair it and regenerate the CSV.
* Never hide invalid entries using denylists or blanket filters.

## Diagnosis

Use the original virtual address.

```powershell
Push-Location build-msvc420
..\.decomp-venv\Scripts\reccmp-reccmp.exe --target LEMBALL --verbose 0xADDRESS --print-rec-addr
Pop-Location
```

## Measurement

```powershell
python tools/build_msvc420.py
.decomp-venv\Scripts\reccmp-project.exe detect --search-path data
Push-Location build-msvc420
..\.decomp-venv\Scripts\reccmp-reccmp.exe --target LEMBALL --json reccmp.json --json-diet
..\.decomp-venv\Scripts\reccmp-decomplint.exe --target LEMBALL
..\.decomp-venv\Scripts\reccmp-datacmp.exe --target LEMBALL --no-color
Pop-Location
```

Before every commit:

* rerun available verification;
* account for every worktree change and leave `git status` clean;
* run `git diff --check`;
* stage files explicitly;
* commit with a descriptive message;
* push.

End with a concise plain-English summary of one or two sentences. State what changed and whether it was committed and pushed. Mention a blocker only when one prevented progress. Do not use a rigid report template, repeat command output, or describe routine verification steps.
