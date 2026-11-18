#!/usr/bin/env python3
"""
simtex_mom_corpus.py -- OG Master of Magic naming corpus: shipped file names, LBX entry-name tables, and the LBX file-name
string literals in the OG executables, with case-style statistics calibrated against MoO2.

Companion to tools/simtex_style/simtex_authorship.py (MoO2 debug-symbol authorship model); see
doc/@SimTex_Style/SimTex-Coding-Style-And-Authorship.md section 10.

Inputs (all read-only):
  --mom-dir   OG MoM v1.31 install            (default C:/STU/DBWD/MoM131)
  --demo-dir  OG MoM demo install             (default C:/STU/DBWD/MOMDEM; skipped if absent)
  --asm-dir   IDA 5.5 exports WIZARDS.asm / MAGIC.asm (default C:/STU/devel/STU-Extras/Piethawn/Piethawn/in)
  doc/@MoO2/ORION2.asm + modules/*.fwv + doc/@SimTex_Style/SimTex-Authorship-*.csv (via tools/simtex_style/simtex_authorship.py)

Outputs (--outdir, default doc/@SimTex_Style):
  SimTex-Corpus-MoM-Files.csv          every file in the installs, classified (simtex / runtime / third_party / publisher / user_added)
  SimTex-Corpus-MoM-LBX-Entries.csv    LBX header name table: 9-byte entry name + 23-byte description per entry
  SimTex-Corpus-MoM-LBX-Literals.csv   each file-name string in WIZARDS/MAGIC dseg, its case style, the code segments that use it,
                                       and the ReMoM file(s) those segments map to (via the `// WZD o54p01` comments)
  SimTex-Corpus-MoO2-LBX-Literals.csv  same for ORION2.EXE, with the MoO2 module and its FINAL owner
  SimTex-Corpus-Stats.txt              the statistics printed to stdout

Usage:  python3 tools/simtex_style/simtex_mom_corpus.py [--seed 1] [--iters 20000]
"""
import argparse
import collections
import csv
import glob
import os
import random
import re
import statistics
import struct
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))  # tools/simtex_style/ -> repo root
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import simtex_authorship as sa  # noqa: E402

MOO2_DIR = os.path.join(REPO, 'doc', '@MoO2')            # inputs: modules/*.fwv, ORION2.asm
STYLE_DIR = os.path.join(REPO, 'doc', '@SimTex_Style')   # outputs, and the SimTex-Authorship-* tables read back in

# --- file classification ----------------------------------------------------------------------------------------------
# Names the game code itself creates/reads at run time (they appear as string literals in the EXEs).
RUNTIME_FILES = {'CONFIG.MOM', 'MAGIC.SET', 'COMBAT.TMP'}
RUNTIME_RE = re.compile(r'^SAVE(\d|TEST)?\.GAM$')
SIMTEX_EXES = {'WIZARDS.EXE', 'MAGIC.EXE', 'ITEMMAKE.EXE'}
# Not SimTex-named: Miles AIL timbre/bank files, PKWARE unzip, MicroProse installer / copy utility / readme.
THIRD_PARTY = {'FAT.AD', 'FAT.CAT', 'FAT.MT', 'FAT.OPL', 'FAT-GEN.CAT', 'FAT-OPL3.BNK', 'FATV10.BNK', 'PKUNZJR.COM'}
PUBLISHER = {'INSTALL.EXE', 'INSTALL.DAT', 'INSTALL.GIF', 'MPSCOPY.EXE', 'README.TXT'}
DEMO_SHIPPED_SAVES = {'KALI.GAM', 'MERLIN.GAM'}   # the demo ships these two saves
DOS83_RE = re.compile(r'^[A-Z0-9_\-]{1,8}(\.[A-Z0-9]{1,3})?$')

PERSONAL_MOO2_LBX = re.compile(r'^(jimtext|kentext|billtext|estrings|.?estrngs|h.strngs)\.lbx$', re.I)


