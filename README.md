# Lemmings Paintball Decompilation

[![Build Status](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml/badge.svg)](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml)
[![Code Progress](https://decomp.dev/vonhoff/lemball-decomp.svg?mode=shield&measure=code&label=Code)](https://decomp.dev/vonhoff/lemball-decomp)

[<img src="https://decomp.dev/vonhoff/lemball-decomp.svg?w=512&h=256" width="512" height="256" alt="Decomp Progress Chart">](https://decomp.dev/vonhoff/lemball-decomp)

A matching decompilation of *Lemmings Paintball* (1996, Windows 95).

The project aims to recover readable, semantic C++ while matching the original executable as closely as possible. Reconstructed code is compiled with Microsoft Visual C++ 4.00 and compared function-by-function using [reccmp](https://github.com/isledecomp/reccmp).

The GPL-3.0 license applies only to code that was developed for this project and does not form part of the reconstructed game code. The reconstructed game code itself is provided without a license. See [Legal](#legal) for further details.

## Matching and progress

Use `python tools/match.py 0xADDRESS` to build and compare one function.
`python tools/report.py` writes the reccmp results to `build-msvc400/reccmp.json`
and converts them to `build-msvc400/report.json` (objdiff v2).

A function counts as matched when reccmp gives it `matching == 1.0` and it
isn't a stub. reccmp's [comparator](https://github.com/isledecomp/reccmp/blob/v0.1.7/reccmp/compare/functions.py)
compares assembly with normalized addresses and symbols. Functions with
equivalent register substitutions keep their raw score in fuzzy progress.

The report includes roadmap functions with an original address and a positive
size. It looks up each comparison by address and uses the
[objdiff schema](https://github.com/encounter/objdiff/blob/eed74b99c4e94dd154882259931201badc6fdbd1/objdiff-core/protos/report.proto).

| reccmp input | objdiff field |
| --- | --- |
| Original address | Function `name` as `0xADDRESS`; `metadata.virtual_address` as a decimal string |
| Comparison name, falling back to the roadmap name | `metadata.demangled_name` |
| Roadmap size: rebuilt size, falling back to original size | `size` as a decimal string |
| Raw `matching` * 100; zero for stubs or missing comparisons | `fuzzy_match_percent` |
| Module path with the CMake prefix and `.obj` removed | Unit `name`, or `Compiler-generated` if empty; `metadata.source_path` when the source file exists |

Only 100% scores contribute to exact progress. Fuzzy progress is the average
score weighted by function size. Unit and project totals sum the included
functions. Percentages with a zero denominator are 100%.

## Tool checks

Install `ruff` and `pylint` with pip. Both use `pyproject.toml`.

```sh
python -m ruff check tools
python -m pylint --recursive=y --reports=n tools
python tools/gate.py
```

## References

### Technical resources

- [The Cutting Room Floor — Lemmings Paintball](https://tcrf.net/Lemmings_Paintball)
- [Game Data Digs — Lemmings Paintball](https://gamedatadigs.neocities.org/lemmings_paintball)
- [Reverse Engineering a DOS Game with Ghidra and Codex](https://alexbevi.com/blog/2026/03/14/reverse-engineering-a-dos-game-with-ghidra-and-codex)

### Inspirations

- [openblack/bw1-decomp](https://github.com/openblack/bw1-decomp)
- [marijnvdwerf/legoland](https://github.com/marijnvdwerf/legoland)

## Legal

This is an unofficial reverse-engineering and preservation project. It is not affiliated with, authorized by, or endorsed by any rights holder associated with *Lemmings Paintball*.

*Lemmings Paintball*, its name, trademarks, and copyrighted material from the original game remain the property of their respective rights holders. No original executable or game assets are included in or distributed with this repository.

The reconstructed game code was produced through reverse engineering and analysis of the compiled program. No original or leaked source code was used. The reconstructed game code is provided without a license, and nothing in this repository grants any rights in third-party intellectual property.

Code independently developed for this project that is not part of the reconstructed game code is licensed under the [GNU General Public License v3.0](LICENSE).
