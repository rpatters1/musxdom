#!/usr/bin/env python3
"""Join binary and MUSX analyses into the human-readable corpus inventory."""

from __future__ import annotations

import argparse
import csv
import json
from pathlib import Path


def era(product: str) -> tuple[str, str, str]:
    if product == "unknown":
        return "pre-banner/apparent Finale 2", "low", "uncertain"
    if product in {"3.0", "3.2", "3.5", "3.7", "97", "2000"}:
        return "low-entropy legacy", "high", "likely"
    if product in {"2001", "2002", "2003", "2004", "2004b", "2005"}:
        return "high-entropy legacy", "high", "likely"
    if product == "2006":
        return "high-entropy legacy", "high", "unlikely/unknown"
    if product in {"2007", "2008"}:
        return "typed-zlib transition", "high", "no"
    return "typed-zlib stable", "high", "no"


def corpus_id(sha256: str) -> str:
    """Stable public identifier; it reveals equality, not the source location."""
    return "mus-" + sha256[:16]


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("inventory_csv", type=Path)
    parser.add_argument("structure_csv", type=Path)
    parser.add_argument("semantics_csv", type=Path)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    with args.inventory_csv.open(newline="", encoding="utf-8") as handle:
        inventory = list(csv.DictReader(handle))
    with args.structure_csv.open(newline="", encoding="utf-8") as handle:
        structures = {row["source_relative"]: row for row in csv.DictReader(handle)}
    with args.semantics_csv.open(newline="", encoding="utf-8") as handle:
        semantics = {row["source_relative"]: row for row in csv.DictReader(handle)}

    lines = [
        "# Corpus Inventory",
        "",
        "This table includes every `.mus` data-fork candidate examined. Saving product comes from the file banner; "
        "`unknown` is reserved for pre-banner files and one AppleDouble artifact. SHA-256 hashes cover complete files. "
        "An em dash means no exact adjacent Finale 27 export was found. The public table intentionally shows only "
        "basenames and content-derived IDs. Original paths are local evidence only; keep them in the ignored "
        "`mus_feasibility/private/corpus_locations.csv` file described in the README.",
        "",
        "Version confidence is high when the banner is explicit and low for pre-banner path-based classification. "
        "ETF likelihood is an eligibility estimate, not a verified open/export result. `Created app` is the creator tuple "
        "preserved by Finale 27 and helps identify upgraded documents. `Parts` is based on converted `partDef` records; "
        "conversion may expand sharing.",
        "",
        "| # | Corpus ID | Filename | Bytes | Source SHA-256 | Save product / era | Created app | ETF | Parts / notable converted features | Export | Export SHA-256 |",
        "|---:|---|---|---:|---|---|---|---|---|---|---|",
    ]
    for item in inventory:
        semantic = semantics.get(item["source_relative"], {})
        structure = structures.get(item["source_relative"], {})
        format_era, confidence, etf = era(item["saving_product"])
        created = semantic.get("created_app_version", "") or "—"
        if semantic:
            others = json.loads(semantic["others_types"])
            parts = int(semantic["part_def_count"])
            features = (
                f"parts={parts}; measures={others.get('measSpec', 0)}; staves={others.get('staffSpec', 0)}; "
                f"entries={semantic['entries_count']}; expr={others.get('textExprDef', 0)}; artic={others.get('articDef', 0)}"
            )
        else:
            features = "no exact MUSX reference"
        if structure.get("validated_wrapper_endian", "none") != "none":
            features += f"; wrapper={structure['validated_wrapper_endian']}"
        source = item["source_relative"].rsplit("/", 1)[-1].replace("|", "\\|")
        export = item["export_relative"].rsplit("/", 1)[-1].replace("|", "\\|") or "—"
        export_hash = f"`{item['export_sha256']}`" if item["export_sha256"] else "—"
        lines.append(
            f"| {item['index']} | `{corpus_id(item['source_sha256'])}` | `{source}` | {item['source_size']} | `{item['source_sha256']}` | "
            f"{item['saving_product']} / {format_era} ({confidence}) | {created} | {etf} | {features} | "
            f"`{export}` | {export_hash} |"
        )
    args.output.write_text("\n".join(lines) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
