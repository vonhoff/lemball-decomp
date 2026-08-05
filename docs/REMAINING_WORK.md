# LEMBALL — Remaining Work (continuous goal)

Goal: reach a clean state where
- every uncorrelated function is migrated into a real C++ class method
- naming matches the Macintosh blueprint (or blueprint-style for non-blueprint fns)
- misplaced functions are in the correct class
- legacy ALL-CAPS-extension files (.CPP/.H/) are migrated away and deleted when safe
- MACINTOSH annotations are removed (naming authority becomes the real C++ classes)

Authoritative data lives in `tools/reconstruction_audit.py`:
  --zeros     : zero-ratio game functions needing reconstruction (317)
  --missing   : blueprint classes absent from the source tree (skeleton candidates)
  --legacy    : legacy .CPP / .H / .C files (127) — migrated then deleted
  --mismatch  : source fn names vs blueprint (reconcile / reconstruct) — 23 found
  --misplaced : source class != blueprint class at an address — 24 candidates (verify via Ghidra first; some are heuristic false-positives)

## Audit / naming ground rules
- Mac mangle `Func__NNClassName<sig>`: the `C` prefix is REAL (inside the length field).
- Blueprint (macintosh-x86-correlations.csv) is the naming authority; Windows/AI names are inferred.
- Renaming a symbol does NOT change bytes (safe) as long as signatures match and all
  callers/references are updated in the same commit, and no ODR collision is introduced.
- Same-Named methods across classes must be disambiguated (e.g. CPlayerLemming_ vs CEnemy_)
  because they are defined as __fastcall free functions in one vtable TU.
- Writing multi-file UTF-8: strip BOM (MSVC4.00 chokes on EF BB BF). Never write utf-8-sig.

## Status
See git log for committed reconciliations (all 100% byte-identical, decomp-lint green,
no lost baseline addresses):
- 14/23 name mismatches reconciled (fire methods x2 classes, AddTime, nDead, GetFirst/NextBullet,
  DoButtons, DrawPaused, CheckAgainstCatapults, SetAutoDraw, ReturnFacingDirection,
  4 destructor thunks), ODR-disambiguated.

## Next concrete steps (priority order)
1. CBulletManager/CBullet real-member refactor (B): scaffold CBulletManager replacing
   ProjectilePool, CBullet replacing ProjectileObjectProxy; make Process()/Free()/GetFirst/GetNext
   real members. CONSTRAINT: g_LINKSCF_ProjectilePoolVtable (0x494008) stores raw __fastcall
   free-body addresses as slots — storing &Class::Process emits a member-pointer thunk, so this
   regresses vtable bytes (user authorized regression; fix after). ILT 0x00401019 pins Process.
   5 files: LEVELVT.CPP, Projectile.cpp, LVPRJGEOM.CPP, CGame.cpp, LINKSCF.CPP.
2. Misplaced-class fixes: start with confirmed CArena->CRAMArena, CMBlock->CMRAMBlock
   (blueprint distinguishes base CArena from derived CRAMArena; Windows collapsed them).
   Verify each of the 24 via Ghidra before acting (CNetworkMessage->CBaseSocket trio is false-positive).
3. Scaffold missing blueprint classes (--missing), migrate zero-ratio functions into them as members.
4. As legacy .CPP content is migrated, delete the .CPP files and remove MACINTOSH annotations.
