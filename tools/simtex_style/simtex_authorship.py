#!/usr/bin/env python3
"""
SimTex authorship model: estimates which MoO2 programmer owned each ORION2.EXE source module, then projects
those estimates onto ReMoM (MoM) source files through exact function-name matches and `// MoO2 Module:` comments.

Ground truth = the Watcom debug info of the MS-DOS ORION2.EXE, as extracted into doc/@MoO2/modules/*.fwv, plus the
IDA listing doc/@MoO2/ORION2.asm (proc boundaries and call sites).

Seed (labelled) modules are the ones whose FILE NAME names a programmer: jim.c, ken.c, harold.c, haccess.c, eric.c,
ericnet.c, erichelp.c, escroll.c, estrings.c, evanhelp.c, russ.c, bill.c, billhelp.c.  Everything else is scored.

Evidence families (each a naive-Bayes likelihood, tempered so one module counts as at most EFFECTIVE_N observations):
  dialect : calls to a programmer's personal string loader (H_Message, E_Strings, Get_Text_Message, Ken_Get_Text_Message,
            Load_Russ_Strings, ...) and references to a personal text LBX (jimtext, KENTEXT, billtext, estrings, H?STRNGS)
  helper  : calls into another module that is a seed module of a programmer (inter-module calls only)
  style   : binary naming-style features of the module's own code-symbol names (topic-neutral by construction)

Usage:
  python3 tools/simtex_style/simtex_authorship.py   # writes CSVs into doc/@SimTex_Style/ and prints a summary
  python3 tools/simtex_style/simtex_authorship.py --validate  # also prints leave-one-seed-module-out results

Everything here is an ESTIMATE with stated assumptions; see doc/@SimTex_Style/SimTex-Coding-Style-And-Authorship.md.
"""

import argparse
import collections
import csv
import glob
import math
import os
import re
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))  # tools/simtex_style/ -> repo root
MOO2_DIR = os.path.join(REPO, 'doc', '@MoO2')            # inputs: modules/*.fwv, ORION2.asm
STYLE_DIR = os.path.join(REPO, 'doc', '@SimTex_Style')   # outputs: SimTex-* docs and tables
ASM_PATH = os.path.join(MOO2_DIR, 'ORION2.asm')

AUTHORS = ['Jim', 'Ken', 'Harold', 'Eric', 'Russ', 'Bill', 'Evan', 'Other']
NAMED = AUTHORS[:-1]

SEED_MODULES = {
    'jim.c': 'Jim',
    'ken.c': 'Ken',
    'harold.c': 'Harold', 'haccess.c': 'Harold',
    'eric.c': 'Eric', 'ericnet.c': 'Eric', 'erichelp.c': 'Eric', 'escroll.c': 'Eric', 'estrings.c': 'Eric',
    'evanhelp.c': 'Evan',
    'russ.c': 'Russ',
    'bill.c': 'Bill', 'billhelp.c': 'Bill',
}

# Personal string loaders (callee symbol, without trailing underscore) -> owner.
DIALECT_CALLEES = {
    'H_Message': 'Harold', 'Load_H_Strings': 'Harold', 'Load_H_And_Pro_File_Strings': 'Eric',
    'E_Strings': 'Eric', 'Load_E_Strings': 'Eric',
    'Get_Text_Message': 'Jim',
    'Ken_Get_Text_Message': 'Ken',
    'Load_Russ_Strings': 'Russ',
    'Get_Help_LBX_Message': 'Evan',
}
# Personal text LBX file names (regex on the lower-cased file name) -> owner.
DIALECT_LBX = [
    (re.compile(r'^jimtext\d*\.lbx$'), 'Jim'),
    (re.compile(r'^kentext\d*\.lbx$'), 'Ken'),
    (re.compile(r'^billtex(t|\d)\d*\.lbx$'), 'Bill'),
    (re.compile(r'^estr[a-z]*\.lbx$'), 'Eric'),
    (re.compile(r'^h[a-z]strngs\.lbx$'), 'Harold'),
]

# Assumptions (stated in the write-up; change here, re-run, compare).
EFFECTIVE_N = 12          # max independent observations credited to one module per evidence family
DIALECT_P_OWN = 0.80      # P(a string-loader call goes to the module author's own loader)
HELPER_OWN_BOOST = 4.0    # how much more often an author calls into their OWN seed modules than the global base rate
OTHER_PRIOR = 0.15        # prior mass for "Other" (a programmer with no seed module: e.g. Steve Barcia, unnamed staff)

