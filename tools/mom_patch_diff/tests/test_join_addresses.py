"""
test_join_addresses.py -- join_addresses.py on two small map_addresses.py tables.

    python3 -m unittest tools.mom_patch_diff.tests.test_join_addresses
"""

import contextlib
import csv
import io
import os
import tempfile
import unittest

from tools.mom_patch_diff import join_addresses

HEAD = 'kind\tsegment\tproc\tname\tsize\tida_ea\tida_seg_off\tida_para_off\texe_para_off\tfile_offset\tfile_offset_from\n'


def table(path, rows):
    with open(path, 'w', encoding='utf-8') as f:
        f.write(HEAD + ''.join('\t'.join(r) + '\n' for r in rows))


class Join_Addresses_test(unittest.TestCase):

    def test_join(self):
        with tempfile.TemporaryDirectory() as root:
            mine, his, out = (os.path.join(root, n) for n in ('mine.tsv', 'his.tsv', 'out.tsv'))
            F = lambda seg, proc, name, ea, off, how: ['F', seg, proc, name, '10', ea, seg + ':0000', '', '', off, how]
            table(mine, [['S', 'seg161', '0', 'seg161', '64', '30000', 'seg161:0000', '', '', '0x01A000', 'load module'],
                         F('seg161', '4', 'Square_Production_Bonus', '30010', '0x01A010', 'load module'),
                         F('seg161', '5', 'sub_30020', '30020', '', 'too short'),
                         F('seg162', '1', 'Twin', '30100', '0x020000', 'bytes, 2 places'),
                         F('seg163', '1', 'Only_Mine', '30200', '0x030000', 'bytes')])
            table(his, [F('dseg', '9', 'TILE_GetProd', '40010', '0x01A010', 'load module'),        # other address, same offset
                        F('seg161', '5', 'TILE_Tiny', '30020', '', 'too short'),
                        F('seg162', '1', 'Twin', '30100', '0x020000', 'bytes, 2 places'),
                        F('ovr050', '1', 'Only_His', '50000', '0x040000', 'bytes')])
            with contextlib.redirect_stdout(io.StringIO()):
                self.assertEqual(join_addresses.main(['--left', 'remom=' + mine, '--right', 'drake178=' + his, '--out', out]), 0)
            with open(out, newline='') as f:
                rows = list(csv.DictReader(f, delimiter='\t'))
        by = {(r['remom_name'] or r['drake178_name']): r for r in rows}
        r = by['Square_Production_Bonus']
        self.assertEqual((r['match'], r['drake178_name'], r['drake178_segment'], r['names']), ('file offset', 'TILE_GetProd', 'dseg', 'differ'))
        self.assertEqual((by['sub_30020']['match'], by['sub_30020']['drake178_name'], by['sub_30020']['names']), ('ida address', 'TILE_Tiny', ''))
        self.assertEqual((by['Twin']['match'], by['Twin']['names']), ('ida address', 'same'))
        self.assertEqual(by['Only_Mine']['match'], 'only remom')
        self.assertEqual(by['Only_His']['match'], 'only drake178')
        self.assertEqual(len(rows), 5)                          # segment rows are not joined


if __name__ == '__main__':
    unittest.main()
