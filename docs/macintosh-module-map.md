# Macintosh module ownership map

| Macintosh CODE module | Recovered families | Windows source owner | Confidence / evidence |
| --- | --- | --- | --- |
| `Visos (Generic)` | `CPVGDIBitmap`, `CPVSurface`, `CPVScrollableSurface`, `CPVBackBuffSurface`, `CPVZBuffSurface`, generic `CSurface` lifecycle | `src/Visos/Generic/CSurface.h`, `CPVSurface.cpp`, and `CSurface.cpp`; remaining platform bodies stay in the existing `CGDI.cpp` TU | High: contiguous MacsBug families, x86 object offsets, vbptr/vtordisp topology, backing widths, lifecycle order |
| `Visos (Generic)` allocator family | `CArena`, `CRAMArena`, `CMBlock`, `CMRAMBlock`, `CBucket`, `CSmallMemory` | `src/Visos/Generic/Memory.cpp`, `Memory.h` | High: contiguous MacsBug family order, matching diagnostics, allocation/list behavior, Windows sizes and lifecycle |
| `Visos (Generic)` resource family | `CMogloadArena`, `CMogDir`, `CMogRes`, typed `CRes*` hierarchy | `src/Visos/Generic/MogLoad.cpp`, `MogPatch.cpp`, `MogConst.cpp`, `MogBst.cpp`, and associated headers | High: contiguous generic resource families, matching archive tags, diagnostics, cache lifecycle, and loader behavior |
| `Visos (Generic)` math family | `CVSMath`, `CVSPoint`, `CVSRange`, `CVSRect`, `CVSSize` | `src/Visos/Generic/VSMath.cpp`, `VSMath.h` | High: self-contained fixed-point/trigonometry and geometry primitive family with matching Macintosh symbols |
| `Visos (Mac Specific)` | Macintosh `CSurface::SetLinePtrs`, `CGDIDevice`, `CGDI` platform hooks | Naming evidence only for Windows counterparts | High for `CSurface::SetLinePtrs`; platform ABI is not transferred |
| Windows platform analogue | Win32 GDI, DIB, DirectDraw, switching, and 240-line mode implementation | `src/Visos/Windows/CGDI.cpp`, `CGDISwitch.cpp`, `CGDI240.cpp` | High: three Visos TUs moved intact at their existing CMake/link positions |
| `AI` | `CDoor`, `CDoorManager`, `CIce`, `CIceManager`; remaining game objects, players, enemies | `src/AI/CDoorAct.cpp`, `CDoorSlot.cpp`, `CDoorCtor.cpp`, `CIceAct.cpp`, `CInvsMgr.cpp`, `CInvsSupp.cpp`; mixed Windows owners retained elsewhere | High for Door and Ice correlations; `CInvsMgr.cpp` and `CInvsSupp.cpp` are coherent invisible-switch manager/object physical fragments, with its function label retained pending call-shape-safe correlation; `LINKSCF.CPP` remains a 296-function mixed Windows owner |
| `Control` | `CGame`, `CGameStatus`, `CLevelLoader`, game lifecycle and control helpers | `src/Control/CGame.cpp`, `CGame.h`, `CGameHelp.cpp`, `CGameVT.cpp` | High for `CGame` and helper ownership; the mixed compiler-vtable TU remains physically intact |
| `Frontend` | screens, menus, UI; recovered `CEditString` family | `src/Frontend`; `CEditString.cpp` is class-owned, remaining TUs retain their Windows boundaries pending per-file audits | High for `CEditString`; module proven, other class ownership remains function-specific |
| `Map` | map/loading | Existing map/loading files; future module pass | Module name proven; class ownership pending |
| `Network` | network | Existing network files; future module pass | Module name proven; class ownership pending |
| `views/2d` | `C2D` group navigation and cancel-move behavior | `src/views/2d/C2D.cpp` | High: matching Macintosh `C2D` behavior and lifecycle |
| `UtilMain` | utility/runtime glue | Existing core/utility files; future module pass | Module name proven; class ownership pending |

Each migration moves complete Windows translation units without splitting emitted bodies. Logical Macintosh ownership remains separate where a Windows TU mixes classes or compiler artifacts; VC4 object order and exact-set stability remain authoritative after every move.
