# Format Notes

Confidence labels: **confirmed** means reproduced across the stated sample; **strong** means multiple independent observations agree; **weak** is a working hypothesis; **open** is not interpreted.

## File header and version identification

### Banner-era files

**Confirmed.** All 1,163 files classified as Finale 3.0 through Finale 2012 begin with this 32-byte area:

| Offset | Size | Meaning |
|---:|---:|---|
| `0x000` | 19 bytes plus zero fill | `ENIGMA BINARY FILE` signature |
| `0x020` | up to 64 bytes | Saving-product/copyright banner, e.g. `Finale(R) 2005 ...` |
| `0x060` | 6 bytes | Unknown header flags/revision data |
| `0x066` | 3 bytes | Creation date: year byte + 1900, month, day |
| `0x06c` | 20 bytes | Creator Enigma/application/file version tuples plus `FIN` and `MAC`/`WIN` |
| `0x08c` | 3 bytes | Last-save date in the same encoding |
| `0x092` | 20 bytes | Last-saver Enigma/application/file version tuples plus platform |
| `0x0b0`–`0x1ff` | variable/zero fill | title, composer, copyright, file-info, or ancillary header data |
| `0x200` | — | first body/block boundary in all examined banner-era examples |

The last-save date matched the filesystem modification day for 1,147 of 1,163 files with a valid date (98.6%). The mismatches are consistent with later filesystem copies. Creation was not assumed from the filename. In 1,039 files where both dates decoded, creation was not later than last save.

The banner is the best direct saving-product classifier. The adjacent version tuples distinguish original creator metadata from the last saver and appear to contain internal major/minor/build/status data, but their platform-dependent packing is not fully decoded. Finale 27 MUSX conversion preserves the legacy `created` tuple but rewrites `modified` to the conversion event; this was verified on corpus ID `mus-8565d1cad82178ae`.

### Pre-banner files

**Strong.** Fifty-four substantive files—nearly all under directories named `Finale 2 Files`—lack `ENIGMA BINARY FILE` and commonly begin their body with forms such as `000000ab000002000000444100300000`. One additional `._ScoreI-Fin08.mus` is a 4 KiB AppleDouble metadata artifact, not a score data fork. The Finale 2 classification is path-supported, not explicitly encoded as a readable banner. Corpus ID `mus-6d3b75475a2ca67b` is also pre-banner but has weaker provenance.

## Archive-derived early-version evidence

The expanded archive survey changes the earliest-version picture. StuffIt extraction with `unar` 1.10.7 produced explicit pre-banner samples labeled Finale 1.8.7, 2.0.1, and 2.6, plus additional 3.0–3.7 files. The earliest explicit product currently observed is 1.8.7; no explicit Finale 1.0 sample has yet been found. These files have no Finale 27 exports and should not be conflated with the 55 direct pre-banner/unknown files. Public IDs and hashes are in `data/archive_members.csv`; original archive/member locations are private.

### ETF evidence set

**Confirmed.** Fourteen locally retained ETF exports now provide a semantic record-level reference for the older
families. The original six archival exports remain under ignored `private/evidence/`; the controlled F2002–F2005 pairs
are tracked under `evidence/` because they contain no private source layout. Their hashes and provenance are recorded
in the experiment log.

