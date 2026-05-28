#!/usr/bin/env python3
"""
log_apply.py - mechanically apply dbg_prn / trc_prn / printf / fprintf -> LOG_*
migration to a single source file with a single target category.

This is the "applier" half of the Phase 8 toolchain. The "inventory" half
(log_migrate.py / log_migrate_io.py) catalogs candidates; this script
actually rewrites them. Edit-capable; runs against ONE file at a time so
each invocation produces a small reviewable diff.

Usage:
    py log_apply.py <file.c> <original_macro> <log_macro> <log_category>

        original_macro: dbg_prn | trc_prn | printf | fprintf_stderr | fprintf_stdout
        log_macro:      LOG_TRACE | LOG_DEBUG | LOG_INFO | LOG_WARN | LOG_ERROR | LOG_FATAL
        log_category:   LOG_CAT_GENERAL | LOG_CAT_AIMOVE | ... etc.

    Example:
        py log_apply.py MoM/src/Combat.c dbg_prn LOG_DEBUG LOG_CAT_COMBAT

Transformations applied:
    dbg_prn("fmt\\n", args)            -> LOG_DEBUG(LOG_CAT_X, "fmt", args)
    trc_prn("fmt\\n", args)            -> LOG_TRACE(LOG_CAT_X, "fmt", args)
    printf("fmt\\n", args)             -> LOG_INFO(LOG_CAT_X, "fmt", args)
    fprintf(stderr, "fmt\\n", args)    -> LOG_INFO(LOG_CAT_X, "fmt", args)
    fprintf(stdout, "fmt\\n", args)    -> LOG_INFO(LOG_CAT_X, "fmt", args)

Strips trailing "\\n" from the format string (LOG_* adds its own newline).
Preserves any /* CLAUDE */ leading marker, surrounding #ifdef wrappers, and
all other code unchanged.

SAFETY:
- Uses paren-balanced extraction (handles embedded strings with escapes).
- Skips matches inside C // and /* */ comments.
- Reports each transformation on stdout.
- Reports any matches it could NOT transform (e.g., unbalanced parens,
  unusual whitespace) for human follow-up.
- Writes the file in place. Use git to review/revert.
"""

import re
import sys


STRING_LIT_RE = re.compile(r'"((?:[^"\\]|\\.)*)"')


def strip_c_comments(text):
    """Replace C // line and /* */ block comments with spaces (preserves line numbering and byte offsets). String/char literals are NOT stripped."""
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


def find_call(text, search_start, call_re):
    """Find the next call matching call_re starting at search_start. Returns (match_start, open_paren, close_paren) or None if no match. Walks balanced parens through strings/chars."""
    m = call_re.search(text, search_start)
    if not m:
        return None
    open_paren = m.end() - 1  # the '(' is the last char of the match
    depth = 0
    in_string = False
    in_char = False
    escape = False
    i = open_paren
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
                return (m.start(), open_paren, i)
        i += 1
    return None


def transform_dbgtrc(original_call, log_macro, log_category):
    """dbg_prn("fmt\\n", args)  ->  LOG_DEBUG(LOG_CAT_X, "fmt", args)"""
    m = re.match(r"\s*(?:dbg_prn|trc_prn)\s*\((.*)\)\s*$", original_call, re.DOTALL)
    if not m:
        return None
    inner = m.group(1).strip()
    inner_no_nl = re.sub(r'\\n"', '"', inner, count=1)
    return f"{log_macro}({log_category}, {inner_no_nl})"


def transform_printf(original_call, log_macro, log_category):
    """printf("fmt\\n", args)  ->  LOG_INFO(LOG_CAT_X, "fmt", args)"""
    m = re.match(r"\s*printf\s*\((.*)\)\s*$", original_call, re.DOTALL)
    if not m:
        return None
    inner = m.group(1).strip()
    inner_no_nl = re.sub(r'\\n"', '"', inner, count=1)
    return f"{log_macro}({log_category}, {inner_no_nl})"


