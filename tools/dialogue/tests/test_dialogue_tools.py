#!/usr/bin/env python3
import shutil
import tempfile
import unittest
from pathlib import Path

import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from dialogue_common import parse_dialogue_file, render_string_block, text_hash, validate_rows


class DialogueToolsTest(unittest.TestCase):
    def test_parse_assembly_dialogue(self):
        repo_root = Path(__file__).resolve().parents[3]
        fixture = repo_root / "tools/dialogue/tests/fixtures/sample_scripts.inc"
        result = parse_dialogue_file(fixture, repo_root)
        self.assertEqual(len(result.entries), 2)
        first = result.entries[0]
        self.assertEqual(first.dialogue_id, "SAMPLE_HELLO")
        self.assertEqual(first.text, r"Hello, {PLAYER}!\nWelcome here.")
        self.assertEqual(first.original_hash, text_hash(first.text))

    def test_render_restores_terminator(self):
        lines = render_string_block(r"Hi.\nBye.", "\t")
        self.assertEqual(lines, ['\t.string "Hi.\\n"\n', '\t.string "Bye.$"\n'])

    def test_validation_detects_removed_token(self):
        repo_root = Path(__file__).resolve().parents[3]
        fixture = repo_root / "tools/dialogue/tests/fixtures/sample_scripts.inc"
        entry = parse_dialogue_file(fixture, repo_root).entries[0]
        rows = [
            {
                "dialogue_id": entry.dialogue_id,
                "source_file": entry.source_file,
                "label": entry.label,
                "map": "",
                "speaker": "",
                "context": "",
                "tags": "",
                "text": "Hello!",
                "notes": "",
                "original_hash": entry.original_hash,
            }
        ]
        messages = validate_rows(rows, {(entry.source_file, entry.label): entry})
        self.assertTrue(any(message.level == "error" and "removed token" in message.message for message in messages))


if __name__ == "__main__":
    unittest.main()
