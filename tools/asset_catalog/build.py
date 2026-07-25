"""
build.py — scan the game's LBX archives into an asset catalog.

Output columns:
  lbx        archive file name (e.g. MAINSCRN.LBX)
  entry      entry number within the archive
  name       LBX name-table name (may be empty)
  kind       'sprite' (valid FLIC header) or 'other' (palette / sound / data)
  width      sprite width  (blank for 'other')
  height     sprite height (blank for 'other')
  frames     FLIC frame count (blank for 'other')
  offset     byte offset of the entry within the archive
  length     entry length in bytes
"""
import os

from . import lbx

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
ASSETS_DIR = os.path.join(REPO_ROOT, "assets")

COLUMNS = ["lbx", "entry", "name", "kind", "width", "height", "frames", "loop",
           "has_palette", "store", "remap", "offset", "length"]
NUMERIC = {"entry", "width", "height", "frames", "loop", "has_palette", "store", "remap",
           "offset", "length"}
_SPRITE_FIELDS = ("width", "height", "frames", "loop", "has_palette", "store", "remap")


def scan_lbx_file(path):
    with open(path, "rb") as f:
        data = f.read()
    entries = lbx.parse_lbx(data)
    if entries is None:
        return None  # not an LBX archive
    name = os.path.basename(path)
    rows = []
    for entry_num, ename, start, length in entries:
        flic = lbx.parse_flic(data, start, length)
        row = {"lbx": name, "entry": entry_num, "name": ename,
               "offset": start, "length": length}
        if flic is not None:
            row.update(kind="sprite", **flic)
        else:
            row["kind"] = "other"
            for k in _SPRITE_FIELDS:
                row[k] = ""
        rows.append(row)
    return rows


def build_catalog(assets_dir=None):
    assets_dir = assets_dir or ASSETS_DIR
    rows = []
    skipped = []
    for name in sorted(os.listdir(assets_dir)):
        if not name.lower().endswith(".lbx"):
            continue
        path = os.path.join(assets_dir, name)
        got = scan_lbx_file(path)
        if got is None:
            skipped.append(name)
        else:
            rows.extend(got)
    return rows, skipped
