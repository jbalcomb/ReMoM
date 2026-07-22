"""
scan.py — find Add_*Field call sites in the game's .c source.

For each call it returns: file, line, enclosing function, LHS symbol (if the
call is assigned), the add-func name, and the top-level argument list.

Robustness: comments and the *contents* of string/char literals are blanked
(replaced with spaces, offsets preserved) before any structural scan, so commas
and parens inside "a, b" or '(' never break argument splitting or paren
matching. Argument text is sliced from the original source; delimiter positions
come from the blanked copy.
"""
import os
import re

from .signatures import KNOWN_ADD_FUNCS

SRC_DIRS = ["MoM/src", "MoX/src"]
_CALL_RE = re.compile(r"\b(" + "|".join(re.escape(f) for f in KNOWN_ADD_FUNCS) + r")\s*\(")
_LHS_RE = re.compile(r"([A-Za-z_]\w*(?:\[[^\]]*\])?)\s*=\s*$")
_FUNC_NAME_RE = re.compile(r"([A-Za-z_]\w*)\s*$")
# A C parameter declaration (function definition/prototype), not a call argument.
_PARAM_RE = re.compile(r"^(?:const\s+)?(?:u?int(?:8|16|32|64)_t|int|char|short|long|void|unsigned|SAMB_ptr)\b")


def _next_nonspace(blanked, idx):
    n = len(blanked)
    while idx < n and blanked[idx] in " \t\r\n":
        idx += 1
    return blanked[idx] if idx < n else ""


def blank_noncode(text):
    """Return a same-length copy with comment / string / char *contents* spaced out.
    Quote and comment delimiters are replaced too, so the result is pure structure."""
    out = list(text)
    i = 0
    n = len(text)
    state = None  # None | 'line' | 'block' | 'str' | 'chr'
    while i < n:
        c = text[i]
        nxt = text[i + 1] if i + 1 < n else ""
        if state is None:
            if c == "/" and nxt == "/":
                state = "line"; out[i] = " "; out[i + 1] = " "; i += 2; continue
            if c == "/" and nxt == "*":
                state = "block"; out[i] = " "; out[i + 1] = " "; i += 2; continue
            if c == '"':
                state = "str"; out[i] = " "; i += 1; continue
            if c == "'":
                state = "chr"; out[i] = " "; i += 1; continue
            i += 1; continue
        # inside something
        if state == "line":
            if c == "\n":
                state = None  # keep the newline
            else:
                out[i] = " "
            i += 1; continue
        if state == "block":
            if c == "*" and nxt == "/":
                out[i] = " "; out[i + 1] = " "; state = None; i += 2; continue
            if c != "\n":
                out[i] = " "
            i += 1; continue
        if state in ("str", "chr"):
            if c == "\\":  # escape — blank both chars
                out[i] = " "
                if i + 1 < n and text[i + 1] != "\n":
                    out[i + 1] = " "
                i += 2; continue
            if (state == "str" and c == '"') or (state == "chr" and c == "'"):
                out[i] = " "; state = None; i += 1; continue
            if c != "\n":
                out[i] = " "
            i += 1; continue
    return "".join(out)


def match_paren(blanked, open_idx):
    """Given index of '(', return index just past the matching ')', or -1."""
    depth = 0
    n = len(blanked)
    i = open_idx
    while i < n:
        c = blanked[i]
        if c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
            if depth == 0:
                return i + 1
        i += 1
    return -1


def split_top_level(blanked, orig, start, end):
    """Split orig[start:end] at top-level commas found in blanked[start:end]."""
    args = []
    depth = 0
    seg_start = start
    for i in range(start, end):
        c = blanked[i]
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
        elif c == "," and depth == 0:
            args.append(orig[seg_start:i].strip())
            seg_start = i + 1
    tail = orig[seg_start:end].strip()
    if tail or args:
        args.append(tail)
    return args


def build_function_map(blanked):
    """Return a list of (start_offset, end_offset, func_name) for top-level function bodies."""
    funcs = []
    depth = 0
    n = len(blanked)
    i = 0
    pending_name = ""
    body_start = -1
    cur_name = ""
    while i < n:
        c = blanked[i]
        if c == "(" and depth == 0:
            # candidate function/declarator name is the identifier just before '('
            m = _FUNC_NAME_RE.search(blanked[max(0, i - 96):i])
            if m:
                pending_name = m.group(1)
        elif c == "{":
            if depth == 0:
                cur_name = pending_name
                body_start = i
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0 and body_start >= 0:
                funcs.append((body_start, i, cur_name))
                body_start = -1
                pending_name = ""
        i += 1
    return funcs


