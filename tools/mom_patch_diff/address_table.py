"""
address_table.py -- one table of every function in WIZARDS.EXE and MAGIC.EXE: its ReMoM id (WZD s161p04), IDA segment,
address and name, its file offset in each version's .EXE, and the C function that reconstructs it in ReMoM and in any
other reconstruction given.

    python3 -m tools.mom_patch_diff.address_table --remom . --out /c/MoM/address_table \\
        --map WIZARDS.EXE=/c/MoM/wizards_map.csv --map MAGIC.EXE=/c/MoM/magic_map.csv \\
        --exe 1.31=/c/MoM/MoM131 --exe 1.60=/c/MoM/MoM160 \\
        [--project OTHER=/c/MoM/other_index.csv]

Writes address_table.csv (every column) and address_table.md (a summary and the table), plus remom_index.csv.

Sources:
    --remom DIR    ReMoM's C sources: each function definition preceded by a "// WZD s161p04" / "// MGC o51p01" comment
    --map EXE=CSV  export_map.py output for that EXE (1.31 database): IDA segment, procedure number, name, address, bytes
    --exe LABEL=DIR  an install; each function is found in its WIZARDS.EXE / MAGIC.EXE by its 1.31 bytes, giving the
                   file offset in that version ("changed" if the bytes differ there, "short" if too few known bytes)
    --project NAME=CSV  another reconstruction's index: columns exe,segment,proc,function,file,line
                   (exe = WZD or MGC, segment = sNNN or oNNN, proc = number)

Without --map the table is ReMoM's index alone.  Standard library only.
"""

import argparse
import csv
import os
import re
import sys
from collections import defaultdict

from tools.mom_patch_diff.diff import load_map

EXES = {'WIZARDS.EXE': 'WZD', 'MAGIC.EXE': 'MGC'}

ID_COMMENT = re.compile(r'^\s*//\s*(WZD|MGC)\s+([so])0*(\d+)p0*(\d+)\b')
DEFINITION = re.compile(r'^[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\([^;]*\)\s*(\{.*)?$')
PROTOTYPE = re.compile(r'^[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\([^;]*\)\s*;\s*(//.*|/\*.*)?$')


def key(exe, kind, seg, proc):
    return (exe, kind, int(seg), int(proc))


def key_text(k):
    return '%s %s%03dp%02d' % k


def scan_remom(root):
    """{key: [(function, file, line, evidence)]} from ReMoM's id comments ("// WZD s161p04").

    evidence: "definition"  the next function definition after the comment
              "prototype"   a prototype after the comment ("void Init_New_Game(void);") names the function, and this
                            is its definition (wherever it is); preferred over "definition" when they disagree
              "...?"        (either evidence) the same function is also claimed by another id of the same EXE: a helper
                            placed between a comment and its function, or a mistyped id; check by hand
              "comment"     the id is only in a comment: ReMoM has no code for it yet (file, line of the comment)
    """
    definitions = {}                                    # function name -> (file, line), every definition in the sources
    near, protos, comments = defaultdict(list), defaultdict(set), {}
    for top in ('MoM', 'MoX', 'src', 'STU'):
        for folder, _, names in os.walk(os.path.join(root, top)):
            for name in sorted(names):
                if not name.endswith('.c'):
                    continue
                path = os.path.join(folder, name)
                with open(path, encoding='utf-8', errors='replace') as f:
                    lines = f.read().splitlines()
                rel = os.path.relpath(path, root).replace(os.sep, '/')
                pending = None
                for number, line in enumerate(lines, 1):
                    m = ID_COMMENT.match(line)
                    if m:
                        pending = (key(*m.groups()), number)
                        comments.setdefault(pending[0], (rel, number))
                        continue
                    if line.startswith((' ', '\t', '/', '*', '#', '{', '}')) or not line.strip():
                        continue
                    proto = PROTOTYPE.match(line)
                    if proto:
                        if pending:
                            protos[pending[0]].add(proto.group(1))
                            pending = None
                        continue
                    d = DEFINITION.match(line)
                    if not d:
                        if pending and line.rstrip().endswith(';'):
                            pending = None
                        continue
                    following = next((l for l in lines[number:number + 3] if l.strip()), '')
                    if not (line.rstrip().endswith('{') or following.startswith('{')):
                        continue
                    definitions.setdefault(d.group(1), (rel, number))
                    if pending and (number - pending[1]) <= 60:
                        near[pending[0]].append((d.group(1), rel, number))
                    pending = None
    found = {}
    for k in comments:
        named = [(fn,) + definitions[fn] + ('prototype',) for fn in sorted(protos.get(k, ())) if fn in definitions]
        if named:
            found[k] = named
        elif near.get(k):
            found[k] = [n + ('definition',) for n in near[k]]
        else:
            found[k] = [('',) + comments[k] + ('comment',)]
    # one function claimed by several ids of the same EXE: a helper sits between comment and code, or an id is
    # mistyped.  Mark every such claim doubtful ("?") rather than guess which one is right.
    claims = defaultdict(set)
    for k, hits in found.items():
        for fn, _, _, evidence in hits:
            if fn:
                claims[(k[0], fn)].add(k)
    for k, hits in found.items():
        found[k] = [(fn, path, line, evidence + ('?' if fn and len(claims[(k[0], fn)]) > 1 else '')) for fn, path, line, evidence in hits]
    return found