def transform_fprintf(original_call, log_macro, log_category):
    """fprintf(stream, "fmt\\n", args)  ->  LOG_INFO(LOG_CAT_X, "fmt", args)"""
    m = re.match(r"\s*fprintf\s*\(\s*[A-Za-z_][A-Za-z_0-9]*\s*,\s*(.*)\)\s*$", original_call, re.DOTALL)
    if not m:
        return None
    inner = m.group(1).strip()
    inner_no_nl = re.sub(r'\\n"', '"', inner, count=1)
    return f"{log_macro}({log_category}, {inner_no_nl})"


def fprintf_call_filter(original_call, expected_stream, log_macro, log_category):
    """Filter fprintf calls by stream identifier (stderr or stdout) before transforming. Returns None for other streams so the site is skipped silently."""
    m = re.match(r"\s*fprintf\s*\(\s*([A-Za-z_][A-Za-z_0-9]*)\s*,", original_call)
    if not m or m.group(1) != expected_stream:
        return None
    return transform_fprintf(original_call, log_macro, log_category)


def main():
    if len(sys.argv) != 5:
        sys.stderr.write(__doc__)
        return 2
    file_path     = sys.argv[1]
    original      = sys.argv[2]
    log_macro     = sys.argv[3]
    log_category  = sys.argv[4]

    valid_originals = {"dbg_prn", "trc_prn", "printf", "fprintf_stderr", "fprintf_stdout"}
    if original not in valid_originals:
        sys.stderr.write(f"log_apply: original must be one of {sorted(valid_originals)}\n")
        return 2

    with open(file_path, "r", encoding="utf-8", errors="replace") as f:
        raw_text = f.read()
    stripped = strip_c_comments(raw_text)

    if original == "dbg_prn":
        call_re = re.compile(r"\bdbg_prn\s*\(")
        transform = lambda c: transform_dbgtrc(c, log_macro, log_category)
    elif original == "trc_prn":
        call_re = re.compile(r"\btrc_prn\s*\(")
        transform = lambda c: transform_dbgtrc(c, log_macro, log_category)
    elif original == "printf":
        call_re = re.compile(r"(?<![a-zA-Z_])printf\s*\(")
        transform = lambda c: transform_printf(c, log_macro, log_category)
    elif original == "fprintf_stderr":
        # Regex must match through '(' only so find_call's paren-balancer sees a clean
        # opening paren. Stream filtering happens in the transform via fprintf_call_filter.
        call_re = re.compile(r"\bfprintf\s*\(")
        transform = lambda c: fprintf_call_filter(c, "stderr", log_macro, log_category)
    elif original == "fprintf_stdout":
        call_re = re.compile(r"\bfprintf\s*\(")
        transform = lambda c: fprintf_call_filter(c, "stdout", log_macro, log_category)

    # Find all real (non-comment) call sites in the stripped text.
    sites = []
    pos = 0
    while True:
        found = find_call(stripped, pos, call_re)
        if found is None:
            break
        sites.append(found)
        pos = found[2] + 1

    # Apply transformations on raw_text, from last to first (preserves offsets).
    applied = 0
    skipped = []
    new_text = raw_text
    for match_start, open_paren, close_paren in reversed(sites):
        original_call = raw_text[match_start:close_paren + 1]
        replacement = transform(original_call)
        if replacement is None:
            line_no = raw_text.count("\n", 0, match_start) + 1
            skipped.append((line_no, original_call[:80]))
            continue
        new_text = new_text[:match_start] + replacement + new_text[close_paren + 1:]
        applied += 1

    if applied > 0:
        with open(file_path, "w", encoding="utf-8", newline="") as f:
            f.write(new_text)

    print(f"applied: {applied}   skipped: {len(skipped)}   file: {file_path}")
    for line_no, snippet in skipped:
        print(f"  skipped {file_path}:{line_no}  {snippet}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
