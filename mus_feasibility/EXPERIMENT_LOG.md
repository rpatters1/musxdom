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

## 2026-08-05 — Archive and extensionless survey

- **Question:** Do archives contain additional Finale files, including files without `.mus` suffixes or earlier than the direct corpus?
- **Method:** `scripts/archive_probe.py` scanned ZIP archives and hashed candidate `.mus`/extensionless members without altering originals; candidate Finale 2.6 members were extracted to `/tmp` for the existing structure probe. StuffIt archives were counted but not treated as extracted because no compatible extractor was available.
- **Observation:** 230 ZIP archives yielded 3,468 candidate members (1,737 with Enigma banners, 1,846 extensionless, 1,870 unique member hashes). 831 unique Enigma hashes were not in the direct inventory. Nine members explicitly identify as `Finale(TM) 2.6`; all show a pre-banner/low-entropy body family and have no Finale 27 counterpart. An archive of extensionless originals contains normal Enigma-banner files, confirming that suffix absence is not itself a different binary format.
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

## Commands

Reproduction commands are in [README.md](README.md). Additional spot checks used `xxd -g 1`, `strings -a`, `unzip -l`, `unzip -p`, Python's `zlib`, `gzip`, `zipfile`, and `xml.etree.ElementTree`. Temporary decoded samples were written only under `/tmp`.
