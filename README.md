# Lemmings Paintball Decompilation

Work-in-progress reconstruction of 1996 Win32 `LEMBALL.EXE`, built with MSVC
4.20 and compared against original binary with reccmp.

## Concurrent agents

Primary integration checkout holds `msvc420`, `.decomp-venv`, and original
`data\LEMBALL.EXE`. Worker setup points at those files; build output is private.

Coordinator provisions workers sequentially from clean integration checkout:

```powershell
python tools\claims.py codex-01
git add CLAIMS.md
git commit -m "Claim codex-01"
git worktree add -b worker/codex-01 ..\lemball-decomp-codex-01 HEAD
python tools\setup_worker.py ..\lemball-decomp-codex-01
```

Repeat with another owner only after prior claim command and commit complete.
Give each agent its worktree path and claim. Agents do not create claims.

Coordinator merges one worker branch at a time, runs full build/reccmp, then:

```powershell
python tools\claims.py release codex-01
git add CLAIMS.md
git commit -m "Release codex-01 claim"
git worktree remove ..\lemball-decomp-codex-01
git branch -d worker/codex-01
```

`python tools\claims.py check` validates ledger before integration.

### Worker starter prompt

Copy this into an already-provisioned worker task, replacing both values:

```text
Workspace: C:\path\to\lemball-decomp-codex-01
Claim: text-001.

Read AGENTS.md. Work only in assigned workspace and claim. Do not run
tools\claims.py or edit another workspace.

Use Ghidra MCP and reccmp to make as many functions 100% as practical. Use
C/C++ only; no assembly. Follow build, measurement, dependency, and handoff
rules in AGENTS.md. Update range CSV, run checks available in worktree, and
commit source and notes. Keep claim active when handing off.
```

## Setup

Run commands in `AGENTS.md` for normal work.

## Legal

Encrypted non-working original executable is included only for automated
comparison. No game assets required for play are included. This unofficial
preservation project is not affiliated with any rights holder.
