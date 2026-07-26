# Lemmings Paintball Decompilation

This repository contains a work-in-progress reconstruction of the original 1996 Windows C++ codebase behind `LEMBALL.EXE`.

The game is not yet functional and remains under active development. The primary goal is to produce a complete decompilation of *Lemmings Paintball* that matches the original machine code as closely as reasonably possible. Once complete, the reconstructed codebase may also serve as a useful technical reference for future ports, reimplementations, and preservation work.

> [!WARNING]
> This is a decompilation project. The reconstructed source code is intentionally kept close to the behavior and structure of the original release, including its platform-specific assumptions. The game targets 32-bit Windows and is not expected to build or run on other platforms without substantial additional work.

## Development

Install [MSVC 4.20](https://github.com/itsmattkc/msvc420) in `msvc420`, create a Python virtual environment, then install `requirements-dev.txt`. Place the original `LEMBALL.EXE` in `data`, build, then register it:

```bat
call msvc420\bin\VCVARS32.BAT x86
cmake --preset msvc420
cmake --build --preset msvc420
reccmp-project detect --search-path data
```

Run comparison tools from the build directory:

```bat
cd build-msvc420
reccmp-decomplint --target LEMBALL
reccmp-reccmp --target LEMBALL --json reccmp.json --json-diet
reccmp-datacmp --target LEMBALL
reccmp-roadmap --target LEMBALL --csv roadmap.csv
cd ..
python tools\generate_objdiff_report.py
```

The generated `report.json` is an [objdiff report v2 accepted by decomp.dev](https://decomp.wiki/tools/decomp-dev). `data/objdiff-functions.csv` preserves full original-function denominator and current reconstructed-unit assignment; `reccmp.json` supplies match percentages.

Roadmap CSV records original address, rebuilt address, displacement, symbol, and current object module. Use it for coverage and address-cluster review, not original filename provenance; `reccmp-roadmap --target LEMBALL --order fun` suggests current module order. `reccmp-vtable` and `reccmp-stackcmp` remain available for focused investigation. `python tools\lint_reccmp_recommendations.py` checks [reccmp's optional recommendations](https://github.com/isledecomp/reccmp/blob/master/docs/recommendations.md): size assertions, member offsets in sized types, and virtual method offsets in annotated vtables.

Run `tools/ExportObjdiffFunctions.java` in Ghidra to regenerate `data/objdiff-functions.csv`; tagged Ghidra functions are the source of truth.
Run `tools/AuditObjdiffOwnership.java` to flag unit candidates supported by referenced `.CPP` strings or a strong vtable-unit majority. Results are suggestions: vtables prove class grouping, not original filenames.

Source organization follows recovered translation units where binary evidence exists. Other `.CPP` files are reconstruction units, not claims about original filenames; preserve link order and prefer debug strings, assertions, and matching evidence over one-class-per-file style.

## Development Status

This project is still highly experimental and is currently developed largely with the assistance of AI tools. Expect frequent changes, incomplete implementations, rough edges, and code that still requires manual review and verification.

The issue tracker and pull requests are temporarily disabled while the project is still in this early stage. I plan to reopen them once the codebase is more stable and I have a better established workflow for handling contributions, bug reports, and technical discussions.

## Legal

This repository contains an encrypted, non-working copy of the original *Lemmings Paintball* executable, which is used solely to support automated binary comparison and verification during development. It is not intended to provide a playable version of the game or replace the original software. No game data, artwork, audio, or other proprietary assets required for normal gameplay are included. Users are responsible for supplying any required game files from a copy they are legally entitled to use.

All copyrights, trademarks, and other intellectual property rights relating to *Lemmings Paintball* and the original software remain the property of their respective rights holders. This is an unofficial preservation and research project. It is not affiliated with, authorized by, sponsored by, or endorsed by any current or former rights holder.
