# dummy_lbx

Generates stand-in `.LBX` files so ReMoM and HeMoM can run **without the original Master of Magic data**:
noise pictures, noise sounds, placeholder text and numbers. For development, CI and mod work — not for playing.

```sh
python3 -m tools.dummy_lbx /path/to/out              # 103 archives, ~20 MB, about 25 s
python3 -m tools.dummy_lbx /path/to/out --seed 7     # same seed -> byte-identical files
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
| Data tables | Original record count and size (see `DATA_TABLES`), zero-filled, with placeholder names and text, and the few numbers the game cannot take as zero (spell costs, building costs, diplomacy message counts, summon unit types). |
| `TERRAIN.LBX` | A tile sheet with a valid 20x18 header at every tile the index (entry 1) points to. |
| Sounds | `0xDEAF` entries: a noise VOC (type 2) or a silent XMIDI (type 1, `MUSIC.LBX`). |
| Everything else | Random bytes of the original length. |

The game's SHA-256 compatibility check reports these files as "unrecognized" — expected.

## Results (Linux, clang-debug, HeMoM integration tests with `REMOM_DATA_DIR` set)

Without any data, none of the HeMoM integration tests can run. With these files:

- **Pass:** WorldGen_Run, WorldGen_Validate, Continue_Setup / Run / Assertions, NextTurn_Setup / Run,
  AIBehavior_Setup, Combat_Strategic_Run / Assertions, Cast_Summon_Run / Assertions, Preflight_Missing_Data.
- **Fail by design** (they assert values from the real data): WorldGen_Fields (city names), OOB_Autotiling
  (terrain codes), NextTurn_Assertions (a gold total).
- **AIBehavior_Run** times out: the scripted player ends up waiting on the Army List screen; its click script was
  recorded against the real game's screens.
- **MainScr_WildGame_Run** pins `REMOM_DATA_DIR` to the build's binary folder, so it does not see these files.

## Limits

- Placeholder numbers make a game that runs, not one that plays sensibly.
- Formats are only as right as the code paths exercised so far; a crash on a screen nobody has opened yet may need
  another rule here. Run HeMoM under `gdb` and look at what the crashing code reads.
