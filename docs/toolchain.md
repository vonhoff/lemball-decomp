# Toolchain notes

## Reality check

We are developing on macOS, but the target binary is a 32-bit Windows executable from the
mid-1990s. A native macOS build is useful for source organization and syntax checking, but
it will not produce a byte-matching `LEMBALL.EXE`.

## Recommended split

- Use the macOS host build only for editor integration, syntax checks, and project hygiene.
- Use a Windows-oriented compiler environment for any candidate base objects intended for
  matching.

## Language stance

The current binary evidence points to an original Win32 C++ codebase, not a plain C one.

That evidence includes:

- `.CPP` artifact names such as `VSINIT.CPP`, `VSMEM.CPP`, and `VSGDI.CPP`
- Microsoft Visual C++ runtime/assertion strings
- subsystem naming that looks framework-driven rather than C-library-driven

Because of that, the repository now treats conservative mid-1990s C++ as the default source
language for decomp work.

## Candidate compiler paths

### 1. Original-era Microsoft compiler

The binary evidence now points more strongly at Microsoft Visual C++ than Watcom:

- the executable contains `Microsoft Visual C++ Runtime Library`
- it also contains the standard Visual C++ assertion text
- the CRT/startup region looks like statically linked Microsoft runtime code

That makes an MSVC 4.x / 5.0-era build the primary matching path, not an optional extra.

Practical first-pass assumptions for the Microsoft lane:

- compiler family: Visual C++ 4.x or 5.0
- target: Win32, x86, Windows 95-era desktop app
- optimization stance: start from `/Od`
- inline expansion: `/Ob0`
- frame pointer omission: `/Oy-`
- function-level linking: `/Gy-`
- RTTI: `/GR-` unless binary evidence proves otherwise
- C++ exceptions: `/GX-` unless binary evidence proves otherwise
- runtime library: likely static CRT, but `/ML` versus `/MT` still needs proof

These are informed starting points, not proven facts. Each one should be tested against real
function bytes.

## What the real matching loop looks like

The `legoland` project is a good reference model here:

- verify the exact target binary by SHA-256
- rebuild the project with the original Microsoft compiler family
- keep function-address annotations in source
- emit debug info so rebuilt functions can be mapped back to their source and addresses
- run `reccmp` to compare each rebuilt function against the original binary

That means the serious LEMBALL loop should become:

1. compile with the recovered Microsoft toolchain
2. produce `LEMBALL.EXE` plus debug info
3. run `tools/verify` / `reccmp`
4. inspect per-function diffs
5. tune source, types, locals, and compiler flags
6. repeat

The original binary hash is just the guardrail that says "we are comparing against the right
retail build."

The current repository-level support around that loop is:

1. `make validate-target` proves the retail executable on disk is the expected one
2. `make build-base` emits current probe objects and disassembly
3. `make build-target` emits target-side wrapper objects from verified slices
4. `make objdiff-config` regenerates objdiff input from the current tree
5. `make status` records the truthful current state without claiming matches

### 2. Open Watcom V2

Open Watcom's current documentation says the toolchain targets 32-bit Windows, including
Windows 95/98/Me, and the project publishes current builds and docs online.

- Docs: https://open-watcom.github.io/open-watcom-v2-wikidocs/c_readme.html
- Project site: https://open-watcom.github.io/
- Downloads / daily builds: https://www.openwatcom.org/

This is the most practical probe compiler to try from a modern machine, especially for
studying calling conventions, object layout, and how recovered C++ decompiles under a
period-style frontend.

The repository now treats Watcom as an exploratory lane, not the final matching lane.
`toolchains/openwatcom/wpp386-byteprobe.flags` intentionally uses a conservative profile:

- `-bt=nt`
- `-3`
- `-d1`
- `-od`

The important part is `-od`: the Watcom documentation says this disables all optimizations,
which is exactly what we want while the binary still appears to preserve code that a more
aggressive optimizer would likely fold away or delete.

### 3. Original-era Microsoft compiler under emulation or a VM

Practical options:

- a Windows 95/98 virtual machine
- Wine/CrossOver with an older Windows toolchain
- a dedicated container or helper VM that emits `.obj` files for comparison
- a modern Windows GitHub Actions runner driving a portable VC 4.20 checkout, which is the
  current repository CI direction because the host image only needs to run the old tools, not
  provide them

## Hosted CI reality

GitHub-hosted runners do not provide Visual C++ 4.2 as a built-in toolchain.
As of July 7, 2026, GitHub documents `windows-latest` as a Windows Server 2025 image, and
`windows-2022` remains available as an explicit label. In either case, the realistic way to
run VC 4.20 in CI is the LEGO Island approach: check out a portable VC 4.20 tree and invoke it
manually. Sources:

- [GitHub-hosted runners reference](https://docs.github.com/en/actions/reference/runners/github-hosted-runners)
- [actions/runner-images](https://github.com/actions/runner-images)
- [isledecomp/isle build workflow](https://github.com/isledecomp/isle)

This repository now carries that same style of CI workflow in
`.github/workflows/msvc420-build.yml`.

The main difference is how the original retail executable is supplied: this repository uses an
encrypted checked-in file, `orig/LEMBALL/LEMBALL.EXE.gpg`, plus a GitHub Actions secret,
`LEMBALL_GPG_PASSPHRASE`, to reconstruct the raw executable during CI.

## macOS reality

A direct macOS lane is possible in principle with `wibo`, because the project publishes a
`wibo-macos` binary specifically for running simple 32-bit Windows command-line tools on macOS.
Source: [decompals/wibo](https://github.com/decompals/wibo)

On this machine, `wibo` itself ran successfully under Rosetta, but a first VC 4.20 probe
stopped immediately on a missing `malloc` import from `MSVCRT40.dll`. So the honest status is:

- GitHub-hosted Windows is the primary dependable CI lane
- local macOS `wibo` work is promising, but still experimental
- Docker is only worth pursuing after the Windows lane is stable, because a Linux container plus
  `wibo`/Wine is more moving parts than the repository needs right now

## Repository layout support

This repository now includes:

- `LEMBALL/` for game-facing translation units and project headers
- `LEMBALL/VISOS/` for framework-facing translation units and project headers
- `manifests/target_slices/LEMBALL.txt` for verified target object slices
- `manifests/source_files/LEMBALL.txt` for the recovered translation-unit inventory
- `reccmp-project.yml` for target metadata once a matching executable build exists
- `scripts/check_host_sources.sh` for host-side syntax validation
- `toolchains/openwatcom/` for a containerized Watcom bootstrap path
- `scripts/check_watcom_profile.sh` for an actual Open Watcom object compile pass

## Immediate next steps

1. Recover more compiler fingerprint evidence from the binary before locking in a single MSVC release.
2. Stand up an MSVC-era object build lane that can emit COFF objects and debug info suitable for real function comparison.
3. Keep Watcom as a probe lane for fast iteration, but do not treat a Watcom byte mismatch as a source mismatch.
4. Use `build/base/LEMBALL/disasm/` to inspect object-level codegen after every non-trivial reconstruction step.

## Current comparison status

We can now emit candidate base objects with Open Watcom into `build/base/LEMBALL/`, and
the probe lane also emits Watcom disassembly listings into `build/base/LEMBALL/disasm/`.
We can also emit honest target-side wrappers for verified binary slices into
`build/target/LEMBALL/`.

What we still do not have is a trustworthy Microsoft-compiler comparison lane. Until the
project has a working MSVC-era COFF/PDB or equivalent setup, the truthful status artifact is
still `build/status.json`.
