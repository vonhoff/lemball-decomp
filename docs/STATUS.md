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

## C2D batch (71 gaps→69, dominant remaining block — workable via member-dispatch pattern)

C2D class spans +0x44→+0x22xx; funcs are 100-300B geometry/raster with dispatch tables, globals (0x4a9bf4 cursor, 0x49ce08/0x49ce04 time, 0x49eb80 variant mgr, 0x4a6408 modal, 0x49cb68) + subobjects (0x914 tile-grid {+0xc data,+0x10 w,+0x14 h}, 0x96c/0x95c arrays, 0x974/0x978/0x97c sinks). **BREAKTHROUGH**: single-arg dispatch methods reconstruct as thiscall member fns on LevelScreenManagedEntitySelectionView (this in ecx, ret 4): SelectLemming(0x437340,74.6%) + SelectObject(0x4373B0,65.6%) DONE. Remaining are event-dispatch-adjacent (NoStateLeftClick/GroupingLeftClick/LeftClick/RightClick/InGroupByObjectNo/RemoveFromGroupByObjectNo) then geometry (ScreenToGame/SetMouseShape/ProcessMsg verified complex) + Draw* batch. C2D.cpp has 5 methods; CGame.cpp has a few.


## Dead-ends (confirmed not source-fixable; don't re-triage)

- Register-scheduling quirks (ebp-vs-esi, `lea` operand order, byte-vs-dword param reload, mul/div reg swap) — behaviorally identical, effective-100.
- Convert flat `__ct__`/`__dt__`/placement/vtable fns to real C++ ctor/dtor breaks codegen (CArena::~CArena→0.0). Keep ABI-anchored.
- `<OFFSET>` CRT-data (`_pctype`/`__mb_cur_max`) and vtable-store operand diffs — not normalizable via repo compat hook.
- **Converting a free fn to a member changes EXTERNAL CALLERS' codegen** → regresses them (AppendUIntHex/AppendHexUInt→75%/63% but accuracy 83.83→83.79, reverted). Only safe if the fn has NO external callers (AppendPointerToStreamVariant→100% had none).
- `VsInitStreamFormatTargetState` subobject-offset divergence (`m_dwFlags`/`m_nRadix` read via `VsInitFORMAT_TARGET` at wrong base) blocks UIntHex/HexUInt exact match — needs struct/base audit.
