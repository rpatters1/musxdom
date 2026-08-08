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

The expanded archive survey changes the earliest-version picture. StuffIt extraction with `unar` 1.10.7 produced explicit pre-banner samples labeled Finale 1.8.7, 2.0.1, and 2.6, plus additional 3.0–3.7 files. The earliest explicit product currently observed is 1.8.7; no explicit Finale 1.0 sample has yet been found. These files should not be conflated with the 55 direct pre-banner/unknown files. Public IDs and hashes are in `data/archive_members.csv`; original archive/member locations are private.

Finale 27 successfully opened the selected 1.8.7, 2.0.1, and 2.6 files after `.mus` was appended to their
filenames and produced private `.fin27.musx` references. Thus there is no parser compatibility cutoff among these
tested versions. The initial failure mode was file recognition for extensionless classic-Mac documents, not rejection
of their data format. The 2.6 conversion reported font issues, which affect rendering fidelity but did not block
conversion. Finale 1.0 remains untested.

### ETF evidence set

**Confirmed.** Fifteen locally retained ETF exports now provide a semantic record-level reference for the older
families. The original six archival exports remain under ignored `private/evidence/`; the controlled F2002–F2005 pairs
are tracked under `evidence/` because they contain no private source layout. Their hashes and provenance are recorded
in the experiment log.

| Evidence | Source era | ETF size | Observed sections | Selected observations |
|---|---|---:|---|---|
| `nestedTupletFin05RC2.etf` (`mus-d89e8fe12e271440`) | Finale 2005 | 16,893 bytes | header, others, details, entries, text, lyrics | Explicit ETF header identifies Finale 2005; six `eE` entry records and tuple/detail records expose the high-entropy era's logical model. |
| `template-Fin2000-from-Fin2000.etf` (`mus-3597fd4fce0c272b`) | Finale 2000 | 27,945 bytes | header, others, details, entries, text, lyrics | Explicit header identifies Finale 2000; no `eE` entries; compact options/defaults and text blocks. |
| `template-Fin2000-from-Fin2005.etf` | Finale 2000 source, Finale 2005 saver | 34,029 bytes | header, others, details, entries, text, lyrics | Same source document but Finale 2005 header; adds `&f`, `PD`, `XA`, expressions, and other records. This is direct evidence that resaving can synthesize/upgrade records. |
| `tremolos-from-Fin2000.etf` (`mus-3a8b724cf3adba80`) | Finale 2000 | 28,718 bytes | header, others, details, entries, text, lyrics | Exact source-version pair: 1,107 others, 64 details, eight entries, and raw text match the uncompressed binary pools; includes `CN`, `GF`, and `TP`. |
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

Targeted Finale 27 conversions of the three earliest ETF-backed sources are retained locally:

| Private semantic reference | Size | SHA-256 | Finale 27 pool counts (others/details/entries) |
|---|---:|---|---:|
| `guitar pc.fin27.musx` | 60,732 | `4742e6fc35dd6892a6548c45c75c19dd139a6eb1be302733d8725a73d49ccd06` | 2,018 / 787 / 1,320 |
| `Dream of Summer I.fin27.musx` | 38,937 | `07d9f18fae4973f31fc69c8f0ecc551cfb8aa79b2131d2ff6dbdbeb818ab1328` | 1,347 / 478 / 659 |
| `Score-from-sit-archive.fin27.musx` | 392,491 | `bea52ad03c93c6ba5c7d6dd41c7a2c3ca57675ad5510bd68795963172cc1f311` | 14,913 / 6,095 / 11,153 |

The counts differ from ETF and binary physical rows, so these remain normalized semantic references rather than
serialization maps. Finale 27 reported font issues for `Score`, but generated a valid MUSX container.