| Evidence | Source era | ETF size | Observed sections | Selected observations |
|---|---|---:|---|---|
| `nestedTupletFin05RC2.etf` (`mus-d89e8fe12e271440`) | Finale 2005 | 16,893 bytes | header, others, details, entries, text, lyrics | Explicit ETF header identifies Finale 2005; six `eE` entry records and tuple/detail records expose the high-entropy era's logical model. |
| `template-Fin2000-from-Fin2000.etf` (`mus-3597fd4fce0c272b`) | Finale 2000 | 27,945 bytes | header, others, details, entries, text, lyrics | Explicit header identifies Finale 2000; no `eE` entries; compact options/defaults and text blocks. |
| `template-Fin2000-from-Fin2005.etf` | Finale 2000 source, Finale 2005 saver | 34,029 bytes | header, others, details, entries, text, lyrics | Same source document but Finale 2005 header; adds `&f`, `PD`, `XA`, expressions, and other records. This is direct evidence that resaving can synthesize/upgrade records. |
| `guitar pc.etf` (`mus-7aa45639c14b3864`) | Finale 1.8.7 | 123,084 bytes | others, details, entries, text, lyrics | 1,094 `eE` entry lines and 891 detail lines; no modern binary-style header section. |
| `Dream of Summer I-from-Fin2.6.3.etf` (`mus-2c0a5e8897b436d5`) | Finale 2.0.1 source, Finale 2.6.3 exporter | 74,040 bytes | others, details, entries, text, lyrics | 549 `eE` entry lines and 497 detail lines; old ETF uses the same broad logical sections despite the pre-banner binary family. |
| `Score-from-sit-archive.etf` (`mus-bd0042f8e0354192` source class) | Finale 2.6 | 1,272,164 bytes | others, details, entries, text, lyrics | 9,446 `eE` entry lines and 6,814 detail lines; the large sample is suitable for testing whether early records scale regularly. The StuffIt copy was necessary because the ZIP copy lacked the resource fork. |
| `F2002-baseline.etf` | Finale 2002a.r1 | 14,068 bytes | header, others, details, entries, text, lyrics | Three `eE` entries; exact pair with `F2002-baseline.mus`. |
| `F2002-changed-C-to-D.etf` | Finale 2002a.r1 | 14,075 bytes | header, others, details, entries, text, lyrics | Same three entries, with localized pitch-related field changes; exact pair with `F2002-changed-C-to-D.mus`. |
| `F2003-baseline.etf` | Finale 2003a.r1 | 16,033 bytes | header, others, details, entries, text, lyrics | Three `eE` entries; exact pair with `F2003-baseline.mus`. |
| `F2003-changed-C-to-D.etf` | Finale 2003a.r1 | 16,034 bytes | header, others, details, entries, text, lyrics | Same pool layout; one-byte length increase in the entry record and localized entry-payload change. |
| `F2004-baseline.etf` | Finale 2004c.r1 | 16,334 bytes | header, others, details, entries, text, lyrics | Three `eE` entries; same typed pool sequence, with a larger `0x0010` record than F2002/03. |
| `F2004-changed-C-to-D.etf` | Finale 2004c.r1 | 16,334 bytes | header, others, details, entries, text, lyrics | Entry pitch changes plus `BC` records; automatic note spacing is the leading explanation for the derived changes. |
| `F2005-baseline.etf` | Finale 2005b.r1 | 16,381 bytes | header, others, details, entries, text, lyrics | Three `eE` entries; same typed pool sequence. |
| `F2005-changed-C-to-D.etf` | Finale 2005b.r1 | 16,381 bytes | header, others, details, entries, text, lyrics | Entry pitch changes plus the same `BC` dependency pattern seen in F2004; automatic note spacing is the leading explanation. |

ETF records are textual and use explicit section names plus two-character (or extended) structure identifiers, with
`(cmper)` for “other” records and `(cmper,inci)`-like keys for details. `eE` is the prominent entry structure in the
older exports. This confirms that the conceptual “other/detail/entry/text” decomposition predates the 2007 typed-zlib
container. ETF **by itself** does not identify the pre-2007 binary codec: it contains no binary offsets, compressed
bytes, or direct byte-for-byte rendering guarantee. An ETF paired with the exact `.mus` saved by the same Finale
version can still be valuable as known semantic input for codec experiments, especially with controlled edits.

The two exports of the same Finale 2000 template are especially important: the Finale 2005 export adds records and
changes some defaults, while the header changes from Finale 2000 to Finale 2005. A later-version ETF must therefore be
treated as a normalized semantic reference, not as a lossless reconstruction of the original binary save.

## Platform coverage risk

The corpus is overwhelmingly Macintosh-derived. Header platform tuples, classic Mac resource forks, Macintosh archive containers, and Mac-originated conversion history dominate the evidence. The few Windows indicators in the current corpus are not enough to establish whether Windows changes only metadata or also changes byte order, string encoding, padding, record serialization, or option/entry layouts. Windows samples must be treated as a separate validation axis before declaring a parser cross-platform.

## Proposed format eras

| Era | Products in corpus | Structural evidence | Status |
|---|---|---|---|
| Pre-banner | apparent Finale 2 | Distinct header/body; no `ENIGMA BINARY FILE` | Separate parser likely |
| Low-entropy legacy | 3.0, 3.2, 3.5, 3.7, 97, 2000 | Body sample entropy about 3.1–4.1 bits/byte; tables/short ASCII tokens visible | Codec/layout unresolved |
| High-entropy legacy | 2001–2006 | Body entropy usually 7.3–7.8; no valid zlib members; stable `0x000f`-like prefix family | Codec unresolved; ETF highest value |
| Typed zlib transition | 2007–2008 | Four typed blocks; 2007 is mixed big/little endian, 2008 mostly little endian | Wrapper solved; records partly solved |
| Typed zlib stable | 2009, 2010, 2012 | Same four principal blocks and CRC validation; little endian in all but transition exceptions | Best implementation target |

