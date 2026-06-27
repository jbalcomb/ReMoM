# AI_Next_Turn — Downstream Done-Done Plan

Tracking the per-turn review/cleanup work for every function reachable from [`AI_Next_Turn`](../MoM/src/AIDUDES.c) ([AIDUDES.c:157-393](../MoM/src/AIDUDES.c#L157-L393)).

---

## Todolist

### Wave 1 — Settle-sibling per-landmass deciders
- [ ] **1A** Review [`AI_Stacks_Do_Purify`](../MoM/src/AIMOVE.c#L4309) — OG asm verification, bug catalog, walkthrough doc, name-sweep audit
- [ ] **1B** Review [`AI_Stacks_Do_RoadBuild`](../MoM/src/AIMOVE.c#L4444) — same pattern as Purify
- [ ] **1C** Review `AI_Order_Purify` + `AI_Order_RoadBuild` leaf order-issuers

### Wave 2 — Audit unknown-status `AI_Set_Unit_Orders` slots
- [ ] **2A** Audit slot 1 (`AI_Disband_To_Balance_Budget`) — location, status, OG verification
- [ ] **2B** Audit slot 2 (`AI_Shift_Off_Home_Plane`)
- [ ] **2C** Audit slot 3 (`AI_Move_Out_Boats` + `AI_Stack_Set_Boats_Goto`)
- [ ] **2D** Audit slot 9 (`AI_Find_Opportunity_City_Target`)

### Wave 3 — Pre-orders setup phase (`AI_Next_Turn` per-AI loop body)
- [ ] **3A** [`AI_Evaluation_Map`](../MoM/src/AIMOVE.c#L6809) — populates `g_ai_evaluation_map`; foundational for downstream
- [ ] **3B** [`AI_Evaluate_Continents`](../MoM/src/AIMOVE.c#L6283) — populates `_ai_continents` and `_ai_landmass_settler_targets`
- [ ] **3C** [`AI_Choose_War_Landmass`](../MoM/src/AIMOVE.c#L7033) — extend the prior `lmt_NoTargets` analysis to full code-walk
- [ ] **3D** `AI_Evaluate_Hostility` + `AI_Evaluate_Magic_Power_Strategy` + `Player_Hostile_Opponents` cluster
- [ ] **3E** `AI_Player_Calculate_Target_Values` + `AI_Landmass_Values_And_Strengths` cluster
- [ ] **3F** `AI_Update_Magic_Power` + `AI_Update_Gold_And_Mana_Reserves` + `AI_Update_Gold_Income_And_Food_Income` + `AI_Sanity_Check_Overland_Enchantments` cluster
- [ ] **3G** [`Player_All_Colony_Autobuild`](../MoM/src/AIBUILD.c#L45) — colony auto-builder
- [ ] **3H** `AI_Kill_Excess_Settlers_And_Engineers` — exit-phase function

### Wave 4 — Overland Spell-AI Subsystem
The overland spell-casting AI is its own coherent subsystem. Triggered from [AIDUDES.c:261-272](../MoM/src/AIDUDES.c#L261-L272) when the AI's queued spell finishes accumulating mana.

- [x] **4A** [`Cast_Spell_Overland`](../MoM/src/OverSpel.c#L627) — **done-done 2026-06-10**, see [SETTLE-Cast_Spell_Overland.md](ComputerPlayer/SETTLE-Cast_Spell_Overland.md). Completes the in-progress overland spell cast (lines 627-1956, 1330 lines). 9-entry bug catalog (5× `OGBUG no return value` cluster, OG-faithful preserved; 3× uncertain query markers; 1× confirmed parameter-missing OGBUG with documented workaround). Nature's Wrath retaliation wired via `Call_Forth_The_Force_Of_Nature(player_idx)` at [OverSpel.c:1944](../MoM/src/OverSpel.c#L1944). Call site at [AIDUDES.c:264](../MoM/src/AIDUDES.c#L264) wrapped in `PHASE()`. `Cast_Successful` → `cast_can_continue`, `AI_Eval_After_Spell` → `g_ai_recompute_needed` renames complete. `__WIP` suffix dropped; GEMINI variant deleted.
- [x] **4B** [`AI_Spell_Select`](../MoM/src/AISPELL.c#L309) — **done-done 2026-06-11**, see [AISPELL-AI_Spell_Select.md](ComputerPlayer/AISPELL-AI_Spell_Select.md). Dispatcher complete (lines 309-378, plus Doxygen header at 287-308). `__STUB` suffix dropped; variable casing normalized to lower_snake_case (`Selected_Category` → `spell_category`); GEMINI variant deleted; Dasm-divergence OGBUG flagged at [line 320](../MoM/src/AISPELL.c#L320). Call site at [AIDUDES.c:278](../MoM/src/AIDUDES.c#L278) wrapped in `PHASE()`. The 9 `AI_OVL_*` leaf pickers are now `return 0;` stubs at [AISPELL.c:382](../MoM/src/AISPELL.c#L382)/471/477/483/489/495/501/519/525 — build clean. Runtime effect: AI never starts an overland cast until the leaves are reconstructed (Wave 4C/4D/4E).
- [ ] **4C** Target-picker cluster: [`Pick_Target_For_City_Enchantment__WIP`](../MoM/src/AISPELL.c#L795), [`IDK_Pick_Target_For_Unit_Enchantment__STUB`](../MoM/src/AISPELL.c#L1022), [`IDK_AITP_Target_Wizard__STUB`](../MoM/src/AISPELL.c#L1347), [`AITP_Disjunction`](../MoM/src/AISPELL.c#L1041) — pick who/what to cast on
- [ ] **4D** Specific-spell-targeting cluster: [`AITP_ChangeTerrain__WIP`](../MoM/src/AISPELL.c#L550), [`AITP_SpellWard__STUB`](../MoM/src/AISPELL.c#L632), [`AITP_SpellWard_Wrapper__STUB`](../MoM/src/AISPELL.c#L1357)
- [ ] **4E** Spell-AI helpers: [`IDK_Get_Global_Enchant_Index__STUB`](../MoM/src/OverSpel.c#L2274)

### Wave 5 — NPC + event phase
- [ ] **5A** [`NPC_Farmers`](../MoM/src/AIDATA.c) + [`NPC_Destinations`](../MoM/src/AIDATA.c#L581) cluster
- [ ] **5B** [`Make_Raiders`](../MoM/src/AIDATA.c#L353) + [`Make_Monsters`](../MoM/src/AIDATA.c#L124) — event-generation cluster
- [ ] **5C** [`NPC_Excess_Garrison`](../MoM/src/AIDATA.c#L53) + `AI_Hopeless_Stasis` — cleanup cluster

### Wave 6 — Helpers / plumbing
- [ ] **6A** `EMMDATAH_Map` + [`EMM_Map_CONTXXX__WIP`](../MoM/src/AIDUDES.c#L2207) — EMS paging helpers
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
│           Spell_Select__STUB, Update_Magic_Power, Sanity_Check_Overland_Enchantments,
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

## Status snapshot (as of 2026-06-04)

### Done-done already (from prior sessions)

- `AI_Execute_Orders` + all 5 per-unit Status-handlers (`AI_Unit_Army_Do_Road`/`Meld`/`Settle`/`Move`/`Ferry`)
- `AI_Stacks_Do_Meld` AoC
- `AI_Stacks_Do_Settle` AoC (incl. `AI_Stacks_Reorder_Settle_Elsewhere`, `AI_Find_Tower_To_Settle_Elsewhere`, `Active_Army_Do_Settle`, `Army_Do_Settle`, `Unit_Army_Do_Settle`)
- Slot 8: `AI_Stacks_Init_Build_Target_Order`
- Slot 10: `AI_Build_Target_List` (+ `AI_Add_Target`, `AI_Stacks_Assign_Target`)
- Slot 11: `AI_Stacks_Move_Out_NonMilitary_Garrisoned`
- Slot 12: `AI_Stacks_Survey_Expedition_Forces` (+ `_Stack`)
- Slot 13: `AI_Stacks_Garrison_Sites`
- Both per-plane post-passes: `AI_Stacks_Wartime_Ocean_Movement_And_Cleanup`, `AI_Stacks_Ocean_Landmass_Orders`
- Continent helper: `AI_Reevaluate_Continent`
- Roamer paths: `AI_Stacks_Roamers_Target_Or_Deploy`, `AI_Stacks_Order_To_War_Landmass`, `AI_Stacks_Relocate_Roamers`, `AI_Stacks_Stage_Expedition_Forces`
- Ferry paths: `AI_Stacks_Setup_Ferry`, `AI_Stacks_Ferry_Add_Location`, `AI_Stacks_Order_Ferry`

### Suffix-tagged functions still hanging (`__WIP`/`__STUB`/`__GEMINI`/`__ALWAYS_FALSE` grep)

- **AIMOVE.c**: 0 ✓ (cleaned up over the Settle/Meld AoC reviews)
- **AIDUDES.c**: [`EMM_Map_CONTXXX__WIP`](../MoM/src/AIDUDES.c#L2207) (helper, low-impact)
- **AISPELL.c + OverSpel.c**: scoped into **Wave 4** (overland spell-AI subsystem) — `Cast_Spell_Overland`, `AI_Spell_Select`, `AITP_ChangeTerrain__WIP`, `AITP_SpellWard__STUB`, `Pick_Target_For_City_Enchantment__WIP`, `IDK_Pick_Target_For_Unit_Enchantment__STUB`, `AITP_Disjunction`, `IDK_AITP_Target_Wizard__STUB`, `AITP_SpellWard_Wrapper__STUB`, `IDK_Get_Global_Enchant_Index__STUB`
- **CityScr.c**, **CITYSCAP.c**, **AdvsrScr.h**: out of scope for AI_Next_Turn

### Naming convention completed

- `AI_Stacks_Do_Purify` ([AIMOVE.c:4309](../MoM/src/AIMOVE.c#L4309)) — renamed from `AI_Do_Purify`
- `AI_Stacks_Do_RoadBuild` ([AIMOVE.c:4444](../MoM/src/AIMOVE.c#L4444)) — renamed from `AI_Do_RoadBuild`

---

## Plan rationale

**Wave 1** — closes out the remaining per-landmass `AI_Stacks_Do_*` deciders. Mechanically similar to recent Settle/Meld work; likely fast wins.

**Wave 2** — audits the unknown-status `AI_Set_Unit_Orders` slots (1, 2, 3, 9). Verifies they're not stub-rotten before walking through.

**Wave 3** — the big setup-phase functions (15 of them). Most have walkthroughs missing entirely. Biggest payoff (foundational understanding of AI's per-turn data flow).

**Wave 4** — NPC + event phase. Discrete chunk; lower-familiarity territory but well-scoped.

**Wave 5** — helpers / plumbing. Last because they're small and low-risk.

**Capstone** — `AI_Next_Turn` parent walkthrough doc. Done last so it can reference completed sub-walkthroughs.

---

## Recommended starting point

**Wave 1A — `AI_Stacks_Do_Purify`**. Closest to recent context, simplest fallback target after the Settle AoC just wrapped. Same shape: per-(landmass, plane) decider that consumes `_ai_own_stack_*` + leaf `AI_Order_Purify` order-issuer. drake178 OG name `AI_ProcessPurifiers`. Probably finds bugs analogous to the ones we caught in `AI_Stacks_Do_Settle` (gate restructure, naming convention).

---

## Per-function done-done checklist

For each function in the todolist, "done-done" means:

1. **Walkthrough doc** — code walk, mermaid (if multi-branch), bug catalog, history note
2. **OG asm verification** — read Piethawn output, identify reconstruction errors vs OGBUGs
3. **Bug catalog** — B*/R* numbered, severity tagged
4. **GEMINI variant deletion** (if any)
5. **`__WIP` / `__STUB` / `__ALWAYS_FALSE` suffix removal** (if real body exists)
6. **Naming convention sweep** — `Tile` → `Square`, `AI_Do_*` → `AI_Stacks_Do_*` family, `AI_UNIT_*` → `AI_Unit_Army_Do_*` family, etc.
7. **Cross-doc sweep** — index entry in [MoM-AI-AIMOVE-Index.md](ComputerPlayer/MoM-AI-AIMOVE-Index.md), [stub_wip_todo.md](#TODO/stub_wip_todo.md), call-chain docs

---

## Related references

- [doc/ComputerPlayer/](ComputerPlayer/) — per-function walkthrough docs (sister-directory for sub-reviews)
- [doc/MoM-NextTurn-AI-Move.md](MoM-NextTurn-AI-Move.md) — the original chain notes
- [doc/ComputerPlayer/MoM-AI-Move.md](ComputerPlayer/MoM-AI-Move.md) — overview map
- [doc/ComputerPlayer/MoM-AI-AIMOVE-Index.md](ComputerPlayer/MoM-AI-AIMOVE-Index.md) — AIMOVE.c function index
- [doc/#TODO/stub_wip_todo.md](#TODO/stub_wip_todo.md) — running `__STUB`/`__WIP` punchlist