ETF records are textual and use explicit section names plus two-character (or extended) structure identifiers, with
`(cmper)` for “other” records and `(cmper,inci)`-like keys for details. `eE` is the prominent entry structure in the
older exports. This confirms that the conceptual “other/detail/entry/text” decomposition predates the 2007 typed-zlib
container. ETF **by itself** did not identify the pre-2007 binary codec because it contains no binary offsets,
compressed bytes, or direct byte-for-byte rendering guarantee. Now that PKWARE DCL decoding is solved for 2001–2006,
an ETF paired with the exact `.mus` saved by the same Finale version is valuable as known semantic input for decoded
record and field correlation, especially with controlled edits.

The two exports of the same Finale 2000 template are especially important: the Finale 2005 export adds records and
changes some defaults, while the header changes from Finale 2000 to Finale 2005. A later-version ETF must therefore be
treated as a normalized semantic reference, not as a lossless reconstruction of the original binary save.

### Correlation between Finale 1.8.7–2.6 and Finale 3.0

**Strong physical and semantic continuity; framing still differs.** Re-extraction of the three ETF-backed early
binaries shows that all begin their main record region at `0x20a`, ten bytes after the common `0x200` body boundary,
and maintain a 16-byte cadence. They do not use Finale 3.0's four typed/length pool headers.

The early ordinary layout places the two-byte tag first, followed by the 12-byte payload and 16-bit comparator. Finale
3.x moves the comparator to the front but retains the same 16-byte capacity. Early detail tags occur ten bytes into
each 16-byte row; ordinary two-character tags and the compact pseudo-detail identifiers are directly recognizable.
The mapping `#v1`–`#v10` → `0x8001`–`0x800a`, `#c*` → `0x9001`–`0x900a`, and `#s*` →
`0xa001`–`0xa00a` is exact in the three samples.

Tag-order correlations are unusually strong:

| Source | Ordinary correlation | Detail correlation | Entries | Text |
|---|---|---|---|---|
| Finale 1.8.7 `mus-7aa45639c14b3864` | all 1,447 ETF ordinary tags match from the first binary row | all 891 tags match | all 1,094 32-byte rows reconstruct byte-for-byte from `eE` | ETF text is an exact 405-byte prefix of the raw binary tail |
| Finale 2.0.1 `mus-2c0a5e8897b436d5` | first 38 tags match; Finale 2.6.3 export then adds/reorders ordinary data | all 497 tags match | all 549 rows reconstruct byte-for-byte | exact 936-byte text prefix |
| Finale 2.6 source class | first 80 tags match in the ZIP data fork used for comparison; source-copy/build normalization remains possible | all 6,814 tags match | all 9,446 rows reconstruct byte-for-byte | exact 1,401-byte text prefix |

The early entry ID is implicit in row position. Each entry occupies 32 bytes and corresponds directly to one `eE`
record. Finale 3.x's 38-byte entry row is the same 32-byte entry data with a four-byte explicit entry ID prepended and
a two-byte zero/reserved suffix appended. This is a direct structural bridge across the 2.x/3.0 boundary.

Unexplained index/directory spans remain between the ordinary, detail, entry, and text regions. Thus a 1.x–2.x reader
cannot yet locate every pool generically, even though its core rows and semantics are closely related to Finale 3.0.
Finale 27 opened all three tested sources once their extensionless names were given a `.mus` suffix. This separates
container compatibility from classic Mac file discovery: suffix handling and missing type/creator metadata can make
a readable data fork appear unsupported. The three conversions are semantic references, not proof of losslessness.
Their Finale 27 pool counts differ substantially from ETF, confirming normalization: 1.8.7 converts to
2,018 others/787 details/1,320 entries; 2.0.1 to 1,347/478/659; and 2.6 to 14,913/6,095/11,153.

## Platform coverage risk

The corpus is overwhelmingly Macintosh-derived. Header platform tuples, classic Mac resource forks, Macintosh archive containers, and Mac-originated conversion history dominate the evidence. Four banner-era Windows files nevertheless establish one concrete platform difference: three Finale 3.0 files and one Finale 2000 file serialize the otherwise identical four-pool container and fixed rows little-endian, while 185 recognized Mac-era samples are big-endian. This does not resolve Windows string, option, padding, or later-era behavior; Windows remains a separate validation axis before declaring a parser cross-platform.

