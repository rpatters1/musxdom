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
- **Why:** This is a 3,351-byte, explicitly Finale 2005 file at the ETF compatibility boundary. Its Finale 27 export contains recognizable nested tuplet/entry data. The pair should reveal the 2001–2006 high-entropy codec's decompressed record order, numeric identifiers, keys, payload lengths, entry representation, and trailer behavior.

### P2 — Analyzed — essential

- **Source:** `mus-3597fd4fce0c272b` (`template.mus`)
- **Operation:** Attempt to open with the earliest available compatible Finale version and export ETF without deliberately changing the document. Record success and the exact application version/build.
- **Preferred output:** `template-Fin2000.etf`
- **Why:** The header says Finale 2000 and the document is small (19,557 bytes) and template-like. It tests the low-entropy pre-2001 family with many default/options records and little musical noise. Comparison with P1 directly tests the apparent 2000/2001 codec boundary and the fixed-16-word hypothesis in an earlier era.

### P3 — Proposed — important

- **Source:** `mus-a23053bf74c5e11a` (`Freire2.mus`)
- **Operation:** First try the earliest available compatible Finale version. Export ETF without resaving if it opens; report the exact version/build and, if it fails, the earliest version attempted.
- **Preferred output:** `Freire2-Fin3.etf`
- **Why:** This is the smallest file whose explicit banner says Finale 3.0 (38,565 bytes). It tests the earliest banner-era layout, endian interpretation, and whether later ETF export preserves old logical record identities.

### P4 — Proposed — useful

- **Source:** `mus-c7e0faa94df7fc67` (`v1v2beamFin04.mus`)
- **Operation:** Open and export ETF in Finale 2004 or Finale 2005; record application version.
- **Preferred output:** `v1v2beamFin04.etf`
- **Why:** At 3,294 bytes, this is an unusually small Finale 2004 file with recognizable beam/detail structures. It tests whether the P1 codec/framing interpretation is stable one release earlier and provides strong detail-record candidates.

## Controlled-difference requests

### A1 — Supplied/Analyzed — essential for earliest banner-era files

The archive survey found explicit pre-banner files labeled `Finale(TM) 1.8.7`, `2.0.1`, and `2.6`. The first three
selected samples were exported and analyzed locally: `guitar pc` (`mus-7aa45639c14b3864`, 1.8.7), `Dream of
Summer I` (`mus-2c0a5e8897b436d5`, 2.0.1), and `Score` (`mus-bd0042f8e0354192`, 2.6). The `Score` source had to
come from the parallel StuffIt archive because the ZIP copy did not preserve its classic Mac resource fork. These
are the earliest explicit-version binary samples currently identified and have no Finale 27 counterparts.

### A2 — Proposed — important archive-format coverage

Use the installed `unar`/`lsar` 1.10.7 command-line tools to inspect all 275 `.sit` archives, prioritizing members in `-Archive` directories. Preserve archive and member hashes, then add only Finale-like members to the public manifest. Verify resource-fork handling and do not treat an archive as empty merely because it lacks a filename extension. This is an extraction task rather than a request to resave every discovered document.

### A3 — Proposed — useful cross-era binary controls

Use archive candidates `mus-ee1382238443129b` (`1ALightningStrike asv.mus`, Finale 2007, validated big-endian framing) and `mus-43c11614815f485c` (`1Heart asv.mus`, Finale 2008, validated little-endian framing) as controlled comparison targets if their documents can be opened. Export ETF where supported or save equivalent copies from the indicated Finale versions. The purpose is to test whether the observed endian transition changes only serialization or also record identities and payload meanings.

### W1 — Proposed — essential platform coverage

Supply a Windows-origin corpus, preferably with exact Finale version/build provenance, covering at least one minimal document and one musically rich document from Finale 3.x/2000, 2001–2006, 2007, 2008, and 2012. For the 2007/2008 pair, save the same document on Mac and Windows where possible. Preserve source hashes, header bytes, platform tuples, and any resource-fork absence. This request is essential because the current corpus is overwhelmingly Macintosh-derived and cannot establish Windows compatibility or platform invariants.

### C1 — Proposed — useful after P1/P2

Using the earliest available Finale version that supports ETF, create a minimal one-staff/one-measure document and save/export four pairs (`.mus` and `.etf`): empty measure, add one quarter-note middle C, change only that note to C-sharp, and attach one articulation. Do not change layout between saves. This isolates entry, pitch/alteration, and articulation record changes in the high-entropy/ETF era.

### C2 — Proposed — important for transition

If both an early Finale 2007 and Finale 2008 installation are available, open the same minimal document and save one `.mus` from each without other edits. Record exact maintenance/build numbers. This tests the observed big-/little-endian record serialization transition and whether type codes/payloads stayed stable.

### C3 — Proposed — essential for sharing

In Finale 2012, create a score plus one linked part with one expression and one articulation. Save A with all items shared; save B after unlinking only the expression; save C after changing only the part's expression. Supply all `.mus` files and Finale 27 `.musx` conversions. This isolates the binary fields behind `part`, `shared=true/false`, duplication, and override behavior.

## Status legend

- **Proposed:** documented but not yet requested/supplied.
- **Requested:** user has initiated the evidence creation.
- **Supplied:** file is present but not analyzed.
- **Analyzed:** incorporated into the notes and catalog.
- **No longer needed:** superseded by stronger evidence.
