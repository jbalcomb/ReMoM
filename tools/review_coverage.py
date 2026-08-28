#!/usr/bin/env python3
"""Reconcile CodeReview doc coverage against the combat sources and the disassembly.

Four independent inventories are compared:

  MARKERS   every `// WZD <overlay><part>` marker in the combat sources -> the name that follows
  INTENT    the `...\\WIZARDS\\ovrNNN\\Name.asm` paths in a review doc's header block, bridged to
            production names through the doc's rename ledger and the coverage tables
  VERDICT   the rows of a review doc's coverage table (`## Scope` / `## Review status` / the
            unnamed lead table), each of which renders a fidelity result on one function
  TRACKER   the checked boxes in doc/@TODO/stub_wip_todo.md

COVERAGE AND DEBT ARE TWO DIFFERENT AXES.  A VERDICT says the C matches the listing; it says
nothing about whether the C is clean.  `--debt` reports the second axis -- non-compliant PARAMETERS,
header prototypes that disagree with their .c definition, raw `; BUG:` markers,
non-compliant locals, `/* CLAUDE */` edits to original game code, `DBG_` leftovers and
commented-out code -- per function, cross-referenced with the DONE set.  A function can be
perfectly faithful and still be a mess, and nothing here could see that until 2026-08-26.

A function counts as COVERED only when a DONE-DONE doc gives it a VERDICT row, or a DONE-DONE
doc is named after it.  A header-block listing is a statement of intent, not a verdict -- one
that appears there and in no table is exactly the discrepancy this script exists to surface.
Nothing here guesses; it reports set differences for a human to settle.

Overlay prefixes are written three ways across the sources (`s163`, `o163`, `ovr163`), so marker
keys are normalised to (overlay number, part number).  Without that, a table-of-contents marker
and the real one land under different keys and a reconstructed function reads as a stub.

Usage:
  python3 tools/review_coverage.py                     full report
  python3 tools/review_coverage.py --doc NAME          one doc's reconciliation
  python3 tools/review_coverage.py --homeless          the homeless functions -- Combat-Homeless.md's list body
  python3 tools/review_coverage.py --attribution       which review each uncovered function belongs to
  python3 tools/review_coverage.py --referrals         cross-doc "covered in X" claims that X does not support
  python3 tools/review_coverage.py --anchors           dead #L line anchors across the docs
  python3 tools/review_coverage.py --debt              style/annotation debt in DONE functions
  python3 tools/review_coverage.py --csv OUT.csv       per-marker matrix
"""
import argparse
import io
import os
import re

SRC_DIR = 'MoM/src'
SOURCES = ['Combat.c', 'COMBINIT.c', 'CMBMAGIC.c', 'CMBTAI.c', 'CMBTMVPT.c']
HEADERS = ['Combat.h']
DOC_DIR = 'doc/@CodeReview'
DOC_PREFIX = 'Combat-'
SKIP_DOCS = {'Combat-Homeless.md'}
TRACKER = 'doc/@TODO/stub_wip_todo.md'
ASM_ROOT = 'C:/STU/devel/STU-Extras/Piethawn/Piethawn/out/WIZARDS'


def read(path):
    return io.open(path, encoding='utf-8', errors='replace').read()


# ---------------------------------------------------------------- markers ---
MARKER_RE = re.compile(r'^//\s*WZD\s+([a-z]*)(\d+)p(\d+)\s*$')
DEF_RE = re.compile(r'^\s*(?:static\s+)?(?:void|int8_t|int16_t|int32_t|uint8_t|uint16_t|uint32_t|char|int|long|unsigned|SAMB_ptr|s_[A-Za-z_0-9]+\s*\*?|[A-Za-z_][A-Za-z_0-9]*\s*\*)\s*\**\s*([A-Za-z_][A-Za-z_0-9]*)\s*\(')
CMT_RE = re.compile(r'^//\s*(?:drake178:\s*)?([A-Za-z_][A-Za-z_0-9]*)\s*\(')
DRAKE_RE = re.compile(r'^//\s*drake178:')
HOMELESS_DOC_SPREAD = 3  # callers owned by this many distinct reviews -> nobody owns it, it is homeless
LOOKAHEAD = 400          # a slot's name may sit past a long banner comment; the next marker stops the scan


def scan_markers(paths, src_dir=SRC_DIR):
    """(overlay_num, part_num) -> merged view of that overlay slot

    Candidates are collected from every file carrying the marker.  A definition beats a
    commented-out placeholder, and a plain placeholder beats a `drake178:` alias -- the alias is
    an IDA database label, not a name this project uses.
    """
    slots = {}
    for rel in paths:
        lines = read(os.path.join(src_dir, rel)).split('\n')
        for i, raw in enumerate(lines):
            m = MARKER_RE.match(raw.strip())
            if not m:
                continue
            prefix, ovr, part = m.group(1), int(m.group(2)), int(m.group(3))
            slot = slots.setdefault((ovr, part), {'raw': prefix + m.group(2) + 'p' + m.group(3), 'cands': []})
            in_block = False
            for j in range(i + 1, min(i + LOOKAHEAD, len(lines))):
                nxt = lines[j]
                s = nxt.strip()
                if in_block:                               # inside /* ... */: prose here looks like code
                    in_block = '*/' not in s
                    continue
                if not s:
                    continue
                if s.startswith('/*'):
                    in_block = '*/' not in s[2:]
                    continue
                if MARKER_RE.match(s):
                    break                                  # a run of markers: this slot names nothing here
                d = DEF_RE.match(nxt)
                if d:
                    slot['cands'].append((d.group(1), 2, rel, j + 1))
                    break
                c = CMT_RE.match(s)
                if c:
                    # a name in a comment may be a placeholder for an unreconstructed slot, or
                    # just an annotation ahead of the real definition -- keep looking for a body
                    slot['cands'].append((c.group(1), 0 if DRAKE_RE.match(s) else 1, rel, j + 1))
                    continue
                if s.startswith(('//', '*')):
                    continue
                break
    for slot in slots.values():
        slot['cands'].sort(key=lambda c: -c[1])
    return slots


