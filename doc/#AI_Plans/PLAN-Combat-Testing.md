# PLAN — Deterministic End-to-End Combat Testing

**Status:** Draft
**Owner:** TBD
**Date:** 2026-07-09
**Companion:** `PRD-Combat-Testing.md` (requirements), this file (survey + plan)

---

## Goal

Test combat all the way through — both tactical (battle screen, turn-by-turn) and strategic (auto-resolve) — with known inputs and known outputs, deterministically via a pinned random seed. Fit the new tests into the existing CTest/GTest, HeMoM, and record-and-replay infrastructure rather than building a parallel system.

## What already exists

### Test infrastructure

- **HeMoM** (`src/HeMoM.c`) is the headless integration harness: `--newgame [ReMoM.ini]`, `--load SAVEn.GAM`, `--continue`, `--scenario FILE.hms`, `--record`/`--replay FILE.RMR`, `--dump-save`, `--seed`. It links `momlib MOX STU Platform_Headless` and replicates the ReMoM menu path's RNG draws (e.g. the 66-draw `Randomize_Book_Heights()`) so seed streams align between headless and windowed runs.
- **Scenario scripts** (`assets/*.hms`, parsed by `src/Artificial_Human_Player.c`) inject one input action per frame (clicks, keys, `next_turn`, `wait`) through the same path the replay system uses.
- **CTest fixtures** (`tests/CMakeLists.txt`) chain Setup → Run → Assertions so validation only runs after a successful setup (see the `HeMoM_Continue_*` and `HeMoM_AIBehavior_*` groups). Run with `ctest -R "HeMoM_"` — always the whole HeMoM suite, never a single scoped test.
- **Checkers:** `tests/check_save_fields.c` compares a `Game_Save_Dump` text dump against an `assert_*.txt` file of exact `key = value` expectations, with `# NONDET`-commented opt-outs for fields that are not yet deterministic. `tests/check_ai_metrics.c` evaluates `LIVENESS`/`BOUNDS` range rules against CSV metrics.
- **Determinism:** `Random(n)` is a pure function of the global `random_seed` (`MoX/src/random.c`). `--seed N` is parsed centrally by `Check_Command_Line_Parameters_` (`MoX/src/MOX2.c`) into `_cmd_line_seed`; INI `seed=` goes through `Set_Random_Seed()`. Byte-level golden saves are invalid (uninitialized padding regions); curated field assertions are the working pattern.
- **GTest unit targets:** `MOM_tests` (`MoM/tests/`) links `momlib + MOX + STU + Platform_Headless` and runs headless; the worldgen tests (`test_Create_Lair.cpp`, `test_Init_New_Game.cpp`, …) are the pattern for seeded module tests.
- **Record & replay** (`platform/replay/Replay.c`, `.RMR` files) exists but has no CTest wiring; it is frame/coordinate-based and is the wrong tool for combat logic (right tool for input-path/pixel regressions).

### Combat code

- **The dispatcher is a clean seam.** `Combat__WIP()` (Combat.c:3915) does all setup, then at Combat.c:4087-4113 picks the resolver: `Combat_Screen__WIP()` (tactical, Combat.c:1505) when a human participates and `magic_set.strategic_combat_only == ST_FALSE`, otherwise `Strategic_Combat__WIP()` (auto-resolve, Combat.c:10326). Post-combat writeback (unit move-in, city ownership/razing, item pool, diplomacy) is shared in `Combat__WIP` after either resolver.
- **Strategic combat is nearly headless already** — a couple of draw calls at entry, otherwise pure computation over `battle_units`/`battlefield`.
- **Tactical AI-vs-AI needs no input bridge.** With `_auto_combat_flag == ST_TRUE` the tactical loop (branch at Combat.c:1697) drives both sides through `Auto_Cast_Spell_And_Do_Combat_Turn()` (Combat.c:4508) → `Auto_Do_Combat_Turn()` (CMBTAI.c:776) with no `Get_Input()` in the decision path. Termination is guaranteed: `Check_For_Winner()` awards the defender when `_combat_turn > 50` (Combat.c:9178).
- **Human decisions are injectable.** The tactical loop consumes input via registered fields (`Add_Button_Field`/`Add_Grid_Field`/`Add_Hot_Key`, Combat.c:1784-1812) read by one blocking `Get_Input()` — `.hms` scripted clicks can drive specific unit actions.
- **Determinism holds** — combat's only entropy is `Random()` (~88 call sites in Combat.c). Caveat: cosmetic draws (e.g. music selection `Random(2)` at Combat.c:1595,1600) advance the shared seed, so outcomes are seed-*stream*-sensitive; any RNG-consuming code change upstream of an assertion shifts it. Same tradeoff the worldgen tests already accept.
- **Existing scaffolding:** `Combat_Screen_TST_001..004` (`MoM/src/CMBTTST.c`) are self-contained tactical harnesses, wired (commented out) into `Combat__WIP` at Combat.c:4101-4104. They still open the screen and block on `Get_Input()`. See `doc/Combat/MoX-Combat-Test.md`.
- **Outputs:** `End_Of_Combat__WIP()` (Combat.c:21256) writes unit death/status/XP, city losses (`CMB_Population_Lost`, `CMB_Buildings_Lost`), gold (`CMB_Gold_Reward`), lair guard updates; `Combat__WIP` then applies move-in, ownership change, relations.

