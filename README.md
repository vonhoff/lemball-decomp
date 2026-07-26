# Lemmings Paintball Decompilation

Work-in-progress reconstruction of 1996 Win32 `LEMBALL.EXE`, built with MSVC
4.20 and compared against original binary with reccmp.

## Decompilation workflow

Use one checkout and one editing agent at a time. Other agents may inspect
Ghidra or suggest work, but do not edit or build concurrently.

Get next targets:

```powershell
python tools\coordinator.py
```

Coordinator prints ten unmatched functions: five bytes or fewer first, then
increasing size, match percentage, and address. Agent works first viable target,
measures, and commits before next editing agent starts.

### Worker starter prompt

Copy this into next editing task:

```text
Work through every target printed by `python tools\coordinator.py` in
this session. Make every viable target 100% with C/C++ only; no assembly.

Read AGENTS.md, then run coordinator. It prepares missing `.decomp-venv`
dependencies and prints targets. Do not edit before it succeeds. Work targets
in order, using Ghidra MCP and reccmp. For each target, either reach 100% or
report exact evidence that blocks it.

You are sole editing agent. Do not run another build or edit concurrently.
Follow AGENTS.md build, measurement, dependency, and handoff rules. When done,
run checks, commit, and push.
```

## Setup

No separate setup command. `python tools\coordinator.py` creates missing
`.decomp-venv` and repairs missing pinned packages before printing targets.

## Legal

Encrypted non-working original executable is included only for automated
comparison. No game assets required for play are included. This unofficial
preservation project is not affiliated with any rights holder.
