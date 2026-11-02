#!/usr/bin/env python3
"""
ida_help.py -- turns IDA's help file (idahelp.chm) into plain text that grep can search, plus an index of its pages
and of the IDC functions it documents.

    python3 tools/ida_help/ida_help.py --chm /c/STU_DASM/IDA55/idahelp.chm --out /c/STU_DASM/IDA55_help
    python3 tools/ida_help/ida_help.py --out /c/STU_DASM/IDA55_help --grep "file offset"

Writes into --out:
    html/               the pages, as decompiled from the .chm
    text/<page>.txt     each page as plain text
    pages.tsv           page, title, number of lines
    idc_functions.tsv   every IDC declaration found in the pages ("long GetSegmentAttr(long segea,long attr);"):
                        function, returns, arguments, page, and its description (the // comment above it)

The .chm is decompiled with Windows' own hh.exe ("hh.exe -decompile"), which needs no install.  On Linux, 7z (if
installed) is used instead; or decompile the .chm elsewhere and pass the folder with --html.  Standard library only.
"""

import argparse
import csv
import html
import html.parser
import os
import re
import shutil
import subprocess
import sys

DECLARATION = re.compile(r'^\s*(long|string|success|void|char|auto|float|int|object)\s+([A-Za-z_]\w*)\s*\(([^)]*)\)\s*;?')


def native(path):
    """Git Bash paths (/c/...) to Windows ones (C:/...)."""
    if os.name == 'nt' and len(path) > 2 and path[0] == '/' and path[2] == '/':
        return path[1].upper() + ':' + path[2:]
    return path


def decompile(chm, folder):
    os.makedirs(folder, exist_ok=True)
    if os.name == 'nt':
        hh = os.path.join(os.environ.get('WINDIR', r'C:\Windows'), 'hh.exe')
        command = [hh, '-decompile', os.path.abspath(folder).replace('/', '\\'), os.path.abspath(chm).replace('/', '\\')]
    elif shutil.which('7z'):
        command = ['7z', 'x', '-y', '-o' + folder, chm]
    else:
        sys.exit('ida_help: cannot decompile %s here: no hh.exe (Windows) and no 7z.  Decompile it elsewhere '
                 '("hh.exe -decompile FOLDER idahelp.chm") and pass the folder with --html.' % chm)
    print('ida_help: running', ' '.join(command))
    subprocess.run(command, check=False)                      # hh.exe returns no useful exit code
    pages = [p for p in html_pages(folder)]
    if not pages:
        sys.exit('ida_help: no .htm/.html pages in %s after decompiling %s' % (folder, chm))


def html_pages(folder):
    for root, _, names in os.walk(folder):
        for name in sorted(names):
            if name.lower().endswith(('.htm', '.html')):
                yield os.path.join(root, name)


class Text(html.parser.HTMLParser):
    """HTML to text: block elements end lines, <pre> keeps its layout, scripts and styles are dropped."""

    BLOCKS = {'p', 'div', 'br', 'tr', 'li', 'h1', 'h2', 'h3', 'h4', 'h5', 'h6', 'pre', 'table', 'dt', 'dd', 'hr'}

    def __init__(self):
        super().__init__(convert_charrefs=True)
        self.parts, self.title, self.skip, self.pre, self.in_title = [], '', 0, 0, False

    def handle_starttag(self, tag, attrs):
        if tag in ('script', 'style'):
            self.skip += 1
        elif tag == 'title':
            self.in_title = True
        if tag == 'pre':
            self.pre += 1
        if tag in self.BLOCKS:
            self.parts.append('\n')

    def handle_endtag(self, tag):
        if tag in ('script', 'style'):
            self.skip = max(0, self.skip - 1)
        elif tag == 'title':
            self.in_title = False
        if tag == 'pre':
            self.pre = max(0, self.pre - 1)
        if tag in self.BLOCKS:
            self.parts.append('\n')

    def handle_data(self, data):
        if self.in_title:
            self.title += data
            return
        if self.skip:
            return
        self.parts.append(data if self.pre else re.sub(r'\s+', ' ', data))

    def text(self):
        lines = [l.rstrip() for l in ''.join(self.parts).splitlines()]
        out = []
        for l in lines:
            if l.strip() or (out and out[-1].strip()):
                out.append(l)
        return '\n'.join(out).strip() + '\n'


