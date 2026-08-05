#!/usr/bin/env python3
"""Render the machine-readable record observations as a compact catalog."""

from __future__ import annotations

import argparse
import csv
import json
from collections import defaultdict
from pathlib import Path


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("record_catalog", type=Path)
    parser.add_argument("correlations", type=Path)
    parser.add_argument("--inventory", type=Path, help="private inventory used to resolve source IDs")
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    with args.record_catalog.open(newline="", encoding="utf-8") as handle:
        observations = list(csv.DictReader(handle))
    with args.correlations.open(newline="", encoding="utf-8") as handle:
        correlations = list(csv.DictReader(handle))
    source_ids: dict[str, str] = {}
    if args.inventory and args.inventory.exists():
        with args.inventory.open(newline="", encoding="utf-8") as handle:
            for row in csv.DictReader(handle):
                source_ids[row["source_relative"]] = "mus-" + row["source_sha256"][:16]

    best: dict[str, dict[str, str]] = {}
    for row in correlations:
        if row["candidate_rank"] != "1":
            continue
        current = best.get(row["field0_hex"])
        if current is None or float(row["score"]) > float(current["score"]):
            best[row["field0_hex"]] = row
    grouped: dict[str, list[dict[str, str]]] = defaultdict(list)
    for row in observations:
        grouped[row["field0_hex"]].append(row)

    lines = [
        "# Record Catalog",
        "",
        "This catalogs numeric identifiers observed after decompressing record-framed zlib blocks. "
        "It does not claim that pre-2007 encoded bytes use the same identifiers. The proposed XML names "
        "come from corpus-wide count correlation with Finale 27 exports; conversion differences and count "
        "collisions remain possible. All candidate rankings are preserved in "
        "[`data/record_correlations.csv`](data/record_correlations.csv), and per-version raw observations in "
        "`private/record_catalog.csv` (local-only).",
        "",
        "Frame fields are described in [FORMAT_NOTES.md](FORMAT_NOTES.md). `Example offset` is relative to the "
        "decompressed zlib member, not the file.",
        "",
        "| Identifier | Proposed structure | Pool | Confidence | Versions | Payload bytes observed | Example (member:offset) |",
        "|---|---|---|---|---|---|---|",
    ]
    for code in sorted(grouped):
        rows = grouped[code]
        versions = sorted({row["saving_product"] for row in rows})
        sizes = set()
        for row in rows:
            sizes.update(int(value) for value in json.loads(row["payload_sizes"]).keys())
        candidate = best.get(code)
        if candidate:
            exact = float(candidate["all_file_exact_rate"])
            corr = float(candidate["pearson"])
            nonzero = int(candidate["nonzero_exact"])
            if corr >= 0.999 and exact >= 0.95 and nonzero >= 20:
                confidence = "strong"
            elif corr >= 0.98 and nonzero >= 10:
                confidence = "moderate"
            else:
                confidence = "weak"
            proposed = candidate["xml_tag"]
            pool = candidate["pool"]
        else:
            proposed = "unknown"
            pool = "unknown/options"
            confidence = "open"
        example = rows[0]
        # Keep the public catalog location-neutral and unambiguous.
        path = source_ids.get(example["example_source"], "file-" + example["example_source"].rsplit("/", 1)[-1])
        lines.append(
            f"| `{code}` | {proposed} | {pool} | {confidence} | {', '.join(versions)} | "
            f"{', '.join(map(str, sorted(sizes)))} | `{path}` ({example['example_member']}:{example['example_decoded_offset']}) |"
        )
    args.output.write_text("\n".join(lines) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
