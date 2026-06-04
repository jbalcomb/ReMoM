# -*- coding: latin-1 -*-
#
# stage_tile_to_square_renames.py
#
# Stage ONLY the unstaged git hunks that are pure "tile" -> "square" renames.
#
# A hunk qualifies when every changed ('-'/'+') line pairs up such that the two
# lines are byte-for-byte identical AFTER collapsing every tile/tiles/square/
# squares token (any case) to a single sentinel -- i.e. the line changed, and
# the ONLY thing that changed was a tile<->square token. Hunks that mix a rename
# with any other edit are left alone and reported for manual review.
#
# Works under Python 2.7 and 3.x. Byte-faithful via latin-1 round-tripping.
#
# Usage:
#   python tools/stage_tile_to_square_renames.py            # dry run (default)
#   python tools/stage_tile_to_square_renames.py --apply    # git apply --cached the rename hunks
#   python tools/stage_tile_to_square_renames.py --apply -- MoM/src/special.c   # limit to paths
#
# After --apply, review with `git diff --cached` and commit yourself.

import re
import sys
import subprocess

TOKEN_RE = re.compile(r'(?i)(?:tiles|tile|squares|square)')
TILEISH_RE = re.compile(r'(?i)tiles?')


def emit(text):
    # Byte-faithful stdout under both Python 2 and 3.
    buf = getattr(sys.stdout, 'buffer', sys.stdout)
    buf.write(text.encode('latin-1'))
    buf.flush()


def run(args, stdin_bytes=None):
    p = subprocess.Popen(args, stdin=subprocess.PIPE if stdin_bytes is not None else None,
                         stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = p.communicate(stdin_bytes)
    return p.returncode, out, err


def normalize(content):
    # Collapse every tile/square token to one sentinel so a rename cancels out.
    return TOKEN_RE.sub(u'\x00', content)


def hunk_is_pure_rename(lines):
    # lines: the hunk body (everything after the @@ header line).
    minus, plus = [], []
    for ln in lines:
        if not ln:
            continue
        c = ln[0]
        if c == u'-':
            minus.append(ln[1:])
        elif c == u'+':
            plus.append(ln[1:])
        elif c == u'\\':
            continue  # "\ No newline at end of file"
        # ' ' context lines are ignored on purpose
    if not minus or len(minus) != len(plus):
        return False
    tileish_before = tileish_after = 0
    for m, p in zip(minus, plus):
        if m == p:
            return False                       # unchanged pair -> not a rename
        if normalize(m) != normalize(p):
            return False                       # something other than tokens changed
        tileish_before += len(TILEISH_RE.findall(m))
        tileish_after += len(TILEISH_RE.findall(p))
    return tileish_before > tileish_after      # direction must be tile -> square


def split_files(diff_text):
    # Yield (header_lines, [hunk_blocks]) per file. A hunk_block is the @@ line
    # plus its body lines.
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
    # Skip new/deleted/binary/rename-only/mode-only sections; we only do
    # in-place content edits with a real old blob and new blob.
    for h in header:
        if h.startswith(u'new file') or h.startswith(u'deleted file'):
            return False
        if h.startswith(u'Binary files') or h.startswith(u'GIT binary patch'):
            return False
        if h.startswith(u'rename from') or h.startswith(u'rename to'):
            return False
    return True


def main():
    argv = sys.argv[1:]
    apply = False
    paths = []
    if '--apply' in argv:
        apply = True
        argv = [a for a in argv if a != '--apply']
    if '--' in argv:
        paths = argv[argv.index('--') + 1:]

    cmd = ['git', '-c', 'core.quotepath=false', 'diff', '--no-color']
    if paths:
        cmd += ['--'] + paths
    rc, raw, err = run(cmd)
    if rc != 0:
        sys.stderr.write(err.decode('latin-1'))
        return rc

    diff_text = raw.decode('latin-1')
    kept_patch = []
    kept_count = 0
    skipped_mixed = []

    for header, hunks in split_files(diff_text):
        path = header[0].split(u' b/', 1)[-1] if u' b/' in header[0] else header[0]
        if not is_content_diff(header):
            continue
        kept_hunks = []
        for hunk in hunks:
            body = hunk[1:]
            if hunk_is_pure_rename(body):
                kept_hunks.append(hunk)
            else:
                # Only flag as "mixed/manual" if the hunk actually touches a
                # tile/square token at all -- otherwise it's just unrelated.
                if any(TOKEN_RE.search(l[1:]) for l in body if l[:1] in (u'+', u'-')):
                    skipped_mixed.append((path, hunk[0]))
        if kept_hunks:
            kept_count += len(kept_hunks)
            kept_patch.extend(header)
            for h in kept_hunks:
                kept_patch.extend(h)

    if not kept_patch:
        print('No pure tile->square rename hunks found.')
    else:
        patch_text = u'\n'.join(kept_patch)
        if not patch_text.endswith(u'\n'):
            patch_text += u'\n'
        patch_bytes = patch_text.encode('latin-1')

        if apply:
            rc, out, err = run(['git', 'apply', '--cached', '--recount', '--whitespace=nowarn'], patch_bytes)
            if rc != 0:
                sys.stderr.write('git apply failed:\n' + err.decode('latin-1'))
                sys.stderr.write('\nThe filtered patch was:\n' + patch_text)
                return rc
            print('Staged %d pure tile->square rename hunk(s).' % kept_count)
            print('Review with:  git diff --cached')
            print('Commit with:  git commit -m "rename: tile -> square"')
        else:
            # Dry run: show what WOULD be staged.
            emit(patch_text)
            print('\n--- DRY RUN: %d pure rename hunk(s) would be staged. Re-run with --apply. ---' % kept_count)

    if skipped_mixed:
        print('\n%d hunk(s) touch tile/square but are NOT pure renames -> left for manual review:' % len(skipped_mixed))
        for path, atat in skipped_mixed:
            print('  %s  %s' % (path, atat.strip()))

    return 0


if __name__ == '__main__':
    sys.exit(main())