def resolve(slot):
    """(name, implemented, file, line) for an overlay slot, or None if nothing names it"""
    if not slot['cands']:
        return None
    name, rank, rel, line = slot['cands'][0]
    return name, rank == 2, rel, line


# ------------------------------------------------------------------- docs ---
ASMPATH_RE = re.compile(r'WIZARDS[\\/](ovr\d+)[\\/]([A-Za-z_0-9@]+)\.asm')
LISTING_RE = re.compile(r'(ovr\d+)/([A-Za-z_0-9@]+)\.asm')
STATUS_RE = re.compile(r'\*\*Status:\s*([A-Za-z-]+)')
RENAME_RE = re.compile(r'^([A-Za-z_][A-Za-z_0-9@.]*)\s*\(?\)?\s*==>\s*([A-Za-z_][A-Za-z_0-9]*)\s*\(?\)?\s*$')
VERDICT_RE = re.compile(r'faithful|deviat|\bR\d|body walk|not covered|OGBUG|matches|reviewed|deferred|out of scope|stub|adjudicat|needs a row', re.I)
# a coverage row settles its function unless the result cell says otherwise -- result cells are
# not written to a fixed vocabulary (some are just an asm line count), so only the explicit
# still-open wordings can be detected, and everything else in an adjudicating doc counts
NEGATE_RE = re.compile(r'never adjudicated|not covered|needs a row|out of scope|deferred|\bclaimed\b|unreviewed|pending', re.I)
IDENT_RE = re.compile(r'^`?([A-Za-z_][A-Za-z_0-9]*)`?(?:\s*\([^)]*\))?$')
SUFFIXES = ('__WIP', '__STUB', '__HACK', '__SEGRAX', '__NOOP', '__1')


def aliases(name):
    """the forms an IDA listing name may take in production"""
    out = {name}
    for suf in SUFFIXES:
        if name.endswith(suf):
            out.add(name[:-len(suf)])
    for n in list(out):
        if n.startswith('z'):
            out.add(n[1:])
    return out


def split_doc(text):
    """(header block, body) -- the header ends at the first top-level review heading"""
    for marker in ('# 1:1 Fidelity Review', '# Review status'):
        i = text.find(marker)
        if i >= 0:
            return text[:i], text[i:]
    i = text.find('\n---\n')
    return (text[:i], text[i:]) if i >= 0 else (text, text)


def table_rows(text):
    for raw in text.split('\n'):
        s = raw.strip()
        if not s.startswith('|'):
            continue
        cells = [c.strip() for c in s.strip('|').split('|')]
        if cells and set(''.join(cells)) <= set('- :'):
            continue                                        # separator row
        yield cells


def scan_docs():
    docs = {}
    for fn in sorted(os.listdir(DOC_DIR)):
        if not (fn.startswith(DOC_PREFIX) and fn.endswith('.md')) or fn in SKIP_DOCS:
            continue
        text = read(os.path.join(DOC_DIR, fn))
        head, body = split_doc(text)
        st = STATUS_RE.search(text)
        renames = {}
        for raw in text.split('\n'):
            m = RENAME_RE.match(raw.strip())
            if m:
                renames[m.group(1)] = m.group(2)
        docs[fn] = {
            'subject': fn[len(DOC_PREFIX):-3],
            'status': st.group(1).upper() if st else None,
            'header': [g[0] + '/' + g[1] for g in ASMPATH_RE.findall(head)],
            'renames': renames,
            'rows': list(table_rows(body)),
            'text': text,
        }
    return docs


def verdict_names(doc, universe):
    """production name -> (result cell, listing or None) for the doc's coverage rows

    A coverage row is one whose first cell is a bare identifier the rest of the project knows
    about, and which either cites an `ovrNNN/Name.asm` listing or ends in a fidelity verdict.
    Both forms are in use across the docs; requiring only one of them misses whole tables.
    """
    out = {}
    for cells in doc['rows']:
        if len(cells) < 2:
            continue
        m = IDENT_RE.match(cells[0])
        if not m or m.group(1) not in universe:
            continue
        joined = ' | '.join(cells)
        lst = ASMPATH_RE.search(joined) or LISTING_RE.search(joined)
        if not lst and not VERDICT_RE.search(cells[-1]):
            continue
        out.setdefault(m.group(1), (cells[-1], lst.group(1) + '/' + lst.group(2) if lst else None))
    return out