# Link-order blocks.  ORION2.EXE's module numbers are link order.  The game objects fall into runs that are each
# alphabetically sorted and each contain exactly one programmer's seed module -- read as that programmer's makefile
# object list.  (inclusive module-number ranges; 0-8 is the L:\MOX core list and names no one)
LINK_BLOCKS = [
    (9, 17, 'Jim'),      # antaroom smack racecust | council diplodef dip-scrn events jim npcdiplo
    (18, 33, 'Ken'),     # cmbtai ... diplomac ... ken ... techinit   (alphabetical)
    (34, 64, 'Harold'),  # aipower aidesign hotpop | advciv antarans ... harold ... textbox   (alphabetical from 37)
    (65, 89, 'Eric'),    # beams ... eric ... ocpolpup (alphabetical) | erichelp escroll estrings newpup
    (90, 121, 'Russ'),   # aibomb ... russ ... trade   (alphabetical)
    (122, 132, 'Bill'),  # bill billhelp fmtpara info list racerprt racescrn science tech turnsum (alphabetical) | fieldsav
]


def link_block_owner(num):
    for lo, hi, owner in LINK_BLOCKS:
        if lo <= num <= hi:
            return owner
    return ''


# MoM projection.  MoM's credited programmers (MoM/src/CREDITS.c): Jim Cowlishaw, Ken Burd, Steve Barcia, Grissel Barcia.
# Only Jim and Ken also have MoO2 seed modules.  If the MoO2 owner of the matching code is one of them, the same person
# wrote the MoM ancestor with probability TRANSFER_TAU, otherwise the MoM author is drawn from the uniform MoM-team prior.
# If the MoO2 owner was not on the MoM team (Harold, Eric, Russ, Bill, Evan, Other), the MoO2 owner says nothing.
MOM_TEAM = ['Jim', 'Ken', 'Steve', 'Grissel']
TRANSFER_TAU = 0.6

THIRD_PARTY_RE =re.compile(r'(\\AIL\\|^ail|\\SMACK\\|^rfile|^unsmack|GCL520|\\ASM_386\\|\\compress\\)', re.I)

VERBS = set('''get set draw init load play print make do add clear is has check calc compute update build save restore read write
    find pick fade free allocate create remove reset copy move fill show display handle process assign apply select sort
    open close start stop end begin generate enable disable put execute release record take turn squeeze cache decode
    increment decrement inc dec shuffle compare convert determine test validate go kill destroy delete insert append
    render refresh toggle mark place create count'''.split())
SMALL_WORDS = set('of to in at and or on by for from with x y n id ok no a an the as is it'.split())
TYPE_TOKENS = set('char sint int long short uint word byte far near ptr seg'.split())
PERSON_TOKENS = set(a.lower() for a in NAMED) | {'h', 'e'}


def module_short(path):
    return re.split(r'[\\/]', path)[-1].lower()


def module_group(path):
    p = path.upper()
    if THIRD_PARTY_RE.search(path):
        return 'third_party'
    if p.startswith('L:\\MOX\\'):
        return 'L:\\MOX'
    if p.startswith('D:\\MOX\\OTHERS\\'):
        return 'D:\\MOX\\OTHERS'
    if p.startswith('D:\\MOX\\'):
        return 'D:\\MOX'
    if 'WLIBS' in p or re.match(r'^[a-z]+\\[a-z0-9_]+\.(asm|msm)$', path, re.I):
        return 'WLIBS'
    return 'crt'


def load_modules():
    mods = []
    for fwv in sorted(glob.glob(os.path.join(MOO2_DIR, 'modules', '*.fwv'))):
        num = None
        path = None
        syms = []
        with open(fwv, encoding='latin-1') as f:
            for line in f:
                if line.startswith('module_num='):
                    num = int(line.split('=', 1)[1])
                elif line.startswith('module='):
                    path = line.split('=', 1)[1].strip()
                else:
                    parts = line.split()
                    if len(parts) >= 8 and parts[5] in ('code', 'data') and (parts[0] == '-' or re.match(r'^m\d{3}f', parts[0])):
                        syms.append((parts[5], parts[6], parts[7]))
        mods.append({'num': num, 'path': path, 'short': module_short(path), 'group': module_group(path), 'syms': syms})
    return mods


