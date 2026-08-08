# Evidence Requests

The first ETF evidence set is now present locally under `mus_feasibility/private/evidence/` (ignored by git).
Requests remain deliberately small and hypothesis-driven. When multiple Finale versions are available, use the
**earliest compatible version** for the export and record the exact application version and maintenance/build number.
Source locations are intentionally omitted from this public document. Resolve each `corpus_id` through the local-only
`mus_feasibility/private/corpus_locations.csv` mapping (ignored by git); the public manifest supplies the filename and hash.

## Priority requests

### P1 — Analyzed — essential

- **Source:** `mus-d89e8fe12e271440` (`nestedTupletFin05RC2.mus`)
- **Operation:** Open with the earliest available Finale version that accepts the file, without resaving first; export as Enigma Transportable/Portable (`.etf`). Record the exact application version/build and whether opening/export succeeds.
- **Preferred output:** `nestedTupletFin05RC2.etf`
- **Why:** This is a 3,351-byte, explicitly Finale 2005 file at the ETF compatibility boundary. Its Finale 27 export contains recognizable nested tuplet/entry data. DCL and physical rows are now solved, and the public Finale 2000 PDK identifies `TP` as the nested-tuplet entry-detail tag. This pair is retained to map the version-expanded `TP` fields and their relationship to the 38-byte entry rows.

### P2 — Analyzed — essential

- **Source:** `mus-3597fd4fce0c272b` (`template.mus`)
- **Operation:** Attempt to open with the earliest available compatible Finale version and export ETF without deliberately changing the document. Record success and the exact application version/build.
- **Preferred output:** `template-Fin2000.etf`
- **Why:** The header says Finale 2000 and the document is small (19,557 bytes) and template-like. It tests defaults and upgrade synthesis with little musical noise. The later exact `tremolos` pair and corpus-wide probe have now answered the physical question: Finale 3.x–2000 uses uncompressed 16-byte other/detail and 38-byte entry rows.

### P3 — Proposed — important

- **Source:** `mus-a23053bf74c5e11a` (`Freire2.mus`)
- **Operation:** First try the earliest available compatible Finale version. Export ETF without resaving if it opens; report the exact version/build and, if it fails, the earliest version attempted.
- **Preferred output:** `Freire2-Fin3.etf`
- **Why:** This is the smallest file whose explicit banner says Finale 3.0 (38,565 bytes). The corpus probe now proves it is a Windows-origin, little-endian instance of the four uncompressed pools. Its ETF would test whether field values and logical identities are exactly byte-swapped equivalents of the Mac Finale 2000 pair.

### P4 — Supplied/Analyzed — essential

- **Source:** `mus-3a8b724cf3adba80` (`tremolos.mus`)
- **Operation:** Open directly in Finale 2000 without resaving and export Enigma Transportable/Portable.
- **Supplied output:** `tremolos-from-Fin2000.etf`, stored locally under ignored `private/evidence/`
- **Why/result:** The musically small file contains entries, note alterations, tuplets, beams, and stem details. Its exact pair proves four uncompressed pools, one-for-one 16-byte other/detail rows, eight 38-byte entries, literal tag ordering, and byte-identical raw text after ETF separator removal. It triggered corpus-wide confirmation across 189 Finale 3.x–2000 files.

### P5 — Proposed — useful

- **Source:** `mus-c7e0faa94df7fc67` (`v1v2beamFin04.mus`)
- **Operation:** Open and export ETF in Finale 2004 or Finale 2005; record application version.
- **Preferred output:** `v1v2beamFin04.etf`
- **Why:** At 3,294 bytes, this is an unusually small Finale 2004 file with recognizable beam/detail structures. DCL/framing is already confirmed for Finale 2004; the export would provide strong decoded detail-record candidates and field-level semantic correlations.

## Controlled-difference requests

### A1 — Supplied/Analyzed — essential for earliest banner-era files

