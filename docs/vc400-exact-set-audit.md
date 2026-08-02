# VC4.00 exact-set regression audit

A fresh-build audit used Microsoft Visual C++ 4.00 (`cl` 10.00.5270), regenerated full `reccmp.json` at every checkpoint, and left `exact-baseline.json` unchanged.

## Range result

| Checkpoint | Added | Lost |
| --- | ---: | ---: |
| `a110572` | 170 | 17 |
| `efd016d` | 169 | 21 |

Net accounting: five new post-baseline exact matches, six prior-added matches lost, three immutable-baseline matches recovered, and seven immutable-baseline matches lost.

## Permanent attribution

| Commit | Permanent exact-set effect through `efd016d` |
| --- | --- |
| `236135f` | added `0x00423C10` |
| `a5beb36` | added `0x00423EB0` |
| `b8a01c8` | added `0x0046C990` |
| `78640a9` | added `0x00474DC0` |
| `338ae66` | recovered baseline `0x00467210`, `0x0046B3B0`; lost baseline `0x00430A50`, `0x00465050`, `0x0046BA50`; lost prior-added `0x00406BA0`, `0x00430110`, `0x00430B30`, `0x0044D290`, `0x00465E00` |
| `9e839ca` | recovered baseline `0x00447610`; lost baseline `0x00408FE0`, `0x00465D50`, `0x0046B310` |
| `ebed907` | lost prior-added `0x0046BC90` |
| `7341d22` | added `0x0046BF70`; lost baseline `0x004663D0` |

This accounts exactly for `170 + 5 - 6 = 169` added and `17 + 7 - 3 = 21` lost.

## Retention decisions

- Keep the other audited commits: their permanent effects are exact gains, repairs, or neutral.
- Repair `338ae66` and `9e839ca` surgically; wholesale reverts would discard recovered behavior and exact matches.
- Do not reverse `ebed907` wholesale at current `main`: a clean VC4.00 probe recovered `0x0046BC90` but changed the aggregate from `169/21` to `168/21`. The probe was completely reverted. Recover its source shape without displacing another exact match.
- Repair `7341d22` without the old raw-initializer substitution: that probe recovered `0x004663D0` but displaced protected exact function `0x00466BD0`, so it was completely reverted.
- Compare actual added/lost address sets after every full regenerated JSON; aggregate counts alone hide displaced exact matches.

The complete 24-checkpoint audit artifacts remain outside the repository under `C:\Users\simon\vc400-audit-results`.
