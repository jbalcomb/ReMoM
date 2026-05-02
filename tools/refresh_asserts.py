#!/usr/bin/env python3
"""
refresh_asserts.py — refresh the value side of an assertion file from a current
HeMoM save dump, preserving comments, blank lines, and the order/curation of
fields in the assertion file.

Why this exists
---------------
The integration tests (HeMoM_WorldGen_Fields, HeMoM_NextTurn_Assertions, etc.)
compare a curated subset of fields against expected values defined in
tests/assert_*.txt.  When game-state changes intentionally — world-gen tweak,
AI tweak, scenario change — the expected values drift out of date.  This tool
reads the current dump and emits a new assertion file with the same field
selection (and same comments) but updated right-hand sides.

What it does NOT do
-------------------
- Decide which fields are "meaningful."  The curation lives in the assertion
  file; this tool just refreshes values.
- Sanity-check the diff.  That's a human review step.  Inspect the result with
  `git diff -- tests/assert_*.txt`.
- Add new fields.  If the current code dumps fields that aren't in the
  assertion file, they're silently ignored.  That's intentional — the
  assertion file IS the spec.
- Drop assertions silently.  If the assertion file mentions a field that the
  current dump does NOT contain, the line is emitted with a clearly-marked
  placeholder ("MISSING_FROM_DUMP") and the original value preserved as a
  comment so the diff makes the loss obvious.

Usage
-----
    python tools/refresh_asserts.py \\
        --dump=path/to/SAVE9.txt \\
        --asserts=tests/assert_worldgen.txt \\
        [--in-place]

Without --in-place, the refreshed file is written to stdout (useful for
piping to a diff or eyeballing before committing).
"""

import argparse
import re
import sys


# Match "key = value" lines.  The key is anything up to the first '=' that
# doesn't start with '#'.  The value is everything after, trimmed.
_KV_RE = re.compile(r'^(\s*)([^=#][^=]*?)\s*=\s*(.*?)\s*$')


def parse_dump(path):
    """Load a dump.txt into a dict mapping key -> last-seen value."""
    out = {}
    with open(path, 'r', encoding='latin-1') as fp:
        for raw in fp:
            line = raw.rstrip('\n')
            stripped = line.lstrip()
            if not stripped or stripped.startswith('#'):
                continue
            m = _KV_RE.match(line)
            if not m:
                continue
            key = m.group(2).strip()
            val = m.group(3).strip()
            out[key] = val
    return out


def detect_line_ending(path):
    """Return '\\r\\n' or '\\n' depending on what the file uses."""
    with open(path, 'rb') as fp:
        chunk = fp.read(8192)
    return '\r\n' if b'\r\n' in chunk else '\n'


def refresh_lines(asserts_path, dump):
    """Yield refreshed lines from an assertion file."""
    with open(asserts_path, 'r', encoding='latin-1') as fp:
        for raw in fp:
            line = raw.rstrip('\r\n')
            stripped = line.lstrip()
            # Pass-through: comments and blank lines.
            if not stripped or stripped.startswith('#'):
                yield line
                continue
            m = _KV_RE.match(line)
            if not m:
                # Non-KV non-comment line — leave it alone.
                yield line
                continue
            indent = m.group(1)
            key = m.group(2).strip()
            old_val = m.group(3).strip()
            if key in dump:
                new_val = dump[key]
                yield f'{indent}{key} = {new_val}'
            else:
                # Key not present in current dump.  Don't lose it silently;
                # mark it loudly so review catches it.
                yield (f'{indent}{key} = MISSING_FROM_DUMP'
                       f'  # was: {old_val}')


def main(argv=None):
    ap = argparse.ArgumentParser(
        description=('Refresh expected values in an assertion file from a '
                     'current HeMoM save dump.'))
    ap.add_argument('--dump', required=True,
                    help='Path to the .txt dump (savedump output / '
                         '--dump-save .txt result).')
    ap.add_argument('--asserts', required=True,
                    help='Path to the assertion file to refresh.')
    ap.add_argument('--in-place', action='store_true',
                    help='Rewrite the asserts file in place. '
                         'Without this, output goes to stdout.')
    args = ap.parse_args(argv)

    dump = parse_dump(args.dump)
    eol = detect_line_ending(args.asserts)
    refreshed = eol.join(refresh_lines(args.asserts, dump)) + eol

    # Encode FIRST so that a bad-byte error doesn't leave us with the target
    # file already truncated open for write.  Only after encoding succeeds do
    # we touch the destination.
    data = refreshed.encode('latin-1')

    if args.in_place:
        with open(args.asserts, 'wb') as fp:
            fp.write(data)
    else:
        # Write raw bytes to stdout so non-ASCII content (UTF-8 em-dashes in
        # comments, garbage bytes from uninit memory in dump strings) survives
        # without the cp1252 default-encoding choking on Windows.
        sys.stdout.buffer.write(data)

    return 0


if __name__ == '__main__':
    sys.exit(main())
