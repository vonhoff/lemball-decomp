# AGENTS.md

*Lemmings Paintball*: `LEMBALL.EXE`, MSVC 4.00.

## Style

- Documents and communication: telegraphese. Short phrases; facts first; no filler. Precision, evidence, uncertainty intact.
- Tools: single responsibility. Separate parsing, comparison/ranking, output. Upstream APIs first.

## Evidence

- Priority: `data/LEMBALL.EXE` > Ghidra/PDB/x86 > upstream reccmp > observed MSVC codegen. 68K: intent only.
- Original: Ghidra. Reconstructed: codebase-memory skill/graph, source verification.
- Infer from callers, consumers, widths, layout, behavior. Assumptions explicit; no invented evidence. Historical headers/symbols optional.
- Plausible source first; matching validates. Compiler tricks require semantic evidence.
- `README.md`, `Manifest.h`, reference hashes, compiler flags: edit only when asked.

## Tools

Project scripts; run with `python`. Details: `tools/USAGE.md`.

| Script | Purpose |
| --- | --- |
| `tools/match.py 0xADDR` | Build/compare/diff; current build: `--no-build` |
| `tools/next.py --kind near` | Rank unfinished functions; `gain` also |
| `tools/gate.py` | Source/tests; `--path`, `--names`, `--names-strict`, `--vtable`, `--all` |
| `tools/report.py` | Canonical `build-msvc400/{reccmp,report}.json` |
| `tools/badges.py` | Separate README badges; Effective includes exact + equivalence |
| `tools/build.py` | Build/link; PDB desync: `--clean-first` |

Deep comparison: `reccmp-stackcmp` / `reccmp-datacmp` from `build-msvc400`.
Upstream reccmp authoritative; raw scores. Exact = 100%; equivalence stays fuzzy.
Canonical report: measured fields; no effective fields/score promotion.

## Workflow

1. One function: `triage_report`, canonical report, `get_function_memory(addr)`.
2. Read full function/declarations and original callers/callees. Match before/after trials.
3. Preserve ABI, dispatch, data flow, side effects, reload/snapshot timing, narrowing, ownership, allocation failures, lifetime, initialization. Ambiguous diff: raw x86.
4. `record_attempt`: retained/reverted/failed; actual trials only. Revert failures before switching. Retry: new evidence; dead ends tree-specific.
5. Durable x86 facts: `record_observation`, citation, Windows address. MCP offline: continue.
6. Batch: snapshot `build-msvc400/report-baseline.json`; regenerate report once; audit prior exacts. Header/ABI/multi-TU edits: `detect_changes`. Explain losses; zero regressions preferred. No full reports per trial.
7. Clang-format touched C/C++; full gate; commit coherent, verified work.

Audit: `docs/reconstruction-audit-backlog.md`; path/function, expression, classification, evidence, intended representation, confidence, disposition. Group shared causes; unresolved items open.

## Source

- One primary class/file; `.h`/`.cpp` stem = class. Identical definitions across TUs; no score-driven guards/inline asm. Functions: ascending original address.
- Real objects/members, typed indexing/base conversions, SDK records. Investigate casts, punning, byte offsets, overlays, adjacent scalars as arrays.
- Extents: allocations, access widths, producers, terminators, consumers; padding/SIZE insufficient. Serialized prefixes/strides != runtime layout.
- Allocation: `sizeof(Type)` / `count * sizeof(*elements)`. Byte cursors: streams/pixels.
- Reject codegen-only `volatile`, aliases, helpers, API caches, fake classes, raw backing, comma assignments.
- Windows ABI types/callbacks; trace arguments/forwarding/cleanup. Zero-argument `RET`: no convention proof. Qualified base calls require direct-dispatch evidence.
- Constants: semantic domain. Audit hex cases/assignments/arithmetic/indexing/masks. Decode switches; trace Manifest/RC IDs/neighbors; use `RES_*`. Equal values != equal meaning.

Placement:

- SDK ABI: `src/Platform/{DirectX,WinSock}/`.
- Game wrappers: `src/Visos/Target/{Graphics,Sound,Input,Network,UI,System}/`.
- Startup/options: `Visos/Foundation/VsInit.cpp`; lifecycle: `*Init.cpp`; network workers: `Visos/Network/NetworkInit.cpp`.
- Renames: preserve assertion filenames.

## MSVC 4.00

- ESI/EDI/EBX: live intervals, declaration/first-use ties. Whole-register swap: try declaration order.
- Compare operand order: AST. `while`: initial bottom-test jump; `do/while`: none.
- `short`/`char`: extensions, register width. Virtual calls: possible member reloads.

## Annotations and names

`// <TYPE>: LEMBALL 0xADDR [OPTION]`. Types: FUNCTION, STUB, TEMPLATE, SYNTHETIC, LIBRARY, VTABLE, GLOBAL, STRING, LINE. `FOLDED`; `SYMBOL` before debug name. Describe original code; promote STUB only when substantially complete.

- `tools/data/catalog.csv`: leading 68K naming evidence. Preserve spelling/case/prefixes; otherwise evidenced behavior. SDK/passive record names intact.
- Ordinary reccmp annotations; no duplicate 68K comments.
- `gate.py --names`: Windows addresses; parameter names ignored, types/constness checked. Differences: Windows review; `--verbose` lists, `--names-strict` fails reviews.
- Exceptions/unmapped names/signature reviews: unresolved. Normal pass never closes reviews.
