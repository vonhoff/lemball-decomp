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

`tools/generate_objdiff_report.py` derives logical units and recovered function labels from the accepted correlation table rather than inventing units from physical Windows filenames. Correlated functions are grouped as `Macintosh module/class`, display the Macintosh method name, and retain the exact mangled symbol in metadata. Explicit entries from `data/reccmp-compiler-generated.csv` are grouped as `Windows/Compiler-generated`; every other uncorrelated x86 function is grouped as `Windows/Uncorrelated`. The latter is an ownership state, not a claim that the function is MSVC-specific or unreconstructed. Logical ownership never changes Windows TU boundaries.

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

All twenty-five portable Macintosh `CGenericGroupManager` methods align with Windows `0x0041E8F0..0x0041EED0` through the 40-slot manager layout, vtable/ILT calls, exact count/index fields, nested iterator and mutation inverse pairs, signatures, allocation sizes, bounding-box/view-data strides, and intersection dispatch. Target-only child-destruction helper `0x0041E9A0` remains explicitly excluded.
### CPlayerLemmingGroup portable core

All fifteen Macintosh lifecycle, view-data, child mutation, use-object, player-control, dead-child, waypoint, and SFX methods align with Windows `0x00414010..0x004148F0` through the player-group vtable, exact signatures, fields `0x160..0x170`, paired child operations, command semantics, ILTs, and callers. Target-only range helper `0x004140D0`, current-dead iterator `0x004148C0`, and child-dirty walker `0x004149A0` remain excluded. `HasSFXChanged` is `0x00414960`: the already-evidenced manager method at `0x004193F0` calls its ILT directly. The source-identifier refactor is tracked with a temporary fidelity debt: exact accounting changed from `169/21` to `166/21` and implemented functions from `2803` to `2802`; the protected checkpoint remains unchanged.
### CMover portable family

All sixteen portable Macintosh `CMover` methods align with Windows `0x0042E590..0x0042F140` through exact signatures, mover vtable/ILTs, type `0x34`, the 16x16 footprint, route/timing fields, fixed-point interpolation, rider attachment inverse operations, and manager callers. Target-only rider synchronization `0x0042EAC0` and zero callbacks `0x0042EEC0`/`0x0042EED0` remain excluded.
### CMoverManager portable family

All ten portable Macintosh `CMoverManager` methods align with Windows `0x0042F190..0x0042F680` through exact signatures, EFF type `0x1A`, mover stride `0x1A0`, manager/object vtables and ILTs, live/capacity fields, rendering, slot dispatch, versioned loading, and inverse `CMover` calls. Reset-count `0x0042F2E0`, array removal `0x0042F350`, trigger query `0x0042F590`, noop `0x0042FB90`, and deleting wrapper `0x0042FBA0` remain excluded.
### CMap portable family

All sixteen portable Macintosh `CMap` methods align with Windows `0x004303C0..0x00431030` through exact signatures, the `0x54`-byte map layout, 12-byte terrain cells, walk buffer, moving-platform override, four-view transforms, chunk-loader callers, bounded level-name copy, and cliff derivation. Windows-only mask test `0x004305A0`, in-place screen-to-game transform `0x00430B30`, and variant validator `0x00430EB0` remain excluded. The shared Windows partial-layout views remain in their physical TUs but now use Macintosh-led `CMap` ownership and method terminology wherever their declarations are compatible. The initial unsynchronized probe appeared fidelity-negative; after source names and reccmp metadata were updated together, the canonical result remained `166/21` exact with datacmp `0`.
### CGround portable family

The three portable `CGround` methods and `CGroundArray::Clear` align with Windows `0x0042FFE0..0x00430370` through exact signatures, the 12-byte terrain-cell layout, type-specific height/mask tables, collision flags at `+6`, and two-dimensional array clearing to type `0x209`. This completes the portable Map module.
### CNode portable family

All six portable `CNode` and six portable `CNodeManager` methods align with Windows `0x00421180..0x00421470` through exact ABI, 0x14-byte node stride, counted allocation, fixed-point coordinates, neighbour-pair storage, ILTs, callers, and NODE chunk parsing. Windows-only tile-coordinate overloads and record helpers remain excluded. Macintosh-led source class/method naming retains mixed-TU aliases and currently carries measured exact-set debt of `161/24` versus the protected baseline; vtables remain `39/39` and datacmp remains `0`.
### CBallManager portable family

All seven portable `CBallManager` methods align with Windows `0x00421EA0..0x00422060` through exact lifecycle, active/capacity fields, pointer-array allocation, CBall vtable dispatch, active-prefix compaction, and BALL chunk loading. Windows-only load-cursor reset `0x00422050` and CBall deleting wrapper `0x00422350` remain excluded. Macintosh-led source ownership improved the tracked exact set to `161/23`; vtables remain `39/39` and datacmp remains `0`.
### CBall portable family

All ten portable `CBall` methods align with Windows `0x00421660..0x00421DA0` through the 0x13c Windows layout, managed-entity base/vtable, fixed-point endpoints, segment timing, terrain sampling, collision dispatch, active-list removal, state machine, and BALL payload cursor consumption. Scalar deleting wrapper `0x00422350` remains generated and excluded.
### CCollectable portable families

All four portable `CCollectable` and seven portable `CCollectableManager` methods align with Windows `0x004223C0..0x00422900` through network-stream inheritance, manager capacity/live-count fields, subtype allocation, 0x13c child layout, fixed-point positions, collectable state transitions, and COLL payload parsing. Target-only helpers `0x00422510` and `0x00422710` plus deleting wrappers remain excluded.
### CBullet portable families

All ten portable `CBullet` and twelve portable `CBulletManager` methods align with Windows `0x00417D80..0x00418120` and `0x0041A510..0x0041ACA0` through the 40-object pool, 0x1a4 Windows stride, active-table iteration, fixed-point segment interpolation, terrain/entity collision, view emission, and network serialization/dispatch. Target-only destroy helpers, scalar deleting wrapper `0x0041AF10`, and `-0x138` adjustor thunk remain excluded. Broad `ProjectilePool`/`ProjectileObjectProxy` source renames were reverted because their changed C++ symbol identities regressed the exact set; Macintosh ownership remains recorded without disturbing Windows linker order.
### CFormationManager portable family

All five portable `CFormationManager` methods align with Windows `0x0041A140..0x0041A320` through the 0x104-byte three-block vector table, fixed16.12 initialization, signed sine/cosine rotation of eight vectors, transformed output at `+0xC0`, and wrapped accessor. Windows iterator helpers `0x0041A2E0` and `0x0041A300` remain separate.
### CHand portable families

All eight portable `CHand` and nine portable `CHandManager` methods align with Windows `0x00427AD0..0x004284C0` through level chunk type `0x1F`, 0x144-byte child stride, capture states `0x17..0x1A`, contact geometry, frame timers, EFF-backed manager fields, view emission and versioned loader records. Windows remove helper `0x00427FD0` and deleting wrappers `0x004287D0`/`0x00428800` remain separate.
### CLaser portable families

All ten portable `CLaser` and nine portable `CLaserManager` methods align with Windows `0x00428890..0x00429950` through chunk type `0x2F`, 0x148-byte child stride, directional terrain footprints, target acquisition, states `0x17..0x1A`, EFF-backed manager fields, variable view output and versioned loader records. The `CLaser` constructor, `Restart`, `Initialise`, destructor, `Set`, `CheckHits`, `Process`, `Activate`, `StepOn`, and `GetViewData` now have final ownership in `src/AI/CLaser.cpp`; the lifecycle members remain exact, `Activate` is a 100% effective match, and `Set`, `CheckHits`, `Process`, `StepOn`, and `GetViewData` are behaviorally reconstructed at `56.62%`, `46.73%`, `68.77%`, `97.06%`, and `15.38%`. The process slot preserves the original 16-word vtable, while direct `GetViewData` ownership restores the manager-to-child view path without inventing a virtual slot. `CLaserManager::CLaserManager(CAI*, int)`, `Restart`, `Initialise`, the destructor, `StepOn`, `Process`, `GetViewData`, `Add`, and `LoadLevel` now have final ownership in `src/AI/CLaserManager.cpp`; the constructor, `Restart`, and destructor remain exact, while `Initialise`, `StepOn`, `Process`, `GetViewData`, `Add`, and `LoadLevel` are behaviorally reconstructed at `74.14%`, `86.08%`, `62.07%`, `73.02%`, `96.67%`, and `15.19%` with native array construction, the evidenced `0x148` child stride, owner links, idle-child filtering, variable-count view aggregation, coordinate conversion, versioned level records, and typed manager-vtable slots. Windows remove/count helpers `0x00429470`/`0x004297E0`, no-op callback `0x00429E40`, and deleting wrappers `0x00429EA0`/`0x00429ED0` remain separate.
### CBalloonPost portable family