def classify_file(name, demo=False):
    up = name.upper()
    if name != up or not DOS83_RE.match(name):
        return 'user_added'
    if up.endswith('.LBX'):
        return 'simtex_lbx'
    if up in SIMTEX_EXES:
        return 'simtex_exe'
    if up in THIRD_PARTY:
        return 'third_party'
    if up in PUBLISHER:
        return 'publisher'
    if up in RUNTIME_FILES:
        return 'runtime'
    if demo and up in DEMO_SHIPPED_SAVES:
        return 'runtime'
    if up == 'SAVE.GAM' or (RUNTIME_RE.match(up) and up[4:5].isdigit() and len(up) == 9):
        return 'user_save'   # SAVEn.GAM: the name pattern is SimTex's, the file is the player's
    return 'user_added'


# --- LBX header ---------------------------------------------------------------------------------------------------------
def read_lbx(path):
    """Return (count, lbx_type, entries[(name, desc)] or None if no name table)."""
    with open(path, 'rb') as f:
        data = f.read()
    count, magic, _reserved, lbx_type = struct.unpack_from('<HHHH', data, 0)
    if magic != 0xFEAD:
        return None
    offsets = struct.unpack_from('<%dI' % (count + 1), data, 8)
    entries = None
    if offsets[0] >= 0x200 + 32 * count:
        entries = []
        for i in range(count):
            rec = data[0x200 + 32 * i: 0x200 + 32 * (i + 1)]
            name = rec[:9].split(b'\0', 1)[0].decode('latin-1')
            desc = rec[9:32].split(b'\0', 1)[0].decode('latin-1')
            entries.append((name, desc))
        if not any(n or d for n, d in entries):
            entries = None
    return count, lbx_type, entries


# --- case style ---------------------------------------------------------------------------------------------------------
def case_class(s):
    letters = [c for c in s if c.isalpha()]
    if not letters:
        return 'none'
    w = ''.join(letters)
    if w.isupper():
        return 'UPPER'
    if w.islower():
        return 'lower'
    if w[0].isupper() and w[1:].islower():
        return 'Title'
    return 'Mixed'


def split_name(lit):
    if '.' in lit:
        stem, ext = lit.rsplit('.', 1)
        return stem, ext
    return lit, ''


# --- asm parsing --------------------------------------------------------------------------------------------------------
STR_DEF_RE = re.compile(r"^(\w+)\s+db\s+'([^']*)'")
SEG_RE = re.compile(r'^segment\s+(\S+)')
OFFSET_RE = re.compile(r'\boffset\s+([A-Za-z_@$?][\w@$?]*)')


def mom_file_literals(asm_path, known_stems):
    """Return list of dicts for every dseg string that is a file name: label, literal, order, segments referencing it."""
    with open(asm_path, encoding='latin-1') as f:
        lines = f.readlines()
    seg = None
    defs = {}
    order = 0
    fname_re = re.compile(r'^(?:' + '|'.join(re.escape(s) for s in sorted(known_stems, key=len, reverse=True)) + r')(?:\.[A-Za-z]{3})?$', re.I)
    generic_re = re.compile(r'^[A-Za-z0-9_]{1,8}\.(?:lbx|gam|mom|set|tmp|exe)$', re.I)
    for line in lines:
        m = SEG_RE.match(line)
        if m:
            seg = m.group(1)
            continue
        if seg != 'dseg':
            continue
        m = STR_DEF_RE.match(line)
        if m:
            lit = m.group(2)
            if fname_re.match(lit) or generic_re.match(lit):
                defs[m.group(1)] = {'label': m.group(1), 'literal': lit, 'order': order}
                order += 1
    refs = collections.defaultdict(collections.Counter)
    seg = None
    for line in lines:
        m = SEG_RE.match(line)
        if m:
            seg = m.group(1)
            continue
        code = line.split(';', 1)[0]
        for lab in OFFSET_RE.findall(code):
            if lab in defs:
                refs[lab][seg] += 1
    out = []
    for lab, d in sorted(defs.items(), key=lambda kv: kv[1]['order']):
        d = dict(d)
        d['segments'] = dict(refs.get(lab, {}))
        out.append(d)
    return out


