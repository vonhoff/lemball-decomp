# Lemmings Paintball Decompilation

Work-in-progress reconstruction of 1996 Win32 `LEMBALL.EXE`, built with MSVC
4.20 and compared against original binary with reccmp.

## Decompilation workflow

Use one checkout and one editing agent at a time. Other agents may inspect
Ghidra or suggest work, but do not edit or build concurrently.

Get next range:

```powershell
python tools\coordinator.py
```

Coordinator prints first unfinished range and every function in it not at 100%.
Agent works that range, measures, updates status CSV, and commits before next
editing agent starts.

### Worker starter prompt

Copy this into next editing task:

```text
Read AGENTS.md. Run `python tools\coordinator.py`; work only returned range.
You are sole editing agent. Do not run another build or edit concurrently.

Use Ghidra MCP and reccmp to make as many functions 100% as practical. Use
C/C++ only; no assembly. Follow build, measurement, dependency, and handoff
rules in AGENTS.md. Update range CSV, run checks, and commit source and notes.
```

## Setup

Run commands in `AGENTS.md` for normal work.

## Legal

Encrypted non-working original executable is included only for automated
comparison. No game assets required for play are included. This unofficial
preservation project is not affiliated with any rights holder.
