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

Effective counts non-stub raw 100% matches, reccmp equivalents, and additional
matches accepted by [matches.py](tools/lib/comparison/matches.py). The badge weights
accepted functions by original code size. Stubs and unmatched functions contribute zero.

Raw 100% and reccmp effective matches count directly. For remaining functions,
the adapter resolves direct calls or jumps through one `E9` thunk to a paired
function, then reruns reccmp's comparison. A thunk changes the route to the callee;
the final paired target supplies the comparison name. Address-taking instructions
keep their original treatment.

If thunk normalization is insufficient, [normalize.py](tools/lib/comparison/normalize.py)
applies the following rules. Acceptance requires equality of the complete normalized
instruction/table sequences; a matching fragment is insufficient.

- **Placeholder numbering.** Reccmp's `<OFFSETn>` numbering includes resolved
  symbols, so asymmetric symbol resolution can shift later numbers. Renumber
  unknown addresses by first occurrence. Preserve repeated references, distinct
  identities, and resolved symbol/string text. Collapsing every unknown address
  to one token would hide changed aliasing.
- **Branch destinations.** Different instruction encodings can change byte
  displacements without changing the destination instruction. Replace internal
  branch and jump-table destinations with instruction-position labels. Preserve
  branch conditions, table order, and target positions. Unresolved direct branch
  destinations and jumps into instruction interiors reject this additional pass.
- **Zero comparisons.** `cmp reg, 0` and `test reg, reg` agree on ZF, SF, PF, CF,
  and OF. The same applies to `cmp reg, zero_reg` when local register tracking
  establishes zero. Writes, including partial-register writes, invalidate the
  fact; control-flow boundaries clear it. Calls invalidate volatile-register
  facts under the Windows x86 ABI. AF differs, so functions reading AF are excluded
  from this rule.
- **Instruction scheduling.** Independent instructions can execute in different
  orders under MSVC's scheduler. Put supported instructions into a deterministic
  dependency order. Preserve register read/write dependencies, implicit operands,
  partial-register overlap, and flag dependencies. Calls, stores, stack operations,
  unsupported instructions, and block entries stop reordering. This avoids relying
  on the upstream relocation scan's register-write-only hazard check.

The additional pass rejects incomplete decoding except `INT3` padding, unresolved
indirect jumps, local subroutine calls, interrupts, and privileged instructions.
Its scheduling rule assumes ordinary memory loads; it does not establish volatile
or MMIO semantics or exception ordering. Placeholder correspondence follows
reccmp's address abstraction and does not prove referenced data contents.
These rules are bounded comparison heuristics, not a whole-program equivalence proof.
Stack-frame shifts, combined register-allocation/scheduling changes, and broader
arithmetic rewrites remain unsupported by this additional pass.

Raw scores and diffs remain unchanged. Effective addresses are saved separately
in `build-msvc400/effective.json`, bound to the canonical `report.json` hash and
matching-policy version. [make_report.py](tools/make_report.py) regenerates both;
[check_function.py](tools/check_function.py) displays raw and Effective scores.
Acceptance and rejection cases live in
[test_normalize.py](tools/tests/comparison/test_normalize.py).

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
