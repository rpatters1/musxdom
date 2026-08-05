# Version Matrix

The saving product comes from the binary banner, not filenames or timestamps. “ETF likely” means the file should be
tested with the earliest compatible application available; it is not a guarantee. Six local ETF exports are now
available as private evidence, including two saves of the same Finale 2000 template by Finale 2000 and Finale 2005.

| Saving product | Files | Structural family | Header/body characteristic | MUSX counterparts | ETF likely | Notes |
|---|---:|---|---|---:|---|---|
| apparent Finale 1.8.7–2.6 (archive-derived) | 136 unique archive members | pre-banner | explicit `Finale(TM)` product strings; low-entropy/pre-banner bodies; no Finale 27 exports | none | three ETF exports analyzed | ETF confirms ordinary/details/entries/text sections, but exporter version and binary-to-ETF mapping remain separate questions; no Finale 1.0 yet |
| apparent Finale 2 / unknown | 55 | pre-banner | no Enigma banner; distinct `DA`/numeric body family; includes one AppleDouble artifact | 53 exact | uncertain but valuable | path-supported classification only |
| Finale 3.0 | 3 | low-entropy legacy | explicit banner; little-endian-looking prefix | 3 | yes | earliest explicit product in corpus |
| Finale 3.2 | 3 | low-entropy legacy | explicit banner; body entropy about 3.6 | 3 | yes | creator/save tuples present |
| Finale 3.5 | 2 | low-entropy legacy | explicit banner | 2 | yes | small sample |
| Finale 3.7 | 20 | low-entropy legacy | explicit banner; one incidental valid zlib signature is not a wrapper | 20 | yes | overlaps later “97” era signatures |
| Finale 97 | 70 | low-entropy legacy | visible tables/ASCII tokens after `0x200` | 68 | yes | likely high ETF value |
| Finale 2000 | 92 | low-entropy legacy | similar low-entropy table/stream family | 91 | analyzed | same template exported by Finale 2000 and 2005; later export adds records and rewrites saver metadata |
| Finale 2001 | 4 | high-entropy legacy | abrupt shift to ~7.65-bit body; no zlib | 4 | yes | probable major codec boundary |
| Finale 2002 | 48 | high-entropy legacy | `0x000f`-family prefix; no zlib | 47 | yes | several very small test files |
| Finale 2003 | 168 | high-entropy legacy | same broad family | 163 | yes | good corpus depth |
| Finale 2004 | 7 | high-entropy legacy | same broad family | 7 | yes | limited sample |
| Finale 2004b | 13 | high-entropy legacy | endian/prefix variants | 6 | yes | some missing conversions |
| Finale 2005 | 120 | high-entropy legacy | same broad family; no zlib | 118 | analyzed | ETF header identifies Finale 2005; logical entry/detail records are available, but binary codec remains unresolved |
| Finale 2006 | 66 | high-entropy legacy | same broad family with changed stable bytes | 63 | unlikely | ETF availability must be tested, not assumed |
| Finale 2007 | 108 | typed zlib transition | four CRC blocks; 81 big-endian, 27 little-endian | 104 | no | first solved wrapper era; mixed serialization |
| Finale 2008 | 182 | typed zlib transition | 180 little-endian, 2 big-endian | 180 | no | sharing/linked parts common |
| Finale 2009 | 5 | typed zlib stable | four blocks, little-endian | 5 | no | small sample |
| Finale 2010 | 4 | typed zlib stable | four blocks, little-endian | 4 | no | small sample |
| Finale 2012 / File Converter | 248 | typed zlib stable | four principal blocks, little-endian in all sampled wrappers | 248 | no | banner count combines 238 Finale 2012 and 10 File Converter files |

## Internal and creation versions

The header stores separate creator and last-saver version tuples. Finale 27 conversion exposes the creator tuple in EnigmaXml. Across 1,189 decoded exports, common creator application versions include 5.0.2.1 (138), 17.0.3.13 (123), 13.0.2.1 (118), 3.8.2.1 (103), and 12.0.1.3 (100); 173 exports have no creator application version. This demonstrates that “created by” and “last saved by” are distinct and that many documents were upgraded across releases.

The exact meaning of all tuple members (Enigma version, application version, file version, status/build) is already represented in Finale 27 EnigmaXml and should be used as the semantic reference when the binary packing is decoded. The banner remains the reliable saving-product label for this study.

## Compatibility families

Evidence currently supports at least four parsers/codecs, not one parser per Finale release:

1. archive-derived explicit Finale 1.8.7–2.6 pre-banner family;
2. pre-banner/Finale 2;
3. Finale 3.x–2000 low-entropy legacy;
4. Finale 2001–2006 high-entropy legacy;
5. Finale 2007–2012 typed zlib, with two record serialization variants around 2007–2008.

Whether the first two families share a decompressed logical record model is an open question. ETF pairs are the shortest path to answering it.