def parse_asm(symbol_to_module):
    """Return (calls, lbx_refs): per-module Counter of callee symbols, per-module Counter of personal-LBX owners."""
    proc_re = re.compile(r'^(\S+)\s+proc\b')
    endp_re = re.compile(r'^(\S+)\s+endp\b')
    call_re = re.compile(r'^\s*call\s+(?:near ptr |far ptr )?([A-Za-z_@$?][\w@$?]*)')
    lbx_label_re = re.compile(r"^(\w+)\s+db\s+'([^']+\.lbx)'", re.I)
    label_owner = {}
    with open(ASM_PATH, encoding='latin-1') as f:
        lines = f.readlines()
    for line in lines:
        m = lbx_label_re.match(line)
        if m:
            fname = m.group(2).lower()
            for rx, owner in DIALECT_LBX:
                if rx.match(fname):
                    label_owner[m.group(1)] = owner
    token_re = re.compile(r'\b(' + '|'.join(re.escape(k) for k in label_owner) + r')\b') if label_owner else None
    # Dialect per module: a personal-LBX reference counts once for its owner; a loader call counts for the loader's owner
    # ONLY when no personal-LBX reference occurred in the preceding LOADER_WINDOW lines (Get_Text_Message_ is generic:
    # Watcom register args, LBX file name in EAX -- `mov eax, offset aBilltext_lbx_0` / `call Get_Text_Message_`).
    LOADER_WINDOW = 8
    calls = collections.defaultdict(collections.Counter)
    dialect = collections.defaultdict(collections.Counter)
    current = None
    last_lbx_line = -999
    for idx, line in enumerate(lines):
        m = proc_re.match(line)
        if m:
            current = symbol_to_module.get(m.group(1))
            last_lbx_line = -999
            continue
        if endp_re.match(line):
            current = None
            continue
        if current is None:
            continue
        m = call_re.match(line)
        if m:
            callee = m.group(1)
            calls[current][callee] += 1
            name = callee.rstrip('_')
            if name in DIALECT_CALLEES and idx - last_lbx_line > LOADER_WINDOW:
                dialect[current][('call', name)] += 1
            continue
        if token_re:
            for t in token_re.findall(line.split(';')[0]):
                dialect[current][('lbx', label_owner[t])] += 1
                last_lbx_line = idx
    return calls, dialect, label_owner


def name_tokens(name):
    return [t for t in name.strip('_').split('_') if t]


def style_features(name):
    """Binary, topic-neutral naming features of one code symbol (name without the Watcom trailing underscore)."""
    toks = name_tokens(name)
    low = [t.lower() for t in toks]
    feats = {}
    feats['leading_underscore'] = name.startswith('_')
    feats['tokens_le2'] = len(toks) <= 2
    feats['tokens_ge5'] = len(toks) >= 5
    first = low[0] if low else ''
    feats['verb_first'] = first in VERBS
    feats['noun_first'] = bool(low) and first not in VERBS and first not in PERSON_TOKENS
    feats['predicate'] = first in ('is', 'has') or any(t in ('is', 'has', 'can') for t in low[1:])
    feats['n_count'] = 'n' in low
    feats['from_or_to_conv'] = 'from' in low or ('to' in low[1:] and first not in VERBS)
    feats['lower_token'] = any(t[0].islower() for t in toks[1:]) or (bool(toks) and toks[0][0].islower())
    feats['camel_token'] = any(re.search(r'[a-z][A-Z]', t) for t in toks)
    feats['caps_token'] = any(len(t) >= 2 and t.isupper() and t.isalpha() for t in toks)
    feats['type_token'] = any(t in TYPE_TOKENS for t in low)
    feats['digit_token'] = any(re.search(r'\d', t) for t in toks)
    feats['abbrev_token'] = any(len(t) <= 3 and t.isalpha() and t not in SMALL_WORDS and t not in PERSON_TOKENS and not t.isupper() for t in low if t == t)
    feats['to_bitmap_variant'] = name.endswith('To_Bitmap')
    feats['my_their'] = any(t in ('my', 'their', 'your') for t in low)
    return feats


STYLE_KEYS = list(style_features('Get_X').keys())


def log_normalize(logp):
    mx = max(logp.values())
    z = sum(math.exp(v - mx) for v in logp.values())
    return {k: math.exp(v - mx) / z for k, v in logp.items()}


