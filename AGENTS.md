# Decompilation worker

## Constraints

- **Never edit `README.md` unless requested.**
- Preserve ABI, layout, annotations, source placement, and link order.
- Reconstruct from the original binary (`data/LEMBALL.EXE`) using reccmp as the source of truth.
- Use compiler-generated C/C++; use assembly only for proven real ABI/vtordisp thunks after failed C++ probes. Mark it `// ABI-THUNK:` with original bytes, ABI, failed form, and reccmp result. Never embed opcodes or recreate ILT wrappers.
- Repair inventories; never hide entries with denylists. Add ownership only after Ghidra confirms address and source owner. Investigate every datacmp issue.
- Temporary probes belong only in ignored `build-msvc400/`. Run `clang-format -i <paths>` on modified C/C++.
- Use project commands below for verification.

## Binary layout

Original LINK 3.00 ILT: `0x00401000`–`0x00403890`, 2,077 five-byte `E9` entries through `0x0040388C`; real code starts `0x00406160`. Ghidra marks ILT as `linker-ilt`; source wrappers use `// LINKERILT:`. Keep MSVC 4.00 `/INCREMENTAL:YES`; compare thunk destination identity. Whole-image parity is incomplete across `.text`, resources, CRT, imports, and object order.

## Commands

Focused diagnosis, from `build-msvc400`:

```powershell
..\.decomp-venv\Scripts\reccmp-reccmp.exe --target LEMBALL --verbose 0xADDRESS --print-rec-addr
```

Canonical verification, from repository root:

```powershell
.decomp-venv\Scripts\cmake.exe --fresh --preset msvc400
.decomp-venv\Scripts\cmake.exe --build --preset msvc400 --clean-first
.decomp-venv\Scripts\python.exe tools\generate_objdiff_report.py
```

## Completion

Run canonical verification and `git diff --check`; review diff; stage explicit paths; commit/push verified progress; leave worktree clean. Report change and commit/push status in one or two sentences.
