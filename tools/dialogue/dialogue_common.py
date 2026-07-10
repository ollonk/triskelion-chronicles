#!/usr/bin/env python3
"""Shared helpers for Triskelion dialogue export/import tools."""

from __future__ import annotations

import csv
import fnmatch
import hashlib
import os
import re
import shutil
import tempfile
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, Iterable, List, Optional, Sequence, Tuple


TSV_COLUMNS = [
    "dialogue_id",
    "source_file",
    "label",
    "map",
    "speaker",
    "context",
    "tags",
    "text",
    "notes",
    "original_hash",
]

DEFAULT_INCLUDES = [
    "data/maps/**/*.inc",
    "data/text/**/*.inc",
    "data/**/*.s",
]

DEFAULT_EXCLUDES = [
    ".git/**",
    "build/**",
    "tools/dialogue/**",
    "dialogue/**",
    "src/**",
    "include/**",
    "graphics/**",
    "sound/**",
]

LABEL_RE = re.compile(r"^\s*(?P<label>[A-Za-z_][A-Za-z0-9_]*):(?::)?\s*(?:@.*)?$")
STRING_RE = re.compile(r"^(?P<indent>\s*)\.string\s+\"(?P<text>(?:\\.|[^\"\\])*)\"(?P<tail>.*)$")
ANNOTATION_RE = re.compile(r"(?:@|//|#|;)\s*@dialogue\b(?P<body>.*)$")
KEY_VALUE_RE = re.compile(r"(?P<key>[A-Za-z_][A-Za-z0-9_]*)=(?P<value>\"[^\"]*\"|\S+)")
TOKEN_RE = re.compile(r"\{[A-Z0-9_][A-Z0-9_ ]*(?:\s+0x[0-9A-Fa-f]+|\s+\d+)?\}")
ESCAPE_RE = re.compile(r"\\.")


@dataclass
class DialogueEntry:
    dialogue_id: str
    source_file: str
    label: str
    map: str
    speaker: str
    context: str
    tags: str
    text: str
    notes: str
    original_hash: str
    start_line: int
    end_line: int
    string_start_line: int
    string_end_line: int
    indent: str
    annotated: bool = False
    annotation: Dict[str, str] = field(default_factory=dict)

    def to_row(self) -> Dict[str, str]:
        return {column: getattr(self, column) for column in TSV_COLUMNS}


@dataclass
class ParseResult:
    entries: List[DialogueEntry]
    warnings: List[str]
    files_scanned: int = 0


@dataclass
class ValidationMessage:
    level: str
    dialogue_id: str
    source_file: str
    label: str
    message: str

    def format(self) -> str:
        location = self.dialogue_id or f"{self.source_file}::{self.label}"
        return f"{self.level.upper()}: {location}: {self.message}"


def repo_path(path: Path) -> str:
    return path.as_posix()


def text_hash(text: str) -> str:
    return hashlib.sha256(text.encode("utf-8")).hexdigest()[:16]


def strip_final_terminator(text: str) -> str:
    if text.endswith("$"):
        return text[:-1]
    return text


def ensure_final_terminator(text: str) -> str:
    return text if text.endswith("$") else text + "$"


def normalize_tsv_text(text: str) -> str:
    return text.replace("\r\n", "\\n").replace("\r", "\\n").replace("\n", "\\n")


def escape_source_string(text: str) -> str:
    return text.replace('"', r'\"')


def visible_lines(text: str) -> List[str]:
    text = strip_final_terminator(text)
    parts: List[str] = [""]
    i = 0
    while i < len(text):
        if text.startswith("\\n", i) or text.startswith("\\l", i):
            parts.append("")
            i += 2
            continue
        if text.startswith("\\p", i):
            parts.append("")
            i += 2
            continue
        if text[i] == "{" and "}" in text[i:]:
            end = text.find("}", i)
            i = end + 1
            continue
        if text[i] == "\\" and i + 1 < len(text):
            i += 2
            continue
        parts[-1] += text[i]
        i += 1
    return parts


def split_source_segments(text: str) -> List[str]:
    text = ensure_final_terminator(text)
    segments: List[str] = []
    current = ""
    i = 0
    while i < len(text):
        if text[i] == "\\" and i + 1 < len(text):
            current += text[i : i + 2]
            if text[i + 1] in {"n", "l", "p"}:
                segments.append(current)
                current = ""
            i += 2
            continue
        current += text[i]
        if text[i] == "$":
            segments.append(current)
            current = ""
        i += 1
    if current:
        segments.append(current)
    return segments or ["$"]


def render_string_block(text: str, indent: str) -> List[str]:
    return [f'{indent}.string "{escape_source_string(segment)}"\n' for segment in split_source_segments(text)]


