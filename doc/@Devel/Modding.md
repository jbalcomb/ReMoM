# Modding — Phase 1 (Lua hooks)

ReMoM can now load a mod folder whose Lua script changes the five city-economy results.
With no mod loaded nothing changes: every hook returns the value the original code computed.

## Using a mod

```
./HeMoM --mod mods/example_economy --newgame
./ReMoM --mod mods/example_economy
```

`--mod <dir>` may be given more than once; mods run in the order given.
Log lines go to the `MOD` category (`[Log]` section of `ReMoM.ini`).

## Mod folder

```
mods/<name>/
    mod.ini            [Mod] name=..., version=...; [CAS] binds Caster of Magic scripts to hooks
    scripts/main.lua   optional; run once at load; registers handlers with game.on(...)
```

A mod needs `scripts/main.lua`, at least one `[CAS]` script, or both.

See [mods/example_economy](../../mods/example_economy) for a complete example.

## Hooks

| Hook name | Wraps | Called from |
|---|---|---|
| `city_food` | `City_Food_Production()` | `Do_City_Calculations()`, [CITYCALC.c](../../MoM/src/CITYCALC.c) |
| `city_production` | `City_Production_Production()` | same |
| `city_gold` | `City_Gold_Production()` | same |
| `city_research` | `City_Research_Production()` | same |
| `city_power` | `City_Mana_Production()` | same |

A handler is `function(city_idx, value) return new_value end`.
Handlers on one hook run in registration order, each receiving the previous result, so mods stack.
A handler that ignores `value` overrides the original computation.
The hooks run **before** the AI difficulty modifier.

Results are stored as full `int32_t` (the city economy fields were one byte in 1.31). Where 1.31 would have wrapped a
value, the `OGBUG` log category records it.

If a handler raises an error, returns a non-number, or returns a value outside 32-bit range, the value is kept unchanged and the error is logged.

## Save files

ReMoM writes its own save format ([MoX/src/SAVEFILE.h](../../MoX/src/SAVEFILE.h)): a `ReMoMSav` header, then tagged
sections, each holding an in-memory array as-is with its record size and count. Structures can grow (`struct s_CITY`
went from 114 to 134 bytes) without converting anything; a save from a build with different record sizes is refused,
naming the section; sections the loader does not know (e.g. added by a mod) are skipped.

1.31-layout saves (from 1.31 or earlier ReMoM builds) still load: they are imported once, and the next save is written
in the ReMoM format. Nothing writes the 1.31 layout any more, so
[tools/parity_check.py](../../tools/parity_check.py) (byte comparison against DOSBox saves) applies to the `faithful`
branch only. [src/Game_Save_Dump.c](../../src/Game_Save_Dump.c) and `savedump` read both formats.

## Script API (`game` table)

Names follow Caster of Magic's CAS commands, lower-cased, so CAS scripts can later be converted line by line.

| Function | Returns | CAS equivalent |
|---|---|---|
| `game.on(hook, fn)` | — registers `fn` on `hook` | — |
| `game.rnd(x)` | random integer `0 .. x-1` from the game's RNG | `RND(X)` |
| `game.maxcity()` | highest city index, `-1` if none | `MAXCITY()` |
| `game.cityowner(c)` | owner player index | `CITYOWNER(C)` |
| `game.cityrace(c)` | race | `CITYRACE(C)` |
| `game.citysize(c)` | size | — |
| `game.isbuilt(c, b)` | `1` if building `b` is built (or replaced), else `0` | `ISBUILT(C,B)` |
| `game.log(text)` / `print(...)` | writes to the log | — |

An invalid city or building index raises a Lua error (logged; the hook keeps its value).

## Sandbox and determinism

- Loaded libraries: base, coroutine, math, string, table, utf8. Not available: `io`, `os`, `package`, `debug`, `dofile`, `loadfile`, `load`, `require`.
- `math.random` draws from the game's `Random()`; `math.randomseed` is removed. Scripts therefore stay deterministic under `--record` / `--replay`.

## CAS scripts (Caster of Magic)

ReMoM runs Caster of Magic for Windows `.CAS` scripts directly ([MOD/src/CAS.h](../../MOD/src/CAS.h) describes the language). Bind them in `mod.ini`:

