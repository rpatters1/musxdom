#!/usr/bin/env python3
"""Inventory Finale-like members in ZIP and StuffIt archives without changing the corpus.

Only archive basename and content-derived IDs are written to the public output;
the original archive path is intentionally not emitted.
"""

from __future__ import annotations

import argparse
import csv
import hashlib
import os
import re
import subprocess
import tempfile
import zipfile
from pathlib import Path

from structure_probe import entropy, parse_zero_trailed_records, wrapper_endian, zlib_members


PRODUCT_RE = re.compile(rb"Finale\((?:R|TM)\)\s+([^\x00\r\n ]+)")


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def likely_member(name: str) -> bool:
    base = name.rsplit("/", 1)[-1]
    if not base or base.startswith("._") or "/__MACOSX/" in f"/{name}":
        return False
    suffix = Path(base).suffix.lower()
    return suffix == ".mus" or not suffix


def add_member(rows: list[dict[str, str | int]], archive: Path, archive_id: str, member_path: str, data: bytes) -> None:
    match = PRODUCT_RE.search(data[:256])
    members = zlib_members(data)
    framed_records = sum(len(parsed[3]) for _, _, decoded in members if (parsed := parse_zero_trailed_records(decoded)) is not None)
    body = data[0x200 : min(len(data), 0x4200)]
    rows.append({
        "archive_id": archive_id,
        "archive_filename": archive.name,
        "member_filename": member_path.rsplit("/", 1)[-1],
        "member_id": "mus-" + digest(data)[:16],
        "member_size": len(data),
        "member_sha256": digest(data),
        "is_enigma_binary": str(data.startswith(b"ENIGMA BINARY FILE")),
        "saving_product": match.group(1).decode("latin1") if match else "unknown",
        "member_name_has_extension": str(bool(Path(member_path).suffix)),
        "body_prefix": data[0x200:0x210].hex(),
        "body_entropy": f"{entropy(body):.4f}",
        "wrapper_endian": wrapper_endian(data),
        "zlib_members": len(members),
        "framed_records": framed_records,
        "_archive_path": str(archive),
        "_member_path": member_path,
    })


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("root", type=Path)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--summary", type=Path, required=True)
    parser.add_argument("--private-output", type=Path)
    args = parser.parse_args()

    rows: list[dict[str, str | int]] = []
    archive_count = 0
    unsupported: dict[str, int] = {}
    for directory, _, names in os.walk(args.root):
        for name in names:
            archive = Path(directory) / name
            if archive.suffix.lower() not in {".zip", ".sit"}:
                continue
            try:
                archive_id = "arc-" + digest(archive.read_bytes())[:16]
                if archive.suffix.lower() == ".zip":
                    zf = zipfile.ZipFile(archive)
                    archive_count += 1
                    for info in zf.infolist():
                        if info.is_dir() or not likely_member(info.filename):
                            continue
                        add_member(rows, archive, archive_id, info.filename, zf.read(info))
                    zf.close()
                else:
                    archive_count += 1
                    with tempfile.TemporaryDirectory(prefix="mus_sit_") as temp:
                        result = subprocess.run(["unar", "-quiet", "-p", "", "-o", temp, str(archive)],
                                                capture_output=True, text=True, check=False)
                        if result.returncode != 0:
                            raise RuntimeError(result.stderr.strip() or "unar failed")
                        extracted_root = Path(temp) / archive.stem
                        if not extracted_root.exists():
                            extracted_root = Path(temp)
                        for extracted in extracted_root.rglob("*"):
                            if not extracted.is_file() or not likely_member(extracted.name):
                                continue
                            add_member(rows, archive, archive_id, str(extracted.relative_to(extracted_root)), extracted.read_bytes())
            except (zipfile.BadZipFile, OSError, RuntimeError) as exc:
                unsupported[archive.suffix.lower() or "<none>"] = unsupported.get(archive.suffix.lower() or "<none>", 0) + 1

    fields = ["archive_id", "archive_filename", "member_filename", "member_id", "member_size", "member_sha256",
              "is_enigma_binary", "saving_product", "member_name_has_extension", "body_prefix", "body_entropy",
              "wrapper_endian", "zlib_members", "framed_records"]
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=fields)
        writer.writeheader()
        writer.writerows({field: row[field] for field in fields} for row in rows)
    if args.private_output:
        args.private_output.parent.mkdir(parents=True, exist_ok=True)
        private_fields = ["archive_id", "archive_path", "member_id", "member_path"]
        with args.private_output.open("w", newline="", encoding="utf-8") as handle:
            writer = csv.DictWriter(handle, fieldnames=private_fields)
            writer.writeheader()
            for row in rows:
                writer.writerow({"archive_id": row["archive_id"], "archive_path": row["_archive_path"],
                                 "member_id": row["member_id"], "member_path": row["_member_path"]})
    args.summary.write_text(
        "# Archive Survey\n\n"
        f"Scanned {archive_count} ZIP/StuffIt archives and found {len(rows)} candidate `.mus` or extensionless members. "
        "Archive paths are intentionally omitted; use the local ignored `mus_feasibility/private/archive_locations.csv` "
        "mapping for archive/member locations. The `unar`/`lsar` 1.10.7 tools can process StuffIt 5 archives, "
        "including resource forks; a complete StuffIt pass may take substantially longer than the ZIP pass.\n\n"
        f"Enigma-banner members: {sum(r['is_enigma_binary'] == 'True' for r in rows)}.\n"
        f"Extensionless candidates: {sum(r['member_name_has_extension'] == 'False' for r in rows)}.\n",
        encoding="utf-8",
    )


if __name__ == "__main__":
    main()
