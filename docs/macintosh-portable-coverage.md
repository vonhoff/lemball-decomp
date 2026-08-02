# Portable Macintosh function coverage audit

Audit target: `4dfde41` (`4dfde41d7bdddc0627a8cb2791a2f18ab067dbeb`), read-only.

## Result

This audit does **not** establish complete portable coverage. Only accepted correlations are represented contracts; lexical and inline candidates require body/ABI proof.

| Category | Count | Meaning |
| --- | ---: | --- |
| Accepted x86 correlation | 185 | Exact Macintosh module+mangled spelling has a reviewed x86 mapping; offset integrity is reported separately. |
| Present spelling/source analogue | 8 | Exact scoped spelling exists in current source, but equivalence is not proven. |
| Likely inlined/merged | 0 | Exact scoped header definition exists, but no standalone x86 address is accepted. |
| Platform-specific | 881 | Unmapped body belongs to `Visos (Mac Specific)` or the reviewed Macintosh Toolbox/Open Transport `UtilMain` set. |
| Genuinely missing/unresolved | 1775 | No accepted address and no exact scoped source spelling. |
| **Total** | **2849** | Complete raw inventory accounting. |

### Accepted-correlation referential integrity

The accepted table has **209** rows, but only **185** match an exact raw module+mangled spelling. Of all accepted rows: **162** match the full raw tuple, **23** match name+module but carry a different trailer offset, and **24** have no exact raw name in that module. The latter are family-level/alternate-sibling claims, not coverage of an exact raw symbol.

This is why the 209-row accepted table must not be reported as 209 exact raw-symbol contracts. See `data/macintosh-correlation-integrity.csv`.

The non-Mac-specific target contains 1958 symbols after excluding the 12 reviewed `UtilMain` Toolbox/resolver bodies. Ten accepted cross-platform analogues found inside the Macintosh-specific module remain represented separately.

## Mechanical accounting by module

| Module | accepted_x86_correlation | present_spelling_or_source_analogue | likely_inlined_or_merged | platform_specific | genuinely_missing_or_unresolved | Total |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| AI | 54 | 0 | 0 | 0 | 730 | 784 |
| Control | 21 | 0 | 0 | 0 | 38 | 59 |
| Frontend | 3 | 0 | 0 | 0 | 237 | 240 |
| Map | 0 | 0 | 0 | 0 | 20 | 20 |
| Network | 0 | 0 | 0 | 0 | 24 | 24 |
| UtilMain | 0 | 0 | 0 | 12 | 0 | 12 |
| Visos (Generic) | 46 | 8 | 0 | 0 | 572 | 626 |
| Visos (Mac Specific) | 10 | 0 | 0 | 869 | 0 | 879 |
| views_2d | 51 | 0 | 0 | 0 | 154 | 205 |

## Highest-priority unresolved coherent families

Priority is unresolved cardinality, not a claim of equivalence. Families must be resolved as complete ordered contracts with body, ABI, callers, vtable slots, and target TU ownership.

| Rank | Module | Exact class/owner | Unresolved symbols |
| ---: | --- | --- | ---: |
| 1 | views_2d | `C2D` | 54 |
| 2 | AI | `CAI` | 47 |
| 3 | AI | `CPlayerLemming` | 39 |
| 4 | Frontend | `CNetworkOptionsDrawer` | 31 |
| 5 | AI | `CEnemy` | 26 |
| 6 | AI | `CGenericGroup` | 26 |
| 7 | AI | `CPlayerLemmingGroupManager` | 26 |
| 8 | Visos (Generic) | `CNetworkMessage` | 25 |
| 9 | AI | `CGenericGroupManager` | 25 |
| 10 | Visos (Generic) | `CBaseNetwork` | 23 |
| 11 | Frontend | `CBaseFrontendDrawer` | 21 |
| 12 | views_2d | `CMain2DDisplay` | 21 |
| 13 | Visos (Generic) | `CSoundManager` | 20 |
| 14 | Frontend | `CPreviewDrawer` | 20 |
| 15 | views_2d | `CPauseWindow` | 20 |
| 16 | Visos (Generic) | `CBaseSoundDevice` | 19 |
| 17 | Visos (Generic) | `CPVButton` | 18 |
| 18 | Frontend | `CGunController` | 17 |
| 19 | AI | `CMover` | 16 |
| 20 | Frontend | `CPasswordDrawer` | 16 |
| 21 | Frontend | `CFrontendResourceLoader` | 16 |
| 22 | Map | `CMap` | 16 |
| 23 | Visos (Generic) | `CMogRes` | 15 |
| 24 | Visos (Generic) | `CWriteSocket` | 15 |
| 25 | Visos (Generic) | `CChangeList` | 15 |
| 26 | AI | `CPlayerLemmingGroup` | 15 |
| 27 | Frontend | `CHiliteController` | 15 |
| 28 | Visos (Generic) | `CBaseQueue` | 14 |
| 29 | Visos (Generic) | `CBroadcast` | 14 |
| 30 | AI | `CObjectManager` | 14 |
| 31 | views_2d | `CLemmingAnimsManager` | 14 |
| 32 | Visos (Generic) | `CConnect` | 13 |
| 33 | Visos (Generic) | `CPVWnd` | 13 |
| 34 | Frontend | `CSuccFailDrawer` | 13 |
| 35 | Visos (Generic) | `CResBase` | 12 |
| 36 | Visos (Generic) | `CFileBroadcast` | 12 |
| 37 | Visos (Generic) | `CReadSocket` | 12 |
| 38 | Visos (Generic) | `CVSOStream` | 12 |
| 39 | AI | `CBulletManager` | 12 |
| 40 | AI | `CGlobalGameObject` | 12 |

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
