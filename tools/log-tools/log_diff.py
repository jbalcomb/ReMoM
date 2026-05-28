#!/usr/bin/env python3
"""
log_diff.py - compare two STU_LOG log files in raw or semantic mode.

Modes:
    --raw       Normalize timestamps to <TS>, then unified diff. Forensic mode.
                Reordering and line-level differences show up.
    --semantic  (default) Parse each line into (severity, category, template),
                mask numeric tokens in the template, build counters per
                (category, severity), report symmetric differences and count
                deltas. Ignores benign timing/ordering; flags real behavioral
                divergence.

Usage:
    python log_diff.py [--raw|--semantic] <a.log> <b.log>

Output: JSON on stdout. Exit 0 always (even on differences); exit 1 only if a
file can't be read.
"""

import argparse
import collections
import difflib
import json
import re
import sys


RE_TIMESTAMP = re.compile(r"\[\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}Z\]")
RE_NUMERIC   = re.compile(r"\b-?\d+(?:\.\d+)?\b")

# Matches a regular STU_LOG line for semantic parsing.
RE_LOG_LINE = re.compile(
    r"^\[(?P<ts>\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}Z)\] "
    r"\[(?P<sev>TRACE|DEBUG|INFO |WARN |ERROR|FATAL)\] "
    r"\[(?P<cat>[A-Z ]+?)\s*\] "
    r"(?P<file>[^:]+):(?P<line>\d+) "
    r"(?P<func>[^:]+): "
    r"(?P<msg>.*)$"
)


def normalize_timestamps(text):
    return RE_TIMESTAMP.sub("<TS>", text)


def parse_for_template(line):
    """Return (cat, sev, template) tuple, or None if line doesn't match the regular log format."""
    m = RE_LOG_LINE.match(line)
    if not m:
        return None
    sev = m.group("sev").rstrip()
    cat = m.group("cat").rstrip()
    msg = m.group("msg")
    template = RE_NUMERIC.sub("<N>", msg)
    return (cat, sev, template)


def build_template_counter(path):
    counter = collections.Counter()
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        for raw in f:
            line = raw.rstrip("\n").rstrip("\r")
            parsed = parse_for_template(line)
            if parsed is not None:
                counter[parsed] += 1
    return counter


def diff_semantic(a_path, b_path):
    a_counts = build_template_counter(a_path)
    b_counts = build_template_counter(b_path)

    only_in_a = []
    only_in_b = []
    count_deltas = []

    all_keys = set(a_counts) | set(b_counts)
    for key in sorted(all_keys):
        cat, sev, template = key
        a = a_counts.get(key, 0)
        b = b_counts.get(key, 0)
        entry = {"category": cat, "severity": sev, "template": template}
        if a > 0 and b == 0:
            only_in_a.append({**entry, "count": a})
        elif b > 0 and a == 0:
            only_in_b.append({**entry, "count": b})
        elif a != b:
            count_deltas.append({**entry, "a_count": a, "b_count": b})

    return {
        "files": [a_path, b_path],
        "mode": "semantic",
        "only_in_a": only_in_a,
        "only_in_b": only_in_b,
        "count_deltas": count_deltas,
        "summary": [
            f"{len(only_in_a)} template(s) only in A",
            f"{len(only_in_b)} template(s) only in B",
            f"{len(count_deltas)} template(s) with count deltas",
        ],
    }


def diff_raw(a_path, b_path):
    with open(a_path, "r", encoding="utf-8", errors="replace") as f:
        a_text = f.read()
    with open(b_path, "r", encoding="utf-8", errors="replace") as f:
        b_text = f.read()

    a_norm = normalize_timestamps(a_text).splitlines(keepends=False)
    b_norm = normalize_timestamps(b_text).splitlines(keepends=False)

    diff_lines = list(difflib.unified_diff(
        a_norm, b_norm,
        fromfile=a_path, tofile=b_path,
        lineterm="",
    ))

    return {
        "files": [a_path, b_path],
        "mode": "raw",
        "diff": "\n".join(diff_lines),
        "diff_line_count": len(diff_lines),
        "has_differences": len(diff_lines) > 0,
    }


def main():
    parser = argparse.ArgumentParser(description="Diff two STU_LOG log files.")
    parser.add_argument("a", help="Path to log file A")
    parser.add_argument("b", help="Path to log file B")
    group = parser.add_mutually_exclusive_group()
    group.add_argument("--raw", action="store_true", help="Forensic mode: normalize timestamps then unified diff")
    group.add_argument("--semantic", action="store_true", help="Default mode: compare template counts per (cat, sev), masking numerics")
    args = parser.parse_args()

    try:
        if args.raw:
            report = diff_raw(args.a, args.b)
        else:
            report = diff_semantic(args.a, args.b)
    except FileNotFoundError as e:
        sys.stderr.write(f"log_diff: {e}\n")
        return 1
    except OSError as e:
        sys.stderr.write(f"log_diff: {e}\n")
        return 1

    json.dump(report, sys.stdout, indent=2)
    sys.stdout.write("\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
