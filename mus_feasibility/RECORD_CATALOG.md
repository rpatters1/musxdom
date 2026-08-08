# Record Catalog

This catalogs numeric identifiers observed after decompressing record-framed zlib blocks. It does not claim that pre-2007 encoded bytes use the same identifiers. The proposed XML names come from corpus-wide count correlation with Finale 27 exports; conversion differences and count collisions remain possible. All candidate rankings are preserved in [`data/record_correlations.csv`](data/record_correlations.csv), and per-version raw observations in `private/record_catalog.csv` (local-only).

Frame fields are described in [FORMAT_NOTES.md](FORMAT_NOTES.md). `Example offset` is relative to the decompressed zlib member, not the file.

## Finale 2000 PDK tag reference

This reference table records two-character tags identified from the publicly accessible Finale 2000 PDK at GUIDOLib commit `9f74ba9b3e287f240bbd454c2259fc3f7737c6ad`. The meanings are `public-PDK-derived`; every listed tag is also observed in the available ETF evidence. Rows marked binary-verified occur at the expected position in Finale 2000 or decoded 2002–2005 fixed rows. See FORMAT_NOTES.md for provenance and the independent framing checks.

| Tag | Logical meaning | Storage family | Verification |
|---|---|---|---|
| `CN` | notehead modification | entry detail | binary-verified in Finale 2000 |
| `DI` | separate score-expression placement | other | ETF-observed |
| `DO` | shape-expression definition | other | ETF-observed |
| `DT` | text-expression definition | other | ETF-observed |
| `DY` | score-expression assignment | other | ETF-observed |
| `ED` | staff-expression assignment | entry detail | ETF-observed |
| `GF` | frame holder | detail | binary-verified |
| `IM` | articulation assignment | entry detail | ETF-observed |
| `IS` | staff attributes | other | binary-verified |
| `Iu` | staff-list membership / staff used | other | binary-verified |
| `MN` | measure-number region | other | ETF-observed |
| `MS` | measure attributes | other | binary-verified |
| `NG` | staff-group attributes | detail | ETF-observed |
| `PD` | expression MIDI-dump playback data | other | ETF-observed |
| `PS` | page attributes | other | binary-verified |
| `SD` | shape definition | other | binary-verified |
| `SS` | staff-system attributes | other | binary-verified |
| `TP` | tuplet definition | entry detail | binary-verified in Finale 2000 |
| `TX` | text-block definition | other | ETF-observed |
| `pT` | page-text assignment | other | ETF-observed |
| `eE` | entry | entry pool | 38-byte row binary-verified |

## Finale 2007+ numeric identifiers

