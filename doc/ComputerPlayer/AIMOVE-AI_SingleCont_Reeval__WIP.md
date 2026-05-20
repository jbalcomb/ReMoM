AIMOVE-AI_SingleCont_Reeval__WIP.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_SingleCont_Reeval__WIP.c

---

# AI_SingleCont_Reeval__WIP — Walkthrough

**Location:** [MoM/src/AIMOVE.c:5913](../../MoM/src/AIMOVE.c#L5913) (~260 lines, ends line 6172)
**WZD overlay:** ovr158, p38
**drake178 name:** (none listed at o158p38)

## Re-review note (lesson from `AI_Choose_War_Landmass` BUG-B)

In a recent rereview of `AI_Choose_War_Landmass` (formerly `AI_Pick_Action_Conts__WIP`), I flagged a "BUG-B" where production diverged from both the OG comment and the GEMINI second opinion. User verified against disassembly: **production matched the OG bytes; the comment and GEMINI were both wrong.** My BUG-B claim was a misdiagnosis.

**Applied to this function:** there is no `__GEMINI` variant for `AI_SingleCont_Reeval__WIP`, so the cross-check pressure is lower. But the outstanding **BUG-7** flag below ("refined stage point computed but never written back") rests on:
- Phase 6's structural similarity to the sibling `AI_Continent_Reeval__WIP` Phase 14, which DOES have the write-back, AND
- The "looks like dead code" semantic intuition.

Neither is disassembly proof. **BUG-7 should be treated as "needs disassembly verification," not as a confirmed translation bug.** The OG bytes may legitimately have no write-back here — perhaps because the per-landmass reeval relies on the immediately-following AI passes to consume `stage_wx`/`stage_wy` from somewhere else, or because the OG dev intentionally left this as a partial computation. Without verification, BUG-7 is a *candidate* for disassembly check, not an established defect.

The flagged-in-source OGBUG comments (OGBUG-A, OGBUG-B, OGBUG-C, OGBUG-D) remain valid because they're explicit OG annotations in the code itself — those are documentation of OG behavior, not my inferences.

## Purpose

**Single-landmass reclassifier + stage-point picker.** Called when a specific landmass on a specific plane needs its `_ai_continents.plane[wp].player[player_idx]` entries refreshed (type + center coords + stage square) — without doing the full all-landmasses rebuild that [`AI_Continent_Reeval__WIP`](../../MoM/src/AIMOVE.c#L6902) does.

The function does six sequential things:

1. **Cancel orders** for every non-engineer unit this player owns on this landmass.
2. **Count owned cities** and sum their coordinates to get a geographic centroid.
3. **Accumulate unit costs** (own and enemy separately) into the same vars used in Phase 2.
4. **Classify** the landmass as `lmt_Own` / `lmt_Contested` / `lmt_NoOwnCity` (then optionally promote to `lmt_NoOwnCityAndAllyHasCity` if an ally has a city here, and force slot-0 sentinel).
5. **Pick a stage square** near the centroid, using random-weighted distance, excluding tiles with "opposition" present.
6. **For `lmt_Own` landmasses only**, refine the stage point further by snapping it to a load square nearest the embark point of the main war continent — but the result is **never written back**.

## When it's called

```c
// AIMOVE.c:248-254 — only caller
void G_AI_RallyFill__WIP(int16_t landmass_idx, int16_t wp, int16_t player_idx)
{
    ...
    if(cp_staged_unit_count == MAX_STACK)
    {
        if(Random(20) == 1)  // 1:20  5% chance
        {
            AI_SingleCont_Reeval__WIP(player_idx, landmass_idx, wp);
        }
    }
    ...
}
```

**Only fires when:** the stage point has a full stack (`MAX_STACK`) AND a 5% random roll succeeds. So this is a probabilistic "stage is full, occasionally reconsider where this continent is going" trigger — not a regular per-turn classifier.

## Signature and locals

```c
void AI_SingleCont_Reeval__WIP(int16_t player_idx, int16_t landmass_idx, int16_t wp)
{
    int16_t square_weight = 0;
    int16_t unit_type = 0;                                    // used in Phase 3 unit-cost loop
    int16_t sum_own_city_wy__enemy_units_cost = 0;            // self-documenting dual-use name
    int16_t sum_own_city_wx__own_units_cost = 0;              // same
    int16_t stage_wy = 0;
    int16_t stage_wx = 0;
    int16_t Tile_Y = 0;
    int16_t Tile_X = 0;                                        // reused in Phase 6 as a load-tile counter
    int16_t square_occupation_value = 0;
    int16_t LoadTile_Y = 0;
    int16_t LoadTile_X = 0;
    int16_t Best_Tile_Weight = 0;
    int16_t Next_Tile_ChainIndex = 0;
    int16_t Center_Y = 0;
    int16_t Center_X = 0;
    int16_t own_city_count = 0;
    int16_t itr = 0;

    EMM_Map_CONTXXX__WIP();  // page the CONTXXX handle into the EMS frame so CONTX_* arrays are accessible
    ...
}
```

**Variable naming notes:**
- `sum_own_city_wx__own_units_cost` and `sum_own_city_wy__enemy_units_cost` carry the dual-use explicitly in their names: they hold both the city coordinate sum (from Phase 2) and the unit cost sum (added on top in Phase 3). The `__` separator makes the dual semantic visible.
- `Tile_X` does double duty: holds a tile X coord in Phases 2-5, then becomes the count of load tiles on the main war continent in Phase 6.
- `stage_wx` / `stage_wy` are the chosen stage-point coordinates (Phase 5 writes them; Phase 6 computes a refinement but never stores it — see BUG-7).
- `square_weight` and `square_occupation_value` reflect the tile-evaluation pass: weight is the picker score (distance + jitter), occupation value comes from `g_ai_evaluation_map[][]`.

## Phase 1 — Cancel orders for all non-engineer units on this landmass

```c
// AIMOVE.c:5936-5950
// ; cancel the orders of every unit on the continent except for engineers
/* OGBUG  should condition on same plane */
for(itr = 0; itr < _units; itr++)
{
    if(
        (_UNITS[itr].owner_idx == player_idx)
        &&
        (_unit_type_table[_UNITS[itr].type].Construction == 0)
        &&
        (_landmasses[((_UNITS[itr].wp * WORLD_SIZE) + (_UNITS[itr].wy * WORLD_WIDTH) + _UNITS[itr].wx)] == landmass_idx)
    )
    {
        _UNITS[itr].Status = us_Ready;
    }
}
```

**Intent:** wipe stale orders so the post-classification re-tasking starts clean. Engineers (`Construction != 0`) are spared because their road-build/purify orders should persist across reevaluations.

### 🐛 OGBUG-A — no same-plane check

The landmass lookup correctly uses the unit's own `.wp/.wy/.wx`. But the lookup returns *which landmass index that tile belongs to on that unit's plane*. If a plane-1 unit happens to be on a landmass whose index also exists on plane 0 (which is common — landmass indices are per-plane and the value space overlaps), the comparison `== landmass_idx` will spuriously pass.

**Example:** caller passes `(wp=0, landmass_idx=5)`. The loop hits a plane-1 unit standing on plane 1's landmass 5. The lookup returns 5. The check passes. We cancel the plane-1 unit's orders even though we're processing the plane-0 landmass.

Flagged at line 5937 as faithful-to-OG.

## Phase 2 — Geographic centroid of owned cities

```c
// AIMOVE.c:5952-5979
/*
    BEGIN:  geographic centroid
*/
_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = 0;       // zero the slot before using as counter
sum_own_city_wx__own_units_cost = 0;
sum_own_city_wy__enemy_units_cost = 0;
for(itr = 0; itr < _cities; itr++)
{
    if(
        (_CITIES[itr].owner_idx == player_idx)
        &&
        (_landmasses[((_CITIES[itr].wp * WORLD_SIZE) + (_CITIES[itr].wy * WORLD_WIDTH) + _CITIES[itr].wx)] == landmass_idx)
    )
    {
        sum_own_city_wx__own_units_cost += _CITIES[itr].wx;                                         // X-coordinate accumulator
        sum_own_city_wy__enemy_units_cost += _CITIES[itr].wy;                                       // Y-coordinate accumulator
        _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] += 1;                  // type_array slot used as city counter
    }
}
own_city_count = _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx];
if(own_city_count > 0)
{
    _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = (sum_own_city_wx__own_units_cost / own_city_count);
    _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = (sum_own_city_wy__enemy_units_cost / own_city_count);
}
/*
    END:  geographic centroid
*/
```

**Intent:** for every owned city on this landmass, accumulate its `(wx, wy)` coordinates and bump a counter (using the type_array slot itself as the counter). After the loop, divide to get the geographic centroid of owned cities — used downstream as the "this is the center of my territory here" point. The slot is zeroed first (so the counter starts at 0 regardless of prior classification).

## Phase 3 — Accumulate unit costs into the same vars

```c
// AIMOVE.c:5981-6003
/* OGBUG  should use separate vars/clear vars beforehand - vars still contain city coordinate sums */
/* Phase 3: Evaluate military strength ratio */
for (itr = 0; itr < _units; itr++)
{
    unit_type = _UNITS[itr].type;
    /* Skip settlers/outpost builders */
    if ((_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) != 0)
    {
        continue;
    }
    /* OGBUG  should condition on same plane */
    if(_landmasses[((_UNITS[itr].wp * WORLD_SIZE) + (_UNITS[itr].wy * WORLD_WIDTH) + _UNITS[itr].wx)] == landmass_idx)
    {
        if (_UNITS[itr].owner_idx == player_idx)
        {
            sum_own_city_wx__own_units_cost += _unit_type_table[unit_type].cost;
        }
        else
        {
            sum_own_city_wy__enemy_units_cost += _unit_type_table[unit_type].cost;
        }
    }
}
```

**Intent:** for every non-settler unit on this landmass, accumulate `_unit_type_table[unit_type].cost` into the own-cost or enemy-cost var. These cost sums are what the classifier compares.

### 🐛 OGBUG-B — coord sums contaminate cost sums

The same `sum_own_city_wx__own_units_cost` and `sum_own_city_wy__enemy_units_cost` vars were used in Phase 2 to accumulate city wx/wy coordinates, and are not cleared between phases. So the values entering the classifier are:
```
sum_own_city_wx__own_units_cost   = (sum of X coords of own cities)  + (total cost of own units on landmass)
sum_own_city_wy__enemy_units_cost = (sum of Y coords of own cities)  + (total cost of enemy units on landmass)
```

Effect is small in mid/late game where unit costs (100s-1000s) dominate coordinate sums (0-59 per city × maybe a handful of cities). More distortion in early game with few/cheap units.

Flagged at line 5981 as faithful-to-OG.

### 🐛 OGBUG-C — no same-plane check

Same issue as Phase 1. Cross-plane units with matching landmass index contribute to the cost sums. Flagged at line 5991.

## Phase 4 — Classify the landmass

```c
// AIMOVE.c:6005-6049
/* Determine continent status based on force presence */
/* if landmass has any cities for this player, replace city count with landmass type/status */
if(_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] > 0)  /* HERE: still 'own city count' */
{
    /* If enemy forces are less than 10% of ours, we own it */
    if((sum_own_city_wy__enemy_units_cost * 10) < sum_own_city_wx__own_units_cost)
    {
        _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_Own;
    }
    else
    {
        _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_Contested;
    }
}
else  /* landmass has no player's cities */
{
    _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_NoOwnCity;
}

// ; if the continent has no own city, but has an allied one, mark it for no landing instead
/* change lmt_NoOwnCity to lmt_NoOwnCityAndAllyHasCity */
for(itr = 0; itr < _cities; itr++)
{
    if(
        (_CITIES[itr].owner_idx != player_idx)
        &&
        (_CITIES[itr].owner_idx != NEUTRAL_PLAYER_IDX)
        &&
        (_landmasses[((_CITIES[itr].wp * WORLD_SIZE) + (_CITIES[itr].wy * WORLD_WIDTH) + _CITIES[itr].wx)] == landmass_idx)
    )
    {
        if(
            (_players[player_idx].Dipl.Dipl_Status[_CITIES[itr].owner_idx] == DIPL_Alliance)
            &&
            (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_NoOwnCity)
        )
        {
            _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_NoOwnCityAndAllyHasCity;
        }
    }
}

_ai_continents.plane[0].player[player_idx].type_array[0] = lmt_NoOwnCityAndAllyHasCity;
_ai_continents.plane[1].player[player_idx].type_array[0] = lmt_NoOwnCityAndAllyHasCity;
```

**Classification rules:**

| Has owned city? | Cost comparison | Then ally-city promotion |
|---|---|---|
| Yes | `(enemy_cost × 10) < own_cost` | → `lmt_Own` |
| Yes | `(enemy_cost × 10) >= own_cost` | → `lmt_Contested` |
| No | (not evaluated) | → `lmt_NoOwnCity`; promoted to `lmt_NoOwnCityAndAllyHasCity` if any allied wizard has a city here |

**Slot-0 force:** unconditionally sets `plane[0]/[1] ... type_array[0]` to `lmt_NoOwnCityAndAllyHasCity` regardless of which landmass is being processed. Slot 0 = ocean / no-landmass sentinel; this defensive write ensures the sentinel always holds the "don't land" marker.

## Phase 5 — Pick stage square near city centroid

```c
// AIMOVE.c:6066-6115
if(
    (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_Contested)
    ||
    (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_Own)
    ||
    (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_NoOwnCityAndAllyHasCity)
)
{
    // DEDU  What's in wx_array,wy_array at this point?
    Center_X = _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx];   // city centroid from Phase 2
    Center_Y = _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx];
    Best_Tile_Weight = 1000;

    // ; find a semi-random empty tile on the continent, if any (weighted by distance + a random value)
    Next_Tile_ChainIndex = _ai_landmass_dock_squares_heads[wp][landmass_idx];
    while(Next_Tile_ChainIndex != ST_UNDEFINED)
    {
        square_weight = (
            Random(5)
            +
            Delta_XY_With_Wrap(CONTX_TileXs[wp][Next_Tile_ChainIndex], CONTX_TileYs[wp][Next_Tile_ChainIndex], Center_X, Center_Y, WORLD_WIDTH)
        );

        // "Best" = lowest because lower distance = closer to the city centroid = better stage point.
        if(square_weight < Best_Tile_Weight)
        {
            Tile_X = CONTX_TileXs[wp][Next_Tile_ChainIndex];                         // ← walks LoadT chain but reads generic Tile arrays
            Tile_Y = CONTX_TileYs[wp][Next_Tile_ChainIndex];
            /* Check if square is occupied by a site or an enemy stack */
            square_occupation_value = g_ai_evaluation_map[wp][((Tile_Y * WORLD_WIDTH) + Tile_X)];
            if(square_occupation_value == 0)  /* none or own */
            {
                Best_Tile_Weight = square_weight;
                stage_wx = CONTX_TileXs[wp][Next_Tile_ChainIndex];
                stage_wy = CONTX_TileYs[wp][Next_Tile_ChainIndex];
            }
        }
        Next_Tile_ChainIndex = _ai_landmass_dock_squares_lists[wp][Next_Tile_ChainIndex];
    }

    /* Update stage-point */
    if(Best_Tile_Weight < 1000)
    {
        _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = (uint8_t)stage_wx;
        _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = (uint8_t)stage_wy;
    }
    else
    {
        // no acceptable tile found — clear stage point and DOWNGRADE the classification
        _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = 0;
        _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = 0;
        _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_NoOwnCityAndAllyHasCity;
    }
}
```

**Intent:** for landmasses where positioning matters (Own / Contested / AllyLand), find a stage square — a load tile near the city centroid that has no occupation per `g_ai_evaluation_map`. The chosen tile becomes the stage point where stacks gather. If no acceptable tile is found, clear the stage point and downgrade to `lmt_NoOwnCityAndAllyHasCity`.

**Why "Best" = lowest:** `square_weight = distance + Random(5)`. Lower weight = closer to city centroid + small jitter. `Best_Tile_Weight` tracks the lowest weight seen for any tile that ALSO has `square_occupation_value == 0`. Initialized to 1000 as a sentinel above any valid weight (max realistic ~54 for a 60×40 world), which doubles as the "did we find anything?" marker after the loop.

### ⚠️ Possible BUG-5 — generic Tile arrays used while walking the LoadT chain

Phase 5 walks `_ai_landmass_dock_squares_lists[wp][...]` (the load-tile chain) but indexes into `CONTX_TileXs[wp][...]` and `CONTX_TileYs[wp][...]` (the generic-tile arrays). Phase 6 (below) walks the same `_ai_landmass_dock_squares_lists` and uses `_ai_landmass_dock_squares_wx_array` / `_ai_landmass_dock_squares_wy_array` consistently. If the load-tile and generic-tile arrays use **different index spaces**, Phase 5 is reading the wrong coordinates.

**Verify whether `CONTX_TileXs`/`Ys` and `_ai_landmass_dock_squares_wx_array`/`Ys` share an index space.** If they do, Phase 5 is just stylistically inconsistent. If they don't, Phase 5's coordinate reads are garbage.

## Phase 6 — Refine stage point for Own landmasses (snap to embark tile near main war continent)

```c
// AIMOVE.c:6118-6164
// ; if the continent has no presence, ensure that the stage point is a valid disembarking tile,
// ; or if that is not possible, re-mark the continent as not landable with a stage point of [0,0]

if(_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_Own)
{
    // ; ensure that the stage point of the continent is a valid embarking tile
    // ; BUG: coming from above, if there is no main war continent, this code will crash with a division by 0
    LoadTile_X = 0;
    LoadTile_Y = 0;
    Tile_X = 0;                                                  // ← reused as a counter
    Next_Tile_ChainIndex = _ai_landmass_dock_squares_heads[wp][_ai_landmass_war_targets[wp][player_idx]];

    // Step A: compute centroid of load tiles on the main war continent
    while(Next_Tile_ChainIndex != ST_UNDEFINED)
    {
        LoadTile_X += _ai_landmass_dock_squares_wx_array[wp][Next_Tile_ChainIndex];
        LoadTile_Y += _ai_landmass_dock_squares_wy_array[wp][Next_Tile_ChainIndex];
        Tile_X++;                                                // ← counter
        Next_Tile_ChainIndex = _ai_landmass_dock_squares_lists[wp][Next_Tile_ChainIndex];
    }

    /* OGBUG  possible division by 0 */
    LoadTile_X /= Tile_X;
    LoadTile_Y /= Tile_X;

    Best_Tile_Weight = 1000;
    Next_Tile_ChainIndex = _ai_landmass_dock_squares_heads[wp][landmass_idx];

    // Step B: walk our landmass's load tiles, find the one closest to that centroid
    while(Next_Tile_ChainIndex != ST_UNDEFINED)
    {
        square_weight = (Delta_XY_With_Wrap(_ai_landmass_dock_squares_wx_array[wp][Next_Tile_ChainIndex], _ai_landmass_dock_squares_wy_array[wp][Next_Tile_ChainIndex], LoadTile_X, LoadTile_Y, WORLD_WIDTH) + Random(5));

        if(square_weight < Best_Tile_Weight)
        {
            Tile_X = _ai_landmass_dock_squares_wx_array[wp][Next_Tile_ChainIndex];
            Tile_Y = _ai_landmass_dock_squares_wy_array[wp][Next_Tile_ChainIndex];

            if(g_ai_evaluation_map[wp][((Tile_Y * WORLD_WIDTH) + Tile_X)] == 0)
            {
                Best_Tile_Weight = square_weight;
                stage_wx = Tile_X;
                stage_wy = Tile_Y;
            }
        }
        Next_Tile_ChainIndex = _ai_landmass_dock_squares_lists[wp][Next_Tile_ChainIndex];
    }
}
```

**Intent:** for landmasses we own decisively, the stage point shouldn't just be near our city center — it should be positioned to ship troops out toward the main war. Step A computes the centroid of load tiles on the main war continent (the "embark destination"). Step B picks our landmass's load tile closest to that centroid.

### 🐛 BUG-7 — Refined stage point computed but never stored

**`stage_wx` / `stage_wy` are NEVER written back to `wx_array` / `wy_array` in Phase 6.** Phase 5's writes are the last ones that take effect. So Phase 6 does the calculation but the result is silently discarded.

Compare with Phase 5, which has the explicit write-back:
```c
// Phase 5 — has the write-back
_ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = (uint8_t)stage_wx;
_ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = (uint8_t)stage_wy;
```

Phase 6 is missing the equivalent. **Verify against disassembly:** is the write-back also missing from the OG? If so, OGBUG. If the OG has the write-back, this is a translation omission.

### 🐛 OGBUG-D — Division by zero risk (already noted in comments)

Lines 6142-6143: `LoadTile_X /= Tile_X` and `LoadTile_Y /= Tile_X`. If the main war continent has no load tiles, `Tile_X` remains 0 and these divisions crash.

The "no main war continent" case is the OG bug noted at line 6127. `_ai_landmass_war_targets[wp][player_idx]` could be 0 (no main war set), in which case `_ai_landmass_dock_squares_heads[wp][0]` is the slot-0 / ocean entry — which likely has no load tiles in its chain — so `Tile_X` ends up 0 and we divide by zero.

## Bug catalog summary

All previously-claimed translation typos have been resolved. Remaining issues are either preserved OG bugs (faithful-to-dasm) or one suspected translation omission:

| # | Severity | Site | Description | Type |
|---|---|---|---|---|
| OGBUG-A | Medium | Line 5937 (Phase 1) | No same-plane check on unit landmass lookup | OG, preserved |
| OGBUG-B | Low | Line 5981 (Phase 3) | Coord sums contaminate cost sums (vars not cleared between Phase 2 and Phase 3) | OG, preserved |
| OGBUG-C | Medium | Line 5991 (Phase 3) | Same cross-plane lookup issue as OGBUG-A | OG, preserved |
| Uncertain | Unknown | Lines 6078-6098 (Phase 5) | Walks LoadT chain but reads generic Tile arrays | Needs disassembly verification |
| OGBUG-D | Known | Lines 6142-6143 (Phase 6) | Division by zero if main war continent has no load tiles | OG, preserved (commented) |
| BUG-7 | Suspected | Phase 6 entirely | Refined stage point computed but `stage_wx`/`stage_wy` never written back to `wx_array`/`wy_array` | **Needs disassembly verification** — sibling has write-back, but post-BUG-B lesson, do not assume production is wrong without checking |

## What the function correctly does post-fix

The classifier produces meaningful `lmt_Own` / `lmt_Contested` / `lmt_NoOwnCity` / `lmt_NoOwnCityAndAllyHasCity` based on real city counts and unit-cost ratios. Downstream consumers (see [MoM-AI-Landmass-Types.md](MoM-AI-Landmass-Types.md) for the eight decisions driven by `lmt_NoOwnCity` alone, plus the stage-point logic) get correctly classified landmasses to work with.

The function is broadly functional — Phase 1 cancels orders, Phase 2 computes a centroid, Phases 3-4 classify, Phase 5 places a stage tile. Phase 6 alone is silently dead (BUG-7).

## Comparison with sibling: `AI_Continent_Reeval__WIP`

| Aspect | `AI_SingleCont_Reeval__WIP` | `AI_Continent_Reeval__WIP` |
|---|---|---|
| Scope | One specific (player, landmass, plane) | All landmasses on both planes for one player |
| Caller | `G_AI_RallyFill__WIP` on 5% random roll when stage is full | (TBD — needs verification) |
| Zeros type_array before counter use? | Yes (line 5955) | Yes (line ~7013) |
| Has unit-cost summing loop? | Yes (lines 5983-6003) | Yes (line ~7141) |
| Uses correct `.wp/.wy/.wx` landmass lookup? | Yes (lines 5945, 5963, 5992, 6033) | Yes |
| Forces slot-0 sentinel? | Yes (lines 6047-6049) | Yes |
| Has stage-tile picker? | Yes (Phases 5-6) | Yes (different code path) |
| Same-plane check on unit/city lookups? | No (OGBUG-A, OGBUG-C) | (Needs verification) |
| Coord/cost var sharing? | Yes (OGBUG-B) | No — explicit clearing between phases (line ~7110) |

The two functions are siblings — `AI_Continent_Reeval__WIP` is the "rebuild everything" full pass, `AI_SingleCont_Reeval__WIP` is the "just this one landmass" focused pass. With the recent fixes, the per-landmass version mirrors the full-rebuild structure closely. The remaining differences are the preserved-OG bugs (cross-plane contamination, coord/cost var sharing) plus the Phase 5 BUG-4 distance defect and the Phase 6 BUG-7 missing write-back.

## Related references

- [MoM-AI-Landmass-Types.md](MoM-AI-Landmass-Types.md) — `lmt_*` enum state machine; this function's writes feed into the Tier-1 classifier outputs
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index entry
- [AIMOVE-AI_Choose_War_Landmass](AIMOVE-AI_AI_Choose_War_Landmass sibling function notes; `_ai_landmass_war_targets[]` cross-reference
- `AI_Continent_Reeval__WIP` at [AIMOVE.c:6902](../../MoM/src/AIMOVE.c#L6902) — full-rebuild counterpart
