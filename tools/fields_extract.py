#!/usr/bin/env python3
"""
fields_extract.py - static extractor for ReMoM input field registrations.

Scans MoM/src and MoX/src C sources for calls to Add_*Field / Add_Hot_Key and
writes one CSV row per call site, with enclosing function + guard context.

Output: tools/fields/_static.csv  (all screens; filter/merge downstream)

Paired with:
  - runtime FIELDADD / FIELDSNAPSHOT lines emitted by Fields.c (STU_DEBUG)
  - tools/fields_merge.py (not yet written)
"""
import csv
import os
import re
import sys

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
SRC_DIRS = [
    os.path.join(REPO_ROOT, "MoM", "src"),
    os.path.join(REPO_ROOT, "MoX", "src"),
]
OUT_DIR = os.path.join(REPO_ROOT, "tools", "fields")
OUT_CSV = os.path.join(OUT_DIR, "_static.csv")

ADD_FUNCS = [
    "Add_Button_Field",
    "Add_Hidden_Field",
    "Add_Hot_Key",
    "Add_Multi_Hot_Key_Field",
    "Add_Grid_Field",
    "Add_Scroll_Field",
    "Add_Input_Field",
    "Add_Continuous_String_Input_Field",
    "Add_Picture_Field",
    "Add_String_List_Field",
]
CALL_RE = re.compile(r"\b(" + "|".join(ADD_FUNCS) + r")\s*\(")
FUNC_DEF_RE = re.compile(r"^[A-Za-z_][\w\s\*]*?\b([A-Za-z_]\w*)\s*\([^;]*\)\s*$")
GUARD_RE = re.compile(r"^\s*(if|for|while|switch|else\s+if)\b[^;]*$")

# Strip line + block comments cheaply so regexes don't trip on commented calls.
def strip_comments(src):
    out = []
    i = 0
    n = len(src)
    in_block = False
    in_line = False
    in_str = False
    str_ch = ""
    while i < n:
        c = src[i]
        nxt = src[i+1] if i+1 < n else ""
        if in_block:
            if c == "*" and nxt == "/":
                in_block = False
                out.append("  ")
                i += 2
                continue
            out.append(" " if c != "\n" else "\n")
            i += 1
            continue
        if in_line:
            if c == "\n":
                in_line = False
                out.append("\n")
            i += 1
            continue
        if in_str:
            out.append(c)
            if c == "\\" and nxt:
                out.append(nxt)
                i += 2
                continue
            if c == str_ch:
                in_str = False
            i += 1
            continue
        if c == "/" and nxt == "*":
            in_block = True
            i += 2
            continue
        if c == "/" and nxt == "/":
            in_line = True
            i += 2
            continue
        if c == '"' or c == "'":
            in_str = True
            str_ch = c
            out.append(c)
            i += 1
            continue
        out.append(c)
        i += 1
    return "".join(out)


def extract_call_args(src, start_paren):
    """Given index of '(' after an Add_* name, return (args_text, end_index)."""
    depth = 0
    i = start_paren
    n = len(src)
    while i < n:
        c = src[i]
        if c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
            if depth == 0:
                return src[start_paren + 1:i].strip(), i
        i += 1
    return src[start_paren + 1:].strip(), n


