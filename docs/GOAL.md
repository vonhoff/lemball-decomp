# LEMBALL continuous reconstruction goal

Reconstruct the original source the developers likely wrote, from collected evidence.
Each turn pick the best next target and land one small, verified, committed win. Drive
from `docs/BACKLOG.md` + the coverage inventory in `data/` — never improvise.

## Authorities
- **Layout/ABI = /LEMBALL.EXE** (bytes, vtables, offsets). Ground truth; never transfer Mac layout to Windows.
- **Naming = Macintosh 68K symbols.** /LEMBALL.EXE names (mostly AI-via-Ghidra) are NOT authoritative. Name after the Mac-correlated class; neutral Mac-style name only for Windows-only structure.

## Decide any Mac symbol (one disposition each in macintosh-symbol-coverage.csv)
- accepted_x86_correlation → implement
- platform_specific → don't implement
- genuinely_missing_or_unresolved → leave until correlated
- likely_inlined_or_merged → document, no stub

## Working procedure
1. Trace candidate in /LEMBALL.EXE via Ghidra (bytes/boundaries/ABI/flow/xrefs/owner). ILTs are never targets.
2. Categorize: reconstruct from `data/unreconstructed-correlations.csv`; reconcile an invented label via the `docs/BACKLOG.md` audit table; a near-match partial; or a documented bug. Pick highest-value, lowest-risk.
3. Recover minimal C/C++ (compiler-generated; assembly only for proven ABI/vtordisp thunks, `// ABI-THUNK:`).
4. Build MSVC 4.00 (env MSVC400_ROOT + INCLUDE/LIB/PATH at C:\dev\MSVC400, else MSVC 4.20 headers break it).
5. reccmp; compare lost-addrs vs `docs/exact-baseline.json`. Tolerated losses (exit 1) OK; never accept a NEW lost addr unless a bounded one is clearly outweighed. Recover vtable-display artifacts via metadata naming rather than shipping worse losses.
6. clang-format touched C/C++, `git diff --check`, clean tree, commit/push — updating coverage/correlations + reccmp metadata in the same change as any rename.

## Rename/reconciliation pitfalls
- Bulk rename regresses reccmp unless the CSV rows embedding the class name are updated in the same commit: `reccmp-compiler-generated.csv` (??_E/??_G) and `reccmp-linker-ilts.csv` (parameter mangling). grep src + these CSVs before build.
- Flattening is NOT always valid: verify shared-base status first; do not collapse a Windows-only decomposition (e.g. ManagedEntityPacket*) to one Mac class.

## Loop
After each committed win, immediately scan for the next best backlog target and proceed. Stop only when told, or state clearly that reconstruction is complete and why.
