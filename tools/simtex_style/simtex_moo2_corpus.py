#!/usr/bin/env python3
"""MoO2 v1.31 corpus: shipped file names, LBX layout, LBX text (including the per-programmer string files), EXE strings.

The MoO2 counterpart of simtex_moo1_corpus.py / simtex_starlords_corpus.py.  MoO2 is the one game with SimTex debug
symbols, so its EXE strings come from the IDA listing (doc/@MoO2/ORION2.asm) and carry the referencing module and that
module's owner (SimTex-Authorship-MoO2-Modules.csv, FINAL).  ORION95.EXE (the Windows build) is read as raw bytes.

LBX: same 0xFEAD header as MoO1 / MoM, but the header block is 0x800 bytes and holds no name table; the bytes after the
offset table are uninitialised.  A name table is accepted only if every name and description is printable.

Inputs:
  --moo2-dir   the MoO2 v1.31 install (default C:/STU_DASM/_MoO2v131)
  doc/@MoO2/ORION2.asm, doc/@MoO2/modules/*.fwv, doc/@SimTex_Style/SimTex-Authorship-MoO2-Modules.csv
  doc/@SimTex_Style/SimTex-Corpus-MoO1-*.fwv, SimTex-Corpus-Starlords-Files.fwv, SimTex-Corpus-MoM-Files.csv (comparisons)

Outputs (--outdir, default doc/@SimTex_Style):
  SimTex-Corpus-MoO2-Files.fwv        every file, classified, with its LBX layout
  SimTex-Corpus-MoO2-LBX-Text.fwv     text fields of the text-bearing LBX entries, with a language guess and the owner
                                      of the per-programmer string files
  SimTex-Corpus-MoO2-EXE-Strings.fwv  ORION2.EXE strings (IDA labels, referencing modules, owner) + ORION95.EXE strings
  SimTex-Corpus-MoO2-Literals.fwv     the file-name literals among them
  SimTex-Corpus-MoO2-Stats.md         the statistics and the comparisons

stdlib only.  Run:  python3 tools/simtex_style/simtex_moo2_corpus.py
"""

import argparse
import collections
import csv
import datetime
import hashlib
import os
import re
import statistics
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import simtex_authorship as sa  # noqa: E402
import simtex_mom_corpus as mc  # noqa: E402
import simtex_name_score as ns  # noqa: E402
import simtex_starlords_corpus as slc  # noqa: E402

STYLE_DIR = os.path.join(sa.REPO, 'doc', '@SimTex_Style')
SIMTEX_EXES = {'ORION2.EXE', 'ORION95.EXE'}
RUNTIME = {'MOX.SET', 'HOF.M2', 'SAVE10.GAM', 'ORIONCD.INI'}
PUBLISHER = {'README.TXT'}
THIRD_PARTY_RE = re.compile(r'(\.MDI|\.DIG|^AILDRVR\.LST|^SAMPLE\.|^SETSOUND\.EXE|^RKERNEL\.COM|^FILE_ID\.DI)$|^SAMPLE\.', re.I)
# Per-programmer string files (section 3.5 of the style doc).
OWNER_FILES = {'JIMTEXT.LBX': 'Jim', 'JIMTEXT2.LBX': 'Jim', 'KENTEXT.LBX': 'Ken', 'KENTEXT1.LBX': 'Ken',
               'BILLTEXT.LBX': 'Bill', 'BILLTEX2.LBX': 'Bill', 'ESTRINGS.LBX': 'Eric', 'HESTRNGS.LBX': 'Harold'}
STOP = {
    'en': set('the of to and you your is are a in for with this will be not have has it on that from'.split()),
    'de': set('der die das und ist nicht sie ihr ein eine zu mit von den dem des werden wird auf für'.split()),
    'fr': set('le la les des de et est vous votre pas une un du pour sur qui que avec au aux'.split()),
    'es': set('el los las del que es su sus para una un con por se no al lo como'.split()),
    'it': set('il della che è per non gli di una un con del al sono le si alla'.split()),
}
FILE_LIT_RE = re.compile(r'^[A-Za-z0-9_\-]{1,8}\.(lbx|exe|gam|set|smk|m2|ini|txt|com|fli|voc|mdi|dig)$', re.I)


