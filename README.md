# Lemmings Paintball Decompilation

[![Build Status](https://img.shields.io/github/actions/workflow/status/vonhoff/lemball-decomp/build.yml)](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml)
[![Effective Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Feffective.json)](#effective-matching)
[![Exact Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Fexact.json)](https://decomp.dev/vonhoff/lemball-decomp)
[![Fuzzy Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Ffuzzy.json)](https://decomp.dev/vonhoff/lemball-decomp)

[<img src="https://decomp.dev/vonhoff/lemball-decomp.svg?w=512&h=256" width="512" height="256" alt="Decomp Progress Chart">](https://decomp.dev/vonhoff/lemball-decomp)

This project is a matching decompilation of *Lemmings Paintball* (1996, Windows 95).
The goal is to reconstruct the full game in readable C++, preserve its original
behavior, and reach 100% effective matching across all functions. The code is 
compiled by Microsoft Visual C++ 4.00. 

Each function is compared with the original executable using [reccmp](https://github.com/isledecomp/reccmp).

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

The Effective badge counts functions with a raw 100% assembly comparison score
or an accepted equivalent instruction sequence. Each accepted function contributes
its full original byte size, once. Stubs and unmatched functions contribute zero.

Effective percentage = bytes in exact or accepted equivalent functions / total
original function bytes × 100.

Equivalence comes from reccmp's checks, such as register allocation differences,
plus the additional checks in [tools/lib/effective.py](tools/lib/effective.py):

| Difference | Example | Required checks |
| --- | --- | --- |
| Linker thunks (jump stubs) | A call reaches `Foo` through a linker jump stub instead of naming `Foo` directly. | Verify the jump target against the original/rebuilt function pairing. Resolve verified thunk references to that function identity. |
| `CMP` versus `TEST` | `cmp eax, ebp` versus `test eax, eax`, with `ebp` known to be zero. | Prove the zero value at that point in both functions; require the same following conditional branch. Reject functions that can observe the differing auxiliary flag. |
| Overwritten construction vtable stores | A temporary vtable pointer differs; both sequences then write the same final vtable pointer. | Require an overwrite of the same slot within the next three instructions, before any read, call, branch, or change to the base register. Intervening instructions must match and be limited to non-overlapping stores or address calculations. |

The whole function must match after these checks, including any remaining
differences accepted by reccmp. A high raw similarity alone earns no effective credit.

For example, an accepted equivalent 100-byte function with 80% raw similarity
contributes 100 bytes to Effective, 80 weighted bytes to Fuzzy, and zero bytes to
Exact. Its raw score remains 80% in decomp.dev. [tools/badges.py](tools/badges.py)
exports the Effective badge separately; `report.json` contains no effective fields.

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
