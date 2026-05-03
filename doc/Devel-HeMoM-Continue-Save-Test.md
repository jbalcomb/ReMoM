
# HeMoM Continue + Save Test

An end-to-end gameplay integration test that exercises the **Continue** code path (the WIZARDS.EXE path) and verifies specific game-state changes caused by a scripted artificial human player session.

## What This Test Does

The test is a chain of three CTest steps with `FIXTURES_SETUP` / `FIXTURES_REQUIRED` ordering, all labeled `continue`:

| # | Test | What it does |
|---|------|---|
| 1 | `HeMoM_Continue_Setup` | Runs `HeMoM --newgame test_worldgen.ini --scenario test_worldgen_smoke.hms` to produce a fresh `SAVE9.GAM` with a deterministic seed. |
| 2 | `HeMoM_Continue_Run` | Runs `HeMoM --continue --scenario test_continue_save.hms --dump-save SAVE1.GAM`. This loads SAVE9.GAM as if the player clicked Continue from the Main Menu, drives the game through a scripted sequence, and dumps the resulting SAVE1.GAM to text. |
| 3 | `HeMoM_Continue_Assertions` | Runs `check_save_fields SAVE1.txt assert_continue_save.txt` to verify specific field values. |

### The Scenario

[test_continue_save.hms](../assets/test_continue_save.hms) drives the game through this flow:

1. **Home-city naming popup** (fires on first entry to Main Screen with `_turn == 0`)
   - `backspace` — clears the random pre-filled name in one stroke
   - Types `Claudesville`
   - `enter` — confirms, popup closes
2. **Main Screen — Patrol the starting stack**
   - Clicks the Patrol button at (290, 180)
3. **Open the Game menu → Save / Load screen**
   - Hits `G` hotkey
4. **Save / Load screen — save to slot 1 with custom name**
   - Clicks save slot 1 at (80, 54) to activate the name input field
   - 20 backspaces to clear whatever name was there
   - Types `AHP-202604071014` (dashes because the input field rejects underscore)
   - Clicks the Save button at (140, 177) — this calls `Save_SAVE_GAM(0)` → writes `SAVE1.GAM`
5. **Return to Main Screen → Main Menu**
   - Hits `G` again, clicks the Quit button on the Save/Load screen at (62, 177)
   - That returns to `scr_Main_Menu_Screen`
6. **Quit to DOS**
   - Hits `Q` hotkey on the Main Menu → `scr_Quit_To_DOS` → sets the local `quit_flag`
   - `Screen_Control()` loop exits, HeMoM main() runs the dump step, shuts down

## Tested State Changes

These are the fields the test explicitly asserts in [assert_continue_save.txt](../tests/assert_continue_save.txt). Each was chosen because it represents a direct effect of the artificial human player's input.

### Wizard identity (sanity checks)

These don't change during the scenario — they're carried over from `test_worldgen.ini` via the new game config. Asserting them confirms the continue path loaded the right save.

```
player[0].wizard_id = 6          # Rjak
player[0].name = Rjak
player[0].capital_race = 8       # High Men
player[0].banner_id = 4          # Yellow
```

### City rename (home-city naming popup)

The scenario types `Claudesville` into the first-turn naming popup for the human player's home city (city[0]).

```
city[0].name = Claudesville
city[0].owner_idx = 0
city[0].race = 8                 # High Men, matches player[0].capital_race
```

### Patrol action (Patrol button click)

The human player starts with two units in a single stack at the home city location. `Stack_Action(..., 1, 0, 0)` — action code 1 is Patrol — sets the whole stack's status. Both units become `us_Patrol (1)` and get `finished = 1` so they don't move again this turn.

```
unit[0].owner_idx = 0
unit[0].status = 1               # us_Patrol (was us_Ready=0)
unit[0].finished = 1             # marked done for this turn

unit[5].owner_idx = 0
unit[5].status = 1
unit[5].finished = 1
```

**Note on indices:** Units 0 and 5 are both owned by player 0 and co-located at the home city's tile. The game uses a sparse slot-array layout — the two starting units happen to land at indices 0 and 5 when the world generator populates the `_UNITS[]` table, not at 0 and 1.

## Other State Changes (Not Asserted)

The `diff SAVE9.txt SAVE1.txt` output shows more changes than just the ones we assert. These are **side effects** of the continue load path and the in-game save path. They are deliberately not asserted because they are not caused directly by the artificial human player's input, and asserting them would make the test brittle against unrelated engine changes.

