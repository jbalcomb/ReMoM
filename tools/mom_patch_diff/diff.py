"""
diff.py -- compares two Master of Magic installs (for example 1.31 and a version built from it with drake178's
MOMDP_32.EXE) and writes what changed, file by file, in a form ReMoM's code can be checked against.

    python3 -m tools.mom_patch_diff OLD_DIR NEW_DIR --out REPORT_DIR
        [--map WIZARDS.EXE=wizards_map.csv] [--map MAGIC.EXE=magic_map.csv]
        [--old-label 1.31] [--new-label 1.60] [--disasm]

Run from the root of the ReMoM repository (it reuses tools/asset_catalog and tools/dummy_lbx).

What it writes into REPORT_DIR:
    summary.md / summary.json    every file: same / changed / only in one install, sizes and SHA-256
    <EXE>/functions.md           with a --map: every function that changed, by ReMoM's name (WZD s161p04) and IDA's
    <EXE>/functions/<id>.txt     one per changed function: the byte differences, and with --disasm both disassemblies
    <EXE>/data.md                with a --map: named data items whose bytes changed, with old and new values
    <EXE>/regions.md             changed byte ranges no function or data item covers (or all of them, without a map)
    <EXE>/strings.md             text added or removed
    <EXE>/layout.md              the MZ header, the load module and the overlay area of both files
    <LBX>.md                     changed entries; data tables decoded field by field (tools/dummy_lbx/tables.py)
    <FILE>.diff                  other text files: a unified diff

A --map is the CSV that tools/mom_patch_diff/export_map.idc writes from the 1.31 IDA database.  Each function is found
in both files by its bytes (relocation fixups masked), so moved and resized functions are still matched.
--disasm needs the capstone Python package (optional; vet it per doc/@Devel/Dependency-Vetting.md before installing).
Standard library only otherwise.
"""

import argparse
import csv
import difflib
import hashlib
import json
import os
import re
import struct
import sys
from collections import Counter

from tools.asset_catalog.lbx import parse_lbx
from tools.dummy_lbx.tables import TABLES

MIN_ANCHOR = 6          # shortest run of known bytes trusted to locate a function
MERGE_GAP = 8           # changed byte ranges closer than this are reported as one
HEX_LIMIT = 256         # bytes of hex shown per changed range in the Markdown files
STRING_MIN = 4


# --------------------------------------------------------------------------------------------------------------------
#  Helpers
# --------------------------------------------------------------------------------------------------------------------

def sha256(data):
    return hashlib.sha256(data).hexdigest()


def hexdump(data, limit=None):
    shown = data if limit is None else data[:limit]
    text = ' '.join('%02X' % b for b in shown)
    if limit is not None and len(data) > limit:
        text += ' ... (%d bytes)' % len(data)
    return text


def printable(data):
    return ''.join(chr(b) if 32 <= b < 127 else '.' for b in data)


def changed_ranges(a, b, gap=MERGE_GAP):
    """Ranges [start, end) where equal-length a and b differ, merged across small gaps."""
    ranges = []
    start = None
    last = None
    for i in range(len(a)):
        if a[i] != b[i]:
            if start is None:
                start = i
            elif i - last > gap:
                ranges.append((start, last + 1))
                start = i
            last = i
    if start is not None:
        ranges.append((start, last + 1))
    return ranges


def strings_with_offsets(data):
    return [(m.start(), m.group().decode('ascii')) for m in re.finditer(rb'[\x20-\x7e]{%d,}' % STRING_MIN, data)]


def write(path, text):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, 'w', encoding='utf-8', newline='\n') as f:
        f.write(text)


def safe_name(name):
    return re.sub(r'[^A-Za-z0-9_\-]', '_', name)


# --------------------------------------------------------------------------------------------------------------------
#  MZ executables
# --------------------------------------------------------------------------------------------------------------------

