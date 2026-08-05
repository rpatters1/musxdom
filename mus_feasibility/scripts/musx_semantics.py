#!/usr/bin/env python3
"""Summarize Finale 27 MUSX exports as semantic references.

The score.dat recoding algorithm is the public, symmetric algorithm documented
by the MIT-licensed denigma project (``src/score_encoder/score_encoder.h``).
This script keeps decoded XML in memory and does not modify the evidence corpus.
"""

from __future__ import annotations

import argparse
import csv
import gzip
import json
import zipfile
from collections import Counter
from pathlib import Path
from xml.etree import ElementTree as ET


INITIAL_STATE = 0x28006D45
RESET_LIMIT = 0x20000


def local_name(tag: str) -> str:
    return tag.rsplit("}", 1)[-1]


def decode_score_dat(data: bytes) -> bytes:
    result = bytearray(data)
    state = INITIAL_STATE
    for index in range(len(result)):
        if index % RESET_LIMIT == 0:
            state = INITIAL_STATE
        state = (state * 0x41C64E6D + 0x3039) & 0xFFFFFFFF
        upper = state >> 16
        result[index] ^= (upper + upper // 255) & 0xFF
    return gzip.decompress(result)


def child(parent: ET.Element | None, name: str) -> ET.Element | None:
    if parent is None:
        return None
    return next((element for element in parent if local_name(element.tag) == name), None)


def text(parent: ET.Element | None, name: str) -> str:
    element = child(parent, name)
    return (element.text or "") if element is not None else ""


def version(element: ET.Element | None) -> str:
    if element is None:
        return ""
    parts = [text(element, name) for name in ("major", "minor", "maint", "build")]
    while parts and not parts[-1]:
        parts.pop()
    return ".".join(part or "0" for part in parts)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("inventory_csv", type=Path)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    with args.inventory_csv.open(newline="", encoding="utf-8") as handle:
        inventory = list(csv.DictReader(handle))

    rows: list[dict[str, object]] = []
    for item in inventory:
        if not item["export_path"]:
            continue
        export = Path(item["export_path"])
        status = "ok"
        error = ""
        pools: dict[str, Counter[str]] = {}
        try:
            with zipfile.ZipFile(export) as archive:
                xml_data = decode_score_dat(archive.read("score.dat"))
            root = ET.fromstring(xml_data)
            root_children = {local_name(element.tag): element for element in root}
            for pool_name in ("options", "others", "details", "entries", "texts"):
                pool = root_children.get(pool_name)
                pools[pool_name] = Counter(local_name(element.tag) for element in pool) if pool is not None else Counter()
            header_data = child(root_children.get("header"), "headerData")
            created = child(header_data, "created")
            modified = child(header_data, "modified")
            created_date = "-".join(text(created, name) for name in ("year", "month", "day"))
            modified_date = "-".join(text(modified, name) for name in ("year", "month", "day"))
            created_enigma = version(child(created, "enigmaVersion"))
            created_app = version(child(created, "appVersion"))
            created_file = version(child(created, "fileVersion"))
            created_platform = text(created, "platform")
            part_defs = pools.get("others", Counter()).get("partDef", 0)
            part_ids = []
            others = root_children.get("others")
            if others is not None:
                part_ids = [element.attrib.get("cmper", "") for element in others if local_name(element.tag) == "partDef"]
            all_names = Counter(local_name(element.tag) for element in root.iter())
            sharing_names = {name: count for name, count in all_names.items() if "share" in name.casefold() or "linked" in name.casefold()}
            part_scoped = Counter()
            shared_values = Counter()
            for element in root.iter():
                if "part" in element.attrib:
                    part_scoped[local_name(element.tag)] += 1
                if "shared" in element.attrib:
                    shared_values[element.attrib["shared"]] += 1
        except Exception as exc:  # preserve corpus-level failures for investigation
            status = "error"
            error = f"{type(exc).__name__}: {exc}"
            created_date = modified_date = created_enigma = created_app = created_file = created_platform = ""
            part_defs = 0
            part_ids = []
            sharing_names = {}
            part_scoped = Counter()
            shared_values = Counter()
        row: dict[str, object] = {
            "source_relative": item["source_relative"],
            "saving_product": item["saving_product"],
            "export_relative": item["export_relative"],
            "status": status,
            "error": error,
            "created_date": created_date,
            "created_enigma_version": created_enigma,
            "created_app_version": created_app,
            "created_file_version": created_file,
            "created_platform": created_platform,
            "part_def_count": part_defs,
            "part_ids": json.dumps(part_ids),
            "sharing_named_elements": json.dumps(sharing_names, sort_keys=True),
            "part_scoped_count": sum(part_scoped.values()),
            "part_scoped_types": json.dumps(dict(sorted(part_scoped.items())), separators=(",", ":")),
            "shared_attribute_values": json.dumps(dict(sorted(shared_values.items()))),
        }
        for pool_name in ("options", "others", "details", "entries", "texts"):
            counts = pools.get(pool_name, Counter())
            row[f"{pool_name}_count"] = sum(counts.values())
            row[f"{pool_name}_types"] = json.dumps(dict(sorted(counts.items())), separators=(",", ":"))
        rows.append(row)

    args.output.parent.mkdir(parents=True, exist_ok=True)
    fields = list(rows[0]) if rows else []
    with args.output.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=fields)
        writer.writeheader()
        writer.writerows(rows)


if __name__ == "__main__":
    main()
