# Feasibility Assessment

## Conclusion

**Feasible with substantial reverse engineering.** A reader for a limited, later range—especially Finale 2007–2012—appears practical. A high-quality all-version converter is not yet justified because three older format families, entry encoding, option semantics, text layout, version-dependent fields, and sharing overrides remain unresolved.

## Current disposition

This investigation is intentionally shelved rather than turned into an implementation. The evidence does not support a complete reader with the current effort and tooling, but it leaves a strong, reproducible foundation for a future, more capable researcher or LLM. The tracked F2002–F2005 pairs, format-era notes, and prioritized evidence requests are the recommended resume point.

This is stronger than “partial recovery only”: the later wrapper is deterministic and checked, generic “other” and detail framing is reproducible at corpus scale, version/creator metadata is explicit, dozens of record identities map directly to the same Enigma concepts already modeled by musxdom, and six ETF exports now expose the older families' logical sections and entry structures. The complete archive pass adds 4,898 candidates and explicit Finale 1.8.7–2.6 samples, extending the evidence backward but also confirming that the earliest pre-banner families do not use the later zlib/framed-record organization. It is weaker than “clearly feasible” for all versions because the 2001–2006 codec, binary-to-ETF correspondence, and version-specific pre-banner semantics remain unresolved.

## Feasibility by scope

| Scope | Assessment | Reason |
|---|---|---|
| Header/version inventory for 3.0–2012 | clearly feasible | explicit banner, dates, creator/saver tuples, platform |
| 2007–2012 container and record extraction | clearly feasible | typed zlib, length, CRC, two framed pools |
| 2007–2012 partial musical recovery | feasible now with substantial mapping work | entry/text formats separate; many other/detail IDs known |
| 2007–2012 high-fidelity conversion | feasible but high risk | sharing, options, normalized Finale 27 evidence, version fields |
| 2001–2006 reading | feasible candidate, presently blocked at payload codec | F2002–F2005 independently reveal a variable-length typed outer stream; payload decoding and derived-record semantics remain unresolved |
| 3.x–2000 reading | feasible candidate, moderate confidence | ETF confirms logical sections and entries; binary stream and version-specific fields remain unresolved |
| archive-derived Finale 1.8.7–2.6 | feasible candidate, improved evidence but unresolved | explicit early products plus three ETF exports; no MUSX exports and no validated zlib/framed records |
| apparent Finale 2 | uncertain | distinct layout, no explicit version banner, sparse clean provenance |
| one universal parser | not recommended | at least four structural/codec families and transition endian variants |

## Largest technical risks

1. **Legacy codecs.** The 2001–2006 high-entropy body is not zlib. The low-entropy 3.x–2000 body also appears encoded/table-driven; visible ASCII is not enough to call it raw records.
2. **Entry pool.** Notes and entries are central to useful conversion, yet block `0x0016` uses a dense separate representation.
3. **Sharing/linked parts.** Part scope and shared overrides cut across ordinary record types. Finale 27 may expand references, so naïvely importing converted multiplicities could duplicate or lose relationships.
4. **Options and variable records.** Options are structured in later files but their numeric codes and fields are unmapped. Some payloads are thousands of bytes.
5. **Version semantics.** Type codes are stable across the later corpus, but payload meanings and sizes can change. A release name alone does not choose endian layout in 2007–2008.
6. **Conversion as an imperfect oracle.** Finale 27 reassigns/normalizes/synthesizes data. The new ETF pairs help, but later-version exports can also synthesize records, as shown by the Finale 2000 template saved by Finale 2005.
7. **Pre-banner provenance.** Filename paths identify apparent Finale 2 files, but the binary lacks the later product banner.
8. **Platform bias.** Nearly all examined files originate from Macintosh Finale installations or Macintosh archive workflows. This leaves Windows-specific byte order, header tuples, resource-fork absence, path/encoding behavior, and any platform-specific serialization insufficiently tested. A reader validated only against this corpus could mistake Mac behavior for format invariants.

## Significance of sharing

Sharing is the largest quality wildcard rather than a reason to abandon the effort. The corpus contains excellent natural samples: 301 converted files have multiple part definitions and both `shared=true` and `shared=false` are common. The exact mapping `0x011a → partDef` gives an anchor. A controlled three-file link/unlink experiment should reveal whether remaining key words encode part ID and shared state or whether records are duplicated with an override table.

A first reader could explicitly support only score-level records and report part-scoped data as unsupported, but that would be partial recovery, not a complete converter. Silent flattening should be avoided.

## Highest-value additional evidence

1. Binary/ETF correlation for `nestedTupletFin05RC2.mus`: codec boundary, entries, tuplets, and record frame.
2. Binary comparison of the original Finale 2000 template against both ETF exports: source-era records versus Finale 2005 upgrade records.
3. Binary/ETF correlation for the 1.8.7, 2.0.1, and 2.6 archive samples, including resource-fork-preserving extraction.
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

1. Correlate the supplied ETF exports with their original binary files, starting with the nested-tuplet, F2002, and two template samples. ETF alone is not expected to reveal the codec; exact MUS/ETF pairs and controlled edits are required.
2. With F2002–F2005 now confirming the six-byte typed/length outer framing, focus codec work on the opaque payloads and separate direct entry changes from configuration-dependent `BC`/spacing rewrites. Confirm the automatic-note-spacing explanation with a matched disabled-spacing pair.
3. Decode entry block `0x0016` using ETF `eE` records, minimal controlled note changes, and Finale 27 entry values.
4. Run the sharing C3 experiment and map part/shared key fields.
5. Confirm high-confidence record names with field-level distinctive values, not counts alone.
6. Only then build a disposable reader that populates a narrow subset of musxdom and reports every unsupported record.
