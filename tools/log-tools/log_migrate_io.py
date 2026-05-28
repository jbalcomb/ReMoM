#!/usr/bin/env python3
"""
log_migrate_io.py - inventory printf and fprintf(stderr/stdout, ...) call-sites
and propose LOG_* replacements.

Companion to log_migrate.py (which handles dbg_prn / trc_prn). This script
treats user-facing diagnostic output (printf to stdout, fprintf to stderr or
stdout) as logging candidates. It explicitly SKIPS fprintf calls whose first
argument is anything other than the literal `stderr` or `stdout` — those write
to caller-owned file pointers for non-logging purposes (save dumps, replay
recording, field extraction, etc.) and must not be touched.

Like log_migrate.py: stdlib-only, JSON output, NEVER writes to any source file.

Usage:
    python log_migrate_io.py <path> [<path>...]
        e.g.  python log_migrate_io.py MoM/src STU/src MoX/src platform src

Output schema (top-level):
    scanned_paths      list of paths walked
    files_scanned      count of .c / .cpp files visited
    files_with_matches count of files containing at least one candidate
    total_matches      total candidate call-sites
    skipped_fprintf    count of fprintf calls whose stream wasn't stderr/stdout
    items              list of {file, line_no, kind, original_call,
                                proposed_severity, proposed_category,
                                proposed_call}
        kind is one of: "printf", "fprintf_stderr", "fprintf_stdout"

Severity heuristic (case-insensitive on first string-literal arg, same as log_migrate.py):
    "FATAL" or "abort" or "die"                   -> FATAL
    "error" or "fail" or "unable" or "cannot"     -> ERROR
    "warn" or "unexpected" or "should not"        -> WARN
    (default)                                     -> INFO  (note: not DEBUG)

Category heuristic (same as log_migrate.py).

Why INFO default and not DEBUG: printf/fprintf-stderr calls were chosen by
the author to be visible to a user, not gated behind a debug build. Mapping
them to DEBUG would silence them in Release. INFO preserves visibility while
giving humans a chance to demote during review.
"""

import argparse
import json
import os
import re
import sys


PRINTF_RE   = re.compile(r"\bprintf\s*\(")
FPRINTF_RE  = re.compile(r"\bfprintf\s*\(\s*([A-Za-z_][A-Za-z_0-9]*)\s*,")
STRING_LIT_RE = re.compile(r'"((?:[^"\\]|\\.)*)"')

SEVERITY_PATTERNS = [
    ("FATAL", [r"\bFATAL\b", r"\babort\b", r"\bdie\b"]),
    ("ERROR", [r"\berror\b", r"\bfail\b", r"\bunable\b", r"\bcannot\b"]),
    ("WARN",  [r"\bwarn\b", r"\bunexpected\b", r"\bshould not\b"]),
]
SEVERITY_COMPILED = [(sev, [re.compile(p, re.IGNORECASE) for p in pats]) for sev, pats in SEVERITY_PATTERNS]

LOGGING_STREAMS = {"stderr", "stdout"}


def extract_call(text, start):
    """Given the index of '(' of a call, return (call_text, end_index) including the matching ')'. Returns (None, start) if unbalanced."""
    depth = 0
    in_string = False
    in_char = False
    escape = False
    i = start
    while i < len(text):
        c = text[i]
        if escape:
            escape = False
        elif c == "\\":
            escape = True
        elif in_string:
            if c == '"':
                in_string = False
        elif in_char:
            if c == "'":
                in_char = False
        elif c == '"':
            in_string = True
        elif c == "'":
            in_char = True
        elif c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
            if depth == 0:
                return (text[start:i + 1], i + 1)
        i += 1
    return (None, start)


def first_string_literal(call_text):
    m = STRING_LIT_RE.search(call_text)
    if m:
        return m.group(1)
    return ""


def guess_severity(call_text):
    body = first_string_literal(call_text)
    for sev, patterns in SEVERITY_COMPILED:
        for p in patterns:
            if p.search(body):
                return sev
    return "INFO"  # DIFFERENT default from log_migrate.py — see module docstring


