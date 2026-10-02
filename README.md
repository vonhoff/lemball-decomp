# Lemmings Paintball Decompilation

[![Build Status](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml/badge.svg)](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml)
[![Exact Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Fexact.json)](#matching-and-progress)
[![Fuzzy Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Ffuzzy.json)](#matching-and-progress)
[![Effective Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Feffective.json)](#matching-and-progress)

[<img src="https://decomp.dev/vonhoff/lemball-decomp.svg?w=512&h=256" width="512" height="256" alt="Decomp Progress Chart">](https://decomp.dev/vonhoff/lemball-decomp)

This project is a matching decompilation of *Lemmings Paintball* (1996, Windows 95).

The reconstructed game is in a playable state. Reconstruction and matching work are ongoing.

The reconstructed C++ is compiled with Microsoft Visual C++ 4.00. [Reccmp](https://github.com/isledecomp/reccmp) compares each function with the original executable.

Only code independently developed for this project outside the reconstructed game code is covered by GPL-3.0. The reconstructed game code is provided without a license. Further details are given under [Legal](#legal).

## Matching and progress

Build and compare one function with `python tools/match.py 0xADDRESS`.

**Exact Match** is the percentage of reported code in non-stub functions with a raw
100% assembly score. Reccmp's
[comparator](https://github.com/isledecomp/reccmp/blob/v0.1.7/reccmp/compare/functions.py)
normalizes addresses and symbols before comparison, so exact assembly matches do
not imply a byte-identical executable.

**Fuzzy Match** is the average raw score, weighted by function size. Equivalent
register substitutions retain their raw score below 100%; stubs and missing
comparisons contribute zero.

**Effective Match** is the percentage of code in exact or reccmp-equivalent functions,
including recognized register substitutions. It includes **Exact Match**. This supplementary
badge does not increase exact progress or prove gameplay correctness; decomp.dev
continues to track the raw exact and fuzzy measures.

All percentages use the same function inventory and code sizes.

Generate the current progress report with `python tools/report.py`. README badges
update after successful builds on `main`.

Use `python tools/next.py --kind near` to rank unfinished functions, or `--kind gain`
to rank by size times raw score. Build options and tool checks:
[tools/USAGE.md](tools/USAGE.md).

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
