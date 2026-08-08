# Experiment Log

## 2026-08-05 — Corpus enumeration

- **Question:** What evidence is actually present?
- **Method:** recursive `find`, then deterministic `scripts/inventory.py`; full-file SHA-256 for legacy sources and exact matched exports.
- **Observation:** 1,218 `.mus`, 2,042 `.fin27.musx`, 1,189 exact adjacent matches, 29 unmatched sources. Legacy data totals 165,758,809 bytes; matched exports total 178,990,189 bytes.
- **Conclusion:** The corpus is much larger than an initial shallow listing suggested and supports statistical comparison. The assertion that every source has an export is nearly, but not completely, true.
- **Follow-up:** preserve full rows in `private/corpus_inventory.csv` and sanitized findings in `CORPUS_INVENTORY.md`.

## 2026-08-05 — Explicit saving version

- **Question:** Is the saving Finale version encoded?
- **Method:** hex dump bytes `0x000–0x080` and aggregate strings from bytes `0x020–0x060`.
- **Observation:** Banner-era files say `Finale(R) <product> Copyright...`; products range 3.0–2012. Counts are in `inventory_summary.json`.
- **Conclusion:** Saving-product identification is direct for 1,163 files. Filename/timestamp classification is unnecessary there.
- **Failed hypothesis:** version classification would require record-set heuristics. It does not for banner-era files.

## 2026-08-05 — Creator versus saver header

- **Question:** Can creation and last save be distinguished?
- **Method:** interpret bytes at `0x66` and `0x8c` as `tm_year`, month, day; compare to filesystem dates and Finale 27 EnigmaXml header.
- **Observation:** `0x8c` date matches filesystem day in 98.6% of valid cases. `0x66` precedes/equal `0x8c` in all 1,039 valid pairs. Adjacent creator/saver version/platform tuples decode semantically in the Finale 27 export.
- **Conclusion:** The header distinguishes creation metadata, last-save metadata, internal Enigma/application/file versions, and platform. Exact binary tuple packing remains partly open.

## 2026-08-05 — Body clusters and entropy

- **Question:** Are there format eras?
- **Method:** entropy of up to 16 KiB after `0x200`, prefix clustering, zlib validation.
- **Observation:** 3.x–2000 is low entropy (roughly 3.1–4.1); 2001–2006 abruptly becomes high entropy (usually 7.3–7.8) without zlib; 2007+ has four valid zlib members; pre-banner files differ again.
- **Conclusion:** At least four codec/layout strategies are needed. The 2000/2001 boundary is high value for ETF comparison.

## 2026-08-05 — Typed zlib wrapper

- **Question:** How are 2007+ streams delimited and checked?
- **Method:** parse both byte orders from `0x200`; validate stored size, zlib EOF, and CRC-32.
- **Observation:** header is type (2), total length (4), CRC-32 (4). Principal types repeat as `0x1a`, `0x1b`, `0x16`, `0x17`; terminal six-byte `0x13`/`0x1d` markers follow. 2007 and 2008 contain both endian variants.
- **Conclusion:** Wrapper traversal and corruption detection are solved for this era.
- **Failed hypothesis:** platform string alone selects byte order. Transition-era Mac files use both.

## 2026-08-05 — 16-word record test

- **Question:** Are logical records fixed at 16 words, and what are the two unexplained words?
- **Method:** decompress block `0x1a`; test candidate strides and then a length-driven frame. Require exact full-member consumption and four zero trailer bytes on every record.
- **Observation:** the fixed strides drift. A variable frame succeeds on more than 1.59 million records. Common 12-byte records serialize to 13 words, not 16; large payloads vary widely. Every accepted record ends in two zero words.
- **Conclusion:** Fixed 16-word records are disproved for framed 2007+ pools. The two unexplained words are a stable zero trailer/reserved terminator in this era.
- **Open:** whether this represents padding, reserved metadata, or a terminator; whether pre-2007 decompressed records differ.

## 2026-08-05 — Pool identification