All seven portable `CBalloonPost` methods align with Windows `0x00429F50..0x0042A4E0` through the four typed posts (`0x28`, `0x2A`, `0x2C`, `0x2E`), active-mask bits, fixed-point coordinates, terrain-height updates, child view dispatch and loader records. Windows activation/deactivation helpers `0x0042A3A0` and `0x0042A460` remain separate; `0x0042A5C0` begins the child `CBalloon` family.

### CTrampoline portable families

All six portable `CTrampoline` and nine portable `CTrampolineManager` methods align with Windows `0x0042A990..0x0042B600` through chunk type `0x22`, a `0x144`-byte child stride, fixed-point placement, states `0x18`/`0x1B`, bounded overlap impulses, EFF stream payload `0x0D`, view records and versioned loader records. Windows activation helpers `0x0042AAF0`/`0x0042B440`, count/remove helpers `0x0042B0B0`/`0x0042B0C0`, no-op `0x0042B9B0`, and deleting wrappers `0x0042BA10`/`0x0042BA40` remain separate.

### CPaintGun portable families

All five portable `CPaintGun` and eight portable `CPaintGunManager` methods align with Windows `0x0042BAD0..0x0042C610` through chunk type `0x21`, a `0x144`-byte child stride, fixed-point placement, timed states and four-direction projectile emission, EFF stream payload `0x14`, view records and versioned loader records. Constant callbacks `0x0042BFC0`/`0x0042BFD0`/`0x0042C4C0`/`0x0042C580`, count/remove helpers `0x0042C140`/`0x0042C150`, no-op `0x0042C9A0`, and deleting wrappers `0x0042C9B0`/`0x0042C9E0` remain separate.

### CIce portable families

All nine portable `CIce` and nine portable `CIceManager` methods align with Windows `0x0042CA70..0x0042DEA0` through chunk type `0x33`, a `0x188`-byte child stride, bounded endpoint geometry, movement of up to ten attached entities, terrain-contact redispatch, EFF stream payload `0x0E`, exact zero-body view behavior and versioned loader records. Windows switch request/dispatch helpers `0x0042D550`/`0x0042DD90`, count/remove helpers `0x0042D900`/`0x0042D910`, auxiliary-copy helper `0x0042DE40`, and deleting wrapper `0x0042E4D0` remain separate.

### CGame portable family

All nine portable `CGame` methods align with Windows `0x00406DF0..0x00407950` through the main-context lifecycle, resource validation and frontend-resource helper, flow-process switching, active-mode processing, view presentation and message-driven run loop. The Macintosh `char*` constructor parameter is the Windows startup path argument recovered from the stack; the physical Windows owner remains `src/Control/CGame.cpp`. Adjacent stream, callback and deleting-wrapper functions remain separate.

### CDemo portable family

All 11 portable `CDemo` methods align with Windows `0x004091B0..0x004096B0` through the `0x58`-byte controller lifecycle, resource/file-backed demo buffering, length-prefixed records, per-frame synthetic input dispatch, playback reset/cleanup and live-input filtering. Physical ownership remains `src/Platform/Windows/Mixed/Level/RUN/DEMO.CPP`; frame-timer helpers and deleting wrappers at `0x004098B0`/`0x004098E0` consume no portable symbol.

### CLevelLoader portable family

All six portable `CLevelLoader` methods align with Windows `0x00408210..0x00408B00`: the `0x8`-byte loader context, tagged chunk dispatch, override-file buffer acquisition, aligned block traversal, preview extraction and skill-specific resource-ID arithmetic. Windows physical ownership stays split between `LINKSCF.CPP` (`LoadLevel`) and `LEVELRUN.CPP` (the other five methods).

### Portable Control globals

Ten portable Control globals map through independent Windows behavior: `VSPreInit` (`0x00406160`), `VSmain` (`0x00406310`), direction and distance helpers (`0x00413E80`, `0x00413F80`), demo lifecycle (`0x00409150`, `0x00409180`), and frame-clock operations (`0x00408080..0x00408190`). Physical ownership remains split across the existing Windows Control and mixed level TUs. `GETWORD` and `GETLONG` are final Macintosh-only big-endian load adapters: Windows consumers directly read native little-endian values at the corresponding call sites, and no standalone x86 equivalent is emitted.

### CInvisibleSwitch and CInvisibleSwitchManager

All 18 portable methods map at `0x00409CA0..0x0040A490`. Windows evidence preserves the `0x2B8` object stride, `0x40` manager layout, two-coordinate bounds setup, `0x18` tracked-object capacity, contact/activation state machine, and versioned level loader. The constructor, destructor, `Restart`, `Initialise`, `Set`, `VerifyObjects`, `AddObject`, `StepOn`, `DoActivate`, `Process`, and `Load` are genuine `CInvisibleSwitch` members in `CInvisibleSwitch.cpp`; all seven manager members are genuine members in `CInvisibleSwitchManager.cpp`. Their Macintosh signatures replace the neutral compatibility views without attribution annotations. Typed member-function slots preserve both original 32-bit vtable layouts. The current ownership checkpoint is `164/21`; manager constructor and `Process` plus the reconstructed object lifecycle and state-reset members are exact, while object `Set`/`Load`, manager `Restart`, `Initialise`, and `LoadLevel` are behaviorally reconstructed at `78.02%`, `90.24%`, `54.55%`, `81.97%`, and `41.38%` respectively with relocation-name or compiler register/scheduling differences. The target-only deleting wrapper reports a semantic relocation-name mismatch, the original constructor ILT at `0x0040349F` relocates with the owning translation unit while the constructor body stays exact, and three unrelated functions remain 100% effective-only matches after compiler-state changes. Aggregate objdiff is `1799/3415`; vtables remain exact and datacmp has zero issues. The point-only object overload `0x00409D20`, manager Add overloads `0x0040A3E0`/`0x0040A440`, and deleting wrapper `0x0040AC60` are target-only and consume no portable symbols.

### CLift and CLiftManager

All 21 portable methods map at `0x00424D00..0x00425FC0`. Windows evidence preserves the `0x190` object stride, endpoint/legacy setup overloads, boundary cliff updates, eight passenger slots, timed motion states, view records, slot switching, and versioned loader while retaining split Windows ownership. `CLift::CLift()`, `CLift::~CLift()`, `CLift::CalculateCliff()`, `CLift::Edit(...)`, both `CLift::Set(...)` overloads, `CLift::Process()`, and `CLift::CheckObjects()` now have final ownership across `src/AI/CLift.cpp` and `src/AI/CLiftRun.cpp`; the lifecycle pair is 100% exact, while `CalculateCliff`, `Edit`, legacy `Set`, endpoint `Set`, `Process`, and `CheckObjects` are behaviorally reconstructed at `97.18%`, `59.92%`, `91.67%`, `24.69%`, `89.62%`, and `34.15%`. The setup path preserves the six-byte coordinates, unique object ID, fixed-point position, activation modes, passenger reset, tile flags/heights, signed tile division, and the evidenced end-X reuse in the second boundary loop. The required target-only array/scalar deletion path delegates through the reconstructed destructor while remaining exact. Serialization helpers `0x004257E0`/`0x00425830`, remove helper `0x00425890`, Add helpers `0x00425C80`/`0x00425CE0`, manager cliff helper `0x00425DC0`, and deleting wrappers `0x004266E0`/`0x00426710` consume no portable symbols.

### CMaze path search

The 12 portable `CMaze` methods and global `Direction` map at `0x00423090..0x004238B0`. Windows evidence preserves the row-allocated 16-bit passability/distance grid, two fixed-stride frontier banks, edge-mask-constrained nine-neighbor relaxation, reference arguments, bounded breadth expansion, direction coding, and turn-only solution output. Physical ownership stays split between `LEVELRUN.CPP` and `LVPATHDIR.CPP`; Macintosh-only `AdvanceBit`, `ChangeAddr`, and `ChangeNextAddr` remain unresolved pending standalone Windows identities.

