#!/usr/bin/env python3
"""Read a Borland C++ 3.x MZ executable with VROOMM overlays without a disassembler.

What it recovers, and how:
  - DGROUP (initialized data) base: the 4 zero bytes before "Borland C++ - Copyright", paragraph aligned.
  - Resident code segments: the distinct segment values that the MZ relocation table fixes up, below the overlay stubs.
    Under the medium model each .c file is its own code segment, so these are module boundaries.
  - Overlays: the stub chain (`INT 3Fh` stubs, 0x20-byte header + 5 bytes per entry, paragraph rounded) between the
    resident code and DGROUP.  Overlay k's code is at  load_end + 16 + stub.fileoff,  stub.codesize bytes long.
    Numbering: overlay k = the k-th stub in the chain (IDA's ovrNNN numbers are a fixed offset of this; see verify).
  - DGROUP strings: NUL-terminated printable runs, with their DS offsets.
  - References to a string: an immediate operand equal to its DS offset in code  (mov r16,imm = B8..BF;
    push imm = 68; mov word [bp+d8],imm = C7 46 d8; mov word [abs],imm = C7 06 lo hi).  Medium model = near data,
    so a literal's address is a 16-bit DS offset.  A random 3-byte match is possible; verify_on_mom() measures it.

stdlib only.
"""

import re
import struct

COPYRIGHT = b'Borland C++ - Copyright'


class BorlandExe:
    def __init__(self, path):
        self.path = path
        with open(path, 'rb') as f:
            self.data = data = f.read()
        if data[:2] != b'MZ':
            raise ValueError(f'{path}: not an MZ executable')
        cblp, cp, crlc, cparhdr = struct.unpack_from('<HHHH', data, 2)
        lfarlc = struct.unpack_from('<H', data, 0x18)[0]
        self.hdr = cparhdr * 16
        self.load_end = (cp - 1) * 512 + (cblp or 512)
        relocs = [struct.unpack_from('<HH', data, lfarlc + 4 * i) for i in range(crlc)]
        i = data.find(COPYRIGHT)
        if i < 0 or (i - 4 - self.hdr) % 16:
            raise ValueError(f'{path}: Borland DGROUP marker not found / not paragraph aligned')
        self.ds_base = i - 4                                       # file offset of DS:0000
        self.stubs = self._stub_chain()
        stub_start = self.stubs[0]['file_off'] if self.stubs else self.ds_base
        # Resident code segments = segment values patched by relocations, that land below the stubs.
        segs = set()
        for off, seg in relocs:
            pos = self.hdr + seg * 16 + off
            if self.hdr <= pos + 2 <= self.load_end:
                val = struct.unpack_from('<H', data, pos)[0]
                fpos = self.hdr + val * 16
                if self.hdr <= fpos < stub_start:
                    segs.add(fpos)
        segs.add(self.hdr)
        starts = sorted(segs)
        self.resident = [{'name': f's{k:02d}', 'start': s, 'end': (starts[k + 1] if k + 1 < len(starts) else stub_start)} for k, s in enumerate(starts)]
        self.overlays = []
        for k, st in enumerate(self.stubs):
            start = self.load_end + 16 + st['fileoff']
            self.overlays.append({'name': f'o{k:02d}', 'start': start, 'end': start + st['codesize'], 'stub': st})
        self.code_regions = self.resident + self.overlays
        self.ds_end = self.load_end   # DGROUP runs to the end of the load image (BSS/stack are not in the file)

    def _stub_chain(self):
        data = self.data
        first = None
        for off in range(self.hdr, self.ds_base, 16):
            if data[off:off + 2] == b'\xCD\x3F':
                fo, cs, rs, ne = struct.unpack_from('<IHHH', data, off + 4)
                if fo == 0 and cs > 0:
                    first = off
                    break
        stubs = []
        off = first
        while off is not None and off < self.ds_base and data[off:off + 2] == b'\xCD\x3F':
            fo, cs, rs, ne = struct.unpack_from('<IHHH', data, off + 4)
            stubs.append({'file_off': off, 'fileoff': fo, 'codesize': cs, 'relsize': rs, 'nentries': ne})
            off += (0x20 + 5 * ne + 15) // 16 * 16
        return stubs

    def strings(self, min_len=3):
        """All NUL-terminated printable strings in DGROUP: [(ds_offset, text)] in DGROUP order."""
        out = []
        blob = self.data[self.ds_base:self.ds_end]
        for m in re.finditer(rb'[\x20-\x7e\x09\x0a\x0d]{%d,}(?=\x00)' % min_len, blob):
            out.append((m.start(), m.group(0).decode('latin-1')))
        return out

    def file_name_literals(self, known_stems=()):
        """DGROUP strings that end in an 8.3 file name: [(ds_offset, literal)].
        A printable run can carry junk bytes in front of the real string start (WIZARDS.EXE: 'bbbbLOAD.LBX'), so every
        start inside the name part is a candidate; the one that code references wins, then a known stem, then the run."""
        tail = re.compile(r'([A-Za-z0-9_]{1,8})(\.[A-Za-z]{3})$')
        known = {s.upper() for s in known_stems}
        runs = []
        for off, text in self.strings(1):
            m = tail.search(text)
            if not m:
                continue
            stem_start = m.start(1)
            # the name part may continue to the left of the regex match (more than 8 name characters in the run)
            while stem_start > 0 and re.match(r'[A-Za-z0-9_]', text[stem_start - 1]):
                stem_start -= 1
            cands = [(off + k, text[k:]) for k in range(stem_start, m.start(2)) if m.start(2) - k <= 8]
            runs.append(cands)
        refs = self.references([o for cands in runs for o, _ in cands])
        out = []
        for cands in runs:
            pick = next((c for c in cands if refs.get(c[0])), None)
            if pick is None:
                pick = next((c for c in cands if c[1].rsplit('.', 1)[0].upper() in known), None)
            if pick is None:
                pick = cands[0]
            out.append(pick)
        return out

    def region_of(self, file_pos):
        for r in self.code_regions:
            if r['start'] <= file_pos < r['end']:
                return r['name']
        return None

    def references(self, ds_offsets):
        """{ds_offset: Counter(region name)} for every immediate operand equal to one of ds_offsets, in code only."""
        import collections
        want = set(ds_offsets)
        refs = collections.defaultdict(collections.Counter)
        data = self.data
        for r in self.code_regions:
            seg = data[r['start']:r['end']]
            for m in re.finditer(rb'(?=([\xB8-\xBF\x68])(..))', seg, re.S):
                v = struct.unpack('<H', m.group(2))[0]
                if v in want:
                    refs[v][r['name']] += 1
            for m in re.finditer(rb'(?=\xC7\x46.(..))', seg, re.S):
                v = struct.unpack('<H', m.group(1))[0]
                if v in want:
                    refs[v][r['name']] += 1
            for m in re.finditer(rb'(?=\xC7\x06..(..))', seg, re.S):
                v = struct.unpack('<H', m.group(1))[0]
                if v in want:
                    refs[v][r['name']] += 1
        return refs
