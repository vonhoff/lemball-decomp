# 68K annotation evidence

`catalog.json.gz` contains derived metadata only: 2,848 Mac symbols, 2,685
reviewed Windows pairs, and the original resource fork's SHA256. No game code
or resource payload. Deterministic gzip keeps the catalog at about 38 KB.
Normal checks need neither a Mac original nor an external checkout.

```powershell
python tools/gate.py --68k --annot-strict
```

The normal gate includes this check; `--all` adds naming and vtable checks.
`invalid` means a malformed comment or a name/address absent from the catalog.
`review` means an unlisted Windows pair; strict mode fails these entries.
`symbol-only` means no adjacent Windows address, as with an inline declaration.
A listed pair records reviewed research, not proof of cross-platform equivalence.

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

This checks the recorded SHA256 and re-extracts the complete symbol dictionary.
Original files stay in ignored `data/`; `*.rsrc` is also ignored. Without a private
original, verification trusts the reviewed catalog. Its digest identifies the
extraction input; it does not independently authenticate the catalog or prove
Windows equivalence.

## Reviewing catalog changes

```powershell
python tools/gate.py --68k-export build-msvc400/68k-review.json
# Review or edit the exported rows using independent disassembly evidence.
python tools/gate.py --68k-pack build-msvc400/68k-review.json
python tools/gate.py --68k --annot-strict
```

Format 1 stores each row as `[Mac address, original symbol, [Windows addresses]]`.
Addresses are eight lowercase hex digits; an empty Windows list is valid.
Export gives one readable row per line. Pack validates and sorts the rows,
then omits filenames and timestamps from gzip. Adding `--68k-resource` to pack
also checks the symbol dictionary before replacing the catalog.

Do not generate accepted pairs from the annotations under test. Record new
mapping rationale in the reviewed commit, citing original addresses and behavior.
Shared names, source order, or similar instruction counts alone are insufficient.
Compare exported catalogs when reviewing changes to the compressed file.

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