### CMineManager

All 11 portable manager methods map at `0x00424020..0x00424850`. Windows evidence preserves the `0x150` mine stride, six-byte tile-position cache, delayed chain threshold `< 0x801`, eight-pixel contact box, versioned slot IDs, and split CMine/manager ownership. Remove helper `0x004241A0` and deleting wrappers `0x00424C30`/`0x00424C60` consume no portable symbols.

### CGameObject movement/lift tail

Five `CODE_06_AI.bin` methods have high-confidence Windows identities in the mixed `LEVELVT.CPP` region: `Jump` at `0x00416130`, `Fall` at `0x00416220`, the one- and two-reference `OffLift` forwarders at `0x00416410` and `0x004164F0`, and member `StartSommersault` at `0x00416510`. The movement bodies preserve trajectory, terrain, landing, and lift/contact behavior; the forwarders preserve vtable slots and `RET 4`/`RET 8`; `StartSommersault` preserves the exact RNG/deadline/phase algorithm. `0x00416050` remains a Windows-only move-command queue helper: Macintosh `Blocked` is a distinct flag update and stays unresolved.

### CObjectManager

All 14 portable GMOB manager methods map at `0x0041AF60..0x0041BF00` in physical `LINKSCF.CPP`. Windows evidence anchors the `0x40` manager layout, `CAI +0x158` ownership, pointer-table lifecycle, linked type-`0x11` creation, type-`0x14` switch creation/conversion, active-only lookup/removal, versioned loader, and network command `0x2A`. Windows-only bulk cleanup `0x0041B160`, recursive removal `0x0041B1B0`, integer-coordinate adapter `0x0041B310`, ID/slot helpers `0x0041B8A0..0x0041B990`, spatial queries `0x0041B9F0..0x0041BB10`, and deleting wrapper `0x0041C330` consume no portable symbols.

### CDoor and CDoorManager

The remaining 18 portable methods map at `0x0040D490..0x0040E630`; audit correction places `Unlock` at `0x0040DD00` and the pre-existing `DoActivate` at `0x0040DEC0`, leaving the latch helper at `0x0040DD50` target-only. Windows evidence preserves the `0x14c` stride, vertical/horizontal tile footprints, key and state values, delayed timing, overlap bounds, versioned loader, and split physical TUs. Remove helper `0x0040E140`, tile access helpers `0x0040EB70`/`0x0040EB90`, and deleting wrappers `0x0040EBE0`/`0x0040EC10` consume no portable symbols.

## CAICursor exact 68K and Windows queue-cursor reconstruction

The six Macintosh `CAICursor` bodies map to constructor `0x00414DA0`, destructor `0x00414DE0`, `SetCursorXY` `0x00414E00`, `CheckAndClipCursorBounds` `0x00414E20`, `GetCursorSurfaceCoordinates` `0x00414E60`, and `ProcessMsg` `0x00414E80`. Exact 68K disassembly confirms the same bounds, queue registration, coordinate clamping, outputs, and message dispatch. Windows layout offsets differ; constructor code is physically in `LINKSCF.CPP` and the remaining bodies in `LEVELVT.CPP`. Scalar deleting wrapper `0x00414F00` remains Windows-only.

## CGlobalGameObject action and network-message reconstruction

All twelve portable bodies map at `0x00416D20..0x00417150` in physical `LEVELRUN.CPP`. Exact 68K and Windows evidence agrees on lifecycle state, both `Action` overloads, request/cancel behavior, packet switch `0x23..0x29`, remove/cancel sends, and construction/destruction of nine shared message helpers. `UsableState` at `0x00416D90` returns the pre-clear value; its former void prototype was incorrect. Windows-only `RequestAction(eAction,int)` adapter `0x00416E00` consumes no Macintosh symbol.

## CRocket and CRocketManager reconstruction

All seventeen portable methods map at `0x004267D0..0x00427110`. Exact 68K and Windows evidence preserves type `0x20`, rocket state/timer constants, contact radius `0x20`, target state `0x15`, manager capacity/count semantics, Windows rocket stride `0x144`, view stride `0x4C`, and versioned loading. Physical ownership is mixed: `0x004267D0..0x00426B50` remains in `LINKSCF.CPP`, while `0x00426C00` onward remains in `LEVELVT.CPP`. Count reset `0x00426C20`, compaction helper `0x00426C30`, network serializer `0x004273F0`, and deleting wrappers `0x004274F0`/`0x00427520` are Windows-only.

## CTrapDoor and CTrapDoorManager reconstruction

All eleven portable methods map in the Windows type-`0x18` region at `0x0040C2D0..0x0040CA40`. Exact 68K and Windows evidence preserves state sequence `0x18→0x1F→0x20→0x21→0x22→0x23→0x1E`, timing constants, terrain sampling, eight child slots, Windows object size `0x150`, manager size `0x54`, and versioned network-marker loading. Physical ownership remains split among `LEVELVT.CPP`, `TYPE18RESET.CPP`, and `TYPE18.CPP`. Position/access helpers `0x0040C720`, `0x0040C950`, `0x0040CA10`, clear helper `0x0040CBC0`, and deleting wrappers `0x0040CE50`, `0x0040CEA0`, and `0x0040CED0` are target-only.

## CTower reconstruction

The three portable methods map consecutively at `0x0041CF70`, `0x0041D000`, and `0x0041D010` in physical `LEVELVT.CPP`. Exact 68K and Windows bodies agree on terrain-height refresh, unconditional activation success, and activation position `(x-0x30000,y-0x8000,z)`. Their ILTs occupy one Windows vtable at slots `+0x14`, `+0x18`, and `+0x2C`; neighboring CCrate and GMOB type-`0x14` bodies consume no CTower symbols.

## CBalloon reconstruction

The five portable methods map at `0x0041D600..0x0041D7B0` in physical `LEVELVT.CPP`. Exact 68K and Windows bodies agree on randomized timestamp restart, terrain refresh, network action `0x2A`, state `0x1A`, target permission/callback dispatch, score `50`, and unchanged activation-position output. The output method preserves the Windows hidden-result-pointer ABI; adjacent type-`0x14` action and render helpers consume no CBalloon symbols.

## CCrate reconstruction

All six portable methods map at `0x0041CCA0..0x0041CF10`. Independent exact-body auditing confirms saved-position restart, linked-content release through the GMOB manager, state `0x19/0x1A` processing, timer/action `0x14` behavior, contents-dependent scoring, and subtype-selected activation offsets. The Windows object uses linked object/id fields at `+0x148/+0x14C`; `ActivatePosition` preserves its hidden-result-pointer ABI. Physical ownership remains split between `LVSAVEPOS.CPP` and `LEVELVT.CPP`; neighboring CAmmo/CTower bodies and wrappers consume no CCrate symbols.

## CCatapult reconstruction

All five portable methods map at `0x0041C700..0x0041CA60`. Independent auditing identifies the type-`4` vtable at `0x00494640` and confirms exact state, clock, randomized launch-vector, linked-target, owner-transfer, score-`20`, and activation-offset behavior. Restart is the distinct `0x0041C700` wrapper that clears its 16-bit field after calling shared helper `0x0041C6C0`; that helper and saved-position activation helper `0x0041C670` remain target-only. Physical ownership stays split between `LVSAVEPOS.CPP` and `LEVELVT.CPP`.

## CAmmo reconstruction

All five portable methods map at `0x0041CA90..0x0041CC70` on the coherent Windows vtable at `0x00494268`: slots `5/6/11/65/67` hold Process, Activate, ActivatePosition, Restart, and DoActivate. Independent auditing corrected the preliminary crossed-family hypothesis: `0x0041C700/0x0041C720` belong to CCatapult, while CAmmo uses `0x0041CA90/0x0041CAB0`. Exact bodies agree on the class-specific duration field, states `0x1A/0x1B/0x18`, action `0x11`, target action `0x19`, score `50`, and unchanged hidden-result position output. Physical ownership remains split between `LVSAVEPOS.CPP` and `LEVELVT.CPP`.

## CSlinky reconstruction

