# Lemmings Paintball Decompilation

Work-in-progress reconstruction of the original 1996 Windows C++ codebase behind
`LEMBALL.EXE`.

This project is being shaped toward a functionally complete decompilation of Lemmings
Paintball. It aims to be as accurate as possible, matching the recompiled instructions to the
original machine code as much as possible. The goal is to provide a workable codebase that can
be modified, improved, and ported to other platforms later on.

> Note: This repository is for decompilation only and its code is true to the original release.
> The reconstructed game target will not compile for targets other than 32-bit Windows.

This repository is not "report-first". The current goal is to reconstruct the original
translation units, startup flow, and compiler situation honestly enough that function-level
matching becomes possible.

## Current pipeline

1. Put the original binary at `orig/LEMBALL/LEMBALL.EXE`.
2. Recover source-file names and startup behavior from the binary.
3. Build target-side object wrappers from verified byte slices.
4. Compile candidate source translation units with a period-correct toolchain profile.
5. Compare MSVC-era rebuilds function-by-function against the original with `reccmp`.
6. Generate `objdiff.json` from the active manifests and source tree.
7. Only publish decomp.dev progress once the object pipeline is truthful.

## Useful commands

- `make host-check`
  Host-side `C++98` syntax pass over the current `LEMBALL` tree.
- `make build-base`
  Compile the current candidate translation units with the Open Watcom profile.
- `make build-target`
  Assemble verified target-side objects from `LEMBALL.EXE`.
- `make validate-target`
  Verify that the local retail executable matches the pinned SHA-256 in `reccmp-project.yml`.
- `make objdiff-config`
  Generate `objdiff.json` from the current source tree and target-slice manifest.
- `make pipeline`
  Run the current honest local pipeline: validate target, build probe objects, build slice
  objects, regenerate `objdiff.json`, and write `build/status.json`.
- `make verify`
  Run `reccmp` against a future MSVC-era rebuild. This is the serious byte-matching path, not a
  whole-file hash compare.
- `make status`
  Regenerate `objdiff.json` and write `build/status.json`, the truthful machine-readable project
  status.

## Matching build lane

The intended byte-matching build lane now follows the same general model used by LEGO Island:

1. build the project with portable Microsoft Visual C++ 4.20
2. produce `LEMBALL.EXE` and `LEMBALL.PDB`
3. generate `reccmp-user.yml` for the original retail executable
4. generate `reccmp-build.yml` for the rebuilt executable
5. compare functions with `reccmp`

For GitHub Actions, the repository now includes a Windows workflow that checks out
`itsmattkc/msvc420`, patches `C2.EXE`, configures with `NMake Makefiles`, and builds
`build-msvc420/LEMBALL.EXE`.

The repository stores the retail executable as an encrypted file at
`orig/LEMBALL/LEMBALL.EXE.gpg`. GitHub Actions decrypts that file with the
`LEMBALL_GPG_PASSPHRASE` secret, validates the SHA-256, and then runs `tools/verify`.

## Layout

- `LEMBALL/`
  Single package containing the reconstructed game and framework code.
- `LEMBALL/VISOS/`
  ViSOS framework-side translation units such as `VSINIT.CPP`, `VSMEM.CPP`, and `VSGDI.CPP`.
- `orig/LEMBALL/`
  Local target-binary area. Only `LEMBALL.EXE.gpg` should be committed there.
- `manifests/source_files/LEMBALL.txt`
  Recovered translation-unit inventory consumed by the pipeline.
- `manifests/target_slices/LEMBALL.txt`
  Verified target byte slices used for target-side object recovery.
- `reccmp-project.yml`
  Target metadata for future function matching against the original executable.

## Notes

- The current tree follows the recovered file names from the binary and an inferred period-style
  split between the ViSOS framework and the game-specific layer.
- `MAIN.CPP` is currently an inferred startup shim reconstructed from the CRT-to-game handoff
  and period Win32 project style, not from a preserved filename string in the binary.
- The active build can produce candidate base objects and target-side slice objects.
- The SHA-256 in `reccmp-project.yml` verifies that we are targeting the right retail binary.
  It is not the matching metric.
- The encrypted retail binary is committed as `orig/LEMBALL/LEMBALL.EXE.gpg`; the raw
  `orig/LEMBALL/LEMBALL.EXE` is a local/generated file and should never be committed.
- Real matching is per-function: the rebuild is compiled with the original compiler family,
  debug info is used to map rebuilt functions back to original addresses, and `reccmp`
  compares normalized function bytes against the original executable.
- `objdiff.json` is generated and ignored, following objdiff's recommended workflow.
- The generated `objdiff.json` is still partial and not yet a full decomp.dev-ready view of the
  binary because the target-object coverage is still partial.
- A direct macOS `wibo` path is plausible, but it is still experimental here. The first local
  VC 4.20 probe on this arm64 Mac ran `wibo` itself successfully but stopped on an
  `MSVCRT40.dll` import issue before the compiler could be used as a dependable local lane.

See:

- `docs/pipeline.md`
- `docs/toolchain.md`
