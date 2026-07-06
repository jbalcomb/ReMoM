# Next_Turn_Proc — Downstream Done-Done Plan

Tracking the per-turn review/cleanup work for every function reachable from [`Next_Turn_Proc`](../MoM/src/NEXTTURN.c#L264) and its main callee [`Next_Turn_Calc`](../MoM/src/NEXTTURN.c#L609). `AI_Next_Turn` and its subtree are covered separately by [`__TODO-AiTurn.md`](__TODO-AiTurn.md) — this plan cross-references but does not re-review that branch.

---

## Todolist

### Wave 1 — Unit lifecycle helpers
Called at multiple points in the outer wrapper and the inner sim entry — used to clean up dead units, refresh movement points, and reset status flags between phases.

- [x] **1A** [`Delete_Dead_Units`](../MoM/src/NEXTTURN.c#L161) — **done-done**, doc [NextTurn/NEXTTURN-Delete_Dead_Units.md](NextTurn/NEXTTURN-Delete_Dead_Units.md). Compacts `_UNITS[]`, removing entries whose `owner_idx` is outside `[0,5]` (dead = `ST_UNDEFINED`) and fixing up hero `unit_idx` back-references; verified 1:1 against `ovr060/Delete_Dead_Units.asm`. Called 5× per turn (`Next_Turn_Proc` bookends, `Next_Turn_Calc`, `Loaded_Game_Update*`). No divergences; `continue` is an equivalent restructure (not in the asm).
- [ ] **1B** [`All_Units_In_Towers`](../MoM/src/WZD_o059.h#L64) — called 2× in `Next_Turn_Proc` bookends. Marks units at tower positions with `in_tower = TRUE`.
- [ ] **1C** [`Update_Units_MvsSts`](../MoM/src/NEXTTURN.c#L502) — called once in `Next_Turn_Proc`. Refreshes `moves2_max`/`moves2` from `Unit_Moves2()` and normalizes `Status` (Wait/ReachedDest → Ready).
- [ ] **1D** [`All_AI_Refresh_Units_Movement`](../MoM/src/NEXTTURN.c#L441) — sibling of 1C for non-human units; handles Stasis via `mutations & (C_STASISINIT | C_STASISLINGER) → Finished = TRUE`. Not called by `Next_Turn_Proc` directly but reached via `Loaded_Game_Update*` — noted for completeness.
- [ ] **1E** [`All_AI_Players_Contacted`](../MoM/src/NEXTTURN.c#L557) — called at the tail of 1C and 1D. Marks all AI wizards as contacted-by-human and processes Nature Awareness. Preserved OG comment flags an "invisibility BUG: only checks the enchantment, ignoring natural ability and item power".

### Wave 2 — Draw / graphics / sound setup
Infrastructure surrounding the sim. Not gameplay-load-bearing; may be lower priority for OG-fidelity review.

- [ ] **2A** [`Set_Unit_Draw_Priority`](../MoM/src/NEXTTURN.c) + [`Set_Entities_On_Map_Window`](../MoM/src/NEXTTURN.c) — draw ordering for the overland view.
- [ ] **2B** [`Reset_Map_Draw`](../MoM/src/MainScr_Maps.c#L1900) + [`Cache_Graphics_Overland`](../MoM/src/LOADER.c#L1657) — map draw state reset + LBX pre-caching.
- [ ] **2C** `Set_Draw_Active_Stack_Always` + `Reset_Draw_Active_Stack` — active-stack cursor draw toggles. Bracket the building-message loop.
- [ ] **2D** `Increment_Background_Music` — bgm cycle advance. Standalone.

### Wave 3 — End-of-turn UI
Chancellor summary, per-city building-complete popups, and cursor placement at the next idle stack. All human-facing.

- [ ] **3A** [`Chancellor_Screen__WIP`](../MoM/src/REPORT.c#L205) — still `__WIP` suffix. Gated on `magic_set.end_of_turn_summary == ST_TRUE`.
- [ ] **3B** Building-complete message loop ([NEXTTURN.c:308-376](../MoM/src/NEXTTURN.c#L308-L376)) — iterates `MSG_Building_Complete[]` up to `g_bldg_msg_ctr`. Per message: `Center_Map`, `City_Built_Building_Message` OR autobuild-fallback text via `Warn0`, then `City_Screen__WIP()` popup.
  - Sub-helpers: [`Center_Map`](../MoM/src/NEXTTURN.c), `City_Built_Building_Message`, [`City_Screen__WIP`](../MoM/src/CityScr.c#L178) — still `__WIP`.
- [ ] **3C** [`WIZ_NextIdleStack`](../MoM/src/UNITSTK.c#L438) — advances `_map_x`/`_map_y`/`_map_plane` to the next idle human stack. Called at the tail of `Next_Turn_Proc`.

### Wave 4 — Sim setup
Everything `Next_Turn_Calc` does before dispatching the AI turn.

- [ ] **4A** [`Set_Random_Seed(RNG_AI_Turn_Seed)`](../MoM/src/NEXTTURN.c#L626) + [`Get_Random_Seed`](../MoM/src/NEXTTURN.c#L854) round-trip — pins the AI-turn RNG to `RNG_AI_Turn_Seed` for the whole sim, then saves the advanced seed back. Critical for PRNG parity across saves.
- [ ] **4B** [`All_City_Calculations`](../MoM/src/NEXTTURN.c#L2083) — called 2× per turn (start and end of sim). Iterates cities calling `Do_City_Calculations` + `City_Minimum_Farmers` + `City_Rebel_Count`; clamps farmer count if below minimum.
- [ ] **4C** [`AI_Kill_Lame_Units`](../MoM/src/NEXTTURN.c#L4667) — pre-turn AI unit cull. Preserved OG source comment: `¿ BUGBUG  leaves dead/deleteable units lying around ?` — verify against OG asm.

### Wave 5 — AI turn ✅ done (cross-referenced)
- [x] **5** [`AI_Next_Turn`](../MoM/src/AIDUDES.c#L158) — **done-done** (this session), doc [AIDUDES-AI_Next_Turn.md](ComputerPlayer/AIDUDES-AI_Next_Turn.md). Whole subtree tracked by [`__TODO-AiTurn.md`](__TODO-AiTurn.md). Not re-reviewed here.

### Wave 6 — Post-AI misc
Called immediately after the AI turn dispatch, before the time-stop-vs-normal split.

- [ ] **6A** [`Next_Turn_Process_Purify`](../MoM/src/UNITSTK.c#L1787) — post-purify processing. Called once, unconditionally.
- [ ] **6B** [`Initialize_Reports`](../MoM/src/NEXTTURN.c#L2108) — initializes per-turn report data structures.

### Wave 7 — Time Stop path OR normal end-of-turn
`Next_Turn_Calc` splits on `g_timestop_player_num != 0`. Time-stop path only drains mana; normal path runs everything else.

- [ ] **7A** Time Stop mana drain block ([NEXTTURN.c:651-674](../MoM/src/NEXTTURN.c#L651-L674)) — `mana_reserve -= 200` per turn (manual says 150 — potential B-number OG behavior mismatch); if mana runs out, clear `Globals[TIME_STOP]`, set `MSG_GEs_Lost` if human, zero `g_timestop_player_num`. Not a separate function; inline block in `Next_Turn_Calc`. Verify against OG asm.
- [ ] **7B** [`Decrease_Peace_Duration`](../MoM/src/DIPLOMAC.c#L5691) — decrements all `peace_duration[]` counters. Only fires on non-time-stop turns.
- [ ] **7C** [`Update_Players_Gold_Reserve`](../MoM/src/NEXTTURN.c#L2360) — applies gold income to all players' `gold_reserve` (clamped to `MAX_GOLD_RESERVE` at loop tail in `Next_Turn_Calc`).
- [x] **7D** [`Players_Update_Magic_Power`](../MoM/src/CITYCALC.c#L116) — **done-done** (commit `1830e9fe`, cross-ref [`__TODO-AiTurn.md`](__TODO-AiTurn.md#L28) Wave 3F).
- [ ] **7E** [`Players_Apply_Magic_Power`](../MoM/src/NEXTTURN.c#L2442) — distributes magic power into mana / research / skill per player's strategy split.
- [ ] **7F** [`Players_Check_Spell_Research`](../MoM/src/NEXTTURN.c#L3615) — advances research progress; picks new spell if research complete.
  - [x] [`Player_Gets_Spell`](../MoM/src/NEXTTURN.c#L3778) (o140p21, callee) — **done-done**, doc [NextTurn/NEXTTURN-Player_Gets_Spell.md](NextTurn/NEXTTURN-Player_Gets_Spell.md). Records a learned/traded/gifted/treasure/conquest spell: mark `spells_list`, discount SoM cost, and if it was the research target clear the candidate list + re-select. Verified 1:1 against `ovr140/Player_Gets_Spell.asm`. This session fixed R1 (`som_research_cost` formula — missing `/2`, assign→floor-subtract), R2 (else-loop compared `researching_spell_idx`→`spell_idx`), R3 (remaining-cost `spell_data_table[spell_idx]`→`[researching_spell_idx]`), R4 (`spl_UNDEFINED`→`spl_NONE`); builds clean.
- [ ] **7G** [`Players_Apply_Upkeeps__WIP`](../MoM/src/NEXTTURN.c#L2497) — still `__WIP` suffix. Deducts unit/enchantment upkeep from gold/mana.
- [ ] **7H** `OVL_DisableIncmBlink` (pre-turn) + `OVL_EnableIncmBlink` (post-turn) — income-display blink toggles. Sandwich the sim block.

### Wave 8 — Events
Random events fire during normal end-of-turn (skipped during Time Stop).

- [x] **8A** [`Determine_Event`](../MoM/src/EVENTS.c#L221) — **done-done**, doc [NextTurn/EVENTS-Determine_Event.md](NextTurn/EVENTS-Determine_Event.md). Once-per-turn random-event picker: build `event_pressure_accumulator` (difficulty-weighted turns-since-last-event) → `Random(512)` fire test (`P = pressure/512`) → ≤5 `Random(18)` draws through the veto gauntlet → commit into `events_table`. Verified 1:1 against `ovr080/Determine_Event.asm` (910 lines; `.c` is a **Fable 5** translation, not Gemini), no divergences. Renamed this session: `EVNT_Enabled`→`g_random_events_enabled`, `Event_Chance`→`event_pressure_accumulator`, `Unused_Diff_Var`→`niu_escalation_scalar`, + locals lowercased. Preserved OGBUGs: first-5-turns event suppression, `niu_escalation_scalar` dead code, Donation/Rebellion out-of-order checks. Noted `g_random_events_enabled` is a vestigial OG global (hardcoded `TRUE`; real gate is the caller's `magic_set.random_events`).
  - Downstream helper: [`Get_Event_Victim`](../MoM/src/EVENTS.c#L1671) — **done-done**, doc [NextTurn/EVENTS-Get_Event_Victim.md](NextTurn/EVENTS-Get_Event_Victim.md). Weighted-random event target: wizard power (`magic + research + army`), inverted (`600 − power`) for good events, halved until total ≤ 500, then a `Random(total)` roulette walk (`-1` if all weights collapse to 0). Verified 1:1 against `ovr081/Get_Event_Victim.asm`, no divergences. Renamed this session: `IDK`→`total_weight`, `In_RNG_Range`→`weight_total_in_range`, `Weights_Remainder`→`roll_remaining`. The `event_type` param is a good/bad flag, not an event id. (Confirmed `Random(n)` is 1-based `1..n` per `random.c` — also corrected the `0..17`→`1..18` note in the 8A doc.)
- [x] **8B** [`Event_Twiddle`](../MoM/src/EVENTS.c#L654) — **done-done**, doc [NextTurn/EVENTS-Event_Twiddle.md](NextTurn/EVENTS-Event_Twiddle.md). Per-turn advancement of every starting/ongoing event (all 18 types): apply effect, tick multi-turn durations, roll to expire, `Show_Event_Message`. Verified 1:1 against `ovr080/Event_Twiddle.asm` (1222 lines), builds clean. **R1** (Pirates gold floor on `HUMAN_PLAYER_IDX`→`player_idx`), **R2** (spurious `Plague_Status = 0` + extra `EVNT_MsgDataValue` breaking the `es_Starting` branch — both removed), **R3** (Nature/Sorcery/Mana-Short `m_current_event` copy-pasted `et_Conjunction_Chaos` → own ids) all fixed this session; status/message literals enum-ized. Renamed `CRP_EVNT_Var_1/2`→`niu_event_twiddle_flag`/`_type_flag` (write-only OG globals, never read). ReMoM deviations: Gift `Gradient_Fill`→`DARKEN_SCREEN()`, `DBG_trigger_event` reset.
- [x] **8C** [`Cool_Off_Volcanoes`](../MoM/src/NEXTTURN.c#L4525) — **done-done**, doc [NextTurn/NEXTTURN-Cool_Off_Volcanoes.md](NextTurn/NEXTTURN-Cool_Off_Volcanoes.md). Per-turn map sweep: each Volcano tile has a 2% (`Random(100) < 3`, 1-based) chance to cool to Mountain, then `Volcano_Counts` re-tallies. Verified 1:1 against `ovr140/Cool_Off_Volcanoes.asm` — no divergences, no deviations.

### Wave 9 — City-wide passes
Outpost growth, city changes, nightshade counts, and building removal.

- [x] **9A** [`All_Outpost_Population_Growth`](../MoM/src/CITYCALC.c#L3225) — **done-done**, doc [NextTurn/CITYCALC-All_Outpost_Population_Growth.md](NextTurn/CITYCALC-All_Outpost_Population_Growth.md). Per-turn outpost `Pop_10s` grow/shrink; verified 1:1 against `ovr142/All_Outpost_Population_Growth.asm`. R1 (inverted outpost test), R3 (16-bit grow multiply → int32_t), R4 (`Random(3)`→`Random(2)` shrink) fixed; R2 (terrain scan indexes `[itr_cities]`) restored as a preserved OGBUG this session.
  - Downstream helper: [`City_Food_Terrain`](../MoM/src/CITYCALC.c#L1643) — **done-done**, doc [NextTurn/CITYCALC-City_Food_Terrain.md](NextTurn/CITYCALC-City_Food_Terrain.md). Terrain food yield of a city's work area (Σ `Square_Food_x2` × Gaia / 4 = base max city size); verified 1:1 against `ovr142/City_Food_Terrain.asm`, no divergences. Doc explains the `/ 4` (quarter-food `Square_Food_x2` → real catchment food).
    - Downstream helper: [`Get_Useable_City_Area`](../MoM/src/CITYCALC.c#L1740) — **done-done**, doc [NextTurn/CITYCALC-Get_Useable_City_Area.md](NextTurn/CITYCALC-Get_Useable_City_Area.md). Corner-trimmed 5×5 city catchment, corruption-filtered; verified 1:1 against `ovr142/Get_Useable_City_Area.asm`. R1 (east-edge wrap `>`→`>=`) fixed + stale Doxygen `@warning` corrected this session.
    - Downstream helper: [`Square_Food_x2`](../MoM/src/Terrain.c#L70) (was `Square_Food2`; on-disk OG `Map_Square_Food2`) — **done-done**, doc [NextTurn/Terrain-Square_Food_x2.md](NextTurn/Terrain-Square_Food_x2.md). Per-tile food in quarter-food units (halved when shared); verified 1:1 against `ovr161/Map_Square_Food2.asm`, no divergences. Renamed + flattened this session (flatten made the range-0 early-returns match the asm).
  - Downstream helper: [`City_Food_WildGame`](../MoM/src/CITYCALC.c#L1663) — **done-done**, doc [NextTurn/CITYCALC-City_Food_WildGame.md](NextTurn/CITYCALC-City_Food_WildGame.md). Wild-game terrain-special food (2 food owned / 1 shared per `TS_WILDGAME` catchment square); verified 1:1 against `ovr142/City_Food_WildGame.asm`, no divergences (no Gemini `.c` — asm-only reference). Sibling of `City_Food_Terrain` under `City_Maximum_Size`; no Gaia's Blessing boost (faithful).
  - Downstream helper: [`City_Maximum_Size`](../MoM/src/CITYCALC.c#L1912) — **done-done**, doc [NextTurn/CITYCALC-City_Maximum_Size.md](NextTurn/CITYCALC-City_Maximum_Size.md). Uncapped max city population = `City_Food_Terrain` base → ½ under Famine → +2 Granary → +3 Farmers' Market → + `City_Food_WildGame` (the parent that calls both food helpers above; itself reached from `City_Growth_Rate` / `Compute_Base_Values_For_Map_Square`); verified 1:1 against `ovr142/City_Maximum_Size.asm`, no divergences. Preserved `// BUGBUG  ¿ no FORESTERS_GUILD ?` DEDU (`0x1F` unchecked; asm also skips it — faithful).
- [x] **9B** [`Apply_City_Changes`](../MoM/src/NEXTTURN.c#L2901) — **done-done**, doc [NextTurn/NEXTTURN-Apply_City_Changes.md](NextTurn/NEXTTURN-Apply_City_Changes.md). Per-turn outpost/city population maintenance; verified 1:1 against `ovr140/Apply_City_Changes.asm`. R1 (`Pop_10s` overwrite → accumulate) fixed this session.
  - Downstream helper: [`City_Apply_Production`](../MoM/src/NEXTTURN.c#L2159) — **done-done**, doc [NextTurn/NEXTTURN-City_Apply_Production.md](NextTurn/NEXTTURN-City_Apply_Production.md). Per-city production advance/completion; verified 1:1 against `ovr140/City_Apply_Production.asm`. Building-completion bookkeeping restructure (nested → unconditional) fixed this session.
    - Downstream helper: [`Evict_Weakest_Unit`](../MoM/src/CITYCALC.c#L1441) (was `UNIT_RemoveExcess`) — **done-done**, doc [NextTurn/CITYCALC-Evict_Weakest_Unit.md](NextTurn/CITYCALC-Evict_Weakest_Unit.md). Post-spawn stack-cap trim (evicts the least-valuable unit); verified 1:1 against `ovr120/UNIT_RemoveExcess.asm`. Renamed this session; general helper (~13 call sites, not just City production).
      - Downstream helper: [`Evict_Unit_With_Message`](../MoM/src/CITYCALC.c#L1376) (was `UNIT_LoggedPushOff`) — **done-done**, doc [NextTurn/CITYCALC-Evict_Unit_With_Message.md](NextTurn/CITYCALC-Evict_Unit_With_Message.md). Push-off wrapper that logs a human unit-loss message when the push is fatal; verified 1:1 against `ovr120/UNIT_LoggedPushOff.asm`. Renamed this session; R1 (missing `msg_unit_lost_ctr++`) fixed this session.
        - Downstream helper: [`Evict_Unit`](../MoM/src/SETTLE.c#L446) (was `UNIT_PushOffTile`) — **done-done**, doc [NextTurn/SETTLE-Evict_Unit.md](NextTurn/SETTLE-Evict_Unit.md). Relocates a unit to a free square in the 3×3 neighborhood or kills it; verified 1:1 against `ovr100/UNIT_PushOffTile.asm`. Renamed this session; R1 (`<=`→`<` 3×3 scan) and R2 (`&&`→`||` combat-square skip) fixed this session.
  - Downstream helper: [`City_Growth_Rate`](../MoM/src/CITYCALC.c#L2573) — **done-done**, doc [NextTurn/CITYCALC-City_Growth_Rate.md](NextTurn/CITYCALC-City_Growth_Rate.md). Per-turn city population growth rate (food-headroom base + Granary/Farmers' + Stream-of-Life/Population-Boom ×2 + Housing/Dark-Rituals/Sawmill/Builders' percentage modifier, then neutral/AI-difficulty scaling and 0-floor); calls [`City_Maximum_Size`](../MoM/src/CITYCALC.c#L1912) (the 9A cluster above). Verified 1:1 against `ovr142/City_Growth_Rate.asm`. R1 (modifier/owner/floor block wrongly nested inside the Housing `if`, so non-Housing cities skipped the difficulty scaling + 0-floor — moved to the shared post-Housing block that the asm's `loc_C8B28` convergence reaches, with all early-exits using `goto Done`) and R2 (modifier formula `(rate·mod + 100)/100` → `rate·(mod + 100)/100`) fixed this session. Preserved drake178 BUG #1/#2/#3 (max-pop growth-suppression OGBUGs).
- [ ] **9C** [`All_City_Nightshade_Count`](../MoM/src/Terrain.c#L1625) — recounts nightshade tiles per city (affects Life realm bonuses).
- [ ] **9D** [`All_City_Removed_Buildings`](../MoM/src/City_ovr55.c#L2284) — sweeps `bldg_status[]` for state changes and applies effects.

### Wave 10 — Diplomacy cluster
NPC diplomatic actions, contact discovery, treaty adjustments. Most functions carry `__WIP` / `__STUB` suffixes.

- [x] **10A** [`Diplomacy_Growth_For_Enchantments`](../MoM/src/NEXTTURN.c#L4030) — **done-done** (`__WIP` dropped), doc [NextTurn/NEXTTURN-Diplomacy_Growth_For_Enchantments.md](NextTurn/NEXTTURN-Diplomacy_Growth_For_Enchantments.md). Per-turn, per-player: reset casting-cost cache + `Change_Relations_For_Enchantments` for each of 22 active global enchantments (weight `10`, or `2` for Spell of Mastery), with `WIZ_GreatWasting`/`WIZ_MeteorStorm`/`WIZ_Armageddon` map effects first. Verified 1:1 against `ovr140/Diplomacy_Growth_For_Enchantments__WIP.asm`. This session: fixed a compile-blocking `]` typo (Wind Mastery block), dropped the IDA `j_` prefix off the three `WIZ_*` thunk calls, and those callees are now reconstructed ([Spells132.c](../MoM/src/Spells132.c)); builds clean.
- [x] **10B** [`Determine_Offer`](../MoM/src/NEXTTURN.c#L3359) — **done-done**, doc [NextTurn/NEXTTURN-Determine_Offer.md](NextTurn/NEXTTURN-Determine_Offer.md). Per-player wandering-merchant / mercenary / hero-for-hire offers (human popups, AI auto-resolve); verified 1:1 against `ovr140/Determine_Offer.asm`, no divergences. Preserved OGBUGs: B1 (`player_fame` never per-player), B2 (item affordability tested before Charismatic discount). `IDK`→`offer_chance_pct` + locals lowercased this session.
- [x] **10C** [`Diplomacy_Growth`](../MoM/src/DIPLOMAC.c#L2652) — **done-done**, doc [NextTurn/DIPLOMAC-Diplomacy_Growth.md](NextTurn/DIPLOMAC-Diplomacy_Growth.md). Per-turn wizard-relations drift (pact/alliance goodwill, human military-threat + overextension penalties, gravitate-to-default, symmetrize); verified 1:1 against `ovr085/Diplomacy_Growth.asm`. R1 (overextension `*= 2`→`*= 4`) and R2 (16-bit `too_strong_treshold` → int32_t) fixed; locals lowercased + `Dipl_182h_Field`→`default_relations` this session.
  - Downstream helper: [`Change_Relations`](../MoM/src/DIPLOMAC.c#L3344) (was `Change_Relations__WIP`; on-disk OG `Change_Relations__WIP`) — **done-done**, doc [NextTurn/DIPLOMAC-Change_Relations.md](NextTurn/DIPLOMAC-Change_Relations.md). Central diplomatic-relation applier (reciprocation/personality/charisma scaling, war & overextension, side modifiers, hostility); verified 1:1 against `ovr085/Change_Relations__WIP.asm`. R1 (symmetrize wrote `Dipl_Status`, should be `Visible_Rel`) fixed; `__WIP` dropped + locals renamed (`value`→`relation_value`, `NoCharisma_RelValue`→`relation_value_pre_charisma`, `Rel_Divisor`→`diminishing_returns_divisor`) this session. Preserved OGBUG: Earth-Lore→Declare_War (should be Spell_Of_Mastery).
    - [x] [`Adjust_Diplomat_Modifiers`](../MoM/src/DIPLOMAC.c#L3369) — **done-done** (no doc, per request). Shared diplomacy leaf helper (~15 call sites across `DIPLOMAC.c`): subtracts `10` from `treaty_modifier`/`exchange_modifier`/`peace_modifier[player2]` under `_players[player1].Dipl`. Verified 1:1 against `ovr085/Adjust_Diplomat_Modifiers.asm` — trivial, no divergences.
- [x] **10D** [`Determine_First_Contacts`](../MoM/src/DIPLOMAC.c#L5598) — **done-done** (no doc, too small). For each wizard (itr from 1), if the human's first-contact handshake is unstarted (`contact_stage[itr] == 0`, was `Contact_Progress`) but `Contacted[itr] == ST_TRUE`, advance it to `1` and set mutual `Dipl_Status` to `DIPL_NoTreaty`. Verified 1:1 against `ovr087/Determine_First_Contacts.asm` — no divergences. Faithful: `itr=1` start (asm:7). `Contact_Progress` renamed `contact_stage` (0=unmet / 1=met, intro pending / 2=introduced).
- [x] **10E** [`NPC_To_NPC_Treaty_Negotiations`](../MoM/src/DIPLOMAC.c#L4837) — **done-done**, doc [NextTurn/DIPLOMAC-NPC_To_NPC_Treaty_Negotiations.md](NextTurn/DIPLOMAC-NPC_To_NPC_Treaty_Negotiations.md). Per-turn AI-vs-AI (NPC-to-NPC) diplomacy: treaty-negotiation nest (score → alliance/pact/spell-exchange, goodwill roll, war→peace, post-passes) + ally-drags-you-to-war nest; verified 1:1 against `ovr086/NPC_To_NPC_Treaty_Negotiations__STUB.asm`, both loop nests' bounds checked. `__STUB` dropped (body fully reconstructed), stale `Change_Relations__WIP` call updated to `Change_Relations`; wired live at [NEXTTURN.c:772](../MoM/src/NEXTTURN.c#L772). Preserved OGBUG: human `spl_Spell_Of_Return` gates the AI-vs-AI pass.
  - New callees added + committed this session, **quick error-scan only — full 1:1 review still pending**: [`G_DIPL_PickSides`](../MoM/src/DIPLOMAC.c#L3240), [`G_DIPL_NeedForWar`](../MoM/src/DIPLOMAC.c#L5755) (+ [`G_DIPL_SuperiorityWar`](../MoM/src/DIPLOMAC.c#L5902)), [`DIPL_GetOffMyLawn`](../MoM/src/DIPLOMAC.c#L5995). Build clean (Debug, MSVC), no obvious inversions.
- [x] **10F** [`NPC_To_Human_Diplomacy`](../MoM/src/DIPLOMAC.c#L5619) — **done-done** (`__WIP` dropped), doc [NextTurn/DIPLOMAC-NPC_To_Human_Diplomacy.md](NextTurn/DIPLOMAC-NPC_To_Human_Diplomacy.md). Per-turn, per-AI-wizard: decide the diplomatic action toward the human (intro for new contacts, pending bounty, or relation-score gate → war/peace/alliance/demand), with `G_DIPL_NeedForWar`/`DIPL_GetOffMyLawn` fallbacks. PRNG-critical (`Random(75/100/4)`). Verified 1:1 against `ovr087/NPC_To_Human_Diplomacy__WIP.asm`. This session fixed R1 (inactive-fortress branch must `continue`, skipping the fallbacks) and R2 (`da_strength` missing its `× 2` on `DA_Strength` — a PRNG-stream bug); callees all reconstructed, magic action codes named `do_*`, builds clean.
- [x] **10G** [`Resolve_Delayed_Diplomacy_Orders`](../MoM/src/DIPLOMAC.c#L6033) — **done-done** (no doc, too small). Unless the human is casting `spl_Spell_Of_Return`, pops each pending per-wizard `Npc_Diplomacy_Screen()` for `_players[HUMAN].Dipl.Dipl_Action[itr] != do_None` (itr from 1), then clears each to `do_None`. Verified 1:1 against `ovr087/Resolve_Delayed_Diplomacy_Orders.asm` — no divergences. Faithful: `itr=1` start (asm:18), the Spell-of-Return gate, and the human-only (`_players[0]`) Dipl_Action indexing.
- [x] **10H** [`End_Of_Turn_Diplomacy_Adjustments`](../MoM/src/DIPLOMAC.c#L6181) — **done-done** (no doc, per request). Per-turn drift of the three diplomatic modifiers back up toward 100: for each ordered player pair `itr1 != itr2`, `treaty`/`exchange`/`peace_modifier[itr2] += 10` when `< 100` (mirror of `Adjust_Diplomat_Modifiers`'s `-= 10`). Verified 1:1 against `ovr087/End_Of_Turn_Diplomacy_Adjustments.asm` — no divergences. Faithful quirk: `< 100 → += 10` overshoots (no clamp to 100), as in the asm.
- [x] **10I** [`Modifier_Diplomacy_Adjustments`](../MoM/src/DIPLOMAC.c#L6248) — **done-done**, doc [NextTurn/DIPLOMAC-Modifier_Diplomacy_Adjustments.md](NextTurn/DIPLOMAC-Modifier_Diplomacy_Adjustments.md). Two per-turn nests: modifier recovery (`Random(5)` bumps toward 0/50 per non-Crusade pair + peace clamp `[-200,200]`) and delayed-diplomacy-memory reset. Verified 1:1 against `ovr088/Modifier_Diplomacy_Adjustments.asm`. Faithful quirks: `itr2=1` first-nest start (asm:17), write-only `IDK_Dipl_Action`. The Nest-2 `field_120`/`field_6C` reset (renamed `niu_bounty_collect`/`niu_attack_bounty`) is deprecated MoO1 attack-bounty carryover — NIU in MoM; the earlier `e_1oom_PLAYER_NONE` DEDU is resolved (last-player enum ≈ `MAX_PLAYERS`, immaterial on dead fields).

### Wave 11 — Spell casting completion
Runs after normal/time-stop-else block. Applies all queued spell casts for all players.

- [ ] **11A** [`All_Players_Apply_Spell_Casting`](../MoM/src/NEXTTURN.c#L3206) — iterates players and applies any queued overland spell that finished accumulating mana this turn.
- [x] **11B** Human `Cast_Spell_Overland(_human_player_idx)` call inside `Next_Turn_Proc` ([NEXTTURN.c:403](../MoM/src/NEXTTURN.c#L403)) — **done-done** (commit `9cd8113d`, cross-ref [`__TODO-AiTurn.md`](__TODO-AiTurn.md) Wave 4A). Human-side casting completion; AI-side is in `AI_Next_Turn`.

### Wave 12 — End-of-turn unit passes
XP, healing, history logging.

- [x] **12A** [`Do_All_Units_XP_Check`](../MoM/src/NEXTTURN.c#L4370) — **done-done**, doc [NextTurn/NEXTTURN-Do_All_Units_XP_Check.md](NextTurn/NEXTTURN-Do_All_Units_XP_Check.md). Per-turn unit XP/Level + Herb Mastery heal + Stasis countdown + Armsmaster stack bonus; verified 1:1 against `ovr140/Do_All_Units_XP_Check.asm`. R1 (Time-Stop owner test dropped the `- 1` on 1-based `g_timestop_player_num`) and R2 (`_HEROES2[HUMAN_PLAYER_IDX]` → per-player `_HEROES2[itr_players]`) fixed this session; `processed_hero_list` made an offset-backed array so the OG's unconditional `[-1]` write no longer trips MSVC `/RTCs`.
- [x] **12B** [`Heal_All_Units`](../MoM/src/NEXTTURN.c#L3875) — **done-done**, doc [NextTurn/NEXTTURN-Heal_All_Units.md](NextTurn/NEXTTURN-Heal_All_Units.md). Per-turn healing: per-unit divisor table (20/10/6 = field/city/Animist's), location-based regen, and the `UA_HEALER` flat-20% stack bonus. Verified 1:1 against `ovr140/Heal_All_Units.asm`. This session fixed R1 (Time-Stop owner test dropped the 1-based `- 1`), R2 (healer-stack filter indexed `_UNITS[itr_troops]` instead of `_UNITS[troops[itr_troops]]`), and R3 (final bonus-heal loop healed the healer via the RNG-consuming `ST_FALSE` branch instead of `Heal_Unit(troops[i], 0, ST_TRUE)` — a PRNG-stream bug); the `/* HACK */ owner_idx == ST_UNDEFINED` guard was dropped to restore 1:1. Builds clean.
  - [x] **12B.1** [`Heal_Unit`](../MoM/src/NEXTTURN.c#L3834) (o140p22, asm `G_UNIT_OvlHealing`) — **done-done**, callee of `Heal_All_Units`. Applies the per-unit heal: `flag == ST_FALSE` = location-based regen (`while(hits_total > fraction)` + `Random(fraction)` remainder, consumes RNG), `flag == ST_TRUE` = flat 20% (`hits_total / 5`) that stamps `NATURAL_HEALER_ALREADY_APPLIED`. Verified 1:1 against `ovr140/G_UNIT_OvlHealing.asm`. Both drake178 signed-byte "damage over 127" OGBUGs preserved.
- [x] **12C** [`Record_History`](../MoM/src/CITYCALC.c#L3346) — **done-done**, doc [NextTurn/CITYCALC-Record_History.md](NextTurn/CITYCALC-Record_History.md). Computes each wizard's power graph value and appends to `history[288]`; verified 1:1 against `ovr142/Record_History.asm`. R1 (scaling operand swap + 32-bit) and R2 (history scroll direction) fixed this session. The four astrologer input getters (`Astrologer_Power`, `Astrologer_Strength`, `Astrologer_Research`, `Empire_Population_Points`) reconstructed from `__STUB`s and verified against `ovr142` asm this session.

### Wave 13 — Persistence
Autosave and RNG-seed roundtrip. Runs at the tail of `Next_Turn_Calc`.

- [ ] **13A** `_turn++` — simple counter bump; verify placement matches OG (before or after gold/mana clamp?).
- [ ] **13B** Per-player `gold_reserve` / `mana_reserve` clamp to `MAX_GOLD_RESERVE` / `MAX_MANA_RESERVE` via `SETMAX` ([NEXTTURN.c:832-839](../MoM/src/NEXTTURN.c#L832-L839)).
- [ ] **13C** [`Do_Autosave`](../MoM/src/NEXTTURN.c#L4919) — writes the auto-save slot at end of turn.
- [ ] **13D** `RNG_AI_Turn_Seed = Get_Random_Seed()` — save the advanced RNG state for next turn's seed.

### Capstone
- [ ] **Final** [`Next_Turn_Proc`](../MoM/src/NEXTTURN.c#L264) + [`Next_Turn_Calc`](../MoM/src/NEXTTURN.c#L609) — parent walkthroughs. Tie the whole chain together once all downstream is done-done. Split into two docs (one per function) or one combined doc — TBD.

---

## Review — what's downstream of `Next_Turn_Proc`

### Call chain overview

```
Next_Turn_Proc()  [NEXTTURN.c:264]
├── Delete_Dead_Units()                   Wave 1A
├── All_Units_In_Towers()                 Wave 1B
├── Set_Unit_Draw_Priority()              Wave 2A
├── Set_Entities_On_Map_Window()          Wave 2A
├── Reset_Map_Draw()                      Wave 2B
├── Next_Turn_Calc()                      Wave 4-13 (contains everything)
│   ├── Set_Random_Seed(RNG_AI_Turn_Seed) Wave 4A
│   ├── All_City_Calculations()           Wave 4B
│   ├── AI_Kill_Lame_Units()              Wave 4C
│   ├── Delete_Dead_Units()               Wave 1A
│   ├── AI_Next_Turn()                    Wave 5 (done, __TODO-AiTurn.md)
│   ├── Delete_Dead_Units()               Wave 1A
│   ├── Next_Turn_Process_Purify()        Wave 6A
│   ├── Initialize_Reports()              Wave 6B
│   │
│   ├── IF g_timestop_player_num != 0:
│   │   └── Time Stop mana drain          Wave 7A
│   │
│   ├── ELSE (normal end-of-turn):
│   │   ├── Decrease_Peace_Duration()     Wave 7B
│   │   ├── Update_Players_Gold_Reserve() Wave 7C
│   │   ├── Players_Update_Magic_Power()  Wave 7D (done)
│   │   ├── Players_Apply_Magic_Power()   Wave 7E
│   │   ├── Players_Check_Spell_Research()Wave 7F
│   │   ├── OVL_DisableIncmBlink()        Wave 7H
│   │   ├── Determine_Event() (gated)     Wave 8A
│   │   ├── Event_Twiddle()               Wave 8B
│   │   ├── Players_Apply_Upkeeps__WIP()  Wave 7G
│   │   ├── EMMDATAH_Map()                (verified 1:1, no walkthrough)
│   │   ├── All_Outpost_Population_Growth()  Wave 9A
│   │   ├── Apply_City_Changes()          Wave 9B
│   │   ├── Diplomacy_Growth_For_Enchantments()       Wave 10A
│   │   ├── [Grand Vizier — TODO]
│   │   ├── Determine_Offer()             Wave 10B
│   │   ├── All_City_Nightshade_Count()   Wave 9C
│   │   ├── Diplomacy_Growth()            Wave 10C
│   │   ├── Determine_First_Contacts()    Wave 10D
│   │   ├── NPC_To_NPC_Treaty_Negotiations()  Wave 10E ✓
│   │   ├── NPC_To_Human_Diplomacy() Wave 10F
│   │   ├── Resolve_Delayed_Diplomacy_Orders()  Wave 10G
│   │   ├── End_Of_Turn_Diplomacy_Adjustments() Wave 10H
│   │   ├── Modifier_Diplomacy_Adjustments()    Wave 10I
│   │   └── Cool_Off_Volcanoes()          Wave 8C
│   │
│   ├── All_Players_Apply_Spell_Casting() Wave 11A
│   ├── Delete_Dead_Units()               Wave 1A
│   ├── Set_Unit_Draw_Priority()          Wave 2A
│   ├── Set_Entities_On_Map_Window()      Wave 2A
│   ├── All_City_Removed_Buildings()      Wave 9D
│   ├── Do_All_Units_XP_Check_()          Wave 12A
│   ├── Heal_All_Units()                  Wave 12B
│   ├── Record_History()                  Wave 12C
│   ├── Increment_Background_Music()      Wave 2D
│   ├── _turn++                           Wave 13A
│   ├── gold_reserve / mana_reserve clamp Wave 13B
│   ├── OVL_EnableIncmBlink()             Wave 7H
│   ├── Do_Autosave()                     Wave 13C
│   ├── All_City_Calculations()           Wave 4B
│   └── RNG_AI_Turn_Seed = Get_Random_Seed()  Wave 13D
│
├── Cache_Graphics_Overland()             Wave 2B
├── Delete_Dead_Units()                   Wave 1A
├── All_Units_In_Towers()                 Wave 1B
├── Chancellor_Screen__WIP(0) (gated)     Wave 3A
├── Building-complete message loop        Wave 3B
├── Update_Units_MvsSts()                 Wave 1C
├── Cast_Spell_Overland(_human)           Wave 11B (done via __TODO-AiTurn.md)
├── WIZ_NextIdleStack()                   Wave 3C
└── Reset_Draw_Active_Stack()             Wave 2C
```

### Known `__WIP` / `__STUB` items requiring reconstruction

| Function | Suffix | Wave | Notes |
|---|---|---|---|
| `Chancellor_Screen__WIP` | `__WIP` | 3A | End-of-turn Chancellor summary. |
| `City_Screen__WIP` | `__WIP` | 3B | Per-city popup for building-complete messages. |
| `Players_Apply_Upkeeps__WIP` | `__WIP` | 7G | Gold/mana upkeep application. |
| `Diplomacy_Growth_For_Enchantments` | done | 10A | Enchantment-based diplomacy adjustments. Reconstructed & verified 1:1. |
| `NPC_To_Human_Diplomacy` | `__WIP` | 10F | AI-to-human diplomatic actions. |

### Out-of-scope / infrastructure notes

- Draw-state functions (Wave 2) are infrastructure — visual behavior can be verified in a running build without OG-asm 1:1 comparison. Consider lower-priority.
- `Increment_Background_Music` (Wave 2D) is sound-hardware plumbing — unlikely to have gameplay implications.
- `Do_Autosave` (Wave 13C) writes to disk. Save-file format has its own separate review track.
- ReMoM-added STU_LOG / STU_DEBUG blocks throughout `Next_Turn_Calc` (LOG_INFO, LOG_TRACE, PHASE macro) — instrumentation only, not OG.
- Grand Vizier stub at [NEXTTURN.c:733-736](../MoM/src/NEXTTURN.c#L733-L736) — placeholder for the auto-build-in-all-cities feature; TODO'd.

---

## Working conventions (same as AI turn plan)

- **R-numbered fixes**: reconstruction-error deltas found during walkthrough. Get flagged, fixed by the user, and after fix the R section is removed from the doc entirely.
- **B-numbered bugs**: preserved OG bugs (faithful-to-Dasm). Documented in the walkthrough but do NOT get fixed.
- **`__WIP` / `__STUB` suffixes**: mark functions whose reconstruction is incomplete. Removing the suffix is the concrete "done-done" trigger.
- **Cross-references**: any function already covered by another plan (e.g., `AI_Next_Turn` via `__TODO-AiTurn.md`) is linked, not re-reviewed.
- **Preserved comments**: OG inline comments (BUG, OGBUG, DEDU, etc.) stay in production source verbatim.
- **Wave X** terminology: internal to this plan. Do NOT use "Wave" numbering in per-function walkthrough docs — those should stand on their own.
