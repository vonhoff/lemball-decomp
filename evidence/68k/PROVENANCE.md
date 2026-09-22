# Derived 68K symbol evidence

`catalog.csv` contains derived metadata only: 2,848 Mac symbols and 2,685
reviewed Windows pairs. Plain CSV, about 136 KB; no game payload, archive, JSON,
or whole-file SHA256. Review and edit directly with ordinary text tools.
Normal checks need neither a Mac original nor an external checkout.

```powershell
python tools/gate.py --68k --annot-strict
```

The normal gate includes this check; `--all` adds naming and vtable checks.

## Naming authority

`python tools/gate.py --names` looks up the original Windows annotation in this
CSV, then decodes the catalog symbol. Source files need only the ordinary
`// FUNCTION: LEMBALL 0x0043a250` annotation; duplicate `// 68K` comments have been
removed. Folded entries retain all catalog candidates.
Compiler-emitted functions without C++ declarations are reported as synthetic;
Windows entries without a reviewed pairing are reported as unmapped.

The decoder checks class, method, parameter types, and method constness. Argument
names and defaults are ignored. Ordinary return types are not encoded and are
not inferred. Unmangled names provide no parameter evidence. Unsupported C++
declarators and typedef differences remain explicit review items, not matches.
The CodeWarrior grammar was cross-checked against
[cwdemangle](https://github.com/encounter/cwdemangle), a CC0 implementation.
Normal operation uses only the bundled Python implementation and CSV.

`--names --verbose` lists signature differences. `--names-strict` fails those
reviews and naming case differences. Mac types must not override stronger
Windows evidence: ABI changes, platform APIs, and typedefs can explain a review.
`--names-original` audits exact original spelling, including prefixes and case;
normal naming checks apply the project's documented spelling policy.

Catalog-backed class prefixes are preserved, including constructors, destructors,
filenames, includes, and guards. Acronyms retain project style (`CPVWnd` becomes
`CPvWnd`). Project-owned Windows classes also use `C` + PascalCase, with concise
method names based on Windows behavior. Those names are reconstruction choices;
they do not establish original spellings or add catalog pairs. Platform API names
and passive record structs retain their established spelling. `CHAR4` retains its
complete original spelling; the old `Har4` spelling resulted from stripping its
first letter. Original-filename exceptions `VsGdi` and `MogLoad` remain documented
in the layout gate. Method names that coincide with class names remain methods.

## Three linked values

```csv
mac_address,symbol,windows_address
10b0f952,__ct__12CPadToButtonFi,43a250
```

Addresses are hexadecimal integers without a prefix or redundant leading zeros.
The catalog retains three linked facts:

1. The Mac address identifies an extracted procedure entry.
2. The symbol matches the MacsBug name at that exact address.
3. The Windows address identifies the separately reviewed counterpart.

Two columns (`windows_address,symbol`) would suffice for naming lookup alone.
The Mac address remains an extraction anchor: private verification checks that
each name belongs to that exact CODE procedure, including repeated symbols.
Removing duplicate source comments saves space without removing this evidence.

A blank Windows cell records a Mac symbol without a reviewed Windows pairing.
Multiple compiler variants repeat the Mac address/name with distinct Windows
addresses. Folded functions may share a Windows address. Conflicting names and
duplicate pairs fail validation. Rows are sorted by Mac address, then Windows
address.

Do not generate accepted pairs from the annotations under test. Record new
mapping rationale in the reviewed commit, citing original addresses and behavior.
Shared names, source order, or similar instruction counts alone are insufficient.
A listed pair records reviewed research; lookup alone does not prove semantic
equivalence between architectures.

## Independent symbol verification

The symbol catalog came from MacsBug procedure trailers in the original CODE
resources, independently of reconstructed source annotations. Addresses use
`0x10000000 + resource_id * 0x100000 + procedure_offset`. Extraction skips CODE 0,
constant pools, and relocation records. Template punctuation receives the same
symbol sanitization as the Ghidra import. The parser remains in
`tools/lib/provenance.py`; regression fixtures contain synthetic procedures.
These are deterministic Ghidra import addresses, not Mac runtime load addresses.

The original research Ghidra export was checked again: all 2,848 catalog
address/name entries agree with that export and with fresh resource extraction.
Two extra Ghidra-generated `FUN_...` labels are excluded because they are not
MacsBug symbols. The reviewed pairing ledger supplies the starting Windows
correspondence; subsequent corrections below are retained separately from raw
symbol provenance. Research origin does not make every proposed pairing correct.

Optional verification against a privately supplied resource fork:

```powershell
python tools/gate.py --68k --annot-strict --68k-resource data/paintball-68k.rsrc
```

This re-extracts and compares the complete address/name dictionary. Missing,
extra, renamed, or relocated symbols fail. A whole-file hash is unnecessary for
this comparison; unrelated container bytes do not affect symbol provenance.
Original files stay in ignored `data/`; `*.rsrc` is also ignored. Without a private
original, verification trusts the reviewed catalog. The Mac parser does not
establish Windows correspondence; that requires separate disassembly review.

## PadToButton example

Private resource extraction confirms `__ct__12CPadToButtonFi` at `0x10b0f952`.
It decodes to `CPadToButton::CPadToButton(int)`: `__ct__` is a constructor,
`12CPadToButton` supplies its owner, and `Fi` encodes one `int` parameter.
The constructor repeats the complete class name, including `C`. The trailer
does not encode a parameter identifier such as `p_arg0`.
The Mac constructor and Windows `0x0043a250` both construct a queue handler,
allocate capacity entries, store capacity at `+0x18`, clear count at `+0x14`,
clear each button pointer, and attach the handler at priority -25. Entry sizes
are 6 bytes on Mac and 8 bytes on Windows. Windows table `0x00497208`, allocator
call `0x0043a26b`, and Attach call `0x0043a29c` support this reviewed mapping.

## Corrections retained from the original mapping audit

- 80 non-deleting destructor entries: exact calls and linker-jump chains from
  previously paired deleting wrappers reach the additional entries before free.
  BigBitmap adds one inline variant: wrapper `0x00439620` resets vtable
  `0x00496ca8` at `0x00439626`; `0x00439720` performs that reset and returns.
- Main2DDisplay pairs were reversed. OnZoom `0x10b006fe` maps to `0x00431880`:
  Drawer dispatch `+0x0c`, `RET 4`. OnMove `0x10b00746` maps to `0x004318a0`:
  Drawer dispatch `+0x14`, `RET`.
- DrawingMark Draw/Render/destructor `0x101019c0` / `0x101019f4` / `0x10101a3c`
  map to `0x00432380` / `0x00432390` / `0x00432320`. Table `0x00496c98` and
  Render's GetChangeList/SetDrawMark calls establish ownership.
- ScreenScroll Draw/Render/destructor `0x10101e34` / `0x10101e6a` / `0x10101eae`
  map to `0x00439860` / `0x00439870` / `0x00439650`. Render reaches Surface
  scroll `0x00474c20` through table `0x00499df0` and adjustor `0x0046db40`.
  Its rectangle, color-buffer, and Z-buffer operations establish scroll intent.

These corrections use original x86 evidence; older imported labels were stale.
The catalog preserves all reviewed pairs after these explicit corrections.
