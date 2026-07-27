# Lemmings Paintball Decompilation

Work-in-progress reconstruction of 1996 Win32 `LEMBALL.EXE`, built with MSVC
4.20 and compared against original binary with reccmp.

Original executable reports LINK 3.00 and contains an incremental link table at
`0x00401000` through `0x00403890`; real code begins at `0x00406160`. Current
MSVC 4.20 build deliberately uses `/INCREMENTAL:NO` for function-level
comparison. Whole-image parity remains separate work: resource section, CRT,
and imports differ.

## Decompilation workflow

Use one checkout and one editing agent at a time. Other agents may inspect
Ghidra or suggest work, but do not edit or build concurrently.

Start with a shape-first batch:

1. Save current non-stub 100% address set from `build-msvc420/reccmp.json`.
2. Group unmatched real functions by normalized original instruction shape.
3. Prefer largest cluster with exact exemplar or simple compiler-owned C++
   pattern.
4. Inspect every member in Ghidra, recover one parameterized C/C++ form, and
   implement all proven members.
5. Compare every member and run full JSON. Keep zero-loss exact gains, then
   continue next cluster.

This turns repeated constructors, destructors, scalar deletes, vtable restores,
accessors, and small forwarding bodies into one compiler investigation plus
many verified matches. Shape similarity is candidate generation, never proof:
ABI, stack cleanup, offsets, constants, xrefs, and source ownership still need
per-member confirmation.

Use coordinator only after high-yield families are exhausted:

```powershell
python tools\coordinator.py
```

Coordinator remains useful for dependency bootstrap, ILT inventory validation,
and fallback singleton ranking. It deliberately deprioritizes five-byte
functions and untouched stubs, then estimates remaining mismatched bytes to
favor cheap, nearly complete targets.

### Cron worker prompt

Copy this into next editing task:

```text
Work autonomously on Lemmings Paintball decompilation project in
C:\Users\Simon\CLionProjects\lemball-decomp. Follow AGENTS.md.

Continue unfinished work. If none remains, refresh reccmp results when stale
and run shape-first batch workflow: group unmatched source-owned real functions
by normalized original instruction shape, prioritize highest-yield proven
family, inspect every member in Ghidra, recover one minimal C/C++ template, and
verify every member. Preserve all previous 100% addresses with before/after
exact-set gate. Continue through additional viable clusters; do not stop after
one tiny function. Use coordinator only after batch candidates are exhausted,
as dependency, inventory, and fallback singleton helper.

Run canonical checks, review all diffs, commit verified progress, push, and
leave clean worktree. Finish only after successful commit and push or
repository-wide blocker. Return one or two plain-English result sentences.
```

## Setup

No separate setup command. `python tools\coordinator.py` creates missing
`.decomp-venv` and repairs missing pinned packages before printing targets.

## Legal

Encrypted non-working original executable is included only for automated
comparison. No game assets required for play are included. This unofficial
preservation project is not affiliated with any rights holder.
