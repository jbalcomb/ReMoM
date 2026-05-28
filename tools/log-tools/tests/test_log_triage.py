#!/usr/bin/env python3
"""
test_log_triage.py - stdlib unittest tests for tools/log-tools/log_triage.py.

Each test invokes the script as a subprocess on a fixture file under
tests/fixtures/ and asserts on the structured JSON output. This style
catches regressions in both the parser and the JSON contract that
the log-curator agent depends on.

Run from anywhere:
    py -m unittest tools/log-tools/tests/test_log_triage.py
    # or
    py tools/log-tools/tests/test_log_triage.py
"""

import json
import os
import subprocess
import sys
import unittest


TESTS_DIR    = os.path.dirname(os.path.abspath(__file__))
TOOL_DIR     = os.path.dirname(TESTS_DIR)
SCRIPT_PATH  = os.path.join(TOOL_DIR, "log_triage.py")
FIXTURES_DIR = os.path.join(TESTS_DIR, "fixtures")


def run_triage(fixture_name, extra_args=None):
    fixture_path = os.path.join(FIXTURES_DIR, fixture_name)
    cmd = [sys.executable, SCRIPT_PATH, fixture_path]
    if extra_args:
        cmd.extend(extra_args)
    completed = subprocess.run(cmd, capture_output=True, text=True, check=True)
    return json.loads(completed.stdout)


class TestTriageClean(unittest.TestCase):
    def test_no_fatal_no_error_no_crash_no_drops(self):
        report = run_triage("clean.log")
        self.assertEqual(report["fatals"], [])
        self.assertEqual(report["errors"], [])
        self.assertEqual(report["crashes"], [])
        self.assertEqual(report["logger_drops"], [])

    def test_severity_counts(self):
        report = run_triage("clean.log")
        self.assertEqual(report["severity_counts"]["INFO"], 3)
        self.assertEqual(report["severity_counts"]["DEBUG"], 3)
        self.assertEqual(report["severity_counts"]["FATAL"], 0)

    def test_category_counts(self):
        report = run_triage("clean.log")
        self.assertEqual(report["category_counts"]["GENERAL"], 3)
        self.assertEqual(report["category_counts"]["AIMOVE"], 2)
        self.assertEqual(report["category_counts"]["PFL"], 1)

    def test_no_time_gaps(self):
        report = run_triage("clean.log")
        self.assertEqual(report["time_gaps"], [])

    def test_summary_strings(self):
        report = run_triage("clean.log")
        self.assertIn("0 FATAL", report["summary"])
        self.assertIn("0 ERROR", report["summary"])
        self.assertIn("0 CRASH", report["summary"])


class TestTriageWithFatal(unittest.TestCase):
    def test_one_fatal(self):
        report = run_triage("with_fatal.log")
        self.assertEqual(len(report["fatals"]), 1)
        self.assertIn("cannot recover", report["fatals"][0]["line"])

    def test_one_error(self):
        report = run_triage("with_fatal.log")
        self.assertEqual(len(report["errors"]), 1)
        self.assertIn("write failed", report["errors"][0]["line"])

    def test_one_crash_with_signal_name(self):
        report = run_triage("with_fatal.log")
        self.assertEqual(len(report["crashes"]), 1)
        self.assertEqual(report["crashes"][0]["exception_or_signal"], "EXCEPTION_ACCESS_VIOLATION")

    def test_fatal_includes_context(self):
        report = run_triage("with_fatal.log")
        ctx = report["fatals"][0]["context"]
        ctx_text = " ".join(c["line"] for c in ctx)
        self.assertIn("write failed", ctx_text)

    def test_summary_strings(self):
        report = run_triage("with_fatal.log")
        self.assertIn("1 FATAL", report["summary"])
        self.assertIn("1 ERROR", report["summary"])
        self.assertIn("1 CRASH", report["summary"])


class TestTriageWithDrops(unittest.TestCase):
    def test_drop_marker_parsed(self):
        report = run_triage("with_drops.log")
        self.assertEqual(len(report["logger_drops"]), 1)
        self.assertEqual(report["logger_drops"][0]["n_dropped"], 4242)

    def test_time_gap_detected(self):
        report = run_triage("with_drops.log")
        self.assertEqual(len(report["time_gaps"]), 1)
        self.assertGreater(report["time_gaps"][0]["gap_seconds"], 5)

    def test_time_gap_threshold_is_configurable(self):
        report = run_triage("with_drops.log", extra_args=["--gap-seconds", "100"])
        self.assertEqual(report["time_gaps"], [])

    def test_summary_includes_drop_event(self):
        report = run_triage("with_drops.log")
        self.assertIn("1 LOGGER drop event(s)", report["summary"])


class TestTriageMissingFile(unittest.TestCase):
    def test_missing_file_exits_nonzero_with_stderr_message(self):
        cmd = [sys.executable, SCRIPT_PATH, "definitely_does_not_exist.log"]
        completed = subprocess.run(cmd, capture_output=True, text=True)
        self.assertNotEqual(completed.returncode, 0)
        self.assertIn("log_triage", completed.stderr)


if __name__ == "__main__":
    unittest.main()
