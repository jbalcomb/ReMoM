"""
test_map_addresses.py -- map_addresses.py on a synthetic .EXE (load module plus an overlay appended after it) and its map.

    python3 -m unittest tools.mom_patch_diff.tests.test_map_addresses
"""

import csv
import os
import random
import tempfile
import unittest

from tools.mom_patch_diff import map_addresses
from tools.mom_patch_diff.tests.test_diff import BASE_EA, HEADER, build_exe, function_body


class Map_Addresses_test(unittest.TestCase):

    def setUp(self):
        rng = random.Random(7)
        f1, f2, f3 = function_body(rng, 40), function_body(rng, 60), function_body(rng, 30)
        self.tmp = tempfile.TemporaryDirectory()
        root = self.tmp.name
        exe = bytearray(build_exe([f1, f2])) + f3                    # f3: overlay data after the load module
        exe[HEADER + 40 + 10] ^= 0xFF                                # f2 patched in this .EXE
        self.exe = os.path.join(root, 'WIZARDS.EXE')
        with open(self.exe, 'wb') as f:
            f.write(exe)
        hexs = lambda b: ''.join('%02X' % x for x in b)
        ovr_ea = BASE_EA + 0x2000
        lines = ['kind\tsegment\tproc\tname\tea\tsize\tbytes',
                 'S\tseg000\t0\tseg000\t%X\t100\t' % BASE_EA,
                 'F\tseg000\t1\tstart\t%X\t40\t%s' % (BASE_EA, hexs(f1)),
                 'F\tseg000\t2\tTILE_GetProd\t%X\t60\t%s' % (BASE_EA + 40, hexs(f2)),
                 'S\t_AIL_\t0\t_AIL_\t%X\t30\t' % ovr_ea,
                 'F\t_AIL_\t1\tAIL_startup\t%X\t30\t%s' % (ovr_ea, hexs(f3))]
        self.map = os.path.join(root, 'map.tsv')
        with open(self.map, 'w') as f:
            f.write('\n'.join(lines) + '\n')
        self.out = os.path.join(root, 'addresses.tsv')
        self.assertEqual(map_addresses.main(['--map', self.map, '--exe', self.exe, '--out', self.out]), 0)
        with open(self.out, newline='') as f:
            self.rows = {r['name']: r for r in csv.DictReader(f, delimiter='\t')}

    def tearDown(self):
        self.tmp.cleanup()

    def test_load_module_procedure(self):
        r = self.rows['TILE_GetProd']
        self.assertEqual((r['ida_ea'], r['ida_seg_off'], r['ida_para_off'], r['exe_para_off']),
                         ('10028', 'seg000:0028', '1000:0028', '0000:0028'))
        self.assertEqual((r['file_offset'], r['file_offset_from']), ('0x%06X' % (HEADER + 40), 'load module, differs'))
        self.assertEqual(self.rows['start']['file_offset_from'], 'load module')

    def test_overlay_procedure_is_found_by_bytes(self):
        r = self.rows['AIL_startup']
        self.assertEqual((r['segment'], r['ida_seg_off'], r['exe_para_off']), ('_AIL_', '_AIL_:0000', ''))
        self.assertEqual((r['file_offset'], r['file_offset_from']), ('0x%06X' % (HEADER + 100), 'bytes'))

    def test_without_exe_the_ida_columns_are_still_written(self):
        out = os.path.join(self.tmp.name, 'ida_only.tsv')
        self.assertEqual(map_addresses.main(['--map', self.map, '--out', out]), 0)
        with open(out, newline='') as f:
            r = {r['name']: r for r in csv.DictReader(f, delimiter='\t')}['TILE_GetProd']
        self.assertEqual((r['ida_seg_off'], r['file_offset']), ('seg000:0028', ''))


if __name__ == '__main__':
    unittest.main()
