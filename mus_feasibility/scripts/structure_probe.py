#!/usr/bin/env python3
"""Probe wrappers, zlib members, and a candidate record frame.

The record parser is deliberately conditional.  It recognizes a block only if
the proposed frame consumes the complete decompressed member and every record's
four-byte trailer is zero.  It does not assign semantic names to numeric fields.
"""

from __future__ import annotations

import argparse
import binascii
import csv
import json
import math
import struct
import zlib
from collections import Counter, defaultdict
from pathlib import Path


ZLIB_HEADERS = {b"\x78\x01", b"\x78\x5e", b"\x78\x9c", b"\x78\xda"}


def entropy(data: bytes) -> float:
    if not data:
        return 0.0
    counts = Counter(data)
    size = len(data)
    return -sum((count / size) * math.log2(count / size) for count in counts.values())


def zlib_members(data: bytes, start: int = 0x200) -> list[tuple[int, int, bytes]]:
    order = wrapper_endian(data)
    if order != "none":
        prefix = ">" if order == "big" else "<"
        results: list[tuple[int, int, bytes]] = []
        offset = start
        while offset + 6 <= len(data):
            _, block_size = struct.unpack_from(prefix + "HI", data, offset)
            if block_size == 6:  # terminal marker without CRC or payload
                offset += 6
                continue
            if block_size < 10 or offset + block_size > len(data):
                break
            checksum = struct.unpack_from(prefix + "I", data, offset + 6)[0]
            payload = data[offset + 10 : offset + block_size]
            try:
                decoded = zlib.decompress(payload)
            except zlib.error:
                break
            if (binascii.crc32(decoded) & 0xFFFFFFFF) != checksum:
                break
            results.append((offset + 10, len(payload), decoded))
            offset += block_size
        return results

    results: list[tuple[int, int, bytes]] = []
    offset = start
    while offset + 2 <= len(data):
        if data[offset : offset + 2] not in ZLIB_HEADERS:
            offset += 1
            continue
        try:
            decoder = zlib.decompressobj()
            decoded = decoder.decompress(data[offset:])
        except zlib.error:
            offset += 1
            continue
        if not decoder.eof:
            offset += 1
            continue
        consumed = len(data[offset:]) - len(decoder.unused_data)
        results.append((offset, consumed, decoded))
        offset += consumed
    return results


def wrapper_endian(data: bytes) -> str:
    """Return byte order when the first typed block validates by length and CRC."""
    for name, prefix in (("big", ">"), ("little", "<")):
        if len(data) < 0x20A:
            continue
        block_type, block_size, checksum = struct.unpack_from(prefix + "HII", data, 0x200)
        if block_type != 0x1A or block_size < 10 or 0x200 + block_size > len(data):
            continue
        try:
            decoded = zlib.decompress(data[0x20A : 0x200 + block_size])
        except zlib.error:
            continue
        if (binascii.crc32(decoded) & 0xFFFFFFFF) == checksum:
            return name
    return "none"


