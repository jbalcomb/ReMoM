"""
constants.py — build a name->value table of C manifest constants and fold
integer coordinate expressions against it.

Source headers: MoX/src/MOM_DEF.h and MoX/src/MOX_DEF.h. Several coordinate
macros are defined more than once behind video-mode #ifdefs:

    #ifdef VIDEO_MODE_MODE_Y        -> 320x200  (SCREEN_XMAX 319)   <-- the HMS space
    #ifdef VIDEO_MODE_MODE_Y_2x     -> 640x400  (SCREEN_XMAX 639)
    #ifdef VIDEO_MODE_MODE_Y_2x_XBGR-> 640x400

HMS click coordinates are always in the 320x200 logical framebuffer, so we pin
the VIDEO_MODE_MODE_Y branch and reject the _2x branches. Defines outside any
video-mode #ifdef are accepted unconditionally.
"""
import ast
import os
import re

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
DEF_HEADERS = [
    os.path.join(REPO_ROOT, "MoX", "src", "MOM_DEF.h"),
    os.path.join(REPO_ROOT, "MoX", "src", "MOX_DEF.h"),
]

# Video-mode guard tokens: which #ifdef branch to keep / drop for the 320x200 space.
VIDEO_MODE_KEEP = {"VIDEO_MODE_MODE_Y"}
VIDEO_MODE_DROP = {"VIDEO_MODE_MODE_Y_2x", "VIDEO_MODE_MODE_Y_2x_XBGR"}

_DEFINE_RE = re.compile(r"^\s*#\s*define\s+([A-Za-z_]\w*)\s+(.+?)\s*$")
_IFDEF_RE = re.compile(r"^\s*#\s*(ifdef|ifndef|if)\b\s*(\w+)?")
_ELSE_RE = re.compile(r"^\s*#\s*else\b")
_ENDIF_RE = re.compile(r"^\s*#\s*endif\b")


def _strip_line_comment(s):
    # Blank a trailing // comment and any /* */ that closes on the same line.
    s = re.sub(r"/\*.*?\*/", " ", s)
    s = re.sub(r"//.*$", "", s)
    return s


def load_defines(headers=None):
    """Return {name: raw_value_string} honoring the video-mode branch selection."""
    headers = headers or DEF_HEADERS
    defines = {}
    for path in headers:
        if not os.path.isfile(path):
            continue
        # Stack of frame verdicts: 'keep' | 'drop' | 'neutral'. A define is
        # accepted only if no enclosing frame is 'drop'.
        stack = []
        with open(path, "r", encoding="utf-8", errors="replace") as f:
            for raw in f:
                line = raw.rstrip("\n")
                m = _IFDEF_RE.match(line)
                if m:
                    tok = m.group(2) or ""
                    if tok in VIDEO_MODE_KEEP:
                        stack.append("keep")
                    elif tok in VIDEO_MODE_DROP:
                        stack.append("drop")
                    else:
                        stack.append("neutral")
                    continue
                if _ELSE_RE.match(line):
                    if stack:
                        top = stack[-1]
                        stack[-1] = {"keep": "drop", "drop": "keep"}.get(top, "neutral")
                    continue
                if _ENDIF_RE.match(line):
                    if stack:
                        stack.pop()
                    continue
                if "drop" in stack:
                    continue
                dm = _DEFINE_RE.match(line)
                if dm:
                    name = dm.group(1)
                    value = _strip_line_comment(dm.group(2)).strip()
                    if value == "":
                        continue
                    # Skip function-like macros: `#define NAME(x) ...`
                    if name.endswith("(") or "(" in dm.group(1):
                        continue
                    defines[name] = value
    return defines


class Folder:
    """Constant-folds integer expressions against a name->value table."""

    def __init__(self, defines=None):
        self.defines = defines if defines is not None else load_defines()
        self._cache = {}

    def fold(self, expr):
        """Return an int if `expr` folds to a constant, else None."""
        if expr is None:
            return None
        s = expr.strip()
        if s == "":
            return None
        # Strip an integer cast prefix: (int16_t)X, (short)X, (int)X
        s = re.sub(r"^\(\s*(?:u?int(?:8|16|32|64)_t|short|int|char|long)\s*\)\s*", "", s)
        return self._eval(s, depth=0)

    def _eval(self, s, depth):
        if depth > 24:
            return None
        s = s.strip()
        if s in self._cache:
            return self._cache[s]
        try:
            node = ast.parse(s, mode="eval").body
        except SyntaxError:
            return None
        val = self._walk(node, depth)
        self._cache[s] = val
        return val

    def _walk(self, node, depth):
        if isinstance(node, ast.Constant):
            return node.value if isinstance(node.value, int) else None
        if isinstance(node, ast.UnaryOp) and isinstance(node.op, (ast.USub, ast.UAdd)):
            v = self._walk(node.operand, depth)
            if v is None:
                return None
            return -v if isinstance(node.op, ast.USub) else v
        if isinstance(node, ast.BinOp):
            a = self._walk(node.left, depth)
            b = self._walk(node.right, depth)
            if a is None or b is None:
                return None
            op = node.op
            try:
                if isinstance(op, ast.Add): return a + b
                if isinstance(op, ast.Sub): return a - b
                if isinstance(op, ast.Mult): return a * b
                if isinstance(op, ast.Div): return a // b if b else None
                if isinstance(op, ast.FloorDiv): return a // b if b else None
                if isinstance(op, ast.Mod): return a % b if b else None
                if isinstance(op, ast.LShift): return a << b
                if isinstance(op, ast.RShift): return a >> b
            except (ValueError, ZeroDivisionError):
                return None
            return None
        if isinstance(node, ast.Name):
            raw = self.defines.get(node.id)
            if raw is None:
                return None
            return self._eval(raw, depth + 1)
        return None
