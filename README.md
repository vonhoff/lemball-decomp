# Lemmings Paintball Decompilation

Work-in-progress reconstruction of 1996 Win32 `LEMBALL.EXE`, built with MSVC
4.20 and compared against original binary with reccmp.

Original executable reports LINK 3.00 and contains an incremental link table at
`0x00401000` through `0x00403890`; real code begins at `0x00406160`. Current
MSVC 4.00 build uses `/INCREMENTAL:YES` and compares linker thunks by
resolved destination identity. Whole-image parity remains separate work: resource
section, CRT, and imports differ.

## Decompilation workflow

Use one checkout and one editing agent at a time. Other agents may inspect
Ghidra or suggest work, but do not edit or build concurrently.

Start with a shape-first batch:

1. Run candidate miner with `--save-baseline` to save current non-stub 100%
   address set and group unmatched real functions by normalized original
   instruction shape.
2. Prefer largest cluster with exact exemplar or simple compiler-owned C++
   pattern.
3. Inspect every member in Ghidra, recover one parameterized C/C++ form, and
   implement all proven members.
4. Compare every member and run full JSON. Keep zero-loss exact gains, then
   continue next cluster.

This turns repeated constructors, destructors, scalar deletes, vtable restores,
accessors, and small forwarding bodies into one compiler investigation plus
many verified matches. Shape similarity is candidate generation, never proof:
ABI, stack cleanup, offsets, constants, xrefs, and source ownership still need
per-member confirmation.

```powershell
.decomp-venv\Scripts\python.exe tools\find_decompilation_candidates.py `
  --save-baseline
```

After high-yield families are exhausted, use miner's `singleton fallback`.
Ghidra inventory export validates all 2,077 ILT entries and excludes them.

## Setup

Install [MSVC 4.00](https://github.com/vonhoff/MSVC400) outside the checkout
and set `MSVC400_ROOT` to the directory containing `bin\CL.EXE`, `include`,
and `lib`. For a per-user installation in PowerShell:

```powershell
$msvc400 = "$env:LOCALAPPDATA\Programs\MSVC400"
git clone https://github.com/vonhoff/MSVC400 $msvc400
[Environment]::SetEnvironmentVariable("MSVC400_ROOT", $msvc400, "User")
$env:MSVC400_ROOT = $msvc400
```

The build script checks `MSVC400_ROOT`, then the ignored `msvc400` directory in
the checkout. It validates the selected installation and propagates the same
root to CMake.

```powershell
py -m venv .decomp-venv
.decomp-venv\Scripts\python.exe -m pip install -r requirements.txt
.decomp-venv\Scripts\python.exe tools\build_msvc400.py
```

The canonical build installs a narrow reccmp 0.1.6 compatibility hook. For
data comparison only, original pointers to verified `E9` entries in the
documented linker ILT are resolved to their real destinations.

## Legal

Encrypted non-working original executable is included only for automated
comparison. No game assets required for play are included. This unofficial
preservation project is not affiliated with any rights holder.
