# Lemmings Paintball Decompilation

Work-in-progress reconstruction of 1996 Win32 `LEMBALL.EXE`, built with MSVC
4.20 and compared against original binary with reccmp.

## Start agent

Change only owner:

```text
set OWNER=codex-01

Read AGENTS.md. Run `python tools\claims.py %OWNER%`, then work only returned
range. Use Ghidra MCP and reccmp to make as many functions 100% as practical.
Use C/C++ only; no assembly. Follow build, measurement, dependency, and handoff
rules in AGENTS.md. Keep claim active when handing off.
```

## Setup

Install `requirements.txt` into `.decomp-venv`. Original compiler must exist in
ignored `msvc420/`; comparison target is configured through reccmp project
files. Use commands in `AGENTS.md` for normal work.

## Legal

Encrypted non-working original executable is included only for automated
comparison. No game assets required for play are included. This unofficial
preservation project is not affiliated with any rights holder.