def parse_mz(data):
    if len(data) < 0x1C or data[:2] not in (b'MZ', b'ZM'):
        return None
    (last, pages, nrel, hdr_par, min_alloc, max_alloc, ss, sp, checksum, ip, cs, rel_off, ovl) = struct.unpack_from('<13H', data, 2)
    load_end = pages * 512 - ((512 - last) if last else 0)
    info = {
        'header_size': hdr_par * 16, 'load_module_end': load_end, 'load_module_size': load_end - hdr_par * 16,
        'relocations': nrel, 'relocation_table': rel_off, 'cs_ip': '%04X:%04X' % (cs, ip), 'ss_sp': '%04X:%04X' % (ss, sp),
        'min_alloc': min_alloc, 'max_alloc': max_alloc, 'overlay_number': ovl, 'file_size': len(data),
        'after_load_module': len(data) - load_end,
    }
    fbov = data.find(b'FBOV', max(load_end - 16, 0))
    info['fbov_offset'] = fbov
    if fbov >= 0 and fbov + 16 <= len(data):
        size, segtab, nseg = struct.unpack_from('<III', data, fbov + 4)
        info['fbov_header'] = {'overlay_size': size, 'segment_table': segtab, 'segments': nseg}
    reloc = set()
    for i in range(nrel):
        o = rel_off + i * 4
        if o + 4 > len(data):
            break
        off, seg = struct.unpack_from('<HH', data, o)
        f = hdr_par * 16 + seg * 16 + off
        reloc.update((f, f + 1))
    info['_reloc_bytes'] = reloc
    return info


class Item:
    """A function or named data item from an export_map.idc CSV."""

    def __init__(self, row):
        self.kind = row['kind']
        self.segment = row['segment']
        self.proc = int(row['proc'] or 0)
        self.name = row['name']
        self.ea = int(row['ea'], 16)
        self.size = int(row['size'])
        text = row['bytes'].strip()
        self.pattern = bytearray()
        self.mask = bytearray()
        for i in range(0, len(text), 2):
            pair = text[i:i + 2]
            if pair == '..':
                self.pattern.append(0)
                self.mask.append(0)
            else:
                self.pattern.append(int(pair, 16))
                self.mask.append(1)
        self.runs = []                                           # (offset, bytes) of every run of known bytes
        i = 0
        while i < len(self.mask):
            if self.mask[i]:
                j = i
                while j < len(self.mask) and self.mask[j]:
                    j += 1
                self.runs.append((i, bytes(self.pattern[i:j])))
                i = j
            else:
                i += 1
        self.anchor = max(self.runs, key=lambda r: len(r[1])) if self.runs else (0, b'')

    @property
    def wzd(self):
        m = re.match(r'(seg|ovr)(\d+)$', self.segment, re.I)
        if self.kind == 'F' and m:
            return '%s%03dp%02d' % ('s' if m.group(1).lower() == 'seg' else 'o', int(m.group(2)), self.proc)
        return ''

    @property
    def label(self):
        return (self.wzd + ' ' if self.wzd else '') + self.name

    def matches_at(self, data, start):
        if start < 0 or start + len(self.pattern) > len(data):
            return False
        return all(data[start + o:start + o + len(r)] == r for o, r in self.runs)

    def find(self, data, limit=8):
        """File offsets where the item's known bytes match (at most `limit`)."""
        a_off, anchor = self.anchor
        if len(anchor) < MIN_ANCHOR:
            return None                                          # too little known to place it reliably
        hits = []
        i = data.find(anchor)
        while i != -1 and len(hits) < limit:
            if self.matches_at(data, i - a_off):
                hits.append(i - a_off)
            i = data.find(anchor, i + 1)
        return hits


def load_map(path):
    with open(path, newline='', encoding='latin-1') as f:
        return [Item(row) for row in csv.DictReader(f)]


def most_common_delta(pairs):
    counts = Counter(off - ea for ea, off in pairs)
    return counts.most_common(1)[0][0] if counts else None


