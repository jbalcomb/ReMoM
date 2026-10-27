"""
check.py -- validate a folder of .LBX files against the catalog (entry counts, picture sizes) and the data-table
ranges in tables.py.

    python3 -m tools.dummy_lbx --check FOLDER

Works on generated files and on the original game's files alike.  On the originals, a reported problem means either
the original data really holds a value the code mishandles, or a range in tables.py is too narrow -- read the cited
code before widening it.
"""

import os
import struct

from tools.asset_catalog import lbx as lbx_parser

from .tables import RECORD_ARCHIVES, TABLES


def check_data_entry(table, data, where):
    if len(data) < 4:
        return ["%s: %d bytes, too short for a (count, size) header" % (where, len(data))]
    count, size = struct.unpack_from("<hh", data, 0)
    problems = []
    if size != table.record_size:
        return ["%s: header record size %d, expected %d" % (where, size, table.record_size)]
    if table.count is not None and count != table.count:
        problems.append("%s: header record count %d, expected %d" % (where, count, table.count))
    if 4 + count * size > len(data):
        return problems + ["%s: %d records of %d bytes do not fit in %d bytes" % (where, count, size, len(data))]
    for index in range(count):
        record = data[4 + index * size: 4 + (index + 1) * size]
        problems += table.check_record(record, index, where)
    return problems


def check_flic(row, data, where):
    if len(data) < 0x12:
        return ["%s: %d bytes, too short for a FLIC header" % (where, len(data))]
    width, height, _, frames = struct.unpack_from("<hhhh", data, 0)
    expected = (int(row["width"]), int(row["height"]), int(row["frames"]))
    if (width, height, frames) != expected:
        return ["%s: picture %dx%d x%d frames, catalog says %dx%d x%d" % ((where, width, height, frames) + expected)]
    end = 0x12 + 4 * (frames + 1)
    if end > len(data):
        return ["%s: frame offset table runs past the entry" % where]
    offsets = struct.unpack_from("<%dI" % (frames + 1), data, 0x12)
    if any(o > len(data) for o in offsets):
        return ["%s: a frame offset points past the entry" % where]
    return []


def check_folder(folder, archives):
    """Returns (problems, counts) for every catalogued archive found in `folder`."""
    problems = []
    counts = {"archives": 0, "tables": 0, "records": 0, "pictures": 0, "missing": 0}
    for name in sorted(archives):
        path = os.path.join(folder, name)
        if not os.path.exists(path):
            counts["missing"] += 1
            continue
        counts["archives"] += 1
        with open(path, "rb") as fp:
            blob = fp.read()
        entries = lbx_parser.parse_lbx(blob)
        if entries is None:
            problems.append("%s: not an LBX archive" % name)
            continue
        rows = archives[name]
        if len(entries) != len(rows):
            problems.append("%s: %d entries, catalog says %d" % (name, len(entries), len(rows)))
        for (entry, _, start, length), row in zip(entries, rows):
            data = blob[start:start + length]
            where = "%s entry %d" % (name, entry)
            if length == 0:
                continue
            table = TABLES.get((name, entry)) or RECORD_ARCHIVES.get(name)
            if table is not None:
                counts["tables"] += 1
                counts["records"] += max(0, (length - 4) // table.record_size)
                problems += check_data_entry(table, data, where)
            elif row["kind"] == "sprite" and name != "TERRAIN.LBX":
                counts["pictures"] += 1
                problems += check_flic(row, data, where)
    return problems, counts
