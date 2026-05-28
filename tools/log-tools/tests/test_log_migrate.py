#!/usr/bin/env python3
"""
test_log_migrate.py - stdlib unittest tests for tools/log-tools/log_migrate.py.

Fixtures under tests/fixtures/migrate/ are intentionally not compilable C —
they're just text the script greps. Each fixture exercises a specific
heuristic branch (severity from message content, category from path/filename).

The no-source-writes invariant is verified by snapshotting all fixture file
contents before running the script and comparing after.
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
SCRIPT_PATH  = os.path.join(TOOL_DIR, "log_migrate.py")
FIXTURES_DIR = os.path.join(TESTS_DIR, "fixtures", "migrate")


def run_migrate(path_args):
    cmd = [sys.executable, SCRIPT_PATH] + list(path_args)
    completed = subprocess.run(cmd, capture_output=True, text=True, check=True)
    return json.loads(completed.stdout)


def items_for_file(report, filename_endswith):
    return [i for i in report["items"] if i["file"].replace("\\", "/").endswith(filename_endswith)]


class TestSeverityHeuristic(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.report = run_migrate([os.path.join(FIXTURES_DIR, "severity_probe.c")])
        cls.items  = items_for_file(cls.report, "severity_probe.c")

    def test_five_calls_found(self):
        self.assertEqual(len(self.items), 5)

    def test_fatal_keyword_maps_to_FATAL(self):
        item = next(i for i in self.items if "cannot recover" in i["original_call"])
        self.assertEqual(item["proposed_severity"], "FATAL")

    def test_error_keyword_maps_to_ERROR(self):
        item = next(i for i in self.items if "write failed" in i["original_call"])
        self.assertEqual(item["proposed_severity"], "ERROR")

    def test_warn_keyword_maps_to_WARN(self):
        item = next(i for i in self.items if "unexpected state" in i["original_call"])
        self.assertEqual(item["proposed_severity"], "WARN")

    def test_no_keyword_defaults_to_DEBUG(self):
        item = next(i for i in self.items if "turn 1 begin" in i["original_call"])
        self.assertEqual(item["proposed_severity"], "DEBUG")

    def test_trc_prn_call_also_detected(self):
        item = next(i for i in self.items if "TRACE stack" in i["original_call"])
        self.assertIn("trc_prn", item["original_call"])


class TestCategoryHeuristic(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.report = run_migrate([FIXTURES_DIR])

    def test_AIMOVE_filename(self):
        items = items_for_file(self.report, "/AIMOVE.c")
        self.assertEqual(len(items), 1)
        self.assertEqual(items[0]["proposed_category"], "AIMOVE")

    def test_Combat_filename(self):
        items = items_for_file(self.report, "/Combat.c")
        self.assertEqual(len(items), 1)
        self.assertEqual(items[0]["proposed_category"], "COMBAT")

    def test_LOADSAVE_filename(self):
        items = items_for_file(self.report, "/LOADSAVE.c")
        self.assertEqual(len(items), 1)
        self.assertEqual(items[0]["proposed_category"], "SAVE")

    def test_Fields_filename(self):
        items = items_for_file(self.report, "/Fields.c")
        self.assertEqual(len(items), 1)
        self.assertEqual(items[0]["proposed_category"], "IKI")

    def test_platform_path_component(self):
        items = items_for_file(self.report, "/pfl_probe.c")
        self.assertEqual(len(items), 1)
        self.assertEqual(items[0]["proposed_category"], "PFL")

    def test_default_category_is_GENERAL(self):
        items = items_for_file(self.report, "/severity_probe.c")
        for item in items:
            self.assertEqual(item["proposed_category"], "GENERAL")


class TestProposedCall(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.report = run_migrate([FIXTURES_DIR])

    def test_proposed_call_uses_LOG_macro_and_category_enum(self):
        items = items_for_file(self.report, "/AIMOVE.c")
        self.assertIn("LOG_DEBUG(LOG_CAT_AIMOVE,", items[0]["proposed_call"])

    def test_proposed_call_strips_trailing_newline(self):
        items = items_for_file(self.report, "/AIMOVE.c")
        # Original is dbg_prn("turn 1 begin\n"); proposed should drop the \n
        # because LOG_* macros add their own newline.
        self.assertNotIn(r"\n\"", items[0]["proposed_call"])
        self.assertIn("turn 1 begin", items[0]["proposed_call"])

    def test_proposed_call_for_FATAL(self):
        report = run_migrate([os.path.join(FIXTURES_DIR, "severity_probe.c")])
        items  = items_for_file(report, "/severity_probe.c")
        item = next(i for i in items if "cannot recover" in i["original_call"])
        self.assertIn("LOG_FATAL(LOG_CAT_GENERAL,", item["proposed_call"])


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
        self.assertGreater(len(before), 0, "sanity: at least one fixture .c should exist")
        run_migrate([FIXTURES_DIR])
        after = self._snapshot_fixtures()
        self.assertEqual(before, after)


class TestRealTreeSmoke(unittest.TestCase):
    """Sanity check: run against the real repo paths and assert a non-empty inventory.

    Phase 7's plan calls for this; the actual mass-migration is Phase 8."""

    def test_real_tree_produces_inventory(self):
        mom_src = os.path.join(REPO_ROOT, "MoM", "src")
        stu_src = os.path.join(REPO_ROOT, "STU", "src")
        if not (os.path.isdir(mom_src) and os.path.isdir(stu_src)):
            self.skipTest("repo tree not available")
        report = run_migrate([mom_src, stu_src])
        self.assertGreater(report["files_scanned"], 0)
        self.assertGreater(report["total_matches"], 0,
            "expected at least one remaining dbg_prn/trc_prn call-site somewhere in MoM/src or STU/src")


if __name__ == "__main__":
    unittest.main()
