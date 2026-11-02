#!/usr/bin/env python3
"""
export_map.py -- runs export_map.idc in IDA Pro 5.5 from the command line and writes the function/data map TSV that
mom_patch_diff and address_table read.

    python3 tools/mom_patch_diff/export_map.py --ida "/c/Program Files/IDA 5.5/idag.exe" \\
        --idb /c/STU/IDA/WIZARDS.idb --out /c/MoM/wizards_map.tsv

On Linux, IDA 5.5 runs under Wine: add --wine, and every path IDA sees is converted to its Windows form (winepath -w,
or Z:\\... when winepath is not installed):

    python3 tools/mom_patch_diff/export_map.py --wine --ida ~/.wine/drive_c/IDA55/idag.exe \\
        --idb ~/MoM/ida/WIZARDS.idb --out ~/MoM/wizards_map.tsv

IDA works on a copy of the database (the .idb and any unpacked .id0/.id1/.nam/.til next to it), so the original is
never opened or saved.  IDA runs autonomously (-A), the script is passed with -S, and IDA's messages go to a log
(-L) printed afterwards.  Standard library only.
"""

import argparse
import glob
import os
import shutil
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))


def wine_path(path):
    """A Linux path as Wine's Windows programs see it."""
    try:
        result = subprocess.run(['winepath', '-w', path], capture_output=True, text=True, timeout=60)
        if result.returncode == 0 and result.stdout.strip():
            return result.stdout.strip()
    except (OSError, subprocess.TimeoutExpired):
        pass
    return 'Z:' + os.path.abspath(path).replace('/', '\\')


def native(path):
    """Git Bash paths (/c/...) to Windows ones (C:/...), which IDA understands."""
    if os.name == 'nt' and len(path) > 2 and path[0] == '/' and path[2] == '/':
        return path[1].upper() + ':' + path[2:]
    return path


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    p.add_argument('--ida', required=True, help='idag.exe (GUI) or idaw.exe (text mode) of IDA Pro 5.5')
    p.add_argument('--idb', required=True, help='the 1.31 database, e.g. WIZARDS.idb')
    p.add_argument('--out', required=True, help='TSV to write')
    p.add_argument('--timeout', type=int, default=1800, help='seconds to wait for IDA (default 1800)')
    p.add_argument('--keep', action='store_true', help='keep the working copy and the generated script')
    p.add_argument('--wine', action='store_true', help='run IDA under Wine (Linux)')
    p.add_argument('--wine-command', default='wine', help='the Wine launcher (default: wine)')
    args = p.parse_args(argv)

    ida, idb, out = native(args.ida), os.path.abspath(native(args.idb)), os.path.abspath(native(args.out))
    if not os.path.isfile(ida):
        sys.exit('export_map: no IDA at %s (--ida must name idag.exe or idaw.exe)' % ida)
    if not os.path.isfile(idb):
        sys.exit('export_map: no database at %s' % idb)
    if os.path.exists(out):
        os.remove(out)

    work = tempfile.mkdtemp(prefix='mom_export_map_')
    stem = os.path.splitext(os.path.basename(idb))[0]
    for companion in glob.glob(os.path.join(os.path.dirname(idb), glob.escape(stem) + '.*')):
        if os.path.splitext(companion)[1].lower() in ('.idb', '.id0', '.id1', '.id2', '.nam', '.til'):
            shutil.copy2(companion, work)
    db_copy = os.path.join(work, os.path.basename(idb))

    with open(os.path.join(HERE, 'export_map.idc'), encoding='latin-1') as f:
        script = f.read()
    if '@@OUTPUT_PATH@@' not in script:
        sys.exit('export_map: export_map.idc has no @@OUTPUT_PATH@@ placeholder')
    ida_sees = wine_path if args.wine else (lambda path: path)
    script = script.replace('@@OUTPUT_PATH@@', ida_sees(out).replace('\\', '/'))   # IDC string: forward slashes need no escaping
    script_path = os.path.join(work, 'export_map_run.idc')
    with open(script_path, 'w', encoding='latin-1') as f:
        f.write(script)
    log_path = os.path.join(work, 'ida.log')

    command = [ida, '-A', '-S' + ida_sees(script_path), '-L' + ida_sees(log_path), ida_sees(db_copy)]
    if args.wine:
        command = [args.wine_command] + command
    print('export_map: running', ' '.join('"%s"' % c if ' ' in c else c for c in command))
    try:
        result = subprocess.run(command, cwd=work, timeout=args.timeout)
        code = result.returncode
    except subprocess.TimeoutExpired:
        code = None
    if os.path.exists(log_path):
        with open(log_path, encoding='latin-1', errors='replace') as f:
            log = f.read()
        tail = log.strip().splitlines()[-15:]
        print('export_map: last lines of the IDA log:\n  ' + '\n  '.join(tail))

    ok = os.path.isfile(out) and os.path.getsize(out) > 0
    if not args.keep:
        shutil.rmtree(work, ignore_errors=True)
    else:
        print('export_map: working copy kept in', work)
    if code is None:
        sys.exit('export_map: IDA did not finish within %d seconds' % args.timeout)
    if not ok:
        sys.exit('export_map: IDA exited with %s and wrote no map (run with --keep and read ida.log)' % code)
    with open(out, encoding='latin-1') as f:
        kinds = [line[:1] for line in f][1:]
    print('export_map: %s: %d segments, %d functions, %d data items' % (out, kinds.count('S'), kinds.count('F'), kinds.count('D')))
    return 0


if __name__ == '__main__':
    sys.exit(main())