## Proposed format eras

| Era | Products in corpus | Structural evidence | Status |
|---|---|---|---|
| Pre-banner | apparent Finale 2 | Distinct header/body; no `ENIGMA BINARY FILE` | Separate parser likely |
| Uncompressed fixed-row legacy | 3.0, 3.2, 3.5, 3.7, 97, 2000 | Four typed/length pools; platform byte order; 16-byte other/detail rows, 38-byte entries, raw text | Container and physical rows solved; tag fields incomplete |
| DCL-compressed legacy | 2001–2006 | Big-endian typed/length blocks; CRC-32; PKWARE DCL; fixed 16-byte other/detail rows and 38-byte entry rows | Container, codec, and physical pool rows solved; logical field mapping incomplete |
| Typed zlib transition | 2007–2008 | Four typed blocks; 2007 is mixed big/little endian, 2008 mostly little endian | Wrapper solved; records partly solved |
| Typed zlib stable | 2009, 2010, 2012 | Same four principal blocks and CRC validation; little endian in all but transition exceptions | Best implementation target |

The release boundary is not absolute. Of 108 Finale 2007 files, 81 validate as big-endian wrappers and 27 as little-endian; all report `MAC` except one little-endian Windows file. Of 182 Finale 2008 files, 180 are little-endian and two big-endian. Therefore version plus an observed wrapper/CRC test is safer than version alone.

## Finale 3.x–2000 uncompressed typed pools

**Confirmed.** Starting at offset `0x200`, recognized files contain four consecutive pools. Each has a two-byte type
and four-byte total length in the file's byte order; the length includes the six-byte header. There is no CRC and no
compression:

| Type | Contents | Physical representation |
|---:|---|---|
| `0x0001` | others/options | 16-bit comparator, two-byte tag, 12-byte payload; 16 bytes total |
| `0x0002` | details | two 16-bit comparators, two-byte tag, 10-byte payload; 16 bytes total |
| `0x0003` | entries | 32-bit entry ID plus entry data; 38 bytes total |
| `0x0004` | text | raw concatenated Enigma text commands; variable length |

The deterministic probe recognizes 189/190 direct Finale 3.x–2000 files through EOF with the exact type sequence
`1,2,3,4`: 185 big-endian files and four Windows-origin little-endian files. Across those files, all 1,552,762
other rows and 770,960 detail rows are exact multiples of 16 bytes, and all 394,984 entries are exact multiples of
38 bytes. The only unrecognized file is one Finale 97 sample; it requires separate integrity/classification work.

The exact Finale 2000 pair `mus-3a8b724cf3adba80` (`tremolos.mus`) is decisive. Its blocks are:

| File offset | Type | Total bytes | Payload interpretation |
|---:|---:|---:|---|
| `0x0200` | `0x0001` | `0x4536` | 17,712 bytes = 1,107 other rows |
| `0x4736` | `0x0002` | `0x0406` | 1,024 bytes = 64 detail rows |
| `0x4b3c` | `0x0003` | `0x0136` | 304 bytes = eight entry rows |
| `0x4c72` | `0x0004` | `0x0243` | 573 bytes of raw text through EOF |

Those counts equal the ETF sections exactly. All 1,107 ordinary tags occur in identical order. The first 34 detail
tags, including `CN`, `GF`, and `TP`, also occur literally and in ETF order; the remaining `#v1`–`#v10`,
`#c1`–`#c10`, and `#s1`–`#s10` pseudo-tags use compact non-ASCII binary identifiers. The eight 38-byte rows equal
the eight `eE` records, and selected fields match directly. Removing only ETF's separators between six text records
makes its text section byte-identical to the 573-byte `0x0004` payload.