def locate_changed(item, old, new, old_off, new_hint):
    """Best guess at where a changed function now is: (offset, size, similarity)."""
    body = old[old_off:old_off + item.size]
    candidates = set()
    if new_hint is not None:
        candidates.add(new_hint)
    if old_off + item.size <= len(new):
        candidates.add(old_off)
    for o, r in item.runs[:1] + [item.anchor]:
        if len(r) >= MIN_ANCHOR:
            i = new.find(r)
            n = 0
            while i != -1 and n < 16:
                candidates.add(i - o)
                i = new.find(r, i + 1)
                n += 1
    best = None
    for c in candidates:
        if c < 0 or c >= len(new):
            continue
        window = new[c:c + item.size + 64]
        sm = difflib.SequenceMatcher(None, body, window, autojunk=False)
        ratio = sm.ratio()
        if best is None or ratio > best[2]:
            blocks = [b for b in sm.get_matching_blocks() if b.size]
            end = (blocks[-1].b + blocks[-1].size + (item.size - blocks[-1].a - blocks[-1].size)) if blocks else item.size
            best = (c, min(max(end, 1), len(window)), ratio)
    return best


def disassemble(data, base):
    try:
        import capstone
    except ImportError:
        return None
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    return ['%04X  %-24s %s %s' % (i.address, i.bytes.hex(' ').upper(), i.mnemonic, i.op_str) for i in md.disasm(bytes(data), base)]


def function_report(item, old, new, old_off, new_off, new_size, ratio, labels, use_disasm):
    a = old[old_off:old_off + item.size]
    b = new[new_off:new_off + new_size]
    out = ['%s  (%s %s, IDA ea %X)' % (item.label, labels[0], labels[1], item.ea), '',
           '%s: file offset 0x%X, %d bytes' % (labels[0], old_off, len(a)),
           '%s: file offset 0x%X, %d bytes (similarity %.2f)' % (labels[1], new_off, len(b), ratio), '',
           'Changes (offsets relative to the function start; ea = IDA address in the %s database;' % labels[0],
           '"fixup" = only relocation slots differ, i.e. a segment reference, not the code):']
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'equal':
            continue
        if tag == 'replace' and (i2 - i1) == (j2 - j1) and not any(item.mask[i1:i2]):
            tag = 'fixup'                                        # only relocation slots: a segment reference, not code
        out.append('  %-7s +0x%04X (ea %X)  %s: %s' % (tag, i1, item.ea + i1, labels[0], hexdump(a[i1:i2]) or '-'))
        out.append('  %-7s %s %s: %s' % ('', ' ' * 18, labels[1], hexdump(b[j1:j2]) or '-'))
    if use_disasm:
        da, db = disassemble(a, item.ea & 0xFFFF), disassemble(b, item.ea & 0xFFFF)
        if da is None:
            out += ['', '(--disasm: the capstone package is not installed)']
        else:
            strip = lambda lines: [l[4:] for l in lines]      # drop the address so moved code still lines up
            out += ['', 'Disassembly diff (16-bit, %s -> %s):' % labels]
            out += list(difflib.unified_diff(strip(da), strip(db), labels[0], labels[1], lineterm='', n=3))
            out += ['', 'Full disassembly, %s:' % labels[1]] + db
    return '\n'.join(out) + '\n'