- **Question:** What do the four block types contain?
- **Method:** inspect decoded strings/structure and compare Finale 27 XML pool counts.
- **Observation:** `0x1a` accepts other/option frame and record counts; `0x1b` accepts detail frame; `0x16` scales with entries but fails generic framing; `0x17` exposes text/font/Enigma commands.
- **Conclusion:** block categories are strongly identified. Entry/text layouts remain separate work.
- **Failed hypothesis:** one generic record parser covers all four blocks.

## 2026-08-05 — Finale 27 semantic census

- **Question:** Can converted XML give record identities at corpus scale?
- **Method:** decode all 1,189 matched `score.dat` members in memory with `scripts/musx_semantics.py`; count direct pool children. Correlate numeric binary code vectors with XML tag vectors over 375 other-framed and 324 detail-framed files.
- **Observation:** dozens of exact/near-exact mappings. Examples: `0x0086/durAllot` exact 375/375, `0x011a/partDef` exact 375/375, `0x03f3/baselinesExprAboveStaff` exact 324/324. Others show conversion expansion but correlations above 0.99.
- **Conclusion:** type mapping is practical without proprietary code. Count collisions remain possible; field-level value correlation or ETF should confirm names.
- **Failed hypothesis:** a Finale 27 XML object count always equals legacy binary count. Counterexamples include `frameSpec`, smart shapes, text definitions, and part-scoped data.

## 2026-08-05 — Sharing census

- **Question:** Is sharing visible and localized?
- **Method:** count `partDef`, `part`, and `shared` in decoded Finale 27 XML.
- **Observation:** 301 multi-part documents, 315 with part-scoped objects, 2,188,767 part-scoped elements, and both true/false shared attributes. `0x011a` maps to `partDef`.
- **Conclusion:** sharing is pervasive within ordinary record types rather than visibly isolated in a named XML section. Legacy duplication versus references remains unresolved.

## 2026-08-05 — Public-source search

- **Question:** Is an existing open decoder documented?
- **Method:** web search for exact signature and Finale MUS binary decoder/source.
- **Observation:** public preservation descriptions confirm MUS/ETF roles, but no primary open MUS decoder or technical specification was found in the searched results.
- **Conclusion:** continue clean-room correlation. No proprietary MakeMusic source was accessed.

## 2026-08-05 — Finale 2002 controlled MUS/ETF pair

- **Question:** Does a same-version one-pitch edit reveal outer record framing in the 2001–2006 high-entropy family?
- **Files examined:** `private/evidence/F2002/F2002-baseline.mus` and `.etf`, plus `F2002-changed-C-to-D.mus` and `.etf`; provenance records Finale `2002a.r1`, Mac OS 9.0.4, SheepShaver.
- **Method:** SHA-256, binary comparison, hex inspection from `0x200`, typed/length candidate scan, and normalized ETF diff.
- **Observation:** The baseline is 2,713 bytes; the changed file is 2,717 bytes. Both contain big-endian records beginning at `0x200`: type `0x000f`, total length 1,976; type `0x0010`, total length 158; type `0x0011`, total length 53/57; and terminal type `0x0012`, total length 6. The length includes the six-byte header. The changed terminal marker moves by four bytes.
- **Observation:** Only 45 byte positions differ before the baseline EOF, from 1-based offset 2,652 through 2,712; the preceding `0x000f` and `0x0010` records are byte-identical. The ETF diff changes the three `eE` entry records, including the note value from `0` to `16` and related entry fields.
- **Conclusion at the time:** This pair provides strong evidence for a variable-length typed outer container in Finale 2002, not fixed 16-word records. It localizes pitch/entry changes to type `0x0011`. The payload codec was still unresolved at this stage; the 2026-08-08 DCL experiment below supersedes that limitation.
- **Follow-up:** Create the same minimal baseline and one-pitch variant in F2003, F2004, and F2005; compare type sequences, length semantics, terminal markers, and whether entry edits remain isolated to the corresponding pool.

## 2026-08-05 — Finale 2003 controlled MUS/ETF pair