SECTION_RE = re.compile(r'^#\s+([A-Za-z_][A-Za-z_0-9]*(?:\s+and\s+[A-Za-z_][A-Za-z_0-9]*)*)\s*$', re.M)
WALKED_RE = re.compile(r'^\s*(?:\*\*)?(?:Both\s+)?(?:faithful|reconstructed|matches\b|1:1\b)', re.I)


def walked_sections(doc):
    """name -> the opening verdict of a `# <Function>` section that adjudicates it in prose

    Several docs walk a function under its own heading and declare it faithful there, but never
    add it to the coverage table.  That is a bookkeeping hole in the doc, not an unreviewed
    function, and the two must not be reported the same way.
    """
    out = {}
    marks = list(SECTION_RE.finditer(doc['text']))
    for k, m in enumerate(marks):
        end = marks[k + 1].start() if k + 1 < len(marks) else len(doc['text'])
        body = doc['text'][m.end():end].strip()
        first = next((l for l in body.split('\n') if l.strip()), '')
        if not WALKED_RE.match(first):
            continue
        for name in m.group(1).split(' and '):
            out.setdefault(name.strip(), first.strip().rstrip('.')[:60])
    return out


def tracker_boxes():
    done, still_open = set(), set()
    for raw in read(TRACKER).split('\n'):
        m = re.match(r'\s*- \[([ xX])\]\s+([A-Za-z_][A-Za-z_0-9]*)', raw)
        if m:
            (done if m.group(1).lower() == 'x' else still_open).add(m.group(2))
    return done, still_open


# ------------------------------------------------------------------ model ---
def build():
    slots = scan_markers(SOURCES)
    hdr_slots = scan_markers(HEADERS)
    docs = scan_docs()
    tracker_done, _ = tracker_boxes()

    universe = set()
    for group in (slots, hdr_slots):
        for slot in group.values():
            universe.update(c[0] for c in slot['cands'])
    for d in docs.values():
        universe.add(d['subject'])
        universe.update(l.split('/')[1] for l in d['header'])
        for k, v in d['renames'].items():
            universe.update((k, v))
    universe.update(tracker_done)

    # the coverage tables are the authoritative IDA-listing -> production bridge: first cell is
    # the production name, the ASM column names the listing it was checked against
    listing_to_production = {}
    for d in docs.values():
        d['verdicts'] = verdict_names(d, universe)
        for nm, (_result, listing) in d['verdicts'].items():
            if listing:
                listing_to_production.setdefault(listing.split('/')[1], nm)

    # An adjudication is a property of the ROW, not of the document.  Gating on a DONE-DONE
    # status line meant a doc reopened for a second pass instantly lost the verdicts its first
    # pass had earned, and its backlog rows -- "claimed, never adjudicated" -- counted as
    # coverage the moment they were written down.  Read the result cell instead.
    covered, walked, claimed = {}, {}, {}
    for fn, d in docs.items():
        adjudicating = d['status'] in ('DONE-DONE', 'REOPENED')
        for nm, (result, _l) in d['verdicts'].items():
            if adjudicating and not NEGATE_RE.search(result):
                covered.setdefault(nm, []).append((fn, result))
        if adjudicating and d['subject'] in universe:
            covered.setdefault(d['subject'], []).append((fn, 'doc subject'))
        if adjudicating:
            for nm, verdict in walked_sections(d).items():
                walked.setdefault(nm, (fn, verdict))

    def to_production(base):
        return listing_to_production.get(base) or next((a for a in aliases(base) if a in covered or a in universe), base)

    rows = []
    for key in sorted(slots):
        hdr = hdr_slots.get(key)
        got = resolve(slots[key]) or (resolve(hdr) if hdr else None)
        if not got:
            rows.append((key, slots[key]['raw'], None, False, 'NO NAME', '', '', 0))
            continue
        name, implemented, src, line = got
        if not implemented and hdr:
            alt = resolve(hdr)
            if alt and not alt[0].startswith('sub_'):
                name = alt[0]                              # the .h keeps the project name; the .c may hold a drake178 alias
        if name in covered:
            state, note = 'DONE', '%s (%s)' % (covered[name][0][0], covered[name][0][1][:44])
        elif name in tracker_done:
            state, note = 'DONE', 'tracker box only'
        elif name in walked:
            state, note = 'WALKED', '%s: "%s" -- no coverage-table row' % walked[name]
        else:
            in_header = [fn for fn, d in docs.items()
                         if d['status'] in ('DONE-DONE', 'REOPENED')
                         and any(to_production(l.split('/')[1]) == name for l in d['header'])]
            mentioned = [fn for fn, d in docs.items() if re.search(r'\b%s\b' % re.escape(name), d['text'])]
            if in_header:
                claimed[name] = in_header[0]
                state, note = 'CLAIMED', '%s header block, never adjudicated' % in_header[0]
            else:
                state = 'NOT DONE'
                note = ('no production body' if not implemented
                        else ('mentioned in %s, no verdict' % mentioned[0] if mentioned else ''))
        rows.append((key, slots[key]['raw'], name, implemented, state, note, src, line))

    return slots, docs, covered, to_production, rows, universe


