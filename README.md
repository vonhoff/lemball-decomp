# Lemmings Paintball Decompilation

[![Build Status](https://img.shields.io/github/actions/workflow/status/vonhoff/lemball-decomp/build.yml)](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml)
[![Effective Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Feffective.json)](#matching)
[![Exact Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Fexact.json)](#matching)
[![Fuzzy Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Ffuzzy.json)](#matching)

This project is a matching decompilation of *Lemmings Paintball*, an action game in the *Lemmings* franchise developed
by Visual Sciences and published by Psygnosis in 1996 for Windows 95.

The goal is to reconstruct the game's codebase in idiomatic, maintainable C++ while preserving its original behavior and
matching the original machine code as closely as possible. The resulting codebase will serve as a faithful reference and
foundation for future ports.

> [!NOTE]
> This project targets 32-bit Windows using Microsoft Visual C++ 4.0. For a modern, cross-platform version,
see [lemball-portable](https://github.com/vonhoff/lemball-portable).

## Reconstruction

The Macintosh 68000 version retained Metrowerks and MacsBug debugging symbols, providing over 2,800 original function
names and revealing C++ class hierarchies.

The original Windows executable lacks debugging symbols, so functions were matched across versions using strings,
constants, algorithms, virtual tables, object layouts, and call relationships.

## Matching

Functions are compared against the original executable using [reccmp](https://github.com/isledecomp/reccmp).

- **Exact:** Non-stub functions with 100% raw assembly similarity.
- **Fuzzy:** Raw assembly similarity, including equivalent functions.
- **Effective:** Non-stub exact matches and reccmp equivalence results.

Exact and fuzzy metrics are tracked on [decomp.dev](https://decomp.dev/vonhoff/lemball-decomp)
and [bytematch.dev](https://bytematch.dev/vonhoff/lemball-decomp).

## Building

Requires Python 3.10+, Git, and PowerShell.

Install dependencies and the MSVC 4.0 toolchain:

```powershell
python -m venv .decomp-venv
.\.decomp-venv\Scripts\Activate.ps1
python -m pip install -r requirements.txt
git clone https://github.com/vonhoff/MSVC400 msvc400
```

Build the executable:

```powershell
.\.decomp-venv\Scripts\Activate.ps1
python tools/make_binary.py
```

Use `--disable-startup-checks` to bypass CD-ROM and installation checks.

## References

- [The Cutting Room Floor](https://tcrf.net/Lemmings_Paintball)
- [Game Data Digs](https://datadigs.info/doku.php?id=wiki:games:lemmings_paintball)
- [Reverse Engineering a DOS Game with Ghidra and Codex](https://alexbevi.com/blog/2026/03/14/reverse-engineering-a-dos-game-with-ghidra-and-codex)
- [isledecomp/isle](https://github.com/isledecomp/isle)
- [marijnvdwerf/legoland](https://github.com/marijnvdwerf/legoland)

## AI Disclosure

AI is used for research, tooling, and code reconstruction. All code is manually reviewed.

## Legal

This is an unofficial project unaffiliated with the original rights holders. Original game assets are not included.

No license is granted for reconstructed game code. Independent code is released under [the Unlicense](UNLICENSE).