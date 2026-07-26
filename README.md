# Lemmings Paintball Decompilation

Work-in-progress reconstruction of 1996 Win32 `LEMBALL.EXE`, built with MSVC
4.20 and compared against original binary with reccmp.

## Concurrent agents

### One-time host setup

From primary integration checkout, move local compiler, Python tools, and
original executable into one shared host cache:

```powershell
python tools\worker.py configure --toolchain-root C:\lemball-tools
```

The cache is outside repository and contains `msvc420`, `.decomp-venv`, and
`LEMBALL.EXE`. Worker worktrees use cheap junctions/hard links to it; only their
`build-msvc420` output is private.

### Provision workers

Coordinator runs this sequentially from clean primary integration checkout:

```powershell
python tools\worker.py start codex-01
python tools\worker.py start deepseek-01
```

Each command creates an exclusive claim, branch, and worktree. Give the printed
worktree path and claim to that agent. Agents do not create worktrees or claims.

Coordinator reviews and merges each worker branch, runs full build/reccmp, then:

```powershell
python tools\worker.py release codex-01
```

`python tools\worker.py check` validates ledger before integration.

### Worker starter prompt

Copy this into an already-provisioned worker task, replacing both values:

```text
Set OWNER=codex-01.
Workspace: C:\path\to\lemball-decomp-codex-01
Claim: text-001.

Read AGENTS.md. Work only in assigned workspace and claim. Do not run
tools\worker.py or edit another workspace.

Use Ghidra MCP and reccmp to make as many functions 100% as practical. Use
C/C++ only; no assembly. Follow build, measurement, dependency, and handoff
rules in AGENTS.md. Update range CSV, run checks available in worktree, and
commit source and notes. Keep claim active when handing off.
```

## Setup

Run shared toolchain setup above before creating workers. Use commands in
`AGENTS.md` for normal work.

## Legal

Encrypted non-working original executable is included only for automated
comparison. No game assets required for play are included. This unofficial
preservation project is not affiliated with any rights holder.