def parse_annotation(line: str) -> Optional[Dict[str, str]]:
    match = ANNOTATION_RE.search(line)
    if not match:
        return None
    result: Dict[str, str] = {}
    for item in KEY_VALUE_RE.finditer(match.group("body")):
        value = item.group("value")
        if value.startswith('"') and value.endswith('"'):
            value = value[1:-1]
        result[item.group("key")] = value
    return result


def infer_map(path: Path) -> str:
    parts = path.parts
    if len(parts) >= 3 and parts[0] == "data" and parts[1] == "maps":
        return parts[2]
    return ""


def infer_speaker_context(label: str) -> Tuple[str, str]:
    if "_Text_" in label:
        _, suffix = label.split("_Text_", 1)
        pieces = suffix.split("_")
        if len(pieces) > 1:
            return pieces[0], "_".join(pieces[1:])
        return "", suffix
    if "Text_" in label:
        suffix = label.split("Text_", 1)[1]
        pieces = suffix.split("_")
        if len(pieces) > 1:
            return pieces[0], "_".join(pieces[1:])
        return "", suffix
    return "", ""


def make_dialogue_id(source_file: str, label: str, annotation: Dict[str, str]) -> str:
    return annotation.get("id") or f"{source_file}::{label}"


def parse_dialogue_file(path: Path, repo_root: Path) -> ParseResult:
    rel_path = repo_path(path.relative_to(repo_root))
    warnings: List[str] = []
    try:
        lines = path.read_text(encoding="utf-8").splitlines(keepends=True)
    except UnicodeDecodeError as exc:
        return ParseResult([], [f"{rel_path}: could not read as UTF-8: {exc}"], files_scanned=1)

    entries: List[DialogueEntry] = []
    pending_annotation: Optional[Dict[str, str]] = None
    pending_annotation_line = 0
    i = 0
    while i < len(lines):
        annotation = parse_annotation(lines[i])
        if annotation is not None:
            pending_annotation = annotation
            pending_annotation_line = i + 1
            i += 1
            continue

        label_match = LABEL_RE.match(lines[i])
        if not label_match:
            if lines[i].strip() and not lines[i].lstrip().startswith(("@", "//", "#", ";")):
                pending_annotation = None
            i += 1
            continue

        label = label_match.group("label")
        j = i + 1
        while j < len(lines) and not lines[j].strip():
            j += 1
        if j >= len(lines):
            pending_annotation = None
            i += 1
            continue

        first_string = STRING_RE.match(lines[j])
        if not first_string:
            pending_annotation = None
            i += 1
            continue

        chunks: List[str] = []
        string_start = j
        indent = first_string.group("indent")
        while j < len(lines):
            string_match = STRING_RE.match(lines[j])
            if not string_match:
                break
            chunks.append(string_match.group("text"))
            j += 1
            if chunks[-1].endswith("$"):
                break

        raw_text = "".join(chunks)
        editable_text = strip_final_terminator(raw_text)
        annotation = pending_annotation or {}
        speaker, context = infer_speaker_context(label)
        entry = DialogueEntry(
            dialogue_id=make_dialogue_id(rel_path, label, annotation),
            source_file=rel_path,
            label=label,
            map=annotation.get("map", infer_map(Path(rel_path))),
            speaker=annotation.get("speaker", speaker),
            context=annotation.get("context", context),
            tags=annotation.get("tags", ""),
            text=editable_text,
            notes="",
            original_hash=text_hash(editable_text),
            start_line=i + 1,
            end_line=j,
            string_start_line=string_start + 1,
            string_end_line=j,
            indent=indent,
            annotated=bool(annotation),
            annotation=annotation,
        )
        if pending_annotation and pending_annotation_line != i:
            warnings.append(f"{rel_path}:{pending_annotation_line}: @dialogue annotation is not immediately above {label}")
        entries.append(entry)
        pending_annotation = None
        i = j

    return ParseResult(entries, warnings, files_scanned=1)


def is_excluded(path: str, excludes: Sequence[str]) -> bool:
    return any(fnmatch.fnmatch(path, pattern) for pattern in excludes)


def iter_source_files(repo_root: Path, includes: Sequence[str], excludes: Sequence[str]) -> List[Path]:
    files: Dict[str, Path] = {}
    for pattern in includes:
        for path in repo_root.glob(pattern):
            if not path.is_file():
                continue
            rel = repo_path(path.relative_to(repo_root))
            if is_excluded(rel, excludes):
                continue
            files[rel] = path
    return [files[key] for key in sorted(files)]


