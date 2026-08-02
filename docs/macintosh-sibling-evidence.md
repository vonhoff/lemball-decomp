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

The first high-confidence x86 correction is the complete `CPVGDIBitmap` family. Method order and behavior correlate one-for-one with `/LEMBALL.EXE` `0x00472290..0x00472760`. The twelve reviewed mappings are recorded in `data/macintosh-x86-correlations.csv`. The x86 functions are now named in Ghidra as `CPVGDIBitmap` constructor/destructor, `Free`, `Initialise`, `CreateLinePtrs`, `ResetLinePtrs`, `SetLinePtrs`, `Scroll`, `SetSize`, `SetBitsBase`, `GetRects`, and `ResetScroll` using their recovered CodeWarrior manglings.

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

## Immediate inventory order

1. Preserve data and resource forks for both installed applications.
2. Enumerate resource types, `CODE` resources, Code Fragment Manager `cfrg` entries, PowerPC containers, symbol/debug resources, assertions, paths, and printable strings.
3. Export 68K and PowerPC names without demangling or normalization.
4. Correlate distinctive VSGDI, level, object, loader, and gameplay families against `/LEMBALL.EXE`.
5. Apply reviewed corrections to the Windows Ghidra project and repository together.
