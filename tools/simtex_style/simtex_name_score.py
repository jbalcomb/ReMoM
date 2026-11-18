#!/usr/bin/env python3
"""Score every ReMoM function name for how closely it matches SimTex's naming style.

The yardstick is the MoO2 ORION2.EXE debug-symbol code names (the only SimTex names we have; see
doc/@SimTex_Style/SimTex-Coding-Style-And-Authorship.md section 3).  Each name is reduced to a handful of topic-neutral shape
features (token count, length, first-token class, verb position, case, abbreviations, digits, underscores).  The
probability of each feature value is learned from the MoO2 names, and a name's surprisal is the sum of -log2 P over
its features.

    score = percent of MoO2 SimTex names whose surprisal is >= this name's surprisal

so 50 is a typical SimTex name, 100 is as plain as they come, and 2 means only 2% of real SimTex names are this odd.

ReMoM bookkeeping markers (`__WIP`, `__STUB`, `__NIU`, `UU_`, ...) are stripped before scoring and reported separately.
Third-party APIs (Miles `AIL_*`) are listed but not scored.  Borland C++ 3.1 keeps 32 significant characters and
prefixes C names with `_`, so a MoM name over 31 characters is flagged `over_31` (a MoM constraint, not learned).

Outputs (doc/@SimTex_Style/): SimTex-Name-Scores-ReMoM-Functions.fwv, SimTex-Name-Scores-Stats.md.
Input: SimTex-Name-Renames-Proposed.fwv (hand-curated; columns function / kind / proposed / why).

stdlib only.  Run:  python3 tools/simtex_style/simtex_name_score.py
"""

import argparse
import bisect
import collections
import math
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import simtex_authorship as sa  # noqa: E402

SIMTEX_GROUPS = ('L:\\MOX', 'D:\\MOX', 'D:\\MOX\\OTHERS', 'WLIBS')
BORLAND_MAX = 31

STATUS_SUFFIX_RE = re.compile(r'__(WIP|NIU|STUB|SUTB|GEMINI|NOP|DUPE|MSDOS|ALWAYS_FALSE|\d+)$')
STATUS_PREFIX_RE = re.compile(r'^(UU|NIU)_')
SEGMENT_RE = re.compile(r'^\s*//\s*(WZD|MGC)\s+(o|s|ovr|seg)\d+p\d+', re.I)
THIRD_PARTY_FILES = {'MoX/src/SOUND.c': {'load_global_timbre'}}  # AIL demo code (header cites AIL2 MIXDEMO.C / XPLAY.C)
LIBC_NAMES = {'strcasecmp'}  # POSIX shim names: must keep the C library spelling

# Verbs: the authorship tool's set plus the verbs that open MoO2 and MoM names.
VERBS = sa.VERBS | set('''resolve enforce setup remap tint shear darken swap send scan calculate show spawn cast flip
    wait animate erase expand shrink raise lower grow refresh recalc rebuild redraw reload enter exit leave quit hide
    attack heal fly walk retreat flee capture transfer unload unpack pack sell buy hire dismiss disband train upgrade
    research learn summon banish dispel destroy kill resurrect raze conquer settle explore survey scout plan choose
    evaluate rank score count total sum fix tidy purge cleanup clean dump log trace print sort merge split crop clip
    scale stretch rotate wrap mix blend fade cycle store fetch push pop vanish appear'''.split())
STRONG_TAIL_VERBS = set('''draw load set get init reset update clear calc compute allocate free save restore add remove
    apply build create make check fill print display show play copy move find pick generate assign select sort
    process handle'''.split())
SMALL_WORDS = set('of to in at and or on by for from with x y n a an the as is it up off out no ok'.split())
ENGLISH_SHORT = set('''map war gem ore fly hit cut day end age old new top low big all any max min sum net set get
    add tax mod one two six ten dir row col sub key box bar pen ink arm leg eye sea sky sun air fog oil act job art
    gui ui cpu ems emm vga sdl fps ini rng bit hex log fx sfx id ok mp hp tp cp np pc ai
    not zap ask who mid mud yes win lose run use own via per'''.split())


