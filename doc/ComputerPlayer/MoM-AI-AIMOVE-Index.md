# AIMOVE.c Function Index

Navigation reference for `MoM/src/AIMOVE.c` (~8500 lines). One row per function. Use this to jump straight to the relevant code without re-grepping.

**Coverage status:** Skeleton complete (names, line ranges, drake178 names, `lmt_*` writes/reads). Purpose fields marked **TBD** are not yet filled — they require reading each function's header comment. Filled in incrementally as functions get touched.

**Two overlay groups:**
- `ovr158` — AI Move overlay (`AI_Set_Unit_Orders` driver and per-landmass action helpers)
- `ovr162` — AI Continent overlay (stack/destination management, continent reevaluation)

`__GEMINI` variants are excluded from this index — they are second-opinion translation drafts, not production code.

---

## ovr158 — AI Move (driver and per-landmass actions)

### `AI_Set_Unit_Orders` — [line 133](../../MoM/src/AIMOVE.c#L133)
- **drake178:** `AI_SetUnitOrders()` (o158p01)
- **End:** ~line 239
- **Purpose:** TBD
- **Reads `lmt_*`:** `lmt_Contested`, `lmt_Own`, `lmt_NoOwnCity`, `lmt_NoOwnCityAndAllyHasCity`, `lmt_Abandon` (lines 186-217)

### `G_AI_RallyFill__WIP` — [line 240](../../MoM/src/AIMOVE.c#L240)
- **drake178:** `G_AI_RallyFill()` (o158p02)
- **End:** ~line 297
- **Purpose:** TBD
- **Reads `lmt_*`:** `lmt_Abandon`, `lmt_NoOwnCity` (lines 259-263)

### `AI_FillGarrisons__WIP` — [line 298](../../MoM/src/AIMOVE.c#L298)
- **drake178:** `AI_FillGarrisons()` (o158p03)
- **End:** ~line 741
- **Purpose:** TBD
- **Reads `lmt_*`:** `lmt_Abandon`, `lmt_Own` for `G_Low_Threat` gate (lines 331-344)

### `AI_ProcessOcean__WIP` — [line 742](../../MoM/src/AIMOVE.c#L742)
- **drake178:** `AI_ProcessOcean()` (o158p04)
- **End:** ~line 958
- **Purpose:** TBD
- **Reads `lmt_*`:** `lmt_Contested`, `lmt_NoOwnCity` (line 787-791)

### `G_AI_ProcessTransports__WIP` — [line 959](../../MoM/src/AIMOVE.c#L959)
- **drake178:** `G_AI_ProcessTransports()` (o158p05)
- **End:** ~line 1634
- **Purpose:** TBD
- **Reads `lmt_*`:** `lmt_NoOwnCity` (line 1394)

