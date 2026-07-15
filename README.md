# Lemmings Paintball Decompilation

Work-in-progress reconstruction of the original 1996 Windows C++ codebase behind
`LEMBALL.EXE`.

The goal of this project is to create a fully functional decompilation of Lemmings Paintball. The goal is to be as accurate as possible by matching the recompiled instructions to the original machine code. The goal is to provide a workable codebase that can be modified, improved, and later ported to other platforms.

> Note: This repository is for decompilation only and its code is true to the original release.
> The reconstructed game target will not compile for targets other than 32-bit Windows.

The reconstructed game and VISOS runtime share one original-era source tree under `src/`; subsystem directories contain both implementations and their related headers, without artificial game/engine container layers or a standalone public-header tree.

The MSVC 4.20 link uses four forced archive anchors from
`cmake/msvc420-force-symbols.rsp`. They pull otherwise-unreferenced VISOS
archive members containing ten annotated callbacks and debug helpers. Game-side
functions and reconstructed thunks are linked through their normal ownership
and require no forced-symbol list.

## Build lanes

- `msvc420` is the byte-matching verification lane. This is the one CI should trust for rebuilt-binary comparison.

## Objdiff

This repository follows the same broad reporting pattern as `marijnvdwerf/legoland`: CI verifies the rebuilt binary with `reccmp` and then publishes an objdiff-format `report.json` generated from the comparison results.

- [`tools/generate_reccmp_report.py`](tools/generate_reccmp_report.py) builds the report artifact from the `reccmp` compare results plus the source markers and Ghidra-backed manifest.
- `make report` writes that artifact to `build/report.json`.
- CI uploads the report as `LEMBALL_report`, which matches decomp.dev's expected artifact naming.