def compare_exe(name, old, new, map_path, out_dir, labels, use_disasm):
    """Writes the EXE's report files; returns a summary dict."""
    base = os.path.join(out_dir, name)
    mo, mn = parse_mz(old), parse_mz(new)
    layout = ['# %s: layout' % name, '', '| | %s | %s |' % labels, '|---|---|---|']
    for key in ('file_size', 'header_size', 'load_module_size', 'load_module_end', 'after_load_module', 'relocations',
                'cs_ip', 'ss_sp', 'min_alloc', 'max_alloc', 'fbov_offset', 'fbov_header'):
        layout.append('| %s | %s | %s |' % (key, (mo or {}).get(key, ''), (mn or {}).get(key, '')))
    write(os.path.join(base, 'layout.md'), '\n'.join(layout) + '\n')

    summary = {'kind': 'exe', 'same_size': len(old) == len(new)}
    covered = []                                                # (old_start, old_end, label) of mapped items

    if map_path:
        items = load_map(map_path)
        functions = [i for i in items if i.kind == 'F']
        data_items = [i for i in items if i.kind == 'D']
        rows, unchanged, moved, not_in_old, unplaceable, ambiguous = [], 0, 0, [], 0, 0
        old_pairs, new_pairs = {}, {}
        found = {}
        for f in functions:
            hits_old = f.find(old)
            if hits_old is None:
                unplaceable += 1
                continue
            if not hits_old:
                not_in_old.append(f)
                continue
            if len(hits_old) > 1:
                ambiguous += 1
            o = hits_old[0]
            covered.append((o, o + f.size, f.label))
            old_pairs.setdefault(f.segment, []).append((f.ea, o))
            hits_new = f.find(new)
            if hits_new:
                n = min(hits_new, key=lambda h: abs(h - o))
                new_pairs.setdefault(f.segment, []).append((f.ea, n))
                unchanged += 1
                moved += (n != o)
            else:
                found[f] = o
        # where the new file puts each segment, from the functions that did not change
        seg_delta_new = {s: most_common_delta(p) for s, p in new_pairs.items()}
        seg_delta_old = {s: most_common_delta(p) for s, p in old_pairs.items()}
        resident = [p for s, ps in old_pairs.items() if not s.lower().startswith('ovr') for p in ps]
        resident_new = [p for s, ps in new_pairs.items() if not s.lower().startswith('ovr') for p in ps]
        res_old, res_new = most_common_delta(resident), most_common_delta(resident_new)

        os.makedirs(os.path.join(base, 'functions'), exist_ok=True)
        for f, o in sorted(found.items(), key=lambda kv: kv[1]):
            d = seg_delta_new.get(f.segment)
            hint = (f.ea + d) if d is not None else None
            loc = locate_changed(f, old, new, o, hint)
            if loc is None:
                rows.append((f, o, None, None, 0.0))
                continue
            n, size, ratio = loc
            file_name = safe_name(f.wzd or f.name) + '.txt'
            write(os.path.join(base, 'functions', file_name), function_report(f, old, new, o, n, size, ratio, labels, use_disasm))
            rows.append((f, o, n, size, ratio))

        md = ['# %s: changed functions (%s -> %s)' % (name, labels[0], labels[1]), '',
              '%d functions in the map: %d unchanged (%d of them moved), %d changed, %d not found in %s, '
              '%d too short to place, %d found more than once (first match used).'
              % (len(functions), unchanged, moved, len(rows), len(not_in_old), labels[0], unplaceable, ambiguous), '',
              'ReMoM names its functions by these ids: `grep -rn "WZD s161p04" MoM MoX` finds the C function.', '',
              '| Id | IDA name | Segment | Old offset | New offset | Old size | New size | Similarity | Details |',
              '|---|---|---|---|---|---|---|---|---|']
        for f, o, n, size, ratio in rows:
            md.append('| %s | %s | %s | 0x%X | %s | %d | %s | %.2f | %s |' % (
                f.wzd, f.name, f.segment, o, ('0x%X' % n) if n is not None else 'not found', f.size,
                size if size is not None else '', ratio, ('functions/' + safe_name(f.wzd or f.name) + '.txt') if n is not None else ''))
        if not_in_old:
            md += ['', '## Not found in %s' % labels[0], '',
                   'These functions from the map do not match the %s file: is the map from the same version?' % labels[0], '']
            md += ['- %s (%s, %d bytes)' % (f.label, f.segment, f.size) for f in not_in_old[:200]]
        write(os.path.join(base, 'functions.md'), '\n'.join(md) + '\n')

        # named data, placed through its segment's offset (or the resident image's)
        dmd = ['# %s: changed data items (%s -> %s)' % (name, labels[0], labels[1]), '',
               '| Item | Segment | IDA ea | Size | Changed bytes (offset in item: old -> new) |', '|---|---|---|---|---|']
        changed_data, unplaced = 0, 0
        for d in data_items:
            dold = seg_delta_old.get(d.segment, res_old if not d.segment.lower().startswith('ovr') else None)
            dnew = seg_delta_new.get(d.segment, res_new if not d.segment.lower().startswith('ovr') else None)
            if dold is None or dnew is None or not d.matches_at(old, d.ea + dold):
                unplaced += 1
                continue
            o, n = d.ea + dold, d.ea + dnew
            covered.append((o, o + d.size, d.name))
            a, b = old[o:o + d.size], new[n:n + d.size]
            if a == b:
                continue
            changed_data += 1
            parts = []
            for s, e in changed_ranges(a, b, gap=0)[:40]:
                if d.size % 2 == 0 and s % 2 == 0 and (e - s) <= 2:
                    va, vb = struct.unpack_from('<h', a, s)[0], struct.unpack_from('<h', b, s)[0] if s + 2 <= len(b) else None
                    parts.append('+%d: %s -> %s' % (s, va, vb))
                else:
                    parts.append('+%d: %s -> %s' % (s, hexdump(a[s:e], 16), hexdump(b[s:e], 16)))
            dmd.append('| %s | %s | %X | %d | %s |' % (d.name, d.segment, d.ea, d.size, '; '.join(parts)))
        dmd[1:1] = ['', '%d named data items: %d changed, %d could not be placed in both files.' % (len(data_items), changed_data, unplaced)]
        write(os.path.join(base, 'data.md'), '\n'.join(dmd) + '\n')
        summary.update({'functions': len(functions), 'functions_changed': len(rows), 'functions_moved': moved,
                        'functions_not_in_old': len(not_in_old), 'data_changed': changed_data})

    # byte ranges by position (only meaningful when the layout did not change)
    rmd = ['# %s: changed byte ranges (%s -> %s)' % (name, labels[0], labels[1]), '']
    if len(old) == len(new):
        covered.sort()
        def owner(s, e):
            return [lab for cs, ce, lab in covered if cs < e and s < ce]
        ranges = changed_ranges(old, new)
        mz_reloc = (mo or {}).get('_reloc_bytes', set())
        free = [(s, e) for s, e in ranges if not owner(s, e)]
        rmd += ['%d changed ranges, %d of them outside every mapped function and data item (listed below).' % (len(ranges), len(free)), '',
                '| File offset | Bytes | %s | %s | Text (%s / %s) |' % (labels + labels), '|---|---|---|---|---|']
        for s, e in free:
            note = ' (relocation)' if any(x in mz_reloc for x in range(s, e)) else ''
            rmd.append('| 0x%X%s | %d | %s | %s | `%s` / `%s` |' % (s, note, e - s, hexdump(old[s:e], 32), hexdump(new[s:e], 32),
                                                               printable(old[max(s - 8, 0):e + 8]), printable(new[max(s - 8, 0):e + 8])))
        summary['changed_ranges'] = len(ranges)
        summary['unattributed_ranges'] = len(free)
    else:
        rmd.append('The files differ in size (%d -> %d), so ranges by position are not meaningful; see functions.md and strings.md.' % (len(old), len(new)))
    write(os.path.join(base, 'regions.md'), '\n'.join(rmd) + '\n')

    so, sn = strings_with_offsets(old), strings_with_offsets(new)
    co, cn = Counter(s for _, s in so), Counter(s for _, s in sn)
    removed = [(o, s) for o, s in so if co[s] > cn[s]]
    added = [(o, s) for o, s in sn if cn[s] > co[s]]
    smd = ['# %s: text (%s -> %s)' % (name, labels[0], labels[1]), '',
           'Printable runs of %d+ characters present more often in one file than the other.' % STRING_MIN, '',
           '## Only in %s (%d)' % (labels[0], len(removed)), ''] + ['- 0x%X `%s`' % (o, s.replace('`', "'")) for o, s in removed] + \
          ['', '## Only in %s (%d)' % (labels[1], len(added)), ''] + ['- 0x%X `%s`' % (o, s.replace('`', "'")) for o, s in added]
    write(os.path.join(base, 'strings.md'), '\n'.join(smd) + '\n')
    summary.update({'strings_removed': len(removed), 'strings_added': len(added)})
    return summary


