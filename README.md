# Lemmings Paintball Decompilation

This repository contains a work-in-progress C++ reconstruction of the 1996 Windows game *Lemmings Paintball*. It targets Microsoft Visual C++ 4.0 and uses [`reccmp`](https://github.com/isledecomp/reccmp) to compare the reconstructed code with the original executable.

The goal is to produce a readable representation of the program while reproducing its behavior, data layout, and compiler output where practical. The project is intended for software preservation, research, and technical analysis.

A matching progress report is available on [decomp.dev](https://decomp.dev/vonhoff/lemball-decomp).

## Development

The project is primarily developed with the assistance of AI agents because the scale of the reconstruction exceeds the time available for manual analysis alone. These agents assist with binary analysis, implementation, naming, refactoring, and verification.

Generated output is treated as a hypothesis, not as authoritative source code. Changes are validated against binary evidence, recovered symbols, compiler output, and automated comparison results. Implementations may be revised or replaced as better evidence becomes available.

## Reference material

The original executable and game assets are not included. Users must provide their own compatible copy of `LEMBALL.EXE`.

Original binaries, assets, extracted resources, encrypted or decrypted reference files, access credentials, and private download locations must not be committed to this repository.

## Legal

This repository is an unofficial software-preservation and research project. It is not affiliated with, authorized by, or endorsed by the original developers, publishers, or other rights holders.

Reverse engineering and decompilation are subject to applicable copyright, contract, anti-circumvention, and other laws, which vary by jurisdiction. Independent reconstruction or binary matching does not, by itself, confer a right to reproduce, modify, distribute, or otherwise use protected material.

All rights, title, and interest in *Lemmings Paintball*, including the original software, assets, names, and trademarks, remain with their respective owners. The terms of [`LICENSE`](LICENSE) apply only to material that contributors are legally entitled to license and confer no rights in the original game or any third-party material.

Users and contributors are solely responsible for determining whether their use, modification, or distribution of this project is lawful in their jurisdiction.
