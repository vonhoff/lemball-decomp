# AGENTS.md

Matching reconstruction: *Lemmings Paintball* (`LEMBALL.EXE`, MSVC 4.00).

Communication style: Telegraphese.

## Evidence: static first

1. `data/LEMBALL.EXE`
2. Ghidra, PDB, x86 disassembly
3. `reccmp` through `tools/match.py`
4. Observed MSVC 4.00 codegen
5. 68K symbols: intent only

Never invent symbols, addresses, members, offsets, annotations. Record uncertain assumptions. Original code: Ghidra. Reconstructed source: graph/IDE search. Do not edit `README.md`, `Manifest.h`, reference hashes, compiler flags unless asked.

## Tools

Use only these project scripts:

| Script | Job |
| --- | --- |
| `tools/match.py 0xADDR` | Build, compare, diff; `--no-build` if EXE current |
| `tools/next.py` | Rank targets; `--kind tiny|near|gain|...` |
| `tools/gate.py` | Smell, annot, layout, decomplint; `--path`, `--vtable`, `--names` |
| `tools/report.py` | Progress report for CI/ranking |
| `tools/build.py` | Build only; CMake link wrapper |

`python tools/match.py 0xADDRESS`; `python tools/next.py --kind near`; `python tools/gate.py --path src/Foo.cpp`; `python tools/gate.py`.

`--clean-first`: PDB desync only. Header, ABI, multi-TU edits: audit prior 100% matches with `detect_changes`; prefer zero regressions. Deep asm: `reccmp-stackcmp` / `reccmp-datacmp` from `build-msvc400`.

## Focused workflow

One function at a time. Select from the current report or `tools/next.py`. Read reconstruction memory only for the selected target before editing; inspect related functions only when a concrete caller, callee, type, or regression demands it. Prior dead ends are tree-specific, not permanent bans.

Use focused `match.py` before and after each trial. Check original x86 when a normalized diff suggests changed semantics: normalized operands can mislead. Log a failed trial, then revert it before switching targets. Do not run a full report for each speculative edit.

Use `build-msvc400/report.json` as the canonical full report. At a promising batch boundary, keep one fixed `build-msvc400/report-baseline.json` snapshot, overwrite `report.json` once, compare all prior exact matches, then replace the baseline at the next batch. Avoid per-trial report filenames. Run full `gate.py` and commit only coherent, verified gains. Clang-format touched C/C++.

## Reconstruction memory MCP

If available, key by canonical address (for example `0x00408240`); symbol secondary.

- Before target edit: `get_function_memory(addr, symbol)`. Review state, attempts, dead ends. Repeat a failed trial under the same tree only with new evidence.
- Durable fact/dead end: `record_observation` with x86/reccmp/PDB citation. Testable idea: `record_hypothesis`; close via `resolve_hypothesis`. Score change: `set_function_state` (0-100).
- Before revert or target switch: `record_attempt` (`retained`, `reverted`, `partial`, `failed`, `dead_end`).
- MCP absent/offline: continue reconstruction.

## Source rules

Identical class definition in every TU. Never vary method declarations, signatures, or inline bodies behind TU-specific guards for score. Resolve from original x86. No inline asm. One primary class per `.h`/`.cpp`; filename stem equals class, no exceptions. Functions in ascending original x86 address order. Use `RES_*` from `Manifest.h`. Prefer named members over offset pokes.

DirectX SDK ABI: `src/Platform/DirectX/`, SDK names. Shared WinSock: `src/Platform/WinSock/`. Game wrappers/state: `src/Visos/Target/` under `Graphics/`, `Sound/`, `Input/`, `Network/`, `UI/`, `System/`. Keep original filename strings in reconstructed assertions after source renames.

Startup orchestration and command-line options: `src/Visos/Foundation/VsInit.cpp`. Subsystem lifecycle: matching `*Init.cpp`. Network workers/startup/shutdown: `Visos/Network/NetworkInit.cpp`.

## MSVC 4.00 codegen

- Callee-saved `ESI`, `EDI`, `EBX`: live-interval priority; ties follow declaration/first-use order. Whole-function `esi`/`edi` swap: test local declaration order.
- `cmp a,b; jle` versus `cmp b,a; jge`: AST operand order. Test inverted condition/bounds.
- `while`: initial jump to bottom test. `do/while`: no initial jump. Match original loop form.
- Coordinates/indices often `short` or `char`. Wrong `int` hides `movsx`/`movzx` and 16-bit register use.
- Virtual calls can clobber object state. Re-read members through `this` after a virtual call when original does.

## Annotations

`// <TYPE>: LEMBALL <ORIGINAL_X86_ADDR> [OPTION]`; ascending address order. Types: `FUNCTION` `STUB` `TEMPLATE` `SYNTHETIC` `LIBRARY` `VTABLE` `GLOBAL` `STRING` `LINE`. Option: `FOLDED`. `SYMBOL` marks following debug name. Describe original code, not progress. Promote `STUB` only when substantially complete.

## Match loop

1. Known `0xADDR`: annotated source and Ghidra; graph only if path unknown.
2. Inspect needed callers, callees, types: Ghidra original; graph reconstructed.
3. Run `python tools/match.py 0xADDR` before editing; `--no-build` if EXE current.
4. Audit parameters, returns, members, locals. Tighten `undefined*` only with stronger Win32 evidence.
5. Accept only matching instruction groups, data flow, side effects, stack, control flow; opcode similarity alone insufficient.

## Naming

`tools/data/catalog.csv` is leading 68K evidence. Keep exact spelling, case, acronyms, prefixes, underscores. No artificial PascalCase/acronym conversion; do not strip prefixes or change leading `_` to `Internal`. For project-owned Windows classes without catalog evidence, choose concise behavior names. Keep established platform API and passive record names. `tools/lib/layout.py` checks class/filename agreement.

`python tools/gate.py --names` compares names by original Windows address. Use ordinary reccmp annotations; no duplicate `// 68K` comments. Parameter names ignored; encoded parameter types and method constness compared. Signature differences need Windows review (`--names --verbose`; `--names-strict` fails review items).
