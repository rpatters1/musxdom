# Feasibility Assessment

## Conclusion

**Feasible with substantial reverse engineering.** A reader for Finale 3.0–2012 now appears practical at the
container and physical-record layers: Finale 3.x–2000 uses uncompressed typed pools, Finale 2001–2006 uses checked
PKWARE DCL streams around the same fixed rows, and Finale 2007–2012 uses checked zlib streams with a later frame.
Several stable tags have publicly documented meanings. A high-quality all-version converter is not yet justified
because 1.x–2.x pool indexes/boundaries, tag-specific fields, later entry formats, options, and sharing overrides
remain unresolved.

## Current disposition

This investigation was intentionally shelved rather than turned into an implementation. Research resumed after the
PKWARE DCL discovery and the adoption of the public-source PDK provenance policy, but implementation remains out of
scope. The recommended starting point is now field-level assembly of the solved F2002–F2005 rows into versioned
logical records, followed by direct construction of a narrow musxdom subset.

This is stronger than “partial recovery only”: all banner-era wrappers from 3.0 onward are deterministic, the DCL codec
is independently implemented and permissively licensed, generic 2007+ “other” and detail framing is reproducible at
corpus scale, version/creator metadata is explicit, and ETF exports expose the older families' logical sections and
entry structures. The complete archive pass adds 4,898 candidates and explicit Finale 1.8.7–2.6 samples. It is weaker
than “clearly feasible” for all versions because most tag-specific fields, later entries, sharing, and
version-specific pre-banner semantics remain unresolved.

## Feasibility by scope

| Scope | Assessment | Reason |
|---|---|---|
| Header/version inventory for 3.0–2012 | clearly feasible | explicit banner, dates, creator/saver tuples, platform |
| 3.x–2000 container and physical records | clearly feasible | 189/190 files: four uncompressed pools; 16-byte other/detail rows, 38-byte entries, raw text; platform byte order |
| 3.x–2000 partial musical recovery | feasible now with substantial mapping work | exact Finale 2000 MUS/ETF pair plus contemporary public PDK field layouts |
| 2007–2012 container and record extraction | clearly feasible | typed zlib, length, CRC, two framed pools |
| 2007–2012 partial musical recovery | feasible now with substantial mapping work | entry/text formats separate; many other/detail IDs known |
| 2007–2012 high-fidelity conversion | feasible but high risk | sharing, options, normalized Finale 27 evidence, version fields |
| 2001–2006 container and decompression | clearly feasible | 1,603/1,603 encountered DCL members decode and CRC-validate; 410 files also traverse completely under the current outer framing |
| 2001–2006 physical records | clearly feasible | 16-byte other/detail rows and 38-byte entries hold without exception across 375 framed direct-corpus files |
| 2001–2006 partial musical recovery | feasible now with substantial mapping work | exact ETF pairs plus public Finale 2000 tag/layout facts; logical assembly and fields remain |
| archive-derived Finale 1.8.7–2.6 | feasible with substantial reverse engineering | exact detail tags, byte-exact 32-byte entries, raw text, and 16-byte cadence correlate with Finale 3.0; Finale 27 opens tested files after suffix repair; indexes/boundaries unresolved |
| apparent Finale 2 | uncertain | distinct layout, no explicit version banner, sparse clean provenance |
| one universal parser | not recommended | at least four structural/codec families and transition endian variants |

## Largest technical risks

1. **Early pool indexes.** Finale 1.8.7–2.6 core rows correlate strongly with Finale 3.0, but the intervening index/directory spans and generic method for finding every pool boundary remain unresolved.
2. **Later entry pool.** Finale 2001–2006 entries use solved 38-byte rows, but Finale 2007+ block `0x0016` uses a different dense representation.
3. **Sharing/linked parts.** Part scope and shared overrides cut across ordinary record types. Finale 27 may expand references, so naïvely importing converted multiplicities could duplicate or lose relationships.
4. **Options and variable records.** Options are structured in later files but their numeric codes and fields are unmapped. Some payloads are thousands of bytes.
5. **Version semantics.** Type codes are stable across the later corpus, but payload meanings and sizes can change. A release name alone does not choose endian layout in 2007–2008.
6. **Conversion as an imperfect oracle.** Finale 27 reassigns/normalizes/synthesizes data. The new ETF pairs help, but later-version exports can also synthesize records, as shown by the Finale 2000 template saved by Finale 2005.
7. **Pre-banner provenance.** Filename paths identify apparent Finale 2 files, but the binary lacks the later product banner.
8. **Platform bias.** Nearly all examined files originate from Macintosh Finale installations or Macintosh archive workflows. Four Windows files prove that the 3.x–2000 pool and row byte order is platform-sensitive, but Windows string, option, resource-fork-independent, and later-era behavior remains insufficiently tested.

## Significance of sharing

Sharing is the largest quality wildcard rather than a reason to abandon the effort. The corpus contains excellent natural samples: 301 converted files have multiple part definitions and both `shared=true` and `shared=false` are common. The exact mapping `0x011a → partDef` gives an anchor. A controlled three-file link/unlink experiment should reveal whether remaining key words encode part ID and shared state or whether records are duplicated with an override table.