This proves that Finale 2001 changed the wrapper/codec, not the core fixed physical rows. Finale 3.0's three files
are Windows-origin and little-endian; the single little-endian Finale 2000 file is also Windows-origin. Thus byte
order must be detected from the block headers rather than inferred only from the release name.

## Finale 2001–2006 typed DCL blocks

**Confirmed.** The high-entropy payloads use the PKWARE Data Compression Library (DCL) format decoded by Mark
Adler's open-source [`blast`](https://github.com/madler/zlib/tree/master/contrib/blast) implementation. This is the
format produced by the PKWARE DCL `implode()` function, not the incompatible PKZIP compression method that was also
named “implode.” `blast` is shipped as a small, permissively licensed contribution in the zlib source tree, but it is
not part of the ordinary installed zlib API and should be built or vendored separately.

At `0x200`, a compressed block has this big-endian layout:

| Field | Size | Meaning |
|---|---:|---|
| block type | 2 | numeric pool/block identifier, commonly `0x000f`–`0x0013` |
| block size | 4 | complete block size, including the six-byte type/length header |
| CRC-32 | 4 | big-endian CRC-32 of the decompressed bytes |
| payload | variable | complete PKWARE DCL stream |

All 1,603 candidate compressed members encountered in the Finale 2001–2006 corpus decoded successfully and matched
their stored CRC-32. Independently, 410 files traverse cleanly as complete typed-block sequences. The completely
recognized file counts were 3/4 for Finale 2001, 47/48 for 2002, 168/168 for 2003,
7/7 for 2004, 6/13 for the `2004b` banner variant, 116/120 for 2005, and 63/66 for 2006. The remaining files did not
match this outer framing through EOF at `0x200`; some nevertheless contain valid leading DCL members, so they were
not DCL failures and still require outer-framing classification.

Every tested stream begins with either `00 04` or `01 04`, the two legal literal-mode values followed by dictionary
parameter 4 (a 1 KiB dictionary). Of the 1,603 members, 1,202 used `00 04` and 401 used `01 04`. The reference decoder
returned success for every member, and CRC validation independently rules out accidental decoding.

The F2002 baseline `0x000f` member is a compact example: its DCL stream is 1,966 bytes, expands to 8,000 bytes, and
has stored and computed CRC-32 `0xcb68f112`. Its 500 fixed 16-byte rows correspond one-for-one and in order with the
500 lines in the ETF `others` section. The following decoded `0x0010` member contains 33 fixed 16-byte rows matching
the 33 ETF `details` lines, and `0x0011` contains three 38-byte rows matching the three ETF `eE` entries.

Six-byte blocks are empty pool markers. Their type is the next sequential pool type that has no data: for example,
the minimal controlled files end with empty `0x0012`, while 58 Finale 2006 files with nonempty `0x0012` end with an
empty `0x0013`. Therefore the earlier interpretation of `0x0012` itself as a terminal type was wrong. Nonempty
`0x0012` members are variable-length and follow the entry pool; text/lyrics are the leading interpretation, but
their internal organization remains open.

## Public Finale 2000 PDK evidence