# ----------------------------------------------------------------- output ---
def report_docs(docs, covered, to_production, only=None):
    print('=' * 92)
    print('PER-DOC RECONCILIATION    header block (intent) vs. coverage table (verdict)')
    print('=' * 92)
    for fn, d in docs.items():
        if only and only not in fn:
            continue
        intent = set()
        for l in d['header']:
            base = l.split('/')[1]
            intent.add(d['renames'].get(base) or to_production(base))
        got = set(d['verdicts']) | ({d['subject']} if d['subject'] in covered else set())
        missing = sorted(intent - got)
        orphans = [n for n in missing if not covered.get(n)]
        flag = '   <-- INTENT NEVER GIVEN A VERDICT ANYWHERE' if orphans else ''
        print('\n%-42s status=%-9s intent=%-3d verdicts=%-3d%s'
              % (fn, d['status'], len(intent), len(d['verdicts']), flag))
        for n in missing:
            elsewhere = [x[0] for x in covered.get(n, []) if x[0] != fn]
            print('      %-46s %s' % (n, 'covered by ' + elsewhere[0] if elsewhere else '*** NO VERDICT ANYWHERE ***'))
        for n in sorted(set(d['verdicts']) - intent):
            print('      %-46s (verdict, not in this header block)' % n)

    print('\n--- header listings that are not on disk ---')
    seen = set()
    for fn, d in docs.items():
        for l in d['header']:
            if l in seen:
                continue
            seen.add(l)
            if not os.path.exists(os.path.join(ASM_ROOT, l + '.asm')):
                print('   %-52s (%s)' % (l + '.asm', fn))


def report_markers(rows):
    print('\n' + '=' * 92)
    print('COVERAGE OF EVERY // WZD MARKER IN THE COMBAT SOURCES')
    print('=' * 92)
    for state in ('NO NAME', 'WALKED', 'CLAIMED', 'NOT DONE'):
        sel = [r for r in rows if r[4] == state]
        if not sel:
            continue
        print('\n--- %s (%d) ---' % (state, len(sel)))
        for _key, raw, name, impl, _st, note, _src, _line in sel:
            print('  %-10s %-44s %-8s %s' % (raw, name or '', 'impl' if impl else 'notimpl', note))
    counts = dict((s, len([r for r in rows if r[4] == s])) for s in ('DONE', 'NOT DONE', 'NO NAME'))
    print('\ntotals: %d overlay slots | DONE %d | NOT DONE %d | unnamed %d'
          % (len(rows), counts['DONE'], counts['NOT DONE'], counts['NO NAME']))


def emit_homeless(rows, covered):
    """the Combat-Homeless.md list body, grouped by overlay

    Homeless means no review owns it.  A function whose callers were adjudicated by some review has
    a home -- it is that review's backlog, not a loose end -- so attribution runs first and only
    what it cannot place lands here.  Use --attribution-md for the ones that do have a home.
    """
    _groups, homeless = attribution_groups(rows, covered)
    homeless = set(homeless)
    out, last = [], None
    for (ovr, _part), raw, name, impl, state, _note, _src, _line in rows:
        if state not in ('NOT DONE', 'CLAIMED') or name not in homeless:
            continue
        if ovr != last:
            if last is not None:
                out.append('')
            out.append('WIZARDS.EXE  ovr%03d' % ovr)
            last = ovr
        out.append('// WZD ' + raw)
        out.append('%s()%s' % (name, '' if impl else '    // no production body \u2014 comment-only placeholder'))
    return '\n'.join(out)


ANCHOR_RE = re.compile(r'\]\(([^)#\s]+)#L(\d+)(?:-L(\d+))?\)')


def report_anchors():
    """line anchors in the review docs that point past the end of the file they name

    Production line numbers drift as the sources are edited, and a dead anchor is worse than
    none -- it still has to be hunted for by hand.  Only out-of-range anchors are detectable
    without content assertions, so this is a floor on the drift, not a measure of it.
    """
    print('=' * 92)
    print('DEAD LINE ANCHORS   #L targets past the end of the file')
    print('=' * 92)
    lengths, bad = {}, 0
    for fn in sorted(os.listdir(DOC_DIR)):
        if not fn.endswith('.md'):
            continue
        path = os.path.join(DOC_DIR, fn)
        for m in ANCHOR_RE.finditer(read(path)):
            target = os.path.normpath(os.path.join(DOC_DIR, m.group(1)))
            if not os.path.exists(target):
                # some docs were written with repo-root-relative links, which do not resolve
                # from doc/@CodeReview/ -- a different defect from a target that is simply gone
                root = os.path.normpath(m.group(1))
                kind = 'root-relative link' if os.path.exists(root) else 'missing file'
                print('   %-42s %-20s %s' % (fn, kind, m.group(1)))
                bad += 1
                continue
            if target not in lengths:
                lengths[target] = len(read(target).splitlines())
            for v in (m.group(2), m.group(3)):
                if v and int(v) > lengths[target]:
                    print('   %-42s %s#L%s  (file has %d lines)'
                          % (fn, os.path.basename(target), v, lengths[target]))
                    bad += 1
    print()
    print('%d dead anchors' % bad)


# ------------------------------------------------------- call attribution ---
CALL_SOURCES = SOURCES + ['CMBTTSTU.c']
TOPLEVEL_DEF_RE = re.compile(r'^(?:static\s+)?[A-Za-z_][A-Za-z_0-9]*[\s*]+\**([A-Za-z_][A-Za-z_0-9]*)\s*\([^;]*$')


