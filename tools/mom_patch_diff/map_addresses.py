#!/usr/bin/env python3
"""
map_addresses.py -- one TSV row per segment and procedure of an IDA database (from an export_map.py map), with its
address in IDA and in the .EXE file.

    python3 -m tools.mom_patch_diff.map_addresses --map drake_wizards_map.tsv --exe /c/MoM/MoM131/WIZARDS.EXE \\
        --out drake_wizards_addresses.tsv

Columns:
    kind            S (segment) or F (procedure)
    segment         IDA segment name, as in that database (seg161, ovr142, _AIL_, dseg, ...)
    proc            procedure number in its segment, 1-based (0 for a segment)
    name            IDA name
    size            bytes
    ida_ea          IDA linear address (hex)
    ida_seg_off     IDA segment name : offset in the segment (seg161:0005)
    ida_para_off    IDA paragraph : offset (1A00:0005); paragraph = segment start / 16
    exe_para_off    the same paragraph relative to the start of the load module, as the .EXE sees it (0A00:0005);
                    blank for overlays, which are not part of the load module
    file_offset     offset in the .EXE file (0x...)
    file_offset_from how file_offset was found:
                    "load module"           header size + (ida_ea - load module start), and the bytes there match
                    "bytes (computed 0x...)" computed the same way, but the bytes there do not match, so it was found
                                            by its bytes instead; <out>_segments.tsv shows, per segment, how far off
                                            the computation was
                    "load module, differs"  the bytes match neither at the computed offset nor anywhere else: the
                                            database is not of this .EXE, or the procedure was patched
                    "bytes"                 found by its bytes (overlays), at one place
                    "bytes, N places"       found at N places; the first is given
                    "not found"             its bytes are nowhere in the file
                    "too short"             too few known bytes to place by search

Two databases of the same .EXE (drake178's and ReMoM's own) give the same file_offset for the same procedure, whatever
their segments are named or numbered, so file_offset is the column to join them on.  Standard library only.
"""

import argparse
import csv
import os
import struct
import sys
from collections import Counter, defaultdict

from tools.mom_patch_diff.diff import load_map

COLUMNS = ['kind', 'segment', 'proc', 'name', 'size', 'ida_ea', 'ida_seg_off', 'ida_para_off', 'exe_para_off',
           'file_offset', 'file_offset_from']


def load_module(exe):
    """(header size, load module size) of an MZ file."""
    if exe[:2] not in (b'MZ', b'ZM'):
        sys.exit('map_addresses: not an MZ executable')
    last, pages, _, header_paragraphs = struct.unpack_from('<4H', exe, 2)
    image_end = pages * 512 - ((512 - last) if last else 0)
    header = header_paragraphs * 16
    return header, image_end - header