All five portable methods map at `0x0040B480..0x0040B760`. Exact 68K and Windows bodies agree on type-`0x35` construction, inclusive fixed-point bounds, terrain-derived setup, random cardinal movement with eight attempts, state `0x18/0x1B` timing, overlap queries, and callbacks. ILTs and the type-`0x35` vtable keep the family coherent; raw-coordinate helpers `0x0040B5B0/0x0040B5F0` remain target-only. Physical ownership stays split between `LEVELRUN.CPP` and `LV35BOUND.CPP`.

## CSlinkyManager reconstruction

All eight portable methods map at `0x0040B8E0..0x0040BE50` in physical `LEVELRUN.CPP`. Exact Macintosh sequencing and Windows bodies agree on owner/capacity construction, `0x150`-byte CSlinky arrays, restart/destruction, bounded add, render-entry iteration, per-object processing, and serialized level loading. Large removal/compaction helper `0x0040BA30` has no portable counterpart and remains target-only.

## CSheepGroupManager reconstruction

All four portable methods map at `0x0041F0B0..0x0041F2E0`. Independent auditing confirms the SHPG manager constructor and `0xB0` layout, nested restart loop, first/next group process iteration, and six-byte record loader through exact 68K bodies, Windows ILTs, CAI callers, and the `SHPG` chunk route. Physical ownership remains split across `LINKSCF.CPP`, `LEVELRUN.CPP`, and `LEVELVT.CPP`. Windows-only four-sheep factory `0x0041F170` and removal helper `0x0041F250` remain excluded.

## CSheep reconstruction

All five portable methods map at `0x0041F990..0x0041FAD0` in physical `LEVELVT.CPP`. Independent auditing confirms type `7`, the coherent `0x124` Windows layout and vtable, restart bounds and owner registration, PLAS state-table processing, and exact score/vector reactions. Scalar deleting wrapper `0x0041FB70` remains target-only; the Macintosh semantic destructor is platform-specific and is not consumed by that wrapper.

## CSheepGroup reconstruction

All five portable methods map at `0x0041F500..0x0041F870` in physical `LEVELVT.CPP`. Independent auditing confirms `0x16C` group construction, the exact octant/LCG runaway algorithm, ordered three-source lemming checks, child processing, and the constant-zero catapult callback through one coherent SHPG vtable and call chain. Scalar deleting wrapper `0x0041F960` remains excluded from the Macintosh semantic destructor.

## CRAMArena reconstruction

All six portable methods map at `0x0045A3F0..0x0045A500` in physical `Memory.cpp`. Exact Macintosh and Windows behavior agrees on CArena construction, `RARN` magic, `0x50` arena and `0x28` block headers, initial free-block linkage, derived teardown, and both placement-construction paths. Base-arena and block helpers remain separately owned.

## CArena partial reconstruction

Three additional portable methods map at `0x0045A180`, `0x0045A1E0`, and `0x0045A230` in physical `Memory.cpp`. Exact behavior confirms recursive owner selection for shrink and `RMBL` magic/free-bit checks. The duplicate CODE_02 constructor, `MemSet`, and `MemCopy` bodies are recorded as merged with the already accepted CODE_09 x86 bodies rather than violating one-address ownership. `CheckIntegrity` remains unresolved: its Macintosh body returns one, but `0x0047FCA0` is shared by many unrelated vtables and lacks a proven CArena caller role.

## CDuplicator reconstruction

All nine portable methods map at `0x004275B0..0x00427910` in physical `LEVELVT.CPP`. Independent auditing confirms type `0x1C`, the `0x144` Windows object, vtable `0x00496050`, exact two-cell footprint operations, state/history duplication flow, fixed-point activation offsets, action `0x0C`, and score `100`. Platform-specific Usage/render count `0x00427A90` and scalar deleting wrapper `0x00427AA0` remain excluded.

## CBucket reconstruction

All nine portable methods map at `0x00472CE0..0x00473140` in physical `Memory.cpp`. Independent auditing confirms the expanded `0x54` Windows layout, exact storage/bitmap ownership, packed free-slot cursor, child-chain lifecycle, allocation/free counters, and half-open pointer checks. The source retains proven helper forms where forced language-level member rewrites were not shown binary-neutral; adjacent `CSmallMemory`, Mac-specific `Mem2Offs`, and lock adapters remain excluded.

## CSmallMemory reconstruction

All four portable methods map at `0x00473180..0x00473340` in physical `Memory.cpp`. Exact Macintosh trailers and independent Windows auditing confirm the literal `CSmallMemory` owner, unchanged `0x3C` seven-class layout, bucket construction/teardown, interval-based allocation, and bucket-chain free dispatch. `CBucketStore` is rejected as nonexistent terminology; adjacent CBucket helpers and Windows lock/free wrappers remain separate.

## CVSStreambuf reconstruction

The portable base constructor and destructor map to `0x004583E0` and `0x00458400` in physical `VSINIT.CPP`. Exact 68K and x86 bodies only install/restore the base vtable; the constructor retains the two unused stack arguments via `RET 8`. The richer fixed-buffer constructor `0x0045AD70` is a derived object, while `0x00458E60` remains a deleting wrapper.

## CMBlock reconstruction

All five portable base methods map at `0x0045A540..0x0045A630` in physical `Memory.cpp`. Independent exact-body auditing confirms the `0x28` Windows header, arena/list fields, stream report slot, no-op description setter, and placement allocator. Pointer helper `0x0045A580`, derived `CMRAMBlock` construction, allocator routines, and return-this destructor wrappers remain separate.

## CMRAMBlock reconstruction

The sole generic portable constructor maps to `0x0045A640` in physical `Memory.cpp`. Exact bodies forward all arguments to `CMBlock`, install the derived vtable, stamp `RMBL`, and expose payload `this+0x28` with size reduced by `0x28`. The Macintosh destructor is Mac-specific; Windows return-this wrapper `0x0045A900` remains excluded.

## CVSOStream reconstruction

All twelve portable stream-format methods map across `0x00458450..0x00458D40` in physical `VSINIT.CPP`. Exact bodies and ABI/caller evidence distinguish `HEX` at `0x00458780`, zero-filled `HEX8` at `0x004589C0`, and four-byte `CHAR4` at `0x00458D40`. Windows-only unsigned-int, other fixed-width hexadecimal/binary adapters, character forwarding, and LEVELVT deleting wrappers remain separate.

## CSoundManager reconstruction

Nineteen portable methods map across `0x0045AF80..0x0045B5F0`; `StreamOut` remains physically in `VSRETURN.CPP`, while the other accepted bodies remain in `VSSOUND.CPP`. Exact ABI, layout, caller, vtable, and backend-slot auditing rejects `0x0045B370`/`0x0045B390` as Windows PauseMusic/ResumeMusic rather than Macintosh `ProcessMusic`, which stays unresolved. Windows-only dispatch, startup-name, report, and global lifecycle helpers remain excluded.

## CVSDebugStreambuf reconstruction

All five portable callback-buffer methods map across `0x0045AD70..0x0045AF20` in physical `VSINIT.CPP`. Exact 68K and x86 layout, callback, vtable, newline, tab, and full-buffer behavior agree. The scalar deleting wrapper at `0x0045AF60` and the larger formatted-composite wrapper at `0x00407E80` plus its ILT remain target-only.

## CBaseQueueHandler reconstruction

The constructor, `ProcessMsg`, and `StreamOut` map to `0x00462EA0`, `0x00462EC0`, and `0x00462ED0` across their mixed physical Windows owners. Vtable evidence proves handler `StreamOut` remains a distinct byte-identical body from `CBaseQueue::StreamOut` at `0x00463280`; neither is merged or double-consumed.

## CBaseQueue reconstruction

All fourteen portable queue methods map across `0x00463020..0x004638A0` while preserving mixed `VSINIT.CPP`, `VSWING.CPP`, and `CGame.cpp` ownership. Exact ABI, `0x14` x86 message records, ring-buffer layout, stable signed handler ordering, callers, and vtables support the family. The undefined resize helper at `0x00463450`, scalar deleting destructor at `0x00463940`, and adjacent packing/sort helpers remain target-only.

## CResANIM reconstruction

All four portable animation-list resource methods map across `0x0045D610..0x0045D720` in physical `VARLOAD.CPP`. Exact load, counted `CResZRLE` entry-array allocation, per-entry unload, force-load, ABI, layout, caller, and derived-vtable evidence supports the family. Inherited base-list and adjacent `CResFONT` lifecycle bodies remain separate.