- **Question:** Does the F2002 typed/length outer framing persist in Finale 2003, and does a one-pitch edit remain localized to the entry pool?
- **Files examined:** Tracked `evidence/F2003/F2003-baseline.mus` and `.etf`, plus `F2003-changed-C-to-D.mus` and `.etf`; provenance records Finale `2003a.r1`, Mac OS 9.0.4, SheepShaver.
- **Method:** SHA-256, binary comparison, scan for big-endian type/total-length records from `0x200`, and normalized ETF diff.
- **Observation:** The baseline is 3,088 bytes; the changed file is 3,089 bytes. Both contain the same outer sequence: `0x000f` length 2,346 at `0x0200`; `0x0010` length 158 at `0x0b2a`; `0x0011` length 58/59 at `0x0bc8`; and terminal `0x0012` length 6 at `0x0c02`/`0x0c03`.
- **Observation:** Only 44 byte positions differ before the baseline EOF, from 1-based offset 3,022 through 3,088; the preceding records remain byte-identical. The ETF changes only the first `eE` note payload from `0` to `16`.
- **Conclusion at the time:** Finale 2003 independently confirms the F2002 six-byte big-endian typed/length outer framing and localized `0x0011` entry-pool edits. The framing was strong across two adjacent releases; the 2026-08-08 DCL experiment below subsequently solved the payload codec.
- **Follow-up:** Repeat the same pair in F2004 and F2005. If the framing persists, focus the reverse engineering on the payload bytes and test whether the type/payload vocabulary is stable across 2002–2005.

## 2026-08-05 — Finale 2004/2005 controlled MUS/ETF pairs

- **Question:** Does the F2002/F2003 outer framing persist through Finale 2004 and 2005, and does the one-pitch edit remain localized?
- **Files examined:** Tracked `evidence/F2004/` and `evidence/F2005/` baseline and C-to-D MUS/ETF pairs. Provenance records Finale `2004c.r1` and `2005b.r1`, both Mac OS X 10.4.11 under a QEMU PowerPC emulator.
- **Method:** SHA-256, binary comparison, big-endian type/total-length scan from `0x200`, and normalized ETF diff.
- **Observation:** F2004 baseline/changed sizes are 3,179/3,181 bytes; F2005 sizes are 3,183/3,186 bytes. Both preserve the four-record sequence: `0x000f`, `0x0010`, `0x0011`, and terminal `0x0012`, with six-byte headers and total lengths. F2004 lengths are `2429, 166, 58→59, 6`; F2005 lengths are `2436, 166, 55→57, 6`.
- **Observation:** Unlike F2002/F2003, F2004 and F2005 also change ETF `BC` records when C is changed to D. The binary diff therefore spans much of the file, even though the entry pool still changes in `0x0011`. The leading explanation is that these documents have automatic note spacing enabled, causing derived spacing/layout records to be rewritten; this is not yet confirmed.
- **Conclusion at the time:** The typed six-byte big-endian outer framing and four principal record types were strongly reproduced across Finale 2002, 2003, 2004, and 2005. These controlled pairs did not identify the payload codec; the 2026-08-08 DCL experiment below did. The `BC` difference should be treated as a configuration-dependent derived-record effect until a matched automatic-spacing-disabled test confirms it.
- **Revised follow-up:** Decode both sides with `blast`, then use a less layout-sensitive edit to map internal records and distinguish direct entry changes from derived layout changes.

## 2026-08-05 — Clean public ETF/MUS search

- **Question:** Is there a public, non-proprietary description or implementation of the ETF grammar or the 2001–2006 MUS codec?
- **Method:** Search public preservation records, Finale manuals/help, historical format articles, independent ETF tooling, and public source-code indexes. No MakeMusic plugin-development source or headers were accessed.
- **Observations:** The Library of Congress describes ETF as Finale's plain-text transport counterpart to binary MUS and states that no formal MUS specification is available. Finale documentation says ETF files could be created through Finale 2006, but not from Finale 2007 onward. LilyPond's `etf2ly` utility documents an independent parser for a subset of ETF, useful for validating ETF grammar and entry syntax but not for decoding binary MUS. Historical public articles provide signatures and examples, but no 2001–2006 binary codec.
- **Conclusion:** The public search materially improves ETF grammar knowledge but found no credible public 2001–2006 MUS codec implementation or specification. Continue with exact same-version MUS/ETF pairs and controlled differences before considering the side-channel.
- **References:** Library of Congress MUS description; Finale help/glossary; LilyPond `etf2ly` manual; Tyler Thorsted's historical Finale format notes. URLs are listed in the final report and README's external references.

