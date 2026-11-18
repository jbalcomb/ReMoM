#!/usr/bin/env python3
"""tools/bq_locate.py -- map every item in binaryalgorithm's BUGS_AND_QUIRKS.txt to the IDA procedure and the ReMoM function.

Their addresses (e.g. af2c:12bf) use the same segment bases as the IDA 5.5 databases, so each address is looked up in the
Piethawn procedure inventories (start_ea <= ea < end_ea). The IDA procedure name is then looked up as a C function definition
in the ReMoM source.

Usage:
  python3 tools/bq_locate.py                      # writes doc/@CodeReview/Bugs-And-Quirks-Locations.csv, prints a summary
  python3 tools/bq_locate.py --item 37            # print one item's mapping
  python3 tools/bq_locate.py --groups             # print pending items grouped by ReMoM file (for hand-off planning)
"""
import argparse
import csv
import json
import os
import re
import sys
from collections import defaultdict

REPO = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
PIETHAWN = os.environ.get('PIETHAWN_OUT', '/c/STU/devel/STU-Extras/Piethawn/Piethawn/out')
if os.name == 'nt' and PIETHAWN.startswith('/c/'):
    PIETHAWN = 'C:/' + PIETHAWN[3:]
BUGS = os.path.join(REPO, 'doc', '@TODO', 'BUGS_AND_QUIRKS.txt')
TRIAGE = os.path.join(REPO, 'doc', '@CodeReview', 'Bugs-And-Quirks-Triage.md')
OUT_CSV = os.path.join(REPO, 'doc', '@CodeReview', 'Bugs-And-Quirks-Locations.csv')
SRC_DIRS = ['MoM/src', 'MoX/src', 'src', 'STU/src']

ADDR_RE = re.compile(r'\b([0-9a-fA-F]{4}):([0-9a-fA-F]{4})\b')


def linear(seg, off):
    return (int(seg, 16) << 4) + int(off, 16)


def load_inventory(db):
    path = os.path.join(PIETHAWN, 'ida55-procedure-snapshots', db + '.procedure-inventory.json')
    if not os.path.exists(path):
        sys.stderr.write('bq_locate: missing IDA inventory %s\n' % path)
        return []
    procs = []
    for p in json.load(open(path, encoding='utf-8'))['procedures']:
        s_seg, s_off = p['start_ea'].split(':')
        e_seg, e_off = p['end_ea'].split(':')
        procs.append((linear(s_seg, s_off), linear(e_seg, e_off), p['name'], p['segment'], db, p.get('procedure_ordinal')))
    procs.sort()
    return procs


def find_proc(procs, ea):
    for start, end, name, segment, db, ordinal in procs:
        if start <= ea < end:
            return (name, segment, db, ordinal)
    return None


def load_items():
    text = open(BUGS, encoding='utf-8', errors='replace').read()
    items = {}
    for chunk in re.split(r'\n(?=\d+\.\s+[A-Z])', text):
        m = re.match(r'(\d+)\.\s+(\S+)\s+(.*)', chunk, re.S)
        if m:
            items[int(m.group(1))] = (m.group(2), m.group(3))
    return items


def load_triage():
    done = {}
    for line in open(TRIAGE, encoding='utf-8', errors='replace'):
        m = re.match(r'\[(.)\] \| \[#(\d+)\]', line)
        if m:
            done[int(m.group(2))] = (m.group(1) == 'x')
    return done


def index_definitions():
    """function name -> list of 'path:line' where it is defined (name at line start or after a return type, followed by '(' and a '{' line)."""
    defs = defaultdict(list)
    def_re = re.compile(r'^[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\([^;]*$')
    for d in SRC_DIRS:
        root = os.path.join(REPO, d)
        if not os.path.isdir(root):
            continue
        for fn in sorted(os.listdir(root)):
            if not fn.lower().endswith(('.c', '.cpp')):
                continue
            path = os.path.join(root, fn)
            lines = open(path, encoding='utf-8', errors='replace').read().split('\n')
            for i, l in enumerate(lines):
                m = def_re.match(l)
                if not m or l.startswith(('return', 'else', 'if', 'for', 'while', 'switch')):
                    continue
                nxt = lines[i + 1].strip() if i + 1 < len(lines) else ''
                if nxt.startswith('{') or l.rstrip().endswith('{'):
                    defs[m.group(1)].append('%s/%s:%d' % (d, fn, i + 1))
    return defs


HEADER_RE = re.compile(r'^// (WZD|MGC) (ovr|o|s)0*(\d+)p0*(\d+)\b')


