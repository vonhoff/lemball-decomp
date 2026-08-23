# Agents

This repository reconstructs *Lemmings Paintball* (1996 Windows, `LEMBALL.EXE`) as readable C++ that matches the original binary. The goal is byte-accurate compiler output where practical, verified by [reccmp](https://github.com/isledecomp/reccmp).

AI output is a hypothesis. Every change must be validated against the original executable, PDB symbols, and reccmp comparison results.

## Constraints

- **Never edit `README.md` unless requested.**
- Reconstruct from the original binary (`data/LEMBALL.EXE`) using reccmp as the source of truth — not the 68K reference comments alone.
- Do not invent functions, globals, or addresses. Every symbol must have a reccmp annotation.
- Preserve scaffold layout: one primary class per `.h` / `.cpp` pair under `src/`.
- Functions in a compilation unit must be ordered by their annotated address (ascending).
- Match existing naming, types, and style in the surrounding file.

## Prerequisites

1. **Original binary** — place a compatible `LEMBALL.EXE` in `data/`. The reference SHA-256 is in `reccmp-project.yml`. The game is not distributed with this repo.
2. **MSVC 4.00** — portable toolchain at `msvc400/` (see CI: `vonhoff/MSVC400`). Set `MSVC400_ROOT` if it lives elsewhere.
3. **Python venv** — from the repo root:
   ```powershell
   python -m venv .decomp-venv
   .decomp-venv\Scripts\pip install -r requirements.txt
   ```

## Source layout

`src/` is scaffolded from recovered type and symbol information. Top-level areas:

| Directory | Role |
|-----------|------|
| `AI/` | Game objects, navigation, groups, managers, messages |
| `Control/` | Game loop, level loading, demo, game state |
| `Frontend/` | Menus, drawers, frontend processes and windows |
| `Map/` | Map and ground data |
| `Network/` | Multiplayer game logic and messages |
| `Platform/` | OS entry points (`WinMain`, etc.) |
| `Views/` | In-game UI panels, display, sound view |
| `Visos/` | Engine: foundation, graphics, sound, network, resources, animation, messaging, target (Win32/DirectX wrappers) |

Shared typedefs and forward declarations live in `src/Common.h`. Platform-specific types use `undefined`, `undefined2`, and `undefined4` for fields whose signedness or purpose is not yet confirmed.

## 68K source mapping

Each scaffolded function carries a **68K** comment — the address and mangled name from the original cross-platform Motorola 68000 source tree:

```cpp
// 68K 0x1021749c __ct__7CBucketFiiPUcPUl
// FUNCTION: LEMBALL 0x00472ce0
Bucket::Bucket(int p_blockSize, int p_blockCount, unsigned char* p_memory, unsigned long* p_map)
```

Use 68K comments for **naming, class structure, and behavioral intent**. The Windows x86 binary is what reccmp compares against. When 68K and MSVC output diverge, follow the binary.

Do not remove 68K comments when implementing a function.

## reccmp annotations

reccmp matches annotated symbols in source to addresses in the original and recompiled binaries. Full syntax: [reccmp annotations](https://github.com/isledecomp/reccmp/blob/master/docs/annotations.md).

### Required markers

Every function and global must be annotated. Place the reccmp marker **directly above** the implementation (or stub body). Keep 68K comments above the reccmp marker when both are present.

| Marker | When to use |
|--------|-------------|
| `FUNCTION` | Implemented function with a body that should be compared |
| `STUB` | Empty or placeholder body — not compared for accuracy |
| `GLOBAL` | Global or static data variable |
| `VTABLE` | Virtual table address on the class declaration (in `.h`) |
| `TEMPLATE` | Templated function (annotate by name in a following comment) |
| `SYNTHETIC` | Compiler-generated function (destructors, etc.) |
| `LIBRARY` | Third-party / CRT function |
| `FOLDED` | Multiple symbols share one address (`/OPT:ICF`); add after the address |

Format:

```cpp
// FUNCTION: LEMBALL 0x00472ce0
// STUB: LEMBALL 0x0040a6f0
// GLOBAL: LEMBALL 0x004a2a68
// VTABLE: LEMBALL 0x0049a478
```

Target name is always `LEMBALL` (see `reccmp-project.yml`).

When promoting a stub to a real implementation, change `STUB` → `FUNCTION`. Do not leave implemented code marked `STUB`.

### Header conventions

Headers follow reccmp best practices ([recommendations](https://github.com/isledecomp/reccmp/blob/master/docs/recommendations.md)):

```cpp
// SIZE 0x54
// VTABLE: LEMBALL 0x0049a478
class Bucket : public Critical {
public:
    virtual void Destroy() override;  // vtable+0x18
private:
    unsigned int m_blockSize;         // 0x24
    undefined4 m_unk0x30;             // 0x30
};
```

- `SIZE` — total class/struct size in bytes.
- `VTABLE` — vtable address in the original binary.
- Member comments — offset from object base (`0x08`, `0x24`, …).
- `vtable+N` — virtual method slot offset.
- `undefined` / `undefined2` / `undefined4` — unknown scalar fields; prefer these over guessing `int` vs `unsigned int`.

## Implementing a function

1. **Locate the symbol** — roadmap, Ghidra, or `reccmp-reccmp --verbose 0xADDRESS`.
2. **Read context** — 68K comment, class header offsets, callers/callees.
3. **Write the body** — match MSVC 4.0 codegen (`/O2 /Ob1 /Oy /G4`). Avoid modern C++ that changes calling conventions or codegen.
4. **Annotate** — `// FUNCTION: LEMBALL 0x…` above the definition; keep the 68K line.
5. **Build and compare** — see Commands below.
6. **Iterate** — use verbose reccmp diff and `reccmp-stackcmp` for near-matches.

Prefer minimal diffs. Do not refactor unrelated code. Do not rename scaffolded identifiers without strong evidence.

## Commands

All paths assume repository root unless noted.

### Build

```powershell
.decomp-venv\Scripts\cmake.exe --fresh --preset msvc400
.decomp-venv\Scripts\cmake.exe --build --preset msvc400 --clean-first
```

Output: `build-msvc400/LEMBALL.EXE` and `build-msvc400/LEMBALL.pdb`.

Compiler flags are fixed in `cmake/msvc400-toolchain.cmake` (`/O2 /Ob1 /Oy /G4 /Z7`, no RTTI, `bool` as `int`). Do not change them without explicit approval.

### Canonical verification

```powershell
.decomp-venv\Scripts\python.exe tools\generate_objdiff_report.py
```

Runs `reccmp-project detect`, `reccmp-reccmp`, and `reccmp-roadmap`, then writes `build-msvc400/report.json`. Prints matched-function counts.

### Focused diagnosis (from `build-msvc400`)

```powershell
..\.decomp-venv\Scripts\reccmp-reccmp.exe --target LEMBALL --verbose 0xADDRESS --print-rec-addr
```

Other useful tools (run from `build-msvc400` after a build):

```powershell
..\.decomp-venv\Scripts\reccmp-roadmap.exe --target LEMBALL --csv roadmap.csv
..\.decomp-venv\Scripts\reccmp-decomplint.exe --target LEMBALL src
..\.decomp-venv\Scripts\reccmp-vtable.exe --target LEMBALL
..\.decomp-venv\Scripts\reccmp-datacmp.exe --target LEMBALL
..\.decomp-venv\Scripts\reccmp-stackcmp.exe --target LEMBALL 0xADDRESS
```

### Annotation lint

Before committing annotation changes:

```powershell
.decomp-venv\Scripts\reccmp-decomplint.exe --target LEMBALL --warnfail src
```

## Config files

| File | Purpose |
|------|---------|
| `reccmp-project.yml` | Target definition (`LEMBALL`), source root, reference hash |
| `reccmp-user.yml` | User-local paths (gitignored) |
| `build-msvc400/reccmp-build.yml` | Generated at configure — paths to recompiled binary and PDB |

`CMakeLists.txt` auto-generates `reccmp-build.yml` in the build directory.

## What not to do

- Add functions or globals without `FUNCTION` / `GLOBAL` (or appropriate) annotations.
- Implement from 68K logic alone without checking the x86 disassembly.
- Use C++11+ features, exceptions, RTTI, or a different compiler.
- Reorder annotated functions within a `.cpp` file by anything other than address.
- Edit `README.md`, `reccmp-project.yml` hash, or compiler flags casually.
- Commit `data/LEMBALL.EXE` or user-specific reccmp YAML files.
