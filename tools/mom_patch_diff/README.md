# mom_patch_diff

Compares two Master of Magic installs and writes what changed, file by file: functions of `WIZARDS.EXE` and
`MAGIC.EXE` by ReMoM's ids (`WZD s161p04`), named data items with old and new values, LBX data tables field by
field, and text. Use it with drake178's differential patcher (`MOMDP_32.EXE`) to see exactly what each unofficial
patch and Community Patch version changed against 1.31, without anyone reading the binaries by hand.

Standard library only. Run it from the root of the ReMoM repository (it reuses `tools/asset_catalog` and
`tools/dummy_lbx`).

## 1. Build one install per version

Start from a clean 1.31 install and copy it once per version. Run the patcher inside each copy and choose that
version:

```bash
cd /c/MoM
cp -r MoM131 MoM140n;  cp -r MoM131 MoM150;  cp -r MoM131 MoM151;  cp -r MoM131 MoM15203;  cp -r MoM131 MoM160
# then, in each copy: copy MOMDP_32.EXE in, run it, pick the version (1.40n, 1.50, 1.51, 1.52.03, 1.60)
```

## 2. Export the function maps from IDA (once)

`export_map.py` runs `export_map.idc` through IDA Pro 5.5's command line: it copies the database to a temporary
folder (so the original is never opened or saved), starts IDA autonomously (`-A`) with the script (`-S`) and a log
(`-L`), and checks the CSV it wrote:

```bash
cd /c/STU/devel/ReMoM
IDA="/c/Program Files/IDA 5.5/idag.exe"            # or idaw.exe, the text-mode IDA
python3 tools/mom_patch_diff/export_map.py --ida "$IDA" --idb /c/STU/IDA/WIZARDS.idb --out /c/MoM/wizards_map.csv
python3 tools/mom_patch_diff/export_map.py --ida "$IDA" --idb /c/STU/IDA/MAGIC.idb   --out /c/MoM/magic_map.csv
```

The map has one row per segment, function and named data item: IDA segment, procedure number in the segment (the
`p04` of `WZD s161p04`), IDA name, address, size and bytes. `--keep` keeps the working folder and IDA's log for
debugging. Neither script has been run against a real IDA yet; if IDA stops, the log names the IDC line.

Without the maps the comparison still runs, but changes are reported by file offset instead of by function.

## 3. Compare

Each version against the one before it shows what that release added. 1.31 against 1.60 shows the total:

```bash
cd /c/STU/devel/ReMoM
MAPS="--map WIZARDS.EXE=/c/MoM/wizards_map.csv --map MAGIC.EXE=/c/MoM/magic_map.csv"
prev=131
for v in 140n 150 151 15203 160; do
    python3 -m tools.mom_patch_diff /c/MoM/MoM$prev /c/MoM/MoM$v --out /c/MoM/diff_${prev}_to_${v} \
        --old-label $prev --new-label $v $MAPS
    prev=$v
done
python3 -m tools.mom_patch_diff /c/MoM/MoM131 /c/MoM/MoM160 --out /c/MoM/diff_131_to_160 --old-label 131 --new-label 160 $MAPS
```

`--disasm` adds a 16-bit disassembly diff of every changed function. It needs the `capstone` package, which is a
new dependency: vet it per [doc/@Devel/Dependency-Vetting.md](../../doc/@Devel/Dependency-Vetting.md) first. The
report is useful without it.

## 4. Share the reports

Put the `diff_*` folders in a Drive folder. They hold short hex excerpts of the changed code and data, not the game
files. Start with `summary.md`; each changed file has its own folder or file:

| File | What it holds |
|---|---|
| `summary.md`, `summary.json` | every file: same / changed / only in one install, sizes, SHA-256 |
| `<EXE>/functions.md` | changed functions: id, IDA name, segment, old and new offset and size, similarity |
| `<EXE>/functions/<id>.txt` | one changed function: each changed byte range (relocation-only ranges labelled `fixup`), and with `--disasm` the disassembly diff |
| `<EXE>/data.md` | named data items whose bytes changed, with old and new 16-bit values |
| `<EXE>/regions.md` | changed byte ranges outside every mapped item (only when the file size is unchanged) |
| `<EXE>/strings.md` | text added or removed (messages, names) |
| `<EXE>/layout.md` | MZ header, load module and Borland overlay (FBOV) area of both files |
| `<LBX>.md` | changed entries; known data tables decoded field by field (`tools/dummy_lbx/tables.py`) |
| `<FILE>.diff` | other text files, as a unified diff |

## Address table

`address_table.py` joins everything known about each function into one table: ReMoM id, IDA segment, address
(`seg161:01A2` and linear) and name, the file offset in each version's `.EXE`, and the C function that
reconstructs it in ReMoM (and in other reconstructions):

```bash
python3 -m tools.mom_patch_diff.address_table --remom . --out /c/MoM/address_table \
    --map WIZARDS.EXE=/c/MoM/wizards_map.csv --map MAGIC.EXE=/c/MoM/magic_map.csv \
    --exe 1.31=/c/MoM/MoM131 --exe 1.40n=/c/MoM/MoM140n --exe 1.60=/c/MoM/MoM160 \
    --project other=/c/MoM/other_index.csv
```

A version's offset is where that function's 1.31 bytes are in that version's `.EXE`. It reads `changed` if the
function's bytes differ there (see that version's `functions.md`), and `short` if the function is too short to place.

ReMoM's side comes from its id comments (`// WZD s161p04` above a function). `remom_evidence` says how each id was
resolved:
- `definition`: the next function definition after the comment.
- `prototype`: a prototype after the comment names the function. This wins when the two disagree.
- `comment`: ReMoM has the id but no code yet.
- a trailing `?`: another id of the same `.EXE` claims the same function (a helper placed between a comment and its
  function, or a mistyped id); check those by hand.

`--project NAME=CSV` adds another reconstruction from an index with the columns `exe,segment,proc,function,file,line`
(`WZD` or `MGC`, `s161` or `o142`, procedure number). Without `--map` the table is ReMoM's index alone; that runs
anywhere, without IDA.

## How functions are matched

`export_map.idc` writes each function's bytes from the 1.31 database, with relocation fixups masked (`..`). The script
finds each function in both files by those bytes, so a function counts as unchanged even if a patch moved it. A
function missing from the new file is changed. Its new location is the candidate (same offset, the segment's new
position, or a search for its first bytes) whose bytes match best, and the byte-level differences come from aligning
the two bodies. Functions whose longest run of known bytes is under 6 bytes are too short to place; their count is
reported.

Tests: `python3 -m unittest tools.mom_patch_diff.tests.test_diff tools.mom_patch_diff.tests.test_address_table`