A first reader could explicitly support only score-level records and report part-scoped data as unsupported, but that would be partial recovery, not a complete converter. Silent flattening should be avoided.

## Highest-value additional evidence

1. Field-level parsing of the exact Finale 2000 `tremolos` pair, especially `CN`, `TP`, entry flags, and the compact `#v*`/`#c*`/`#s*` detail identifiers.
2. Exact minimal Finale 2.6.3 MUS/ETF pair to isolate early indexes and boundaries without exporter normalization.
3. Exact minimal Finale 1.0 MUS/ETF pair, if ETF exists there, plus a recorded Finale 27 open attempt.
4. Decoded-binary/ETF correlation for `nestedTupletFin05RC2.mus`: version-expanded tuplets and entry details.
5. Controlled Finale 2012 link/unlink files: sharing keys and overrides.
6. A matched Windows corpus: equivalent documents saved by Windows Finale 3.x–2012, with at least one 2007/2008 transition pair and one pre-banner/early-version sample.

See [EVIDENCE_REQUESTS.md](EVIDENCE_REQUESTS.md) for exact operations and paths.

## Proposed future architecture

The viable endpoint is the existing musxdom class structure; there should be no second public DOM.

1. **`LegacyMusReader` front end:** signature, banner, creator/saver header, format classifier, byte order, integrity diagnostics.
2. **Era codec strategy:** pre-banner, 3.x–2000 uncompressed, 2001–2006 typed-DCL, and 2007–2012 typed-zlib implementations. Codecs emit checked decoded block byte spans, not domain objects. The DCL strategy may vendor Mark Adler's `blast` with its license notice or use an equivalently compatible dependency.
3. **Block router:** uncompressed-era types `1`–`4` and DCL-era `0x000f`–`0x0012` both route to others, details, entries, and text; zlib-era types (`0x1a`, `0x1b`, `0x16`, `0x17`) route to their separate decoders.
4. **Physical-row layer:** expose 3.x–2006 16-byte other/detail and 38-byte entry spans with offsets and byte order. Assemble ordered repeated tag/key rows into versioned logical structures; this is an internal parsing view, not a second DOM.
5. **Versioned record registry:** two-character tag plus format/file revision selects a Finale 3.x–2006 assembler; numeric code plus revision selects a zlib-era decoder. Unknown records are preserved in diagnostics with raw offset/key/payload, not guessed.
6. **Direct musxdom population:** construct `OptionsPool`, `OthersPool`, `DetailsPool`, `EntryPool`, and `TextsPool` using existing classes and object-pool relationships.
7. **Resolver/integrity pass:** reuse `Document`, `ObjectPool`, part IDs, entry references, and existing integrity errors after all pools are populated.
8. **Capability report:** list skipped record codes, part scopes, unsupported options, and conversion confidence per file.

The first proof of concept could now target the Finale 2000 `tremolos` pair or the controlled Finale 2002 document and populate header, entries,
measure attributes (`MS`), staff attributes (`IS`), staff use (`Iu`), page (`PS`), staff system (`SS`), and frame hold
(`GF`) into existing musxdom classes. This is now a lower-risk target than starting with the 2007/2008 transition.
It should explicitly report unsupported fields, text, and sharing.

## Publicly accessible historical PDK material

The project initially did not inspect MakeMusic PDK headers. On 2026-08-08 it adopted the public-source provenance
policy in the README and authorized narrowly scoped consultation of a publicly accessible Finale 2000 PDK copy. The
PDK is not needed for codec discovery, but may answer numeric record-code enums, structure sizes, field order, and
option payload layouts that remain critical semantic blockers.

No PDK header or source may be added to this repository. Notes must distinguish `public-PDK-derived` claims from
`independently binary-verified` claims, cite the public source at an immutable commit, restate facts independently,
and verify them against MUS/ETF/MUSX evidence wherever practical. This is no longer a strict clean-room study; it is
an independently implemented interoperability study with explicit public-source provenance.

## Recommended next steps

1. Complete field-level comparison of the Finale 2000 `tremolos` pair, including 38-byte `eE` rows, `CN`, `TP`, and compact pseudo-detail identifiers.
2. Assemble and field-map `MS`, `IS`, `Iu`, `PS`, `SS`, and `GF` from Finale 2000 through 2005, explicitly accounting for row-count growth.
3. Decode the index/directory spans around the already correlated Finale 1.8.7–2.6 ordinary, detail, entry, and text regions.
4. Determine the framing of nonempty DCL `0x0012` and separate raw text, lyrics, and any font/control tables.
5. Confirm the automatic-note-spacing explanation for the F2004/F2005 `BC` rewrites with a matched disabled-spacing pair.
6. Run the sharing C3 experiment and map part/shared key fields.
7. Build only a disposable proof of concept for the exact Finale 2000 `tremolos` pair or controlled Finale 2002 file, populating existing musxdom classes and reporting every unsupported field/tag.
