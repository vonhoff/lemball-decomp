# 68K Symbol Evidence & Provenance

`catalog.csv` maps Mac 68K MacsBug symbols to Windows addresses for naming and signature verification.

## Schema

`mac_address,symbol,windows_address`

- **`mac_address`**: Hex procedure address extracted from CODE resources (`0x10000000 + res_id * 0x100000 + offset`).
- **`symbol`**: MacsBug symbol name extracted from CodeWarrior procedure trailers.
- **`windows_address`**: Reviewed Windows x86 counterpart address (hex without prefix), or blank if unmapped.

## Tooling & Verification

- **Catalog check & coverage**:
  ```powershell
  python tools/gate.py --68k
  ```
- **Name & signature audit**:
  ```powershell
  python tools/gate.py --names
  python tools/gate.py --names --verbose   # list signature differences
  python tools/gate.py --names-strict      # fail on signature differences
  ```
- **Verify against private Mac resource fork** (`data/*.rsrc`):
  ```powershell
  python tools/gate.py --68k --68k-resource data/paintball-68k.rsrc
  ```

## Rules & Limitations

1. **Naming Authority**: Reconstructed code uses ordinary `// FUNCTION: LEMBALL 0x...` annotations; `tools/gate.py --names` resolves catalog symbols (class, method, parameter types, constness).
2. **Windows Precedence**: Windows x86 disassembly takes precedence over Mac symbols when ABI, platform types, or architecture differ.
3. **Windows ABI Reviews**: `tools/lib/names.py` records two exact address/symbol/signature reviews: the zero-argument `CWnd::OnDriverChange` vtable slot and the `CPlatformServices::GetCDDir` member. The naming gate prints their x86 evidence; strict mode still fails these review items. These entries do not permit other spelling changes.