def definition_index():
    """file -> sorted [(line, name)] of top-level function definitions"""
    idx = {}
    for rel in CALL_SOURCES:
        path = os.path.join(SRC_DIR, rel)
        if not os.path.exists(path):
            continue
        defs = []
        for i, raw in enumerate(read(path).split(chr(10))):
            if raw[:1] in (' ', chr(9), '', '/', '*', '#', '}'):
                continue
            m = TOPLEVEL_DEF_RE.match(raw.rstrip())
            if m and not raw.rstrip().endswith(';'):
                defs.append((i + 1, m.group(1)))
        idx[rel] = defs
    return idx


def callers_of(name, idx):
    """the enclosing function of every call site of `name`, as {(file, caller)}

    Answers "which review should this have been part of": a function belongs with whatever
    review covered the code that calls it.
    """
    pat = re.compile(r'(?<![A-Za-z_0-9])%s\s*\(' % re.escape(name))
    out = set()
    for rel, defs in idx.items():
        lines = read(os.path.join(SRC_DIR, rel)).split(chr(10))
        for i, raw in enumerate(lines):
            s = raw.strip()
            if s.startswith(('//', '*', '/*')) or not pat.search(raw):
                continue
            if TOPLEVEL_DEF_RE.match(raw.rstrip()) and raw[:1] not in (' ', chr(9)):
                continue                                   # the definition itself
            enclosing = None
            for line, nm in defs:
                if line > i + 1:
                    break
                enclosing = nm
            if enclosing and enclosing != name:
                out.add((rel, enclosing))
    return out


def attribution_groups(rows, covered):
    """review doc -> [(marker, name, callers)] for every uncovered function it should have held

    Ties (a function called from two reviewed areas) go to the review with more call sites, then
    alphabetically -- without the second key the grouping is not reproducible run to run.
    """
    idx = definition_index()
    groups, homeless = {}, []
    for _key, raw, name, _impl, state, _note, _src, _line in rows:
        if state not in ('NOT DONE', 'CLAIMED') or not name:
            continue
        homes = {}
        seen_callers = set()
        for _rel, caller in callers_of(name, idx):
            if caller in seen_callers:
                continue                                   # one caller, one vote
            for fn, _verdict in covered.get(caller, []):
                homes.setdefault(fn, set()).add(caller)
                seen_callers.add(caller)
                break                                      # a caller adjudicated twice is still one caller
        if len(homes) >= HOMELESS_DOC_SPREAD:
            homeless.append(name)                          # too widely shared for any one review to own
            continue
        if not homes:
            homeless.append(name)
            continue
        fn, callset = sorted(homes.items(), key=lambda kv: (-len(kv[1]), kv[0]))[0]
        groups.setdefault(fn, []).append((raw, name, sorted(callset)))
    return groups, homeless


def report_attribution_md(rows, covered):
    """the attribution table as markdown, for pasting into Combat-Homeless.md"""
    groups, homeless = attribution_groups(rows, covered)
    out = ['| review it belongs to | n | functions |', '| --- | --- | --- |']
    for fn in sorted(groups, key=lambda k: (-len(groups[k]), k)):
        names = ', '.join('`%s`' % n for _r, n, _c in groups[fn])
        out.append('| [%s](%s) | %d | %s |' % (fn, fn, len(groups[fn]), names))
    out.append('')
    out.append('**%d have no covered caller at all** — their callers are themselves unreviewed, so no '
               'existing session claims them: %s.'
               % (len(homeless), ', '.join('`%s`' % n for n in homeless)))
    io.open(1, 'w', encoding='utf-8', closefd=False).write(chr(10).join(out) + chr(10))


def report_attribution(rows, covered):
    """for everything not done, name the review that covered its callers"""
    idx = definition_index()
    print('=' * 92)
    print('ATTRIBUTION   which review each uncovered function belongs to, by its call sites')
    print('=' * 92)
    homeless = 0
    for _key, raw, name, _impl, state, _note, _src, _line in rows:
        if state not in ('NOT DONE', 'CLAIMED') or not name:
            continue
        homes = {}
        for rel, caller in callers_of(name, idx):
            for fn, _verdict in covered.get(caller, []):
                homes.setdefault(fn, set()).add(caller)
        if not homes:
            homeless += 1
            continue
        best = sorted(homes.items(), key=lambda kv: (-len(kv[1]), kv[0]))   # deterministic tie-break
        print()
        print('  %-10s %s' % (raw, name))
        for fn, callset in best:
            print('        belongs with %-42s called from %s' % (fn, ', '.join(sorted(callset))))
    print()
    print('%d uncovered functions have no covered caller -- no review claims them yet' % homeless)


# ------------------------------------------------------- referral checking ---
REFERRAL_RE = re.compile(r'\]\((Combat-[A-Za-z_0-9]+\.md)\)')


