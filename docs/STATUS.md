# LEMBALL status tracker

Live checkpoints, genuine gaps, and dead-ends. Keep small. Update the `CURRENT` line
and `Genuine gaps` after each committed session; append to `Dead-ends` when a new quirk
is confirmed so it is never re-triaged. Goal prompt drives *what*, this tracks *remaining*.

## CURRENT

- Progress 76.46% | Impl 91.21% (2917/3198) | Accuracy 83.83% | Exact lost vs baseline: `0x417b50`, `0x462990` (pre-existing network-refactor deleters, not a regression)
- Head: `898da52` — tracker; this round: 3 VSINIT vtable-dispatch wins (AppendCString/AppendCharToStream)

## Genuine gaps (fixable, high-value)

| Addr | Function | State |
|---|---|---|
| 0x462990 | `??_ECWriteSocket` deleting dtor | lost; recover via CWriteSocket vtable refactor |
| 0x417b50 | `??_GManagedEntityPacketBase` deleting dtor | lost; same |
| 0x45e5d0/0x45e6e0 | CString append-with-result | needs temp-CString (Construct/CopyConstruct/Destroy) model; complex, not quick |
| 0x45af20 | AppendCStringToFixedBufferStream | 24%, count-loop+vtbl dispatch done; register-noise remains |

## Unreconstructed `[gap]` family (door/trampoline/paintgun/ice managers)

Each is genuinely missing but infrastructure-bound — needs door ctor `0x401eba`→`0x40d470` ✅ (exists), door vtable Restart `[+0x104]`, and/or global tile-map `0x4a74b4` (`+0xc` tile data, `+0x10` w, `+0x14` h). Not isolated wins — reconstruct as a subsystem. CDoorManager::Initialise (0x40DFC0) is the tractable entry (array-alloc + ctor each 0x14c object + Restart).


## Dead-ends (confirmed not source-fixable; don't re-triage)

- Register-scheduling quirks (ebp-vs-esi, `lea` operand order, byte-vs-dword param reload, mul/div reg swap) — behaviorally identical, effective-100.
- Convert flat `__ct__`/`__dt__`/placement/vtable fns to real C++ ctor/dtor breaks codegen (CArena::~CArena→0.0). Keep ABI-anchored.
- `<OFFSET>` CRT-data (`_pctype`/`__mb_cur_max`) and vtable-store operand diffs — not normalizable via repo compat hook.