## 2026-08-05 — Archive and extensionless survey

- **Question:** Do archives contain additional Finale files, including files without `.mus` suffixes or earlier than the direct corpus?
- **Method:** `scripts/archive_probe.py` scanned ZIP archives and hashed candidate `.mus`/extensionless members without altering originals; candidate Finale 2.6 members were extracted to `/tmp` for the existing structure probe. StuffIt archives were counted but not treated as extracted because no compatible extractor was available.
- **Observation:** 230 ZIP archives yielded 3,468 candidate members (1,737 with Enigma banners, 1,846 extensionless, 1,870 unique member hashes). 831 unique Enigma hashes were not in the direct inventory. Nine members explicitly identify as `Finale(TM) 2.6`; all show a pre-banner/low-entropy body family and had no Finale 27 counterpart at this stage. Later targeted conversion succeeded after adding `.mus`. An archive of extensionless originals contains normal Enigma-banner files, confirming that suffix absence is not itself a different binary format.
- **Conclusion:** Archives materially expand the corpus and provide the earliest explicit-version samples found so far. ZIP-derived candidates are inventoried in `data/archive_members.csv`; the nine 2.6 probes are summarized in `data/archive_legacy_probe.csv`. The 197 `.sit` files remain an extraction gap.
- **Follow-up:** Request ETF exports for the highest-value 2.6 samples and obtain a compatible StuffIt extractor before selecting additional candidates.

## 2026-08-05 — Complete StuffIt extraction

- **Question:** Does the expanded corpus contain earlier Finale formats inside StuffIt archives?
- **Method:** Installed `unar`/`lsar` 1.10.7; listed and extracted all 275 `.sit` files into temporary directories, preserving data-fork hashes and observing resource-fork extraction. Combined results with the 230 ZIP archives.
- **Observation:** The complete archive pass produced 4,898 candidate members, including 2,271 Enigma-banner members and 2,990 extensionless candidates. Explicit pre-banner products include 1.8.7 (10 unique hashes), 2.0.1 (26), 2.6 (100), 3.0 (15), 3.2 (14), 3.5 (19), and 3.7 (25).
- **Conclusion:** StuffIt archives substantially extend the corpus backward. Finale 1.8.7 is the earliest explicit product currently found; no explicit Finale 1.0 sample has been identified. Resource forks can be extracted by `unar` and should be retained for future metadata analysis.
- **Follow-up:** Add targeted ETF/opening requests for 1.8.7, 2.0.1, and 2.6 representatives; inspect any remaining pre-banner candidates for earlier implicit versions.

## 2026-08-05 — Platform coverage risk

- **Observation:** The direct and archive corpora are overwhelmingly Macintosh-derived, including classic Mac resource-fork and StuffIt evidence. Current Windows-origin samples are too sparse to establish platform invariants.
- **Conclusion:** Platform bias is a material feasibility risk. Header tuples, byte order, string encoding, padding, and serialization behavior may differ on Windows even when Finale release labels match.
- **Follow-up:** Request a matched Windows corpus and Mac/Windows same-document saves, especially around the 2007/2008 transition.

## 2026-08-05 — ETF evidence set

