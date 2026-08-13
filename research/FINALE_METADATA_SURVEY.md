# Finale metadata survey

Survey date: 2026-08-13

## Scope and conventions

This survey covers the installed Finale 27 application bundles (English and Spanish), the system-wide and per-user `Application Support/MakeMusic/Finale 27` trees, and installed Finale 25, 26, 2014.x, 2012, and 2011 material where it adds something not already represented by Finale 27. It searches conventional text extensions (`.txt`, `.xml`, `.json`, `.plist`, `.strings`, `.ini`, `.cfg`, `.conf`, `.csv`, `.tsv`, `.html`, `.htm`, `.js`, `.lua`, `.config`, `.properties`, and `.md`) and also files identified as text despite other or absent extensions, notably `.soundmap`, `.xsd`, `.dtd`, `.mod`, `.ent`, `.xsl`, `.xib`, `.cache`, `CodeResources`, `PkgInfo`, and installer manifests.

Paths below are deliberately generic:

- `Finale.app/...` means an application-bundle-relative path.
- `System Application Support/Finale 27/...` means the machine-wide MakeMusic Finale 27 support directory.
- `User Application Support/Finale 27/...` means the current user's corresponding directory.

“Preserve” means the file contains stable identifiers, mappings, semantics, or defaults that would be difficult to reconstruct. It does not imply that the file may be redistributed without checking its license.

## Executive findings

The most valuable metadata is not in `.musx` documents. It is in five interlocking registries:

1. `FinaleInstUUIDs.xml` assigns Finale's stable universal UUIDs to named instruments and broad generic families.
2. `PercNoteTypes.txt`, MIDI Device Annotation XML, `PercMapConversionTable.txt`, and `.soundmap` files jointly define percussion identities, MIDI-note mappings, legacy-map migrations, and playback-library assignments.
3. `glyphnamesFinale.json` plus the 31 `Data/SMuFL Conversion/*.json` files map Finale's private/legacy music-font characters to SMuFL glyphs, including positioning offsets.
4. Human Playback XML files define recognition vocabulary, techniques, filters, actions, controller behavior, and factory/user preference sets.
5. `SMInstrumentsMap.json` and MusicXML `sounds.xml` connect Finale instrument UUIDs to Standard Music Font Layout/MusicXML instrument-sound identifiers.

All five clusters should be preserved outside the installed application/support folders. Some have already been captured in the named repositories, but this survey intentionally lists them anyway.

The installation also contains substantial non-text assets that are usable outside Finale: modern and legacy music fonts, a General MIDI SoundFont, Finale libraries and document styles, a large template/example corpus, MIDI drum grooves, font-annotation metrics, MusicXML schemas/transforms, and vector artwork. These are cataloged separately below.

## Catalog

### Instrument identity and semantic classification

| Generic location | File(s) | Information present | Assessment |
|---|---|---|---|
| `Finale.app/Contents/Resources/` | `FinaleInstUUIDs.xml` | Universal instrument name, broad generic family, and Finale UUID. Covers conventional, educational, world, percussion, voice, keyboard, fretted/TAB, and other score-manager instruments. | **Preserve.** This is the canonical bridge from document UUIDs to human-readable instrument identities. `denigma` already contains a derived instrument-sound table, but the source registry itself was not found in the surveyed repos. |
| same | `SMInstrumentsMap.json` | Mapping from Finale instrument UUIDs to standardized instrument-sound identifiers used for semantic interchange/playback. | **Preserve.** Particularly useful together with `FinaleInstUUIDs.xml`; appears only in Finale 27 among the installed versions. |
| same | `dictionary.instruments.xml` | Human Playback/Score Merger multilingual instrument-name recognition dictionary: abbreviations and names in English, French, German, Italian, Spanish, Dutch, and related spellings mapped to compact instrument classes. | **Preserve.** This is a useful parsing vocabulary independent of UI localization. |
| `System Application Support/Finale 27/Configuration Files/` | `ensembles.txt` | Setup Wizard ensemble definitions: ensemble names, ordered instrument names, copy counts, range/display flags, and the corresponding universal instrument UUID lists. | **Preserve.** It gives curated orchestration templates and demonstrates UUID use. The user copy may contain additions; keep system and user variants when they differ. |
| `System Application Support/Finale 27/Data/` | `smpsoloinst.txt` | Numeric SmartMusic/assessment solo-instrument IDs, display names, and assessment/Intelligent Accompaniment eligibility flags. | Preserve if SmartMusic-era interoperability matters; it is a small otherwise undocumented ID table. |
| older support trees | `instrument.txt`, `GIFFinstruments.txt`, `Tapspaceinstruments.txt`, `chordmap.txt` | Retired setup/playback-era instrument, Garritan, Tapspace, and chord mapping lists. These occur only in a few older installations and are not Finale 27's primary registries. | **Preserve one distinct copy of each.** They may explain legacy document or playback identifiers absent from current tables. |