def remom_segment_map():
    """(exe, 'o'|'s', num) -> Counter(ReMoM file) from the `// WZD o54p01` / `// MGC s08p02` header comments."""
    rx = re.compile(r'^//\s*(WZD|MGC)\s+([os])(\d+)p\d+', re.M)
    seg_map = collections.defaultdict(collections.Counter)
    for root in ('MoM/src', 'MoX/src'):
        for path in glob.glob(os.path.join(REPO, root, '**', '*.c'), recursive=True):
            rel = os.path.relpath(path, REPO).replace('\\', '/')
            with open(path, encoding='latin-1') as f:
                for exe, kind, num in rx.findall(f.read()):
                    seg_map[('WIZARDS' if exe == 'WZD' else 'MAGIC', kind, int(num))][rel] += 1
    return seg_map


def asm_seg_key(exe, seg):
    m = re.match(r'^(ovr|seg)(\d+)$', seg)
    if not m:
        return None
    return (exe, 'o' if m.group(1) == 'ovr' else 's', int(m.group(2)))


def moo2_file_literals():
    mods = sa.load_modules()
    sym_mod = {}
    for m in mods:
        for kind, name, sym in m['syms']:
            if kind == 'code':
                sym_mod.setdefault(sym, m['num'])
    num_to = {m['num']: m for m in mods}
    final = {}
    with open(os.path.join(STYLE_DIR, 'SimTex-Authorship-MoO2-Modules.csv'), encoding='latin-1') as f:
        for r in csv.DictReader(f):
            final[int(r['module_num'])] = (r['FINAL'], float(r['P_FINAL']))
    with open(sa.ASM_PATH, encoding='latin-1') as f:
        lines = f.readlines()
    lbx_re = re.compile(r"^(\w+)\s+db\s+'([A-Za-z0-9_\-]{1,8}\.lbx)'", re.I)
    defs = {}
    for i, line in enumerate(lines):
        m = lbx_re.match(line)
        if m:
            defs[m.group(1)] = {'label': m.group(1), 'literal': m.group(2), 'order': i}
    proc_re = re.compile(r'^(\S+)\s+proc\b')
    endp_re = re.compile(r'^(\S+)\s+endp\b')
    refs = collections.defaultdict(collections.Counter)
    cur = None
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
        for lab in OFFSET_RE.findall(line.split(';', 1)[0]):
            if lab in defs:
                refs[lab][cur] += 1
    out = []
    for lab, d in sorted(defs.items(), key=lambda kv: kv[1]['order']):
        d = dict(d)
        d['modules'] = dict(refs.get(lab, {}))
        out.append(d)
    return out, num_to, final


# --- statistics -----------------------------------------------------------------------------------------------------------
def purity(groups):
    """Weighted majority-class share over groups with >= 2 members: sum(max class count) / sum(n)."""
    num = den = 0
    for labels in groups.values():
        if len(labels) >= 2:
            num += collections.Counter(labels).most_common(1)[0][1]
            den += len(labels)
    return (num / den) if den else float('nan'), den


def perm_test(items, group_of, class_of, stratum_of, iters, rng):
    """items: list; observed purity of class_of grouped by group_of.  Two nulls:
       global  -- shuffle classes across all items;
       stratum -- shuffle classes only within the same stratum (e.g. the same LBX stem), so a stem's own case habit is kept."""
    def groups_for(classes):
        g = collections.defaultdict(list)
        for it, c in zip(items, classes):
            g[group_of(it)].append(c)
        return g
    classes = [class_of(it) for it in items]
    obs, n = purity(groups_for(classes))
    res = {'observed': obs, 'n': n}
    for name in ('global', 'stratum'):
        ge = 0
        vals = []
        for _ in range(iters):
            if name == 'global':
                c2 = classes[:]
                rng.shuffle(c2)
            else:
                by = collections.defaultdict(list)
                for idx, it in enumerate(items):
                    by[stratum_of(it)].append(idx)
                c2 = classes[:]
                for idxs in by.values():
                    vals_s = [classes[i] for i in idxs]
                    rng.shuffle(vals_s)
                    for i, v in zip(idxs, vals_s):
                        c2[i] = v
            p, _ = purity(groups_for(c2))
            vals.append(p)
            if p >= obs - 1e-12:
                ge += 1
        res[name] = {'mean': statistics.mean(vals), 'p': (ge + 1) / (iters + 1)}
    return res