| Field | SAVE9 | SAVE1 | Cause |
|---|---|---|---|
| `game.unit = 0` | 0 | 5 | The game's current unit selection index. `GAME_NextHumanStack()` and the Patrol action together advance the selection to the next idle stack. Since the patrolled stack is now finished, the next `Main_Screen_Reset()` call advances the selection index. |
| `player[0].power_base = 0` (+ players 1–4) | 0 | 9-12 | `Loaded_Game_Update()` → `All_Colony_Calculations()` recomputes per-player power from cities, nodes, and retorts. On a fresh save, power_base is zero; after load it is filled in. |
| `city[0..4].bldg_status[0] = 0` | 0 | 1 | Building slot 0 (likely "Barracks" or another always-built starting building) gets marked as Built during load-time city recalculation. On the freshly written SAVE9.GAM, it was still Not Built. |
| `# Source: SAVE9.GAM` → `# Source: SAVE1.GAM` | — | — | The dump file's header comment — not a game state field, just a label. |

**Why these aren't asserted:**

- `power_base` and `bldg_status[0]` are the output of `Loaded_Game_Update()`. Any future change to how load-time recomputation works would flip these values and break the test — even though nothing the test cares about actually changed. That's exactly the brittleness we want to avoid.
- `game.unit` is a UI-selection pointer, not a gameplay outcome. The exact value depends on the order cycles through idle stacks and could reasonably change with engine tweaks.
- If the test needs to catch regressions in load-time recomputation, those should live in a separate dedicated test against a known-good golden reference (which is what `HeMoM_WorldGen_Golden` already does for new game creation).

---

## Footnote: Code Changes Required

Getting this test working required several fixes to the HeMoM tooling and the headless platform backend. Most of these were latent bugs exposed by trying to drive the game through synthetic input headlessly for the first time.

### Platform layer

**`platform/headless/headless_PFL.c` — `Platform_Get_Window_Width()`**

Previously returned `PLATFORM_WINDOW_WIDTH` (1280), but `Platform_Get_Scale()` on headless returns 1. `User_Mouse_Handler()` in [MoX/src/Mouse.c](../MoX/src/Mouse.c) computes its own scale as `window_width / SCREEN_WIDTH`, so incoming window coords were being divided by 4 — clicks at `(62, 177)` became game coords `(15, 44)`, missing every button. Fixed by returning `PLATFORM_SCREEN_WIDTH` (320) so the computed scale is 1, matching `Platform_Get_Scale()`.

**`platform/win32/win_PFL.cpp` — WM_PAINT guard**

`ShowWindow(SW_SHOW)` in `Win_Init_Window()` synchronously dispatches `WM_PAINT` before `MOM_main()` has run `Allocate_Data_Space()` to populate `video_page_buffer[]`. Added a NULL guard so `WM_PAINT` skips drawing when the engine framebuffer isn't allocated yet.

**`platform/win32/win_PFL.cpp` — `Platform_Mouse_Input_Enable()` missing**

The Win32 backend never called `Platform_Mouse_Input_Enable()`, so `platform_mouse_input_enabled` stayed 0 and all mouse clicks were ignored by the engine. SDL2/SDL3 called it from `WINDOW_FOCUS_GAINED`. Added the call to `WM_CREATE` and `WM_ACTIVATEAPP`.

**`platform/win32/win_PFL.cpp` — `Platform_Palette_Update()` R/B channel swap**

The Win32 backend had red and blue channels swapped when building the palette buffer for `StretchDIBits`'s `BI_RGB` format. Fixed by swapping the source byte offsets when writing `.r` and `.b`.

**`platform/win32/win_PFL.cpp` — Backspace/Enter packed as ASCII**

The Win32 `WM_KEYDOWN` handler was setting `mox_character = '\b'` for Backspace and `'\r'` for Enter. `Read_Key()` returns the character byte when non-zero, so input loops checking against `ST_KEY_BACKSPACE (0x0B)` and `ST_KEY_ENTER (0x0C)` never matched — they got `0x08` and `0x0D` instead. Fixed by leaving `mox_character = 0` for those keys so `Read_Key()` returns the MoX key code.

### Artificial human player (Artificial_Human_Player.c)

**Missing `act_BACKSPACE`**

The scenario parser had `backspace` as a command but the action enum and executor didn't have a corresponding case. Scenarios with `backspace` lines were silently dropped as "unknown action" and the text typed afterward got appended to whatever was already in the input field. Added `act_BACKSPACE` with a dedicated `Pack_Key_Backspace()` that leaves the character byte at 0.

**Inline comment stripping**

The single-token actions (`backspace`, `enter`, `escape`, `quit`, `end`) used `stu_stricmp(p, "...")` — full-string match — while multi-token actions (`key X`, `wait N`, `click X Y`) used `stu_strnicmp` — prefix match. Lines like `backspace                 # clear the pre-fill` in a .hms file were being compared whole, so the comment prevented the match. The parser now strips inline `#` comments before matching.

**Enter/Escape also had character-byte bugs**

Same root cause as the Win32 Backspace issue. `Pack_Key_Enter()` was setting `'\r'` in the character byte; `Pack_Key_Escape()` was setting `0x1B` (which happens to be correct because `ST_KEY_ESCAPE == 0x1B` too, but Enter's ASCII code `0x0D` doesn't equal `ST_KEY_ENTER (0x0C)`). Fixed both to leave the character byte at 0.

