#!/usr/bin/env python3
"""Starlords (May 1993 pitch / preview of MoO1) corpus: shipped file names, LBX archive layout, LBX text, EXE strings.

The Starlords counterpart of simtex_moo1_corpus.py.  Starlords predates both formats MoO1 and MoM use:
  - LBX: no 0xFEAD header and no 0x200 name table.  An archive is a table of 8-byte slots (u32 offset, u32 0);
    the first offset is the table size; the last offset is the file size.  Some .LBX files are a single picture
    (320x200 header) or raw data (text tables) instead.
  - EXE: Microsoft C, NE (segmented) executables behind a DOS stub loader; read with ne_exe.py.

Inputs:
  --sl-dir     the Starlords install (default C:/STU_DASM/_SL)
  doc/@SimTex_Style/SimTex-Corpus-MoO1-*.fwv  (run simtex_moo1_corpus.py first) for the MoO1 comparison.

Outputs (--outdir, default doc/@SimTex_Style):
  SimTex-Corpus-Starlords-Files.fwv          every file, classified, with its LBX layout
  SimTex-Corpus-Starlords-LBX-Entries.fwv    archive slots: offset and size (there are no entry names)
  SimTex-Corpus-Starlords-LBX-Text.fwv       text fields of the text-bearing LBX files
  SimTex-Corpus-Starlords-EXE-Strings.fwv    every DGROUP string of the six EXEs, with referencing code segments
  SimTex-Corpus-Starlords-Literals.fwv       the file-name literals among them
  SimTex-Corpus-Starlords-Stats.md           the statistics and the MoO1 comparison

stdlib only.  Run:  python3 tools/simtex_style/simtex_starlords_corpus.py
"""

import argparse
import collections
import csv
import datetime
import os
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import ne_exe  # noqa: E402
import simtex_authorship as sa  # noqa: E402
import simtex_mom_corpus as mc  # noqa: E402
import simtex_name_score as ns  # noqa: E402

STYLE_DIR = os.path.join(sa.REPO, 'doc', '@SimTex_Style')
EXES = ['START.EXE', 'VORTEX.EXE', 'STARLORD.EXE', 'FILES.EXE', 'SETUP.EXE', 'TEST.EXE']
THIRD_PARTY = {'CHKLIST.MS'}                 # MS-DOS 6 Anti-Virus checklist (1995), not part of the build
RUNTIME = {'STARLORD.CFG', 'SAVE7.GAM'}      # written by SETUP / the game (both dated 2000)
FILE_LIT_RE = re.compile(r'^[A-Za-z0-9_]{1,8}\.[A-Za-z]{3}$')
TEXT_MIN_RUNS = 10                           # an LBX is text-bearing if it has at least this many sentence-like fields


def classify(name):
    up = name.upper()
    if up in EXES:
        return 'simtex_exe'
    if up.endswith('.LBX'):
        return 'simtex_lbx'
    if up in THIRD_PARTY:
        return 'third_party'
    if up in RUNTIME:
        return 'runtime'
    return 'user_added'