def func_at(funcs, offset):
    for start, end, name in funcs:
        if start <= offset <= end:
            return name
    return ""


_GUARD_KW = ("if", "for", "while", "switch")


def _guard_before(blanked, text, brace_idx):
    """Return the control-flow condition guarding the block opened at brace_idx,
    e.g. 'if(all_units_moved == ST_TRUE)', 'for(...)', 'else', or '' for a plain
    block / function body / do-block."""
    j = brace_idx - 1
    while j >= 0 and blanked[j] in " \t\r\n":
        j -= 1
    if j < 0:
        return ""
    if blanked[j] != ")":
        word = _FUNC_NAME_RE.search(blanked[max(0, brace_idx - 64):brace_idx])
        return "else" if (word and word.group(1) == "else") else ""
    # Backward paren-match from the ')' to its '('.
    depth = 0
    k = j
    while k >= 0:
        ch = blanked[k]
        if ch == ")":
            depth += 1
        elif ch == "(":
            depth -= 1
            if depth == 0:
                break
        k -= 1
    if k < 0:
        return ""
    kw = _FUNC_NAME_RE.search(blanked[max(0, k - 64):k])
    if not kw or kw.group(1) not in _GUARD_KW:
        return ""
    cond = re.sub(r"\s+", " ", text[k + 1:j]).strip()
    return f"{kw.group(1)}({cond})"


def build_block_guards(blanked, text):
    """Return [(start, end, guard_text)] for every {...} block."""
    blocks = []
    stack = []
    for i, c in enumerate(blanked):
        if c == "{":
            stack.append((i, _guard_before(blanked, text, i)))
        elif c == "}":
            if stack:
                start, guard = stack.pop()
                blocks.append((start, i, guard))
    return blocks


def guards_at(blocks, offset):
    """Outermost-to-innermost control-flow conditions enclosing offset."""
    enc = [(s, g) for (s, e, g) in blocks if s <= offset <= e and g]
    enc.sort(key=lambda t: t[0])
    return " && ".join(g for _, g in enc)


def line_of(text, offset):
    return text.count("\n", 0, offset) + 1


def scan_file(path):
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        text = f.read()
    blanked = blank_noncode(text)
    funcs = build_function_map(blanked)
    blocks = build_block_guards(blanked, text)
    rel = os.path.relpath(path, os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))).replace("\\", "/")
    rows = []
    for m in _CALL_RE.finditer(blanked):
        add_func = m.group(1)
        open_idx = m.end() - 1  # index of '('
        close = match_paren(blanked, open_idx)
        if close < 0:
            continue
        # Skip the function definition/prototype itself: a definition is followed
        # by '{', and its first argument is a typed parameter declaration.
        if _next_nonspace(blanked, close) == "{":
            continue
        args = split_top_level(blanked, text, open_idx + 1, close - 1)
        if args and _PARAM_RE.match(args[0]):
            continue
        # Collapse internal whitespace/newlines so a multi-line source call stays
        # a single record in the line-oriented output.
        args = [re.sub(r"\s+", " ", a).strip() for a in args]
        raw_args = re.sub(r"\s+", " ", text[open_idx + 1:close - 1]).strip()
        # LHS symbol: identifier '=' immediately before the call, same statement.
        pre = text[max(0, m.start() - 120):m.start()]
        lhs = _LHS_RE.search(pre)
        symbol = lhs.group(1) if lhs else ""
        rows.append({
            "file": rel,
            "line": line_of(text, m.start()),
            "function": func_at(funcs, m.start()),
            "add_func": add_func,
            "symbol": symbol,
            "args": args,
            "guards": guards_at(blocks, m.start()),
            "raw_args": raw_args,
        })
    return rows


def scan_repo(src_dirs=None):
    repo = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
    src_dirs = src_dirs or SRC_DIRS
    rows = []
    for d in src_dirs:
        base = os.path.join(repo, *d.split("/"))
        if not os.path.isdir(base):
            continue
        for name in sorted(os.listdir(base)):
            if name.endswith(".c"):
                rows.extend(scan_file(os.path.join(base, name)))
    return rows
