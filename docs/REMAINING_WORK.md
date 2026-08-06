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
- Exact-set regression gate: `tools/decomp_gate.py --check-baseline`
  (against `docs/exact-baseline.json`, tracked). Regenerate only on intentional
  improvements: run full `reccmp --json reccmp.json --json-diet`, then
  `tools/decomp_gate.py --save-baseline`. Never accept a NEW
  lost address beyond the tolerated set (0x417b50 / 0x462990).
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
- `reset_ball_chunk_manager_load_cursor` -> `CBallManager::ResetBallChunkManagerLoadCursor`
  (0x422050), migrated free __fastcall fn to real member, 100% byte-identical.

## Lessons (verify before bulk-rename)
- A "// FUNCTION: 0xADDR" comment followed by a snake/Thunk NAME is often a CALL/SITE INSIDE a
  real method (e.g. CGenericGroup::Restart calls ResetManagedEntityRuntimeStateThunk), NOT the
  function at that address. Always confirm the placeholder is the DEFINITION at the mapped
  address before renaming (parse the def line: `Ret NAME(args) {`).
- Same-named methods across classes (7x `Restart`) collide as __fastcall free fns -> prefix with class.
- reccmp `name` is the ORIGINAL target name, not the source symbol; don't measure rename progress by it.

## Next concrete steps (priority order)
0. Manager object-count resets: `__fastcall reset/clear_*_{chunk,manager}_object_count` one-liners writing
   +0x34 promote to `C{X}Manager::ResetObjectCount()` (100% byte-identical). DONE (all): CIceManager(0x42d900),
   CMoverManager(0x42f2e0), CLaserManager(0x4297e0), CPaintGunManager(0x42c140), CTrampolineManager(0x42b0b0),
   CRocketManager(0x426c20). ALSO renamed LinkScfPgun/Tram/RockChunkManagerView->CPaintGunManager/CTrampolineManager/
   CRocketManager (Mac class names) in LINKSCF.H/CPP + CGame.cpp.
   BLOCKER for the remaining LinkScf*View (Door/Move/Enmy/Shpg/Plas/Boon): those Mac classes
   (CDoorManager, CMoverManager, etc.) are NOT safe to rename to because DUPLICATE local structs
   already define them across other TUs. DONE: CMoverManager consolidated into shared src/ai/CMoverManager.h
   (LEVELSTAT+LEVELVT, byte-faithful). NEXT: CDoorManager has 4 divergent local variants (CDoorAct has
   m_nCapacity38+Switch/Restart/Open/Initialise; CDoorSlot has reserved38+Id(); LVACTDSP/LVMODESLOT decl-only)
   - consolidate into a shared header with union of fields+methods; then remaining Enmy/Shpg/Plas/Boon.
   DONE: CMoverManager (LEVELSTAT+LEVELVT), CDoorManager (4 variants). 
   Small identical view/tuple structs (3-int point types) consolidated: LevelPoint3 (3 files), LevelBoundsPoint (2),
   LevelNodePoint (2). Pattern proven byte-safe: create src/ai/X.h, replace per-TU local 'struct X{..}' with #include.
   LARGE INVENTORY: the ~48 remaining "struct in >1 file" are mostly (a) LEGITIMATE local views of shared-header
   types (VsNetEffStreamCommon, LevelChunkObjectBaseView, CGround) — NOT ODR dups, leave; (b) entangled vs-net
   transport structs with divergent layouts + cross-refs — high risk, low per-struct value. ODR-cleanup of simple
   identical self-contained dups is essentially DONE (16 consolidated). PIVOT to reconstruction backlog (below)
   rather than force risky vs-net merges.
4. RECONSTRUCTION (the core remaining value): the ~317 zero-ratio + ~391 weak uncorrelated game functions
   (construct_*/destroy_*/emit_*/dispatch_*/handle_* families). Migrate them into proper C++ class methods using
   address-range + Mac blueprint correlation (tools/reconstruction_audit.py --zeros). This is the main goal.
   PROGRESS: first verified reconstruction LANDED - append_invs_chunk_object_with_bounds -> CInvisibleSwitchManager::
   AddInvsChunkObjectWithBounds (0x40a3e0), 100% byte-identical (impl 2944->2945). SKILL: use
   lemball-function-reconstruction for the byte-fidelity pattern (ILT-callee resolution via reccmp-linker-ilts.csv,
   register-reuse rule, reject stack-packing thunks). Rejected: append_invs_chunk_object_at_point (stack-packing, 0%, revert),
   collect_free_managed_entity_slot_ids 0x4166d0 (multi-counter bitset loop, 46.75% register-codegen, revert),
   find_counted_level_child_containing_point (unreconstructed ILT callees). MEASURED YIELD: ~1 in 4 reconstructs clean.
   PRIORITIZE targets shaped like the success: simple `if(count<capacity){ array[idx]->Method(...); count++; }` with
   ALREADY-reconstructed ILT callees. Mult-counter loops + stack-packing thunks + unresolved-ILT targets = skip.
1. CBulletManager/CBullet real-member refactor (B) — IN PROGRESS:
   - DONE: renamed ProjectilePool->CBulletManager, ProjectileObjectProxy->CBullet,
     ProjectilePoolOwnerView->CBulletManagerOwnerView (type renames); promoted
     CBulletManager::GetFirstBullet/GetNextBullet to real members (100% byte-identical).
   - NEXT: CBulletManagerProcess(0x418040)->CBulletManager::Process DONE, GetFirstBullet/GetNextBullet DONE,
     CBulletFree(0x41ac70)->CBullet::Free DONE, ResetProjectilePool DONE.
     CONSOLIDATION DONE: duplicate CBulletManagerOwnerView (Projectile.cpp) merged into single shared
     CBulletManager in src/ai/CBulletManager.h; CBullet also migrated to src/ai/CBullet.h (both byte-faithful).
     To-do: define CBullet::ServiceProjectile/EmitProjectileRenderEntry/SetState bodies (currently
     vtable-virtuals, no source bodies). CAUTION: g_LINKSCF_ProjectilePoolVtable(0x494008)
 stores raw __fastcall free-body addr as slot2; ILT 0x00401019 pins Process. To keep vtable bytes,
 keep a thin __fastcall forwarding thunk at the vtable name OR accept member-pointer vtable thunk
 (regression authorized). Files: LEVELVT.CPP, Projectile.cpp, LVPRJGEOM.CPP, CGame.cpp, LINKSCF.CPP.
2. Misplaced-class fixes: 
   - CArena/CMBlock: Ghidra confirms 0x45a3f0 = CRAMArena::CRAMArena (derived, magic 0x5241524e "RARN") that
     calls CArena base ctor; source collapsed base+derived into one `CArena`. This is a genuine base/derived
     class-hierarchy collapse needing a structural CArena/CRAMArena split — HIGH RISK (foundational allocator,
     every allocation path) -> DEFER unless user wants the deep split. Verification caught several of the 24
     are heuristic false-positives (CNetworkMessage->CBaseSocket trio). Fix the confirmed-true renames only.
   - Verify each of the 24 via Ghidra before acting.
3. Scaffold missing blueprint classes (--missing), migrate zero-ratio functions into them as members.
4. As legacy .CPP content is migrated, delete the .CPP files and remove MACINTOSH annotations.
