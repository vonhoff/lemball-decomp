# Decompilation worker

## Constraints

- **Never edit `README.md` unless requested.**
- Reconstruct from the original binary (`data/LEMBALL.EXE`) using reccmp as the source of truth.
- Use project commands below for verification.

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