def index_headers(defs):
    """(db, 'o'|'s', segment number, procedure ordinal) -> 'path:line' of the '// WZD oNNNpMM' / '// WZD sNNpMM' header."""
    heads = {}
    for d in SRC_DIRS:
        root = os.path.join(REPO, d)
        if not os.path.isdir(root):
            continue
        for fn in sorted(os.listdir(root)):
            if not fn.lower().endswith(('.c', '.cpp')):
                continue
            for i, l in enumerate(open(os.path.join(root, fn), encoding='utf-8', errors='replace')):
                m = HEADER_RE.match(l)
                if m:
                    kind = 's' if m.group(2) == 's' else 'o'
                    heads.setdefault((m.group(1), kind, int(m.group(3)), int(m.group(4))), []).append(('%s/%s' % (d, fn), i + 1))
    # a header also appears in file-top contents lists: keep the occurrence that has a function definition just below it
    def_lines = defaultdict(list)
    for locs in defs.values():
        for loc in locs:
            f, l = loc.rsplit(':', 1)
            def_lines[f].append(int(l))
    best = {}
    for key, occ in heads.items():
        for f, l in occ:
            below = [x for x in def_lines.get(f, []) if l < x <= l + 60]
            if below:
                best[key] = '%s:%d' % (f, min(below))
                break
    return best


def norm_name(name):
    name = re.sub(r'^(j_|zzz|zz|z)', '', name)
    name = re.sub(r'(__[A-Za-z0-9]+)+$', '', name)
    return name.lower()


def header_key(db, segment, ordinal):
    m = re.match(r'(ovr|seg)0*(\d+)$', segment)
    if not m or ordinal is None:
        return None
    return ('WZD' if db == 'WZD' else 'MGC', 'o' if m.group(1) == 'ovr' else 's', int(m.group(2)), int(ordinal))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--item', type=int)
    ap.add_argument('--groups', action='store_true')
    args = ap.parse_args()

    wzd = load_inventory('WZD')
    mgc = load_inventory('MGC')
    items = load_items()
    done = load_triage()
    defs = index_definitions()
    heads = index_headers(defs)
    norm_defs = defaultdict(list)
    for k, v in defs.items():
        norm_defs[norm_name(k)].extend(v)

    rows = []
    for n in sorted(items):
        status, body = items[n]
        hits = []
        for seg, off in ADDR_RE.findall(body):
            ea = linear(seg, off)
            p = find_proc(wzd, ea)
            if p is None and ('MAGIC' in body.upper()):
                p = find_proc(mgc, ea)
            if p is None:
                hits.append(('%s:%s' % (seg.lower(), off.lower()), '', '', '', '', '', ''))
                continue
            name, segment, db, ordinal = p
            remom = ';'.join(defs.get(name, []))
            how = 'name' if remom else ''
            if not remom:
                remom = ';'.join(norm_defs.get(norm_name(name), []))
                how = 'name-normalized' if remom else ''
            key = header_key(db, segment, ordinal)
            header = heads.get(key, '') if key else ''
            if not remom and header:
                remom, how = header, 'header-VERIFY'
            hits.append(('%s:%s' % (seg.lower(), off.lower()), db, segment, ordinal, name, remom, how))
        if not hits:
            hits.append(('', '', '', '', '', '', ''))
        for addr, db, segment, ordinal, name, remom, how in hits:
            rows.append({'item': n, 'status': status, 'done': 'x' if done.get(n) else '', 'addr': addr, 'db': db,
                         'ida_segment': segment, 'ida_ordinal': ordinal, 'ida_function': name, 'remom_definition': remom, 'matched_by': how})

    if args.item is not None:
        for r in rows:
            if r['item'] == args.item:
                print(r)
        return

    with open(OUT_CSV, 'w', newline='', encoding='utf-8') as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0]))
        w.writeheader()
        w.writerows(rows)

    per_item = defaultdict(list)
    for r in rows:
        per_item[r['item']].append(r)
    n_items = len(per_item)
    n_addr = sum(1 for v in per_item.values() if any(r['addr'] for r in v))
    n_ida = sum(1 for v in per_item.values() if any(r['ida_function'] for r in v))
    n_remom = sum(1 for v in per_item.values() if any(r['remom_definition'] for r in v))
    print('items %d | with address %d | mapped to IDA function %d | mapped to ReMoM definition %d' % (n_items, n_addr, n_ida, n_remom))
    print('wrote %s' % os.path.relpath(OUT_CSV, REPO))

    if args.groups:
        groups = defaultdict(set)
        unmapped = []
        for n, v in per_item.items():
            if v[0]['done']:
                continue
            files = sorted({r['remom_definition'].split(';')[0].split(':')[0] for r in v if r['remom_definition']})
            if files:
                for f in files:
                    groups[f].add(n)
            else:
                unmapped.append(n)
        for f, ns in sorted(groups.items(), key=lambda kv: -len(kv[1])):
            print('%3d  %s  %s' % (len(ns), f, ' '.join(str(x) for x in sorted(ns))))
        print('unmapped pending: %d  %s' % (len(unmapped), ' '.join(str(x) for x in sorted(unmapped))))


if __name__ == '__main__':
    main()