| Identifier | Proposed structure | Pool | Confidence | Versions | Payload bytes observed | Example (member:offset) |
|---|---|---|---|---|---|---|
| `0x000f` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x0) |
| `0x0010` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x1a) |
| `0x0011` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x34) |
| `0x0012` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x4e) |
| `0x0013` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x68) |
| `0x0014` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x82) |
| `0x0015` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x9c) |
| `0x0016` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xb6) |
| `0x0017` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xd0) |
| `0x0018` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xea) |
| `0x0019` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x104) |
| `0x001a` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x11e) |
| `0x001b` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x138) |
| `0x001c` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x152) |
| `0x001d` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x16c) |
| `0x001e` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x186) |
| `0x001f` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x1a0) |
| `0x0020` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 36 | `mus-5d8b3820abbb676b` (0:0x1ba) |
| `0x0021` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x1ec) |
| `0x0022` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x206) |
| `0x0023` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x220) |
| `0x0024` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x23a) |
| `0x0025` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x254) |
| `0x0026` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 276 | `mus-5d8b3820abbb676b` (0:0x26e) |
| `0x0027` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 24 | `mus-5d8b3820abbb676b` (0:0x390) |
| `0x0028` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 180 | `mus-5d8b3820abbb676b` (0:0x3b6) |
| `0x0029` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x478) |
| `0x002a` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x492) |
| `0x002b` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x4ac) |
| `0x002c` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x4c6) |
| `0x002d` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x4e0) |
| `0x002e` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x4fa) |
| `0x002f` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x514) |
| `0x0030` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x52e) |
| `0x0031` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x548) |
| `0x0032` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x562) |
| `0x0033` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x57c) |
| `0x0034` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x596) |
| `0x0035` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x5b0) |
| `0x0036` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 1536, 1800 | `mus-5d8b3820abbb676b` (0:0x5ca) |
| `0x0037` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 36 | `mus-5d8b3820abbb676b` (0:0xbd8) |
| `0x0038` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xc0a) |
| `0x0039` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xc24) |
| `0x003a` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xc3e) |
| `0x003b` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xc58) |
| `0x003c` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xc72) |
| `0x003d` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xc8c) |
| `0x003e` | unknown | unknown/options | open | 2008, 2009, 2010, 2012 | 84 | `mus-c9980ba9b8150520` (0:0xca6) |
| `0x0040` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xca6) |
| `0x0041` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xcc0) |
| `0x0042` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 24 | `mus-5d8b3820abbb676b` (0:0xcda) |
| `0x0043` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 24 | `mus-5d8b3820abbb676b` (0:0xd00) |
| `0x0045` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 60 | `mus-5d8b3820abbb676b` (0:0xd26) |
| `0x0046` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 36 | `mus-5d8b3820abbb676b` (0:0xd70) |
| `0x0047` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12, 36 | `mus-5d8b3820abbb676b` (0:0xda2) |
| `0x0048` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12, 24 | `mus-5d8b3820abbb676b` (0:0xdbc) |
| `0x0049` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xdd6) |
| `0x004a` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xdf0) |
| `0x004b` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xe0a) |
| `0x004c` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xe24) |
| `0x004d` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xe3e) |
| `0x004e` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xe58) |
| `0x004f` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xe72) |
| `0x0050` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xe8c) |
| `0x0051` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xea6) |
| `0x0052` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xec0) |
| `0x0053` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xeda) |
| `0x0054` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xef4) |
| `0x0055` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xf0e) |
| `0x0056` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xf28) |
| `0x0057` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xf42) |
| `0x0058` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xf5c) |
| `0x0059` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12, 276 | `mus-5d8b3820abbb676b` (0:0xf76) |
| `0x005a` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xf90) |
| `0x005b` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 108 | `mus-5d8b3820abbb676b` (0:0xfaa) |
| `0x005c` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 96, 108 | `mus-5d8b3820abbb676b` (0:0x1024) |
| `0x005d` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x1092) |
| `0x005e` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x10ac) |
| `0x005f` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x10c6) |
| `0x0060` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x10e0) |
| `0x0061` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x10fa) |
| `0x0062` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 48 | `mus-5d8b3820abbb676b` (0:0x1114) |
| `0x0063` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 48 | `mus-5d8b3820abbb676b` (0:0x1152) |
| `0x0064` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 60 | `mus-5d8b3820abbb676b` (0:0x1190) |
| `0x0065` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 24 | `mus-5d8b3820abbb676b` (0:0x11da) |
| `0x0066` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 120 | `mus-5d8b3820abbb676b` (0:0x1200) |
| `0x0068` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 108 | `mus-5d8b3820abbb676b` (0:0x1286) |
| `0x0069` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x1300) |
| `0x006a` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x131a) |
| `0x006b` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x1334) |
| `0x006c` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 36 | `mus-5d8b3820abbb676b` (0:0x134e) |
| `0x006d` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 324, 360 | `mus-5d8b3820abbb676b` (0:0x1380) |
| `0x006e` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x14d2) |
| `0x006f` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 24, 36 | `mus-5d8b3820abbb676b` (0:0x14ec) |
| `0x0070` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 48 | `mus-5d8b3820abbb676b` (0:0x1512) |
| `0x0071` | unknown | unknown/options | open | 2008, 2009, 2010, 2012 | 84 | `mus-c9980ba9b8150520` (0:0x15be) |
| `0x0072` | unknown | unknown/options | open | 2010 | 24 | `mus-4b3246869b9d07d7` (0:0x1620) |
| `0x0073` | acciOrderSharps | others | weak | 2010 | 24 | `mus-7b8b404a75679350` (0:0x1620) |
| `0x0074` | unknown | unknown/options | open | 2010 | 24 | `mus-4b3246869b9d07d7` (0:0x166c) |
| `0x0075` | acciOrderSharps | others | weak | 2010 | 24 | `mus-7b8b404a75679350` (0:0x166c) |
| `0x0076` | ossiaMusic | others | weak | 2007, 2008 | 12 | `mus-4da0620f61adddcc` (0:0x1536) |
| `0x0077` | ossiaMusic | others | weak | 2007, 2008 | 12 | `mus-4da0620f61adddcc` (0:0x17f4) |
| `0x0078` | ossiaMusic | others | weak | 2007, 2008 | 12 | `mus-4da0620f61adddcc` (0:0x1ab2) |
| `0x0079` | articDef | others | weak | 2007, 2008, 2009, 2010, 2012 | 36, 48, 60 | `mus-5d8b3820abbb676b` (0:0x1550) |
| `0x007a` | measExprAssign | others | weak | 2007, 2008, 2009, 2010, 2012 | 24, 36, 48, 60, 72, 84, 96, 108, 120, 132, 144, 156, 168, 180, 192, 204, 216, 228, 240, 252, 264, 276, 288, 300, 312, 324, 336, 348, 360, 372, 384, 396, 408, 420, 432, 444, 468, 492, 516, 600, 636, 660, 684 | `mus-5d8b3820abbb676b` (0:0x1bda) |
| `0x007b` | bookmark | others | weak | 2008, 2012 | 24, 72 | `mus-f8816f39ba27df5a` (0:0x1caa) |
| `0x007c` | channelPlayData | others | strong | 2007, 2008, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xa034) |
| `0x007d` | chordSuffixPlay | others | weak | 2007, 2008, 2009, 2010, 2012 | 12, 24, 36, 48, 60, 72, 84, 96, 108, 120, 132, 144, 156, 168, 192, 216, 240, 264, 288, 312, 336 | `mus-8fc93fa852c6c878` (0:0x3432) |
| `0x007e` | chordSuffixPlay | others | strong | 2007, 2008, 2009, 2010, 2012 | 12, 24, 36, 48, 60, 72, 84, 96, 108, 144, 168 | `mus-8fc93fa852c6c878` (0:0x4f42) |
| `0x007f` | textExpressionEnclosure | others | weak | 2007, 2008, 2012 | 12, 24, 36, 48, 60 | `mus-5d8b3820abbb676b` (0:0xa04e) |
| `0x0083` | fretInst | others | weak | 2007, 2008, 2009, 2010, 2012 | 12, 24, 36, 48, 60 | `mus-dd1e34438b93fd7b` (0:0x7280) |
| `0x0084` | drumStaff | others | moderate | 2007, 2008, 2012 | 12, 24 | `mus-b0e2c1c68e58c241` (0:0x2188) |
| `0x0085` | drumLayoutPmmRef | others | weak | 2007, 2008, 2009 | 24 | `mus-8971876eba2eb983` (0:0x1c00) |
| `0x0086` | durAllot | others | strong | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0xa5b0) |
| `0x0088` | execShape | others | strong | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-8fc93fa852c6c878` (0:0x5f58) |
| `0x0089` | fileURLBookmark | others | weak | 2007, 2008 | 516, 528, 540, 564, 576, 588, 600, 624 | `mus-b8a1371ceef29dfc` (0:0x37e4) |
| `0x008a` | filePath | others | moderate | 2007, 2008, 2012 | 12 | `mus-8858b546e24a0d38` (0:0x4c56) |
| `0x008b` | filePath | others | moderate | 2007, 2008, 2012 | 12, 24, 48 | `mus-8858b546e24a0d38` (0:0x4c70) |
| `0x008c` | fretboardSymbol | others | strong | 2009, 2010, 2012 | 12 | `mus-da9e72c0a8ce7f8c` (0:0x41f6) |
| `0x008d` | layerAtts | others | weak | 2007, 2008, 2009, 2010, 2012 | 12, 36, 48 | `mus-5d8b3820abbb676b` (0:0xa7ec) |
| `0x0090` | fontName | others | weak | 2007, 2008, 2009, 2010, 2012 | 24, 36, 48, 60 | `mus-5d8b3820abbb676b` (0:0xa86c) |
| `0x0092` | frameSpec | others | moderate | 2007, 2008, 2009, 2010, 2012 | 12, 24 | `mus-5d8b3820abbb676b` (0:0xab0a) |
| `0x0093` | lockMeas | others | strong | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x161c0) |
| `0x0094` | fretGroup | others | weak | 2007, 2008, 2009, 2010, 2012 | 60, 120, 180, 204, 240, 300, 360, 408, 420, 480, 540, 612, 816, 1224 | `mus-21b30fb5dfc9bca2` (0:0x64cc) |
| `0x0095` | fretInst | others | strong | 2007, 2008, 2009, 2010, 2012 | 72, 84 | `mus-5d8b3820abbb676b` (0:0x16a62) |
| `0x0097` | fretStyle | others | strong | 2007, 2008, 2009, 2010, 2012 | 156 | `mus-5d8b3820abbb676b` (0:0x16ab8) |
| `0x0098` | splitMeas | others | weak | 2007 | 12 | `mus-af66b8418cb05fa6` (0:0x649a) |
| `0x009a` | fstKeyFlats | others | strong | 2007, 2008, 2009, 2010, 2012 | 24 | `mus-5d8b3820abbb676b` (0:0x16b62) |
| `0x009b` | fstKeySharps | others | strong | 2007, 2008, 2009, 2010, 2012 | 24 | `mus-5d8b3820abbb676b` (0:0x16b88) |
| `0x009e` | hpOptions | others | strong | 2007, 2008, 2009, 2010, 2012 | 36 | `mus-5d8b3820abbb676b` (0:0x16bae) |
| `0x009f` | staffSpec | others | weak | 2007, 2008, 2009, 2010, 2012 | 24, 48, 72, 96, 120, 144, 168, 192, 216, 240, 264, 288, 312, 336, 360, 384, 408, 432, 456, 480, 504, 528, 552, 576, 600, 720, 744, 792, 816, 984, 1008 | `mus-5d8b3820abbb676b` (0:0x16be0) |
| `0x00a0` | keyFormat | others | strong | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x19292) |
| `0x00a1` | keyMap | others | weak | 2010 | 48, 132, 384 | `mus-7b8b404a75679350` (0:0xb84e) |
| `0x00a2` | keysAttrib | others | strong | 2007, 2008, 2010, 2012 | 12, 216 | `mus-21b30fb5dfc9bca2` (0:0xa0fa) |
| `0x00a3` | layerAtts | others | weak | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x192ac) |
| `0x00a4` | measNumbRegion | others | strong | 2007, 2008, 2009, 2010, 2012 | 96, 204, 360 | `mus-5d8b3820abbb676b` (0:0x19314) |
| `0x00a5` | metaArtic | others | strong | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x194cc) |
| `0x00a6` | metaChord | others | strong | 2007, 2008, 2009, 2012 | 12, 24 | `mus-5d8b3820abbb676b` (0:0x19604) |
| `0x00a7` | metaClef | others | strong | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x196ee) |
| `0x00a8` | metaDynam | others | strong | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x1988e) |
| `0x00a9` | metaKeySig | others | strong | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x199e0) |
| `0x00aa` | metaRepeat | others | strong | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-8fc93fa852c6c878` (0:0x873c) |
| `0x00ab` | metaShape | others | strong | 2007, 2008, 2010, 2012 | 12 | `mus-21b30fb5dfc9bca2` (0:0xb01a) |
| `0x00ac` | metaStaffStyle | others | strong | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x19aca) |
| `0x00ad` | metaTimeSig | others | strong | 2007, 2008, 2010, 2012 | 12 | `mus-8fc93fa852c6c878` (0:0x892a) |
| `0x00ae` | metaTuplet | others | strong | 2007, 2008, 2012 | 36 | `mus-c2cf68bd12bf7532` (0:0x5ad2) |
| `0x00af` | mmRest | others | strong | 2007, 2008, 2009, 2012 | 24 | `mus-dd1e34438b93fd7b` (0:0xa5ca) |
| `0x00b0` | measSpec | others | weak | 2007, 2008, 2009, 2010, 2012 | 8, 26 | `mus-5d8b3820abbb676b` (0:0x19afe) |
| `0x00b1` | measExprAssign | others | weak | 2007, 2008, 2009, 2010, 2012 | 12, 24, 36, 48, 60, 72, 84, 96, 120, 144, 156, 168, 192, 216, 240, 264, 288, 312, 336, 360, 384, 408, 432, 456, 504, 528 | `mus-5d8b3820abbb676b` (0:0x1d776) |
| `0x00b3` | namePosAbbrv | others | moderate | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x1df10) |
| `0x00b4` | namePosAbbrvStyle | others | moderate | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x1df92) |
| `0x00b5` | namePosFull | others | moderate | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x1e118) |
| `0x00b6` | namePosFullStyle | others | moderate | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x1e19a) |
| `0x00b7` | textBlock | others | moderate | 2007, 2008, 2009, 2010, 2012 | 36 | `mus-5d8b3820abbb676b` (0:0x1e306) |
| `0x00b9` | staffPlayData | others | weak | 2007, 2008, 2009, 2010, 2012 | 12, 24 | `mus-5d8b3820abbb676b` (0:0x21b78) |
| `0x00ba` | staffPlayData | others | weak | 2007, 2008, 2009, 2010, 2012 | 12, 24, 36, 48, 60 | `mus-5d8b3820abbb676b` (0:0x2222c) |
| `0x00bb` | pageSpec | others | strong | 2007, 2008, 2009, 2010, 2012 | 24 | `mus-5d8b3820abbb676b` (0:0x22338) |
| `0x00bc` | pageGraphicAssign | others | weak | 2007, 2008, 2012 | 36, 72 | `mus-b8a1371ceef29dfc` (0:0x7792) |
| `0x00be` | playDefs | others | weak | 2007, 2008, 2009, 2010, 2012 | 24 | `mus-5d8b3820abbb676b` (0:0x22760) |
| `0x00bf` | shapeExprDef | others | weak | 2008, 2009, 2012 | 12 | `mus-edb65aa5ece1e002` (0:0x329be) |
| `0x00c0` | playDumpText | others | moderate | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x22786) |
| `0x00c2` | namePosAbbrvStyle | others | weak | 2007, 2008, 2009, 2010, 2012 | 24, 48, 72, 96, 120, 144, 168, 192, 216, 264 | `mus-5d8b3820abbb676b` (0:0x22d9e) |
| `0x00cb` | repeatBack | others | moderate | 2007, 2008, 2012 | 24 | `mus-7e3ae6e56c54d145` (0:0x84be) |
| `0x00cc` | repeatEndingStart | others | weak | 2007, 2008, 2012 | 24 | `mus-6dc76cafea588314` (0:0x15128) |
| `0x00cd` | repeatEndingText | others | weak | 2007, 2008 | 12 | `mus-6dc76cafea588314` (0:0x1514e) |
| `0x00ce` | repeatPassList | others | weak | 2007, 2008, 2012 | 12 | `mus-6dc76cafea588314` (0:0x15168) |
| `0x00cf` | repeatStaffListScoreOverride | others | weak | 2007, 2008 | 12, 24, 36, 48, 60 | `mus-b8a1371ceef29dfc` (0:0x7f9a) |
| `0x00d5` | shapeList | others | strong | 2007, 2008, 2009, 2010, 2012 | 60, 72, 84, 96, 108, 120, 132, 144, 156, 168, 180, 192, 204, 216, 228, 240, 252, 264, 276, 336, 372, 384, 396, 408, 432, 456, 576, 588, 660, 1176 | `mus-5d8b3820abbb676b` (0:0x22f72) |
| `0x00d6` | shapeDef | others | moderate | 2007, 2008, 2009, 2010, 2012 | 12, 24, 36 | `mus-5d8b3820abbb676b` (0:0x24126) |
| `0x00d7` | shapeList | others | strong | 2007, 2008, 2009, 2010, 2012 | 12, 24, 36, 48, 60, 72, 84, 96, 108, 120, 132, 144, 156, 168, 180, 192, 204, 252, 324 | `mus-5d8b3820abbb676b` (0:0x24244) |
| `0x00d9` | smartShape | others | moderate | 2007, 2008, 2009, 2012 | 84, 96 | `mus-5d8b3820abbb676b` (0:0x24ab0) |
| `0x00da` | smartShape | others | weak | 2007, 2008, 2009, 2012 | 12, 24, 36, 48, 60, 72, 84, 96, 108, 120, 132, 144, 156, 168, 180, 192, 204, 216, 228, 240, 252, 264, 276, 312, 372 | `mus-5d8b3820abbb676b` (0:0x47268) |
| `0x00db` | repeatStaffListScore | others | weak | 2009, 2012 | 24 | `mus-76ef4d96d05bedff` (0:0x5e5a) |
| `0x00dd` | splitMeas | others | weak | 2007, 2012 | 12 | `mus-af66b8418cb05fa6` (0:0xea72) |
| `0x00de` | ssLineStyle | others | strong | 2007, 2008, 2009, 2010, 2012 | 72 | `mus-5d8b3820abbb676b` (0:0x4c9ac) |
| `0x00df` | staffSystemSpec | others | strong | 2007, 2008, 2009, 2010, 2012 | 36 | `mus-5d8b3820abbb676b` (0:0x4cbb0) |
| `0x00e1` | repeatStaffListScore | others | strong | 2007, 2008, 2009, 2012 | 12, 24, 36 | `mus-5d8b3820abbb676b` (0:0x4dc4a) |
| `0x00e2` | repeatStaffListParts | others | strong | 2007, 2008, 2009, 2012 | 12, 24, 36 | `mus-5d8b3820abbb676b` (0:0x4dc64) |
| `0x00e3` | repeatStaffListPartsOverride | others | weak | 2007, 2008, 2009, 2012 | 12 | `mus-dd1e34438b93fd7b` (0:0x2dc96) |
| `0x00e4` | repeatStaffListScore | others | strong | 2007, 2008, 2009, 2012 | 12, 24, 36 | `mus-5d8b3820abbb676b` (0:0x4dc7e) |
| `0x00e5` | repeatStaffListScoreOverride | others | weak | 2007, 2008, 2009, 2012 | 12 | `mus-dd1e34438b93fd7b` (0:0x2dd32) |
| `0x00e6` | staffPlayData | others | moderate | 2007, 2008, 2009, 2010, 2012 | 48 | `mus-5d8b3820abbb676b` (0:0x4dc98) |
| `0x00e7` | staffSpec | others | moderate | 2007, 2008, 2009, 2010, 2012 | 72, 84, 96 | `mus-5d8b3820abbb676b` (0:0x4de88) |
| `0x00e8` | staffStyle | others | moderate | 2007, 2008, 2009, 2010, 2012 | 132, 144, 300 | `mus-5d8b3820abbb676b` (0:0x4e08c) |
| `0x00e9` | staffStyleAssign | others | weak | 2007, 2008, 2012 | 24, 48, 72, 96, 120, 144, 168, 192, 216, 240, 288, 312, 336, 360, 408, 624, 696, 1104, 1320, 1728 | `mus-5d8b3820abbb676b` (0:0x4e91a) |
| `0x00eb` | shapeExprDef | others | weak | 2007, 2008, 2009, 2010, 2012 | 36, 48, 60 | `mus-5d8b3820abbb676b` (0:0x4ed38) |
| `0x00ed` | timeLower | others | weak | 2007, 2012 | 12, 24 | `mus-8fc93fa852c6c878` (0:0x291a6) |
| `0x00ee` | timeUpper | others | strong | 2007, 2008, 2010, 2012 | 12, 24 | `mus-5d8b3820abbb676b` (0:0x4ee32) |
| `0x00f0` | tempoDef | others | weak | 2007, 2008, 2012 | 12, 24, 36, 48, 72, 96, 108, 120, 132, 144, 156, 168, 192, 204 | `mus-b3f48779d83ef8a4` (0:0x57092) |
| `0x00f1` | textExprDef | others | moderate | 2007, 2008, 2009, 2010, 2012 | 36, 48, 60, 72, 84, 96, 108, 120, 132, 156, 168 | `mus-5d8b3820abbb676b` (0:0x4eece) |
| `0x00f2` | textExpressionEnclosure | others | strong | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x5025c) |
| `0x00f3` | textRepeatAssign | others | weak | 2008, 2012 | 24 | `mus-7cc5359be983cf2b` (0:0x16abe) |
| `0x00f4` | textRepeatText | others | strong | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-b3f48779d83ef8a4` (0:0x5a3a6) |
| `0x00f6` | textRepeatText | others | strong | 2007, 2008, 2009, 2010, 2012 | 12, 24, 36 | `mus-b3f48779d83ef8a4` (0:0x5a3c0) |
| `0x00f7` | keysAttrib | others | weak | 2007, 2008, 2012 | 48 | `mus-21b30fb5dfc9bca2` (0:0x1d1b8) |
| `0x00f8` | unknown | unknown/options | open | 2007 | 12 | `mus-67e0bd1b0b07568e` (0:0x9220) |
| `0x0112` | viSetup | others | strong | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x50360) |
| `0x0113` | unknown | unknown/options | open | 2008 | 468 | `mus-0399fdd3c1e59432` (0:0x2c8fc) |
| `0x0114` | unknown | unknown/options | open | 2008 | 2280 | `mus-7c2b86b72ca5c793` (0:0x1df8c) |
| `0x0115` | audioUnitInfo | others | strong | 2008, 2010, 2012 | 540 | `mus-0399fdd3c1e59432` (0:0x2cade) |
| `0x0116` | unknown | unknown/options | open | 2008, 2012 | 540 | `mus-7c2b86b72ca5c793` (0:0x1e882) |
| `0x011a` | partDef | others | strong | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x5037a) |
| `0x011b` | viSetup | others | weak | 2007, 2008, 2009, 2012 | 6252, 6276, 6288, 7008, 7416, 7572, 8220, 8232, 8784, 8796, 8808, 8880, 8940 | `mus-5d8b3820abbb676b` (0:0x50394) |
| `0x0120` | unknown | unknown/options | open | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x525fe) |
| `0x0122` | moviePlayerState | others | strong | 2007, 2008, 2009, 2010, 2012 | 12 | `mus-5d8b3820abbb676b` (0:0x52618) |
| `0x012c` | keysAttrib | others | weak | 2010, 2012 | 12, 72, 84, 96, 108, 120, 132, 144, 168, 180, 216, 228, 240, 252, 324, 456, 756 | `mus-7b8b404a75679350` (0:0x1ed12) |
| `0x012d` | markingsCategoryName | others | weak | 2009, 2010, 2012 | 36 | `mus-76ef4d96d05bedff` (0:0x89aa) |
| `0x012e` | markingsCategoryName | others | weak | 2009, 2010, 2012 | 12, 24, 36 | `mus-76ef4d96d05bedff` (0:0x8b08) |
| `0x012f` | categoryStaffListName | others | weak | 2009, 2012 | 12, 24 | `mus-76ef4d96d05bedff` (0:0x8bfa) |
| `0x0130` | markingsCategoryName | others | weak | 2009, 2010, 2012 | 12 | `mus-76ef4d96d05bedff` (0:0x8c7a) |
| `0x0132` | markingsCategoryName | others | weak | 2009, 2010, 2012 | 12 | `mus-76ef4d96d05bedff` (0:0x8ce2) |
| `0x0139` | metaShape | others | weak | 2010, 2012 | 24, 48, 72, 84, 96, 132, 144, 156, 168, 180, 192, 204, 228, 252, 264, 288, 300, 312, 336, 360, 408, 432, 456, 504, 528, 564, 600, 672, 720, 732, 1128, 1464, 1752 | `mus-7b8b404a75679350` (0:0x1f26e) |
| `0x013a` | percMapRef | others | weak | 2012 | 36 | `mus-1b75490c27bd4b82` (0:0x4204c) |
| `0x013b` | volumeValue | others | strong | 2010, 2012 | 12 | `mus-7b8b404a75679350` (0:0x20398) |
| `0x013c` | bypassFxValue | others | strong | 2010, 2012 | 12 | `mus-7b8b404a75679350` (0:0x20482) |
| `0x013d` | percMapRef | others | weak | 2012 | 12 | `mus-1b75490c27bd4b82` (0:0x422b8) |
| `0x013e` | percMapRef | others | weak | 2012 | 36 | `mus-1b75490c27bd4b82` (0:0x422ec) |
| `0x0141` | drumLayoutPmmRef | others | weak | 2012 | 12 | `mus-4c75dfe49da42598` (0:0x10a96) |
| `0x0142` | multiStaffInstGroup | others | weak | 2012 | 12 | `mus-8589af3cf400adf6` (0:0x296a2) |
| `0x0143` | multiStaffGroupID | others | weak | 2012 | 12 | `mus-8589af3cf400adf6` (0:0x296bc) |
| `0x0144` | stylePlayData | others | weak | 2012 | 48 | `mus-36b9b971409a7330` (0:0x56658) |
| `0x03ef` | acciAlter | details | weak | 2007, 2008, 2010, 2012 | 20, 40, 60, 80 | `mus-128dcc6587f4bbea` (1:0x0) |
| `0x03f0` | entrySize | details | strong | 2007, 2008, 2012 | 10 | `mus-e11eeb5e29ceef6f` (1:0x120) |
| `0x03f1` | articAssign | details | moderate | 2007, 2008, 2012 | 20, 40, 60, 80 | `mus-128dcc6587f4bbea` (1:0x100) |
| `0x03f2` | baselinesChords | details | strong | 2007, 2008, 2009, 2010, 2012 | 10 | `mus-128dcc6587f4bbea` (1:0x3d0) |
| `0x03f3` | baselinesExprAboveStaff | details | strong | 2007, 2008, 2009, 2010, 2012 | 10 | `mus-128dcc6587f4bbea` (1:0x3ea) |
| `0x03f4` | baselinesExprBelowStaff | details | strong | 2007, 2008, 2009, 2010, 2012 | 10 | `mus-128dcc6587f4bbea` (1:0x404) |
| `0x03f5` | baselinesFingerboards | details | strong | 2010, 2012 | 10 | `mus-7b8b404a75679350` (1:0x4e) |
| `0x03f6` | baselinesChords | details | weak | 2007, 2008, 2009, 2010, 2012 | 100 | `mus-128dcc6587f4bbea` (1:0x41e) |
| `0x03f7` | baselinesChords | details | weak | 2007, 2008, 2009, 2010, 2012 | 10, 100 | `mus-128dcc6587f4bbea` (1:0x492) |
| `0x03f8` | baselinesExprAboveStaff | details | weak | 2007, 2008, 2009, 2010, 2012 | 10, 20, 30, 100 | `mus-128dcc6587f4bbea` (1:0x506) |
| `0x03fd` | beamExtendDownStem | details | strong | 2007, 2008, 2012 | 10, 20 | `mus-2f80988ad753ed4c` (1:0x112e) |
| `0x03fe` | beamExtendUpStem | details | strong | 2007, 2008, 2012 | 10 | `mus-c598b03b9090d358` (1:0x1576) |
| `0x03ff` | beamStemAdjust | details | strong | 2008, 2012 | 10, 20 | `mus-0943136b60b4b2c6` (1:0x2b2) |
| `0x0400` | beamStub | details | moderate | 2008, 2012 | 10 | `mus-0bf7b27df9b938de` (1:0x766) |
| `0x0401` | beamAltPrimDownStem | details | strong | 2007, 2008, 2012 | 10, 20, 30, 40 | `mus-128dcc6587f4bbea` (1:0x57a) |
| `0x0402` | beamAltPrimUpStem | details | strong | 2007, 2008, 2010, 2012 | 10, 20, 40, 60 | `mus-128dcc6587f4bbea` (1:0x7de) |
| `0x0403` | beamAltSecDownStem | details | weak | 2008, 2012 | 20, 40, 60 | `mus-0f190c22d68ac55f` (1:0x6656) |
| `0x0404` | beamAltSecUpStem | details | weak | 2007, 2012 | 20, 40 | `mus-2f80988ad753ed4c` (1:0x1a1a) |
| `0x0406` | centerShape | details | strong | 2007, 2008, 2009, 2012 | 30 | `mus-128dcc6587f4bbea` (1:0xa66) |
| `0x0407` | chordAssign | details | weak | 2008 | 20 | `mus-00e0bb57f7f785fa` (1:0x840a) |
| `0x0408` | unknown | unknown/options | open | 2010 | 20 | `mus-4b3246869b9d07d7` (1:0x1c4) |
| `0x0409` | unknown | unknown/options | open | 2010 | 20 | `mus-7b8b404a75679350` (1:0x1c4) |
| `0x040a` | crossChord | details | weak | 2009, 2010, 2012 | 160 | `mus-da9e72c0a8ce7f8c` (1:0x1aa) |
| `0x040c` | crossStaff | details | weak | 2008, 2012 | 10, 20, 30, 40, 50 | `mus-527671f651531470` (1:0x259e) |
| `0x040d` | dotOffset | details | weak | 2007, 2008, 2012 | 10, 20, 30 | `mus-c598b03b9090d358` (1:0x3856) |
| `0x040e` | chordAssign | details | weak | 2007, 2008, 2009 | 10 | `mus-e11eeb5e29ceef6f` (1:0x3422) |
| `0x040f` | chordAssign | details | weak | 2007, 2008, 2009 | 10, 20, 30, 40, 50 | `mus-e11eeb5e29ceef6f` (1:0x4122) |
| `0x0410` | staffSize | details | strong | 2007, 2008, 2012 | 10 | `mus-8fc93fa852c6c878` (1:0x74e2) |
| `0x0411` | floats | details | strong | 2007, 2008, 2009, 2010, 2012 | 20 | `mus-128dcc6587f4bbea` (1:0xaf0) |
| `0x0413` | fretboard | details | strong | 2007, 2008, 2009, 2010, 2012 | 10, 30, 40, 50 | `mus-5a6641ad7416ea92` (1:0x15e2) |
| `0x0414` | gfhold | details | moderate | 2007, 2008, 2009, 2010, 2012 | 20 | `mus-128dcc6587f4bbea` (1:0xb38) |
| `0x0416` | unknown | unknown/options | open | 2010 | 10 | `mus-4b3246869b9d07d7` (1:0x174e2) |
| `0x0417` | unknown | unknown/options | open | 2007 | 20, 40, 80, 120, 160, 180, 280, 1780 | `mus-5a6641ad7416ea92` (1:0x4e0e) |
| `0x0418` | lyrDataChorus | details | weak | 2008 | 10 | `mus-00e0bb57f7f785fa` (1:0xe56e) |
| `0x0419` | unknown | unknown/options | open | 2008 | 10 | `mus-527671f651531470` (1:0x495c) |
| `0x041a` | lyrDataVerse | details | weak | 2007, 2008 | 10, 20 | `mus-128dcc6587f4bbea` (1:0x1a8c) |
| `0x041b` | lyricEntryInfo | details | moderate | 2008, 2012 | 10 | `mus-527671f651531470` (1:0x52ce) |
| `0x041c` | measOssiaAssign | details | weak | 2008 | 10 | `mus-ae6c07997b7e3b2a` (1:0x14f7c) |
| `0x041d` | measGraphicAssign | details | weak | 2008 | 40 | `mus-edb65aa5ece1e002` (1:0x7690) |
| `0x041e` | midiExprs | details | weak | 2008 | 10, 20, 30, 50, 60, 110, 130, 140, 150, 160, 170, 180, 200, 220, 230, 250, 260, 270, 280, 410, 420, 520 | `mus-f8816f39ba27df5a` (1:0x20c8) |
| `0x041f` | measNumbIndivPos | details | moderate | 2007, 2008, 2009, 2012 | 30 | `mus-8fc93fa852c6c878` (1:0x8c14) |
| `0x0420` | measTextAssign | details | moderate | 2007, 2008, 2012 | 10, 30 | `mus-2f80988ad753ed4c` (1:0x24c6) |
| `0x0421` | staffGroup | details | weak | 2007, 2008, 2010, 2012 | 40 | `mus-128dcc6587f4bbea` (1:0x1d30) |
| `0x0422` | noteAlter | details | moderate | 2007, 2008, 2012 | 20, 40, 60 | `mus-128dcc6587f4bbea` (1:0x1ef0) |
| `0x0423` | measOssiaAssign | details | weak | 2007, 2008 | 10, 20, 30, 40, 50 | `mus-128dcc6587f4bbea` (1:0x1fa4) |
| `0x0424` | perfData | details | moderate | 2007, 2008, 2012 | 10, 20, 30, 40, 50, 60, 70 | `mus-e11eeb5e29ceef6f` (1:0x7966) |
| `0x0425` | secBeamBreak | details | strong | 2007, 2008, 2012 | 10, 20, 30 | `mus-2f80988ad753ed4c` (1:0x28b6) |
| `0x0426` | shapeNote | details | weak | 2012 | 10, 410 | `mus-fce2c66dcf5e75fb` (1:0x23cbe) |
| `0x0427` | shapeNoteStyle | details | weak | 2007, 2008, 2010, 2012 | 10, 210, 410 | `mus-5a6641ad7416ea92` (1:0xbc20) |
| `0x0428` | smartShapeEntryMark | details | moderate | 2007, 2008, 2009, 2012 | 10, 20, 30, 40, 60, 80, 160 | `mus-128dcc6587f4bbea` (1:0x21e4) |
| `0x0429` | lyrDataVerse | details | weak | 2007, 2008 | 10, 20, 30 | `mus-128dcc6587f4bbea` (1:0x29c4) |
| `0x042a` | stemAdjust | details | strong | 2007, 2008, 2012 | 10, 20 | `mus-128dcc6587f4bbea` (1:0x2acc) |
| `0x042b` | stemDefDown | details | strong | 2007, 2008, 2012 | 10 | `mus-e11eeb5e29ceef6f` (1:0xb03e) |
| `0x042c` | stemDefUp | details | strong | 2007, 2008, 2012 | 10 | `mus-e11eeb5e29ceef6f` (1:0xb10e) |
| `0x042e` | tieAlterEnd | details | moderate | 2007, 2008, 2012 | 30, 60, 90, 120 | `mus-128dcc6587f4bbea` (1:0x2bea) |
| `0x042f` | tieAlterStart | details | moderate | 2007, 2008, 2010, 2012 | 30, 60, 90, 120, 150 | `mus-128dcc6587f4bbea` (1:0x2fcc) |
| `0x0430` | tupletDef | details | strong | 2007, 2008, 2012 | 30, 60 | `mus-128dcc6587f4bbea` (1:0x3606) |
| `0x0443` | baselinesSysChords | details | weak | 2007, 2008, 2012 | 10 | `mus-8fc93fa852c6c878` (1:0xb38e) |
| `0x0444` | baselinesSysExprAboveStaff | details | strong | 2008, 2012 | 10 | `mus-3ee93edd4a0bf8f3` (1:0x1a7a0) |
| `0x0445` | baselinesSysExprBelowStaff | details | weak | 2007, 2008, 2012 | 10 | `mus-5a6641ad7416ea92` (1:0xc05e) |
| `0x0449` | baselinesSysLyricsVerse | details | weak | 2007, 2008, 2012 | 10, 20 | `mus-128dcc6587f4bbea` (1:0x388a) |
| `0x0450` | unknown | unknown/options | open | 2012 | 30, 60 | `mus-db81556fddeccd8a` (1:0x1d53c) |
| `0x0451` | percussionNoteCode | details | weak | 2012 | 10, 20, 30, 50 | `mus-4c75dfe49da42598` (1:0x6ff2) |
| `0x0452` | unknown | unknown/options | open | 2012 | 20 | `mus-db81556fddeccd8a` (1:0x1e37c) |
| `0x0454` | lyrDataVerse | details | weak | 2012 | 20, 40 | `mus-84761f30804afb36` (1:0x6104) |
| `0x0455` | activeLyric | details | strong | 2012 | 10 | `mus-84761f30804afb36` (1:0x65f0) |