# --------------------------------------------------------------------------------------------------------------------
#  LBX archives
# --------------------------------------------------------------------------------------------------------------------

def compare_lbx(name, old, new, out_dir, labels):
    eo, en = parse_lbx(old), parse_lbx(new)
    if eo is None or en is None:
        return None
    md = ['# %s (%s -> %s)' % (name, labels[0], labels[1]), '', 'Entries: %d -> %d' % (len(eo), len(en)), '']
    changed = 0
    for idx in range(max(len(eo), len(en))):
        if idx >= len(eo) or idx >= len(en):
            md.append('- entry %d: only in %s' % (idx, labels[1] if idx >= len(eo) else labels[0]))
            changed += 1
            continue
        _, nm, so, lo = eo[idx]
        _, _, sn, ln = en[idx]
        a, b = old[so:so + lo], new[sn:sn + ln]
        if a == b:
            continue
        changed += 1
        table = TABLES.get((name.upper(), idx))
        title = 'entry %d%s' % (idx, (' "%s"' % nm) if nm else '')
        if table is None or len(a) < 4 or len(b) < 4:
            diff = len(changed_ranges(a, b)) if len(a) == len(b) else None
            md.append('- %s: %d -> %d bytes%s' % (title, len(a), len(b), ('' if diff is None else ', %d changed ranges' % diff)))
            continue
        ca, sa = struct.unpack_from('<hh', a, 0)
        cb, sb = struct.unpack_from('<hh', b, 0)
        md += ['', '## %s: table, %d -> %d records of %d -> %d bytes' % (title, ca, cb, sa, sb), '']
        if sa != table.record_size or sb != table.record_size:
            md.append('Record size differs from the known layout (%d); not decoded.' % table.record_size)
            continue
        name_field = next((f for f in table.fields if f.name == 'name'), None)
        md += ['| Record | Name | Field | %s | %s |' % labels, '|---|---|---|---|---|']
        for r in range(max(ca, cb)):
            ra = a[4 + r * sa:4 + (r + 1) * sa] if r < ca else None
            rb = b[4 + r * sb:4 + (r + 1) * sb] if r < cb else None
            if ra == rb:
                continue
            label = (name_field.unpack(ra if ra is not None else rb) if name_field else '') or ''
            if ra is None or rb is None or len(ra) < sa or len(rb) < sb:
                md.append('| %d | %s | (whole record) | %s | %s |' % (r, label, 'present' if ra else '-', 'present' if rb else '-'))
                continue
            known = bytearray(len(ra))
            for f in table.fields:
                va, vb = f.unpack(ra), f.unpack(rb)
                for k in range(f.offset, min(f.offset + f.width, len(known))):
                    known[k] = 1
                if va != vb:
                    md.append('| %d | %s | %s | %s | %s |' % (r, label, f.name, va, vb))
            for s, e in changed_ranges(ra, rb, gap=0):
                if not any(known[s:e]):
                    md.append('| %d | %s | (unnamed bytes +%d) | %s | %s |' % (r, label, s, hexdump(ra[s:e]), hexdump(rb[s:e])))
    if not changed:
        return {'kind': 'lbx', 'entries_changed': 0}
    write(os.path.join(out_dir, name + '.md'), '\n'.join(md) + '\n')
    return {'kind': 'lbx', 'entries_changed': changed}


