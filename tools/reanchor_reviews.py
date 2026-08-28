#!/usr/bin/env python3
"""Re-resolve `#L` line anchors in the CodeReview docs against the current sources.

WHY THIS EXISTS

`review_coverage.py --anchors` only catches anchors past the end of a file.  An anchor that
drifted but still lands *inside* the file is invisible to it, and that is the larger and more
dangerous population: a link that resolves to the wrong line still reads as authoritative, so it
is worth less than a link that is obviously broken.  Five such anchors were found by hand on
2026-08-27, all in docs marked DONE-DONE.

WHAT IT CAN AND CANNOT PROVE

An anchor is only re-resolvable when the doc itself says what it is pointing at.  Two shapes do:

    | `Function_Name` | [File.c:123](../../MoM/src/File.c#L123) | ...      coverage-table row
    ## `Function_Name` ([File.c:123](../../MoM/src/File.c#L123), asm 45)   walk heading

In both, the backticked identifier names the function whose *definition* the anchor should point
at, so the correct line can be recovered from the source and the anchor rewritten.

A third shape is even stronger, because the link text names its own target and needs no context:

    [Function_Name](../../MoM/src/File.c#L123)

Every other anchor points into the middle of a function -- a specific statement, a guard, a range
of a few lines -- and nothing in the doc says which one.  Those are NOT rewritten.  Guessing would
turn a wrong-but-checkable link into a wrong-and-plausible one, which is strictly worse.

THE AUDIT, AND WHY IT DOES NOT AUTO-FIX

`` `Ident` ([File.c:123](...)) `` -- identifier immediately before its own anchor -- looks like it
should be resolvable and is not.  The same shape carries at least three different meanings:

    **`Fn`** ([File.c:9826](...), 445) - the largest...     the DEFINITION
    `Animate_Bolt` has a second caller in `Fn` ([File.c:271](...))   a CALL SITE inside Fn
    per the `switch` in `Fn` ([File.c:515](...))            a STATEMENT inside Fn

and on a line naming two functions the regex pairs them off by position, which can mis-attribute.
So this shape is only AUDITED: when the target line does not mention the identifier within a couple
of lines, the anchor has drifted -- true whichever meaning was intended -- and it is reported with
the function's real definition line as a hint.  Resolving it needs a human reading the sentence.

Usage:
  python3 tools/reanchor_reviews.py                 report + audit, change nothing
  python3 tools/reanchor_reviews.py --apply         rewrite the provable ones
  python3 tools/reanchor_reviews.py --doc NAME      restrict to one doc
  python3 tools/reanchor_reviews.py --prefix P      doc prefix (default Combat-)
"""
import argparse
import io
import os
import re

DOC_DIR = 'doc/@CodeReview'
SRC_DIR = 'MoM/src'
ALT_DIRS = ['MoM/src', 'MoX/src', 'STU/src']

# `[File.c:123](../../MoM/src/File.c#L123)` / `...#L123-L130`
LINK_RE = re.compile(r'\[([A-Za-z_0-9]+\.[ch]):(\d+)(?:-(\d+))?\]\((\.\./\.\./[A-Za-z_0-9/]+/([A-Za-z_0-9]+\.[ch]))#L(\d+)(?:-L(\d+))?\)')
# `[Function_Name](../../MoM/src/File.c#L123)` -- the link TEXT names the function, which is a
# stronger signal than the row/heading heuristic below and needs no surrounding context at all.
# Missing this shape left seven anchors pointing 4,800 lines past the end of Combat.c.
NAMED_RE = re.compile(r'\[`?([A-Za-z_]\w*)`?\]\((\.\./\.\./[A-Za-z_0-9/]+/([A-Za-z_0-9]+\.[ch]))#L(\d+)(?:-L(\d+))?\)')
ROW_RE = re.compile(r'^\|\s*`([A-Za-z_][A-Za-z_0-9]*)`\s*\|')
HEAD_RE = re.compile(r'^#{2,4}\s+`?([A-Za-z_][A-Za-z_0-9]*)`?')
DEF_RE = re.compile(
    r'^(?:static\s+)?(?:const\s+)?(?:void|int8_t|int16_t|int32_t|int64_t|uint8_t|uint16_t'
    r'|uint32_t|uint64_t|char|long|short|unsigned|signed|size_t|FILE|SAMB_ptr'
    r'|struct\s+\w+|[A-Za-z_]\w*_ptr|[A-Za-z_]\w*_t)\s+\*?\s*([A-Za-z_]\w*)\s*\(')

