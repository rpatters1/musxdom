#!/usr/bin/env python3
"""Create a location-neutral public manifest and an optional private path map.

The public manifest is suitable for version control.  The private map is
intentionally opt-in and should be written to the ignored
``mus_feasibility/private/corpus_locations.csv`` path.
"""

from __future__ import annotations

import argparse
import csv
from pathlib import Path


def corpus_id(sha256: str) -> str:
    return "mus-" + sha256[:16]


def basename(value: str) -> str:
    return value.rsplit("/", 1)[-1]


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("inventory_csv", type=Path)
    parser.add_argument("--public-output", type=Path, required=True)
    parser.add_argument("--private-output", type=Path)
    args = parser.parse_args()

    with args.inventory_csv.open(newline="", encoding="utf-8") as handle:
        rows = list(csv.DictReader(handle))

    public_fields = [
        "corpus_id", "filename", "source_size", "source_sha256", "saving_product",
        "export_match", "export_filename", "export_size", "export_sha256",
    ]
    public = []
    private = []
    for row in rows:
        cid = corpus_id(row["source_sha256"])
        public.append({
            "corpus_id": cid,
            "filename": basename(row["source_relative"]),
            "source_size": row["source_size"],
            "source_sha256": row["source_sha256"],
            "saving_product": row["saving_product"],
            "export_match": row["export_match"],
            "export_filename": basename(row["export_relative"]) if row["export_relative"] else "",
            "export_size": row["export_size"],
            "export_sha256": row["export_sha256"],
        })
        private.append({
            "corpus_id": cid,
            "source_relative": row["source_relative"],
            "source_path": row["source_path"],
            "export_relative": row["export_relative"],
            "export_path": row["export_path"],
        })

    args.public_output.parent.mkdir(parents=True, exist_ok=True)
    with args.public_output.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=public_fields)
        writer.writeheader()
        writer.writerows(public)

    if args.private_output:
        args.private_output.parent.mkdir(parents=True, exist_ok=True)
        fields = list(private[0]) if private else ["corpus_id", "source_relative", "source_path", "export_relative", "export_path"]
        with args.private_output.open("w", newline="", encoding="utf-8") as handle:
            writer = csv.DictWriter(handle, fieldnames=fields)
            writer.writeheader()
            writer.writerows(private)


if __name__ == "__main__":
    main()
