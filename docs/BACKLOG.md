# LEMBALL Decompilation Backlog & Decision Framework

This document is the single source of truth for what to implement, what to treat
as a blueprint, and what to leave alone. Let it drive every session; do not improvise.

The governing rule is **AGENTS.md §22** and the coverage inventory in
`data/macintosh-symbol-coverage.csv` (2849 symbols, one disposition each). The
Windows binary is authoritative for code and layout; the Macintosh source is
evidence for naming and logical ownership only.

## Decision framework: map vs blueprint vs do-not-implement

| Mac symbol category | Count | Meaning | Action |
|---|---|---|---|
| `accepted_x86_correlation` | 1374 | Mac method proven to correspond to a Windows function. | **Implement / map it.** |
| `platform_specific` | 884 | Mac-only CODE (Toolbox, CFM, FS, sound, display, input). | **Do NOT implement on Windows.** |
| `genuinely_missing_or_unresolved` | 577 | No proven Windows counterpart yet. | Leave until x86 correlation proves one. Research, not construction. |
| `likely_inlined_or_merged` | 14 | Mac method inlined/merged into a Windows function. | Document; do not spawn a stub. |

Everything in `accepted_x86_correlation` already carries a proven x86 address.
The **only actionable backlog** is the subset that is correlated but not yet
reconstructed in source — see [_Data_](#data) and item 1 below.

## Naming authority (what to call code in this codebase)

When building, name by this order of priority:

1. **Windows binary wins.** Use the `/LEMBALL.EXE` mangled symbol
   (e.g. `Add__15CNetworkMessage...` → class `CNetworkMessage`). This is
   authoritative for class/function names.
2. **Else, the Macintosh mangled name** for logical ownership/terminology, only
   *after* independent x86 correlation (AGENTS.md §22.3).
3. **Else, a descriptive neutral name** based on the Windows structure, clearly
   marked, only when nothing maps. Never apply a Mac-inherited name to code that
   is Windows-only (e.g. VSGDI surface/upload topology — AGENTS.md §40).

Notes from past work:

- Shared-base classes that the binary does **not** name (e.g. `VsNetEffStreamCommon`,
  `GameEffStream`) are reconstruction labels for genuine shared bases — not
  Mac-named classes. Do not rename them to a Mac class; the binary has no such symbol.
- Mac-only framing methods with no x86 equivalent (e.g. `CNetworkMessage`'s
  `CheckMessage`/`GetHeader`/`AddHeader`) are `platform_specific` — never add them.
- A bulk class rename only compiles-and-matches cleanly if the reccmp symbol rows
  (`reccmp-compiler-generated.csv` `??_E`/`??_G`, `reccmp-linker-ilts.csv`
  parameter mangling) are updated in the same commit. Keep those in lockstep.

## Working procedure (every session)

For the top ready item:

1. Trace the candidate in `/LEMBALL.EXE` with Ghidra (bytes, boundaries, ABI,
   xrefs, owner). ILT entries are never targets.
2. Recover a minimal C/C++ form from binary evidence + VC4.00 probes. Use the
   naming rule above.
3. Build the canonical MSVC 4.00 target (env: `MSVC400_ROOT=C:/dev/MSVC400`,
   plus `INCLUDE`/`LIB`/`PATH` pointed at `C:\dev\MSVC400` — the plain cmake
   preset otherwise picks up contaminated MSVC 4.20 headers).
4. Run reccmp + compare the lost-address set against `exact-baseline.json`.
   Exit `1` for tolerated losses is expected; **never accept a new lost address**
   beyond the known tolerated set unless the gain clearly outweighs a bounded one.
5. clang-format touched C/C++, commit/push. Update coverage correlations and
   naming metadata in the same change.

## Data

- `data/macintosh-symbol-coverage.csv` — every Mac symbol, one disposition (the source of truth).
- `data/macintosh-x86-correlations.csv` — reviewed Mac→Windows mappings.
- `data/unreconstructed-correlations.csv` — generated work-list of accepted
  correlations that are not yet reconstructed in source (item 1 below).
- `data/macintosh-68k-symbols.csv` — raw symbols.

Regenerate the work-list with:
`.decomp-venv\Scripts\python.exe tools/generate_unreconstructed_backlog.py`

## Backlog (evidence-based, priority order)

1. **264 accepted-but-unreconstructed correlations.** Highest-value, lowest-risk.
   Each is a proven name + x86 address, needing only source reconstruction
   (item 3 in the table; work-list in `data/unreconstructed-correlations.csv`).
   Spread: 87 views_2d, 72 AI, 53 Control, 47 Visos/Generic, 3 Mac-specific,
   2 Frontend. Includes clean families (e.g. `CString`, `CPVZBuffSurface`,
   `CGDIDevice`, `CSurface`, `FindFreeSurface`) that reconstruct quickly.
2. **3 partial CNetworkMessage transport functions** — bounded, in the shared base:
   `OpenDataStream` (0x45F1E0, 64.7%), `CopyDataStream` (0x45F250, 43.9%),
   `Send` (0x45F2B0, 66.7%).
3. **~100 functions at 90–99%** — mostly register/scheduling quirks (reccmp already
   blesses these as behavior-identical). Chase the ones with real near-misses; skip pure quirks.
4. **Documented genuine bugs** — `CLiftManager::StepOn` (wrong class view; original
   dispatches `CInvisibleSwitch::StepOn`, source uses `CLift`), and the `CString`
   append-vs-destroy leads. Real reconstruction bug fixes.
5. **Deferred / research** — 577 `genuinely_missing_or_unresolved` Mac symbols and
   248 Windows stubs. Not assignment targets until correlated.

## Do-not-implement list

- All 884 `platform_specific` Mac symbols (no Windows equivalence by design).
- All 577 unresolved — do not fabricate a Windows function for them.
- Mac-only methods on a class that the Windows binary lacks.
