# Pipeline

This project is not at the "build matching binaries and upload a progress badge" stage yet.
The honest pipeline is:

1. Put the exact original executable at `orig/LEMBALL/LEMBALL.EXE`.
2. Recover translation-unit names from the binary.
3. Recover target-side byte ranges for those units.
4. Build candidate source translation units with a period-correct compiler profile.
5. Compare candidate functions against exact target bytes, not just whole-object existence.
6. Record the compiler profile used for each comparison result.
7. Generate decomp.dev progress only after the comparison backend is truthful.

## Current commands

- `make host-check`
  Runs a host-side syntax pass over the current `LEMBALL` tree.
- `make validate-target`
  Confirms that the local retail `LEMBALL.EXE` matches the pinned SHA-256 in
  `reccmp-project.yml`.
- `make build-base`
  Compiles the current candidate translation units with the Open Watcom probe profile and
  emits per-object disassembly into `build/base/LEMBALL/disasm/`.
- `make build-target`
  Builds target-side object wrappers from verified byte slices in `LEMBALL.EXE`.
- `make objdiff-config`
  Regenerates `objdiff.json` from the current source tree and the verified target-slice
  manifest.
- `make pipeline`
  Runs the honest local loop in one step: validate the target binary, build current base
  objects, build target-side slice objects, regenerate `objdiff.json`, and write
  `build/status.json`.
- `make verify`
  Runs `reccmp` against a future MSVC-era rebuild. This is the real function-matching command
  once the Microsoft toolchain lane exists.
- `make status`
  Regenerates `objdiff.json` and writes `build/status.json`, which is the realistic
  machine-readable project status today.

## MSVC 4.20 lane

The repository now includes the missing skeleton for the real matching lane:

- `CMakeLists.txt` can emit a Win32 `LEMBALL.EXE` when `LEMBALL_ENABLE_WIN32=ON`
- `CMakePresets.json` records an `msvc420-nmake` configure preset for local Windows use
- `tools/patch_c2.py` patches VC 4.20's `C2.EXE` the same way LEGO Island does
- `tools/generate_reccmp_user.py` writes `reccmp-user.yml` for the original executable
- `tools/generate_reccmp_build.py` writes `reccmp-build.yml` for the rebuilt executable
- `tools/verify` now auto-generates those YAML files when the expected inputs exist
- `.github/workflows/msvc420-build.yml` builds the project with portable VC 4.20 on
  `windows-2022`, decrypts `orig/LEMBALL/LEMBALL.EXE.gpg`, and verifies against the pinned
  target hash

The original binary is now provided to CI through an encrypted repo file plus the
`LEMBALL_GPG_PASSPHRASE` secret. The raw executable is reconstructed only during the workflow.

## Manifests

The machine-consumed manifests live under `manifests/`, not `config/`.

- `manifests/source_files/LEMBALL.txt`
  recovered translation-unit inventory
- `manifests/target_slices/LEMBALL.txt`
  verified byte slices used to assemble synthetic target objects

If a file is not consumed by the build or compare pipeline, it should not live there.

## Why `make status` matters

Right now the project can:

- locate the original binary
- validate that the local binary is the pinned retail target
- recover file names and startup/logging evidence
- build target-side wrappers for verified slices
- compile candidate base objects with a 1990s Windows-capable compiler profile
- regenerate objdiff input from the current manifests and source tree

Right now the project cannot yet:

- claim function-level matches from the active pipeline
- rebuild a byte-matching `LEMBALL.EXE`

## Matching definition

A function matches when the bytes produced by the reconstruction toolchain are identical to the
original function bytes after the comparison tool normalizes relocations and other accepted
non-semantic differences.

That means the real milestone is not "some report file exists". The real milestone is
"the compiler and verifier agree that this function matches."

## Hashes versus matching

The target hash in `reccmp-project.yml` has one job: verify that the original binary on disk is
the exact retail build the project is targeting.

It does **not** measure decompilation progress.

For Win95-era C++ projects in the `isle` / `legoland` style, progress comes from:

1. building the reconstructed code with the original compiler family
2. producing debug info that maps rebuilt functions to source and addresses
3. comparing each rebuilt function against the original binary with `reccmp`

The practical rule is:

- hash proves we have the right target
- `reccmp` proves whether a function matches
- objdiff/decomp.dev summarize progress over the current object set

## Byte-accuracy priorities

For this project, the practical order is:

1. Recover real function boundaries and target-side byte ranges.
2. Reconstruct source with period-correct C++ structure.
3. Lock down the original compiler family and flags closely enough that dead code,
   inlining, frame setup, and helper-call shape stop drifting.
4. Compare one function at a time.

In other words: compiler configuration is part of reverse engineering, not a cleanup step at
the end.

## Isle-style matching track

`reccmp-project.yml` records the original `LEMBALL.EXE` target and its verified SHA-256. With
the current inferred split, the practical source root is the repository itself, with active code
under `LEMBALL/` and `LEMBALL/VISOS/`.

Treat an MSVC-era build plus a reccmp-style comparison as the serious function-matching path.
Treat the current Watcom lane as a probe that helps us study code generation and calling
convention behavior while the real Microsoft build path is still being recovered.

The closest reference model right now is `marijnvdwerf/legoland`:

- original binary pinned by SHA-256
- original compiler family (MSVC6 there) used for the rebuild
- rebuild produces binary plus debug symbols
- `tools/verify` wraps `reccmp-reccmp --target ...`
- progress is tracked per function, not by comparing whole executable hashes
- generated reporting files are derived from the build/manifests, not hand-maintained

That is the direction LEMBALL should follow.

## decomp.dev status

The current LEMBALL decomp.dev-style path is **not fully correct yet**.

Why:

- target-object coverage still only covers a tiny subset of translation units today
- our current target objects are synthetic wrappers around verified byte slices, not full
  original target objects for the whole binary
- our current base objects come from the Watcom probe lane, not the inferred original MSVC lane

So the current decomp.dev/object pipeline is useful for experimentation, and it is now stable
enough to regenerate consistently, but it is not yet a truthful project-wide progress backend.
