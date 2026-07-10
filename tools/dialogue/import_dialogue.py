#!/usr/bin/env python3
"""Validate and import edited dialogue TSV rows back into source files."""

from __future__ import annotations

import argparse
from collections import defaultdict
from pathlib import Path
from typing import Dict, List, Tuple

from dialogue_common import (
    DEFAULT_EXCLUDES,
    DEFAULT_INCLUDES,
    DialogueEntry,
    normalize_tsv_text,
    parse_dialogue_file,
    parse_repo,
    read_tsv,
    render_string_block,
    text_hash,
    validate_rows,
    write_text_atomically,
)


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Import edited dialogue TSV rows into source files.")
    parser.add_argument("tsv", nargs="?", default="dialogue/dialogue.tsv", help="Dialogue TSV to import.")
    parser.add_argument("--write", action="store_true", help="Write source changes. Default is dry-run.")
    parser.add_argument("--force", action="store_true", help="Allow stale original_hash rows to overwrite current source.")
    parser.add_argument("--backup", action="store_true", help="Create .bak backups before writing files.")
    parser.add_argument("--validate-only", action="store_true", help="Validate TSV/source references without planning writes.")
    parser.add_argument("--only-id", action="append", default=[], help="Only process a dialogue_id. May be passed more than once.")
    parser.add_argument("--only-file", action="append", default=[], help="Only process a source_file. May be passed more than once.")
    parser.add_argument("--line-length", type=int, default=32, help="Visible line length warning threshold.")
    parser.add_argument("--verbose", action="store_true", help="Print skipped unchanged rows.")
    return parser


def index_entries(entries: List[DialogueEntry]) -> Dict[Tuple[str, str], DialogueEntry]:
    return {(entry.source_file, entry.label): entry for entry in entries}


def load_referenced_entries(repo_root: Path, rows: List[Dict[str, str]]) -> Dict[Tuple[str, str], DialogueEntry]:
    by_file = sorted({row["source_file"] for row in rows if row.get("source_file")})
    entries: Dict[Tuple[str, str], DialogueEntry] = {}
    for rel_file in by_file:
        path = repo_root / rel_file
        if not path.exists():
            continue
        result = parse_dialogue_file(path, repo_root)
        entries.update(index_entries(result.entries))
    return entries


def filter_rows(rows: List[Dict[str, str]], only_ids: List[str], only_files: List[str]) -> List[Dict[str, str]]:
    result = []
    only_id_set = set(only_ids)
    only_file_set = set(only_files)
    for row in rows:
        if only_id_set and row.get("dialogue_id") not in only_id_set:
            continue
        if only_file_set and row.get("source_file") not in only_file_set:
            continue
        result.append(row)
    return result


def main() -> int:
    args = build_parser().parse_args()
    repo_root = Path.cwd()
    rows = read_tsv(repo_root / args.tsv)
    rows = filter_rows(rows, args.only_id, args.only_file)

    source_entries = load_referenced_entries(repo_root, rows)
    validation = validate_rows(rows, source_entries, line_length=args.line_length)
    errors = [message for message in validation if message.level == "error"]
    warnings = [message for message in validation if message.level == "warning"]
    if args.validate_only or args.verbose:
        for message in validation:
            print(message.format())
    else:
        for message in errors:
            print(message.format())
        if warnings:
            print(f"Validation warnings: {len(warnings)}; rerun with --validate-only or --verbose to list them.")
    if args.validate_only:
        print(f"Validated {len(rows)} row(s): {len(errors)} error(s), {len(warnings)} warning(s).")
        return 1 if errors else 0
    if errors:
        print(f"Aborting: validation found {len(errors)} error(s).")
        return 1

    changes_by_file: Dict[str, List[Tuple[DialogueEntry, str, str]]] = defaultdict(list)
    conflicts = 0
    skipped = 0
    for row in rows:
        source_file = row["source_file"]
        label = row["label"]
        entry = source_entries.get((source_file, label))
        if entry is None:
            skipped += 1
            continue
        new_text = normalize_tsv_text(row.get("text", ""))
        if text_hash(new_text) == row.get("original_hash", ""):
            skipped += 1
            if args.verbose:
                print(f"unchanged: {row['dialogue_id']}")
            continue
        if entry.original_hash != row.get("original_hash", "") and not args.force:
            print(f"CONFLICT: {row['dialogue_id']}: source text changed since export; use --force to overwrite.")
            conflicts += 1
            continue
        if new_text == entry.text:
            skipped += 1
            continue
        changes_by_file[source_file].append((entry, row["dialogue_id"], new_text))

    planned = sum(len(items) for items in changes_by_file.values())
    if conflicts:
        print(f"Aborting: {conflicts} conflict(s) detected.")
        return 1
    if planned == 0:
        print(f"No source changes needed. Skipped {skipped} unchanged row(s).")
        return 0

    mode = "write" if args.write else "dry-run"
    print(f"Import {mode}: {planned} change(s) across {len(changes_by_file)} file(s).")
    if args.write and not args.backup:
        print("No backups requested; rely on Git or rerun with --backup to create .bak files.")

    for source_file, changes in sorted(changes_by_file.items()):
        print(f"{source_file}: {len(changes)} change(s)")
        if not args.write:
            for _, dialogue_id, _ in changes:
                print(f"  would update {dialogue_id}")
            continue

        path = repo_root / source_file
        lines = path.read_text(encoding="utf-8").splitlines(keepends=True)
        replacement_by_start = {
            entry.string_start_line: (entry, new_text)
            for entry, _, new_text in sorted(changes, key=lambda item: item[0].string_start_line)
        }
        new_lines: List[str] = []
        line_number = 1
        while line_number <= len(lines):
            replacement = replacement_by_start.get(line_number)
            if replacement is None:
                new_lines.append(lines[line_number - 1])
                line_number += 1
                continue
            entry, new_text = replacement
            new_lines.extend(render_string_block(new_text, entry.indent))
            line_number = entry.string_end_line + 1
        write_text_atomically(path, "".join(new_lines), backup=args.backup)

    print("Applied changes." if args.write else "Dry-run complete; rerun with --write to modify files.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
