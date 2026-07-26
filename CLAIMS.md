# Active claims

Claim a range before starting so two people or AI sessions do not grind
the same functions. Keep one active code range per worker. Commit a claim
alone before source work. Mark it released when done or stopped.

Ranges are stable half-open intervals. Function ownership follows entry
address. Shared class, vtable, inheritance, and global edits also require
an address-anchored ABI lease through `tools/claims.py take-abi`.
Cross-range calls never widen ownership; record blockers through
`tools/claims.py dependency` for coordinator routing.

## Code ranges

| Range | Addresses | Functions | Who | Claimed | Status |
|---|---|---:|---|---|---|
| text-001 | 0x00401005..0x00401212 | 64 |  |  | available |
| text-002 | 0x00401212..0x00401415 | 64 |  |  | available |
| text-003 | 0x00401415..0x00401613 | 64 |  |  | available |
| text-004 | 0x00401613..0x004017F3 | 64 |  |  | available |
| text-005 | 0x004017F3..0x004019CE | 64 |  |  | available |
| text-006 | 0x004019CE..0x00401C35 | 64 |  |  | available |
| text-007 | 0x00401C35..0x00401E47 | 64 |  |  | available |
| text-008 | 0x00401E47..0x00402036 | 64 |  |  | available |
| text-009 | 0x00402036..0x0040225C | 64 |  |  | available |
| text-010 | 0x0040225C..0x00402450 | 64 |  |  | available |
| text-011 | 0x00402450..0x00402676 | 64 |  |  | available |
| text-012 | 0x00402676..0x0040286A | 64 |  |  | available |
| text-013 | 0x0040286A..0x00402A36 | 64 |  |  | available |
| text-014 | 0x00402A36..0x00402C20 | 64 |  |  | available |
| text-015 | 0x00402C20..0x00402DF1 | 64 |  |  | available |
| text-016 | 0x00402DF1..0x00402FC2 | 64 |  |  | available |
| text-017 | 0x00402FC2..0x0040318E | 64 |  |  | available |
| text-018 | 0x0040318E..0x0040336E | 64 |  |  | available |
| text-019 | 0x0040336E..0x00403558 | 64 |  |  | available |
| text-020 | 0x00403558..0x00403706 | 64 |  |  | available |
| text-021 | 0x00403706..0x004071D0 | 64 |  |  | available |
| text-022 | 0x004071D0..0x00408830 | 31 |  |  | available |
| text-023 | 0x00408830..0x0040A020 | 37 |  |  | available |
| text-024 | 0x0040A020..0x0040B0D0 | 64 |  |  | available |
| text-025 | 0x0040B0D0..0x0040CA40 | 42 |  |  | available |
| text-026 | 0x0040CA40..0x0040E060 | 31 |  |  | available |
| text-027 | 0x0040E060..0x0040FA80 | 41 |  |  | available |
| text-028 | 0x0040FA80..0x004117A0 | 27 |  |  | available |
| text-029 | 0x004117A0..0x00412AD0 | 19 |  |  | available |
| text-030 | 0x00412AD0..0x00414960 | 51 |  |  | available |
| text-031 | 0x00414960..0x00415D90 | 29 |  |  | available |
| text-032 | 0x00415D90..0x00417150 | 61 |  |  | available |
| text-033 | 0x00417150..0x00419060 | 59 |  |  | available |
| text-034 | 0x00419060..0x0041AC70 | 49 |  |  | available |
| text-035 | 0x0041AC70..0x0041C530 | 34 |  |  | available |
| text-036 | 0x0041C530..0x0041E3C0 | 58 |  |  | available |
| text-037 | 0x0041E3C0..0x0041FCD0 | 50 |  |  | available |
| text-038 | 0x0041FCD0..0x00421770 | 55 |  |  | available |
| text-039 | 0x00421770..0x00422C00 | 30 |  |  | available |
| text-040 | 0x00422C00..0x00424630 | 43 |  |  | available |
| text-041 | 0x00424630..0x00425C80 | 25 |  |  | available |
| text-042 | 0x00425C80..0x00427630 | 35 |  |  | available |
| text-043 | 0x00427630..0x00428F90 | 35 |  |  | available |
| text-044 | 0x00428F90..0x0042AB90 | 33 |  |  | available |
| text-045 | 0x0042AB90..0x0042C040 | 22 |  |  | available |
| text-046 | 0x0042C040..0x0042D4D0 | 18 |  |  | available |
| text-047 | 0x0042D4D0..0x0042EEB0 | 29 |  |  | available |
| text-048 | 0x0042EEB0..0x00430A20 | 31 |  |  | available |
| text-049 | 0x00430A20..0x004326E0 | 52 |  |  | available |
| text-050 | 0x004326E0..0x00433FB0 | 17 |  |  | available |
| text-051 | 0x00433FB0..0x004363C0 | 18 |  |  | available |
| text-052 | 0x004363C0..0x004376B0 | 25 |  |  | available |
| text-053 | 0x004376B0..0x00439550 | 34 |  |  | available |
| text-054 | 0x00439550..0x0043A4F0 | 64 |  |  | available |
| text-055 | 0x0043A4F0..0x0043BCE0 | 19 |  |  | available |
| text-056 | 0x0043BCE0..0x0043CE30 | 19 |  |  | available |
| text-057 | 0x0043CE30..0x0043E220 | 17 |  |  | available |
| text-058 | 0x0043E220..0x0043F620 | 4 |  |  | available |
| text-059 | 0x0043F620..0x004407E0 | 13 |  |  | available |
| text-060 | 0x004407E0..0x00442F80 | 26 |  |  | available |
| text-061 | 0x00442F80..0x00444680 | 26 |  |  | available |
| text-062 | 0x00444680..0x00445ED0 | 28 |  |  | available |
| text-063 | 0x00445ED0..0x004477B0 | 51 |  |  | available |
| text-064 | 0x004477B0..0x00448EE0 | 36 |  |  | available |
| text-065 | 0x00448EE0..0x0044A250 | 18 |  |  | available |
| text-066 | 0x0044A250..0x0044BC50 | 30 |  |  | available |
| text-067 | 0x0044BC50..0x0044CFB0 | 27 |  |  | available |
| text-068 | 0x0044CFB0..0x0044E940 | 18 |  |  | available |
| text-069 | 0x0044E940..0x00450160 | 41 |  |  | available |
| text-070 | 0x00450160..0x00451610 | 27 |  |  | available |
| text-071 | 0x00451610..0x00452BF0 | 33 |  |  | available |
| text-072 | 0x00452BF0..0x00454050 | 27 |  |  | available |
| text-073 | 0x00454050..0x004552A0 | 27 |  |  | available |
| text-074 | 0x004552A0..0x004570B0 | 48 |  |  | available |
| text-075 | 0x004570B0..0x00458280 | 31 |  |  | available |
| text-076 | 0x00458280..0x004599F0 | 49 |  |  | available |
| text-077 | 0x004599F0..0x0045ADC0 | 62 |  |  | available |
| text-078 | 0x0045ADC0..0x0045C200 | 46 |  |  | available |
| text-079 | 0x0045C200..0x0045D7B0 | 40 |  |  | available |
| text-080 | 0x0045D7B0..0x0045E840 | 64 |  |  | available |
| text-081 | 0x0045E840..0x0045F3D0 | 64 |  |  | available |
| text-082 | 0x0045F3D0..0x004605B0 | 51 |  |  | available |
| text-083 | 0x004605B0..0x00461700 | 46 |  |  | available |
| text-084 | 0x00461700..0x004628D0 | 39 |  |  | available |
| text-085 | 0x004628D0..0x00462F20 | 64 |  |  | available |
| text-086 | 0x00462F20..0x00464220 | 35 |  |  | available |
| text-087 | 0x00464220..0x004654F0 | 15 |  |  | available |
| text-088 | 0x004654F0..0x004666E0 | 51 |  |  | available |
| text-089 | 0x004666E0..0x004679E0 | 48 |  |  | available |
| text-090 | 0x004679E0..0x00468DD0 | 42 |  |  | available |
| text-091 | 0x00468DD0..0x0046A140 | 49 |  |  | available |
| text-092 | 0x0046A140..0x0046B460 | 37 |  |  | available |
| text-093 | 0x0046B460..0x0046C5D0 | 19 |  |  | available |
| text-094 | 0x0046C5D0..0x0046D7E0 | 12 |  |  | available |
| text-095 | 0x0046D7E0..0x0046F210 | 64 |  |  | available |
| text-096 | 0x0046F210..0x004704E0 | 38 |  |  | available |
| text-097 | 0x004704E0..0x004715B0 | 25 |  |  | available |
| text-098 | 0x004715B0..0x00471FC0 | 64 |  |  | available |
| text-099 | 0x00471FC0..0x00473340 | 46 |  |  | available |
| text-100 | 0x00473340..0x004744A0 | 31 |  |  | available |
| text-101 | 0x004744A0..0x00475BC0 | 23 |  |  | available |
| text-102 | 0x00475BC0..0x00476EE0 | 12 |  |  | available |
| text-103 | 0x00476EE0..0x00477F50 | 10 |  |  | available |
| text-104 | 0x00477F50..0x00479190 | 8 |  |  | available |
| text-105 | 0x00479190..0x0047A420 | 34 |  |  | available |
| text-106 | 0x0047A420..0x0047B4D0 | 21 |  |  | available |
| text-107 | 0x0047B4D0..0x0047BE30 | 64 |  |  | available |
| text-108 | 0x0047BE30..0x0047CFE0 | 47 |  |  | available |
| text-109 | 0x0047CFE0..0x0047E350 | 38 |  |  | available |
| text-110 | 0x0047E350..0x0047F5B0 | 54 |  |  | available |
| text-111 | 0x0047F5B0..0x0047FAAE | 33 |  |  | available |

Statuses: `available`, `active`, `released`. Git history preserves old
claims. Contact listed worker before taking over a stale active row.
