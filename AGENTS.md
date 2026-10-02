# AGENTS.md

*Lemmings Paintball* reconstruction: `LEMBALL.EXE`, MSVC 4.00.

## Evidence

- Priority: `data/LEMBALL.EXE` → Ghidra/PDB/x86 → upstream reccmp → observed MSVC codegen. 68K: intent only.
- Original: Ghidra. Reconstructed: codebase-memory skill/graph, verified source.
- Infer from callers, consumers, widths, layout and behavior. Historical headers/symbols optional. Qualify assumptions; never invent evidence.
- Plausible original source first. Matching validates hypotheses; compiler tricks need semantic evidence.
- `README.md`, `Manifest.h`, reference hashes, compiler flags: edit only when asked.

## Tools

Run with `python`:

| Script | Purpose |
| --- | --- |
| `tools/match.py 0xADDR` | Build/compare/diff; `--no-build` when current |
| `tools/next.py --kind near` | Rank report; `gain` also |
| `tools/gate.py` | Source/tests; `--path`, `--names`, `--names-strict`, `--vtable`, `--all` |
| `tools/report.py` | Canonical progress report |
| `tools/build.py` | Build/link wrapper; `--clean-first` for PDB desync |

Use project scripts. Deep comparison: `reccmp-stackcmp` / `reccmp-datacmp` from `build-msvc400`.
Upstream reccmp authoritative. Raw scores; exact = 100%; equivalence stays fuzzy. Measured report fields only.
PE/COFF scaffold: `openblack/bw1-decomp` / `encounter/dtk-template`. Native scaffold: `tools2/README.md`. Latest template files verbatim; PE port separate. Native objdiff, executable hash check. Keep native reports separate from reccmp's canonical report; promote linked units only after verified source replacement.

## Workflow

1. One function. Screen with `triage_report` and canonical report; read selected `get_function_memory(addr)`.
2. Read full function/declarations and relevant original callers/callees. Focused match before/after each trial.
3. Preserve ABI, dispatch, data flow, side effects, reload/snapshot timing, narrowing, ownership and allocation failures. Check raw x86 when normalized diffs obscure semantics.
4. Record actual trials with `record_attempt`: retained/reverted/failed. Revert failures before switching. Retry with new evidence; dead ends tree-specific.
5. Record durable x86 facts with `record_observation` and citation; key by Windows address. Review-only work isn't an attempt. MCP offline: continue.
6. Batch boundary: snapshot `build-msvc400/report-baseline.json`; regenerate `report.json` once; audit prior exact matches. Header/ABI/multi-TU edits: `detect_changes`. Explain losses; prefer zero regressions.
7. Clang-format touched C/C++; full gate. Commit coherent, verified work. No full reports per speculative trial.

Audit ledger: `docs/reconstruction-audit-backlog.md`. Record path/function, expression, classification, evidence, intended representation, confidence, disposition. Group shared causes; keep unresolved work open.

## Source

- One primary class per `.h`/`.cpp`; stem = class. Identical class definitions across TUs; no score-driven TU guards or inline asm. Functions ascending original address.
- Real objects, named members, typed indexing/base conversions, SDK records. Investigate cast chains, punning, byte offsets, overlays, adjacent scalars treated as arrays.
- Recover extents from allocation, access widths, producers, terminators and consumers. Padding/SIZE comments aren't extent proof. Serialized prefixes/strides differ from runtime objects.
- Allocations/counts: `sizeof(Type)` / `count * sizeof(*elements)`. Byte cursors appropriate for streams/pixels.
- Reject codegen-only `volatile`, aliases, helpers, API caches, fake classes, raw backing and comma assignments. Preserve evidenced lifetime, initialization and call behavior.
- Windows ABI governs types/callbacks; trace arguments, forwarding and cleanup. Zero-argument `RET` doesn't prove convention. Qualified base calls require direct-dispatch evidence.
- Constants: actual semantic domain. Review `case 0x`, `= 0x`, `+ 0x`, `- 0x`, `* 0x`, `[0x`, `&=`, `|=`. Decode switch tables; trace Manifest/RC IDs and neighbors; use `RES_*`. Equal numbers aren't interchangeable meanings.
- Names: catalog spelling/case/prefixes intact; otherwise evidenced behavior. Qualify inferred names/layouts. Gate exceptions and unmapped/signature reviews remain questions.

Placement: SDK ABI in `src/Platform/{DirectX,WinSock}/`; game wrappers in `src/Visos/Target/{Graphics,Sound,Input,Network,UI,System}/`.
Startup/options: `Visos/Foundation/VsInit.cpp`; lifecycle: `*Init.cpp`; network workers: `Visos/Network/NetworkInit.cpp`. Preserve original assertion filenames after renames.

## MSVC 4.00

- ESI/EDI/EBX allocation: live intervals, declaration/first-use ties. Whole-register swap: try declaration order.
- Compare operand order follows AST. `while` starts with bottom-test jump; `do/while` doesn't.
- `short`/`char` affect extensions and register width. Virtual calls may require member reloads.

## Annotations and naming

`// <TYPE>: LEMBALL 0xADDR [OPTION]`. Types: FUNCTION, STUB, TEMPLATE, SYNTHETIC, LIBRARY, VTABLE, GLOBAL, STRING, LINE. `FOLDED`; `SYMBOL` precedes debug name. Describe original code; promote STUB only when substantially complete.

`tools/data/catalog.csv`: leading 68K naming evidence. Ordinary reccmp annotations; no duplicate 68K comments. SDK/passive record names intact.
`gate.py --names`: Windows-address comparison; parameter names ignored, types/constness checked. Differences: Windows review; `--verbose` lists, `--names-strict` fails reviews. Normal success doesn't resolve pending items.
