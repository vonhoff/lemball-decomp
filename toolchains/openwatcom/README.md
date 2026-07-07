# Open Watcom container bootstrap

This image gives the project a reproducible Windows-targeting compiler environment from
macOS or Linux without conflating it with the native host build.

## What it is for

- trying candidate Watcom builds
- producing exploratory `.obj` files for `objdiff`
- checking calling conventions and library behavior
- emitting disassembly listings from the exact probe objects

## What it is not for

- proving that the original game used Watcom
- guaranteeing a byte-matching `LEMBALL.EXE`

## Current profile

The repository's default probe profile is:

- `toolchains/openwatcom/wpp386-byteprobe.flags`

This profile is intentionally conservative. In particular, it uses `-od` so the probe lane
does not optimize away structure that may still exist in the original binary.

## Usage

Build the image:

```sh
docker build -t lemball-openwatcom toolchains/openwatcom
```

Start a shell in the repository:

```sh
docker run --rm -it -v "$PWD":/work -w /work lemball-openwatcom
```

Inside the container, the Open Watcom tools are placed on `PATH`.

## Source of the compiler archive

The container currently downloads the portable stable archive published by the Open Watcom
project at:

- http://openwatcom.org/ftp/source/ow_portable_v2_stable.zip

If you want to try the test build instead, override `OW_PORTABLE_URL` at build time.
