"""
test_diff.py -- mom_patch_diff on synthetic files: a small MZ executable with a map, and a SPELLDAT.LBX.

    python3 -m unittest tools.mom_patch_diff.tests.test_diff
"""

import os
import random
import struct
import tempfile
import unittest

from tools.dummy_lbx.tables import TABLES
from tools.mom_patch_diff.diff import main

HEADER = 32                 # MZ header: 2 paragraphs
BASE_EA = 0x10000           # where the fake "IDA" put the load module


def function_body(rng, size, fixup_at=None):
    body = bytearray([0x55, 0x8B, 0xEC]) + bytearray(rng.randrange(256) for _ in range(size - 4)) + bytearray([0xCB])
    if fixup_at is not None:
        body[fixup_at:fixup_at + 2] = b'\x00\x00'
    return body


def build_exe(blocks):
    image = b''.join(blocks)
    total = HEADER + len(image)
    header = bytearray(HEADER)
    struct.pack_into('<2s13H', header, 0, b'MZ', total % 512, (total + 511) // 512, 0, HEADER // 16, 0, 0xFFFF, 0, 0, 0, 0, 0, 0x1C, 0)
    return bytes(header) + image


def map_rows(layout):
    """layout: [(kind, segment, proc, name, offset_in_image, bytes, fixup_at)]"""
    lines = ['kind\tsegment\tproc\tname\tea\tsize\tbytes']
    for kind, seg, proc, name, off, data, fixup_at in layout:
        hexs = ''.join('..' if fixup_at is not None and fixup_at <= i < fixup_at + 2 else '%02X' % b for i, b in enumerate(data))
        lines.append('%s\t%s\t%d\t%s\t%X\t%d\t%s' % (kind, seg, proc, name, BASE_EA + off, len(data), hexs))
    return '\n'.join(lines) + '\n'


def build_lbx(entry):
    offset = 0x200
    buf = bytearray(offset)
    struct.pack_into('<HHHH', buf, 0, 1, 0xFEAD, 0, 0)
    struct.pack_into('<II', buf, 8, offset, offset + len(entry))
    return bytes(buf) + entry


def spelldat(change=None):
    table = TABLES[('SPELLDAT.LBX', 0)]
    rec = bytearray(4 + 215 * table.record_size)
    struct.pack_into('<hh', rec, 0, 215, table.record_size)
    for i in range(215):
        name = ('Spell%d' % i).encode()
        rec[4 + i * table.record_size:4 + i * table.record_size + len(name)] = name
    if change:
        index, field_name, value = change
        f = next(f for f in table.fields if f.name == field_name)
        f.pack(memoryview(rec)[4 + index * table.record_size:], value, 'test')
    return bytes(rec)


class Mom_Patch_Diff_test(unittest.TestCase):

    def setUp(self):
        rng = random.Random(7)
        self.tmp = tempfile.TemporaryDirectory()
        root = self.tmp.name
        self.old_dir, self.new_dir, self.out = (os.path.join(root, d) for d in ('v131', 'v160', 'report'))
        os.makedirs(self.old_dir)
        os.makedirs(self.new_dir)

        f1 = function_body(rng, 64)
        f2 = function_body(rng, 80, fixup_at=20)             # has a relocation fixup
        f3 = function_body(rng, 48)
        data = bytearray(struct.pack('<10h', *range(10)))     # a named data item: 10 int16 values
        pad = bytearray(16)

        # 1.31: f1 f2 pad data f3
        old_blocks = [f1, f2, pad, data, f3]
        offsets = [0, 64, 144, 160, 180]
        layout = [('F', 'seg001', 1, 'TILE_GetProd', offsets[0], f1, None),
                  ('F', 'seg001', 2, 'CTY_GetGold', offsets[1], f2, 20),
                  ('D', 'dseg', 0, '_production_table', offsets[3], data, None),
                  ('F', 'ovr142', 1, 'CTY_GetResearch', offsets[4], f3, None)]

        # "1.60": f2 has one changed instruction byte and its fixup slot holds a different (unrelocated) value,
        # data item value 3 -> 7, f3 grows by 4 bytes (so the file size differs)
        f2_new = bytearray(f2)
        f2_new[30] ^= 0xFF
        f2_new[20:22] = b'\x12\x34'                           # fixup: must not count as a change
        data_new = bytearray(data)
        struct.pack_into('<h', data_new, 6, 7)
        f3_new = bytearray(f3[:-1]) + bytearray([0x90, 0x90, 0x90, 0x90, 0xCB])
        f2_old_file = bytearray(f2)
        f2_old_file[20:22] = b'\x56\x78'
        new_blocks = [f1, f2_new, pad, data_new, f3_new]
        old_blocks[1] = f2_old_file

        with open(os.path.join(self.old_dir, 'WIZARDS.EXE'), 'wb') as f:
            f.write(build_exe(old_blocks))
        with open(os.path.join(self.new_dir, 'WIZARDS.EXE'), 'wb') as f:
            f.write(build_exe(new_blocks))
        self.map_path = os.path.join(root, 'wizards_map.tsv')
        with open(self.map_path, 'w') as f:
            f.write(map_rows(layout))

        with open(os.path.join(self.old_dir, 'SPELLDAT.LBX'), 'wb') as f:
            f.write(build_lbx(spelldat()))
        with open(os.path.join(self.new_dir, 'SPELLDAT.LBX'), 'wb') as f:
            f.write(build_lbx(spelldat(change=(45, 'casting_cost', 40))))
        for d, text in ((self.old_dir, 'a\nb\n'), (self.new_dir, 'a\nc\n')):
            with open(os.path.join(d, 'README.TXT'), 'w') as f:
                f.write(text)
        with open(os.path.join(self.old_dir, 'SAME.DAT'), 'wb') as f:
            f.write(b'\x01\x02')
        with open(os.path.join(self.new_dir, 'SAME.DAT'), 'wb') as f:
            f.write(b'\x01\x02')

        self.assertEqual(main([self.old_dir, self.new_dir, '--out', self.out, '--map', 'WIZARDS.EXE=' + self.map_path,
                               '--old-label', '1.31', '--new-label', '1.60']), 0)

    def tearDown(self):
        self.tmp.cleanup()

    def read(self, *parts):
        with open(os.path.join(self.out, *parts), encoding='utf-8') as f:
            return f.read()

    def test_summary_lists_every_file(self):
        summary = self.read('summary.md')
        self.assertIn('| WIZARDS.EXE | changed |', summary)
        self.assertIn('| SPELLDAT.LBX | changed |', summary)
        self.assertIn('| README.TXT | changed |', summary)
        self.assertIn('| SAME.DAT | same |', summary)

    def test_changed_functions_are_named_and_unchanged_ones_are_not(self):
        functions = self.read('WIZARDS.EXE', 'functions.md')
        self.assertIn('| s001p02 | CTY_GetGold |', functions)
        self.assertIn('| o142p01 | CTY_GetResearch |', functions)
        self.assertNotIn('TILE_GetProd', functions)
        self.assertIn('3 functions in the map: 1 unchanged', functions)

    def test_function_detail_shows_the_changed_byte_and_labels_the_fixup(self):
        detail = self.read('WIZARDS.EXE', 'functions', 's001p02.txt')
        self.assertIn('+0x001E', detail)                       # byte 30 changed
        self.assertIn('fixup   +0x0014', detail)              # the relocation slot at 20 is labelled, not counted as code

    def test_changed_data_item_shows_values(self):
        data = self.read('WIZARDS.EXE', 'data.md')
        self.assertIn('| _production_table | dseg |', data)
        self.assertIn('+6: 3 -> 7', data)

    def test_lbx_table_is_decoded_by_field(self):
        lbx = self.read('SPELLDAT.LBX.md')
        self.assertIn('| 45 | Spell45 | casting_cost |', lbx)

    def test_text_files_get_a_unified_diff(self):
        diff = self.read('README.TXT.diff')
        self.assertIn('-b', diff)
        self.assertIn('+c', diff)


if __name__ == '__main__':
    unittest.main()
