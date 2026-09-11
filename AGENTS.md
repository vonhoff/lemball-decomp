# AGENTS.md

Byte-for-byte reconstruction of *Lemmings Paintball* (`LEMBALL.EXE`, MSVC 4.00).

## Evidence (static first)

1. `data/LEMBALL.EXE`
2. Ghidra / PDB / x86 disassembly
3. `reccmp` via `tools/match.py`
4. Observed MSVC 4.00 codegen
5. 68K symbols for intent only

Never invent symbols, addresses, members, offsets, or annotations. Do not edit `README.md`, `Manifest.h`, reference hashes, or compiler flags unless asked. Record assumptions when evidence is incomplete. Prefer Ghidra for original code; graph/IDE search for reconstructed source.

## Agent tools (only these)

| Script | Job |
|--------|-----|
| `tools/match.py 0xADDR` | Build (unless `--no-build`) + compare + diff |
| `tools/next.py` | Rank what to match next (`--kind tiny\|near\|gain\|…`) |
| `tools/gate.py` | Smell + annot + layout + decomplint (`--path`, `--vtable`, `--names`) |
| `tools/report.py` | Progress `report.json` (CI / ranking input) |
| `tools/build.py` | Build only (also cmake link wrapper) |

```powershell
python tools/match.py 0xADDRESS
python tools/match.py 0xADDRESS --no-build
python tools/next.py --kind near
python tools/gate.py
python tools/gate.py --path src/Foo.cpp
```

Use `--clean-first` only for PDB desync. After header/ABI/multi-TU edits, audit prior 100% matches (`detect_changes`). Prefer zero regressions.

Deep asm: `reccmp-stackcmp` / `reccmp-datacmp` from `build-msvc400` when needed.

## Reconstruction memory MCP

The repo-local script table above remains authoritative. In addition, use the
`reconstruction-memory` MCP server when its tools are available in the current agent
session. It normally runs at `http://127.0.0.1:8765/mcp`; do not block reconstruction work
if it is offline or absent from the tool inventory.

Memory is keyed by original x86 function address. Always use the annotated LEMBALL
address in canonical form such as `0x00408240`; pass the symbol as secondary metadata.

Before modifying a reconstruction target:

1. Call `get_function_memory` with its address and known symbol.
2. Review prior attempts, dead ends, facts, open hypotheses, scores, and recorded Git state.
3. Do not repeat an equivalent failed experiment under the same repository state unless
   new evidence justifies it. Old failures are state-specific, not permanent prohibitions.

While working:

- Use `record_observation` for durable facts, notes, and strong evidence-backed dead ends.
  Facts and dead ends must cite concrete evidence such as original/recompiled instructions,
  reccmp output, PDB/Ghidra findings, or verified type/layout behavior.
- Use `record_hypothesis` for unresolved, testable ideas. Never store a guess as a fact.
- Use `resolve_hypothesis` when evidence confirms, rejects, supersedes, or leaves a
  hypothesis inconclusive.
- Use `set_function_state` when establishing or changing the function's current retained
  score. Scores are percentages from 0 through 100.

Before reverting, abandoning, or moving away from a target, call `record_attempt` for each
meaningful experiment. Include the hypothesis, concise change, before/after score when
available, status (`retained`, `reverted`, `partial`, `failed`, or `dead_end`), result,
objective evidence, and repository state. Use `dead_end` only for strong negative evidence;
ordinary unsuccessful work is `failed` or `reverted`.

The server captures current Git state automatically, but record an explicit commit/state
when the tested tree has already been reverted or differs from the current worktree. If a
previous approach becomes relevant after surrounding code or types change, link the retry
with `repeated_attempt_id` and state the `new_evidence`. Do not add trivial retries, raw chat
logs, speculative summaries, or duplicate entries. Preserve history through resolution or
supersession rather than deletion. Use `search_attempts` for deeper or cross-function lookup
when the compact `get_function_memory` result is truncated.

## Source rules

No inline asm. One primary class per `.h`/`.cpp` (stem = class), unless `tools/lib/layout.py` `OVERRIDE_STEMS`. Functions in ascending original x86 address order. Use `RES_*` from `Manifest.h`. Prefer named members over offset pokes. Keep `undefined`/`undefined2`/`undefined4` until Win32 evidence justifies a tighter type. Preserve original loop shape, 32-bit size math, post-virtual pointer re-fetches, and message `switch` widening. Stop at compiler noise (reg alloc, alignment NOPs).

## MSVC 4.00 Codegen Quirks

- **Register Allocation Order**: Callee-saved registers (`ESI`, `EDI`, `EBX`) are allocated by live-interval priority, but ties fall back to declaration and first-use order. If `esi` and `edi` are swapped across an entire function, reorder local variable declarations.
- **Comparison Operand Order**: Inverted branch comparisons (`cmp a, b; jle` vs `cmp b, a; jge`) indicate operand order differences in the AST. Invert condition operands or loop bounds (`a <= b` vs `b >= a`).
- **Loop Canonicalization**: `while (cond)` emits an initial `jmp` to the test at the bottom; `do { ... } while (cond)` emits no initial jump. If an unexpected jump precedes the loop, convert `do-while` to `while` or vice-versa.
- **Type Widths & Sign Extension**: Coordinate and index math was predominantly 16-bit `short` or 8-bit `char`. Using 32-bit `int` hides necessary `movsx`/`movzx` instructions or 16-bit register usage (`ax`, `cx`).
- **Post-Virtual Pointer Reloads**: MSVC assumes virtual function calls may clobber object state. Members accessed across virtual calls must be re-read from `this` rather than cached in locals, or MSVC will omit the reload from memory.

## Annotations

`// <TYPE>: LEMBALL <ORIGINAL_X86_ADDR> [OPTION]` — address order in each file.

Types: `FUNCTION` `STUB` `TEMPLATE` `SYNTHETIC` `LIBRARY` `VTABLE` `GLOBAL` `STRING` `LINE`; option `FOLDED`; `SYMBOL` for a following debug name. Annotation describes the original, not progress. Promote `STUB` only when substantially complete.

## Match loop

1. Known `0xADDR` → annotated source + Ghidra (graph only if path unknown).
2. Inspect callers/callees/types (Ghidra original, graph reconstructed).
3. `python tools/match.py 0xADDR` before editing (`--no-build` if EXE current).
4. Audit params, returns, members, locals.
5. Tighten `undefined*` only with stronger Win32 evidence.
6. Accept match only when instruction groups, data flow, side effects, stack, and control flow agree—not opcode similarity alone.

## Naming

PascalCase; leading `_` → `Internal`; drop `_` separators. Intentional Mac/Win divergences: `tools/lib/names.py` `INTENTIONAL`. Layout stem overrides: `tools/lib/layout.py` `OVERRIDE_STEMS` / `INTENTIONAL`.

```powershell
python tools/gate.py --names
```

Compares reconstructed names to adjacent `// 68K` comments in `src/` (no external Mapping tree).

clang-format touched C/C++.
