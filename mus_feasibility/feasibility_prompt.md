# Assess the Feasibility of Reading Legacy Finale `.mus` Files

Conduct an exploratory reverse-engineering study of the legacy Finale `.mus` file format.

The primary goal is to determine how feasible it would be to read `.mus` files without Finale. This is an investigation, not an implementation project.

## Background

Legacy Finale `.mus` files use a proprietary binary format with a proprietary layout. However, the format is not necessarily opaque. There is reason to believe that substantial portions of it have a regular structure.

A working hypothesis is that many records occupy **16 words**, where each word is 16 bits. This is **not established as fact** and must be tested rather than assumed.

Based on existing knowledge, such a record may contain:

* Two bytes, usually ASCII, identifying the type of data structure.
* One `cmper` word for an "other" record, or two `cmper` words for a "detail" record.
* A payload of:

  * 12 words for an "other" record.
  * 10 words for a "detail" record.

If the 16-word hypothesis is correct, these known components appear to leave **two words unaccounted for**. Determining what these words contain—or whether the accounting itself is incorrect—is an important early question.

Possible explanations include record metadata, flags, sizes, links, offsets, checksums, additional identifiers, padding, or something else entirely. Do not assume any particular interpretation without evidence.

Known or suspected exceptions include:

* The entry pool is stored separately and may or may not follow this pattern.
* Options data may be completely free-form. Its organization is currently unknown.
* The format changed in small ways depending on which Finale version saved it.
* There may be variable-length structures, indexes, directories, strings, or other data outside the apparent fixed-record organization.

## Available Evidence

You have access to a directory containing `.mus` files created by multiple Finale versions. This directory is at the top of this prompt.

The survey corpus should be expanded beyond files whose names end in `.mus`. Recursively inspect archives found in the corpus, especially files under directories whose names contain `-Archive`, and safely extract or un-stuff supported containers for read-only examination. Likely archive types include ZIP, StuffIt, and other historically common Macintosh archive formats. Preserve the original archive as evidence and record the archive-relative member name for every extracted candidate.

On the current macOS survey system, The Unarchiver command-line tools `lsar` and `unar` are available. Use `lsar` for a non-destructive listing and `unar -o <temporary-directory> <archive>` for extraction; never extract over the source corpus. Preserve resource forks and extended attributes when the extractor provides them. Treat StuffIt extraction as part of the required survey, not as an optional sample.

Some legacy Finale files have no filename extension and depend on the classic Mac OS 9 type/creator codes or resource fork for identification. Treat extensionless files as possible Finale documents when their metadata, contents, or surrounding archive context supports that conclusion. Preserve and report resource-fork/type-creator evidence where available; do not rename the original evidence in place. The expanded survey should seek examples as far back as Finale 1.0. Finale 27 may not read files earlier than Finale 2.0, so pre-2.0 candidates may have no `.fin27.musx` counterpart and must be tracked as unconverted evidence rather than treated as missing files.

The completed archive pass currently covers 230 ZIP and 275 StuffIt archives, yielding 4,898 candidate members. It identified explicit pre-banner products through Finale 1.8.7, but no explicit Finale 1.0 file yet. Treat these counts and classifications as a reproducible baseline, not as a reason to stop searching for earlier material.

Nearly all current evidence is Macintosh-derived. Treat this platform bias as a major feasibility risk: do not assume that Mac header tuples, byte order, resource-fork handling, string encoding, padding, or record serialization are platform-independent. Plan a separate Windows corpus and, where possible, matched Mac/Windows saves of the same document across early, 2001–2006, 2007/2008, and 2012 eras before making cross-platform claims.

All available `.mus` files should have also been converted using Finale 27. The converted files are stored in an `-exports` directory and use names ending in:

```text
.fin27.musx
```

These `.fin27.musx` files should be the initial semantic reference for the investigation.

Files discovered only inside archives, extensionless legacy files, and files from formats earlier than Finale 2.0 will generally not have Finale 27 counterparts. Inventory them separately, link them to their containing archive and member path, and do not infer that the absence of an export means the file is uninteresting. If a candidate appears especially valuable, add a targeted conversion or opening attempt to `EVIDENCE_REQUESTS.md` rather than assuming that an export already exists.