### `AI_ProcessRoamers__WIP` — [line 1635](../../MoM/src/AIMOVE.c#L1635)
- **drake178:** `AI_ProcessRoamers()` (o158p06)
- **End:** line 1814
- **Purpose:** Searches for and assigns targets + corresponding move orders to all roamer stacks on a continent. If no targets exist but there is at least one roaming stack of 7+ units, **switches the continent to `lmt_Abandon`** and picks a rally point at a reachable dock square — preparing the troops to embark and move to the action continent instead.
- **Writes `lmt_*`:** **`lmt_Abandon`** at [line 1806](../../MoM/src/AIMOVE.c#L1806) (the "abandon this continent" decision)
- **Known issue per header:** "when setting the leave continent type, the rally point chosen ignores own units, and can't be a tile with a city, tower, or node either"

### `G_AI_HomeRallyFill__WIP` — [line 1828](../../MoM/src/AIMOVE.c#L1828)
- **drake178:** `G_AI_HomeRallyFill()` (o158p07)
- **End:** ~line 1897
- **Purpose:** TBD
- **Reads `lmt_*`:** none direct

### `AI_PullForMainWar__WIP` — [line 1898](../../MoM/src/AIMOVE.c#L1898)
- **drake178:** `AI_PullForMainWar()` (o158p08)
- **End:** ~line 1992
- **Purpose:** TBD
- **Reads `lmt_*`:** none direct

### `G_AI_RallyOrFerry__WIP` — [line 1993](../../MoM/src/AIMOVE.c#L1993)
- **drake178:** `G_AI_RallyOrFerry()` (o158p09)
- **End:** ~line 2326
- **Purpose:** TBD
- **Reads/writes `lmt_*`:** none direct (earlier draft of this doc misattributed the `lmt_Abandon` write at line 1806 to this function; that write actually belongs to `AI_ProcessRoamers__WIP` — see entry above)

### `AI_Build_Target_List` — [line 2327](../../MoM/src/AIMOVE.c#L2327)
- **drake178:** `AI_CreateTargetList()` (o158p10)
- **End:** ~line 2772
- **Purpose:** TBD
- **Writes `lmt_*`:** `lmt_NoTargets` (line 2757)

### `AI_Add_Target` — [line 2773](../../MoM/src/AIMOVE.c#L2773)
- **drake178:** `AI_AddTarget()` (o158p11)
- **End:** ~line 2803
- **Purpose:** TBD
- **Reads/writes `lmt_*`:** none

### `AI_AssignStackTarget__WIP` — [line 2804](../../MoM/src/AIMOVE.c#L2804)
- **drake178:** `AI_AssignStackTarget()` (o158p12)
- **End:** ~line 2993
- **Purpose:** TBD
- **Reads/writes `lmt_*`:** none direct

### `AI_Build_Stacks_Find_Targets_Order_Moves` — [line 2994](../../MoM/src/AIMOVE.c#L2994)
- **drake178:** `AI_CONTX_Regroup()` (o158p13)
- **End:** ~line 3291
- **Purpose:** TBD — name in current source is much broader than drake's `AI_CONTX_Regroup()`
- **Reads/writes `lmt_*`:** none direct

### `AI_Find_Nearest_Target_Unit` — [line 3292](../../MoM/src/AIMOVE.c#L3292)
- **drake178:** `AI_FindClosestEnemy()` (o158p14)
- **End:** ~line 3382
- **Purpose:** TBD

### `AI_Find_Opportunity_City_Target` — [line 3383](../../MoM/src/AIMOVE.c#L3383)
- **drake178:** `AI_OppMoveOverride()` (o158p15)
- **End:** ~line 3536
- **Purpose:** TBD

### `AI_Disband_To_Balance_Budget` — [line 3537](../../MoM/src/AIMOVE.c#L3537)
- **drake178:** `AI_Balance_Upkeep()` (o158p16)
- **End:** ~line 3752
- **Purpose:** TBD — likely income/upkeep balancing by unit disbandment

### `AI_Shift_Off_Home_Plane` — [line 3753](../../MoM/src/AIMOVE.c#L3753)
- **drake178:** `AI_ShiftOffHomePlane()` (o158p17)
- **End:** ~line 3885
- **Purpose:** TBD

### `AI_Move_Out_Boats` — [line 3886](../../MoM/src/AIMOVE.c#L3886)
- **drake178:** `AI_Transport_Sailoff()` (o158p18)
- **End:** ~line 3992
- **Purpose:** TBD

### `AI_GarrBuilderPush__WIP` — [line 3993](../../MoM/src/AIMOVE.c#L3993)
- **drake178:** `AI_GarrBuilderPush()` (o158p19)
- **End:** ~line 4161
- **Purpose:** TBD

### `AI_Survey_Excess_Units` — [line 4162](../../MoM/src/AIMOVE.c#L4162)
- **drake178:** `AI_Survey_Excess_Units()` (o158p20)
- **End:** ~line 4218
- **Purpose:** TBD

### `AI_Survey_Excess_Units_In_Stack` — [line 4219](../../MoM/src/AIMOVE.c#L4219)
- **drake178:** (no drake name) (o158p21)
- **End:** ~line 4441
- **Purpose:** TBD — new helper (no drake equivalent)

### `AI_Do_Meld` — [line 4442](../../MoM/src/AIMOVE.c#L4442)
- **drake178:** `AI_ProcessMelders()` (o158p22)
- **End:** ~line 4611
- **Purpose:** TBD
- **Reads `lmt_*`:** `lmt_Own`, `≥ lmt_Abandon` (lines 4538-4540)

### `AI_Do_Settle` — [line 4612](../../MoM/src/AIMOVE.c#L4612)
- **drake178:** `AI_ProcessSettlers()` (o158p23)
- **End:** ~line 4879
- **Purpose:** TBD

### `AI_Do_Purify` — [line 4880](../../MoM/src/AIMOVE.c#L4880)
- **drake178:** `AI_ProcessPurifiers()` (o158p24)
- **End:** ~line 5028
- **Purpose:** TBD

### `AI_Do_RoadBuild` — [line 5029](../../MoM/src/AIMOVE.c#L5029)
- **drake178:** `AI_ProcessRoadBuilds()` (o158p25)
- **End:** ~line 5191
- **Purpose:** TBD

### `AI_Set_Move_Or_Goto_Target` — [line 5192](../../MoM/src/AIMOVE.c#L5192)
- **drake178:** (no drake name) (o158p26)
- **End:** ~line 5260
- **Purpose:** TBD — new helper

### `AI_Order_Settle` — [line 5261](../../MoM/src/AIMOVE.c#L5261)
- **drake178:** `AI_UNIT_SettleOrder()` (o158p27)
- **End:** ~line 5293
- **Purpose:** TBD

### `AI_Order_RoadBuild` — [line 5294](../../MoM/src/AIMOVE.c#L5294)
- **drake178:** `AI_UNIT_RoadBldOrder()` (o158p28)
- **End:** ~line 5334
- **Purpose:** TBD

### `AI_Order_SeekTransport` — [line 5335](../../MoM/src/AIMOVE.c#L5335)
- **drake178:** `AI_UNIT_SeekTransport()` (o158p29)
- **End:** ~line 5366
- **Purpose:** TBD

### `AI_Order_Meld` — [line 5367](../../MoM/src/AIMOVE.c#L5367)
- **drake178:** `AI_UNIT_SetMeldOrder()` (o158p30)
- **End:** ~line 5396
- **Purpose:** TBD

### `AI_Order_Purify` — [line 5397](../../MoM/src/AIMOVE.c#L5397)
- **drake178:** `AI_UNIT_PurifyOrder()` (o158p31)
- **End:** ~line 5429
- **Purpose:** TBD

### `Map_Square_Area_Has_Opponent` — [line 5430](../../MoM/src/AIMOVE.c#L5430)
- **drake178:** `TILE_CheckEnemyUnits()` (o158p32)
- **End:** ~line 5486
- **Purpose:** TBD

### `AI_Enemy_Unit_In_Range` — [line 5487](../../MoM/src/AIMOVE.c#L5487)
- **drake178:** `AI_TILE_CheckEnemies()` (o158p33)
- **End:** ~line 5548
- **Purpose:** TBD

### `AI_CanSettleOffPlane__STUB` — [line 5549](../../MoM/src/AIMOVE.c#L5549)
- **drake178:** `AI_CanSettleOffPlane()` (o158p34)
- **End:** ~line 5567
- **Purpose:** TBD — STUB suffix indicates not implemented

### `AI_SendToColonize__WIP` — [line 5568](../../MoM/src/AIMOVE.c#L5568)
- **drake178:** `AI_SendToColonize()` (o158p35)
- **End:** ~line 5797
- **Purpose:** TBD

### `AI_SeekTransportFrom__WIP` — [line 5798](../../MoM/src/AIMOVE.c#L5798)
- **drake178:** `AI_SeekTransportFrom()` (o158p36)
- **End:** ~line 5843
- **Purpose:** TBD

### `AI_Tower_Target_Worthwhile` — [line 5844](../../MoM/src/AIMOVE.c#L5844)
- **drake178:** `AI_CheckOtherPlane()` (o158p37)
- **End:** ~line 5932
- **Purpose:** TBD — current name differs significantly from drake's `AI_CheckOtherPlane()` — verify alignment
- **Reads `lmt_*`:** `lmt_Own`, `lmt_NoOwnCityAndAllyHasCity`, `lmt_NoOwnCity` (lines 5860-5891)

### `AI_SingleCont_Reeval__WIP` — [line 5933](../../MoM/src/AIMOVE.c#L5933)
- **drake178:** (no drake name listed at o158p38)
- **End:** ~line 6268
- **Purpose:** Per-landmass reclassifier — cancels orders on the landmass, recomputes city center / unit-cost sums, assigns `lmt_*` type, picks rally point
- **Writes `lmt_*`:** `lmt_Own` (6027), `lmt_Contested` (6031), `lmt_NoOwnCity` (6036), `lmt_NoOwnCityAndAllyHasCity` (6056, 6061, 6063, 6134)
- **Reads `lmt_*`:** `lmt_NoOwnCity` (6053), `lmt_Contested`/`lmt_Own`/`lmt_NoOwnCityAndAllyHasCity` (6066-6071), `lmt_Own` (6146)
- **Known issue:** does not zero slot before counting (compare with `AI_Reevaluate_All_Continents:7013`). Stale prior value can spuriously pass the city-count test.

---

## ovr162 — AI Continent (stack/destination management)

### Unreconstructed stubs (o162p01–p11)

Lines 6227-6258 contain WZD/drake markers for 11 unreconstructed disassembly stubs (`sub_F48F0()` through `sub_F5432()`). No function bodies. These mark positions in the original disassembly that haven't been translated yet.

### `AI_Stack_Set_Destination` — [line 6269](../../MoM/src/AIMOVE.c#L6269)
- **drake178:** (no drake name at o162p12 — see header comment XREF to `NPC_Destinations`)
- **End:** ~line 6377
- **Purpose:** Per stack target-destination assignment (called from `NPC_Destinations` for neutral-player city targets per header). GEMINI translation present.
- **Reads/writes `lmt_*`:** none direct

### Unreconstructed stubs (o162p13–p18)

Lines 6348-6364 contain stubs for `sub_F57AF()` through `sub_F5EBF()`. No bodies.

### `AI_Stack_Set_Boats_Goto` — [line 6378](../../MoM/src/AIMOVE.c#L6378)
- **drake178:** `AI_STK_MoveTransports()` (o162p19)
- **End:** ~line 6433
- **Purpose:** TBD

### Unreconstructed stub (o162p20)

Line 6419-6421: `sub_F60F4()`. No body.

### `TILE_AI_FindLoadTile__WIP` — [line 6434](../../MoM/src/AIMOVE.c#L6434)
- **drake178:** `TILE_AI_FindLoadTile()` (o162p21)
- **End:** ~line 6543
- **Purpose:** TBD

### Unreconstructed stub (o162p22)

Line 6525-6527: `sub_F6316()`. No body.

### `TILE_AI_FindEmptyLnd__WIP` — [line 6544](../../MoM/src/AIMOVE.c#L6544)
- **drake178:** `TILE_AI_FindEmptyLnd()` (o162p23)
- **End:** ~line 6659
- **Purpose:** TBD

### Unreconstructed stubs (o162p24–p31)

Lines 6605-6628 contain stubs for `sub_F64C2()` through `sub_F6EBF()`. No bodies.

### `Adjacent_Free_Square` — [line 6660](../../MoM/src/AIMOVE.c#L6660)
- **drake178:** (no drake name) — sits in stub range
- **End:** ~line 6746
- **Purpose:** TBD — new helper

### `Make_Monster_List` — [line 6747](../../MoM/src/AIMOVE.c#L6747)
- **drake178:** (no drake name at o162p33)
- **End:** ~line 6802
- **Purpose:** TBD

### `AI_Stack_Can_Mobilize` — [line 6803](../../MoM/src/AIMOVE.c#L6803)
- **drake178:** (no drake name at o162p34)
- **End:** ~line 6901
- **Purpose:** TBD

### `AI_Reevaluate_All_Continents` — [line 6902](../../MoM/src/AIMOVE.c#L6902)
- **drake178:** `AI_Continent_Reeval()` (o162p35)
- **End:** ~line 7605
- **Purpose:** Full-rebuild classifier for ALL landmasses on ALL planes for one player. Counterpart to per-landmass `AI_SingleCont_Reeval__WIP`.
- **Writes `lmt_*`:** `lmt_Unevaluated` (7013), `lmt_Own` (7161), `lmt_Contested` (7167), `lmt_NoOwnCity` (7175), `lmt_NoOwnCityAndAllyHasCity` (7206, 7215, 7217, 7387, 7470)
- **Reads `lmt_*`:** `lmt_NoOwnCity` (6950, 7203, 7235, 7412), `lmt_NoTargets` (7144), `lmt_Unevaluated` (7150), `lmt_Contested`/`lmt_Own`/`lmt_NoOwnCityAndAllyHasCity` (7313-7318), `lmt_Own` (7494)

### `AI_Evaluation_Map` — [line 7606](../../MoM/src/AIMOVE.c#L7606)
- **drake178:** (no drake name at o162p36)
- **End:** ~line 7833
- **Purpose:** TBD — likely populates `g_ai_evaluation_map[][]` (the per-tile attribute bitmap used by other AI passes)

### `AI_Choose_War_Landmass [line 7834](../../MoM/src/AIMOVE.c#L7834)
- **drake178:** (no drake name at o162p37)
- **End:** ~line 8265
- **Purpose:** Picks "main war continent" per player per plane. Decides per-landmass whether existing assignment should be reevaluated based on type and hostility.
- **Reads `lmt_*`:** switch on `lmt_Unevaluated`/`lmt_Own`/`lmt_Contested`/`lmt_NoOwnCity`/`lmt_NoOwnCityAndAllyHasCity`/`lmt_Abandon` (lines 7896-7969), plus `lmt_Own`/`lmt_NoTargets` filter (line 8199-8202)
- **Known issue:** `lmt_NoTargets` is unhandled in the switch (Gemini groups it with the NoOwnCity case). `lmt_Unevaluated` case is dead (disassembly uses `(type-1)` jump table).

### `Player_Clear_All_Unit_Orders` — [line 8491](../../MoM/src/AIMOVE.c#L8491)
- **drake178:** (no drake name at o162p38)
- **End:** end of file
- **Purpose:** TBD

---

## How to fill in `TBD` purpose lines

When you touch a function whose purpose is `TBD`, please:

1. Read the header comment block (the `/* ... */` block immediately above the function definition).
2. Replace `TBD` with a single sentence summarizing what the function does.
3. Note any explicit `BUG`, `OGBUG`, `BUGBUG`, or `DEDU` markers in the comment.
4. If the function's role in the AI turn pipeline is non-obvious, add a `Pipeline:` line.

This is intentionally incremental — better to fill in 5 verified entries per session than 49 fabricated ones.

## Related references

- [MoM-AI-Landmass-Types.md](MoM-AI-Landmass-Types.md) — `lmt_*` state machine (writes, reads, transitions)
- [MoM-AI-Move.md](MoM-AI-Move.md) — broader AIMOVE.c overview / call graph
- [MoM-NextTurn-AI.md](../MoM-NextTurn-AI.md) — overall AI turn sequencing
- [MoM-AI-Continents.md](MoM-AI-Continents.md) — continent / landmass data structures
