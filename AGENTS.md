# AGENTS.md

Byte-matching reconstruction of *Lemmings Paintball* (`LEMBALL.EXE`, 1996 Win32).

## Core Directives & Evidence Priority

When evidence conflicts, resolve strictly in this order:
1. `data/LEMBALL.EXE`
2. x86 disassembly / Ghidra / PDB
3. `reccmp` output
4. Observed MSVC 4.00 codegen
5. 68K symbols/comments/code (provides names/intent; never overrides x86)

**Hard Rules:**
- Never invent symbols, addresses, members, behavior, or helpers.
- Never edit `README.md`, `Manifest.h`, reference hashes, or compiler flags unless explicitly instructed.
- Do not stop on minor ambiguity: choose the most reasonable path, proceed, and document assumptions at the end.

## Constraints & Environment

- **Compiler:** MSVC 4.00 (`/O2 /Ob1 /Oy /G4`). No C++11+, RTTI, exceptions, or inline asm.
- **Layout:** One primary class per `.h`/`.cpp`. Functions in `.cpp` must appear in ascending original x86 address order.
- **Resources:** Use `RES_*` constants from `src/Visos/Resources/Manifest.h` instead of raw numeric IDs. Values must remain binary-identical.
- **Style:** Match original types, local names, and layout. Avoid off-target refactoring; keep diffs minimal and task-scoped.

## `reccmp` Annotations

Format: `// <TYPE>: LEMBALL <ORIGINAL_X86_ADDR> [FLAGS]` (Always use original addresses, never rebuild addresses).

| Annotation | Usage / Precedence | Example / Syntax |
| :--- | :--- | :--- |
| `FUNCTION` | Complete/near-complete implementation. Must assemble-match. | `// FUNCTION: LEMBALL 0x00472ce0` |
| `STUB` | Missing or placeholder bodies only. Promoted to `FUNCTION` once real logic exists. | `// STUB: LEMBALL 0x00472ce0` |
| `TEMPLATE` | Template functions. Follow with exact name comment if unattached. | `// TEMPLATE: LEMBALL 0x00401230`<br>`// SomeTemplate<T>::Func` |
| `SYNTHETIC` | Compiler-generated methods (dtors, scalar deleting dtors, op=). Takes precedence over `TEMPLATE`. | `// SYNTHETIC: LEMBALL 0x00401230`<br>`// Bucket::'scalar deleting destructor'` |
| `LIBRARY` | CRT / 3rd-party library calls only. Never use for unknown project code. | `// LIBRARY: LEMBALL 0x00401230`<br>`// _MemPoolInit@4` |
| `VTABLE` | Place directly above class declaration. Annotate slot offsets in class body. | `// VTABLE: LEMBALL 0x0049a478`<br>`virtual void Destroy(); // vtable+0x18` |
| `GLOBAL` | Known global or static data. Never attach to locals. | `// GLOBAL: LEMBALL 0x0049f000`<br>`int g_count = 0;` |
| `STRING` | String literal bytes address (distinct from pointer `GLOBAL`). | `// STRING: LEMBALL 0x0049f000` |
| `LINE` | Critical diff/codegen realignments only. Must precede code directly. | `// LINE: LEMBALL 0x00401234` |
| `FOLDED` | Suffix flag for identical code folded to one address. Not a standalone tag. | `// FUNCTION: LEMBALL 0x00401230 FOLDED` |

**Rules:**
- **Decorated Symbols:** Prefix `?` indicates MSVC mangling (`// ?Func@@...`). For other symbol schemes, use `SYMBOL` flag (`// LIBRARY: LEMBALL 0x00401230 SYMBOL`).
- **68K Comments:** Place above `reccmp` annotation (e.g., `// 68K 0x1021749c ...`). Never map 68K addresses to `reccmp`.

## Types & Memory Representation

- Declare class size: `// SIZE 0x54` above class.
- Annotate member offsets: `unsigned int m_size; // 0x24`.
- Use `undefined`, `undefined2`, `undefined4` for unknown scalar widths. Do not arbitrarily choose signed/unsigned without assembly backing.
- Access via structured struct/class members. Do not offset-poke or manually traverse `vbptr` when structured representations are possible.

## MSVC 4.00 Codegen Quirks (`/O2 /Ob1 /Oy /G4`)