class Model:
    def __init__(self, mods, calls, lbx_refs, exclude_seed=None):
        self.mods = mods
        self.calls = calls
        self.lbx_refs = lbx_refs
        self.seed_of = {}
        for m in mods:
            a = SEED_MODULES.get(m['short'])
            if a and m['group'] != 'crt' and m['short'] != exclude_seed:
                self.seed_of[m['num']] = a
        self.sym_module = {}
        for m in mods:
            for kind, name, sym in m['syms']:
                if kind == 'code':
                    self.sym_module.setdefault(sym, m['num'])
        # Style: per-author Bernoulli rates with Beta(1,1) smoothing over seed code symbols.
        counts = {a: collections.Counter() for a in NAMED}
        totals = collections.Counter()
        all_counts = collections.Counter()
        all_total = 0
        for m in mods:
            if m['group'] in ('crt', 'third_party'):
                continue
            a = self.seed_of.get(m['num'])
            for kind, name, sym in m['syms']:
                if kind != 'code':
                    continue
                f = style_features(name)
                all_total += 1
                for k, v in f.items():
                    all_counts[k] += v
                if a:
                    totals[a] += 1
                    for k, v in f.items():
                        counts[a][k] += v
        self.style_rate = {}
        for a in NAMED:
            self.style_rate[a] = {k: (counts[a][k] + 1) / (totals[a] + 2) for k in STYLE_KEYS}
        self.style_rate['Other'] = {k: (all_counts[k] + 1) / (all_total + 2) for k in STYLE_KEYS}
        self.style_n = dict(totals)

    def prior(self):
        p = {a: (1 - OTHER_PRIOR) / len(NAMED) for a in NAMED}
        p['Other'] = OTHER_PRIOR
        return p

    def evidence(self, m):
        dialect = collections.Counter()
        helper = collections.Counter()
        # A module that DEFINES a programmer's loader is excluded from that programmer's dialect evidence (tautology).
        defined_loader_owners = {DIALECT_CALLEES[name] for kind, name, sym in m['syms'] if kind == 'code' and name in DIALECT_CALLEES}
        for (kind, key), n in self.lbx_refs.get(m['num'], {}).items():
            owner = DIALECT_CALLEES[key] if kind == 'call' else key
            if owner not in defined_loader_owners:
                dialect[owner] += n
        # Helper affinity: inter-module calls into seed modules, EXCLUDING the string loaders (already the dialect family).
        for callee, n in self.calls.get(m['num'], {}).items():
            if callee.rstrip('_') in DIALECT_CALLEES:
                continue
            target = self.sym_module.get(callee)
            if target is not None and target != m['num'] and target in self.seed_of:
                helper[self.seed_of[target]] += n
        style = [style_features(name) for kind, name, sym in m['syms'] if kind == 'code']
        return dialect, helper, style

    @staticmethod
    def _categorical_loglik(counter, p_own):
        """Each observation names an author B; P(B | true author A) = p_own if A==B else spread over the rest."""
        n = sum(counter.values())
        if n == 0:
            return {a: 0.0 for a in AUTHORS}, 0
        w = min(1.0, EFFECTIVE_N / n)
        k = len(NAMED)
        out = {}
        for a in AUTHORS:
            ll = 0.0
            for b, c in counter.items():
                if a == 'Other':
                    pb = 1.0 / k
                else:
                    pb = p_own if a == b else (1 - p_own) / (k - 1)
                ll += c * math.log(pb)
            out[a] = w * ll
        return out, n

    def _style_loglik(self, style):
        n = len(style)
        if n == 0:
            return {a: 0.0 for a in AUTHORS}
        # The style features are strongly correlated (one name trips several), so one SYMBOL counts as ~one observation:
        # divide by the feature count, then apply the same per-module cap as the other families.
        w = min(1.0, EFFECTIVE_N / n) / len(STYLE_KEYS)
        out = {}
        for a in AUTHORS:
            r = self.style_rate[a]
            ll = 0.0
            for f in style:
                for k in STYLE_KEYS:
                    ll += math.log(r[k] if f[k] else 1 - r[k])
            out[a] = w * ll
        return out

    def helper_base_rate(self):
        """Global share of inter-module helper calls landing in each programmer's seed modules (all game modules)."""
        if not hasattr(self, '_helper_base'):
            tot = collections.Counter()
            for m in self.mods:
                if m['group'] in ('L:\\MOX', 'D:\\MOX', 'D:\\MOX\\OTHERS', 'WLIBS'):
                    tot.update(self.evidence(m)[1])
            s = sum(tot.values())
            self._helper_base = {a: (tot[a] + 1) / (s + len(NAMED)) for a in NAMED}
        return self._helper_base

    def _helper_loglik(self, counter):
        """Everyone calls everyone's utilities at the base rate g_B; the author over-uses their own by HELPER_OWN_BOOST."""
        n = sum(counter.values())
        if n == 0:
            return {a: 0.0 for a in AUTHORS}
        w = min(1.0, EFFECTIVE_N / n)
        g = self.helper_base_rate()
        out = {}
        for a in AUTHORS:
            z = sum(g[b] * (HELPER_OWN_BOOST if b == a else 1.0) for b in NAMED)
            out[a] = w * sum(c * math.log(g[b] * (HELPER_OWN_BOOST if b == a else 1.0) / z) for b, c in counter.items())
        return out

    def posterior(self, m, families=('dialect', 'helper')):
        dialect, helper, style = self.evidence(m)
        logp = {a: math.log(p) for a, p in self.prior().items()}
        parts = {}
        parts['dialect'], _ = self._categorical_loglik(dialect, DIALECT_P_OWN)
        parts['helper'] = self._helper_loglik(helper)
        parts['style'] = self._style_loglik(style)
        for name, fam in parts.items():
            if name in families:
                for a in AUTHORS:
                    logp[a] += fam[a]
        return log_normalize(logp), dialect, helper, len(style), {f: log_normalize({a: math.log(self.prior()[a]) + v[a] for a in AUTHORS}) for f, v in parts.items()}