def report_referrals(docs, covered, universe):
    """assertions of the form "X is covered in <other doc>" that the other doc does not support

    A cross-doc referral is a coverage claim like any other, and nothing was checking them --
    which is how a doc came to send readers to a review that never mentions the function.
    """
    print('=' * 92)
    print('CROSS-DOC REFERRALS   "X is covered in <doc>" where <doc> renders no verdict on X')
    print('=' * 92)
    bad = 0
    for fn, d in docs.items():
        for raw in d['text'].split(chr(10)):
            if 'cover' not in raw.lower():
                continue
            targets = REFERRAL_RE.findall(raw)
            if not targets:
                continue
            for name in re.findall(r'`([A-Za-z_][A-Za-z_0-9]*)`', raw):
                if name not in universe:
                    continue                               # `faithful`, `ovr099` and friends
                where = [x[0] for x in covered.get(name, [])]
                if any(name in docs.get(t, {}).get('verdicts', {}) for t in targets):
                    continue
                if any(t in where for t in targets):
                    continue                               # a second link on the line is the right one
                print('   %-40s %-38s claimed in %-38s %s'
                      % (fn, name, ', '.join(targets), 'actually ' + where[0] if where else 'NO VERDICT ANYWHERE'))
                bad += 1
    print()
    print('%d unsupported referrals' % bad)



# ----------------------------------------------------------------- debt ---
# Fidelity is one axis; house style is another.  These are the tells that a function was walked
# against its listing but never cleaned up afterwards.  All deterministic text scans, no judgement.
DEBT_DEF_RE = re.compile(
    r'^(?:static\s+)?(?:void|int8_t|int16_t|int32_t|uint8_t|uint16_t|uint32_t'
    r'|char|long|unsigned|size_t|FILE|SAMB_ptr|struct\s+\w+\s*\*?|[A-Za-z_]\w*_ptr)\s+\*?'
    r'\s*([A-Za-z_]\w*)\s*\(')
DEBT_DECL_RE = re.compile(
    r'^\s+(?:static\s+)?(?:void|int8_t|int16_t|int32_t|uint8_t|uint16_t|uint32_t'
    r'|char|long|unsigned|size_t|FILE|SAMB_ptr|struct\s+\w+\s*\*?|[A-Za-z_]\w*_ptr)\s+\*?'
    r'\s*([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?\s*(?:=|;)')
DEBT_RAW_BUG_RE = re.compile(r'(?:^|[^A-Za-z_])(?:BUG|TODO|FIXME|XXX)\b')
DEBT_OGBUG_RE = re.compile(r'OGBUG')
DEBT_CMT_CODE_RE = re.compile(r'^\s*//\s*[A-Za-z_][\w\[\]\.\->]*\s*(?:\(|=|\+=|-=)')
# `NOTE(who,when): prose` and friends are annotation conventions, not commented-out code -- but
# they are an identifier followed by '(', so DEBT_CMT_CODE_RE scores them as debt.
DEBT_CMT_ANNOT_RE = re.compile(r'^\s*//\s*(?:NOTE|TODO|DEDU|OGBUG|BUG|FIXME|XXX|HACK|CLAUDE|GEMINI|WZD)\b',
                               re.I)

# Parameters live on the column-0 signature line, which DEBT_DECL_RE can never match:
# that pattern anchors on ^\s+ (body indentation) and requires the line to end in '=' or ';'.
# Scanning only bodies left every parameter in the tree unchecked, which is how Max_X / Max_Y
# survived a full review pass.  DEBT_PARAM_TYPE mirrors the type alternation used above.
DEBT_PARAM_TYPE = (r'(?:const\s+)?(?:void|int8_t|int16_t|int32_t|uint8_t|uint16_t|uint32_t'
                   r'|char|long|short|unsigned|signed|size_t|FILE|SAMB_ptr'
                   r'|struct\s+\w+|[A-Za-z_]\w*_ptr|[A-Za-z_]\w*_t)')
DEBT_PARAM_RE = re.compile(DEBT_PARAM_TYPE + r'\s+\*?\s*([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?\s*$')
DEBT_SIG_RE = re.compile(r'^(?:static\s+)?' + DEBT_PARAM_TYPE + r'\s+\*?\s*[A-Za-z_]\w*\s*\(([^;]*)\)\s*$')
DEBT_GOOD_IDENT_RE = re.compile(r'^[a-z][a-z0-9_]*$')
DEBT_CATS = ('CLAUDE', 'BUG', 'loc', 'prm', 'DBG', 'cmt', 'blank')


def _debt_is_preserved_original(body, i):
    """True when a commented-out line is the OG form kept beside its live replacement.

    AGENTS.md REQUIRES that shape -- faithful code that must change is commented out with the
    replacement below it -- so counting it as debt makes `cmt` a number that can never reach zero,
    and a metric nobody can clear is a metric nobody reads.  The tell is a live statement within a
    couple of lines below, after any further comment lines in the same stack.

    Deliberately NOT keyed on the identifier matching: the platform substitutions rename as they
    replace (`gfopen` -> `stu_fopen_ci`, `gfread` -> `fread`), and an identifier test scores those
    as reviewable while passing the genuinely dead ones.  That mistake hid a corrupted half-typed
    comment inside Combat_Cache_Read for a month.
    """
    for j in range(i + 1, min(i + 4, len(body))):
        s = body[j].strip()
        if not s or s.startswith('//'):
            continue                      # a stack of drafts: keep looking for the live line
        if s.startswith(('/*', '*', '*/')):
            return False                  # a prose block, not a replacement
        return not s.startswith(('}', '{'))
    return False


def debt_bad_params(sig_line):
    """Parameter names on one signature line that are not lower_snake_case."""
    m = DEBT_SIG_RE.match(sig_line.rstrip())
    if not m:
        return []
    bad = []
    for raw in m.group(1).split(','):
        raw = raw.strip()
        if not raw or raw == 'void':
            continue
        pm = DEBT_PARAM_RE.match(raw)
        if pm and not DEBT_GOOD_IDENT_RE.match(pm.group(1)):
            bad.append(pm.group(1))
    return bad


