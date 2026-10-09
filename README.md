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
> This repository is dedicated to reconstructing the original game. For a modern version with support for other
platforms and the Web, see [lemball-portable](https://github.com/vonhoff/lemball-portable).

## Building

The reconstructed code is compiled using Microsoft Visual C++ 4.00 and targets 32-bit Windows.

### Prerequisites

1. **Python 3.10+** with required dependencies:
   ```bash
   pip install -r requirements.txt
   ```
2. **MSVC 4.00 toolchain**:
   Clone the compiler into `msvc400/` or point `MSVC400_ROOT` to its directory:
   ```bash
   git clone https://github.com/vonhoff/MSVC400 msvc400
   ```

### Build

```bash
python tools/make_binary.py
```

Useful flags:

- `--clean-first`: Perform a full clean rebuild.
- `--disable-startup-checks`: Bypass startup CD-ROM and installation checks.

## Matching

Each function is compared against the original executable using [reccmp](https://github.com/isledecomp/reccmp).

- **Exact:** Non-stub functions with a raw 100% assembly comparison score.
- **Fuzzy:** Raw assembly similarity, including equivalent functions with lower scores.
- **Effective:** Non-stub raw 100% matches and reccmp's own equivalence results.

The [decomp.dev report](https://decomp.dev/vonhoff/lemball-decomp) only includes exact and fuzzy matches.

## References

### Technical Resources

- [The Cutting Room Floor — Lemmings Paintball](https://tcrf.net/Lemmings_Paintball)
- [Game Data Digs — Lemmings Paintball](https://gamedatadigs.neocities.org/lemmings_paintball)
- [Reverse Engineering a DOS Game with Ghidra and Codex](https://alexbevi.com/blog/2026/03/14/reverse-engineering-a-dos-game-with-ghidra-and-codex)

### Inspirations

- [isledecomp/isle](https://github.com/isledecomp/isle)
- [marijnvdwerf/legoland](https://github.com/marijnvdwerf/legoland)

## Legal

This is an unofficial reverse-engineering project intended to preserve *Lemmings Paintball*. It is not affiliated with
or endorsed by the original rights holders.

The original game and its assets remain the property of their respective rights holders. The original game assets are
not included in this repository.

The reconstructed game code is not offered under a license. The independently developed code is licensed under
the [GNU General Public License v3.0](LICENSE).
