# mom_patches

Indexes the FILESET patch files that come with drake178's Community Patch (`140` = Kyrub's 1.40n, `151`, `152`,
`Optional`, each with an `OBSOLETE/` folder) and writes a porting checklist per version for ReMoM.

A patch file is `;` comment lines and records of three hex lines: file offset, original byte, new byte. Comment
blocks between records start a new section. The name gives the target and the kind: `W`/`IW` → WIZARDS.EXE,
`M`/`IM` → MAGIC.EXE, then the number and `FIX`/`FEAT`/`AI`/`DIPL`/`OPT`/`REV`. An `; apply to X.exe` line overrides
the name (`I001` patches ITEMMAKE.EXE). A 2-line file that says *This patch is now obsolete* is a stub; its original
is in `OBSOLETE/`.

```bash
cd /c/STU/devel/ReMoM
# names and descriptions only:
python3 -m tools.mom_patches.patch_index /c/MoM/patches --out /c/MoM/patch_index --remom .
# with the original bytes checked and every patched byte placed in a function (needs the export_map.py maps):
python3 -m tools.mom_patches.patch_index /c/MoM/patches --out /c/MoM/patch_index --remom . \
    --base WIZARDS.EXE=/c/MoM/MoM131/WIZARDS.EXE --base MAGIC.EXE=/c/MoM/MoM131/MAGIC.EXE \
    --map WIZARDS.EXE=/c/MoM/wizards_map.tsv --map MAGIC.EXE=/c/MoM/magic_map.tsv
```

Writes, all tab-separated with a header row:

| File | What it holds |
|---|---|
| `patches.tsv` | one row per patch file: version, status (current / obsolete / obsolete stub), exe, number, kind, title, sections, bytes, how many original bytes match `--base`, functions patched (with `--map`), the ReMoM functions they are, drake178 names mentioned and their ReMoM functions, patches it mentions (reverses, obsoletes, requires), description |
| `records.tsv` | one row per patched byte: offset, original, new, whether the original matches `--base`, and the function (`WZD s161p04+0x1A`) or data item it is in |
| `todo_<version>.md` | checklist of the version's current patches grouped by kind, each with what it touches in ReMoM; obsolete patches listed apart |

**Matching to ReMoM.** Without maps, a patch is matched to ReMoM only through drake178 function names in its
comments (`TILE_GetProd()`), looked up in ReMoM's `// drake178: NAME()` comments. ReMoM records 441 of those names,
so this is partial. With `--base` and `--map`, every patched byte is placed in a function by offset, which is
complete. The original-byte check also shows which version each patch expects as its base: a patch whose original
bytes don't match 1.31 applies on top of earlier patches.

Tests: `python3 -m unittest tools.mom_patches.tests.test_patch_index`