def name_tokens(name):
    return [t for t in name.strip('_').split('_') if t]


def split_markers(name):
    stem = name
    markers = []
    m = STATUS_PREFIX_RE.match(stem)
    if m:
        markers.append(m.group(1) + '_')
        stem = stem[m.end():]
    while True:
        m = STATUS_SUFFIX_RE.search(stem)
        if not m:
            break
        markers.insert(0, m.group(0))
        stem = stem[:m.start()]
    return stem, ''.join(markers)


def bucket(value, edges):
    for e in edges:
        if value <= e:
            return f'<={e}'
    return f'>{edges[-1]}'


class Vocab:
    """MoO2 token counts; leave-one-out when the scored name is itself a MoO2 name."""

    def __init__(self, names):
        self.tok = collections.Counter(t.lower() for n in names for t in set(name_tokens(n)))
        self.caps = collections.Counter(t for n in names for t in set(name_tokens(n)) if len(t) >= 2 and t.isupper())

    def known(self, token, own=False):
        return self.tok[token.lower()] - (1 if own else 0) > 0

    def caps_known(self, token, own=False):
        return self.caps[token] - (1 if own else 0) > 0


def features(name, vocab, own=False):
    """Categorical shape features plus human-readable notes for the unusual ones."""
    toks = name_tokens(name)
    low = [t.lower() for t in toks]
    f = {}
    notes = {}
    f['ntok'] = bucket(len(toks), [1, 2, 3, 4, 5, 6])
    f['len'] = bucket(len(name), [8, 14, 20, 26, 31, 40])
    first = toks[0] if toks else ''
    if not first:
        cls = 'empty'
    elif first[0].islower():
        cls = 'lowercase'
    elif first.lower() in VERBS:
        cls = 'verb'
    elif first in ('Is', 'Has', 'Can'):
        cls = 'predicate'
    elif first == 'N':
        cls = 'count'
    elif len(first) >= 2 and first.isupper():
        cls = 'caps_attested' if vocab.caps_known(first, own) else 'caps_prefix'
    else:
        cls = 'noun'
    f['first'] = cls
    if cls == 'caps_prefix':
        notes['first'] = f'all-caps prefix {first}_'
    elif cls == 'lowercase':
        notes['first'] = f'lower-case first token {first}'
    f['verb_late'] = cls not in ('verb', 'predicate') and len(low) >= 2 and low[-1] in STRONG_TAIL_VERBS
    if f['verb_late']:
        notes['verb_late'] = f'verb last ({toks[-1]})'
    worst = 'proper'
    rank = ['proper', 'caps_attested', 'small_lower', 'caps_other', 'lower_other', 'camel']
    # The first token's case is already carried by `first`; charge it again only when it is CamelCase.
    for i, t in enumerate(toks):
        if i == 0 and cls in ('caps_attested', 'caps_prefix', 'lowercase') and not re.search(r'[a-z][A-Z]', t):
            continue
        if re.search(r'[a-z][A-Z]', t):
            c = 'camel'
        elif len(t) >= 2 and t.isupper() and not t.isdigit():
            c = 'caps_attested' if vocab.caps_known(t, own) or t in ('AI', 'NPC', 'LBX', 'XP') else 'caps_other'
        elif t[0].islower():
            c = 'small_lower' if t in SMALL_WORDS else 'lower_other'
        else:
            c = 'proper'
        if rank.index(c) > rank.index(worst):
            worst = c
            if c == 'camel':
                notes['case'] = f'CamelCase token {t}'
            elif c == 'caps_other':
                notes['case'] = f'unattested all-caps {t}'
            elif c == 'lower_other':
                notes['case'] = f'lower-case token {t}'
    f['case'] = worst
    abbrevs = []
    for t in toks:
        tl = t.lower()
        if t.isdigit() or (len(t) >= 2 and t.isupper()) or tl in SMALL_WORDS or tl in ENGLISH_SHORT or vocab.known(t, own):
            continue
        if (len(tl) <= 3 and t.isalpha()) or (len(tl) >= 2 and not re.search(r'[aeiouy]', tl) and t.isalpha()):
            abbrevs.append(t)
    f['abbrev'] = bucket(len(abbrevs), [0, 1])
    if abbrevs:
        notes['abbrev'] = 'abbreviation ' + '/'.join(abbrevs)
    if any(re.search(r'\d', t) for t in toks[:-1]):
        f['digit'] = 'inner'
        notes['digit'] = 'digit inside the name'
    elif toks and re.search(r'\d', toks[-1]):
        f['digit'] = 'tail'
    else:
        f['digit'] = 'none'
    if '__' in name.strip('_'):
        f['underscore'] = 'double'
        notes['underscore'] = 'double underscore'
    elif name.startswith('_'):
        f['underscore'] = 'leading'
    else:
        f['underscore'] = 'plain'
    if len(toks) >= 6:
        notes['ntok'] = f'{len(toks)} tokens'
    if len(name) > 26:
        notes['len'] = f'{len(name)} chars'
    return f, notes


