# AIMOVE.c Function Index

Navigation reference for `MoM/src/AIMOVE.c` (~7400 lines). One row per function, in source order. Use this to jump straight to the relevant code without re-grepping.

**Coverage status:** Skeleton complete (names, line ranges, drake178 names, overlay pages). Purpose fields marked **TBD** are not yet filled — they require reading each function's header comment. Filled in incrementally as functions get touched.

**Two overlay groups:**
- `ovr158` — AI Move overlay (`AI_Set_Unit_Orders` driver and per-landmass action helpers)
- `ovr162` — AI Continent overlay (stack/destination management, continent reevaluation)

`__GEMINI` variants are excluded from this index — they are second-opinion translation drafts, not production code.

> **Line numbers re-synced 2026-06-05** after the `__GEMINI` deletions and Doxygen-header additions reflowed the file. Function start/end lines and every `lmt_*` read/write line link below were re-verified by grep against the current source.

---

## ovr158 — AI Move (driver and per-landmass actions)

### `AI_Set_Unit_Orders` — [line 221](../../MoM/src/AIMOVE.c#L221)
- **drake178:** `AI_SetUnitOrders()` (o158p01)
- **End:** ~line 340
- **Purpose:** TBD
- **Reads `lmt_*` (per-slot dispatcher gates, [lines 291-322](../../MoM/src/AIMOVE.c#L291-L322)):** `lmt_Leaveable` ([291](../../MoM/src/AIMOVE.c#L291), [305](../../MoM/src/AIMOVE.c#L305), [322](../../MoM/src/AIMOVE.c#L322)), `lmt_Own` ([293](../../MoM/src/AIMOVE.c#L293), [307](../../MoM/src/AIMOVE.c#L307), [318](../../MoM/src/AIMOVE.c#L318)), `lmt_NoOwnCityAndAllyHasCity` ([295](../../MoM/src/AIMOVE.c#L295)), `lmt_NoOwnCity` ([297](../../MoM/src/AIMOVE.c#L297)), `lmt_Contested` ([320](../../MoM/src/AIMOVE.c#L320))

### `AI_Stacks_Stage_Expedition_Forces` — [line 376](../../MoM/src/AIMOVE.c#L376)
- **drake178:** `G_AI_RallyFill()` (o158p02)
- **End:** ~line 437
- **Purpose:** Per-(plane, landmass) — pull top-9 units from the `G_Pushout_*` pool toward this landmass's stage point, gradually assembling an expedition force sized for downstream overseas attacks. UNGATED at the dispatcher; applies own sanity-check gates internally. See [AIMOVE-AI_Stacks_Stage_Expedition_Forces.md](AIMOVE-AI_Stacks_Stage_Expedition_Forces.md).
- **Reads `lmt_*`:** `lmt_Leaveable` ([404](../../MoM/src/AIMOVE.c#L404)), `lmt_NoOwnCity` ([406](../../MoM/src/AIMOVE.c#L406)) — Phase 2 sanity-check gates

### `AI_Stacks_Garrison_Sites` — [line 471](../../MoM/src/AIMOVE.c#L471)
- **drake178:** `AI_FillGarrisons()` (o158p03)
- **End:** ~line 806
- **Purpose:** Slot 13 — survey own cities and own-plane SITE-flagged nodes on this landmass, compute per-asset garrison shortfalls, then draft non-builder units from `AISTK_Roamer`/`AISTK_Unknown` stacks (NOT at the stage point) toward the best-scored target. City-first, node-fallback. See [AIMOVE-AI_Stacks_Garrison_Sites.md](AIMOVE-AI_Stacks_Garrison_Sites.md).
- **Reads `lmt_*`:** `lmt_Leaveable` ([508](../../MoM/src/AIMOVE.c#L508)), `lmt_Own` ([510](../../MoM/src/AIMOVE.c#L510)) — early `low_concern` gate. The slot-13 dispatcher gate that decides whether this runs lives in `AI_Set_Unit_Orders` ([291-322](../../MoM/src/AIMOVE.c#L291-L322)).

### `AI_Stacks_Wartime_Ocean_Movement_And_Cleanup` — [line 847](../../MoM/src/AIMOVE.c#L847)
- **drake178:** `AI_ProcessOcean()` (o158p04)
- **End:** ~line 1033
- **Purpose:** Per-(player, plane) **post-pass** — runs once per plane after the inner per-landmass loop closes. Two halves: (1) pull land-stationed mobile-on-water units (AirTravel/WaterTravel/NonCorporeal, non-melder, non-transport, us_Ready) toward the war-landmass stage point — fortress-closest `lmt_Contested`/`lmt_NoOwnCity` landmass as fallback; (2) rebuild own-stacks for ocean (`landmass_idx = 0`) and `Kill_Unit`-drown every stack with no consumed slot AND no water-mobile unit. See [AIMOVE-AI_Stacks_Wartime_Ocean_Movement_And_Cleanup.md](AIMOVE-AI_Stacks_Wartime_Ocean_Movement_And_Cleanup.md).
- **Reads `lmt_*`:** `lmt_Contested` ([889](../../MoM/src/AIMOVE.c#L889)), `lmt_NoOwnCity` ([891](../../MoM/src/AIMOVE.c#L891)) — Phase 2 fallback stage-point scan

### `AI_Stacks_Ocean_Landmass_Orders` — [line 1037](../../MoM/src/AIMOVE.c#L1037)
- **drake178:** `G_AI_ProcessTransports()` (o158p05)
- **End:** ~line 1527
- **Purpose:** Per-(player, plane) **post-pass** — sibling of [`AI_Stacks_Wartime_Ocean_Movement_And_Cleanup`](AIMOVE-AI_Stacks_Wartime_Ocean_Movement_And_Cleanup.md). Five phases: (1) dead init of `GoingTo_Order`; (2) debit `_ai_ferry_*` queue entries by the capacity of en-route transports and invalidate exhausted entries (with an OGBUG that uses the wrong unit-index); (3) rebuild `_ai_own_stack_*` for the OCEAN (`landmass_idx = 0`); (4) `AI_Stacks_Do_Meld(player_idx)` out-of-band; (5) for each ocean stack containing at least one transport, run a 10-block decision tree (5a classify, 5b gate, 5c all-transport → closest ferry-queue [caller=3], 5d-j mixed stacks: scan adjacent, gate by settler/non-settler landing eligibility, land dispatch [caller=4, transports get suspect `us_Ferry`] OR settler-fallback dispatch [caller=5] OR war-landing dock dispatch [caller=6]). See [AIMOVE-AI_Stacks_Ocean_Landmass_Orders.md](AIMOVE-AI_Stacks_Ocean_Landmass_Orders.md).
- **Reads `lmt_*`:** `lmt_NoOwnCity` ([1361](../../MoM/src/AIMOVE.c#L1361)) — Phase 5g settler-landing gate

### `AI_Stacks_Roamers_Target_Or_Deploy` — [line 1555](../../MoM/src/AIMOVE.c#L1555)
- **drake178:** `AI_ProcessRoamers()` (o158p06)
- **End:** ~line 1720
- **Purpose:** Searches for and assigns targets + corresponding move orders to all roamer stacks on a continent. If no targets exist but there is at least one roaming stack of 7+ units, **switches the continent to `lmt_Leaveable`** and picks a stage point at a reachable dock square — preparing the troops to embark and move to the action continent instead.
- **Writes `lmt_*`:** **`lmt_Leaveable`** ([1715](../../MoM/src/AIMOVE.c#L1715)) — the "abandon this continent" decision (per the inline note, the only place in the codebase that sets it)
- **Known issue per header:** "when setting the leave continent type, the stage point chosen ignores own units, and can't be a square with a city, tower, or node either"

### `AI_Stacks_Relocate_Roamers` — [line 1750](../../MoM/src/AIMOVE.c#L1750)
- **drake178:** `G_AI_HomeRallyFill()` (o158p07)
- **End:** ~line 1796
- **Purpose:** For one landmass, set up ferry-departure for each `AISTK_Roamer` stack via [`AI_Stacks_Setup_Ferry`](AIMOVE-AI_Stacks_Setup_Ferry.md). Gated to landmass types where leaving makes sense (`lmt_Leaveable`, `lmt_NoTargets`, `lmt_Own`). See [AIMOVE-AI_Stacks_Relocate_Roamers.md](AIMOVE-AI_Stacks_Relocate_Roamers.md).
- **Reads `lmt_*`:** none direct (gated by the caller)

### `AI_Stacks_Order_To_War_Landmass` — [line 1800](../../MoM/src/AIMOVE.c#L1800)
- **drake178:** `AI_PullForMainWar()` (o158p08)
- **End:** ~line 1863
- **Purpose:** Order mobile (air- or water-traveling) non-melder units to head to the main war landmass's stage point. Skips stacks already on the war landmass; skips Garrison/FortressGarrison stacks. See [AIMOVE-AI_Stacks_Order_To_War_Landmass.md](AIMOVE-AI_Stacks_Order_To_War_Landmass.md).
- **Reads `lmt_*`:** none direct (gated by the caller on landmass type — see walkthrough)

### `AI_Stacks_Setup_Ferry` — [line 1903](../../MoM/src/AIMOVE.c#L1903)
- **drake178:** `G_AI_RallyOrFerry()` (o158p09)
- **End:** ~line 2135
- **Purpose:** Per-stack workhorse called from [`AI_Stacks_Relocate_Roamers`](AIMOVE-AI_Stacks_Relocate_Roamers.md). For one stack: file a ferry-pickup request, walk units to the stage point (capped by parent's budget), and opportunistically embark on a nearby ocean transport if one exists. See [AIMOVE-AI_Stacks_Setup_Ferry.md](AIMOVE-AI_Stacks_Setup_Ferry.md) — drake178 noted "full of BUGs and inconsistencies."
- **Reads/writes `lmt_*`:** none direct (the `lmt_Leaveable` write belongs to `AI_Stacks_Roamers_Target_Or_Deploy`, not this function)

### `AI_Build_Target_List` — [line 2158](../../MoM/src/AIMOVE.c#L2158)
- **drake178:** `AI_CreateTargetList()` (o158p10)
- **End:** ~line 2502
- **Purpose:** TBD
- **Writes `lmt_*`:** `lmt_NoTargets` ([2499](../../MoM/src/AIMOVE.c#L2499)) — per the inline note, the only place it is set

### `AI_Add_Target` — [line 2520](../../MoM/src/AIMOVE.c#L2520)
- **drake178:** `AI_AddTarget()` (o158p11)
- **End:** ~line 2530
- **Purpose:** TBD
- **Reads/writes `lmt_*`:** none

### `AI_Stacks_Assign_Target` — [line 2559](../../MoM/src/AIMOVE.c#L2559)
- **drake178:** `AI_AssignStackTarget()` (o158p12)
- **End:** ~line 2672
- **Purpose:** TBD
- **Reads/writes `lmt_*`:** none direct

### `AI_Stacks_Init_Build_Target_Order` — [line 2705](../../MoM/src/AIMOVE.c#L2705)
- **drake178:** `AI_CONTX_Regroup()` (o158p13)
- **End:** ~line 2939
- **Purpose:** TBD — name in current source is much broader than drake's `AI_CONTX_Regroup()`
- **Reads/writes `lmt_*`:** none direct

### `AI_Stacks_Target_Nearest_Hostile_Stack` — [line 2964](../../MoM/src/AIMOVE.c#L2964)
- **drake178:** `AI_FindClosestEnemy()` (o158p14)
- **End:** ~line 3025
- **Purpose:** TBD

### `AI_Find_Opportunity_City_Target` — [line 3032](../../MoM/src/AIMOVE.c#L3032)
- **drake178:** `AI_OppMoveOverride()` (o158p15)
- **End:** ~line 3123
- **Purpose:** TBD

### `AI_Disband_To_Balance_Budget` — [line 3147](../../MoM/src/AIMOVE.c#L3147)
- **drake178:** `AI_Balance_Upkeep()` (o158p16)
- **End:** ~line 3344
- **Purpose:** TBD — likely income/upkeep balancing by unit disbandment

### `AI_Shift_Off_Home_Plane` — [line 3348](../../MoM/src/AIMOVE.c#L3348)
- **drake178:** `AI_ShiftOffHomePlane()` (o158p17)
- **End:** ~line 3420
- **Purpose:** TBD

### `AI_Move_Out_Boats` — [line 3424](../../MoM/src/AIMOVE.c#L3424)
- **drake178:** `AI_Transport_Sailoff()` (o158p18)
- **End:** ~line 3484
- **Purpose:** TBD

### `AI_Stacks_Move_Out_NonMilitary_Garrisoned` — [line 3503](../../MoM/src/AIMOVE.c#L3503)
- **drake178:** `AI_GarrBuilderPush()` (o158p19)
- **End:** ~line 3586
- **Purpose:** Push non-military units (Settlers, Engineers, Melders not on a node) out of Garrison/FortressGarrison stacks to an adjacent free square. See [AIMOVE-AI_Stacks_Move_Out_NonMilitary_Garrisoned.md](AIMOVE-AI_Stacks_Move_Out_NonMilitary_Garrisoned.md).

### `AI_Stacks_Survey_Expedition_Forces` — [line 3610](../../MoM/src/AIMOVE.c#L3610)
- **drake178:** `AI_Survey_Excess_Units()` (o158p20)
- **End:** ~line 3664
- **Purpose:** Dispatch slot 3 — clears the `G_Pushout_*` and `G_Seafaring_*` pools, then iterates `_ai_own_stack_*` and submits qualifying stacks to `AI_Stacks_Survey_Expedition_Forces_Stack`. Per-stack-type submission rules: Unknown/Roamer = all units; Garrison = excess above 5; FortressGarrison = excess above 5 only when `_turn < 100`.

### `AI_Stacks_Survey_Expedition_Forces_Stack` — [line 3697](../../MoM/src/AIMOVE.c#L3697)
- **drake178:** (no drake name) (o158p21)
- **End:** ~line 3911
- **Purpose:** Per-stack scorer — filters to military units (no engineers/settlers/melders/transports), assigns values (OGBUG: bogus random instead of real strength), bubble-sorts, truncates to excess_count, and inserts the top candidates into the `G_Pushout_*` pool (and the `G_Seafaring_*` pool for water-capable units). See [MoM-AI-Move-ai_own_stack.md](MoM-AI-Move-ai_own_stack.md) for the pool structure.

### `AI_Stacks_Do_Meld` — [line 3939](../../MoM/src/AIMOVE.c#L3939)
- **drake178:** `AI_ProcessMelders()` (o158p22)
- **End:** ~line 4067
- **Purpose:** Slot 4 — for each `UA_MELD` unit in an own-stack, find the closest non-owned SITE-flagged node on an eligible landmass and issue a meld order (in place) or goto order (toward it). See [AIMOVE-AI_Do_Meld.md](AIMOVE-AI_Do_Meld.md).
- **Reads `lmt_*`:** `lmt_Own` ([4022](../../MoM/src/AIMOVE.c#L4022)), `lmt_Leaveable` ([4024](../../MoM/src/AIMOVE.c#L4024), `>=`)

### `AI_Stacks_Do_Settle` — [line 4071](../../MoM/src/AIMOVE.c#L4071)
- **drake178:** `AI_ProcessSettlers()` (o158p23)
- **End:** ~line 4275
- **Purpose:** TBD

### `AI_Stacks_Do_Purify` — [line 4309](../../MoM/src/AIMOVE.c#L4309)
- **drake178:** `AI_ProcessPurifiers()` (o158p24)
- **End:** ~line 4415
- **Purpose:** Slot 6 — scout the landmass for a corrupted square near an `AI_TARGET_SITE` (city/node area), then order `UA_PURIFY` units to purify in place or route toward it. See [AIMOVE-AI_Stacks_Do_Purify.md](AIMOVE-AI_Stacks_Do_Purify.md).

### `AI_Do_RoadBuild` — [line 4425](../../MoM/src/AIMOVE.c#L4425)
- **drake178:** `AI_ProcessRoadBuilds()` (o158p25)
- **End:** ~line 4584
- **Purpose:** TBD

### `AI_Stacks_Order_Attack_Target_Or_Goto_Destination` — [line 4614](../../MoM/src/AIMOVE.c#L4614)
- **drake178:** (no drake name) (o158p26) — ReMoM rename of OG `AI_Set_Move_Or_Goto_Target`
- **End:** ~line 4663
- **Purpose:** TBD — shared move/goto order-issuer used by most slot helpers (called with `g_ai_set_target_caller` tag identifying the caller).

### `AI_Order_Settle` — [line 4667](../../MoM/src/AIMOVE.c#L4667)
- **drake178:** `AI_UNIT_SettleOrder()` (o158p27)
- **End:** ~line 4672
- **Purpose:** TBD

### `AI_Order_RoadBuild` — [line 4676](../../MoM/src/AIMOVE.c#L4676)
- **drake178:** `AI_UNIT_RoadBldOrder()` (o158p28)
- **End:** ~line 4685
- **Purpose:** TBD

### `AI_Stacks_Order_Ferry` — [line 4710](../../MoM/src/AIMOVE.c#L4710)
- **drake178:** `AI_UNIT_SeekTransport()` (o158p29)
- **End:** ~line 4716
- **Purpose:** Mark a single unit as waiting for ferry transport — sets `_UNITS[unit_idx].Status = us_Ferry`, stores a "timer" value of 10 in `dst_wx`, consumes the unit's slot in `_ai_own_stack_unit_list`. Bounds-checked against `MAX_UNIT_COUNT` (= 1000). See [AIMOVE-AI_Stacks_Order_Ferry.md](AIMOVE-AI_Stacks_Order_Ferry.md).

### `AI_Stacks_Order_Meld` — [line 4741](../../MoM/src/AIMOVE.c#L4741)
- **drake178:** `AI_UNIT_SetMeldOrder()` (o158p30)
- **End:** ~line 4746
- **Purpose:** Sets `_UNITS[unit_idx].Status = us_Meld` and consumes the stack slot, bounds-checked against `MAX_UNIT_COUNT`. See [AIMOVE-AI_Do_Meld.md](AIMOVE-AI_Do_Meld.md).

### `AI_Stacks_Order_Purify` — [line 4772](../../MoM/src/AIMOVE.c#L4772)
- **drake178:** `AI_UNIT_PurifyOrder()` (o158p31)
- **End:** ~line 4778
- **Purpose:** Sets `_UNITS[unit_idx].Status = us_Purify`, stores a purify timer (20) in `dst_wx`, consumes the stack slot, bounds-checked against `MAX_UNIT_COUNT`. See [AIMOVE-AI_Stacks_Do_Purify.md](AIMOVE-AI_Stacks_Do_Purify.md).

### `Map_Square_Area_Has_Opponent` — [line 4786](../../MoM/src/AIMOVE.c#L4786)
- **drake178:** `TILE_CheckEnemyUnits()` (o158p32)
- **End:** ~line 4822
- **Purpose:** TBD

### `AI_Enemy_Unit_In_Range` — [line 4830](../../MoM/src/AIMOVE.c#L4830)
- **drake178:** `AI_TILE_CheckEnemies()` (o158p33)
- **End:** ~line 4870
- **Purpose:** TBD

### `AI_Find_Tower_To_Settle_Elsewhere` — [line 4900](../../MoM/src/AIMOVE.c#L4900)
- **drake178:** `AI_CanSettleOffPlane__ALWAYS_FALSE()` (o158p34) — empirical name, not structural
- **End:** ~line 4991
- **Purpose:** Tower-hop eligibility check for off-plane settling. Returns TRUE + writes destination coords iff: no Planar Seal anywhere, settler is on home-Fortress plane, settler not currently on a Tower, player owns at least one Tower on the settler's landmass, and the off-plane landmass at that Tower is not `lmt_Contested` or `lmt_NoOwnCityAndAllyHasCity`. Renamed from `__STUB` 2026-05-30 (to `AI_CanSettleOffPlane`), then again 2026-06-03 (to current — describes what it does, not what it queries). See [AIMOVE-AI_Stacks_Do_Settle.md](AIMOVE-AI_Stacks_Do_Settle.md).
- **Reads `lmt_*`:** `lmt_Contested` ([4976](../../MoM/src/AIMOVE.c#L4976)), `lmt_NoOwnCityAndAllyHasCity` ([4981](../../MoM/src/AIMOVE.c#L4981))

### `AI_Stacks_Reorder_Settle_Elsewhere` — [line 4995](../../MoM/src/AIMOVE.c#L4995)
- **drake178:** `AI_SendToColonize()` (o158p35)
- **End:** ~line 5152
- **Purpose:** Branch C-fallback of `AI_Stacks_Do_Settle`. When a settler has no good square on its current landmass, route it to a pre-decided off-landmass colony target (set by `AI_Evaluate_Continents`) via the best available transport method: direct GOTO (seafaring/flying), walk-to-adjacent-transport, queue a ferry request, or walk-to-dock-then-wait. Renamed from `AI_SendToColonize__WIP` 2026-06-03 at done-done. See [AIMOVE-AI_Stacks_Reorder_Settle_Elsewhere.md](AIMOVE-AI_Stacks_Reorder_Settle_Elsewhere.md).

### `AI_Stacks_Ferry_Add_Location` — [line 5173](../../MoM/src/AIMOVE.c#L5173)
- **drake178:** `AI_SeekTransportFrom()` (o158p36)
- **End:** ~line 5200
- **Purpose:** Register a `(wx, wy, wp)` request in the global seek-transport list (`_ai_ferry_*`), with dedup and `MAX_AI_FERRIES` (= 15) capacity guard. See [AIMOVE-AI_Stacks_Ferry_Add_Location.md](AIMOVE-AI_Stacks_Ferry_Add_Location.md).

### `AI_Tower_Target_Worthwhile` — [line 5207](../../MoM/src/AIMOVE.c#L5207)
- **drake178:** `AI_CheckOtherPlane()` (o158p37)
- **End:** ~line 5263
- **Purpose:** TBD — current name differs significantly from drake's `AI_CheckOtherPlane()` — verify alignment
- **Reads `lmt_*`:** `lmt_Own` ([5219](../../MoM/src/AIMOVE.c#L5219)), `lmt_NoOwnCityAndAllyHasCity` ([5221](../../MoM/src/AIMOVE.c#L5221)), `lmt_NoOwnCity` ([5244](../../MoM/src/AIMOVE.c#L5244))

### `AI_Reevaluate_Continent` — [line 5306](../../MoM/src/AIMOVE.c#L5306)
- **drake178:** (no drake name) (o158p38)
- **End:** ~line 5589
- **Purpose:** Per-landmass reclassifier — cancels orders on the landmass, recomputes city center / unit-cost sums, assigns `lmt_*` type, picks stage point
- **Writes `lmt_*`:** `lmt_Own` ([5421](../../MoM/src/AIMOVE.c#L5421)), `lmt_Contested` ([5425](../../MoM/src/AIMOVE.c#L5425)), `lmt_NoOwnCity` ([5430](../../MoM/src/AIMOVE.c#L5430)), `lmt_NoOwnCityAndAllyHasCity` ([5449](../../MoM/src/AIMOVE.c#L5449), [5455](../../MoM/src/AIMOVE.c#L5455), [5456](../../MoM/src/AIMOVE.c#L5456), [5522](../../MoM/src/AIMOVE.c#L5522))
- **Reads `lmt_*`:** `lmt_NoOwnCity` ([5446](../../MoM/src/AIMOVE.c#L5446)), `lmt_Contested` ([5473](../../MoM/src/AIMOVE.c#L5473)), `lmt_Own` ([5475](../../MoM/src/AIMOVE.c#L5475), [5534](../../MoM/src/AIMOVE.c#L5534)), `lmt_NoOwnCityAndAllyHasCity` ([5477](../../MoM/src/AIMOVE.c#L5477))
- **Known issue:** does not zero slot before counting (compare with `AI_Evaluate_Continents`). Stale prior value can spuriously pass the city-count test.

---

## ovr162 — AI Continent (stack/destination management)

### Unreconstructed stubs (o162p01–p11)

Lines [5596-5628](../../MoM/src/AIMOVE.c#L5596-L5628) — WZD/drake markers for 11 unreconstructed disassembly stubs (`sub_F48F0()` through `sub_F5432()`). No function bodies.

### `AI_Stack_Set_Destination` — [line 5638](../../MoM/src/AIMOVE.c#L5638)
- **drake178:** (no drake name at o162p12 — header XREF to `NPC_Destinations`)
- **End:** ~line 5714
- **Purpose:** Per-stack target-destination assignment (called from `NPC_Destinations` for neutral-player city targets per header).
- **Reads/writes `lmt_*`:** none direct

### Unreconstructed stubs (o162p13–p18)

Lines [5717-5733](../../MoM/src/AIMOVE.c#L5717-L5733) — stubs `sub_F57AF()` through `sub_F5EBF()`. No bodies.

### `AI_Stack_Set_Boats_Goto` — [line 5737](../../MoM/src/AIMOVE.c#L5737)
- **drake178:** `AI_STK_MoveTransports()` (o162p19)
- **End:** ~line 5764
- **Purpose:** TBD

### Unreconstructed stub (o162p20)

Line [5767](../../MoM/src/AIMOVE.c#L5767): `sub_F60F4()`. No body.

### `Next_Nearest_Ferry_Square` — [line 5789](../../MoM/src/AIMOVE.c#L5789)
- **drake178:** `TILE_AI_FindLoadTile()` (o162p21)
- **End:** ~line 5849
- **Purpose:** TBD

### Unreconstructed stub (o162p22)

Line [5852](../../MoM/src/AIMOVE.c#L5852): `sub_F6316()`. No body.

### `Adjacent_Land_Square` — [line 5882](../../MoM/src/AIMOVE.c#L5882)
- **drake178:** `TILE_AI_FindEmptyLnd()` (o162p23)
- **End:** ~line 5920
- **Purpose:** TBD. Renamed from `TILE_AI_FindEmptyLnd__WIP` (the `__WIP` is the project's not-yet-reconstructed marker). Called from `AI_Find_Tower_To_Settle_Elsewhere`.

### Unreconstructed stubs (o162p24–p31)

Lines [5923-5945](../../MoM/src/AIMOVE.c#L5923-L5945) — stubs `sub_F64C2()` through `sub_F6EBF()`. No bodies.

### `Adjacent_Free_Square` — [line 5978](../../MoM/src/AIMOVE.c#L5978)
- **drake178:** (no drake name) (o162p32)
- **End:** ~line 6061
- **Purpose:** TBD — find a free square adjacent to `(wx,wy,wp)`; used by `AI_Stacks_Move_Out_NonMilitary_Garrisoned`.

### `Make_Monster_List` — [line 6065](../../MoM/src/AIMOVE.c#L6065)
- **drake178:** (no drake name at o162p33)
- **End:** ~line 6111
- **Purpose:** TBD

### `AI_Stack_Can_Mobilize` — [line 6121](../../MoM/src/AIMOVE.c#L6121)
- **drake178:** (no drake name at o162p34)
- **End:** ~line 6208
- **Purpose:** TBD

### `AI_Evaluate_Continents` — [line 6252](../../MoM/src/AIMOVE.c#L6252)
- **drake178:** `AI_Continent_Reeval()` (o162p35)
- **End:** ~line 6749
- **Purpose:** Full-rebuild classifier for ALL landmasses on ALL planes for one player. Counterpart to per-landmass `AI_Reevaluate_Continent`.
- **Writes `lmt_*`:** `lmt_Own` ([6454](../../MoM/src/AIMOVE.c#L6454)), `lmt_Contested` ([6458](../../MoM/src/AIMOVE.c#L6458)), `lmt_NoOwnCity` ([6463](../../MoM/src/AIMOVE.c#L6463)), `lmt_NoOwnCityAndAllyHasCity` ([6486](../../MoM/src/AIMOVE.c#L6486), [6490](../../MoM/src/AIMOVE.c#L6490), [6491](../../MoM/src/AIMOVE.c#L6491), [6619](../../MoM/src/AIMOVE.c#L6619), [6672](../../MoM/src/AIMOVE.c#L6672))
- **Reads `lmt_*`:** `lmt_NoOwnCity` ([6306](../../MoM/src/AIMOVE.c#L6306), [6484](../../MoM/src/AIMOVE.c#L6484), [6504](../../MoM/src/AIMOVE.c#L6504), [6636](../../MoM/src/AIMOVE.c#L6636)), `lmt_NoTargets` ([6438](../../MoM/src/AIMOVE.c#L6438)), `lmt_Contested` ([6563](../../MoM/src/AIMOVE.c#L6563)), `lmt_Own` ([6565](../../MoM/src/AIMOVE.c#L6565), [6691](../../MoM/src/AIMOVE.c#L6691)), `lmt_NoOwnCityAndAllyHasCity` ([6567](../../MoM/src/AIMOVE.c#L6567))

### `AI_Evaluation_Map` — [line 6778](../../MoM/src/AIMOVE.c#L6778)
- **drake178:** (no drake name at o162p36)
- **End:** ~line 6998
- **Purpose:** TBD — likely populates `g_ai_evaluation_map[][]` (the per-square attribute bitmap used by other AI passes)

### `AI_Choose_War_Landmass` — [line 7002](../../MoM/src/AIMOVE.c#L7002)
- **drake178:** (no drake name at o162p37)
- **End:** ~line 7369
- **Purpose:** Picks "main war continent" per player per plane. Decides per-landmass whether existing assignment should be reevaluated based on type and hostility.
- **Reads `lmt_*`:** switch cases `lmt_Unevaluated` ([7068](../../MoM/src/AIMOVE.c#L7068)), `lmt_Own` ([7073](../../MoM/src/AIMOVE.c#L7073)), `lmt_Contested` ([7078](../../MoM/src/AIMOVE.c#L7078)), `lmt_NoOwnCity` ([7083](../../MoM/src/AIMOVE.c#L7083)), `lmt_NoOwnCityAndAllyHasCity` ([7084](../../MoM/src/AIMOVE.c#L7084)), `lmt_Leaveable` ([7139](../../MoM/src/AIMOVE.c#L7139)); plus filter `lmt_Own` ([7321](../../MoM/src/AIMOVE.c#L7321)), `lmt_NoTargets` ([7323](../../MoM/src/AIMOVE.c#L7323))
- **Known issue:** `lmt_NoTargets` is unhandled in the switch (Gemini groups it with the NoOwnCity case). `lmt_Unevaluated` case is dead (disassembly uses `(type-1)` jump table).

### `Player_Clear_All_Unit_Orders` — [line 7376](../../MoM/src/AIMOVE.c#L7376)
- **drake178:** (no drake name at o162p38)
- **End:** end of file (~line 7394)
- **Purpose:** TBD

---

## How to fill in `TBD` purpose lines

When you touch a function whose purpose is `TBD`, please:

1. Read the header comment block (the `/* ... */` block immediately above the function definition).
2. Replace `TBD` with a single sentence summarizing what the function does.
3. Note any explicit `BUG`, `OGBUG`, `BUGBUG`, or `DEDU` markers in the comment.
4. If the function's role in the AI turn pipeline is non-obvious, add a `Pipeline:` line.
5. If the function reads/writes `lmt_*`, add linked line refs in the same `[NNNN](../../MoM/src/AIMOVE.c#LNNNN)` style as the other entries.

This is intentionally incremental — better to fill in 5 verified entries per session than 49 fabricated ones.

## Related references

- [MoM-AI-Landmass-Types.md](MoM-AI-Landmass-Types.md) — `lmt_*` state machine (writes, reads, transitions)
- [MoM-AI-Move.md](MoM-AI-Move.md) — broader AIMOVE.c overview / call graph
- [MoM-NextTurn-AI.md](../MoM-NextTurn-AI.md) — overall AI turn sequencing
- [MoM-AI-Continents.md](MoM-AI-Continents.md) — continent / landmass data structures
