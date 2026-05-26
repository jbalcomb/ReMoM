#!/usr/bin/env python3
"""
log_triage.py - structured triage report for a STU_LOG-produced log file.

Reads a remom_log_*.txt file, classifies each line, and emits a JSON report
to stdout describing severity counts, ERROR/FATAL/CRASH/LOGGER events with
context, and any timestamp gaps exceeding the configured threshold (default
5 seconds, may indicate a hang or a drop window the logger never recorded).

Usage:
    python log_triage.py <path/to/remom_log_new.txt> [--gap-seconds N]

Exit codes:
    0   report produced successfully (even if the log contains errors)
    1   the input file does not exist or cannot be read

Output schema (top-level keys):
    file              path read
    line_count        total number of lines in the file
    parsed_count      number of lines that matched the expected format
    first_timestamp   ISO8601 of the earliest parsed line, or null
    last_timestamp    ISO8601 of the latest parsed line, or null
    severity_counts   dict of {severity: count} for TRACE..FATAL
    category_counts   dict of {category: count} (only categories actually seen)
    fatals            list of {line_no, line, context} for FATAL lines
    errors            list of {line_no, line, context} for ERROR lines
    crashes           list of {line_no, line, exception_or_signal} for [CRASH] lines
    logger_drops      list of {line_no, line, n_dropped} for [LOGGER] lines
    time_gaps         list of {after_line, before_line, gap_seconds, after_ts, before_ts}
    summary           plain-string headline counts, e.g. ["1 FATAL", "0 ERROR", ...]
"""

import argparse
import datetime
import json
import re
import sys


# Matches the regular log lines emitted by STU_LOG.
# [2026-05-26T17:14:31Z] [INFO ] [GENERAL ] file.c:NN function: message
RE_LOG_LINE = re.compile(
    r"^\[(?P<ts>\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}Z)\] "
    r"\[(?P<sev>TRACE|DEBUG|INFO |WARN |ERROR|FATAL)\] "
    r"\[(?P<cat>[A-Z ]+?)\s*\] "
    r"(?P<file>[^:]+):(?P<line>\d+) "
    r"(?P<func>[^:]+): "
    r"(?P<msg>.*)$"
)

# Synthetic line written directly by log_pump when drops occurred.
# [2026-05-26T17:14:31Z] [LOGGER] 1234 messages dropped since last drain
RE_LOGGER_LINE = re.compile(
    r"^\[(?P<ts>\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}Z)\] "
    r"\[LOGGER\] "
    r"(?P<n>\d+) messages dropped since last drain$"
)

# Synthetic line written by the SEH / signal crash handler.
# [2026-05-26T17:14:31Z] [CRASH] EXCEPTION_ACCESS_VIOLATION
RE_CRASH_LINE = re.compile(
    r"^\[(?P<ts>\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}Z)\] "
    r"\[CRASH\] "
    r"(?P<sig>.+)$"
)

ISO_FMT = "%Y-%m-%dT%H:%M:%SZ"


def parse_ts(s):
    return datetime.datetime.strptime(s, ISO_FMT).replace(tzinfo=datetime.timezone.utc)


def context_for(lines, idx, before=2, after=2):
    lo = max(0, idx - before)
    hi = min(len(lines), idx + after + 1)
    return [{"line_no": i + 1, "line": lines[i]} for i in range(lo, hi) if i != idx]


def triage(path, gap_seconds):
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        raw = f.read()
    lines = raw.splitlines()

    severity_counts = {sev: 0 for sev in ["TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"]}
    category_counts = {}
    fatals = []
    errors = []
    crashes = []
    logger_drops = []
    time_gaps = []

    parsed_count = 0
    first_ts_str = None
    last_ts_str = None
    last_ts_dt = None
    last_ts_line_no = None

    for idx, raw_line in enumerate(lines):
        line = raw_line.rstrip("\r")
        line_no = idx + 1
        ts_str = None

        m = RE_LOG_LINE.match(line)
        if m:
            parsed_count += 1
            ts_str = m.group("ts")
            sev = m.group("sev").rstrip()
            cat = m.group("cat").rstrip()
            severity_counts[sev] = severity_counts.get(sev, 0) + 1
            category_counts[cat] = category_counts.get(cat, 0) + 1
            if sev == "FATAL":
                fatals.append({"line_no": line_no, "line": line, "context": context_for(lines, idx)})
            elif sev == "ERROR":
                errors.append({"line_no": line_no, "line": line, "context": context_for(lines, idx)})
        else:
            m = RE_LOGGER_LINE.match(line)
            if m:
                parsed_count += 1
                ts_str = m.group("ts")
                logger_drops.append({"line_no": line_no, "line": line, "n_dropped": int(m.group("n"))})
            else:
                m = RE_CRASH_LINE.match(line)
                if m:
                    parsed_count += 1
                    ts_str = m.group("ts")
                    crashes.append({"line_no": line_no, "line": line, "exception_or_signal": m.group("sig")})

        if ts_str is not None:
            if first_ts_str is None:
                first_ts_str = ts_str
            last_ts_str = ts_str

            current_dt = parse_ts(ts_str)
            if last_ts_dt is not None:
                gap = (current_dt - last_ts_dt).total_seconds()
                if gap > gap_seconds:
                    time_gaps.append({
                        "after_line": last_ts_line_no,
                        "before_line": line_no,
                        "gap_seconds": int(gap),
                        "after_ts": last_ts_str if last_ts_line_no == line_no else lines[last_ts_line_no - 1][1:21],
                        "before_ts": ts_str,
                    })
            last_ts_dt = current_dt
            last_ts_line_no = line_no

    summary = [
        f"{len(fatals)} FATAL",
        f"{len(errors)} ERROR",
        f"{len(crashes)} CRASH",
        f"{len(logger_drops)} LOGGER drop event(s)",
        f"{len(time_gaps)} time gap(s) > {gap_seconds}s",
    ]

    return {
        "file": path,
        "line_count": len(lines),
        "parsed_count": parsed_count,
        "first_timestamp": first_ts_str,
        "last_timestamp": last_ts_str,
        "severity_counts": severity_counts,
        "category_counts": category_counts,
        "fatals": fatals,
        "errors": errors,
        "crashes": crashes,
        "logger_drops": logger_drops,
        "time_gaps": time_gaps,
        "summary": summary,
    }


def main():
    parser = argparse.ArgumentParser(description="Triage a STU_LOG log file.")
    parser.add_argument("path", help="Path to a remom_log_*.txt file")
    parser.add_argument("--gap-seconds", type=int, default=5, help="Flag timestamp gaps > N seconds (default 5)")
    args = parser.parse_args()

    try:
        report = triage(args.path, args.gap_seconds)
    except FileNotFoundError as e:
        sys.stderr.write(f"log_triage: {e}\n")
        return 1
    except OSError as e:
        sys.stderr.write(f"log_triage: {e}\n")
        return 1

    json.dump(report, sys.stdout, indent=2)
    sys.stdout.write("\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