class Model:
    def __init__(self, moo2_names, vocab):
        self.vocab = vocab
        rows = [features(n, vocab, own=True)[0] for n in moo2_names]
        self.keys = list(rows[0].keys())
        self.counts = {k: collections.Counter(r[k] for r in rows) for k in self.keys}
        self.n = len(rows)
        self.moo2_surprisal = sorted(self.surprisal(r) for r in rows)

    def prob(self, key, value):
        c = self.counts[key]
        k = len(c) + 1
        return (c[value] + 0.5) / (self.n + 0.5 * k)

    def surprisal(self, feats):
        return sum(-math.log2(self.prob(k, feats[k])) for k in self.keys)

    def score(self, s):
        # percent of MoO2 names at least as surprising as s
        i = bisect.bisect_left(self.moo2_surprisal, s - 1e-9)
        return 100.0 * (len(self.moo2_surprisal) - i) / len(self.moo2_surprisal)


def block_comment_mask(lines):
    """True for each line that starts inside a /* ... */ block comment (string literals are not tracked)."""
    mask = []
    inside = False
    for line in lines:
        mask.append(inside)
        i = 0
        while i < len(line):
            if inside:
                j = line.find('*/', i)
                if j < 0:
                    break
                inside = False
                i = j + 2
            else:
                j = line.find('/*', i)
                k = line.find('//', i)
                if j < 0 or (0 <= k < j):
                    break
                inside = True
                i = j + 2
    return mask


def remom_function_origins(funcs):
    """og = has a `// WZD oNNNpNN` / `// MGC ...` segment comment in its header; third_party = Miles AIL_* / AIL demo code;
    libc = C-library shim names.  Matches that sit inside a block comment are dropped."""
    cache = {}
    masks = {}
    kept = []
    for fn in funcs:
        path = os.path.join(sa.REPO, fn['file'])
        if path not in masks:
            with open(path, encoding='latin-1') as f:
                masks[path] = block_comment_mask(f.readlines())
        if masks[path][fn['line'] - 1]:
            continue
        kept.append(fn)
    funcs = kept
    for fn in funcs:
        path = os.path.join(sa.REPO, fn['file'])
        if path not in cache:
            with open(path, encoding='latin-1') as f:
                cache[path] = f.readlines()
        lines = cache[path]
        origin = 'remom'
        if fn['name'].startswith('AIL_') or fn['name'] in THIRD_PARTY_FILES.get(fn['file'], ()):
            origin = 'third_party'
        elif fn['name'] in LIBC_NAMES:
            origin = 'third_party'
        else:
            for j in range(fn['line'] - 2, max(-1, fn['line'] - 40), -1):
                if lines[j].startswith('}'):
                    break
                if SEGMENT_RE.match(lines[j]):
                    origin = 'og'
                    break
        fn['origin'] = origin
    return funcs


