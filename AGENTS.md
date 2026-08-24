# Agents

Reconstruct *Lemmings Paintball* (1996 Win32, `LEMBALL.EXE`) as C++ that matches the original binary. Verify with [reccmp](https://github.com/isledecomp/reccmp). AI output is a hypothesis — validate against `data/LEMBALL.EXE`, PDB, and reccmp.

## Constraints

- Never edit `README.md` unless asked.
- Source of truth: x86 binary via reccmp/Ghidra — not 68K comments alone.
- No invented symbols/addresses. Every symbol needs a reccmp annotation.
- Scaffold: one primary class per `.h` / `.cpp` under `src/`.
- Functions in a `.cpp` ordered by annotated address (ascending).
- Match local naming, types, and style. Minimal diffs.

## Prerequisites

1. Compatible `LEMBALL.EXE` in `data/` (SHA-256 in `reccmp-project.yml`; not shipped).
2. MSVC 4.00 at `msvc400/` (or `MSVC400_ROOT`). See CI `vonhoff/MSVC400`.
3. Venv from repo root:
   ```powershell
   python -m venv .decomp-venv
   .decomp-venv\Scripts\pip install -r requirements.txt
   ```

## Source layout

| Dir | Role |
|-----|------|
| `AI/` | Objects, nav, groups, managers, messages |
| `Control/` | Game loop, levels, demo, state |
| `Frontend/` | Menus, drawers, FE processes/windows |
| `Map/` | Map / ground |
| `Network/` | Multiplayer |
| `Platform/` | `WinMain`, OS entry |
| `Views/` | In-game panels, display, sound view |
| `Visos/` | Engine (foundation, gfx, sound, net, resources, anim, messaging, Win32/DX) |

Shared types: `src/Common.h`. Unknown scalars: `undefined` / `undefined2` / `undefined4`.

## 68K + reccmp annotations

68K comment = naming / structure / intent from the 68K tree. Binary wins on divergence. Keep 68K lines when implementing.

```cpp
// 68K 0x1021749c __ct__7CBucketFiiPUcPUl
// FUNCTION: LEMBALL 0x00472ce0
Bucket::Bucket(int p_blockSize, int p_blockCount, unsigned char* p_memory, unsigned long* p_map)
```

Marker **directly above** the body. 68K above marker when both present. Target name always `LEMBALL`.

| Marker | Use |
|--------|-----|
| `FUNCTION` | Compared implementation |
| `STUB` | Placeholder (not accuracy-compared) |
| `GLOBAL` | Global/static data |
| `VTABLE` | On class in `.h` |
| `TEMPLATE` / `SYNTHETIC` / `LIBRARY` | As named |
| `FOLDED` | Shared addr (`/OPT:ICF`); after address |

Promote `STUB` → `FUNCTION` when implementing. Never leave real bodies as `STUB`.

Headers ([recommendations](https://github.com/isledecomp/reccmp/blob/master/docs/recommendations.md)):

```cpp
// SIZE 0x54
// VTABLE: LEMBALL 0x0049a478
class Bucket : public Critical {
public:
    virtual void Destroy() override; // vtable+0x18
private:
    unsigned int m_blockSize; // 0x24
    undefined4 m_unk0x30;     // 0x30
};
```

Full annotation syntax: [reccmp annotations](https://github.com/isledecomp/reccmp/blob/master/docs/annotations.md).

## Implement loop

1. Locate — roadmap, Ghidra, or reccmp.
2. Context — 68K, header offsets, callers/callees.
3. Body — MSVC 4.0 shape (`/O2 /Ob1 /Oy /G4`). No modern C++ that changes codegen.
4. Annotate — keep 68K; `FUNCTION: LEMBALL 0x…`.
5. Grind — Commands below (score-only first).
6. Near-miss — `-Diff` / `reccmp-stackcmp` only when stuck.

Do not rename scaffold ids without strong evidence.

## Commands (token-efficient)

Paths from repo root. Prefer wrappers; avoid dumping full logs into chat.

### Grind one function (default)

Incremental build (~2–3s, tiny log) + score line only:

```powershell
.\tools\check_fn.ps1 '0x0045ca30'
.\tools\check_fn.ps1 '0x0045ca30','0x0045cab0'    # multi
.\tools\check_fn.ps1 '0x0045ca30' -Diff           # asm diff when stuck
.\tools\check_fn.ps1 '0x0045ca30' -CleanFirst     # PDB desync only
.\tools\check_fn.ps1 '0x0045ca30' -NoBuild        # reccmp only
```

Quote hex addrs in PowerShell (`'0x…'`). Bare `0x…` becomes an int.

`PDB_DESYNC` / `Failed to find a match` / `Debug data out of sync` → one `-CleanFirst` (or quiet clean), then retry. **Do not clean every iteration.**

### Quiet build

```powershell
.\tools\quiet_build.ps1              # incremental; full log -> build-msvc400/last_build.log
.\tools\quiet_build.ps1 -CleanFirst  # ~80s; stdout = warnings/errors + RESULT only
```

Raw equivalent (noisy; avoid in agent loops):

```powershell
.decomp-venv\Scripts\cmake.exe --fresh --preset msvc400
.decomp-venv\Scripts\cmake.exe --build --preset msvc400
.decomp-venv\Scripts\cmake.exe --build --preset msvc400 --clean-first
```

Out: `build-msvc400/LEMBALL.EXE`, `LEMBALL.pdb`. Flags fixed in `cmake/msvc400-toolchain.cmake` — no casual changes.

### Batch scores (one PDB parse)

```powershell
cd build-msvc400
..\.decomp-venv\Scripts\reccmp-reccmp.exe --target LEMBALL --silent --json-diet --json scores.json
cd ..
.decomp-venv\Scripts\python.exe tools\score_addrs.py 0x0045ca30 0x0045cab0
```

### Session-end / PR verification

```powershell
.decomp-venv\Scripts\python.exe tools\generate_objdiff_report.py
.decomp-venv\Scripts\reccmp-decomplint.exe --target LEMBALL --warnfail src
```

### Extra (from `build-msvc400` after a build)

```powershell
..\.decomp-venv\Scripts\reccmp-stackcmp.exe --target LEMBALL 0xADDRESS
..\.decomp-venv\Scripts\reccmp-roadmap.exe --target LEMBALL --csv roadmap.csv
..\.decomp-venv\Scripts\reccmp-vtable.exe --target LEMBALL
..\.decomp-venv\Scripts\reccmp-datacmp.exe --target LEMBALL
```

## Agent cost rules

- Default grind: `check_fn` score-only. Do not paste full build logs or full verbose diffs.
- Full log: `build-msvc400/last_build.log` — read only on failure.
- Verbose diff / stackcmp: when score flat or structural mismatch.
- Full report: end of session or before commit — not every edit.
- Incremental NMake already rebuilds one `.obj` + link; clean is the expensive path.

## Config

| File | Purpose |
|------|---------|
| `reccmp-project.yml` | Target, source root, reference hash |
| `reccmp-user.yml` | Local paths (gitignored) |
| `build-msvc400/reccmp-build.yml` | Generated binary/PDB paths |

## Do not

- Add symbols without annotations.
- Ship 68K-only logic without x86 check.
- Use C++11+, exceptions, RTTI, or another compiler.
- Reorder annotated funcs except by address.
- Edit `README.md`, hash, or compiler flags casually.
- Commit `data/LEMBALL.EXE` or user reccmp YAML.
