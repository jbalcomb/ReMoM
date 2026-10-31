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
// @MoM WZD s161p04 ida:seg161:0005 file:0x01A2B5 len:0x0084
// drake178: TILE_GetProd()
int16_t Square_Production_Bonus_Original(int16_t wx, int16_t wy, int16_t wp, int16_t have_gaias_blessing)
```

| Token | Meaning | Form |
|---|---|---|
| `@MoM` | marks the tag | literal |
| exe | `WZD` (WIZARDS.EXE) or `MGC` (MAGIC.EXE); more for the demo, see open questions | `[A-Z]{3}` |
| id | segment kind, number and procedure: `s` resident or `o` overlay, segment 3 digits, procedure 2 digits | `[so][0-9]{3}p[0-9]{2}` |
| `ida:` | IDA segment name and offset in it, exactly as IDA shows the address | `ida:seg161:0005` |
| `file:` | byte offset of the function in the **1.31** `.EXE`; Ghidra jumps to it directly (Navigation > Go To... > file offset) | `file:0x` + 6 hex |
| `len:` | function size in bytes | `len:0x` + 4 hex |

- **Fixed order, single spaces, uppercase hex, fixed widths.** A plain `grep` finds a tag, and `sort` orders tags
  by id.
- **Other versions are not in the tag.** File offsets in 1.40n–1.60 and the demo come from the address table. The tag
  stays a stable 1.31 key and never has to change when another version is added.
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
- **Segment addresses** go in a separate optional line, `@MoM-seg WZD s161 ida:seg161 base:0x1A000 file:0x01A000`,
  for files that correspond to exactly one segment, as the other project's files do.

## Regular expressions (POSIX ERE)

Function tag, capturing executable, segment kind, segment, procedure, IDA segment, IDA offset, file offset, length:

```
^// @MoM (WZD|MGC) ([so])([0-9]{3})p([0-9]{2}) ida:([A-Za-z0-9_]+):([0-9A-F]{4}) file:0x([0-9A-F]{6}) len:0x([0-9A-F]{4})$
```

File tag and ReMoM-only tag:

```
^[[:space:]]*@MoM-file (WZD|MGC)( [so][0-9]{3}(-[so][0-9]{3})?)+$
^// @ReMoM [^[:cntrl:]]+$
```

The documentation will carry examples like these:

```bash
# every reconstructed function of WIZARDS.EXE, with file and line
grep -rnE '^// @MoM WZD [so][0-9]{3}p[0-9]{2} ' MoM MoX src

# the C function for one id
grep -rn -A3 '^// @MoM WZD s161p04 ' MoM MoX src

# from an IDA address to the C function
grep -rn 'ida:seg161:0005 ' MoM MoX src

# a table of id, file offset and C location
grep -rnoE '^// @MoM (WZD|MGC) [so][0-9]{3}p[0-9]{2} .* file:0x[0-9A-F]{6}' MoM MoX src \
  | sed -E 's/^([^:]+):([0-9]+):\/\/ @MoM ([A-Z]{3}) ([so0-9p]+) .* file:(0x[0-9A-F]+)/\3\t\4\t\5\t\1:\2/'
```

```python
import re
TAG = re.compile(r'^// @MoM (WZD|MGC) ([so])([0-9]{3})p([0-9]{2}) ida:(\w+):([0-9A-F]{4}) file:0x([0-9A-F]{6}) len:0x([0-9A-F]{4})$')
```

An IDC helper for IDA 5.5 (paste a tag line, jump to the function) comes with the tooling below.

The same patterns serve the other reconstruction once its keys are known (open questions). All of the above were
checked against sample lines with `grep -E` and Python: valid tags match, and the old bare `// WZD s161p04` and
unpadded ids do not.

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
6. Other reconstruction: scanner and `--project` index; its files' segment addresses checked against our `S` rows.
7. Demo and early versions: their own IDA databases → their own maps → columns in the address table.

## Open questions

1. **Other project:** where is it, and what does each file carry? Its folders give the executable. Does each file
   name its segment number, its IDA segment start address, or both? Are function names or addresses tagged inside
   the files? With the segment start address alone, the join needs our `S` rows from the same `.EXE`. That is fine as
   long as both use the same IDA load base, which they will if both come from IDA's default.
2. **Demo:** which executables does it have, and does it share segment numbering with 1.31? If not, its functions
   need exe codes of their own (for example `WZDd`, `MGCd`), keyed to the demo's databases.
3. **Ghidra:** is a Ghidra `seg:off` (relative to Ghidra's load segment) wanted in the tag itself, or is `file:`
   enough? `file:` works in both tools without depending on where each one loads the image.
4. **Tag prefix:** `@MoM` / `@MoM-file` / `@ReMoM` are proposals. Any short literal that never occurs otherwise works.
