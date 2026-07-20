# Lemmings Paintball Decompilation

Work-in-progress reconstruction of the original 1996 Windows C++ codebase behind `LEMBALL.EXE`. The project is not yet functional and remains under active development.

The aim is to produce a fully functional decompilation of *Lemmings Paintball* that matches the original machine code as closely as possible. The completed codebase is intended to serve as a faithful reference for re-implementations.

> **Note:** This repository is focused exclusively on decompilation, and the reconstructed code is kept faithful to the original release. The game target is designed for 32-bit Windows and is not expected to compile for other platforms.

## Objdiff

This repository follows the same broad reporting pattern as `marijnvdwerf/legoland`: CI verifies the rebuilt binary with `reccmp` and then publishes an objdiff-format `report.json` generated from the comparison results.