The release boundary is not absolute. Of 108 Finale 2007 files, 81 validate as big-endian wrappers and 27 as little-endian; all report `MAC` except one little-endian Windows file. Of 182 Finale 2008 files, 180 are little-endian and two big-endian. Therefore version plus an observed wrapper/CRC test is safer than version alone.

## 2007+ typed blocks

**Confirmed.** At `0x200`, a principal block has:

| Field | Size | Meaning |
|---|---:|---|
| block type | 2 | numeric block identifier |
| block size | 4 | complete block size, including the 10-byte header |
| CRC-32 | 4 | CRC-32 of decompressed payload |
| payload | variable | zlib member |

Both byte orders occur. Length and CRC validate the choice. The recurring block sequence is:

| Type | Strong interpretation | Evidence |
|---:|---|---|
| `0x001a` | options plus “other” records | generic frame; hundreds of exact XML count correlations |
| `0x001b` | detail records | second generic frame; detail count correlations |
| `0x0016` | entry pool | position and semantic scale; generic frame does not fit |
| `0x0017` | texts/free-form data | decoded strings and Enigma text commands; generic frame does not fit |

Terminal six-byte markers of types `0x0013` and, in later files, `0x001d` occur after the data blocks. Their meaning is open. No separate central directory was required to walk the four principal blocks; each stored length leads to the next.

## Generic record frame and the 16-word hypothesis

**Confirmed for successfully framed `0x001a` and `0x001b` members.** The frame is variable-length and ends with two zero words. More than 1.59 million records across Finale 2007–2012 were accepted only when the proposed frame consumed the complete decompressed member and every trailer was zero.

Two serialized variants were observed:

| Variant | Header before data | Stored size behavior | Trailer |
|---|---|---|---|
| Big-endian/earlier | five 16-bit fields; fifth is payload bytes | payload is exactly stored bytes | two zero words |
| Little-endian/transition | four 16-bit fields; fourth is size | serialized data extends two bytes beyond the stored size | two zero words |

The latter may mean that one logical header word moved across the size boundary rather than that the payload truly grew; this is unresolved. The stable logical fields are a numeric type, one or more key fields, a byte count, variable data, and two zero words.

The literal **16-word fixed-record hypothesis is disproved for these blocks**:

- a common big-endian record with a 12-byte payload occupies 13 words total;
- the corresponding little-endian serialized record also occupies 13 words;
- observed payload sizes include 12, 24, 26, 36, 48, 60, 72, 84, 96, 108, 120, 132, 180, 276, 1,536, 8,796, and others;
- records begin at variable offsets rather than a 32-byte grid.

The hypothesized two unaccounted words do exist in this era, but as a **four-byte all-zero record trailer/reserved terminator**, not as the last two words of a fixed 16-word structure. Their semantic purpose remains open (reserved fields versus terminator/padding), but their position and zero value are strongly established.

This does not disprove a 16-word in-memory or pre-2007 decompressed structure. ETF pairs are needed before generalizing backward.

### Finale 2002 controlled pair

**Strong for this sample; broader version coverage pending.** The F2002 baseline and one-pitch-change pair were saved
by Finale `2002a.r1` under Mac OS 9.0.4 in SheepShaver. Beginning at `0x200`, both files expose a simple outer stream
of big-endian variable-length records:

| Offset | Type | Total record length | Baseline / changed |
|---:|---:|---:|---:|
| `0x0200` | `0x000f` | 1,976 | unchanged |
| `0x09b8` | `0x0010` | 158 | unchanged |
| `0x0a56` | `0x0011` | 53 / 57 | changed |
| `0x0a8b` / `0x0a8f` | `0x0012` | 6 | terminal marker moved with preceding record |

The six-byte outer header is two bytes of type followed by a four-byte **total record length including the header**.
Records are not constrained to 16 words or 32-byte alignment; the terminal record begins at an odd offset in the
changed file. Changing the pitch from C to D changes 45 observed bytes overall, including the length field and a
four-byte extension, while the complete `0x000f` and `0x0010` records remain identical. The ETF changes only the
`eE` entry payloads, supporting the interpretation that `0x0011` is an entry-related pool and that the edit is
localized at the outer-record level.