**Public-PDK-derived, with the physical framing independently binary-verified.** On 2026-08-08 the project adopted
the public-source provenance policy in the README and consulted the Finale 2000 PDK copy included in GRAME's public
GUIDOLib repository at immutable commit
[`9f74ba9b3e287f240bbd454c2259fc3f7737c6ad`](https://github.com/grame-cncm/guidolib/tree/9f74ba9b3e287f240bbd454c2259fc3f7737c6ad/platforms/win32/finale-plugin).
The included README explicitly identifies the material as the Finale 2000 PDK. No PDK file is stored in this
repository.

The PDK data API supplies three decisive factual concepts:

1. An extended data tag is a 32-bit value whose high 16 bits select a storage class and whose low 16 bits are the
   two-character Enigma tag.
2. Ordinary “other” IDs use one 16-bit comparator; detail IDs use two; entry IDs use a 32-bit entry number. An
   incident is an API selection/order dimension, not an additional field found in the fixed physical row.
3. Logical structures larger than one physical payload are stored across a declared number of successive
   incidences. Strings, arrays, and specially handled structures use separate storage classes.

These concepts explain both the two-character tags in the decoded pools and why records with the same tag/key recur
in ETF. They also warn against treating every physical row as a complete musxdom object. For example, the Finale 2000
definitions occupy two physical rows for `MS` (measure attributes), two for `Iu` (staff-list membership), two for
`PS` (page layout), two for `SS` (staff-system layout), and three for `IS` (staff attributes). The controlled ETF
evidence shows later expansion without changing the tag: `IS` grows from three rows in Finale 2002 to six in Finale
2003, while `MS` and `SS` grow from two rows through Finale 2004 to three in Finale 2005. A decoder must therefore
select a versioned logical layout after reading stable physical rows.

Public source files consulted, access date 2026-08-08:

- [`edata.h`](https://github.com/grame-cncm/guidolib/blob/9f74ba9b3e287f240bbd454c2259fc3f7737c6ad/platforms/win32/finale-plugin/edata.h): storage classes, IDs, tags, and ordinary logical structures;
- [`EEDDATA.H`](https://github.com/grame-cncm/guidolib/blob/9f74ba9b3e287f240bbd454c2259fc3f7737c6ad/platforms/win32/finale-plugin/EEDDATA.H): entry-detail tags and structures;
- [`EXTYPES.H`](https://github.com/grame-cncm/guidolib/blob/9f74ba9b3e287f240bbd454c2259fc3f7737c6ad/platforms/win32/finale-plugin/EXTYPES.H): entry/note API types and PDK version history;
- [`VERSION.H`](https://github.com/grame-cncm/guidolib/blob/9f74ba9b3e287f240bbd454c2259fc3f7737c6ad/platforms/win32/finale-plugin/VERSION.H): primitive widths and tag construction; and
- [`FINEXTND.H`](https://github.com/grame-cncm/guidolib/blob/9f74ba9b3e287f240bbd454c2259fc3f7737c6ad/platforms/win32/finale-plugin/FINEXTND.H): versioned data API behavior.

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

## Finale 2001–2006 physical records and the 16-word hypothesis

**Confirmed across every directly resolved framed sample.** Decoded `0x000f` and `0x0010` pools consist of fixed
**16-byte**, not 16-word, physical records:

| Pool | Physical row | Total |
|---|---|---:|
| `0x000f` other/options | 16-bit comparator, two-byte tag, 12-byte payload | 16 bytes / 8 words |
| `0x0010` details | two 16-bit comparators, two-byte tag, 10-byte payload | 16 bytes / 8 words |

The apparent “two unexplained words” were an accounting error: the remembered payload capacities were 12 and 10
**bytes**, not words. Thus `2 + 2 + 12 = 16` bytes for an other and `4 + 2 + 10 = 16` bytes for a detail, with no
unaccounted trailer or metadata. Incident number is implicit in the ordered run of rows sharing a key/tag.

Across the 375-file directly resolved source subset, all 375 `0x000f` members contain an integral 4,601,857
rows and all 375 `0x0010` members contain an integral 1,574,280 rows; there are no remainder bytes. All 348 nonempty
`0x0011` members are exact multiples of 38 bytes, totaling 837,086 entry rows. In the eight controlled Finale
2002–2005 MUS/ETF files, the decoded/ETF counts match exactly: 4,552 other rows, 272 detail rows, and 24 entries.

The PDK independently explains the row fields and multi-incidence continuation model; the MUS/ETF pairs establish
the serialized sizes and ordering. This is the clearest solved part of the legacy semantic container so far.

## Finale 2007+ generic record frame

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

This later variable frame does not retain the earlier fixed 16-byte physical rows. Many payload sizes are multiples
of the old 12-byte other capacity, suggesting that later versions coalesced successive incidences into one
variable-length payload, but that historical relationship still needs field-level verification.

### Finale 2002 controlled pair

**Strong for this sample; broader version coverage pending.** The F2002 baseline and one-pitch-change pair were saved
by Finale `2002a.r1` under Mac OS 9.0.4 in SheepShaver. Beginning at `0x200`, both files expose a simple outer stream
of big-endian variable-length records:

| Offset | Type | Total record length | Baseline / changed |
|---:|---:|---:|---:|
| `0x0200` | `0x000f` | 1,976 | unchanged |
| `0x09b8` | `0x0010` | 158 | unchanged |
| `0x0a56` | `0x0011` | 53 / 57 | changed |
| `0x0a8b` / `0x0a8f` | `0x0012` | 6 | empty/end-pool marker moved with preceding record |

The six-byte outer header is two bytes of type followed by a four-byte **total record length including the header**.
Records are not constrained to 16 words or 32-byte alignment; the terminal record begins at an odd offset in the
changed file. Changing the pitch from C to D changes 45 observed bytes overall, including the length field and a
four-byte extension, while the complete `0x000f` and `0x0010` records remain identical. The ETF changes only the
`eE` entry payloads, supporting the interpretation that `0x0011` is an entry-related pool and that the edit is
localized at the outer-record level.

Subsequent DCL testing solved the payload codec and confirmed the same checked container across all recognized
2001–2006 samples. The fixed rows inside `0x000f`–`0x0011` are now established, while tag-specific logical field
layouts and `0x0012` text organization remain incomplete. F2003, F2004, and F2005 independently confirm the same
outer framing and broad type sequence.

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

**Finale 2001–2006 framing solved; fields partly mapped.** Every nonempty decoded `0x0011` pool in the directly
resolved corpus is a sequence of 38-byte rows. The controlled documents contain three rows matching three ETF `eE`
records; the first four bytes are the big-endian entry number, and the controlled C-to-D edit changes the note pitch
inside the corresponding row while preserving its size. The public PDK confirms that entry numbers and entry/note
flags are 32-bit and several duration/count fields are 16-bit, but its 146-byte plug-in API structure includes
expanded note capacity and computed fields and must not be mistaken for the 38-byte disk row. Exact raw field and
flag mapping is now a bounded correlation task.

For Finale 2007+, block `0x0016` decompresses cleanly and CRC-validates but does not use either the earlier 38-byte
row or the later generic zero-trailed frame. Its decoded size tracks document complexity; that era remains open.

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
- 2001–2006: PKWARE DCL streams, decoded with `blast`; explicit big-endian CRC-32 and stored block length, confirmed.
- 3.x–2000: uncompressed typed pools with no identified checksum; fixed rows and text framing confirmed.
- pre-banner/Finale 2: separate organization.

No evidence of whole-file encryption was found. No checksum was identified in the pre-2001 eras.

## Failed or revised hypotheses

- Records do **not** start at offset zero; the common body boundary is `0x200`.
- The 2001–2006 fixed physical records are **16 bytes, not 16 words**. The remembered 12/10 payload capacities are
  bytes, so the proposed two unexplained words were an artifact of mixed units.
- The formerly “low-entropy/encoded” Finale 3.x–2000 family is **not encoded or compressed**: it contains the same
  fixed physical rows directly in four typed pools, with platform-dependent byte order.
- DCL block type `0x0012` is **not inherently a terminal marker**; it is compressed and nonempty in many documents.
  A six-byte block of the next sequential type marks the first empty/end pool.
- 2007+ principal records are **not** fixed at 16 words or 32-byte aligned.
- Treating all zlib members as the generic record pool failed; only `0x001a` and `0x001b` fit, while entries and texts use other layouts.
- Byte order is **not** derivable from `MAC` versus `WIN` alone during the 2007–2008 transition.
- XML counts are not always binary counts; Finale 27 expands or normalizes `frameSpec`, smart shapes, text definitions, details, and part-scoped data.
- The 2001–2006 high-entropy payload is **not** an unknown transform or encryption; it is standard PKWARE DCL. Tests
  that considered only zlib missed it.
