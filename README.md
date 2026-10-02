# Lemmings Paintball Decompilation

[![Build Status](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml/badge.svg)](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml)
[![Exact Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Fexact.json)](#matching-and-progress)
[![Fuzzy Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Ffuzzy.json)](#matching-and-progress)
[![Effective Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Feffective.json)](#matching-and-progress)

[<img src="https://decomp.dev/vonhoff/lemball-decomp.svg?w=512&h=256" width="512" height="256" alt="Decomp Progress Chart">](https://decomp.dev/vonhoff/lemball-decomp)

This project is a matching decompilation of *Lemmings Paintball* (1996, Windows 95).

Microsoft Visual C++ 4.00 compiles the C++ code. [Reccmp](https://github.com/isledecomp/reccmp) compares each function with the original executable.

## Matching and progress

- **Exact Match** shows the percentage of code contained in functions that reach
  a raw 100% [normalized assembly](https://github.com/isledecomp/reccmp/blob/v0.1.7/reccmp/compare/functions.py)
  score. Stub functions do not count as exact matches.
- **Fuzzy Match** shows the average raw assembly similarity reported by reccmp,
  weighted by function size in bytes. Larger functions contribute more to this
  percentage. Stubs and functions without a comparison contribute zero. Functions
  that reccmp accepts as equivalent retain their raw scores, even when those
  scores are below 100%.
- **Effective Match** shows the percentage of code contained in exact matches or
  functions that reccmp accepts as equivalent, including some differences in
  register use. This badge is reported separately; equivalent functions do not
  increase the assembly-exact function count or exact matched code bytes.

For the available commands and validation checks, see [tools/USAGE.md](tools/USAGE.md).

## References

### Technical resources

- [The Cutting Room Floor — Lemmings Paintball](https://tcrf.net/Lemmings_Paintball)
- [Game Data Digs — Lemmings Paintball](https://gamedatadigs.neocities.org/lemmings_paintball)
- [Reverse Engineering a DOS Game with Ghidra and Codex](https://alexbevi.com/blog/2026/03/14/reverse-engineering-a-dos-game-with-ghidra-and-codex)

### Inspirations

- [openblack/bw1-decomp](https://github.com/openblack/bw1-decomp)
- [marijnvdwerf/legoland](https://github.com/marijnvdwerf/legoland)

## Legal

This unofficial project reverse-engineers *Lemmings Paintball* for preservation.
The original game and assets remain the property of their rights holders and are
not distributed with this repository.

The reconstructed game code has no license. Code independently developed for this
project is licensed under the [GNU General Public License v3.0](LICENSE).
