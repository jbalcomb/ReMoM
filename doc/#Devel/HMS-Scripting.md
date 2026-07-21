# HMS Scripting — the `.hms` scenario language

`.hms` ("**H**eMoM **M**otion **S**cript") files drive the game with simulated input: mouse clicks,
key presses, and waits, one action per frame, through the same input path a real player's mouse and
keyboard go through. They are used for:

- **Regression tests** — HeMoM headless runs that click through a flow and diff logs / saves.
- **Demo-reel scenes** — footage captured from the Release binary (see
  [BRA-Demo-Reel.md](../%23AI_Plans/BRA-Demo-Reel.md)).
- **Manual repro** — replaying a recorded bug without hand-playing to the state.

A `.hms` file is either hand-authored or produced by `rmr2hms` from a recorded `.RMR` and then
tightened by hand.

## Where it lives in the code

| Piece | File | Notes |
|---|---|---|
| Parser + per-frame executor | [src/Artificial_Human_Player.c](../../src/Artificial_Human_Player.c) | the only `.hms` interpreter |
| Header / public API | [src/Artificial_Human_Player.h](../../src/Artificial_Human_Player.h) | |
| Registration (ReMoMber) | [src/ReMoM.c](../../src/ReMoM.c) (`--scenario`) | |
| Registration (HeMoM) | [src/HeMoM.c](../../src/HeMoM.c) (`--scenario`) | |
| `.RMR` → `.hms` converter | [tools/rmr2hms.c](../../tools/rmr2hms.c) | standalone CLI |
| Record/replay format | [platform/replay/Replay.c](../../platform/replay/Replay.c), [platform/include/Platform_Replay.h](../../platform/include/Platform_Replay.h) | text CSV, `# ReMoM Replay v2` |
| Field catalog (coord → name) | [tools/fields/](../../tools/fields/) | 80 per-screen CSVs + `_static.csv` |

Run a scenario:

```sh
ReMoMber.exe --scenario demo/scenes/08b_som_complete.hms
HeMoM.exe    --scenario assets/test_worldgen.hms --seed1 12345
```

## Execution model

