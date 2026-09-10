# AGENTS.md

This is a project to reconstruct *Lemmings Paintball* (`LEMBALL.EXE`, 1996 Win32) byte for byte.

## Evidence

Use evidence in this order:

1. `data/LEMBALL.EXE`
2. x86 disassembly, Ghidra, and PDB
3. Live Cheat Engine observation via the `cheatengine` MCP (runtime corroboration only)
4. `reccmp`
5. Observed MSVC 4.00 code generation
6. 68K symbols and comments for intent only

Never invent symbols, addresses, members, helpers, offsets, behavior, or annotations. Do not edit `README.md`, `Manifest.h`, reference hashes, or compiler flags unless the user asks. Record assumptions when evidence is incomplete.

## Cheat Engine MCP Bridge (cheatengine)

This project uses [cheatengine-mcp-bridge](https://github.com/miscusi-peek/cheatengine-mcp-bridge) for **dynamic** reverse engineering of a running `LEMBALL.EXE` (or rebuilt match binary) under Cheat Engine. Install lives at `C:\Tools\cheatengine-mcp-bridge`. MCP server id: `cheatengine`.

Use CE MCP when runtime evidence is needed: live memory layout, pointer chains, what-writes/what-accesses, AOB scans, structure dissection, RTTI, hardware breakpoints, or confirming a reconstructed type against process state. Prefer Ghidra MCP for static disassembly/decompile of the PE. Prefer codebase-memory-mcp for source discovery in this repo. Prefer `tools/check.py` / `reccmp` for match verification. Do not invent addresses or members from CE alone — cross-check against `data/LEMBALL.EXE`, Ghidra/PDB, and annotations.

### Prerequisites (every session)
1. Cheat Engine 7.7+ running (`C:\Program Files\Cheat Engine\`). Autorun loads `autorun\mcp_bridge_autoload.lua` → `MCP_Server\ce_mcp_bridge.lua`. Expect log: `[MCP v12.0.0] MCP Server Listening on: CE_MCP_Bridge_v99`. If missing, execute `dofile([[C:\Tools\cheatengine-mcp-bridge\MCP_Server\ce_mcp_bridge.lua]])` from CE Lua.
2. Attach CE to the target process (`LEMBALL.EXE` or the rebuilt binary under test).
3. CE → Settings → Extra → **disable "Query memory region routines"** (BSOD risk with DBVM/scans).
4. Agent MCP clients restarted after config change. First tool call: `ping`. `process_id: 0` means bridge up but no process attached yet.

### Priority Order
1. `ping` — confirm bridge alive
2. `get_process_info` / `enum_modules` — confirm attached process and module bases
3. `read_memory` / `read_integer` / `read_string` / `read_pointer_chain` — inspect live values
4. `disassemble` / `analyze_function` — dynamic code view at a VA
5. `aob_scan` / `scan_all` — find patterns or values (paginate; do not dump unbounded results)
6. `dissect_structure` / `get_rtti_classname` — name/layout hypotheses for heap/globals
7. `find_references` / `find_call_references` / hardware breakpoints / DBVM watch — prove writers and callers at runtime
8. Full command list: `C:\Tools\cheatengine-mcp-bridge\AI_Context\MCP_Bridge_Command_Reference.md`

### Rules of use
- Evidence order in this file still wins. CE results are runtime observations, not ground-truth symbols.
- Prefer read/analyze tools. Do not write memory, inject DLLs, assemble patches, or enable shell tools (`CE_MCP_ALLOW_SHELL`) unless the user explicitly asks.
- Prefer hardware breakpoints / DBVM watches over software `0xCC` patches.
- Paginate list tools (`offset` / `limit`). Summarize; do not paste megabyte dumps into chat.
- Image base may differ from the static Ghidra image. Convert with module base from `enum_modules` before comparing to annotated `LEMBALL` x86 addresses.
- Leave `CE_MCP_ALLOW_SHELL` unset. Default bridge timeout is `CE_MCP_TIMEOUT` (30s).

### Examples
- Bridge check: `ping`
- Module base: `enum_modules` then resolve `LEMBALL.EXE+offset`
- Follow `[[base+0x10]+0x8]`: `read_pointer_chain`
- Prove who mutates a global: data breakpoint / `find_what_writes` family, then `disassemble` the hit

### Session resets and subagents
- After restart or compaction, `ping` then `get_process_info` before memory work.
- Before spawning a subagent for dynamic RE, pass: bridge version from `ping`, attached PID/name, module bases, relevant VAs (and static RVA equivalents), scans already done, and open questions.
- Do not assume subagents inherit MCP access. If a child lacks `cheatengine` tools, it must not claim CE access; use parent-supplied runtime evidence only.

## Codebase Knowledge Graph (codebase-memory-mcp)

This project uses codebase-memory-mcp to maintain a knowledge graph of the codebase.
ALWAYS prefer MCP graph tools over IDE text search and over grep/glob/file-search for structural code discovery.

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
- After candidate paths are known in any tier, call `check_index_coverage` once with every evidence path. Add relevant scopes for negative or exhaustive claims. A clean result means no recorded gap, not proof of completeness. For partial, skipped, excluded, stale, pending, or unknown coverage, use JetBrains Index MCP (`clion-index`) on the reported ranges or scope before relying on graph results; only then fall back to read/grep.

### JetBrains Index MCP (`clion-index`) — use instead of grep/glob
CLion runs [IDE Index MCP Server](https://github.com/hechtcarmel/jetbrains-index-mcp-plugin) on `http://127.0.0.1:29177/index-mcp/streamable-http` (server id `clion-index`). Prefer it for every text/file search that is not a graph query:

- String literals, error messages, config values, annotations, addresses
- Non-code files (scripts, configs, Dockerfiles, tables, manifests)
- Filename lookup (`ide_find_file`) and Find-in-Files (`ide_search_text`)
- When graph tools return insufficient results for a text-shaped question

Call `ide_index_status` first when unsure the IDE is smart. After external edits, `ide_sync_files`. Pass `project_path` as `C:/Dev/lemball-decomp` when multiple projects are open. Prefer read-only tools; do not rename/move/refactor via IDE MCP unless the user asks.

### When to fall back to grep/glob
Only after `clion-index` fails or is unusable:

- CLion not running / port `29177` unreachable / MCP tools missing
- `ide_index_status` stuck in dumb mode with no useful results
- `ide_search_text` / `ide_find_file` error or empty when a hit is expected and a second narrowed query still fails
- Subagent has no IDE MCP access and parent did not supply search evidence

### Examples
- Find a handler: `search_graph(name_pattern=".*OrderHandler.*")`
- Who calls it: `trace_path(function_name="OrderHandler", direction="inbound")`
- Read source: `get_code_snippet(qualified_name="pkg/orders.OrderHandler")`
- Find a literal: `ide_search_text` for `FUNCTION: LEMBALL 0x`
- Find a file by name: `ide_find_file` for `EnemyProcessAction`

### Session resets and subagents
- At session start or after compaction, confirm the nearest graph project and generation with `list_projects` or `index_status`, then choose Scout, Verify, or Auditor. Also `ide_index_status` if text search may be needed.
- Before spawning a subagent, query the graph and coverage in the parent. Pass the tier, project, generation/freshness, bounded scope, queries and pagination state, qualified symbols, paths, call-chain findings, coverage evidence with ranges/reasons, IDE search results already performed, source fallback already performed, and unresolved questions in the delegated task context.
- Do not assume subagents inherit MCP access or the parent conversation. If a child lacks MCP tools, it must not call or claim MCP access. It should use the supplied evidence and read/grep exact source, especially every reported missed-coverage range.

## Build and source rules

Do not use inline assembly. Keep one primary class per `.h` and `.cpp`, with the file stem equal to that class name, unless `tools/check_tu.py` `OVERRIDE_STEMS` lists a proven original TU basename. Keep functions in ascending original x86 address order. Use `RES_*` from `src/Visos/Resources/Manifest.h` and preserve its values.

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
python tools/check_tu.py
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
python tools/check_tu.py
```

Some 68K symbols intentionally diverge from the C++ name (Mac nested types flattened, free functions moved onto classes, Mac `OnZoomBox` kept as Windows `OnDriverChange` so it overrides `PvWnd`). Those entries live in `tools/check_names.py` as `INTENTIONAL` and report status `intentional`, not `mismatch`. Do not rename source to match the 68K spelling for those. Add or remove `INTENTIONAL` entries when a divergence is introduced or retired.

Translation units follow the same evidence rule: default stem is the primary class name; proven original basenames that differ live in `tools/check_tu.py` as `OVERRIDE_STEMS`. Full-path exceptions live there as `INTENTIONAL`.
