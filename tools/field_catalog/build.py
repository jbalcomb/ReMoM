"""
build.py — turn scanned Add_*Field call sites into catalog rows with resolved
320x200 geometry and decoded hotkeys, flagging what still needs the runtime log.

A row's clickable geometry is resolvable statically when every coordinate arg
folds to a constant (constants.Folder). Otherwise runtime=1 and the runtime
FIELDADD log is the authority for the pixel rect. Hot-key registrations carry
no geometry — they are addressable by key, so runtime=0 for them.
"""
import re

from . import fwv
from .constants import Folder
from .scan import scan_repo
from .signatures import SIGNATURES

# Non-printable hotkey codes worth a readable name (subset of e_SimTex_Key_Codes).
_KEY_NAMES = {
    1: "LEFT", 2: "RIGHT", 3: "UP", 4: "DOWN", 5: "RIGHTUP", 6: "RIGHTDOWN",
    7: "LEFTUP", 8: "LEFTDOWN", 10: "DELETE", 11: "BACKSPACE", 12: "ENTER",
    13: "TAB", 27: "ESCAPE", 32: "SPACE",
}
# args that explicitly mean "no hotkey".
_NO_HOTKEY = {"", "0", "-1", "ST_UNDEFINED"}
_CHAR_LIT_RE = re.compile(r"^'(\\?.)'$")
_STR_INDEX_RE = re.compile(r"\[\s*\d+\s*\]\s*$")  # NAME[0] — first char of a string, not static


def _key_name(v):
    if v is None:
        return ""
    if v in _KEY_NAMES:
        return _KEY_NAMES[v]
    if 0x20 <= v <= 0x7E:
        return chr(v)
    if 0xA0 <= v <= 0xB9:
        return "ALT_" + chr(ord("A") + (v - 0xA0))
    return f"0x{v:02X}"


def decode_hotkey(raw, folder):
    """Return (value_or_None, name, unresolved_flag)."""
    raw = (raw or "").strip()
    if raw in _NO_HOTKEY:
        return None, "", False
    m = _CHAR_LIT_RE.match(raw)
    if m:
        ch = m.group(1)
        esc = {"n": 10, "r": 13, "t": 9, "0": 0, "\\": 92, "'": 39}
        if ch.startswith("\\"):
            v = esc.get(ch[1:], ord(ch[-1]))
        else:
            v = ord(ch)
        return v, _key_name(v), False
    if _STR_INDEX_RE.search(raw):
        return None, "(str)", True   # e.g. hotkey_GameButton[0] — first char of a string table
    v = folder.fold(raw)
    if v is None:
        return None, "(?)", True
    if v in (0, -1):
        return None, "", False
    return v, _key_name(v), False


def _fold_role(args, sig, role, folder):
    idx = sig.get(role)
    if idx is None or idx >= len(args):
        return None
    return folder.fold(args[idx])


def resolve_geometry(add_func, args, folder):
    """Return a dict of geometry columns + runtime for one call site."""
    sig = SIGNATURES[add_func]
    kind = sig["geom_kind"]
    g = {"geom_kind": kind, "xmin": "", "ymin": "", "xmax": "", "ymax": "",
         "box_w": "", "box_h": "", "cols": "", "rows": "",
         "click_cx": "", "click_cy": "", "runtime": 0}

    if kind == "none":
        return g  # key-addressable; no geometry to resolve

    xmin = _fold_role(args, sig, "xmin", folder)
    ymin = _fold_role(args, sig, "ymin", folder)
    if xmin is not None:
        g["xmin"] = xmin
    if ymin is not None:
        g["ymin"] = ymin

    if kind == "rect":
        xmax = _fold_role(args, sig, "xmax", folder)
        ymax = _fold_role(args, sig, "ymax", folder)
        if None not in (xmin, ymin, xmax, ymax):
            g.update(xmax=xmax, ymax=ymax, click_cx=(xmin + xmax) // 2, click_cy=(ymin + ymax) // 2)
        else:
            if xmax is not None: g["xmax"] = xmax
            if ymax is not None: g["ymax"] = ymax
            g["runtime"] = 1

    elif kind == "wh":
        w = _fold_role(args, sig, "width", folder)
        h = _fold_role(args, sig, "height", folder)
        if None not in (xmin, ymin, w, h):
            xmax, ymax = xmin + w, ymin + h
            g.update(xmax=xmax, ymax=ymax, click_cx=(xmin + xmax) // 2, click_cy=(ymin + ymax) // 2)
        else:
            g["runtime"] = 1

    elif kind == "grid":
        bw = _fold_role(args, sig, "box_w", folder)
        bh = _fold_role(args, sig, "box_h", folder)
        cols = _fold_role(args, sig, "cols", folder)
        rows = _fold_role(args, sig, "rows", folder)
        if None not in (xmin, ymin, bw, bh, cols, rows):
            # Game rects store the inclusive last pixel, so the extent is
            # xmin + box_w*cols - 1 (not the exclusive edge).
            g.update(box_w=bw, box_h=bh, cols=cols, rows=rows,
                     xmax=xmin + bw * cols - 1, ymax=ymin + bh * rows - 1,
                     click_cx=xmin + bw // 2, click_cy=ymin + bh // 2)  # cell (0,0) center
        else:
            g["runtime"] = 1

    elif kind == "w_only":
        w = _fold_role(args, sig, "width", folder)
        if None not in (xmin, ymin, w):
            g["xmax"] = xmin + w
        g["runtime"] = 1  # height not in the call — no reliable click center

    elif kind == "sprite":
        g["runtime"] = 1  # extent comes from the sprite; top-left only is known statically

    return g


COLUMNS = ["file", "line", "function", "symbol", "add_func", "geom_kind",
           "xmin", "ymin", "xmax", "ymax", "box_w", "box_h", "cols", "rows",
           "click_cx", "click_cy", "hotkey_value", "hotkey_name", "hotkey_unresolved",
           "runtime", "guards", "raw_args"]

# Right-aligned in the .fwv output; everything else left-aligned. raw_args is the
# free-text tail (see fwv.write).
_NUMERIC = {"line", "xmin", "ymin", "xmax", "ymax", "box_w", "box_h", "cols", "rows",
            "click_cx", "click_cy", "hotkey_value", "hotkey_unresolved", "runtime"}


def build_catalog():
    folder = Folder()
    out = []
    for r in scan_repo():
        g = resolve_geometry(r["add_func"], r["args"], folder)
        sig = SIGNATURES[r["add_func"]]
        hk_idx = sig.get("hotkey")
        hk_raw = r["args"][hk_idx] if (hk_idx is not None and hk_idx < len(r["args"])) else ""
        hv, hn, hu = decode_hotkey(hk_raw, folder)
        row = {
            "file": r["file"], "line": r["line"], "function": r["function"],
            "symbol": r["symbol"], "add_func": r["add_func"],
            "hotkey_value": "" if hv is None else hv, "hotkey_name": hn,
            "hotkey_unresolved": 1 if hu else 0,
            "guards": r.get("guards", ""), "raw_args": r["raw_args"],
        }
        row.update(g)
        out.append(row)
    return out


def write_fwv(rows, path):
    """Write the catalog as a fixed-width values file (see fwv.write)."""
    fwv.write(rows, COLUMNS, _NUMERIC, path)
