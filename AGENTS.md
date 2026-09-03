# AGENTS.md

Byte-matching reconstruction of *Lemmings Paintball* (`LEMBALL.EXE`, 1996 Win32).

## Evidence Priority

Resolve conflicts in this order:

1. `data/LEMBALL.EXE`
2. x86 disassembly / Ghidra / PDB
3. `reccmp`
4. Observed MSVC 4.00 codegen
5. 68K symbols/comments/code — names/intent only; never overrides x86

Rules:

* Never invent symbols, addresses, members, behavior, or helpers.
* Never edit `README.md`, `Manifest.h`, reference hashes, or compiler flags unless asked.
* On minor ambiguity, choose the best-supported path and document assumptions at the end.

## Code Intelligence

Use `codebase-memory-mcp` first for source-tree relationships.

* Before editing shared/existing code, inspect callers, dependencies, inheritance, and related symbols.
* Use graph search for relationships; use `rg`/`grep` for literal text only.
* Run `detect_changes` before final verification when multiple symbols/TUs may be affected.
* The graph describes reconstructed source only. Binary/Ghidra/PDB/reccmp remain authoritative.

## Environment

* MSVC 4.00: `/O2 /Ob1 /Oy /G4`.
* No C++11+, RTTI, exceptions, or inline asm.
* One primary class per `.h`/`.cpp`.
* Functions in each `.cpp` must follow ascending original x86 address.
* Use `RES_*` from `src/Visos/Resources/Manifest.h`; preserve binary values.

## `reccmp` Annotations

Format: `// <TYPE>: LEMBALL <ORIGINAL_X86_ADDR> [OPTION]`

Addresses are always from the original `LEMBALL.EXE`, never the rebuilt executable. Functions within one compilation unit must be annotated in ascending original-address order.

| Annotation  | Usage / Rules                                                                                                                                                                                                                                           | Example                                                                         |
| :---------- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | :------------------------------------------------------------------------------ |
| `FUNCTION`  | Reasonably complete, non-template, non-synthetic function. `reccmp` compares its rebuilt assembly with the original. A 100% match is the goal, not a requirement for using `FUNCTION`. Prefer placing the annotation directly above the implementation. | `// FUNCTION: LEMBALL 0x00472ce0`                                               |
| `STUB`      | No implementation or very incomplete implementation. `reccmp` does **not** compare it against the original assembly. Promote to the appropriate function annotation once substantially implemented.                                                     | `// STUB: LEMBALL 0x00472ce0`                                                   |
| `TEMPLATE`  | Template-generated function. When it cannot be attached directly to an implementation, put the exact function name or debug symbol on the following comment line.                                                                                       | `// TEMPLATE: LEMBALL 0x00401230`<br>`// SomeTemplate<int>::Func`               |
| `SYNTHETIC` | Compiler-generated function, commonly scalar deleting destructors, implicit/default destructors, or assignment operators. Takes precedence over `TEMPLATE`. Usually followed by the exact generated function name.                                      | `// SYNTHETIC: LEMBALL 0x00401230`<br>`// Bucket::'scalar deleting destructor'` |
| `LIBRARY`   | Function originating from a third-party/runtime library. Use only with evidence that the function is library code; never classify unknown game code as `LIBRARY`. Usually followed by its name or symbol.                                               | `// LIBRARY: LEMBALL 0x00401230`<br>`// _MemPoolInit@4`                         |
| `VTABLE`    | Virtual-table address for a class. Place above the class declaration. `// vtable+0xNN` comments on virtual methods are project bookkeeping/convention, not part of the `VTABLE` annotation grammar itself.                                              | `// VTABLE: LEMBALL 0x0049a478`                                                 |
| `GLOBAL`    | Address of global/static storage in the original binary. Do not use for automatic/local variables.                                                                                                                                                      | `// GLOBAL: LEMBALL 0x0049f000`<br>`int g_count = 0;`                           |
| `STRING`    | Address of the actual string bytes. Usually optional because `reccmp` can auto-detect strings. A pointer variable may have a separate `GLOBAL` address.                                                                                                 | `// STRING: LEMBALL 0x0049f000`                                                 |
| `LINE`      | Associates an original instruction address with a specific source line to improve diff alignment. The annotation must be followed immediately by a code line, not a blank line or another comment. Use only when useful for difficult mismatches.       | `// LINE: LEMBALL 0x00401234`<br>`short xmax = xofs + width - 1;`               |
| `FOLDED`    | Option on a `FUNCTION` annotation when multiple functions were folded to the same original address by identical-code folding. Not a standalone annotation.                                                                                              | `// FUNCTION: LEMBALL 0x00401230 FOLDED`                                        |
| `SYMBOL`    | Option used when the following comment contains a debug symbol rather than a function name. MSVC decorated names beginning with `?` are recognized automatically; otherwise add `SYMBOL`.                                                               | `// LIBRARY: LEMBALL 0x00401230 SYMBOL`<br>`// __strlwr`                        |

