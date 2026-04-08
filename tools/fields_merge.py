#!/usr/bin/env python3
"""
fields_merge.py - merge static field catalog with runtime TRACE.LOG capture.

Inputs:
  tools/fields/_static.csv           (from fields_extract.py)
  TRACE.LOG                          (path on argv; default: ./TRACE.LOG)

Runtime lines recognized (emitted by Fields.c when g_dbg_fields_trace != 0,
and FIELDSNAPSHOT from Dump_Fields_CSV):

  FIELDADD,<screen>,<idx>,<origin>,<basename>:<line>,type=N,x1=N,y1=N,x2=N,y2=N,hotkey=N,help=N
  FIELDSNAPSHOT_BEGIN,<screen>,count=N
  FIELDSNAPSHOT_HDR,...
  FIELDSNAPSHOT,<screen>,<idx>,<type>,<x1>,<y1>,<x2>,<y2>,<hotkey>,<help>,<param1>,<param2>
  FIELDSNAPSHOT_END,<screen>

Output: tools/fields/<screen>.csv with columns:
  screen, src_file, src_line, function, add_func, args, guards,
  rt_seen, rt_type, rt_x1, rt_y1, rt_x2, rt_y2, rt_hotkey, rt_help, rt_origin, rt_hit_count

Rows are joined on (basename(src_file), src_line) matching the runtime
FIELDADD <basename>:<line>. A static row with rt_seen=0 was not hit this run
(likely conditional - see guards column). A runtime row with no static match
is emitted as a trailing "runtime_only" row so it is still visible.
"""
import csv
import os
import re
import sys
from collections import defaultdict

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
STATIC_CSV = os.path.join(REPO_ROOT, "tools", "fields", "_static.csv")
OUT_DIR = os.path.join(REPO_ROOT, "tools", "fields")

# Subset of enum e_SimTex_Key_Codes in MoX/src/MOX_KEYS.h. Extend as needed.
HOTKEY_NAMES = {
    0x01: "LEFT", 0x02: "RIGHT", 0x03: "UP", 0x04: "DOWN",
    0x05: "RIGHTUP", 0x06: "RIGHTDOWN", 0x07: "LEFTUP", 0x08: "LEFTDOWN",
    0x0A: "DELETE", 0x0B: "BACKSPACE", 0x0C: "ENTER", 0x0D: "TAB",
    0x0E: "F1", 0x0F: "F2", 0x10: "F3", 0x11: "F4", 0x12: "F5",
    0x13: "F6", 0x14: "F7", 0x15: "F8", 0x16: "F9", 0x17: "F10",
    0x1B: "ESCAPE", 0x91: "F11", 0x92: "F12",
}

def hotkey_name(hk):
    try:
        v = int(hk)
    except (TypeError, ValueError):
        return ""
    if v == 0 or v == -1:
        return ""
    if v in HOTKEY_NAMES:
        return HOTKEY_NAMES[v]
    if 0x20 <= v <= 0x7E:
        return chr(v)
    if 0xA0 <= v <= 0xB9:
        return "ALT_" + chr(ord('A') + (v - 0xA0))
    return f"0x{v:02X}"

FIELDADD_RE = re.compile(
    r"FIELDADD,(?P<screen>[^,]*),(?P<idx>-?\d+),(?P<origin>[^,]*),(?P<basename>[^:]+):(?P<line>\d+),"
    r"type=(?P<type>-?\d+),x1=(?P<x1>-?\d+),y1=(?P<y1>-?\d+),x2=(?P<x2>-?\d+),y2=(?P<y2>-?\d+),"
    r"hotkey=(?P<hotkey>-?\d+),help=(?P<help>-?\d+)"
)
SNAPBEGIN_RE = re.compile(r"FIELDSNAPSHOT_BEGIN,(?P<screen>[^,]*),count=(?P<count>\d+)")
SNAPROW_RE = re.compile(
    r"FIELDSNAPSHOT,(?P<screen>[^,]*),(?P<idx>-?\d+),(?P<type>-?\d+),"
    r"(?P<x1>-?\d+),(?P<y1>-?\d+),(?P<x2>-?\d+),(?P<y2>-?\d+),"
    r"(?P<hotkey>-?\d+),(?P<help>-?\d+),(?P<param1>-?\d+),(?P<param2>-?\d+)"
)


def load_static():
    rows = []
    with open(STATIC_CSV, "r", encoding="utf-8") as f:
        r = csv.DictReader(f)
        for row in r:
            row["src_basename"] = os.path.basename(row["file"])
            rows.append(row)
    return rows