## CResBaseLIST reconstruction

All six portable list-resource base methods map across `0x0045D290..0x0045D5C0` in physical `VARLOAD.CPP`. Exact header/data arrival, archive loading, force-load and unload loops, layout adaptations, and common slots across four list vtables support the family. Derived resource bodies and deleting wrappers remain separate.

## CMogRes reconstruction

All fifteen portable archive-manager methods map across `0x0045C630..0x0045CF10` in physical `MogLoad.cpp`. Exact archive lifecycle, directory traversal, sparse cache, eviction, allocation, lookup, resource ownership, ranged loading, aging and cleanup behavior supports the family. Interleaved path wrappers and the typed-resource family beginning at `0x0045CF20` remain target-only or separate.

## CResBase reconstruction

Eleven portable resource-base methods map across `0x0045CF20..0x0045D250`, preserving mixed `MogLoad.cpp` and `VARLOAD.CPP` ownership. Exact ABI, expanded layout, vtable slots, archive loading, reference and unload semantics support the mappings; notably, `DoLoad` is `0x0045CF20`, not `CMogRes::Load` at `0x0045CB80`. The Macintosh-only diagnostic `StreamOut` has no emitted Windows counterpart and does not consume an independently owned short stream identity.

## CNetworkMessage reconstruction

All twenty-five portable network-message methods map across `0x0045EE80..0x0045F2B0`, preserving mixed `EFFSTRM.CPP` and `VSNETSTR.CPP` ownership. Exact big-endian append/read/convert behavior, pointer aliases, copy semantics, expanded layout, vtables and send-event path support the family. Deleting wrappers, a duplicate destructor thunk, and the following request-control streams remain target-only or separate.

## Typed resource loader reconstruction

Eight portable `CResPALETTE`, `CResSTRING`, `CResBITMAP`, `CResZRLE`, `CResEFFECT`, and `CResBIN` methods map across `0x0045DD90..0x0045E540`. Exact type tags, expanded layouts, descriptor parsing, callers and vtables support the batch while preserving the non-obvious `VARLOAD.CPP`, `MogLoad.cpp`, and `DEMO.CPP` physical split. Adjacent Windows-only loaders, parsers, type slots, and deleting wrappers remain separate.

## CResMOVIE reconstruction

All seven portable movie/list-resource methods map across `0x0045DE70..0x0045E120` in `VARLOAD.CPP`. Exact LIST-tag loading, paired `CResSTRING`/`CResINT` arrays, overload ABI, callers and vtable slots support the family. The unused Windows default constructor, scalar deleting wrapper, adjacent GAMI family and trivial filler slots remain target-only or separate.

## CResFONT reconstruction

All thirteen portable font-resource methods map across `0x0045D7B0..0x0045DB30`, preserving mixed `VARLOAD.CPP`, `DRAWTEXT.CPP`, and `TXTCNT.CPP` ownership. Exact LIST semantics, paired ZRLE glyph and INT character-code arrays, indexed VRAM operations, vtables, glyph fallback and metric accumulation support the family. The unused default constructor, deleting wrapper, entry-array helpers, and inherited base-list slots remain separate.

## CReadSocket and CWriteSocket reconstruction

All twenty-seven portable socket methods map across `0x0045F820..0x00460280`, preserving mixed `VSNETSTR.CPP`, `VSNET.CPP`, `EFFSTRM.CPP`, and `LEVELRUN.CPP` ownership. Exact composite layouts, vtables, critical/noncritical record tables, fragmentation, acknowledgements, retries, idle close, and adjusted close ABI support the pair. Deleting wrappers, virtual-base thunks, the adjusted channel thunk, shared EFF-stream predecessors, and the following runtime-stack family remain separate.

## CMogDir reconstruction

All eight portable archive-directory methods map across `0x0045BDA0..0x0045C2D0` in `MogLoad.cpp`. Exact record parsing, directory/entry layouts, cursor threading, recursive lookup, callers, and ownership support the family. Archive-arena and `CRawRead` helpers plus Windows-only name-search and integrity routines remain separate.

## CConnect reconstruction

All thirteen portable connection methods map across `0x00460A90..0x00461030`, preserving `VSNETTCP.CPP` construction/destruction and `VSNET.CPP` runtime ownership. Composite layouts, vtables/adjustors, timers, event posting, acknowledgement, send/close state, and callers support the family. `InitConnect` now forwards the peer-key argument as the raw x86 and Macintosh signature require. Deleting/virtual-base wrappers, transport helpers, the following locked-record family, and concrete derived transports remain separate.

## CChangeList reconstruction

All fifteen portable change-map methods map across `0x004669A0..0x00467020` in `CGDI.cpp`. Exact map quantization, aligned item layout, active marker stack, lazy list generation, callers, and ABI support the family. `GetNItem` is `0x00467000` and `GetDrawMark` is `0x00467020`; the Windows-only two-argument accessor at `0x00466FD0` and rectangle initializer at `0x00467040` remain separate.

## CReadPacket and CWritePacket reconstruction

All six portable packet-slot methods map across `0x00461090..0x004611E0` in `VSNET.CPP`. Exact 68K bodies, locked-versus-simple layouts, direct table and deleting-wrapper callers, x86 calling conventions, and payload ownership establish the two families. `CReadPacket` retains the expanded Win32 critical section; `CWritePacket` retains the network-message owner and retry fields. The deleting wrappers and adjacent `CBasePacketBuff`, `CReadPacketBuff`, and `CWritePacketBuff` families remain separate.

## CFontTable reconstruction

All four portable glyph-table methods map across `0x00473650..0x00473730` in `CSurface.cpp`. Exact 68K bodies, the shared 8-byte layout, the 256-entry pointer table, vtable slots, `CResFONT` callers, and teardown order establish construction, indexed lookup, reverse lookup, and destruction. `CResFONT::ASCIItoZRLE` at `0x0045DB20` and `GetSize` at `0x0045DB30` remain distinct published methods; scalar deleting wrapper `0x00473750` remains compiler glue.

## CBaseRemap reconstruction

All four portable palette-remap methods map across `0x0046AA80..0x0046AB70`. Exact 68K bodies and x86 behavior share the 8-byte remap-table/resource layout, palette load and release lifetime, direct-table mode, counted sparse-pair mode, and weighted 2:7:1 grayscale nearest-match algorithm. Windows keeps constructor `0x0046AA80` physically in `VSWINDOW.CPP` and the destructor/builders in `VSINIT.CPP`. The Macintosh destructor's conditional delete tail remains wrapper glue; the following `CBasePalManager` pointer-table family beginning at `0x0046ACD0` remains separate.

## CBasePalManager reconstruction

All four portable palette-manager methods map across `0x0046ACD0..0x0046ADD0` in `VSINIT.CPP`. Exact 68K bodies and x86 behavior share the 12-byte pointer-table/capacity/active-count layout, capacity-sized zeroed allocation, first-hole registration, occupied-slot destruction, and active-count-based hole skipping. Constructor `0x0046ACD0` is a 100% reccmp match and unregistration `0x0046ADD0` is a 100% effective match. The `CBaseRemap` family and placement wrapper `0x00473610` remain separate.

## CAnimsManager reconstruction

All eight portable animation-manager methods map across `0x00467260..0x004678C0`. Exact 68K bodies and x86 behavior share the manager layout, sparse resource maps, ANIM-to-ZRLE fallback, frame-count and frame-extent access, ZRLE/ANIM draw-entry construction, and immediate/double-buffer reset paths. Source terminology now uses `CAnimsManager` and the Macintosh method names while preserving the Windows split: `GetAnimSize` remains physically linked through the `MENUSEL.CPP` object and the other family bodies remain in `LEVELSTAT.CPP`. Target-only maximum-extent `0x004675D0`, frame resolver `0x004676A0`, and unrelated `C2D::ResetPrimitives` `0x00440400` remain separate.

## Animation selector reconstruction

The four portable animation-selector symbols map to `0x00467170`, `0x004671B0`, `0x004671E0`, and `0x00467210`. Exact 68K/x86 timing equations, shared fields, distinct vtable slots, fixed-time overrides, direction reversal, and focused reccmp agree. The two byte-identical `StartAnim` bodies remain separate `CRepeatAnim` and `CPlayThruAnim` slots; the timed selector repeats modulo duration while the play-through selector clamps to the terminal frame.

