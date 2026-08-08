# Version Matrix

The saving product comes from the binary banner, not filenames or timestamps. “ETF likely” means the file should be
tested with the earliest compatible application available; it is not a guarantee. Fifteen local ETF exports are now
available across private and tracked evidence, including two saves of the same Finale 2000 template by Finale 2000
and Finale 2005, the exact Finale 2000 `tremolos` pair, plus controlled Finale 2002–2005 pairs. Three targeted Finale
27 conversions now cover the ETF-backed 1.8.7, 2.0.1, and 2.6 sources.

| Saving product | Files | Structural family | Header/body characteristic | MUSX counterparts | ETF likely | Notes |
|---|---:|---|---|---:|---|---|
| apparent Finale 1.8.7–2.6 (archive-derived) | 136 unique archive members | early fixed-row/indexed | explicit `Finale(TM)` product strings; 16-byte ordinary/detail cadence, implicit-ID 32-byte entries, raw text | 3 targeted | three ETF exports analyzed | Finale 27 opens all selected files after adding `.mus`; detail tags and all 11,089 entry rows correlate exactly; indexes/boundaries unresolved; no Finale 1.0 yet |
| apparent Finale 2 / unknown | 55 | pre-banner | no Enigma banner; distinct `DA`/numeric body family; includes one AppleDouble artifact | 53 exact | uncertain but valuable | path-supported classification only |
| Finale 3.0 | 3 | uncompressed fixed-row | four little-endian typed pools; all three are Windows-origin | 3 | yes | 3/3 framed; earliest explicit product in direct corpus |
| Finale 3.2 | 3 | uncompressed fixed-row | four big-endian typed pools | 3 | yes | 3/3 framed |
| Finale 3.5 | 2 | uncompressed fixed-row | four big-endian typed pools | 2 | yes | 2/2 framed |
| Finale 3.7 | 20 | uncompressed fixed-row | four big-endian typed pools | 20 | yes | 20/20 framed; incidental zlib signature is not a wrapper |
| Finale 97 | 70 | uncompressed fixed-row | four big-endian typed pools in 69/70 | 68 | yes | one file needs integrity/classification work |
| Finale 2000 | 92 | uncompressed fixed-row | four typed pools; 91 big-endian, one Windows-origin little-endian | 91 | analyzed | exact `tremolos` ETF pair; same fixed rows as 2001–06; template resave evidence remains |
| Finale 2001 | 4 | DCL-compressed legacy | big-endian typed/length/CRC blocks; 3/4 framed files, all 9 members DCL/CRC-valid | 4 | yes | major codec boundary; resolved direct files retain 16-byte other/detail rows |
| Finale 2002 | 48 | DCL-compressed legacy | same framing; 47/48 files, all 181 members DCL/CRC-valid | 47 | yes | controlled pairs prove 16-byte other/detail and 38-byte entry rows; `IS` uses three rows |
| Finale 2003 | 168 | DCL-compressed legacy | same framing; 168/168 files, all 659 members DCL/CRC-valid | 163 | yes | fixed physical rows persist; controlled `IS` expands to six rows |
| Finale 2004 | 7 | DCL-compressed legacy | same framing; 7/7 files, all 25 members DCL/CRC-valid | 7 | yes | controlled MUS/ETF pairs available |
| Finale 2004b | 13 | DCL-compressed legacy | 6/13 framed files, all 23 recognized members DCL/CRC-valid | 6 | yes | seven unframed variants need classification |
| Finale 2005 | 120 | DCL-compressed legacy | 116/120 framed files, all 459 members DCL/CRC-valid | 118 | analyzed | fixed rows persist; controlled `MS` and `SS` expand from two physical rows to three |
| Finale 2006 | 66 | DCL-compressed legacy | 63/66 framed files, all 247 members DCL/CRC-valid; empty `0x0013` after nonempty `0x0012` | 63 | unlikely | fixed rows persist; three unframed files need classification |
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

1. archive-derived explicit Finale 1.8.7–2.6 fixed-row/indexed family;
2. pre-banner/Finale 2;
3. Finale 3.x–2000 uncompressed typed pools with platform byte order;
4. Finale 2001–2006 typed PKWARE DCL with CRC-32;
5. Finale 2007–2012 typed zlib, with two record serialization variants around 2007–2008.

The explicit 1.8.7–2.6 family shares Finale 3.0's logical record model; the direct apparent-Finale-2/unknown family still needs classification. Exact minimal pairs are the shortest path to its index and boundary rules.