def parse_repo(repo_root: Path, includes: Sequence[str], excludes: Sequence[str], only_annotated: bool = False) -> ParseResult:
    entries: List[DialogueEntry] = []
    warnings: List[str] = []
    files = iter_source_files(repo_root, includes, excludes)
    for path in files:
        result = parse_dialogue_file(path, repo_root)
        warnings.extend(result.warnings)
        if only_annotated:
            entries.extend(entry for entry in result.entries if entry.annotated)
        else:
            entries.extend(result.entries)
    return ParseResult(entries, warnings, files_scanned=len(files))


def read_tsv(path: Path) -> List[Dict[str, str]]:
    with path.open("r", encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        missing = [column for column in TSV_COLUMNS if column not in (reader.fieldnames or [])]
        if missing:
            raise ValueError(f"{path}: missing required TSV columns: {', '.join(missing)}")
        rows = []
        for row in reader:
            rows.append({column: row.get(column, "") for column in TSV_COLUMNS})
        return rows


def write_tsv(path: Path, rows: Iterable[Dict[str, str]]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=TSV_COLUMNS, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        for row in rows:
            writer.writerow({column: row.get(column, "") for column in TSV_COLUMNS})


def validate_rows(
    rows: Sequence[Dict[str, str]],
    source_entries: Optional[Dict[Tuple[str, str], DialogueEntry]] = None,
    line_length: int = 32,
) -> List[ValidationMessage]:
    messages: List[ValidationMessage] = []
    seen: Dict[str, int] = {}
    for index, row in enumerate(rows, start=2):
        dialogue_id = row.get("dialogue_id", "")
        source_file = row.get("source_file", "")
        label = row.get("label", "")
        text = normalize_tsv_text(row.get("text", ""))

        if not dialogue_id:
            messages.append(ValidationMessage("error", dialogue_id, source_file, label, f"row {index} is missing dialogue_id"))
        elif dialogue_id in seen:
            messages.append(ValidationMessage("error", dialogue_id, source_file, label, f"duplicate dialogue_id also appears on row {seen[dialogue_id]}"))
        else:
            seen[dialogue_id] = index

        if not source_file:
            messages.append(ValidationMessage("error", dialogue_id, source_file, label, f"row {index} is missing source_file"))
        if not label:
            messages.append(ValidationMessage("error", dialogue_id, source_file, label, f"row {index} is missing label"))
        if "\t" in text:
            messages.append(ValidationMessage("error", dialogue_id, source_file, label, "text contains a raw tab character"))
        if text.endswith("\\"):
            messages.append(ValidationMessage("error", dialogue_id, source_file, label, "text ends with a dangling backslash"))
        if "$" in text:
            messages.append(ValidationMessage("warning", dialogue_id, source_file, label, "text contains '$'; the importer restores the final terminator automatically"))
        if '"' in text and r"\"" not in text:
            messages.append(ValidationMessage("warning", dialogue_id, source_file, label, "text contains a quote; importer will escape it for assembly syntax"))

        for escape in ESCAPE_RE.findall(text):
            if escape[1] not in {"n", "l", "p", '"', "\\", "t", "r", "0"}:
                messages.append(ValidationMessage("warning", dialogue_id, source_file, label, f"unknown backslash escape {escape!r}"))

        for line in visible_lines(text):
            if len(line) > line_length:
                messages.append(ValidationMessage("warning", dialogue_id, source_file, label, f"visible line is {len(line)} chars; threshold is {line_length}"))

        for paragraph in text.split("\\p"):
            lines = re.split(r"\\[nl]", paragraph)
            if len(lines) > 3:
                messages.append(ValidationMessage("warning", dialogue_id, source_file, label, "text box appears to contain more than 3 visible lines"))

        if source_entries is not None:
            entry = source_entries.get((source_file, label))
            if entry is None:
                messages.append(ValidationMessage("error", dialogue_id, source_file, label, "source label was not found"))
                continue
            if not text and entry.text:
                messages.append(ValidationMessage("warning", dialogue_id, source_file, label, "edited text is empty but original was not"))
            old_tokens = set(TOKEN_RE.findall(entry.text))
            new_tokens = set(TOKEN_RE.findall(text))
            missing = sorted(old_tokens - new_tokens)
            if missing:
                messages.append(ValidationMessage("error", dialogue_id, source_file, label, f"edited text removed token(s): {', '.join(missing)}"))

    return messages


def write_text_atomically(path: Path, text: str, backup: bool = False) -> Optional[Path]:
    backup_path = None
    if backup:
        backup_path = path.with_suffix(path.suffix + ".bak")
        shutil.copy2(path, backup_path)
    fd, temp_name = tempfile.mkstemp(prefix=path.name + ".", suffix=".tmp", dir=str(path.parent))
    try:
        with os.fdopen(fd, "w", encoding="utf-8", newline="") as stream:
            stream.write(text)
        os.replace(temp_name, path)
    finally:
        if os.path.exists(temp_name):
            os.unlink(temp_name)
    return backup_path
