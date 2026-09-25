# reccmp compatibility review

Baseline: `3c8328a4`. Date: 2026-09-25. Pinned package: reccmp 0.1.7.
Ponytail: remove unused behavior; share existing machinery; prefer evidenced annotations.

## Decisions

| Mechanism | Finding / action |
|---|---|
| Local forward MOV/LEA relocation rule | Ablation over the complete build: zero effective matches depend on it. Deleted rule, hook, register regex and four dedicated tests. Retained upstream movement checker. |
| CMP-zero versus TEST | Five real beneficiaries. Old backward text scan unsound: branch can bypass the zeroing instruction. Replaced with decoded register writes and branch/table-entry invalidation. Calls preserve only Win32 nonvolatile registers. Reject AF observers and unknown indirect control flow. |
| Composition with upstream | Retained bounded substitutions. Use upstream FunctionComparator for both comparisons, with the original split points. Return its original raw diff/ratio; only its effective result may change. Removed global find_effective_match replacement and second unpinned matcher. |
| Linker thunks | Retained verified section-leading E9 table and mapped destinations. Calls, jumps, address operands and indirect calls share name lookup; upstream owns replacement caching. Arbitrary E9 following stays restricted to direct call/jump targets. Data merely beginning with E9 is not renamed. |
| Vtable gate E9 decoder | Deleted duplicate decoder; use shared direct_jump_target. Existing cycle, depth, named-body and destination checks retained. |
| CLoadUpdate transient vptr | Annotation alternative verified. Added original VTABLE 0x496cd8; constructor now assembly exact, no overwrite exception. |
| CFrames transient vptr | Retain local overwrite proof. Original 0x496d00 slot is __purecall at0x47fca0; live CFrames table0x496d04 points through0x402a59 to GetFrameNo0x435880. They are different tables. Relabeling the transient table as the live one would be wrong. |
| Extent, jump-table, relocation, CRT-string and nested-vtable repairs | Retained. Extent reader has a live tools/next.py caller. Stripped debug flags hide real linker thunks/asserts; MSVC vtable display names lose inheritance paths; CRT pointer addends require exact relocation/base identities. Source annotations do not repair these parser limitations. |
| Operand/token checks | Retained exact operands and quoted-string exclusions. Existing tests demonstrate changed offsets, identifiers, literals and TEST branches that upstream accepts incorrectly. |

## Concrete false positive closed

Input EAX=EBP=1. Two complete bodies:

```text
original: eb02 31ed 3bc5 7506 b801000000 c3 b802000000 c3
rebuilt:  eb02 31ed 85c0 7506 b801000000 c3 b802000000 c3
          JMP skips XOR EBP,EBP
          CMP EAX,EBP versus TEST EAX,EAX
original returns 1; rebuilt returns 2
```

Old local checker returned true. Revised byte-fixture test rejects it. Also checks a backward branch into the comparison, subregister writes, call-clobbered ECX, AF observation after Jcc and unknown indirect control flow. No actual audited function is classified as disproven by this fixture.

Five actual zero-comparison beneficiaries remain 100% effective: 0x40f500, 0x41dfc0, 0x41ecb0, 0x44fc50, 0x472ce0. Original EXE/Ghidra evidence for SwapElements: XOR EBP at0x41dfd6 dominates CMP EDX,EBP at0x41dfdc. Source already compares against its initialized loop index. Prior source operand-order trials produced the same code (reconstruction-memory attempt3760); no register forcing added.

After annotation, six functions still need the bounded overwrite proof: 0x445420, 0x4455f0, 0x4491b0, 0x44c870, 0x44f440, 0x451210. Conditions: same destination; overwrite within three identical following instructions; no call, branch or read; intervening stores disjoint, LEA preserves base. No unknown-target wildcard.

## Annotation evidence

`7f034209`: CLoadUpdate.

* Ghidra/original constructor432b50: store496cd8 to this+70 at432b8a; replace with derived table496cdc at432b9b.
* Original496cd8: `a0fc4700`, sole slot __purecall47fca0.
* PDB `??_7CLoadUpdate@@6B@`: rebuilt46fe78, size4, `704e4600`; slot464e70 mapped to the same __purecall.
* `tools/match.py 0x00432b50`: 98.1132075% raw / 100% effective -> 100% assembly exact, 208 bytes. Only an annotation added; no source behavior change.

## Verification

* Incremental build and clang-format pass. Graph detect_changes plus full before/after report audit after header edit.
* Seven focused tools/match.py checks retain100%: five zero comparisons, BaseFrontendDrawer445420, SortViewData43ff70. CLoadUpdate constructor verified separately with build.
* All3454 inventory functions checked: zero prior exact/effective losses. Sole ratio/status change: constructor432b50 effective -> assembly exact.
* Assembly exact: 2699 -> 2700 functions; 199995 -> 200203 bytes.
* Effective unchanged: 3032 functions; 269054/405552 bytes = **66.3426638261%**.
* tools/gate.py --all:127 tests;407/407 vtables;6352/6352 slots;173/173 adjusters. Five previously documented orphan stores unchanged.
* tools/report.py unchanged; objdiff v2 checks pass.
* Ponytail net deletion:24 production-tool lines and9 test lines, excluding this evidence document. No new tool entry point.

Original96-function cohort unchanged:78 recovered/22478 bytes;17 proven equivalent but matcher unresolved/4544 bytes;1 evidence unresolved/168 bytes;0 disproven. Remaining closure conditions stay in [PROMOTION_AUDIT.md](PROMOTION_AUDIT.md#closing-the-remaining-gap): scheduling/alias proofs, exact interior-string mapping, and default-array input-bound evidence. Historical JSON remains the preserved original audit snapshot.
