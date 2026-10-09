# Lemmings Paintball Decompilation

[![Build Status](https://img.shields.io/github/actions/workflow/status/vonhoff/lemball-decomp/build.yml)](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml)
[![Effective Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Feffective.json)](#matching)
[![Exact Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Fexact.json)](https://decomp.dev/vonhoff/lemball-decomp)
[![Fuzzy Match](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fvonhoff%2Flemball-decomp%2Fbadges%2Ffuzzy.json)](https://decomp.dev/vonhoff/lemball-decomp)

This project is a matching decompilation of *Lemmings Paintball*, an action video game from the *Lemmings* franchise
developed by Visual Sciences and published by Psygnosis in 1996 for Windows 95.

The goal is to reconstruct the game's codebase using semantic, maintainable C++ that matches the original machine code
as closely as possible. The resulting codebase will serve as a faithful reference and foundation for ports.

> [!NOTE]
> The reconstructed code only targets 32-bit Windows. For a modern version with support for other platforms,
> see [lemball-portable](https://github.com/vonhoff/lemball-portable).

## Reconstruction

The original function and class names were recovered from the Macintosh 68000 version, which preserved debugging symbols
from Metrowerks and MacsBug. These provided over 2,800 original function names and revealed C++ class hierarchies
throughout the game and engine.

Since the Windows executable had its symbols removed, functions were matched between the two versions using strings,
constants, algorithms, virtual tables, object layouts, and call relationships.

## Building

Requires Python 3.10+ and the MSVC 4.00 toolchain:

```bash
pip install -r requirements.txt
git clone https://github.com/vonhoff/MSVC400 msvc400
python tools/make_binary.py
```

Pass `--disable-startup-checks` to `make_binary.py` to bypass the CD-ROM and installation checks.

## Matching

Each function is compared against the original executable using [reccmp](https://github.com/isledecomp/reccmp).

- **Exact:** Non-stub functions with a raw 100% assembly comparison score.
- **Fuzzy:** Raw assembly similarity, including equivalent functions with lower scores.
- **Effective:** Non-stub raw 100% matches and reccmp's own equivalence results.

The [decomp.dev report](https://decomp.dev/vonhoff/lemball-decomp) only includes exact and fuzzy matches.

### Prerequisites

Matching requires the built executable in `build-msvc400` and the original `LEMBALL.EXE` placed in `data`.

SHA-256: `d6337b58ccaf98df728b1490812cad0f927802d2e2c5fc932d00961f97027f63`

### Workflow

1. Find target: `python tools/triage_targets.py`
2. Rebuild and diff: `python tools/make_binary.py && python tools/check_function.py 0xADDR`
3. Check source: `python tools/check_source.py`
4. Update report: `python tools/make_report.py` (writes to `build-msvc400/report.json`)

## AI Disclosure

This project uses AI for tooling, research, and code reconstruction. All code is linted, formatted, and manually
reviewed for correctness and maintainability.

## References

### Technical Resources

- [The Cutting Room Floor — Lemmings Paintball](https://tcrf.net/Lemmings_Paintball)
- [Game Data Digs — Lemmings Paintball](https://gamedatadigs.neocities.org/lemmings_paintball)
- [Reverse Engineering a DOS Game with Ghidra and Codex](https://alexbevi.com/blog/2026/03/14/reverse-engineering-a-dos-game-with-ghidra-and-codex)

### Inspirations

- [isledecomp/isle](https://github.com/isledecomp/isle)
- [marijnvdwerf/legoland](https://github.com/marijnvdwerf/legoland)

## Legal

This is an unofficial reverse-engineering project not affiliated with or endorsed by the original rights holders.
Original game assets remain the property of their respective owners and are not included in this repository.

The reconstructed game code is not offered under a license. Independently developed code is licensed under
the [GNU General Public License v3.0](LICENSE).