Do not invent annotation types or addresses. Use the annotation type that describes what the original function actually is; annotation type is not a progress score.

## Types & Layout

```cpp
// SIZE 0x54
class Foo {
    unsigned int m_size; // 0x24
};
```

* Use `undefined`, `undefined2`, `undefined4` for unknown-width scalars.
* Do not guess signedness without assembly evidence.
* Prefer structured member access over raw offset/vbptr manipulation.

## MSVC 4.00 Matching Notes

* Stores may force reloads; avoid keeping locals live when original code reloads.
* Re-fetch pointers after virtual calls when assembly does so.
* `/Ob1`: same-TU inline definitions inline; cross-TU definitions do not.
* For narrow stack params read as raw 32-bit values, use `*(unsigned int*)&param`.
* Message dispatch may require `switch` rather than `if` to reproduce widening codegen.
* Prefer original loop shape; do not add zero-count guards without evidence.
* Keep size/capacity arithmetic 32-bit when original uses 32-bit arithmetic.
* Typical loop registers:

  * 3 saved values: `EBX` offset, `ESI` counter, `EDI` `this`.
  * 4 saved values + `EBP` constant: `ESI` offset, `EBX` counter, `EDI` `this`.
* Stop grinding when remaining differences are compiler noise: equivalent register swaps, equal-length jump displacement changes, or trailing alignment `nop`s.

## Workflow

```powershell
# Match loop
python tools/check.py 0xADDRESS
python tools/check.py 0xADDRESS --diff
python tools/check.py 0xADDRESS --no-build
python tools/check.py 0xADDRESS --clean-first   # PDB/debug desync only

# Targets
python tools/targets.py --kind tiny --max-size 5
python tools/targets.py --kind near|unit|clone

# Verification
python tools/build.py
python tools/report.py
reccmp-decomplint --target LEMBALL --warnfail src
python tools/smell.py
python tools/smell.py --annot
reccmp-stackcmp --target LEMBALL 0xADDRESS
reccmp-vtable --target LEMBALL
reccmp-datacmp --target LEMBALL
```

Use the cheapest diagnostic first:

`check.py` → `check.py --diff` → `stackcmp`

Avoid clean builds and full reports inside the tight matching loop.

## Direct `reccmp` Tools

Target-based tools use `build-msvc400/reccmp-build.yml`. Run their venv executables from `build-msvc400`:

```powershell
Push-Location build-msvc400
try { ..\.decomp-venv\Scripts\reccmp-vtable.exe --target LEMBALL }
finally { Pop-Location }
```

Same pattern for `reccmp-datacmp.exe` and `reccmp-stackcmp.exe`.

`reccmp-decomplint.exe` does not require the generated build configuration.

## Done When

* Annotation types and original x86 addresses are correct.
* Functions remain sorted by original address.
* Implemented functions are not left as `STUB`.
* Target check and build pass.
* `reccmp-decomplint` has zero warnings/errors.
* No invented symbols or out-of-scope churn.
