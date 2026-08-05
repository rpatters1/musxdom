#!/usr/bin/env python3
"""Correlate numeric binary record codes with Finale 27 XML element counts.

Count equality is evidence of identity, not proof: conversion may synthesize or
discard records, and unrelated types can coincide in small documents.  Output
therefore includes sample size, Pearson correlation, exact-count rate, and the
number of files in which both sides are nonzero.
"""

from __future__ import annotations

import argparse
import csv
import json
import math
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from structure_probe import parse_zero_trailed_records, zlib_members  # noqa: E402


def pearson(left: list[int], right: list[int]) -> float:
    n = len(left)
    if n < 3:
        return 0.0
    ml = sum(left) / n
    mr = sum(right) / n
    dl = sum((value - ml) ** 2 for value in left)
    dr = sum((value - mr) ** 2 for value in right)
    if not dl or not dr:
        return 0.0
    return sum((a - ml) * (b - mr) for a, b in zip(left, right)) / math.sqrt(dl * dr)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("inventory_csv", type=Path)
    parser.add_argument("semantics_csv", type=Path)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    with args.inventory_csv.open(newline="", encoding="utf-8") as handle:
        inventory = list(csv.DictReader(handle))
    with args.semantics_csv.open(newline="", encoding="utf-8") as handle:
        semantics = {row["source_relative"]: row for row in csv.DictReader(handle) if row["status"] == "ok"}

    # pool -> list of (source, binary code counts, XML tag counts)
    samples: dict[str, list[tuple[str, Counter[int], dict[str, int]]]] = defaultdict(list)
    for item in inventory:
        semantic = semantics.get(item["source_relative"])
        if semantic is None:
            continue
        data = Path(item["source_path"]).read_bytes()
        members = zlib_members(data)
        for member_index, pool in ((0, "others"), (1, "details")):
            if member_index >= len(members):
                continue
            parsed = parse_zero_trailed_records(members[member_index][2])
            if parsed is None:
                continue
            records = parsed[3]
            if pool == "others":
                records = [record for record in records if int(record["field1"]) != 0xFFFE]
            binary_counts = Counter(int(record["field0"]) for record in records)
            xml_counts = json.loads(semantic[f"{pool}_types"])
            samples[pool].append((item["source_relative"], binary_counts, xml_counts))

    rows: list[dict[str, object]] = []
    for pool, pool_samples in sorted(samples.items()):
        codes = sorted({code for _, counts, _ in pool_samples for code in counts})
        tags = sorted({tag for _, _, counts in pool_samples for tag in counts})
        for code in codes:
            left = [counts.get(code, 0) for _, counts, _ in pool_samples]
            candidates = []
            for tag in tags:
                right = [counts.get(tag, 0) for _, _, counts in pool_samples]
                both = sum(a > 0 and b > 0 for a, b in zip(left, right))
                if both < 3:
                    continue
                corr = pearson(left, right)
                exact = sum(a == b for a, b in zip(left, right)) / len(left)
                nonzero_exact = sum(a == b and a > 0 for a, b in zip(left, right))
                score = corr + exact + min(nonzero_exact, 20) / 20
                candidates.append((score, tag, corr, exact, both, nonzero_exact))
            for rank, candidate in enumerate(sorted(candidates, reverse=True)[:3], 1):
                score, tag, corr, exact, both, nonzero_exact = candidate
                rows.append({
                    "pool": pool,
                    "field0_hex": f"0x{code:04x}",
                    "candidate_rank": rank,
                    "xml_tag": tag,
                    "files_tested": len(pool_samples),
                    "both_nonzero": both,
                    "nonzero_exact": nonzero_exact,
                    "all_file_exact_rate": f"{exact:.4f}",
                    "pearson": f"{corr:.6f}",
                    "score": f"{score:.6f}",
                })
    args.output.parent.mkdir(parents=True, exist_ok=True)
    fields = list(rows[0]) if rows else []
    with args.output.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=fields)
        writer.writeheader()
        writer.writerows(rows)


if __name__ == "__main__":
    main()
