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
measures, updates status CSV, and commits before next editing agent starts.

### Worker starter prompt

Copy this into next editing task:

```text
Read AGENTS.md. Run `python tools\coordinator.py`; start with first viable target.
You are sole editing agent. Do not run another build or edit concurrently.

Use Ghidra MCP and reccmp to make as many functions 100% as practical. Use
C/C++ only; no assembly. Follow build, measurement, dependency, and handoff
rules in AGENTS.md. Update status CSV, run checks, and commit source and notes.
```

## Setup

Run once after cloning, and again after `requirements.txt` changes:

```powershell
if (!(Test-Path .decomp-venv\Scripts\python.exe)) {
  python -m venv .decomp-venv
}
.decomp-venv\Scripts\python.exe -m pip install --requirement requirements.txt
```

This creates missing environment only and installs exact pinned tool versions.
`AGENTS.md` requires this preflight for workers.

## Legal

Encrypted non-working original executable is included only for automated
comparison. No game assets required for play are included. This unofficial
preservation project is not affiliated with any rights holder.