def parse_zero_trailed_records(data: bytes) -> tuple[str, int, int, list[dict[str, object]]] | None:
    for byte_order, prefix in (("big", ">"), ("little", "<")):
        for header_words in (5, 4):
            for payload_adjustment in (0, 2):
                records: list[dict[str, object]] = []
                offset = 0
                while offset < len(data):
                    header_bytes = header_words * 2
                    if offset + header_bytes + 4 > len(data):
                        break
                    fields = struct.unpack_from(prefix + f"{header_words}H", data, offset)
                    payload_bytes = fields[-1]
                    payload_end = offset + header_bytes + payload_bytes + payload_adjustment
                    end = payload_end + 4
                    if end > len(data) or data[payload_end:end] != b"\x00\x00\x00\x00":
                        break
                    records.append({
                        "offset": offset,
                        "field0": fields[0],
                        "field1": fields[1],
                        "field2": fields[2],
                        "field3": fields[3] if header_words == 5 else None,
                        "payload_bytes": payload_bytes,
                    })
                    offset = end
                if records and offset == len(data):
                    return byte_order, header_words, payload_adjustment, records
    return None


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("inventory_csv", type=Path)
    parser.add_argument("--output-dir", type=Path, required=True)
    args = parser.parse_args()
    output_dir = args.output_dir.resolve()
    output_dir.mkdir(parents=True, exist_ok=True)
    with args.inventory_csv.open(newline="", encoding="utf-8") as handle:
        inventory = list(csv.DictReader(handle))

    rows: list[dict[str, object]] = []
    catalog: dict[tuple[object, ...], dict[str, object]] = {}
    version_stats: dict[str, Counter[str]] = defaultdict(Counter)
    for item in inventory:
        path = Path(item["source_path"])
        data = path.read_bytes()
        members = zlib_members(data)
        parsed_members = 0
        parsed_records = 0
        decoded_bytes = 0
        member_descriptions: list[str] = []
        for member_index, (file_offset, consumed, decoded) in enumerate(members):
            decoded_bytes += len(decoded)
            parsed = parse_zero_trailed_records(decoded)
            if parsed is not None:
                byte_order, header_words, payload_adjustment, records = parsed
                parsed_members += 1
                parsed_records += len(records)
                for record in records:
                    key = (item["saving_product"], byte_order, header_words, payload_adjustment, int(record["field0"]))
                    entry = catalog.setdefault(key, {
                        "saving_product": item["saving_product"],
                        "byte_order": byte_order,
                        "header_words": header_words,
                        "payload_adjustment": payload_adjustment,
                        "field0_hex": f"0x{int(record['field0']):04x}",
                        "count": 0,
                        "payload_sizes": Counter(),
                        "field1_values": Counter(),
                        "field2_values": Counter(),
                        "field3_values": Counter(),
                        "example_source": item["source_relative"],
                        "example_member": member_index,
                        "example_decoded_offset": f"0x{int(record['offset']):x}",
                    })
                    entry["count"] = int(entry["count"]) + 1
                    entry["payload_sizes"][int(record["payload_bytes"])] += 1
                    entry["field1_values"][int(record["field1"])] += 1
                    entry["field2_values"][int(record["field2"])] += 1
                    entry["field3_values"][record["field3"]] += 1
            member_descriptions.append(
                f"0x{file_offset:x}:{consumed}->{len(decoded)}"
                + (f":records={len(parsed[3])}:{parsed[0]}:header_words={parsed[1]}:payload_adjustment={parsed[2]}" if parsed else "")
            )
        if members:
            wrapper = "zlib-members"
        elif data[0x200:0x202] in (b"\x00\x01", b"\x00\x0f"):
            wrapper = "non-zlib-low-entropy"
        else:
            wrapper = "non-zlib-other"
        body_sample = data[0x200 : min(len(data), 0x4200)]
        row = {
            "source_relative": item["source_relative"],
            "saving_product": item["saving_product"],
            "file_size": len(data),
            "body_prefix": data[0x200:0x210].hex(),
            "body_sample_entropy": f"{entropy(body_sample):.4f}",
            "wrapper_class": wrapper,
            "validated_wrapper_endian": wrapper_endian(data),
            "zlib_members": len(members),
            "zlib_member_map": ";".join(member_descriptions),
            "decoded_bytes": decoded_bytes,
            "framed_members": parsed_members,
            "framed_records": parsed_records,
        }
        rows.append(row)
        stats = version_stats[item["saving_product"]]
        stats["files"] += 1
        stats[wrapper] += 1
        stats["zlib_members"] += len(members)
        stats["framed_members"] += parsed_members
        stats["framed_records"] += parsed_records

    fields = list(rows[0]) if rows else []
    with (output_dir / "structure_probe.csv").open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=fields)
        writer.writeheader()
        writer.writerows(rows)

    catalog_rows: list[dict[str, object]] = []
    for key in sorted(catalog):
        entry = catalog[key]
        catalog_rows.append({
            "saving_product": entry["saving_product"],
            "byte_order": entry["byte_order"],
            "header_words": entry["header_words"],
            "payload_adjustment": entry["payload_adjustment"],
            "field0_hex": entry["field0_hex"],
            "count": entry["count"],
            "payload_sizes": json.dumps(dict(sorted(entry["payload_sizes"].items()))),
            "field1_top": json.dumps(entry["field1_values"].most_common(8)),
            "field2_top": json.dumps(entry["field2_values"].most_common(8)),
            "field3_top": json.dumps(entry["field3_values"].most_common(8)),
            "example_source": entry["example_source"],
            "example_member": entry["example_member"],
            "example_decoded_offset": entry["example_decoded_offset"],
        })
    catalog_fields = list(catalog_rows[0]) if catalog_rows else []
    with (output_dir / "record_catalog.csv").open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=catalog_fields)
        writer.writeheader()
        writer.writerows(catalog_rows)

    version_rows = []
    for product, stats in sorted(version_stats.items()):
        version_rows.append({"saving_product": product, **dict(stats)})
    all_keys = ["saving_product", "files", "zlib-members", "non-zlib-low-entropy", "non-zlib-other", "zlib_members", "framed_members", "framed_records"]
    with (output_dir / "version_structure_summary.csv").open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=all_keys)
        writer.writeheader()
        for row in version_rows:
            writer.writerow({key: row.get(key, 0) for key in all_keys})


if __name__ == "__main__":
    main()
