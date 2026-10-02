# Lemmings Paintball Decompilation

[![Build Status](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml/badge.svg)](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml)
[![Exact](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Fexact.json)](#matching-and-progress)
[![Fuzzy](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Ffuzzy.json)](#matching-and-progress)
[![Effective](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Feffective.json)](#matching-and-progress)

[<img src="https://decomp.dev/vonhoff/lemball-decomp.svg?w=512&h=256" width="512" height="256" alt="Decomp Progress Chart">](https://decomp.dev/vonhoff/lemball-decomp)

This project is a matching decompilation of *Lemmings Paintball* (1996, Windows 95).

Microsoft Visual C++ 4.00 compiles the C++ code. [Reccmp](https://github.com/isledecomp/reccmp) compares each function with the original executable.

## Matching and progress

The badges measure code bytes in the reported original functions:

| Badge | What counts |
| --- | --- |
| Exact Match | Functions with a raw 100% [normalized assembly](https://github.com/isledecomp/reccmp/blob/v0.1.7/reccmp/compare/functions.py) score, excluding stubs. |
| Fuzzy Match | Raw assembly similarity, weighted by original function size. Stubs and functions without a matched comparison score zero. |
| Effective Match | Exact matches plus functions reccmp accepts as equivalent, including some differences in register use. Stubs score zero. |

Fuzzy Match is `sum(score * original size) / sum(original size)`.

### Reccmp to objdiff reports

| Objdiff field | Mapping |
| --- | --- |
| `units[].name` | Rebuilt PDB module name, or `Compiler-generated` when no module is available. |
| `functions[].name` | Original virtual address in hexadecimal. |
| `functions[].size` | Original function size in bytes, stored as a decimal string. |
| `functions[].metadata.virtual_address` | Original virtual address, stored as a decimal string. |
| `functions[].metadata.demangled_name` | Name from the matched comparison, or the entity name when unmatched. |
| `functions[].fuzzy_match_percent` | Reccmp `accuracy * 100` for matched functions. Stubs and functions without a matched comparison score zero. |
| `measures.total_code` | Sum of original function sizes. |
| `measures.total_functions` / `total_units` | Number of reported functions and PDB module groups. |
| `measures.matched_code` / `matched_functions` | Bytes and count of functions whose exported score is exactly 100%. |
| `measures.fuzzy_match_percent` | Average exported score, weighted by original bytes. |
| `measures.matched_code_percent` / `matched_functions_percent` | Exact matched bytes or functions divided by the corresponding total, times 100. |

## References

### Technical resources

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
