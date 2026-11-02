"""
test_address_table.py -- export_map.py with a stand-in for IDA, and address_table.py on synthetic sources, map,
executable and another project's index.

    python3 -m unittest tools.mom_patch_diff.tests.test_address_table
"""

import csv
import os
import stat
import sys
import tempfile
import unittest

from tools.mom_patch_diff import address_table, export_map
from tools.mom_patch_diff.tests.test_diff import BASE_EA, build_exe, function_body, map_rows

FAKE_IDA = '''#!{python}
# Stand-in for idag.exe: checks the command line, then does what export_map.idc would: writes the map.
import sys, re, os
args = sys.argv[1:]
assert args[0] == '-A', args
script = next(a[2:] for a in args if a.startswith('-S'))
log = next(a[2:] for a in args if a.startswith('-L'))
db = args[-1]
assert os.path.dirname(db) == os.path.dirname(script), 'IDA must run on the working copy'
text = open(script, encoding='latin-1').read()
assert '@@OUTPUT_PATH@@' not in text
out = re.search(r'path = "([^"]+)";', text).group(1)
open(out, 'w').write('kind\\tsegment\\tproc\\tname\\tea\\tsize\\tbytes\\nS\\tseg001\\t0\\tseg001\\t10000\\t64\\t\\nF\\tseg001\\t1\\tf\\t10000\\t4\\t558BECCB\\n')
open(log, 'w').write('export_map: 1 functions, 0 data items written\\n')
'''


class Export_Map_test(unittest.TestCase):

    def test_runs_ida_on_a_copy_and_writes_the_map(self):
        with tempfile.TemporaryDirectory() as root:
            ida = os.path.join(root, 'idag')
            with open(ida, 'w') as f:
                f.write(FAKE_IDA.format(python=sys.executable))
            os.chmod(ida, os.stat(ida).st_mode | stat.S_IEXEC)
            idb = os.path.join(root, 'WIZARDS.idb')
            with open(idb, 'wb') as f:
                f.write(b'IDB')
            out = os.path.join(root, 'wizards_map.tsv')
            self.assertEqual(export_map.main(['--ida', ida, '--idb', idb, '--out', out]), 0)
            with open(out) as f:
                self.assertIn('F\tseg001\t1\tf\t10000\t4\t558BECCB', f.read())
            with open(idb, 'rb') as f:
                self.assertEqual(f.read(), b'IDB')           # the original database is untouched


FAKE_WINE = '''#!{python}
# Stand-in for wine: every path argument must arrive in Windows form (Z:\\\\...); map them back and run the program.
import os, subprocess, sys
def linux(arg):
    for prefix in ('-S', '-L', ''):
        if arg.startswith(prefix + 'Z:\\\\'):
            return prefix + arg[len(prefix) + 2:].replace('\\\\', '/')
    return arg
args = sys.argv[1:]
assert all(a.startswith(('Z:', '-SZ:', '-LZ:')) for a in args[2:]) and args[1] == '-A', args
script = args[2][2:]
text = open(linux('-S' + script)[2:], encoding='latin-1').read()
assert 'path = "Z:/' in text, 'the output path inside the IDC must be a Windows path'
open(linux('-S' + script)[2:], 'w', encoding='latin-1').write(text.replace('path = "Z:/', 'path = "/'))
sys.exit(subprocess.call([args[0]] + [linux(a) for a in args[1:]]))
'''


class Export_Map_Wine_test(unittest.TestCase):

    def test_runs_ida_under_wine_with_windows_paths(self):
        with tempfile.TemporaryDirectory() as root:
            ida = os.path.join(root, 'idag.exe')
            wine = os.path.join(root, 'fakewine')
            for path, text in ((ida, FAKE_IDA), (wine, FAKE_WINE)):
                with open(path, 'w') as f:
                    f.write(text.format(python=sys.executable))
                os.chmod(path, os.stat(path).st_mode | stat.S_IEXEC)
            idb = os.path.join(root, 'WIZARDS.idb')
            with open(idb, 'wb') as f:
                f.write(b'IDB')
            out = os.path.join(root, 'wizards_map.tsv')
            self.assertEqual(export_map.main(['--wine', '--wine-command', wine, '--ida', ida, '--idb', idb, '--out', out]), 0)
            with open(out) as f:
                self.assertIn('F\tseg001\t1\tf', f.read())


