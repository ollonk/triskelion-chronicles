#!/usr/bin/env python3
"""Export project dialogue strings to an editable TSV."""

from __future__ import annotations

import argparse
from pathlib import Path

from dialogue_common import DEFAULT_EXCLUDES, DEFAULT_INCLUDES, TSV_COLUMNS, parse_repo, write_tsv


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Export Triskelion dialogue strings to TSV.")
    parser.add_argument("--output", default="dialogue/dialogue.tsv", help="TSV path to write.")
    parser.add_argument("--include", action="append", default=[], help="Glob to include. May be passed more than once.")
    parser.add_argument("--exclude", action="append", default=[], help="Glob to exclude. May be passed more than once.")
    parser.add_argument("--only-annotated", action="store_true", help="Export only labels with @dialogue annotations.")
    parser.add_argument("--verbose", action="store_true", help="Print parse warnings.")
    return parser


def main() -> int:
    args = build_parser().parse_args()
    repo_root = Path.cwd()
    includes = args.include or DEFAULT_INCLUDES
    excludes = DEFAULT_EXCLUDES + args.exclude
    output = repo_root / args.output

    result = parse_repo(repo_root, includes, excludes, only_annotated=args.only_annotated)
    rows = [entry.to_row() for entry in result.entries]
    write_tsv(output, rows)

    annotated = sum(1 for entry in result.entries if entry.annotated)
    report_lines = [
        "Dialogue export report",
        "======================",
        f"Files scanned: {result.files_scanned}",
        f"Dialogue entries exported: {len(result.entries)}",
        f"Annotated entries: {annotated}",
        f"Entries missing annotations: {len(result.entries) - annotated}",
        f"Warnings: {len(result.warnings)}",
        "",
        "Columns:",
        "\t".join(TSV_COLUMNS),
    ]
    if result.warnings:
        report_lines.extend(["", "Parse warnings:"])
        report_lines.extend(result.warnings)
    report_path = output.with_name("dialogue_export_report.txt")
    report_path.write_text("\n".join(report_lines) + "\n", encoding="utf-8")

    print(f"Scanned {result.files_scanned} files.")
    print(f"Exported {len(result.entries)} dialogue entries to {output.relative_to(repo_root)}.")
    print(f"Annotated: {annotated}; missing annotations: {len(result.entries) - annotated}.")
    print(f"Report written to {report_path.relative_to(repo_root)}.")
    if args.verbose and result.warnings:
        for warning in result.warnings:
            print(f"warning: {warning}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
