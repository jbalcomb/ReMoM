# Plan: function and file tags (keyed comments that map ReMoM to the executables)

Status: plan. Branch: `remom-mod`.

## Goal

Every function in ReMoM carries a one-line keyed comment. From that line alone, a person or a script can find the
original function in IDA, in Ghidra, and in the 1.31 executable. Every source file carries a keyed header line naming
the executable and the segments it covers. The same keys join ReMoM to the other reconstruction (one C file per
segment, in a folder per executable) and to every version of the game, demo included, through the address table
(`tools/mom_patch_diff/address_table.py`).

## Today

- 1,959 ids in comments like `// WZD s161p04` (`tools/mom_patch_diff/address_table.py --remom .`, no map). Of these,
  1,678 resolve to a function: 1,563 by the next definition, 106 by a prototype, and 9 doubtful. The other 281 have
  only the comment, with no code yet.
- The spelling varies: `WZD`/`MGC`; `s`/`o`; padded or not (`o56p2`, `o051p01`); the id sits above a prototype or
  above the definition; and it is sometimes followed by `// drake178: NAME()` or other notes.
- No addresses appear anywhere in the sources. IDA addresses and file offsets exist only in the IDA databases, now
  exportable with `tools/mom_patch_diff/export_map.py`.
- IDA linear addresses are not unique across the two executables: each database loads its `.EXE` at the same default
  base. Every key below therefore starts with the executable.

## Function tag

One line, directly above the function definition. It replaces the bare `// WZD s161p04` line, and every other
comment stays where it is.

```c
// @MoM WZD s161p04 ida:seg161:0005 seg:1A00:0005 file:0x01A2B5 len:0x0084
// drake178: TILE_GetProd()
int16_t Square_Production_Bonus_Original(int16_t wx, int16_t wy, int16_t wp, int16_t have_gaias_blessing)
```

| Token | Meaning | Form |
|---|---|---|
| `@MoM` | marks the tag | literal |
| exe | `WZD` (WIZARDS.EXE) or `MGC` (MAGIC.EXE) | `[A-Z]{3}` |
| id | segment kind, number and procedure: `s` resident or `o` overlay, segment 3 digits, procedure 2 digits | `[so][0-9]{3}p[0-9]{2}` |
| `ida:` | IDA segment name and offset in it, exactly as IDA shows the address | `ida:seg161:0005` |
| `seg:` | real-mode segment:offset. For resident code, the segment is the paragraph relative to the start of the load module: add the load segment a tool or debugger uses (Ghidra's default `1000`, DOSBox's PSP + 0x10) to get its address. Overlay code has no fixed place in the load module, so it gives the overlay number instead: `seg:o142:0005` | `seg:` + 4 hex or `o` + 3 digits, `:` + 4 hex |
| `file:` | byte offset of the function in the **1.31** `.EXE`; Ghidra jumps to it directly (Navigation > Go To... > file offset) | `file:0x` + 6 hex |
| `len:` | function size in bytes | `len:0x` + 4 hex |

- **Fixed order, single spaces, uppercase hex, fixed widths.** A plain `grep` finds a tag, and `sort` orders tags
  by id.
- **`seg:` comes from the map, not from IDA's display.** The paragraph is (segment start − start of the first
  segment) / 16, from the `S` rows `export_map.py` writes, so it is the same whichever tool loads the image.
- **Other versions are not in the tag.** File offsets in 1.40n–1.60 and the demo come from the address table. The tag
  stays a stable 1.31 key and never has to change when another version is added. Segment numbering may differ
  between versions, and nothing here assumes it doesn't (see *Other versions* below).
- **One function, two executables.** MoX code linked into both `.EXE`s gets two tag lines, one per executable, in
  `WZD`, `MGC` order.
- **Ids with no code yet** keep their tag above a `/* not reconstructed */` comment, so the 281 gaps stay
  greppable.

### ReMoM-only functions

Functions with no original counterpart (MOD, platform, test helpers, `CLAUDE` additions) get `// @ReMoM` followed by
a short reason:

```c
// @ReMoM rules dispatcher for WZD s161p04
int16_t Square_Production_Bonus(int16_t wx, int16_t wy, int16_t wp, int16_t have_gaias_blessing)
```