def _fwv_cell(v):
    if v is None:
        return ''
    if isinstance(v, bool):
        return 'True' if v else 'False'
    return str(v)


def write_fwv(path, cols, rows):
    """Fixed-width text: a header line, then one row per line.  Columns are separated by two spaces; numbers are
    right-aligned, text left-aligned; free-text columns go last.  read_fwv() slices by the header's column starts."""
    cells = [[_fwv_cell(r.get(c, '')) for c in cols] for r in rows]
    numeric = []
    for ci, c in enumerate(cols):
        vals = [row[ci] for row in cells if row[ci] != '']
        numeric.append(bool(vals) and all(re.fullmatch(r'-?\d+(\.\d+)?', v) for v in vals))
    widths = [max([len(c)] + [len(row[ci]) for row in cells]) for ci, c in enumerate(cols)]

    def fmt(vals, header=False):
        out = []
        for ci, v in enumerate(vals):
            if numeric[ci] and not header:
                out.append(v.rjust(widths[ci]))
            else:
                out.append(v.ljust(widths[ci]))
        return '  '.join(out).rstrip()
    with open(path, 'w', encoding='utf-8', newline='\n') as f:
        f.write(fmt(cols, header=True) + '\n')
        for row in cells:
            f.write(fmt(row) + '\n')


