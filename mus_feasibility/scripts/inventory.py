#!/usr/bin/env python3
"""Inventory legacy Finale files without modifying the source corpus.

Matching assumption (reported, not hidden): a source ``name.mus`` normally maps
to ``source-parent/-exports/name.fin27.musx``.  A normalized, case-insensitive
basename search is used only when that exact convention does not match.
"""

from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
import unicodedata
from collections import Counter
from datetime import datetime
from pathlib import Path


BANNER_RE = re.compile(rb"Finale\(R\) ([^\x00]{1,72}?)(?: Copyright| File Converter)")


def norm(value: str) -> str:
    return unicodedata.normalize("NFC", value).casefold()


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def parse_banner(header: bytes) -> tuple[str, str]:
    raw = header[0x20:0x60].split(b"\x00", 1)[0].decode("mac_roman", "replace")
    match = BANNER_RE.search(header[:0x80])
    product = match.group(1).decode("ascii", "replace") if match else "unknown"
    return raw, product


def find_export(source: Path, exports_by_stem: dict[str, list[Path]]) -> tuple[Path | None, str]:
    expected = source.parent / "-exports" / f"{source.stem}.fin27.musx"
    if expected.is_file():
        return expected, "adjacent-exact"
    candidates = exports_by_stem.get(norm(source.stem), [])
    if not candidates:
        return None, "missing"
    # Prefer the candidate with the longest common parent prefix.  Ambiguous
    # fallback matches remain visibly labelled and are not treated as exact.
    source_parts = source.parent.parts
    ranked: list[tuple[int, str, Path]] = []
    for candidate in candidates:
        common = 0
        for left, right in zip(source_parts, candidate.parent.parent.parts):
            if norm(left) != norm(right):
                break
            common += 1
        ranked.append((common, str(candidate), candidate))
    ranked.sort(key=lambda item: (-item[0], item[1]))
    best = ranked[0]
    ties = [item for item in ranked if item[0] == best[0]]
    return best[2], "fallback-unique" if len(ties) == 1 else "fallback-ambiguous"


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("root", type=Path, help="Corpus root")
    parser.add_argument("--output-dir", type=Path, required=True)
    args = parser.parse_args()
    root = args.root.resolve()
    output_dir = args.output_dir.resolve()
    output_dir.mkdir(parents=True, exist_ok=True)

    sources = sorted(
        (path for path in root.rglob("*") if path.is_file() and path.suffix.casefold() == ".mus"),
        key=lambda path: norm(str(path.relative_to(root))),
    )
    exports = sorted(root.rglob("*.fin27.musx"), key=lambda path: norm(str(path.relative_to(root))))
    exports_by_stem: dict[str, list[Path]] = {}
    for export in exports:
        stem = export.name[: -len(".fin27.musx")]
        exports_by_stem.setdefault(norm(stem), []).append(export)

    rows: list[dict[str, object]] = []
    export_hash_cache: dict[Path, str] = {}
    for index, source in enumerate(sources, 1):
        with source.open("rb") as handle:
            header = handle.read(0x200)
        banner, product = parse_banner(header)
        export, match_kind = find_export(source, exports_by_stem)
        stat = source.stat()
        row: dict[str, object] = {
            "index": index,
            "source_relative": str(source.relative_to(root)),
            "source_path": str(source),
            "source_size": stat.st_size,
            "source_sha256": sha256(source),
            "source_mtime": datetime.fromtimestamp(stat.st_mtime).astimezone().isoformat(),
            "signature_ok": header.startswith(b"ENIGMA BINARY FILE\x00"),
            "banner": banner,
            "saving_product": product,
            "header_0x60_0xb0": header[0x60:0xB0].hex(),
            "export_match": match_kind,
            "export_relative": str(export.relative_to(root)) if export else "",
            "export_path": str(export) if export else "",
            "export_size": export.stat().st_size if export else "",
            "export_sha256": "",
        }
        if export:
            if export not in export_hash_cache:
                export_hash_cache[export] = sha256(export)
            row["export_sha256"] = export_hash_cache[export]
        rows.append(row)

    fieldnames = list(rows[0]) if rows else []
    with (output_dir / "corpus_inventory.csv").open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(rows)

    products = Counter(str(row["saving_product"]) for row in rows)
    matches = Counter(str(row["export_match"]) for row in rows)
    summary = {
        "corpus_root": str(root),
        "source_count": len(sources),
        "export_count": len(exports),
        "matched_source_count": sum(bool(row["export_path"]) for row in rows),
        "unique_matched_export_count": len(export_hash_cache),
        "source_bytes": sum(int(row["source_size"]) for row in rows),
        "matched_export_bytes": sum(path.stat().st_size for path in export_hash_cache),
        "saving_product_counts": dict(sorted(products.items())),
        "match_counts": dict(sorted(matches.items())),
    }
    (output_dir / "inventory_summary.json").write_text(
        json.dumps(summary, indent=2, ensure_ascii=False) + "\n", encoding="utf-8"
    )

    markdown = [
        "# Corpus Inventory",
        "",
        f"Generated from `{root}` by `scripts/inventory.py`.",
        "",
        f"- Legacy `.mus` files: **{len(sources)}**",
        f"- `.fin27.musx` files present: **{len(exports)}**",
        f"- Legacy sources matched to an export: **{summary['matched_source_count']}**",
        "",
        "The saving product is read from the binary header. Timestamps and path names are supporting provenance only. "
        "SHA-256 values cover the complete data fork. Full machine-readable paths and header bytes are in "
        "`private/corpus_inventory.csv` (local-only).",
        "",
        "## Saving-product distribution",
        "",
        "| Header product | Files |",
        "|---|---:|",
    ]
    markdown.extend(f"| {key} | {value} |" for key, value in sorted(products.items()))
    markdown.extend([
        "",
        "## Files examined",
        "",
        "| # | Source (relative to corpus root) | Bytes | SHA-256 | Header product | Export | Match |",
        "|---:|---|---:|---|---|---|---|",
    ])
    for row in rows:
        source_rel = str(row["source_relative"]).replace("|", "\\|")
        export_rel = str(row["export_relative"]).replace("|", "\\|") or "—"
        markdown.append(
            f"| {row['index']} | `{source_rel}` | {row['source_size']} | `{row['source_sha256']}` | "
            f"{row['saving_product']} | `{export_rel}` | {row['export_match']} |"
        )
    (output_dir.parent / "CORPUS_INVENTORY.md").write_text("\n".join(markdown) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