### Percussion metadata

| Generic location | File(s) | Information present | Assessment |
|---|---|---|---|
| `Finale.app/Contents/Resources/` | `PercNoteTypes.txt` | Master percussion note-type and group definitions: note-type ID, parent ID, translatable name, default staff position, GM MIDI note, group ID, copy count, and copy-group behavior. Includes 128 custom slots. | **Preserve.** Already captured as `finale/PercNoteTypes.txt` and represented in `musxdom` by `PercussionNoteType`, but it remains a primary source asset. |
| `Finale.app/Contents/PlugIns/2012FileConverter.bundle/Contents/Resources/` | `2012FileConverterPercNoteTypes.txt`, `2012FileConverterPercMapConversionTable.txt` | Converter-specific snapshot of percussion identities and pre-2010 percussion-map migration rules. | **Preserve.** These freeze the converter's historical interpretation and may differ from the live tables. |
| `System Application Support/Finale 27/Configuration Files/` | `PercMapConversionTable.txt` | Pre-2010 percussion map name to MIDI Device Annotation filename and map-name conversion. Includes Garritan/CaMB, VDL/Tapspace, Latin percussion, marching percussion, and other mappings. | **Preserve.** Essential for interpreting/migrating old percussion setups. |
| `System` and `User Application Support/Finale 27/MIDI Device Annotation/` | `General MIDI.xml`, `Garritan Instruments for Finale.xml`, `SmartMusic SoftSynth.xml`, `Tapspace Drumline for Finale.xml`; user tree also has `NotePerformer.xml` | Manufacturer/model, named note lists, MIDI note number to Finale `PercNoteType` ID mappings, and device/channel/program names where supplied. | **Preserve all distinct files.** Together they translate library-specific keymaps into Finale's stable percussion identities. User copies largely duplicate system copies, but NotePerformer is user-only here. |
| `Finale.app/Contents/PlugIns/2012FileConverter.../Resources/` | `2012FileConverterGeneralMIDI.soundmap`, `2012FileConverterSoftSynth.soundmap` | Historical converter playback maps connecting old content to GM/SoftSynth sounds and UUIDs. | Preserve as historical migration evidence. |

### Playback libraries and sound maps

| Generic location | File(s) | Information present | Assessment |
|---|---|---|---|
| `System Application Support/Finale 27/Data/` | `GIFF.soundmap` | Garritan Instruments for Finale library description and per-sound IDs/UUIDs, patch/sample paths, MIDI commands, techniques, keyswitches/controllers, and instrument assignments. | **Preserve.** One of the richest playback metadata sources. |
| same | `SoftSynth.soundmap` | SmartMusic SoftSynth/General MIDI sound registry: Finale instrument UUID, bank/program/control-change setup, and instrument associations. | **Preserve.** Useful authoritative UUID-to-GM mapping. |
| older application/support trees | `General MIDI.soundmap`, `NotePerformer.soundmap`, older `GIFF.soundmap` and `SoftSynth.soundmap` revisions | Older or third-party mappings. `NotePerformer.soundmap` is not present in the Finale 27 system Data folder but exists in earlier support material; app bundles also carry General MIDI maps. | **Preserve the newest distinct file of each type and any hash-distinct historical revision.** NotePerformer is specifically valuable. |
| `User Application Support/Finale 27/Configuration Files/` | `preferences.humanplayback.xml` and `.npbackup` variant | The user's complete Human Playback preference set, including NotePerformer modifications; the backup captures a pre-modification state. | Preserve distinct variants, but label them user-derived rather than factory defaults. |
| `Finale.app/Contents/Resources/` | `defaults.humanplayback.xml` | Factory Human Playback preferences: dynamics, MIDI behavior, glissandi, ornaments, tempo, Garritan options, named techniques, action chains, filters, and instrument applicability. | **Preserve.** This is executable musical interpretation policy expressed as data. |
| same | `strings.humanplayback.xml`, `strings.humanplayback-expressions.xml` | Human Playback terminology and recognition dictionaries for expressions/techniques, including alternate textual spellings and localized UI/message strings. | **Preserve the base-language semantic dictionaries.** Localized copies are useful for multilingual recognition/UI reconstruction but are lower priority. |