- The parser reads the whole file into a fixed action array (`HEMOM_MAX_ACTIONS = 4096`,
  [Artificial_Human_Player.c:64](../../src/Artificial_Human_Player.c#L64)) at load time. Parse errors
  log a line and **skip** that action — a bad line never aborts the run.
- `HeMoM_Player_Frame()` ([:624](../../src/Artificial_Human_Player.c#L624)) runs once per platform
  frame, executing at most one action per frame. It is registered as a platform frame callback.
- Input is injected by writing the engine's input globals
  (`Platform_Keyboard_Buffer_Add_Key_Press`, `platform_frame_mouse_buttons` + `User_Mouse_Handler`),
  so `Get_Input()` / `Interpret_Mouse_Input()` process it through the Fields system normally.
- When the action list is exhausted, the player sets `quit_game_flag = ST_TRUE`
  ([:642](../../src/Artificial_Human_Player.c#L642)) — a script that runs off the end quits the game.
  Use `end` to stop the player but leave the game running.

### Coordinate space

**All `click`/`rclick` coordinates are in the original 320×200 logical framebuffer.** Do not
pre-scale for the window: `HeMoM_Player_Frame` multiplies by `Platform_Get_Scale()` itself
([:734-736](../../src/Artificial_Human_Player.c#L734-L736)) and warps the OS cursor via
`Set_Pointer_Position` so a second consecutive click is not reverted to the real mouse position by the
next `SDL_GetMouseState` poll ([:742](../../src/Artificial_Human_Player.c#L742)).

### Timing

`1 frame = PLATFORM_MILLISECONDS_PER_FRAME = 55 ms` (the DOS BIOS tick). `wait` is **wall-clock**, not
frame-counted: it idles until `Platform_Get_Millies()` reaches the target
([:646-654](../../src/Artificial_Human_Player.c#L646-L654)).

## Command reference (implemented today)

Verbs are case-insensitive. `#` starts a comment (whole-line or trailing). Blank lines are ignored.

| Verb | Syntax | Meaning | Parser |
|---|---|---|---|
| `include` | `include FILE` | inline another `.hms` (relative to this file); max depth 8 | [:471](../../src/Artificial_Human_Player.c#L471) |
| `wait` | `wait N{f\|ms\|s\|m}` | idle N frames / ms / s / min. **Unit required** — a bare number is rejected. | [:493](../../src/Artificial_Human_Player.c#L493) |
| `key` | `key K` | press one key (single char) | [:506](../../src/Artificial_Human_Player.c#L506) |
| `escape` | `escape` | press Escape | [:514](../../src/Artificial_Human_Player.c#L514) |
| `enter` | `enter` | press Enter | [:520](../../src/Artificial_Human_Player.c#L520) |
| `backspace` | `backspace` | press Backspace | [:526](../../src/Artificial_Human_Player.c#L526) |
| `left` `right` `up` `down` | | move unit stack one square (arrow / numpad) | [:533-536](../../src/Artificial_Human_Player.c#L533-L536) |
| `upright` `downright` `upleft` `downleft` | | diagonal unit-stack move | [:537-540](../../src/Artificial_Human_Player.c#L537-L540) |
| `click` | `click X Y` | left-click at (X, Y) in 320×200 space | [:541](../../src/Artificial_Human_Player.c#L541) |
| `rclick` | `rclick X Y` | right-click at (X, Y) | [:551](../../src/Artificial_Human_Player.c#L551) |
| `next_turn` | `next_turn` | sugar for `key n` | [:561](../../src/Artificial_Human_Player.c#L561) |
| `type` | `type STRING` | one key per character; expands `$VAR` | [:579](../../src/Artificial_Human_Player.c#L579) |
| `quit` | `quit` | set `quit_game_flag` — exit the game | [:568](../../src/Artificial_Human_Player.c#L568) |
| `end` | `end` | stop the player, leave the game running | [:574](../../src/Artificial_Human_Player.c#L574) |

Any other verb logs `unknown action` ([:609](../../src/Artificial_Human_Player.c#L609)) and is skipped.

### Variables

Only `$SAVE_NAME` exists today — `AHP-YYYYMMDDHHMM`, populated at load
([:283-302](../../src/Artificial_Human_Player.c#L283-L302)). Substitution runs **only inside `type`**.
Max 4 variables.

## Named actions — clicking fields by name (design; not yet implemented)

The pain point: a recorded click on the Patrol button lands somewhere inside its rectangle — a
different `(x, y)` every recording. We want to standardize on the field's center and address it by a
stable name per screen, so scripts read `click Main_Screen.Patrol_Button` instead of `click 290 180`.

### The data already exists

[tools/fields/](../../tools/fields/) is a per-screen catalog of every `Add_*Field` call site. Each row
carries the runtime bounding box **and a precomputed center** `(rt_click_cx, rt_click_cy)`. Example —
[Main_Menu_Screen.csv:4](../../tools/fields/Main_Menu_Screen.csv#L4):

```
Main_Menu_Screen, MoM/src/MainMenu.c, 410, ..., _new_button, "108,162,211,173,...", ..., rt_click_cx=159, rt_click_cy=167
```

`assets/Stage_0.hms` already hand-clicks `160 168` for exactly that button. Named actions replace the
magic number with `click Main_Menu_Screen.New_Game_Button`, resolved to `(159, 167)` — the center.

### Name form: curated Proper_Snake_Case alias

The raw catalog `symbol` is the C identifier (`_new_button`, leading underscore; some call sites have
no LHS or an array-subscript symbol). Scripts use a **curated alias** in the project's
`Proper_Snake_Case` convention, mapped in a checked-in table:

```
# tools/fields/aliases.csv
# screen,             src_file,         src_line, symbol,        alias
Main_Menu_Screen,     MoM/src/MainMenu.c, 410,    _new_button,   New_Game_Button
Main_Screen,          MoM/src/MainScr.c,  2039,   _patrol_button,Patrol_Button
```

The alias is keyed on `src_file:src_line` (stable across coordinate changes) plus `symbol` (a tripwire
— if the symbol at that line changes, the row is stale and the tooling should flag it, not silently
mismap).

### Resolution: in-engine, via the RECORD sidecar log

A bare `(x, y)` is **ambiguous** across screens and popups: fields register into one global
`p_fields[]`, and a modal overlay's fields sit on top of the underlying screen's
([tools/fields/README.md](../../tools/fields/README.md), "Overlay fields"). Point-in-rect over the CSVs
alone cannot tell which screen was active.

The engine can, because at click time it knows `current_screen`
([MOX_T4.h:61](../../MoX/src/MOX_T4.h#L61)) and which field the cursor is inside. The record log
already resolves each click to a field — today only to an **index**
([HeMoM.c:795-807](../../src/HeMoM.c#L795-L807)):

```
idx=10  mouse=(291,104) btn=1  ...  field[1]=(0,0)-(319,199)
```

The design extends that one log line to carry the screen tag and the `Add_*Field` call site, so the
join to `aliases.csv` is unambiguous:

```
idx=10  mouse=(291,104) btn=1  ...  field[1]=Main_Menu_Screen@MainMenu.c:410
```

`rmr2hms` then reads the `.RMR`'s sidecar `*-RECORD.log`, and for each click looks up
`screen@file:line` → alias, emitting:

```
click Main_Menu_Screen.New_Game_Button    # resolved to the field center (159,167)
```

Falling back to raw `click X Y` (with a comment) when no field matched or no alias is defined.

### Runtime side must be re-enabled first

The field-catalog instrumentation exists but is **commented out** in
[Fields.c:899](../../MoX/src/Fields.c#L899) and [:905-911](../../MoX/src/Fields.c#L905-L911) (all
`// trc_prn(...)`). The interception macros that capture each call site's `__FILE__:__LINE__` are live
under `STU_DEBUG` ([Fields.h:853-862](../../MoX/src/Fields.h#L853-L862)), and `s_Field` is
byte-offset-faithful to the disassembly ([Fields.h:90-154](../../MoX/src/Fields.h#L90-L154)) — so the
name association must live in a **debug-only shadow array keyed by field index**, never a new field in
`s_Field`. Re-enabling those lines also unblocks regenerating the catalog for screens not yet captured.

## State-aware waits (design; not yet implemented)

The biggest robustness problem in current scripts is blind `wait 800ms` sleeps that guess how long an
animation or screen transition takes. `08b_som_complete.hms` is full of them, and they are the reason a
scene breaks when timing drifts. Replace them with waits that block on real game state:

| Verb | Syntax | Blocks until | Backing global |
|---|---|---|---|
| `wait_screen` | `wait_screen NAME` | `current_screen == scr_NAME` | [`current_screen`, MOX_T4.h:61](../../MoX/src/MOX_T4.h#L61) |
| `wait_turn` | `wait_turn N` | `_turn >= N` | [`_turn`, MOM_DAT.h:4235](../../MoX/src/MOM_DAT.h#L4235) |
| `wait_field` | `wait_field SCREEN.ALIAS` | that field is registered and hittable | `p_fields[]` scan |

`NAME` maps to `e_SCREENS` in [MoM/src/MOM_SCR.h](../../MoM/src/MOM_SCR.h) (`scr_Main_Menu_Screen`,
`scr_Main_Screen`, `scr_Magic_Screen`, …). Each state-aware wait needs a **timeout** so a script that
never reaches the state fails loudly instead of hanging a CI run — proposal: reuse the `wait`
duration-suffix grammar, e.g. `wait_screen Main_Screen 10s`, defaulting to a generous cap.

`08b_som_complete.hms` becomes, e.g.:

```
# old — brittle
click 290 180
wait 800ms
click 290 180

# new — synchronized
wait_field Main_Screen.Patrol_Button
click Main_Screen.Patrol_Button
wait_field Main_Screen.Patrol_Button
click Main_Screen.Patrol_Button
```

This need is already filed in `doc/__TODO-Test.md` ("Artificial_Human_Player needs state-aware waits").

## Known gaps / open issues

- `demo/scenes/` does not currently exist on disk; the richest hand-authored example is
  [assets/Stage_0.hms](../../assets/Stage_0.hms), whose every click carries a comment quoting the
  originating `Add_*Field` call — the manual version of named actions.
- `rmr2hms` emits exact-ms waits and as-recorded coordinates; both usually want hand-tightening.
- `rmr2hms` duplicates the MOX key constants ([rmr2hms.c:44-56](../../tools/rmr2hms.c#L44-L56)) instead
  of including `Platform_Keys.h` — keep them in sync by hand until that's fixed.
- `Platform_Replay.h`'s header comment still describes an obsolete binary `"RMOM"` format; the real
  format is the text CSV documented at [Replay.c:7-13](../../platform/replay/Replay.c#L7-L13).
- No `ReMoM.ini` toggle for `g_dbg_fields_trace` yet.

## Related docs

- [PRD-Scripted-Demo-Capture.md](../%23AI_Plans/PRD-Scripted-Demo-Capture.md) — the in-engine capture machinery.
- [BRA-Demo-Reel.md](../%23AI_Plans/BRA-Demo-Reel.md) — why footage must come from the real player path.
- [Scenario-Patching.md](Scenario-Patching.md) — `--patch` to set up a starting state.
- [tools/fields/README.md](../../tools/fields/README.md) — the field-catalog schema and pipeline.
