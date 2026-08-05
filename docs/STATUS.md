# LEMBALL status tracker

Live checkpoints, genuine gaps, and dead-ends. Keep small. Update the `CURRENT` line
and `Genuine gaps` after each committed session; append to `Dead-ends` when a new quirk
is confirmed so it is never re-triaged. Goal prompt drives *what*, this tracks *remaining*.

## CURRENT

- Progress 76.46% | Impl 91.21% (2917/3198) | Accuracy 83.83% | Exact lost vs baseline: `0x417b50`, `0x462990` (pre-existing network-refactor deleters, not a regression)
- Head: `51d5a09` — AppendCStringToStream vtable-slot dispatch (50→81.8%)

## Genuine gaps (fixable, high-value)

| Addr | Function | State |
|---|---|---|
| 0x462990 | `??_ECWriteSocket` deleting dtor | lost; recover via CWriteSocket vtable refactor |
| 0x417b50 | `??_GManagedEntityPacketBase` deleting dtor | lost; same |
| 0x45e5d0/0x45e6e0 | CString append-with-result | hand-rolled double-buffer; rewrite to binary idiom |
| 0x45af20 | AppendCStringToFixedBufferStream | 24%, count-loop+vtbl dispatch done; register-noise remains |

## Dead-ends (confirmed not source-fixable; don't re-triage)

- Register-scheduling quirks (ebp-vs-esi, `lea` operand order, byte-vs-dword param reload, mul/div reg swap) — behaviorally identical, effective-100.
- Convert flat `__ct__`/`__dt__`/placement/vtable fns to real C++ ctor/dtor breaks codegen (CArena::~CArena→0.0). Keep ABI-anchored.
- `<OFFSET>` CRT-data (`_pctype`/`__mb_cur_max`) and vtable-store operand diffs — not normalizable via repo compat hook.