The archive survey found explicit pre-banner files labeled `Finale(TM) 1.8.7`, `2.0.1`, and `2.6`. The first three
selected samples were exported and analyzed locally: `guitar pc` (`mus-7aa45639c14b3864`, 1.8.7), `Dream of
Summer I` (`mus-2c0a5e8897b436d5`, 2.0.1), and `Score` (`mus-bd0042f8e0354192`, 2.6). The `Score` source had to
come from the parallel StuffIt archive because the ZIP copy did not preserve its classic Mac resource fork. These
are the earliest explicit-version binary samples currently identified. Finale 27 counterparts have now been supplied
for all three after copies were given `.mus` suffixes; the extensionless originals were not recognized directly.

### A2 — Analyzed — important archive-format coverage

The installed `unar`/`lsar` 1.10.7 tools were used to inspect all 275 `.sit` archives, preserving archive/member hashes and resource-fork observations. The results are incorporated in `ARCHIVE_SURVEY.md` and the public archive data. No further blanket extraction request remains; future archive work should target named members only.

### A3 — Proposed — useful cross-era binary controls

Use archive candidates `mus-ee1382238443129b` (`1ALightningStrike asv.mus`, Finale 2007, validated big-endian framing) and `mus-43c11614815f485c` (`1Heart asv.mus`, Finale 2008, validated little-endian framing) as controlled comparison targets if their documents can be opened. Export ETF where supported or save equivalent copies from the indicated Finale versions. The purpose is to test whether the observed endian transition changes only serialization or also record identities and payload meanings.

### W1 — Proposed — essential platform coverage

Supply additional Windows-origin files, preferably with exact Finale version/build provenance, covering at least one minimal document and one musically rich document from Finale 2001–2006, 2007, 2008, and 2012. For the 2007/2008 pair, save the same document on Mac and Windows where possible. Preserve source hashes, header bytes, platform tuples, and resource-fork absence. Four existing Windows files now prove little-endian serialization of the same Finale 3.x–2000 pools, but later Windows coverage remains insufficient.

### C1 — Proposed — useful for decoded record mapping

Using the earliest available Finale version that supports ETF, create a minimal one-staff/one-measure document and save/export four pairs (`.mus` and `.etf`): empty measure, add one quarter-note middle C, change only that note to C-sharp, and attach one articulation. Do not change layout between saves. DCL decoding is solved; this isolates entry, pitch/alteration, and articulation changes in the decoded pools.

### C2 — Proposed — important for transition

If both an early Finale 2007 and Finale 2008 installation are available, open the same minimal document and save one `.mus` from each without other edits. Record exact maintenance/build numbers. This tests the observed big-/little-endian record serialization transition and whether type codes/payloads stayed stable.

### C3 — Proposed — essential for sharing

In Finale 2012, create a score plus one linked part with one expression and one articulation. Save A with all items shared; save B after unlinking only the expression; save C after changing only the part's expression. Supply all `.mus` files and Finale 27 `.musx` conversions. This isolates the binary fields behind `part`, `shared=true/false`, duplication, and override behavior.

### C4 — Proposed — essential for the Finale 2.6.3 boundary

In Finale 2.6.3, create a new minimal document using that version's ordinary default-document command. Keep one staff
and one measure and enter three quarter notes C4, D4, and E4. Save the document once as `minimal-Fin263.mus`, then
immediately export `minimal-Fin263.etf` without another edit or resave. Record the exact Finale build and platform.
Preserve the resource fork when moving the MUS file. This pair should expose the early ordinary/detail/entry indexes
with very little noise; general Finale 27 compatibility through 2.6.3 is already confirmed.

### C5 — Proposed — essential if Finale 1.0 supports ETF

Repeat C4 in Finale 1.0 as `minimal-Fin100.mus` and `minimal-Fin100.etf`. If Finale 1.0 has no ETF export command,
preserve the MUS file and record that fact rather than exporting it through a later Finale version. Try a `.mus`-
suffixed copy in Finale 27 and record the result. This is the highest-value test of whether the 1.8.7–2.6 fixed-row/
indexed model and verified Finale 27 compatibility extend to the first release.

## Status legend

- **Proposed:** documented but not yet requested/supplied.
- **Requested:** user has initiated the evidence creation.
- **Supplied:** file is present but not analyzed.
- **Analyzed:** incorporated into the notes and catalog.
- **No longer needed:** superseded by stronger evidence.
