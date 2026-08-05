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

The banner is the best direct saving-product classifier. The adjacent version tuples distinguish original creator metadata from the last saver and appear to contain internal major/minor/build/status data, but their platform-dependent packing is not fully decoded. Finale 27 MUSX conversion preserves the legacy `created` tuple but rewrites `modified` to the conversion event; this was verified on `Score3Drop-Fin07.mus`.

### Pre-banner files

**Strong.** Fifty-four substantive files—nearly all under directories named `Finale 2 Files`—lack `ENIGMA BINARY FILE` and commonly begin their body with forms such as `000000ab000002000000444100300000`. One additional `._ScoreI-Fin08.mus` is a 4 KiB AppleDouble metadata artifact, not a score data fork. The Finale 2 classification is path-supported, not explicitly encoded as a readable banner. `Cat Menagerie.org/Old Tom.mus` is also pre-banner but has weaker provenance.

## Archive-derived early-version evidence

The expanded archive survey changes the earliest-version picture. StuffIt extraction with `unar` 1.10.7 produced explicit pre-banner samples labeled Finale 1.8.7, 2.0.1, and 2.6, plus additional 3.0–3.7 files. The earliest explicit product currently observed is 1.8.7; no explicit Finale 1.0 sample has yet been found. These files have no Finale 27 exports and should not be conflated with the 55 direct pre-banner/unknown files. Public IDs and hashes are in `data/archive_members.csv`; original archive/member locations are private.

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
