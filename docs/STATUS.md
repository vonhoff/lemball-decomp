# LEMBALL status tracker

Live checkpoints, genuine gaps, and dead-ends. Keep small. Update the `CURRENT` line
and `Genuine gaps` after each committed session; append to `Dead-ends` when a new quirk
is confirmed so it is never re-triaged. Goal prompt drives *what*, this tracks *remaining*.

## CURRENT

- Progress 76.47% | Impl 91.21% (2917/3198) | Accuracy 83.83% | Exact lost vs baseline: `0x417b50`, `0x462990` (pre-existing network-refactor deleters, not a regression)
- Head: `dee079f` — AppendPointerToStreamVariant member+literals (71.7→100%)

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

Each is genuinely missing but infrastructure-bound — needs door ctor `0x401eba`→`0x40d470` ✅ (exists), door vtable Restart `[+0x104]`, and/or global tile-map `0x4a74b4` (`+0xc` tile data, `+0x10` w, `+0x14` h). Not isolated wins — reconstruct as a subsystem. CDoorManager::Initialise (0x40DFC0) is the tractable entry (array-alloc + ctor each 0x14c object + Restart).


## Dead-ends (confirmed not source-fixable; don't re-triage)

- Register-scheduling quirks (ebp-vs-esi, `lea` operand order, byte-vs-dword param reload, mul/div reg swap) — behaviorally identical, effective-100.
- Convert flat `__ct__`/`__dt__`/placement/vtable fns to real C++ ctor/dtor breaks codegen (CArena::~CArena→0.0). Keep ABI-anchored.
- `<OFFSET>` CRT-data (`_pctype`/`__mb_cur_max`) and vtable-store operand diffs — not normalizable via repo compat hook.
- **Converting a free fn to a member changes EXTERNAL CALLERS' codegen** → regresses them (AppendUIntHex/AppendHexUInt→75%/63% but accuracy 83.83→83.79, reverted). Only safe if the fn has NO external callers (AppendPointerToStreamVariant→100% had none).
- `VsInitStreamFormatTargetState` subobject-offset divergence (`m_dwFlags`/`m_nRadix` read via `VsInitFORMAT_TARGET` at wrong base) blocks UIntHex/HexUInt exact match — needs struct/base audit.