### Music-font and glyph conversion metadata

| Generic location | File(s) | Information present | Assessment |
|---|---|---|---|
| `Finale.app/Contents/Resources/` | `glyphnames.json` | Standard SMuFL glyph name to Unicode codepoint and description catalog. | Preserve or reference the matching upstream SMuFL release; it is already consumed in `smufl-mapping`. |
| same | `glyphnamesFinale.json` | MakeMusic/Finale optional-range glyph names, codepoints, and descriptions shared by Finale SMuFL fonts. | **Preserve.** Already captured in `smufl-mapping/source_json` and generated into `glyphnames_finale.h`; this is the authoritative installed copy. |
| `System Application Support/Finale 27/Data/SMuFL Conversion/` | `Broadway Copyist.json`, `Broadway Copyist Perc.json`, `Broadway Copyist Text.json`, `Broadway Copyist Text Ext.json`; `Chaconne.json`; `Engraver Font Set.json`, `Engraver Font Extras.json`, `Engraver Text H.json`, `Engraver Text NCS.json`, `Engraver Text T.json`, `Engraver Time.json`; `Finale AlphaNotes.json`, `Finale Copyist Text.json`, `Finale Copyist Text Ext.json`, `Finale Mallets.json`, `Finale Numerics.json`, `Finale Percussion.json`; `Jazz.json`, `JazzCord.json`, `JazzPerc.json`, `JazzText.json`, `JazzText Extended.json`; `Kousaku.json`, `Kousaku Percussion.json`; `Maestro.json`, `Maestro Percussion.json`, `Maestro Wide.json`, `MaestroTimes.json`; `Petrucci.json`; `Rentaro.json`; `Tamburo.json` | Per legacy font, maps glyph names to SMuFL codepoints and legacy decimal codepoints; some records include x/y positioning offsets and descriptions. | **Preserve the full set.** It is already substantially captured and normalized under `smufl-mapping/source_json/legacy`, where singular `legacyCodepoint` values have been upgraded to arrays and validation/generation tooling exists. Keep pristine originals as provenance. |
| `System` and `User Application Support/Finale 27/Configuration Files/` | `MacSymbolFonts.txt`, plus user `.org` backup | Names and aliases Finale treats as symbol fonts rather than ordinary text fonts. | **Preserve.** This affects character semantics during import/export and font substitution. The user version contains a small local change, so retain both factory and user variants. |
| `User Application Support/Finale 27/` | `ESFontFileNames.txt` | Large Elbsound/JW Lua font-name/file-name cache or registry used by installed plug-ins. | Preserve only if maintaining those plug-ins or studying local font discovery; it is environment-derived, not Finale's canonical metadata. |
| Finale 25/26 app resources | `finale.postscriptinformation.plist` | Legacy PostScript/font output information, removed from the Finale 27 resource set. | **Preserve one newest distinct copy.** It may be valuable for legacy font naming/printing behavior. |

### Keyboard, tools, files, pages, and setup defaults