_src_cache = {}


def source_lines(fname):
    if fname in _src_cache:
        return _src_cache[fname]
    lines = None
    for d in ALT_DIRS:
        p = os.path.join(d, fname)
        if os.path.exists(p):
            lines = io.open(p, encoding='latin-1').read().split('\n')
            break
    _src_cache[fname] = lines
    return lines


_def_cache = {}


def definitions(fname):
    """name -> 1-based definition line, for column-0 definitions (multi-line signatures included)."""
    if fname in _def_cache:
        return _def_cache[fname]
    lines = source_lines(fname)
    out = {}
    if lines:
        for i, l in enumerate(lines):
            if not l or l[0] in ' \t' or l.rstrip().endswith(';'):
                continue
            sig, j = l.rstrip(), i
            while sig.count('(') > sig.count(')') and j + 1 < len(lines) and j - i < 20:
                j += 1
                sig += ' ' + lines[j].strip()
            m = DEF_RE.match(' '.join(sig.split()))
            if m:
                out.setdefault(m.group(1), i + 1)
    _def_cache[fname] = out
    return out


# `` `Ident` ([File.c:123](...)) `` -- the identifier sits immediately before its own anchor.
# This shape is AMBIGUOUS: it means "defined here" in a walk heading and "called here" in a
# call-site list, and nothing distinguishes them, so it is never rewritten.  It can still be
# AUDITED: if the target line does not mention the identifier at all, the anchor has drifted --
# true whichever meaning was intended.
ADJACENT_RE = re.compile(r'`([A-Za-z_]\w*)`\*{0,2}\s*\(\[([A-Za-z_0-9]+\.[ch]):(\d+)')
AUDIT_WINDOW = 2


def audit_adjacent(path):
    """[(doc, docline, ident, file, line, what_is_there)] for anchors that provably drifted."""
    out = []
    for n, line in enumerate(io.open(path, encoding='utf-8').read().split('\n')):
        for m in ADJACENT_RE.finditer(line):
            ident, tfile, tline = m.group(1), m.group(2), int(m.group(3))
            src = source_lines(tfile)
            if src is None:
                continue
            # Only identifiers that are actually functions in that file.  Without this the
            # pattern also matches backticked keywords and labels (`static`, `while`,
            # `loc_8E65B`) that happen to precede an unrelated anchor -- noise, not drift.
            defline = definitions(tfile).get(ident)
            if not defline:
                continue
            lo = max(0, tline - 1 - AUDIT_WINDOW)
            hi = min(len(src), tline + AUDIT_WINDOW)
            if any(ident in s for s in src[lo:hi]):
                continue
            here = src[tline - 1].strip() if tline <= len(src) else '<past EOF>'
            out.append((os.path.basename(path), n + 1, ident, tfile, tline, defline, here[:52]))
    return out


def owning_identifier(line):
    m = ROW_RE.match(line)
    if m:
        return m.group(1)
    m = HEAD_RE.match(line)
    if m:
        return m.group(1)
    return None


