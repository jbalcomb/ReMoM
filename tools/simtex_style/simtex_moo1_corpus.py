#!/usr/bin/env python3
"""MoO1 v1.30 corpus: shipped file names, LBX name tables, LBX string tables, and the strings in ORION.EXE / STARMAP.EXE.

The MoO1 counterpart of simtex_mom_corpus.py (doc/@SimTex_Style/SimTex-Coding-Style-And-Authorship.md section 5).
MoO1 has no IDA export, so the EXEs are read directly with borland_exe.py (validated against the MoM IDA exports:
the file-name literals and the overlays that reference them match on all 52 WIZARDS / MAGIC literals).

Inputs:
  --moo1-dir   the MoO1 v1.30 install (default C:/STU_DASM/_MoO1v130)
  doc/@SimTex_Style/SimTex-Corpus-MoM-*.csv and SimTex-Corpus-MoO2-LBX-Literals.csv, for the side-by-side numbers.

Outputs (--outdir, default doc/@SimTex_Style):
  SimTex-Corpus-MoO1-Files.fwv          every file in the install, classified
  SimTex-Corpus-MoO1-LBX-Entries.fwv    LBX header name table: 9-byte entry name + 23-byte description per entry
  SimTex-Corpus-MoO1-LBX-Text.fwv       LBX string tables: every text field of the type-5 / type-3 (help) records
  SimTex-Corpus-MoO1-EXE-Strings.fwv    every DGROUP string of both EXEs, with the code regions that reference it
  SimTex-Corpus-MoO1-Literals.fwv       the file-name literals among them, with case style
  SimTex-Corpus-MoO1-Stats.md           the statistics

STARMAP.EXE: the install's copy was patched (quiz bypass); STARMAP.EXE.quiz_bak holds the original 1994 bytes and is
the one read.  The tool reports the byte differences.

stdlib only.  Run:  python3 tools/simtex_style/simtex_moo1_corpus.py [--iters 20000] [--seed 1]
"""

import argparse
import collections
import csv
import datetime
import hashlib
import os
import random
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import borland_exe as be  # noqa: E402
import simtex_authorship as sa  # noqa: E402
import simtex_mom_corpus as mc  # noqa: E402
import simtex_name_score as ns  # noqa: E402

STYLE_DIR = os.path.join(sa.REPO, 'doc', '@SimTex_Style')

SIMTEX_EXES = {'ORION.EXE', 'STARMAP.EXE'}
THIRD_PARTY = {'FAT.AD', 'FAT.CAT', 'FAT.MT', 'FAT.OPL',                       # Miles AIL timbre / bank files
               'README-MOOV13.1ST'}                                              # a BBS download banner (Audiophiles Paradise), not MicroProse
PUBLISHER = {'INSTALL.EXE', 'MPSCOPY.EXE', 'PKUNZIP.EXE', 'README.TXT', 'README-MOOV12.TXT', 'README-MOOV13.TXT'}
RUNTIME = {'CONFIG.MOO', 'ERROR.LOG'}
SAVE_RE = re.compile(r'^SAVE\d\.GAM$')
DOS83_RE = re.compile(r'^[A-Z0-9_\-]{1,8}(\.[A-Z0-9_]{1,3})?$')
TEXT_TYPES = {3, 5}
GENERIC_FILE_RE = re.compile(r'^[A-Za-z0-9_]{1,8}\.(?:lbx|gam|moo|exe|fli|log)$', re.I)


def classify(name):
    up = name.upper()
    if up in SIMTEX_EXES:
        return 'simtex_exe'
    if up.endswith('.LBX') and DOS83_RE.match(up):
        return 'simtex_lbx'
    if up in THIRD_PARTY:
        return 'third_party'
    if up in PUBLISHER:
        return 'publisher'
    if up in RUNTIME:
        return 'runtime'
    if SAVE_RE.match(up):
        return 'user_save'
    return 'user_added'


