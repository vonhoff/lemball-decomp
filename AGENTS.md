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

## Source rules

No inline asm. One primary class per `.h`/`.cpp` (stem = class), unless `tools/lib/layout.py` `OVERRIDE_STEMS`. Functions in ascending original x86 address order. Use `RES_*` from `Manifest.h`. Prefer named members over offset pokes. Keep `undefined`/`undefined2`/`undefined4` until Win32 evidence justifies a tighter type. Preserve original loop shape, 32-bit size math, post-virtual pointer re-fetches, and message `switch` widening. Stop at compiler noise (reg alloc, alignment NOPs).

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
