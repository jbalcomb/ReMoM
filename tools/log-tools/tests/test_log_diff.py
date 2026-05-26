#!/usr/bin/env python3
"""
test_log_diff.py - stdlib unittest tests for tools/log-tools/log_diff.py.

Fixture pair diff_a.log / diff_b.log differs in three ways:
    1. Timestamps shifted by 1 hour (benign-timing difference)
    2. Lines 2/3 swapped (line-level reordering)
    3. One new ERROR/SAVE line in B (real behavioral divergence)

Semantic mode should ignore #1 and #2 (template counts are identical for
shared lines after numeric masking) but flag #3.
Raw mode should ignore #1 (timestamps normalized) but flag #2 and #3.
"""

import json
import os
import subprocess
import sys
import unittest


TESTS_DIR    = os.path.dirname(os.path.abspath(__file__))
TOOL_DIR     = os.path.dirname(TESTS_DIR)
SCRIPT_PATH  = os.path.join(TOOL_DIR, "log_diff.py")
FIXTURES_DIR = os.path.join(TESTS_DIR, "fixtures")
A_LOG        = os.path.join(FIXTURES_DIR, "diff_a.log")
B_LOG        = os.path.join(FIXTURES_DIR, "diff_b.log")


def run_diff(mode_flag=None, a=A_LOG, b=B_LOG):
    cmd = [sys.executable, SCRIPT_PATH]
    if mode_flag:
        cmd.append(mode_flag)
    cmd.extend([a, b])
    completed = subprocess.run(cmd, capture_output=True, text=True, check=True)
    return json.loads(completed.stdout)


class TestSemanticDiff(unittest.TestCase):
    def test_defaults_to_semantic(self):
        report = run_diff()
        self.assertEqual(report["mode"], "semantic")

    def test_explicit_semantic_flag(self):
        report = run_diff("--semantic")
        self.assertEqual(report["mode"], "semantic")

    def test_new_error_template_appears_only_in_b(self):
        report = run_diff()
        self.assertEqual(len(report["only_in_b"]), 1)
        self.assertEqual(report["only_in_b"][0]["category"], "SAVE")
        self.assertEqual(report["only_in_b"][0]["severity"], "ERROR")
        self.assertIn("write failed", report["only_in_b"][0]["template"])

    def test_nothing_only_in_a(self):
        report = run_diff()
        self.assertEqual(report["only_in_a"], [])

    def test_no_count_deltas_for_shared_templates(self):
        # AIMOVE "stack <N> picked <something>" appears 2x in both files
        # after numeric masking; line-order doesn't change template counts.
        report = run_diff()
        self.assertEqual(report["count_deltas"], [])


class TestRawDiff(unittest.TestCase):
    def test_raw_mode_set_in_report(self):
        report = run_diff("--raw")
        self.assertEqual(report["mode"], "raw")

    def test_raw_has_differences(self):
        report = run_diff("--raw")
        self.assertTrue(report["has_differences"])
        self.assertGreater(report["diff_line_count"], 0)

    def test_raw_includes_the_new_error_line(self):
        report = run_diff("--raw")
        self.assertIn("write failed for slot 3", report["diff"])

    def test_raw_includes_reordering(self):
        # The diff text should reference the swapped stack lines.
        report = run_diff("--raw")
        self.assertIn("stack 1 picked Shield", report["diff"])
        self.assertIn("stack 0 picked Sword", report["diff"])

    def test_raw_normalizes_timestamps_so_timing_alone_doesnt_diff(self):
        # diff_a and diff_b have hour-shifted timestamps for shared content lines.
        # Because both files get normalized to <TS>, the END line (which is
        # otherwise identical) should NOT appear in the diff as a changed line.
        report = run_diff("--raw")
        # The END line itself should not be flagged purely due to its timestamp.
        # If timestamp normalization works, the BEGIN line text isn't on a +/- side.
        # We look for the count of +/- lines mentioning BEGIN — should be 0.
        diff_text = report["diff"]
        plus_minus_begin_lines = [
            ln for ln in diff_text.splitlines()
            if (ln.startswith("+") or ln.startswith("-"))
            and "BEGIN: ReMoM main()" in ln
            and not ln.startswith("+++") and not ln.startswith("---")
        ]
        self.assertEqual(plus_minus_begin_lines, [],
            "BEGIN line should not appear as a diff entry — only its timestamp differs and timestamps are normalized")


class TestDiffMissingFile(unittest.TestCase):
    def test_missing_file_exits_nonzero(self):
        cmd = [sys.executable, SCRIPT_PATH, "does_not_exist_a.log", "does_not_exist_b.log"]
        completed = subprocess.run(cmd, capture_output=True, text=True)
        self.assertNotEqual(completed.returncode, 0)


if __name__ == "__main__":
    unittest.main()
