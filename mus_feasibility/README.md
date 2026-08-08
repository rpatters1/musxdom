# Legacy Finale MUS Feasibility Study

This is a public-source, independently verified exploratory study of legacy Finale `.mus` files. It assesses whether a future reader could populate the existing musxdom model; it is not a reader implementation.

The inventory found 1,218 legacy files and 2,042 Finale 27 exports. An exact adjacent `-exports/<name>.fin27.musx` counterpart exists for 1,189 legacy files; 29 do not have that exact counterpart. Finale 27 exports are semantic references, not byte-for-byte representations: conversion changes the modified header, normalizes data, and can synthesize or expand records.

## Public versus private corpus metadata

Published findings identify each source by a stable content-derived `corpus_id`, such as `mus-65aa1de01997b781`, plus its basename, size, and SHA-256. This permits future maintainers to recognize the same evidence without publishing the drive name or directory layout. The public manifest is [data/corpus_manifest.csv](data/corpus_manifest.csv).

The original path mapping is deliberately not tracked. If the corpus is available locally, create `mus_feasibility/private/corpus_locations.csv` with `scripts/publish_manifest.py --private-output ...`; the repository `.gitignore` excludes that directory. This exact filename is the canonical local key for resolving a public `corpus_id` back to the evidence file. Do not commit it. Existing raw inventory outputs containing paths are working data and should likewise remain local.

Archive members use the same public `member_id` convention. Their ignored local archive/member path mapping is `mus_feasibility/private/archive_locations.csv`, produced with `archive_probe.py --private-output ...`.

For StuffIt archives, install the `unar` package so that both `unar` and `lsar` are available. Use `lsar` for a non-destructive member listing and `unar -o <temporary-directory> <archive>` for extraction; never extract over the source corpus.

The initial ETF evidence set is now present locally under the ignored `private/evidence/` directory. It has since
been extended with tracked controlled MUS/ETF pairs for Finale 2002–2005 under `evidence/F2002/` through
`evidence/F2005/`. There are now fifteen ETF exports plus eight controlled-test MUS files in total. The public notes record
provenance and hashes; the archival source files and their private layout remain uncommitted.

Three targeted Finale 27 conversions of the ETF-backed 1.8.7, 2.0.1, and 2.6 sources are also retained privately.
Finale 27 opened each after `.mus` was appended to its extensionless filename; the 2.6 document had non-blocking font
issues. These conversions disprove the suspected 2.6.x parser cutoff for the tested files while demonstrating that
classic Mac type/creator discovery and modern filename-extension recognition are separate from format support.

## Public-source provenance policy

The investigation began as a strict clean-room study. On 2026-08-08, the project broadened that boundary to permit
consulting historically public copies of Finale PDK material for factual format information. PDK-derived findings must:

- cite an immutable public URL and access date;
- be labeled `public-PDK-derived` until independently checked;
- record only facts needed for interoperability, such as identifiers, sizes, field order, and flag meanings;
- be restated in the project's own terminology rather than copying declarations, comments, or implementation code;
- be checked against locally owned MUS, ETF, or MUSX evidence wherever practical; and
- become `independently binary-verified` only when the corpus confirms them.

No PDK source or header is stored in this repository. This is a public-source provenance boundary, not a claim of
strict clean-room isolation. Corpus-derived conclusions recorded before the boundary change retain their original
provenance.

## Current conclusion

**Feasible with substantial reverse engineering.** Finale 3.0–2012 is now tractable at the container and physical-
record layers. Finale 3.x–2000 uses four uncompressed typed pools; Finale 2001–2006 wraps the same fixed physical rows
in big-endian PKWARE Data Compression Library (DCL) blocks with CRC-32 checks; Finale 2007–2012 uses typed zlib
blocks and a later variable record frame. Mark Adler's open-source `blast` decompressor
successfully decoded all 1,603 candidate compressed members encountered in the 2001–2006 corpus, and every decoded
result matched its stored CRC-32. Of the files tested, 410 traverse cleanly as complete typed-block sequences; some
additional files yield valid leading DCL members before their outer framing stops matching the current probe. Public
Finale 2000 PDK facts plus independent corpus checks establish fixed 16-byte other/detail rows and 38-byte entry rows
from Finale 3.0 through Finale 2006; the former “16-word” hypothesis was a byte/word unit error. The exact Finale 2000
`tremolos.mus`/ETF pair additionally proves one-for-one pool counts, ordered ordinary tags, selected detail and entry
values, and byte-identical text after removing ETF section separators. Tag-specific fields, later entries, options,
and sharing remain incomplete. Finale 1.8.7–2.6 is now known to share the 16-byte logical-record cadence, exact
32-byte entry bodies, tag vocabulary, and raw text with Finale 3.0, but its index/directory spans and generic pool
boundaries remain unresolved. A universal reader is not yet justified, but a version/format-era strategy is.

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

python3 mus_feasibility/scripts/dcl_probe.py \
  mus_feasibility/private/corpus_locations.csv \
  mus_feasibility/data/corpus_manifest.csv \
  --blast '<path-to-blast-compatible-executable>' \
  --output mus_feasibility/private/dcl_probe.json

python3 mus_feasibility/scripts/uncompressed_probe.py \
  mus_feasibility/private/corpus_locations.csv \
  mus_feasibility/data/corpus_manifest.csv \
  --output mus_feasibility/private/uncompressed_probe.json

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

Public references used in the initial clean-room search include Mark Adler's permissively licensed `blast` decoder, the
Library of Congress description of legacy MUS and ETF,
Finale's historical help/glossary stating that ETF creation ended after Finale 2006, and the independent LilyPond
`etf2ly` ETF subset reader. `blast` supplies the independently verified 2001–2006 payload decoder; the other sources
document ETF's role and grammar but do not describe the MUS container:
[`blast` source](https://github.com/madler/zlib/tree/master/contrib/blast),
[`blast.h` format/API notes](https://github.com/madler/zlib/blob/master/contrib/blast/blast.h),
[Library of Congress MUS description](https://www.loc.gov/preservation/digital/formats/fdd/fdd000632.shtml),
[Finale ETF glossary](https://finale.jetzt/finalehelp/Finale26Win/Content/Finale/glossary.htm),
[`etf2ly` manual](https://manpages.ubuntu.com/manpages/stable/man1/etf2ly.1.html), and
[historical Finale format notes](https://preservation.tylerthorsted.com/2024/02/09/finale/).

The later public-PDK-informed phase uses the Finale 2000 PDK copy exposed in GRAME's GUIDOLib repository at immutable
commit `9f74ba9b3e287f240bbd454c2259fc3f7737c6ad`. Its
[`FinalePlugin-ReadMe.txt`](https://github.com/grame-cncm/guidolib/blob/9f74ba9b3e287f240bbd454c2259fc3f7737c6ad/platforms/win32/finale-plugin/FinalePlugin-ReadMe.txt)
identifies the included version. Exact consulted header links and the independently verified findings are recorded
in [FORMAT_NOTES.md](FORMAT_NOTES.md).
