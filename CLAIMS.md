# Active claims

Claim one range with `python tools/claims.py take RANGE OWNER`. Available
ranges come from `data/work-ranges.csv`; list them with
`python tools/claims.py list --available`.

Function ownership follows original entry address. Cross-range blockers use
`claims.py dependency`. Shared class, vtable, inheritance, or global changes
require `claims.py take-abi`.

| Range | Addresses | Functions | Who | Claimed |
|---|---|---:|---|---|
