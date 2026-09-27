# Lemmings Paintball Decompilation

[![Build Status](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml/badge.svg)](https://github.com/vonhoff/lemball-decomp/actions/workflows/build.yml)
[![Code Progress](https://decomp.dev/vonhoff/lemball-decomp.svg?mode=shield&measure=code&label=Code)](https://decomp.dev/vonhoff/lemball-decomp)

[<img src="https://decomp.dev/vonhoff/lemball-decomp.svg?w=512&h=256" width="512" height="256" alt="Decomp Progress Chart">](https://decomp.dev/vonhoff/lemball-decomp)

> [!IMPORTANT]
> The GPL-3.0 license covers non-reconstructed code only. The reconstructed game code is unlicensed. See [Legal](#legal) for details.

A matching decompilation of *Lemmings Paintball* (1996, Windows 95).

The project aims to recover readable, semantic C++ while matching the original executable as closely as possible. Reconstructed code is compiled with Microsoft Visual C++ 4.00 and compared function-by-function using reccmp.

Live progress is tracked on [decomp.dev](https://decomp.dev/vonhoff/lemball-decomp).

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

*Lemmings Paintball*, its name, trademarks, and original copyrighted material remain the property of their respective rights holders. No original executable or game assets are included in or distributed with this repository.

The reconstructed game code was produced through reverse engineering and analysis of the compiled program. No original or leaked source code was used. No license is granted for the reconstructed game code, and nothing in this repository grants any rights in third-party intellectual property.

Code independently developed for this project that is not part of the reconstructed game code is licensed under the [GPL-3.0 license](LICENSE).