They can help identify the musical contents and Finale data represented by each legacy `.mus` file, even though the Finale 27 conversion may normalize, reorganize, upgrade, discard, or synthesize some information rather than preserving the original binary representation exactly.

## Corpus Privacy and Public Identifiers

Keep the survey directory, its parent folders, drive name, and directory layout private. Do not copy absolute paths or full relative paths into committed reports, scripts, manifests, evidence requests, or generated public data.

Use a location-neutral public corpus manifest instead. Each source file should receive a stable content-derived identifier such as `mus-65aa1de01997b781`, together with its basename, size, SHA-256, matching export basename/hash, and analytical findings. Basenames may be public, but they are not sufficient identifiers because names can repeat.

Maintain the original path correspondence only in the local, ignored file:

```text
mus_feasibility/private/corpus_locations.csv
```

For archive members, use the corresponding ignored `mus_feasibility/private/archive_locations.csv` mapping, keyed by the public archive ID and member ID.

That mapping should associate each public corpus identifier with the original source and export paths. Public documents should explicitly name this file so a future maintainer knows how to resolve an identifier when the private corpus is available, but the file itself must never be committed. Raw path-bearing inventory and intermediate analysis files should likewise remain local or be sanitized before publication. Evidence requests should identify files by corpus ID and basename, not by absolute path.

## Enigma Portable Evidence

No Enigma Portable files have currently been exported.

Enigma Portable files can be produced on request for files that can be opened by Finale 2005 or many earlier Finale versions. Those files are especially valuable because Finale could recreate the corresponding binary `.mus` file from the textual Enigma Portable representation.

Do not assume that an Enigma Portable counterpart already exists.

Instead:

1. First determine, as far as possible, which Finale version created or last saved each `.mus` file.
2. Identify files likely to be readable by Finale 2005 or an earlier available Finale version.
3. Determine which specific Enigma Portable exports would provide the greatest investigative value.
4. Request those exports explicitly.
5. Explain what question each requested export is intended to answer.

Do not request Enigma Portable exports indiscriminately. Prefer a small number of carefully selected files that test important hypotheses.

Useful candidates may include:

* The earliest identifiable `.mus` version.
* A minimal or nearly empty document.
* A file containing easily recognized record types.
* A file with unusual searchable values.
* Two files differing by one controlled feature.
* Equivalent documents saved by adjacent Finale versions.
* A file near the Finale 2005 compatibility boundary.

Once supplied, use Enigma Portable files as a primary source for correlating binary records with known Finale data structures and values.

## Initial Priority: Identify the Source Finale Version

An important first task is to determine how a `.mus` file identifies its originating or most recent saving version.

Investigate whether version information appears in:

* A fixed file header.
* An ASCII or encoded version string.
* A numeric version field.
* A file-format revision number.
* Record identifiers present only in certain releases.
* A record directory or metadata block.
* Compression or wrapper signatures associated with particular versions.
* Other stable structural differences.

Distinguish where possible between:

* The Finale version that originally created the document.
* The Finale version that most recently saved it.
* The internal `.mus` format revision.
* A compatibility version or minimum-readable version.

The immediate practical purpose is to determine whether a given file may be opened by Finale 2005 or earlier and is therefore eligible for an Enigma Portable export.

Do not assume that the filename, filesystem timestamp, or surrounding directory reliably identifies the saving version. Treat those only as supporting metadata unless confirmed by the file itself.

If no explicit version field is found, attempt to develop a version-classification method based on headers, structural signatures, record sets, or comparison with known-version samples.

## Potential Additional Evidence: Proprietary MakeMusic Code

There may also be access to plugin-development directories containing proprietary MakeMusic code or headers that could illuminate aspects of the `.mus` format.

**Do not inspect or use this material by default.**

There are legal and provenance considerations around using proprietary source material in a reverse-engineering effort. Even though MakeMusic has discontinued Finale and the practical significance of this concern may now be lower, maintaining a clean legal or intellectual-property firewall may still be desirable.

Instead:

1. Conduct the investigation using the `.mus`, `.fin27.musx`, requested Enigma Portable, and other non-proprietary evidence first.
2. If proprietary MakeMusic material appears likely to answer an important unresolved question, document:

   * What question it might answer.
   * Why the available clean-room evidence is insufficient.
   * What type of information would be useful.
3. Flag this as an optional research path rather than accessing the material automatically.
4. Explicitly discuss the possible legal and provenance implications before recommending its use.

The existence of this material should be considered an available option, but one with caveats.

## Versioning Challenges

The `.mus` format changed with different Finale releases.

Later Finale versions could generally convert files created by earlier versions, but earlier versions could not read files created by later versions. This strongly suggests that the format evolved over time.

Assume that:

* Many changes were probably incremental.
* Record types may have been added, removed, extended, or reinterpreted.
* Field meanings may depend on the saving Finale version.
* File headers or internal version markers may exist.
* Some later files may use wrapping, compression, checksums, block directories, or other mechanisms absent from earlier files.

Do not assume that a parser for one Finale version would automatically work for another.

## Major Unknown: Sharing

Finale's sharing feature was introduced after Enigma Portable files were discontinued.

How sharing is represented in `.mus` files is unknown and is a major wildcard.

Determine, where possible:

* Whether shared data is duplicated or referenced.
* Whether records contain identifiers linking shared objects.
* Whether sharing introduces new record types or changes existing ones.
* Whether shared music, expressions, articulations, staff styles, or other objects are represented differently.
* Whether paired `.mus` and `.fin27.musx` files reveal corresponding sharing relationships.

Treat sharing as a major risk area in the feasibility assessment.

## Objectives

The study should:

1. Assess the feasibility of reading legacy Finale `.mus` files.
2. Determine as much of the binary organization as is reasonably possible.
3. Determine how a `.mus` file identifies its saving version or format revision.
4. Classify the corpus by probable Finale version or format era.
5. Use the existing `.fin27.musx` exports as the initial semantic reference.
6. Identify targeted cases where Enigma Portable exports would materially advance the investigation.
7. Test the hypothesized 16-word record structure rather than assuming it.
8. If the 16-word hypothesis holds, identify the purpose of the two currently unaccounted-for words.
9. Identify stable patterns across Finale versions.
10. Identify important version-specific differences.
11. Correlate binary records with:

    * `.fin27.musx` data.
    * Requested Enigma Portable records.
    * Known Finale data structures and concepts.
12. Investigate headers, record boundaries, identifiers, `cmper` values, payloads, entry data, options data, and possible indexes or directories.
13. Investigate how data sharing may be encoded.
14. Record failed hypotheses as well as successful ones.
15. Leave clear breadcrumbs for a future developer or LLM that may be more capable of completing the reverse engineering.
16. Produce a realistic recommendation about whether a future reader or converter is practical.

## Non-Goals

Do not begin by implementing a complete `.mus` reader.

The goal is not to produce a working converter during this study.

Small disposable analysis tools are encouraged, including tools that:

* Inventory and hash the corpus.
* Extract and compare candidate headers.
* Cluster files by header or structural similarity.
* Search for possible version fields.
* Produce structured hex dumps.
* Interpret data as 16-bit words in both byte orders.
* Search for possible record boundaries and repeating record sizes.
* Test whether 16-word alignment is actually present.
* Search for ASCII record identifiers.
* Compare two related binaries.
* Correlate binary values with `.fin27.musx` contents.
* Correlate binary offsets with requested Enigma Portable records.
* Compare files differing by only one controlled edit.
* Generate statistical summaries of values and record identifiers.
* Detect probable offsets, counts, indexes, pointers, flags, or padding.

These tools should support the investigation rather than evolve prematurely into production code.

Even if a complete reader appears feasible, do not implement it as part of this task. Instead, document the proposed architecture, remaining risks, and next steps.

Only depart from this rule if the format proves unexpectedly simple and a minimal proof of concept can be created without distracting from the assessment. Even then, treat it strictly as a disposable demonstration, not a production implementation. A viable solution will parse a mus file into the existing musxdom class structure. There will not be a separate DOM.

## Investigation Method

Begin by inventorying the available files and their corresponding exports.

For each `.mus` file, determine where possible:

* File size.
* Hash.
* Presence and path of the corresponding `.fin27.musx` file.
* Whether the source was found directly, inside an archive, or as an extensionless classic-Mac file.
* Containing archive name and archive-relative member path, when applicable.
* Classic Mac OS type/creator codes and resource-fork evidence, when available.
* Probable Finale saving version.
* Probable internal format revision or format era.
* Whether it may be readable by Finale 2005 or earlier.
* Whether an Enigma Portable export may therefore be possible.
* Whether the file appears to contain sharing.
* Whether it was created directly by its apparent version or converted from an earlier version.
* Any known musical or document features useful for comparison.

Prefer controlled comparisons over broad speculation.

### Phase 1: Corpus and Version Identification

Start by examining the `.mus` corpus itself.

1. Inventory all `.mus` files and `.fin27.musx` counterparts.
2. Hash the source and export files.
3. Compare the opening and closing regions of all `.mus` files.
4. Search for ASCII strings, version numbers, product names, dates, signatures, and repeated header layouts.
5. Interpret candidate header fields in both byte orders.
6. Cluster files by common prefixes, headers, file-size relationships, record identifiers, or other structural signatures.
7. Compare those clusters with any known file provenance.
8. Develop a preliminary version or format-era classifier.
9. Identify files most likely to be readable by Finale 2005 or earlier.
10. Prepare a prioritized list of Enigma Portable exports to request.

Do not delay all other work until version identification is perfect. A provisional classifier with confidence levels is acceptable.

### Phase 2: MUS / Finale 27 MUSX Comparisons

Use each `.fin27.musx` file as a semantic reference.

Recognize that Finale 27 may have:

* Upgraded legacy structures.
* Reassigned identifiers.
* Normalized defaults.
* Expanded shared data.
* Removed obsolete records.
* Added new metadata.
* Reorganized options.
* Recomputed layout information.

Therefore, use `.fin27.musx` to answer questions such as "what musical object does this value probably represent?" rather than assuming a direct byte-for-XML mapping.

Correlate distinctive values such as:

* Measure numbers.
* Entry numbers.
* Staff IDs.
* Expression IDs.
* Articulation IDs.
* Text block IDs.
* Font IDs.
* Page and system dimensions.
* MIDI values.
* Clefs, key signatures, and time signatures.
* Unusual numeric values likely to be searchable in the binary.

### Phase 3: Requested MUS / Enigma Portable Pairs

After identifying likely pre-2006 files, request selected Enigma Portable exports.

For each request, provide:

* The exact `.mus` file path.
* The suspected Finale version or compatibility range.
* The preferred Finale version to use for the export, if relevant.
* The reason the file was selected.
* The specific hypothesis or unknown the export may clarify.

Once an Enigma Portable file is supplied, correlate it with the binary `.mus` file.

Attempt to determine:

* Record ordering.
* Record identifiers.
* Binary representation of `cmper` values.
* Signedness and byte order.
* Payload correspondence.
* Whether records actually occupy 16 words.
* What precedes and follows the known record components.
* The purpose of the apparent two additional words, if they exist.
* String storage.
* Variable-length record handling.
* Entry pool representation.
* File-level metadata not represented as ordinary Enigma records.

Because Finale could reconstruct the `.mus` file from the Enigma Portable representation, these pairs should provide some of the strongest available evidence.

### Controlled-Difference Files

Where suitable source files or Finale versions are available, compare files that differ by exactly one edit.

Examples:

* Add one note.
* Change one pitch.
* Change one duration.
* Add one articulation.
* Add one expression.
* Modify one option.
* Add one staff.
* Add one measure.
* Create or remove a shared object.
* Change one item in a linked or extracted part.

Use binary diffs to identify which regions and records change.

If creating new controlled-difference files would materially help, describe the exact files and edits needed rather than assuming they are already available.

For this study, propose a limited list of such tests. We are not currently implementing this: we are determining feasibility.

### Cross-Version Comparisons

Compare equivalent documents saved by different Finale versions where available.

Determine:

* Whether the file has an identifiable version marker.
* Whether old record types remain stable.
* Whether records move or merely change content.
* Whether later versions append data or reorganize the entire file.
* Whether conversion rewrites all identifiers.
* Whether offsets and directory structures differ.
* Whether some versions introduce compression or containers.

