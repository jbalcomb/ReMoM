AIMOVE-AI_Stacks_Wartime_Ocean_Movement_And_Cleanup.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Stacks_Wartime_Ocean_Movement_And_Cleanup.c

---

# AI_Stacks_Wartime_Ocean_Movement_And_Cleanup — Walkthrough

**Location:** [MoM/src/AIMOVE.c:929](../../MoM/src/AIMOVE.c#L929) (~183 lines, ends [line 1111](../../MoM/src/AIMOVE.c#L1111)). Function-header docstring (Doxygen) at [lines 892-928](../../MoM/src/AIMOVE.c#L892-L928).
**WZD overlay:** ovr158, p04
**drake178 name:** `AI_ProcessOcean()`
**GEMINI variant:** [`AI_ProcessOcean__GEMINI`](../../MoM/src/AIMOVE.c#L1113) at line 1113 (kept inline for cross-reference per "GEMINI is not ground truth" — see Production vs GEMINI section below).

## Purpose

Per-(player, plane) **post-pass** that runs once after the inner per-landmass loop closes. Two responsibilities, despite the singular "ProcessOcean" name (both are actually ocean-focused — see Phase 3):

1. **Pull mobile-on-water units from the OCEAN toward the main war landmass.** For each us_Ready ocean-stationed unit (this plane, this player) that has AirTravel / WaterTravel / NonCorporeal — and is neither a melder nor a transport — set Status = `us_GOTO` toward a stage point.
2. **Drown stranded ocean stacks.** Rebuild own-stacks for **landmass 0 (ocean)**, then for each ocean stack that has neither a busy unit (slot-1's `ST_UNDEFINED` marker for `us_GOTO`/`us_Purify`/`us_BuildRoad`) nor a unit with water mobility, `Kill_Unit` every player-owned unit on that square.

Both halves operate on ocean squares — Phase 3 acts on individual ocean-stationed units (`_landmasses[...] == 0`), Phase 5 acts on ocean stacks (built via `landmass_idx = 0`).

Called once per plane from [`AI_Set_Unit_Orders`](AIMOVE-AI_Set_Unit_Orders.md), AFTER the per-landmass inner dispatch loop ends — not a slot in the 1-13 sequence. Sibling of [`AI_Stacks_Ocean_Landmass_Orders`](../../MoM/src/AIMOVE.c#L1195) which runs immediately after at [line 316](../../MoM/src/AIMOVE.c#L316).

## Signature

```c
void AI_Stacks_Wartime_Ocean_Movement_And_Cleanup(int16_t player_idx, int16_t wp)
```

Called from:
- Production dispatcher: [AIMOVE.c:315](../../MoM/src/AIMOVE.c#L315) — `AI_Stacks_Wartime_Ocean_Movement_And_Cleanup(player_idx, wp);  /* ¿ only for war landmass ? */`
- GEMINI dispatcher: [AIMOVE.c:417](../../MoM/src/AIMOVE.c#L417)

The trailing comment at line 315 is the user's question to self — the gate at line 949 is internal, not at the call site. The function is **always called** per (player, plane); it returns early itself when there's no main war landmass.

## Inputs (globals read)

| Source | Fields |
|---|---|
| `_ai_landmass_war_targets[wp][player_idx]` | War-landmass index — gate at line 949; centroid lookup at 958-959 |
| `_ai_continents.plane[wp].player[player_idx].wx_array[idx]` / `wy_array[idx]` | War-landmass centroid (Phase 2 primary); fallback-loop coordinates (Phase 2 fallback) |
| `_ai_continents.plane[wp].player[player_idx].type_array[idx]` | Fallback-loop filter — accepts `lmt_Contested` / `lmt_NoOwnCity` |
| `_FORTRESSES[player_idx].{wx, wy}` | Fortress position — distance origin for fallback stage selection |
| `NUM_LANDMASSES` (= 100) | Loop bound for fallback stage scan |
| `_units` | Loop bound for Phase 3 unit scan and Phase 5c Kill_Unit scan |
| `_UNITS[itr].{owner_idx, wp, wx, wy, type, Status}` | Per-unit filter in Phase 3 and Phase 5c |
| `_landmasses[wp*WORLD_SIZE + wy*WORLD_WIDTH + wx]` | Phase 3 ocean-membership filter (zero = ocean, non-zero = land) |
| `_unit_type_table[type].{Abilities, Transport}` | Phase 3 melder/transport exclusion |
| `Unit_Has_AirTravel`, `Unit_Has_WaterTravel`, `Unit_Has_NonCorporeal` | Phase 3 mobility filter; Phase 5b seafarer detection |
| `_ai_own_stack_count` | Phase 5 stack loop bound (refreshed by the `AI_Stacks_Init_Build_Target_Order` call at 1041) |
| `_ai_own_stack_wx[s]`, `_ai_own_stack_wy[s]` | Phase 5c stack position cache |
| `_ai_own_stack_unit_count[s]` | Phase 5b per-stack present-count |
| `_ai_own_stack_unit_list[s][u]` | Phase 5b per-stack unit-index — `ST_UNDEFINED` sentinel marks units that were in busy Status (us_GOTO/us_Purify/us_BuildRoad) at slot-1 build time (Phase 4) |
| `Delta_XY_With_Wrap(...)` | Fortress-distance helper |
| `WORLD_WIDTH`, `WORLD_SIZE` | Toroidal-distance and landmass-array constants |

## Outputs (side effects)

- **Phase 3** writes `_UNITS[itr].Status = us_GOTO` and `_UNITS[itr].dst_wx`/`dst_wy` for each ocean-stationed mobile-on-water unit.
- **Phase 4** calls [`AI_Stacks_Init_Build_Target_Order(player_idx, 0, wp)`](../../MoM/src/AIMOVE.c#L1041) — repopulates `_ai_own_stack_*` for the OCEAN (landmass 0).
- **Phase 5c** calls `Kill_Unit(itr, kt_Normal)` for every player-owned unit on this plane standing at the wx/wy of any qualifying stranded ocean stack.

## Locals

```c
int16_t stack_wy = 0;                       /* Phase 5c per-stack y cache */
int16_t stack_wx = 0;                       /* Phase 5c per-stack x cache */
int16_t list_unit_count = 0;                /* Phase 5a _ai_own_stack_unit_count[itr_stacks] cache */
int16_t itr_list_units = 0;                 /* Phase 5b inner unit-list iterator */
int16_t itr_stacks = 0;                     /* Phase 5 stack iterator */
int16_t stack_has_seafaring_unit = 0;       /* Phase 5b per-stack flag — TRUE when any unit in the stack has AirTravel/WaterTravel/NonCorporeal */
int16_t stack_has_busy_unit = 0;            /* Phase 5b per-stack flag — TRUE when stack contains a unit in busy Status (us_GOTO/us_Purify/us_BuildRoad) at slot-1 build time (the ST_UNDEFINED marker source — see Phase 5 prose) */
int16_t fortress_wy = 0;                    /* Phase 2 fortress y cache */
int16_t fortress_wx = 0;                    /* Phase 2 fortress x cache */
int16_t min_delta_distance = 0;             /* Phase 2 fallback scan — running-min distance */
int16_t delta_distance = 0;                 /* Phase 2 fallback scan — current candidate distance */
int16_t stage_wy = 0;                       /* Phase 2 stage point y */
int16_t stage_wx = 0;                       /* Phase 2 stage point x */
int16_t unit_idx = 0;                       /* Phase 5b _ai_own_stack_unit_list[...] cache */
int16_t itr = 0;                            /* Outer iterator, reused across Phase 2 (landmass), Phase 3 (unit), Phase 5c Kill_Unit scan (unit) */
```

## Code walk

### Phase 1 — War-target gate ([lines 948-952](../../MoM/src/AIMOVE.c#L948-L952))

```c
/* Phase 1:  No War, bail. */
if(_ai_landmass_war_targets[wp][player_idx] == 0)
{
    return;
}
```

If [`AI_Choose_War_Landmass`](AIMOVE-AI_Choose_War_Landmass.md) didn't pick a main war landmass for this player on this plane, do nothing. No stage point → no purpose for this function.

### Phase 2 — Determine stage point ([lines 955-985](../../MoM/src/AIMOVE.c#L955-L985))

```c
/* ¿ war landmass geographic centroid ? */
stage_wx = _ai_continents.plane[wp].player[player_idx].wx_array[_ai_landmass_war_targets[wp][player_idx]];
stage_wy = _ai_continents.plane[wp].player[player_idx].wy_array[_ai_landmass_war_targets[wp][player_idx]];

fortress_wx = _FORTRESSES[player_idx].wx;
fortress_wy = _FORTRESSES[player_idx].wy;

/* ¿ doesn't make sense, cause must be no war ? */
if((stage_wx == 0) && (stage_wy == 0))
{
    min_delta_distance = 1000;
    for(itr = 0; itr < NUM_LANDMASSES; itr++)
    {
        if(
            (_ai_continents.plane[wp].player[player_idx].type_array[itr] == lmt_Contested)
            ||
            (_ai_continents.plane[wp].player[player_idx].type_array[itr] == lmt_NoOwnCity)
        )
        {
            delta_distance = Delta_XY_With_Wrap(fortress_wx, fortress_wy,
                                                _ai_continents.plane[wp].player[player_idx].wx_array[itr],
                                                _ai_continents.plane[wp].player[player_idx].wy_array[itr],
                                                WORLD_WIDTH);
            if(delta_distance < min_delta_distance)
            {
                min_delta_distance = delta_distance;
                stage_wx = _ai_continents.plane[wp].player[player_idx].wx_array[itr];
                stage_wy = _ai_continents.plane[wp].player[player_idx].wy_array[itr];
            }
        }
    }
}
```

**Primary stage point:** read the war landmass's `wx_array`/`wy_array` entry — the per-landmass "stage point" that the continent evaluator wrote during this turn's per-landmass pass.

**Fallback:** if the war landmass has zero stage-point coords (sentinel for "no usable point chosen"), pick the closest `lmt_Contested` or `lmt_NoOwnCity` landmass on this plane to the player's fortress. `min_delta_distance = 1000` is the "no candidate found" sentinel — `Delta_XY_With_Wrap` over a 60-wide world maxes at well under 1000.

The fallback's landmass-type filter is asymmetric with the consumer: any landmass tagged `Contested` (we have cities here and so does an enemy) or `NoOwnCity` (no cities here, an enemy or neutral does) is treated as a viable destination — the goal is to seed Phase 3's pull-direction with SOMETHING on the plane, even if it's not the originally-chosen war landmass.

In OG/asm, `lmt_NoOwnCity` is named `lmt_NoPresence` — same enum value (3), just renamed during reconstruction.

### Phase 3 — Pull mobile-on-water units (ocean → stage) ([lines 988-1036](../../MoM/src/AIMOVE.c#L988-L1036))

```c
/* If a stage point was found, assign all idle individual seafarers to go there */
if((stage_wx != 0) || (stage_wy != 0))
{
    for(itr = 0; itr < _units; itr++)
    {
        if(_UNITS[itr].owner_idx != player_idx) { continue; }
        if(_UNITS[itr].wp != wp) { continue; }
        if(_landmasses[((wp * WORLD_SIZE) + (_UNITS[itr].wy * WORLD_WIDTH) + _UNITS[itr].wx)] != 0)  /* 0 is Ocean landmass */
        {
            continue;
        }
        if(_UNITS[itr].Status != us_Ready) { continue; }
        if((_unit_type_table[_UNITS[itr].type].Abilities & UA_MELD) != 0) { continue; }
        if(_unit_type_table[_UNITS[itr].type].Transport != 0) { continue; }
        /* If the Unit can move on the Ocean, send it on it's way */
        if(
            (Unit_Has_AirTravel(itr) != ST_FALSE)
            ||
            (Unit_Has_WaterTravel(itr) != ST_FALSE)
            ||
            (Unit_Has_NonCorporeal(itr) != ST_FALSE)
        )
        {
            _UNITS[itr].Status = us_GOTO;
            _UNITS[itr].dst_wx = (int8_t)stage_wx;
            _UNITS[itr].dst_wy = (int8_t)stage_wy;
        }
    }
}
```

Scan every unit. Take the order-issuing branch only if **all** of:

1. Owned by this AI player, on this plane.
2. **Standing in the ocean** — `_landmasses[...] == 0` (any land square hashes to a non-zero landmass index, which is the skip condition here).
3. `Status == us_Ready` — hasn't been ordered already this turn.
4. NOT a melder (`UA_MELD` clear) — settlers/melders belong to slot 4-7's overlay logic.
5. NOT a transport (`Transport == 0`) — boats go through [`AI_Stacks_Ocean_Landmass_Orders`](../../MoM/src/AIMOVE.c#L1195) next.
6. Has AirTravel OR WaterTravel OR NonCorporeal — units that can cross ocean WITHOUT needing a transport.

Then set `Status = us_GOTO` toward the stage point.

**Why ocean-only?** This phase handles units that are floating in the ocean under their own power (swimming, flying, or non-corporeal) and don't yet have an order. They get pointed at the war stage so they actually do something instead of drifting. Stranded non-mobile units in the ocean get handled by Phase 5 (drowned). Land-stationed units belong to per-landmass slot 10 ([`AI_Stacks_Order_To_War_Landmass`](AIMOVE-AI_Stacks_Order_To_War_Landmass.md)), not here.

**Why a sweep over `_UNITS[]` directly?** The per-landmass dispatch loop runs slot 1's `AI_Stacks_Init_Build_Target_Order` for landmass_idx 1..99, never for landmass 0. So ocean-stationed units never appear in `_ai_own_stack_*` until Phase 4 below builds them. By that point we want a fresh order-assignment for any swimmer/flier already at sea — done here via direct `_UNITS[]` iteration before the Phase 4 rebuild.

### Phase 4 — Rebuild own-stacks for the ocean ([line 1041](../../MoM/src/AIMOVE.c#L1041))

```c
/* Phase 4: make AI stack data for the Ocean */
AI_Stacks_Init_Build_Target_Order(player_idx, 0, wp);
```

`landmass_idx = 0` rebuilds `_ai_own_stack_*` with **all of this player's stacks currently on the ocean** of this plane. After this call, the `_ai_own_stack_*` parallel arrays describe ocean-only stacks for the duration of Phase 5. (Nowhere else in the dispatch chain does any function build for landmass 0.)

### Phase 5 — Cull stranded ocean stacks ([lines 1044-1109](../../MoM/src/AIMOVE.c#L1044-L1109))

Source structures this as three sub-phases per stack:

```c
for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
{
    /* Phase 5a: (Re-)Init */
    stack_has_busy_unit = ST_FALSE;
    stack_has_seafaring_unit = ST_FALSE;
    list_unit_count = _ai_own_stack_unit_count[itr_stacks];

    /* Phase 5b: Collect our Decision Criteria */
    for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
    {
        /* OGBUG  should evaluate the rest of the units in the stack - misses any more invalid units */
        if(stack_has_busy_unit != ST_FALSE) { break; }
        unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
        if(unit_idx == ST_UNDEFINED)
        {
            stack_has_busy_unit = ST_TRUE;
        }
        else
        {
            if(
                (Unit_Has_AirTravel(unit_idx) != ST_FALSE)
                ||
                (Unit_Has_WaterTravel(unit_idx) != ST_FALSE)
                ||
                (Unit_Has_NonCorporeal(unit_idx) != ST_FALSE)
            )
            {
                stack_has_seafaring_unit = ST_TRUE;
            }
        }
    }

    /* Phase 5c: Invalid State, Destory the Unit */
    if((stack_has_busy_unit == ST_FALSE) && (stack_has_seafaring_unit == ST_FALSE))
    {
        stack_wx = _ai_own_stack_wx[itr_stacks];
        stack_wy = _ai_own_stack_wy[itr_stacks];
        for(itr = 0; itr < _units; itr++)
        {
            if(
                (_UNITS[itr].wx == stack_wx)
                && (_UNITS[itr].wy == stack_wy)
                && (_UNITS[itr].wp == wp)
                && (_UNITS[itr].owner_idx == player_idx)
            )
            {
                Kill_Unit(itr, kt_Normal);
            }
        }
    }
}
```

**Per-stack two-flag classification (Phase 5a/5b):**

- `stack_has_busy_unit` — set TRUE when an `ST_UNDEFINED` slot is encountered. Slot 1 (Phase 4's call) writes `ST_UNDEFINED` for any unit whose Status is `us_GOTO`, `us_Purify`, or `us_BuildRoad` at build time — i.e., a unit already heading somewhere or already doing a multi-turn job. On the ocean, `us_GOTO` is the only realistic source (Purify/BuildRoad need land terrain). So this flag means: "stack contains at least one unit already heading to a destination — leave it alone."
- `stack_has_seafaring_unit` — set TRUE when any unit in the stack has air/water/non-corporeal travel. Meaning: the stack can self-propel out of the ocean. Leave it alone.

**Note on `ST_UNDEFINED` semantics:** in other dispatch contexts (per-landmass slots 2-13), `ST_UNDEFINED` in `_ai_own_stack_unit_list[]` marks slots consumed by an earlier order-issuer (e.g., `AI_Stacks_Order_Attack_Target_Or_Goto_Destination` writes it after issuing an order). That meaning does NOT apply here. Phase 4 calls `AI_Stacks_Init_Build_Target_Order(player_idx, 0, wp)` at line 1004, which wipes the entire `_ai_own_stack_*` table ([lines 3068-3077](../../MoM/src/AIMOVE.c#L3068-L3077)) and rebuilds it from scratch. Any prior-turn or prior-slot consumption is gone before Phase 5 looks. The `ST_UNDEFINED` here is exclusively from slot 1's busy-Status marking ([lines 3149/3185](../../MoM/src/AIMOVE.c#L3149)).

**OGBUG at the top of the 5b loop (line [1056](../../MoM/src/AIMOVE.c#L1056)):** the first sentinel hit short-circuits the rest of the scan via the deferred-break shape (set flag → bottom of body → top-of-loop tests flag). Any seafarers later in the unit-list never get checked, and `stack_has_seafaring_unit` stays FALSE for that scan. But this never matters for the cull, because the cull also requires `stack_has_busy_unit == FALSE` — and that flag is exactly what fires the early-break. So the early-break only fires on stacks that the cull would skip anyway. The OGBUG is real (the scan exits before the work it set out to do) but has no path to affect Phase 5c. **Confirmed OGBUG-faithful per disassembly** (asm `loc_ECBC8`: top-of-loop tests `Have_Assigned_Unit` for jump-back).

**Cull condition (Phase 5c):** neither flag set → no unit in the stack is heading somewhere AND no unit can leave the ocean square on its own → the stack is stranded. `Kill_Unit` every player-owned unit on the stack's square.

This is the in-game "your units drown when their transport sinks" behavior, but applied at AI-decision time rather than at world-event time. A stack on an ocean square with no boat under it and no swimmer among its members has no future — the AI removes them so they don't clutter the unit list with permanently-immobile entries.

## Production vs GEMINI

The GEMINI variant ([`AI_ProcessOcean__GEMINI`](../../MoM/src/AIMOVE.c#L1077)) is kept inline for cross-reference. Per [feedback_gemini_is_not_ground_truth](../../C:/Users/jbalcomb/.claude/projects/c--STU-devel-ReMoM/memory/feedback_gemini_is_not_ground_truth.md): GEMINI is a second opinion, not ground truth — when production and GEMINI diverge, the disassembly bytes decide.

Remaining divergences after IDA verification:

| # | Production behavior | GEMINI behavior | IDA verdict |
|---|---|---|---|
| **D2** | Phase 5b deferred-break: set `stack_has_busy_unit = ST_TRUE`, fall to inc, top-of-loop test exits. | GEMINI breaks immediately on sentinel (line 1185). | **Production is OG-faithful.** Asm `loc_ECB94` sets flag then `jmp loc_ECBC5` (inc); `loc_ECBC8` tests the flag for jump-back. GEMINI's immediate `break` is a shape rewrite that produces the same observable. |
| D4 (cosmetic) | Production uses `_ai_continents.plane[wp].player[player_idx].*` accessor | GEMINI uses `_ai_continents[wp].Player[player_idx].*` (incorrect — flat indexing of a struct) | GEMINI's accessor is wrong shape (won't compile). Production matches OG and current MOM_DAT.h struct definitions. |

Previously-flagged divergences that resolved when source caught up to OG:

- **B1 / D1 (`itr` vs `unit_idx` at Phase 5b seafarer check)** — IDA confirmed OG uses `unit_idx` (asm 391/396/401 push `[bp+unit_idx]`). Earlier production state with `itr` was a transcription error; current source matches OG. No `/* CLAUDE */` marker warranted.
- **Phase 3 land vs ocean filter** — IDA confirmed OG is ocean-only (asm 270: `cmp [byte ptr es:bx], 0; jz loc_ECA83` — jump to processing on ZERO = ocean). Earlier production state with land-filter was a transcription error inverting the JZ. Current source matches OG. No `/* CLAUDE */` marker warranted.

## Bug catalog

| # | Where | Issue | Severity |
|---|---|---|---|
| B1 | [Line 1056](../../MoM/src/AIMOVE.c#L1056) | OGBUG comment: `should evaluate the rest of the units in the stack - misses any more invalid units`. Phase 5b deferred-break short-circuits the unit-list scan once any busy slot is hit. Effect on Phase 5c: NONE — the cull also requires `stack_has_busy_unit == FALSE`, so the early-break only fires on stacks the cull would skip anyway. The OGBUG is a real scan-exits-early bug, but it has no path to affect Kill_Unit behavior. IDA-confirmed OG-faithful. | Behavioral; OG-faithful; no Phase 5c impact |
| B2 | [Line 1104](../../MoM/src/AIMOVE.c#L1104) | Phase 5c `Kill_Unit` blast radius kills every (owner, plane, wx, wy)-matching unit — no Status/type/ability filter. The cull is safe by structural invariant of slot 1's build: `AI_Stacks_Init_Build_Target_Order` inclusion filters are owner + plane + landmass only ([lines 3084-3113](../../MoM/src/AIMOVE.c#L3084-L3113)), so every player-owned unit at the ocean square is in the stack's unit_list. Free units are evaluated by the seafarer check; busy units (ST_UNDEFINED) trip `stack_has_busy_unit = TRUE` and bypass the cull. Both flags FALSE ⇒ all units at the square are free AND non-swimming ⇒ exactly what the cull should drown. **Caveat:** the MAX_AI_STACKS overflow guard at [line 3087](../../MoM/src/AIMOVE.c#L3087) `continue`s on overflow, so once the stacks table fills mid-build, later units at later squares (including existing-stack additions) get skipped entirely — leaving stacks that exist but are incomplete. In that case the cull could over-kill un-scanned units. Rare with MAX_AI_STACKS = 100. The concern is also flagged inline at [line 1089](../../MoM/src/AIMOVE.c#L1089) as `¿ OGBUG  ... killing Seafaring Units that might still be on this square ?` — same concern, kept visible in source. IDA-confirmed OG-faithful. | None except MAX_AI_STACKS overflow edge case |

## ASCII summary

```
AI_Stacks_Wartime_Ocean_Movement_And_Cleanup(player_idx, wp)
  ├─ Phase 1: if _ai_landmass_war_targets[wp][player_idx] == 0: return
  ├─ Phase 2: stage point
  │    ├─ primary: war landmass centroid (wx_array/wy_array)
  │    └─ fallback (if centroid == 0,0):
  │         └─ closest-to-fortress lmt_Contested or lmt_NoOwnCity landmass
  ├─ Phase 3: pull ocean-stationed mobile-on-water units toward stage
  │    └─ for each _UNITS[i]:
  │         filter: own + plane + IN OCEAN + us_Ready + non-melder + non-transport
  │         filter: AirTravel OR WaterTravel OR NonCorporeal
  │         → Status = us_GOTO, dst = stage
  ├─ Phase 4: AI_Stacks_Init_Build_Target_Order(player_idx, 0, wp)  /* rebuild for OCEAN */
  └─ Phase 5: cull stranded ocean stacks
       └─ for each ocean stack:
            ├─ 5a: init flags = FALSE
            ├─ 5b: scan unit_list:
            │       ├─ if stack_has_busy_unit: break (top of next iter)  [OGBUG B1]
            │       ├─ if ST_UNDEFINED: stack_has_busy_unit = TRUE
            │       └─ else if AirTravel/WaterTravel/NonCorporeal: stack_has_seafaring_unit = TRUE
            └─ 5c: if neither flag set:
                 └─ Kill_Unit every player-owned unit on this plane at stack's wx/wy
```

## Position in the dispatch chain

```
AI_Set_Unit_Orders(player_idx)
  └─ for wp in [0, 1]:                                # per-plane outer loop
       ├─ for landmass_idx in [1, NUM_LANDMASSES):     # per-landmass inner loop
       │    ├─ slot 1: AI_Stacks_Init_Build_Target_Order
       │    ├─ slot 2: AI_Stacks_Move_Out_NonMilitary_Garrisoned
       │    ├─ slot 3: AI_Stacks_Survey_Expedition_Forces
       │    ├─ slot 4-7: AI_Do_Meld / AI_Do_Settle / AI_Do_Purify / AI_Do_RoadBuild
       │    ├─ slot 8: AI_Build_Target_List
       │    ├─ slot 9: AI_Stacks_Roamers_Target_Or_Deploy
       │    ├─ slot 10: AI_Stacks_Order_To_War_Landmass        [gated]
       │    ├─ slot 11: AI_Stacks_Relocate_Roamers             [gated]
       │    ├─ slot 12: AI_Stacks_Stage_Expedition_Forces
       │    └─ slot 13: AI_Stacks_Garrison_Sites               [gated]
       │
       ├─ AI_Stacks_Wartime_Ocean_Movement_And_Cleanup(player_idx, wp)         ◄── HERE (per-plane post-pass)
       └─ AI_Stacks_Ocean_Landmass_Orders(player_idx, wp)         (per-plane post-pass)
```

**Pairing with the dispatch:**

- **Phase 3** is a final per-plane sweep over `_UNITS[]` for swimmers/fliers already in the ocean. Slot 10 ([`AI_Stacks_Order_To_War_Landmass`](AIMOVE-AI_Stacks_Order_To_War_Landmass.md)) handles the equivalent for LAND-stationed units via `_ai_own_stack_*`; this handles ocean-stationed equivalents that wouldn't appear in any per-landmass `_ai_own_stack_*` (since per-landmass slot 1 never builds for landmass 0).
- **Phase 4** is the only place in the dispatch chain that rebuilds `_ai_own_stack_*` for `landmass_idx = 0` (ocean). Slots 1-13 only ever build for `landmass_idx >= 1`.
- **`AI_Stacks_Ocean_Landmass_Orders`** runs next ([line 316](../../MoM/src/AIMOVE.c#L316)) — it handles transport ships specifically, while this function handles non-transport units stranded at sea.

## Related references

- [AIMOVE-AI_Set_Unit_Orders.md](AIMOVE-AI_Set_Unit_Orders.md) — dispatcher; this function is a per-plane post-pass, not a slot in 1-13
- [AIMOVE-AI_Stacks_Order_To_War_Landmass.md](AIMOVE-AI_Stacks_Order_To_War_Landmass.md) — slot 10; the per-landmass equivalent of Phase 3 for land-stationed units (works through `_ai_own_stack_*` instead of `_UNITS[]`)
- [AIMOVE-AI_Stacks_Init_Build_Target_Order.md](AIMOVE-AI_Stacks_Init_Build_Target_Order.md) — slot 1; this function calls it with `landmass_idx = 0` for the ocean build
- [AIMOVE-AI_Choose_War_Landmass.md](AIMOVE-AI_Choose_War_Landmass.md) — populates `_ai_landmass_war_targets[wp][player_idx]` that gates Phase 1
- `AI_Stacks_Ocean_Landmass_Orders` at [AIMOVE.c:1195](../../MoM/src/AIMOVE.c#L1195) — sibling per-plane post-pass for transport ships
- [MoM-AI-Move-ai_own_stack.md](MoM-AI-Move-ai_own_stack.md) — `_ai_own_stack_*` area-of-code reference
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Stacks_Wartime_Ocean_Movement_And_Cleanup.asm` — IDA Pro 5.5 disassembly (ground truth)
