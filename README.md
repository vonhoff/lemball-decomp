# Lemmings Paintball Decompilation

[![Build Status](https://img.shields.io/github/actions/workflow/status/vonhoff/lemball-decomp/build.yml)](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml)
[![Effective Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Feffective.json)](#effective-matching)
[![Exact Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Fexact.json)](https://decomp.dev/vonhoff/lemball-decomp)
[![Fuzzy Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Ffuzzy.json)](https://decomp.dev/vonhoff/lemball-decomp)

This project is a matching decompilation of *Lemmings Paintball* (1996, Windows 95).
The goal is to reconstruct the full game in readable C++, preserve its original
behavior, and reach 100% effective matching across all functions. The code is
compiled by Microsoft Visual C++ 4.00.

Each function is compared with the original executable using [reccmp](https://github.com/isledecomp/reccmp).

## Progress Reporting

In the decomp.dev report, `matched_*` counts non-stub functions with a raw 100%
reccmp assembly comparison score. Equivalent functions with lower scores retain
their raw similarity under fuzzy progress. Stubs and unmatched functions score zero.

The inventory and byte totals come from the original LEMBALL function-size catalog,
including original code with no rebuilt counterpart.

## Effective Matching

Effective counts non-stub raw 100% matches, reccmp equivalents, and these additional rules:

| Rule | Accepted difference | Guard |
| --- | --- | --- |
| Jump thunks | Direct call/jump through one `E9` thunk | Same paired function target |
| Placeholders | Shifted `<OFFSETn>` numbering | Preserve repeated references and distinct identities |
| Zero checks | `cmp reg, 0` or proven-zero register versus `test reg, reg` | Track register writes; reject AF readers |
| Scheduling | Reordered independent instructions | Preserve register/flag dependencies and block boundaries; calls and stores stop reordering |

Additional normalization requires complete sequence equality. Raw scores stay unchanged;
Effective results live separately in `build-msvc400/effective.json`. The badge weights
accepted functions by original code size; stubs and unmatched functions contribute zero.

Implementation and limits: [matches.py](tools/lib/comparison/matches.py),
[normalize.py](tools/lib/comparison/normalize.py). These are comparison heuristics,
not a whole-program equivalence proof.

## References

### Technical Resources

- [The Cutting Room Floor — Lemmings Paintball](https://tcrf.net/Lemmings_Paintball)
- [Game Data Digs — Lemmings Paintball](https://gamedatadigs.neocities.org/lemmings_paintball)
- [Reverse Engineering a DOS Game with Ghidra and Codex](https://alexbevi.com/blog/2026/03/14/reverse-engineering-a-dos-game-with-ghidra-and-codex)

### Inspirations

- [openblack/bw1-decomp](https://github.com/openblack/bw1-decomp)
- [marijnvdwerf/legoland](https://github.com/marijnvdwerf/legoland)

## Legal

This is an unofficial reverse-engineering project for the preservation of *Lemmings Paintball*.
The original game and its assets remain the property of their respective rights holders and
are not distributed with this repository.

The reconstructed game code has no license. Code independently developed for this
project is licensed under the [GNU General Public License v3.0](LICENSE).