def classify(name):
    up = name.upper()
    if up in SIMTEX_EXES:
        return 'simtex_exe'
    if THIRD_PARTY_RE.search(up):
        return 'third_party'
    if up in RUNTIME:
        return 'runtime'
    if up in PUBLISHER:
        return 'publisher'
    if up.endswith('.LBX'):
        return 'simtex_lbx'
    return 'user_added'


def read_lbx2(data):
    """(layout, count, offsets, has_name_table) for a MoO2 file."""
    if data[:4] == b'SMK2':
        return 'smacker', 0, [], False
    if len(data) < 8 or struct.unpack_from('<H', data, 2)[0] != 0xFEAD:
        return 'other', 0, [], False
    count = struct.unpack_from('<H', data, 0)[0]
    offs = list(struct.unpack_from('<%dI' % (count + 1), data, 8))
    table = False
    if offs[0] >= 0x200 + 32 * count and count:
        recs = [data[0x200 + 32 * i: 0x200 + 32 * (i + 1)] for i in range(count)]

        def ok(b):
            s = b.split(b'\x00', 1)[0]
            return all(32 <= c < 127 for c in s)
        table = all(ok(r[:9]) and ok(r[9:]) for r in recs) and any(r[0] for r in recs)
    return 'lbx', count, offs, table


TEXT_CHARS = set(b"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 .,!?'\":;-()/&+%#\t\n\r") | set(range(0x80, 0x90))


def texty(raw):
    """Prose-like: almost only letters, digits, spaces and ordinary punctuation, at least half letters, a vowel, and
    either a space or a short word.  Picture bytes (palette indices in the printable range) fail on [ ] ^ _ ` { } |."""
    if not raw:
        return False
    ok = sum(c in TEXT_CHARS for c in raw) / len(raw)
    letters = sum(chr(c).isalpha() for c in raw if c < 128)
    if not (ok >= 0.97 and letters >= 2 and letters / len(raw) >= 0.5 and re.search(rb'[aeiouyAEIOUY]', raw)):
        return False
    if b' ' in raw.strip():
        return True
    # one word: it must look like one (Altair, Pop:, percent) -- runs of palette indices (SUURSRRR, bdeffigfegec) do not
    w = raw.strip(b' .,:;!?')
    return len(w) <= 24 and (re.fullmatch(rb"[A-Z][a-z']+[0-9]*", w) is not None or (re.fullmatch(rb'[a-z]+', w) is not None and re.search(rb'(.)', w) is None and len(set(w)) >= len(w) * 0.6))


def entry_fields(rec):
    """NUL-separated text fields of one entry.  The entry counts as text only if its prose-like fields cover at least
    60% of its non-zero bytes (record headers and padding fall away)."""
    nz = sum(1 for c in rec if c)
    if not nz:
        return []
    good = 0
    for raw in rec.split(b'\x00'):
        m = re.search(rb'[\x20-\x7e\x09\x0a\x0d\x80-\x8f]+$', raw)
        if m and texty(m.group(0)):
            good += len(m.group(0))
    if good / nz < 0.6:
        return []
    out = []
    pos = 0
    for raw in rec.split(b'\x00'):
        m = re.search(rb'[\x20-\x7e\x09\x0a\x0d\x80-\x8f]+$', raw)
        if m and texty(m.group(0)):
            t = ''.join(chr(c) if 32 <= c < 127 else f'\\x{c:02X}' for c in m.group(0))
            out.append((pos + m.start(), t))
        pos += len(raw) + 1
    return out


def language(t):
    words = re.findall(r"[a-zà-ÿ']+", t.lower().replace('\\x', ' '))
    if len(words) < 3:
        return ''
    score = {k: sum(w in v for w in words) for k, v in STOP.items()}
    best = max(score, key=score.get)
    return best if score[best] >= 1 and list(score.values()).count(score[best]) == 1 else '?'