def runs_test(seq, iters, rng):
    """Number of class changes along a sequence vs random order of the same multiset (fewer changes = blocky)."""
    def changes(s):
        return sum(1 for a, b in zip(s, s[1:]) if a != b)
    obs = changes(seq)
    le = 0
    vals = []
    for _ in range(iters):
        s = seq[:]
        rng.shuffle(s)
        c = changes(s)
        vals.append(c)
        if c <= obs:
            le += 1
    return obs, statistics.mean(vals), (le + 1) / (iters + 1)


def lower_heterogeneity(items, group_of, stratum_of, iters, rng):
    """Chi-square of group x {lower, not lower}; null shuffles the lower flag only within the same stem.
    Returns (observed chi2, null mean, p, per-group (lower, n))."""
    flags = [1 if it['case_stem'] == 'lower' else 0 for it in items]
    groups = [group_of(it) for it in items]

    def chi2(fl):
        tot = len(fl)
        low = sum(fl)
        if low in (0, tot):
            return 0.0
        by = collections.defaultdict(lambda: [0, 0])
        for g, f in zip(groups, fl):
            by[g][0] += f
            by[g][1] += 1
        x = 0.0
        for lo, n in by.values():
            for obs, exp in ((lo, n * low / tot), (n - lo, n * (tot - low) / tot)):
                x += (obs - exp) ** 2 / exp
        return x
    obs = chi2(flags)
    strata = collections.defaultdict(list)
    for i, it in enumerate(items):
        strata[stratum_of(it)].append(i)
    ge = 0
    vals = []
    for _ in range(iters):
        f2 = flags[:]
        for idxs in strata.values():
            v = [flags[i] for i in idxs]
            rng.shuffle(v)
            for i, x in zip(idxs, v):
                f2[i] = x
        c = chi2(f2)
        vals.append(c)
        if c >= obs - 1e-12:
            ge += 1
    per = collections.defaultdict(lambda: [0, 0])
    for g, f in zip(groups, flags):
        per[g][0] += f
        per[g][1] += 1
    return obs, statistics.mean(vals), (ge + 1) / (iters + 1), {g: tuple(v) for g, v in per.items()}