def read_fwv(path):
    """Read a write_fwv() file: column boundaries are the start positions of the header's names."""
    with open(path, encoding='utf-8') as f:
        lines = [line.rstrip('\n') for line in f]
    starts = [(m.start(), m.group(0)) for m in re.finditer(r'\S+', lines[0])]
    rows = []
    for line in lines[1:]:
        if not line.strip():
            continue
        row = {}
        for k, (pos, name) in enumerate(starts):
            end = starts[k + 1][0] if k + 1 < len(starts) else None
            row[name] = line[pos:end].strip()
        rows.append(row)
    return rows


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--outdir', default=sa.STYLE_DIR)
    ap.add_argument('--renames', default=os.path.join(sa.STYLE_DIR, 'SimTex-Name-Renames-Proposed.fwv'))
    ap.add_argument('--bottom', type=float, default=20.0, help='percent of scored names that form the bottom group')
    args = ap.parse_args()

    mods = sa.load_modules()
    moo2 = sorted({s[2].rstrip('_') for m in mods if m['group'] in SIMTEX_GROUPS for s in m['syms'] if s[0] == 'code'})
    moo2_set = set(moo2)
    vocab = Vocab(moo2)
    model = Model(moo2, vocab)

    funcs = remom_function_origins(sa.remom_functions())
    by_name = collections.OrderedDict()
    for fn in funcs:
        by_name.setdefault(fn['name'], []).append(fn)

    renames = {}
    if os.path.exists(args.renames):
        for r in read_fwv(args.renames):
            renames[r['function']] = r

    rows = []
    for name, defs in by_name.items():
        stem, markers = split_markers(name)
        first = defs[0]
        origin = 'og' if any(d['origin'] == 'og' for d in defs) else first['origin']
        row = {'function': name, 'stem': stem, 'markers': markers, 'file': first['file'], 'line': first['line'],
               'n_defs': len(defs), 'origin': origin, 'chars': len(name), 'over_31': len(name) > BORLAND_MAX}
        if origin == 'third_party':
            row.update(score='', surprisal='', notes='third-party API, not scored')
        elif stem in moo2_set:
            feats, notes = features(stem, vocab)
            s = model.surprisal(feats)
            row.update(origin=origin + '+moo2_verbatim', score=round(model.score(s), 1), surprisal=round(s, 2),
                       notes='identical to a MoO2 SimTex name: exempt; ' + '; '.join(notes.values()), **{'f_' + k: v for k, v in feats.items()})
        else:
            feats, notes = features(stem, vocab)
            s = model.surprisal(feats)
            row.update(score=round(model.score(s), 1), surprisal=round(s, 2), notes='; '.join(notes.values()), **{'f_' + k: v for k, v in feats.items()})
        rows.append(row)

    exempt = [r for r in rows if r['origin'].endswith('+moo2_verbatim')]
    exempt.sort(key=lambda r: (r['score'], r['function']))
    scored = [r for r in rows if r['score'] != '' and r not in exempt]
    scored.sort(key=lambda r: (r['score'], -r['surprisal'], r['function']))
    cut = int(round(len(scored) * args.bottom / 100.0))
    while 0 < cut < len(scored) and scored[cut]['surprisal'] == scored[cut - 1]['surprisal']:
        cut += 1  # names with exactly the same surprisal as the last one in the group are in too, so the alphabet never decides membership
    for i, r in enumerate(scored):
        r['rank'] = i + 1
        r['bottom'] = i < cut
        rn = renames.get(r['function'])
        r['proposed'] = rn['proposed'] if rn else ''
        r['proposed_kind'] = rn['kind'] if rn else ''
        r['proposed_why'] = rn['why'] if rn else ''
        if r['proposed']:
            pstem, _ = split_markers(r['proposed'])
            pf, _ = features(pstem, vocab)
            r['proposed_score'] = round(model.score(model.surprisal(pf)), 1)
            r['proposed_chars'] = len(r['proposed'])
        else:
            r['proposed_score'] = ''
            r['proposed_chars'] = ''
    unscored = [r for r in rows if r['score'] == '']

    # Self-check of the yardstick: score distribution of the MoO2 names themselves (should be ~uniform 0..100).
    moo2_scores = [model.score(s) for s in model.moo2_surprisal]

    out_fwv = os.path.join(args.outdir, 'SimTex-Name-Scores-ReMoM-Functions.fwv')
    cols = ['rank', 'score', 'bottom', 'function', 'proposed_kind', 'proposed', 'proposed_score', 'proposed_chars', 'origin', 'file', 'line', 'n_defs',
            'chars', 'over_31', 'markers', 'stem', 'surprisal'] + ['f_' + k for k in model.keys] + ['notes', 'proposed_why']
    write_fwv(out_fwv, cols, scored + exempt + unscored)

    # ---- stats (Markdown) -------------------------------------------------------------------------------------------
    def pct(xs, f):
        return xs[min(len(xs) - 1, int(f * len(xs)))]

    def share5(xs):
        return f'{100 * sum(x < 5 for x in xs) / len(xs):.1f}%'

    lines = []
    p = lines.append
    p('# SimTex Function-Name Scores: Stats')
    p('')
    p('Generated by [tools/simtex_style/simtex_name_score.py](../../tools/simtex_style/simtex_name_score.py). Method: '
      '[SimTex-Coding-Style-And-Authorship.md §8.1](SimTex-Coding-Style-And-Authorship.md#81-scoring-remoms-function-names-against-these-rules). '
      'Per-name rows: [SimTex-Name-Scores-ReMoM-Functions.fwv](SimTex-Name-Scores-ReMoM-Functions.fwv).')
    p('')
    p(f'- **Yardstick:** {len(moo2):,} MoO2 SimTex code names ({", ".join("`" + g + "`" for g in SIMTEX_GROUPS)}).')
    p('- **Score:** percent of MoO2 names at least as surprising (feature probabilities learned from MoO2, add-0.5 smoothing).')
    p(f'- **ReMoM names:** {len(rows):,} distinct ({sum(len(v) for v in by_name.values()):,} definitions). Ranked {len(scored):,}; '
      f'identical to a MoO2 name (exempt) {len(exempt)}; third-party {len(unscored)}.')
    p('')
    p('## Score by group')
    p('')
    p('| Group | Names | Median | p10 | Share below 5 |')
    p('|---|---:|---:|---:|---:|')
    xs = sorted(moo2_scores)
    p(f'| MoO2 self-check | {len(xs):,} | {pct(xs, 0.5):.1f} | {pct(xs, 0.1):.1f} | {share5(xs)} |')
    xs = sorted(r['score'] for r in exempt)
    p(f'| ReMoM, identical to a MoO2 name | {len(xs):,} | {pct(xs, 0.5):.1f} | {pct(xs, 0.1):.1f} | {share5(xs)} |')
    for o, label in (('og', 'ReMoM, OG reconstruction'), ('remom', 'ReMoM-only code')):
        xs = sorted(r['score'] for r in scored if r['origin'] == o)
        if xs:
            p(f'| {label} | {len(xs):,} | {pct(xs, 0.5):.1f} | {pct(xs, 0.1):.1f} | {share5(xs)} |')
    p('')
    p('## Bottom group')
    p('')
    p(f'- **Bottom {args.bottom:g}%:** {cut} names, score ≤ {scored[cut-1]["score"] if cut else "-"}.')
    p(f'- **Over 31 characters (Borland):** {sum(r["over_31"] for r in scored)} ranked names, {sum(r["over_31"] for r in scored[:cut])} of them in the bottom group.')
    note_counts = collections.Counter()
    for r in scored[:cut]:
        for n in r['notes'].split('; '):
            if n:
                key = re.sub(r'^\d+ chars$', 'over 26 chars', n)
                key = re.sub(r'^\d+ tokens$', '6+ tokens', key)
                key = re.sub(r' \(\S+\)$', '', key)
                key = re.sub(r'^(all-caps prefix|unattested all-caps|CamelCase token|lower-case first token|lower-case token|abbreviation) \S+$', r'\1', key)
                note_counts[key] += 1
    p('')
    p('Reasons (a name can have several):')
    p('')
    p('| Count | Reason |')
    p('|---:|---|')
    for k, v in note_counts.most_common():
        p(f'| {v} | {k} |')
    p('')
    p('## Proposals')
    p('')
    p('From [SimTex-Name-Renames-Proposed.fwv](SimTex-Name-Renames-Proposed.fwv).')
    p('')
    kinds = collections.Counter(r['proposed_kind'] or 'MISSING' for r in scored[:cut])
    with_prop = [r for r in scored[:cut] if r['proposed_kind'] == 'rename']
    fam = [r for r in scored[cut:] if r['proposed_kind'] == 'family']
    p('| Kind | Names |')
    p('|---|---:|')
    for k, v in kinds.most_common():
        p(f'| {k} | {v} |')
    p(f'| family (outside the bottom group, renamed for consistency) | {len(fam)} |')
    p('')
    if with_prop:
        p(f'- **Renames:** median score {pct(sorted(r["score"] for r in with_prop), 0.5)} before, '
          f'{pct(sorted(r["proposed_score"] for r in with_prop), 0.5)} after.')
        p(f'- **Proposals over 31 characters:** {sum(1 for r in with_prop + fam if r["proposed_chars"] > BORLAND_MAX)}.')
    p('')
    p('## Feature distributions, MoO2 vs ReMoM (ranked names)')
    p('')
    p('| Feature | Value | MoO2 | ReMoM | −log2 P |')
    p('|---|---|---:|---:|---:|')
    rem_counts = {k: collections.Counter(r['f_' + k] for r in scored) for k in model.keys}
    for k in model.keys:
        vals = sorted(set(model.counts[k]) | set(rem_counts[k]), key=lambda v: -model.counts[k][v])
        for v in vals:
            a = model.counts[k][v] / model.n
            b = rem_counts[k][v] / len(scored)
            p(f'| {k} | `{v}` | {100*a:.1f}% | {100*b:.1f}% | {-math.log2(model.prob(k, v)):.2f} |')
    out_md = os.path.join(args.outdir, 'SimTex-Name-Scores-Stats.md')
    with open(out_md, 'w', encoding='utf-8') as f:
        f.write('\n'.join(lines) + '\n')
    out_list = os.path.join(args.outdir, 'SimTex-Name-Change-List.md')
    write_change_list(out_list, scored, cut, args.bottom)
    sys.stdout.flush()
    sys.stdout.buffer.write(('\n'.join(lines) + f'\nwrote {out_fwv}\nwrote {out_md}\nwrote {out_list}\n').encode('utf-8'))  # the Windows console is cp1252: no ≤ / −