def asm_strings():
    """ORION2.asm labelled strings: [(order, label, text)] and {label: Counter(module_num)} from `offset label` in procs."""
    with open(sa.ASM_PATH, encoding='latin-1') as f:
        lines = f.readlines()
    defs = []
    labels = {}
    str_re = re.compile(r"^(\w+)\s+db\s+'([^']*)'")
    for i, line in enumerate(lines):
        m = str_re.match(line)
        if m and m.group(2):
            labels[m.group(1)] = len(defs)
            defs.append((i, m.group(1), m.group(2)))
    mods = sa.load_modules()
    sym_mod = {}
    for m in mods:
        for kind, name, sym in m['syms']:
            if kind == 'code':
                sym_mod.setdefault(sym, m['num'])
    refs = collections.defaultdict(collections.Counter)
    cur = None
    proc_re = re.compile(r'^(\S+)\s+proc\b')
    endp_re = re.compile(r'^(\S+)\s+endp\b')
    for line in lines:
        m = proc_re.match(line)
        if m:
            cur = sym_mod.get(m.group(1))
            continue
        if endp_re.match(line):
            cur = None
            continue
        if cur is None:
            continue
        for lab in mc.OFFSET_RE.findall(line.split(';', 1)[0]):
            if lab in labels:
                refs[lab][cur] += 1
    return defs, refs, {m['num']: m for m in mods}


