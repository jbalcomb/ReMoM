# Master of Magic — FeatureFreak: AI Usage of the Features

> Companion to [#MOM-FeatureFreak.md](#MOM-FeatureFreak.md). Where the overview catalogs *what features exist*, this doc catalogs *how the computer player (AI) uses them* — pillar by pillar, mapped onto the same 4X + MoM-systems structure.

The perspective here is the **implemented AI** as reconstructed in ReMoM (the `AI*` source modules), not an abstract description of "good MoM strategy." Every named routine below is verified against the source; deeper per-function behavior lives in the referenced walkthrough docs under [doc/ComputerPlayer/](../ComputerPlayer/).

**Who the AI controls.** The human is `player_idx 0` and is skipped. The AI drives the **opponent wizards** (`player_idx 1 … _num_players-1`) and a separate **Neutral player** (`NEUTRAL_PLAYER_IDX`) that runs neutral/raider towns and roaming monsters. *(Verified: the two player loops in `AI_Next_Turn()`, `AIDUDES.c`.)*

---

## The AI turn — one pass, in order

Everything the AI does is driven from **`AI_Next_Turn()`** (`AIDUDES.c`). The verified phase order for each AI wizard, then the shared post-passes, is:

**Per-wizard loop (`player_idx = 1 … _num_players-1`):**

1. **`AI_Evaluate_Hostility`** — set posture toward the human and rivals.
2. **`AI_Evaluate_Magic_Power_Strategy`** — decide the mana/skill/research emphasis.
3. **`Player_Hostile_Opponents`** — enumerate who this wizard is at war with.
4. **`AI_Player_Calculate_Target_Values`** — score cities/stacks/lairs as targets.
5. **`AI_Landmass_Values_And_Strengths`** — aggregate strength & value per landmass/plane.
6. **`AI_Choose_War_Landmass`** — pick where to concentrate offensive effort.
7. **`Cast_Spell_Overland`** (if a cast just completed) → then **`AI_Spell_Select`** to queue the next spell.
8. **`AI_Update_Magic_Power`**, **`AI_Sanity_Check_Overland_Enchantments`**, **`AI_Update_Gold_And_Mana_Reserves`**, **`AI_Update_Gold_Income_And_Food_Income`** — settle the economy.
9. Decrement peace-treaty durations.
10. **`Player_All_Colony_Autobuild`** — set every city's production.
11. **`AI_Evaluation_Map`**, **`AI_Evaluate_Continents`**, **`AI_Set_Unit_Orders`** — plan all unit orders.
12. **`AI_Kill_Excess_Settlers_And_Engineers`** — trim units it can't use.

**Shared post-passes (all AI wizards, then Neutral):**

13. **`AI_Execute_Orders`** — actually move every wizard's units (movement phase, separate from planning).
14. Neutral player: **`Player_All_Colony_Autobuild(NEUTRAL)`**, **`NPC_Farmers`**, **`NPC_Destinations`**, **`AI_Execute_Orders(NEUTRAL)`**.
15. World events: **`Make_Raiders`**, **`Make_Monsters`**.
16. Cleanup: **`NPC_Excess_Garrison`**, **`AI_Hopeless_Stasis`**.

*(All 16 verified against the `PHASE(...)` call sequence in `AI_Next_Turn()`.)* Note the **plan-then-execute split**: orders are computed for all wizards in the per-wizard loop, and only later does `AI_Execute_Orders` walk the units. Cross-referenced: [MoM-NextTurn-AI.md](../MoM-NextTurn-AI.md), [ComputerPlayer/AIDUDES-AI_Next_Turn.md](../ComputerPlayer/AIDUDES-AI_Next_Turn.md), [ComputerPlayer/AI-Turn-Diagrams.md](../ComputerPlayer/AI-Turn-Diagrams.md).

---

## 1. eXplore — how the AI reveals and reads the world

- **Map model rebuild.** Before (and repeatedly during) each turn the AI rebuilds its spatial model with **`EMMDATAH_Map()`** and **`AI_Evaluation_Map`**, producing the value/strength grids all later decisions read. Cross-referenced: [ComputerPlayer/AIMOVE-AI_Evaluation_Map.md](../ComputerPlayer/AIMOVE-AI_Evaluation_Map.md).
- **Continent awareness.** **`AI_Evaluate_Continents`** and **`AI_Reevaluate_Continent`** classify each landmass (home, frontier, contested, ocean) so the AI knows what it has explored and what it must reach. Cross-referenced: [MoM-AI-Continents.md](../ComputerPlayer/MoM-AI-Continents.md), [MoM-AI-Landmass-Types.md](../ComputerPlayer/MoM-AI-Landmass-Types.md), [AIMOVE-AI_Evaluate_Continents.md](../ComputerPlayer/AIMOVE-AI_Evaluate_Continents.md).
- **Roamers as scouts/clearers.** "Roamer" stacks are targeted and deployed to explore and clear the map via **`AI_Stacks_Roamers_Target_Or_Deploy`** and **`AI_Stacks_Relocate_Roamers`**. Cross-referenced: [ComputerPlayer/AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md](../ComputerPlayer/AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md).
- **Lair assessment.** Intact lairs feed into target valuation (§4) — the AI weighs a lair's garrison strength against its reward before committing.

---

## 2. eXpand — how the AI founds and grows cities

- **City production.** Each city's build queue is chosen by **`Player_All_Colony_Autobuild`**, which dispatches by owner: **`Player_Colony_Autobuild_CP`** — **CP = Computer Player** (the AI wizards) — and **`Player_Colony_Autobuild_NP`** — **NP = Neutral Player** *(verified: the `NEUTRAL_PLAYER_IDX` branch in [AIBUILD.c:58-65](../../MoM/src/AIBUILD.c#L58-L65))*. The sibling **`Player_Colony_Autobuild_HP`** — **HP = Human Player** — is the **Grand Vizier's** auto-build for the human's own cities and is **not** part of the AI turn. Cross-referenced: [MoM-AI-Building.md](../MoM-AI-Building.md), [ComputerPlayer/AIBUILD-Player_All_Colony_Autobuild.md](../ComputerPlayer/AIBUILD-Player_All_Colony_Autobuild.md), [ComputerPlayer/AIBUILD-Player_Colony_Autobuild_CP.md](../ComputerPlayer/AIBUILD-Player_Colony_Autobuild_CP.md).
- **Buying production.** The AI can rush-buy a city's current item via **`AI_Player_City_Buy_Production`** when gold and priority warrant it. *(Verified: `AIDUDES.c`.)*
- **Settling new cities.** Settler stacks are ordered to found towns through **`AI_Stacks_Do_Settle`**, with relocation logic (**`AI_Stacks_Reorder_Settle_Elsewhere`**) when the first choice is blocked. Cross-referenced: [ComputerPlayer/AIMOVE-AI_Stacks_Do_Settle.md](../ComputerPlayer/AIMOVE-AI_Stacks_Do_Settle.md), [ComputerPlayer/SETTLE-AI_Unit_Army_Do_Meld.md](../ComputerPlayer/SETTLE-AI_Unit_Army_Do_Meld.md).
- **Terraforming its holdings.** The AI improves land with road-building (**`AI_Stacks_Do_RoadBuild`**) and corruption-clearing (**`AI_Stacks_Do_Purify`**). Cross-referenced: [ComputerPlayer/AIMOVE-AI_Stacks_Do_RoadBuild.md](../ComputerPlayer/AIMOVE-AI_Stacks_Do_RoadBuild.md), [ComputerPlayer/AIMOVE-AI_Stacks_Do_Purify.md](../ComputerPlayer/AIMOVE-AI_Stacks_Do_Purify.md).
- **Hard cap on settlers & engineers (significant AI limitation).** The AI is limited to roughly **one Settler and one Engineer per landmass**, enforced at *two* stages:
  - **Production gate (primary).** `Player_Colony_Autobuild_CP` computes `need_settler` / `need_engineer` and **zeros the build weight** of any settler/engineer the AI doesn't need, so it never queues a second one *(verified: [AIBUILD.c:202-250](../../MoM/src/AIBUILD.c#L202-L250), [350-395](../../MoM/src/AIBUILD.c#L350-L395))*. The "already exists" test is **per-landmass** (same plane + landmass as the city); the "already in production" test is **empire-wide** (any other city of the player building one blocks all others). Settlers additionally require city population > 2; both get a build-weight boost when genuinely needed (×1.5 before turn 50, plus an objective multiplier).
  - **Kill backstop (secondary).** **`AI_Kill_Excess_Settlers_And_Engineers`** *destroys* (`Kill_Unit`) any surplus that slips past the gate — captured units, units that migrate onto an already-occupied landmass, or simultaneous completions: at most **one Settler per landmass at all times**, and at most **one Engineer per landmass but only once `_turn > 200`** *(verified: [AIDUDES.c:1934-1987](../../MoM/src/AIDUDES.c#L1934-L1987); settlers keyed on `UA_CREATEOUTPOST`, engineers on `Construction > 0`; ocean/no-landmass squares excluded)*.

  This is a real constraint on AI expansion tempo — it can never stockpile settlers to colonize a continent in parallel (the empire-wide in-production check makes it even more conservative than the per-landmass steady state), and cannot run more than one road/terraform engineer per landmass in the late game. drake178 documented this as "one Settler and one Engineer at a time"; the verified refinements are the **per-landmass scope**, the **two-stage** enforcement, and the **turn-200 gate** on the engineer *kill* (early-game engineers are never culled, though the production gate still throttles them).

---

## 3. eXploit — how the AI runs the economy

- **Power strategy.** **`AI_Evaluate_Magic_Power_Strategy`** decides how the wizard leans across mana / casting-skill / research for the coming turns. Cross-referenced: [MoM-NextTurn-AI.md](../MoM-NextTurn-AI.md).
- **Power & income bookkeeping.** **`AI_Update_Magic_Power`**, **`AI_Update_Gold_And_Mana_Reserves`**, and **`AI_Update_Gold_Income_And_Food_Income`** reconcile the resource quartet each turn. Cross-referenced: [ComputerPlayer/AIDUDES-AI_Update_Magic_Power.md](../ComputerPlayer/AIDUDES-AI_Update_Magic_Power.md), [ComputerPlayer/AIDUDES-AI_Update_Gold_And_Mana_Reserves.md](../ComputerPlayer/AIDUDES-AI_Update_Gold_And_Mana_Reserves.md), [ComputerPlayer/AIDUDES-AI_Update_Gold_Income_And_Food_Income.md](../ComputerPlayer/AIDUDES-AI_Update_Gold_Income_And_Food_Income.md).
- **Budget defense.** When upkeep outruns income the AI sheds units via **`AI_Disband_To_Balance_Budget`**. Cross-referenced: [ComputerPlayer/AIMOVE-AI_Disband_To_Balance_Budget.md](../ComputerPlayer/AIMOVE-AI_Disband_To_Balance_Budget.md).
- **Node exploitation.** Captured nodes are melded via **`AI_Stacks_Do_Meld`** (and the `AITP_Node` spell picker) to feed the power base. Cross-referenced: [ComputerPlayer/AIMOVE-AI_Do_Meld.md](../ComputerPlayer/AIMOVE-AI_Do_Meld.md), [ComputerPlayer/AISPELL-AITP_Node.md](../ComputerPlayer/AISPELL-AITP_Node.md).
- **Mercenaries & offers.** The AI evaluates mercenary/hero offers with **`AI_Accept_Mercenaries`**. *(Verified: `AIDUDES.c`.)*

---

## 4. eXterminate — how the AI wages war

- **Target valuation.** **`AI_Player_Calculate_Target_Values`** scores every enemy city, stack, and lair; **`AI_Landmass_Values_And_Strengths`** rolls those up per landmass. Cross-referenced: [MoM-AI-Target-Values.md](../ComputerPlayer/MoM-AI-Target-Values.md), [ComputerPlayer/AIMOVE-AI_Stacks_Assign_Target.md](../ComputerPlayer/AIMOVE-AI_Stacks_Assign_Target.md).
- **Choosing the front.** **`AI_Choose_War_Landmass`** commits the wizard's offensive weight to a landmass; **`AI_Find_Opportunity_City_Target`** spots targets of opportunity. Cross-referenced: [ComputerPlayer/AIMOVE-AI_Choose_War_Landmass.md](../ComputerPlayer/AIMOVE-AI_Choose_War_Landmass.md), [ComputerPlayer/AIMOVE-AI_Find_Opportunity_City_Target.md](../ComputerPlayer/AIMOVE-AI_Find_Opportunity_City_Target.md).
- **Order assignment (the planner).** **`AI_Set_Unit_Orders`** is the master planner: it builds the target list (**`AI_Build_Target_List`**), assigns stacks to targets (**`AI_Stacks_Assign_Target`**), garrisons sites (**`AI_Stacks_Garrison_Sites`**), and stages invasion forces (**`AI_Stacks_Stage_Expedition_Forces`**). Cross-referenced: [ComputerPlayer/AIMOVE-AI_Set_Unit_Orders.md](../ComputerPlayer/AIMOVE-AI_Set_Unit_Orders.md), [MoM-AI-AIMOVE-Index.md](../ComputerPlayer/MoM-AI-AIMOVE-Index.md).
- **Sea invasions.** Amphibious warfare is handled by the ferry/boat machinery — **`AI_Stacks_Setup_Ferry`**, **`AI_Stacks_Order_Ferry`**, **`AI_Stacks_Ferry_Add_Location`**, **`AI_Move_Out_Boats`**, **`AI_Stacks_Ocean_Landmass_Orders`**, and **`AI_Stacks_Wartime_Ocean_Movement_And_Cleanup`**. Cross-referenced: [ComputerPlayer/AIMOVE-AI_Stacks_Setup_Ferry.md](../ComputerPlayer/AIMOVE-AI_Stacks_Setup_Ferry.md), [ComputerPlayer/AIMOVE-AI_Move_Out_Boats.md](../ComputerPlayer/AIMOVE-AI_Move_Out_Boats.md).
- **Execution.** **`AI_Execute_Orders`** carries out the plans, moving stacks and initiating combat. Cross-referenced: [ComputerPlayer/SETTLE-AI_Execute_Orders.md](../ComputerPlayer/SETTLE-AI_Execute_Orders.md), [MoM-AI-Move.md](../ComputerPlayer/MoM-AI-Move.md).
- **Tactical combat.** Once a battle starts, the AI fights it (unit maneuver + combat spellcasting). *(The overland modules above stop at initiating combat; tactical resolution is a separate subsystem — see [Combat/](../Combat/).)* **→ (gap: AI tactical-combat behavior is not yet a walkthrough doc)**

---

## 5. Magic — how the AI casts and researches

- **Spell selection.** **`AI_Spell_Select`** chooses what to research/cast; **`AI_Compute_Spells_Info`** scores candidate spells, then **`AI_Select_Spell_Group_*`** routines pick within a category — Attack, Summon, City/Unit/Global Enchantment, Disenchant, Disjunction, Suppression. Cross-referenced: [ComputerPlayer/AISPELL-AI_Spell_Select.md](../ComputerPlayer/AISPELL-AI_Spell_Select.md), [ComputerPlayer/AISPELL-AI_Compute_Spells_Info.md](../ComputerPlayer/AISPELL-AI_Compute_Spells_Info.md), [ComputerPlayer/AISPELL-AI_Select_Spell_Group.md](../ComputerPlayer/AISPELL-AI_Select_Spell_Group.md).
- **Overland casting.** Completed casts are applied by **`Cast_Spell_Overland`**; **`AI_Sanity_Check_Overland_Enchantments`** keeps the wizard's active global/city enchantments coherent. Cross-referenced: [ComputerPlayer/SETTLE-Cast_Spell_Overland.md](../ComputerPlayer/SETTLE-Cast_Spell_Overland.md), [ComputerPlayer/AISPELL-AI_Sanity_Check_Overland_Enchantments.md](../ComputerPlayer/AISPELL-AI_Sanity_Check_Overland_Enchantments.md).
- **Per-spell target pickers (AITP).** Each spell that needs a target has a dedicated **`AITP_*`** picker — e.g. `AITP_Node`, `AITP_Change_Terrain`, `AITP_City_Enchantment`, `AITP_Unit_Enchantment`, `AITP_Corruption`, `AITP_Move_Fortress`, `AITP_Astral_Gate`, `AITP_Plane_Shift`, `AITP_Wall_Of_Fire`, and many more (see the `AISPELL-AITP_*` doc set). This is the AI's answer to "the game has 214 spells" — a per-spell handler that decides *whether and where* to cast it. Cross-referenced: [ComputerPlayer/#AI_Target_Pickers_AITP.md](../ComputerPlayer/#AI_Target_Pickers_AITP.md).
- **Target helpers.** **`Get_Map_Square_Target_For_Spell`** and **`Select_Unit_For_Enchantment`** back the pickers. Cross-referenced: [ComputerPlayer/AISPELL-Get_Map_Square_Target_For_Spell.md](../ComputerPlayer/AISPELL-Get_Map_Square_Target_For_Spell.md), [ComputerPlayer/AISPELL-Select_Unit_For_Enchantment.md](../ComputerPlayer/AISPELL-Select_Unit_For_Enchantment.md).
- **→ (future child doc: AI spell strategy per realm)**

---

## 6. Wizards & retorts — how personality steers the AI

- **Hostility & posture.** **`AI_Evaluate_Hostility`** and **`Player_Hostile_Opponents`** derive the wizard's stance from personality, relations, and board state — the entry point through which a Maniacal vs. Peaceful temperament changes behavior. Cross-referenced: [MoM-Diplomacy.md](../MoM-Diplomacy.md), [MoX-Diplomacy-NextTurn.md](../MoX-Diplomacy-NextTurn.md).
- **Retorts as modifiers.** Retorts (Alchemy, the Mastery set, Node Mastery, Myrran, Archmage, etc.) act as economic/combat multipliers the AI benefits from implicitly through the shared game rules; the AI does not have separate "use my retort" logic so much as it plays a stronger game when its wizard build is stronger.
- **→ (gap: how much the AI's plan diverges by personality/retort is not yet documented per-wizard)**

---

## 7. Heroes & items — how the AI staffs and equips

- **Accepting heroes.** Hero/mercenary offers route through **`AI_Accept_Mercenaries`**; the AI fills its **6 hero slots** (`NUM_HEROES`) as opportunity and gold allow. *(Verified: `AIDUDES.c`, `NUM_HEROES` used across `CITYCALC.c`, `ItemScrn.c`, `ArmyList.c`.)*
- **Item usage.** Heroes carry the same item slots as the human's; item *creation* by the AI (Create Artifact / Enchant Item) is driven through the spell-selection path (§5) rather than a dedicated hero-equipping planner.
- **→ (gap: AI hero deployment/equipping is thinly documented)**

---

## 8. The two planes — how the AI plays both worlds

- **Plane-aware planning.** Landmass/continent evaluation (§1) runs per-plane, so target values, war landmass, and garrisoning are all computed for Arcanus and Myrror independently.
- **Shifting planes.** **`AI_Shift_Off_Home_Plane`** relocates forces off the starting plane when strategy calls for it; the `AITP_Plane_Shift` and `AITP_Astral_Gate`/`AITP_Earth_Gate` spell pickers give the AI magical inter-plane travel. Cross-referenced: [ComputerPlayer/AIMOVE-AI_Shift_Off_Home_Plane.md](../ComputerPlayer/AIMOVE-AI_Shift_Off_Home_Plane.md), [ComputerPlayer/AISPELL-AITP_Plane_Shift.md](../ComputerPlayer/AISPELL-AITP_Plane_Shift.md), [ComputerPlayer/AISPELL-AITP_Astral_Gate.md](../ComputerPlayer/AISPELL-AITP_Astral_Gate.md).

---

## 9. Neutral player & world events (AI-only content)

Features with **no human equivalent**, run entirely by the AI at the end of the turn:

- **Neutral towns.** Built and grown via `Player_All_Colony_Autobuild(NEUTRAL)`, `NPC_Farmers`, and moved via `NPC_Destinations` + `AI_Execute_Orders(NEUTRAL)`. Cross-referenced: [ComputerPlayer/AIDATA-NPC_Destinations.md](../ComputerPlayer/AIDATA-NPC_Destinations.md), [ComputerPlayer/AIDATA-NPC_Farmers.md](../ComputerPlayer/AIDUDES-NPC_Farmers.md).
- **Raiders & wandering monsters.** **`Make_Raiders`** and **`Make_Monsters`** spawn the roaming threats that populate the map; **`Build_NPC_Stacks`** assembles neutral garrisons. Cross-referenced: [ComputerPlayer/AIDATA-Make_Raiders.md](../ComputerPlayer/AIDATA-Make_Raiders.md), [ComputerPlayer/AIDATA-Make_Monsters.md](../ComputerPlayer/AIDATA-Make_Monsters.md), [ComputerPlayer/AIDATA-Build_NPC_Stacks.md](../ComputerPlayer/AIDATA-Build_NPC_Stacks.md).
- **Garrison cleanup & stasis.** **`NPC_Excess_Garrison`** trims over-garrisoned neutral sites; **`AI_Hopeless_Stasis`** freezes AI players judged unable to progress. Cross-referenced: [ComputerPlayer/AIDATA-NPC_Excess_Garrison.md](../ComputerPlayer/AIDATA-NPC_Excess_Garrison.md), [ComputerPlayer/AIDUDES-AI_Hopeless_Stasis.md](../ComputerPlayer/AIDUDES-AI_Hopeless_Stasis.md).

---

## Coverage map (feature → AI handling)

| Pillar (overview §) | Primary AI routines | Depth of docs |
| --- | --- | --- |
| 1 eXplore | `EMMDATAH_Map`, `AI_Evaluation_Map`, `AI_Evaluate_Continents`, roamer stacks | good |
| 2 eXpand | `Player_All_Colony_Autobuild` (CP=AI / NP=neutral; HP=human Vizier), `AI_Stacks_Do_Settle`, `_Do_RoadBuild`, `_Do_Purify` | good |
| 3 eXploit | `AI_Evaluate_Magic_Power_Strategy`, `AI_Update_*`, `AI_Disband_To_Balance_Budget`, `AI_Stacks_Do_Meld` | good |
| 4 eXterminate (overland) | `AI_Player_Calculate_Target_Values`, `AI_Set_Unit_Orders`, ferry set, `AI_Execute_Orders` | good |
| 4 eXterminate (tactical) | — | **gap** |
| 5 Magic | `AI_Spell_Select`, `AI_Compute_Spells_Info`, `AI_Select_Spell_Group_*`, `AITP_*` | good |
| 6 Wizards/retorts | `AI_Evaluate_Hostility`, personality | **partial** |
| 7 Heroes/items | `AI_Accept_Mercenaries` | **thin** |
| 8 Two planes | `AI_Shift_Off_Home_Plane`, `AITP_Plane_Shift`/`_Astral_Gate` | good |
| 9 Neutral/events | `Make_Raiders`, `Make_Monsters`, `NPC_*` | good |

**Documentation gaps to fill:** (a) AI **tactical-combat** behavior, (b) how **personality/retorts** measurably change the plan, (c) AI **hero deployment & item creation**.

---

## Cross-references

- **AI overview:** [MoM-NextTurn-AI.md](../MoM-NextTurn-AI.md), [MoM-NextTurn-AI-Move.md](../MoM-NextTurn-AI-Move.md), [MoM-AI-Stage.md](../MoM-AI-Stage.md), [MoM-AI-Data.md](../MoM-AI-Data.md)
- **AI walkthrough set:** [doc/ComputerPlayer/](../ComputerPlayer/) (AIBUILD-, AIDATA-, AIDUDES-, AIMOVE-, AISPELL-, SETTLE- prefixed docs)
- **AIMOVE index:** [MoM-AI-AIMOVE-Index.md](../ComputerPlayer/MoM-AI-AIMOVE-Index.md)
- **Source modules:** `MoM/src/AIDUDES.c`, `AIMOVE.c`, `AISPELL.c`, `AIBUILD.c`, `AIDATA.c`

---

*Status: initial AI-usage breakdown, mapped to the overview pillars. Verified against `AI_Next_Turn()` and the ComputerPlayer walkthrough docs; per-function behavior claims defer to those docs. Gaps flagged above.*