A particularly useful goal is to determine whether there are identifiable **format eras** rather than treating every Finale release as an entirely independent format.

It may prove possible to describe the history as a small number of structural formats with incremental additions inside each era. Do not assume this is true, but investigate it.

## Analytical Discipline

Maintain a clear distinction between:

* Confirmed facts.
* Strongly supported hypotheses.
* Weak hypotheses.
* Open questions.
* Disproved hypotheses.

For every inferred field or record type, record:

* The evidence supporting the inference.
* Which Finale versions or format clusters were tested.
* Which files were tested.
* Known counterexamples.
* Confidence level.
* Suggested experiments that could confirm or disprove it.

Do not silently encode guesses into scripts as though they were established facts.

Preserve raw evidence where useful, including:

* File hashes.
* Relevant offsets.
* Hexadecimal and decimal values.
* Byte order.
* Record identifiers.
* Before-and-after binary excerpts.
* Corresponding Enigma Portable or `.fin27.musx` content.
* Commands used to generate results.

When an assumption turns out to be wrong, preserve that result. Knowing what the format **is not** may save substantial work for a future investigation.

## Communication and Evidence Requests

The investigation may require evidence that is not currently present.

When additional evidence is needed, create a clearly prioritized request rather than stopping the investigation or making an unsupported assumption.

A request should identify:

* The exact source file involved.
* The requested output or comparison file.
* Which Finale version should be used, if known.
* The exact operation to perform.
* The question the result is intended to answer.
* Whether the request is essential or merely useful.

Examples include:

* Export a specified `.mus` file as Enigma Portable from Finale 2005.
* Attempt to open a specified file in Finale 2005 and report whether it succeeds.
* Resave the same document from two specified Finale versions.
* Make one controlled edit and save a second copy.
* Create equivalent files with and without sharing.

Maintain these requests in a dedicated file such as:

```text
EVIDENCE_REQUESTS.md
```

Mark requests as:

* Proposed.
* Requested.
* Supplied.
* Analyzed.
* No longer needed.

## Expected Deliverables

All deliverables should be stored inside the mus_feasibility directory. Use subdirectories if appropriate.

### `README.md`

A concise overview of:

* The purpose of the investigation.
* The available evidence.
* The relationship between `.mus` files and the `-exports/*.fin27.musx` files.
* The fact that Enigma Portable files must be requested and are not currently present.
* How to reproduce the analysis.
* The current feasibility conclusion.
* Links to the other research documents and scripts.

### `CORPUS_INVENTORY.md`

Document:

* Every `.mus` file examined.
* Its hash and size.
* Its corresponding `.fin27.musx` file.
* Any known provenance.
* Suspected saving version.
* Suspected format era.
* Version-confidence level.
* Likelihood that an Enigma Portable export is possible.
* Notable musical or document features.

### `FORMAT_NOTES.md`

Document all discovered or suspected aspects of the format, including:

* File header.
* Version identification.
* Byte order.
* Record framing.
* The proposed 16-word structure and the evidence for or against it.
* The two currently unexplained words, if the 16-word structure is confirmed.
* "Other" records.
* "Detail" records.
* Entry pool.
* Options.
* Strings and variable-length data.
* Indexes, offsets, or directories.
* Sharing.
* Checksums, compression, or wrapping.
* Differences by Finale version.

Use tables where they improve clarity.

### `RECORD_CATALOG.md`

For each observed record identifier, document:

* Identifier bytes.
* Probable Finale or Enigma structure name.
* Record category.
* Known field meanings.
* Unknown fields.
* Finale versions or format clusters in which it appears.
* Example file offsets.
* Confidence level.

### `VERSION_MATRIX.md`

Provide a matrix showing:

* Finale versions or probable versions represented in the corpus.
* Known format characteristics.
* Header differences.
* Record differences.
* Compatibility observations.
* Availability of `.fin27.musx` counterparts.
* Availability or feasibility of Enigma Portable exports.
* Known conversion history.

Where the evidence supports it, identify broader format eras or compatibility families.

### `EVIDENCE_REQUESTS.md`