# ----------------------------------------------------------------------------------------------------------------------
# ReMoM side
# ----------------------------------------------------------------------------------------------------------------------

DEF_RE = re.compile(r'^(?!\s)(?!(?:if|else|while|for|switch|return|typedef|struct|union|enum|do|case|extern)\b)[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\(([^;{}]*)\)?\s*$')
MOO2_MODULE_RE = re.compile(r'MoO2[:\s]+Module:\s*([A-Za-z0-9_\-\.]+)', re.I)


def remom_functions():
    out = []
    for sub in ('MoM/src', 'MoX/src'):
        for path in sorted(glob.glob(os.path.join(REPO, sub, '**', '*.c'), recursive=True)):
            rel = os.path.relpath(path, REPO).replace('\\', '/')
            with open(path, encoding='latin-1') as f:
                lines = f.readlines()
            for i, line in enumerate(lines):
                m = DEF_RE.match(line)
                if not m or line.lstrip().startswith(('#', '//', '/*', '*')):
                    continue
                nxt = ''.join(lines[i:i + 3])
                if '{' not in nxt or nxt.find(';') != -1 and nxt.find(';') < nxt.find('{'):
                    continue
                name = m.group(1)
                if name in ('main', 'WinMain') and sub == 'MoX/src':
                    pass
                hint = None
                for j in range(i - 1, max(-1, i - 30), -1):
                    if re.match(r'^\}', lines[j]):
                        break
                    h = MOO2_MODULE_RE.search(lines[j])
                    if h:
                        hint = h.group(1).lower().rstrip('.')
                        break
                out.append({'file': rel, 'line': i + 1, 'name': name, 'hint': hint})
    return out