def guess_category(path):
    norm = path.replace("\\", "/")
    parts = [p.lower() for p in norm.split("/")]
    base = os.path.basename(norm)
    base_upper = base.upper()

    if base_upper.startswith("AI") and base_upper.endswith(".C"):
        return "AIMOVE"
    if base_upper == "COMBAT.C":
        return "COMBAT"
    if base_upper == "LOADSAVE.C":
        return "SAVE"
    if base.startswith(("Save_", "Load_")):
        return "SAVE"
    if base_upper == "FIELDS.C":
        return "IKI"
    if "platform" in parts:
        return "PFL"
    return "GENERAL"


def severity_to_macro(sev):
    return {
        "TRACE": "LOG_TRACE",
        "DEBUG": "LOG_DEBUG",
        "INFO":  "LOG_INFO",
        "WARN":  "LOG_WARN",
        "ERROR": "LOG_ERROR",
        "FATAL": "LOG_FATAL",
    }[sev]


def category_to_enum(cat):
    return {
        "GENERAL": "LOG_CAT_GENERAL",
        "AIMOVE":  "LOG_CAT_AIMOVE",
        "COMBAT":  "LOG_CAT_COMBAT",
        "SAVE":    "LOG_CAT_SAVE",
        "PFL":     "LOG_CAT_PFL",
        "IKI":     "LOG_CAT_IKI",
    }[cat]


def propose_call_printf(original_call, severity, category):
    """printf("fmt", args) -> LOG_*(LOG_CAT_*, "fmt", args). Strips trailing \\n from format string since LOG_* macros add their own newline."""
    inner = re.match(r"\s*printf\s*\((.*)\)\s*;?\s*$", original_call, re.DOTALL)
    if not inner:
        return None
    args = inner.group(1).strip()
    args_no_trailing_nl = re.sub(r'\\n"', '"', args, count=1)
    return f"{severity_to_macro(severity)}({category_to_enum(category)}, {args_no_trailing_nl});"


def propose_call_fprintf(original_call, severity, category):
    """fprintf(stream, "fmt", args) -> LOG_*(LOG_CAT_*, "fmt", args). Drops the stream argument and strips trailing \\n."""
    m = re.match(r"\s*fprintf\s*\(\s*[A-Za-z_][A-Za-z_0-9]*\s*,\s*(.*)\)\s*;?\s*$", original_call, re.DOTALL)
    if not m:
        return None
    args = m.group(1).strip()
    args_no_trailing_nl = re.sub(r'\\n"', '"', args, count=1)
    return f"{severity_to_macro(severity)}({category_to_enum(category)}, {args_no_trailing_nl});"


def strip_c_comments(text):
    """Replace C // line and /* ... */ block comments with spaces (preserving line numbering and offsets). Skips comment-like sequences inside string and char literals."""
    out = []
    i = 0
    n = len(text)
    in_string = False
    in_char = False
    escape = False
    while i < n:
        c = text[i]
        if escape:
            out.append(c)
            escape = False
            i += 1
            continue
        if in_string:
            out.append(c)
            if c == "\\":
                escape = True
            elif c == '"':
                in_string = False
            i += 1
            continue
        if in_char:
            out.append(c)
            if c == "\\":
                escape = True
            elif c == "'":
                in_char = False
            i += 1
            continue
        if c == '"':
            in_string = True
            out.append(c)
            i += 1
            continue
        if c == "'":
            in_char = True
            out.append(c)
            i += 1
            continue
        if c == "/" and i + 1 < n and text[i + 1] == "/":
            # Line comment: replace with spaces until newline (preserve newline)
            j = i
            while j < n and text[j] != "\n":
                out.append(" ")
                j += 1
            i = j
            continue
        if c == "/" and i + 1 < n and text[i + 1] == "*":
            # Block comment: replace with spaces (preserve newlines for line numbering)
            j = i + 2
            out.append(" ")
            out.append(" ")
            while j < n - 1 and not (text[j] == "*" and text[j + 1] == "/"):
                out.append("\n" if text[j] == "\n" else " ")
                j += 1
            if j < n - 1:
                out.append(" ")
                out.append(" ")
                j += 2
            i = j
            continue
        out.append(c)
        i += 1
    return "".join(out)