Then "every function has a tag" is checkable, and a debug helper can no longer be mistaken for the function after it
(today `gd_dump_units` picks up five ids that way).

## File header tag

One keyed line per executable, inside the file's opening comment:

```c
/*
    @MoM-file WZD s161 o142-o145
    @MoM-file MGC s001
    Terrain -- ...
*/
```

- **Tokens:** the executable, then the segments the file covers, in id order. A range like `o142-o145` stands for
  every segment between the two.
- **Generated, not hand-written:** the line comes from the file's function tags, so it can't disagree with them.
  The checker regenerates it and compares.
- **Segment addresses** go in an optional line per segment, `@MoM-seg WZD s161 ida:seg161 seg:1A00 file:0x01A000`:
  the segment's IDA name, its `seg:` paragraph and its file offset in 1.31. This is the key that joins the other
  reconstruction, whose files carry their segment's IDA start address and nothing else.

## Regular expressions (POSIX ERE)

Function tag, capturing executable, segment kind, segment, procedure, IDA segment, IDA offset, file offset, length:

```
^// @MoM (WZD|MGC) ([so])([0-9]{3})p([0-9]{2}) ida:([A-Za-z0-9_]+):([0-9A-F]{4}) seg:([0-9A-F]{4}|o[0-9]{3}):([0-9A-F]{4}) file:0x([0-9A-F]{6}) len:0x([0-9A-F]{4})$
```

File tag, segment line and ReMoM-only tag:

```
^[[:space:]]*@MoM-file (WZD|MGC)( [so][0-9]{3}(-[so][0-9]{3})?)+$
^[[:space:]]*@MoM-seg (WZD|MGC) ([so][0-9]{3}) ida:([A-Za-z0-9_]+) seg:([0-9A-F]{4}|o[0-9]{3}) file:0x([0-9A-F]{6})$
^// @ReMoM [^[:cntrl:]]+$
```

The documentation will carry examples like these:

```bash
# every reconstructed function of WIZARDS.EXE, with file and line
grep -rnE '^// @MoM WZD [so][0-9]{3}p[0-9]{2} ' MoM MoX src

# the C function for one id
grep -rn -A3 '^// @MoM WZD s161p04 ' MoM MoX src

# from an IDA address, or a Ghidra / debugger segment:offset (minus the load segment), to the C function
grep -rn 'ida:seg161:0005 ' MoM MoX src
grep -rn ' seg:1A00:0005 ' MoM MoX src

# a table of id, file offset and C location
grep -rnoE '^// @MoM (WZD|MGC) [so][0-9]{3}p[0-9]{2} .* file:0x[0-9A-F]{6}' MoM MoX src \
  | sed -E 's/^([^:]+):([0-9]+):\/\/ @MoM ([A-Z]{3}) ([so0-9p]+) .* file:(0x[0-9A-F]+)/\3\t\4\t\5\t\1:\2/'
```

```python
import re
TAG = re.compile(r'^// @MoM (WZD|MGC) ([so])([0-9]{3})p([0-9]{2}) ida:(\w+):([0-9A-F]{4}) '
                 r'seg:([0-9A-F]{4}|o[0-9]{3}):([0-9A-F]{4}) file:0x([0-9A-F]{6}) len:0x([0-9A-F]{4})$')
```

An IDC helper for IDA 5.5 (paste a tag line, jump to the function) comes with the tooling below.

All of the above were checked against sample lines with `grep -E` and Python: valid tags match, and the old bare
`// WZD s161p04` and unpadded ids do not.

## The other reconstruction

Each C file is one segment, in a folder named for its executable. The file carries its segment's IDA start address
and nothing else: no segment number, and no tags on its functions. So the join works like this:

1. **Executable:** from the folder name.
2. **Segment:** the file's IDA start address, looked up in that executable's `S` rows (`export_map.py`). This holds
   only if the other project's IDA used the same load base as ours. The scanner checks that every file's address hits
   exactly one segment start, and reports any that don't. If they all miss by the same amount, the bases differ, and
   that difference is applied.
3. **Procedure number:** the order of the function definitions in the file, compared with the procedure count of the
   segment in our map. A file whose count differs is reported, not matched, because the order can't be trusted then.