| Generic location | File(s) | Information present | Assessment |
|---|---|---|---|
| `System Application Support/Finale 27/Configuration Files/` | `keymaps.txt` | UTF-16 shortcut tables for default and laptop layouts; numeric command/tool identifiers, keyboard codes, modifier/state fields, and localized display labels. | **Preserve.** The numeric command IDs may be useful for decoding shortcuts or automation. |
| `Finale.app/Contents/Resources/` | `finale.toolshortcuts.plist` | Ordered list of built-in tool shortcut IDs. | Preserve with `keymaps.txt`; meaningful primarily in relation to Finale's internal command IDs. |
| same | `finale.filedialogtypeinfo.plist` | Finale file-type IDs, UTIs/extensions, allowed dialog actions, and string-resource IDs used by open/save/import/export dialogs. | **Preserve.** Useful for cataloging Finale's supported and legacy file formats. |
| Finale 25/26 resources | `defaults.speedyentrykeymap.plist` | Factory Speedy Entry key mapping, absent as a separate file in Finale 27. | **Preserve the newest distinct copy.** It is unique older behavioral metadata. |
| `System Application Support/Finale 27/Configuration Files/` | `pagesizes.txt` | Named page dimensions and default page/system margins, in inches or centimeters. User tree contains current and `.org` variants. | Preserve factory and any distinct user variant; useful for exact Setup Wizard/document defaults. |
| same | `ensembles.txt` | See instrument section; also functions as Setup Wizard scoring defaults. | Preserve. |

### FinaleScript automation metadata

`System Application Support/Finale 27/FinaleScript/` contains XML scripts in these functional groups:

- Batch Process: export MusicXML, generate linked parts, open MIDI files into a template, print, save graphics/MIDI, and convert a folder to the current Finale format.
- Cleanup Score: run score-preparation plug-ins and perform lyric typo correction.
- Convert: change key and reduce SATB to piano.
- Import: apply a Music Scanning Notebook file, and import Human Playback style, house style, or document options.
- Misc/Set Page Size: book-style page numbers, booklet printing, portrait Letter to landscape A4, and B4/Concert/Folio/Letter/Tabloid presets.
- Simple Scripts: combined selections, reset transposition metatools, respace/update layout, safety prompt, choose Human Playback style, replace smart quotes, swap layers, and demonstrate unit syntax.

The user tree duplicates these and adds NotePerformer scripts for mute/open/plunger, vibrato levels, solo/unison section size, tenuto, and resetting playback effects. `_foldersettings.xml` and `_finalescriptoptions.xml` hold ordering/display/options rather than musical data. Older installations contain equivalent `.txt` scripts and a few retired recipes (`check accidentals and ties/respace/update layout`, `convert to C score`, `euphonium setup`, `plug-in to harmonize and drum groove`, and `voice to Bb sax`). Preserve the NotePerformer set and one representative factory script corpus if FinaleScript compatibility matters; otherwise this is useful documentation but lower-value than the registries above.

### UI, localization, plug-in, and package metadata

The broad text scan also found the following complete classes. They are cataloged here so they are not mistaken for omitted Finale-domain metadata:

- `Finale.app/Contents/Resources/*.lproj/*.strings` and plug-in `*.lproj/*.strings`: localized menus, dialogs, errors, accessibility labels, command names, and plug-in text in the installed languages. Base strings can reveal internal command/resource identifiers; translations mostly duplicate semantics.
- `strings.scoremerger.xml` and `strings.textsearchandreplace.xml`: Score Merger and text-search UI/message dictionaries. `dictionary.instruments.xml` is the semantically important part of the Score Merger resources and is cataloged separately.
- Plug-in `Info.plist`, framework `Info.plist`, `PkgInfo`, `_CodeSignature/CodeResources`, and `InstallerComponents/FinaleAppCore_changes.xml`: bundle identity/version/capability declarations, package markers, signed-resource hashes, and installer file-change manifests. Useful for provenance/version forensics, not musical semantics.
- `images/**/*.xcassets/**/Contents.json`: image filename, scale, idiom, and asset-catalog metadata for Finale's palettes. These describe UI packaging, not music glyph semantics.
- NIB/XIB XML and binary plists: serialized UI layouts and object graphs for Finale and plug-ins. Potentially useful to reconstruct dialogs or internal numeric control IDs, but not a primary metadata source.
- Plug-in-specific text such as `ff_plugin_classnames.h`, localized `FinPI` strings, Elbsound/JW/RGP Lua scripts/configuration, and plug-in preference/storage XML: third-party API/UI metadata or user state. Surveyed, but not authoritative Finale document metadata.
- `Finale.app/.../MusicXML.bundle/.../DTD/`: MusicXML DTD/XSD/module/entity files, version-conversion XSLT, `catalog.xml`, and `sounds.xml`; plus the bundled Java runtime's configuration, licenses, security policies, and properties. The MusicXML schemas/transforms and instrument `sounds.xml` are useful standards resources, but are upstream MusicXML metadata rather than Finale-specific. The duplicated Java runtime trees and legal/config files are not Finale metadata.
- `epub/container.xml` and older iBooks display-options XML: EPUB packaging boilerplate used by help/publication content.
- Help HTML/JS/CSS/data and installer change logs: searchable product documentation and update metadata. They can explain features and UI behavior but do not form a compact machine-readable Finale domain registry.
- `privpol.txt`: privacy-policy text; `AudioDeviceErrors.log`, Plogue/JALAYOUG logs, `LastFANFileCheck.txt`, `LastUpdate.txt`, AU validation/load crumbs, AU caches, `jwpluginprefs.txt`, and `jwpluginstorage.xml`: logs, timestamps, caches, preferences, and local plug-in state. These may expose local environment details and should not be archived as canonical metadata.
- `Exercises.CFG`: an extended-ASCII configuration/index associated with the installed exercise corpus. It is application content metadata, but not a Finale file-format registry.
- `JALAYOUG.ini`: local plug-in configuration/state. Not canonical Finale metadata.
- `iZotope.dat`: a tiny third-party AIO library data marker, not music-domain metadata.

### Older-version-only observations

Finale 27 subsumes most older machine-readable resources. The genuinely notable older-only items are:

- `defaults.speedyentrykeymap.plist` (Finale 25/26);
- `finale.postscriptinformation.plist` (Finale 25/26);
- older `gmsounds.xml`/`sounds.xml` resources (Finale 2014.x), useful as historical MusicXML/GM sound-name snapshots;
- `NotePerformer.soundmap` and the retired `instrument.txt`, `GIFFinstruments.txt`, `Tapspaceinstruments.txt`, and `chordmap.txt` support files;
- the 2012 converter's frozen percussion tables and sound maps;
- retired FinaleScript recipes listed above.

Localized German/Spanish app bundles mostly duplicate the same structural resources and add translated strings. Preserve translations only if multilingual UI or recognition behavior is a goal. Do not treat a translated copy as a separate metadata model.

## Externally usable non-text resources

### Installed Finale fonts

The current `Finale.app` bundle does **not** contain loose `.otf`, `.ttf`, `.ttc`, `.dfont`, `.woff`, or `.woff2` files. Finale's fonts are installed in the system Fonts folder instead. This distinction matters when archiving an installation: copying only `Finale.app` omits the fonts.

There is an important historical exception. `Finale25.5.app/Contents/Resources/` and the English `Finale26.3.app/Contents/Resources/` each contain the same 38 loose legacy TrueType fonts. SHA-256 comparison found the Finale 25.5 and 26.3 sets byte-for-byte identical. The installed Spanish/German Finale 26 bundles, Spanish Finale 27 bundle, current Finale 27 bundle, and installed 2011–2014.x bundles do not contribute another loose Finale font set.

The four particularly unusual Japanese-distribution fonts in this leaked collection are:

- `Chaconne.ttf` — Chaconne;
- `Kousaku.ttf` — Kousaku;
- `Kousaku Percussion.ttf` — Kousaku Percussion;
- `Rentaro.ttf` — Rentaro.

All four also have Finale 27 legacy-to-SMuFL conversion JSON, and the user installation has copies because they were previously discovered and installed. Their presence in the user Fonts folder therefore must not be interpreted as ordinary Finale 27 distribution evidence. The application-bundle copies are the provenance evidence.

The leak is valuable beyond those four files. It supplies portable data-fork TTF versions of many legacy Finale fonts that are otherwise commonly installed as old macOS suitcase/resource-fork fonts:

- engraving families: `Maestro`, `Maestro Percussion`, `Maestro Wide`, `Petrucci`, and `Seville`;
- Engraver components: `EngraverFontSet`, `EngraverFontExtras`, `EngraverTextH`, `EngraverTextNCS`, `EngraverTextT`, and `EngraverTime`;
- handwritten families: `Jazz`, `JazzCord`, `JazzPerc`, `JazzText`, and `JazzText Extended`;
- Broadway Copyist families: `BroadwayCopyist`, `BroadwayCopyistPerc`, `BroadwayCopyistText`, and `BroadwayCopyistTextExt`;
- specialized Finale fonts: `Finale AlphaNotes`, `Finale Mallets`, `Finale Percussion`, `Finale Numerics`, Copyist Text/Text Ext, and Finale Lyrics regular/bold/italic;
- `Tamburo` and the four-style `MaestroTimes` text family.

The MusicXML plug-in inside Finale 25 additionally bundles eight Lucida TTFs as part of its Java runtime (`Lucida Sans`, `Lucida Typewriter`, and `Lucida Bright` styles). Those are an embedded Java runtime dependency, not a Finale music-font leak, and no preservation priority is assigned beyond preserving that runtime snapshot.

**Preserve the complete 38-font Finale 25/26 bundle collection immediately as a unit**, with the original filenames and hashes. It is a safer and more portable source for legacy-font analysis than the resource-fork suitcases, and it may be the only installed source for reconstructing the Japanese faces. Licensing still governs use and redistribution.

The installed modern OpenType families are:

- `FinaleMaestro` and Finale Maestro Text Regular/Bold/Italic/Bold Italic;
- `FinaleEngraver`;
- `FinaleJazz`, Finale Jazz Text, and Finale Jazz Text Lowercase;
- `FinaleBroadway`, Finale Broadway Text, and Finale Broadway Legacy Text;
- `FinaleAsh` and Finale Ash Text;
- `FinaleLegacy`;
- Finale Lyrics Regular/Bold/Italic.

Installed TrueType companion/legacy fonts include Finale Copyist Text and Text Ext, Finale Lyrics regular/bold/italic, Finale Numerics, and MaestroTimes regular/bold/italic/bold italic.

Legacy suitcase/resource-fork fonts are also installed for Maestro, Maestro Percussion, Maestro Wide, Engraver Font Set/Extras/Text H/Text NCS/Text T/Time, Broadway Copyist/Text/Text Ext/Percussion, Jazz/JazzCord/JazzPerc/JazzText/JazzText Extended, Petrucci, Seville, Tamburo, Finale AlphaNotes, Finale Mallets, and Finale Percussion. Several appear as zero-byte data-fork files because their content lives in macOS resource forks or associated suitcase structures. **Do not preserve these with a copier or archive format that discards extended attributes/resource forks.** Use a macOS-aware archive/copy mechanism and verify the restored fonts before deleting an original installation.

These fonts are externally useful for:

- rendering legacy Finale documents faithfully;
- testing legacy character-to-SMuFL conversion;
- measuring glyph geometry and comparing metrics with the JSON conversion offsets;
- producing compatibility previews or regression fixtures;
- resolving font-specific characters during MusicXML, SVG, PDF, or other export.

**Preserve the complete installed Finale font set**, not just the flagship SMuFL fonts. Record PostScript/family names, file hashes, font table metadata, and resource-fork hashes. Redistribution and embedding rights must be checked separately; presence in an installed Finale distribution is not evidence of a permissive license.

### Font Annotation files

`System Application Support/Finale 27/Font Annotation/` contains 45 `.fan` files, with further user-installed `.fan` files. They encode Finale's per-character font annotation/metric data used for spacing, selection, and collision geometry. The factory set includes annotations for the Finale legacy fonts and ordinary text fonts; additional files cover SMuFL and third-party fonts such as Bravura, November2, Chaconne, Kousaku, PattersonSonata, and others.

The 2012 converter bundle carries a frozen subset under `Finale.app/Contents/PlugIns/2012FileConverter.../Font Annotation/`, duplicated inside the Spotlight importer. Preserve one distinct converter copy plus all distinct system annotations. These are especially valuable when reverse-engineering Finale's glyph bounds or reproducing layout without Finale, even though `.fan` is not a plain-text format.

### Playback and reusable musical content

