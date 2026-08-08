# LEMBALL SEMANTIC-CLEANUP BRIEF — legacy .CPP decompilation modernization

Background: LEMBALL has many legacy `.CPP` files full of **raw decompilation pointer-math** (`*(int*)((char*)this + 0x9c)`) and flat `void __fastcall SomeClass_Method(void* pThis, ...)` free-functions. Real class headers now exist (`src/AI/*.h`, `src/views/2d/C2D.h`) with named byte-exact fields, and many bodies are already semantic `Class::Method()` members.

## GOAL
Convert the REMAINING raw-pointer-math / flat-free-function bodies into clean semantic code:
- `*(int*)((char*)this + 0x9c)` → `this->m_xPosWorld9C` (field at that offset)
- `((void(**)(void*,int))(*(void***)this + SLOT/4))(...)` → named vtable-method call OR by-name real method where identity is known (Mac sig in `data/macintosh-x86-correlations.csv`)
- flat `void __fastcall CGameObject_StopMoving(void* pThis,...)` → `void CGameObject::StopMoving(...)`
Keep **byte-identical codegen** (same offsets, `this`→ECX, same ILT/vtable slot) — a refactor, not behavior change.

## CRITICAL RULES
1. Byte-offset fidelity: named field MUST be at the same offset as the raw cast it replaces. The header is source of truth. Unknown offset with no named field: either name+add to header (exact offset + reserved pads to align) or leave raw. Never shift layout.
2. Member conventions (MSVC 4.0): `void __fastcall X_M(void* pThis, int nUnused, args)` → `void X::M(args)` (drop pThis/nUnused). **NEVER write `__thiscall`** (reserved/breaking). For non-member cases (TEXTWIN debug-text/EDX-slop) use `__fastcall X_M(void* pThis, int nEdxSlop, args)`. vtable slot S: `(*(ret(__fastcall**)(...))((*(void***)this)+S/4))(this,...)`. ILT thunk: `((ret(__fastcall*)(...))0xNNNNNNNN)(this,...)`.
3. Cross-file callers: before renaming a function, grep whole `src/` for old-name callers; rename them too (else undefined-symbol build break).
4. Per-file verify: after modernizing, clean-first build (`cmd.exe /c "set PATH=C:\dev\lemball-decomp\.decomp-venv\Scripts;C:\dev\MSVC400\bin;%PATH% && ...cmake --build --preset msvc400 --target LEMBALL --clean-first"`), fix compile errors (missing field/decl → add to header), then confirm Implemented% did NOT drop (`cd build-msvc400 && ../.decomp-venv/Scripts/reccmp-reccmp.exe --target LEMBALL --json reccmp.json --json-diet`). If a modernized fn's match% drops vs its prior value, revert that rewrite.
5. Dedup: these are already-COMMITTED owned functions (have `// FUNCTION:`). REWRITE bodies to be semantic, keeping the `// FUNCTION: LEMBALL 0xADDR` line + offsets. Do NOT convert unowned functions here (that's the reconstruction stream).
6. Lint `python tools/lint_reccmp_metadata.py` before commit (clean, no dup). Commit+push in batches of 2-4 files, `refactor(semantic): modernize <files> (byte-identical)`.
7. **Work ONLY on `main`. NO branches/worktrees** (user explicit). `git pull --rebase origin main` before committing; commit+push promptly; don't race shared files (esp. ClusterScanOps2.cpp, LINKSCF.CPP).

## COORDINATION
Concurrent streams: medium-pool reconstruction + large-pool reconstruction both work `main`. Modernizing an already-owned file is low-collision, but pull-rebase before commit; if another agent is mid-edit on a file (e.g. ClusterScanOps2.cpp), pick a different file first. Everything lands on `main`, linear.

## PRIORITY FILES (raw-pointer-math density; biggest cleanup win first)
1. src/Control/ClusterScanOps2.cpp (305 casts — largest scratch TU)
2. src/Platform/Windows/Mixed/Engine/MEDIA/VSSOUND.CPP (239)
3. src/Frontend/MENUSEL.CPP (219)
4. src/Visos/Windows/CGDI.cpp (166)
5. src/Control/CGame.cpp (163)
6. src/AI/CInvisibleSwitch.cpp (162)
7. src/Platform/Windows/Mixed/Level/STATE/LEVELSTAT.CPP (109)
8. src/Platform/Windows/Mixed/Resource/VARLOAD.CPP (103)
9. src/Platform/Windows/Mixed/Engine/GDI/WINDOW/VSWINDOW.CPP (101)
10. src/Platform/Windows/Mixed/Engine/CORE/LINKSCF.CPP (101)
11. src/Frontend/MAINMENU.CPP (90)
12. src/Control/ClusterScanOps.cpp (80)
13. src/Platform/Windows/Mixed/Engine/NET/VSNET.CPP (66)
14. src/AI/CBullet.cpp (65)
15. src/AI/CPlayerLemming.cpp (60)
... then LEVELVT, LEVELRUN, VSNETTCP, CHUNKOBJVT, VSNETFIL, CGameObjectMove, TEXTWIN, CDuplicator, CDoorAct, CAI.

## Deliverable
Per file: # raw casts → named-field refs converted, confirmed byte-identical (build green + Implemented% unchanged/up), any offsets you had to name-and-add (with +off). Prefer finishing whole files over grazing.