Maintain a prioritized list of additional evidence needed, especially:

* Enigma Portable exports.
* Controlled-difference files.
* Cross-version resaves.
* Sharing test cases.

Do not assume these files will appear automatically. Make each request precise and justified.

### `EXPERIMENT_LOG.md`

Maintain a chronological log containing:

* Question or hypothesis.
* Files examined.
* Method used.
* Commands or scripts run.
* Observations.
* Conclusion.
* Follow-up experiment.

Include failed investigations. They are valuable breadcrumbs.

### `FEASIBILITY_ASSESSMENT.md`

Give a direct assessment of:

* Whether reading any `.mus` version appears feasible.
* Whether reading a limited range of versions appears feasible.
* Whether a version-specific parser would be required.
* Whether a small number of format-era parsers might suffice.
* Whether conversion quality could be complete or only partial.
* The largest technical risks.
* The significance of the sharing problem.
* The amount and type of additional evidence needed.
* Which Enigma Portable exports would have the highest value.
* Whether consulting proprietary MakeMusic material would materially improve the prospects, and the associated provenance and legal caveat.
* A proposed architecture for a future implementation.
* Recommended next steps.

Use a conclusion such as:

* Clearly feasible.
* Feasible for limited versions.
* Feasible with substantial reverse engineering.
* Feasible only for partial recovery.
* Currently infeasible with available evidence.

Justify the conclusion with concrete findings.

### Analysis Scripts

Place disposable analysis tools in an appropriately named directory.

Scripts should:

* Avoid modifying source files.
* Accept paths through command-line arguments.
* Produce deterministic output where practical.
* Clearly state assumptions.
* Preserve offsets and raw values.
* Be small and composable.
* Avoid pretending that speculative interpretations are confirmed.

## First Steps

Begin with the existing `.mus` corpus and the `.fin27.musx` files in the `-exports` directory.

1. Inventory and hash all `.mus` files, extensionless candidates, and archive files.
2. Inspect archives (especially `-Archive` directories) without modifying originals; extract candidates to a separate working area and record archive/member provenance.
3. Capture classic Mac OS type/creator metadata and resource-fork evidence for extensionless candidates.
4. Match directly available `.mus` files with their `.fin27.musx` counterparts; record archive-only and pre-2.0 candidates as unconverted rather than dropping them.
5. Inspect and compare candidate file headers.
6. Search for explicit or implicit version indicators, including Finale 1.0-era evidence.
7. Cluster the files into likely versions or format eras.
8. Determine which files are probably readable by Finale 2005 or earlier.
9. Use the `.fin27.musx` files to understand the semantic contents of promising `.mus` samples.
10. Search for recognizable values and possible record identifiers.
11. Determine whether there is evidence for fixed-size records.
12. Specifically test the 16-word hypothesis at multiple offsets and across multiple files.
13. If the 16-word structure is supported, identify the location and possible purpose of the two currently unaccounted-for words.
14. Prepare a small, prioritized set of Enigma Portable or other conversion/opening requests for valuable files lacking exports.
15. Continue investigating the entry pool, options, indexes, strings, and sharing while those requests remain outstanding.
16. Incorporate requested conversion files when they become available.
17. Record findings before expanding the scope.

Do not assume that offset zero is the beginning of the record stream. Search systematically for possible headers, block boundaries, embedded streams, indexes, and directories.

If the evidence contradicts the 16-word hypothesis, abandon or revise it rather than attempting to force the data into the expected model.

## Final Standard

The value of this work is not measured by whether it produces a parser.

A successful result may instead be:

* A reliable method for identifying or classifying `.mus` format versions.
* A well-supported partial description of the format.
* A catalog of stable record structures.
* An explanation of whether the apparent 16-word structure is real.
* Identification of the currently unexplained words.
* A map of version boundaries or format eras.
* A prioritized list of Enigma Portable exports and other evidence needed.
* A collection of reproducible experiments.
* A clear account of the unsolved portions.
* A realistic feasibility judgment.
* A body of evidence that allows a future developer or more capable LLM to resume the work without starting over.

Prioritize durable research notes, evidence, reproducibility, and precise evidence requests over speculative implementation.
