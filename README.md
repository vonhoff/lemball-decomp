# Lemmings Paintball Decompilation

[![Build Status](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml/badge.svg)](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml)
[![Exact Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Fexact.json)](#matching-and-progress)
[![Fuzzy Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Ffuzzy.json)](#matching-and-progress)
[![Effective Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Feffective.json)](#matching-and-progress)

[<img src="https://decomp.dev/vonhoff/lemball-decomp.svg?w=512&h=256" width="512" height="256" alt="Decomp Progress Chart">](https://decomp.dev/vonhoff/lemball-decomp)

This project is a matching decompilation of *Lemmings Paintball* (1996, Windows 95).

Microsoft Visual C++ 4.00 compiles the C++ code. [Reccmp](https://github.com/isledecomp/reccmp) compares each function with the original executable.

Only code written independently for this project, outside the reconstructed game code, is covered by GPL-3.0. The reconstructed game code has no license. See [Legal](#legal).

## Matching and progress

Build and compare one function with `python tools/match.py 0xADDRESS`.

**Exact Match**: share of code in functions with a raw 100%
[normalized assembly](https://github.com/isledecomp/reccmp/blob/v0.1.7/reccmp/compare/functions.py) score, excluding stubs.

**Fuzzy Match**: average raw score, with larger functions counting more. Functions
with equivalent register changes keep their score below 100%; stubs and functions
without a comparison score zero.

**Effective Match**: share of code in exact matches or functions reccmp accepts as
equivalent, including register changes. Shown separately from exact and fuzzy progress.

Create the current progress report with `python tools/report.py`. README badges
update after successful builds on `main`.

Commands and tool checks: [tools/USAGE.md](tools/USAGE.md).

## References

### Technical resources

- [The Cutting Room Floor — Lemmings Paintball](https://tcrf.net/Lemmings_Paintball)
- [Game Data Digs — Lemmings Paintball](https://gamedatadigs.neocities.org/lemmings_paintball)
- [Reverse Engineering a DOS Game with Ghidra and Codex](https://alexbevi.com/blog/2026/03/14/reverse-engineering-a-dos-game-with-ghidra-and-codex)

### Inspirations

- [openblack/bw1-decomp](https://github.com/openblack/bw1-decomp)
- [marijnvdwerf/legoland](https://github.com/marijnvdwerf/legoland)

## Legal

*Lemmings Paintball* is being reverse-engineered for preservation in this unofficial project. The project is not affiliated with, authorized by, or endorsed by the game's rights holders.

The game, its name, trademarks, and original copyrighted material remain the property of their respective rights holders. Neither the original executable nor game assets are included in or distributed with this repository.

The reconstructed game code was produced through reverse engineering and analysis of the compiled program. No original or leaked source code was used. This code is provided without a license. No rights in third-party intellectual property are granted by this repository.

Code independently developed for this project, outside the reconstructed game code, is licensed under the [GNU General Public License v3.0](LICENSE).