def debt_functions(path):
    """(name, first_line, [body lines], joined signature) for every column-0 definition.

    Two things here are easy to get wrong and both produced silent, wrong answers:

    A definition's signature may span lines.  Requiring the opening line to end in ')' or '{'
    skips those functions entirely -- and worse, the PRECEDING function then swallows them,
    because the body end used to be found by scanning BACKWARD from the next recognised
    definition to the last '}'.  That overshoot is what made `Build_Battlefield` report a blank
    run that sits three functions further down the file.

    So: join continuation lines to recover the full signature, and find the body end by scanning
    FORWARD to the first column-0 '}' -- which is the real close of a column-0 definition and does
    not depend on correctly spotting whatever comes next.
    """
    lines = read(path).split('\n')
    starts = []
    for i, l in enumerate(lines):
        if l.rstrip().endswith(';'):
            continue
        sig, j = l.rstrip(), i
        while sig.count('(') > sig.count(')') and j + 1 < len(lines) and j - i < 20:
            j += 1
            sig += ' ' + lines[j].strip()
        sig = ' '.join(sig.split())
        m = DEBT_DEF_RE.match(sig)
        if m and (sig.endswith(')') or sig.endswith('{')):
            starts.append((i, m.group(1), sig))
    for n, (i, name, sig) in enumerate(starts):
        limit = starts[n + 1][0] if n + 1 < len(starts) else len(lines)
        j = i + 1
        while j < limit and lines[j].rstrip() != '}':
            j += 1
        yield name, i + 1, lines[i:min(j, limit - 1) + 1], sig


def scan_debt_body(body, sig=None):
    """Per-function tell counts.  A 2+ run of blank lines counts once, not once per line."""
    c = dict((k, 0) for k in DEBT_CATS)
    if sig is None:
        sig = body[0] if body else ''
    c['prm'] = len(debt_bad_params(sig))
    run = 0
    for i, l in enumerate(body):
        s = l.strip()
        if not s:
            run += 1
            if run == 2:
                c['blank'] += 1
            continue
        run = 0
        if 'CLAUDE' in l:
            c['CLAUDE'] += 1
        if DEBT_RAW_BUG_RE.search(l) and not DEBT_OGBUG_RE.search(l):
            c['BUG'] += 1
        if 'DBG_' in l:
            c['DBG'] += 1
        if (DEBT_CMT_CODE_RE.match(l)
                and not DEBT_CMT_ANNOT_RE.match(l)
                and not _debt_is_preserved_original(body, i)):
            c['cmt'] += 1
        m = DEBT_DECL_RE.match(l)
        if m and not s.startswith('//'):
            ident = m.group(1)
            if ident != ident.lower() and ident != ident.upper():
                c['loc'] += 1
    return c


# A header prototype and its definition are two hand-maintained copies of one signature, so a
# rename applied to the .c silently leaves the .h asserting the old name.  That is worse than a
# non-compliant name: the header actively contradicts the source.  Nothing checked it until
# 2026-08-27, by which point 22 parameters across 14 prototypes had drifted.
DEBT_HDR_PAIRS = [('Combat.h', ['Combat.c']),
                  ('CMBTAI.h', ['CMBTAI.c']),
                  ('CMBMAGIC.h', ['CMBMAGIC.c']),
                  ('COMBINIT.h', ['COMBINIT.c']),
                  ('CMBTMVPT.h', ['CMBTMVPT.c'])]
DEBT_PROTO_RE = re.compile(r'^(?:extern\s+)?(?:static\s+)?' + DEBT_PARAM_TYPE +
                           r'\s+\*?\s*([A-Za-z_]\w*)\s*\((.*)\)\s*;\s*$')


def _debt_param_names(sig):
    """Parameter identifiers on one signature, or None if any chunk will not parse."""
    out = []
    for raw in sig.split(','):
        s = ' '.join(raw.split())
        if not s or s == 'void':
            continue
        m = DEBT_PARAM_RE.match(s)
        if not m:
            return None
        out.append(m.group(1))
    return out


def scan_header_drift():
    """[(header, function, argno, header_name, source_name)] where the two copies disagree."""
    out = []
    for hdr, srcs in DEBT_HDR_PAIRS:
        hpath = os.path.join(SRC_DIR, hdr)
        if not os.path.exists(hpath):
            continue
        defs = {}
        for s in srcs:
            spath = os.path.join(SRC_DIR, s)
            if not os.path.exists(spath):
                continue
            for l in read(spath).split('\n'):
                m = DEBT_SIG_RE.match(l.rstrip())
                if m:
                    dm = DEBT_DEF_RE.match(l)
                    if dm:
                        defs.setdefault(dm.group(1), _debt_param_names(m.group(1)))
        for l in read(hpath).split('\n'):
            m = DEBT_PROTO_RE.match(l.strip())
            if not m:
                continue
            want = defs.get(m.group(1))
            have = _debt_param_names(m.group(2))
            if not want or not have or len(want) != len(have):
                continue
            for i, (h, s) in enumerate(zip(have, want)):
                if h != s:
                    out.append((hdr, m.group(1), i + 1, h, s))
    return out


