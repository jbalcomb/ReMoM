"""
runtime.py — harvest field pixel geometry from RECORD.log sidecars.

The engine's replay field-hit log (Phase 2) stamps each hit with both the field's
runtime rect and its Add_* origin:

    idx=6  ...  field[3]=(0,20)-(239,199)@MainScr.c:2019

That is the authority for fields the static catalog flags `runtime=1` (sprite
buttons, popup-relative and loop-registered fields) — they have no static click
point, but the log captured their real rect. This module accumulates
`basename:line -> (x1,y1,x2,y2)` from one or more RECORD.logs into
tools/fields/runtime_geometry.fwv, which the resolver overlays on the static
catalog.
"""
import os
import re

from . import fwv

RUNTIME_FWV = os.path.join(os.path.dirname(__file__), "..", "fields", "runtime_geometry.fwv")
RUNTIME_COLUMNS = ["origin", "x1", "y1", "x2", "y2"]
_RUNTIME_NUMERIC = {"x1", "y1", "x2", "y2"}

# field[N]=(x1,y1)-(x2,y2)@basename:line
_HIT_RE = re.compile(r"field\[\d+\]=\((-?\d+),(-?\d+)\)-\((-?\d+),(-?\d+)\)@(\S+)")


def parse_record_log(path):
    """Return {origin: (x1, y1, x2, y2)} from a RECORD.log. Last hit per origin wins
    within a single log (rects for a given field are stable across its frames)."""
    rects = {}
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            m = _HIT_RE.search(line)
            if not m:
                continue
            x1, y1, x2, y2 = (int(m.group(i)) for i in range(1, 5))
            origin = m.group(5).strip()
            rects[origin] = (x1, y1, x2, y2)
    return rects


def load_runtime(path=None):
    """Load the accumulated runtime geometry table -> {origin: (x1,y1,x2,y2)}."""
    path = path or RUNTIME_FWV
    if not os.path.isfile(path):
        return {}
    out = {}
    for r in fwv.read(path):
        try:
            out[r["origin"]] = (int(r["x1"]), int(r["y1"]), int(r["x2"]), int(r["y2"]))
        except (KeyError, ValueError):
            continue
    return out


def merge_logs(log_paths, out=None):
    """Accumulate rects from log_paths into the runtime geometry table (fill-only:
    existing origins are kept, new ones added). Returns (added, total)."""
    out = out or RUNTIME_FWV
    existing = load_runtime(out)
    added = 0
    for lp in log_paths:
        for origin, rect in parse_record_log(lp).items():
            if origin not in existing:
                existing[origin] = rect
                added += 1
    rows = [{"origin": o, "x1": r[0], "y1": r[1], "x2": r[2], "y2": r[3]}
            for o, r in sorted(existing.items())]
    fwv.write(rows, RUNTIME_COLUMNS, _RUNTIME_NUMERIC, os.path.abspath(out))
    return added, len(existing)


def _main():
    import sys
    if len(sys.argv) < 2:
        print("usage: python -m tools.field_catalog.runtime <RECORD.log> [...]")
        return 1
    added, total = merge_logs(sys.argv[1:])
    print(f"merged: +{added} new origins, {total} total -> {os.path.abspath(RUNTIME_FWV)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(_main())
