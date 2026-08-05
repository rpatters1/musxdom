# Archive Survey

Scanned all 230 ZIP and 275 StuffIt archives and found 4,898 candidate `.mus` or extensionless members. Archive paths are intentionally omitted; use the local ignored `mus_feasibility/private/archive_locations.csv` mapping for archive/member locations. The `unar`/`lsar` 1.10.7 tools successfully listed and extracted the StuffIt set, including resource forks.

Enigma-banner members: 2271.
Extensionless candidates: 2990.

The deep probe found 2,271 Enigma-banner members and 1,383 unique Enigma hashes, including 1,082 unique hashes absent from the direct inventory. Explicit pre-banner products include 1.8.7 (10 unique hashes), 2.0.1 (26), 2.6 (100), 3.0 (15), 3.2 (14), 3.5 (19), and 3.7 (25). The earliest explicit archive product is Finale 1.8.7; no explicit Finale 1.0 sample has yet been identified.

For 2007–2012 archive members, the deep probe validated the same typed-zlib wrapper and generic record framing seen in the direct corpus: 220 framed 2007 members, 98 framed 2008 members, and 357 framed 2012 members. The early 1.8.7–2.6 members had no validated zlib members or generic framed records, confirming that they belong to a separate pre-banner/low-entropy family. Incidental zlib signatures in 3.5/3.7 did not validate as wrappers.
