# Lemmings Paintball Decompilation

[![Build Status](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml/badge.svg)](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml)
[![Effective](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Feffective.json)](#matching-and-progress)
[![Exact](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Fexact.json)](https://decomp.dev/vonhoff/lemball-decomp)
[![Fuzzy](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Ffuzzy.json)](https://decomp.dev/vonhoff/lemball-decomp)

[<img src="https://decomp.dev/vonhoff/lemball-decomp.svg?w=512&h=256" width="512" height="256" alt="Decomp Progress Chart">](https://decomp.dev/vonhoff/lemball-decomp)

This project is a matching decompilation of *Lemmings Paintball* (1996, Windows 95).
The goal is to reconstruct the full game in readable C++, preserve its original
behavior, and reach 100% effective matching across all functions.

Microsoft Visual C++ 4.00 compiles the C++ code. [Reccmp](https://github.com/isledecomp/reccmp) compares each function with the original executable.

## Matching and progress

In the decomp.dev report, `matched_*` counts functions with a raw 100% assembly
comparison score. Equivalent functions with lower scores retain their raw
similarity under fuzzy progress.

The Effective metric includes exact matches, reccmp's register and instruction-order
equivalents, and the checks below. Code percentages are weighted by original
function size.

| Additional rule | Why it counts | Required evidence |
| --- | --- | --- |
| Linker thunk targets | Calls and function pointers can reach the same function through different linker jump stubs. | An `E9` jump to a paired function. Incremental linker tables require closing padding and forward code targets, indirect pointers require relocation entries. |
| Known-zero `CMP` versus `TEST` | Comparing a register against zero and testing that register produce the same branch decision. | A proved zero on both sides, the same following branch, and no instructions that observe the differing auxiliary flag. Writes and control-flow joins invalidate zero facts. |
| Overwritten construction vtable stores | An intermediate vtable address has no effect when overwritten before use. | The same destination, overwritten within the next three instructions before any read, call, or branch. |

[tools/lib/effective.py](tools/lib/effective.py) records additional matches and their
reasons in `build-msvc400/effective.json`. These checks affect the Effective metric,
with raw comparison scores and exact totals unchanged.

`tools/next.py` skips effective matches and accepts `--exact` to rank by raw scores.
`tools/match.py` displays raw and Effective scores with the assembly diff.

[tools/report.py](tools/report.py) maps reccmp results to objdiff's `report.json`:

| Reccmp data | Objdiff field |
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