def text_style(fields):
    """Descriptive style figures over a list of English text fields."""
    words = [len(t.split()) for t in fields]
    n = len(fields)
    if not n:
        return None
    return {
        'n': n,
        'mean_words': statistics.mean(words),
        'excl': sum('!' in t for t in fields) / n,
        'quest': sum('?' in t for t in fields) / n,
        'contr': sum(bool(re.search(r"\b\w+(n't|'re|'ll|'ve|'m|'d)\b", t, re.I)) for t in fields) / n,
        'printf': sum(bool(re.search(r'%[-0-9.]*[sdcuxlf]', t)) for t in fields) / n,
        'ctrl': sum('\\x8' in t for t in fields) / n,
        'caps_word': sum(bool(re.search(r'\b[A-Z]{3,}\b', t)) for t in fields) / n,
        'ellipsis': sum('...' in t for t in fields) / n,
    }


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--moo2-dir', default='C:/STU_DASM/_MoO2v131')
    ap.add_argument('--outdir', default=STYLE_DIR)
    args = ap.parse_args()
    d = args.moo2_dir
    out = args.outdir

    # ---- files and LBX -----------------------------------------------------------------------------------------------
    file_rows, text_rows = [], []
    first_by_hash = {}
    for fn in sorted(os.listdir(d), key=str.lower):
        p = os.path.join(d, fn)
        if not os.path.isfile(p):
            continue
        st = os.stat(p)
        stem, ext = mc.split_name(fn)
        row = {'file': fn, 'stem': stem, 'ext': ext, 'bytes': st.st_size, 'date': datetime.date.fromtimestamp(st.st_mtime).isoformat(),
               'name_case': mc.case_class(fn), 'class': classify(fn), 'layout': '', 'entries': '', 'has_name_table': '', 'text_fields': '', 'owner': OWNER_FILES.get(fn.upper(), ''),
               'duplicate_of': ''}
        if fn.upper().endswith('.LBX'):
            data = open(p, 'rb').read()
            h = hashlib.sha256(data).hexdigest()
            if h in first_by_hash:
                row['duplicate_of'] = first_by_hash[h]
            else:
                first_by_hash[h] = fn
            layout, count, offs, table = read_lbx2(data)
            row.update(layout=layout, entries=count if layout == 'lbx' else '', has_name_table=int(table) if layout == 'lbx' else '')
            if layout == 'smacker':
                row['class'] = 'simtex_video'
            nf = 0
            if layout == 'lbx' and not row['duplicate_of']:
                for e in range(count):
                    for o, t in entry_fields(data[offs[e]:offs[e + 1]]):
                        text_rows.append({'lbx': fn, 'entry': e, 'offset': f'{o:05X}', 'chars': len(t), 'lang': language(t), 'owner': row['owner'], 'text': t})
                        nf += 1
            row['text_fields'] = nf or ''
        file_rows.append(row)

    # ---- EXE strings -------------------------------------------------------------------------------------------------
    owners = {}
    with open(os.path.join(STYLE_DIR, 'SimTex-Authorship-MoO2-Modules.csv'), encoding='latin-1') as f:
        for r in csv.DictReader(f):
            owners[int(r['module_num'])] = r['FINAL']
    defs, refs, num_to = asm_strings()
    string_rows, lit_rows = [], []
    for k, (line, lab, t) in enumerate(defs):
        r = refs.get(lab, {})
        mods = ' '.join(f'{num_to[n]["short"]}:{c}' for n, c in r.most_common()) if r else ''
        prim = r.most_common(1)[0][0] if r else None
        own = owners.get(prim, '') if prim is not None else ''
        string_rows.append({'exe': 'ORION2', 'order': k, 'label': lab, 'chars': len(t), 'case': mc.case_class(t), 'primary_module': num_to[prim]['short'] if prim is not None else '',
                            'owner': own, 'modules': mods, 'text': t})
        if FILE_LIT_RE.match(t):
            stem, ext = mc.split_name(t)
            lit_rows.append({'exe': 'ORION2', 'order': k, 'literal': t, 'stem': stem, 'ext': ext, 'case_stem': mc.case_class(stem), 'case_ext': mc.case_class(ext),
                             'primary_module': num_to[prim]['short'] if prim is not None else '', 'owner': own})
    w95 = open(os.path.join(d, 'ORION95.EXE'), 'rb').read()
    for k, m in enumerate(re.finditer(rb'[\x20-\x7e]{4,}(?=\x00)', w95)):
        t = m.group(0).decode('latin-1')
        if sum(c.isalpha() for c in t) < 3:
            continue
        string_rows.append({'exe': 'ORION95', 'order': k, 'label': f'{m.start():07X}', 'chars': len(t), 'case': mc.case_class(t), 'primary_module': '', 'owner': '', 'modules': '', 'text': t})
        tm = re.search(r'([A-Za-z0-9_\-]{1,8}\.(lbx|exe|gam|set|smk|m2|ini))$', t, re.I)
        if tm:
            stem, ext = mc.split_name(tm.group(1))
            lit_rows.append({'exe': 'ORION95', 'order': k, 'literal': tm.group(1), 'stem': stem, 'ext': ext, 'case_stem': mc.case_class(stem), 'case_ext': mc.case_class(ext), 'primary_module': '', 'owner': ''})

    ns.write_fwv(os.path.join(out, 'SimTex-Corpus-MoO2-Files.fwv'), ['file', 'stem', 'ext', 'bytes', 'date', 'name_case', 'class', 'layout', 'entries', 'has_name_table', 'text_fields', 'owner', 'duplicate_of'], file_rows)
    ns.write_fwv(os.path.join(out, 'SimTex-Corpus-MoO2-LBX-Text.fwv'), ['lbx', 'entry', 'offset', 'chars', 'lang', 'owner', 'text'], text_rows)
    ns.write_fwv(os.path.join(out, 'SimTex-Corpus-MoO2-EXE-Strings.fwv'), ['exe', 'order', 'label', 'chars', 'case', 'primary_module', 'owner', 'modules', 'text'], string_rows)
    ns.write_fwv(os.path.join(out, 'SimTex-Corpus-MoO2-Literals.fwv'), ['exe', 'order', 'literal', 'stem', 'ext', 'case_stem', 'case_ext', 'primary_module', 'owner'], lit_rows)

    # ---- comparison data ---------------------------------------------------------------------------------------------
    m1_files = ns.read_fwv(os.path.join(STYLE_DIR, 'SimTex-Corpus-MoO1-Files.fwv'))
    m1_text = ns.read_fwv(os.path.join(STYLE_DIR, 'SimTex-Corpus-MoO1-LBX-Text.fwv'))
    sl_files = ns.read_fwv(os.path.join(STYLE_DIR, 'SimTex-Corpus-Starlords-Files.fwv'))
    with open(os.path.join(STYLE_DIR, 'SimTex-Corpus-MoM-Files.csv'), encoding='latin-1', newline='') as f:
        mom_files = [r for r in csv.DictReader(f) if r['install'] == 'MoM131']
    stem_sets = {
        'Starlords': {r['stem'].upper() for r in sl_files if r['class'] == 'simtex_lbx'},
        'MoO1': {r['stem'].upper() for r in m1_files if r['class'] == 'simtex_lbx'},
        'MoM': {r['stem'].upper() for r in mom_files if r['class'] == 'simtex_lbx'},
    }

    # ---- stats -------------------------------------------------------------------------------------------------------
    L = []
    p = L.append
    p('# SimTex Corpus: MoO2 v1.31')
    p('')
    p('Generated by [tools/simtex_style/simtex_moo2_corpus.py](../../tools/simtex_style/simtex_moo2_corpus.py) from `' + d + '`. '
      'Compared against [MoO1 v1.30](SimTex-Corpus-MoO1-Stats.md), [Starlords](SimTex-Corpus-Starlords-Stats.md) and MoM (§5 of the style doc).')
    p('')
    p('Tables: [Files](SimTex-Corpus-MoO2-Files.fwv) · [LBX text](SimTex-Corpus-MoO2-LBX-Text.fwv) · [EXE strings](SimTex-Corpus-MoO2-EXE-Strings.fwv) · '
      '[file-name literals](SimTex-Corpus-MoO2-Literals.fwv)')
    p('')
    p('## Files')
    p('')
    p('| Class | Files |')
    p('|---|---:|')
    for k, v in collections.Counter(r['class'] for r in file_rows).most_common():
        p(f'| {k} | {v} |')
    p('')
    lbx = [r for r in file_rows if r['class'] == 'simtex_lbx']
    byc = collections.defaultdict(list)
    for r in file_rows:
        if r['class'] in ('simtex_lbx', 'simtex_exe', 'simtex_video'):
            byc[r['name_case']].append(r)
    p('| Name case on disk | SimTex files | Dates |')
    p('|---|---:|---|')
    for k, rows in sorted(byc.items(), key=lambda kv: -len(kv[1])):
        ds = sorted(r['date'] for r in rows)
        p(f'| {k} | {len(rows)} | {ds[0]} … {ds[-1]} |')
    p('')
    p('The upper-case names are the v1.31 patch files (both EXEs and the text LBX files); the lower-case names are the base install. '
      'Name case on disk reflects how each set was copied (CD vs patch archive), so it is not read as a SimTex habit.')
    p('')
    stems = [r['stem'].upper() for r in lbx]
    p('| | MoO2 | MoM | MoO1 | Starlords |')
    p('|---|---:|---:|---:|---:|')
    p(f'| LBX files | {len(stems)} | {len(stem_sets["MoM"])} | {len(stem_sets["MoO1"])} | {len(stem_sets["Starlords"])} |')
    for label, fn in (('stems of exactly 8 characters', lambda ss: sum(len(s) == 8 for s in ss)),
                      ('stems with `_`', lambda ss: sum('_' in s for s in ss)),
                      ('stems with a digit', lambda ss: sum(any(c.isdigit() for c in s) for s in ss))):
        p(f'| {label} | {fn(stems)} | {fn(stem_sets["MoM"])} | {fn(stem_sets["MoO1"])} | {fn(stem_sets["Starlords"])} |')
    p('')
    fam = collections.Counter(re.sub(r'\d+', '#', s) for s in stems)
    p('- **Numbered families:** ' + ', '.join(f'`{k}` ×{v}' for k, v in fam.most_common() if v >= 3) + '.')
    up = set(stems)
    for g in ('Starlords', 'MoO1', 'MoM'):
        p(f'- **Stems shared with {g}:** {len(up & stem_sets[g])}: ' + ', '.join(f'`{s}`' for s in sorted(up & stem_sets[g])) + '.')
    p('- **In all four:** ' + (', '.join(f'`{s}`' for s in sorted(up & stem_sets['Starlords'] & stem_sets['MoO1'] & stem_sets['MoM'])) or 'none') + '.')
    dups = collections.defaultdict(list)
    for r in file_rows:
        if r['duplicate_of']:
            dups[r['duplicate_of']].append(r['file'])
    p('- **Byte-identical files:** ' + '; '.join(f'`{k}` = ' + ', '.join(f'`{x}`' for x in v) for k, v in dups.items()) + '. '
      'The four `diplomsX` and four `eventmsX` files are one English file each, so the last letter is a per-language slot that this English install fills with the same text. '
      'Their text is extracted once.')
    vid = [r['file'] for r in file_rows if r['class'] == 'simtex_video']
    p(f'- **`.lbx` files that are Smacker videos** (`SMK2`): {len(vid)}: ' + ', '.join(f'`{v}`' for v in vid) + '.')
    p('')

    p('## LBX layout')
    p('')
    real = [r for r in file_rows if r['layout'] == 'lbx']
    p(f'- **{len(real)} LBX archives**, all type 0, {sum(int(r["entries"]) for r in real):,} entries.')
    hdr = collections.Counter()
    for r in real:
        data = open(os.path.join(d, r['file']), 'rb').read(16)
        hdr[struct.unpack_from('<I', data, 8)[0]] += 1
    p('- **Header block size** (first entry offset): ' + ', '.join(f'0x{k:X} ×{v}' for k, v in hdr.most_common(4)) + '.')
    p(f'- **Name tables:** {sum(1 for r in real if r["has_name_table"] == 1)} of {len(real)} archives have a printable 0x200 name table. '
      'The bytes after the offset table are uninitialised memory (e.g. `fonts.lbx` holds `ain palette` where a description field would be). '
      'So MoO2 dropped the per-entry names that MoO1 and MoM carry.')
    p('')

    p('## LBX text')
    p('')
    p('Entries count as text when prose-like NUL-terminated fields cover at least 60% of their non-zero bytes; picture entries fail that test.')
    p('')
    p('| LBX | Owner | Fields | English | Example (English) |')
    p('|---|---|---:|---:|---|')
    per = collections.defaultdict(list)
    for t in text_rows:
        per[t['lbx']].append(t)
    for fn, rows in sorted(per.items(), key=lambda kv: (kv[0].upper() not in OWNER_FILES, -len(kv[1]))):
        if len(rows) < 20 and fn.upper() not in OWNER_FILES:
            continue
        en = [t for t in rows if t['lang'] == 'en']
        ex = next((t['text'] for t in en if len(t['text']) > 25), en[0]['text'] if en else '')
        p(f'| {fn} | {OWNER_FILES.get(fn.upper(), "")} | {len(rows):,} | {len(en):,} | {ex[:70].replace("|", "/")} |')
    small = [fn for fn, rows in per.items() if len(rows) < 20 and fn.upper() not in OWNER_FILES]
    p(f'| {len(small)} more files with fewer than 20 fields | | {sum(len(per[f]) for f in small)} | | |')
    p('')
    langs = collections.Counter(t['lang'] or 'short' for t in text_rows)
    p('- **Language guess** (stop words; `?` = tie, `short` = under 3 words): ' + ', '.join(f'{k} {v:,}' for k, v in langs.most_common()) + '.')
    p('')
    p('### The per-programmer string files')
    p('')
    p('English fields only. Descriptive figures, not a model: each file holds the text its owner\'s code prints, so subject matter differs.')
    p('')
    p('| Owner | Files | English fields | Mean words | `!` | `?` | contractions | printf `%s`/`%d` | control-byte codes | ALL-CAPS word | `...` |')
    p('|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|')
    for own in ('Jim', 'Ken', 'Bill', 'Eric', 'Harold'):
        files = sorted({t['lbx'] for t in text_rows if t['owner'] == own})
        st = text_style([t['text'] for t in text_rows if t['owner'] == own and t['lang'] == 'en'])
        if not st:
            p(f'| {own} | {", ".join(files)} | 0 | | | | | | | | |')
            continue
        p(f'| {own} | {", ".join(files)} | {st["n"]} | {st["mean_words"]:.1f} | {100*st["excl"]:.0f}% | {100*st["quest"]:.0f}% | {100*st["contr"]:.0f}% | '
          f'{100*st["printf"]:.0f}% | {100*st["ctrl"]:.0f}% | {100*st["caps_word"]:.0f}% | {100*st["ellipsis"]:.0f}% |')
    p('')
    p('Insertion style over **all** fields of each file (every language, short fields included):')
    p('')
    p('| File | Owner | Fields | printf `%s`/`%d` | control-byte codes |')
    p('|---|---|---:|---:|---:|')
    for fn in sorted({t['lbx'] for t in text_rows if t['owner']}, key=lambda f: (OWNER_FILES[f.upper()], f)):
        rows_f = [t['text'] for t in text_rows if t['lbx'] == fn]
        p(f'| {fn} | {OWNER_FILES[fn.upper()]} | {len(rows_f)} | {sum(bool(re.search(r"%[-0-9.]*[sdcuxlf]", t)) for t in rows_f)} | '
          f'{sum(chr(92) + "x8" in t for t in rows_f)} |')
    p('')
    p('English-only style table, shared text files for comparison:')
    p('')
    st = text_style([t['text'] for t in text_rows if not t['owner'] and t['lang'] == 'en'])
    p(f'| (shared text files) | | {st["n"]} | {st["mean_words"]:.1f} | {100*st["excl"]:.0f}% | {100*st["quest"]:.0f}% | {100*st["contr"]:.0f}% | '
      f'{100*st["printf"]:.0f}% | {100*st["ctrl"]:.0f}% | {100*st["caps_word"]:.0f}% | {100*st["ellipsis"]:.0f}% |')
    p('')

    p('### Text carried over from MoO1')
    p('')
    m1n = {slc.norm(t['text']): t for t in m1_text if len(slc.norm(t['text'])) >= 12}
    s2 = [(t, slc.norm(t['text'])) for t in text_rows if t['lang'] == 'en' and len(slc.norm(t['text'])) >= 12]
    same = [(t, m1n[n]) for t, n in s2 if n in m1n]
    p(f'{len(same)} English MoO2 fields of 12+ characters equal a MoO1 v1.30 field (ignoring case, spacing and insertion codes), out of {len(s2):,}. '
      'By file: ' + ', '.join(f'{a} ← {b}: {v}' for (a, b), v in collections.Counter((t['lbx'], m['lbx']) for t, m in same).most_common(8)) + '.')
    p('')
    p('Examples: ' + ', '.join(f'`{t["text"][:40]}`' for t, m in same[:10]) + '.')
    p('')

    p('## EXE strings')
    p('')
    o2 = [r for r in string_rows if r['exe'] == 'ORION2']
    o95 = [r for r in string_rows if r['exe'] == 'ORION95']
    p(f'- **ORION2.EXE:** {len(o2):,} labelled strings in the IDA listing; {sum(1 for r in o2 if r["primary_module"])} are referenced from a symbol-named procedure, '
      'so each carries its MoO2 module and that module\'s owner.')
    p(f'- **ORION95.EXE** (Windows build): {len(o95):,} NUL-terminated strings read from the bytes; '
      f'{len({r["text"] for r in o95} & {r["text"] for r in o2})} of ORION2\'s distinct strings also occur in it.')
    p('')
    lits2 = [r for r in lit_rows if r['exe'] == 'ORION2' and r['ext'].upper() == 'LBX']
    lits95 = [r for r in lit_rows if r['exe'] == 'ORION95' and r['ext'].upper() == 'LBX']
    p('| `NAME.LBX` literals | ORION2 | ORION95 |')
    p('|---|---:|---:|')
    for k in ('UPPER', 'lower', 'Title', 'Mixed'):
        p(f'| stem {k} | {sum(r["case_stem"] == k for r in lits2)} | {sum(r["case_stem"] == k for r in lits95)} |')
    p('')
    byown = collections.defaultdict(collections.Counter)
    for r in lits2:
        byown[r['owner'] or '(unattributed)'][r['case_stem']] += 1
    p('ORION2 `.LBX` literal case by the owner of the referencing module (the §5.4 analysis tests whether this is a personal mark; it is not):')
    p('')
    p('| Owner | UPPER | lower | Title | Mixed |')
    p('|---|---:|---:|---:|---:|')
    for own, c in sorted(byown.items(), key=lambda kv: -sum(kv[1].values())):
        p(f'| {own} | {c["UPPER"]} | {c["lower"]} | {c["Title"]} | {c["Mixed"]} |')
    p('')
    with open(os.path.join(out, 'SimTex-Corpus-MoO2-Stats.md'), 'w', encoding='utf-8', newline='\n') as f:
        f.write('\n'.join(L) + '\n')
    sys.stdout.flush()
    sys.stdout.buffer.write(('\n'.join(L) + '\n').encode('utf-8'))


if __name__ == '__main__':
    main()
