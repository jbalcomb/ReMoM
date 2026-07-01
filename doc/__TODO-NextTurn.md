# Next_Turn_Proc — Downstream Done-Done Plan

Tracking the per-turn review/cleanup work for every function reachable from [`Next_Turn_Proc`](../MoM/src/NEXTTURN.c#L264) and its main callee [`Next_Turn_Calc`](../MoM/src/NEXTTURN.c#L609). `AI_Next_Turn` and its subtree are covered separately by [`__TODO-AiTurn.md`](__TODO-AiTurn.md) — this plan cross-references but does not re-review that branch.

---

## Todolist

### Wave 1 — Unit lifecycle helpers
Called at multiple points in the outer wrapper and the inner sim entry — used to clean up dead units, refresh movement points, and reset status flags between phases.

- [ ] **1A** [`Delete_Dead_Units`](../MoM/src/NEXTTURN.c#L161) — called 5× per turn (pre-sim, mid-sim after AI turn, post-spell-casting, and 2× in `Next_Turn_Proc` bookends). Cleanup of `_UNITS[]` entries with `owner_idx == ST_UNDEFINED`.
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
- [ ] **7G** [`Players_Apply_Upkeeps__WIP`](../MoM/src/NEXTTURN.c#L2497) — still `__WIP` suffix. Deducts unit/enchantment upkeep from gold/mana.
- [ ] **7H** `OVL_DisableIncmBlink` (pre-turn) + `OVL_EnableIncmBlink` (post-turn) — income-display blink toggles. Sandwich the sim block.

### Wave 8 — Events
Random events fire during normal end-of-turn (skipped during Time Stop).

- [ ] **8A** [`Determine_Event`](../MoM/src/EVENTS.c#L223) — gated on `magic_set.random_events == ST_TRUE AND DBG_Alt_A_State == ST_FALSE`. Picks and triggers a random event.
- [ ] **8B** [`Event_Twiddle`](../MoM/src/EVENTS.c#L884) — per-turn event state advancement (event counters, expiration).
- [ ] **8C** [`Cool_Off_Volcanoes`](../MoM/src/NEXTTURN.c#L4621) — volcano tiles slowly convert back to mountains (event fallout).

### Wave 9 — City-wide passes
Outpost growth, city changes, nightshade counts, and building removal.

- [ ] **9A** [`All_Outpost_Population_Growth`](../MoM/src/CITYCALC.c#L3207) — outposts with sufficient food grow toward "city" status.
- [x] **9B** [`Apply_City_Changes`](../MoM/src/NEXTTURN.c#L2917) — **done-done**, doc [NextTurn/NEXTTURN-Apply_City_Changes.md](NextTurn/NEXTTURN-Apply_City_Changes.md). Per-turn outpost/city population maintenance; verified 1:1 against `ovr140/Apply_City_Changes.asm`. R1 (`Pop_10s` overwrite → accumulate) fixed this session.
- [ ] **9C** [`All_City_Nightshade_Count`](../MoM/src/Terrain.c#L1625) — recounts nightshade tiles per city (affects Life realm bonuses).
- [ ] **9D** [`All_City_Removed_Buildings`](../MoM/src/City_ovr55.c#L2284) — sweeps `bldg_status[]` for state changes and applies effects.

### Wave 10 — Diplomacy cluster
NPC diplomatic actions, contact discovery, treaty adjustments. Most functions carry `__WIP` / `__STUB` suffixes.

- [ ] **10A** [`Diplomacy_Growth_For_Enchantments__WIP`](../MoM/src/NEXTTURN.c#L4050) — still `__WIP`. Adjusts diplomacy based on active overland enchantments.
- [ ] **10B** [`Determine_Offer`](../MoM/src/NEXTTURN.c#L3340) — computes what each AI wizard could/would offer human this turn.
- [ ] **10C** [`Diplomacy_Growth`](../MoM/src/DIPLOMAC.c#L2661) — natural per-turn hostility/friendship drift.
- [ ] **10D** [`Determine_First_Contacts`](../MoM/src/DIPLOMAC.c#L5421) — checks whether any wizard-to-wizard first contact triggered this turn.
- [ ] **10E** `NPC_To_NPC_Treaty_Negotiations__STUB` — still `__STUB`; TODO-commented at [NEXTTURN.c:765](../MoM/src/NEXTTURN.c#L765). Full body not yet reconstructed.
- [ ] **10F** [`NPC_To_Human_Diplomacy__WIP`](../MoM/src/DIPLOMAC.c#L5453) — still `__WIP`. AI-to-human diplomatic action dispatcher.
- [ ] **10G** [`Resolve_Delayed_Diplomacy_Orders`](../MoM/src/DIPLOMAC.c#L5649) — processes queued diplomacy actions from prior turns.
- [ ] **10H** [`End_Of_Turn_Diplomacy_Adjustments`](../MoM/src/DIPLOMAC.c#L5719) — final tweaks to `Hostility[]` / `Personality` / `Contacted[]`.
- [ ] **10I** [`Modifier_Diplomacy_Adjustments`](../MoM/src/DIPLOMAC.c#L5782) — applies modifier-based adjustments (game rules that affect diplomacy).

### Wave 11 — Spell casting completion
Runs after normal/time-stop-else block. Applies all queued spell casts for all players.

- [ ] **11A** [`All_Players_Apply_Spell_Casting`](../MoM/src/NEXTTURN.c#L3206) — iterates players and applies any queued overland spell that finished accumulating mana this turn.
- [x] **11B** Human `Cast_Spell_Overland(_human_player_idx)` call inside `Next_Turn_Proc` ([NEXTTURN.c:403](../MoM/src/NEXTTURN.c#L403)) — **done-done** (commit `9cd8113d`, cross-ref [`__TODO-AiTurn.md`](__TODO-AiTurn.md) Wave 4A). Human-side casting completion; AI-side is in `AI_Next_Turn`.

### Wave 12 — End-of-turn unit passes
XP, healing, history logging.

- [ ] **12A** [`Do_All_Units_XP_Check_`](../MoM/src/NEXTTURN.c#L4400) — trailing underscore in the name (preserved from OG). Iterates units checking XP thresholds → Level bumps.
- [ ] **12B** [`Heal_All_Units`](../MoM/src/NEXTTURN.c#L3890) — per-unit healing (natural + Regeneration + city-based).
- [ ] **12C** [`Record_History`](../MoM/src/CITYCALC.c#L3332) — logs per-turn stats to the history buffer for the graph screen.

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
│   │   ├── Diplomacy_Growth_For_Enchantments__WIP()  Wave 10A
│   │   ├── [Grand Vizier — TODO]
│   │   ├── Determine_Offer()             Wave 10B
│   │   ├── All_City_Nightshade_Count()   Wave 9C
│   │   ├── Diplomacy_Growth()            Wave 10C
│   │   ├── Determine_First_Contacts()    Wave 10D
│   │   ├── [NPC_To_NPC_Treaty_Negotiations__STUB — TODO]  Wave 10E
│   │   ├── NPC_To_Human_Diplomacy__WIP() Wave 10F
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
| `Diplomacy_Growth_For_Enchantments__WIP` | `__WIP` | 10A | Enchantment-based diplomacy adjustments. |
| `NPC_To_Human_Diplomacy__WIP` | `__WIP` | 10F | AI-to-human diplomatic actions. |
| `NPC_To_NPC_Treaty_Negotiations__STUB` | `__STUB` | 10E | Not-yet-reconstructed body (TODO-commented at call site). |

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
