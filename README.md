# Lemmings Paintball Decompilation

[![Build Status](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml/badge.svg)](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml)
[![Code Progress](https://decomp.dev/vonhoff/lemball-decomp.svg?mode=shield&measure=code&label=Code)](https://decomp.dev/vonhoff/lemball-decomp)

[<img src="https://decomp.dev/vonhoff/lemball-decomp.svg?w=512&h=256" width="512" height="256" alt="Decomp Progress Chart">](https://decomp.dev/vonhoff/lemball-decomp)

This project is a matching decompilation of *Lemmings Paintball* (1996, Windows 95).

The goal is to recover readable C++ that expresses the game's behavior and matches the original executable as closely as possible. We compile the reconstructed code with Microsoft Visual C++ 4.00 and compare each function with [reccmp](https://github.com/isledecomp/reccmp).

GPL-3.0 covers only code independently developed for this project outside the reconstructed game code. The reconstructed game code is provided without a license. See [Legal](#legal) for details.

## Matching and progress

Use `python tools/match.py 0xADDRESS` to build and compare one function.
To generate a progress report, run `python tools/report.py`. It writes reccmp
results to `build-msvc400/reccmp.json` and converts them to
`build-msvc400/report.json` (objdiff v2).

A function counts as matched if it isn't a stub and reccmp reports
`matching == 1.0`. The reccmp [comparator](https://github.com/isledecomp/reccmp/blob/v0.1.7/reccmp/compare/functions.py)
normalizes addresses and symbols when comparing assembly. Functions with
equivalent register substitutions retain their raw score for fuzzy progress.

The report includes each roadmap function that has an original address and a
positive size. Comparisons are looked up by address, and the results use the
[objdiff schema](https://github.com/encounter/objdiff/blob/eed74b99c4e94dd154882259931201badc6fdbd1/objdiff-core/protos/report.proto).

| reccmp input | objdiff field |
| --- | --- |
| Original address | Function `name` as `0xADDRESS`; `metadata.virtual_address` as a decimal string |
| Comparison name, falling back to the roadmap name | `metadata.demangled_name` |
| Roadmap size: rebuilt size, falling back to original size | `size` as a decimal string |
| Raw `matching` * 100; zero for stubs or missing comparisons | `fuzzy_match_percent` |
| Module path with the CMake prefix and `.obj` removed | Unit `name`, or `Compiler-generated` if empty; `metadata.source_path` when the source file exists |

Exact progress counts only functions with a 100% score. Fuzzy progress averages
the scores, weighted by function size. Unit and project totals sum the functions
included in the report. A percentage with a zero denominator is reported as 100%.

## Tool checks

Install `ruff` and `pylint` with pip, then run the checks below. Both tools read
their configuration from `pyproject.toml`.

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

This unofficial project reverse-engineers *Lemmings Paintball* for preservation. The game's rights holders have not authorized or endorsed the project, and it is not affiliated with them.

The game, its name, trademarks, and original copyrighted material remain the property of their respective rights holders. This repository does not include or distribute the original executable or game assets.

The reconstructed game code comes from reverse engineering and analysis of the compiled program. No original or leaked source code was used. This code is provided without a license. Nothing in this repository grants rights in third-party intellectual property.

Code independently developed for this project, outside the reconstructed game code, is licensed under the [GNU General Public License v3.0](LICENSE).
