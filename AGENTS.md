# AGENTS.md

*Lemmings Paintball* reconstruction: `LEMBALL.EXE`, MSVC 4.00.

## Style

- Documents and communication: telegraphese. Short phrases; concrete facts; explicit uncertainty.
- Tools: single responsibility; upstream APIs first. Remove unused features and duplicate logic.

## Evidence

- `data/LEMBALL.EXE` and original x86: behavior and ABI evidence. Ghidra: analysis; PDB: rebuilt symbols.
- Structural source exploration: codebase-memory skill; verify against source.
- `tools/data/mac-symbol-catalog.csv`: Mac symbols with optional Windows mappings. Naming/type evidence; Windows ABI differences require Windows evidence.
- Matching: validation of a source hypothesis. Qualify inferred names, types, layouts; no score-only source tricks.
- `README.md`, `Manifest.h`, reference hashes, compiler flags: edit only when asked.

## Commands

| Script | Purpose |
| --- | --- |
| `tools/build.py` | Build/link; `--clean-first` for stale PDB/build artifacts |
| `tools/match.py 0xADDR` | Build/compare/diff with raw and Effective scores; `--no-build` for current artifacts, `--summary` to omit diffs |
| `tools/next.py` | Select unfinished functions; Effective filtering, `--exact` for raw, `--min-size N --sort size` for larger targets |
| `tools/gate.py` | Source policy, annotation, and catalog checks |
| `tools/report.py` | Canonical reports plus console Effective score; `--check` snapshots the saved report and audits prior exact matches |
| `tools/badges.py` | README badges, separate from canonical progress |
| `python -m unittest discover -s tests` (from `tools/`) | Tool tests |

Deep comparison: `reccmp-stackcmp` / `reccmp-datacmp` from `build-msvc400`.
Canonical progress: exact = non-stub, raw 100% assembly comparison score. Stubs contribute zero.
Effective matches retain raw fuzzy scores. Effective badge includes exact + equivalent code; no effective fields in `report.json`.

## Documentation changes

Documentation-only edits, including `README.md` and `AGENTS.md`: review wording and links; run `git diff --check`.
Do not run gate, builds, matching, or reports for documentation-only edits unless explicitly requested.
The source reconstruction workflow below applies to C/C++ source changes. Tool changes: run gate and relevant tool checks.

## Source changes

1. Select a focused target with `tools/next.py` from the current report. When reconstruction-memory tools are available: `get_function_memory(addr)` before editing.
2. Read the full function, declarations, relevant original callers/callees. Preserve ABI, dispatch, side effects, reload timing, narrowing, ownership, initialization, allocation failures. Ambiguous diff: inspect raw x86.
3. Match before/after trials. Revert failed trials; retry with new evidence. `record_attempt`: actual trials only. `record_observation`: durable x86 facts with address/citation. Tools unavailable: continue locally.
4. Batch boundary: `tools/report.py --check` snapshots the saved report to `build-msvc400/report-baseline.json`, regenerates progress, and audits prior exact matches. Header/ABI/multi-TU changes: `detect_changes`. Explain regressions. No full reports per speculative trial.
5. Clang-format touched C/C++; run gate and relevant checks; commit verified work.

## Source conventions

- Code comments: functional only; reccmp annotations/symbols, layout offsets/sizes, and format controls.
- Class files: stem = primary class; free-function files allowed. Class definitions identical across TUs. Annotated functions: ascending original address.
- Use typed objects, members, indexing, base conversions. Derive extents/strides from allocations and accesses; distinguish serialized and runtime layouts.
- Allocations: `sizeof(Type)` / `count * sizeof(*elements)`. Byte cursors for streams/pixels.
- Calling conventions: arguments, forwarding, cleanup; zero-argument `RET` alone insufficient. Qualified base calls require direct-dispatch evidence.
- Constants: evidenced meaning; verify resource IDs against Manifest/RC. Preserve original assertion filenames when renaming files.
- Reccmp annotations: original Windows addresses; STUB promotion only when substantially implemented.
- `gate.py --names`: catalog review details by Windows address; parameter names ignored, types/constness checked. ABI differences need Windows evidence; normal pass leaves reviews open.