def parse_trace(path):
    """Return: (fieldadd_by_key, snapshots_by_screen, fieldadd_list)."""
    fieldadd_by_key = defaultdict(list)  # (basename, line) -> [dict,...]
    fieldadd_list = []
    snapshots = defaultdict(list)  # screen -> [row dict,...]
    cur_snapshot_screen = None
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            m = FIELDADD_RE.search(line)
            if m:
                d = m.groupdict()
                fieldadd_list.append(d)
                fieldadd_by_key[(d["basename"], int(d["line"]))].append(d)
                continue
            m = SNAPBEGIN_RE.search(line)
            if m:
                cur_snapshot_screen = m.group("screen")
                # Reset (keep only the latest snapshot per screen).
                snapshots[cur_snapshot_screen] = []
                continue
            m = SNAPROW_RE.search(line)
            if m:
                snapshots[m.group("screen")].append(m.groupdict())
                continue
            if line.startswith("FIELDSNAPSHOT_END"):
                cur_snapshot_screen = None
    return fieldadd_by_key, snapshots, fieldadd_list


def screen_for_static_row(row):
    """Heuristic: use the enclosing function name, trimming common helper suffixes."""
    fn = row["function"]
    # Known helpers that feed Main_Screen. Extend as other screens are added.
    helper_to_screen = {
        "Add_Game_Button_Fields": "Main_Screen",
        "Add_Unit_Window_Fields": "Main_Screen",
        "Add_Unit_Action_Fields": "Main_Screen",
        "Main_Screen_Add_Fields": "Main_Screen",
    }
    return helper_to_screen.get(fn, fn)


def merge(screen_filter=None):
    static_rows = load_static()
    trace_path = sys.argv[1] if len(sys.argv) > 1 else os.path.join(REPO_ROOT, "TRACE.LOG")
    if not os.path.isfile(trace_path):
        print(f"warning: trace file not found: {trace_path}", file=sys.stderr)
        fieldadd_by_key, snapshots, fieldadd_list = {}, {}, []
    else:
        fieldadd_by_key, snapshots, fieldadd_list = parse_trace(trace_path)
        print(f"Loaded {len(fieldadd_list)} FIELDADD and {sum(len(v) for v in snapshots.values())} FIELDSNAPSHOT rows from {trace_path}")

    os.makedirs(OUT_DIR, exist_ok=True)

    # Bucket static rows by inferred screen.
    by_screen = defaultdict(list)
    for r in static_rows:
        r["screen"] = screen_for_static_row(r)
        by_screen[r["screen"]].append(r)

    matched_runtime_keys = set()
    screens = [screen_filter] if screen_filter else sorted(by_screen.keys())
    for screen in screens:
        rows = by_screen.get(screen, [])
        if not rows and screen_filter is None:
            continue
        out_path = os.path.join(OUT_DIR, f"{screen}.csv")
        with open(out_path, "w", newline="", encoding="utf-8") as f:
            w = csv.writer(f)
            w.writerow([
                "screen", "src_file", "src_line", "function", "add_func", "symbol", "args", "guards",
                "rt_seen", "rt_hit_count", "rt_type", "rt_x1", "rt_y1", "rt_x2", "rt_y2",
                "rt_click_cx", "rt_click_cy", "rt_hotkey", "rt_hotkey_name", "rt_help", "rt_origin",
            ])
            for r in rows:
                key = (r["src_basename"], int(r["line"]))
                hits = fieldadd_by_key.get(key, [])
                if hits:
                    matched_runtime_keys.add(key)
                    h0 = hits[0]
                    cx = (int(h0["x1"]) + int(h0["x2"])) // 2
                    cy = (int(h0["y1"]) + int(h0["y2"])) // 2
                    w.writerow([
                        screen, r["file"], r["line"], r["function"], r["add_func"], r.get("symbol", ""), r["args"], r["guards"],
                        1, len(hits), h0["type"], h0["x1"], h0["y1"], h0["x2"], h0["y2"],
                        cx, cy, h0["hotkey"], hotkey_name(h0["hotkey"]), h0["help"], h0["origin"],
                    ])
                else:
                    w.writerow([
                        screen, r["file"], r["line"], r["function"], r["add_func"], r.get("symbol", ""), r["args"], r["guards"],
                        0, 0, "", "", "", "", "", "", "", "", "", "", "",
                    ])
            # Runtime-only rows: FIELDADDs for this screen tag that never matched a static row.
            for add in fieldadd_list:
                if add["screen"] != screen:
                    continue
                key = (add["basename"], int(add["line"]))
                if key in matched_runtime_keys:
                    continue
                # Try to match to any static row by basename+line ignoring screen bucket
                # (covers helpers not yet in helper_to_screen).
                if key in fieldadd_by_key and any((sr["src_basename"], int(sr["line"])) == key for sr in static_rows):
                    continue
                cx = (int(add["x1"]) + int(add["x2"])) // 2
                cy = (int(add["y1"]) + int(add["y2"])) // 2
                w.writerow([
                    screen, f"(runtime-only) {add['basename']}", add["line"], "", add["origin"], "", "", "",
                    1, 1, add["type"], add["x1"], add["y1"], add["x2"], add["y2"],
                    cx, cy, add["hotkey"], hotkey_name(add["hotkey"]), add["help"], add["origin"],
                ])
        # Stats
        total = len(rows)
        seen = sum(1 for r in rows if (r["src_basename"], int(r["line"])) in fieldadd_by_key)
        print(f"  {screen}: {total} static rows, {seen} hit at runtime -> {out_path}")


if __name__ == "__main__":
    merge()
