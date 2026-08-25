#!/usr/bin/env python3
"""Reconcile CodeReview doc coverage against the combat sources and the disassembly.

Four independent inventories are compared:

  MARKERS   every `// WZD <overlay><part>` marker in the combat sources -> the name that follows
  INTENT    the `...\\WIZARDS\\ovrNNN\\Name.asm` paths in a review doc's header block, bridged to
            production names through the doc's rename ledger and the coverage tables
  VERDICT   the rows of a review doc's coverage table (`## Scope` / `## Review status` / the
            unnamed lead table), each of which renders a fidelity result on one function
  TRACKER   the checked boxes in doc/#TODO/stub_wip_todo.md

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
  python3 tools/review_coverage.py --csv OUT.csv       per-marker matrix
"""
import argparse
import io
import os
import re

SRC_DIR = 'MoM/src'
SOURCES = ['Combat.c', 'COMBINIT.c', 'CMBMAGIC.c', 'CMBTAI.c', 'CMBTMVPT.c']
HEADERS = ['Combat.h']
DOC_DIR = 'doc/#CodeReview'
DOC_PREFIX = 'Combat-'
SKIP_DOCS = {'Combat-Homeless.md'}
TRACKER = 'doc/#TODO/stub_wip_todo.md'
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
                # from doc/#CodeReview/ -- a different defect from a target that is simply gone
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
    ap.add_argument('--quiet-docs', action='store_true', help='skip the per-doc reconciliation')
    args = ap.parse_args()

    if args.anchors:
        report_anchors()
        return

    _slots, docs, covered, to_production, rows, universe = build()

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
