"""
patch_index.py -- indexes the Master of Magic FILESET patch files (the unofficial patches 1.40n - 1.52 released with
drake178's Community Patch) and writes a to-do list per version for porting them to ReMoM.

    python3 -m tools.mom_patches.patch_index PATCH_DIR --out OUT_DIR [--remom .]
        [--base WIZARDS.EXE=/c/MoM/MoM131/WIZARDS.EXE --base MAGIC.EXE=/c/MoM/MoM131/MAGIC.EXE]
        [--map WIZARDS.EXE=/c/MoM/wizards_map.tsv --map MAGIC.EXE=/c/MoM/magic_map.tsv]

PATCH_DIR holds one folder per version (140, 151, 152, Optional), each with an optional OBSOLETE/ folder.  A patch file
is ';' comment lines followed by records of three hex lines: file offset, original byte, new byte.  Comment blocks
between records start a new section ("reverse W489", ...).

    --remom DIR   ReMoM's sources: maps function ids (WZD s161p04) and drake178 names (// drake178: TILE_GetProd())
                  to ReMoM functions
    --base EXE=FILE  the .EXE the offsets are checked against: each record's original byte must be there
    --map EXE=TSV    export_map.py output for the same version as --base: maps each patched offset to a function id

Writes (all tables tab-separated, with a header row):
    patches.tsv        one row per patch file
    records.tsv        one row per patched byte, with its function when --map is given
    todo_<version>.md  a checklist per version, grouped by kind, obsolete patches listed apart

Standard library only.
"""

import argparse
import csv
import os
import re
import sys
from collections import Counter, defaultdict

from tools.mom_patch_diff.address_table import ID_COMMENT, drake178_comments, key, key_text, scan_remom
from tools.mom_patch_diff.diff import load_map

KINDS = {
    'FIX': 'Bug fix', 'FEAT': 'Feature', 'AI': 'AI', 'DIPL': 'Diplomacy', 'OPT': 'Optimization', 'REV': 'Revision',
    '': 'Other',
}
HEX = re.compile(r'^[0-9A-Fa-f]+$')
PATCH_REF = re.compile(r'\b(I?[WM]\d{3}[A-Z]*)\b')
DRAKE_NAME = re.compile(r'\b([A-Z][A-Za-z0-9]*_[A-Za-z0-9_]+)\(\)')
OBSOLETE_NOTE = 'This patch is now obsolete'
NOT_A_TITLE = re.compile(r'(This patch is now obsolete|Please do not try|folder, but it can no longer|^Provided by|^by \w+$|'
                         r'^apply to \w+\.exe|Its contents are preserved)', re.I)
APPLY_TO = re.compile(r'\bapply to (\w+\.exe)', re.I)


def parse_name(stem):
    """(exe, number, kind) from a file stem like W037FIX, IW076, IM185, M026FEAT, DIAGONAL."""
    m = re.match(r'^(I?)([WM])(\d{3})([A-Z]*)$', stem.upper())
    if not m:
        return '', '', ''
    exe = 'WZD' if m.group(2) == 'W' else 'MGC'
    return exe, m.group(3), m.group(4)


def parse_patch(text):
    """{'comments': [...], 'sections': [(label, [(offset, old, new)])], 'malformed': [...]}"""
    comments, sections, malformed = [], [], []
    label, block, values = '', [], []
    pending_comment = []

    def flush_values():
        nonlocal values
        records = []
        for i in range(0, len(values) - 2, 3):
            records.append((int(values[i], 16), int(values[i + 1], 16), int(values[i + 2], 16)))
        if len(values) % 3:
            malformed.append('%d trailing values in section "%s"' % (len(values) % 3, label))
        values = []
        return records

    for raw in text.splitlines():
        line = raw.strip()
        if not line:
            continue
        code, _, comment = line.partition(';')
        code = code.strip()
        if code:
            if pending_comment:
                if values:
                    sections.append((label, flush_values()))
                text_lines = [c for c in pending_comment if c.strip('-= ')]
                label = text_lines[0] if text_lines else label
                pending_comment = []
            for token in code.split():
                if HEX.match(token):
                    values.append(token)
                else:
                    malformed.append(token)
        if comment or line.startswith(';'):
            c = comment.strip() if code else line.lstrip(';').strip()
            comments.append(c)
            if not code:
                pending_comment.append(c)
    if values:
        sections.append((label, flush_values()))
    return {'comments': comments, 'sections': sections, 'malformed': malformed}


def drake178_index(root):
    """drake178 function name -> ReMoM id, from a "// drake178: NAME()" comment within a few lines of an id comment."""
    out = {}
    for k, names in sorted(drake178_comments(root).items()):
        for n in names:
            out.setdefault(n, k)
    return out


