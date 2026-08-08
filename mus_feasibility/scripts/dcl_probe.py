#!/usr/bin/env python3
"""Validate Finale 2001-2006 DCL blocks and report aggregate row sizes.

This tool invokes an external PKWARE DCL decoder compatible with Mark Adler's
``blast`` test program: compressed bytes on stdin, decoded bytes on stdout.
It reads private paths but never includes them in its output.
"""

from __future__ import annotations

import argparse
import binascii
import csv
import json
import struct
import subprocess
from collections import Counter, defaultdict
from pathlib import Path


PRODUCTS = ("2001", "2002", "2003", "2004", "2004b", "2005", "2006")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("locations", type=Path, help="private corpus_locations.csv")
    parser.add_argument("manifest", type=Path, help="public corpus_manifest.csv")
    parser.add_argument("--blast", type=Path, required=True, help="blast-compatible executable")
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


def decode(blast: Path, encoded: bytes) -> bytes | None:
    process = subprocess.run(
        [str(blast)], input=encoded, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=False
    )
    return process.stdout if process.returncode == 0 else None


def probe_file(path: Path, blast: Path) -> tuple[bool, list[tuple[int, int, bytes | None, bool]]]:
    data = path.read_bytes()
    offset = 0x200
    members: list[tuple[int, int, bytes | None, bool]] = []
    while offset + 6 <= len(data):
        kind, size = struct.unpack_from(">HI", data, offset)
        if size < 6 or offset + size > len(data):
            return False, []
        if size == 6:
            members.append((kind, size, b"", True))
            return True, members
        if size < 10:
            return False, []
        expected_crc = struct.unpack_from(">I", data, offset + 6)[0]
        decoded = decode(blast, data[offset + 10 : offset + size])
        if decoded is None:
            members.append((kind, size, None, False))
        else:
            crc_ok = (binascii.crc32(decoded) & 0xFFFFFFFF) == expected_crc
            members.append((kind, size, decoded, crc_ok))
        offset += size
    return bool(members), members


def main() -> None:
    args = parse_args()
    stats: dict[str, Counter[str]] = defaultdict(Counter)
    for product, path in load_inputs(args.locations, args.manifest):
        if not path.is_file():
            stats[product]["unresolved_files"] += 1
            continue
        framed, members = probe_file(path, args.blast)
        if not framed:
            stats[product]["unframed_files"] += 1
            continue
        stats[product]["framed_files"] += 1
        for kind, _stored_size, decoded, valid in members:
            stats[product][f"members_0x{kind:04x}"] += 1
            if decoded is None:
                stats[product]["compressed_members"] += 1
                stats[product]["decode_or_crc_failures"] += 1
                continue
            if not decoded:
                stats[product][f"empty_0x{kind:04x}"] += 1
                continue
            stats[product]["compressed_members"] += 1
            if not valid:
                stats[product]["decode_or_crc_failures"] += 1
                continue
            stats[product]["crc_valid_members"] += 1
            if kind in (0x000F, 0x0010):
                stats[product][f"rows_0x{kind:04x}"] += len(decoded) // 16
                if len(decoded) % 16:
                    stats[product][f"nonmultiple_16_0x{kind:04x}"] += 1
            elif kind == 0x0011:
                stats[product]["rows_0x0011"] += len(decoded) // 38
                if len(decoded) % 38:
                    stats[product]["nonmultiple_38_0x0011"] += 1

    result = {product: dict(sorted(stats[product].items())) for product in PRODUCTS}
    rendered = json.dumps(result, indent=2, sort_keys=True) + "\n"
    if args.output:
        args.output.write_text(rendered, encoding="utf-8")
    else:
        print(rendered, end="")


if __name__ == "__main__":
    main()