## CMasterInput reconstruction

The three portable `CMasterInput` symbols map to constructor `0x00472070`, semantic destructor `0x00472090`, and `StreamOut` `0x00472210` in `VSINIT.CPP`. Exact 68K/x86 bodies share the queue pointer, zeroed input count and list head, linked-list teardown, and identity stream slot. The Windows object is `0x18` bytes and the startup runtime is its sole constructor caller. Source terminology now reflects `CMasterInput`; scalar deleting wrapper `0x00472270` remains separate.

## CRawRead reconstruction

The four portable `CRawRead` methods map to `0x0045BBC0`, `0x0045BBE0`, `0x0045BC40`, and `0x0045BD50` in `MogLoad.cpp`. Both architectures use one shared archive handle, one- and four-byte reads, and the same case-folded path-component comparison with `/` terminating an archive name. Windows implements the stateless methods as free functions; source names now preserve the Macintosh logical owner without inventing an object layout or changing the physical TU/ABI. Generic CRT wrappers remain separate.

## CMogloadArena reconstruction

Portable `CMogloadArena::operator new` and `operator delete` map to `0x0045BAF0` and `0x0045BB70`. Exact 68K/x86 behavior falls back to the default allocator when no archive arena exists, tries the small-memory bucket path when enabled, then dispatches allocation/free through the main arena. Both focused reccmp checks are exact. Macintosh terminology replaces provisional archive-memory helper names without changing the Windows free-function ABI or existing TU boundaries.

## Resource-global lifecycle reconstruction

Portable `_RES_Init` and `_RES_Quit` map to `0x0045B900` and `0x0045BA50` in `VSINIT.CPP`. Both architectures construct three resource tag tables with the same capacities and INT/ZRLE/STRG order, create the palette manager, then destroy those resources in reverse lifecycle order. Source terminology now follows the exact Macintosh spellings. Reusing typed local pointers also restores the intended destructor call ABI and raises `_RES_Quit` to a 100% effective reccmp match without consuming startup wrappers or child methods.

## RNAME stream insertion reconstruction

Portable `operator<<(CVSOStream&, RNAME)` maps to `0x0045BAD0` in `DRAWTEXT.CPP`. Both bodies forward the 32-bit `RNAME` payload through the integer stream insertion routine and return the original stream reference. The sole Windows caller reports a font resource identifier. Focused reccmp is 100%; the underlying integer insertion method remains separately owned.

## CResBase StreamOut final disposition

Portable `CResBase::StreamOut` has exact Macintosh body `0x00002C72..0x00002F00` and trailer `0x00002F01`. The 655-byte body formats resource type, identity, load flags, offsets, and sizes through a global diagnostic stream; its explicit stream argument is not consumed. Windows has no corresponding body, no resource-vtable slot, and no direct or indirect formatter candidate. It is therefore finalized as Macintosh-specific with no x86 correlation; nearby identity methods, shared stream helpers, wrappers, and `CResBaseLIST` methods remain independently owned.

## CTextManager reconstruction

All eight portable `CTextManager` symbols map to the Windows manager at `0x00469C60..0x0046A070`: constructor/destructor, `LoadFont`, `GetFont`, `UnLoadFont`, both `DrawString` overloads, and `ResetPrimitives`. Both architectures maintain a font-pointer array plus a short resource-ID remap, reuse pooled draw primitives, apply a nonzero relative point with flag `0x200`, and keep the raw-character and `CString` lifetime paths distinct. The Windows object is `0x24` bytes and physically splits the six primary methods into `DRAWTEXT.CPP` while the `CString` overload and reset helper remain owned by `GDIVT.CPP`. Source terminology now follows `CTextManager` without moving either TU.

# Portable `CBaseCursor` family

The eleven generic Macintosh methods from constructor trailer `0x00005B68` through `Process` trailer `0x00006B7A` map in order to the Windows cursor-client family at `0x0046AEC0..0x0046B810`. Exact 68K bodies, resource replacement, packed-position event handling, fixed-point motion, window-relative drawing, callers, and the Windows cursor render-client vtable establish the family independently of adjacency. The Windows object expands to `0xA4` bytes and keeps implementations split across `CGDI.cpp`, `LEVELSTAT.CPP`, `GDISTATUSVT.CPP`, and `CGame.cpp`.

The two overloads remain distinct: `SetMainID(unsigned long)` selects a ZRLE resource at `0x0046B310`; `SetMainID(unsigned long,int)` selects a LIST resource entry at `0x0046B3B0`. Windows global/fastcall helpers retain their actual ABI even where the Macintosh identity is a member. Macintosh-specific `RefreshPos` and `GetPos` remain platform-specific and consume no Windows functions.

## CHeaderMessage and CHeaders reconstruction

The three portable `CHeaderMessage` symbols map to `0x00479540`, `0x00479580`, and `0x004795D0`; constructor state, marker serialization, two `0x15`-byte fields, and the unchanged-marker `0x2E` skip match exactly. Its Macintosh-specific destructor remains excluded.

The four portable `CHeaders` symbols map to `0x00479620`, `0x004796E0`, `0x00479790`, and `0x004797D0`. Windows widens each Macintosh `0x56`-byte child to `0x60`, while preserving the counted child array, parallel marker table, ordered `AddData`/`GetData` loops, and parent cursor equations. The Windows-only `FindNextChangedSlot` at `0x00479720` remains unconsumed; physical ownership stays in `VSNETFIL.CPP`.

## CPortsMessage reconstruction

Portable `CPortsMessage` construction and `AnyUsed` map to `0x00479810` and `0x00479860`. Both architectures allocate and zero exactly `0x200` state bytes, include that extent in the inherited stream length, and scan the full map for any nonzero byte. Windows expands the inherited stream prefix so the state pointer moves from Macintosh `+0x24` to x86 `+0x2C`; physical ownership remains in `VSNETFIL.CPP`.

## File socket foundations reconstruction

`CFileCommonSocket::CreateSocket` and `SysCloseSocket` map to `0x00479880` and `0x00479900`: both architectures use the same virtual open/create fallback, 100 ms retry, close-on-create success, and close status `6`/`0`. The Windows bodies remain global/helper ABIs in `VSNETFIL.CPP`. `CFileBaseSocket` construction maps independently to the exact zero-word body at `0x00479920`; the adjacent composite constructor at `0x00479930` remains unconsumed.

## CFileReadSocket reconstruction

The four portable `CFileReadSocket` methods map in order to `0x00479930`, `0x00479A40`, `0x00479B30`, and `0x00479C10`. Exact 68K bodies and x86 code preserve embedded channel/read-stream construction, lock/read/unlock behavior, per-slot payload reads, source-address selection, marker/latch comparisons, one-payload service, and the pending-slot cursor. Windows keeps widened embedded objects, `0x60`-byte `CHeaderMessage` slots, hidden constructor state, and physical ownership in `VSNETFIL.CPP`.

## CFileWriteSocket reconstruction

The five portable `CFileWriteSocket` methods map in order to `0x00479E20`, `0x00479F40`, `0x00479FA0`, `0x0047A090`, and `0x0047A0B0`. Constructor, locked stream write, adjusted destination forwarding, and record-slot packet append preserve the Macintosh control flow with widened Windows objects. The x86 destructor core restores final vtables while generated wrappers retain subobject teardown; those wrappers remain unconsumed. Attribution is metadata-only because the preceding source-class rename probe was fidelity-negative.

## CFileRWSocket reconstruction

The portable constructor and `SendPacket` map to `0x0047A220` and `0x0047A420`. Both architectures compose read and write sockets with shared channel/file state; `SendPacket` forwards payload and size to `CFileWriteSocket` and wraps the active slot modulo the header count after success. The adjacent `0x0047A470` marker helper remains unconsumed.

## CFileOpenManagement reconstruction

`IncOpenCount`, `DecOpenCount`, and `SysCloseSocket` map to `0x0047A470`, `0x0047A4D0`, and `0x0047A530`. The first two read the shared header under lock, adjust its 16-bit open count, and write it back while retaining the lock. Close decrements that count, preserves the common close status adaptation, and closes the underlying file only at zero. Physical Windows adapter ownership remains unchanged.

## CFileBroadcast reconstruction