def length_hist(names):
    return dict(sorted(collections.Counter(len(n) for n in names).items()))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--mom-dir', default='C:/STU/DBWD/MoM131')
    ap.add_argument('--demo-dir', default='C:/STU/DBWD/MOMDEM')
    ap.add_argument('--asm-dir', default='C:/STU/devel/STU-Extras/Piethawn/Piethawn/in')
    ap.add_argument('--outdir', default=STYLE_DIR)
    ap.add_argument('--iters', type=int, default=20000)
    ap.add_argument('--seed', type=int, default=1)
    args = ap.parse_args()
    rng = random.Random(args.seed)
    report = []

    def say(*a):
        s = ' '.join(str(x) for x in a)
        print(s)
        report.append(s)

    # ---------------------------------------------------------------- 1. files
    file_rows = []
    installs = [('MoM131', args.mom_dir, False)]
    if os.path.isdir(args.demo_dir):
        installs.append(('MOMDEM', args.demo_dir, True))
    lbx_rows = []
    for tag, d, demo in installs:
        for name in sorted(os.listdir(d)):
            p = os.path.join(d, name)
            if not os.path.isfile(p):
                continue
            cls = classify_file(name, demo)
            stem, ext = split_name(name)
            n_entries = lbx_type = has_table = ''
            if cls == 'simtex_lbx':
                info = read_lbx(p)
                if info:
                    n_entries, lbx_type, entries = info
                    has_table = 1 if entries else 0
                    for i, (en, de) in enumerate(entries or []):
                        lbx_rows.append({'install': tag, 'lbx': name, 'entry': i, 'lbx_type': lbx_type, 'entry_name': en, 'description': de})
            file_rows.append({'install': tag, 'file': name, 'stem': stem, 'ext': ext, 'bytes': os.path.getsize(p), 'class': cls,
                              'lbx_entries': n_entries, 'lbx_type': lbx_type, 'has_name_table': has_table})

    # ---------------------------------------------------------------- 2. EXE literals
    known_stems = {r['stem'].upper() for r in file_rows if r['class'] in ('simtex_lbx', 'simtex_exe', 'runtime')} | {'SAVE'}
    seg_map = remom_segment_map()
    files_csv = {}
    with open(os.path.join(STYLE_DIR, 'SimTex-Authorship-ReMoM-Files.csv'), encoding='latin-1') as f:
        for r in csv.DictReader(f):
            files_csv[r['remom_file']] = r
    lit_rows = []
    for exe in ('WIZARDS', 'MAGIC'):
        asm = os.path.join(args.asm_dir, exe + '.asm')
        for d in mom_file_literals(asm, known_stems):
            stem, ext = split_name(d['literal'])
            segs = d['segments']
            remom = collections.Counter()
            for s in segs:
                k = asm_seg_key(exe, s)
                if k and k in seg_map:
                    remom.update(seg_map[k])
            top_file = remom.most_common(1)[0][0] if remom else ''
            mom_proj = ''
            if top_file in files_csv and all(files_csv[top_file]['P_MoM_' + a] for a in sa.MOM_TEAM):
                r = files_csv[top_file]
                pm = {a: float(r['P_MoM_' + a]) for a in sa.MOM_TEAM}
                best = max(pm, key=pm.get)
                if pm[best] > 0.30:   # flat prior is 0.25 each
                    mom_proj = f'{best}:{pm[best]:.2f}'
            lit_rows.append({'exe': exe, 'order': d['order'], 'label': d['label'], 'literal': d['literal'], 'stem': stem.upper(), 'ext': ext,
                             'case_stem': case_class(stem), 'case_ext': case_class(ext) if ext else '',
                             'segments': ' '.join(f'{s}:{c}' for s, c in sorted(segs.items())),
                             'primary_segment': max(segs, key=segs.get) if segs else '',
                             'remom_files': ' '.join(f'{k}:{v}' for k, v in remom.most_common(3)), 'MoM_projection': mom_proj})

    moo2_lits, num_to, final = moo2_file_literals()
    moo2_rows = []
    for d in moo2_lits:
        stem, ext = split_name(d['literal'])
        mods = d['modules']
        prim = max(mods, key=mods.get) if mods else None
        owner, pown = final.get(prim, ('', 0.0)) if prim is not None else ('', 0.0)
        moo2_rows.append({'order': d['order'], 'label': d['label'], 'literal': d['literal'], 'stem': stem.upper(), 'ext': ext,
                          'case_stem': case_class(stem), 'case_ext': case_class(ext),
                          'personal_text_file': 1 if PERSONAL_MOO2_LBX.match(d['literal']) else 0,
                          'modules': ' '.join(f"{num_to[n]['short']}:{c}" for n, c in sorted(mods.items())),
                          'primary_module': num_to[prim]['short'] if prim is not None else '', 'primary_module_num': prim if prim is not None else '',
                          'owner': owner, 'P_owner': f'{pown:.3f}' if prim is not None else ''})

    # ---------------------------------------------------------------- write CSVs
    def write(name, rows):
        path = os.path.join(args.outdir, name)
        with open(path, 'w', newline='', encoding='latin-1') as f:
            w = csv.DictWriter(f, fieldnames=list(rows[0].keys()))
            w.writeheader()
            w.writerows(rows)
        return path
    write('SimTex-Corpus-MoM-Files.csv', file_rows)
    write('SimTex-Corpus-MoM-LBX-Entries.csv', lbx_rows)
    write('SimTex-Corpus-MoM-LBX-Literals.csv', lit_rows)
    write('SimTex-Corpus-MoO2-LBX-Literals.csv', moo2_rows)

    # ---------------------------------------------------------------- stats: files
    say('=== 1. OG MoM file names (MoM131) ===')
    mom_files = [r for r in file_rows if r['install'] == 'MoM131']
    say('class counts:', dict(collections.Counter(r['class'] for r in mom_files)))
    lbx_stems = sorted(r['stem'] for r in mom_files if r['class'] == 'simtex_lbx')
    say('LBX files:', len(lbx_stems), ' stem length histogram:', length_hist(lbx_stems), ' at the 8-char limit:', sum(len(s) == 8 for s in lbx_stems))
    say('LBX stems with a digit:', [s for s in lbx_stems if any(c.isdigit() for c in s)])
    demo_only = sorted({r['file'] for r in file_rows if r['install'] == 'MOMDEM' and r['class'] == 'simtex_lbx'} - {r['file'] for r in mom_files})
    say('LBX files only in the demo:', demo_only)
    moo2_stems = sorted({r['stem'] for r in moo2_rows})
    say('MoO2 LBX stems referenced in ORION2.EXE:', len(moo2_stems), ' length histogram:', length_hist(moo2_stems), ' at 8:', sum(len(s) == 8 for s in moo2_stems))
    shared = sorted(set(lbx_stems) & set(moo2_stems))
    say('LBX stems shared by MoM and MoO2:', shared)

    # ---------------------------------------------------------------- stats: LBX entry tables
    say('')
    say('=== 2. LBX entry-name tables ===')
    mom_lbx_files = [r for r in mom_files if r['class'] == 'simtex_lbx']
    with_tab = [r for r in mom_lbx_files if r['has_name_table'] == 1]
    say('MoM131 LBX files with a name table:', len(with_tab), '/', len(mom_lbx_files), ' without:', sorted(r['file'] for r in mom_lbx_files if r['has_name_table'] != 1))
    ent = [r for r in lbx_rows if r['install'] == 'MoM131']
    names = [r['entry_name'] for r in ent if r['entry_name']]
    descs = [r['description'] for r in ent if r['description']]
    say('entries:', len(ent), ' with name:', len(names), ' distinct names:', len(set(names)), ' with description:', len(descs))
    say('entry-name case:', dict(collections.Counter(case_class(n) for n in names)))
    say('entry-name length histogram:', length_hist(names), ' at the 8-char limit:', sum(len(n) == 8 for n in names))
    say('entry names containing "_":', sum('_' in n for n in names), ' containing a digit:', sum(any(c.isdigit() for c in n) for n in names),
        ' containing a space:', sum(' ' in n for n in names))
    # name reuse: consecutive repeats inside a file, and names appearing in several LBX files
    rep = 0
    by_file = collections.defaultdict(list)
    for r in ent:
        by_file[r['lbx']].append(r['entry_name'])
    for lst in by_file.values():
        rep += sum(1 for a, b in zip(lst, lst[1:]) if a and a == b)
    say('entries whose name repeats the previous entry (same source file, many records):', rep, '/', len(ent))
    files_per_name = collections.defaultdict(set)
    for r in ent:
        if r['entry_name']:
            files_per_name[r['entry_name']].add(r['lbx'])
    multi = sorted((n for n, s in files_per_name.items() if len(s) > 1), key=lambda n: -len(files_per_name[n]))
    say('entry names used in more than one LBX file:', len(multi), ' top:', [(n, len(files_per_name[n])) for n in multi[:12]])
    eq_stem = sum(1 for r in ent if r['entry_name'] and r['entry_name'].upper() == r['lbx'][:-4].upper())
    say('entry name == its LBX stem:', eq_stem)
    say('description case:', dict(collections.Counter(case_class(d) for d in descs)))
    say('description length histogram (23-byte field => max 22):', length_hist(descs), ' at 22:', sum(len(d) == 22 for d in descs))
    words = collections.Counter(w.lower() for d in descs for w in re.findall(r'[A-Za-z]+', d))
    say('top description words:', words.most_common(30))
    # name-vs-ReMoM: how many distinct entry names are also ReMoM identifiers/strings? (quick text check)
    demo_ent = [r for r in lbx_rows if r['install'] == 'MOMDEM']
    full_key = {(r['lbx'], r['entry']): (r['entry_name'], r['description']) for r in ent}
    diff = [(r['lbx'], r['entry'], full_key.get((r['lbx'], r['entry'])), (r['entry_name'], r['description'])) for r in demo_ent
            if (r['lbx'], r['entry']) in full_key and full_key[(r['lbx'], r['entry'])] != (r['entry_name'], r['description'])]
    # Index-by-index comparison overstates change (records were inserted/reordered between builds); compare name SETS per file.
    sets = collections.defaultdict(lambda: collections.defaultdict(set))
    for r in lbx_rows:
        if r['entry_name']:
            sets[r['lbx']][r['install']].add(r['entry_name'])
    common = [f for f in sets if 'MOMDEM' in sets[f] and 'MoM131' in sets[f]]
    all_rel = {n for f in sets for n in sets[f].get('MoM131', ())}
    only_demo = sorted({n for f in common for n in sets[f]['MOMDEM'] - sets[f]['MoM131']})
    gone = sorted(n for n in only_demo if n not in all_rel)
    say('demo vs v1.31: LBX files with a table in both:', len(common), ' index positions that differ:', len(diff),
        ' names in a demo file but not in the same v1.31 file:', len(only_demo), ' of which in NO v1.31 file:', len(gone))
    say('   names absent from v1.31 entirely:', gone)

    # ---------------------------------------------------------------- stats: literals
    say('')
    say('=== 3. File-name string literals in the OG executables ===')
    for exe in ('WIZARDS', 'MAGIC'):
        rows = [r for r in lit_rows if r['exe'] == exe]
        say(f'{exe}: literals:', len(rows), ' with .ext:', sum(1 for r in rows if r['ext']), ' stem case:', dict(collections.Counter(r['case_stem'] for r in rows)),
            ' unreferenced:', sum(1 for r in rows if not r['segments']))
    say('MoO2: LBX literals:', len(moo2_rows), ' personal-text-file literals:', sum(r['personal_text_file'] for r in moo2_rows),
        ' stem case:', dict(collections.Counter(r['case_stem'] for r in moo2_rows)),
        ' ext case:', dict(collections.Counter(r['case_ext'] for r in moo2_rows)))
    say('MoM: stem case vs presence of ".LBX":', dict(collections.Counter((r['case_stem'], bool(r['ext'])) for r in lit_rows)))

    # per-stem consistency
    def stem_consistency(rows):
        by = collections.defaultdict(set)
        for r in rows:
            by[r['stem']].add(r['case_stem'])
        multi = {s: c for s, c in by.items() if sum(1 for r in rows if r['stem'] == s) >= 2}
        return sum(1 for c in multi.values() if len(c) == 1), len(multi), {s: sorted(c) for s, c in multi.items() if len(c) > 1}
    for tag, rows in (('MoM', lit_rows), ('MoO2', moo2_rows)):
        one, tot, mixed = stem_consistency(rows)
        say(f'{tag}: stems written >=2 times: {tot}; always the same case: {one}; mixed: {mixed}')

    # ---------------------------------------------------------------- calibration on MoO2: does case follow the owner?
    say('')
    say('=== 4. Calibration on MoO2: does literal case follow the programmer? ===')
    cal = [r for r in moo2_rows if r['owner'] and r['owner'] != 'Other' and not r['personal_text_file'] and float(r['P_owner'] or 0) >= 0.5]
    say('MoO2 literals usable (owner known with P>=0.5, personal text files excluded):', len(cal))
    tab = collections.defaultdict(collections.Counter)
    for r in cal:
        tab[r['owner']][r['case_stem']] += 1
    for o in sa.NAMED:
        if tab[o]:
            say(f'   {o:7s}', dict(tab[o]))
    for unit, key in (('owner', lambda r: r['owner']), ('module', lambda r: r['primary_module'])):
        res = perm_test(cal, key, lambda r: r['case_stem'], lambda r: r['stem'], args.iters, rng)
        say(f'MoO2 purity by {unit}: observed {res["observed"]:.3f} (n={res["n"]}); global null {res["global"]["mean"]:.3f} p={res["global"]["p"]:.2g};'
            f' same-stem null {res["stratum"]["mean"]:.3f} p={res["stratum"]["p"]:.2g}')
    obs, mean, p, per = lower_heterogeneity(cal, lambda r: r['owner'], lambda r: r['stem'], args.iters, rng)
    say(f'MoO2 lowercase rate differs by owner? chi2 {obs:.2f} vs same-stem null mean {mean:.2f}; p={p:.2g};  lower/n:',
        {o: per[o] for o in sa.NAMED if o in per})
    ken = [dict(r, grp='Ken' if r['owner'] == 'Ken' else 'rest') for r in cal]
    obs, mean, p, per = lower_heterogeneity(ken, lambda r: r['grp'], lambda r: r['stem'], args.iters, rng)
    say(f'MoO2 Ken vs rest lowercase: chi2 {obs:.2f} vs same-stem null mean {mean:.2f}; p={p:.2g};  lower/n:', dict(per))

    # ---------------------------------------------------------------- MoM: does literal case cluster by segment?
    say('')
    say('=== 5. MoM: does literal case cluster by code segment (overlay)? ===')
    mom_ref = [r for r in lit_rows if r['primary_segment']]
    for exe in ('WIZARDS', 'MAGIC', 'both'):
        rows = mom_ref if exe == 'both' else [r for r in mom_ref if r['exe'] == exe]
        res = perm_test(rows, lambda r: (r['exe'], r['primary_segment']), lambda r: r['case_stem'], lambda r: r['stem'], args.iters, rng)
        say(f'{exe:8s} purity by segment: observed {res["observed"]:.3f} (n={res["n"]}); global null {res["global"]["mean"]:.3f} p={res["global"]["p"]:.2g};'
            f' same-stem null {res["stratum"]["mean"]:.3f} p={res["stratum"]["p"]:.2g}')
    # runs along overlay (link) order, WIZARDS overlays only
    seg_major = []
    by_seg = collections.defaultdict(list)
    for r in mom_ref:
        if r['exe'] == 'WIZARDS' and r['primary_segment'].startswith('ovr'):
            by_seg[r['primary_segment']].append(r['case_stem'])
    for s in sorted(by_seg):
        seg_major.append(collections.Counter(by_seg[s]).most_common(1)[0][0])
    obs, mean, p = runs_test(seg_major, args.iters, rng)
    say(f'WIZARDS overlays in link order: {len(seg_major)} overlays with literals; case changes between neighbours {obs} vs random-order mean {mean:.1f}; p(<=obs)={p:.2g}')
    say('WIZARDS overlay sequence (majority stem case):')
    line = []
    for s in sorted(by_seg):
        c = collections.Counter(by_seg[s])
        line.append(f"{s[3:]}:{''.join(k[0] for k in sorted(c.elements()))}")
    for i in range(0, len(line), 12):
        say('   ' + '  '.join(line[i:i + 12]))
    say('WIZARDS overlays with literals -> ReMoM file (by `// WZD oNNpMM` comments):')
    for s in sorted(by_seg):
        k = asm_seg_key('WIZARDS', s)
        files = ' '.join(f.split('/')[-1] for f, _ in seg_map.get(k, collections.Counter()).most_common(2))
        lits = sorted({r['literal'] for r in mom_ref if r['exe'] == 'WIZARDS' and r['primary_segment'] == s})
        say(f'   {s}  {files:28s} {" ".join(lits)[:150]}')
    # cross-tab vs MoM projection
    say('MoM literal case vs projected MoM author of the ReMoM file (projection > 0.30 only):')
    xt = collections.defaultdict(collections.Counter)
    for r in mom_ref:
        if r['MoM_projection']:
            xt[r['MoM_projection'].split(':')[0]][r['case_stem']] += 1
    for a in sa.MOM_TEAM:
        if xt[a]:
            say(f'   {a:8s}', dict(xt[a]))

    with open(os.path.join(args.outdir, 'SimTex-Corpus-Stats.txt'), 'w', encoding='latin-1', newline='\n') as f:
        f.write('\n'.join(report) + '\n')


if __name__ == '__main__':
    main()
