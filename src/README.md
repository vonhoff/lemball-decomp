# LEMBALL C++ Scaffold

This is a source-shaped documentation scaffold for the Windows x86 reverse-engineering database. It is not currently intended to compile.

## Organization

- Folders follow recovered original modules, such as `AI/`, `Views/`, and `Visos/`.
- Module paths record 68K source provenance, not target applicability. `Visos/` unifies the recovered generic, Mac-specific, and Windows-specific Visos code; the Mac-specific classes may have substantial Windows implementations in `LEMBALL.EXE`.
- Module names are presented in CamelCase and unified: `Views/` for the recovered `views_2d` segment, and `Visos/` for the recovered `Visos__Generic_`/`Visos__Mac_Specific_` segments plus the Windows-specific target classes. The recovered linker-segment name is preserved in the mapping ledger.
- Within `Visos/`, `AI/`, `Frontend/`, and `Views/`, classes are further grouped one level deep by behavior via a curated `"categories"` map in `scaffold-layout.json` (`Visos`: `Network`, `Messaging`, `Resources`, `Graphics`, `Sound`, `Target`, `Animation`, `Foundation`; `AI`: `Objects`, `Managers`, `Navigation`, `Groups`, `Messages`, `Base`; `Frontend`: `Base`, `Drawers`, `Processes`, `Windows`, `Controls`, `Resources`, `Support`; `Views`: `Display`, `Panel`, `Pause`, `Animation`, `Sound`, `Input`, `Target`). The recovered linker segment stays available in the mapping ledger.
- Each recovered class has one `.cpp` file containing its layout, inheritance, declarations, and empty mapped function bodies. Where an assertion `__FILE__` string reveals the original source file, a per-class `"file"` override (or a `"file"` on a free-function entry) names the file to match (`GAME.CPP` -> `Game.cpp`, `VSGDI.CPP` -> `VsGdi.cpp`, `MOGLOAD.CPP` -> `MogLoad.cpp`, `VSMEM.CPP` -> `VsMem.cpp`, `VSINIT.CPP` -> `VsInit.cpp`).
- A class is assigned to its configured module or, when not configured, the module containing most of its confirmed functions.
- Recovered class names drop the CodeWarrior `C` convention prefix and PascalCase all-caps acronyms (`CGameObject` -> `GameObject`, `CResPALETTE` -> `ResPalette`, `CZRLE` -> `Zrle`, `AI` -> `Ai`); `scaffold-layout.json` stores the normalized name directly, and the prefixed Metrowerks name stays verbatim in each `// 68K` annotation. `C2D`/`C3DVector` retain the prefix because their semantic names begin with a digit.
- `Common.h` contains only shared unknown aliases, enum declarations, and forward declarations.
- `scaffold-layout.json` is the curated source for class sizes, vtables, inheritance, fields, and physical-layout notes.
- `vtableSlots` records exact source-symbol-to-byte-offset mappings for virtual declarations; `overrides` marks only independently confirmed overrides.
- `manifest.txt` is generated and lists every scaffold source file.

## Annotations

Recovered functions use the reccmp `STUB` marker (address known, body not yet reconstructed) with the Windows address:

```cpp
// STUB: LEMBALL 0x0045f010
```

Addresses are lowercase and zero-padded. The preceding plain comment preserves the independently matched 68K address and original Metrowerks symbol (not a reccmp marker):

```cpp
// 68K 0x1020a700 Get__15CNetworkMessageFRUl
// STUB: LEMBALL 0x0045f010
```

Bodies are empty, so every function is currently a `STUB`; when a body is reconstructed the marker changes to `FUNCTION` and the function becomes comparable by `reccmp-reccmp`. Folded functions (two source symbols sharing one Windows body) append `FOLDED`.

Class and field conventions follow the [reccmp recommendations](https://github.com/isledecomp/reccmp/blob/master/docs/recommendations.md):

- `// SIZE 0x...` records confirmed Windows class sizes.
- `// VTABLE: LEMBALL 0x...` records confirmed Windows vtables.
- Member comments record relative offsets.
- `undefined`, `undefined2`, and `undefined4` distinguish unresolved scalar types from proven types.
- Vtable method comments record byte offsets when the mapping evidence identifies a slot.
- Compiler-generated vptr/vbptr entries may remain in `scaffold-layout.json` as physical-layout evidence, but they are not emitted as ordinary C++ members.

Unknown gaps are omitted. `MINIMUM SIZE` and physical-layout notes are evidence boundaries, not assertions of a complete C++ ABI model.
`DECOMP_SIZE_ASSERT` is intentionally deferred until a class has enough padding and base-layout information for the scaffold to compile without asserting a knowingly incomplete declaration.
Parameter types are decoded from the original Metrowerks symbols. Proven return types and signatures can be recorded by exact source symbol in a class's `returns` and `parameters` maps; other return types and parameter names remain provisional. Empty bodies deliberately carry no behavioral reconstruction yet.

## Regeneration

Run from `C:\Research` after updating `Mapping/confirmed-mappings.tsv`, the 68K inventory, or `Scaffold/scaffold-layout.json`:

```powershell
node generate_cpp_scaffold.js
```

Generated class files must not be edited directly. Put newly confirmed layout facts in `scaffold-layout.json`; function annotations are derived from the authoritative mapping ledger.
Generated files remain tracked so mapping changes and source annotations are reviewable without running Ghidra or the generator.
