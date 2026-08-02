# Macintosh module ownership map

| Macintosh CODE module | Recovered families | Windows source owner | Confidence / evidence |
| --- | --- | --- | --- |
| `Visos (Generic)` | `CPVGDIBitmap`, `CPVSurface`, `CPVScrollableSurface`, `CPVBackBuffSurface`, `CPVZBuffSurface`, generic `CSurface` lifecycle | `src/Visos/Generic/CSurface.h`, `CPVSurface.cpp`, and `CSurface.cpp`; remaining platform bodies stay in the existing `CGDI.cpp` TU | High: contiguous MacsBug families, x86 object offsets, vbptr/vtordisp topology, backing widths, lifecycle order |
| `Visos (Generic)` allocator family | `CArena`, `CRAMArena`, `CMBlock`, `CMRAMBlock`, `CBucket`, `CSmallMemory` | `src/Visos/Generic/Memory.cpp`, `Memory.h` | High: contiguous MacsBug family order, matching diagnostics, allocation/list behavior, Windows sizes and lifecycle |
| `Visos (Generic)` resource family | `CMogloadArena`, `CMogDir`, `CMogRes`, typed `CRes*` hierarchy | `src/Visos/Generic/MogLoad.cpp`, `MogPatch.cpp`, `MogConst.cpp`, `MogBst.cpp`, and associated headers | High: contiguous generic resource families, matching archive tags, diagnostics, cache lifecycle, and loader behavior |
| `Visos (Mac Specific)` | Macintosh `CSurface::SetLinePtrs`, `CGDIDevice`, `CGDI` platform hooks | Naming evidence only for Windows counterparts | High for `CSurface::SetLinePtrs`; platform ABI is not transferred |
| Windows platform analogue | Win32 GDI, DIB, DirectDraw, switching, and 240-line mode implementation | `src/Visos/Windows/CGDI.cpp`, `CGDISwitch.cpp`, `CGDI240.cpp` | High: three Visos TUs moved intact at their existing CMake/link positions |
| `AI` | game objects, players, enemies | Existing game/object files; future module pass | Unmapped in this correction |
| `Control` | input/control | Existing control files; future module pass | Unmapped in this correction |
| `Frontend` | screens, menus, UI | Existing `FRONTEND` files; future module pass | Module name proven; class ownership pending |
| `Map` | map/loading | Existing map/loading files; future module pass | Module name proven; class ownership pending |
| `Network` | network | Existing network files; future module pass | Module name proven; class ownership pending |
| `views/2d` | `C2D` and views | Existing 2D/view files; future module pass | Module name proven; class ownership pending |
| `UtilMain` | utility/runtime glue | Existing core/utility files; future module pass | Module name proven; class ownership pending |

The Visos migration moves three complete Windows translation units and extracts declarations only. It does not split emitted bodies across new translation units. Whole-project migration follows this same module-family sequence; VC4 object order and exact-set stability remain authoritative after every move.
