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
  - [ ] `AI_Update_Gold_And_Mana_Reserves` ([AIDUDES.c:1750](../MoM/src/AIDUDES.c#L1750)) — body exists, CamelCase locals, no doc
  - [ ] `AI_Update_Gold_Income_And_Food_Income` ([AIDUDES.c:1921](../MoM/src/AIDUDES.c#L1921)) — body exists, CamelCase locals, no doc
  - [ ] `AI_Sanity_Check_Overland_Enchantments` ([AISPELL.c:4106](../MoM/src/AISPELL.c#L4106)) — body exists, no doc
- [ ] **3G** [`Player_All_Colony_Autobuild`](../MoM/src/AIBUILD.c#L45) — body exists, no doc, no done-done commit
- [ ] **3H** [`AI_Kill_Excess_Settlers_And_Engineers`](../MoM/src/AIDUDES.c#L2084) — body exists, no doc, no done-done commit

### Wave 4 — Overland Spell-AI Subsystem ✅ done
Triggered from [AIDUDES.c:261-272](../MoM/src/AIDUDES.c#L261-L272) when the AI's queued spell finishes accumulating mana.

- [x] **4A** [`Cast_Spell_Overland`](../MoM/src/OverSpel.c#L627) — **done-done 2026-06-10** (commit `9cd8113d`), doc [SETTLE-Cast_Spell_Overland.md](ComputerPlayer/SETTLE-Cast_Spell_Overland.md). Completes the in-progress overland spell cast (lines 627-1956, 1330 lines). 9-entry bug catalog. Nature's Wrath retaliation wired via `Call_Forth_The_Force_Of_Nature(player_idx)` at [OverSpel.c:1944](../MoM/src/OverSpel.c#L1944).
- [x] **4B** [`AI_Spell_Select`](../MoM/src/AISPELL.c#L309) — **done-done 2026-06-11** (commit `0c27ba57`), doc [AISPELL-AI_Spell_Select.md](ComputerPlayer/AISPELL-AI_Spell_Select.md). Dispatcher complete; OGBUG flagged at [line 320](../MoM/src/AISPELL.c#L320). Spell-group leaf pickers (8 of them: Summon, Unit_Enchantment, City_Enchantment, Disenchant, Disjunction, Global, Suppression, Attack) all reconstructed and have walkthrough docs under `AISPELL-AI_Select_Spell_Group_*.md`.
- [x] **4C** Target-picker cluster — **done-done**: `AITP_City_Enchantment` (`d30d8ead`), `AITP_Unit_Enchantment` (`47cb2bac`), `AITP_Attack_Wizard` (`176fc764`), `AITP_Disjunction` (`963ce29c`); docs in `doc/ComputerPlayer/AISPELL-AITP_*.md`
- [x] **4D** Specific-spell-targeting cluster — **done-done**: `AITP_Change_Terrain` (`90785676`), `AITP_Spell_Ward` (`1a450f9d`), `AITP_Spell_Ward_Wrapper` (`ffb08b13`), and the full AITP_* lineup (Transmute, Wall_Of_Stone, Wall_Of_Fire, Wall_Of_Darkness, Move_Fortress, Flying_Fortress, Earth_Gate, Gaias_Blessing, Heavenly_Light, Altar_Of_Battle, Stream_Of_Life, Inspirations, Prosperity, Astral_Gate, Dark_Rituals, Cloud_Of_Shadow, Consecration, Enchant_Road, Disenchant, Attack_Terrain, Plane_Shift, Natures_Cures, Floating_Island, Attack_Stack, Corruption, Node, Select_Unit_For_Enchantment, Get_Map_Square_Target_For_Spell)
- [x] **4E** `Overland_Enchantment_Spell_Index` ([OverSpel.c:1988](../MoM/src/OverSpel.c#L1988), renamed from `IDK_Get_Global_Enchant_Index__STUB`) — **folded into [AISPELL-AITP_Disjunction.md](ComputerPlayer/AISPELL-AITP_Disjunction.md)** as a Companion section

### Wave 5 — NPC + event phase
- [ ] **5A** [`NPC_Farmers`](../MoM/src/AIDUDES.c#L2023) + [`NPC_Destinations`](../MoM/src/AIDATA.c#L581) cluster — bodies exist, no commits, no docs
- [ ] **5B** [`Make_Raiders`](../MoM/src/AIDATA.c#L353) + [`Make_Monsters`](../MoM/src/AIDATA.c#L124) — bodies added (commits `2bb05530`, `c161d7a5`) but **not done-done**; both still call `Create_Unit__WIP` ([AIDATA.c:314](../MoM/src/AIDATA.c#L314), [549](../MoM/src/AIDATA.c#L549)); no docs
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
- **AIDATA.c**: `Create_Unit__WIP` callers — `Make_Monsters` ([AIDATA.c:314](../MoM/src/AIDATA.c#L314)) and `Make_Raiders` ([AIDATA.c:549](../MoM/src/AIDATA.c#L549)); the helper itself lives outside AI-Turn scope
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
