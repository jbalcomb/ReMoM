# AI_Next_Turn — Downstream Done-Done Plan

Tracking the per-turn review/cleanup work for every function reachable from [`AI_Next_Turn`](../MoM/src/AIDUDES.c) ([AIDUDES.c:157-393](../MoM/src/AIDUDES.c#L157-L393)).

---

## Todolist

### Wave 1 — Settle-sibling per-landmass deciders ✅ done
- [x] **1A** [`AI_Stacks_Do_Purify`](../MoM/src/AIMOVE.c#L4309) — **done-done** (commit `4e0734f5`), doc [AIMOVE-AI_Stacks_Do_Purify.md](ComputerPlayer/AIMOVE-AI_Stacks_Do_Purify.md)
- [x] **1B** [`AI_Stacks_Do_RoadBuild`](../MoM/src/AIMOVE.c#L4444) — **done-done** (commit `f5cd3a0e`), doc [AIMOVE-AI_Stacks_Do_RoadBuild.md](ComputerPlayer/AIMOVE-AI_Stacks_Do_RoadBuild.md)
- [x] **1C** `AI_Stacks_Order_Purify` ([AIMOVE.c:4803](../MoM/src/AIMOVE.c#L4803)) + `AI_Stacks_Order_RoadBuild` ([AIMOVE.c:4707](../MoM/src/AIMOVE.c#L4707)) — **folded into 1A/1B walkthroughs**. Real names use `_Stacks_` prefix; TODO previously listed stale `AI_Order_*` names.

### Wave 2 — Audit `AI_Set_Unit_Orders` slots ✅ done
Parent `AI_Set_Unit_Orders` is done-done (commit `f1481999`, doc [AIMOVE-AI_Set_Unit_Orders.md](ComputerPlayer/AIMOVE-AI_Set_Unit_Orders.md)). Slot audits are covered as one-line entries in the parent doc with location, role, and OGBUG flags. No per-slot walkthroughs exist or are required at the audit level.
- [x] **2A** slot 1 `AI_Disband_To_Balance_Budget` ([AIMOVE.c:3147](../MoM/src/AIMOVE.c#L3147)) — **done-done 2026-06-30**, doc [AIMOVE-AI_Disband_To_Balance_Budget.md](ComputerPlayer/AIMOVE-AI_Disband_To_Balance_Budget.md). Full walkthrough: two near-identical loops (food+gold deficits → non-fantastic units; mana deficit → fantastic units). 4-entry bug catalog (B1 = `Effective_Unit_Strength(.type)` param mismatch in Loop 2, IDA-confirmed OG-faithful; B2/B3 = post-Kill_Unit `wp=0` resets in both loops; B4 = `niu_var_1C` ghost local). Per-loop 200-iteration cap, asymmetric food (--1) vs gold (-= upkeep) drain in Loop 1, upkeep floor of 1.
- [x] **2B** slot 2 `AI_Shift_Off_Home_Plane` ([AIMOVE.c:3323](../MoM/src/AIMOVE.c#L3323)) — **done-done 2026-06-30**, doc [AIMOVE-AI_Shift_Off_Home_Plane.md](ComputerPlayer/AIMOVE-AI_Shift_Off_Home_Plane.md). Walkthrough plus 2 reconstruction-error fixes restored to 1:1 with OG asm: R1 (tower path) — inner unit-position check + `_UNITS[itr].wp` update + post-loop `_ai_all_own_stacks[itr2].wp` set; R2 (Astral-Gate path) — added missing post-inner-loop `_ai_all_own_stacks[itr2].wp` set. Both branches now structurally identical (matching `loc_EF72C` / `loc_EF7C9` symmetry). PLANAR_SEAL global gate scans all players including self — OG-faithful, preserved.
- [x] **2C** slot 3 `AI_Move_Out_Boats` ([AIMOVE.c:3408](../MoM/src/AIMOVE.c#L3408)) — **done-done 2026-06-30**, doc [AIMOVE-AI_Move_Out_Boats.md](ComputerPlayer/AIMOVE-AI_Move_Out_Boats.md). Walkthrough plus 2 reconstruction-error fixes restored to 1:1 with OG asm: R1 (center-skip gate) — `&&` → `||` so all 8 neighbors are scanned (was only 4 diagonals); R2 (missing lower-bound checks) — added `(itr_wx + unit_wx) >= 0` and `(itr_wy + unit_wy) >= 0` guards to prevent negative coords reaching `Square_Is_OceanLike`. Additional clause-order fix landed: upper-bound and center-skip clauses reordered to match OG asm sequence (center → upper-wx → upper-wy → lower-wx → lower-wy); operand order on the center-skip swapped to put `itr_wy` first per asm:96-99. Sub-helper `AI_Stack_Set_Boats_Goto` ([AIMOVE.c:5755](../MoM/src/AIMOVE.c#L5755)) — **done-done 2026-06-30**, doc [AIMOVE-AI_Stack_Set_Boats_Goto.md](ComputerPlayer/AIMOVE-AI_Stack_Set_Boats_Goto.md). Walkthrough verified 1:1 faithful to OG asm; commits `us_GOTO` Status + `dst_wx/wy` to every Transport-capable unit at the stack's `(wx, wy, wp)` and marks the stack `.value = ST_UNDEFINED`. Single caller (`AI_Move_Out_Boats` Phase 4).
- [x] **2D** slot 9 `AI_Find_Opportunity_City_Target` ([AIMOVE.c:3032](../MoM/src/AIMOVE.c#L3032)) — **done-done 2026-06-30**, doc [AIMOVE-AI_Find_Opportunity_City_Target.md](ComputerPlayer/AIMOVE-AI_Find_Opportunity_City_Target.md). Walkthrough 1:1 faithful to OG asm: for each `us_GOTO` stack, scans 3×3 box on plane `wp` for weakly-defended (<4 defenders) enemy city; if found, resets stack and per-unit Status to `us_Ready` so the per-landmass dispatch can re-target. Caller-side OGBUG (`wp=0` uninitialized at call site, Arcanus-only scope) documented in parent `AI_Set_Unit_Orders` doc — preserved.

### Wave 3 — Pre-orders setup phase (per-AI loop body)
- [x] **3A** [`AI_Evaluation_Map`](../MoM/src/AIMOVE.c#L6796) — **done-done 2026-06-30**, doc [AIMOVE-AI_Evaluation_Map.md](ComputerPlayer/AIMOVE-AI_Evaluation_Map.md). Walkthrough 1:1 faithful to OG asm. 7 phases: EMM remap → `nonhostiles[]` build → clear map → unit pass (+strength, +NONHOSTILE flag) → lair pass 1 (+guardian strength, `intact == ST_TRUE`) → city pass (+1, +SITE) → lair pass 2 (+SITE, `intact != ST_FALSE`) → node pass (+SITE). 3-entry bug catalog (B1/B2 = unit owner_idx/wp `ST_UNDEFINED` OOB; B3 = lair/city/node `(wp, wx, wy)` range OOB) — all are OG bugs that would access-violate on modern OS; production preserves OG up to crash point and mitigates with `/* HACK */ continue;` blocks at four lair/city/node sites + two unit-pass sites. Two preserved OG quirks: duplicate own-skip test (`||` chain) in unit pass; asymmetric intact filter (`==ST_TRUE` vs `!=ST_FALSE`) between lair passes. `AI_TARGET_NONHOSTILE=0x4000`, `AI_TARGET_SITE=0x8000`. No EMM restore (downstream `AI_Set_Unit_Orders` Phase 5 handles it).
- [x] **3B** [`AI_Evaluate_Continents`](../MoM/src/AIMOVE.c#L6283) — **done-done** (commit `0c9cd1bf` bugfix & test), doc [AIMOVE-AI_Eevaluate_Continents.md](ComputerPlayer/AIMOVE-AI_Eevaluate_Continents.md) (filename has typo `Eevaluate`)
- [x] **3C** [`AI_Choose_War_Landmass`](../MoM/src/AIMOVE.c#L7033) — **done-done** (commit `5af131c9`), doc [AIMOVE-AI_Choose_War_Landmass.md](ComputerPlayer/AIMOVE-AI_Choose_War_Landmass.md)
- [x] **3D** Hostility cluster — **done-done**: `AI_Evaluate_Hostility` (`7923da51`), `AI_Evaluate_Magic_Power_Strategy` (`cd60a653`), `Player_Hostile_Opponents` (`4f34cc35`)
- [x] **3E** Target-values cluster — **done-done**: `AI_Player_Calculate_Target_Values` (`c95bc5a6`), `AI_Landmass_Values_And_Strengths` (`e74c3bbb`); see [MoM-AI-Target-Values.md](ComputerPlayer/MoM-AI-Target-Values.md)
- [ ] **3F** Magic/gold cluster — **partial**:
  - [x] `Players_Update_Magic_Power` ([CITYCALC.c:116](../MoM/src/CITYCALC.c#L116)) — **done-done** (commit `1830e9fe`)
  - [x] `AI_Update_Magic_Power` ([AIDUDES.c:571](../MoM/src/AIDUDES.c#L571)) — **done-done 2026-06-30**, doc [AIDUDES-AI_Update_Magic_Power.md](ComputerPlayer/AIDUDES-AI_Update_Magic_Power.md). Walkthrough 1:1 faithful to OG asm; lower_snake_case sweep applied (`Mana_Total→mana_reserve`, `Nominal_Skill→base_casting_skill`, `Unresearched_Spells→knowable_spell_count`). R1 reconstruction-error fix: missing `casting_spell_idx == spl_Spell_Of_Mastery → 0/10/90` block added between Fire_Elemental (70/10/20) and `_turn < 30` (35/25/40) overrides (asm:385-406). Four preserved OG quirks: Knowable-scan ignores already-queued candidates; all-researched branch replaces skill_ratio instead of adding; `jmp loc_D2C9F` skips overrides when all researched; Fire_Elemental override checks researching_spell_idx not casting (OG-faithful per asm:368).
  - [x] `AI_Update_Gold_And_Mana_Reserves` ([AIDUDES.c:1709](../MoM/src/AIDUDES.c#L1709)) — **done-done 2026-06-30**, doc [AIDUDES-AI_Update_Gold_And_Mana_Reserves.md](ComputerPlayer/AIDUDES-AI_Update_Gold_And_Mana_Reserves.md). Walkthrough 1:1 faithful to OG asm; production embeds `/* loc_DXXXX */` asm-label comments in the source for verifiable mapping. Three reconstruction-error fixes landed: R1 (int16_t locals → int32_t to match OG `dword ptr` — fixes `Resource_Total = gold + mana` overflow when both reserves clamp near 30000), R3 (missing path 2 rebalance restored via `else if (gold < mana/4)` with `mana > 100` gate matching `loc_D4487`), R4 (`=` → `+=` on the "grow" sides of paths 1 and 2 to match OG `add`/`adc` in `loc_D43B5` / `loc_D449A`). Three preserved OGBUGs: Spell of Return overwrites `mana_reserve` instead of adding (both alchemy branches, faithful to OG `mov` store); mana==0 init reads original `gold_reserve` twice; Random(10) gate rolls only once per invocation. Lower_snake_case sweep on `Desired_Mana` / `Desired_Gold` / `Resource_Total` still pending.
  - [x] `AI_Update_Gold_Income_And_Food_Income` ([AIDUDES.c:1811](../MoM/src/AIDUDES.c#L1811)) — **done-done 2026-06-30**, doc [AIDUDES-AI_Update_Gold_Income_And_Food_Income.md](ComputerPlayer/AIDUDES-AI_Update_Gold_Income_And_Food_Income.md). Walkthrough 1:1 faithful to OG asm. Five phases: ghost-local init + first income fetch → tax up-nudge (gate `(_turn/50)*75 > gold AND tax<6`) → income refresh + tax down-nudge (gate `gold>200 AND tax>1`) → tax clamp to 4 → food-deficit remediation guarded by `if(food_income<0)`. R1 reconstruction-error fix: production unconditional negate replaced with `if (food_income < 0) { SETABS(food_income); while(...) { } }` matching OG `cmp; jl loc_D46D8; jmp @@Done` structure — surplus case now correctly exits without adding farmers. Six preserved OGBUGs/quirks: three ghost `niu_variable_*` locals (init order 1,3,2), tax up-nudge `<6` over-permissive vs clamp `>4`, two `Player_Resource_Income_Total` calls per invocation, farmer counter subtracts 2 even for Halfling/Granary cities (produces 3), `Random(_cities)-1` with no `_cities==0` guard, SETABS-inside-guard is semantically-plain-negate (kept for readability).
  - [x] `AI_Sanity_Check_Overland_Enchantments` ([AISPELL.c:4096](../MoM/src/AISPELL.c#L4096)) — **done-done 2026-06-30**, doc [AISPELL-AI_Sanity_Check_Overland_Enchantments.md](ComputerPlayer/AISPELL-AI_Sanity_Check_Overland_Enchantments.md). Walkthrough 1:1 faithful to OG asm. Six phases: 5 realm-conflict dispels (ETERNAL_NIGHT if not-Death, EVIL_OMENS if Life/Nature, CHAOS_SURGE if not-Chaos, TRANQUILITY if Chaos, LIFE_FORCE if Death) + mana-zero mass-dispel of 24 specific globals. Three reconstruction-error fixes landed: R1 (EVIL_OMENS `&&` chain of impossible == combos → OR chain matching OG `jz-to-dispel` shape), R2 (TRANQUILITY `Prim && Sec == Chaos` → OR), R3 (LIFE_FORCE — two compounded errors: `&&` → `||` AND wrong realm `sbr_Chaos` → `sbr_Death`). Preserved OG behaviors: mana-zero wipes 24 globals not all globals, dispel-to-0 not to sentinel, no RNG. Full function 1:1 with OG asm; no open issues.
- [x] **3G** [`Player_All_Colony_Autobuild`](../MoM/src/AIBUILD.c#L45) — **done-done 2026-06-30**, doc [AIBUILD-Player_All_Colony_Autobuild.md](ComputerPlayer/AIBUILD-Player_All_Colony_Autobuild.md). Walkthrough 1:1 faithful to OG asm. Four phases: owner filter → set `assign_new_building = FALSE` and check `bt_AUTOBUILD` → check `bt_Housing`/`bt_TradeGoods` with `Random(6) == 1` gate (16.67%) → dispatch to `Player_Colony_Autobuild_NP` (neutral) or `Player_Colony_Autobuild_CP` (AI wizards), or `AI_Player_City_Buy_Production` for real projects on non-neutral players. Called twice per turn: [AIDUDES.c:299](../MoM/src/AIDUDES.c#L299) per AI player + [AIDUDES.c:354](../MoM/src/AIDUDES.c#L354) for `NEUTRAL_PLAYER_IDX`. No reconstruction errors; production 1:1 with OG.
- [x] **3H** [`AI_Kill_Excess_Settlers_And_Engineers`](../MoM/src/AIDUDES.c#L1938) — **done-done 2026-06-30**, doc [AIDUDES-AI_Kill_Excess_Settlers_And_Engineers.md](ComputerPlayer/AIDUDES-AI_Kill_Excess_Settlers_And_Engineers.md). Walkthrough 1:1 faithful to OG asm. Two phases: zero per-landmass counters → per-unit scan (owned units only) with independent settler branch (`UA_CREATEOUTPOST`, every turn, `> 1` per landmass → `Kill_Unit(itr, kt_Normal)`) and engineer branch (`Construction > 0` AND `_turn > 200`, same cap). R1 reconstruction-error fix landed: settler filter `UA_CREATEUNDEAD` (`0x0080`) → `UA_CREATEOUTPOST` (`0x0020`) matching OG asm:59 — settler-cull branch now actually fires. Five preserved OG behaviors: oldest unit per landmass survives (`_UNITS[]` append-only + index-order iteration); `landmass_idx == 0` skips (ocean/unassigned); `landmass_idx >= NUM_LANDMASSES` skips (defensive upper bound); engineer cull gates on `_turn > 200` while settler cull runs every turn; ability + Construction tests are independent (no dual-count guard, no stock unit has both flags). Redundant declaration-initializer + explicit init loop noted as minor structural; not a bug.

### Wave 4 — Overland Spell-AI Subsystem ✅ done
Triggered from [AIDUDES.c:261-272](../MoM/src/AIDUDES.c#L261-L272) when the AI's queued spell finishes accumulating mana.

- [x] **4A** [`Cast_Spell_Overland`](../MoM/src/OverSpel.c#L627) — **done-done 2026-06-10** (commit `9cd8113d`), doc [SETTLE-Cast_Spell_Overland.md](ComputerPlayer/SETTLE-Cast_Spell_Overland.md). Completes the in-progress overland spell cast (lines 627-1956, 1330 lines). 9-entry bug catalog. Nature's Wrath retaliation wired via `Call_Forth_The_Force_Of_Nature(player_idx)` at [OverSpel.c:1944](../MoM/src/OverSpel.c#L1944).
- [x] **4B** [`AI_Spell_Select`](../MoM/src/AISPELL.c#L309) — **done-done 2026-06-11** (commit `0c27ba57`), doc [AISPELL-AI_Spell_Select.md](ComputerPlayer/AISPELL-AI_Spell_Select.md). Dispatcher complete; OGBUG flagged at [line 320](../MoM/src/AISPELL.c#L320). Spell-group leaf pickers (8 of them: Summon, Unit_Enchantment, City_Enchantment, Disenchant, Disjunction, Global, Suppression, Attack) all reconstructed and have walkthrough docs under `AISPELL-AI_Select_Spell_Group_*.md`.
- [x] **4C** Target-picker cluster — **done-done**: `AITP_City_Enchantment` (`d30d8ead`), `AITP_Unit_Enchantment` (`47cb2bac`), `AITP_Attack_Wizard` (`176fc764`), `AITP_Disjunction` (`963ce29c`); docs in `doc/ComputerPlayer/AISPELL-AITP_*.md`
- [x] **4D** Specific-spell-targeting cluster — **done-done**: `AITP_Change_Terrain` (`90785676`), `AITP_Spell_Ward` (`1a450f9d`), `AITP_Spell_Ward_Wrapper` (`ffb08b13`), and the full AITP_* lineup (Transmute, Wall_Of_Stone, Wall_Of_Fire, Wall_Of_Darkness, Move_Fortress, Flying_Fortress, Earth_Gate, Gaias_Blessing, Heavenly_Light, Altar_Of_Battle, Stream_Of_Life, Inspirations, Prosperity, Astral_Gate, Dark_Rituals, Cloud_Of_Shadow, Consecration, Enchant_Road, Disenchant, Attack_Terrain, Plane_Shift, Natures_Cures, Floating_Island, Attack_Stack, Corruption, Node, Select_Unit_For_Enchantment, Get_Map_Square_Target_For_Spell)
- [x] **4E** `Overland_Enchantment_Spell_Index` ([OverSpel.c:1988](../MoM/src/OverSpel.c#L1988), renamed from `IDK_Get_Global_Enchant_Index__STUB`) — **folded into [AISPELL-AITP_Disjunction.md](ComputerPlayer/AISPELL-AITP_Disjunction.md)** as a Companion section

### Wave 5 — NPC + event phase
- [ ] **5A** [`NPC_Farmers`](../MoM/src/AIDUDES.c#L1889) + [`NPC_Destinations`](../MoM/src/AIDATA.c#L581) cluster — **partial**:
  - [x] `NPC_Farmers` ([AIDUDES.c:1889](../MoM/src/AIDUDES.c#L1889)) — **done-done 2026-06-30**, doc [AIDUDES-NPC_Farmers.md](ComputerPlayer/AIDUDES-NPC_Farmers.md). GEMINI-translated function verified 1:1 against OG asm. Five phases: outer per-neutral-city loop → helper calls (`City_Minimum_Farmers`, `Map_Square_Unit_Count`) → Halfling / `bldg_status[ANIMISTS_GUILD]` bonus check → 3-food or 2-food `ceil((min + count) / divisor)` branch → population clamp. R1 reconstruction-error fix: production restored to `bldg_status[ANIMISTS_GUILD]` (byte offset 10) matching OG asm `+0Ah` — preserving OG-as-written (author almost certainly meant GRANARY but coded ANIMISTS_GUILD, flagged inline as `/* OGBUG */`). Five preserved OG quirks: `City_Minimum_Farmers` misleading name (returns food units, not farmer count), `unit_count` counts only units on city tile, ANIMISTS_GUILD-instead-of-GRANARY OGBUG, Halfling+Building-Built are OR not additive, no RNG.
  - [x] `NPC_Destinations` ([AIDATA.c:581](../MoM/src/AIDATA.c#L581)) — **done-done 2026-06-30**, doc [AIDATA-NPC_Destinations.md](ComputerPlayer/AIDATA-NPC_Destinations.md). Walkthrough 1:1 faithful to OG asm (642-line asm). Seven phases: `EMMDATAH_Map` + `Build_NPC_Stacks` → per-neutral-stack loop with (a) adjacent-city search in 2×2 box → (b) opportunity-redirect if GOTO-and-better-score, with `10 - troops - Δxy` scoring plus `+5` per difficulty level for human-owned cities → (c) idle-stack target-seek (30-city cap, plane + LandOnly-landmass filters) → (d) disband via `Kill_Unit` if no reachable targets → return stack count. R1 reconstruction-error fix (preservation-style): removed defensive `if(target_city_idx != -1)` guard that had been added around the current-target difficulty-bonus block, restoring OG bytes exactly (asm:320-340) and flagging the OOB read hazard via inline `/* OGBUG  OOB AVRL */` comment. Preserved OGBUGs: caller pushes NEUTRAL_PLAYER_IDX for `Build_NPC_Stacks` even though callee is void (hard-coded internally); `Delta_XY_With_Wrap` in adj-city branch always returns 0/1; `stacl_wx` typo in OG stack-frame silently corrected. Sub-helper `Build_NPC_Stacks` ([AIDATA.c:1161](../MoM/src/AIDATA.c#L1161)) — **done-done 2026-06-30**, doc [AIDATA-Build_NPC_Stacks.md](ComputerPlayer/AIDATA-Build_NPC_Stacks.md). Walkthrough 1:1 faithful to OG asm. Six phases: reset count → per-unit scan (dual-exit at `_units` OR count `>= 80`) → owner filter (hard-coded NEUTRAL) → position read → existing-stack search with position-match downgrade to `AICAP_None` if no air/water travel → city-defender exclusion → new-stack record with `AICAP_LandOnly` (air OR water) or `AICAP_None` (neither). No reconstruction errors; production 1:1 with OG. Called only from `NPC_Destinations` Phase 1.
- [x] **5B** [`Make_Raiders`](../MoM/src/AIDATA.c#L353) + [`Make_Monsters`](../MoM/src/AIDATA.c#L124) — **done**:
  - [x] `Make_Raiders` ([AIDATA.c:353](../MoM/src/AIDATA.c#L353)) — **done-done 2026-06-30**, doc [AIDATA-Make_Raiders.md](ComputerPlayer/AIDATA-Make_Raiders.md). Walkthrough 1:1 faithful to OG asm (404-line asm) — GEMINI-translated body re-verified against bytes. Thirteen phases: neutral-city presence check → accumulator `+= Random(_difficulty + 1)` → `>= 30` threshold → 1000-try loop with random-city roll, neutral-owner filter, same-landmass AI-fortress + non-neutral-city checks, `Adjacent_Free_Square`, `Army_At_Square_2`, `raiders_count = troop_count * difficulty / 6` (× 2/3 if AI fortress, /2 if Myrror-and-turn<200, min 1), turn-scaled `raiders_level_neg` (-4/-3/-2/-1), per-raider unit picker with 3 filters (`<= ut_BarbSwordsmen`, `Transport`, `UA_CREATEOUTPOST`), `Create_Unit` at NEUTRAL, kill `units_created/3` of source garrison → failure fallback bumps `average_unit_cost += 15`. No reconstruction errors; production 1:1 with OG. Nine preserved OGBUGs/quirks: `s_WIZARD` fields repurposed as accumulators, `<= ut_BarbSwordsmen` includes Spearmen+Swordsmen, cross-plane landmass_idx false match, `Random - 1` false-BUG comment preserved, duplicate `tries = 0` in OG compiler artifact, `var_3A` clever trick for `Random-1` array indexing, neutral-scan no-early-break vs non-neutral-scan early-break, AI-fortress reduction to 2/3 (intent unclear), Myrror early-turn halving.
  - [x] `Make_Monsters` ([AIDATA.c:124](../MoM/src/AIDATA.c#L124)) — **done-done 2026-07-01**, doc [AIDATA-Make_Monsters.md](ComputerPlayer/AIDATA-Make_Monsters.md). Walkthrough 1:1 faithful to OG asm (497-line asm). Sibling of Make_Raiders — spawns rampage stacks from lairs (`NUM_LAIRS=102`) instead of neutral cities. Twelve phases: eligible-lair scan (intact + guard1 + non-Life) → accumulator `average_unit_cost += Random(_difficulty + 1)` → threshold `>= 50 - _difficulty*5` (Intro=50 down to Impossible=30) → turn gate `>= 50` → unified `while(1)` with `if/else`: search body (1000-try) OR plane-check block (`Random(3) <= 1` accept, else plane match against `_FORTRESSES[HUMAN].wp` — spin on mismatch) → tries-maxed exit → landmass compute → AI-home-continent check (human short-circuit + AI-fortress scan from `itr=1`) → race determination (`< rt_Arcane → rt_Death`) → `Adjacent_Free_Square` → budget `((Random(diff+1) + Random(diff+1)) * turn) / 5` (halved if AI shares landmass) → `Make_Monster_List` → per-monster `Create_Unit` at `-1` level-neg. R1 reconstruction-error fix landed: plane-check gate mechanic completely rewritten to match OG's spin-on-mismatch semantics — merged outer search loop and plane-check block into single `while(1)` with `if/else` predicate (De Morgan's inverse of OG's `loc_F9B36` loop-exit test). Old wrong behavior (`Random(3) == 2` gate + `lair_idx = ST_UNDEFINED` reset triggering a new-lair reroll) replaced with OG-faithful `Random(3) <= 1` early-accept + spin-on-mismatch preserving lair_idx and tries. Ten preserved OGBUGs/quirks: `_players[NEUTRAL].average_unit_cost` repurposed as monster accumulator; Life-guarded lairs never rampage; `rt_Death` fallback for below-`rt_Arcane` races; human-fortress-on-landmass disables AI-home-continent halving; cross-plane landmass_idx false match; `Random(NUM_LAIRS) - 1` inline BUG comment preserved (false alarm); plane-check OOB read of `_LAIRS[-1].wp` on tries-maxed spin path (marked with `/* OGBUG  OOB AVRL */` in-code); IDA label noise (`s_UNIT.type`/`s_UNIT.wp` for `s_LAIR` fields); `uu_bogus` unused local; `raiders_level_neg = -1` hard-coded for monsters; threshold reset even on turn-gate rejection.
- [ ] **5C** [`NPC_Excess_Garrison`](../MoM/src/AIDATA.c#L53) + [`AI_Hopeless_Stasis`](../MoM/src/AIDUDES.c#L2168) — bodies added (commits `0f3f9415`, `94fdf41a`) but **not done-done**; no docs

### Wave 6 — Helpers / plumbing
- [ ] **6A** `EMMDATAH_Map` + [`EMM_Map_CONTXXX__WIP`](../MoM/src/AIDUDES.c#L2208) — EMS paging helpers; still `__WIP` suffix, 6+ call sites across AIMOVE.c/AIDUDES.c
- [ ] **6B** [`Allocate_AI_Data`](../MoM/src/AIDATA.c#L735) — per-turn AI data allocation
- [ ] **6C** `AI_Log_Metrics` + `AI_Metrics_Emit_Turn_Summary` — STU_LOG metrics emitters

### Capstone
- [ ] **Final** [`AI_Next_Turn`](../MoM/src/AIDUDES.c#L157) parent walkthrough doc — tie the whole chain together once all downstream is done-done

---

## Review — what's downstream of `AI_Next_Turn`

Mapped from [AIDUDES.c:157-393](../MoM/src/AIDUDES.c#L157-L393). ~35 distinct downstream functions across 5 phases:

```
AI_Next_Turn()
├── PRE   : EMMDATAH_Map, Allocate_AI_Data, unit sanity-check loop
├── PER-AI: 15 functions (Eval_Hostility, Magic_Power_Strategy, Hostile_Opponents,
│           Target_Values, Landmass_Values_And_Strengths, Choose_War_Landmass,
│           Spell_Select, Update_Magic_Power, Sanity_Check_Overland_Enchantments,
│           Update_Gold_And_Mana_Reserves, Update_Gold_Income_And_Food_Income,
│           Player_All_Colony_Autobuild, Evaluation_Map, Evaluate_Continents,
│           Set_Unit_Orders, Kill_Excess_Settlers_And_Engineers)
├── MOVE  : AI_Execute_Orders × per-AI-player + neutral   ✓ DONE
├── NPC   : Player_All_Colony_Autobuild, NPC_Farmers, NPC_Destinations,
│           AI_Execute_Orders for NEUTRAL_PLAYER
├── EVENTS: Make_Raiders, Make_Monsters
└── CLEAN : NPC_Excess_Garrison, AI_Hopeless_Stasis, AI_Log_Metrics
```

---

## Status snapshot (as of 2026-06-30)

### Wave totals
- **Wave 1**: ✅ done (3/3 items)
- **Wave 2**: ✅ done (4/4 items, audited via parent `AI_Set_Unit_Orders` doc)
- **Wave 3**: ⚠️ partial — 3B/3C/3D/3E done; **3A, 3F (4 funcs), 3G, 3H open**
- **Wave 4**: ✅ done (4A/4B/4C/4D/4E all done-done, plus full AITP_* lineup + spell-group leaves)
- **Wave 5**: ❌ open (6 funcs; 4 added but not done-done, 2 with no commits)
- **Wave 6**: ❌ open (3 sub-items)
- **Capstone**: ❌ open

### Suffix-tagged functions still hanging in AI-Turn paths
- **AIDUDES.c / AIMOVE.c**: `EMM_Map_CONTXXX__WIP` ([AIDUDES.c:2208](../MoM/src/AIDUDES.c#L2208)) — Wave 6A; 6+ call sites
- **AIDATA.c**: `Create_Unit` callers — `Make_Monsters` ([AIDATA.c:314](../MoM/src/AIDATA.c#L314)) and `Make_Raiders` ([AIDATA.c:549](../MoM/src/AIDATA.c#L549)); the helper itself lives outside AI-Turn scope
- **AIBUILD.c**: `j_CTY_GrandVizier__STUB` ([AIBUILD.c:774](../MoM/src/AIBUILD.c#L774)) — single call, low-impact
- **AISPELL.c / OverSpel.c**: 0 ✅ (Wave 4 cleanup complete)

### Done-done already (from this and prior sessions)

**AIMOVE.c** — all Settle/Meld/Roamer/Ferry paths plus per-landmass deciders:
- `AI_Execute_Orders` + 5 per-unit Status-handlers (Road/Meld/Settle/Move/Ferry)
- `AI_Stacks_Do_Meld`, `AI_Stacks_Do_Settle`, `AI_Stacks_Do_Purify`, `AI_Stacks_Do_RoadBuild`
- `AI_Set_Unit_Orders` (parent + per-slot audits)
- Slot 8: `AI_Stacks_Init_Build_Target_Order`
- Slot 10: `AI_Build_Target_List` (+ `AI_Add_Target`, `AI_Stacks_Assign_Target`)
- Slot 11: `AI_Stacks_Move_Out_NonMilitary_Garrisoned`
- Slot 12: `AI_Stacks_Survey_Expedition_Forces` (+ `_Stack`)
- Slot 13: `AI_Stacks_Garrison_Sites`
- Both per-plane post-passes: `AI_Stacks_Wartime_Ocean_Movement_And_Cleanup`, `AI_Stacks_Ocean_Landmass_Orders`
- Continent helper: `AI_Reevaluate_Continent`
- Roamer paths: `AI_Stacks_Roamers_Target_Or_Deploy`, `AI_Stacks_Order_To_War_Landmass`, `AI_Stacks_Relocate_Roamers`, `AI_Stacks_Stage_Expedition_Forces`
- Ferry paths: `AI_Stacks_Setup_Ferry`, `AI_Stacks_Ferry_Add_Location`, `AI_Stacks_Order_Ferry`
- Pre-orders setup (Wave 3 covered): `AI_Evaluate_Continents`, `AI_Choose_War_Landmass`, `AI_Evaluate_Hostility`, `AI_Evaluate_Magic_Power_Strategy`, `Player_Hostile_Opponents`, `AI_Player_Calculate_Target_Values`, `AI_Landmass_Values_And_Strengths`

**OverSpel.c / AISPELL.c** — full overland spell-AI subsystem (Wave 4):
- `Cast_Spell_Overland` (cast completion)
- `AI_Spell_Select` (dispatcher) + `AI_Compute_Spells_Info` + 8 `AI_Select_Spell_Group_*` leaf pickers
- Full `AITP_*` lineup (target pickers for ~30 distinct spell-target types) + `Get_Map_Square_Target_For_Spell` + `Select_Unit_For_Enchantment`
- `Overland_Enchantment_Spell_Index` (slot→spell helper)

**CITYCALC.c**: `Players_Update_Magic_Power`

---

## Plan rationale

**Waves 1, 2, 4** — closed out. Wave 1 mechanical cleanup of the Settle-sibling family; Wave 2 audited at the parent-doc level (slot audits don't require per-leaf walkthroughs); Wave 4 was the bulk of recent session effort (overland spell-AI subsystem) and is fully cleaned.

**Wave 3 partial** — the foundational pre-orders setup phase is mostly done (3B/3C/3D/3E walkthroughs cover ~7 functions). Remaining: `AI_Evaluation_Map` (3A, foundational), four magic/gold update helpers (3F), `Player_All_Colony_Autobuild` (3G), `AI_Kill_Excess_Settlers_And_Engineers` (3H).

**Wave 5** — NPC + event phase. Discrete, well-scoped chunk. Three of six functions have implementations committed but no review/cleanup pass.

**Wave 6** — helpers / plumbing. EMM paging (6A) is the only one with a `__WIP` suffix still in active use across the AI-Turn paths.

**Capstone** — last; ties everything together.

---

## Recommended starting point

Open file context: [AIDATA.c](../MoM/src/AIDATA.c). Natural next picks (in increasing scope):

1. **Wave 5C — `NPC_Excess_Garrison`** ([AIDATA.c:53](../MoM/src/AIDATA.c#L53)) — already `add`-committed, just needs review pass / walkthrough doc.
2. **Wave 5A — `NPC_Farmers`** ([AIDUDES.c:2023](../MoM/src/AIDUDES.c#L2023)) — same shape, no commit yet.
3. **Wave 6B — `Allocate_AI_Data`** ([AIDATA.c:735](../MoM/src/AIDATA.c#L735)) — small helper, fits the AIDATA.c context.

Alternative direction (Wave 3 finish):
- **Wave 3F (4 funcs)** — the `AI_Update_*` cluster in AIDUDES.c. Mostly self-contained per-resource update helpers; mechanically similar to one another.
- **Wave 3A — `AI_Evaluation_Map`** ([AIMOVE.c:6809](../MoM/src/AIMOVE.c#L6809)) — the last big foundational setup function; downstream consumers are already documented.

---

## Per-function done-done checklist

For each function in the todolist, "done-done" means:

1. **Walkthrough doc** — code walk, mermaid (if multi-branch), bug catalog, history note
2. **OG asm verification** — read Piethawn output, identify reconstruction errors vs OGBUGs
3. **Bug catalog** — B*/R* numbered, severity tagged
4. **GEMINI variant deletion** (if any)
5. **`__WIP` / `__STUB` / `__ALWAYS_FALSE` suffix removal** (if real body exists)
6. **Naming convention sweep** — `Tile` → `Square`, `AI_Do_*` → `AI_Stacks_Do_*` family, CamelCase locals → lower_snake_case, etc.
7. **Cross-doc sweep** — index entry in [MoM-AI-AIMOVE-Index.md](ComputerPlayer/MoM-AI-AIMOVE-Index.md), [stub_wip_todo.md](#TODO/stub_wip_todo.md), call-chain docs

---

## Related references

- [doc/ComputerPlayer/](ComputerPlayer/) — per-function walkthrough docs (sister-directory for sub-reviews)
- [doc/MoM-NextTurn-AI-Move.md](MoM-NextTurn-AI-Move.md) — the original chain notes
- [doc/ComputerPlayer/MoM-AI-Move.md](ComputerPlayer/MoM-AI-Move.md) — overview map
- [doc/ComputerPlayer/MoM-AI-AIMOVE-Index.md](ComputerPlayer/MoM-AI-AIMOVE-Index.md) — AIMOVE.c function index
- [doc/#TODO/stub_wip_todo.md](#TODO/stub_wip_todo.md) — running `__STUB`/`__WIP` punchlist