def describe(lines, i, m):
    """The description of the declaration on line i.  IDA's help (like idc.idc) puts it in the // comment lines just
    above the declaration; otherwise it is a comment on the declaration's own line, or else the next line of text."""
    above = []
    for line in reversed(lines[max(0, i - 8):i]):
        s = line.strip()
        if not s.startswith('//'):
            break
        above.insert(0, s.lstrip('/').strip())
    above = [a for a in above if a]
    if above:
        return ' '.join(above)
    same = lines[i][m.end():].strip()
    if same.startswith('//'):
        return same.lstrip('/').strip()
    return next((l.strip() for l in lines[i + 1:i + 6] if l.strip() and not DECLARATION.match(l) and not l.strip().startswith('//')), '')


def convert(html_folder, out):
    text_folder = os.path.join(out, 'text')
    pages, functions = [], []
    for path in html_pages(html_folder):
        with open(path, 'rb') as f:
            raw = f.read()
        parser = Text()
        parser.feed(raw.decode('utf-8', errors='replace') if raw.startswith(b'\xef\xbb\xbf') else raw.decode('cp1252', errors='replace'))
        body = parser.text()
        page = os.path.relpath(path, html_folder).replace(os.sep, '/')
        target = os.path.join(text_folder, os.path.splitext(page)[0] + '.txt')
        os.makedirs(os.path.dirname(target), exist_ok=True)
        with open(target, 'w', encoding='utf-8', newline='\n') as f:
            f.write(body)
        title = html.unescape(re.sub(r'\s+', ' ', parser.title)).strip()
        lines = body.splitlines()
        pages.append((page, title, len(lines)))
        for i, line in enumerate(lines):
            m = DECLARATION.match(line)
            if m:
                functions.append((m.group(2), m.group(1), re.sub(r'\s+', ' ', m.group(3)).strip(), page, describe(lines, i, m)))
    with open(os.path.join(out, 'pages.tsv'), 'w', newline='', encoding='utf-8') as f:
        w = csv.writer(f, delimiter='\t', lineterminator='\n')
        w.writerow(['page', 'title', 'lines'])
        w.writerows(pages)
    seen, unique = set(), []
    for fn in functions:
        if (fn[0], fn[3]) not in seen:
            seen.add((fn[0], fn[3]))
            unique.append(fn)
    with open(os.path.join(out, 'idc_functions.tsv'), 'w', newline='', encoding='utf-8') as f:
        w = csv.writer(f, delimiter='\t', lineterminator='\n')
        w.writerow(['function', 'returns', 'arguments', 'page', 'description'])
        w.writerows(sorted(unique))
    print('ida_help: %d pages, %d IDC declarations (%d functions) written to %s'
          % (len(pages), len(unique), len({u[0] for u in unique}), out))


def search(out, pattern):
    regex = re.compile(pattern, re.I)
    titles = {}
    with open(os.path.join(out, 'pages.tsv'), newline='', encoding='utf-8') as f:
        for row in csv.DictReader(f, delimiter='\t'):
            titles[os.path.splitext(row['page'])[0] + '.txt'] = row['title']
    text_folder = os.path.join(out, 'text')
    hits = 0
    for root, _, names in os.walk(text_folder):
        for name in sorted(names):
            path = os.path.join(root, name)
            rel = os.path.relpath(path, text_folder).replace(os.sep, '/')
            with open(path, encoding='utf-8') as f:
                for number, line in enumerate(f, 1):
                    if regex.search(line):
                        hits += 1
                        print('%s:%d [%s] %s' % (rel, number, titles.get(rel, ''), line.strip()))
    print('ida_help: %d matching lines' % hits)


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    p.add_argument('--chm', help='idahelp.chm to decompile and convert')
    p.add_argument('--html', help='an already decompiled .chm folder to convert (instead of --chm)')
    p.add_argument('--out', required=True, help='folder to write into (or, with only --grep, to search)')
    p.add_argument('--grep', help='print every line of the converted pages that matches this regular expression (case-insensitive)')
    args = p.parse_args(argv)
    out = native(args.out)
    if args.chm or args.html:
        html_folder = native(args.html) if args.html else os.path.join(out, 'html')
        if args.chm:
            decompile(native(args.chm), html_folder)
        convert(html_folder, out)
    elif not args.grep:
        p.error('give --chm or --html to convert, and/or --grep to search')
    if args.grep:
        search(out, args.grep)
    return 0


if __name__ == '__main__':
    sys.exit(main())
