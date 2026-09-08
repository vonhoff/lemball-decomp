# AGENTS.md

This is a project to reconstruct *Lemmings Paintball* (`LEMBALL.EXE`, 1996 Win32) byte for byte.

## Evidence

Use evidence in this order:

1. `data/LEMBALL.EXE`
2. x86 disassembly, Ghidra, and PDB
3. `reccmp`
4. Observed MSVC 4.00 code generation
5. 68K symbols and comments for intent only

Never invent symbols, addresses, members, helpers, offsets, behavior, or annotations. Do not edit `README.md`, `Manifest.h`, reference hashes, or compiler flags unless the user asks. Record assumptions when evidence is incomplete.

## Codebase Knowledge Graph (codebase-memory-mcp)

This project uses codebase-memory-mcp to maintain a knowledge graph of the codebase.
ALWAYS prefer MCP graph tools over grep/glob/file-search for code discovery.

### Priority Order
1. `search_graph` — find functions, classes, routes, variables by pattern
2. `trace_path` — trace who calls a function or what it calls
3. `get_code_snippet` — read specific function/class source code
4. `check_index_coverage` — validate candidate paths and missed ranges before claims
5. `query_graph` — run Cypher queries for complex patterns
6. `get_architecture` — high-level project summary

### Evidence tiers
- **Scout (Tier 1):** quick positive lookup with few calls and targeted source checks. Mark it provisional; do not make negative or exhaustive claims.
- **Verify (Tier 2, default):** task-directed graph evidence, relevant trace directions, exact snippets for material claims, and relevant pagination.
- **Auditor (Tier 3):** bounded-scope full verification with current generation, complete relevant pagination, both call directions and broader relationships when material, and every limitation disclosed.
- After candidate paths are known in any tier, call `check_index_coverage` once with every evidence path. Add relevant scopes for negative or exhaustive claims. A clean result means no recorded gap, not proof of completeness. For partial, skipped, excluded, stale, pending, or unknown coverage, read/grep the reported ranges or scope before relying on graph results.

### When to fall back to grep/glob
- Searching for string literals, error messages, config values
- Searching non-code files (Dockerfiles, shell scripts, configs)
- When MCP tools return insufficient results

### Examples
- Find a handler: `search_graph(name_pattern=".*OrderHandler.*")`
- Who calls it: `trace_path(function_name="OrderHandler", direction="inbound")`
- Read source: `get_code_snippet(qualified_name="pkg/orders.OrderHandler")`

### Session resets and subagents
- At session start or after compaction, confirm the nearest graph project and generation with `list_projects` or `index_status`, then choose Scout, Verify, or Auditor.
- Before spawning a subagent, query the graph and coverage in the parent. Pass the tier, project, generation/freshness, bounded scope, queries and pagination state, qualified symbols, paths, call-chain findings, coverage evidence with ranges/reasons, source fallback already performed, and unresolved questions in the delegated task context.
- Do not assume subagents inherit MCP access or the parent conversation. If a child lacks MCP tools, it must not call or claim MCP access. It should use the supplied evidence and read/grep exact source, especially every reported missed-coverage range.

## Build and source rules

Use MSVC 4.00 with `/O2 /Ob1 /Oy /G4`. Do not use C++11 or newer, RTTI, exceptions, or inline assembly. Keep one primary class per `.h` and `.cpp`. Keep functions in ascending original x86 address order. Use `RES_*` from `src/Visos/Resources/Manifest.h` and preserve its values.

Use `undefined`, `undefined2`, and `undefined4` for unknown-width values. Do not guess signedness. Prefer named members over raw offsets and vbptr operations.

For matching, preserve the original loop shape. Keep 32-bit size and capacity arithmetic. Re-fetch pointers after virtual calls when the original does so. Use a `switch` when message dispatch requires the original widening. Avoid zero-count guards unless the original has one. Stop when the remaining difference is compiler noise, such as register allocation or alignment NOPs.

## Annotations

Use this form:

`// <TYPE>: LEMBALL <ORIGINAL_X86_ADDR> [OPTION]`

Addresses refer to the original executable. Keep annotations in address order within each source file.

`FUNCTION` marks a substantially implemented ordinary function. `STUB` marks an absent or incomplete function. `TEMPLATE` marks template-generated code. `SYNTHETIC` marks compiler-generated code, such as deleting destructors. `LIBRARY` marks third-party or runtime code. `VTABLE` marks a class vtable and belongs above the class. `GLOBAL` marks original global storage. `STRING` marks string bytes. `LINE` maps an original instruction to a source line. `FOLDED` is an option on `FUNCTION`. `SYMBOL` marks a following debug symbol.

Use the type that describes the original function. Do not use an annotation as a progress score. Promote a `STUB` only when the implementation is substantially complete.

## Matching workflow

Run the cheapest check first:

```powershell
python tools/check.py 0xADDRESS
python tools/check.py 0xADDRESS --diff
python tools/check.py 0xADDRESS --no-build
python tools/check.py 0xADDRESS --clean-first
```

Use `--clean-first` only for PDB or debug desynchronization. Inspect diffs before editing. Accept register, scheduling, thunk, or PDB-name equivalence only when complete instruction groups, data flow, flags, side effects, stack behavior, targets, and control flow agree. An unresolved offset that resembles a named symbol is not proof.

Run target lists with `tools/targets.py`. Use the original x86 evidence for sizes. Treat readiness and expected gain as ranking heuristics.

Run verification with:

```powershell
python tools/build.py
python tools/report.py
reccmp-decomplint --target LEMBALL --warnfail src
python tools/smell.py
python tools/smell.py --annot
```

Run target-based tools from `build-msvc400` with its virtual environment:

```powershell
Push-Location build-msvc400
try { ..\.decomp-venv\Scripts\reccmp-stackcmp.exe --target LEMBALL 0xADDRESS }
finally { Pop-Location }
```

Use the same form for `reccmp-vtable.exe` and `reccmp-datacmp.exe`. `reccmp-decomplint.exe` can run from the repository root.

Run clang-format on touched C and C++ files. After shared-header, ABI-sensitive, or multi-TU changes, audit prior effective 100% matches and run `detect_changes`. Prefer zero regressions. Accept a regression only when measured evidence shows a clear project-wide gain.

## Naming

Keep reconstructed C++ names in PascalCase. Leading Metrowerks `_` becomes `Internal` (`_DrawButton` -> `InternalDrawButton`). Underscores are word separators and are dropped (`EnemyRule_Radius50` -> `EnemyRuleRadius50`).

Check with:

```powershell
python tools/check_names.py --inventory C:/Research/Mapping/Lemmings_Paintball__68K_.functions.tsv
```

Some 68K symbols intentionally diverge from the C++ name (Mac nested types flattened, free functions moved onto classes, Mac `OnZoomBox` kept as Windows `OnDriverChange` so it overrides `PvWnd`). Those entries live in `tools/check_names.py` as `INTENTIONAL` and report status `intentional`, not `mismatch`. Do not rename source to match the 68K spelling for those. Add or remove `INTENTIONAL` entries when a divergence is introduced or retired.
