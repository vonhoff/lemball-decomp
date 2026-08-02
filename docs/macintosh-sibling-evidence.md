# Japanese Macintosh sibling-build evidence

## Acquisition

| Artifact | Source | SHA-1 | Notes |
| --- | --- | --- | --- |
| `Lemmings_Paintball_Japan.bin` | [Internet Archive: `lemmings-pinball-japan`](https://archive.org/details/lemmings-pinball-japan) | `e444018dd6c5ad68d34c3200ef44618b86e118ac` | Redump; 17,716 Mode-1/2352 sectors. |
| `lemmings_paintball_mac.zip` | [Macintosh Repository download 42127](https://www.macintoshrepository.org/40779-lemmings-paintball) | `8447b5ce17eac60259e3947f89789c2fef45d0c7` | Contains the same 36,282,368-byte HFS image. |
| `Lemmings-Paintball.sit` | Macintosh Repository download 72816 | `2f504add09f7165f3e856a1f8512f9cfb0df6778` | Preinstalled PPC application and data. |
| 68K application resource fork | Losslessly extracted from the CD installer | `f92ea0748934c3c64b8dc896fba91adf607141ed` | Contains eleven named `CODE` modules and MacsBug symbols. |
| PPC application data fork | Extracted from the CD installer | `b77a86d61640fe208405b9bdd6d1a4397d349055` | Byte-identical to the preinstalled archive's application data fork. |

The HFS volume is an installer disc. Its package metadata names separate `Lemmings Paintball (68K)` and `Lemmings Paintball (PPC)` applications plus the shared `pbaimog.vsr` game-data archive.

The installer stores target forks as consecutive raw-deflate streams after its PPC code fragment. The 68K target has no data fork; its complete application is a 662,446-byte resource fork. Its code is grouped into `Visos (Mac Specific)`, `Visos (Generic)`, `AI`, `Control`, `Frontend`, `Map`, `Network`, `views\\2d`, and `UtilMain`, plus runtime-library modules.

## Recovered source symbols

`data/macintosh-68k-symbols.csv` contains **2,849 unique CodeWarrior MacsBug function symbols** recovered directly from the 68K `CODE` resources. It records the owning module, resource-relative symbol offset, MacsBug attribute bit, and original CodeWarrior mangled spelling.

The inventory immediately confirms original shared-source types including `CPVGDIBitmap`, `CSurface`, `CGDIDevice`, `CSmallMemory`, `CBucket`, `CArena`, `CGameObject`, `CPlayerLemming`, `CEnemy`, `CMap`, and `C2D`.

The VSGDI inheritance names are also corroborated by x86 layout and behavior: the compact virtual base is `CPVSurface`; the primary `CPVGDIBitmap` branch is `CPVScrollableSurface`; the byte-backed group at `+0x98` is `CPVBackBuffSurface`; the 16-bit group at `+0x44` is `CPVZBuffSurface`; and the complete `0x5A0` target is `CSurface`. These names describe shared source structure only; the Windows offsets, vtables, vtordisps, thunks, and calling conventions remain derived from `/LEMBALL.EXE` and VC4.00.

The first high-confidence x86 correction is the complete `CPVGDIBitmap` family. Method order and behavior correlate one-for-one with `/LEMBALL.EXE` `0x00472290..0x00472760`. The reviewed mappings are recorded in `data/macintosh-x86-correlations.csv`, including the `CPVSurface`, back-buffer, z-buffer, and `CSurface::SetLinePtrs` extensions. The x86 functions are named in Ghidra using their recovered CodeWarrior manglings.

The ordered C2D lifecycle and input-selection family at `/LEMBALL.EXE` `0x004363C0..0x00438210` now contributes 36 additional high-confidence correlations. Function bodies confirm remap lifecycle, grouping event payloads, object-id storage at `+0xA48..+0xA50`, screen-to-game conversion, pause state, cursor messaging, and button dispatch; Windows physical owners and ABI remain unchanged.

The Windows implementation translation unit now lives at `src/Visos/Windows/CGDI.cpp`, retaining its original CMake/link position. Shared family skeletons live at `src/Visos/Generic/CSurface.h`; this mirrors the sibling `Visos (Generic)` versus platform-specific module split without transferring Macintosh ABI details.

Analysis copies live outside the repository under `C:\Users\simon\lemball-mac-source`; build products must not depend on them.

## Evidence policy

Macintosh symbols are potentially original source names and therefore outrank invented semantic names after cross-platform correlation. They do **not** establish the Windows ABI. Each proposed correction must record:

- exact Macintosh spelling and mangling;
- architecture and containing code/resource fragment;
- Macintosh address or resource identity;
- correlated `/LEMBALL.EXE` address;
- behavior, constants, strings, callers, and table-position evidence;
- whether the function is shared game code or platform-specific;
- confidence.

Only high-confidence correlations should rename Windows source, Ghidra symbols, and reccmp metadata. Keep aliases for uncertain or platform-specific names rather than forcing a false match.

## Objdiff ownership reporting

`tools/generate_objdiff_report.py` derives logical units and recovered function labels from the accepted correlation table rather than inventing units from physical Windows filenames. Correlated functions are grouped as `Macintosh module/class`, display the Macintosh method name, and retain the exact mangled symbol in metadata. Uncorrelated Windows and compiler-generated functions remain in the single `Windows/MSVC` layer. Each function also retains its Windows name and physical `source_path`, so logical diagrams stay clean without claiming unsupported source ownership or changing translation-unit order.

## Immediate inventory order

1. Preserve data and resource forks for both installed applications.
2. Enumerate resource types, `CODE` resources, Code Fragment Manager `cfrg` entries, PowerPC containers, symbol/debug resources, assertions, paths, and printable strings.
3. Export 68K and PowerPC names without demangling or normalization.
4. Correlate distinctive VSGDI, level, object, loader, and gameplay families against `/LEMBALL.EXE`.
5. Apply reviewed corrections to the Windows Ghidra project and repository together.

### CAI portable core

Forty-one Macintosh `CAI` methods now align with Windows `0x00410C10..0x00413390` through lifecycle, signatures, ILTs/callers, manager offsets, constants, and behavior. The accepted family covers construction/restart/real teardown/start, state networking, frame/view service, collision and interaction dispatch, load/fixup, ids, network profiles, history, and score while retaining all mixed Windows TUs. `OnLoaded`, `AddData`, `GetData()`, `LemmingsSFXChanged`, `LevelName`, and `NLemmings` remain unresolved rather than consuming order-only or semantically mismatched targets.

### CEnemy lifecycle, behavior, and fire core

Twenty-five Macintosh `CEnemy` methods align with Windows `0x0041FBA0..0x00420720` through ENMY layout, signatures, ILTs/callers, vtable slots, rule/action semantics, distinctive delays, radius 50, directional constants, projectile behavior, and hit lifecycle. The clipped Macintosh `ype` symbol remains unresolved because its damaged spelling/signature cannot support ABI-safe acceptance by family order.

### CPlayerLemming lifecycle, grouping, and fire core

All thirty-nine portable Macintosh `CPlayerLemming` methods align with Windows `0x0040ECB0..0x004102D0` through PLAS lifecycle/layout, signatures, ILTs/callers, vtable slots, stream inverse pairs, group/object fields, distinctive balloon codes, fixed-point coordinates, RNG constants, and state transitions. Target-only `0x0040F7E0`, deleting wrapper `0x00410B20`, and nine Macintosh-specific duplicate members remain explicitly excluded.

### CGenericGroup portable family

All twenty-six portable Macintosh `CGenericGroup` methods align with Windows `0x0041DDA0..0x0041E640` through managed-group lifecycle/layout, vtable slots, iterator and membership inverse pairs, bounds sentinels, formation geometry, waypoint dispatch, intersection behavior, and render-record stride. Target-only `0x0041DF90` and deleting wrappers remain excluded; physical Windows TUs stay intact.

### CPlayerLemmingGroupManager lifecycle core

All twenty-six portable Macintosh `CPlayerLemmingGroupManager` methods align with Windows `0x00418400..0x00419490` through PLAS manager/stream/group vtables, exact this-adjustment, five-child allocation, dead-history and selection fields, signatures, inverse operations, loader formats, callers, and render stride. Five target-only interleavings and deleting wrapper `0x00419930` remain explicitly excluded.

### CGenericGroupManager lifecycle and iterator core

Fifteen Macintosh lifecycle and group/nested-element iterator methods align with Windows `0x0041E8F0..0x0041EB90` through the 40-slot manager layout, vtable/ILT calls, exact count/index fields, nested iterator inverse pairs, signatures, and family sequence. Target-only child-destruction helper `0x0041E9A0` and the later mutation/render tail remain separate.
