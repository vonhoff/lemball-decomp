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

All ten portable `CLaser` and nine portable `CLaserManager` methods align with Windows `0x00428890..0x00429950` through chunk type `0x2F`, 0x148-byte child stride, directional terrain footprints, target acquisition, states `0x17..0x1A`, EFF-backed manager fields, variable view output and versioned loader records. Windows remove/count helpers `0x00429470`/`0x004297E0`, no-op callback `0x00429E40`, and deleting wrappers `0x00429EA0`/`0x00429ED0` remain separate.
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

All 18 portable methods map at `0x00409CA0..0x0040A490`. Windows evidence preserves the `0x2B8` object stride, two-coordinate bounds setup, `0x18` tracked-object capacity, contact/activation state machine, versioned level loader, and split physical ownership between `LINKSCF.CPP` and `CInvsMgr.cpp`. The point-only object overload `0x00409D20`, manager Add overloads `0x0040A3E0`/`0x0040A440`, and deleting wrapper `0x0040AC60` are target-only and consume no portable symbols.

### CLift and CLiftManager

All 21 portable methods map at `0x00424D00..0x00425FC0`. Windows evidence preserves the `0x190` object stride, endpoint/legacy setup overloads, boundary cliff updates, eight passenger slots, timed motion states, view records, slot switching, and versioned loader while retaining split Windows ownership. Serialization helpers `0x004257E0`/`0x00425830`, remove helper `0x00425890`, Add helpers `0x00425C80`/`0x00425CE0`, manager cliff helper `0x00425DC0`, and deleting wrappers `0x004266E0`/`0x00426710` consume no portable symbols.

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