- **Stores/Reloads:** Stores force memory reloads. Avoid long-lived local variables across stores when original code re-fetches.
- **Call Liveness:** Re-fetch pointers after virtual calls if codegen indicates register reloads (e.g., `obj = arr[i]; obj->Run(); obj = arr[i];`).
- **Inlining (`/Ob1`):** Same-TU inline definitions inline; cross-TU definitions do not. Placement across `.cpp` files dictates inlining codegen.
- **Narrow Stack Parameters:** If the original reads stack bytes/words as 32-bit values without zero-extension, use `*(unsigned int*)&param`.
- **Guard Widening:** Message dispatch uses `switch (p_message) { case 0x2a: … }`, not `if` — produces the original's `mov+and+cmp` widening on word params.
- **Loop Shapes:** Use standard `int i = 0; while (i < max) { ... i++; }`. Do not add zero-count preconditions unless proven by branches.
- **32-bit Size Arithmetic:** Compute capacities/lengths in an `int` local (`int capacity = p + 4;`) so codegen is `add eax`, not `add ax`.
- **Loop Registers:**
    - 3 saved live values: `EBX` (offset), `ESI` (counter), `EDI` (`this`).
    - 4 saved live values + `EBP` constant: `ESI` (offset), `EBX` (counter), `EDI` (`this`).
- **Match Ceiling:** Cease iteration when differences are compiler noise: matched register swaps, equal-length jump offsets, or trailing `nop` alignments.

## Workflow & Tooling

```powershell
# 1. Target Iteration Loop
python tools/check.py 0xADDRESS                 # Compile target & compare score
python tools/check.py 0xADDRESS --diff          # Inspect assembly divergence
python tools/check.py 0xADDRESS --no-build      # reccmp only (fast)
python tools/check.py 0xADDRESS --clean-first   # Fix PDB_DESYNC or debug desyncs (run once)

# 2. Target Discovery
python tools/targets.py --kind tiny --max-size 5
python tools/targets.py --kind near|unit|clone

# 3. Session Diagnostics & Final Checks
python tools/build.py                           # Incremental build (log: build-msvc400/last_build.log)
python tools/report.py                          # Update report.json
reccmp-decomplint --target LEMBALL --warnfail src
python tools/smell.py && python tools/smell.py --annot
reccmp-stackcmp --target LEMBALL 0xADDRESS      # Stack alignment issues
reccmp-vtable --target LEMBALL                  # Vtable layout
reccmp-datacmp --target LEMBALL                 # Global data matching

```

## reccmp Tool Invocation

Target-based tools use the generated `build-msvc400/reccmp-build.yml` and the
virtual-environment executables. Run them from `build-msvc400`, or use
explicit `--paths` arguments from the repository root:

```powershell
Push-Location build-msvc400
try { ..\.decomp-venv\Scripts\reccmp-vtable.exe --target LEMBALL }
finally { Pop-Location }
```

Use the same form for `reccmp-datacmp.exe` and `reccmp-stackcmp.exe`.
`reccmp-decomplint.exe` does not require the generated build configuration.

**Cost Guidelines:** Run the cheapest tool first (`check.py` -> `check.py --diff` -> `stackcmp`). Avoid clean builds or full reports inside the tight matching loop.

## Code Intelligence (`codebase-memory-mcp`)

The repo is indexed into a persistent knowledge graph (auto-index on first connect; background watcher re-indexes on change). Prefer these over grep/read for structural questions — one query replaces file-by-file searching:

- **Callers before edits:** before touching a function or header, trace who breaks — `trace_path` (inbound, function name) or the call graph. Mandatory for shared headers (`GameObject.h`, `BaseObjectManager.h`, ...): one member rename shifts codegen in every derived unit.
- **`detect_changes`:** maps uncommitted changes to affected symbols. Run before session-end checks to catch unintended codegen-adjacent fallout.
- **`search_graph`:** structural search (regex names, file scope). First stop for "where is X declared/used" — cheaper than `grep`.
- **`semantic_query`:** when you know intent but not the name ("where is level data parsed"). Fallback after `search_graph` misses.
- **`search_code`:** graph-augmented grep over indexed sources — use when plain text matching is genuinely needed.
- **`get_architecture`:** fresh-session orientation for unfamiliar units (packages, hotspots, boundaries).
- **Dead code / zero callers:** identify stubs and orphaned helpers before annotating.

Evidence order unchanged: CBM reasons about the *source tree only* — never a substitute for the binary, Ghidra, or reccmp output.

## Completion Checklist

* [ ] Valid annotation types; addresses match original x86 binary.
* [ ] Source files remain strictly sorted in ascending x86 address order.
* [ ] No completed implementations left marked as `STUB`.
* [ ] `check.py` verifies match; `build.py` passes without errors.
* [ ] `reccmp-decomplint` passes with zero warnings or errors.
* [ ] No out-of-scope refactoring, formatting churn, or invented symbols.