4. **Check:** where ReMoM and the other project both reconstruct a function, list their names side by side for a
   spot check. Names differ by design, but a whole segment that is shifted by one stands out.

The result is a `--project` index (`exe`, `segment`, `proc`, `function`, `file`, `line`) for `address_table.py`,
written by `tools/mom_tags/scan_project.py`.

## Other versions and the demo

Nothing assumes segments are the same across versions:
- **Own maps:** each version with an IDA database gets its own map (`export_map.py`) and its own ids. Those are
  qualified by the version in the address table (`WZD 1.20 s012p03`), never in ReMoM's tags.
- **Patched versions** (1.40n–1.60, built from 1.31 by drake178's patcher) need no database: the 1.31 map places each
  function in their `.EXE` by its bytes, and anything not found is a change.
- **The demo and the early versions** (1.00–1.20) are matched to 1.31 function by function, by byte similarity of the
  bodies (with fixups masked). They are not matched by segment and procedure number. The table records each match
  with its similarity, so a segment that kept its numbering shows up as a run of `same id` matches, and one that
  didn't shows up as a different id.
- **Functions that exist only in one version** get rows of their own, under that version's ids.

## Tooling (`tools/mom_tags/`)

1. **`tags_report.py`:** lists every function definition in `MoM`, `MoX`, `src` and `STU`, with its tag or the lack
   of one, and every malformed, duplicate or misplaced tag. TSV output. Report only; it changes nothing.
2. **`tags_apply.py`:** writes tags from the address table (after `export_map.py` has run on the 1.31 databases).
   - Rewrites each existing `// WZD s161p04` line into the full tag.
   - Normalises padding.
   - Adds `@ReMoM` to functions that have no id.
   - Regenerates the `@MoM-file` lines.

   It is idempotent and has a `--dry-run` that prints a unified diff. Cases it can't resolve (doubtful `?` evidence,
   ids missing from the map) are listed, not guessed.
3. **CTest `Tags_Valid`:** fails on a malformed tag, two functions with one tag, or a `@MoM-file` line that disagrees
   with the file's function tags. Once the migration is done, it also fails on a function without a tag.
4. **`address_table.py`:** reads the new tags (keeping the old form as a fallback until the migration is done), and
   gains `--project` scanners for the other reconstruction and per-version maps for the demo and the early versions.
5. **Documentation:** `doc/@Devel/Code-Tags.md` gets the formats, the regular expressions above, and the grep / sed /
   Python / IDC examples. The documentation index and `CONTRIBUTING.md` link to it.

## Steps

1. Run `export_map.py` on the 1.31 `WIZARDS` and `MAGIC` databases (your machine). Check the maps against
   `address_table.py`: every `seg*`/`ovr*` function should place in the 1.31 `.EXE`.
2. Write `tags_report.py` and run it. Resolve the 9 doubtful ids and the misplaced comments by hand.
3. Write `tags_apply.py`; review the dry-run diff; apply in one commit per directory (`MoX`, `MoM`, `src`, `STU`).
4. Add `Tags_Valid` (report mode first, enforcing once the counts reach zero).
5. Write `doc/@Devel/Code-Tags.md`.
6. Other reconstruction: `scan_project.py` (segment by IDA start address, procedure by order, counts checked) →
   `--project` index.
7. Demo and early versions: their own IDA databases → their own maps → similarity matching against 1.31 → columns in
   the address table.

## Decisions

- **Prefixes:** `@MoM`, `@MoM-file`, `@MoM-seg`, `@ReMoM`.
- **Addresses in the tag:** IDA `segment:offset`, real-mode `seg:` (load-module paragraph, or overlay number), and
  the 1.31 file offset.
- **Versions:** segments are not assumed to match across versions; each version is matched by bytes or similarity
  and keyed by its own ids in the address table.
- **The other project:** it carries no segment numbers or function tags, only each file's IDA segment start address;
  it is joined as described above.

## Open questions

1. **Load base:** did the other project's IDA use IDA's default load base for the MZ image? `scan_project.py` will
   detect and report a difference, but knowing it up front saves a step.
2. **IDA databases for the demo and 1.00–1.20:** which exist? Each needs `export_map.py` once.
