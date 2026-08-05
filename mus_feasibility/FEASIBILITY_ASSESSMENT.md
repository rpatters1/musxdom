# Feasibility Assessment

## Conclusion

**Feasible with substantial reverse engineering.** A reader for a limited, later range—especially Finale 2007–2012—appears practical. A high-quality all-version converter is not yet justified because three older format families, entry encoding, option semantics, text layout, version-dependent fields, and sharing overrides remain unresolved.

This is stronger than “partial recovery only”: the later wrapper is deterministic and checked, generic “other” and detail framing is reproducible at corpus scale, version/creator metadata is explicit, and dozens of record identities map directly to the same Enigma concepts already modeled by musxdom. The complete archive pass adds 4,898 candidates and explicit Finale 1.8.7–2.6 samples, extending the evidence backward but also confirming that the earliest pre-banner families do not use the later zlib/framed-record organization. It is weaker than “clearly feasible” for all versions because the 2001–2006 codec and all pre-banner logical record semantics remain unresolved.

## Feasibility by scope

| Scope | Assessment | Reason |
|---|---|---|
| Header/version inventory for 3.0–2012 | clearly feasible | explicit banner, dates, creator/saver tuples, platform |
| 2007–2012 container and record extraction | clearly feasible | typed zlib, length, CRC, two framed pools |
| 2007–2012 partial musical recovery | feasible now with substantial mapping work | entry/text formats separate; many other/detail IDs known |
| 2007–2012 high-fidelity conversion | feasible but high risk | sharing, options, normalized Finale 27 evidence, version fields |
| 2001–2006 reading | feasible candidate, presently blocked at codec | abundant samples and ETF compatibility, but no decompressor |
| 3.x–2000 reading | feasible candidate, lower confidence | visible organization and ETF compatibility; logical stream unresolved |
| archive-derived Finale 1.8.7–2.6 | feasible candidate, presently unresolved | explicit early products, no MUSX exports, no validated zlib/framed records |
| apparent Finale 2 | uncertain | distinct layout, no explicit version banner, sparse clean provenance |
| one universal parser | not recommended | at least four structural/codec families and transition endian variants |

## Largest technical risks

1. **Legacy codecs.** The 2001–2006 high-entropy body is not zlib. The low-entropy 3.x–2000 body also appears encoded/table-driven; visible ASCII is not enough to call it raw records.
2. **Entry pool.** Notes and entries are central to useful conversion, yet block `0x0016` uses a dense separate representation.
3. **Sharing/linked parts.** Part scope and shared overrides cut across ordinary record types. Finale 27 may expand references, so naïvely importing converted multiplicities could duplicate or lose relationships.
4. **Options and variable records.** Options are structured in later files but their numeric codes and fields are unmapped. Some payloads are thousands of bytes.
5. **Version semantics.** Type codes are stable across the later corpus, but payload meanings and sizes can change. A release name alone does not choose endian layout in 2007–2008.
6. **Conversion as an imperfect oracle.** Finale 27 reassigns/normalizes/synthesizes data. Count and value correlations need ETF or controlled differences for confirmation.
7. **Pre-banner provenance.** Filename paths identify apparent Finale 2 files, but the binary lacks the later product banner.
8. **Platform bias.** Nearly all examined files originate from Macintosh Finale installations or Macintosh archive workflows. This leaves Windows-specific byte order, header tuples, resource-fork absence, path/encoding behavior, and any platform-specific serialization insufficiently tested. A reader validated only against this corpus could mistake Mac behavior for format invariants.

## Significance of sharing

Sharing is the largest quality wildcard rather than a reason to abandon the effort. The corpus contains excellent natural samples: 301 converted files have multiple part definitions and both `shared=true` and `shared=false` are common. The exact mapping `0x011a → partDef` gives an anchor. A controlled three-file link/unlink experiment should reveal whether remaining key words encode part ID and shared state or whether records are duplicated with an override table.

A first reader could explicitly support only score-level records and report part-scoped data as unsupported, but that would be partial recovery, not a complete converter. Silent flattening should be avoided.

## Highest-value additional evidence

1. Finale 2005 ETF for `nestedTupletFin05RC2.mus`: codec boundary, entries, tuplets, and record frame.
2. Finale 2005 ETF for the Finale 2000 `bMiniatures/template.mus`: pre/post-2001 codec comparison and option defaults.
3. ETF/opening attempts for archive IDs `mus-7aa45639c14b3864` (1.8.7), `mus-2c0a5e8897b436d5` (2.0.1), and `mus-bd0042f8e0354192` (2.6): earliest explicit eras.
4. ETF for the Finale 3.0 `Freire2.mus`: early low-entropy era.
5. Controlled Finale 2012 link/unlink files: sharing keys and overrides.
6. A matched Windows corpus: equivalent documents saved by Windows Finale 3.x–2012, with at least one 2007/2008 transition pair and one pre-banner/early-version sample.

See [EVIDENCE_REQUESTS.md](EVIDENCE_REQUESTS.md) for exact operations and paths.

## Proposed future architecture

The viable endpoint is the existing musxdom class structure; there should be no second public DOM.

1. **`LegacyMusReader` front end:** signature, banner, creator/saver header, format classifier, byte order, integrity diagnostics.
2. **Era codec strategy:** pre-banner, 3.x–2000, 2001–2006, and 2007–2012 typed-zlib implementations. Codecs emit checked decoded block byte spans, not domain objects.
3. **Block router:** versioned mapping of numeric block types (`0x1a`, `0x1b`, `0x16`, `0x17`) to pool decoders.
4. **Versioned record registry:** numeric code plus format/file revision selects a decoder. Unknown records are preserved in diagnostics with raw offset/key/payload, not guessed.
5. **Direct musxdom population:** construct `OptionsPool`, `OthersPool`, `DetailsPool`, `EntryPool`, and `TextsPool` using existing classes and object-pool relationships. A small internal raw-record view is acceptable as a parsing mechanism, but not a separate document model.
6. **Resolver/integrity pass:** reuse `Document`, `ObjectPool`, part IDs, entry references, and existing integrity errors after all pools are populated.
7. **Capability report:** list skipped record codes, part scopes, unsupported options, and conversion confidence per file.

The first proof of concept should target one minimal Finale 2007/2008 file and populate header plus a few exact-mapped “other”/detail types. It should not claim entry or sharing completeness.

## Proprietary MakeMusic material

It was not inspected. Clean-room evidence is already sufficient to solve the later wrapper and map many types. Proprietary headers might answer three unresolved questions: numeric record-code enums, option payload layouts, and the legacy 2001–2006 codec. Before using them, decide whether the project can accept provenance contamination and possible license/trade-secret/copyright concerns. Even after Finale's discontinuation, that decision is legal/project policy, not a technical assumption.

Recommended policy: continue with ETF and controlled differences first. Reconsider proprietary material only if the codec remains the critical blocker after P1–P4, document exactly what files would be inspected, and keep any resulting implementation provenance explicit. Legal advice may be appropriate before distribution.

## Recommended next steps

1. Obtain and analyze P1 and P2 ETF exports.
2. Determine the 2001–2006 decompressor and test whether decoded records share 2007 numeric codes/trailers.
3. Decode entry block `0x0016` using minimal controlled note changes and Finale 27 entry values.
4. Run the sharing C3 experiment and map part/shared key fields.
5. Confirm high-confidence record names with field-level distinctive values, not counts alone.
6. Only then build a disposable reader that populates a narrow subset of musxdom and reports every unsupported record.
