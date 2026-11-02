"""
test_ida_help.py -- ida_help.py on a small decompiled-help folder.

    python3 -m unittest tools.ida_help.tests.test_ida_help
"""

import contextlib
import csv
import io
import os
import tempfile
import unittest

from tools.ida_help import ida_help

PAGE = '''<html><head><title>Segment functions</title><style>p { color: red }</style></head><body>
<h2>GetSegmentAttr</h2>
<pre>
// get segment attribute
long    GetSegmentAttr  (long segea,long attr);
</pre>
<p>Returns the   attribute of
a segment.</p>
<pre>long    SegStart        (long ea);              // returns start of the segment</pre>
<p>Start &amp; end of the segment; see the file offset of a byte.</p>
</body></html>'''


class Ida_Help_test(unittest.TestCase):

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.html = os.path.join(self.tmp.name, 'html')
        os.makedirs(os.path.join(self.html, 'idc'))
        with open(os.path.join(self.html, 'idc', 'segments.htm'), 'w', encoding='cp1252') as f:
            f.write(PAGE)
        self.out = os.path.join(self.tmp.name, 'out')
        with contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(ida_help.main(['--html', self.html, '--out', self.out]), 0)

    def tearDown(self):
        self.tmp.cleanup()

    def test_page_text(self):
        with open(os.path.join(self.out, 'text', 'idc', 'segments.txt'), encoding='utf-8') as f:
            text = f.read()
        self.assertIn('Returns the attribute of a segment.', text)
        self.assertIn('long    GetSegmentAttr  (long segea,long attr);', text)     # <pre> keeps its layout
        self.assertIn('Start & end of the segment', text)
        self.assertNotIn('color: red', text)

    def test_indexes(self):
        with open(os.path.join(self.out, 'pages.tsv'), newline='') as f:
            self.assertEqual(next(csv.DictReader(f, delimiter='\t'))['title'], 'Segment functions')
        with open(os.path.join(self.out, 'idc_functions.tsv'), newline='') as f:
            rows = {r['function']: r for r in csv.DictReader(f, delimiter='\t')}
        self.assertEqual((rows['GetSegmentAttr']['returns'], rows['GetSegmentAttr']['arguments']), ('long', 'long segea,long attr'))
        self.assertEqual(rows['GetSegmentAttr']['description'], 'Returns the attribute of a segment.')
        self.assertEqual(rows['SegStart']['page'], 'idc/segments.htm')

    def test_grep(self):
        buf = io.StringIO()
        with contextlib.redirect_stdout(buf):
            ida_help.main(['--out', self.out, '--grep', 'file offset'])
        self.assertIn('idc/segments.txt:', buf.getvalue())
        self.assertIn('[Segment functions]', buf.getvalue())
        self.assertIn('1 matching lines', buf.getvalue())


if __name__ == '__main__':
    unittest.main()
