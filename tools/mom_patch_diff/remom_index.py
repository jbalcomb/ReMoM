#!/usr/bin/env python3
"""
remom_index.py -- ReMoM's own list of procedures: every id comment ("// WZD s161p04") in its sources, with the C function
that reconstructs it, its file and line.

    python3 -m tools.mom_patch_diff.remom_index --remom . --out remom_index.tsv

Columns: exe (WZD, MGC), id, letter (s or o, as written), segment (number), proc, function, file, line, evidence.
evidence: definition (the next function definition after the comment), prototype (a prototype after the comment names
the function), comment (an id with no code yet); a trailing "?" means another id of the same EXE claims the same
function.  Standard library only.
"""

import argparse
import csv
import sys

from tools.mom_patch_diff.address_table import key_text, scan_remom

COLUMNS = ['exe', 'id', 'letter', 'segment', 'proc', 'function', 'file', 'line', 'evidence']


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    p.add_argument('--remom', default='.', help='the ReMoM checkout')
    p.add_argument('--out', required=True, help='TSV to write')
    args = p.parse_args(argv)
    found = scan_remom(args.remom)
    with open(args.out, 'w', newline='', encoding='utf-8') as f:
        w = csv.writer(f, delimiter='\t', lineterminator='\n')
        w.writerow(COLUMNS)
        for k in sorted(found):
            for fn, path, line, evidence in found[k]:
                w.writerow([k[0], key_text(k), k[1], k[2], k[3], fn, path, line, evidence])
    print('remom_index: %s: %d ids' % (args.out, len(found)))
    return 0


if __name__ == '__main__':
    sys.exit(main())