# --------------------------------------------------------------------------------------------------------------------
#  Driver
# --------------------------------------------------------------------------------------------------------------------

def is_text(data):
    if not data:
        return True
    sample = data[:4096]
    return sum(1 for b in sample if b in (9, 10, 13) or 32 <= b < 127) / len(sample) > 0.95


def files_by_name(folder):
    out = {}
    for name in os.listdir(folder):
        path = os.path.join(folder, name)
        if os.path.isfile(path):
            out[name.upper()] = path
    return out


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    p.add_argument('old_dir')
    p.add_argument('new_dir')
    p.add_argument('--out', required=True)
    p.add_argument('--map', action='append', default=[], help='EXE=map.csv from export_map.idc (repeatable)')
    p.add_argument('--old-label')
    p.add_argument('--new-label')
    p.add_argument('--disasm', action='store_true', help='disassemble changed functions (needs capstone)')
    args = p.parse_args(argv)

    labels = (args.old_label or os.path.basename(os.path.normpath(args.old_dir)),
              args.new_label or os.path.basename(os.path.normpath(args.new_dir)))
    maps = {}
    for m in args.map:
        exe, _, path = m.partition('=')
        maps[exe.upper()] = path
    old_files, new_files = files_by_name(args.old_dir), files_by_name(args.new_dir)
    os.makedirs(args.out, exist_ok=True)

    rows, details = [], {}
    for name in sorted(set(old_files) | set(new_files)):
        if name not in new_files or name not in old_files:
            rows.append((name, 'only in %s' % (labels[0] if name in old_files else labels[1]), '', '', '', ''))
            continue
        with open(old_files[name], 'rb') as f:
            old = f.read()
        with open(new_files[name], 'rb') as f:
            new = f.read()
        if old == new:
            rows.append((name, 'same', len(old), len(new), sha256(old)[:16], ''))
            continue
        info = None
        where = ''
        if parse_mz(old) and parse_mz(new):
            info = compare_exe(name, old, new, maps.get(name), args.out, labels, args.disasm)
            where = name + '/'
        elif name.endswith('.LBX'):
            info = compare_lbx(name, old, new, args.out, labels)
            where = name + '.md'
        if info is None and is_text(old) and is_text(new):
            diff = difflib.unified_diff(old.decode('latin-1').splitlines(), new.decode('latin-1').splitlines(),
                                        labels[0] + '/' + name, labels[1] + '/' + name, lineterm='')
            write(os.path.join(args.out, name + '.diff'), '\n'.join(diff) + '\n')
            info, where = {'kind': 'text'}, name + '.diff'
        if info is None:
            info = {'kind': 'binary', 'changed_ranges': len(changed_ranges(old, new)) if len(old) == len(new) else None}
        details[name] = dict(info, old_sha256=sha256(old), new_sha256=sha256(new), old_size=len(old), new_size=len(new))
        rows.append((name, 'changed', len(old), len(new), sha256(old)[:16] + ' -> ' + sha256(new)[:16], where))

    md = ['# %s -> %s' % labels, '', 'Generated by tools/mom_patch_diff.', '',
          '| File | Status | Old size | New size | SHA-256 (first 16) | Details |', '|---|---|---|---|---|---|']
    md += ['| %s | %s | %s | %s | %s | %s |' % r for r in rows]
    write(os.path.join(args.out, 'summary.md'), '\n'.join(md) + '\n')
    write(os.path.join(args.out, 'summary.json'), json.dumps({'old': labels[0], 'new': labels[1], 'files': details}, indent=1, default=str) + '\n')
    print('%s -> %s: %d files compared, %d changed; report in %s' % (labels + (len(rows), len(details), args.out)))
    return 0


if __name__ == '__main__':
    sys.exit(main())
