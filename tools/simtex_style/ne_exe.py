#!/usr/bin/env python3
"""Read a Microsoft C "New Executable" (NE) as used by the Starlords preview (1993): an MZ stub loader
("Not enough memory to load program" / "DGROUP greater than 64k") plus an NE image with a segment table.

Same interface as borland_exe.BorlandExe, so the string / reference code is shared:
  - code_regions: one per NE code segment, named cNN (1-based NE segment number).  The Microsoft linker can pack
    several modules into one segment (/PACKCODE), so a region is a segment, not necessarily a source file.
  - ds_base / ds_end: the automatic data segment (DGROUP).  The MS C runtime puts its copyright 8 bytes in.
  - strings(), file_name_literals(), references(): inherited unchanged.

stdlib only.
"""

import struct

import borland_exe as be


class NeExe(be.BorlandExe):
    def __init__(self, path):  # noqa: super().__init__ is Borland-specific; only the shared methods are inherited
        self.path = path
        with open(path, 'rb') as f:
            self.data = data = f.read()
        ne = struct.unpack_from('<I', data, 0x3C)[0]
        if data[:2] != b'MZ' or data[ne:ne + 2] != b'NE':
            raise ValueError(f'{path}: not an NE executable')
        self.ne = ne
        autodata = struct.unpack_from('<H', data, ne + 0x0E)[0]
        cseg = struct.unpack_from('<H', data, ne + 0x1C)[0]
        segtab = struct.unpack_from('<H', data, ne + 0x22)[0]
        align = struct.unpack_from('<H', data, ne + 0x32)[0]
        self.segments = []
        for i in range(cseg):
            so, sl, sf, sm = struct.unpack_from('<HHHH', data, ne + segtab + 8 * i)
            start = so << align
            self.segments.append({'num': i + 1, 'start': start, 'end': start + (sl or 65536), 'data': bool(sf & 1), 'flags': sf})
        ds = self.segments[autodata - 1]
        self.ds_base = ds['start']
        self.ds_end = ds['end']
        self.code_regions = [{'name': f'c{s["num"]:02d}', 'start': s['start'], 'end': s['end']} for s in self.segments if not s['data']]
        self.resident = self.code_regions
        self.overlays = []
        self.stub_strings = [t for _, t in self._stub_strings()]

    def _stub_strings(self):
        import re
        stub = self.data[:self.ne]
        return [(m.start(), m.group(0).decode('latin-1')) for m in re.finditer(rb'[\x20-\x7e]{5,}', stub)]
