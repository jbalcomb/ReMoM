"""
test_link_remom.py -- remom_index.py and link_remom.py on a small ReMoM tree and joined table.

    python3 -m unittest tools.mom_patch_diff.tests.test_link_remom
"""

import contextlib
import csv
import io
import os
import tempfile
import unittest

from tools.mom_patch_diff import link_remom, remom_index


class Link_Remom_test(unittest.TestCase):

    def test_index_and_link(self):
        with tempfile.TemporaryDirectory() as root:
            src = os.path.join(root, 'remom', 'MoM', 'src')
            os.makedirs(src)
            with open(os.path.join(src, 'Terrain.c'), 'w') as f:
                f.write('// WZD s161p04\nint16_t Square_Production_Bonus(int16_t wx)\n{\n}\n'
                        '// WZD s36p01\nvoid Thirty_Six(void)\n{\n}\n'
                        '// WZD o050p01\n// not reconstructed yet\n')
            index = os.path.join(root, 'index.tsv')
            joined = os.path.join(root, 'joined.tsv')
            addresses = os.path.join(root, 'addresses.tsv')
            out = os.path.join(root, 'linked.tsv')
            with open(addresses, 'w') as f:
                f.write('kind\tsegment\tproc\tname\tsize\tida_ea\n'
                        'S\tovr161\t0\tovr161\t10\tF3190\nS\tseg036\t0\tseg036\t10\t30000\nS\t_AIL_\t0\t_AIL_\t10\t31000\n')
            with open(joined, 'w') as f:
                f.write('match\tfile_offset\tremom_segment\tremom_proc\tremom_name\tdrake178_name\n'
                        'file offset\t0x0EC9EA\tovr161\t4\tMap_Square_Production_Bonus\tTILE_GetProd\n'
                        'file offset\t0x001000\tseg036\t1\tThirty_Six\tthirty_six\n'
                        'file offset\t0x002000\t_AIL_\t1\tAIL_startup\tAIL_startup\n')
            with contextlib.redirect_stdout(io.StringIO()):
                self.assertEqual(remom_index.main(['--remom', os.path.join(root, 'remom'), '--out', index]), 0)
                self.assertEqual(link_remom.main(['--exe', 'WZD', '--joined', joined, '--addresses', addresses,
                                                  '--index', index, '--out', out]), 0)
            with open(out, newline='') as f:
                rows = list(csv.DictReader(f, delimiter='\t'))
        by = {r['remom_name'] or r['remom_id']: r for r in rows}
        r = by['Map_Square_Production_Bonus']
        self.assertEqual((r['remom_id'], r['remom_function'], r['id_letter']), ('WZD s161p04', 'Square_Production_Bonus', 's for an overlay'))
        self.assertEqual((by['Thirty_Six']['remom_id'], by['Thirty_Six']['id_letter']), ('WZD s036p01', ''))
        self.assertEqual(by['AIL_startup']['remom_id'], '')                 # renamed segment: no number, not linked
        self.assertEqual(by['WZD o050p01']['match'], 'only in ReMoM')


if __name__ == '__main__':
    unittest.main()