def report_debt(rows, only_done=True):
    """Second-axis report: what a VERDICT never looked at."""
    state = dict((name, st) for (_k, _raw, name, _impl, st, _n, _s, _l) in rows if name)
    found = []
    for src in SOURCES:
        path = os.path.join(SRC_DIR, src)
        if not os.path.exists(path):
            continue
        for name, line, body, sig in debt_functions(path):
            c = scan_debt_body(body, sig)
            if not sum(c.values()):
                continue
            if only_done and state.get(name, '-') != 'DONE':
                continue
            found.append((sum(c.values()), src, name, line, c, debt_bad_params(sig)))
    found.sort(key=lambda r: -r[0])

    scope = 'DONE-marked' if only_done else 'all'
    print('--- style/annotation debt in %s combat functions: %d ---' % (scope, len(found)))
    print('  %-38s %-12s %7s %6s %4s %4s %4s %4s %4s %5s'
          % ('function', 'file', 'line', 'CLAUDE', 'BUG', 'loc', 'prm', 'DBG', 'cmt', 'blank'))
    for _tot, src, name, line, c, _bad in found:
        print('  %-38s %-12s %7d %6d %4d %4d %4d %4d %4d %5d'
              % (name[:38], src, line, c['CLAUDE'], c['BUG'], c['loc'], c['prm'],
                 c['DBG'], c['cmt'], c['blank']))
    agg = dict((k, sum(r[4][k] for r in found)) for k in DEBT_CATS)
    print()
    print('  totals: ' + ' | '.join('%s %d' % (k, agg[k]) for k in DEBT_CATS))
    claude = [r for r in found if r[4]['CLAUDE']]
    if claude:
        print()
        print('  /* CLAUDE */ edits to original game code sit in %d function(s):' % len(claude))
        for _tot, src, name, line, c, _bad in claude:
            print('    %-38s %-12s line %-6d %d marker(s)' % (name[:38], src, line, c['CLAUDE']))
    print()
    prm = [r for r in found if r[4]['prm']]
    if prm:
        print()
        print('  non-lower_snake_case PARAMETERS sit in %d function(s):' % len(prm))
        for _tot, src, name, line, c, bad in prm:
            print('    %-38s %-12s line %-6d %s'
                  % (name[:38], src, line, ', '.join(bad)))
    drift = scan_header_drift()
    if drift:
        print()
        print('  HEADER DRIFT -- %d prototype parameter(s) disagree with the .c definition:' % len(drift))
        for hdr, fn, argno, h, s in drift:
            print('    %-12s %-42s arg%-2d .h=%-24s .c=%s' % (hdr, fn[:42], argno, h, s))
        print('    The header is the copy that is wrong; sync it to the definition.')
    print()
    print('  CLAUDE  edits to original game code      BUG    raw ; BUG:/TODO not in OGBUG form')
    print('  loc     local not lower_snake_case       prm    PARAMETER not lower_snake_case')
    print('  DBG     DBG_ leftovers                   cmt    commented-out code')
    print('  blank   2+ consecutive blank lines')


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--doc', help='report on one doc only')
    ap.add_argument('--csv', help='write the per-marker matrix here')
    ap.add_argument('--homeless', '--misc', dest='homeless', action='store_true',
                    help="emit the Combat-Homeless.md list body (--misc kept as an alias)")
    ap.add_argument('--attribution', action='store_true', help='name the review each uncovered function belongs to')
    ap.add_argument('--attribution-md', action='store_true', dest='attribution_md', help='attribution table as markdown')
    ap.add_argument('--referrals', action='store_true', help='check cross-doc "covered in X" claims')
    ap.add_argument('--anchors', action='store_true', help='report dead #L line anchors and exit')
    ap.add_argument('--debt', action='store_true', help='style/annotation debt per function (the second axis)')
    ap.add_argument('--debt-all', action='store_true', dest='debt_all', help='--debt over every function, not just DONE')
    ap.add_argument('--quiet-docs', action='store_true', help='skip the per-doc reconciliation')
    args = ap.parse_args()

    if args.anchors:
        report_anchors()
        return

    _slots, docs, covered, to_production, rows, universe = build()

    if args.debt or args.debt_all:
        report_debt(rows, only_done=not args.debt_all)
        return

    if args.referrals:
        report_referrals(docs, covered, universe)
        return

    if args.attribution_md:
        report_attribution_md(rows, covered)
        return

    if args.attribution:
        report_attribution(rows, covered)
        return

    if args.homeless:
        out = emit_homeless(rows, covered)
        io.open(1, 'w', encoding='utf-8', closefd=False).write(out + '\n')
        return

    if not args.quiet_docs:
        report_docs(docs, covered, to_production, args.doc)
    report_markers(rows)

    if args.csv:
        with io.open(args.csv, 'w', encoding='utf-8') as f:
            f.write('overlay,part,marker,name,body,state,note,file,line\n')
            for (ovr, part), raw, name, impl, state, note, src, line in rows:
                f.write('%d,%d,%s,%s,%s,%s,"%s",%s,%s\n'
                        % (ovr, part, raw, name or '', 'impl' if impl else 'notimpl', state, note, src, line))
        print('wrote %s' % args.csv)


if __name__ == '__main__':
    main()
