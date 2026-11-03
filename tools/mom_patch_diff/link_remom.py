#!/usr/bin/env python3
"""
link_remom.py -- ties a join_addresses.py table to ReMoM's code: each procedure of the IDA database that ReMoM's ids
were taken from is looked up in remom_index.py's list by EXE, segment number and procedure number.

    python3 -m tools.mom_patch_diff.link_remom --exe WZD --joined wizards_joined.tsv \\
        --addresses wizards_addresses.tsv --index remom_index.tsv --out wizards_linked.tsv

The segment number comes from IDA's segment name (seg036 -> 36, ovr161 -> 161).  ReMoM ids use s for segNNN and o for
ovrNNN, but some are written with the wrong letter (WZD s161p04 is ovr161), so ids are looked up by number and a wrong
letter is reported in id_letter.  Segments renamed in the database (_AIL_, SOUND, dseg) carry no number and are not
linked, unless given one with --segment SOUND=38.  --addresses (that database's map_addresses.py table) supplies the segment names.

Output: the joined table's columns, then remom_id, remom_function, remom_file, remom_line, remom_evidence, and
id_letter ("s for an overlay" or "o for a segment" when ReMoM's comment uses the other letter).  Then one row per
ReMoM id that names no procedure of the database (match = "only in ReMoM").  Standard library only.
"""

import argparse
import csv
import re
import sys

NUMBERED = re.compile(r'(seg|ovr)0*(\d+)$', re.I)


def segment_numbers(addresses):
    """{IDA segment name: number} for segNNN and ovrNNN.  Renamed segments (_AIL_, SOUND, dseg) get none: IDA's numbers
    are not their position in address order (stubNNN and ovrNNN share NNN), so a renamed segment's number is unknown."""
    with open(addresses, newline='', encoding='utf-8') as f:
        names = {r['segment'] for r in csv.DictReader(f, delimiter='\t') if r['kind'] == 'S'}
    return {n: int(m.group(2)) for n in names for m in [NUMBERED.match(n)] if m}


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    p.add_argument('--exe', required=True, choices=['WZD', 'MGC'])
    p.add_argument('--joined', required=True, help='TSV from join_addresses.py')
    p.add_argument('--side', default='remom', help='the joined table side whose names ReMoM ids follow (default remom)')
    p.add_argument('--addresses', required=True, help="that side's map_addresses.py TSV (for the segment numbers)")
    p.add_argument('--index', required=True, help='TSV from remom_index.py')
    p.add_argument('--segment', action='append', default=[], metavar='NAME=NUMBER',
                   help='the number of a segment renamed in the database, e.g. SOUND=38 (repeatable)')
    p.add_argument('--out', required=True, help='TSV to write')
    args = p.parse_args(argv)

    numbers = segment_numbers(args.addresses)
    for spec in args.segment:
        name, _, number = spec.partition('=')
        numbers[name] = int(number)
    index = {}
    with open(args.index, newline='', encoding='utf-8') as f:
        for r in csv.DictReader(f, delimiter='\t'):
            if r['exe'] == args.exe:
                index.setdefault((int(r['segment']), int(r['proc'])), []).append(r)
    with open(args.joined, newline='', encoding='utf-8') as f:
        reader = csv.DictReader(f, delimiter='\t')
        columns = list(reader.fieldnames)
        joined = list(reader)
    extra = ['remom_id', 'remom_function', 'remom_file', 'remom_line', 'remom_evidence', 'id_letter']
    used, counts = set(), {'linked': 0, 'linked, ReMoM has code': 0, 'id letter differs': 0, 'only in ReMoM': 0}
    for row in joined:
        seg, proc = row.get(args.side + '_segment', ''), row.get(args.side + '_proc', '')
        if seg not in numbers or not proc:
            continue
        key = (numbers[seg], int(proc))
        hits = index.get(key)
        if not hits:
            continue
        used.add(key)
        row['remom_id'] = '; '.join(sorted({h['id'] for h in hits}))
        row['remom_function'] = '; '.join(h['function'] for h in hits if h['function'])
        row['remom_file'] = '; '.join(h['file'] for h in hits)
        row['remom_line'] = '; '.join(h['line'] for h in hits)
        row['remom_evidence'] = '; '.join(h['evidence'] for h in hits)
        overlay = seg.lower().startswith('ovr')
        letters = {h['letter'] for h in hits}
        if overlay and 's' in letters:
            row['id_letter'] = 's for an overlay'
        elif not overlay and 'o' in letters:
            row['id_letter'] = 'o for a segment'
        counts['linked'] += 1
        counts['linked, ReMoM has code'] += any(h['evidence'] != 'comment' for h in hits)
        counts['id letter differs'] += bool(row.get('id_letter'))
    only = []
    for key in sorted(set(index) - used):
        for h in index[key]:
            only.append({'match': 'only in ReMoM', 'remom_id': h['id'], 'remom_function': h['function'], 'remom_file': h['file'],
                         'remom_line': h['line'], 'remom_evidence': h['evidence']})
            counts['only in ReMoM'] += 1
    with open(args.out, 'w', newline='', encoding='utf-8') as f:
        w = csv.DictWriter(f, columns + extra, delimiter='\t', lineterminator='\n')
        w.writeheader()
        for row in joined + only:
            w.writerow({c: row.get(c, '') for c in columns + extra})
    print('link_remom: %s: %d procedures; %s' % (args.out, len(joined), ', '.join('%s %d' % kv for kv in counts.items())))
    return 0


if __name__ == '__main__':
    sys.exit(main())