def lbx_text_records(path):
    """Text fields of a type-5 (array) / type-3 (help) LBX: [(entry, record, field, text)].
    Each entry is  u16 n_records, u16 record_size, then n_records fixed-size records; text fields are NUL-terminated."""
    with open(path, 'rb') as f:
        data = f.read()
    count, magic, _r, lbx_type = struct.unpack_from('<HHHH', data, 0)
    offs = struct.unpack_from('<%dI' % (count + 1), data, 8)
    out = []
    for e in range(count):
        rec = data[offs[e]:offs[e + 1]]
        if len(rec) < 4:
            continue
        n, size = struct.unpack_from('<HH', rec, 0)
        if n * size + 4 != len(rec) or size == 0:
            continue
        for r in range(n):
            body = rec[4 + r * size: 4 + (r + 1) * size]
            fld = 0
            for raw in body.split(b'\x00'):
                if sum(chr(c).isalpha() for c in raw) < 2:
                    continue
                # control bytes (e.g. the name-insertion codes in DIPLOMAT / EVENTMSG) are kept, written as \xNN
                text = ''.join(chr(c) if 32 <= c < 127 or c in (10, 13) else f'\\x{c:02X}' for c in raw)
                out.append((e, r, fld, text))
                fld += 1
    return lbx_type, out


def refs_probe(e, ds_off):
    return bool(e.references([ds_off]).get(ds_off))


def fwv_text(s):
    return s.replace('\r', '\\r').replace('\n', '\\n')


