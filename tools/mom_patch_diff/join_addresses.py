#!/usr/bin/env python3
"""
join_addresses.py -- joins two map_addresses.py tables of the same .EXE (two IDA databases of it, e.g. drake178's and
ReMoM's own) procedure by procedure, on the file offset.

    python3 -m tools.mom_patch_diff.join_addresses --left remom=wizards_addresses.tsv \\
        --right drake178=drake_wizards_addresses.tsv --out wizards_joined.tsv

A procedure is matched on its file offset.  One without a single file offset (too short to place, not found, or
found at several places) is matched on its IDA address, which is the same in both databases when both loaded the .EXE at the same base.

Columns: match (file offset / ida address / only <left> / only <right>), file_offset, then for each side: segment,
proc, name, ida_ea, ida_seg_off, size; then names (same / differ / blank when either side has IDA's default sub_ name).
Standard library only.
"""

import argparse
import csv
import re
import sys

SIDE = ['segment', 'proc', 'name', 'ida_ea', 'ida_seg_off', 'size']
DEFAULT_NAME = re.compile(r'(sub|nullsub|unknown_libname|loc|j)_[0-9A-F]+$', re.I)


def load(path):
    with open(path, newline='', encoding='utf-8') as f:
        return [r for r in csv.DictReader(f, delimiter='\t') if r['kind'] == 'F']


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    p.add_argument('--left', required=True, help='NAME=TSV from map_addresses.py')
    p.add_argument('--right', required=True, help='NAME=TSV from map_addresses.py')
    p.add_argument('--out', required=True, help='TSV to write')
    args = p.parse_args(argv)
    (ln, _, lpath), (rn, _, rpath) = args.left.partition('='), args.right.partition('=')
    left, right = load(lpath), load(rpath)

    def unique_offset(row):
        """The file offset, when it names one place (not "bytes, N places")."""
        return row['file_offset'] if row['file_offset'] and not row['file_offset_from'].startswith('bytes, ') else ''

    right_by = {}
    for r in right:
        if unique_offset(r):
            right_by.setdefault(('file', unique_offset(r)), r)
        right_by.setdefault(('ea', r['ida_ea']), r)
    used, rows = set(), []
    for l in left:
        match, r = None, None
        for how, k in (('file offset', ('file', unique_offset(l))), ('ida address', ('ea', l['ida_ea']))):
            candidate = right_by.get(k) if k[1] else None
            if candidate is not None and id(candidate) not in used:
                match, r = how, candidate
                break
        row = {'match': match or 'only ' + ln, 'file_offset': l['file_offset']}
        row.update({ln + '_' + c: l[c] for c in SIDE})
        if r is not None:
            used.add(id(r))
            row.update({rn + '_' + c: r[c] for c in SIDE})
            if not (DEFAULT_NAME.match(l['name']) or DEFAULT_NAME.match(r['name'])):
                row['names'] = 'same' if l['name'] == r['name'] else 'differ'
        rows.append(row)
    for r in right:
        if id(r) not in used:
            row = {'match': 'only ' + rn, 'file_offset': r['file_offset']}
            row.update({rn + '_' + c: r[c] for c in SIDE})
            rows.append(row)

    columns = ['match', 'file_offset'] + [ln + '_' + c for c in SIDE] + [rn + '_' + c for c in SIDE] + ['names']
    with open(args.out, 'w', newline='', encoding='utf-8') as f:
        w = csv.DictWriter(f, columns, delimiter='\t', lineterminator='\n')
        w.writeheader()
        for row in rows:
            w.writerow({c: row.get(c, '') for c in columns})
    count = lambda key, value: sum(1 for r in rows if r.get(key) == value)
    print('join_addresses: %s: %d rows; matched on file offset %d, on IDA address %d, only %s %d, only %s %d; '
          'names same %d, differ %d' % (args.out, len(rows), count('match', 'file offset'), count('match', 'ida address'),
                                        ln, count('match', 'only ' + ln), rn, count('match', 'only ' + rn),
                                        count('names', 'same'), count('names', 'differ')))
    return 0


if __name__ == '__main__':
    sys.exit(main())
