#!/usr/bin/env python3
"""
find_stable_fields.py — like refresh_asserts.py, but takes MULTIPLE dumps from
independent runs of the same scenario and emits an assertion file in which
only fields with the SAME value across every dump are asserted.  Fields that
varied across runs are commented out with a NONDET marker recording what was
observed, so the assertion file documents what was set aside and why.

Why this exists
---------------
Some fields in the save format depend on uninitialized memory (struct padding,
unused trailing slots) or on AI logic that reads run-to-run-varying state.
Asserting their values produces a flaky test.  Running the scenario several
times and asserting only fields whose values are stable across all runs lets
us catch real regressions without false positives from non-determinism.

Output for stable fields:
    field.name = value

Output for unstable (nondeterministic) fields:
    # NONDET field.name — observed values: A, B, C  (set aside until determinism is restored)
    # field.name = A

Output for fields missing from one or more dumps:
    field.name = MISSING_FROM_DUMP  # was: <original value>

Usage
-----
    python tools/find_stable_fields.py \\
        --dumps=run1.txt,run2.txt,run3.txt,run4.txt,run5.txt \\
        --asserts=tests/assert_continue_next_turn.txt \\
        [--in-place]
"""

import argparse
import re
import sys


_KV_RE = re.compile(r'^(\s*)([^=#][^=]*?)\s*=\s*(.*?)\s*$')


def parse_dump(path):
    """Load a dump.txt into a dict mapping key -> last-seen value."""
    out = {}
    with open(path, 'r', encoding='latin-1') as fp:
        for raw in fp:
            line = raw.rstrip('\n').rstrip('\r')
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
    with open(path, 'rb') as fp:
        chunk = fp.read(8192)
    return '\r\n' if b'\r\n' in chunk else '\n'


def refresh_lines(asserts_path, dumps):
    """Yield refreshed lines.  dumps is a list of {key: value} dicts.

    Stats are accumulated and returned via a closure so the caller can print
    a summary; we use a list to side-channel that out of the generator.
    """
    stats = {'stable': 0, 'nondet': 0, 'missing': 0, 'passthrough': 0}

    def gen():
        with open(asserts_path, 'r', encoding='latin-1') as fp:
            for raw in fp:
                line = raw.rstrip('\n').rstrip('\r')
                stripped = line.lstrip()
                if not stripped or stripped.startswith('#'):
                    stats['passthrough'] += 1
                    yield line
                    continue
                m = _KV_RE.match(line)
                if not m:
                    stats['passthrough'] += 1
                    yield line
                    continue
                indent = m.group(1)
                key = m.group(2).strip()
                old_val = m.group(3).strip()

                values = [d.get(key) for d in dumps]
                if any(v is None for v in values):
                    stats['missing'] += 1
                    yield (f'{indent}{key} = MISSING_FROM_DUMP'
                           f'  # was: {old_val}')
                elif len(set(values)) == 1:
                    stats['stable'] += 1
                    yield f'{indent}{key} = {values[0]}'
                else:
                    stats['nondet'] += 1
                    sample = ', '.join(values)
                    yield (f'{indent}# NONDET {key} '
                           f'-- observed values across {len(dumps)} runs: '
                           f'{sample}  '
                           f'(set aside until determinism is restored)')
                    yield f'{indent}# {key} = {values[0]}'

    return gen, stats


def main(argv=None):
    ap = argparse.ArgumentParser(
        description=('Refresh an assertion file from N independent dumps; '
                     'only fields stable across all N are asserted, and '
                     'nondet fields are commented out with a marker.'))
    ap.add_argument('--dumps', required=True,
                    help='Comma-separated list of dump.txt paths from '
                         'independent runs (>=2; >=5 recommended).')
    ap.add_argument('--asserts', required=True,
                    help='Path to the assertion file to refresh.')
    ap.add_argument('--in-place', action='store_true',
                    help='Rewrite the asserts file in place.')
    args = ap.parse_args(argv)

    dump_paths = [p.strip() for p in args.dumps.split(',') if p.strip()]
    if len(dump_paths) < 2:
        sys.stderr.write('ERROR: --dumps requires at least 2 paths '
                         '(>=5 recommended for confidence).\n')
        return 2

    dumps = [parse_dump(p) for p in dump_paths]

    eol = detect_line_ending(args.asserts)
    gen, stats = refresh_lines(args.asserts, dumps)
    refreshed = eol.join(gen()) + eol

    # Encode FIRST so that a bad-byte error doesn't leave us with the target
    # file already truncated open for write.  Only after encoding succeeds do
    # we touch the destination.
    data = refreshed.encode('latin-1')

    if args.in_place:
        with open(args.asserts, 'wb') as fp:
            fp.write(data)
    else:
        sys.stdout.buffer.write(data)

    sys.stderr.write(
        f'[find_stable_fields] dumps={len(dump_paths)}  '
        f'stable={stats["stable"]}  nondet={stats["nondet"]}  '
        f'missing={stats["missing"]}  passthrough={stats["passthrough"]}\n')
    return 0


if __name__ == '__main__':
    sys.exit(main())
