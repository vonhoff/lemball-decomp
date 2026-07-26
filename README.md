# Lemmings Paintball Decompilation

This repository contains a work-in-progress reconstruction of the original 1996 Windows C++ codebase behind `LEMBALL.EXE`.

The game is not yet functional and remains under active development. The primary goal is to produce a complete decompilation of *Lemmings Paintball* that matches the original machine code as closely as reasonably possible. Once complete, the reconstructed codebase may also serve as a useful technical reference for future ports, reimplementations, and preservation work.

> [!WARNING]
> The reconstructed source code is intentionally kept close to the behavior and structure of the original release, including its platform-specific assumptions. The game targets 32-bit Windows and is not expected to build or run on other platforms without substantial additional work.

## Start an AI worker

Coordinator assigns an available range. Edit only first two lines:

```text
set OWNER=vonhoff
set RANGE=text-042

Read AGENTS.md and execute its workflow for %RANGE% as %OWNER%. Range is
assigned; never choose or edit another. Use Ghidra MCP and reccmp. Make at
least one previously non-exact function 100% using C/C++ only. Record every
attempt and blocker. Keep claim active for handoff. Communicate tersely.
```

## Legal

This repository contains an encrypted, non-working copy of the original *Lemmings Paintball* executable, which is used solely to support automated binary comparison and verification during development. It is not intended to provide a playable version of the game or replace the original software. No game data, artwork, audio, or other proprietary assets required for normal gameplay are included. Users are responsible for supplying any required game files from a copy they are legally entitled to use.

All copyrights, trademarks, and other intellectual property rights relating to *Lemmings Paintball* and the original software remain the property of their respective rights holders. This is an unofficial preservation and research project. It is not affiliated with, authorized by, sponsored by, or endorsed by any current or former rights holder.
