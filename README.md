# Lemmings Paintball Decompilation

[![Build Status](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml/badge.svg)](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml)
[![Effective](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Feffective.json)](#effective-matching)
[![Exact](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Fexact.json)](https://decomp.dev/vonhoff/lemball-decomp)
[![Fuzzy](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Ffuzzy.json)](https://decomp.dev/vonhoff/lemball-decomp)

[<img src="https://decomp.dev/vonhoff/lemball-decomp.svg?w=512&h=256" width="512" height="256" alt="Decomp Progress Chart">](https://decomp.dev/vonhoff/lemball-decomp)

This project is a matching decompilation of *Lemmings Paintball* (1996, Windows 95).
The goal is to reconstruct the full game in readable C++, preserve its original
behavior, and reach 100% effective matching across all functions.

The code is compiled by Microsoft Visual C++ 4.00. Each function is compared with the original executable using [reccmp](https://github.com/isledecomp/reccmp).

## Progress Reporting

In the decomp.dev report, `matched_*` counts functions with a raw 100% assembly
comparison score. Equivalent functions with lower scores retain their raw
similarity under fuzzy progress.

[tools/report.py](tools/report.py) maps reccmp results to objdiff's `report.json`:

| Reccmp Data | Objdiff Field |
| --- | --- |
| Rebuilt PDB module name | `units[].name` |
| Original virtual address (`orig_addr`) | `functions[].name`, `functions[].metadata.virtual_address` |
| Comparison name or entity name | `functions[].metadata.demangled_name` |
| Original function size in bytes | `functions[].size` |
| Raw similarity (`accuracy * 100`) | `functions[].fuzzy_match_percent` |
| Total original function bytes | `measures.total_code` |
| Function and module counts | `measures.total_functions`, `measures.total_units` |
| Bytes and count of exact matches | `measures.matched_code`, `measures.matched_functions` |
| Raw similarity weighted by original size | `measures.fuzzy_match_percent` |
| Exact-match by bytes and count | `measures.matched_code_percent`, `measures.matched_functions_percent` |

## Effective Matching

Effective matching counts exact matches, reccmp equivalents, and matches covered by these rules:

| Rule | Reason |
| --- | --- |
| Linker thunks | Different jump stubs can reach the same matched function. |
| `CMP` versus `TEST` | Comparing against a known zero and testing the same register give the same branch result. |
| Overwritten vtable stores | An intermediate vtable pointer is overwritten before use. |

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