### HeMoM executable

**`--continue` CLI flag**

Added to both HeMoM and ReMoMber. On HeMoM it's sugar for `--load SAVE9.GAM`; on ReMoMber it sets a flag that makes `MOM_main` skip the intro logos (adds a second condition to the existing JENNY-arg check) and makes `Screen_Control()` enter at `scr_Continue` instead of `scr_Main_Menu_Screen`.

**`--scenario` CLI flag on ReMoMber**

Previously only HeMoM supported scenarios. ReMoMber got the same flag plus a link against `Artificial_Human_Player.c`, with registration via `Platform_Register_Frame_Callback()`.

**`Platform_Register_Frame_Callback()` in Platform.h**

Added a generic per-frame callback API to `Platform.h` so the artificial human player can register once and have it called from any backend's `Platform_Event_Handler()`. Previously the hook was only in the headless backend.

**`--dump-save FILE.GAM` CLI flag**

Added to HeMoM. After `Screen_Control()` returns, HeMoM reads the specified .GAM file and writes a `.txt` dump next to it. This is how the test captures the final game state.

### Test infrastructure

**`tests/check_save_fields.c`**

New C tool. Takes a dump file and an assertions file. Loads the dump into an in-memory key→value table, then iterates the assertions file and reports any mismatches with exact field names, actual values, and expected values. Exits 0 on all-pass, 1 on any failure, 2 on usage/file errors.

**`tests/assert_continue_save.txt`**

The 13 assertions listed above.

**`tests/CMakeLists.txt`**

Added the `HeMoM_Continue_*` test chain with CTest fixture ordering: `Setup` creates a fresh SAVE9.GAM, `Run` depends on `Setup` and produces SAVE1.txt, `Assertions` depends on `Run` and runs the field checker.

---

## Summary of What Isn't Covered

This test locks down a specific, narrow slice of gameplay state. Many things remain untested.

**Not covered by this test:**

- **Multi-turn behavior.** The scenario ends after one Patrol action. `game.turn` is still 0. None of the AI turn processing, city production, spell research, unit movement, or end-of-turn event logic has been exercised.
- **World generation correctness.** That's already covered by `HeMoM_WorldGen_Golden` via byte-level comparison against a checked-in reference save.
- **Load-time recomputation.** The fields that change because of `Loaded_Game_Update()` (power_base, bldg_status, etc.) are *deliberately* not asserted here. A separate test — loading a known-good save and asserting its post-load derived state — would be needed to lock that down.
- **Combat.** No combat initiation, no battle-field actions, no combat resolution. Would need a scenario that moves a unit into an enemy's tile, a complete combat scripting vocabulary, and a much larger assertion surface.
- **Spellcasting.** No overland casting, no spellbook navigation, no research selection.
- **Diplomacy.** No AI wizard interactions.
- **City management.** No opening city screen, no production changes, no buildings purchased, no tax rate or research allocation changes.
- **Hero hiring.** No hero popup interaction.
- **Non-happy paths.** Cancelling dialogs, right-click help, error popups, out-of-memory, missing asset files — none are exercised.
- **Save file format migration.** The test uses a fixed seed and always produces the same save layout. It won't catch bugs where an old save file becomes incompatible with a newer engine.
- **Audio.** No sound effects, no music loading or playback — headless has no audio backend at all.
- **Replay determinism.** The scenario is artificial human player input, not `.RMR` replay. A separate test using `--record` + `--replay` would verify that the replay system itself is deterministic for this scenario.
- **Real-keyboard Backspace/Enter on SDL backends.** I fixed the Win32 backend's `mox_character` bug for these keys, but didn't check or fix the same pattern in `sdl2_KD.c` and `sdl3_KD.c`, which both still set `mox_character = '\b'` / `'\r'`. The text input popup will misbehave on SDL2/SDL3 for real keyboard input the same way it did before the artificial human player fix. This is a latent bug exposed by the investigation but not addressed by this test.

**What this test is good for:**

Catching regressions in the input pipeline from "synthetic action in a .hms file" through "engine state written to a .GAM file". It verifies that:
- Loading SAVE9.GAM and entering the Continue path produces a working game state
- The home-city naming popup accepts typed input and writes it to `city[0].name`
- The Patrol button action correctly updates both units in the starting stack
- The Save / Load screen's save path writes a new `.GAM` file with the modified state
- `Screen_Control()` cleanly exits via the Main Menu → Quit to DOS path from a continue'd game
- The artificial human player's click coordinate math works end-to-end on the headless backend (the very first test of this at all, which is what surfaced the `Platform_Get_Window_Width()` bug)

If any of that chain breaks, this test fails with a specific field assertion pointing at the exact piece of state that's wrong.
