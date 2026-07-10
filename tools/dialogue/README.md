# Dialogue TSV Workflow

These tools export pokeemerald-style event/dialogue strings from the source tree to a TSV file, then import edited TSV rows back into the original source files.

The existing script files remain the source of truth. This does not replace map scripts, introduce a new scripting language, or require a binary ROM editor.

## Export

Run from the repo root:

```bash
python3 tools/dialogue/export_dialogue.py --output dialogue/dialogue.tsv
```

The exporter scans assembly `.string` dialogue blocks in likely source locations, currently `data/maps/**/*.inc`, `data/text/**/*.inc`, and `data/**/*.s`. It skips build outputs, `.git`, existing dialogue output, and tooling directories.

Useful filters:

```bash
python3 tools/dialogue/export_dialogue.py --output dialogue/dialogue.tsv --only-annotated
python3 tools/dialogue/export_dialogue.py --include "data/maps/GoldenrodCity/scripts.inc" --output dialogue/goldenrod.tsv
python3 tools/dialogue/export_dialogue.py --exclude "data/text/**"
```

The export also writes `dialogue/dialogue_export_report.txt`.

## Edit

Open `dialogue/dialogue.tsv` in LibreOffice Calc, OnlyOffice, VS Code, Codium, or another TSV-aware editor.

Only edit the `text` and `notes` columns for normal dialogue work. The importer ignores `notes`.

Keep engine tokens such as `{PLAYER}`, `{STR_VAR_1}`, `{RIVAL}`, `{PAUSE 0x0F}`, `\n`, `\l`, and `\p` intact unless you deliberately know they should change. The final `$` terminator is omitted from TSV text and restored by the importer.

## Validate And Dry-Run

```bash
python3 tools/dialogue/import_dialogue.py dialogue/dialogue.tsv --validate-only
python3 tools/dialogue/import_dialogue.py dialogue/dialogue.tsv
```

Default import mode is dry-run. It prints the source labels it would update and does not write files.

Warnings include long visible lines, likely overfull text boxes, unknown escapes, empty replacement text, quotes, and `$` in editable text. Errors include duplicate IDs, missing source files/labels, raw tabs, dangling backslashes, and removed required tokens.

The visible line warning threshold defaults to 32 characters:

```bash
python3 tools/dialogue/import_dialogue.py dialogue/dialogue.tsv --line-length 36
```

## Write Changes

```bash
python3 tools/dialogue/import_dialogue.py dialogue/dialogue.tsv --write
```

Optional backups:

```bash
python3 tools/dialogue/import_dialogue.py dialogue/dialogue.tsv --write --backup
```

Backups are written next to edited source files with a `.bak` suffix. Without `--backup`, rely on Git:

```bash
git diff
git status
```

Commit before large dialogue passes so recovery is simple.

## Conflict Detection

Each export row has an `original_hash` for the source text at export time. During import, the tool re-reads the current source label.

If the current source hash differs from the TSV `original_hash`, import refuses to overwrite that row by default. This prevents stale TSV edits from clobbering manual source edits or Codex changes.

Use `--force` only after reviewing the conflict:

```bash
python3 tools/dialogue/import_dialogue.py dialogue/dialogue.tsv --write --force
```

## Annotations

Add optional comments immediately above dialogue labels:

```asm
@ @dialogue id=MARA_GOLDENROD_BEFORE map=GoldenrodCity speaker=Mara context=BeforeBattle tags=Act1,Rival
GoldenrodCity_Text_MaraBeforeBattle::
	.string "There you are!\n"
	.string "Goldenrod is huge.$"
```

Supported keys are `id`, `map`, `speaker`, `context`, and `tags`. If no annotation exists, the exporter still exports the string and uses `source_file::label` as the stable ID.

## Recommended Workflow

```bash
# 1. Export current dialogue
python3 tools/dialogue/export_dialogue.py --output dialogue/dialogue.tsv

# 2. Edit dialogue/dialogue.tsv in LibreOffice Calc, OnlyOffice, VS Code, or Codium

# 3. Validate and preview changes
python3 tools/dialogue/import_dialogue.py dialogue/dialogue.tsv --validate-only
python3 tools/dialogue/import_dialogue.py dialogue/dialogue.tsv

# 4. Apply changes
python3 tools/dialogue/import_dialogue.py dialogue/dialogue.tsv --write

# 5. Build
make

# 6. Review
git diff
git status
```

## Tests

```bash
python3 -m unittest tools.dialogue.tests.test_dialogue_tools
```

Manual smoke test:

1. Export to `dialogue/dialogue.tsv`.
2. Edit one row in a throwaway branch.
3. Run dry-run import and confirm only that label is planned.
4. Run `--write`, inspect `git diff`, then revert the test edit.
5. Re-export and confirm the TSV reflects the source.

## Future Local Editor

A future Flask or FastAPI local browser editor can sit on top of `dialogue/dialogue.tsv` and provide search, filtering, editing, and preview. The TSV export/import workflow is the stable foundation and does not require web dependencies.

## Known Limitations

The first pass focuses on assembly `.string` blocks used by map and text includes. C `_()` UI strings are detected during repo inspection but are not imported by this tool yet. The importer rewrites the matched `.string` lines for a label and preserves surrounding labels, scripts, and comments.
