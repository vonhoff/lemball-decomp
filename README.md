# Lemmings Paintball Decompilation

Work-in-progress reconstruction of 1996 Win32 `LEMBALL.EXE`, built with MSVC
4.20 and compared against original binary with reccmp.

## Concurrent workers

Coordinator creates isolated worker checkout from clean integration branch:

```powershell
python tools\create_worker_worktree.py codex-01
```

Worker changes into printed worktree, reads `AGENTS.md`, works only claimed
range, commits source and `data/function-status` notes. Do not edit another
worker's checkout or run full shared-worktree builds.

Coordinator reviews and merges worker branch, runs full build/reccmp, then:

```powershell
python tools\claims.py release codex-01
git add CLAIMS.md
git commit -m "Release codex-01 claim"
```

`python tools\claims.py check` validates ledger in CI and before coordinator
integration. `claims.py OWNER` remains available for one-off local work.

### Worker starter prompt

Copy this after coordinator creates worker worktree and gives worker its path:

```text
Set OWNER=codex-01.

Work only in assigned isolated worktree. Read AGENTS.md. Do not run
`python tools\claims.py %OWNER%`: coordinator already created and owns active
claim. Work only returned range recorded in CLAIMS.md.

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
