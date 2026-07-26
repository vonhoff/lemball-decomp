# Lemmings Paintball Decompilation

Work-in-progress reconstruction of 1996 Win32 `LEMBALL.EXE`, built with MSVC
4.20 and compared against original binary with reccmp.

## Concurrent workers

Worker starts from clean integration checkout:

```powershell
python tools\coordinator.py start codex-01
```

Worker changes into printed worktree, reads `AGENTS.md`, works only claimed
range, commits source and `data/function-status` notes. Do not edit another
worker's checkout or run full shared-worktree builds.

Coordinator reviews and merges worker branch, runs full build/reccmp, then:

```powershell
python tools\coordinator.py release codex-01
```

`python tools\coordinator.py check` validates ledger before integration.

### Worker starter prompt

Copy this into a new worker task from clean integration checkout:

```text
Set OWNER=codex-01.

Run `python tools\coordinator.py start codex-01`, substituting OWNER. Change
into printed worker worktree. Read AGENTS.md. Work only returned range recorded
in CLAIMS.md.

Use Ghidra MCP and reccmp to make as many functions 100% as practical. Use
C/C++ only; no assembly. Follow build, measurement, dependency, and handoff
rules in AGENTS.md. Update range CSV, run checks available in worktree, and
commit source and notes. Keep claim active when handing off.
```

## Setup

Install `requirements.txt` into `.decomp-venv`. Original compiler must exist in
ignored `msvc420/`; comparison target is configured through reccmp project
files. Use commands in `AGENTS.md` for normal work.

## Legal

Encrypted non-working original executable is included only for automated
comparison. No game assets required for play are included. This unofficial
preservation project is not affiliated with any rights holder.