All twelve portable methods map contiguously from constructor `0x0047A570` through `Process` at `0x0047AEF0`. The x86 family preserves global `CPortsMessage` state, a 20-slot `CHeaders` table, open-count lifecycle, file initialization, locked port-table read/write, free-port allocation/reset, listen flags, and 100 ms processing cadence. Windows widened layout, adjusted views, generated destructor wrappers, and `VSNETFIL.CPP` ownership remain intact; the following `0x0047AF30` constructor is excluded.

## CFileConnect reconstruction

All eight portable methods map from constructor `0x0047AF30` through `ConnectSetup` at `0x0047B580`. The Windows composite preserves two three-slot `CHeaders` tables, copied path pairs, file open-count and payload-base setup, listen/connect state transitions, and paired file-range initialization with a 100 ms lock retry. Expanded virtual-base views and generated destructor wrappers remain physically owned by `VSNETFIL.CPP`.

## File-protocol control messages

`CBroadcastMessage::AddHeader/GetHeader` map to `0x0045F360/0x0045F370`, shared by six control-stream vtables. Five derived request/response classes contribute fifteen constructor and `GetData`/`AddData` bodies at `0x0045F3D0..0x0045F670`: request-connect serializes a port, 0x200-byte map and host; new-port appends a dword; OK/GO each serialize a word+dword; FAILED serializes its string. The existing `VSNET.CPP` helpers, widened layouts, and shared vtable references remain physically unchanged.

## Base socket reconstruction

`CBaseCommonSocket` constructor, destructor, `SocketError`, and `CloseSocket` map to `0x0045F680`, `0x0045F6C0`, `0x0045F6E0`, and `0x0045F720`. `CloseSocket` is the standalone state wrapper: it clears active fields, invokes virtual close, and falls back to `SocketError` on `-1`; raw callback `0x00471A60` remains unconsumed. `CBaseSocket` constructor, `AddData`, and `GetData` map to `0x0045F750/0x0045F790/0x0045F7F0`, preserving the two-dword, three-word, one-byte tagged header.

## Broadcast runtime reconstruction

All fourteen `CBroadcast` bodies map in order to `0x00460350..0x00460A50`. The constructor composes adjusted common/read/write socket views and allocates the 0x200-byte port map; the family covers peer close, port claiming, specific-address setup, broadcast payload construction, stop/read dispatch, message loading, one-second request retry, failure events, run/suspend gating, and addressed send. Physical `VSNET.CPP` ownership and generated wrappers remain unchanged.

## Base packet-buffer reconstruction

`CBasePacketBuff` constructor/destructor map to `0x00461210/0x00461250`. The constructor stores count and 16-bit payload size and allocates the pointer array; the destructor invokes each non-null slot's deleting destructor before releasing the array. Derived buffer constructors remain separate.

## Read packet-buffer reconstruction

`CReadPacketBuff` constructor, `FillPacket`, and `UnUseAll` map to `0x00461290/0x004612F0/0x00461310`. The constructor allocates one 0x2c-byte `CReadPacket` per base-table slot, `FillPacket` copies the current global packet into an indexed slot, and `UnUseAll` clears every slot's payload-present flag.

## Write packet-buffer reconstruction

`CWritePacketBuff` constructor and `FillPacket` map to `0x00461340/0x004613A0`. The constructor allocates one 0x18-byte `CWritePacket` per base-table slot; `FillPacket` forwards the caller payload, size, and network-message pointer into the indexed write slot.

## Read multi-segment buffer reconstruction

`CReadMSBuff` constructor, destructor, and `FillPacket` map to `0x004613D0/0x00461440/0x00461470`. The family owns fragment-buffer allocation and assembly while preserving the first 0x10-byte packet header. Its definitions now live in class-aligned `src/Visos/Generic/CReadMSBuff.cpp`, included at the original `VSNET.CPP` position so MSVC emits the same physical translation unit and link order.

## Read non-contiguous buffer reconstruction

`CReadNCBuff` constructor, `UpdatePacket`, and `GetPacket` map to `0x00461560/0x00461580/0x004615F0`. Packet IDs zero through two retain their indices; later IDs shift down by three. `UpdatePacket` copies only fresh sequence data into an unused locked slot, while `GetPacket` is the direct shifted accessor. Definitions live in `src/Visos/Generic/CReadNCBuff.cpp` and remain included at their original `VSNET.CPP` and `EFFSTRM.CPP` positions.

## Read non-contiguous multi-segment buffer reconstruction

`CReadNCMSBuff` constructor, destructor, and `UpdateSubPacket` map to `0x00461610/0x004616B0/0x00461700`. The object owns a packet-ID range of child `CReadMSBuff` fragment assemblers and rejects stale or skipped sequence transitions. Physical definitions remain in `VSNET.CPP`: extracting them into a class-aligned include lost protected exact function `0x00462130`, so the fidelity-negative source move was reverted.

## Read contiguous buffer reconstruction

`CReadCBuff` constructor, `StorePacket`, and `NextPacketReady` map to `0x00461780/0x004617A0/0x00461810`. Incoming sequence numbers select ring slots modulo capacity; gaps are marked unavailable until contiguous consumption catches up. Physical definitions remain in `VSNET.CPP`: the class-aligned extraction lost protected exact function `0x00462130` during the complete regenerated gate sequence and was reverted.

## Read contiguous multi-segment buffer reconstruction

`CReadCMSBuff` constructor, destructor, and `StoreSubPacket` map to `0x00461840/0x004618E0/0x00461930`. The object owns a ring of child `CReadMSBuff` assemblers selected by packet sequence modulo capacity. Physical definitions remain in `VSNET.CPP`, following the demonstrated downstream exact-ownership constraint.

## Write contiguous buffer reconstruction

`CWriteCBuff` constructor, `StorePacket`, and `IsPacketAvailable` map to `0x00461970/0x00461990/0x004619D0`. Packet IDs select write slots modulo capacity, payload/message context is copied only into available slots, and availability checks inspect the subsequent sequence slot. Physical definitions remain in `VSNET.CPP` under the demonstrated downstream exact-ownership constraint.

## Base network lifecycle reconstruction

`CBaseNetwork` constructor, `Initialise`, `DoInitialise`, destructor, and `ShutDown` map to `0x004619F0/0x00461AA0/0x00461BD0/0x00461DB0/0x00461E10`. Together they own queue registration, control-message/global-stream allocation, transport startup readiness, peer/broadcast shutdown, and global resource release. Physical definitions remain in `VSNET.CPP`; the remaining `CBaseNetwork` peer-routing and dispatch methods are tracked separately.

`CBaseNetwork::Delete`, `NewConnect`, `Exists`, `FindConnection`, and `KillUnBornConnection` map to `0x00461FC0/0x00462040/0x00462130/0x00462180/0x004621C0`. They maintain the doubly linked connection list, configure buffers on newly allocated peers, refresh live-peer timeouts, match network addresses, and kill matching unborn connections.

The six connect-control methods `CtoSRequestConnect`, `CtoSRequestNewPort`, `StoCOKConnect`, `StoCFAILEDConnect`, `CtoSGOConnect`, and `Establish` map to `0x004621E0/0x00462280/0x00462340/0x00462460/0x00462480/0x004624A0`. Their shared control streams implement request, port negotiation, authorisation, failure, go-ahead, and payload dispatch.

The final seven `CBaseNetwork` methods map `SetNCBuffers`, `SetCBuffers`, `AttachMessageQueue`, and `DetachMessageQueue` to `0x00462550/0x00462570/0x00462590/0x004625B0` in physical `EFFSTRM.CPP`, and `Process`, `SendAll`, and `ProcessMsg` to `0x004625E0/0x00462720/0x004627B0` in physical `VSNET.CPP`. This completes all 23 portable `CBaseNetwork` methods while preserving the Windows mixed-TU split. The `DetachMessageQueue` entry is `0x004625B0`; `0x004625C0` lies inside that body.

## Portable button lifecycle reconstruction

The two `CPVButton` constructors, `Initialise`, and destructor map to `0x00467C10/0x00467CD0/0x00467D50/0x00467DD0`. Constructor callers include derived button/window classes; both constructors establish CPVGWnd ownership before shared button-state initialization, and the destructor reverses that ownership. Physical definitions remain in `MENUSEL.CPP`; draw and pointer-interaction methods remain a separate pending subset.

