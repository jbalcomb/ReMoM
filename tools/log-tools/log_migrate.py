#!/usr/bin/env python3
"""
log_migrate.py - inventory dbg_prn / trc_prn call-sites and propose LOG_* replacements.

Scans one or more source paths for calls to dbg_prn(...) or trc_prn(...),
extracts each call's text (handling balanced parens across one logical
statement), applies severity and category heuristics, and emits a structured
JSON inventory on stdout. NEVER writes to any source file — output is a
proposal for human (or agent) review.

Usage:
    python log_migrate.py <path> [<path>...]
        e.g.  python log_migrate.py MoM/src STU/src platform

Output schema (top-level):
    scanned_paths     list of paths walked
    files_scanned     count of .c / .cpp files visited
    files_with_matches count of files containing at least one match
    total_matches     total number of call-sites found
    items             list of {file, line_no, original_call, proposed_severity,
                              proposed_category, proposed_call}

Severity heuristic (case-insensitive scan of the first string-literal arg):
    "FATAL" or "abort" or "die"                           -> FATAL
    "error" or "fail" or "unable" or "cannot"             -> ERROR
    "warn" or "unexpected" or "should not"                -> WARN
    (default)                                             -> DEBUG

Category heuristic (filename first, then path components):
    AIDATA.c / AIDUDES.c / AIMOVE.c / AISPELL.c / AI_*.c  -> AIMOVE
    Combat.c                                              -> COMBAT
    LOADSAVE.c, files matching Save_/Load_                -> SAVE
    Fields.c                                              -> IKI
    any path containing 'platform/'                       -> PFL
    (default)                                             -> GENERAL
"""

import argparse
import json
import os
import re
import sys


CALL_RE = re.compile(r"\b(dbg_prn|trc_prn)\s*\(")
STRING_LIT_RE = re.compile(r'"((?:[^"\\]|\\.)*)"')

SEVERITY_PATTERNS = [
    ("FATAL", [r"\bFATAL\b", r"\babort\b", r"\bdie\b"]),
    ("ERROR", [r"\berror\b", r"\bfail\b", r"\bunable\b", r"\bcannot\b"]),
    ("WARN",  [r"\bwarn\b", r"\bunexpected\b", r"\bshould not\b"]),
]
SEVERITY_COMPILED = [(sev, [re.compile(p, re.IGNORECASE) for p in pats]) for sev, pats in SEVERITY_PATTERNS]


def extract_call(text, start):
    """Given text and the index of the opening '(' of a call, return (call_text, end_index) covering the matched ')'. Returns (None, start) if unbalanced."""
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
    return "DEBUG"


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
            j = i
            while j < n and text[j] != "\n":
                out.append(" ")
                j += 1
            i = j
            continue
        if c == "/" and i + 1 < n and text[i + 1] == "*":
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


def propose_call(original_call, severity, category):
    """Turn 'dbg_prn("FATAL: x\\n");' into 'LOG_FATAL(LOG_CAT_..., "FATAL: x");'. Strips trailing '\\n' from the format string because LOG_* macros add their own newline."""
    inner_match = re.match(r"\s*(?:dbg_prn|trc_prn)\s*\((.*)\)\s*;?\s*$", original_call, re.DOTALL)
    if not inner_match:
        return None
    args = inner_match.group(1).strip()
    args_no_trailing_nl = re.sub(r'\\n"', '"', args, count=1)
    return f"{severity_to_macro(severity)}({category_to_enum(category)}, {args_no_trailing_nl});"


def scan_file(path):
    items = []
    try:
        with open(path, "r", encoding="utf-8", errors="replace") as f:
            raw_text = f.read()
    except OSError:
        return items
    text = strip_c_comments(raw_text)

    pos = 0
    while True:
        m = CALL_RE.search(text, pos)
        if not m:
            break
        open_paren = m.end() - 1
        call_text, end = extract_call(text, open_paren)
        if call_text is None:
            pos = open_paren + 1
            continue
        full_call = m.group(0) + call_text[1:]  # `dbg_prn(...)`
        line_no = text.count("\n", 0, m.start()) + 1
        severity = guess_severity(full_call)
        category = guess_category(path)
        proposed = propose_call(full_call, severity, category)
        items.append({
            "file": path,
            "line_no": line_no,
            "original_call": full_call.strip(),
            "proposed_severity": severity,
            "proposed_category": category,
            "proposed_call": proposed,
        })
        pos = end
    return items


def walk_paths(paths):
    files_scanned = 0
    items = []
    for root in paths:
        if os.path.isfile(root):
            if root.endswith((".c", ".cpp", ".cc", ".cxx")):
                files_scanned += 1
                items.extend(scan_file(root))
            continue
        for dirpath, _dirnames, filenames in os.walk(root):
            for name in filenames:
                if name.endswith((".c", ".cpp", ".cc", ".cxx")):
                    files_scanned += 1
                    items.extend(scan_file(os.path.join(dirpath, name)))
    return files_scanned, items


def main():
    parser = argparse.ArgumentParser(description="Inventory dbg_prn / trc_prn call-sites and propose LOG_* replacements.")
    parser.add_argument("paths", nargs="+", help="Source paths to scan (files or directories)")
    args = parser.parse_args()

    files_scanned, items = walk_paths(args.paths)
    files_with_matches = len({i["file"] for i in items})

    report = {
        "scanned_paths": args.paths,
        "files_scanned": files_scanned,
        "files_with_matches": files_with_matches,
        "total_matches": len(items),
        "items": items,
    }
    json.dump(report, sys.stdout, indent=2)
    sys.stdout.write("\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