def name_table_metrics(entries):
    """entries: [(lbx, idx, name, desc)] -> dict of the section-5.2 metrics."""
    named = [n for _, _, n, _ in entries if n]
    descs = [d for _, _, _, d in entries if d]
    prev = None
    repeats = 0
    for lbx, idx, n, d in entries:
        if n and prev == (lbx, n):
            repeats += 1
        prev = (lbx, n)
    by_name = collections.defaultdict(set)
    for lbx, _, n, _ in entries:
        if n:
            by_name[n].add(lbx)
    return {
        'entries': len(entries), 'named': len(named), 'distinct': len(set(named)),
        'upper': sum(1 for n in named if mc.case_class(n) in ('UPPER', 'none')),
        'eight': sum(1 for n in named if len(n) == 8),
        'repeat_prev': repeats,
        'multi_file': sum(1 for v in by_name.values() if len(v) > 1),
        'descs': len(descs),
        'desc_lower': sum(1 for d in descs if mc.case_class(d) == 'lower'),
        'desc_at_22': sum(1 for d in descs if len(d) == 22),
    }


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--moo1-dir', default='C:/STU_DASM/_MoO1v130')
    ap.add_argument('--outdir', default=STYLE_DIR)
    ap.add_argument('--iters', type=int, default=20000)
    ap.add_argument('--seed', type=int, default=1)
    args = ap.parse_args()
    rng = random.Random(args.seed)
    d = args.moo1_dir

    # ---- files -------------------------------------------------------------------------------------------------------
    file_rows = []
    lbx_entries = []
    text_rows = []
    for fn in sorted(os.listdir(d)):
        p = os.path.join(d, fn)
        if not os.path.isfile(p):
            continue
        st = os.stat(p)
        stem, ext = mc.split_name(fn)
        row = {'file': fn, 'stem': stem, 'ext': ext, 'bytes': st.st_size, 'date': datetime.date.fromtimestamp(st.st_mtime).isoformat(),
               'class': classify(fn), 'lbx_type': '', 'lbx_entries': '', 'has_name_table': '', 'text_fields': ''}
        if fn.upper().endswith('.LBX'):
            r = mc.read_lbx(p)
            if r:
                count, t, ents = r
                row.update(lbx_type=t, lbx_entries=count, has_name_table=int(bool(ents)))
                for i, (n, desc) in enumerate(ents or []):
                    lbx_entries.append((fn, i, n, desc))
                if t in TEXT_TYPES:
                    _, recs = lbx_text_records(p)
                    row['text_fields'] = len(recs)
                    for e, rr, fld, text in recs:
                        text_rows.append({'lbx': fn, 'entry': e, 'record': rr, 'field': fld, 'chars': len(text), 'case': mc.case_class(text), 'text': fwv_text(text)})
        file_rows.append(row)

    # ---- EXEs --------------------------------------------------------------------------------------------------------
    patched = os.path.join(d, 'STARMAP.EXE')
    orig = os.path.join(d, 'STARMAP.EXE.quiz_bak')
    exe_paths = {'ORION': os.path.join(d, 'ORION.EXE'), 'STARMAP': orig if os.path.exists(orig) else patched}
    patch_note = ''
    if os.path.exists(orig) and os.path.exists(patched):
        a, b = open(patched, 'rb').read(), open(orig, 'rb').read()
        diffs = [i for i in range(min(len(a), len(b))) if a[i] != b[i]]
        patch_note = (f'`STARMAP.EXE` differs from `STARMAP.EXE.quiz_bak` in {len(diffs)} byte(s) '
                      f'({", ".join(f"0x{i:X}: 0x{b[i]:02X} -> 0x{a[i]:02X}" for i in diffs[:4])}); the `.quiz_bak` (original) is read.')
    lbx_stems = {r['stem'].upper() for r in file_rows if r['class'] == 'simtex_lbx'}
    exes = {}
    string_rows = []
    lit_rows = []
    for exe, path in exe_paths.items():
        e = be.BorlandExe(path)
        exes[exe] = e
        strs = e.strings(3)
        lits = e.file_name_literals(lbx_stems | {'ORION', 'STARMAP', 'CONFIG', 'SAVE1', 'ERROR'})
        # bare LBX stems (MoM passes most LBX names this way and the loader adds '.LBX'): the longest known-stem suffix of a run
        for o, t in strs:
            for k in range(max(0, len(t) - 8), len(t) - 2):
                if t[k:].upper() in lbx_stems and (k == 0 or not t[k - 1].isalpha() or refs_probe(e, o + k)):
                    lits.append((o + k, t[k:]))
                    break
        lits.sort()
        lit_offsets = {o for o, _ in lits}
        refs = e.references([o for o, _ in strs] + list(lit_offsets))
        for k, (o, t) in enumerate(strs):
            r = refs.get(o, {})
            string_rows.append({'exe': exe, 'order': k, 'ds_off': f'{o:04X}', 'chars': len(t), 'case': mc.case_class(t),
                                'refs': ' '.join(f'{n}:{c}' for n, c in sorted(r.items())), 'text': fwv_text(t)})
        for k, (o, t) in enumerate(lits):
            if not GENERIC_FILE_RE.match(t) and t.upper() not in lbx_stems:
                continue
            r = refs.get(o, {})
            stem, ext = mc.split_name(t)
            primary = max(r, key=r.get) if r else ''
            lit_rows.append({'exe': exe, 'order': k, 'ds_off': f'{o:04X}', 'literal': t, 'form': 'bare' if not ext else 'full', 'stem': stem, 'ext': ext,
                             'case_stem': mc.case_class(stem), 'case_ext': mc.case_class(ext), 'n_refs': sum(r.values()),
                             'regions': ' '.join(f'{n}:{c}' for n, c in sorted(r.items())), 'primary_region': primary,
                             'is_shipped_file': int(t.upper() in {f['file'].upper() for f in file_rows})})

    # ---- write tables ------------------------------------------------------------------------------------------------
    out = args.outdir
    ns.write_fwv(os.path.join(out, 'SimTex-Corpus-MoO1-Files.fwv'),
                 ['file', 'stem', 'ext', 'bytes', 'date', 'class', 'lbx_type', 'lbx_entries', 'has_name_table', 'text_fields'], file_rows)
    ns.write_fwv(os.path.join(out, 'SimTex-Corpus-MoO1-LBX-Entries.fwv'), ['lbx', 'entry', 'entry_name', 'description'],
                 [{'lbx': a, 'entry': b, 'entry_name': c, 'description': e} for a, b, c, e in lbx_entries])
    ns.write_fwv(os.path.join(out, 'SimTex-Corpus-MoO1-LBX-Text.fwv'), ['lbx', 'entry', 'record', 'field', 'chars', 'case', 'text'], text_rows)
    ns.write_fwv(os.path.join(out, 'SimTex-Corpus-MoO1-EXE-Strings.fwv'), ['exe', 'order', 'ds_off', 'chars', 'case', 'refs', 'text'], string_rows)
    ns.write_fwv(os.path.join(out, 'SimTex-Corpus-MoO1-Literals.fwv'),
                 ['exe', 'order', 'ds_off', 'literal', 'form', 'stem', 'ext', 'case_stem', 'case_ext', 'n_refs', 'primary_region', 'is_shipped_file', 'regions'], lit_rows)

    # ---- comparison data (MoM, MoO2) ---------------------------------------------------------------------------------
    def read_csv(name):
        p = os.path.join(STYLE_DIR, name)
        with open(p, encoding='latin-1', newline='') as f:
            return list(csv.DictReader(f))
    mom_files = [r for r in read_csv('SimTex-Corpus-MoM-Files.csv') if r['install'] == 'MoM131']
    mom_lbx_stems = {r['stem'].upper() for r in mom_files if r['class'] == 'simtex_lbx'}
    mom_entries = [(r['lbx'], int(r['entry']), r['entry_name'], r['description']) for r in read_csv('SimTex-Corpus-MoM-LBX-Entries.csv') if r['install'] == 'MoM131']
    mom_lits = read_csv('SimTex-Corpus-MoM-LBX-Literals.csv')
    moo2_lits = read_csv('SimTex-Corpus-MoO2-LBX-Literals.csv')
    moo2_stems = {r['stem'].upper() for r in moo2_lits if r['ext'].upper() == 'LBX'}

    # ---- stats -------------------------------------------------------------------------------------------------------
    L = []
    p = L.append
    p('# SimTex Corpus: MoO1 v1.30')
    p('')
    p('Generated by [tools/simtex_style/simtex_moo1_corpus.py](../../tools/simtex_style/simtex_moo1_corpus.py) from `' + d + '`. '
      'Method and the MoM side: [SimTex-Coding-Style-And-Authorship.md §5](SimTex-Coding-Style-And-Authorship.md#5-the-og-mom-corpus-file-names-lbx-name-tables-file-name-literals).')
    p('')
    p('Tables: [Files](SimTex-Corpus-MoO1-Files.fwv) · [LBX name tables](SimTex-Corpus-MoO1-LBX-Entries.fwv) · [LBX string tables](SimTex-Corpus-MoO1-LBX-Text.fwv) · '
      '[EXE strings](SimTex-Corpus-MoO1-EXE-Strings.fwv) · [file-name literals](SimTex-Corpus-MoO1-Literals.fwv)')
    p('')
    if patch_note:
        p('> ' + patch_note)
        p('')
    for exe, e in exes.items():
        h = hashlib.sha256(e.data).hexdigest()[:16]
        p(f'- **{exe}** (`{os.path.basename(e.path)}`, sha256 `{h}…`): {len(e.resident)} resident code segments, {len(e.overlays)} overlays, '
          f'{sum(1 for r in string_rows if r["exe"] == exe):,} DGROUP strings.')
    p('')

    p('## Files')
    p('')
    cls = collections.Counter(r['class'] for r in file_rows)
    p('| Class | Files |')
    p('|---|---:|')
    for k, v in cls.most_common():
        p(f'| {k} | {v} |')
    p('')
    stems = [r['stem'] for r in file_rows if r['class'] == 'simtex_lbx']
    mom_stems = [r['stem'] for r in mom_files if r['class'] == 'simtex_lbx']
    p('| | MoO1 | MoM |')
    p('|---|---:|---:|')
    p(f'| LBX files | {len(stems)} | {len(mom_stems)} |')
    p(f'| stems of exactly 8 characters | {sum(len(s) == 8 for s in stems)} | {sum(len(s) == 8 for s in mom_stems)} |')
    p(f'| mean stem length | {sum(map(len, stems)) / len(stems):.2f} | {sum(map(len, mom_stems)) / len(mom_stems):.2f} |')
    p('')
    up = {s.upper() for s in stems}
    p(f'- **Stems shared with MoM:** {len(up & mom_lbx_stems)}: ' + ', '.join(f'`{s}`' for s in sorted(up & mom_lbx_stems)) + '.')
    p(f'- **Stems shared with MoO2** (MoO2 LBX names in ORION2.EXE): {len(up & moo2_stems)}: ' + ', '.join(f'`{s}`' for s in sorted(up & moo2_stems)) + '.')
    p(f'- **In all three:** ' + (', '.join(f'`{s}`' for s in sorted(up & mom_lbx_stems & moo2_stems)) or 'none') + '.')
    dates = collections.Counter(r['date'][:7] for r in file_rows if r['class'] in ('simtex_lbx', 'simtex_exe'))
    p('- **File dates** (month: SimTex files): ' + ', '.join(f'{k}: {v}' for k, v in sorted(dates.items())) + '. Files dated after the September 1993 release are the patches: '
      + ', '.join(f'`{r["file"]}` ({r["date"]})' for r in file_rows if r['class'] in ('simtex_lbx', 'simtex_exe') and r['date'] > '1993-09-30') + '.')
    p('')

    p('## LBX name tables')
    p('')
    m1 = name_table_metrics(lbx_entries)
    m2 = name_table_metrics(mom_entries)
    n1 = sum(1 for r in file_rows if r['has_name_table'] == 1)
    p(f'{n1} of {len(stems)} LBX files carry the 0x200 name table (MoM: {sum(1 for r in mom_files if r["has_name_table"] == "1")} of {len(mom_stems)}).')
    p('')
    p('| Metric | MoO1 | MoM |')
    p('|---|---:|---:|')
    rows_m = [('entries', 'entries'), ('named entries', 'named'), ('distinct names', 'distinct'), ('names all upper case', 'upper'),
              ('names of exactly 8 characters', 'eight'), ('name repeats the previous entry', 'repeat_prev'), ('names in more than one file', 'multi_file'),
              ('descriptions', 'descs'), ('descriptions all lower case', 'desc_lower'), ('descriptions at the 22-character limit', 'desc_at_22')]
    for label, k in rows_m:
        p(f'| {label} | {m1[k]:,} | {m2[k]:,} |')
    p('')
    shared_names = sorted({n for _, _, n, _ in lbx_entries if n} & {n for _, _, n, _ in mom_entries if n})
    p(f'- **Entry names also in MoM tables:** {len(shared_names)}: ' + ', '.join(f'`{s}`' for s in shared_names) + '.')
    nonupper = sorted({n for _, _, n, _ in lbx_entries if n and mc.case_class(n) not in ('UPPER', 'none')})
    p(f'- **Entry names not all upper case:** ' + (', '.join(f'`{s}`' for s in nonupper) or 'none') + '.')
    descs = [(l, n, dd) for l, _, n, dd in lbx_entries if dd]
    p(f'- **Descriptions:** ' + ', '.join(sorted({f'`{dd}`' for _, _, dd in descs})[:40]) + ('…' if len({dd for _, _, dd in descs}) > 40 else '') + '.')
    p('')

    p('## LBX string tables')
    p('')
    p('Type-5 (array) and type-3 (help) LBX files hold the game text as fixed-size records of NUL-terminated fields.')
    p('')
    p('| LBX | Type | Text fields | Example |')
    p('|---|---:|---:|---|')
    for r in file_rows:
        if r['text_fields']:
            ex = next((t['text'] for t in text_rows if t['lbx'] == r['file'] and len(t['text']) > 12), next((t['text'] for t in text_rows if t['lbx'] == r['file']), ''))
            p(f'| {r["file"]} | {r["lbx_type"]} | {r["text_fields"]:,} | {ex[:70].replace("|", "/")} |')
    fli = sorted({t['text'] for t in text_rows if re.fullmatch(r'[A-Za-z0-9_]{1,8}\.[A-Za-z]{3}', t['text'])})
    p('')
    p(f'- **File names inside the help text:** {len(fli)}. They are the art files the help screens were made from, so they are SimTex working names: '
      + ', '.join(f'`{s}`' for s in fli) + '.')
    p(f'- **Case of those names:** ' + ', '.join(f'{k} {v}' for k, v in collections.Counter(mc.case_class(mc.split_name(s)[0]) for s in fli).most_common()) + '.')
    p('')

    p('## File-name literals in the EXEs')
    p('')
    p('Reference regions: `sNN` = resident code segment NN, `oNN` = overlay NN, in file order. Under the medium model each `.c` file is one segment, '
      'so a region is a source module.')
    p('')
    full = [r for r in lit_rows if r['form'] == 'full']
    bare = [r for r in lit_rows if r['form'] == 'bare']

    def lit_case(rows, exe):
        rr = [r for r in rows if r['exe'] == exe and r['ext'].upper() == 'LBX']
        return len(rr), collections.Counter(r['case_stem'] for r in rr)
    cols = [lit_case(full, 'ORION'), lit_case(full, 'STARMAP'), lit_case(mom_lits, 'WIZARDS'), lit_case(mom_lits, 'MAGIC')]
    p('Literals written with the extension (`PLANETS.LBX`). In MoO1 every one of them is referenced from code:')
    p('')
    p('| | ORION | STARMAP | MoM WIZARDS | MoM MAGIC |')
    p('|---|---:|---:|---:|---:|')
    p('| `NAME.LBX` literals | ' + ' | '.join(str(n) for n, _ in cols) + ' |')
    for k in ('UPPER', 'lower', 'Title', 'Mixed'):
        p(f'| stem {k} | ' + ' | '.join(str(c[k]) for _, c in cols) + ' |')
    mom_bare = {exe: sum(1 for r in mom_lits if r['exe'] == exe and r['ext'] == '' and r['stem'].upper() in mom_lbx_stems) for exe in ('WIZARDS', 'MAGIC')}
    p(f'| bare LBX stems (`MAPBACK`) | {sum(1 for r in bare if r["exe"] == "ORION")}* | {sum(1 for r in bare if r["exe"] == "STARMAP")}* | {mom_bare["WIZARDS"]} | {mom_bare["MAGIC"]} |')
    p('')
    p('\\* MoO1 bare-stem matches are strings equal to an LBX stem, not confirmed file names: '
      f'{sum(1 for r in bare if not r["n_refs"])} of {len(bare)} are not referenced by any immediate operand, and the referenced ones '
      '(' + ', '.join(sorted({f'`{r["literal"]}`' for r in bare if r['n_refs']})) + ') read as screen labels. They are kept in the '
      '[literals table](SimTex-Corpus-MoO1-Literals.fwv) with `form = bare` and left out of everything below. In MoM, bare stems are the main load form.')
    p('')
    odd = [r for r in full if r['case_stem'] != 'UPPER' or r['case_ext'] != 'UPPER']
    p('- **`NAME.EXT` literals not all upper case:** ' + (', '.join(f'`{r["literal"]}` ({r["exe"]} {r["primary_region"] or "unreferenced"})' for r in odd) or 'none') + '.')
    unref = [r for r in full if not r['n_refs']]
    p(f'- **Unreferenced `NAME.EXT` literals:** {len(unref)}' + (': ' + ', '.join(f'`{r["literal"]}`' for r in unref) if unref else '') + '.')
    notship = sorted({r['literal'] for r in full if not r['is_shipped_file']})
    p('- **Names that are not shipped files:** ' + (', '.join(f'`{s}`' for s in notship) or 'none') + '.')
    p('')
    p('### Does case cluster by module?')
    p('')
    p('Same tests as MoM §5.4: purity of the stem case within each referencing region, against shuffles of the case labels; '
      'the same-stem null keeps each LBX name\'s own case habit. Runs: case changes along DGROUP order (= link order) vs random order.')
    p('')
    p('| EXE | Literals in regions with ≥ 2 | Purity | Global null mean / p | Same-stem null mean / p | Runs (changes) | Random mean | p |')
    p('|---|---:|---:|---:|---:|---:|---:|---:|')
    for exe in exes:
        items = [r for r in full if r['exe'] == exe and r['primary_region']]
        if not items:
            continue
        res = mc.perm_test(items, lambda it: it['primary_region'], lambda it: it['case_stem'], lambda it: it['stem'].upper(), args.iters, rng)
        seq = [r['case_stem'] for r in full if r['exe'] == exe]
        obs, mean, pr = mc.runs_test(seq, args.iters, rng)
        p(f'| {exe} | {res["n"]} | {res["observed"]:.3f} | {res["global"]["mean"]:.3f} / {res["global"]["p"]:.2g} | '
          f'{res["stratum"]["mean"]:.3f} / {res["stratum"]["p"]:.2g} | {obs} | {mean:.1f} | {pr:.2g} |')
    p('')
    minority = sum(1 for r in full if r['case_stem'] != 'UPPER')
    if minority < 5:
        p(f'Only {minority} of {len(full)} MoO1 `NAME.EXT` literals are not upper case, so these tests have no power: '
          'there is too little case variation for a module structure to show.')
        p('')
    p('Case sequence along DGROUP order (U = UPPER, l = lower, T = Title, M = Mixed):')
    p('')
    for exe in exes:
        seq = ''.join({'UPPER': 'U', 'lower': 'l', 'Title': 'T', 'Mixed': 'M'}.get(r['case_stem'], '?') for r in full if r['exe'] == exe)
        p(f'- {exe}: `{seq}`')
    p('')

    p('## Shared library code')
    p('')
    so = [r['text'] for r in string_rows if r['exe'] == 'ORION']
    ss = [r['text'] for r in string_rows if r['exe'] == 'STARMAP']
    common = set(so) & set(ss)
    p(f'{len(common)} distinct strings occur in both EXEs. Both are linked from shared modules (the engine library and the start-up code), as WIZARDS / MAGIC are in MoM.')
    p('')
    order_both = [t for t in ss if t in common]
    p('First shared strings in STARMAP DGROUP order: ' + ', '.join(f'`{t[:40]}`' for t in order_both[:12]) + '.')
    p('')
    with open(os.path.join(out, 'SimTex-Corpus-MoO1-Stats.md'), 'w', encoding='utf-8', newline='\n') as f:
        f.write('\n'.join(L) + '\n')
    sys.stdout.flush()
    sys.stdout.buffer.write(('\n'.join(L) + '\n').encode('utf-8'))


if __name__ == '__main__':
    main()