def lbx_layout(data):
    """('archive', [(offset, size)]) | ('picture', w, h, frames) | ('raw', None)."""
    if len(data) >= 8:
        first = struct.unpack_from('<I', data, 0)[0]
        if 16 <= first < len(data) and first % 8 == 0:
            slots = [struct.unpack_from('<II', data, 8 * i) for i in range(first // 8)]
            offs = [a for a, b in slots]
            if all(b == 0 for a, b in slots) and all(x <= y for x, y in zip(offs, offs[1:])) and offs[-1] == len(data):
                return 'archive', [(a, b - a) for a, b in zip(offs, offs[1:])]
        w, h, cur, frames = struct.unpack_from('<HHHH', data, 0)
        if (w, h) == (320, 200):
            return 'picture', (w, h, frames)
    return 'raw', None


def text_fields(data):
    """Sentence-like NUL-terminated fields: the printable tail of each NUL-separated chunk, >= 4 letters."""
    out = []
    pos = 0
    for chunk in data.split(b'\x00'):
        m = re.search(rb'[\x20-\x7e]+$', chunk)
        if m and sum(chr(c).isalpha() for c in m.group(0)) >= 4:
            out.append((pos + m.start(), m.group(0).decode('latin-1')))
        pos += len(chunk) + 1
    return out


def sentence_like(t):
    return ' ' in t.strip() and sum(c.isalpha() for c in t) >= 6 and len(set(t.replace(' ', ''))) >= 6


def norm(t):
    """Compare text across versions: drop insertion codes (~ ^ % in Starlords, \\xNN in MoO1), case and spacing."""
    t = re.sub(r'\\x[0-9A-F]{2}', ' ', t)
    t = re.sub(r'[~^%]', ' ', t)
    return re.sub(r'[^a-z0-9]+', ' ', t.lower()).strip()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--sl-dir', default='C:/STU_DASM/_SL')
    ap.add_argument('--moo1-dir', default='C:/STU_DASM/_MoO1v130', help='MoO1 v1.30 install, for the raw DIPLOMAT.LBX check')
    ap.add_argument('--outdir', default=STYLE_DIR)
    args = ap.parse_args()
    d = args.sl_dir
    out = args.outdir

    # ---- files and LBX -----------------------------------------------------------------------------------------------
    file_rows, slot_rows, text_rows = [], [], []
    for fn in sorted(os.listdir(d)):
        p = os.path.join(d, fn)
        if not os.path.isfile(p):
            continue
        st = os.stat(p)
        stem, ext = mc.split_name(fn)
        row = {'file': fn, 'stem': stem, 'ext': ext, 'bytes': st.st_size, 'date': datetime.date.fromtimestamp(st.st_mtime).isoformat(),
               'class': classify(fn), 'layout': '', 'entries': '', 'text_fields': ''}
        if fn.upper().endswith('.LBX'):
            data = open(p, 'rb').read()
            kind, info = lbx_layout(data)
            row['layout'] = kind
            if kind == 'archive':
                row['entries'] = len(info)
                for i, (o, sz) in enumerate(info):
                    slot_rows.append({'lbx': fn, 'entry': i, 'offset': o, 'bytes': sz})
            elif kind == 'picture':
                row['entries'] = info[2]
            if kind != 'picture':
                fields = text_fields(data)
                good = [(o, t) for o, t in fields if sentence_like(t)]
                if len(good) >= TEXT_MIN_RUNS:
                    row['text_fields'] = len(fields)
                    for o, t in fields:
                        text_rows.append({'lbx': fn, 'offset': f'{o:05X}', 'chars': len(t), 'case': mc.case_class(t), 'text': t})
        file_rows.append(row)
    lbx_stems = {r['stem'].upper() for r in file_rows if r['class'] == 'simtex_lbx'}

    # ---- EXEs --------------------------------------------------------------------------------------------------------
    exes = {}
    string_rows, lit_rows = [], []
    for fn in EXES:
        e = ne_exe.NeExe(os.path.join(d, fn))
        name = fn[:-4]
        exes[name] = e
        strs = e.strings(3)
        lits = [(o, t) for o, t in e.file_name_literals(lbx_stems | {x[:-4] for x in EXES} | {'STARLORD', 'GAME', 'TEMP', 'PICTCAPT'}) if FILE_LIT_RE.match(t)]
        refs = e.references([o for o, _ in strs] + [o for o, _ in lits])
        for k, (o, t) in enumerate(strs):
            r = refs.get(o, {})
            string_rows.append({'exe': name, 'order': k, 'ds_off': f'{o:04X}', 'chars': len(t), 'case': mc.case_class(t),
                                'refs': ' '.join(f'{n}:{c}' for n, c in sorted(r.items())), 'text': t.replace('\r', '\\r').replace('\n', '\\n')})
        for k, (o, t) in enumerate(lits):
            r = refs.get(o, {})
            stem, ext = mc.split_name(t)
            lit_rows.append({'exe': name, 'order': k, 'ds_off': f'{o:04X}', 'literal': t, 'stem': stem, 'ext': ext,
                             'case_stem': mc.case_class(stem), 'case_ext': mc.case_class(ext), 'n_refs': sum(r.values()),
                             'primary_region': max(r, key=r.get) if r else '', 'is_shipped_file': int(t.upper() in {f['file'].upper() for f in file_rows}),
                             'regions': ' '.join(f'{n}:{c}' for n, c in sorted(r.items()))})

    ns.write_fwv(os.path.join(out, 'SimTex-Corpus-Starlords-Files.fwv'), ['file', 'stem', 'ext', 'bytes', 'date', 'class', 'layout', 'entries', 'text_fields'], file_rows)
    ns.write_fwv(os.path.join(out, 'SimTex-Corpus-Starlords-LBX-Entries.fwv'), ['lbx', 'entry', 'offset', 'bytes'], slot_rows)
    ns.write_fwv(os.path.join(out, 'SimTex-Corpus-Starlords-LBX-Text.fwv'), ['lbx', 'offset', 'chars', 'case', 'text'], text_rows)
    ns.write_fwv(os.path.join(out, 'SimTex-Corpus-Starlords-EXE-Strings.fwv'), ['exe', 'order', 'ds_off', 'chars', 'case', 'refs', 'text'], string_rows)
    ns.write_fwv(os.path.join(out, 'SimTex-Corpus-Starlords-Literals.fwv'),
                 ['exe', 'order', 'ds_off', 'literal', 'stem', 'ext', 'case_stem', 'case_ext', 'n_refs', 'primary_region', 'is_shipped_file', 'regions'], lit_rows)

    # ---- MoO1 comparison data ----------------------------------------------------------------------------------------
    m_files = ns.read_fwv(os.path.join(STYLE_DIR, 'SimTex-Corpus-MoO1-Files.fwv'))
    m_text = ns.read_fwv(os.path.join(STYLE_DIR, 'SimTex-Corpus-MoO1-LBX-Text.fwv'))
    m_lits = ns.read_fwv(os.path.join(STYLE_DIR, 'SimTex-Corpus-MoO1-Literals.fwv'))
    m_strings = ns.read_fwv(os.path.join(STYLE_DIR, 'SimTex-Corpus-MoO1-EXE-Strings.fwv'))
    m_stems = {r['stem'].upper() for r in m_files if r['class'] == 'simtex_lbx'}
    with open(os.path.join(STYLE_DIR, 'SimTex-Corpus-MoM-Files.csv'), encoding='latin-1', newline='') as f:
        mom_stems = {r['stem'].upper() for r in csv.DictReader(f) if r['install'] == 'MoM131' and r['class'] == 'simtex_lbx'}

    # ---- stats -------------------------------------------------------------------------------------------------------
    L = []
    p = L.append
    p('# SimTex Corpus: Starlords (May 1993 preview of MoO1)')
    p('')
    p('Generated by [tools/simtex_style/simtex_starlords_corpus.py](../../tools/simtex_style/simtex_starlords_corpus.py) from `' + d + '`. '
      'Compared against [MoO1 v1.30](SimTex-Corpus-MoO1-Stats.md).')
    p('')
    p('Tables: [Files](SimTex-Corpus-Starlords-Files.fwv) · [LBX archive slots](SimTex-Corpus-Starlords-LBX-Entries.fwv) · [LBX text](SimTex-Corpus-Starlords-LBX-Text.fwv) · '
      '[EXE strings](SimTex-Corpus-Starlords-EXE-Strings.fwv) · [file-name literals](SimTex-Corpus-Starlords-Literals.fwv)')
    p('')
    dates = collections.Counter(r['date'] for r in file_rows if r['class'] in ('simtex_lbx', 'simtex_exe'))
    p(f'- **Build date:** every game file is dated ' + ', '.join(f'{k} ({v})' for k, v in dates.most_common()) + '.')
    p('- **Toolchain:** Microsoft C (the `MS Run-Time Library - Copyright` string), NE segmented executables behind a DOS stub loader. '
      'MoO1 v1.30 (September 1993) and MoM are Borland C++ MZ executables with VROOMM overlays.')
    p('')
    p('| EXE | Code segments | DGROUP strings | File-name literals | of them referenced |')
    p('|---|---:|---:|---:|---:|')
    for name, e in exes.items():
        ll = [r for r in lit_rows if r['exe'] == name]
        p(f'| {name} | {len(e.code_regions)} | {sum(1 for r in string_rows if r["exe"] == name)} | {len(ll)} | {sum(1 for r in ll if r["n_refs"])} |')
    p('')
    chain = collections.defaultdict(set)
    for r in lit_rows:
        if r['ext'].upper() == 'EXE':
            chain[r['exe']].add(r['literal'])
    p('- **Program chain** (EXE names in each EXE\'s strings): ' + '; '.join(f'{k} → ' + ', '.join(f'`{x}`' for x in sorted(v)) for k, v in chain.items()) + '.')
    stub = exes['STARLORD'].stub_strings
    p('- **Stub loader strings:** ' + ', '.join(f'`{s}`' for s in stub[:6]) + '.')
    p('')

    p('## Files')
    p('')
    p('| Class | Files |')
    p('|---|---:|')
    for k, v in collections.Counter(r['class'] for r in file_rows).most_common():
        p(f'| {k} | {v} |')
    p('')
    stems = [r['stem'] for r in file_rows if r['class'] == 'simtex_lbx']
    m_stem_list = [r['stem'] for r in m_files if r['class'] == 'simtex_lbx']
    p('| | Starlords | MoO1 |')
    p('|---|---:|---:|')
    p(f'| LBX files | {len(stems)} | {len(m_stem_list)} |')
    p(f'| stems of exactly 8 characters | {sum(len(s) == 8 for s in stems)} | {sum(len(s) == 8 for s in m_stem_list)} |')
    p(f'| mean stem length | {sum(map(len, stems)) / len(stems):.2f} | {sum(map(len, m_stem_list)) / len(m_stem_list):.2f} |')
    p('')
    up = {s.upper() for s in stems}
    p(f'- **Stems kept in MoO1:** {len(up & m_stems)}: ' + ', '.join(f'`{s}`' for s in sorted(up & m_stems)) + '.')
    p(f'- **Starlords only:** {len(up - m_stems)}: ' + ', '.join(f'`{s}`' for s in sorted(up - m_stems)) + '.')
    p(f'- **MoO1 only:** {len(m_stems - up)}: ' + ', '.join(f'`{s}`' for s in sorted(m_stems - up)) + '.')
    skip = sorted((up - m_stems) & mom_stems)
    p(f'- **Starlords stems absent from MoO1 but back in MoM:** ' + (', '.join(f'`{s}`' for s in skip) or 'none') + '. '
      'MoO1 has `DIPLOMAT` and `WINLOSE` in their place.')
    p(f'- **Stems in Starlords, MoO1 and MoM:** ' + ', '.join(f'`{s}`' for s in sorted(up & m_stems & mom_stems)) + '.')
    p('- **Renamed for MoO1:** `DIPLOMAC` → `DIPLOMAT` is confirmed by content (the diplomacy messages carry over, below). '
      'By name alone, and unconfirmed: `DESIGNS` → `DESIGN`, `NEBULAS` → `NEBULA`, `WIN` + `LOSE` → `WINLOSE`, `ISOUND` → `INTROSND`, `SOUND` → `SOUNDFX`.')
    p('')

    p('## LBX layout')
    p('')
    lay = collections.Counter(r['layout'] for r in file_rows if r['class'] == 'simtex_lbx')
    p('| Layout | Files |')
    p('|---|---:|')
    for k, v in lay.most_common():
        p(f'| {k} | {v} |')
    p('')
    p('- **archive:** a table of 8-byte slots (u32 offset, u32 zero); the first offset is the table size and the last is the file size. '
      'No `0xFEAD` signature, no type word, no 0x200 name table. So the per-entry names and descriptions that MoO1 and MoM carry did not exist yet.')
    p('- **picture:** a bare 320×200 animation (' + ', '.join(f'`{r["file"]}` {r["entries"]} frames' for r in file_rows if r['layout'] == 'picture') + '). '
      'The header (width, height, current frame, frame count) is the one MoO1 / MoM pictures carry inside their LBX entries.')
    p('- **raw:** ' + ', '.join(f'`{r["file"]}`' for r in file_rows if r['layout'] == 'raw') + '.')
    p(f'- **Archive entries:** {len(slot_rows)} in {lay["archive"]} archives.')
    p('')

    p('## LBX text')
    p('')
    p('| LBX | Text fields | Example |')
    p('|---|---:|---|')
    for r in file_rows:
        if r['text_fields']:
            ex = next((t['text'] for t in text_rows if t['lbx'] == r['file'] and sentence_like(t['text'])), '')
            p(f'| {r["file"]} | {r["text_fields"]} | {ex[:70].replace("|", "/")} |')
    p('')
    # carried-over text
    m_norm = {norm(t['text']): t for t in m_text if len(norm(t['text'])) >= 12}
    s_norm = [(t, norm(t['text'])) for t in text_rows if len(norm(t['text'])) >= 12]
    same = [(t, m_norm[n]) for t, n in s_norm if n in m_norm]
    p(f'- **Text carried into MoO1 unchanged** (ignoring case, spacing and insertion codes): {len(same)} of {len(s_norm)} Starlords fields of 12+ characters.')
    by = collections.Counter((t['lbx'], m['lbx']) for t, m in same)
    p('  ' + ', '.join(f'{a} → {b}: {v}' for (a, b), v in by.most_common()) + '.')
    tech = [(t, m) for t, m in same if t['lbx'] == 'RESEARCH.LBX' and t['text'] != m['text']]
    tech_up = [(t, m) for t, m in tech if t['case'] == 'UPPER']
    p(f'- **Research text changed case:** {len(tech_up)} of {sum(1 for t, m in same if t["lbx"] == "RESEARCH.LBX")} carried RESEARCH fields were all upper case in Starlords '
      'and are mixed case in MoO1, e.g. ' + ', '.join(f'`{t["text"][:28]}` → `{m["text"][:28]}`' for t, m in tech_up[:4]) + '.')
    dip = [(t, m) for t, m in same if t['lbx'] == 'DIPLOMAC.LBX']
    emp = sum(1 for t, m in dip if re.search(r'\bemperor\b', t['text']) and re.search(r'\bEmperor\b', m['text']))
    lead = sum(1 for t, m in dip if t['text'].startswith('  '))
    moo1_dip = os.path.join(args.moo1_dir, 'DIPLOMAT.LBX')
    mfields = [f for f in open(moo1_dip, 'rb').read().split(b'\x00') if len(f) > 20] if os.path.exists(moo1_dip) else []
    p(f'- **Diplomacy text:** {len(dip)} messages carried over; in {emp} of them `emperor` became `Emperor`. {lead} of the {len(dip)} start with two spaces in Starlords; '
      f'{sum(1 for f in mfields if f.startswith(b" "))} of MoO1\'s {len(mfields)} diplomacy fields start with a space.')
    marks = collections.Counter(c for t in text_rows if t['lbx'].upper().startswith(('DIPLOMAC', 'CRISES')) for c in t['text'] if c in '~^%@#$&*')
    p('- **Insertion codes:** Starlords marks inserted names with printable characters (' + ', '.join(f'`{k}` ×{v}' for k, v in marks.most_common()) +
      '); MoO1 uses control bytes (`\\x80`–`\\x83`).')
    p('')

    p('## File-name literals in the EXEs')
    p('')
    cc = collections.Counter((r['case_stem'], r['case_ext']) for r in lit_rows)
    p('| Stem / extension case | Starlords | MoO1 |')
    p('|---|---:|---:|')
    mcc = collections.Counter((r['case_stem'], r['case_ext']) for r in m_lits if r.get('form', 'full') == 'full')
    for k in sorted(set(cc) | set(mcc), key=lambda k: -cc[k]):
        p(f'| {k[0]} / {k[1]} | {cc[k]} | {mcc[k]} |')
    p('')
    odd = sorted({(r['literal'], r['exe']) for r in lit_rows if r['case_stem'] != 'UPPER' or r['case_ext'] != 'UPPER'})
    p('- **Not all upper case:** ' + ', '.join(f'`{a}` ({b})' for a, b in odd) + '.')
    notship = sorted({r['literal'] for r in lit_rows if not r['is_shipped_file']})
    p('- **Names that are not shipped files:** ' + ', '.join(f'`{s}`' for s in notship) + '.')
    p(f'- **Unreferenced:** {sum(1 for r in lit_rows if not r["n_refs"])} of {len(lit_rows)}.')
    dup = collections.Counter((r['exe'], r['literal'].upper()) for r in lit_rows)
    p(f'- **Duplicated literals:** the same name is stored once per use site (`STARMAP.LBX` ×{dup[("STARLORD", "STARMAP.LBX")]} in STARLORD, '
      f'`VORTEX.LBX` ×{dup[("VORTEX", "VORTEX.LBX")]} in VORTEX): the compiler did not pool identical strings.')
    p('')

    p('## Strings shared with MoO1')
    p('')
    s_set = {r['text'] for r in string_rows if len(r['text']) >= 6}
    m_set = {r['text'] for r in m_strings if len(r['text']) >= 6}
    common = sorted(s_set & m_set)
    p(f'{len(common)} distinct EXE strings of 6+ characters occur in both Starlords and MoO1 v1.30.')
    p('')
    p('Examples: ' + ', '.join(f'`{t[:50]}`' for t in common[:40]) + '.')
    p('')
    with open(os.path.join(out, 'SimTex-Corpus-Starlords-Stats.md'), 'w', encoding='utf-8', newline='\n') as f:
        f.write('\n'.join(L) + '\n')
    sys.stdout.flush()
    sys.stdout.buffer.write(('\n'.join(L) + '\n').encode('utf-8'))


if __name__ == '__main__':
    main()
