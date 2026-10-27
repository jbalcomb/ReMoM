# dummy_lbx

Generates stand-in `.LBX` files so ReMoM and HeMoM can run **without the original Master of Magic data**:
noise pictures, noise sounds, placeholder text and numbers. For development, CI and mod work — not for playing.

```sh
python3 -m tools.dummy_lbx /path/to/out              # 103 archives, ~20 MB, about 25 s
python3 -m tools.dummy_lbx /path/to/out --seed 7     # same seed -> byte-identical files
python3 -m tools.dummy_lbx --check /path/to/folder   # validate any LBX folder (generated or original)
REMOM_DATA_DIR=/path/to/out ./HeMoM --newgame
```

## How it works

The shape of every archive comes from [tools/fields/assets.fwv](../fields/assets.fwv), the catalog
[tools/asset_catalog](../asset_catalog/) built from the original files: same archives, same entry counts and names,
and **every entry exactly its original byte length** (the game sizes its buffers from those lengths).
Only the content is invented.

| Entry kind | What is generated |
|---|---|
| Pictures (FLIC) | Original width / height / frame count, random pixels. Raw noise where the original length allows, runs of one colour where it is tighter, transparent columns as a last resort. Pictures that carry a palette get a random 6-bit palette. |
| Data tables | Every field of every record drawn from the range the game code accepts, and checked against it as it is written (see *Valid ranges* below). |
| `TERRAIN.LBX` | A tile sheet with a valid 20x18 header at every tile the index (entry 1) points to. |
| Sounds | `0xDEAF` entries: a noise VOC (type 2) or a silent XMIDI (type 1, `MUSIC.LBX`). |
| Everything else | Random bytes of the original length. |

The game's SHA-256 compatibility check reports these files as "unrecognized" — expected.

## Valid ranges

[tables.py](tables.py) describes every data table field by field: offset, type, and the values the game code accepts,
each with the code that constrains it (the array it indexes, the switch it feeds, a `Random()` argument that must be
at least 1). [schema.py](schema.py) enforces them in both directions:

- **Generating:** each value is drawn from its range (or a narrower typical range), then checked; a value outside its
  range, or a record that breaks a cross-field rule, stops the build with the field and the reason.
- **Checking:** `--check FOLDER` reports every out-of-range field and broken rule, citing the code. Run it on the
  original files to find a range that is too narrow; a few original values are flagged on purpose because the code
  mishandles them (e.g. ITEMDATA embedded-spell indexes above 127 read as negative through an `int8_t`).

Rules beyond single fields: spell realm = `(index - 1) / 40`; city and global enchantment slots unique; summon spells
name a fantastic unit 154..197; building prerequisites are 0..35 or a terrain code 101/104/110/200, replacements -1
or another building, shapes 0..3 for buildings 3..34; diplomacy message counts 1..15; help rectangles not inverted;
TERRTYPE shore tiles 2..161 and mountain tiles 0xA4 / 0x103..0x112; ITEMDATA slot and icon match the item type.

**Spell kinds come from the documented table.** The code treats spell indexes as specific spells (`spl_Magic_Spirit`
= 201 is summoned by index; `spell_data_table[spl_Hydra].unit_type` is compared directly). So each spell's category,
book, castability, AI group and effect slot are taken from [doc/spell_data_table.json](../../doc/spell_data_table.json)
when it exists (falling back to random valid values with a warning); names, costs and sounds are generated.

## Results (Linux, clang-debug, HeMoM integration tests with `REMOM_DATA_DIR` set)

Without any data, none of the HeMoM integration tests can run. With these files 15 of 21 pass:

- **Pass:** WorldGen_Run, WorldGen_Validate, Continue_Setup / Run / Assertions, NextTurn_Setup / Run,
  AIBehavior_Setup / Run, Combat_Strategic_Run / Assertions, Cast_Summon_Run / Assertions, MainScr_WildGame_Stage,
  Preflight_Missing_Data.
- **Fail by design** (they assert values from the real data): WorldGen_Fields (city names), OOB_Autotiling
  (terrain codes), NextTurn_Assertions (a gold total), AIBehavior_Evaluate (one of 22 checks: neutral-army events,
  which depend on what the placeholder map contains).
- **MainScr_WildGame_Run** pins `REMOM_DATA_DIR` to the build's binary folder, so it does not see these files.

## Limits

- Placeholder numbers make a game that runs, not one that plays sensibly.
- Formats are only as right as the code paths exercised so far; a crash on a screen nobody has opened yet may need
  another rule here. Run HeMoM under `gdb` and look at what the crashing code reads.
