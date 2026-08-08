# LEMBALL LARGE-POOL BRIEF — PART 3 (still-unowned, deterministic)

Follow the same environment/commands/rules as parts 1-2 (`LEMBALL_large_pool_brief.md`): build via `cmd.exe /c` with `.decomp-venv\Scripts` first on PATH, measure from REGENERATED `build-msvc400/reccmp.json`, disassemble via `mcp__ghidra__disassemble_function` `program="/LEMBALL.EXE"`, lint `python tools/lint_reccmp_metadata.py`, Mac sigs via `tools/macintosh_naming.py objdiff_name`.

## SESSION CONTEXT (2026-08-08)
- ALL work lands directly on `main`. **NO separate branches or worktrees** (user explicit). Before writing: `git pull --rebase origin main`. After each 2+ converts: commit + push directly. History stays linear.
- A parallel stream (medium-pool subagents) also works `main`, and NEW functions are being reconstructed concurrently. **Always dedup-check each address FIRST** (grep `src` + both reccmp CSVs, case-insensitive) — skip if now owned.
- Objdiff pool: **266 unowned** remain (133 medium / 120 large / 13 small). This brief covers the LARGE (>250B) pool that is STILL unowned.

## Confirmed pitfalls
- **Stale-json false ABSENT/non-bindable**: clean-first rebuild, then read a REGENERATED reccmp.json. An "ABSENT" from stale json is a false negative (happened with 0x42a3a0 — agent called it non-bindable, it actually binds at 41%).
- **Exact-gate hygiene**: revert a bindable convert if it causes exact-set losses (CGameObject::Fall → 5 losses: 0x408DC0/0x427EC0/0x42C040/0x42F140/0x4662B0; was reverted for this).
- **Non-bindable, skip forever**: 0x426b50, 0x42f220, 0x415f30, 0x41f090.
- **MSVC 4.0 has no `__thiscall`** — use `Class::Method` member form, or `__fastcall X_M(void* pThis, int nEdxSlop, args)` for the debug-text/EDX-slop cases. Keep byte offsets exact (named fields at correct offset + reserved pads).
- **Register/ILT fidelity**: `this`→ECX, args on stack; helper thunk `((ret(__fastcall*)(...))0xNNNNNNNN)(this,...)`; vtable slot S `(*(ret(__fastcall**)(...))((*(void***)this)+S/4))(this,...)`.

## Routing
Class methods go to their real headers/home TUs (C2D→src/views/2d/C2DRender.cpp, CGameObject→src/AI/CGameObjectMove.cpp, CBall/CBullet/CAI/CPlayerLemming/CPlayerLemmingGroup(Manager)/CTrampoline/CIce/CTrapDoor/CCatapult/CSlinky/CMover/CDoor/CSheepGroup/CDemo/CGenericGroup→their src/AI files, VSQSort→a Visos/Generic utility TU). Class-less composite helpers (rasterize_/rebuild_/render_/decode_zrle_/remove_*/plot_/fill_/construct_*) → `src/Control/ClusterScanOps2.cpp` (or their obvious owner if a class exists).

## TARGETS — LARGE still-unowned (all verified unowned at brief time)