- **Question:** Can ETF exports provide a semantic bridge to the pre-2007 binary families, and what does the same source document reveal when exported by Finale 2000 versus Finale 2005?
- **Files examined:** Local private evidence assets `nestedTupletFin05RC2.etf`, `template-Fin2000-from-Fin2000.etf`, `template-Fin2000-from-Fin2005.etf`, `guitar pc.etf`, `Dream of Summer I-from-Fin2.6.3.etf`, and `Score-from-sit-archive.etf`. The corresponding source IDs are `mus-d89e8fe12e271440`, `mus-3597fd4fce0c272b`, `mus-7aa45639c14b3864`, `mus-2c0a5e8897b436d5`, and the Finale 2.6 `Score` archive class.
- **Method:** SHA-256 and byte counts; normalize the classic ETF carriage-return line endings for section and structure counting; inspect headers, section order, structure identifiers, `eE` entry records, and controlled template differences.
- **Observation:** ETF sizes/hashes are: `nestedTupletFin05RC2.etf` 16,893 bytes (`4345805a001e9c198cc2f022c8c469cb1654e290d4c5e8000d16e8d547f64517`); `template-Fin2000-from-Fin2000.etf` 27,945 (`76ce06887b36720aac51ef2578eda11ece72cf3c921ccf832c3f5790eed413de`); `template-Fin2000-from-Fin2005.etf` 34,029 (`b96c24f43be5bcb0a54ffbd7a44a42311c2cae8cf45b0a4c4e2180ca4f7f2e76`); `guitar pc.etf` 123,084 (`d30d569e4b9cc4e642d9a494d1efdc2a975f624a7353f26b2d22a133b1e49844`); `Dream of Summer I-from-Fin2.6.3.etf` 74,040 (`03aa22ec769a5d10dbdb2cde4512cf31d9730c95cad109ebd766f02368edf15e`); `Score-from-sit-archive.etf` 1,272,164 (`ca9ed81de7f782bf1e2e2ccf22714018dae8bb75648d4ff083e01b97a6fd58c1`).
- **Observation:** All six exports contain `others`, `details`, `entries`, `text`, and `lyrics` sections. The older exports use `eE` entry structures at scale: 1,094 lines for `guitar pc`, 549 for Dream of Summer I, and 9,446 for Score. The Finale 2005 nested-tuplet sample contains six `eE` entries; the Finale 2000 template contains no `eE` entries.
- **Observation:** The ETF header explicitly identifies Finale 2000 or Finale 2005 for the template pair and Finale 2005 for the nested-tuplet sample. The older pre-banner ETFs do not have the modern `^header` section, so their exporter/source version cannot be inferred from ETF structure alone.
- **Observation:** The same Finale 2000 template exported by Finale 2005 is larger and contains additional `&f`, `PD`, `XA`, expression, and other records; the saver header also changes to Finale 2005. This demonstrates semantic upgrade/synthesis during resave.
- **Observation:** The ZIP copy of the Finale 2.6 Quartet `Score` could not be opened by Finale 2.6.3 because the ZIP did not preserve the classic Mac resource fork. The parallel StuffIt extraction supplied the readable source and ETF.
- **Conclusion:** ETF confirms that the conceptual other/detail/entry/text decomposition predates the 2007 typed-zlib container and gives strong logical targets for the old binary families. It does not by itself reveal the pre-2007 binary codec or prove byte-for-byte correspondence. Later-version ETF exports must be treated as normalized semantic references.
- **Follow-up:** Correlate ETF structure lines with binary offsets for the five available source classes; use the two template exports to separate source-document records from Finale 2005 upgrade records; obtain the missing Finale 3.0 ETF before attempting a general legacy decoder.

## 2026-08-08 — PKWARE DCL identification and corpus-wide validation

