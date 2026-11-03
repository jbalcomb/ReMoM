#!/usr/bin/env python3
"""
fix_remom_ids.py -- corrects ReMoM id comments that name no procedure of the IDA database ("only in ReMoM" rows of a
link_remom.py table), using the procedure the database has under the same function name.

    python3 -m tools.mom_patch_diff.fix_remom_ids --linked wizards_linked.tsv --remom /c/STU/devel/ReMoM          # show
    python3 -m tools.mom_patch_diff.fix_remom_ids --linked wizards_linked.tsv --remom /c/STU/devel/ReMoM --apply  # write

For each wrong id with a function, the database procedure whose name (the database's own, or else drake178's) equals
the function gives the right segment and procedure number.  An id with no function (a comment only) gets the shift
its segment's named ids got, when they all moved the same way.  The new id is written as WZD s034p49 / WZD o161p04:
s for a segNNN segment, o for an ovrNNN overlay.

Refused, and reported instead: a function the database has under no name or under several; a new id another ReMoM
function already holds (the same function under the right id is fine: the wrong id is a duplicate with a typo); a comment-only id whose segment's named ids moved in different ways.

--apply replaces every occurrence of each wrong id in ReMoM's .c and .h files (MoM, MoX, src, STU), in any zero-padding
(s34p49, s034p49), keeping each file's bytes and line endings otherwise.  Standard library only.
"""

import argparse
import csv
import os
import re
import sys
from collections import defaultdict

ID = re.compile(r'(WZD|MGC)\s+([so])0*(\d+)p0*(\d+)$')
NUMBERED = re.compile(r'(seg|ovr)0*(\d+)$', re.I)


def new_id(exe, segment, proc):
    m = NUMBERED.match(segment)
    if not m:
        return None
    return '%s %s%03dp%02d' % (exe, 's' if m.group(1).lower() == 'seg' else 'o', int(m.group(2)), int(proc))


def plan(linked):
    with open(linked, newline='', encoding='utf-8') as f:
        rows = list(csv.DictReader(f, delimiter='\t'))
    procs = [r for r in rows if r['match'] != 'only in ReMoM']
    wrong = [r for r in rows if r['match'] == 'only in ReMoM']
    held = {r['remom_id'] for r in procs if r['remom_id']}
    by_name = defaultdict(list)
    for side in ('remom_name', 'drake178_name'):
        for r in procs:
            if r.get(side):
                by_name[(side, r[side])].append(r)
    fixes, problems, shifts = {}, [], defaultdict(set)
    for w in wrong:
        m = ID.match(w['remom_id'])
        if not m:
            problems.append((w['remom_id'], 'not an id this script understands'))
            continue
        exe, seg, proc = m.group(1), int(m.group(3)), int(m.group(4))
        if not w['remom_function']:
            continue
        hits = by_name.get(('remom_name', w['remom_function'])) or by_name.get(('drake178_name', w['remom_function'])) or []
        if len(hits) != 1:
            problems.append((w['remom_id'], '%s: the database has it %s' % (w['remom_function'], 'under no name' if not hits else 'at %d places' % len(hits))))
            continue
        h = hits[0]
        target = new_id(exe, h['remom_segment'], h['remom_proc'])
        if target is None:
            problems.append((w['remom_id'], '%s is in %s, which has no number' % (w['remom_function'], h['remom_segment'])))
        elif h['remom_id'] and h['remom_id'] != w['remom_id'] and w['remom_function'] not in h['remom_function'].split('; '):
            problems.append((w['remom_id'], '%s is %s, which ReMoM already uses for %s' % (w['remom_function'], target, h['remom_id'])))
        else:
            fixes[w['remom_id']] = (target, 'named: %s %s p%s' % (w['remom_function'], h['remom_segment'], h['remom_proc']))
            shifts[(exe, seg)].add((target.split()[1][:4], int(h['remom_proc']) - proc))
    for w in wrong:
        m = ID.match(w['remom_id'])
        if not m or w['remom_function'] or w['remom_id'] in fixes:
            continue
        exe, seg, proc = m.group(1), int(m.group(3)), int(m.group(4))
        moves = shifts.get((exe, seg), set())
        if len(moves) != 1:
            problems.append((w['remom_id'], 'comment only, and its segment\'s named ids moved %s' % ('in different ways' if moves else 'nowhere')))
            continue
        (segment, delta), = moves
        target = '%s %sp%02d' % (exe, segment, proc + delta)
        if target in held:
            problems.append((w['remom_id'], 'comment only; %s is already used' % target))
        else:
            fixes[w['remom_id']] = (target, 'comment only: same shift as its segment (%+d)' % delta)
    return fixes, problems


def apply(root, fixes, write):
    patterns = {}
    for old, (new, _) in fixes.items():
        m = ID.match(old)
        patterns[old] = (re.compile((r'\b%s\s+%s0*%dp0*%d\b' % (m.group(1), m.group(2), int(m.group(3)), int(m.group(4)))).encode()), new.encode())
    changed = defaultdict(list)
    for top in ('MoM', 'MoX', 'src', 'STU'):
        for folder, _, names in os.walk(os.path.join(root, top)):
            for name in sorted(names):
                if not name.endswith(('.c', '.h')):
                    continue
                path = os.path.join(folder, name)
                with open(path, 'rb') as f:
                    data = f.read()
                new_data = data
                for old, (regex, new) in patterns.items():
                    for m in regex.finditer(new_data):
                        line = new_data.count(b'\n', 0, m.start()) + 1
                        changed[old].append('%s:%d' % (os.path.relpath(path, root).replace(os.sep, '/'), line))
                    new_data = regex.sub(new, new_data)
                if write and new_data != data:
                    with open(path, 'wb') as f:
                        f.write(new_data)
    return changed


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    p.add_argument('--linked', required=True, help='TSV from link_remom.py')
    p.add_argument('--remom', required=True, help='the ReMoM checkout to fix')
    p.add_argument('--apply', action='store_true', help='write the changes (default: only show them)')
    args = p.parse_args(argv)
    fixes, problems = plan(args.linked)
    changed = apply(args.remom, fixes, args.apply)
    for old in sorted(fixes):
        new, why = fixes[old]
        print('%s -> %s\t(%s)\t%s' % (old, new, why, ', '.join(changed.get(old, [])) or 'no occurrence found'))
    for old, why in problems:
        print('NOT FIXED %s\t%s' % (old, why))
    print('fix_remom_ids: %d ids %s, %d not fixed%s' % (len(fixes), 'fixed' if args.apply else 'to fix', len(problems),
                                                      '' if args.apply else '; run again with --apply to write'))
    return 0


if __name__ == '__main__':
    sys.exit(main())
