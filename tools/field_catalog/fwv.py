"""
fwv.py — read/write fixed-width values files.

Columns are padded to a constant width and joined by a 2-space gutter; the final
column is left unpadded (a free-text tail). Numeric columns are right-aligned in
the data; **headers are always left-aligned** so each header name's start offset
marks its column boundary — that is how read() recovers columns even when a
numeric cell is empty (empty cells would otherwise merge into the gutter and
defeat whitespace splitting).
"""

_GUTTER = "  "


def write(rows, columns, numeric, path):
    cells = [[str(r.get(c, "")) for c in columns] for r in rows]
    widths = [len(c) for c in columns]
    for row in cells:
        for i, v in enumerate(row):
            if len(v) > widths[i]:
                widths[i] = len(v)
    last = len(columns) - 1

    def fmt(values, header):
        parts = []
        for i, v in enumerate(values):
            if i == last:
                parts.append(v)  # free-text tail: no right padding
            elif header or columns[i] not in numeric:
                parts.append(v.ljust(widths[i]))
            else:
                parts.append(v.rjust(widths[i]))
        return _GUTTER.join(parts).rstrip()

    with open(path, "w", newline="\n", encoding="utf-8") as f:
        f.write(fmt(list(columns), header=True) + "\n")
        for row in cells:
            f.write(fmt(row, header=False) + "\n")


def read(path):
    """Parse a fixed-width file written by write() into a list of dicts (all str)."""
    with open(path, "r", encoding="utf-8") as f:
        lines = f.read().splitlines()
    if not lines:
        return []
    header = lines[0]
    names = header.split()  # header cells are non-empty and space-free
    offsets = []
    cur = 0
    for n in names:
        idx = header.index(n, cur)
        offsets.append(idx)
        cur = idx + len(n)
    rows = []
    for ln in lines[1:]:
        if not ln.strip():
            continue
        rec = {}
        for i, n in enumerate(names):
            start = offsets[i]
            end = offsets[i + 1] if i + 1 < len(offsets) else len(ln)
            rec[n] = ln[start:end].strip()
        rows.append(rec)
    return rows