## Decisions

1. **No new executable target.** A separate binary would duplicate HeMoM's newgame RNG-parity plumbing and add another entry to both build systems. Extend HeMoM with a `--combat` mode. Unit-level combat tests go into the existing `MOM_tests` target.
2. **Setup is test data, not new machinery.** HeMoM already gets from launch to a live game state deterministically. The missing piece is crafted saves (or scripted positioning) that place an attacker one move from a defender.
3. **Input bridge only for the human-decision tier.** AI-vs-AI needs none; `.hms` covers scripted human tactics.
4. **Output validation reuses the dump + assert pattern.** New piece: a `Combat_Dump` (sibling of `Game_Save_Dump`, same `key = value` text format) capturing battle-internal results — `_combat_winner`, per-`battle_units[]` final status/HP/XP, gold, buildings lost, city damage, RNG call count. Checked by `check_save_fields` against `assert_combat_*.txt`; `LIVENESS`/`BOUNDS`-style rules for legality properties.
5. **These are characterization tests.** Assertions pin *current* behavior including OGBUGs. A failing combat test means "behavior changed," not "behavior is wrong."

## Three tiers

### Tier 1 — GTest kernel tests (`MoM/tests/`)

Synthesize `s_BATTLE_UNIT` fixtures directly, call `Set_Random_Seed()`, test the pure math: `CMB_AttackRoll__SEGRAX`/`CMB_DefenseRoll__SEGRAX`, `BU_ApplyDamage`, `Combat_Resistance_Check`, `Load_Battle_Unit` (overland → battle conversion), `Deploy_Battle_Units` (placement), `Check_For_Winner`. This is where output *accuracy* gets precise, because every input is controlled. Early obstacle to resolve: `battle_units` is allocated via the `Allocate_*` layer inside combat's load path — fixtures either allocate the same way (the CMBTTST.c pattern) or the kernels get pointed at a static test array.

### Tier 2 — HeMoM `--combat` mode (the workhorse)

New mode: load a crafted save, call `Set_Random_Seed()` *immediately before* combat entry (so overland history cannot shift the rolls), set `_combat_wx/wy/wp` and the troop list, call `Combat__WIP()` directly.

- **Strategic:** both sides AI → `Strategic_Combat__WIP` runs as-is, headless today.
- **Tactical AI-vs-AI:** force the tactical path with `_auto_combat_flag = ST_TRUE`. Unproven piece: the tactical alloc/draw path (`CMB_BaseAllocs__WIP`, `CMB_LoadResources__WIP`, `Cache_Graphics_Combat`, `PageFlip_FX`) under `Platform_Headless` — this is the tracer bullet.

After `End_Of_Combat__WIP`, write the `Combat_Dump` and assert. Scenario matrix: open-field stack fight, city assault with walls, lair, flee/retreat, turn-50 timeout — several seeds each.

### Tier 3 — Full end-to-end via `.hms`

Load the crafted save, scenario script moves the stack onto the enemy so combat triggers through `Move_Units()` → the real overland path, click the Auto button (or set `strategic_combat_only` in config), run to completion, `--dump-save`, assert on post-combat overland state. Only this tier validates the trigger path and shared writeback end-to-end. Keep to a couple of scenarios — slowest and most brittle tier; Tier 2 carries the coverage.

## CTest wiring

Reuse the fixture chain: `HeMoM_Combat_Setup` (seeded newgame + positioning writes the crafted save, or a checked-in `.GAM`) → `HeMoM_Combat_Strategic_Run` / `HeMoM_Combat_Tactical_Run` → `HeMoM_Combat_Assertions`. Picked up by `ctest -R "HeMoM_"` alongside the existing suite. New sources (`--combat` mode, `Combat_Dump.c`) must be added to both `src/CMakeLists.txt` and `Makefile.am`.

## Build-out order

1. **Tracer bullet:** HeMoM `--combat` strategic-only — crafted save, direct `Combat__WIP` call, dump winner + casualties, one assert file. Proves the whole pipeline with the least UI risk.
2. Tactical AI-vs-AI under headless; fix whatever the combat screen's alloc/graphics path needs.
3. `Combat_Dump` fleshed out + scenario matrix (city/walls, lair, flee, timeout) with `LIVENESS`/`BOUNDS` legality rules.
4. Tier 1 GTest kernels, incrementally as each combat function is walked through.
5. Tier 3 end-to-end trigger test, last.

## Open risk

The single unverified premise is step 2: that `Combat_Screen__WIP` runs to completion under `Platform_Headless` with `_auto_combat_flag` set. Other full screens run headless in HeMoM, but the combat screen has never been driven that way. Everything else assembles parts that demonstrably work today.