def process(path, apply_changes):
    lines = io.open(path, encoding='utf-8').read().split('\n')
    fixed, stale, dead = [], [], []
    for n, line in enumerate(lines):
        ident = owning_identifier(line)
        new_line = line

        # `[Function_Name](file#L123)` -- the text names its own target, no context needed.
        for m in NAMED_RE.finditer(line):
            name, tfile, tline = m.group(1), m.group(3), int(m.group(4))
            if source_lines(tfile) is None:
                continue
            want = definitions(tfile).get(name)
            if not want or want == tline:
                continue
            span = (int(m.group(5)) - tline) if m.group(5) else None
            anc = '%s#L%d' % (m.group(2), want)
            if span is not None:
                anc += '-L%d' % (want + span)
            new_line = new_line.replace(m.group(0), '[%s](%s)' % (m.group(1), anc), 1)
            fixed.append((os.path.basename(path), n + 1, name, tfile, tline, want))

        for m in LINK_RE.finditer(line):
            tfile, tline = m.group(5), int(m.group(6))
            src = source_lines(tfile)
            if src is None:
                continue
            if tline > len(src):
                dead.append((os.path.basename(path), n + 1, tfile, tline, len(src)))
            if not ident:
                continue
            want = definitions(tfile).get(ident)
            if not want or want == tline:
                continue
            old = m.group(0)
            end_txt, end_anc = m.group(3), m.group(7)
            span = (int(end_anc) - tline) if end_anc else None
            txt = '%s:%d' % (m.group(1), want)
            if end_txt:
                txt += '-%d' % (want + (int(end_txt) - int(m.group(2))))
            anc = '%s#L%d' % (m.group(4), want)
            if span is not None:
                anc += '-L%d' % (want + span)
            new_line = new_line.replace(old, '[%s](%s)' % (txt, anc), 1)
            fixed.append((os.path.basename(path), n + 1, ident, tfile, tline, want))
        if new_line != line:
            lines[n] = new_line
    if apply_changes and fixed:
        io.open(path, 'w', encoding='utf-8').write('\n'.join(lines))
    return fixed, stale, dead


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--apply', action='store_true', help='rewrite the provable anchors')
    ap.add_argument('--doc', help='restrict to one doc (basename, with or without .md)')
    ap.add_argument('--prefix', default='Combat-', help='doc filename prefix (default Combat-)')
    args = ap.parse_args()

    names = sorted(f for f in os.listdir(DOC_DIR)
                   if f.startswith(args.prefix) and f.endswith('.md'))
    if args.doc:
        want = args.doc if args.doc.endswith('.md') else args.doc + '.md'
        names = [f for f in names if f == want]

    all_fixed, all_dead, all_audit = [], [], []
    for f in names:
        fixed, _stale, dead = process(os.path.join(DOC_DIR, f), args.apply)
        all_fixed += fixed
        all_dead += dead
        all_audit += audit_adjacent(os.path.join(DOC_DIR, f))

    verb = 'rewritten' if args.apply else 'resolvable (not written -- pass --apply)'
    print('--- %d anchor(s) %s ---' % (len(all_fixed), verb))
    for doc, dn, ident, tfile, was, now in all_fixed:
        print('  %-42s line %-5d %-38s %s  L%d -> L%d' % (doc, dn, ident, tfile, was, now))

    print()
    print('--- %d anchor(s) past end of file ---' % len(all_dead))
    for doc, dn, tfile, tline, total in all_dead:
        print('  %-42s line %-5d %s#L%d  (file has %d lines)' % (doc, dn, tfile, tline, total))
    if all_dead:
        print('  These point into the middle of a function; the doc does not say which line,')
        print('  so they cannot be resolved automatically.  Fix by hand.')

    print()
    print('--- %d `Ident` ([File:N]) anchor(s) whose target does not mention Ident ---'
          % len(all_audit))
    for doc, dn, ident, tfile, tline, defline, here in all_audit:
        print("  %-40s line %-5d %-32s -> %s:%d is \"%s\"  (def L%d)" % (doc, dn, ident, tfile, tline, here, defline))
    if all_audit:
        print('  Drifted for certain.  NOT auto-fixed: this shape means "defined here" in a walk')
        print('  heading and "called here" in a call-site list, and nothing tells them apart.')


main()