class FunctionLocator:
    """Maps file offsets in one .EXE to the functions and data items of its export_map.py map."""

    def __init__(self, exe_bytes, map_path, exe_code):
        self.spans = []
        for item in load_map(map_path):
            if item.kind not in ('F', 'D'):
                continue
            hits = item.find(exe_bytes)
            if hits:
                label = ''
                m = re.match(r'(seg|ovr)0*(\d+)$', item.seg_id, re.I)
                if item.kind == 'F' and m:
                    label = key_text((exe_code, 's' if m.group(1).lower() == 'seg' else 'o', int(m.group(2)), item.proc))
                self.spans.append((hits[0], hits[0] + item.size, item.kind, label, item.name))
        self.spans.sort()

    def find(self, offset):
        for start, end, kind, label, name in self.spans:
            if start <= offset < end:
                return kind, label, name, offset - start
            if start > offset:
                break
        return None


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    p.add_argument('patch_dir')
    p.add_argument('--out', required=True)
    p.add_argument('--remom')
    p.add_argument('--base', action='append', default=[])
    p.add_argument('--map', action='append', default=[])
    args = p.parse_args(argv)

    exe_files = {'WZD': 'WIZARDS.EXE', 'MGC': 'MAGIC.EXE'}
    bases, locators = {}, {}
    for spec in args.base:
        exe, _, path = spec.partition('=')
        with open(path, 'rb') as f:
            bases[exe.upper()] = f.read()
    for spec in args.map:
        exe, _, path = spec.partition('=')
        exe = exe.upper()
        code = 'WZD' if exe == 'WIZARDS.EXE' else 'MGC'
        if exe not in bases:
            sys.exit('patch_index: --map %s needs --base %s=... (the offsets are placed in that file)' % (exe, exe))
        locators[exe] = FunctionLocator(bases[exe], path, code)

    remom_ids, drake = {}, {}
    if args.remom:
        remom_ids = {key_text(k): v for k, v in scan_remom(args.remom).items()}
        drake = drake178_index(args.remom)

    patches, records = [], []
    for folder, _, names in sorted(os.walk(args.patch_dir)):
        rel_folder = os.path.relpath(folder, args.patch_dir).replace(os.sep, '/')
        if rel_folder == '.':
            continue
        version = rel_folder.split('/')[0]
        in_obsolete = '/OBSOLETE' in '/' + rel_folder.upper()
        for name in sorted(names):
            if not name.upper().endswith('.TXT'):
                continue
            with open(os.path.join(folder, name), encoding='latin-1') as f:
                text = f.read()
            stem = os.path.splitext(name)[0]
            stub = OBSOLETE_NOTE in text and len(text) < 300 and not in_obsolete
            exe, number, kind = parse_name(stem)
            parsed = parse_patch(text)
            body = [c for c in parsed['comments'] if c.strip('-=: ') and not NOT_A_TITLE.search(c)]
            title = re.sub(r'^\d{3}\s*-\s*', '', body[0]).strip() if body else ''
            refs = sorted({r for c in parsed['comments'] for r in PATCH_REF.findall(c) if r.upper() != stem.upper()})
            names_mentioned = sorted({n for c in parsed['comments'] for n in DRAKE_NAME.findall(c)})
            exe_file = exe_files.get(exe, '')
            stated = next((m.group(1).upper() for c in parsed['comments'] for m in [APPLY_TO.search(c)] if m), '')
            if stated:                                                # "; apply to Itemmake.exe" beats the file name
                exe_file = stated
                exe = {'WIZARDS.EXE': 'WZD', 'MAGIC.EXE': 'MGC'}.get(stated, stated.split('.')[0])
            count = matched = 0
            functions = Counter()
            for label, recs in parsed['sections']:
                for offset, old, new in recs:
                    count += 1
                    row = {'version': version, 'patch': stem, 'obsolete': 'yes' if in_obsolete else '', 'section': label,
                           'offset': '0x%06X' % offset, 'old': '%02X' % old, 'new': '%02X' % new}
                    base = bases.get(exe_file)
                    if base is not None:
                        ok = offset < len(base) and base[offset] == old
                        matched += ok
                        row['old_matches_base'] = 'yes' if ok else 'no'
                    loc = locators.get(exe_file)
                    if loc is not None:
                        hit = loc.find(offset)
                        if hit:
                            kind_, label_, name_, rel = hit
                            row['in'] = (label_ or name_) + ('+0x%X' % rel)
                            functions[label_ or ('data ' + name_)] += 1
                        else:
                            row['in'] = '(outside the map)'
                            functions['(outside the map)'] += 1
                    records.append(row)
            remom_by_id = []
            for fid in functions:
                for fn, path, line, evidence in remom_ids.get(fid, []):
                    if fn:
                        remom_by_id.append('%s %s (%s:%s)' % (fid, fn, path, line))
            remom_by_name = []
            for n in names_mentioned:
                k = drake.get(n)
                if k:
                    impl = remom_ids.get(key_text(k), [])
                    fn = next((i[0] for i in impl if i[0]), '')
                    remom_by_name.append('%s = %s%s' % (n, key_text(k), (' ' + fn) if fn else ''))
            patches.append({
                'version': version, 'patch': stem, 'file': rel_folder + '/' + name,
                'status': 'obsolete' if in_obsolete else ('obsolete (stub)' if stub else 'current'),
                'exe': exe, 'number': number, 'kind': KINDS.get(kind, kind),
                'title': title, 'description': ' '.join(body[1:])[:2000],
                'sections': len(parsed['sections']), 'bytes': count,
                'old_matches_base': ('%d/%d' % (matched, count)) if (exe_file in bases and count) else '',
                'functions': ', '.join('%s (%d)' % kv for kv in sorted(functions.items())),
                'remom_by_id': '; '.join(remom_by_id), 'drake178_names': ', '.join(names_mentioned),
                'remom_by_name': '; '.join(remom_by_name), 'references': ', '.join(refs),
                'malformed': '; '.join(parsed['malformed'])[:300],
            })

    os.makedirs(args.out, exist_ok=True)
    pcols = ['version', 'patch', 'status', 'exe', 'number', 'kind', 'title', 'sections', 'bytes', 'old_matches_base',
             'functions', 'remom_by_id', 'drake178_names', 'remom_by_name', 'references', 'malformed', 'description', 'file']
    rcols = ['version', 'patch', 'obsolete', 'section', 'offset', 'old', 'new', 'old_matches_base', 'in']
    for path, cols, rows in ((os.path.join(args.out, 'patches.tsv'), pcols, patches),
                             (os.path.join(args.out, 'records.tsv'), rcols, records)):
        with open(path, 'w', newline='', encoding='utf-8') as f:
            w = csv.DictWriter(f, cols, delimiter='\t', lineterminator='\n', extrasaction='ignore')
            w.writeheader()
            for row in rows:
                w.writerow({c: str(row.get(c, '')).replace('\t', ' ').replace('\n', ' ') for c in cols})

    by_version = defaultdict(list)
    for pt in patches:
        by_version[pt['version']].append(pt)
    for version, items in sorted(by_version.items()):
        current = [pt for pt in items if pt['status'] == 'current']
        stale = [pt for pt in items if pt['status'] != 'current']
        md = ['# Version %s: porting checklist' % version, '',
              'Generated by tools/mom_patches/patch_index.py from %d patch files: %d current, %d obsolete '
              '(stubs and their OBSOLETE/ originals).' % (len(items), len(current), len(stale)), '',
              'Each item becomes an override in this version\'s rules profile (a Lua rule, a data value, or a C fix behind '
              'the profile), found through the functions it patches. `[ ]` = not ported.', '']
        by_kind = defaultdict(list)
        for pt in current:
            by_kind[pt['kind']].append(pt)
        for kind in sorted(by_kind, key=lambda k: (k == 'Other', k)):
            md += ['## %s (%d)' % (kind, len(by_kind[kind])), '']
            for pt in sorted(by_kind[kind], key=lambda x: x['patch']):
                line = '- [ ] **%s** %s' % (pt['patch'], pt['title'] or '(no title)')
                facts = []
                if pt['bytes']:
                    facts.append('%d bytes' % pt['bytes'])
                if pt['functions']:
                    facts.append('in ' + pt['functions'])
                if pt['remom_by_id']:
                    facts.append('ReMoM: ' + pt['remom_by_id'])
                elif pt['remom_by_name']:
                    facts.append('ReMoM (by drake178 name): ' + pt['remom_by_name'])
                elif pt['drake178_names']:
                    facts.append('drake178 names: ' + pt['drake178_names'])
                if pt['references']:
                    facts.append('mentions ' + pt['references'])
                if pt['old_matches_base'] and pt['old_matches_base'].split('/')[0] != pt['old_matches_base'].split('/')[1]:
                    facts.append('original bytes match the base for ' + pt['old_matches_base'])
                md.append(line + ('  \n  ' + '; '.join(facts) if facts else ''))
            md.append('')
        if stale:
            md += ['## Obsolete (%d)' % len(stale), '', 'Superseded by later patches; kept for reference, not ported on their own.', '']
            md += ['- %s %s%s' % (pt['patch'], pt['title'] or '(stub)', ' (OBSOLETE/)' if pt['status'] == 'obsolete' else '')
                   for pt in sorted(stale, key=lambda x: (x['patch'], x['status']))]
        with open(os.path.join(args.out, 'todo_%s.md' % version), 'w', encoding='utf-8', newline='\n') as f:
            f.write('\n'.join(md) + '\n')
    print('patch_index: %d patches, %d patched bytes; written to %s' % (len(patches), len(records), args.out))
    return 0


if __name__ == '__main__':
    sys.exit(main())