| Generic location | Assets | External use and recommendation |
|---|---|---|
| `System Application Support/Finale 27/Audio Support/` | `synthgms.sf2` (about 42 MB) | General MIDI SoundFont used by SmartMusic SoftSynth. Externally loadable by SoundFont-compatible samplers and useful for playback parity. **Preserve**, subject to license/redistribution terms. |
| `System Application Support/Finale 27/Plugin Components/Drum Groove/` | 29 `.MID` files | Small reusable MIDI grooves spanning jazz, rock, funk, Latin, march, polka, country, reggae, R&B, and related styles. Preserve if exact plug-in behavior or a compact rhythm corpus is useful. |
| `System Application Support/Finale 27/Plugin Components/Latin Percussion/` | Parallel Maestro, Jazz, and SMuFL `.musx` examples for cha-cha, mambo, salsa, bolero, and Afro-Cuban patterns | Particularly useful conversion fixtures: the same musical ideas are encoded with legacy and SMuFL font setups. **Preserve as a matched regression corpus.** |
| `System Application Support/Finale 27/Libraries/` | 108 `.lib` files | Reusable Finale libraries for articulations, expressions, chord suffixes, fretboards, percussion layouts/maps, shapes, staff styles, document options, music spacing, and font-specific defaults. **Preserve the complete hierarchy with filenames.** These are valuable behavioral/default data even though they are binary. |
| `System Application Support/Finale 27/Music Files/Default Files/` | Factory default `.musx` documents for Finale Maestro/Broadway/Jazz and legacy Maestro/Jazz styles | Canonical source of document defaults not represented by the text registries. Preserve and unpack to EnigmaXml for diffable reference fixtures. |
| `System Application Support/Finale 27/Music Files/Document Styles/` | Engraved, handwritten, choral, band, marching, orchestral, handbell, elementary-education, guitar/TAB, and legacy document styles | **Preserve.** They capture curated engraving policy, fonts, page/layout choices, expression/articulation libraries, and instrument setup. |
| `System Application Support/Finale 27/Music Files/Templates/` | Orchestra, band, chamber, jazz, choir, church, guitar/TAB, education, and general templates | Preserve at least one pristine factory corpus. These provide structured orchestration and layout examples tied to the UUID/ensemble metadata. User copies may be modified and should be labeled separately. |
| `System Application Support/Finale 27/Music Files/Worksheets & Repertoire/`, `Exercises/`, and `Tutorials/` | Large `.musx` educational/repertoire corpus plus `Exercises.CFG` | Useful as real-world parsing, conversion, layout, and semantic-classification fixtures. Deduplicate by content hash and check copyright before redistribution. |

The support tree contains 2,332 `.musx` files in total, so this corpus should be preserved with a manifest and hashes rather than enumerated file-by-file in the metadata report. System and user copies overlap heavily; treat the system tree as the pristine baseline and retain only hash-distinct user variants.

### Schemas, artwork, and application integration assets

| Generic location | Assets | External use and recommendation |
|---|---|---|
| `Finale.app/Contents/PlugIns/MusicXML.bundle/Contents/Resources/DTD/` | MusicXML DTD/XSD modules, entities, container/opus schemas, sound definitions, catalog, and XSLT transforms between MusicXML versions and partwise/timewise forms | Immediately reusable for offline validation and conversion, although authoritative upstream MusicXML releases are preferable. Preserve Finale's exact snapshot when reproducing its importer/exporter behavior. |
| `Finale.app/Contents/Resources/images/PaletteImages.xcassets/` | Hundreds of vector PDF palette icons, with normal/open/rollover states | Externally usable as a detailed visual taxonomy of Finale tools and notation operations. Preserve for UI archaeology or compatibility documentation; check artwork licensing before reuse in another product. |
| `Finale.app/Contents/Resources/images/Images.xcassets/` | Raster and some vector UI diagrams for barlines, beams, ties, slurs, key/time signatures, smart shapes, MIDI controls, fretboards, playback/mixer controls, and dialogs | Useful as labeled examples of engraving concepts and internal option states. The asset directory names often carry more semantic information than the pixels themselves. |
| `Finale.app/Contents/Resources/` | Finale application and document-type `.icns` files | Useful for identifying legacy Finale file types and Finder integration. Low research value beyond the file-type plist unless UI preservation is a goal. |
| same | Cursor PNG collection | Named editing modes for smart shapes, MIDI dragging, insertion, playback, zoom, and resizing. Useful for UI behavior reconstruction; otherwise low priority. |
| `System Application Support/Finale 27/Resources/` | Backgrounds and manuscript textures | Reusable page/background artwork for matching Finale document appearance. Preserve if visual fidelity matters; verify licensing before external publication. |

