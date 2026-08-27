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
 .decomp-venv\Scripts\Activate.ps1   # or prepend .decomp-venv\Scripts to PATH
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

## Commands

Paths from repo root. Prefer wrappers; avoid dumping full logs into chat.

### Grind functions (default)

Incremental build (~2–3s, tiny log) + score line only (using objdiff report 100% match rules):

```powershell
python tools/check.py 0x0045ca30
python tools/check.py 0x0045ca30 0x0045cab0     # multi
python tools/check.py 0x0045ca30 --diff        # asm diff when stuck
python tools/check.py 0x0045ca30 --clean-first # PDB desync only
python tools/check.py 0x0045ca30 --no-build    # reccmp only
```

`PDB_DESYNC` / `Failed to find a match` / `Debug data out of sync` → one `--clean-first` (or quiet clean), then retry. **Do not clean every iteration.**

### Quiet build

```powershell
python tools/build.py               # incremental (auto-cleans pdb/ilk/exe before link); full log -> build-msvc400/last_build.log
python tools/build.py --clean-first # ~80s full rebuild; stdout = warnings/errors + RESULT only
```

Raw equivalent (noisy; avoid if possible):

```powershell
cmake --fresh --preset msvc400
cmake --build --preset msvc400
cmake --build --preset msvc400 --clean-first
```

Out: `build-msvc400/LEMBALL.EXE`, `LEMBALL.pdb`. Flags fixed in `cmake/msvc400-toolchain.cmake` — no casual changes.

### Session-end / PR verification

```powershell
python tools/report.py
reccmp-decomplint --target LEMBALL --warnfail src
python tools/smell.py
python tools/smell.py --annot # 68K-only functions missing FUNCTION/STUB
```

### Extra (from `build-msvc400` after a build)

```powershell
reccmp-stackcmp --target LEMBALL 0xADDRESS
reccmp-roadmap --target LEMBALL --csv roadmap.csv
reccmp-vtable --target LEMBALL
reccmp-datacmp --target LEMBALL
```

### Next targets (function-count batches)

Needs `build-msvc400/report.json` (from `python tools/report.py`). Ranks work that raises 100% function count: tiny STUBs, clone groups, near-miss FUNCTION grind, units with few leftovers.

```powershell
python tools/targets.py
python tools/targets.py --refresh
python tools/targets.py --kind tiny --max-size 5
python tools/targets.py --kind near
python tools/targets.py --kind unit
python tools/targets.py --kind clone
python tools/targets.py --kind tiny --addrs
```

## Agent cost rules

- Default grind: `tools\check.py` score-only. Do not paste full build logs or full verbose diffs.
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
- Offset-poke complete objects or walk vbptr (`*(int*)(*(int*)(p + 0x40) + 4)`). Use named members.
- Invent file-static helpers or unannotated functions.
- Ship 68K-only logic without x86 check.
- Use C++11+, exceptions, RTTI, or another compiler.
- Reorder annotated funcs except by address.
- Edit `README.md`, hash, or compiler flags casually.
- Commit `data/LEMBALL.EXE` or user reccmp YAML.
