# Lemmings Paintball Decompilation

Work-in-progress reconstruction of the original 1996 Windows C++ codebase behind
`LEMBALL.EXE`.

The goal of this project is to create a fully functional decompilation of Lemmings Paintball. The goal is to be as accurate as possible by matching the recompiled instructions to the original machine code. The goal is to provide a workable codebase that can be modified, improved, and later ported to other platforms.

> Note: This repository is for decompilation only and its code is true to the original release.
> The reconstructed game target will not compile for targets other than 32-bit Windows.

## Objdiff

This repository follows the same broad reporting pattern as `marijnvdwerf/legoland`: CI verifies the rebuilt binary with `reccmp` and then publishes an objdiff-format `report.json` generated from the comparison results.

- `make ghidra-functions` regenerates `data/manifest.json` from the sibling
  `lemball-ghidra` project using Ghidra's supported headless scripting API.
- [`tools/generate_reccmp_report.py`](tools/generate_reccmp_report.py) converts
  `build/reccmp-sample.json` plus that manifest into an objdiff v2 report with
  one honest unit: `LEMBALL.EXE`.
- `make report` writes that artifact to `build/report.json`.

Pass the Ghidra project explicitly. Set `GHIDRA_HOME` only when
`analyzeHeadless` is not on `PATH`:

```sh
make ghidra-functions GHIDRA_PROJECT=/path/to/LEMBALL_DECOMP.gpr
make ghidra-functions GHIDRA_PROJECT=/path/to/LEMBALL_DECOMP.gpr GHIDRA_HOME=/path/to/ghidra
```

The Ghidra project must not be open for editing while a headless export runs.

## Reverse-engineering tools

Install `reccmp`, activate the environment so `python` and the `reccmp-*`
commands are on `PATH`, then run the tools from the repository root (where
`reccmp-build.yml` is located):

```sh
python -m venv .venv
.venv/Scripts/python -m pip install -r .github/requirements-ci.txt  # Windows
# .venv/bin/python -m pip install -r .github/requirements-ci.txt    # Unix
```

The Make targets select `LEMBALL` by default and forward additional arguments
through `RECCMP_ARGS`:

```sh
# Routine correctness suite: annotations, vtables, and data
make audit

# The audit plus a symbol-location roadmap
make analyze

# Function comparison and stack-layout diagnosis
make decomplint
make reccmp RECCMP_ARGS="--verbose 0x00432320"
make stackcmp RECCMP_ARGS="0x00432320"

# Individual structural checks
make roadmap
make vtable
make datacmp

# Save, combine, and compare native reccmp JSON samples
make snapshot RECCMP_REPORT=build/before.json
make snapshot RECCMP_REPORT=build/after.json
make aggregate RECCMP_ARGS="--samples sample0.json sample1.json --output combined.json"
make aggregate RECCMP_ARGS="--diff build/before.json build/after.json"
```

The roadmap CSV is a navigation and planning index rather than a correctness
score. It maps each original address to its rebuilt address and object module;
sort or filter it by `module`, `row_type`, or `displacement` to group related
reconstruction work and translate addresses while inspecting either binary.

CI invokes every applicable tool after the MSVC 4.20 build and uploads the
native comparison sample, aggregate, objdiff report, and roadmap. The main
comparison remains the hard gate. Annotation, vtable, data, and stack diagnostics
currently use `continue-on-error` because they expose known reconstruction debt;
their output is still visible on every build. Export comparison is omitted
because LEMBALL is an EXE with no DLL export table. Locally, `make audit` is
strict and returns a failure if any structural check fails.
`stackcmp` remains address-specific and is used when `reccmp` identifies an
almost-matching function. `snapshot` and `aggregate` provide repeatable progress
baselines and before/after diffs.

Use `RECCMP_TARGET=name` to override the target. The underlying commands are
`reccmp-decomplint`, `reccmp-reccmp`, `reccmp-stackcmp`, `reccmp-roadmap`,
`reccmp-vtable`, `reccmp-datacmp`, and `reccmp-aggregate`.

