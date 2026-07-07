# Lemmings Paintball Decompilation

Work-in-progress reconstruction of the original 1996 Windows C++ codebase behind
`LEMBALL.EXE`.

The goal of this project is to create a fully functional decompilation of Lemmings Paintball. The goal is to be as accurate as possible by matching the recompiled instructions to the original machine code. The goal is to provide a workable codebase that can be modified, improved, and later ported to other platforms.

> Note: This repository is for decompilation only and its code is true to the original release.
> The reconstructed game target will not compile for targets other than 32-bit Windows.

## Build lanes

- `msvc420` is the byte-matching verification lane. This is the one CI should trust for rebuilt-binary comparison.
- `openwatcom-win32` is an optional local compile-check lane for non-Windows hosts. It targets 32-bit Win32 through Open Watcom 2.x and is only meant to catch portability and syntax issues earlier. It is not expected to produce byte-matching output.

## Objdiff

This repository follows the same broad reporting pattern as `marijnvdwerf/legoland`: CI verifies the rebuilt binary with `reccmp` and then publishes an objdiff-format `report.json` generated from the comparison results.

- [`tools/generate_reccmp_report.py`](/Users/simon/CLionProjects/lemball-decomp/tools/generate_reccmp_report.py) builds the report artifact from the `reccmp` compare results plus the source markers and Ghidra-backed manifest.
- `make report` writes that artifact to `build/report.json`.
- CI uploads the report as `LEMBALL_report`, which matches decomp.dev's expected artifact naming.

Example:

```sh
cmake --preset openwatcom-win32
cmake --build --preset openwatcom-win32
```

This preset expects `wcl386` to be available on `PATH`.
