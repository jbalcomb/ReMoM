#!/usr/bin/env python3
"""
test_log_migrate_io.py - stdlib unittest tests for tools/log-tools/log_migrate_io.py.

Fixture io_probe.c exercises:
    - printf(...) detection (2 calls)
    - fprintf(stderr,...) detection (2 calls + 1 trailing line containing the word 'printf' in its message)
    - fprintf(stdout,...) detection (1 call)
    - SKIP rules for fprintf to other streams (fp, out, fout, PLAY_LOG, replay_file, logfile, Unit_Outcomes_File)
    - sprintf / vfprintf must NOT match the printf detector
"""

import hashlib
import json
import os
import subprocess
import sys
import unittest


TESTS_DIR    = os.path.dirname(os.path.abspath(__file__))
TOOL_DIR     = os.path.dirname(TESTS_DIR)
REPO_ROOT    = os.path.dirname(os.path.dirname(TOOL_DIR))
SCRIPT_PATH  = os.path.join(TOOL_DIR, "log_migrate_io.py")
FIXTURES_DIR = os.path.join(TESTS_DIR, "fixtures", "migrate")
IO_PROBE     = os.path.join(FIXTURES_DIR, "io_probe.c")


def run_io(path_args):
    cmd = [sys.executable, SCRIPT_PATH] + list(path_args)
    completed = subprocess.run(cmd, capture_output=True, text=True, check=True)
    return json.loads(completed.stdout)


def items_for_file(report, filename_endswith):
    return [i for i in report["items"] if i["file"].replace("\\", "/").endswith(filename_endswith)]


class TestDetectionAndSkips(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.report = run_io([IO_PROBE])
        cls.items  = items_for_file(cls.report, "io_probe.c")

    def test_printf_calls_detected(self):
        printfs = [i for i in self.items if i["kind"] == "printf"]
        self.assertEqual(len(printfs), 2)

    def test_fprintf_stderr_calls_detected(self):
        stderrs = [i for i in self.items if i["kind"] == "fprintf_stderr"]
        # 2 fprintf(stderr, "FATAL/stderr...") plus 1 trailing 'fprintf containing word printf' line.
        self.assertEqual(len(stderrs), 3)

    def test_fprintf_stdout_calls_detected(self):
        stdouts = [i for i in self.items if i["kind"] == "fprintf_stdout"]
        self.assertEqual(len(stdouts), 1)

    def test_non_logging_streams_are_skipped(self):
        # fp, out, fout, PLAY_LOG, replay_file, logfile, Unit_Outcomes_File = 7 skipped
        self.assertEqual(self.report["skipped_fprintf"], 7)

    def test_no_skipped_stream_appears_in_items(self):
        for skipped_stream in ("fp", "out", "fout", "PLAY_LOG", "replay_file", "logfile", "Unit_Outcomes_File"):
            for item in self.items:
                self.assertNotIn(f"({skipped_stream},", item["original_call"])

    def test_sprintf_and_vfprintf_not_matched_as_printf(self):
        # sprintf shares 'printf' substring; the printf detector must not match it
        for item in self.items:
            if item["kind"] == "printf":
                self.assertFalse(item["original_call"].startswith("sprintf"))
        # vfprintf likewise — and our fprintf detector requires the exact 'fprintf(' boundary
        for item in self.items:
            self.assertFalse(item["original_call"].startswith("vfprintf"))


class TestSeverityHeuristic(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.report = run_io([IO_PROBE])
        cls.items  = items_for_file(cls.report, "io_probe.c")

    def test_fatal_keyword_maps_to_FATAL(self):
        item = next(i for i in self.items if "cannot proceed" in i["original_call"])
        self.assertEqual(item["proposed_severity"], "FATAL")

    def test_error_keyword_maps_to_ERROR(self):
        item = next(i for i in self.items if "printf with error keyword" in i["original_call"])
        self.assertEqual(item["proposed_severity"], "ERROR")

    def test_default_severity_is_INFO_not_DEBUG(self):
        # Unlike log_migrate.py, the io variant defaults to INFO so that
        # currently-visible diagnostic output stays visible in Release builds.
        item = next(i for i in self.items if "plain printf line" in i["original_call"])
        self.assertEqual(item["proposed_severity"], "INFO")


class TestProposedCall(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.report = run_io([IO_PROBE])
        cls.items  = items_for_file(cls.report, "io_probe.c")

    def test_printf_proposal_uses_LOG_macro(self):
        item = next(i for i in self.items if "plain printf line" in i["original_call"])
        self.assertIn("LOG_INFO(LOG_CAT_GENERAL,", item["proposed_call"])

    def test_fprintf_proposal_drops_stream_argument(self):
        item = next(i for i in self.items if "stderr message" in i["original_call"])
        # The proposed call should NOT contain "stderr,"
        self.assertNotIn("stderr,", item["proposed_call"])
        self.assertIn("LOG_INFO(LOG_CAT_GENERAL,", item["proposed_call"])

    def test_proposed_call_strips_trailing_newline(self):
        item = next(i for i in self.items if "plain printf line" in i["original_call"])
        self.assertNotIn(r"\n\"", item["proposed_call"])


class TestNoSourceWrites(unittest.TestCase):
    def _snapshot_fixtures(self):
        snapshot = {}
        for dirpath, _dirnames, filenames in os.walk(FIXTURES_DIR):
            for name in filenames:
                if name.endswith((".c", ".cpp")):
                    path = os.path.join(dirpath, name)
                    with open(path, "rb") as f:
                        snapshot[path] = hashlib.sha256(f.read()).hexdigest()
        return snapshot

    def test_running_the_script_does_not_modify_any_fixture(self):
        before = self._snapshot_fixtures()
        self.assertGreater(len(before), 0)
        run_io([FIXTURES_DIR])
        after = self._snapshot_fixtures()
        self.assertEqual(before, after)


class TestRealTreeSmoke(unittest.TestCase):
    """Run against real source tree and assert non-empty inventory."""

    def test_real_tree_produces_inventory(self):
        paths = [
            os.path.join(REPO_ROOT, "MoM", "src"),
            os.path.join(REPO_ROOT, "MoX", "src"),
            os.path.join(REPO_ROOT, "STU", "src"),
            os.path.join(REPO_ROOT, "platform"),
            os.path.join(REPO_ROOT, "src"),
        ]
        existing = [p for p in paths if os.path.isdir(p)]
        if not existing:
            self.skipTest("repo tree not available")
        report = run_io(existing)
        self.assertGreater(report["files_scanned"], 0)
        # Recon (Phase 8 planning) saw ~370 candidate sites; expect at least 100.
        self.assertGreater(report["total_matches"], 100,
            "expected ample printf/fprintf-stderr/stdout sites across the real tree")
        # And we should have skipped a meaningful number of fprintf-to-file-pointer calls.
        self.assertGreater(report["skipped_fprintf"], 100)


if __name__ == "__main__":
    unittest.main()