def rows_for(items, exe):
    segments = sorted((i for i in items if i.kind == 'S'), key=lambda s: s.ea)
    if not segments:
        sys.exit('map_addresses: the map has no segment (S) rows')
    load_ea = segments[0].ea
    header, module_size = load_module(exe) if exe is not None else (0, 0)
    seg_start = {s.name: s.ea for s in segments}
    check = defaultdict(Counter)            # segment -> how its procedures' file offsets were found
    rows = []
    for item in sorted((i for i in items if i.kind in ('S', 'F')), key=lambda i: (i.ea, i.kind != 'S')):
        start = seg_start.get(item.segment, item.ea)
        para, offset = start >> 4, item.ea - (start & ~0xF)
        in_module = exe is not None and load_ea <= item.ea < load_ea + module_size
        row = {
            'kind': item.kind, 'segment': item.segment, 'proc': item.proc, 'name': item.name, 'size': item.size,
            'ida_ea': '%X' % item.ea, 'ida_seg_off': '%s:%04X' % (item.segment, item.ea - start),
            'ida_para_off': '%04X:%04X' % (para, offset),
            'exe_para_off': ('%04X:%04X' % (para - (load_ea >> 4), offset)) if in_module else '',
        }
        if exe is None:
            pass
        elif in_module:
            file_offset = header + item.ea - load_ea
            row['file_offset'] = '0x%06X' % file_offset
            if item.kind == 'S' or not item.runs or item.matches_at(exe, file_offset):
                row['file_offset_from'] = 'load module'
                if item.kind == 'F':
                    check[item.segment]['computed'] += 1
            else:
                hits = item.find(exe)
                if hits and len(hits) == 1:
                    row['file_offset'] = '0x%06X' % hits[0]
                    row['file_offset_from'] = 'bytes (computed 0x%06X)' % file_offset
                    check[item.segment]['found by bytes'] += 1
                    check[item.segment]['shift %+X' % (hits[0] - file_offset)] += 1
                else:
                    row['file_offset_from'] = 'load module, differs'
                    check[item.segment]['differs, not found'] += 1
        elif item.kind == 'F':
            hits = item.find(exe)
            if hits is None:
                row['file_offset_from'] = 'too short'
            elif not hits:
                row['file_offset_from'] = 'not found'
            else:
                row['file_offset'] = '0x%06X' % hits[0]
                row['file_offset_from'] = 'bytes' if len(hits) == 1 else 'bytes, %d places' % len(hits)
        rows.append(row)
    return rows, check, (load_ea, header, module_size)


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    p.add_argument('--map', required=True, help='TSV written by export_map.py')
    p.add_argument('--exe', help='the .EXE the database was made from; without it the file columns stay blank')
    p.add_argument('--out', required=True, help='TSV to write')
    args = p.parse_args(argv)

    exe = None
    if args.exe:
        with open(args.exe, 'rb') as f:
            exe = f.read()
    items = load_map(args.map)
    rows, check, (load_ea, header, module_size) = rows_for(items, exe)
    with open(args.out, 'w', newline='', encoding='utf-8') as f:
        w = csv.DictWriter(f, COLUMNS, delimiter='\t', lineterminator='\n')
        w.writeheader()
        for row in rows:
            w.writerow({c: row.get(c, '') for c in COLUMNS})
    if exe is not None:
        seg_ea = {i.name: i.ea for i in items if i.kind == 'S'}
        path = os.path.splitext(args.out)[0] + '_segments.tsv'
        with open(path, 'w', newline='', encoding='utf-8') as f:
            w = csv.writer(f, delimiter='\t', lineterminator='\n')
            w.writerow(['segment', 'ida_ea', 'computed_ok', 'found_by_bytes', 'differs_not_found', 'shifts'])
            for seg in sorted(check, key=lambda s: seg_ea.get(s, 0)):
                c = check[seg]
                shifts = ', '.join('%s x%d' % (k[6:], v) for k, v in c.most_common() if k.startswith('shift '))
                w.writerow([seg, '%X' % seg_ea.get(seg, 0), c['computed'], c['found by bytes'], c['differs, not found'], shifts])
        print('map_addresses: load module: IDA %X-%X, header 0x%X bytes, %d bytes long; per-segment check in %s'
              % (load_ea, load_ea + module_size, header, module_size, path))
        bad = [s for s in sorted(check, key=lambda s: seg_ea.get(s, 0)) if check[s]['found by bytes'] or check[s]['differs, not found']]
        for seg in bad[:25]:
            c = check[seg]
            print('  %-10s %6X  computed ok %3d, by bytes %3d, not found %3d  %s' % (
                seg, seg_ea.get(seg, 0), c['computed'], c['found by bytes'], c['differs, not found'],
                ', '.join('%s x%d' % (k[6:], v) for k, v in c.most_common(3) if k.startswith('shift '))))
        if len(bad) > 25:
            print('  ... and %d more segments; see %s' % (len(bad) - 25, path))
    procs = [r for r in rows if r['kind'] == 'F']
    counts = {}
    for r in procs:
        how = r.get('file_offset_from', '') or '(no --exe)'
        how = 'bytes, several places' if how.startswith('bytes, ') else how
        how = 'bytes (computed was wrong)' if how.startswith('bytes (computed') else how
        counts[how] = counts.get(how, 0) + 1
    print('map_addresses: %s: %d segments, %d procedures; file offset: %s' % (
        args.out, len(rows) - len(procs), len(procs), ', '.join('%s %d' % kv for kv in sorted(counts.items()))))
    return 0


if __name__ == '__main__':
    sys.exit(main())