```ini
[CAS]
master=scripts/MASTER.CAS        ; optional (CoM2: System); run first, its variables are readable everywhere
city_gold=scripts/EcoGold.CAS    ; a hook name, or CoM2's [Scripts] name: EconomyGold, EconomyFood,
EconomyFood=scripts/EcoFood.CAS  ; EconomyProd, EconomyResearch, EconomyPower
```

- A hook script gets `CITYID` and `VALUE` (the value so far), and `RESULT` starts as `VALUE`. If the script adds `ADDRESLINE` lines (CoM2's economy scripts work this way), the hook gets their total. Otherwise it gets `RESULT`.
- The economy hooks also pass the inputs CoM2 gives its economy scripts, computed by [MoM/src/City_Hook_Inputs.c](../../MoM/src/City_Hook_Inputs.c) only when the hook has a handler:

  | Hook | Inputs |
  |---|---|
  | `city_food` (EconomyFood) | `FARMERS`, `MAXPOP` |
  | `city_production` (EconomyProd) | `WORKERS`, `FARMERS`, `PRODPER` (production % from terrain) |
  | `city_gold` (EconomyGold) | `ROADBONUS` (road trade %), `TERRAINGOLD` (shore/river trade %), `BASEGOLD` (tax gold) |
- A `[CAS]` key naming a CoM2 script ReMoM has no hook for yet (e.g. `EconomyUnrest`) is logged and skipped.
- A script that fails to parse fails the mod load. A run-time error, such as a built-in ReMoM does not implement yet, is logged and the hook keeps its value.
- IDs follow CoM2, not ReMoM's arrays ([MOD/src/CAS_Game.h](../../MOD/src/CAS_Game.h)):
  - Cities and units are 1-based (ReMoM index + 1), and 0 means "none". Wizards are 0 (human) to `NOFWIZARDS()`. Planes are 1 (Arcanus) and 2 (Myrror).
  - Buildings, city and global enchantments, retorts, realms, ores, overland tiles and events use the constants in Warlord's MASTER.CAS. They are translated to 1.31's by name; CoM2-only ones read as absent.
  - `DIFFICULTY()` puts 1.31's five levels on CoM2's nine as 1, 2, 3, 5, 7 (Hard and Impossible are a judgement call). `SCORINGOPTION()` is always 0, because ReMoM has none of CoM2's options.
- `GETCITYDATA`/`SETCITYDATA` and `GETWIZARDDATA`/`SETWIZARDDATA` are 100 integer slots per city and per wizard, kept for the scripts. They are saved with the game as the sections `CASCITYD` and `CASWIZD ` ([MoX/src/SAVEFILE.h](../../MoX/src/SAVEFILE.h) `Save_File_Register_Section`). A save made without the mod loads them as zeros.
- Expressions have no precedence and are evaluated right to left: `A*B+C` is `A*(B+C)`.
- CoM2 accepts some broken syntax: a missing `;` or `)`, an extra `)` or `}`, a comment, string or `{` left open, a second `ELSE`. ReMoM accepts the same and logs each case as a warning at load.

`cas_check` checks scripts without running the game. It reports parse errors, warnings, and which built-ins are used and implemented:

```bash
./out/build/clang-debug/bin/Debug/cas_check -v /path/to/WARLORD/*.CAS
```

Status against Warlord 1.5.12.6.2 plus CoM2 1.05.11's own scripts: all 44 files parse, with 275 warnings (132 missing `;`, 95 missing `)`, 39 extra `)`, and 9 structural problems: brace, comment and `ELSE` errors). Of the 111 built-ins they use, 46 are implemented, including all 40 that Warlord's five economy scripts (EcoFood, EcoGold, EcoProd, EcoRes, EcoPower) use. With those five scripts bound, a HeMoM new game plus one turn on the generated stand-in data ran without a script error and changed the cities' values. `GETSTAT` reads only `SOwner` so far.

## Rules profiles (game rules in Lua)

Game rules that differ between games live in Lua, one folder per rules profile under
[assets/rules/](../../assets/rules/). The C code keeps the engine work (reading the map, iterating a city's area) and
asks the loaded profile for each rule's result. The faithful C version of each rule stays as the fallback, and as the
reference the 1.31 script is tested against. The API is in [MOD/src/MOD_Rules.h](../../MOD/src/MOD_Rules.h).

```bash
./HeMoM --rules com2 --newgame          # original (default), community_patch, com2, a folder path, or none (C only)
./ReMoM --rules community_patch
```

| Profile | What it is |
|---|---|
| `original` | 1.31: a line-for-line translation of the reconstructed C. The default. |
| `community_patch` | Starts from `original` and overrides what the Community Patch changes (no change confirmed for the ported rules yet). |
| `com2` | Caster of Magic for Windows 1.05.11, with values from its `Data/TERRAIN.INI`. |

- **Profile layout.** A profile is `rules/<name>/profile.lua`, which loads its files with `rules.load("file.lua")`, or
  another profile's with `rules.load("original", "file.lua")`. That makes "1.31 plus these changes" a two-line profile.
  The folder is looked for in the current directory, then beside the executable (the build copies `assets/`
  there). File and profile names may not leave the rules folders.
- **Rules ported so far:** `rules.square_production_bonus(terrain_type, gaias_blessing, shared)`, the production % a
  square of a city's area gives. `Square_Production_Bonus()` in [MoM/src/Terrain.c](../../MoM/src/Terrain.c) asks
  for it; the faithful C body is `Square_Production_Bonus_Original()`.
- **Fidelity is tested.** [MoM/tests/test_Square_Production_Bonus_Rules.cpp](../../MoM/tests/test_Square_Production_Bonus_Rules.cpp)
  runs all 65,536 map values, with Gaia's Blessing 0/1/2 and shared on and off (393,216 inputs), through the
  `original` profile and through the C original, and requires identical results. A HeMoM new game is byte-identical
  under `--rules original` and `--rules none`. After a played turn the only difference is the wizards'
  `research_spells` list, which already differs between two identical runs on `main`.
- **Errors.** A rule that errors, or returns something other than an integer, falls back to its C version. The first
  10 errors of each rule are logged.

To port another rule:
1. Rename the faithful C function `<Name>_Original`, unchanged.
2. Add a `<Name>` that reads its inputs and calls `MOD_Rules_Call_Int("<rule>", ...)`, falling back to
   `<Name>_Original`.
3. Translate the C into `rules/original/`.
4. Add the all-inputs equivalence test.
5. Write the other profiles' versions.

## Code

| File | Role |
|---|---|
| [MOD/src/MOD_Hook.h](../../MOD/src/MOD_Hook.h) | Hook ids and the C dispatcher API |
| [MOD/src/MOD_Lua.cpp](../../MOD/src/MOD_Lua.cpp) | Lua runtime through sol2 (the only C++ in the game) |
| [MOD/src/MOD_Load.c](../../MOD/src/MOD_Load.c) | Mod folder loader, `[CAS]` hook bindings, `--rules` |
| [MOD/src/MOD_Rules.h](../../MOD/src/MOD_Rules.h) | Rules profiles (implemented in `MOD_Lua.cpp`) |
| [assets/rules/](../../assets/rules/) | The rules profiles: `original`, `community_patch`, `com2` |
| [MOD/src/CAS.c](../../MOD/src/CAS.c) | CAS interpreter |
| [MOD/src/CAS_Game.c](../../MOD/src/CAS_Game.c) | CAS built-ins that read and change the game; CoM2 → ReMoM ID tables |
| [MoM/src/City_Hook_Inputs.c](../../MoM/src/City_Hook_Inputs.c) | Economy hooks with CoM2's script inputs |
| [tools/cas_check.c](../../tools/cas_check.c) | CAS checker CLI |
| [MOD/tests/](../../MOD/tests/) | Unit tests (`MOD_tests`) |
| [ext/lua/](../../ext/lua/), [ext/sol2/](../../ext/sol2/) | Vendored Lua 5.4.8 and sol2 3.5.0, each with `PROVENANCE.md` |

## Not yet

- The other 65 CAS built-ins Warlord uses (unit, spell, wizard and map queries and commands), and the other Caster of Magic hooks (unit stats, combat, spells, AI).
- CAS→Lua converter.
- More rules ported to profiles; the loaded profile recorded in the save; CoM2 values read from a mod's TERRAIN.INI
  instead of written into `rules/com2/terrain.lua`; volcano owner production for CoM2.
- Data tables (units, spells, buildings) loaded from mod files; asset overrides.
- Recording the loaded mods in save files. The CAS slots are not cleared when a new game starts in the same session.
- An end-to-end test that runs a game with a mod (needs the original LBX files, so it belongs with the HeMoM integration tests).
