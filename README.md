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

To build and compare a reconstructed function with the original executable, run
`python tools/match.py 0xADDRESS`. The output shows its raw assembly similarity
score and any differences found by reccmp.

The three progress badges measure different aspects of the reconstruction:

- **Exact Match** shows the percentage of code contained in functions that reach
  a raw 100% [normalized assembly](https://github.com/isledecomp/reccmp/blob/v0.1.7/reccmp/compare/functions.py)
  score. Stub functions do not count as exact matches. Normalization allows the
  comparison to account for differences such as relocated addresses, so an exact
  match does not mean the executables are identical byte for byte.
- **Fuzzy Match** shows the average raw similarity score, weighted by function
  size. Larger functions contribute more to this percentage. Stubs and functions
  without a comparison contribute zero. Functions that reccmp accepts as
  equivalent retain their raw scores, even when those scores are below 100%.
- **Effective Match** shows the percentage of code contained in exact matches or
  functions that reccmp accepts as equivalent, including some differences in
  register use. This badge is reported separately; equivalent functions do not
  increase the assembly-exact function count or exact matched code bytes.

Run `python tools/report.py` to generate the current comparison and progress
reports in `build-msvc400`. The canonical `report.json` records the exact function
count, exact matched code bytes, and raw fuzzy similarity. The Effective Match
badge is calculated separately from the comparison results. README badges update
when the build workflow completes successfully on `main`.

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
