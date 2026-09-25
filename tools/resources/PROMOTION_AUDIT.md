# Removed promotion audit

Baseline: `c24987ba`. Rules: `3820e783^:tools/lib/compare.py`. Date: 2026-09-25.
Lens: [Ponytail](https://raw.githubusercontent.com/DietrichGebert/ponytail/refs/heads/main/skills/ponytail/SKILL.md): existing machinery first; remove duplicate paths.

Reproduced on the initial build: **96 partial functions / 27,190 bytes**.
87 “thunk”; 9 “compiler entropy”. Actual effective coverage **246,490 / 405,552 = 60.7788890204%**.
Legacy promotions: **273,680 / 405,552 = 67.4833313607%**. These are whole-function byte coverage percentages, not fuzzy instruction similarity.

Result: **78 recovered / 22,478 bytes**: 67 normalized assembly exact; 11 reccmp effective.
One additional function outside the cohort: `0x0041dfc0 CGenericGroup::SwapElements`, 86 bytes.
Current objdiff-v2 report: **3,032 / 3,454 functions; 269,054 / 405,552 bytes = 66.3426638261%**.
Zero prior normalized assembly-exact losses; zero prior effective losses. `tools/report.py` unchanged.

| Disposition | Functions | Original bytes |
|---|---:|---:|
| Old promoted cohort | 96 | 27,190 |
| Recovered by shared engine / source / annotations | 78 | 22,478 |
| Proven equivalent; matcher still unresolved | 17 | 4,544 |
| Evidence unresolved: default-array input bound | 1 | 168 |
| Disproven on established valid inputs | 0 | 0 |

Proof scope: original Win32 ABI, valid object lifetimes, documented/inferred function input contracts. Missing contract evidence is explicitly unresolved. This audit does not equate unresolved with mismatch. The 17 remaining equivalence findings support their old semantic scores; they are not automatically written as 100 into the report. Including those independently proved bytes gives 273,598 / 405,552 = 67.4631120054% evidence-supported coverage. This is separate from machine-verified report coverage.

## Evidence and reproduction

[`promotion-audit.json`](promotion-audit.json): all 96 entries. Each records the baseline reccmp diff, exact original/rebuilt changed instruction bytes, relocation sites and targets, original function bytes, Ghidra listing, current diff/status, class and proof. Shared CRT/PDB and complete virtual-base-table evidence included. Image hashes and baseline report hash recorded.

All 96 original byte ranges were independently read through Ghidra and compared with `data/LEMBALL.EXE`: **96 equal; 0 different**. Fourteen ranges lack a defined Ghidra function; their raw bytes still agree. They were decoded with Capstone/reccmp and correlated with rebuilt PDB library symbols. PDB sizes sometimes include alignment; coverage bytes use the unchanged roadmap inventory.

Actual baseline run: `python tools/build.py`, `python tools/report.py`; execute the historical `compute_ratio` against all 3,454 inventory entries. To repeat that live run, use baseline commit `c24987ba` and its build. To verify the preserved cohort against the exact historical predicates on the final tree:

```powershell
@'
import json, subprocess
from pathlib import Path
old = {}
exec(subprocess.check_output(['git', 'show', '3820e783^:tools/lib/compare.py'], text=True), old)
a = json.loads(Path('tools/resources/promotion-audit.json').read_text())
rows = a['functions']
assert all(r['baseline_accuracy'] < 100 and not r['baseline_match'].get('effective') for r in rows)
assert all(old['compute_ratio'](r['baseline_match'])[0] == 100 for r in rows)
print(len(rows), sum(r['size'] for r in rows))
print(100 * a['baseline_effective_bytes'] / a['total_code_bytes'])
print(100 * (a['baseline_effective_bytes'] + sum(r['size'] for r in rows)) / a['total_code_bytes'])
'@ | python -
```

Expected: `96 27190`, `60.77888902039689`, `67.4833313607133`.

## Representative fixes and decisions

| Difference class | Concrete evidence / before → after | Decision |
|---|---|---|
| Unmapped CRT globals | `4827e0 __getmbcp`: original `a1b4474a00c3`; PDB `___mbcodepage`; setter uses same original global. 50 → 100 A. `482800 __ioinit`: identical 256-byte initial `___pioinfo`, handle-count references; 93.38 → 100 A. | Replace wildcard with original GLOBAL annotations. |
| Data/string-pointer tables | `485b10 __output`: all 89 lookup-table bytes equal; relocated pointers lead to identical narrow/wide `(null)` strings. 99.48 → 100 A. `4824d0 __setmbcp`: containing data mapping resolves incidental `"!"` label. 94.21 → 100 A. | Retain data identity; use existing typed/offset mapping. |
| Virtual-base tables | `45f820 CReadSocket`: both complete tables `bcffffff14000000`, offsets {-68,20}. 96.97 → 100 A. Forty-one further complete PDB-sized tables matched byte for byte. | Add original GLOBAL names. No value wildcard. |
| Nested vtable display names | `470270 CTCPIPBroadcast`: read/write paths have different COFF symbols despite identical display names. 95.62 → 100 A. Gate: 406/406 tables, 6,351/6,351 slots, 173/173 adjusters. | Move existing gate lookup into shared engine; delete gate-only duplicate lookup/results. |
| Commuted address sums | `460c00 CConnect` destructor: EAX+ECX versus ECX+EAX; same width, displacement and flat-segment address. 78.26 → 100 E after actual vtable mapping. | Retain upstream effective checker. |
| Initializer scheduling | `439580 CBitmap`: Y/X, height/width and sourceY/sourceX stores. Replace ineffective reverse initializer-list spelling with chained coordinate initializer and original body store order. 72.73 → 100 E. | Source reconstruction; remaining disjoint vptr movement accepted upstream. |
| Linker pointer plus register allocation | `43ff70 SortViewData`: `402eb4: e9a7d00300` targets `43ff60 ViewDataCmp`; comparator passed by pointer. 77.27 → 100 E. ESI/EDI roles: index versus index*0x4c; upstream validates swap. | Narrow thunk recognition to verified linker table plus mapped target, including address operands. |
| Transient vptr plus scheduling | `445420`: store496d00 to this+350 at4454c6, immediately overwritten at4454d0. Rebuilt transient CFrames table differs but cannot be read. 94.06 → 100 E. | Compose existing overwrite proof with upstream checker on complete streams. |
| Flags scheduling | `421ab0`: XOR EDI crosses loads/ADD ECX; CMP EAX,EDI overwrites flags before JLE. 97.92; proven equivalent, still partial. | Preserve finding; no general unchecked move rule. |
| Stack/member/epilogue scheduling | `469a50`: 76.92; same fields/arguments/RET18. `44d8e0`: 92.16; same POP order, restored registers unused by crossed arithmetic. | Preserve proofs; source shape or alias-aware dependency checking needed. |
| Call preparation | `47c050`: 95.59; newline PUSH crosses zero-argument GetLastError; same error/log arguments. `457410`: 98.53; COM loads cross caller-stack initialization, no call. | Preserve ABI-specific proofs; generic movement checker currently cannot establish required memory/callee facts. |
| Interior string address | `484b00 __assert`: 99.30 → 99.77. Both operands parent+4 of mapped18-byte `Assertion failed!`; suffix14 bytes equal. | Proven equivalent. Upstream name lookup explicitly excludes STRING offsets; need bounded parent+offset mapping. |
| Negative array addend | `4131e0`: 95.65. Both64-byte tables equal; `table-16+16*count`. Count1..4 works. Count0/default-path counterexample below. | Narrow semantic claim to proved input domain. Caller bound unresolved. |

## Why the old predicates cannot stand unchanged

Concrete executable counterexample, checked with the historical predicate:

```text
original: b8 01 00 00 00 83 c0 01 c3   mov eax,1; add eax,1; ret  -> 2
rebuilt:  83 c0 01 b8 01 00 00 00 c3   add eax,1; mov eax,1; ret  -> 1
legacy compute_ratio: (100.0, 'MATCH (thunk)')
```

The predicate drops unchanged `add eax,1` before comparing moved lines. Concrete dependency lost. Current checker rejects this example; regression check retained.

The compiler-entropy predicate also accepts a concrete wrong data target: `a1 00 30 00 00 c3` versus `a1 04 30 00 00 c3`, memory[3000]=1, memory[3004]=2. Original operand displayed `<OFFSET1>`, rebuilt operand `Other (DATA)`. Historical `is_codegen_equivalent_diff` returns true despite returns1/2. Exact reproduction inputs/results in JSON. This tests its unresolved-symbol wildcard, not register renaming.

Actual cohort condition requiring proof: `CAI::SetNetworkTrapDoors(count=0, first=-1, ...)`. Original first biased load at49cf50 initially reads0; rebuilt473180 initially contains bytes `65 20 53 74` →0x74532065. The rebuilt address belongs before its array; original overlaps mutable `g_wLocalLemmingIndex`. Valid rows1..4 are identical. Source callers include `CAI::SetNetworkTrapDoorCount` and `CTrapDoorManager::LoadLevel`; target itself checks no bound. No reachable invalid-default call proved. Classification: unresolved input contract, not established valid-input mismatch.

Therefore: remove the flattened-diff and unknown-target predicates; replace their verified cases with source/mapping/engine facts. Retain upstream register/address equivalence. Preserve all concrete residual equivalence findings. No case rejected solely because of a hypothetical risk.

## Closing the remaining gap

* **16 scheduling functions / 3,194 bytes:** proofs below. Recover source initialization/copy/call-expression shape, or add a shared dependency checker with byte-range alias facts, subregister/flag liveness, stack-depth tracking, and explicit zero-argument callee facts. Existing reccmp movement checks cannot prove those facts. No register forcing or instruction-order wildcard justified.
* **1 interior-string function / 1,350 bytes:** represent the exact +4 offset inside the mapped18-byte parent literal on both sides. Preserve extent, encoding, bytes, and offset; reject other offsets/content. Linked CRT supplies this body; no reconstructed source setter exists to change.
* **1 array function / 168 bytes:** prove every default-path call has count1..4, including resource-derived count and later count setters. Then represent all four exact negative addends relative to the mapped64-byte table. Do not rename the unrelated preceding globals or change data layout to hide the issue. If an invalid default-path call is reachable, first reconstruct its actual behavior.

Zero disproven valid-input functions. Remaining evidence identifies exact tasks; it does not prove source recovery impossible.

## Per-function ledger

`A`: normalized assembly exact. `E`: shared-engine effective. “Equivalent unresolved”: proof established; matcher still partial. Exact instruction pairs/bytes and function-specific proof in JSON.

| Original address | Function | Bytes | Before % | After % | Disposition | Difference class |
|---|---|---:|---:|---:|---|---|
| `0x40ecb0` | CPlayerLemming::CPlayerLemming | 212 | 92.98 | 92.98 | equivalent_unresolved | initializer scheduling |
| `0x40fa80` | CPlayerLemming::Resurrect | 351 | 94.19 | 94.19 | equivalent_unresolved | initializer scheduling |
| `0x4131e0` | CAI::SetNetworkTrapDoors | 168 | 95.65 | 95.65 | evidence_unresolved | negative array addend |
| `0x421ab0` | CBall::Delete | 133 | 97.92 | 97.92 | equivalent_unresolved | flags scheduling |
| `0x4368f0` | C2D::GetClipRectangle | 164 | 91.18 | 91.18 | equivalent_unresolved | stack/member scheduling |
| `0x4383c0` | C2D::OnDriverChange | 313 | 94.81 | 94.81 | equivalent_unresolved | stack/member scheduling |
| `0x439580` | CBitmap::CBitmap | 41 | 72.73 | 100 E | recovered | source initializer order |
| `0x43ff70` | C2D::SortViewData | 134 | 77.27 | 100 E | recovered | linker function pointer; register allocation |
| `0x445420` | CBaseFrontendDrawer::CBaseFrontendDrawer | 454 | 94.06 | 100 E | recovered | initializer scheduling; dead construction vptr |
| `0x447120` | CBitmapRes::CBitmapRes | 55 | 85.71 | 85.71 | equivalent_unresolved | vptr scheduling |
| `0x44a970` | CPreviewDrawer::tagPRIMS::tagPRIMS | 55 | 85.71 | 85.71 | equivalent_unresolved | vptr scheduling |
| `0x44d8e0` | CGunController::MoveDown | 175 | 92.16 | 92.16 | equivalent_unresolved | epilogue scheduling |
| `0x451110` | CSuccFailDrawer::tagPRIMS::tagPRIMS | 108 | 84.00 | 84.00 | equivalent_unresolved | vptr scheduling |
| `0x457410` | CDirectDrawDriver::CDirectDrawDriver | 705 | 98.53 | 98.53 | equivalent_unresolved | call preparation scheduling |
| `0x459520` | INIT_QuitSubSystems | 165 | 97.22 | 97.22 | equivalent_unresolved | call preparation scheduling |
| `0x45aab0` | _STAT_Quit | 50 | 88.24 | 88.24 | equivalent_unresolved | call preparation scheduling |
| `0x45f820` | CReadSocket::CReadSocket | 123 | 96.97 | 100 A | recovered | virtual-base tables |
| `0x45fce0` | CWriteSocket::CWriteSocket | 157 | 97.62 | 100 A | recovered | virtual-base tables |
| `0x460350` | CBroadcast::CBroadcast | 399 | 91.11 | 100 A | recovered | virtual-base tables; nested vtable identity |
| `0x4604e0` | CBroadcast::~CBroadcast | 207 | 91.07 | 100 E | recovered | nested vtable identity; commuted address sum |
| `0x460a90` | CConnect::CConnect | 363 | 90.00 | 100 A | recovered | virtual-base tables; nested vtable identity |
| `0x460c00` | CConnect::~CConnect | 92 | 78.26 | 100 E | recovered | nested vtable identity; commuted address sum |
| `0x466440` | CPVBackBuffSurface::CPVBackBuffSurface | 106 | 96.55 | 100 A | recovered | virtual-base tables |
| `0x466670` | CPVZBuffSurface::CPVZBuffSurface | 106 | 93.10 | 100 E | recovered | virtual-base tables; commuted address sum |
| `0x469a50` | CText::Set | 47 | 76.92 | 76.92 | equivalent_unresolved | stack/member scheduling |
| `0x469ac0` | CText::Set | 56 | 80.00 | 80.00 | equivalent_unresolved | stack/member scheduling |
| `0x46fcf0` | CTCPIPCommonSocket::CTCPIPCommonSocket | 122 | 93.55 | 100 E | recovered | virtual-base tables; commuted address sum |
| `0x470030` | CTCPIPWriteSocket::CTCPIPWriteSocket | 183 | 93.48 | 100 A | recovered | virtual-base tables |
| `0x470270` | CTCPIPBroadcast::CTCPIPBroadcast | 619 | 95.62 | 100 A | recovered | nested vtable identity |
| `0x4704e0` | CTCPIPBroadcast::~CTCPIPBroadcast | 153 | 95.45 | 100 A | recovered | nested vtable identity |
| `0x4715e0` | CTCPIPNetwork::GetNewConnect | 554 | 87.29 | 100 E | recovered | virtual-base tables; nested vtable identity; commuted address sum |
| `0x471830` | CTCPIPRWSocket::CTCPIPRWSocket | 475 | 92.93 | 100 E | recovered | nested vtable identity; commuted address sum |
| `0x47a220` | CFileRWSocket::CFileRWSocket | 506 | 94.90 | 100 E | recovered | nested vtable identity; initializer scheduling |
| `0x47a570` | CFileBroadcast::CFileBroadcast | 916 | 92.72 | 100 A | recovered | virtual-base tables; nested vtable identity |
| `0x47a910` | CFileBroadcast::~CFileBroadcast | 253 | 91.43 | 100 E | recovered | nested vtable identity; commuted address sum |
| `0x47af30` | CFileConnect::CFileConnect | 869 | 92.35 | 100 A | recovered | virtual-base tables; nested vtable identity |
| `0x47b2a0` | CFileConnect::~CFileConnect | 208 | 96.88 | 100 A | recovered | nested vtable identity |
| `0x47c050` | CNetworkWnd::CNetworkWnd | 236 | 95.59 | 95.59 | equivalent_unresolved | vptr scheduling; call preparation scheduling |
| `0x47e940` | CMciMusicDevice::CMciMusicDevice | 369 | 98.08 | 98.08 | equivalent_unresolved | stack/member scheduling |
| `0x47fe20` | _WinMainCRTStartup | 409 | 91.60 | 100 A | recovered | CRT globals |
| `0x47ffc0` | __amsg_exit | 42 | 90.91 | 100 A | recovered | CRT globals |
| `0x480a20` | __ftell_lk | 417 | 98.72 | 100 A | recovered | CRT globals |
| `0x481020` | ___crtLCMapStringW | 524 | 99.54 | 100 A | recovered | CRT globals |
| `0x481270` | ___crtLCMapStringA | 555 | 99.55 | 100 A | recovered | CRT globals |
| `0x4814d0` | ___crtGetStringTypeW | 436 | 99.38 | 100 A | recovered | CRT globals |
| `0x481690` | ___crtGetStringTypeA | 291 | 99.15 | 100 A | recovered | CRT globals |
| `0x4817c0` | __cinit | 48 | 76.92 | 100 A | recovered | CRT globals |
| `0x481a75` | __NLG_Notify1 | 9 | 75.00 | 100 A | recovered | CRT globals |
| `0x481a7e` | __NLG_Notify | 19 | 85.71 | 100 A | recovered | CRT globals |
| `0x481aa0` | __XcptFilter | 335 | 96.36 | 100 A | recovered | CRT globals |
| `0x481d60` | __ismbbkana | 42 | 92.86 | 100 A | recovered | CRT globals |
| `0x481dd0` | __setenvp | 219 | 95.06 | 100 A | recovered | CRT globals |
| `0x481eb0` | __setargv | 155 | 90.57 | 100 A | recovered | CRT globals |
| `0x482130` | ___crtGetEnvironmentStringsW | 509 | 99.01 | 100 A | recovered | CRT globals |
| `0x4824d0` | __setmbcp | 547 | 94.21 | 100 A | recovered | CRT globals; array interior address |
| `0x4827e0` | __getmbcp | 6 | 50.00 | 100 A | recovered | CRT globals |
| `0x482800` | __ioinit | 475 | 93.38 | 100 A | recovered | CRT globals |
| `0x4829e0` | __ioterm | 85 | 94.29 | 100 A | recovered | CRT globals |
| `0x482a40` | __mtinit | 96 | 94.12 | 100 A | recovered | CRT globals |
| `0x482aa0` | __mtterm | 48 | 75.00 | 100 A | recovered | CRT globals |
| `0x482af0` | __getptd | 111 | 94.87 | 100 A | recovered | CRT globals |
| `0x482b60` | __freeptd | 161 | 94.74 | 100 A | recovered | CRT globals |
| `0x482d40` | __FF_MSGBANNER | 61 | 88.89 | 100 A | recovered | CRT globals |
| `0x482d80` | __NMSG_WRITE | 494 | 97.48 | 100 A | recovered | CRT globals |
| `0x483030` | __openfile | 526 | 98.70 | 100 A | recovered | CRT globals |
| `0x483330` | __close | 110 | 94.59 | 100 A | recovered | CRT globals |
| `0x4833a0` | __close_lk | 137 | 98.04 | 100 A | recovered | CRT globals |
| `0x483470` | __filbuf | 245 | 97.67 | 100 A | recovered | CRT globals |
| `0x483570` | __read | 120 | 95.12 | 100 A | recovered | CRT globals |
| `0x4835f0` | __read_lk | 556 | 99.49 | 100 A | recovered | CRT globals |
| `0x483820` | __commit | 162 | 96.36 | 100 A | recovered | CRT globals |
| `0x4838d0` | __write | 120 | 95.12 | 100 A | recovered | CRT globals |
| `0x483950` | __write_lk | 507 | 99.40 | 100 A | recovered | CRT globals |
| `0x483b50` | $CRT_C_Initializer | 199 | 98.25 | 100 A | recovered | CRT globals |
| `0x483c40` | __flsbuf | 333 | 98.39 | 100 A | recovered | CRT globals |
| `0x483d90` | __lseek | 120 | 95.12 | 100 A | recovered | CRT globals |
| `0x483e10` | __lseek_lk | 130 | 97.87 | 100 A | recovered | CRT globals |
| `0x484130` | __sopen | 1117 | 99.22 | 100 A | recovered | CRT globals |
| `0x484590` | __alloc_osfhnd | 326 | 96.08 | 100 A | recovered | CRT globals |
| `0x4846e0` | __set_osfhnd | 164 | 94.83 | 100 A | recovered | CRT globals |
| `0x484790` | __free_osfhnd | 145 | 94.12 | 100 A | recovered | CRT globals |
| `0x484830` | __get_osfhandle | 74 | 90.48 | 100 A | recovered | CRT globals |
| `0x484880` | __open_osfhandle | 180 | 98.51 | 100 A | recovered | CRT globals |
| `0x484940` | __lock_fhandle | 97 | 97.22 | 100 A | recovered | CRT globals |
| `0x4849b0` | __unlock_fhandle | 40 | 92.31 | 100 A | recovered | CRT globals |
| `0x4849e0` | __getbuf | 75 | 95.24 | 100 A | recovered | CRT globals |
| `0x484ad0` | __isatty | 48 | 87.50 | 100 A | recovered | CRT globals |
| `0x484b00` | __assert | 1350 | 99.30 | 99.77 | equivalent_unresolved | CRT globals; string interior address |
| `0x485050` | __chsize | 104 | 94.59 | 100 A | recovered | CRT globals |
| `0x485270` | _setvbuf | 204 | 98.61 | 100 A | recovered | CRT globals |
| `0x4855b0` | _raise | 477 | 97.69 | 100 A | recovered | CRT globals |
| `0x485920` | __setmode | 104 | 94.59 | 100 A | recovered | CRT globals |
| `0x485990` | __setmode_lk | 108 | 97.14 | 100 A | recovered | CRT globals |
| `0x485a30` | __stbuf | 146 | 96.36 | 100 A | recovered | CRT globals |
| `0x485b10` | __output | 2522 | 99.48 | 100 A | recovered | CRT globals; relocated string pointers |
| `0x486630` | __wctomb_lk | 148 | 97.73 | 100 A | recovered | CRT globals |

## Residual proofs

* `0x40ecb0` **CPlayerLemming::CPlayerLemming** — ECX/EDX argument loads and stores at this+0xb6/+0x174 move across disjoint stores. EAX shifts, arguments, call boundary and final fields agree; no crossed use of ECX/EDX.
* `0x40fa80` **CPlayerLemming::Resurrect** — Stores at +0xc4/+0x120/+0x6e move among disjoint object fields; EAX=0xffff moves before unrelated zero stores. PUSH 0xfa0 crosses only object stores. Same state before virtual call40fb57; no crossed observer.
* `0x4131e0` **CAI::SetNetworkTrapDoors** — Four loads use table-16+16*count. Original table49cf60 and rebuilt473190: all64 bytes equal. Original biased bases49cf50/49cf58 coincide with unrelated globals. Default-path count in1..4 proves equivalence. Caller bound not established. Concrete entry count=0,first=-1 reads original0 versus rebuilt0x74532065 from first biased dword. Reachability unresolved; no valid-input mismatch claimed.
* `0x421ab0` **CBall::Delete** — XOR EDI,EDI moves across two loads and ADD ECX,0x118. Neither uses EDI. CMP EAX,EDI overwrites flags before JLE; same inputs and branch result.
* `0x4368f0` **C2D::GetClipRectangle** — Two temporary WORD stores [esp+8]/[esp+0xa] move before loads from disjoint [esp+4]/[esp+6]. ClipRectangle store moves across DX load. All four final coordinates and return pointer agree; ESP unchanged inside region.
* `0x4383c0` **C2D::OnDriverChange** — WORD temporaries at esp+0x10/+0x12 move before disjoint loads/stores at esp+4..+0xe. AX/CX/DX definitions reach the same uses. Identical rectangle and arguments at SetRectInnerZoom call4384e0.
* `0x447120` **CBitmapRes::CBitmapRes** — CBitmapResBase and CBitmapRes vptr stores move across WORD coordinate zero stores at +4/+8/+0xa. Vptr order preserved; disjoint destinations; no call/read/branch between stores. Final object identical.
* `0x44a970` **CPreviewDrawer::tagPRIMS::tagPRIMS** — Same CBitmapRes inlining as447120: both vptr stores move across disjoint coordinate zero stores; no observer; final object identical.
* `0x44d8e0` **CGunController::MoveDown** — Store this+90 moves before SUB EAX,ECX without changing ECX. POP EBP/EDI/ESI move across arithmetic using EAX/EDX and stores through EBX. Pop order and final ESP preserved; no crossed use of restored registers or ESP.
* `0x451110` **CSuccFailDrawer::tagPRIMS::tagPRIMS** — Two CBitmapRes objects at this and this+24. Each vptr sequence retains order while moving across disjoint WORD zero stores; no observer before constructor returns.
* `0x457410` **CDirectDrawDriver::CDirectDrawDriver** — LEA EDI,[ESI+1c] moves across unrelated initialization. COM pointer and vptr loads move across stack descriptor/argument stores, never a call. Valid DirectDraw object separate from caller stack; identical CreateSurface receiver, vtable slot+18, descriptor and arguments.
* `0x459520` **INIT_QuitSubSystems** — Master-arena load moves across PUSH newline, error-stream load and PUSH stream. Globals only read; ECX unchanged by crossed operations. Identical receiver and arguments at virtual call45959f.
* `0x45aab0` **_STAT_Quit** — System-output load/PUSH moves before manager/vptr loads. No crossed stack-relative reads or side effects. Same manager receiver, stream argument and virtual call; later cleanup unchanged.
* `0x469a50` **CText::Set** — Font/flags stores interleave differently with stack argument loads. Distinct object members +4/+8/+10/+14; arguments by value; no intervening call. Same six assigned fields and RET18.
* `0x469ac0` **CText::Set** — Same scheduling issue as469a50, with flags/remap argument positions changed by CString value parameter. Same stores, CString destructor receiver esp+10, and RET1c.
* `0x47c050` **CNetworkWnd::CNetworkWnd** — Vptr moves across disjoint zero stores +8/+c. PUSH newline moves before no-argument GetLastError; WINAPI callee consumes no arguments. PUSH returned error crosses only global stream load. Identical log arguments and object result.
* `0x47e940` **CMciMusicDevice::CMciMusicDevice** — MOV EBX,window-name and stack store esp+28 move across disjoint WNDCLASS stores/LEA EAX,[esp+c]. No crossed EBX use or ESP mutation; RegisterClassA receives identical WNDCLASS.
* `0x484b00` **__assert** — CRT globals now mapped. Remaining original4a51bc and rebuilt4799d4 are parent+4 of mapped18-byte Assertion failed! string; identical14-byte suffix. Original suffix is a standalone STRING entity; rebuilt lacks it. Upstream create_name_lookup excludes STRING offsets. Same loaded dword; matcher unresolved.

## Validation / commits

* Incremental build passes. All target gains verified through `tools/match.py`; final complete report checks all3,454 inventory functions.
* `tools/gate.py`: passes,126 tests; smell/annotation/layout/naming checks pass.
* `tools/gate.py --vtable`: all annotated tables/slots/adjusters pass.
* Prior baseline exact and effective sets audited: zero losses. Header changes also inspected with graph `detect_changes`; direct source/reccmp audit supplied coverage where graph metadata was stale or partial.
* `tools/report.py`: unchanged; existing objdiff-v2 tests pass. Audit data is not an input to score computation.
* `2dea5c48`: bitmap source order; first global/table identities.
* `d076b9fc`: CRT globals.
* `f7323513`: shared full-path vtable identity; fewer implementation lines.
* `4acb064c`: virtual-base table annotations.
* `8055cc9`: linker function-pointer recognition.
* `18cb5c41`: compose existing proofs with upstream matching.