### A. Tractable class methods with Mac signatures (highest value — prefer these)
0x414150 CPlayerLemmingGroup::Process() (1180B) Process__19CPlayerLemmingGroupFv
0x42AB90 CTrampoline::Hit(const AICOORD&, CGameObject*) (969B)
0x440840 C2D::CalcGroundCode(eObjectType,int,int,ushort) (903B)
0x419060 CPlayerLemmingGroupManager::LoadAdditionalPlayerStartPositions(uchar*,ulong,uchar) (900B)
0x415A30 CGameObject::Move() (853B)
0x41B370 CObjectManager::Add(ushort,AICOORD,eObjectType,ushort,eObjectType) (852B)
0x41A7A0 CBullet::Process() (761B)
0x411C10 CAI::RemoteGameState(CGameStateMessage*) (706B)
0x43D990 C2D::DrawTrapDoor(CViewData&) (701B)
0x41BBC0 CObjectManager::LoadLevel(uchar*,ulong,uchar) (672B)
0x41C720 CCatapult::Process() (642B)
0x440560 C2D::DrawObjectsZBuff() (629B)
0x40FD10 CPlayerLemming::OnBalloon() (589B)
0x412890 CAI::StepOn(const AICOORD&, CGameObject*, ushort) (564B)
0x42D560 CIce::Switched() (564B)
0x421870 CBall::Move() (559B)
0x437E90 C2D::SetMouseShape() (552B)
0x43D130 C2D::DrawCatapult(CViewData&, int) (545B)
0x43D590 C2D::DrawDoor(CViewData&) (535B)
0x415300 CGameObject::Fly() (534B)
0x41F530 CSheepGroup::RunAway(AICOORD) (512B)
0x40C4F0 CTrapDoor::Process() (510B)
0x409250 CDemo::SendNextPacket(int) (503B)
0x437970 C2D::ScreenToGame(int,int,int&,int&) (494B)
0x421BC0 CBall::Process() (441B)
0x4364D0 C2D::OnLoaded() (434B)
0x40D760 CDoor::Delete() (417B)
0x40B760 CSlinky::Process() (374B)
0x43CBB0 C2D::DrawSlinky(CViewData&) (372B)
0x40CA40 CTrapDoorManager::LoadLevel(uchar*,int,uchar) (371B)
0x4158B0 CGameObject::SearchRoute() (368B)
0x463960 VSQSort (350B)
0x42EFF0 CMover::GetOn(CGameObject*) (334B)
0x40C3B0 CTrapDoor::GetViewData(CViewData&) (313B)
0x41F2E0 CSheepGroupManager::LoadLevel(tagLoadSheepData*,ulong,uchar) (313B)
0x437B60 C2D::ProcessMsg(tagMESSAGE*) (305B)
0x4204E0 CEnemy::EndFiring() (288B)
0x410100 CPlayerLemming::StartStanding() (278B)
0x416220 CGameObject::Fall() (275B) [caused 5 exact losses — retry ONLY if you can avoid spreading register state]
0x41A5C0 CBullet::Set(ushort,eBulletType,eOwner,int,AICOORD,AICOORD) (268B)
0x43C090 C2D::DrawLemmingExternal(CViewData&, uchar) (264B)
0x438210 C2D::OnButtonDown(const CVSPoint&, BUTTON_FLAGS) (260B)
0x43C6E0 C2D::DrawRocket(CViewData&) (258B)
0x43BDE0 C2D::DrawLemmingFlyShadow(CViewData&) (256B)
0x43BCE0 C2D::LemmingFly(CViewData&, int&) (253B)
0x40FF70 CPlayerLemming::RequestBalloon() (251B)

### B. Class-less composite helpers (>250B, no Mac sig) — chart/raster/ZRLE/remove families
Highest-value first (dedup each; route to ClusterScanOps2.cpp unless an obvious class owns them):
0x43E220 rasterize_dynamic_terrain_markers_and_masks (2509B)
0x43F620 rebuild_level_screen_static_render_regions (1711B)
0x444050 measure_level_screen_pause_dialog_layout (1574B)
0x47C260 construct_waveout_effect_patch_buffer (1468B)
0x44AEC0 emit_level_selection_mode_controller_render_entries (1122B)
0x4783C0 render_packaged_zrle_entry_via_helper (1064B)
0x43F060 rasterize_anim_and_lift_chunk_occupancy_masks (1048B)
0x478BB0 render_zrle_frame_at_point_via_helper (1017B)
0x425890 remove_fixed_size_record_by_address_and_compact (1004B)
0x42D910 remove_ice_chunk_object (1004B)
0x4241A0 remove_mine_chunk_object_by_address (957B)
0x40E140 remove_door_chunk_object_by_slot_id (956B)
0x40BA30 remove_type_0x35_chunk_object_by_slot (925B)
0x427FD0 remove_capture_trigger_chunk_object_by_slot (911B)
0x426C30/0x42B0C0/0x42C150 remove_managed_entity_array_entry_* (882B each)
0x429470 remove_lasr_chunk_object_by_address (880B)
0x442690 rebuild_level_status_panel_render_entries (787B)
0x450160 layout_password_entry_text (755B)
0x477C60/0x476BF0/0x476910/0x477F50/0x477740/0x479BD0... decode_zrle_rows_* family (clipped/palette/masked/write-mask)
0x476190 fill_circle_scanlines_clipped (726B)
0x75CE0 plot_circle_outline_quadrants_clipped (638B)
0x75490 fill_helper_surface_circle_command (568B)
0x6D5B0 recompute_child_helper_local_rect_from_point_delta (551B)
0x75290 draw_helper_surface_circle_outline_command (498B)
0x750C0 draw_helper_surface_line_command (450B)
0x6CBE0 update_helper_local_rect_from_packaged_entry (439B)
0x364D0... already listed A — skip dup
... then continue ranking the remaining `>250B` unowned entries from `data/objdiff-functions.csv` (dedup each as you go; the chart/zrle/render families land consistently at high %).

## Deliverable
Report per address: converted-match%, bindable, home TU, and the exact register/ILT divergence for any <25% or skipped. Prefer landing 4-8 solid high-% converts (the remove_/raster/scan families and C2D/CGameObject class methods land 40-90%) over churning many partials. Commit must be to `main`, prompt + linear. If you detect another agent mid-edit on a shared file (e.g. ClusterScanOps2.cpp), pick a different class-owned file first.
