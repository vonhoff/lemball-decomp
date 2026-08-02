# Macintosh module ownership map

| Macintosh CODE module | Recovered families | Windows source owner | Confidence / evidence |
| --- | --- | --- | --- |
| `Visos (Generic)` | `CPVGDIBitmap`, `CPVSurface`, `CPVScrollableSurface`, `CPVBackBuffSurface`, `CPVZBuffSurface`, generic `CSurface` lifecycle | `src/Visos/Generic/CSurface.h` declarations; implementations retained in the existing `CGDI.cpp` TU | High: contiguous MacsBug families, x86 object offsets, vbptr/vtordisp topology, backing widths, lifecycle order |
| `Visos (Mac Specific)` | Macintosh `CSurface::SetLinePtrs`, `CGDIDevice`, `CGDI` platform hooks | Naming evidence only for Windows counterparts | High for `CSurface::SetLinePtrs`; platform ABI is not transferred |
| Windows platform analogue | Win32 GDI, DIB, DirectDraw, and complete surface implementation | `src/Visos/Windows/CGDI.cpp` | High: moved intact from `ENGINE/GDI/RENDER/VSGDI.CPP` at the same CMake/link position |
| `AI` | game objects, players, enemies | Existing game/object files; future module pass | Unmapped in this correction |
| `Control` | input/control | Existing control files; future module pass | Unmapped in this correction |
| `Frontend` | screens, menus, UI | Existing `FRONTEND` files; future module pass | Module name proven; class ownership pending |
| `Map` | map/loading | Existing map/loading files; future module pass | Module name proven; class ownership pending |
| `Network` | network | Existing network files; future module pass | Module name proven; class ownership pending |
| `views/2d` | `C2D` and views | Existing 2D/view files; future module pass | Module name proven; class ownership pending |
| `UtilMain` | utility/runtime glue | Existing core/utility files; future module pass | Module name proven; class ownership pending |

The current correction moves one complete Windows translation unit and extracts declarations only. It does not split emitted bodies across new translation units: VC4 object order and exact-set stability remain authoritative.
