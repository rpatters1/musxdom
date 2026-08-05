# Legacy Finale MUS Feasibility Study

This is a clean-room exploratory study of legacy Finale `.mus` files. It assesses whether a future reader could populate the existing musxdom model; it is not a reader implementation.

The inventory found 1,218 legacy files and 2,042 Finale 27 exports. An exact adjacent `-exports/<name>.fin27.musx` counterpart exists for 1,189 legacy files; 29 do not have that exact counterpart. Finale 27 exports are semantic references, not byte-for-byte representations: conversion changes the modified header, normalizes data, and can synthesize or expand records.

## Public versus private corpus metadata

Published findings identify each source by a stable content-derived `corpus_id`, such as `mus-65aa1de01997b781`, plus its basename, size, and SHA-256. This permits future maintainers to recognize the same evidence without publishing the drive name or directory layout. The public manifest is [data/corpus_manifest.csv](data/corpus_manifest.csv).

The original path mapping is deliberately not tracked. If the corpus is available locally, create `mus_feasibility/private/corpus_locations.csv` with `scripts/publish_manifest.py --private-output ...`; the repository `.gitignore` excludes that directory. This exact filename is the canonical local key for resolving a public `corpus_id` back to the evidence file. Do not commit it. Existing raw inventory outputs containing paths are working data and should likewise remain local.

Archive members use the same public `member_id` convention. Their ignored local archive/member path mapping is `mus_feasibility/private/archive_locations.csv`, produced with `archive_probe.py --private-output ...`.

For StuffIt archives, install the `unar` package so that both `unar` and `lsar` are available. Use `lsar` for a non-destructive member listing and `unar -o <temporary-directory> <archive>` for extraction; never extract over the source corpus.

The initial ETF evidence set is now present locally under the ignored `private/evidence/` directory. It has since
been extended with tracked controlled MUS/ETF pairs for Finale 2002–2005 under `evidence/F2002/` through
`evidence/F2005/`. There are now fourteen ETF exports plus eight controlled-test MUS files in total. The public notes record
provenance and hashes; the archival source files and their private layout remain uncommitted. No proprietary MakeMusic
plugin-development code or headers were inspected.

## Current conclusion

**Feasible with substantial reverse engineering.** Finale 2007–2012 is already tractable at the wrapper and generic-record layers: typed zlib blocks, lengths, CRC-32 checks, two record pools, variable payloads, and many numeric record-to-EnigmaXml mappings are reproducible. Entry, text, option-field, and sharing semantics remain incomplete. Finale 2001–2006 uses an unidentified high-entropy encoding, Finale 3.x–2000 uses a different low-entropy organization/encoding, and apparent Finale 2 files use a distinct pre-banner layout. A universal reader is not yet justified, but a version/format-era strategy is.

## Reproduce

From the musxdom repository root:

```bash
python3 mus_feasibility/scripts/inventory.py \
  '<local-corpus-root>' \
  --output-dir mus_feasibility/private

python3 mus_feasibility/scripts/archive_probe.py \
  '<local-corpus-root>' \
  --output mus_feasibility/data/archive_members.csv \
  --summary mus_feasibility/ARCHIVE_SURVEY.md \
  --private-output mus_feasibility/private/archive_locations.csv

python3 mus_feasibility/scripts/publish_manifest.py \
  mus_feasibility/private/corpus_inventory.csv \
  --public-output mus_feasibility/data/corpus_manifest.csv \
  --private-output mus_feasibility/private/corpus_locations.csv

python3 mus_feasibility/scripts/structure_probe.py \
  mus_feasibility/private/corpus_inventory.csv \
  --output-dir mus_feasibility/private

python3 mus_feasibility/scripts/musx_semantics.py \
  mus_feasibility/private/corpus_inventory.csv \
  --output mus_feasibility/private/musx_semantics.csv

python3 mus_feasibility/scripts/correlate_records.py \
  mus_feasibility/private/corpus_inventory.csv \
  mus_feasibility/private/musx_semantics.csv \
  --output mus_feasibility/data/record_correlations.csv

python3 mus_feasibility/scripts/render_record_catalog.py \
  mus_feasibility/private/record_catalog.csv \
  mus_feasibility/data/record_correlations.csv \
  --inventory mus_feasibility/private/corpus_inventory.csv \
  --output mus_feasibility/RECORD_CATALOG.md

python3 mus_feasibility/scripts/render_corpus_inventory.py \
  mus_feasibility/private/corpus_inventory.csv \
  mus_feasibility/private/structure_probe.csv \
  mus_feasibility/private/musx_semantics.csv \
  --output mus_feasibility/CORPUS_INVENTORY.md
```

The scripts are read-only with respect to the evidence corpus. `musx_semantics.py` uses the public symmetric `score.dat` recoding algorithm documented in the MIT-licensed sibling denigma project and keeps decoded XML in memory.

## Research documents

- [CORPUS_INVENTORY.md](CORPUS_INVENTORY.md): all examined files, sizes, hashes, header products, and counterpart matches.
- [ARCHIVE_SURVEY.md](ARCHIVE_SURVEY.md): archive and extensionless-member findings, including the Finale 2.6 samples.
- [FORMAT_NOTES.md](FORMAT_NOTES.md): headers, format eras, blocks, record framing, entries, text, options, and sharing.
- [RECORD_CATALOG.md](RECORD_CATALOG.md): every numeric record identifier observed in successfully framed 2007+ blocks.
- [VERSION_MATRIX.md](VERSION_MATRIX.md): corpus versions and proposed format eras.
- [EVIDENCE_REQUESTS.md](EVIDENCE_REQUESTS.md): precise ETF and controlled-difference requests.
- [EXPERIMENT_LOG.md](EXPERIMENT_LOG.md): commands, observations, failed hypotheses, and follow-ups.
- [FEASIBILITY_ASSESSMENT.md](FEASIBILITY_ASSESSMENT.md): direct recommendation, risks, architecture, and next steps.

Public references used in the clean-room search include the Library of Congress description of legacy MUS and ETF,
Finale's historical help/glossary stating that ETF creation ended after Finale 2006, and the independent LilyPond
`etf2ly` ETF subset reader. These document ETF's role and grammar but do not provide the 2001–2006 MUS codec:
[Library of Congress MUS description](https://www.loc.gov/preservation/digital/formats/fdd/fdd000632.shtml),
[Finale ETF glossary](https://finale.jetzt/finalehelp/Finale26Win/Content/Finale/glossary.htm),
[`etf2ly` manual](https://manpages.ubuntu.com/manpages/stable/man1/etf2ly.1.html), and
[historical Finale format notes](https://preservation.tylerthorsted.com/2024/02/09/finale/).
