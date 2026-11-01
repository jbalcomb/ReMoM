"""
test_patch_index.py -- patch_index.py on synthetic patch files, a synthetic .EXE and its map.

    python3 -m unittest tools.mom_patches.tests.test_patch_index
"""

import csv
import os
import random
import tempfile
import unittest

from tools.mom_patch_diff.tests.test_diff import HEADER, build_exe, function_body, map_rows
from tools.mom_patches.patch_index import main, parse_patch


def tsv(path):
    with open(path, newline='', encoding='utf-8') as f:
        return {r['patch']: r for r in csv.DictReader(f, delimiter='\t')}


class Patch_Index_test(unittest.TestCase):

    def setUp(self):
        rng = random.Random(5)
        self.tmp = tempfile.TemporaryDirectory()
        root = self.tmp.name
        f1, f2 = function_body(rng, 40), function_body(rng, 60)
        self.exe = os.path.join(root, 'WIZARDS.EXE')
        with open(self.exe, 'wb') as f:
            f.write(build_exe([f1, f2]))
        self.map = os.path.join(root, 'map.tsv')
        with open(self.map, 'w') as f:
            f.write(map_rows([('F', 'seg161', 4, 'TILE_GetProd', 0, f1, None), ('F', 'ovr142', 13, 'CTY_GetResearch', 40, f2, None)]))
        remom = os.path.join(root, 'remom', 'MoM', 'src')
        os.makedirs(remom)
        with open(os.path.join(remom, 'Terrain.c'), 'w') as f:
            f.write('// WZD s161p04\n// drake178: TILE_GetProd()\nint16_t Square_Production_Bonus_Original(int16_t wx)\n{\n}\n')
        patches = os.path.join(root, 'patches')
        os.makedirs(os.path.join(patches, '151', 'OBSOLETE'))
        o1, o2 = HEADER + 10, HEADER + 40 + 5                  # one byte in each function
        with open(os.path.join(patches, '151', 'W400FIX.TXT'), 'w') as f:
            f.write(';------\n; 400 - Mountains give the right production; see TILE_GetProd()\n;------\n'
                    '%X\n%02X\n90\n' % (o1, f1[10]) +
                    ';------\n; second part, reverses W399\n;------\n%X\n%02X\n91\n' % (o2, f2[5]))
        with open(os.path.join(patches, '151', 'W401AI.TXT'), 'w') as f:
            f.write('; This patch is now obsolete. You can still check its contents in the OBSOLETE\n'
                    '; folder, but it can no longer be applied or removed.\n')
        with open(os.path.join(patches, '151', 'OBSOLETE', 'W401AI.TXT'), 'w') as f:
            f.write('; This patch is now obsolete. Its contents are preserved here only for continuity.\n'
                    '; Please do not try to apply or remove it.\n;---\n; AI prefers grassland\n;---\n%X\nFF\n00\n' % o1)
        self.out = os.path.join(root, 'out')
        self.assertEqual(main([patches, '--out', self.out, '--remom', os.path.join(root, 'remom'),
                               '--base', 'WIZARDS.EXE=' + self.exe, '--map', 'WIZARDS.EXE=' + self.map]), 0)

    def tearDown(self):
        self.tmp.cleanup()

    def test_patch_row(self):
        row = tsv(os.path.join(self.out, 'patches.tsv'))['W400FIX']
        self.assertEqual(row['title'], 'Mountains give the right production; see TILE_GetProd()')
        self.assertEqual((row['exe'], row['number'], row['kind'], row['status']), ('WZD', '400', 'Bug fix', 'current'))
        self.assertEqual((row['sections'], row['bytes'], row['old_matches_base']), ('2', '2', '2/2'))
        self.assertEqual(row['functions'], 'WZD o142p13 (1), WZD s161p04 (1)')
        self.assertIn('Square_Production_Bonus_Original', row['remom_by_id'])
        self.assertEqual(row['remom_by_name'], 'TILE_GetProd = WZD s161p04 Square_Production_Bonus_Original')
        self.assertEqual(row['references'], 'W399')

    def test_obsolete_stub_and_original(self):
        with open(os.path.join(self.out, 'patches.tsv'), newline='') as f:
            rows = [r for r in csv.DictReader(f, delimiter='\t') if r['patch'] == 'W401AI']
        self.assertEqual(sorted(r['status'] for r in rows), ['obsolete', 'obsolete (stub)'])
        self.assertEqual([r['title'] for r in rows if r['status'] == 'obsolete'], ['AI prefers grassland'])

    def test_records_name_the_function_and_check_the_original_byte(self):
        with open(os.path.join(self.out, 'records.tsv'), newline='') as f:
            rows = [r for r in csv.DictReader(f, delimiter='\t') if r['patch'] == 'W400FIX']
        self.assertEqual([r['in'] for r in rows], ['WZD s161p04+0xA', 'WZD o142p13+0x5'])
        self.assertEqual([r['old_matches_base'] for r in rows], ['yes', 'yes'])
        with open(os.path.join(self.out, 'records.tsv'), newline='') as f:
            obsolete = [r for r in csv.DictReader(f, delimiter='\t') if r['patch'] == 'W401AI']
        self.assertEqual(obsolete[0]['old_matches_base'], 'no')          # FF is not the 1.31 byte

    def test_todo_lists_current_patches_and_obsolete_apart(self):
        with open(os.path.join(self.out, 'todo_151.md'), encoding='utf-8') as f:
            todo = f.read()
        self.assertIn('- [ ] **W400FIX** Mountains give the right production', todo)
        self.assertIn('ReMoM: WZD s161p04 Square_Production_Bonus_Original', todo)
        self.assertIn('## Obsolete (2)', todo)
        self.assertNotIn('- [ ] **W401AI**', todo)

    def test_parser_tolerates_a_value_glued_to_a_comment(self):
        parsed = parse_patch('1A\n00\n01\n2B\n00;-----\n')
        self.assertEqual(parsed['sections'][0][1], [(0x1A, 0, 1)])
        self.assertTrue(parsed['malformed'])


if __name__ == '__main__':
    unittest.main()
