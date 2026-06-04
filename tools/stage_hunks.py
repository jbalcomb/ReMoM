# -*- coding: latin-1 -*-
#
# stage_hunks.py -- stage selected unstaged git hunks into the index.
#
# Two selection modes:
#
#   --rename OLD NEW
#       Pure token rename only. A hunk qualifies when every '-'/'+' line pairs
#       up such that the two lines are identical AFTER collapsing OLD and NEW
#       (any case) to one sentinel -- i.e. the line changed and the ONLY change
#       was an OLD->NEW token. Mixed hunks are reported, not staged.
#
#   --introduces SYM
#       Hunk where at least one '+' line contains SYM and no '-' line does.
#       Catches both renames-to-SYM AND new-code hunks that add SYM. Coarser.
#
# Works under Python 2.7 and 3.x; byte-faithful via latin-1.
#
# Dry run by default; pass --apply to `git apply --cached`. Limit scope with
# trailing `-- <paths>`. Review with `git diff --cached`; commit yourself.

import re
import sys
import subprocess


def emit(text):
    buf = getattr(sys.stdout, 'buffer', sys.stdout)
    buf.write(text.encode('latin-1'))
    buf.flush()


def run(args, stdin_bytes=None):
    p = subprocess.Popen(args, stdin=subprocess.PIPE if stdin_bytes is not None else None,
                         stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = p.communicate(stdin_bytes)
    return p.returncode, out, err


def split_files(diff_text):
    lines = diff_text.split(u'\n')
    i, n = 0, len(lines)
    while i < n:
        if not lines[i].startswith(u'diff --git'):
            i += 1
            continue
        start = i
        i += 1
        while i < n and not lines[i].startswith(u'@@') and not lines[i].startswith(u'diff --git'):
            i += 1
        header = lines[start:i]
        hunks = []
        while i < n and lines[i].startswith(u'@@'):
            h_start = i
            i += 1
            while i < n and not lines[i].startswith(u'@@') and not lines[i].startswith(u'diff --git'):
                i += 1
            hunks.append(lines[h_start:i])
        yield header, hunks


def is_content_diff(header):
    for h in header:
        if h.startswith(u'new file') or h.startswith(u'deleted file'):
            return False
        if h.startswith(u'Binary files') or h.startswith(u'GIT binary patch'):
            return False
        if h.startswith(u'rename from') or h.startswith(u'rename to'):
            return False
    return True


def make_rename_matcher(old, new):
    token = re.compile(u'(?i)(?:%s|%s)' % (re.escape(old), re.escape(new)))
    old_re = re.compile(u'(?i)%s' % re.escape(old))

    def normalize(content):
        return token.sub(u'\x00', content)

    def is_match(body):
        minus, plus = [], []
        for ln in body:
            if not ln:
                continue
            c = ln[0]
            if c == u'-':
                minus.append(ln[1:])
            elif c == u'+':
                plus.append(ln[1:])
            elif c == u'\\':
                continue
        if not minus or len(minus) != len(plus):
            return False
        old_before = old_after = 0
        for m, p in zip(minus, plus):
            if m == p or normalize(m) != normalize(p):
                return False
            old_before += len(old_re.findall(m))
            old_after += len(old_re.findall(p))
        return old_before > old_after

    def touches(body):
        return any(token.search(l[1:]) for l in body if l[:1] in (u'+', u'-'))

    return is_match, touches


def make_introduces_matcher(sym):
    sym_re = re.compile(re.escape(sym))

    def is_match(body):
        plus_has = any(sym_re.search(l[1:]) for l in body if l[:1] == u'+')
        minus_has = any(sym_re.search(l[1:]) for l in body if l[:1] == u'-')
        return plus_has and not minus_has

    def touches(body):
        return any(sym_re.search(l[1:]) for l in body if l[:1] in (u'+', u'-'))

    return is_match, touches


def main():
    argv = sys.argv[1:]
    apply = '--apply' in argv
    argv = [a for a in argv if a != '--apply']

    # Diff context. Default 0 so an isolated rename does not get merged into a
    # neighbouring non-rename hunk (e.g. a comment rename glued to a block of
    # "= 0" initializer edits). Override with -U N / --context N.
    context = 0
    for flag in ('-U', '--context'):
        if flag in argv:
            i = argv.index(flag)
            context = int(argv[i + 1])
            del argv[i:i + 2]

    paths = []
    if '--' in argv:
        idx = argv.index('--')
        paths = argv[idx + 1:]
        argv = argv[:idx]

    if argv[:1] == ['--rename'] and len(argv) >= 3:
        is_match, touches = make_rename_matcher(argv[1], argv[2])
        label = 'rename %s -> %s' % (argv[1], argv[2])
    elif argv[:1] == ['--introduces'] and len(argv) >= 2:
        is_match, touches = make_introduces_matcher(argv[1])
        label = 'introduces %s' % argv[1]
    else:
        sys.stderr.write('usage: stage_hunks.py (--rename OLD NEW | --introduces SYM) [--apply] [-- paths...]\n')
        return 2

    cmd = ['git', '-c', 'core.quotepath=false', 'diff', '--no-color', '-U%d' % context]
    if paths:
        cmd += ['--'] + paths
    rc, raw, err = run(cmd)
    if rc != 0:
        sys.stderr.write(err.decode('latin-1'))
        return rc

    diff_text = raw.decode('latin-1')
    kept_patch = []
    kept_count = 0
    skipped = []

    for header, hunks in split_files(diff_text):
        path = header[0].split(u' b/', 1)[-1] if u' b/' in header[0] else header[0]
        if not is_content_diff(header):
            continue
        kept_hunks = []
        for hunk in hunks:
            body = hunk[1:]
            if is_match(body):
                kept_hunks.append(hunk)
            elif touches(body):
                skipped.append((path, hunk[0]))
        if kept_hunks:
            kept_count += len(kept_hunks)
            kept_patch.extend(header)
            for h in kept_hunks:
                kept_patch.extend(h)

    if not kept_patch:
        print('No matching hunks for: %s' % label)
    else:
        patch_text = u'\n'.join(kept_patch)
        if not patch_text.endswith(u'\n'):
            patch_text += u'\n'
        if apply:
            apply_cmd = ['git', 'apply', '--cached', '--recount', '--whitespace=nowarn']
            if context == 0:
                apply_cmd.append('--unidiff-zero')  # git apply needs this to accept -U0 hunks
            rc, out, err = run(apply_cmd, patch_text.encode('latin-1'))
            if rc != 0:
                sys.stderr.write('git apply failed:\n' + err.decode('latin-1'))
                return rc
            print('Staged %d hunk(s) [%s].' % (kept_count, label))
            print('Review with:  git diff --cached')
        else:
            emit(patch_text)
            print('\n--- DRY RUN [%s]: %d hunk(s) would be staged. Re-run with --apply. ---' % (label, kept_count))

    if skipped:
        print('\n%d hunk(s) touch the token but did NOT match -> left for manual review:' % len(skipped))
        for path, atat in skipped:
            print('  %s  %s' % (path, atat.strip()))

    return 0


if __name__ == '__main__':
    sys.exit(main())
