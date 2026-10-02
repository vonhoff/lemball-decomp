# AGENTS.md

*Lemmings Paintball* reconstruction: `LEMBALL.EXE`, MSVC 4.00.

## Style

- Documents and communication: telegraphese. Short phrases; concrete facts; explicit uncertainty.
- Tools: single responsibility; upstream APIs first. Remove unused features and duplicate logic.

## Evidence

- `data/LEMBALL.EXE` and original x86: behavior and ABI evidence. Ghidra: analysis; PDB: rebuilt symbols.
- Structural source exploration: codebase-memory skill; verify against source.
- `tools/data/catalog.csv`: Mac symbols with optional Windows mappings. Naming/type evidence; Windows ABI differences require Windows evidence.
- Matching: validation of a source hypothesis. Qualify inferred names, types, layouts; no score-only source tricks.
- `README.md`, `Manifest.h`, reference hashes, compiler flags: edit only when asked.

## Commands

Run with `python`; options and responsibilities: `tools/USAGE.md`.

| Script | Purpose |
| --- | --- |
| `tools/build.py` | Build/link; `--clean-first` for stale PDB/build artifacts |
| `tools/match.py 0xADDR` | Build/compare/diff; `--no-build` for current artifacts |
| `tools/next.py --kind near` | Rank unfinished functions; `gain` also |
| `tools/gate.py` | Source checks and tool tests |
| `tools/report.py` | Canonical comparison/progress reports |
| `tools/badges.py` | README badges, separate from canonical progress |

Deep comparison: `reccmp-stackcmp` / `reccmp-datacmp` from `build-msvc400`.
Canonical progress: exact = non-stub, raw 100% normalized assembly; stubs contribute zero.
Effective matches retain raw fuzzy scores. Effective badge includes exact + equivalent code; no effective fields in `report.json`.

## Source changes

1. Select a focused target from the current report. When reconstruction-memory tools are available: `triage_report`, then `get_function_memory(addr)`.
2. Read the full function, declarations, relevant original callers/callees. Preserve ABI, dispatch, side effects, reload timing, narrowing, ownership, initialization, allocation failures. Ambiguous diff: inspect raw x86.
3. Match before/after trials. Revert failed trials; retry with new evidence. `record_attempt`: actual trials only. `record_observation`: durable x86 facts with address/citation. Tools unavailable: continue locally.
4. Batch boundary: snapshot `build-msvc400/report-baseline.json`; regenerate report; audit prior exact matches. Header/ABI/multi-TU changes: `detect_changes`. Explain regressions. No full reports per speculative trial.
5. Clang-format touched C/C++; run gate and relevant checks; commit verified work.

Unresolved reconstruction findings: `docs/reconstruction-audit-backlog.md`; location, evidence, uncertainty, disposition.

## Source conventions

- Class files: stem = primary class; free-function files allowed. Class definitions identical across TUs. Annotated functions: ascending original address.
- Use typed objects, members, indexing, base conversions. Derive extents/strides from allocations and accesses; distinguish serialized and runtime layouts.
- Allocations: `sizeof(Type)` / `count * sizeof(*elements)`. Byte cursors for streams/pixels.
- Calling conventions: arguments, forwarding, cleanup; zero-argument `RET` alone insufficient. Qualified base calls require direct-dispatch evidence.
- Constants: evidenced meaning; verify resource IDs against Manifest/RC. Preserve original assertion filenames when renaming files.
- Reccmp annotations: original Windows addresses; STUB promotion only when substantially implemented.
- `gate.py --names`: catalog comparison by Windows address; parameter names ignored, types/constness checked. `--verbose`: review details; `--names-strict`: fail case differences and pending signature reviews. Normal pass leaves reviews open.
