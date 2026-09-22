# 68K annotation evidence

`catalog.csv` contains derived metadata only: 2,848 Mac symbols and 2,685
reviewed Windows pairs. Plain CSV, about 136 KB; no game payload, archive, JSON,
or whole-file SHA256. Review and edit directly with ordinary text tools.
Normal checks need neither a Mac original nor an external checkout.

```powershell
python tools/gate.py --68k --annot-strict
```

The normal gate includes this check; `--all` adds naming and vtable checks.

## Three linked values

```csv
mac_address,symbol,windows_address
10b0f952,__ct__12CPadToButtonFi,43a250
```

Addresses are hexadecimal integers without a prefix or redundant leading zeros.
The verifier checks all three values together:

1. The Mac address identifies an extracted procedure entry.
2. The symbol matches the MacsBug name at that exact address.
3. The adjacent Windows annotation belongs to a reviewed pair for that entry.

A real name at another Mac address fails. A real Windows address paired with
another Mac function requires review. Two columns suffice for Mac name/address
lookup; the third preserves Windows correspondence.

`invalid` means a malformed comment or a wrong Mac name/address combination.
`review` means an unlisted Windows pair; strict mode fails these entries.
`symbol-only` means no adjacent Windows address, as with an inline declaration.
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
