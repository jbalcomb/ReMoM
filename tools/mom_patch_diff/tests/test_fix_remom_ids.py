"""
test_fix_remom_ids.py -- fix_remom_ids.py on a small ReMoM tree and link_remom.py table.

    python3 -m unittest tools.mom_patch_diff.tests.test_fix_remom_ids
"""

import contextlib
import io
import os
import tempfile
import unittest

from tools.mom_patch_diff import fix_remom_ids

LINKED = ('match\tfile_offset\tremom_segment\tremom_proc\tremom_name\tdrake178_name\tremom_id\tremom_function\tremom_file\tremom_line\tremom_evidence\tid_letter\n'
          'file offset\t0x1\tseg035\t2\tAdd_Hot_Key\tGUI_AddHotkey\t\t\t\t\t\t\n'
          'file offset\t0x2\tseg035\t4\tsub_1234\tGUI_AddGrid\t\t\t\t\t\t\n'
          'file offset\t0x3\tseg035\t9\tTaken\tTaken\tWZD s035p09\tTaken\tMoX/src/Fields.c\t1\tdefinition\t\n'
          'file offset\t0x4\tseg036\t1\tTwice\tTwice\t\t\t\t\t\t\n'
          'file offset\t0x5\tseg036\t2\tTwice\tTwice\t\t\t\t\t\t\n'
          'only in ReMoM\t\t\t\t\t\tWZD s034p49\tAdd_Hot_Key\tMoX/src/Fields.c\t3\tdefinition\t\n'
          'only in ReMoM\t\t\t\t\t\tWZD s034p50\t\tMoX/src/Fields.c\t9\tcomment\t\n'
          'only in ReMoM\t\t\t\t\t\tWZD s034p51\tGUI_AddGrid\tMoX/src/Fields.c\t11\tprototype\t\n'
          'only in ReMoM\t\t\t\t\t\tWZD s034p56\tTaken\tMoX/src/Fields.c\t20\tdefinition\t\n'
          'file offset\t0x6\tseg036\t49\tAdd_Grid_Field\tAdd_Grid_Field\tWZD s036p49\tAdd_Grid_Field\tMoX/src/Fields.c\t1\tprototype\t\n'
          'only in ReMoM\t\t\t\t\t\tWZD s034p60\tAdd_Grid_Field\tMoX/src/Fields.c\t30\tdefinition\t\n'
          'only in ReMoM\t\t\t\t\t\tWZD s034p57\tTwice\tMoX/src/Fields.c\t25\tdefinition\t\n')


class Fix_Remom_Ids_test(unittest.TestCase):

    def test_plan_and_apply(self):
        with tempfile.TemporaryDirectory() as root:
            linked = os.path.join(root, 'linked.tsv')
            with open(linked, 'w') as f:
                f.write(LINKED)
            src = os.path.join(root, 'remom', 'MoX', 'src')
            os.makedirs(src)
            c = os.path.join(src, 'Fields.c')
            h = os.path.join(src, 'Fields.h')
            with open(c, 'wb') as f:
                f.write(b'// WZD s034p49\r\nvoid Add_Hot_Key(void)\r\n{\r\n}\r\n// WZD s34p50\r\n// WZD s034p51\r\n// WZD s034p510 is not it\r\n')
            with open(h, 'wb') as f:
                f.write(b'// WZD s034p49\nvoid Add_Hot_Key(void);\n')
            out = io.StringIO()
            with contextlib.redirect_stdout(out):
                fix_remom_ids.main(['--linked', linked, '--remom', os.path.join(root, 'remom')])
            text = out.getvalue()
            with open(c, 'rb') as f:
                self.assertIn(b'// WZD s034p49\r\n', f.read())                 # without --apply nothing is written
            self.assertIn('WZD s034p49 -> WZD s035p02', text)
            self.assertIn('WZD s034p51 -> WZD s035p04', text)                   # found by drake178's name
            self.assertIn('WZD s034p50 -> WZD s035p03', text)                   # comment only: same shift (-47)
            self.assertIn('NOT FIXED WZD s034p56\tTaken is WZD s035p09, which ReMoM already uses for WZD s035p09', text.replace('which ReMoM already uses for WZD s035p09', 'which ReMoM already uses for WZD s035p09'))
            self.assertIn('WZD s034p60 -> WZD s036p49', text)                   # the right id holds the same function
            self.assertIn('NOT FIXED WZD s034p57\tTwice: the database has it at 2 places', text)
            with contextlib.redirect_stdout(io.StringIO()):
                fix_remom_ids.main(['--linked', linked, '--remom', os.path.join(root, 'remom'), '--apply'])
            with open(c, 'rb') as f:
                data = f.read()
            self.assertEqual(data, b'// WZD s035p02\r\nvoid Add_Hot_Key(void)\r\n{\r\n}\r\n// WZD s035p03\r\n// WZD s035p04\r\n// WZD s034p510 is not it\r\n')
            with open(h, 'rb') as f:
                self.assertEqual(f.read(), b'// WZD s035p02\nvoid Add_Hot_Key(void);\n')


if __name__ == '__main__':
    unittest.main()
