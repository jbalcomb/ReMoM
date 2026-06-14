# HeMoM `--newgame` Path Alignment with ReMoM `Newgame_Control()`

Goal: make HeMoM's `--newgame` produce the same game state as a regular interactive ReMoM new game.

## ReMoM control flow

The state machine in [Newgame_Control()](../MoM/src/NewGame.c#L1413) walks up to 8 screens, then tail-calls:

```
Init_New_Game()  →  Initialize_Events()  →  Init_Runtime()  →  Save_SAVE_GAM(8)
```

Two screen sequences exist:

- **Prefab path:**  Screen 0 → 1 (preset clicked) → 2 → 3 → 6 → 7 → finalize
- **Custom path:**  Screen 0 → 1 (Custom clicked) → 2 → 3 → 4 → 5 → 6 → 7 → finalize

## Per-screen non-UI side effects (OK-path only)

| Screen | Function | Writes |
|---|---|---|
| 0 Options    | [Newgame_Screen_0](../MoM/src/NewGame.c#L1801-L1828)        | `magic_set` → MAGIC.SET file; `Randomize_Book_Heights()`; copies `magic_set.{LandSize, MagicPower, Opponents+1, Difficulty}` into `_landsize`, `_magic`, `_num_players`, `_difficulty` |
| 1 Wizard     | [Newgame_Screen_1](../MoM/src/NewGame.c#L2287-L2303)        | Preset path: `Human_Player_Wizard_Profile(itr)`. Custom path: nothing |
| 2 Portrait   | [Newgame_Screen_2](../MoM/src/NewGame.c#L2634-L2644)        | `_players[0].wizard_id = itr`; `_players[0].name = preset[itr].name` (overwrites Screen 1's name) |
| 3 Name       | [Newgame_Screen_3](../MoM/src/NewGame.c#L2722-L2733)        | `_players[0].name` (trimmed) |
| 4 Creation   | [Newgame_Screen_4](../MoM/src/NewGame.c#L3963-L3967)        | Zeroes `_players[0].spellranks[0..4]` at entry; click loop drives `_players[0].alchemy[]` (retorts) and `m_bookshelf_*` scroll vars that the draw step copies into `_players[0].spellranks[]` |
| 5 Spells     | [Newgame_Screen_5](../MoM/src/NewGame.c#L4832-L4839)        | Zeroes `_player_start_spells[0].realms[].spells[]`, pre-fills each realm from `_default_spells` per book count, then user click loop overrides individual spell slots |
| 6 Race       | [Newgame_Screen_6](../MoM/src/NewGame.c#L3254-L3268)        | `NEWG_Clicked_Race` (no `_players[]` write — that happens during `Generate_Home_Cities`) |
| 7 Banner     | [Newgame_Screen_7](../MoM/src/NewGame.c#L2935-L2954)        | `_players[0].banner_id` |

Then [Init_New_Game()](../MoM/src/MAPGEN.c#L261) builds the world; [Init_Computer_Players()](../MoM/src/INITGAME.c#L73) auto-fills `_player_start_spells[1..N]` from `_default_spells` for AI players only.

## HeMoM today

[Config_Apply_And_Create_New_Game()](../src/HeMoM.c#L371-L461):

1. Sets `_difficulty / _magic / _landsize / _num_players` from cfg  ✓ matches Screen 0's last 4 lines
2. `Human_Player_Wizard_Profile(cfg->wizard_id)`  ≈ matches Screen 1 preset path
3. Zeroes the wsa array  (clears whatever retort step 2 set)
4. Optionally overrides name, spellranks, retorts from cfg
5. Sets `NEWG_Clicked_Race` and `_players[0].banner_id`  ≈ matches Screens 6 + 7
6. Sets RNG seed if configured
7. `Init_New_Game()` → `Initialize_Events()` → `Init_Runtime()` → `Save_SAVE_GAM(8)`  ✓

## Determinism constraint — Random() audit

**Every `Random()` call advances the global LFSR seed**, so even calls that write to cosmetic-only tables shift the RNG cursor for every downstream consumer (world-gen, AI personalities, lairs, neutral cities, etc.). Skipping a Random()-consumer on the menu path produces a *different game world* even with the same `--seed` value.

### Audit: Random() consumers reachable from `Newgame_Control()` between `Set_Random_Seed` and `Init_New_Game()` entry

Grep of `Random(` across the menu-path call graph turned up **exactly one** consumer:

| Call site | Location | Draws |
|---|---|---|
| `Randomize_Book_Heights()` from Screen 0 OK path | [NewGame.c:2100-2105](../MoM/src/NewGame.c#L2100-L2105) | 66 (= 6 × 11) |

What was checked:
- All `Newgame_Screen_*` OK-path return tails — no other `Random()` calls (`grep "Random(" NewGame.c`)
- All `Newgame_Screen_*_Draw*` callbacks — no `Random()` calls
- Adjacent menu-path modules (`Settings.c`, `RACETYPE.c`, `Spellbook.c`, `Fields.c`, `LBX_Load`) — no `Random()` calls
- `Human_Player_Wizard_Profile` itself ([NewGame.c:5799-5925](../MoM/src/NewGame.c#L5799-L5925)) — no `Random()` calls

`Randomize_Book_Heights` also computes a `niu` value from `magic_set.*` ([NewGame.c:2096](../MoM/src/NewGame.c#L2096)) but **`niu` is never read** ("Not In Use" — see the project glossary), so `magic_set` content does *not* affect the draws.

### Seeding sequence

[Init.c:87-90](../MoM/src/Init.c#L87-L90) calls `Set_Random_Seed(_cmd_line_seed)` early in `Init_Drivers`, before any menu code runs. So the RNG baseline for *both* ReMoM and HeMoM is set once at process startup. From that point to `Init_New_Game()` entry:

- **ReMoM (via menus):**  `Set_Random_Seed(S)` → 66 draws in `Randomize_Book_Heights` → `Init_New_Game`
- **HeMoM today:**  `Set_Random_Seed(S)` → 0 draws → `Init_New_Game`

→ **HeMoM enters world-gen with the RNG cursor 66 ticks ahead of ReMoM.** Every world-gen `Random()` consumes a different value, so the worlds diverge.

HeMoM's own `Set_Random_Seed(cfg->seed)` at [HeMoM.c:426](../src/HeMoM.c#L426) restores the seed *after* the cursor has already drifted from any prior consumer — useful as a re-seed point, but it must land *before* the 66-draw burn, not after.

## Gaps

### RNG-cursor-affecting (must fix for `--seed` reproducibility)

1. **`Randomize_Book_Heights()` is never called.** HeMoM enters `Init_New_Game()` 66 RNG ticks ahead of ReMoM. *This is the dominant determinism gap.*

### Game-state-affecting

2. **`_player_start_spells[0]` goes stale when books are overridden.** `Human_Player_Wizard_Profile` populates the human player's starting spells from the *prefab's* book counts. If `has_books` later rewrites `_players[0].spellranks[]`, the start-spells still reflect the prefab — wrong realm, wrong rarity counts. [Newgame_Screen_5](../MoM/src/NewGame.c#L4755) is the function that does this re-fill correctly; HeMoM bypasses it entirely.

3. **Retort zero is unconditional.** Step 3 always clobbers the preset's single retort. So if a user picks a prefab wizard in the config (`has_retorts == 0`), they lose Merlin's Sage Master, Tauron's Chaos Mastery, etc. Should only zero when `has_retorts != 0`.

4. **`_players[0].wizard_id` cannot be split from "preset to clone".** ReMoM's Screen 1 selects a *preset* (drives books/retorts/spells); Screen 2 then selects a *portrait* which also overwrites `wizard_id` and `name`. HeMoM conflates them. Probably fine for current usage but worth a config note (`wizard_id` = "preset to clone *and* portrait to use").

5. **Pre-existing off-by-one in `Human_Player_Wizard_Profile` itself.** [NewGame.c:5841](../MoM/src/NewGame.c#L5841) uses `m_select_count_common[spellranks] - 2` (where `spellranks` is the raw book count), while [Newgame_Screen_5:4907-4909](../MoM/src/NewGame.c#L4907-L4909) uses `m_select_count_common[spellranks - 2]`. Different indexing schemes — looks like a pre-existing bug where the prefab path under-fills the starting spell pool. Out of scope for HeMoM alignment but HeMoM inherits whatever this produces.

### Safe to skip

6. **MAGIC.SET file write.** That is user-persistence-of-options. Not consumed by game logic. Leave it to `Settings.c`.

7. **`magic_set` mirror itself.** Only `Randomize_Book_Heights` reads any of the four cfg-mirrored fields, and only via the unused `niu` calc. Other `magic_set` members are audio/video toggles. No effect on game state or RNG.

## Plan

### P1 — Match the RNG call stream

The non-negotiable bit. HeMoM must perform the same Random()-consuming operations in the same order as ReMoM's menu OK-path, between `Set_Random_Seed` and `Init_New_Game()` entry.

- **Step 1.** In `Config_Apply_And_Create_New_Game()`, insert `Randomize_Book_Heights()` *after* `Set_Random_Seed(cfg->seed)` and *before* the `Init_New_Game()` call. Order relative to `Human_Player_Wizard_Profile` and the override block does not matter for the RNG (those don't draw) but for clarity place it adjacent to the seed-set so the determinism intent is obvious.

- **Step 2.** Verify by tracing (see Verification protocol below) that no other Random() consumer was missed. If the trace finds a divergence, fix the missing call and re-run.

### P2 — Align player state with ReMoM custom flow

- **Step 3.** Only clobber the wsa array when `cfg->has_retorts != 0`. Today's unconditional zeroing breaks prefab-only configs.

- **Step 4.** After applying any book overrides, re-run the per-realm start-spell fill logic for the human player. Approach:
  - **Preferred:** Extract the body of `Human_Player_Wizard_Profile`'s spell-fill loop into a helper `Human_Player_Default_Start_Spells(void)` that reads `_players[0].spellranks[]` and writes `_player_start_spells[0]`. Call it from both `Human_Player_Wizard_Profile` and from HeMoM after applying book overrides.
  - **Rejected:** Re-calling `Human_Player_Wizard_Profile(cfg->wizard_id)` after overrides won't work — that function reads from the preset table, not from `_players[0].spellranks[]`.
  - **Note:** Neither path calls Random(), so this is RNG-neutral.

- **Step 5.** Document in `s_HeMoM_Config` that `wizard_id` is both the preset-to-clone and the portrait id (matches what Screens 1 + 2 do together). No code change needed.

### P3 — Verification protocol (how we prove it works)

The codebase already has the proof mechanism wired up: [Random_at()](../MoX/src/random.c#L318-L341) emits `[RNG-CALL] call=N n=K before=0xX after=0xY result=R at=FILE:LINE` to stderr whenever `_cmd_line_seed != 0`. Both ReMoM (`--seed S`) and HeMoM (`--newgame ... --seed S`) honour it, so we can compare call-streams byte-for-byte.

Two layers of evidence — pass both or it isn't working:

#### Layer A — RNG call-stream parity (necessary)

The strict test. If this passes, by construction every downstream `Random()` consumer sees the same value, so the worlds are identical.

1. Pick a fixed seed `S`. Set the same wizard/race/banner/difficulty/landsize/magic/opponents in `assets/ReMoM.ini` *and* in a recorded menu-click sequence.
2. Run `ReMoMber --seed S` (or equivalent), click through the new-game menus making the same selections, hit OK on Screen 0 → ... → Screen 7. Capture stderr to `remom_seed${S}_stderr.log`.
3. Run `HeMoM --newgame assets/ReMoM.ini --seed S`. Capture stderr to `hemom_seed${S}_stderr.log`.
4. Filter both to `[RNG-CALL]` lines only (e.g. `*_rng.log`).
5. `diff remom_seed${S}_rng.log hemom_seed${S}_rng.log` — expect zero difference. The first diverging line names the source file:line of the offending call.

The expected first divergence today is at call #1 of `Init_New_Game`: HeMoM's `seed_before` will differ from ReMoM's by 66 ticks (the missing `Randomize_Book_Heights` draws). After Step 1, that line should match.

#### Layer B — SAVE9.GAM byte equality (sufficient)

The end-to-end test. If Layer A passes, this should pass automatically, but it catches non-RNG state divergence (the gaps in §Game-state-affecting above).

6. From the same two runs as above, compare `SAVE9.GAM` byte-for-byte: `cmp og/SAVE9.GAM hemom/SAVE9.GAM`. Expect zero difference.
7. If `cmp` reports a diff, use `Game_Save_Dump` to produce `SAVE9.txt` for both and `diff` those. The structured diff names the field that differs, which usually points directly at one of gaps 2–5.

#### Regression cases to lock in

- **Case 1 — prefab Merlin, no overrides.** Tests Step 3. SAVE9.txt for human player should show `wsa_Sage_Master == 1`, the 5 spellranks from `_wizard_presets_table[0]`, and the prefab's default start-spells.
- **Case 2 — prefab Merlin + override `books_chaos=8`.** Tests Step 4. `_players[0].spellranks` should reflect the overridden chaos book count, AND `_player_start_spells[0].realms[sbr_Chaos].spells[]` should be drawn from `_default_spells[sbr_Chaos]`, not Merlin's prefab nature/sorcery defaults.
- **Case 3 — fully custom wizard.** Same config as a real "Custom" menu click sequence. Layer A trace should match the recorded custom-path trace.
- **Case 4 — same config, two seeds.** Run twice with `--seed 1` and `--seed 2`. Different worlds (sanity check that the seed is actually being honored), but identical wizard profile in SAVE9.txt.
- **Case 5 — same config, same seed, two runs.** Bit-for-bit identical SAVE9.GAM (deflakes any hidden non-determinism, e.g. uninitialized stack).

#### Where to wire it

The HeMoM test-runner ([Devel-HeMoM-Testing.md](Devel-HeMoM-Testing.md)) is the right place for Cases 1–5. Use the existing recording/replay infrastructure (`.RMR` files) to capture the OG menu-click sequence once; then re-replay it whenever the OG path needs to be re-baselined.

If a recorded menu sequence isn't available yet, a *self-consistency* check is still meaningful: run HeMoM twice with the same seed and config — Case 5 — and require bit-equal SAVE9.GAM. That doesn't prove parity with ReMoM but catches latent non-determinism cheaply.

## Trouble flags

- **The "user picks specific spells" UI is unreachable from a config file.** Screen 5 lets the user pick *which* common/uncommon spells go into start. `ReMoM.ini` has no equivalent today. If exact-spell control matters for tests, you would need new `start_spells_nature=...` keys plus a fill helper. Otherwise HeMoM gets the same defaults as a player who clicked OK without touching the lists. *Critically: Screen 5's user-click loop doesn't call `Random()`, but its default-spell pre-fill at [NewGame.c:4909-4920](../MoM/src/NewGame.c#L4909-L4920) doesn't either — it just copies from `_default_spells`. So this gap is game-state-only, not RNG-cursor.*

- **The `Human_Player_Wizard_Profile` indexing oddity (gap 5 above)** means even "perfectly aligned" HeMoM and ReMoM will both be wrong in the same way. Fine for parity testing; file separately as a known bug.

- **Screen 2 overwrites Screen 1's name; Screen 3 overwrites both.** The current HeMoM order (HumanProfile → maybe-override-name) matches the *end* state ReMoM reaches, so this is correct — just non-obvious. None of these writes touch `Random()`.

- **`screen_changed` global** in [Newgame_Screen_0](../MoM/src/NewGame.c#L1886) is set elsewhere; HeMoM doesn't touch it. The state-machine driver wouldn't fire anyway because HeMoM bypasses the loop.

- **Re-baselining when ReMoM's menu code changes.** Any future edit to the new-game screens that adds or removes a `Random()` call (or changes the order) silently invalidates the recorded OG trace. The verification harness should re-capture the OG trace whenever NewGame.c is touched, and CI should compare both `[RNG-CALL]` count and the SAVE9.GAM bytes — not just one.

- **The audit is a grep, not a proof.** The "only `Randomize_Book_Heights`" claim came from grepping `Random(` across the menu-path call graph. Layer A above turns that into a runtime proof — if a Random() consumer was missed (e.g. inside an LBX_Load that lazily seeds something, or a hidden init hook), the trace diff will surface it at the first divergent call.

---

## Appendix A — Program flow, function by function

Reference walk-through of the ReMoM new-game path from `main()` to written `SAVE9.GAM`. Each row notes the source location, what state the function writes, and the **`Random()` draw count** (zero unless flagged). Use this when investigating a Layer-A trace divergence.

### Stage 1 — Process startup and engine init

| # | Function | Random | Notes |
|---|---|---|---|
| 1.1 | [main()](../src/ReMoM.c#L211) | 0 | Program entry. Calls `STU_Log_Startup()`, logs argc/argv, then proceeds to CLI parsing. |
| 1.2 | [Check_Command_Line_Parameters_()](../src/ReMoM.c#L280) | 0 | Shared CLI parser (MoO2 pattern). Sets `_cmd_line_seed` from `--seed S` if present. *No `Random()` draws yet — seed isn't applied here.* |
| 1.3 | [Startup_Platform()](../src/ReMoM.c#L305) | 0 | SDL init (or `SDL_VIDEODRIVER=offscreen` if `--headless`). |
| 1.4 | `Platform_Replay_Register_Random_Seed_Callbacks(Get_Random_Seed, Set_Random_Seed)` | 0 | Wires the record/replay machinery to the seed accessors so `.RMR` capture can persist RNG state. |
| 1.5 | `--record` / `--replay` / `--scenario` / `--demo` flag loop | 0 | Starts a recording or attaches a replay/scenario if requested. Does not touch RNG. |
| 1.6 | [MOM_main()](../src/ReMoM.c#L449) | (delegates) | Main MoM-side entry point. Composition: engine init → optional logos → main-menu music load → `Screen_Control()` loop. |
| 1.7 | [ReMoM_Init_Engine()](../src/ReMoM_Init.c#L47) | seed-set | Core engine init (shared with HeMoM). Loads CONFIG.MOM/MAGIC.SET, allocates data space, then at [ReMoM_Init.c:288](../src/ReMoM_Init.c#L288) calls `Init_Drivers(...)`. |
| 1.8 | [Init_Drivers()](../MoM/src/Init.c#L87-L90) | **seed-set** | If `_cmd_line_seed != 0`: `Set_Random_Seed((uint32_t)_cmd_line_seed)`; else `Randomize()` (time-based). **This is the canonical RNG baseline for both ReMoM and HeMoM.** |
| 1.9 | `Draw_Logos()` / `Init_Credits()` / `LBX_Load(music_lbx__main)` / `Play_Sound(...)` / `Load_Palette(...)` | 0 | Intro chrome + main menu music. None touches `Random()`. (Asset loads and audio init are RNG-clean.) |
| 1.10 | `Screen_Control()` → `Main_Menu_Screen` → user clicks "New Game" → `Newgame_Control()` | 0 | Drives the menu state machine; no `Random()` draws until the user reaches Screen 0's OK button. |

After Stage 1 the RNG cursor sits at the seed Init_Drivers established. Any subsequent draw mismatch between ReMoM and HeMoM is a real divergence.

### Stage 2 — Menu controller

| # | Function | Random | Notes |
|---|---|---|---|
| 2.0 | [Newgame_Control()](../MoM/src/NewGame.c#L1413) | 0 (driver only) | Initializes defaults (`_landsize=1, _magic=1, _num_players=1, _difficulty=1`), then drives the screen state machine. HeMoM bypasses this loop entirely. |

#### Stage 2a — Screen 0 (Options)

| # | Function | Random | Writes |
|---|---|---|---|
| 2a.1 | [Newgame_Screen_0()](../MoM/src/NewGame.c#L1643) entry | 0 | Loads `MAGIC.SET` from disk (or calls `Set_Default_Game_Settings()`). Mutates `magic_set`. |
| 2a.2 | …click loop | 0 | Mutates `magic_set.Difficulty / Opponents / LandSize / MagicPower` until OK. |
| 2a.3 | …on OK: `fwrite(magic_set)` | 0 | Persists settings to `MAGIC.SET`. *Not needed by HeMoM.* |
| 2a.4 | **[Randomize_Book_Heights()](../MoM/src/NewGame.c#L2088)** | **66** | 11 iterations × 6 `Random(3)` draws. Writes `TBL_Realm{0..4}_Books[]` and `TBL_Bookshelf_Books[]` — used only by `NEWG_DrawDefShelf__WIP` *but* burns RNG ticks that downstream world-gen depends on. **This is the dominant determinism gap.** |
| 2a.5 | …on OK: copy to runtime | 0 | `_landsize`/`_magic`/`_num_players`/`_difficulty` ← `magic_set.*`. HeMoM does this directly from cfg at step 1 of `Config_Apply_And_Create_New_Game`. |

#### Stage 2b — Screen 1 (Select Wizard)

| # | Function | Random | Writes |
|---|---|---|---|
| 2b.1 | [Newgame_Screen_1__WIP()](../MoM/src/NewGame.c#L2155) preset OK | 0 | Calls `Human_Player_Wizard_Profile(itr)`. |
| 2b.2 | …Custom OK | 0 | Returns `2`; no state writes. |
| 2b.3 | [Human_Player_Wizard_Profile()](../MoM/src/NewGame.c#L5799) | 0 | Writes `_players[0].wizard_id`, `_players[0].spellranks[0..4]`, `_players[0].alchemy[]` (one retort from preset), `_player_start_spells[0]` (per-realm default fill, with the off-by-one bug — gap 5), `_players[0].name`. |

#### Stage 2c — Screen 2 (Portrait)

| # | Function | Random | Writes |
|---|---|---|---|
| 2c.1 | [Newgame_Screen_2__WIP()](../MoM/src/NewGame.c#L2557) OK | 0 | `_players[0].wizard_id = itr`; `_players[0].name = preset[itr].name`. **Overwrites Screen 1's name/wizard_id.** |

#### Stage 2d — Screen 3 (Name)

| # | Function | Random | Writes |
|---|---|---|---|
| 2d.1 | [Newgame_Screen_3__WIP()](../MoM/src/NewGame.c#L2697) | 0 | `Setup_Input_Box_Popup` for text entry → `_players[0].name` (trimmed; falls back to prior value if empty). |

#### Stage 2e — Screen 4 (Custom Books + Retorts) — *custom path only*

| # | Function | Random | Writes |
|---|---|---|---|
| 2e.1 | [Newgame_Screen_4__WIP()](../MoM/src/NewGame.c#L3857) entry | 0 | Zeros `_players[0].spellranks[0..4]` and `_players[0].alchemy[]`. |
| 2e.2 | …click loop | 0 | Mutates `m_bookshelf_*` scroll vars (copied into `spellranks` by the draw step) and toggles `_players[0].alchemy[]`. |
| 2e.3 | [Newgame_Screen_4_Draw__WIP()](../MoM/src/NewGame.c#L4235) | 0 | Each frame: copies scroll vars to `_players[0].spellranks[]`, re-validates retort prereqs, updates `NEWG_ProfileComplete`. |

#### Stage 2f — Screen 5 (Custom Start Spells) — *custom path only*

| # | Function | Random | Writes |
|---|---|---|---|
| 2f.1 | [Newgame_Screen_5()](../MoM/src/NewGame.c#L4755) entry | 0 | Zeros `_player_start_spells[0].realms[].spells[]` for all 5 realms. |
| 2f.2 | …per-realm default fill | 0 | For each realm with `>1` book: pre-fills from `_default_spells[realm]` per book count. *Uses the correct `m_select_count_common[spellrank_cnt - 1]` indexing, unlike `Human_Player_Wizard_Profile`.* |
| 2f.3 | …click loop | 0 | User picks override individual `p_start_spells[]` slots. |

#### Stage 2g — Screen 6 (Race)

| # | Function | Random | Writes |
|---|---|---|---|
| 2g.1 | [Newgame_Screen_6__WIP()](../MoM/src/NewGame.c#L3166) OK | 0 | `NEWG_Clicked_Race` (the actual `_CITIES[].race` write happens later, inside `Generate_Home_Cities`). |

#### Stage 2h — Screen 7 (Banner)

| # | Function | Random | Writes |
|---|---|---|---|
| 2h.1 | [Newgame_Screen_7__WIP()](../MoM/src/NewGame.c#L2841) OK | 0 | `_players[0].banner_id` ∈ {Green, Blue, Red, Purple, Yellow}. |

**Stage 2 total Random() draws: 66 (prefab) or 66 (custom) — all in `Randomize_Book_Heights`.**

### Stage 3 — World generation: `Init_New_Game()`

[Init_New_Game()](../MoM/src/MAPGEN.c#L261) is the longest stretch. Each sub-call below is itself a heavy `Random()` consumer; counts are descriptive rather than exact because many depend on `_landsize`/`_magic`/`_num_players`.

| # | Function | Random | Notes |
|---|---|---|---|
| 3.1 | `LBX_Load_Data(builddat_lbx_file, ...)` | 0 | Loads `bldg_data_table`. |
| 3.2 | [Init_Computer_Players()](../MoM/src/INITGAME.c#L73) | **many** | Calls `Init_Computer_Players_Wizard_Profile()` (assigns prefab wizards to AI slots, sets `_player_start_spells[1..N]`, Random draws in retort/personality picks) and `Init_Computer_Players_Spell_Library()` (Random draws populating AI starting spell libraries — see [INITGAME.c:1636-1876](../MoM/src/INITGAME.c#L1636-L1876)). |
| 3.3 | `Init_Landmasses(wp)` × 2 | 0 | Zeroes landmass tables per plane. |
| 3.4 | `Generate_Landmasses(wp)` × 2 | **many** | Heightmap random walker per plane. |
| 3.5 | `Translate_Heightmap_To_Base_Terrain_Types(wp)` × 2 | 0 (table-driven) | Heightmap → terrain type lookup. |
| 3.6 | `Generate_Climate_Terrain_Types(wp)` × 2 | **many** | Climate band placement (deserts, tundra, etc.). |
| 3.7 | `Generate_Nodes()` | **many** | Random node placement per plane. |
| 3.8 | `Rebalance_Node_Types(wp)` × 2 | **many** | Reassigns node realms to hit per-plane targets. |
| 3.9 | `Generate_Towers()` | **many** | 6 Towers of Wizardry placed by Random walk. |
| 3.10 | `Extend_Islands(wp)` × 2 | **many** | Small-island grow pass. |
| 3.11 | `Generate_Lairs()` | **many** | Lair placement + difficulty seeding. |
| 3.12 | `Generate_Home_Cities()` | **many** | Places each player's starting fortress; consumes `NEWG_Clicked_Race` for human, prefabs for AI. *Writes `_CITIES[].race`.* |
| 3.13 | `Generate_Neutral_Cities(wp)` × 2 | **many** | Neutral city placement + race assignment. |
| 3.14 | `Generate_Terrain_Specials(wp)` × 2 | **many** | Mineral specials per terrain type (Random rolls per square). Also **zeros `_map_square_flags[]` per plane** at [MAPGEN.c:5610-5620](../MoM/src/MAPGEN.c#L5610-L5620). |
| 3.15 | `Generate_Roads(wp)` × 2 | 0 | Pathfinding-based road placement between neutral cities. *Re-zeros the flag plane it just landed on.* |
| 3.16 | `Simex_Autotiling()` | 0 | Tile-type cleanup. |
| 3.17 | `River_Path(wp)` (looped) | **many** | Random walker, up to 2000 tries per river. |
| 3.18 | `River_Terrain(wp)` × 2 | 0 (terrain rewrite) | Converts walker output to river tiles. |
| 3.19 | `Desert_Autotile()` | 0 | Sand/desert tile cleanup. |
| 3.20 | `Shuffle_Terrains()` | **many** | Per-square variant index randomization. |
| 3.21 | [Movement_Mode_Cost_Maps(wp)](../MoM/src/MAPGEN.c#L6073) × 2 | 0 | Builds `movement_mode_cost_maps[]` from `terrain_stats_table` + `MSF_ROAD` overlay. |
| 3.22 | `CRP_NEWG_CreatePathGrids__WIP(wp)` × 2 | 0 | Builds AI path grids from the cost maps. |
| 3.23 | `Init_Square_Explored()` | 0 | Sets explored bits around each fortress (writes the low 3 bits of `_map_square_flags`). |
| 3.24 | `Animate_Oceans()` | 0 | Ocean tile animation frames. |
| 3.25 | `Set_Upper_Lair_Guardian_Count()` | 0 | Lair difficulty rollup. |

### Stage 4 — Events table

| # | Function | Random | Writes |
|---|---|---|---|
| 4.1 | [Initialize_Events()](../MoM/src/INITGAME.c#L2032) | 0 | Sets `events_table->last_event_turn = 50`, zeros all per-event status flags. |

### Stage 5 — Runtime finalization: `Init_Runtime()`

| # | Function | Random | Writes |
|---|---|---|---|
| 5.1 | [Init_Runtime()](../MoM/src/INITGAME.c#L141) | (delegates) | Calls the sub-functions below, then sets `_players[].capital_race`, human `gold_reserve = (5 - _difficulty) * 25`, AI `gold_reserve = 150`. |
| 5.2 | [Initialize_Items()](../MoM/src/INITGAME.c#L272) | 0 | Zeros `_ITEMS[0..137].cost` and `TBL_Premade_Items[0..249]`. |
| 5.3 | [Init_Heroes()](../MoM/src/INITGAME.c#L2058) | **many** | For each of 5 players × `NUM_HERO_TYPES` heroes: rolls hero abilities via `Random(14)` / `Random(8)` / `Random(NUM_WIZARD_SPECIAL_ABILITIES)` loops ([INITGAME.c:2111, 1017, 1168](../MoM/src/INITGAME.c#L2111)). |
| 5.4 | `Init_Players()` | varies | Per-player runtime field reset (gold, mana, fame, etc.). |
| 5.5 | `Init_CP_Strategy()` | varies | AI strategy timers. |
| 5.6 | [Init_Magic_Personalities_Objectives()](../MoM/src/INITGAME.c#L369) | **many** | Weights-based `Random(Max_Value)` ([INITGAME.c:609, 623](../MoM/src/INITGAME.c#L609)) to pick each AI's personality + objective. Also adds guaranteed researchable spells to AI libraries (Hard/Impossible). |
| 5.7 | [Init_Summoning_Circle_And_Spell_Of_Mastery()](../MoM/src/INITGAME.c#L296) | 0 | Copies fortress `wp/wx/wy` into per-player `summon_*`; sets `som_research_cost` (− 3000 if all 5 spellranks == 11). |
| 5.8 | `Initialize_Messages()` | 0 | Zeros per-player message counters. |
| 5.9 | [Init_Diplomatic_Relations()](../MoM/src/INITGAME.c#L178) | 0 | Pairwise `Dipl.Visible_Rel` / `Default_Rel` from spellbook diffs (Life/Death/Chaos/Sorcery). |
| 5.10 | AI start-circle reeval | per-player `Random(40)` | `AI_SCircle_Reevals[itr] = 80 + Random(40)` for each AI player ([INITGAME.c:161](../MoM/src/INITGAME.c#L161)). |

### Stage 6 — Save

| # | Function | Random | Notes |
|---|---|---|---|
| 6.1 | `Save_SAVE_GAM(8)` | 0 | Writes the continue-save slot. Snapshots `random_seed` itself — so any RNG drift up to this point is frozen into the save. |
| 6.2 | `Game_Save_Dump("SAVE9.GAM", "SAVE9.txt")` (HeMoM only) | 0 | Structured text dump for diffing. |

### HeMoM's current path, mapped onto the stages

HeMoM's [main()](../src/HeMoM.c#L514) replaces Stage 1 with its own arg-parse + early exit (no Main Menu, no `Screen_Control` loop), reaches `Init_Drivers` via `ReMoM_Init_Engine()` at [HeMoM.c:690](../src/HeMoM.c#L690), then calls `Config_Apply_And_Create_New_Game()` at [HeMoM.c:703](../src/HeMoM.c#L703) which performs Stage-2-equivalent work inline before jumping straight to Stages 3–6.

| Stage | ReMoM | HeMoM today | Required after P1 |
|---|---|---|---|
| 1 | `main → ... → Init_Drivers` seed-set | `main → ... → Init_Drivers` seed-set (`ReMoM_Init_Engine` shared) | unchanged |
| 2a | `Newgame_Screen_0` + `Randomize_Book_Heights` (66 draws) | (nothing) | **add `Randomize_Book_Heights()`** |
| 2b | `Newgame_Screen_1` → `Human_Player_Wizard_Profile` | `Human_Player_Wizard_Profile(cfg->wizard_id)` | unchanged |
| 2c | `Newgame_Screen_2` overwrites wizard_id/name | (HeMoM uses cfg's wizard_id throughout) | unchanged |
| 2d | `Newgame_Screen_3` overrides name | `if (cfg->has_name)` override | unchanged |
| 2e | `Newgame_Screen_4` (custom only) zero+set wsa/spellranks | unconditional wsa zero, then cfg overrides | **gate wsa zero on `has_retorts`** |
| 2f | `Newgame_Screen_5` (custom only) re-fill start spells | *missing* | **add `Human_Player_Default_Start_Spells()` call** |
| 2g | `Newgame_Screen_6` sets `NEWG_Clicked_Race` | `NEWG_Clicked_Race = cfg->race` | unchanged |
| 2h | `Newgame_Screen_7` sets banner_id | `_players[0].banner_id = cfg->banner` | unchanged |
| 3–6 | `Init_New_Game` → `Initialize_Events` → `Init_Runtime` → `Save_SAVE_GAM(8)` | same | unchanged |
