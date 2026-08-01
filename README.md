# Lemmings Paintball decompilation

This repository is a work-in-progress C++ reconstruction of the 1996 Windows game `LEMBALL.EXE`.

The reconstructed program builds with Microsoft Visual C++ 4.0 and is compared with the original executable using [reccmp](https://github.com/isledecomp/reccmp). It is not complete yet, but the project builds and the comparison tools can measure progress function by function.

## Build

The build currently runs on Windows.

1. Install [MSVC 4.00](https://github.com/vonhoff/MSVC400) outside this repository.
2. Set `MSVC400_ROOT` to the directory containing `bin\CL.EXE`, `include`, and `lib`.
3. Create the Python environment and build the project:

```powershell
py -m venv .decomp-venv
.decomp-venv\Scripts\python.exe -m pip install -r requirements.txt
.decomp-venv\Scripts\python.exe tools\build_msvc400.py
```

The executable and PDB are written to `build-msvc400`.

## Verify a change

Run these commands from the repository root:

```powershell
.decomp-venv\Scripts\python.exe tools\lint_reccmp_metadata.py
.decomp-venv\Scripts\python.exe tools\build_msvc400.py
.decomp-venv\Scripts\reccmp-project.exe detect --search-path data

Push-Location build-msvc400
..\.decomp-venv\Scripts\reccmp-reccmp.exe --target LEMBALL --json reccmp.json --json-diet
..\.decomp-venv\Scripts\reccmp-decomplint.exe --target LEMBALL
..\.decomp-venv\Scripts\reccmp-datacmp.exe --target LEMBALL --no-color
..\.decomp-venv\Scripts\reccmp-vtable.exe --target LEMBALL
Pop-Location

.decomp-venv\Scripts\python.exe tools\find_decompilation_candidates.py --check-baseline
```

`AGENTS.md` has the full reconstruction rules and evidence requirements.

## Source layout

- `src/ENGINE` contains the recovered runtime, graphics, media, networking, and debug code. Graphics implementations are split into `GDI/RENDER`, `GDI/WINDOW`, and `GDI/VGA`.
- `src/FRONTEND` contains menus, lobby screens, and other user-interface code.
- `src/LEVEL` is split into `CHUNK`, `ENTITY`, `IO`, `RENDER`, `RUN`, `STATE`, and `VTABLE` implementations. Headers remain directly under `src/LEVEL`.
- `src/RESOURCE` contains archive and typed-resource loading code.
- `src/SHELL` contains startup, options, and top-level window code.
- `data` contains comparison metadata and the encrypted reference executable.
- `tools` contains the build, metadata, and comparison helpers.

Some filenames remain short because they reflect the style and tool limits of the original codebase. The folders are the quickest way to find the relevant subsystem.

## Contributing

Treat the original executable as the source of truth. Keep changes compatible with MSVC 4.20, preserve source and link order, and do not use inline assembly to force a match. A readable reconstruction with measured binary improvement is better than decompiler-shaped code or register tricks.

Build after meaningful edits and run the complete verification set before committing. Do not update the exact-match baseline to hide regressions.

## Legal

The encrypted, non-working reference executable is included only for automated comparison. The repository does not include the assets needed to play the game. This is an unofficial preservation project and is not affiliated with the original developers or rights holders.