class Address_Table_test(unittest.TestCase):

    def setUp(self):
        import random
        rng = random.Random(3)
        self.tmp = tempfile.TemporaryDirectory()
        root = self.tmp.name
        f1, f2 = function_body(rng, 40), function_body(rng, 60)
        layout = [('F', 'seg001', 1, 'TILE_GetProd', 0, f1, None), ('F', 'ovr142', 2, 'CTY_GetResearch', 40, f2, None)]
        self.map_path = os.path.join(root, 'map.tsv')
        with open(self.map_path, 'w') as f:
            header = 'kind\tsegment\tproc\tname\tea\tsize\tbytes\n'
            f.write(map_rows(layout).replace(header, header + 'S\tseg001\t0\tseg001\t%X\t40\t\nS\tovr142\t0\tovr142\t%X\t60\t\n'
                                             % (BASE_EA, BASE_EA + 40)))
        for version, blocks in (('v131', [f1, f2]), ('v160', [f1, bytearray(f2[:-1]) + bytearray([0x90, 0xCB])])):
            os.makedirs(os.path.join(root, version))
            with open(os.path.join(root, version, 'WIZARDS.EXE'), 'wb') as f:
                f.write(build_exe(blocks))
        src = os.path.join(root, 'remom', 'MoM', 'src')
        os.makedirs(src)
        with open(os.path.join(src, 'Terrain.c'), 'w') as f:
            f.write('// WZD s161p04\n// drake178: TILE_GetProd()\nint Unrelated_Helper(void)\n{\n}\n\n'
                    '// WZD s1p1\nint16_t Square_Production_Bonus(int16_t wx)\n{\n    return 0;\n}\n'
                    '// WZD o142p02\n// drake178: CTY_GetResearch()\nint16_t City_Research_Production(int16_t city_idx);\n'
                    '// WZD o142p03\n// not reconstructed yet\n')
        with open(os.path.join(src, 'CITYCALC.c'), 'w') as f:
            f.write('/* helper first */\nstatic int helper(void)\n{\n}\n\nint16_t City_Research_Production(int16_t city_idx)\n{\n}\n')
        self.project = os.path.join(root, 'other.tsv')
        with open(self.project, 'w') as f:
            f.write('exe\tsegment\tproc\tfunction\tfile\tline\nWZD\ts001\t1\tterrain_prod_bonus\twizards/seg001.c\t12\n')
        self.out = os.path.join(root, 'out')
        self.assertEqual(address_table.main(['--remom', os.path.join(root, 'remom'), '--out', self.out,
                                             '--map', 'WIZARDS.EXE=' + self.map_path,
                                             '--exe', '1.31=' + os.path.join(root, 'v131'), '--exe', '1.60=' + os.path.join(root, 'v160'),
                                             '--project', 'other=' + self.project]), 0)
        with open(os.path.join(self.out, 'address_table.tsv'), newline='') as f:
            self.rows = {r['id']: r for r in csv.DictReader(f, delimiter='\t')}

    def tearDown(self):
        self.tmp.cleanup()

    def test_function_row_joins_ida_exe_remom_and_the_other_project(self):
        row = self.rows['WZD s001p01']
        self.assertEqual(row['ida_name'], 'TILE_GetProd')
        self.assertEqual(row['seg_off'], 'seg001:0000')
        self.assertEqual(row['offset_1.31'], '0x20')         # after the 32-byte MZ header
        self.assertEqual(row['offset_1.60'], '0x20')
        self.assertEqual(row['remom_function'], 'Square_Production_Bonus')
        self.assertEqual(row['remom_evidence'], 'definition')
        self.assertEqual(row['other_function'], 'terrain_prod_bonus')

    def test_changed_function_and_prototype_evidence(self):
        row = self.rows['WZD o142p02']
        self.assertEqual(row['seg_off'], 'ovr142:0000')
        self.assertEqual(row['offset_1.31'], '0x48')
        self.assertEqual(row['offset_1.60'], 'changed')
        self.assertEqual(row['remom_function'], 'City_Research_Production')
        self.assertEqual(row['remom_evidence'], 'prototype')
        self.assertTrue(row['remom_location'].startswith('MoM/src/CITYCALC.c:'))

    def test_drake178_names_are_checked_against_the_idb(self):
        same = self.rows['WZD o142p02']                      # ReMoM's comment and the IDB agree on the name
        self.assertEqual((same['remom_drake178'], same['name_check'], same['name_elsewhere']), ('CTY_GetResearch', 'same', ''))
        moved = self.rows['WZD s001p01']                     # the IDB's TILE_GetProd is s001p01; ReMoM says s161p04
        self.assertEqual((moved['remom_drake178'], moved['name_check'], moved['name_elsewhere']), ('', '', 'WZD s161p04'))
        self.assertEqual(self.rows['WZD s161p04']['remom_drake178'], 'TILE_GetProd')
        with open(os.path.join(self.out, 'address_table.md')) as f:
            summary = f.read()
        self.assertIn('name at another id 1', summary)
        self.assertIn('name same 1', summary)

    def test_segments_are_aligned_by_names(self):
        # the IDB's TILE_GetProd is seg001 proc 1; ReMoM's drake178 comment puts it at s161p04: seg001 is s161, shifted +3
        row = self.rows['WZD s001p01']
        self.assertEqual((row['aligned_id'], row['aligned_remom_function']), ('WZD s161p04', 'Unrelated_Helper'))
        self.assertEqual(row['alignment'], 'names 1/1 (one name only)')
        same = self.rows['WZD o142p02']                      # names agree in place: shift 0
        self.assertEqual((same['aligned_id'], same['alignment']), ('WZD o142p02', 'names 1/1 (one name only)'))
        with open(os.path.join(self.out, 'segment_alignment.tsv'), newline='') as f:
            seg = {r['ida_segment']: r for r in csv.DictReader(f, delimiter='\t')}
        self.assertEqual((seg['seg001']['remom_segment'], seg['seg001']['proc_shift'], seg['seg001']['aligned']), ('s161', '+3', 'yes'))

    def test_renamed_segment_is_numbered_from_its_neighbours(self):
        from tools.mom_patch_diff.diff import Item, segment_ids
        seg = lambda name, ea: Item({'kind': 'S', 'segment': name, 'proc': '0', 'name': name, 'ea': '%X' % ea, 'size': '16', 'bytes': ''})
        ids = segment_ids([seg('seg034', 0x100), seg('_AIL_', 0x200), seg('seg036', 0x300), seg('dseg', 0x400),
                           seg('ovr050', 0x500), seg('_GUI_', 0x600)])
        self.assertEqual(ids['_AIL_'], 'seg035')
        self.assertEqual(ids['dseg'], '')                    # between seg036 and ovr050: no number to give
        self.assertEqual(ids['_GUI_'], '')                   # after the last named segment: no neighbour to check

    def test_prefix_changes_are_told_apart(self):
        self.assertTrue(address_table.same_but_prefix('UU_CTY_GetResearch', 'CTY_GetResearch'))
        self.assertTrue(address_table.same_but_prefix('RP_GetResearch', 'CTY_GetResearch'))
        self.assertFalse(address_table.same_but_prefix('CTY_GetGold', 'CTY_GetResearch'))
        self.assertEqual(address_table.name_check('UU_CTY_GetResearch', 'k', {'k': ['CTY_GetResearch']}, {})[0], 'prefix')

    def test_ids_only_in_remom_are_listed(self):
        self.assertEqual(self.rows['WZD o142p03']['remom_evidence'], 'comment')
        self.assertEqual(self.rows['WZD o142p03']['ida_name'], '(not in the map)')
        self.assertEqual(self.rows['WZD s161p04']['remom_function'], 'Unrelated_Helper')


if __name__ == '__main__':
    unittest.main()
