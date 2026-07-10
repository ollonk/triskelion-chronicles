# Dialogue TSV Schema

Columns are tab-separated and should stay in this order.

`dialogue_id`: Stable unique ID. Explicit `@dialogue id=...` is preferred; otherwise `source_file::label`.

`source_file`: Repo-relative path to the source file.

`label`: Source label containing the `.string` block.

`map`: Best-effort map name from annotation or `data/maps/<map>/...`.

`speaker`: Best-effort speaker name from annotation or label.

`context`: Best-effort context from annotation or label.

`tags`: Optional comma-separated tags.

`text`: Editable dialogue text. Uses visible source escapes like `\n`, `\l`, and `\p`. The final `$` terminator is intentionally omitted and restored during import.

`notes`: Human notes. Import ignores this column.

`original_hash`: Hash of the exported text for stale-edit conflict detection. Do not edit manually.
