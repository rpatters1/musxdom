#!/usr/bin/env python3
"""Validate uncompressed Finale 3.x-2000 typed pools without exposing paths.

The probe tries both byte orders at offset 0x200, requires a complete walk to
EOF, and reports aggregate framing and fixed-row counts by saving product.
"""

from __future__ import annotations

import argparse
import csv
import json
import struct
from collections import Counter, defaultdict
from pathlib import Path


PRODUCTS = ("3.0", "3.2", "3.5", "3.7", "97", "2000")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("locations", type=Path, help="private corpus_locations.csv")
    parser.add_argument("manifest", type=Path, help="public corpus_manifest.csv")
    parser.add_argument("--output", type=Path, help="write JSON here instead of stdout")
    return parser.parse_args()


def load_inputs(locations_path: Path, manifest_path: Path) -> list[tuple[str, Path]]:
    products: dict[str, str] = {}
    with manifest_path.open(newline="", encoding="utf-8") as handle:
        for row in csv.DictReader(handle):
            if row["saving_product"] in PRODUCTS:
                products[row["corpus_id"]] = row["saving_product"]

    result: list[tuple[str, Path]] = []
    with locations_path.open(newline="", encoding="utf-8") as handle:
        for row in csv.DictReader(handle):
            product = products.get(row["corpus_id"])
            if product:
                result.append((product, Path(row["source_path"])))
    return result


def walk(data: bytes, byte_order: str) -> list[tuple[int, int]] | None:
    prefix = ">" if byte_order == "big" else "<"
    offset = 0x200
    members: list[tuple[int, int]] = []
    while offset + 6 <= len(data):
        kind, size = struct.unpack_from(prefix + "HI", data, offset)
        if size < 6 or offset + size > len(data):
            return None
        members.append((kind, size - 6))
        offset += size
    return members if offset == len(data) and members else None


def main() -> None:
    args = parse_args()
    stats: dict[str, Counter[str]] = defaultdict(Counter)
    for product, path in load_inputs(args.locations, args.manifest):
        if not path.is_file():
            stats[product]["unresolved_files"] += 1
            continue
        data = path.read_bytes()
        matches = [(order, walk(data, order)) for order in ("big", "little")]
        matches = [(order, members) for order, members in matches if members is not None]
        if len(matches) != 1:
            stats[product]["unframed_or_ambiguous_files"] += 1
            continue
        byte_order, members = matches[0]
        assert members is not None
        stats[product]["framed_files"] += 1
        stats[product][f"{byte_order}_endian_files"] += 1
        if tuple(kind for kind, _size in members) != (1, 2, 3, 4):
            stats[product]["unexpected_type_sequences"] += 1
        for kind, payload_size in members:
            stats[product][f"members_0x{kind:04x}"] += 1
            if kind in (1, 2):
                stats[product][f"rows_0x{kind:04x}"] += payload_size // 16
                if payload_size % 16:
                    stats[product][f"nonmultiple_16_0x{kind:04x}"] += 1
            elif kind == 3:
                stats[product]["rows_0x0003"] += payload_size // 38
                if payload_size % 38:
                    stats[product]["nonmultiple_38_0x0003"] += 1

    result = {product: dict(sorted(stats[product].items())) for product in PRODUCTS}
    rendered = json.dumps(result, indent=2, sort_keys=True) + "\n"
    if args.output:
        args.output.write_text(rendered, encoding="utf-8")
    else:
        print(rendered, end="")


if __name__ == "__main__":
    main()
