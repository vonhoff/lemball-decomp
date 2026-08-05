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

Two separate authorities:

- **Layout/ABI authority — the Windows binary.** `/LEMBALL.EXE`'s bytes, vtables,
  field offsets, and structure are ground truth (AGENTS.md §22.6). Never transfer
  Macintosh *layout* to Windows.
- **Naming authority — the Macintosh source.** The mangled class/method names from the
  Japanese Macintosh 68K symbols are the closest thing to the original source's naming.
  The `/LEMBALL.EXE` *names* are (mostly) inferred by an AI agent via Ghidra and carry
  **no serious authority** — do not treat them as ground-truth identifiers.

So when naming code, prefer:

1. **The Macintosh mangled name** for the correlated structure — after independent
   x86 correlation (AGENTS.md §22.3). E.g. for `CNetworkMessage`, the class/method
   names come from the Mac `CNetworkMessage` symbols, not from any Ghidra label.
2. **Else, a descriptive neutral name consistent with the Macintosh convention** for
   Windows-only structure that has no Mac counterpart, clearly marked as such. Keep
   the naming style (e.g. `C...` classes, Mac-style method intent) aligned to the
   reference convention rather than arbitrary AI labels.

Reconstruction-only labels that the Macintosh does *not* provide (e.g.
`VsNetEffStreamCommon`, `GameEffStream`, and other `VsNetEffStream*` names) are
**candidate refinements**, not authoritative class names. Refine them toward the
Macintosh convention for the correlated structure as evidence allows; never let an
AI-invented Ghidra name override a real Macintosh-correlated name.

Notes from past work:

- Shared-base classes (e.g. `VsNetEffStreamCommon`, `GameEffStream`) are
  reconstruction labels for genuine shared bases. Their *layout* is real; their
  *names* should be revisited against the Macintosh convention rather than kept as
  AI-invented identifiers.
- The `VsNetEffStreamBase` → `CNetworkMessage` rename (`0bdc6e7`) was a first step
  in that direction; the remaining `VsNetEffStreamCommon`/`GameEffStream` labels are
  still candidates for refinement.
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
