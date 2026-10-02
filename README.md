# Lemmings Paintball Decompilation

[![Build Status](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml/badge.svg)](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml)
[![Code Progress](https://decomp.dev/vonhoff/lemball-decomp.svg?mode=shield&measure=code&label=Code)](https://decomp.dev/vonhoff/lemball-decomp)

[<img src="https://decomp.dev/vonhoff/lemball-decomp.svg?w=512&h=256" width="512" height="256" alt="Decomp Progress Chart">](https://decomp.dev/vonhoff/lemball-decomp)

This project is a matching decompilation of *Lemmings Paintball* (1996, Windows 95).

The game's behavior is being reconstructed in readable C++ to match the original executable as closely as possible. The reconstructed code is compiled with Microsoft Visual C++ 4.00, and each function is compared with [reccmp](https://github.com/isledecomp/reccmp).

Only code independently developed for this project outside the reconstructed game code is covered by GPL-3.0. The reconstructed game code is provided without a license. Further details are given under [Legal](#legal).

## Matching and progress

A single function can be built and compared with `python tools/match.py 0xADDRESS`.
A progress report is generated with `python tools/report.py`. The reccmp results
are written to `build-msvc400/reccmp.json` and converted to
`build-msvc400/report.json` (objdiff v2).

A function is counted as matched if it isn't a stub and is given
`matching == 1.0` by reccmp. Addresses and symbols are normalized by reccmp's
[comparator](https://github.com/isledecomp/reccmp/blob/v0.1.7/reccmp/compare/functions.py)
when assembly is compared. Raw scores are retained for functions with equivalent
register substitutions when fuzzy progress is calculated.

Each roadmap function with an original address and a positive size is included
in the report. Comparisons are looked up by address, and the results are stored
using the
[objdiff schema](https://github.com/encounter/objdiff/blob/eed74b99c4e94dd154882259931201badc6fdbd1/objdiff-core/protos/report.proto).

| reccmp input | objdiff field |
| --- | --- |
| Original address | Function `name` as `0xADDRESS`; `metadata.virtual_address` as a decimal string |
| Comparison name, falling back to the roadmap name | `metadata.demangled_name` |
| Roadmap size: rebuilt size, falling back to original size | `size` as a decimal string |
| Raw `matching` * 100; zero for stubs or missing comparisons | `fuzzy_match_percent` |
| Module path with the CMake prefix and `.obj` removed | Unit `name`, or `Compiler-generated` if empty; `metadata.source_path` when the source file exists |

Only functions with a 100% score are counted toward exact progress. Fuzzy progress
is calculated as the average score weighted by function size. Unit and project
totals are calculated by summing the functions included in the report. A
percentage with a zero denominator is reported as 100%.

## Tool checks

`ruff` and `pylint` can be installed with pip. Configuration for both tools is read
from `pyproject.toml`. The checks are run with:

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

*Lemmings Paintball* is being reverse-engineered for preservation in this unofficial project. The project is not affiliated with, authorized by, or endorsed by the game's rights holders.

The game, its name, trademarks, and original copyrighted material remain the property of their respective rights holders. Neither the original executable nor game assets are included in or distributed with this repository.

The reconstructed game code was produced through reverse engineering and analysis of the compiled program. No original or leaked source code was used. This code is provided without a license. No rights in third-party intellectual property are granted by this repository.

Code independently developed for this project, outside the reconstructed game code, is licensed under the [GNU General Public License v3.0](LICENSE).
