# LEMBALL status tracker

Live checkpoints, genuine gaps, and dead-ends. Keep small. Update the `CURRENT` line
and `Genuine gaps` after each committed session; append to `Dead-ends` when a new quirk
is confirmed so it is never re-triaged. Goal prompt drives *what*, this tracks *remaining*.

## CURRENT

- Progress 76.4% | Impl 91.24% (2927/3208) | Accuracy 83.7% | Exact lost vs baseline: `0x417b50`, `0x462990` (pre-existing network-refactor deleters, not a regression)
- **Macintosh blueprint portability coverage: 88.0%** (618/702 portable-scope w/ x86 target reconstructed; 84 gaps) — see `macintosh_naming.py coverage`
- Head: `7dd9505` — coverage metric + pointer-integrity

## Genuine gaps (fixable, high-value)

| Addr | Function | State |
|---|---|---|
| 0x462990 | `??_ECWriteSocket` deleting dtor | lost; recover via CWriteSocket vtable refactor |
| 0x417b50 | `??_GManagedEntityPacketBase` deleting dtor | lost; same |
| 0x45e5d0/0x45e6e0 | CString append-with-result | needs temp-CString (Construct/CopyConstruct/Destroy) model; complex, not quick |
| 0x45af20 | AppendCStringToFixedBufferStream | 36%, count-loop+vtbl dispatch done; register-noise remains |
| 0x458780/0x4589c0 | AppendUIntHex/AppendHexUIntToStream | member-conversion partial (75/63%) but caller-regression; needs subobject-offset audit |
| 0x4584c0/0x458450 | ApplyStreamIntegerWidthPadding/ConstructFormattedOutputStream | structural/base divergence, needs VSINIT stream subobject audit |
| 0x45ae10 | AppendCharToFixedBufferStream | 43%, tab/wrap/flush logic diverges |

## Unreconstructed `[gap]` family (door/trampoline/paintgun/ice managers)

All manager Restart/Initialise/Hit/StepOn/Open done (committed). Remaining: manager Add (Set-thunks), LoadLevel (stream-parse), CDoor::Set/Delete/collision (tile-map 0x4a74b4 heavy).

## C2D batch (71 gaps→62, easy tiers DONE; remaining hard tiers)

**DONE (9)**: SelectLemming(74.6%), SelectObject(65.6%), InGroupByObjectNo(37.9%), RemoveFromGroupByObjectNo(38.8%), CheckValidFormGroup(31.9%), FormGroup(54.9%), **MoveGroup(100%)**, **AddObjectToGroup(100%)**, IsInGrouping(51.1%). Patterns: (a) event-dispatch to sink974[+8]; (b) group array +0xa50 (+0xa4c count,+0xa4e cap). **LESSON**: index a 16-bit member DIRECTLY (e.g. `arr[m_nCount]` / `++m_nCount`) — a `unsigned short nCount` local makes MSVC widen to 32-bit regs (`movzx`) and diverge; direct member access reproduces the original `xor/mov dx,[mem]` sequence (AddObjectToGroup 28.6→100%). Counted-loop funcs (RemoveFromGroupByObjectNo/InGroupByObjectNo/IsInGrouping/CheckValidFormGroup) use register-scheduling idioms that don't converge cleanly — low ROI to chase. **REMAINING 62 all hard-tier (no clean pattern)**: remap-palette regs (0x4363C0 RegisterRemaps etc. — global palette tables 0x49e8b8/0x4a2000, array+string allocs), click-handlers (0x437520-0x437930 NoState/Left/RightClick — jump-table type dispatch 0x437644/0x437678, 4 args, call FindGameObject), geometry (0x437970-0x438210 ScreenToGame/SetMouseShape/ProcessMsg/FindGameObject — raster/scan/tile-grid), Draw* (~35, 0x43BCE0+ — rendering). Reconstructing any of these is multi-iteration with uncertain match; weak partials incur accuracy dip.


## Non-C2D gaps (7 CGameObject + singles)

CGameObject is fully-modeled (src/AI/CGameObject.h, sizeof 0x124). **ctor (0x414F30) DONE** as `ConstructCGameObject` member returning this (real C++ ctor suppressed CHand's default base — ABI-safe named member via //FUNCTION address). Slot model: +0x6a m_nRegistryIndex6A (loading registry, g_GAME_ManagedEntityRegistryTable/Count) distinct from +0x6c m_nSlotId6C (runtime bitset). **NEXT: tile-grid global 0x4a74b4 is UNMODELED and unblocks MapCheck(0x4157B0, single-cell tile-code w/ bounds->3), CDoor tile methods, ScreenToGame** — schema +0xc tile-data ptr, +0x10 width, +0x14 height. Other CGameObject (Move/SearchRoute/StartMoving/AlterDestination/StartLand) movement/search heavy. Singles: CGameStatus::NoOfLevelsInSkill(0x408F30), CPVButton ctor(0x467C10), sort trio VSQSort/shortsort/swap.

- Register-scheduling quirks (ebp-vs-esi, `lea` operand order, byte-vs-dword param reload, mul/div reg swap) — behaviorally identical, effective-100.
- Convert flat `__ct__`/`__dt__`/placement/vtable fns to real C++ ctor/dtor breaks codegen (CArena::~CArena→0.0). Keep ABI-anchored.
- `<OFFSET>` CRT-data (`_pctype`/`__mb_cur_max`) and vtable-store operand diffs — not normalizable via repo compat hook.
- **Converting a free fn to a member changes EXTERNAL CALLERS' codegen** → regresses them (AppendUIntHex/AppendHexUInt→75%/63% but accuracy 83.83→83.79, reverted). Only safe if the fn has NO external callers (AppendPointerToStreamVariant→100% had none).
- `VsInitStreamFormatTargetState` subobject-offset divergence (`m_dwFlags`/`m_nRadix` read via `VsInitFORMAT_TARGET` at wrong base) blocks UIntHex/HexUInt exact match — needs struct/base audit.
