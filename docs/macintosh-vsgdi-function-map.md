# VSGDI Macintosh function map

Windows ABI and addresses come from `/LEMBALL.EXE`; names and family order come from the Macintosh `Visos` modules.

| Windows | Recovered owner/function | Macintosh module | Confidence |
| --- | --- | --- | --- |
| `0x004663D0` | `CPVSurface::CPVSurface` | `Visos (Generic)` | High |
| `0x00466440` | `CPVBackBuffSurface::CPVBackBuffSurface` | `Visos (Generic)` | High |
| `0x004664B0` | `CPVBackBuffSurface::~CPVBackBuffSurface` | `Visos (Generic)` | High |
| `0x004664E0` | `CPVBackBuffSurface::HasBackBuff` | `Visos (Mac Specific)` | High |
| `0x00466510` | `CPVBackBuffSurface::FreeBackBuff` | `Visos (Generic)` | High |
| `0x00466540` | `CPVBackBuffSurface::AllocateBackBuff` | `Visos (Generic)` | High |
| `0x00466630` | `CPVBackBuffSurface::EnableBackBuff` | `Visos (Generic)` | High |
| `0x00466660` | `CPVBackBuffSurface::ResizeBackBuff` | `Visos (Generic)` | High |
| `0x00466670` | `CPVZBuffSurface::CPVZBuffSurface` | `Visos (Generic)` | High |
| `0x004666E0` | `CPVZBuffSurface::~CPVZBuffSurface` | `Visos (Generic)` | High |
| `0x00466710` | `CPVZBuffSurface::FreeZBuff` | `Visos (Generic)` | High |
| `0x00466740` | `CPVZBuffSurface::AllocateZBuff` | `Visos (Generic)` | High |
| `0x00466840` | `CPVZBuffSurface::EnableZBuff` | `Visos (Generic)` | High |
| `0x00466870` | `CPVZBuffSurface::ResizeZBuff` | `Visos (Generic)` | High |
| `0x00466990` | `CPVZBuffSurface::HasZBuff` | `Visos (Mac Specific)` | High |
| `0x0046C9F0` | `CSurface::SetLinePtrs` | `Visos (Mac Specific)` | High |
| `0x00472290` | `CPVGDIBitmap::CPVGDIBitmap` | `Visos (Generic)` | High |
| `0x004722D0` | `CPVGDIBitmap::~CPVGDIBitmap` | `Visos (Generic)` | High |
| `0x004722E0` | `CPVGDIBitmap::Free` | `Visos (Generic)` | High |
| `0x00472310` | `CPVGDIBitmap::Initialise` | `Visos (Generic)` | High |
| `0x00472340` | `CPVGDIBitmap::CreateLinePtrs` | `Visos (Generic)` | High |
| `0x004723A0` | `CPVGDIBitmap::ResetLinePtrs` | `Visos (Generic)` | High |
| `0x00472400` | `CPVGDIBitmap::SetLinePtrs` | `Visos (Generic)` | High |
| `0x00472440` | `CPVGDIBitmap::Scroll` | `Visos (Generic)` | High |
| `0x004725F0` | `CPVGDIBitmap::SetSize` | `Visos (Generic)` | High |
| `0x00472670` | `CPVGDIBitmap::SetBitsBase` | `Visos (Generic)` | High |
| `0x004726B0` | `CPVGDIBitmap::GetRects` | `Visos (Generic)` | High |
| `0x00472760` | `CPVGDIBitmap::ResetScroll` | `Visos (Generic)` | High |

`data/macintosh-x86-correlations.csv` is the exhaustive machine-readable source of truth. It records 85 accepted mappings, including the `CPVSurface`, `CPVScrollableSurface`, `CGDIDevice`, `CSurface`, and `CArena` lifecycle/virtual families with Macintosh offsets and evidence.

Windows topology now identifies the provisional names directly: `TargetPrimary` is the `CPVScrollableSurface` primary subobject, `Target` is `CSurface`, `CompactHelper` is the virtual `CPVSurface` base, `Group0` is `CPVBackBuffSurface`, and `Group1` is `CPVZBuffSurface`. The `0x5A0` Windows layout and its vbtable/vtordisp offsets remain Windows-only evidence.

Windows-only rendering adapters, DirectDraw helpers, upload trackers, and compiler adjustor thunks retain slot/behavior labels. They are not assigned a Macintosh method merely by address order; promote them only when caller, signature, lifecycle, or table evidence agrees.
