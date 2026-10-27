"""
schema.py -- field layouts and valid value ranges for the LBX data tables, shared by the generator (build.py) and the
checker (check.py).

A Table is a list of Fields at fixed byte offsets inside one fixed-size record.  Each Field knows its storage type
and the values the game code accepts (a numeric range, a set of allowed values, or "NUL-terminated text of at most N
bytes").  The generator draws every value from that range and the writer refuses anything outside it, so a range
mistake fails at build time rather than as a crash inside the game.  Run the checker against the original files to
find ranges that are too narrow:

    python3 -m tools.dummy_lbx --check /path/to/original/LBX/folder

Ranges cite the code that constrains them; see the TABLES dict at the bottom.
"""

import struct

FORMATS = {"i8": "<b", "u8": "<B", "i16": "<h", "u16": "<H", "i32": "<i", "u32": "<I"}
TYPE_LIMITS = {"i8": (-128, 127), "u8": (0, 255), "i16": (-32768, 32767), "u16": (0, 65535),
               "i32": (-2147483648, 2147483647), "u32": (0, 4294967295)}


class RangeError(ValueError):
    pass


class Field:
    """One field of a record.

    kind:   "i8" "u8" "i16" "u16" "i32" "u32" (numbers) or "text" (NUL-terminated ASCII in `size` bytes)
    lo, hi: inclusive valid range (numbers)
    values: explicit set of valid values (overrides lo/hi)
    gen:    optional function(rng, record_index, record_so_far) -> value, for fields whose good values are narrower
            than the valid range or depend on another field; its result is still range-checked
    count:  >1 for an array of `count` elements of `kind` starting at `offset`
    first:  generate this field before the others (other fields' gen functions read it from the record)
    """

    def __init__(self, offset, kind, name, lo=None, hi=None, values=None, size=None, gen=None, count=1, note="", first=False):
        self.offset, self.kind, self.name, self.first = offset, kind, name, first
        self.values = tuple(values) if values is not None else None
        self.size, self.gen, self.count, self.note = size, gen, count, note
        if kind == "text":
            assert size, "%s: text needs a size" % name
        else:
            tlo, thi = TYPE_LIMITS[kind]
            self.lo = tlo if lo is None else lo
            self.hi = thi if hi is None else hi
            assert tlo <= self.lo <= self.hi <= thi, "%s: range %r..%r outside %s" % (name, self.lo, self.hi, kind)

    @property
    def width(self):
        if self.kind == "text":
            return self.size
        return struct.calcsize(FORMATS[self.kind]) * self.count

    def valid(self, value):
        if self.kind == "text":
            return isinstance(value, str) and len(value.encode("ascii")) < self.size and value.isprintable()
        if self.values is not None:
            return value in self.values
        return self.lo <= value <= self.hi

    def describe(self):
        if self.kind == "text":
            return "text, < %d bytes" % self.size
        if self.values is not None:
            return "one of %s" % (list(self.values),)
        return "%d..%d" % (self.lo, self.hi)

    def random_value(self, rng, index, record):
        if self.gen is not None:
            return self.gen(rng, index, record)
        if self.kind == "text":
            return "%s %d" % (self.name, index)
        if self.values is not None:
            return rng.choice(self.values)
        return rng.randint(self.lo, self.hi)

    def pack(self, buf, value, where):
        """Write value (or a list of `count` values) into buf, refusing anything outside the valid range."""
        items = value if self.count > 1 else [value]
        if len(items) != self.count:
            raise RangeError("%s %s: expected %d values, got %d" % (where, self.name, self.count, len(items)))
        if self.kind == "text":
            if not self.valid(value):
                raise RangeError("%s %s: %r is not printable ASCII shorter than %d bytes" % (where, self.name, value, self.size))
            raw = value.encode("ascii")
            buf[self.offset:self.offset + self.size] = raw + bytes(self.size - len(raw))
            return
        step = struct.calcsize(FORMATS[self.kind])
        for i, item in enumerate(items):
            if not self.valid(item):
                raise RangeError("%s %s: %r is outside %s" % (where, self.name, item, self.describe()))
            struct.pack_into(FORMATS[self.kind], buf, self.offset + i * step, item)

    def unpack(self, buf):
        if self.kind == "text":
            raw = bytes(buf[self.offset:self.offset + self.size])
            if b"\x00" not in raw:
                return None                                                # not NUL-terminated: invalid
            return raw.split(b"\x00", 1)[0].decode("ascii", "replace")
        step = struct.calcsize(FORMATS[self.kind])
        items = [struct.unpack_from(FORMATS[self.kind], buf, self.offset + i * step)[0] for i in range(self.count)]
        return items if self.count > 1 else items[0]


class Table:
    """A data table: `record_size`-byte records described by `fields`; bytes no field covers are written as zero."""

    def __init__(self, record_size, fields, count=None, check=None):
        """count: expected number of records (None: whatever the entry holds).  check(record) -> problem text or None,
        for rules that span fields or depend on the record's position (record["_index"])."""
        self.record_size, self.fields, self.count, self.check = record_size, fields, count, check
        covered = bytearray(record_size)
        for f in fields:
            assert f.offset + f.width <= record_size, "%s ends past the %d-byte record" % (f.name, record_size)
            for b in range(f.offset, f.offset + f.width):
                assert not covered[b], "field %s overlaps another field at byte %d" % (f.name, b)
                covered[b] = 1

    def build_record(self, rng, index, where):
        buf = bytearray(self.record_size)
        record = {"_index": index}
        for f in sorted(self.fields, key=lambda f: not f.first):
            if f.count > 1:
                value = [f.random_value(rng, index, record) for _ in range(f.count)]
            else:
                value = f.random_value(rng, index, record)
            f.pack(buf, value, "%s record %d" % (where, index))
            record[f.name] = value
        if self.check is not None:
            problem = self.check(record)
            if problem:
                raise RangeError("%s record %d: %s" % (where, index, problem))
        return bytes(buf)

    def check_record(self, buf, index, where):
        """Problems with one existing record, as a list of strings (empty = valid)."""
        problems = []
        record = {"_index": index}
        for f in self.fields:
            value = f.unpack(buf)
            record[f.name] = value
            items = value if f.count > 1 else [value]
            for i, item in enumerate(items):
                if item is None or not f.valid(item):
                    label = f.name if f.count == 1 else "%s[%d]" % (f.name, i)
                    problems.append("%s record %d: %s = %r, expected %s" % (where, index, label, item, f.describe()))
        if self.check is not None and not problems:
            problem = self.check(record)
            if problem:
                problems.append("%s record %d: %s" % (where, index, problem))
        return problems