def segment_key(exe, segment_name):
    m = re.match(r'(seg|ovr)0*(\d+)$', segment_name, re.I)
    if not m:
        return None
    return exe, ('s' if m.group(1).lower() == 'seg' else 'o'), int(m.group(2))


def load_project(path):
    out = defaultdict(list)
    with open(path, newline='', encoding='utf-8') as f:
        for row in csv.DictReader(f):
            m = re.match(r'([so])0*(\d+)$', row['segment'].strip())
            if not m:
                continue
            out[key(row['exe'].strip().upper(), m.group(1), m.group(2), row['proc'])].append(
                (row['function'], row.get('file', ''), row.get('line', '')))
    return out


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    p.add_argument('--remom', default='.')
    p.add_argument('--out', required=True)
    p.add_argument('--map', action='append', default=[])
    p.add_argument('--exe', action='append', default=[])
    p.add_argument('--project', action='append', default=[])
    args = p.parse_args(argv)

    remom = scan_remom(args.remom)
    os.makedirs(args.out, exist_ok=True)
    with open(os.path.join(args.out, 'remom_index.csv'), 'w', newline='', encoding='utf-8') as f:
        w = csv.writer(f)
        w.writerow(['id', 'function', 'file', 'line', 'evidence'])
        for k in sorted(remom):
            for fn, path, line, evidence in remom[k]:
                w.writerow([key_text(k), fn, path, line, evidence])

    projects = []
    for spec in args.project:
        name, _, path = spec.partition('=')
        projects.append((name, load_project(path)))

    versions = []
    for spec in args.exe:
        label, _, folder = spec.partition('=')
        files = {}
        for exe in EXES:
            for candidate in os.listdir(folder):
                if candidate.upper() == exe:
                    with open(os.path.join(folder, candidate), 'rb') as f:
                        files[exe] = f.read()
        versions.append((label, files))

    rows = []
    seen = set()
    stats = defaultdict(lambda: defaultdict(int))
    for spec in args.map:
        exe_file, _, path = spec.partition('=')
        exe_file = exe_file.upper()
        exe = EXES.get(exe_file, exe_file)
        items = load_map(path)
        seg_start = {i.name: i.ea for i in items if i.kind == 'S'}
        for item in (i for i in items if i.kind == 'F'):
            sk = segment_key(exe, item.segment)
            k = (sk + (item.proc,)) if sk else None
            seen.add(k)
            start = seg_start.get(item.segment)
            row = {
                'exe': exe, 'id': key_text(k) if k else '', 'ida_segment': item.segment, 'proc': item.proc,
                'ida_name': item.name, 'ida_ea': '%X' % item.ea,
                'seg_off': ('%s:%04X' % (item.segment, item.ea - start)) if start is not None else '', 'size': item.size,
            }
            for label, files in versions:
                data = files.get(exe_file)
                if data is None:
                    row['offset_' + label] = ''
                    continue
                hits = item.find(data)
                if hits is None:
                    row['offset_' + label] = 'short'
                elif not hits:
                    row['offset_' + label] = 'changed'
                    stats[exe][label + ' changed'] += 1
                else:
                    row['offset_' + label] = '0x%X' % hits[0] + (' (+%d more)' % (len(hits) - 1) if len(hits) > 1 else '')
                    stats[exe][label + ' found'] += 1
            impl = remom.get(k, []) if k else []
            row['remom_function'] = '; '.join(fn for fn, _, _, _ in impl)
            row['remom_location'] = '; '.join('%s:%d' % (path, line) for _, path, line, _ in impl)
            row['remom_evidence'] = '; '.join(e for _, _, _, e in impl)
            for name, index in projects:
                hits = index.get(k, []) if k else []
                row[name + '_function'] = '; '.join(fn for fn, _, _ in hits)
                row[name + '_location'] = '; '.join(('%s:%s' % (path, line)) if line else path for _, path, line in hits)
            stats[exe]['functions'] += 1
            stats[exe]['in ReMoM'] += any(e != 'comment' for _, _, _, e in impl)
            for name, _ in projects:
                stats[exe]['in ' + name] += bool(row[name + '_function'])
            rows.append(row)

    # ids ReMoM uses that no map has (or every id, without maps)
    for k in sorted(remom):
        if k in seen:
            continue
        impl = remom[k]
        row = {'exe': k[0], 'id': key_text(k), 'remom_function': '; '.join(fn for fn, _, _, _ in impl),
               'remom_location': '; '.join('%s:%d' % (path, line) for _, path, line, _ in impl),
               'remom_evidence': '; '.join(e for _, _, _, e in impl)}
        if args.map:
            row['ida_name'] = '(not in the map)'
            stats[k[0]]['ReMoM ids not in the map'] += 1
        for name, index in projects:
            hits = index.get(k, [])
            row[name + '_function'] = '; '.join(fn for fn, _, _ in hits)
            row[name + '_location'] = '; '.join(('%s:%s' % (path, line)) if line else path for _, path, line in hits)
        rows.append(row)

    columns = ['exe', 'id', 'ida_segment', 'proc', 'ida_name', 'ida_ea', 'seg_off', 'size'] + \
              ['offset_' + label for label, _ in versions] + ['remom_function', 'remom_location', 'remom_evidence'] + \
              [c for name, _ in projects for c in (name + '_function', name + '_location')]
    with open(os.path.join(args.out, 'address_table.csv'), 'w', newline='', encoding='utf-8') as f:
        w = csv.DictWriter(f, columns)
        w.writeheader()
        for row in rows:
            w.writerow({c: row.get(c, '') for c in columns})

    md = ['# Function address table', '',
          'Every function of WIZARDS.EXE (WZD) and MAGIC.EXE (MGC): ReMoM id, IDA segment/address/name, file offset in '
          'each version, and the reconstruction of it in ReMoM%s. Generated by tools/mom_patch_diff/address_table.py.'
          % ''.join(' and ' + n for n, _ in projects), '']
    for exe in sorted(stats):
        md.append('- **%s**: ' % exe + ', '.join('%s %d' % (k, v) for k, v in sorted(stats[exe].items())))
    if not args.map:
        evidence = defaultdict(int)
        for v in remom.values():
            evidence[v[0][3]] += 1
        md.append('- ReMoM index only (no --map): %d ids; ' % len(remom) + ', '.join('%s %d' % kv for kv in sorted(evidence.items())))
    md += ['', '| ' + ' | '.join(columns) + ' |', '|' + '---|' * len(columns)]
    md += ['| ' + ' | '.join(str(r.get(c, '')).replace('|', '/') for c in columns) + ' |' for r in rows]
    with open(os.path.join(args.out, 'address_table.md'), 'w', encoding='utf-8', newline='\n') as f:
        f.write('\n'.join(md) + '\n')
    print('address_table: %d rows written to %s' % (len(rows), args.out))
    return 0


if __name__ == '__main__':
    sys.exit(main())