def scan_file(path):
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        raw = f.read()
    clean = strip_comments(raw)
    lines = clean.split("\n")
    # Map char offset -> line number for call sites.
    line_offsets = [0]
    for ln in lines:
        line_offsets.append(line_offsets[-1] + len(ln) + 1)

    def offset_to_line(ofs):
        # binary search
        lo, hi = 0, len(line_offsets) - 1
        while lo < hi:
            mid = (lo + hi) // 2
            if line_offsets[mid] <= ofs:
                lo = mid + 1
            else:
                hi = mid
        return lo  # 1-based

    # Walk brace depth to compute enclosing function and guard stack.
    func_stack = []   # list of (name, depth)
    guard_stack = []  # list of (text, depth)
    depth = 0
    current_func = ""
    rows = []

    # We need a line-based walk so we can detect function headers + guards, plus
    # the char-offset call finder. Do them together.
    i = 0
    n = len(clean)
    line_no = 1
    line_start = 0
    pending_header = ""  # accumulates non-brace text on current line for func/guard detection

    while i < n:
        c = clean[i]
        if c == "\n":
            pending_header = ""
            line_no += 1
            line_start = i + 1
            i += 1
            continue
        if c == "{":
            # Is this a function body open? Heuristic: at depth 0 and the
            # recent non-blank content looks like a function signature.
            if depth == 0:
                # Grab the last few lines to check for a function header.
                prev = clean[max(0, i - 400):i]
                # Find last ')' before this '{' and look backwards for identifier(
                last_paren = prev.rfind(")")
                if last_paren != -1:
                    head = prev[:last_paren + 1]
                    # match last  name(...)
                    m = re.search(r"([A-Za-z_]\w*)\s*\([^;{}]*\)\s*$", head)
                    if m:
                        current_func = m.group(1)
                        func_stack.append((current_func, depth))
            else:
                # Check if current line (up to '{') is a guard
                line_text = clean[line_start:i].strip()
                gm = re.match(r"(if|for|while|switch|else\s+if)\b(.*)$", line_text)
                if gm:
                    gtext = line_text
                    guard_stack.append((gtext, depth))
            depth += 1
            i += 1
            continue
        if c == "}":
            depth -= 1
            # Pop guards/funcs whose depth >= current depth
            while guard_stack and guard_stack[-1][1] >= depth:
                guard_stack.pop()
            while func_stack and func_stack[-1][1] >= depth:
                func_stack.pop()
                current_func = func_stack[-1][0] if func_stack else ""
            i += 1
            continue
        # Look for Add_* call
        m = CALL_RE.match(clean, i)
        if m:
            name = m.group(1)
            args_text, end = extract_call_args(clean, m.end() - 1)
            # Skip prototypes / definitions: arg list contains C type keywords.
            if re.search(r"\b(int16_t|int32_t|int64_t|uint8_t|uint16_t|uint32_t|char\s*\*|void|SAMB_ptr)\b", args_text) or re.match(r"^char\s*\*", args_text):
                i = end + 1
                continue
            call_line = offset_to_line(i)
            # Capture LHS assignment target (if any) by scanning the current line up to the call.
            symbol = ""
            line_head = clean[line_start:i]
            lhs_m = re.search(r"([A-Za-z_]\w*(?:\s*\[[^\]]*\])?)\s*=\s*$", line_head)
            if lhs_m:
                symbol = re.sub(r"\s+", "", lhs_m.group(1))
            guards = " && ".join(g[0] for g in guard_stack)
            rows.append({
                "file": os.path.relpath(path, REPO_ROOT).replace("\\", "/"),
                "line": call_line,
                "function": current_func,
                "add_func": name,
                "symbol": symbol,
                "args": " ".join(args_text.split()),
                "guards": guards,
            })
            i = end + 1
            continue
        i += 1
    return rows


def main():
    os.makedirs(OUT_DIR, exist_ok=True)
    all_rows = []
    for d in SRC_DIRS:
        if not os.path.isdir(d):
            continue
        for entry in sorted(os.listdir(d)):
            if not entry.endswith(".c"):
                continue
            all_rows.extend(scan_file(os.path.join(d, entry)))
    with open(OUT_CSV, "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=["file", "line", "function", "add_func", "symbol", "args", "guards"])
        w.writeheader()
        for r in all_rows:
            w.writerow(r)
    print(f"Wrote {len(all_rows)} call sites to {OUT_CSV}")
    # Quick breakdown by function as a sanity check.
    by_func = {}
    for r in all_rows:
        by_func[r["function"]] = by_func.get(r["function"], 0) + 1
    top = sorted(by_func.items(), key=lambda kv: -kv[1])[:15]
    print("Top enclosing functions:")
    for name, cnt in top:
        print(f"  {cnt:4d}  {name}")


if __name__ == "__main__":
    main()
