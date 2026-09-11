# Lemmings Paintball Decompilation

[![Build Status](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml/badge.svg)](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml)
[![Code Progress](https://decomp.dev/vonhoff/lemball-decomp.svg?mode=shield&measure=code&label=Code)](https://decomp.dev/vonhoff/lemball-decomp)

[<img src="https://decomp.dev/vonhoff/lemball-decomp.svg?w=512&h=256" width="512" height="256" alt="Decomp Progress Chart">](https://decomp.dev/vonhoff/lemball-decomp)

A matching decompilation of *Lemmings Paintball* (1996, Windows 95).

The goal is a byte-accurate, semantic C++ reconstruction of the original game. Reconstructed code is compiled with Microsoft Visual C++ 4.00 and verified function-by-function against the original executable with [reccmp](https://github.com/isledecomp/reccmp).

Live progress is tracked on [decomp.dev](https://decomp.dev/vonhoff/lemball-decomp).

## Target Binary

| Property | Value |
| :--- | :--- |
| Executable | `LEMBALL.EXE` |
| Platform | Windows 95 (x86, 32-bit PE/COFF) |
| Toolchain | Microsoft Visual C++ 4.00 |
| Flags | `/O2 /Ob1 /Oy /G4 /Z7 /MT` |
| SHA-256 | `d6337b58ccaf98df728b1490812cad0f927802d2e2c5fc932d00961f97027f63` |

Game assets and the original executable are not included. A copy of `LEMBALL.EXE` is required to build and verify.

## How It Works

Functions are matched without inline assembly:

1. Classes and functions are written in standard C++.
2. Code is compiled with MSVC 4.00 to produce `build-msvc400/LEMBALL.EXE` and debug symbols.
3. Compiled functions are compared byte-for-byte against the original binary using `reccmp`.
4. Types, layouts, and logic are adjusted until the compiler output matches.

### Macintosh Symbol Recovery

The 1996 Macintosh version retained unstripped MacsBug symbol tables. A total of 2,848 original function names, classes, and types were recovered from the 68K binary.

These symbols are mapped to the Windows x86 executable by comparing code structure, strings, and tables.

## AI Assistance

This project is developed with assistance from AI agents. Every change is verified against:
- Clean compilation with MSVC 4.00
- Binary diffs and assembly matches
- Recovered Mac 68K symbol evidence
- Automated test gates (`tools/gate.py`)

## References

### Research
- https://tcrf.net/Lemmings_Paintball
- https://gamedatadigs.neocities.org/lemmings_paintball
- https://alexbevi.com/blog/2026/03/14/reverse-engineering-a-dos-game-with-ghidra-and-codex

### Inspirations
- https://github.com/openblack/bw1-decomp
- https://github.com/marijnvdwerf/legoland

## Legal

This is an unofficial software preservation and research project. It is not affiliated with or endorsed by DMA Design, Psygnosis, or current rights holders.

All trademarks and game assets belong to their respective owners. The [GPLv3 License](LICENSE) covers only code written for this repository.