def aggregator_labels_from_asm(sym_module):
    """A personal string AGGREGATOR (Load_Russ_Strings, ...) calls each owned module's sub-loader (Load_Colcalc_Strings,
    ...).  Returns {module_num: (owner, sub_loader_symbol)}.  Not used by the model; held out for validation, then used
    as a hard label in the FINAL column."""
    aggregators = {'Load_Russ_Strings_': 'Russ', 'Load_H_Strings_': 'Harold', 'Load_E_Strings_': 'Eric'}
    out = {}
    cur = None
    with open(ASM_PATH, encoding='latin-1') as f:
        for line in f:
            pm = re.match(r'^(\S+)\s+proc\b', line)
            if pm:
                cur = pm.group(1)
                continue
            cm = re.match(r'^\s*call\s+(Load_\w+_Strings_)', line)
            if cm and cur in aggregators:
                target = sym_module.get(cm.group(1))
                if target is not None:
                    out[target] = (aggregators[cur], cm.group(1))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--validate', action='store_true', help='print leave-one-seed-module-out results')
    ap.add_argument('--outdir', default=STYLE_DIR)
    args = ap.parse_args()

    mods = load_modules()
    symbol_to_module = {}
    for m in mods:
        for kind, name, sym in m['syms']:
            if kind == 'code':
                symbol_to_module.setdefault(sym, m['num'])
    calls, lbx_refs, label_owner = parse_asm(symbol_to_module)
    model = Model(mods, calls, lbx_refs)
    aggregator_labels = aggregator_labels_from_asm(model.sym_module)
    num_to_mod_early = {m['num']: m for m in mods}
    game_groups = ('L:\\MOX', 'D:\\MOX', 'D:\\MOX\\OTHERS', 'WLIBS')

    # --- MoO2 module table ---------------------------------------------------------------------------------------------
    # Pass 1: model posterior (dialect + helper) per module.
    scored = []
    for m in mods:
        if m['group'] not in game_groups:
            continue
        post, dialect, helper, n_style, fam = model.posterior(m)
        scored.append((m, post, dialect, helper, n_style, fam))

    # Reference labels for estimating the link-block reliability q: hard labels (aggregator) and strong dialect calls.
    # Seeds are excluded -- the blocks were DEFINED by where the seeds sit.
    block_checks = []
    for m, post, dialect, helper, n_style, fam in scored:
        if SEED_MODULES.get(m['short']) or not link_block_owner(m['num']):
            continue
        best = max(post, key=post.get)
        if m['num'] in aggregator_labels:
            block_checks.append((m['short'], aggregator_labels[m['num']][0], link_block_owner(m['num']), 'aggregator'))
        elif sum(dialect.values()) >= 5 and post[best] >= 0.95:
            block_checks.append((m['short'], best, link_block_owner(m['num']), 'dialect'))
    block_agree = sum(1 for s, ref, blk, why in block_checks if ref == blk)
    q_block = (block_agree + 1) / (len(block_checks) + 2)

    # Pass 2: combine with the link block, emit rows.
    rows = []
    post_by_short = {}
    post_by_num = {}
    soft_by_num = {}
    for m, post, dialect, helper, n_style, fam in scored:
        blk = link_block_owner(m['num'])
        final = dict(post)
        if blk:
            for a in AUTHORS:
                lik = (1.0 / len(NAMED)) if a == 'Other' else (q_block if a == blk else (1 - q_block) / (len(NAMED) - 1))
                final[a] *= lik
            z = sum(final.values())
            final = {a: v / z for a, v in final.items()}
        best = max(post, key=post.get)
        fbest = max(final, key=final.get)
        seed = SEED_MODULES.get(m['short'], '')
        hard = seed or aggregator_labels.get(m['num'], ('', ''))[0]
        soft_by_num[m['num']] = (post, dict(final))
        if hard:
            final = {a: (1.0 if a == hard else 0.0) for a in AUTHORS}
            fbest = hard
        post_by_num[m['num']] = final
        post_by_short.setdefault(m['short'].replace('.c', ''), final)
        rows.append({
            'module_num': m['num'], 'module': m['short'], 'path': m['path'], 'group': m['group'], 'seed_author': seed,
            'n_code': n_style, 'n_data': sum(1 for s in m['syms'] if s[0] == 'data'),
            'dialect_evidence': ' '.join(f'{k}:{v}' for k, v in dialect.most_common()),
            'helper_evidence': ' '.join(f'{k}:{v}' for k, v in helper.most_common()),
            'link_block': blk,
            **{f'P_{a}': f'{post[a]:.3f}' for a in AUTHORS},
            'MAP': best, 'P_MAP': f'{post[best]:.3f}',
            'hard_label': hard,
            **{f'Pfinal_{a}': f'{final[a]:.3f}' for a in AUTHORS},
            'FINAL': fbest, 'P_FINAL': f'{final[fbest]:.3f}',
            'MAP_dialect_only': max(fam['dialect'], key=fam['dialect'].get) if dialect else '',
            'MAP_helper_only': max(fam['helper'], key=fam['helper'].get) if helper else '',
            'MAP_style_only': max(fam['style'], key=fam['style'].get) if n_style else '',
        })
    os.makedirs(args.outdir, exist_ok=True)
    mod_csv = os.path.join(args.outdir, 'SimTex-Authorship-MoO2-Modules.csv')
    with open(mod_csv, 'w', newline='', encoding='utf-8') as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0].keys()))
        w.writeheader()
        w.writerows(rows)

    # --- ReMoM function / file tables ------------------------------------------------------------------------------------
    moo2_name_modules = collections.defaultdict(set)
    for m in mods:
        if m['group'] in ('crt', 'third_party'):
            continue
        for kind, name, sym in m['syms']:
            if kind == 'code':
                moo2_name_modules[name].add(m['num'])
    num_to_mod = {m['num']: m for m in mods}
    funcs = remom_functions()
    frows = []
    per_file = collections.defaultdict(list)
    for fn in funcs:
        nums = sorted(moo2_name_modules.get(fn['name'], ()))
        via = 'name' if nums else ''
        if not nums and fn['hint']:
            cands = [m['num'] for m in mods if m['short'].split('.')[0] == fn['hint'].split('.')[0] and m['group'] in game_groups]
            if cands:
                nums = cands[:1]
                via = 'comment'
        mod_names = [num_to_mod[n]['short'] for n in nums]
        groups = sorted({num_to_mod[n]['group'] for n in nums})
        frows.append({'remom_file': fn['file'], 'line': fn['line'], 'function': fn['name'], 'match_via': via,
                      'moo2_modules': ' '.join(mod_names), 'moo2_groups': ' '.join(groups)})
        per_file[fn['file']].append((fn, nums))
    fn_csv = os.path.join(args.outdir, 'SimTex-Authorship-ReMoM-Functions.csv')
    with open(fn_csv, 'w', newline='', encoding='utf-8') as f:
        w = csv.DictWriter(f, fieldnames=list(frows[0].keys()))
        w.writeheader()
        w.writerows(frows)

    file_rows = []
    for path, items in sorted(per_file.items()):
        matched = [(fn, nums) for fn, nums in items if nums]
        modc = collections.Counter()
        grpc = collections.Counter()
        agg = collections.Counter()
        for fn, nums in matched:
            for n in nums:
                modc[num_to_mod[n]['short']] += 1.0 / len(nums)
                grpc[num_to_mod[n]['group']] += 1.0 / len(nums)
                if n in post_by_num:
                    for a, p in post_by_num[n].items():
                        agg[a] += p / len(nums)
        tot = sum(agg.values())
        mom = {a: 0.0 for a in MOM_TEAM}
        if tot:
            for b, pb in agg.items():
                pb /= tot
                for a in MOM_TEAM:
                    if b in MOM_TEAM:
                        mom[a] += pb * ((TRANSFER_TAU if a == b else 0.0) + (1 - TRANSFER_TAU) / len(MOM_TEAM))
                    else:
                        mom[a] += pb / len(MOM_TEAM)
        file_rows.append({
            'remom_file': path, 'n_functions': len(items), 'n_matched': len(matched),
            'pct_matched': f'{100.0 * len(matched) / len(items):.0f}' if items else '0',
            'top_moo2_modules': ' '.join(f'{k}:{v:.0f}' for k, v in modc.most_common(4)),
            'top_moo2_groups': ' '.join(f'{k}:{v:.0f}' for k, v in grpc.most_common(3)),
            **{f'P_MoO2_{a}': (f'{agg[a] / tot:.2f}' if tot else '') for a in AUTHORS},
            **{f'P_MoM_{a}': (f'{mom[a]:.2f}' if tot else '') for a in MOM_TEAM},
        })
    file_csv = os.path.join(args.outdir, 'SimTex-Authorship-ReMoM-Files.csv')
    with open(file_csv, 'w', newline='', encoding='utf-8') as f:
        w = csv.DictWriter(f, fieldnames=list(file_rows[0].keys()))
        w.writeheader()
        w.writerows(file_rows)

    # --- summary -----------------------------------------------------------------------------------------------------------
    print(f'modules scored: {len(rows)}   personal-LBX labels: {len(label_owner)}   ReMoM functions: {len(funcs)}   matched: {sum(1 for r in frows if r["match_via"])} (name {sum(1 for r in frows if r["match_via"] == "name")}, comment {sum(1 for r in frows if r["match_via"] == "comment")})')
    print('style training symbols per author:', dict(model.style_n))
    print(f'link block vs reference labels: {block_agree}/{len(block_checks)} agree -> q_block = {q_block:.3f}   disagreements: ' + ' '.join(f'{s}(ref={r},block={b},{w})' for s, r, b, w in block_checks if r != b))
    for lo, hi, owner in LINK_BLOCKS:
        names = [num_to_mod_early[n]['short'] for n in range(lo, hi + 1) if n in num_to_mod_early]
        runs, run = [], [names[0]]
        for a, b in zip(names, names[1:]):
            if b >= a:
                run.append(b)
            else:
                runs.append(run)
                run = [b]
        runs.append(run)
        print(f'  block {lo:>3}-{hi:<3} {owner:<7} sorted runs: ' + ' | '.join(' '.join(r) for r in runs))
    print('FINAL counts (non-WLIBS):', collections.Counter(r['FINAL'] for r in rows if r['group'] != 'WLIBS'))
    print('MAP counts (non-seed game modules):', collections.Counter(r['MAP'] for r in rows if not r['seed_author'] and r['group'] != 'WLIBS'))
    print('MAP counts (WLIBS):', collections.Counter(r['MAP'] for r in rows if r['group'] == 'WLIBS'))
    print(f'wrote {mod_csv}\nwrote {fn_csv}\nwrote {file_csv}')

    if args.validate:
        # Label-independent checks on non-seed game modules.
        pure_n = pure_hi = agree_n = agree_hit = 0
        disagreements = []
        for m in mods:
            if m['group'] not in game_groups or SEED_MODULES.get(m['short']):
                continue
            dialect, helper, style = model.evidence(m)
            nd, nh = sum(dialect.values()), sum(helper.values())
            if nd >= 5:
                pure_n += 1
                pure_hi += dialect.most_common(1)[0][1] / nd >= 0.8
            if nd >= 5 and nh >= 5:
                agree_n += 1
                hl = model._helper_loglik(helper)
                d_top, h_top = dialect.most_common(1)[0][0], max(NAMED, key=lambda a: hl[a])
                agree_hit += d_top == h_top
                if d_top != h_top:
                    disagreements.append(f'{m["short"]}(dialect={d_top} helper={h_top})')
        # Held-out check: a personal string AGGREGATOR (e.g. Load_Russ_Strings) calls each owned module's sub-loader
        # (Load_Colcalc_Strings, ...).  The model never sees this, so the sub-loader's module is a held-out label.
        held = [(truth, num, sub) for num, (truth, sub) in aggregator_labels.items() if num in soft_by_num and not SEED_MODULES.get(num_to_mod[num]['short'])]
        hit_model = hit_block = 0
        print('\nheld-out: non-seed modules whose string sub-loader is called by a personal aggregator (posteriors BEFORE the hard override;')
        print('          note q_block was estimated on a set that includes these modules, so the model+block column is only semi-independent)')
        for truth, num, sub in held:
            p_model, p_block = soft_by_num[num]
            b1, b2 = max(p_model, key=p_model.get), max(p_block, key=p_block.get)
            hit_model += b1 == truth
            hit_block += b2 == truth
            print(f'  {num_to_mod[num]["short"]:<12} via {sub:<28} truth={truth:<6} model={b1:<6} P={p_model[b1]:.2f}   model+block={b2:<6} P={p_block[b2]:.2f}')
        print(f'  held-out accuracy: model {hit_model}/{len(held)}   model+block {hit_block}/{len(held)}')
        print(f'\ndialect purity: {pure_hi}/{pure_n} non-seed modules with >=5 dialect observations have >=80% from one programmer')
        print(f'dialect vs helper agreement (both >=5 obs): {agree_hit}/{agree_n}   disagreements: {" ".join(disagreements)}')
        print('\nleave-one-seed-module-out (seed module removed from training; loader calls into a module that defines the loader are excluded):')
        hits = {'all': 0, 'dialect': 0, 'helper': 0, 'style': 0}
        n = 0
        for m in mods:
            truth = SEED_MODULES.get(m['short'])
            if not truth or m['group'] not in game_groups:
                continue
            lomo = Model(mods, calls, lbx_refs, exclude_seed=m['short'])
            post, dialect, helper, n_style, fam = lomo.posterior(m)
            best = max(post, key=post.get)
            fb = {k: (max(v, key=v.get) if v else '') for k, v in fam.items()}
            n += 1
            hits['all'] += best == truth
            for k in ('dialect', 'helper', 'style'):
                hits[k] += fb.get(k) == truth
            print(f'  {m["short"]:<12} truth={truth:<7} MAP={best:<7} P={post[best]:.2f}  dialect={fb.get("dialect", ""):<7} helper={fb.get("helper", ""):<7} style={fb.get("style", ""):<7}  dialect_ev=[{" ".join(f"{k}:{v}" for k, v in dialect.most_common())}]')
        print('  accuracy:', {k: f'{v}/{n}' for k, v in hits.items()})


if __name__ == '__main__':
    sys.exit(main())