def scan_file(path):
    items = []
    skipped = 0
    try:
        with open(path, "r", encoding="utf-8", errors="replace") as f:
            raw_text = f.read()
    except OSError:
        return items, skipped
    text = strip_c_comments(raw_text)

    # First pass: printf(...)
    pos = 0
    while True:
        m = PRINTF_RE.search(text, pos)
        if not m:
            break
        # Make sure we're not matching fprintf or sprintf
        prev_char_idx = m.start() - 1
        if prev_char_idx >= 0 and text[prev_char_idx] in ("f", "s", "v"):
            pos = m.end()
            continue
        open_paren = m.end() - 1
        call_text, end = extract_call(text, open_paren)
        if call_text is None:
            pos = open_paren + 1
            continue
        full_call = "printf" + call_text
        line_no = text.count("\n", 0, m.start()) + 1
        severity = guess_severity(full_call)
        category = guess_category(path)
        items.append({
            "file": path,
            "line_no": line_no,
            "kind": "printf",
            "original_call": full_call.strip(),
            "proposed_severity": severity,
            "proposed_category": category,
            "proposed_call": propose_call_printf(full_call, severity, category),
        })
        pos = end

    # Second pass: fprintf(stream, ...) — only if stream in {stderr, stdout}
    pos = 0
    while True:
        m = FPRINTF_RE.search(text, pos)
        if not m:
            break
        stream = m.group(1)
        if stream not in LOGGING_STREAMS:
            skipped += 1
            pos = m.end()
            continue
        # Locate the opening '(' of the fprintf call to extract its full balanced text.
        open_paren = text.find("(", m.start())
        call_text, end = extract_call(text, open_paren)
        if call_text is None:
            pos = open_paren + 1
            continue
        full_call = "fprintf" + call_text
        line_no = text.count("\n", 0, m.start()) + 1
        severity = guess_severity(full_call)
        category = guess_category(path)
        items.append({
            "file": path,
            "line_no": line_no,
            "kind": f"fprintf_{stream}",
            "original_call": full_call.strip(),
            "proposed_severity": severity,
            "proposed_category": category,
            "proposed_call": propose_call_fprintf(full_call, severity, category),
        })
        pos = end

    return items, skipped


def walk_paths(paths):
    files_scanned = 0
    items = []
    skipped_total = 0
    for root in paths:
        if os.path.isfile(root):
            if root.endswith((".c", ".cpp", ".cc", ".cxx")):
                files_scanned += 1
                file_items, file_skipped = scan_file(root)
                items.extend(file_items)
                skipped_total += file_skipped
            continue
        for dirpath, _dirnames, filenames in os.walk(root):
            for name in filenames:
                if name.endswith((".c", ".cpp", ".cc", ".cxx")):
                    files_scanned += 1
                    file_items, file_skipped = scan_file(os.path.join(dirpath, name))
                    items.extend(file_items)
                    skipped_total += file_skipped
    return files_scanned, items, skipped_total


def main():
    parser = argparse.ArgumentParser(description="Inventory printf and fprintf(stderr/stdout) call-sites for migration to LOG_*.")
    parser.add_argument("paths", nargs="+", help="Source paths to scan (files or directories)")
    args = parser.parse_args()

    files_scanned, items, skipped = walk_paths(args.paths)
    files_with_matches = len({i["file"] for i in items})

    report = {
        "scanned_paths": args.paths,
        "files_scanned": files_scanned,
        "files_with_matches": files_with_matches,
        "total_matches": len(items),
        "skipped_fprintf": skipped,
        "items": items,
    }
    json.dump(report, sys.stdout, indent=2)
    sys.stdout.write("\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
