# LEMBALL SESSION BOOTSTRAP — pick up the reconstruction grind

Use as the opening prompt for a fresh Hermes session. Repo: `C:\dev\lemball-decomp`, branch `main` (single canonical copy, atomic at `35960234` at write time). Current: **~276 unowned** functions in the objdiff pool (≈133 medium 30-250B, ≈120 large >250B, ≈23 small/tiny). Implemented ≈99.5%. **Objective: drive the uncorrelated cluster toward 0 and retire the legacy scratch files.**

## Your job
Run the objdiff-pool reconstruction grind, wave by wave: convert un-owned functions from `data/objdiff-functions.csv` into correlated `// FUNCTION: LEMBALL 0xADDR` reconstructions (each = a "convert") until the cluster is empty. Use `delegate_task` same-model subagents for parallel wave-grinds (each owns a disjoint set of files), then consolidate.

## Mechanism (critical)
`reccmp.json` dynamically adds a new address after a **clean-first rebuild**, but ONLY if **bindable** (has a PDB symbol). **Always clean-first rebuild + regenerate reccmp.json before judging any address** — a stale json shows false ABSENT (this exact mistake cost a wave before: 0x42a3a0 mis-called non-bindable, actually binds at 41%).

## Hard rules
1. **Work ENTIRELY on `main`. NO separate branches or worktrees** (user explicit — a parallel strong agent once ran in worktrees and made pulls messy; now consolidated to a single main). `git pull --rebase origin main` BEFORE writing; commit+push directly to main after each 2+ converts. Keep history linear.
2. **Dedup FIRST** for every target: grep `src` + `data/reccmp-source-symbols.csv` + `data/reccmp-compiler-generated.csv` (case-insensitive address). Skip if owned.
3. **Keep ≥25% bindable** converts; **revert below-bar**; always revert non-bindable. Never commit unverified/broken output — build clean-first and measure every convert.
4. **Route to home TU**: class methods → their `src/AI/*.cpp` class file (use the named, byte-exact header fields; name-and-add unknown fields at exact offset with reserved pads, or leave raw — never shift layout). Class-less helpers → `src/Control/ClusterScanOps2.cpp`. Mac sigs for param types from `data/macintosh-x86-correlations.csv`.
5. **MSVC 4.0**: NEVER the `__thiscall` keyword (reserved, won't compile). Use member form (compiles to thiscall) OR `__fastcall X_M(void* pThis, int nEdxSlop, args)` free-fn for the debug-text/EDX-slop cases (TEXTWIN.CPP methods 0x473e20/0x474340/0x474430). If an edit won't compile: revert it + move on.
6. **Lint** `python tools/lint_reccmp_metadata.py` before commit (clean, no dup addresses). **Exact-gate hygiene**: revert a bindable convert if it causes exact-set losses (e.g. CGameObject::Fall → 5 losses: 0x408DC0/0x427EC0/0x42C040/0x42F140/0x4662B0).
7. **Coordination**: a stronger agent ALSO works `main` (large pool + semantic cleanup of legacy `.cpp` files). Pull-rebase before writing; commit promptly; don't race shared files (esp. `src/Control/ClusterScanOps2.cpp`).

## Environment
- Build: `cmd.exe /c "set PATH=C:\dev\lemball-decomp\.decomp-venv\Scripts;C:\dev\MSVC400\bin;%PATH% && ...cmake --build --preset msvc400 --target LEMBALL --clean-first"`.
- Measure: `cd build-msvc400 && ../.decomp-venv/Scripts/reccmp-reccmp.exe --target LEMBALL --json reccmp.json --json-diet`, read `reccmp.json['data']` per `address`, `matching` (0..1).
- Disassemble reference: `mcp__ghidra__disassemble_function`, `program="/LEMBALL.EXE"`.
- Use ONLY `.decomp-venv` (never `.venv`).

## Never-retry (non-bindable)
0x426b50, 0x42f220, 0x415f30, 0x41f090.
**Known walls** (only attempt to beat): C2D DrawMover/Laser/Duplicator (<12%), CBulletManager::RequestBullet (0x417fa0, ret 0x28), CTextManager::DrawString (0x469fd0, vtable-heavy), CEnemy::Process (0x41fec0, compressed switch).

## Proven wave recipe
Pick ~10-14 smallest un-owned medium (30-250B) functions → disassemble → dedup → write to home TU → clean-first build → measure → keep ≥25% bindable / revert below → lint → commit+push `rehome(objdiff medium waveN): <fns>`. Expect ~2-5 converts/wave. Smallest first converts cleanest. The array-scan / remove_* / construct/destroy-resource families historically land 40-90%.

## Start
Pull fresh un-owned medium list from `data/objdiff-functions.csv` (dedup vs src + CSVs), and dispatch wave 10+. Also consider handing the companion briefs to a stronger agent: `data/LEMBALL_large_pool_brief_part3.md` (large >250B pool) and `data/LEMBALL_semantic_cleanup_brief.md` (legacy .cpp modernization).
