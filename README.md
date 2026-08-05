# Lemmings Paintball Decompilation

This repository contains a work-in-progress C++ reconstruction of the 1996 Windows game *Lemmings Paintball*. It targets Microsoft Visual C++ 4.0 and uses [reccmp](https://github.com/isledecomp/reccmp) to compare the reconstructed code against the original executable.

The goal is to produce a readable, maintainable representation of the program that reproduces its behavior, data layout, and compiler output where practical. The project exists for software preservation, research, and technical analysis.

A live progress report is available on [decomp.dev](https://decomp.dev/vonhoff/lemball-decomp).

## Development

The project is developed primarily with the assistance of AI agents, since the scope of the reconstruction exceeds what manual analysis alone could cover in a reasonable timeframe.

**See [`docs/BACKLOG.md`](docs/BACKLOG.md) for the decision framework (map vs Macintosh blueprint vs do-not-implement), naming rules, and the prioritized, evidence-based working backlog.** Its item-1 work-list is regenerated from the Macintosh coverage inventory into `data/unreconstructed-correlations.csv`.

AI-generated output is treated as a hypothesis, not authoritative source code. Every change is validated against binary evidence, recovered symbols, compiler output, and automated comparison results, and implementations are revised or replaced as better evidence emerges.

## Reference material

The original executable and game assets are not included. To build or test against a target, you must supply your own compatible copy of `LEMBALL.EXE`.

## Legal

This is an unofficial software preservation and research project. It is not affiliated with, authorized by, or endorsed by the original developers, publishers, or other rights holders.

Reverse engineering and decompilation are subject to copyright, contract, anti-circumvention, and other laws that vary by jurisdiction. Independent reconstruction or binary matching alone does not grant the right to reproduce, modify, distribute, or otherwise use protected material.

All rights, title, and interest in *Lemmings Paintball*, including the original software, assets, names, and trademarks, remain with their respective owners. The [LICENSE](LICENSE) applies only to material that contributors are legally entitled to license and does not confer any rights to the original game or third-party material.

Users and contributors are solely responsible for determining whether their use, modification, or distribution of this project is lawful in their jurisdiction.