The draw-state subset maps `SetAutoDraw`, `CheckForceDraw`, `_DrawButton`, and `Draw` to `0x00467E40/0x00467E50/0x00467EF0/0x00467F30`. The field setter, force-draw countdown and dirty-target refresh, changed-frame check, and forced virtual draw dispatch use the same object fields initialized by `CPVButton::Initialise`.

The final interaction subset maps `OnEnter`, `OnExit`, `ConvertDoubleClick`, `OnButtonDown`, `OnButtonUp`, `OnExternalButtonUp`, `_OnReleased`, `_OnPressed`, `_OnEnterButton`, and `_OnExitButton` to `0x00467FA0..0x004682B0`. Shared vtable references, local-point storage, click-flag normalization, press/hover latches, timestamped callbacks, and internal call edges complete all 18 portable `CPVButton` methods. Macintosh-specific painting and visibility methods remain outside this portable family.

## Portable depressed-button reconstruction

`CDepressedButton::_DrawButton` and `OnPaint` map to `0x00468300/0x00468360`. They extend the recovered `CPVButton` state with depressed-state change tracking, force-draw refresh, dirty geometry checks, primitive queueing, and change-list reset. Physical ownership remains in `src/Frontend/MENUSEL.CPP`.

## Portable graphic-button reconstruction

The `CGraphicButton` point constructor, `Initialise`, `SetAnimID`, destructor, `OnDestroy`, and `DrawButton` map to `0x00468530/0x004686E0/0x004688E0/0x00468920/0x00468980/0x004689A0`. The family derives from `CPVButton`, loads and unloads animation resources, computes frame extent and alignment flags, and queues the aligned draw primitive. Windows-only rectangle overload `0x00468410` remains separate.

## Portable file-wrapper reconstruction

`vsOpen`, `vsClose`, `vsRead`, `vsWrite`, `vsSeek`, `vsTell`, and `vsGetFileSize` map to `0x00462EE0/0x00462F20/0x00462F30/0x00462F50/0x00462F80/0x00462FB0/0x00462FC0`. They preserve the CRT-backed open/close/read/write/flush/seek/tell semantics and the size query's saved-position contract. Windows physically places `vsWrite` in `VSNETFIL.CPP` and the other six wrappers in `DEMO.CPP`; that split remains intact.

## Portable text primitive reconstruction

`CText::NextPos` and `Render` map to `0x004749C0/0x00474A20`. `NextPos` advances the cursor using glyph or fixed-cell extents and direction flags; `Render` owns font lifetime during the pass, resolves fallback glyphs, submits each glyph through `CGDI`, and advances the cursor. Physical ownership remains `DRAWTEXT.CPP`.

## Portable hot-area handler reconstruction

The two `CHotAreaHandler` constructors, `Initialise`, `Reset`, `ProcessArea`, `SetActive`, and `SetParent` map to `0x0046A290/0x0046A300/0x0046A330/0x0046A350/0x0046A380/0x0046A530/0x0046A560`. Their rectangle fields, six-button state arrays, event switch, virtual transitions, active-state reset, and parent redispatch correspond directly across the packed Macintosh and aligned Windows layouts. Physical ownership remains `VSWINDOW.CPP`.

## Portable hot-area list reconstruction

`CHotAreaList` constructor/destructor, `UpdateHandlers`, `DeleteEntry`, `ProcessMsg`, `ProcessHandlers`, `AddToList`, and `RemoveFromList` map to `0x0046A580/0x0046A650/0x0046A6D0/0x0046A6E0/0x0046A710/0x0046A770/0x0046A9A0/0x0046AA00`. The family owns queue attachment, shared pointer state, doubly linked handlers, coordinate normalization, bounds testing, and selected-handler dispatch. The adjacent deleting wrapper remains separate.

## Portable timed-queue disposition

Macintosh `CTimedQueue::Post` and `Send` are counter-plus-forwarding overrides layered on `CBaseQueue`. Windows folds those operations into `CBaseQueue::Post` and `Send` at `0x004631A0/0x00463230`; the shared addresses already belong to the independently mapped base methods, so the timed overrides have split/merged dispositions rather than duplicate direct correlations.

## Portable initialization-status selector

Global `OkFailed` maps directly to `0x00458F10`: both bodies select the shared `Ok` string for nonzero input and `Failed` for zero. The Windows callers use it repeatedly while reporting subsystem initialization results in physical `VSINIT.CPP`.

## Portable stream and input lifecycle reconstruction

`_STRM_Init`/`_STRM_Quit` map to `0x00458F70/0x004590B0`, preserving creation and teardown of three debug buffers and their three output streams. `_INP_Init`/`_INP_Quit` map to `0x00459130/0x004591F0`, preserving shared queue, master input, handler attachment, input flags, detachment, and destruction. Physical ownership remains `VSINIT.CPP`.

## Portable aggregate initialization reconstruction

`INIT_SubSystems`, `INIT_QuitSubSystems`, `INIT_CheckOptions`, `INIT_CmdLine`, `INIT_Main`, and `_VSExit` map to `0x00459250/0x00459520/0x004595D0/0x004596B0/0x00459860/0x00459970`. The family preserves subsystem dependency order and reverse teardown, option-table parsing, mutable command-line tokenization and compaction, game-session startup with exception frames, and non-returning startup `longjmp` behavior.

## Portable CArena allocator core

The seven CODE_02 spelling variants of `CheckAndAmalgamate`, `AddToBlockList`, `FindSmallestBlock`, `Allocate`, `Free`, `AllocateArena`, and `StreamOut` duplicate already-correlated CODE_09 implementations. Their bodies agree on block links, best-fit selection, split/coalesce accounting, child-arena ownership, locking, and recursive stream output, but one x86 body cannot be consumed twice. They therefore receive split/merged dispositions while canonical CODE_09 identities remain at `0x00459B10/0x00459C00/0x00459D20/0x00459D70/0x00459F70/0x0045A010/0x0045A260`.

`CArena::CheckIntegrity` is a distinct constant-success Macintosh stub (`[0x1557E,0x15588)`). Windows emits no corresponding standalone body; its specific `CheckMemoryBlock` and `CheckFreeMemoryBlock` validators remain separate and already mapped. `CheckIntegrity` is therefore split/merged rather than force-correlated.

## Portable memory-allocation globals

`InternalNew`, `InternalDelete`, global `operator new`, and global `operator delete` map to `0x0045A6B0/0x0045A730/0x0045A780/0x0045A790`. The internal pair tries the optional small-block buckets before falling back to the main arena and asserting allocation/free success; the global operators are thin delegates to that pair.

Global `CheckValidPointer` maps to `0x0045A800`: both implementations scan every enabled small-memory bucket before consulting the main arena. The member `CArena::CheckValidPointer` and `CBucket::CheckValidPointer` bodies remain distinct callees.

## Portable memory pre-initialization

`INIT_PreInit` maps to `0x0046F060`. Macintosh `NewPtr`/Windows `GlobalAlloc` and `GlobalLock` are platform delegates around the same portable contract: acquire master storage, construct the main RAM arena in place, conditionally construct the small-memory bucket table without recursively using it, restore its enable flag, and report success only when required objects exist.

## Portable sorting helpers

`VSQSort`, `shortsort`, and `swap` map to `0x00463960/0x00463AC0/0x00463B20`. Both binaries implement the same fixed-record comparator sort: explicit quicksort partitions, a short-partition selection-sort fallback, and byte-counted swapping. Windows physical ownership remains split between `VSINIT.CPP` and `CGame.cpp`.

## CSmallMemory stream identity stub

Macintosh `operator<<(CVSOStream&, CSmallMemory&)` is an exact identity stub at `[0x17D50,0x17D5D)`: it returns the stream argument without reading the allocator. Windows emits no standalone body, so this is split/merged rather than a forced correlation.

## CSoundManager ProcessMusic

`CSoundManager::ProcessMusic` maps to the previously uncreated x86 entry at `0x0045B350`. Macintosh `[0x18714,0x1873B)` and Windows both test active music and dispatch backend processing; Macintosh passes its stored current-music identifier, whereas the Windows backend slot takes no argument and owns that state internally. The explicit duration parameter is unused on both platforms. The recovered source implementation is a 100% reccmp match; adjacent Windows `PauseMusic` and `ResumeMusic` remain distinct.