Compiled frameworks, plug-in binaries, the bundled Java runtime, and Audio Unit support components are technically reusable only as part of their intended runtime and are architecture/version/license sensitive. They should be archived for application preservation, but they are not recommended as general-purpose project dependencies.

## Repository cross-check

- `musxdom`: keeps proprietary source material only in the local, gitignored `research/private/` archive; the tracked `finale/` directory has been removed. The private archive includes the high-value application/support registries, fonts, annotations, playback assets, libraries, factory document corpus, MusicXML resources, UI assets, and selected historical files cataloged here, with a SHA-256 manifest. A C++ representation of the percussion note types remains in `src/musx/dom/PercussionNoteType.*`.
- `smufl-mapping`: already captures `glyphnamesFinale.json`, an SMuFL `glyphnames.json`, and normalized versions of all surveyed legacy SMuFL conversion maps. Its generator, validator, reverse maps, and legacy font metadata make it the natural home for those assets. Preserve pristine installed originals separately or record their hashes for provenance.
- `denigma`: contains a compiled instrument UUID to MusicXML instrument-sound mapping in `src/formats/musicxml/musicxml_instruments.cpp`, apparently derived from the instrument registries. The installed `FinaleInstUUIDs.xml` and `SMInstrumentsMap.json` are still valuable primary sources and should be retained.
- `finale-mus-reader`: contains broad Finale/EnigmaXml research and default-document captures but, in the searched material, no direct copies of the principal bundle/support registries cataloged above. It is a plausible home for legacy conversion research, not a substitute for preserving the source files.

## Recommended preservation set

Save these outside the live installation, retaining generic provenance, Finale version, byte hash, and original encoding:

1. `FinaleInstUUIDs.xml`, `SMInstrumentsMap.json`, and `dictionary.instruments.xml`.
2. `PercNoteTypes.txt`, both 2012-converter percussion tables, every distinct MIDI Device Annotation XML, and `PercMapConversionTable.txt`.
3. Every distinct `.soundmap`, especially GIFF, SoftSynth, General MIDI, NotePerformer, and converter snapshots.
4. `defaults.humanplayback.xml`, the Human Playback semantic string dictionaries, and distinct factory/user/NotePerformer preference files.
5. `glyphnamesFinale.json` and the complete 31-file SMuFL Conversion directory, even though normalized copies already exist in `smufl-mapping`.
6. `ensembles.txt`, `keymaps.txt`, `pagesizes.txt`, `MacSymbolFonts.txt`, `finale.filedialogtypeinfo.plist`, `finale.toolshortcuts.plist`, and the older-only Speedy Entry/PostScript plists.
7. Older-only `instrument.txt`, `GIFFinstruments.txt`, `Tapspaceinstruments.txt`, `chordmap.txt`, `gmsounds.xml`, and `sounds.xml`.
8. The complete system-installed Finale font set, using a resource-fork-safe archive method, plus all distinct `.fan` annotation files. Archive the 38 loose, byte-identical Finale 25/26 bundle TTFs separately as especially valuable portable originals; this includes Chaconne, Kousaku, Kousaku Percussion, and Rentaro.
9. `synthgms.sf2`, the 29 MIDI drum grooves, and the matched legacy/SMuFL Latin-percussion examples.
10. Factory `.lib` libraries, default files, document styles, templates, and a manifest/hash-deduplicated copy of the worksheet/exercise/tutorial corpus.
11. Finale's exact MusicXML schema/transform snapshot and, when UI preservation matters, the named vector/raster asset catalogs.

Avoid archiving user logs, caches, AU validation artifacts, timestamps, and plug-in preference stores with the canonical set. They add machine-specific noise and can disclose details of the local environment.