def write_change_list(path, scored, cut, bottom_pct):
    """The bottom group and its proposals as a reviewable Markdown list, one table per kind, source lines linked."""
    def loc(r):
        return f'[{os.path.basename(r["file"])}:{r["line"]}](../../{r["file"]}#L{r["line"]})'

    def cell(s):
        return str(s).replace('|', '/')
    bottom = scored[:cut]
    renames = [r for r in bottom if r['proposed_kind'] == 'rename']
    family = [r for r in scored if r['proposed_kind'] == 'family']
    keeps = [r for r in bottom if r['proposed_kind'] == 'keep']
    nones = [r for r in bottom if r['proposed_kind'] == 'none']
    missing = [r for r in bottom if not r['proposed_kind']]
    L = []
    p = L.append
    p('# SimTex Name Change List')
    p('')
    p(f'The bottom {bottom_pct:g}% of ReMoM function names by SimTex-style score ({cut} names, score ≤ {scored[cut - 1]["score"] if cut else "-"}), with the proposals from '
      '[SimTex-Name-Renames-Proposed.fwv](SimTex-Name-Renames-Proposed.fwv). Generated by '
      '[tools/simtex_style/simtex_name_score.py](../../tools/simtex_style/simtex_name_score.py); method in '
      '[SimTex-Coding-Style-And-Authorship.md §8.1](SimTex-Coding-Style-And-Authorship.md#81-scoring-remoms-function-names-against-these-rules). '
      'Scores are percentiles against MoO2\'s SimTex names: 50 is typical, below 5 is rarer than 95% of them.')
    p('')
    p(f'| Kind | Names |')
    p('|---|---:|')
    p(f'| rename | {len(renames)} |')
    p(f'| family member outside the bottom group, renamed with its family | {sum(1 for r in family if not r["bottom"])} |')
    p(f'| keep (the flagged shape is a MoO2 pattern) | {len(keeps)} |')
    p(f'| no proposal (placeholder names) | {len(nones)} |')
    if missing:
        p(f'| **missing a row** | {len(missing)} |')
    p('')
    for title, rows, with_new in (('Renames', renames + [r for r in family if r['bottom']], True),
                                  ('Family members outside the bottom group', [r for r in family if not r['bottom']], True),
                                  ('Keeps', keeps, False), ('No proposal', nones, False), ('Missing a row', missing, False)):
        if not rows:
            continue
        p(f'## {title}')
        p('')
        if with_new:
            p('| Current | Proposed | Score | → | Location | Why |')
            p('|---|---|---:|---:|---|---|')
            for r in sorted(rows, key=lambda r: (r['file'], int(r['line']))):
                p(f'| `{r["function"]}` | `{r["proposed"]}` | {r["score"]} | {r["proposed_score"]} | {loc(r)} | {cell(r["proposed_why"])} |')
        else:
            p('| Name | Score | Location | Why |')
            p('|---|---:|---|---|')
            for r in sorted(rows, key=lambda r: (r['file'], int(r['line']))):
                p(f'| `{r["function"]}` | {r["score"]} | {loc(r)} | {cell(r.get("proposed_why", "") or r["notes"])} |')
        p('')
    with open(path, 'w', encoding='utf-8', newline='\n') as f:
        f.write('\n'.join(L) + '\n')


if __name__ == '__main__':
    main()