This does **not** solve the payload codec: the bytes inside `0x000f`–`0x0011` remain high-entropy and their internal
fields are not yet decoded. It does, however, show that the 2001–2006 family may have a regular typed/length-framed
container beneath the opaque payload encoding. The 2002 result should be tested against F2003–F2005 before treating
the four type codes or framing as universal. F2003, F2004, and F2005 now independently confirm the same framing and
type sequence. The outer framing is strong across 2002–2005, while payload semantics and codec remain open.

## Record fields

In the big-endian `0x001a` variant, the first five words behave as:

1. numeric record type;
2. primary key/`cmper` or `0xfffe` option sentinel;
3. secondary key/`inci`/part dimension candidate;
4. additional key/flags/part dimension candidate;
5. payload byte count.

Fields 2–4 are not fully named. For ordinary “other” records, field 2 tracks the `cmper` sequence and fields 3–4 are often zero. The `0xfffe` records at the front of `0x001a` are singletons with codes beginning at `0x000f` and are strongly interpreted as options. This shows that options are not wholly free-form in 2007+, although their code-to-option and field mappings remain open.

For details, codes around `0x03ef`–`0x0455` correlate with EnigmaXml detail names. The multiple key fields are consistent with `entnum` plus `inci` and possibly part/sharing scope.

See [RECORD_CATALOG.md](RECORD_CATALOG.md) for all observed identifiers. Examples of exact corpus-wide matches include:

| Code | EnigmaXml structure | Evidence |
|---:|---|---|
| `0x0086` | `durAllot` | exact in 375/375 framed files |
| `0x0092` | `frameSpec` | Pearson 0.999; exact in 273/375 (conversion expands some) |
| `0x00a4` | `measNumbRegion` | exact whenever present; 368 files |
| `0x00d9` | `smartShape` | Pearson 0.998; conversion differences observed |
| `0x011a` | `partDef` | exact in 375/375 framed files |
| `0x03ef` | `acciAlter` | strong correlation with conversion differences |
| `0x03f3` | `baselinesExprAboveStaff` | exact in 324/324 detail-framed files |
| `0x0414` | `gfhold` | Pearson 0.999; exact in 291/324 |

## Entry pool

**Open but bounded.** Block `0x0016` decompresses cleanly and CRC-validates but does not use the generic zero-trailed frame. Its decoded size tracks document complexity, and Finale 27 exports provide exact entry numbers, links, durations, pitches, flags, and note IDs for correlation. The first decoded bytes look like dense bit/word structures with repeated entry-number-like values. No entry parser was attempted.

## Options

**Partly structured.** Options are at the beginning of `0x001a`, identified by primary key `0xfffe`; many are fixed 12-byte records, while some have large variable payloads. This contradicts “completely free-form” for 2007+. Option code names and byte layouts remain unknown. Earlier versions may differ.

## Text and variable-length data

**Strong.** Block `0x0017` contains decoded strings, font names, XML printer settings, Enigma text commands such as `^font(...)`, and binary control data. Its representation is not the generic record frame. Header offsets `0x0b0`–`0x1ff` also contain title/composer/copyright/file-info strings when present.

## Sharing and linked parts

**Confirmed present; encoding unresolved.** Finale 27 references show:

- 301 matched documents with more than one `partDef`;
- 315 documents with part-scoped records;
- 2,188,767 part-scoped converted elements;
- 1,156,180 `shared="true"` and 1,032,587 `shared="false"` attributes.

Code `0x011a` maps exactly to `partDef`. Part scope and sharing are likely encoded in the remaining key/flag fields and/or duplicated records inside the ordinary “other” and detail pools, not a named sharing block. Finale 27 may expand shared relationships during conversion, so the counts do not prove the legacy representation is duplicated. A controlled link/unlink test is essential.

## Checksums, compression, and wrapping

- 2007+: zlib plus explicit CRC-32 and stored block length, confirmed.
- 2001–2006: high-entropy non-zlib encoding, stable families but algorithm unknown.
- 3.x–2000: low-entropy tables/encoded streams; visible short ASCII tokens do not by themselves establish raw records.
- pre-banner/Finale 2: separate organization.

No evidence of whole-file encryption was found. No checksum was identified in the earlier eras.

## Failed or revised hypotheses

- Records do **not** start at offset zero; the common body boundary is `0x200`.
- 2007+ principal records are **not** fixed at 16 words or 32-byte aligned.
- Treating all zlib members as the generic record pool failed; only `0x001a` and `0x001b` fit, while entries and texts use other layouts.
- Byte order is **not** derivable from `MAC` versus `WIN` alone during the 2007–2008 transition.
- XML counts are not always binary counts; Finale 27 expands or normalizes `frameSpec`, smart shapes, text definitions, details, and part-scoped data.
