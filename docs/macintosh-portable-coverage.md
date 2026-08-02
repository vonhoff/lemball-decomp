# Portable Macintosh function coverage audit

This tracked ledger is updated with every accepted correlation batch.

## Result

This audit does **not** establish complete portable coverage. Only accepted correlations are represented contracts; lexical and inline candidates require body/ABI proof.

| Category | Count | Meaning |
| --- | ---: | --- |
| Accepted x86 correlation | 837 | Exact Macintosh module+mangled spelling has a reviewed x86 mapping; offset integrity is reported separately. |
| Present spelling/source analogue | 8 | Exact scoped spelling exists in current source, but equivalence is not proven. |
| Likely inlined/merged | 0 | Exact scoped header definition exists, but no standalone x86 address is accepted. |
| Platform-specific | 883 | Unmapped body belongs to `Visos (Mac Specific)` or the reviewed Macintosh Toolbox/Open Transport `UtilMain` set. |
| Genuinely missing/unresolved | 1121 | No accepted address and no exact scoped source spelling. |
| **Total** | **2849** | Complete raw inventory accounting. |

### Accepted-correlation referential integrity

The accepted table has **772** rows, but only **748** match an exact raw module+mangled spelling. Of all accepted rows: **725** match the full raw tuple, **23** match name+module but carry a different trailer offset, and **24** have no exact raw name in that module. The latter are family-level/alternate-sibling claims, not coverage of an exact raw symbol.

This is why the 772-row accepted table must not be reported as 256 exact raw-symbol contracts. See `data/macintosh-correlation-integrity.csv`.

The non-Mac-specific target contains 1958 symbols after excluding the 12 reviewed `UtilMain` Toolbox/resolver bodies. Ten accepted cross-platform analogues found inside the Macintosh-specific module remain represented separately.

## Mechanical accounting by module

| Module | accepted_x86_correlation | present_spelling_or_source_analogue | likely_inlined_or_merged | platform_specific | genuinely_missing_or_unresolved | Total |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| AI | 514 | 0 | 0 | 0 | 270 | 784 |
| Control | 57 | 0 | 0 | 2 | 0 | 59 |
| Frontend | 3 | 0 | 0 | 0 | 237 | 240 |
| Map | 20 | 0 | 0 | 0 | 0 | 20 |
| Network | 0 | 0 | 0 | 0 | 24 | 24 |
| UtilMain | 0 | 0 | 0 | 12 | 0 | 12 |
| Visos (Generic) | 57 | 8 | 0 | 0 | 561 | 626 |
| Visos (Mac Specific) | 10 | 0 | 0 | 869 | 0 | 879 |
| views_2d | 87 | 0 | 0 | 0 | 118 | 205 |

## Highest-priority unresolved coherent families

Priority is unresolved cardinality, not a claim of equivalence. Families must be resolved as complete ordered contracts with body, ABI, callers, vtable slots, and target TU ownership.

| Rank | Module | Exact class/owner | Unresolved symbols |
| ---: | --- | --- | ---: |
| 1 | AI | `CAI` | 47 |
| 2 | AI | `CPlayerLemming` | 39 |
| 3 | Frontend | `CNetworkOptionsDrawer` | 31 |
| 4 | AI | `CEnemy` | 26 |
| 5 | AI | `CGenericGroup` | 26 |
| 6 | AI | `CPlayerLemmingGroupManager` | 26 |
| 7 | AI | `CGenericGroupManager` | 25 |
| 8 | Visos (Generic) | `CNetworkMessage` | 25 |
| 9 | Visos (Generic) | `CBaseNetwork` | 23 |
| 10 | Frontend | `CBaseFrontendDrawer` | 21 |
| 11 | views_2d | `CMain2DDisplay` | 21 |
| 12 | Frontend | `CPreviewDrawer` | 20 |
| 13 | Visos (Generic) | `CSoundManager` | 20 |
| 14 | views_2d | `CPauseWindow` | 20 |
| 15 | Visos (Generic) | `CBaseSoundDevice` | 19 |
| 16 | Visos (Generic) | `CPVButton` | 18 |
| 17 | views_2d | `C2D` | 18 |
| 18 | Frontend | `CGunController` | 17 |
| 19 | AI | `CMover` | 16 |
| 20 | Frontend | `CFrontendResourceLoader` | 16 |
| 21 | Frontend | `CPasswordDrawer` | 16 |
| 22 | Map | `CMap` | 16 |
| 23 | AI | `CPlayerLemmingGroup` | 15 |
| 24 | Frontend | `CHiliteController` | 15 |
| 25 | Visos (Generic) | `CChangeList` | 15 |
| 26 | Visos (Generic) | `CMogRes` | 15 |
| 27 | Visos (Generic) | `CWriteSocket` | 15 |
| 28 | AI | `CObjectManager` | 14 |
| 29 | Visos (Generic) | `CBaseQueue` | 14 |
| 30 | Visos (Generic) | `CBroadcast` | 14 |
| 31 | views_2d | `CLemmingAnimsManager` | 14 |
| 32 | Frontend | `CSuccFailDrawer` | 13 |
| 33 | Visos (Generic) | `CConnect` | 13 |
| 34 | Visos (Generic) | `CPVWnd` | 13 |
| 35 | AI | `CBulletManager` | 12 |
| 36 | AI | `CGlobalGameObject` | 12 |
| 37 | AI | `CMaze` | 12 |
| 38 | Network | `CNetworkManager` | 12 |
| 39 | Visos (Generic) | `CFileBroadcast` | 12 |
| 40 | Visos (Generic) | `CReadSocket` | 12 |

Unresolved global/runtime-shaped symbols are tracked separately because they are not a coherent class family: AI=58, Control=12, Frontend=1, Visos (Generic)=42, views_2d=1.

## Existing-source candidate families requiring proof

| Module | Exact class/owner | Candidate symbols |
| --- | --- | ---: |
| Visos (Generic) | `CArena` | 7 |
| Visos (Generic) | `<global>` | 1 |

## Durable coverage contract

1. `data/macintosh-symbol-coverage.csv` is the row-complete ledger, keyed by `(mac_code_file, mac_name_length_offset, mac_mangled_name)`.
2. Add explicit per-row disposition (`accepted_x86_correlation`, `present_source_analogue`, `inlined_or_merged`, `platform_specific`, `unresolved`) plus evidence and confidence. Do not derive permanent platform exclusions from spelling or order.
3. For accepted bodies, require x86 address, canonical `objdiff-functions.csv` unit, physical reconstructed source path or `unreconstructed`, exact Windows symbol/body identity, and behavioral evidence.
4. For inline/merged rows, require the consuming Windows body/address and compiler/disassembly evidence proving no standalone body; a header spelling alone remains only a candidate.
5. For platform-specific rows, require an explicit reviewed exclusion reason finer than module ownership when closing the contract; record any Windows platform analogue separately without claiming shared implementation.
6. `tools/macintosh_naming.py check` enforces 2,849-row key coverage, accepted-correlation consistency, category vocabulary, and unique accepted x86 addresses. Completion additionally requires zero unresolved or lexical-only portable rows.
7. Add family rollups keyed by exact module and class/owner so every constructor/destructor, overload, virtual slot, thunk, and target-only helper is cardinality-accounted.

## Method and limitations

Accepted raw rows are joined by exact module and mangled spelling; trailer-offset agreement is audited separately and never hidden. Source candidates come only from exact C++ scoped spellings in comment-stripped files at the audited commit. Name/order alone never promotes a candidate. The checked-in ledger records unresolved work explicitly rather than treating correlation count as completion.