- **Question:** Are the Finale 2001–2006 high-entropy members PKWARE Data Compression Library (DCL) streams, and can Mark Adler's open-source `blast` implementation decode them reliably?
- **Method:** Download `blast.c`, `blast.h`, and the small test driver from the official `madler/zlib` repository; compile them outside the repository; walk big-endian typed/total-length blocks from `0x200`; treat bytes 6–9 of each nonempty block as a big-endian checksum and bytes 10 onward as the candidate DCL stream; run `blast`; compute CRC-32 over the decoded output. Test every structurally recognized Finale 2001–2006 file resolved through the private corpus mapping. No proprietary MakeMusic material was accessed.
- **Observation:** All 1,603 candidate compressed members encountered returned `blast` success and matched the stored CRC-32. Separately, 410 files traversed cleanly as complete typed-block sequences. Counts by saving product were: 2001, 3/4 completely framed files and 9 decoded streams; 2002, 47/48 and 181; 2003, 168/168 and 659; 2004, 7/7 and 25; 2004b, 6/13 and 23; 2005, 116/120 and 459; 2006, 63/66 and 247. Some incompletely framed files contributed valid leading streams before the probe reached an unknown or malformed outer block; they were framing/classification failures, not DCL failures.
- **Observation:** Every stream begins with `00 04` or `01 04`, representing DCL literal modes 0/1 and dictionary parameter 4. There were 1,202 `00 04` streams and 401 `01 04` streams. The controlled F2002 baseline `0x000f` stream expands from 1,966 to 8,000 bytes; both its stored and computed CRC-32 are `0xcb68f112`. Its output begins with orderly rows containing `&f` identifiers rather than high-entropy bytes.
- **Conclusion:** **Confirmed.** PKWARE DCL is the Finale 2001–2006 payload codec, and the preceding four bytes are the CRC-32 of the decompressed member. This supersedes the earlier “unknown high-entropy codec” conclusion. The major remaining task for this era is decoded record/pool interpretation, not decompression.
- **Terminology caveat:** This is the format used by the PKWARE DCL `implode()` function, which `blast.h` explicitly distinguishes from PKZIP's incompatible compression method also named “implode.” `blast` is in zlib's `contrib` tree, not the core zlib API.
- **References:** [official `blast` directory](https://github.com/madler/zlib/tree/master/contrib/blast), [`blast.h`](https://github.com/madler/zlib/blob/master/contrib/blast/blast.h).
- **Follow-up:** Add a deterministic, read-only DCL block probe to the analysis scripts; catalog decoded `0x000f`–`0x0012` framing; correlate the F2002–F2005 controlled pairs and the Finale 2005 nested-tuplet ETF with decoded offsets and values.

## 2026-08-08 — Public Finale 2000 PDK consultation and fixed-row validation

- **Boundary change:** The project replaced its strict clean-room rule with the public-source provenance policy in the README. Public historical PDK material may be consulted for interoperability facts, but no PDK source is stored in this repository; derived claims remain labeled until independently checked against the corpus.
- **Public source:** GRAME GUIDOLib commit `9f74ba9b3e287f240bbd454c2259fc3f7737c6ad`, `platforms/win32/finale-plugin/`. Its public README explicitly calls the included material the Finale 2000 PDK. Accessed 2026-08-08. The data-definition, entry-detail, primitive-type, and data-API headers were consulted; plug-in implementation source was not needed.
- **Question:** Does the PDK explain the decoded 2001–2006 row organization, the proposed 16-word records, and the two allegedly missing words?
- **PDK-derived observation:** A 32-bit extended tag combines a 16-bit storage class with a two-character low-word tag. Other IDs carry one 16-bit comparator, details carry two, and entries carry a 32-bit entry number. Larger logical structures are stored across successive physical incidences; strings, arrays, and special structures use separate storage classes.
- **Controlled-pair method:** Decode all four F2002–F2005 baseline/change pairs with `blast`; split `0x000f` and `0x0010` on 16-byte boundaries and `0x0011` on 38-byte boundaries; count the corresponding ETF section lines.
- **Controlled-pair observation:** All eight files match exactly: 4,552 decoded 16-byte other rows equal 4,552 ETF `others` lines; 272 decoded 16-byte detail rows equal 272 ETF `details` lines; 24 decoded 38-byte entry rows equal 24 ETF `eE` records. In each ordinary other row the serialized fields are one comparator, a two-byte tag, and 12 payload bytes. In each detail row they are two comparators, a two-byte tag, and 10 payload bytes.
- **Corpus method:** Run a direct-source fixed-row pass over the 375 resolved Finale 2001–2006 files, then use `scripts/dcl_probe.py` over every source resolvable from the private location mapping (including the broader archive-derived survey), using a compiled `blast`-compatible executable and reporting aggregate counts only.
- **Corpus observation:** All 375 files in the direct-source pass have `0x000f` and `0x0010` decoded lengths divisible by 16, totaling 4,601,857 other rows and 1,574,280 detail rows. All 348 nonempty `0x0011` members are divisible by 38, totaling 837,086 entry rows. The broader probe finds 410 completely framed files; its totals differ because it covers archive-derived locations as well. No row-size counterexample occurred from Finale 2001 through Finale 2006.
- **Conclusion:** The historical fixed record is **16 bytes (eight words), not 16 words**. The remembered 12/10 payload capacities are bytes. Therefore the two unexplained words do not exist in 2001–2006 ordinary other/detail rows; the accounting was off by a factor of two. Incident is implicit in ordered repeated rows rather than serialized as an additional word.
- **Version observation:** Stable tags survive logical structure growth. In the controlled ETF baselines, `IS` uses three rows in Finale 2002 and six from Finale 2003; `MS` and `SS` use two rows through Finale 2004 and three in Finale 2005. `Iu` and `PS` remain two. This supports a stable physical-row parser plus versioned tag-specific assemblers.
- **Failed hypothesis:** `0x0012` is a terminal marker. Many corpus files contain a nonempty, DCL-compressed `0x0012`; empty six-byte blocks instead mark the first absent/end pool. In 58 Finale 2006 files a nonempty `0x0012` is followed by empty `0x0013`. Text/lyrics are the leading interpretation of nonempty `0x0012`, but this is not yet field-verified.
- **Follow-up:** Decode exact raw fields for `MS`, `IS`, `Iu`, `PS`, `SS`, `GF`, and `eE`; compare PDK-era logical sizes with each saving version; then map the resulting values directly into existing musxdom classes.

## 2026-08-08 — Finale 2000 exact ETF pair and uncompressed-era validation

- **Question:** Does the Finale 2000 low-entropy body contain the same fixed rows as Finale 2001–2006, and can an exact MUS/ETF pair reveal its container?
- **Files examined:** `mus-3a8b724cf3adba80` (`tremolos.mus`, 20,149 bytes) and the newly supplied local-only `tremolos-from-Fin2000.etf`, 28,718 bytes, SHA-256 `c02e859d6026de960a44ea07bd0d3154e07e7f85d690cc5eaf5a84b623d3149d`; then all direct Finale 3.0, 3.2, 3.5, 3.7, 97, and 2000 files resolved through the private mapping.
- **Method:** Walk two-byte type/four-byte total-length blocks from `0x200` in both byte orders; require exact EOF consumption and type sequence `1,2,3,4`; test pool payloads modulo 16/16/38; compare ETF section counts and tag order; normalize only ETF separators between text records and compare raw bytes. The reusable aggregate probe is `scripts/uncompressed_probe.py` and never emits source paths.
- **Pair observation:** The binary blocks begin at `0x0200`, `0x4736`, `0x4b3c`, and `0x4c72`, with types `1,2,3,4` and total sizes `0x4536`, `0x0406`, `0x0136`, and `0x0243`. Their payloads contain 1,107 16-byte other rows, 64 16-byte detail rows, eight 38-byte entries, and 573 text bytes, exactly matching the ETF section counts. All ordinary tags are in identical order; ordinary values such as `IS`, `Iu`, `MS`, `PS`, and `SS`, detail values such as `CN`, `GF`, and `TP`, and selected `eE` fields are directly recognizable. After removing ETF record separators, the complete text section is byte-identical to block 4.
- **Corpus observation:** 189/190 direct Finale 3.x–2000 files traverse exactly as four pools. All 1,552,762 type-1 rows and 770,960 type-2 rows divide by 16, and all 394,984 type-3 rows divide by 38. The recognized population is 185 big-endian files and four little-endian files. All four little-endian files are Windows-origin: the three Finale 3.0 samples and one Finale 2000 sample. One Finale 97 file remains unrecognized.
- **Conclusion:** **Confirmed.** Finale 3.x–2000 stores the same fixed physical other/detail/entry rows uncompressed. Finale 2001 primarily changes the pool identifiers and adds CRC-protected DCL compression. The previous “low-entropy codec/table encoding” hypothesis is disproved for the recognized banner-era files. Byte order is platform-sensitive and should be validated from the block headers.
- **Follow-up:** Parse all type-1/type-2 rows into versioned PDK-era logical structures; investigate the compact binary encoding of ETF pseudo-tags `#v*`, `#c*`, and `#s*`; classify the remaining Finale 97 file; then test whether the pre-banner Finale 1.x–2.x families use an earlier form of the same four pools.

## 2026-08-08 — Finale 1.8.7–2.6 correlation with Finale 3.0

- **Question:** Are the explicit early archive files structurally unrelated to Finale 3.0, or do they contain an earlier serialization of the same records?
- **Files examined:** Re-extracted data forks for `mus-7aa45639c14b3864` (Finale 1.8.7), `mus-2c0a5e8897b436d5` (Finale 2.0.1), and the Finale 2.6 `Score` source class, paired with their existing private ETFs. ZIP and several parallel StuffIt `Score` copies were distinguished by hash rather than assumed identical.
- **Method:** Inspect from `0x200`; test 16-byte cadence from `0x20a`; compare literal and compact tag sequences; reconstruct each old `eE` record as four 32-bit values, two packed 16-bit values, and three 32-bit flag values; compare all reconstructed 32-byte rows against the binary; locate raw `^block` text and compare ETF content.
- **Observation:** All three binaries begin 16-byte records at `0x20a`. Finale 1.8.7 matches all 1,447 ordinary ETF tags; the 2.0.1 and 2.6 comparisons match initial prefixes before later-export/source-copy normalization changes ordinary ordering. Detail tags match completely: 891/891, 497/497, and 6,814/6,814, including compact encodings `0x80xx`, `0x90xx`, and `0xa0xx` for ETF `#v*`, `#c*`, and `#s*` pseudo-tags.
- **Entry observation:** Entry IDs are implicit array positions. Every ETF entry reconstructs its corresponding 32-byte binary row exactly: 1,094/1,094, 549/549, and 9,446/9,446. Finale 3.x extends that same entry body to 38 bytes by prepending the explicit four-byte entry ID and appending two zero/reserved bytes.
- **Text observation:** The ETF text payloads are exact prefixes of the raw binary tails: 405, 936, and 1,401 bytes respectively. Each binary has 13 additional trailing bytes outside the ETF text payload.
- **Conclusion:** **Strongly confirmed continuity.** Finale 1.8.7–2.6 and Finale 3.0 share the logical record vocabulary, 16-byte ordinary/detail cadence, entry field representation, and raw text. Finale 3.0 adds explicit typed/length pool framing, rotates key fields within ordinary/detail rows, and expands entries from implicit-ID 32-byte rows to explicit-ID 38-byte rows. The early index/directory spans and generic pool-boundary algorithm remain unresolved.
- **Finale 27 result:** Finale 27 opened all three sources—1.8.7, 2.0.1, and 2.6—after `.mus` was appended to the extensionless filenames. It produced valid MUSX containers with decoded pool counts of 2,018/787/1,320, 1,347/478/659, and 14,913/6,095/11,153 for others/details/entries. The 2.6 file had font issues but converted. This disproves a parser cutoff at 2.6.x for the tested data forks and identifies filename recognition as the original barrier.
- **Follow-up:** Request exact minimal MUS/ETF pairs saved and exported by Finale 2.6.3 and, if supported, Finale 1.0; test Finale 1.0 separately because it remains outside the verified compatibility range.

## Commands

Reproduction commands are in [README.md](README.md). Additional spot checks used `xxd -g 1`, `strings -a`, `unzip -l`, `unzip -p`, Python's `zlib`, `gzip`, `zipfile`, and `xml.etree.ElementTree`. Temporary decoded samples were written only under `/tmp`.
