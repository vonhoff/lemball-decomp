# AGENTS.md

*Lemmings Paintball* reconstruction: `LEMBALL.EXE`, MSVC 4.00.

## Style

- Documents and communication: telegraphese. Short phrases; concrete facts; explicit uncertainty.
- Tools: single responsibility; upstream APIs first. Remove unused features and duplicate logic.

## Evidence

- `data/LEMBALL.EXE` and original x86: behavior and ABI evidence. Ghidra: analysis; PDB: rebuilt symbols.
- Structural source exploration: codebase-memory skill; verify against source.
- `tools/data/original-symbols.csv`: original extents and matching overrides; reccmp supplies import-thunk sizes.
- `tools/data/mac-symbol-catalog.csv`: Mac symbols with optional Windows mappings. Naming/type evidence; Windows ABI differences require Windows evidence.
- Matching: validation of a source hypothesis. Qualify inferred names, types, layouts; no score-only source tricks.
- `README.md`, `Manifest.h`, reference hashes, compiler flags: edit only when asked.

## Reconstruction priorities

- Recover plausible original source structure: clean, idiomatic C++ compatible with MSVC 4.00. Preserve Windows behavior and ABI; investigate original callers, callees, data, and control flow before changing code.
- Original Windows x86 is authoritative. Macintosh symbols, resources, and cross-platform code support names, types, and source hypotheses; resolve platform differences against Windows evidence.
- Reasoned inference is allowed. Prefer the simplest coherent hypothesis supported by the available evidence. Record evidence, uncertainty, alternatives, and validation in reconstruction memory or audit records; leave unknown roles explicit.
- Matching measures a hypothesis; it is not the source-design objective. Retain a cleaner, supported reconstruction when behavior and ABI are preserved, even if instruction-match scores fall. Inspect and document the differences and trade-offs. A higher score alone does not establish correctness.
- No score-driven register assignments, inline assembly, compiler controls, artificial branches, temporary aliases, casts, padding, lifetime scaffolding, declaration/include ordering, or duplicate code. Retain special compiler or ABI mechanisms only with independent evidence for their purpose; document that evidence.
- Do not search syntax variants, variable order, or visibility arrangements merely to steer code generation. Try alternative source structures when new semantic or cross-platform evidence supports them.
- Recover typed fields, arrays, inheritance, ownership, and shared operations. Avoid raw offsets into known runtime objects, instruction-by-instruction transcription, and manual allocation/construction when ordinary C++ expresses the evidenced lifetime.
- Use consistent, meaningful names and evidenced literals. Consolidate repeated constants and expressions only when a shared meaning is supported. Do not invent semantic roles or relationships from a numerical coincidence.
- Remove redundant declarations, duplicate implementations/includes, and unnecessary guards, casts, qualifiers, or conversions after checking their language, ABI, and runtime effects. Preserve required narrowing, signedness, overflow behavior, access timing, and serialized layouts.
- Validate source hypotheses through compilation, source checks, reccmp, and relevant behavior/layout checks. Finite tests and matching scores do not prove every execution path; record material limits. Commit verified changes in logical batches.

## Commands

| Script | Purpose |
| --- | --- |
| `tools/make_binary.py` | Build executable; `--clean-first` for full clean rebuild |
| `tools/link_binary.py` | CMake toolchain MSVC 4.00 linker adapter |
| `tools/check_function.py 0xADDR` | Compare/diff function against target; `--summary` to omit diffs |
| `tools/triage_targets.py` | Select unfinished functions; `--exact` for raw, `--min-size N --sort size` for larger targets |
| `tools/check_source.py` | Source policy and annotation checks |
| `tools/recover_thunks.py --verify` | Compiled catalog identities, mapped implementation coverage, and linker routes |
| `tools/make_report.py` | Canonical reports plus console Effective score |
| `tools/make_badges.py` | README badges, separate from canonical progress |

Deep comparison: `reccmp-vtable` / `reccmp-stackcmp` / `reccmp-datacmp` from `build-msvc400`.
Progress uses original bytes. Exact: non-stub raw 100%; Effective: non-stub raw 100% or reccmp equivalence. Stubs contribute zero; fuzzy retains raw scores.
`report.json`: objdiff progress. `effective.json`: additional Effective addresses and report hash; Exact matches already reside in `report.json`. Badges and triage consume this pair.
Retain incremental build outputs and saved reports in `build-msvc400`. Remove temporary trial artifacts only; clean builds for stale artifacts, not routine batch cleanup.

## Documentation changes

Documentation-only edits, including `README.md` and `AGENTS.md`: review wording and links; run `git diff --check`.
Do not run gate, builds, matching, or reports for documentation-only edits unless explicitly requested.
The source reconstruction workflow below applies to C/C++ source changes. Tool changes: run gate and relevant tool checks.

## Source changes

1. Select a focused target with `tools/triage_targets.py` from the current report; sole selection/ranking entry point. Generate missing reports with `tools/make_report.py` after building. When reconstruction-memory tools are available: `get_function_memory(addr)` before editing.
2. Read the full function, declarations, relevant original callers/callees. Preserve ABI, dispatch, side effects, reload timing, narrowing, ownership, initialization, allocation failures. Ambiguous diff: inspect raw x86.
3. Build and match trials with `tools/make_binary.py && tools/check_function.py 0xADDR`. Use `--summary` for compact multi-address checks. Revert trials that break behavior/ABI or lack source evidence; retry with new evidence. A matching decrease alone is not a failed trial. `record_attempt`: actual trials only. `record_observation`: durable x86 facts with address/citation. Tools unavailable: continue locally.
4. Batch boundary: `tools/make_report.py` to inspect progress and diff against git. Header/ABI/multi-TU changes: `detect_changes`. Explain regressions. No full reports per speculative trial.
5. Clang-format touched C/C++; run `tools/check_source.py`, `tools/recover_thunks.py --verify`, and relevant checks; commit verified work.

## Source conventions

- Code comments: functional only; reccmp annotations/symbols, layout offsets/sizes, and format controls.
- Class files: stem = primary class; free-function files allowed. Class definitions identical across TUs. Annotated functions: ascending original address.
- Use typed objects, members, indexing, base conversions. Derive extents/strides from allocations and accesses; distinguish serialized and runtime layouts.
- Allocations: typed `new` / `delete` and matching array forms where lifetimes support them; `sizeof(Type)` / `count * sizeof(*elements)` for byte counts. Byte cursors for streams, pixels, and serialized data.
- Calling conventions: arguments, forwarding, cleanup; zero-argument `RET` alone insufficient. Qualified base calls require direct-dispatch evidence.
- Constants: evidenced meaning; verify resource IDs against Manifest/RC. Preserve original assertion filenames when renaming files.
- Reccmp annotations: original Windows addresses; STUB promotion only when substantially implemented.
- Catalog checks: mapped Windows addresses and class/method names. Windows ABI and behavior follow x86; unmapped names do not block.
