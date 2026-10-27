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
    mod.ini            optional for now: [Mod] name=..., version=...
    scripts/main.lua   run once at load; registers handlers with game.on(...)
```

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

The results are stored in the original one-byte city fields, so with a handler registered the final value is clamped: gold to 0..255, the others to -128..127 (a warning is logged).

If a handler raises an error, returns a non-number, or returns a value outside 32-bit range, the value is kept unchanged and the error is logged.

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

## Code

| File | Role |
|---|---|
| [MOD/src/MOD_Hook.h](../../MOD/src/MOD_Hook.h) | Hook ids and the C dispatcher API |
| [MOD/src/MOD_Lua.cpp](../../MOD/src/MOD_Lua.cpp) | Lua runtime through sol2 (the only C++ in the game) |
| [MOD/src/MOD_Load.c](../../MOD/src/MOD_Load.c) | Mod folder loader |
| [MOD/tests/](../../MOD/tests/) | Unit tests (`MOD_tests`) |
| [ext/lua/](../../ext/lua/), [ext/sol2/](../../ext/sol2/) | Vendored Lua 5.4.8 and sol2 3.5.0, each with `PROVENANCE.md` |

## Not yet

- CAS script interpreter, and the other Caster of Magic hooks (unit stats, combat, spells, AI).
- Data tables (units, spells, buildings) loaded from mod files; asset overrides.
- Recording the loaded mods in save files.
- An end-to-end test that runs a game with a mod (needs the original LBX files, so it belongs with the HeMoM integration tests).
