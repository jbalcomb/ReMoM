AIMOVE-AI_Reevaluate_Continent.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_SingleCont_Reeval__WIP.c

---

# AI_Reevaluate_Continent — Walkthrough

**Location:** [MoM/src/AIMOVE.c:5753](../../MoM/src/AIMOVE.c#L5753) (~275 lines, ends line 6027). GEMINI variant at [line 6029](../../MoM/src/AIMOVE.c#L6029).
**WZD overlay:** ovr158, p38
**drake178 name:** (none listed at o158p38)

## Purpose

**Single-landmass reclassifier + stage-point picker.** Refreshes one (player, plane, landmass) entry in `_ai_continents` (type + center coords + stage square) without doing the full all-landmasses rebuild that [`AI_Evaluate_Continents`](AIMOVE-AI_Evaluate_Continents.md) does.

The function does six sequential phases (the source code labels them `/* Phase 1: */` through `/* Phase 6: */`):

1. **EMM mapping** — page the CONTXXX data handle into the EMS frame so `_ai_landmass_*` arrays are accessible.
2. **Cancel orders** for every non-engineer unit this player owns on this landmass.
3. **Geographic centroid** — count owned cities and sum their coordinates.
4. **Classify** the landmass — accumulate unit costs (own and enemy), then assign `lmt_Own` / `lmt_Contested` / `lmt_NoOwnCity`, optionally promote to `lmt_NoOwnCityAndAllyHasCity`, and force slot-0 sentinel.
5. **Pick stage square** near the territory centroid (random-weighted distance, excludes occupied squares). Downgrade classification if no acceptable square exists.
6. **For `lmt_Own` landmasses only**, refine the stage point toward the main-war landmass's dock-square centroid, and write back the chosen square.

## When it's called

```c
// AIMOVE.c:238-252 — only caller
void AI_Stacks_Stage_Expedition_Forces(int16_t landmass_idx, int16_t wp, int16_t player_idx)
{
    ...
    if(cp_staged_unit_count == MAX_STACK)
    {
        if(Random(20) == 1)  // 1:20  5% chance
        {
            AI_Reevaluate_Continent(player_idx, landmass_idx, wp);
        }
    }
    ...
}
```

**Only fires when:** the stage point has a full stack (`MAX_STACK`) AND a 5% random roll succeeds. So this is a probabilistic "stage is full, occasionally reconsider where this continent is going" trigger — not a regular per-turn classifier.

## Signature and locals

```c
void AI_Reevaluate_Continent(int16_t player_idx, int16_t landmass_idx, int16_t wp)
{
    int16_t delta_distance = 0;                            // per-candidate distance (Phase 5/6 picker)
    int16_t unit_type = 0;                                 // _UNITS[itr].type cache (Phase 4)
    int16_t sum_own_city_wy__enemy_units_cost = 0;         // dual-use: city wy sum (Phase 3) + enemy unit cost (Phase 4)
    int16_t sum_own_city_wx__own_units_cost = 0;           // dual-use: city wx sum (Phase 3) + own unit cost (Phase 4)
    int16_t target_square_wy = 0;                          // chosen target square Y (written to wx_array/wy_array by Phase 5 and Phase 6)
    int16_t target_square_wx = 0;                          // chosen target square X
    int16_t landmass_node_wy = 0;                          // current chain node's Y
    int16_t landmass_node_wx = 0;                          // current chain node's X
    int16_t square_occupation_value = 0;                   // g_ai_evaluation_map read; 0 means unoccupied
    int16_t landmass_node_centroid_wy = 0;                 // Phase 6: centroid Y of dock squares on war landmass
    int16_t landmass_node_centroid_wx = 0;                 // Phase 6: centroid X of dock squares on war landmass
    int16_t min_delta_distance = 0;                        // best-candidate sentinel (init 1000)
    int16_t landmass_node_index = 0;                       // chain cursor
    int16_t territory_centroid_wy = 0;                     // city centroid Y from Phase 3 (read in Phase 5)
    int16_t territory_centroid_wx = 0;                     // city centroid X from Phase 3 (read in Phase 5)
    int16_t own_city_count = 0;                            // Phase 3 city counter snapshot
    int16_t itr_units = 0;                                 // unit-iteration loop counter
    int16_t itr_cities = 0;                                // city-iteration loop counter
    int16_t landmass_node_count = 0;                       // Phase 6: count for centroid divisor (split from OG's reused Tile_X)
    ...
}
```

**Variable naming notes:**
- `sum_own_city_wx__own_units_cost` and `sum_own_city_wy__enemy_units_cost` carry the dual-use explicitly in their names: they hold both the city coordinate sum (from Phase 3) and the unit cost sum (added on top in Phase 4). The `__` separator makes the dual semantic visible.
- The Dasm-recovered locals previously named `Tile_X`/`Tile_Y`/`Best_Tile_Weight`/`landmass_node_index`/`Center_X`/`Center_Y`/`LoadTile_X`/`LoadTile_Y`/`square_weight` have been renamed to match the sibling's terminology (`landmass_node_wx/wy`, `min_delta_distance`, `landmass_node_index`, `territory_centroid_wx/wy`, `landmass_node_centroid_wx/wy`, `delta_distance`). The OG dev had reused `Tile_X` as a counter in Phase 6 — that counter is now its own variable `landmass_node_count`.
- The OG single `itr` has been split into `itr_units` and `itr_cities` for readability (commented-out `itr` is preserved as a faithful-to-Dasm marker).
- `target_square_wx` / `target_square_wy` are the chosen target square coordinates, written to `wx_array` / `wy_array` by Phase 5 (always when found) and Phase 6 (for `lmt_Own` landmasses).

## Phase 1 — EMM mapping

```c
// AIMOVE.c:5777-5778
/* Phase 1: */
EMM_Map_CONTXXX__WIP();
```

Page the CONTXXX data handle into the EMS frame so the `_ai_landmass_*_squares_*` arrays are accessible during the subsequent phases. Counterpart to the `EMM_Map_DataH()` cleanup at the end of the sibling [`AI_Evaluate_Continents`](AIMOVE-AI_Evaluate_Continents.md) — this function does not perform the cleanup (flagged inline at line 6025 as `/* ¿ OGBUG  no EMM_Map_DataH() ? */`).

## Phase 2 — Cancel orders for all non-engineer units on this landmass

```c
// AIMOVE.c:5781-5798
// ; cancel the orders of every unit on the continent except for engineers
/* ¿ OGBUG  should clear all non-military units ? */
/* OGBUG  should condition on same plane */
/* (landmass indices are per-plane and the value space overlaps) */
for(itr_units = 0; itr_units < _units; itr_units++)
{
    if(
        (_UNITS[itr_units].owner_idx == player_idx)
        &&
        (_unit_type_table[_UNITS[itr_units].type].Construction == 0)
        &&
        (_landmasses[((_UNITS[itr_units].wp * WORLD_SIZE) + (_UNITS[itr_units].wy * WORLD_WIDTH) + _UNITS[itr_units].wx)] == landmass_idx)
    )
    {
        _UNITS[itr_units].Status = us_Ready;
    }
}
```

**Intent:** wipe stale orders so the post-classification re-tasking starts clean. Engineers (`Construction != 0`) are spared because their road-build / purify orders should persist across reevaluations.

### Open question — `¿ OGBUG  should clear all non-military units ?` (line 5783)

The current code excludes only engineers. The OG question asks whether non-military units (settlers, transports) should also have their orders cleared by this loop — or perhaps shouldn't. Open, faithful to OG.

### OGBUG-A — no same-plane check (line 5784)

The landmass lookup correctly uses the unit's own `.wp/.wy/.wx`. But the lookup returns *which landmass index that tile belongs to on that unit's plane*. If a plane-1 unit happens to be on a landmass whose index also exists on plane 0 (landmass indices are per-plane and the value space overlaps — spelled out inline at line 5785), the comparison `== landmass_idx` will spuriously pass.

**Example:** caller passes `(wp=0, landmass_idx=5)`. The loop hits a plane-1 unit standing on plane 1's landmass 5. The lookup returns 5. The check passes. We cancel the plane-1 unit's orders even though we're processing the plane-0 landmass.

Inline OGBUG comment in source. Preserved faithful-to-dasm.

## Phase 3 — Geographic centroid of owned cities

```c
// AIMOVE.c:5801-5829
/* Phase 3: */
/*
    BEGIN:  geographic centroid
*/
_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = 0;       // zero the slot before using as counter
sum_own_city_wx__own_units_cost = 0;
sum_own_city_wy__enemy_units_cost = 0;
for(itr_cities = 0; itr_cities < _cities; itr_cities++)
{
    if(
        (_CITIES[itr_cities].owner_idx == player_idx)
        &&
        (_landmasses[((_CITIES[itr_cities].wp * WORLD_SIZE) + (_CITIES[itr_cities].wy * WORLD_WIDTH) + _CITIES[itr_cities].wx)] == landmass_idx)
    )
    {
        sum_own_city_wx__own_units_cost += _CITIES[itr_cities].wx;
        sum_own_city_wy__enemy_units_cost += _CITIES[itr_cities].wy;
        _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] += 1;   // type_array slot used as city counter
    }
}
own_city_count = _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx];
if(own_city_count > 0)
{
    _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = (sum_own_city_wx__own_units_cost / own_city_count);
    _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = (sum_own_city_wy__enemy_units_cost / own_city_count);
}
```

**Intent:** for every owned city on this landmass, accumulate its `(wx, wy)` coordinates and bump a counter (using the type_array slot itself as the counter). After the loop, divide to get the geographic centroid of owned cities — used downstream in Phase 5 as the "center of my territory here" point. The slot is zeroed first so the counter starts at 0 regardless of prior classification.

## Phase 4 — Classify the landmass

The source labels this whole block `/* Phase 4 */` and wraps it with `BEGIN/END Phase 4 — Classify` markers. It has two sub-steps: (4a) accumulate unit costs, (4b) classify based on cost ratio and city counts.

### Sub-step 4a — Accumulate unit costs into the dual-use vars

```c
// AIMOVE.c:5832-5859
/* Phase 4: */
/* OGBUG  should use separate vars/clear vars beforehand - vars still contain city coordinate sums */
for (itr_units = 0; itr_units < _units; itr_units++)
{
    unit_type = _UNITS[itr_units].type;
    /* ¿ OGBUG  should skip all non-military units ? */
    /* Skip Settlers */
    if ((_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) != 0)
    {
        continue;
    }
    /* OGBUG  should condition on same plane */
    if(_landmasses[((_UNITS[itr_units].wp * WORLD_SIZE) + (_UNITS[itr_units].wy * WORLD_WIDTH) + _UNITS[itr_units].wx)] == landmass_idx)
    {
        if (_UNITS[itr_units].owner_idx == player_idx)
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

#### OGBUG-B — coord sums contaminate cost sums (line 5837)

The same `sum_own_city_wx__own_units_cost` and `sum_own_city_wy__enemy_units_cost` vars were used in Phase 3 to accumulate city wx/wy coordinates, and are not cleared between phases. So the values entering the classifier are:
```
sum_own_city_wx__own_units_cost   = (sum of X coords of own cities)  + (total cost of own units on landmass)
sum_own_city_wy__enemy_units_cost = (sum of Y coords of own cities)  + (total cost of enemy units on landmass)
```

Effect is small in mid/late game where unit costs (100s-1000s) dominate coordinate sums (0-59 per city × maybe a handful of cities). More distortion in early game with few/cheap units. Inline OGBUG. Preserved.

#### OGBUG-C — no same-plane check (line 5847)

Same issue as OGBUG-A but in the unit cost loop. Cross-plane units with matching landmass index contribute to the cost sums. Inline OGBUG. Preserved.

#### Inline open question — `¿ OGBUG  should skip all non-military units ?` (line 5841)

Same question raised in the sibling `AI_Evaluate_Continents` Phase 5 — only Settlers are excluded; transports, engineers, etc. are counted as combat strength. Open question, faithful to OG.

### Sub-step 4b — Classify, ally-promotion, and slot-0 sentinel

```c
// AIMOVE.c:5861-5906
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

/* maybe, change lmt_NoOwnCity to lmt_NoOwnCityAndAllyHasCity */
for(itr_cities = 0; itr_cities < _cities; itr_cities++)
{
    if(
        (_CITIES[itr_cities].owner_idx != player_idx)
        &&
        (_CITIES[itr_cities].owner_idx != NEUTRAL_PLAYER_IDX)
        &&
        (_landmasses[((_CITIES[itr_cities].wp * WORLD_SIZE) + (_CITIES[itr_cities].wy * WORLD_WIDTH) + _CITIES[itr_cities].wx)] == landmass_idx)
    )
    {
        if(
            (_players[player_idx].Dipl.Dipl_Status[_CITIES[itr_cities].owner_idx] == DIPL_Alliance)
            &&
            (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_NoOwnCity)
        )
        {
            _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_NoOwnCityAndAllyHasCity;
        }
    }
}

/* Landmass 0 is Ocean / 'Landmass of No-LandMass' */
_ai_continents.plane[0].player[player_idx].type_array[0] = lmt_NoOwnCityAndAllyHasCity;
_ai_continents.plane[1].player[player_idx].type_array[0] = lmt_NoOwnCityAndAllyHasCity;
```

**Classification rules:**

| Has owned city? | Cost comparison | Result |
|---|---|---|
| Yes | `(enemy_cost × 10) < own_cost` | → `lmt_Own` |
| Yes | `(enemy_cost × 10) >= own_cost` | → `lmt_Contested` |
| No | (not evaluated) | → `lmt_NoOwnCity`; promoted to `lmt_NoOwnCityAndAllyHasCity` if any allied wizard has a city here |

**Slot-0 force:** unconditionally sets `plane[0]/[1] ... type_array[0]` to `lmt_NoOwnCityAndAllyHasCity` regardless of which landmass is being processed. Slot 0 = ocean / no-landmass sentinel; this defensive write ensures the sentinel always holds the "don't land" marker. Sibling [`AI_Evaluate_Continents`](AIMOVE-AI_Evaluate_Continents.md) does the same.

## Phase 5 — Pick stage square near territory centroid

```c
// AIMOVE.c:5909-5967
/* Phase 5: */
if(
    (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_Contested)
    ||
    (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_Own)
    ||
    (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_NoOwnCityAndAllyHasCity)
)
{
    // Would be from AI_Evaluate_Continents(), Phase 7 - nearest square to territory centroid?
    territory_centroid_wx = _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx];   // city centroid from Phase 3
    territory_centroid_wy = _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx];
    min_delta_distance = 1000;
    landmass_node_index = _ai_landmass_land_squares_heads[wp][landmass_idx];
    while(landmass_node_index != ST_UNDEFINED)
    {
        delta_distance = (
            Delta_XY_With_Wrap(
                _ai_landmass_land_squares_wx_array[wp][landmass_node_index],
                _ai_landmass_land_squares_wy_array[wp][landmass_node_index],
                territory_centroid_wx, territory_centroid_wy, WORLD_WIDTH
            )
            +
            Random(5)
        );
        if(delta_distance < min_delta_distance)
        {
            landmass_node_wx = _ai_landmass_land_squares_wx_array[wp][landmass_node_index];
            landmass_node_wy = _ai_landmass_land_squares_wy_array[wp][landmass_node_index];
            /* Check if square is occupied by a site or an enemy stack */
            square_occupation_value = g_ai_evaluation_map[wp][((landmass_node_wy * WORLD_WIDTH) + landmass_node_wx)];
            if(square_occupation_value == 0)  /* none or own */
            {
                min_delta_distance = delta_distance;
                target_square_wx = _ai_landmass_land_squares_wx_array[wp][landmass_node_index];
                target_square_wy = _ai_landmass_land_squares_wy_array[wp][landmass_node_index];
            }
        }
        landmass_node_index = _ai_landmass_land_squares_lists[wp][landmass_node_index];
    }
    /*
        Update stage-point
    */
    if(min_delta_distance < 1000)
    {
        _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = (uint8_t)target_square_wx;
        _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = (uint8_t)target_square_wy;
    }
    else
    {
        // no acceptable square found — clear stage point and DOWNGRADE the classification
        _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = 0;
        _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = 0;
        _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_NoOwnCityAndAllyHasCity;
    }
}
```

**Intent:** for landmasses where positioning matters (Own / Contested / AllyLand), find a stage square near the territory centroid that has no occupation per `g_ai_evaluation_map`. The chosen square becomes the stage point where stacks gather. If no acceptable square is found, clear the stage point and downgrade to `lmt_NoOwnCityAndAllyHasCity`.

**Why "best" = lowest distance:** `delta_distance = Δxy + Random(5)`. Lower = closer to centroid + small jitter. `min_delta_distance` tracks the lowest seen for any square with `square_occupation_value == 0`. Initialized to 1000 as a sentinel above any valid distance (max realistic ~54 for a 60×40 world), which doubles as the "did we find anything?" marker after the loop.

## Phase 6 — Refine stage point for Own landmasses (snap toward main-war embark centroid)

```c
// AIMOVE.c:5970-6023
/* Phase 6: Prepare stage-square for requisition of troops */
/* Phase 6: Own Continent - Calculate Transport Loading Center */
if(_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_Own)
{
    landmass_node_centroid_wx = 0;
    landmass_node_centroid_wy = 0;
    landmass_node_count = 0;
    /* Calculate average position of all possible coastal loading points */
    /* ¿ OG BUG  _ai_landmass_dock_squares_heads[][0] -1 on purpose, because _ai_landmass_war_targets[wp][player_idx] could be 0 as in NONE ? */
    landmass_node_index = _ai_landmass_dock_squares_heads[wp][_ai_landmass_war_targets[wp][player_idx]];

    // Step A: compute centroid of dock squares on the main war landmass
    while(landmass_node_index != ST_UNDEFINED)
    {
        landmass_node_centroid_wx += _ai_landmass_dock_squares_wx_array[wp][landmass_node_index];
        landmass_node_centroid_wy += _ai_landmass_dock_squares_wy_array[wp][landmass_node_index];
        landmass_node_count++;
        landmass_node_index = _ai_landmass_dock_squares_lists[wp][landmass_node_index];
    }

    /* OGBUG  possible division by 0, should `if(landmass_node_count > 0)` */
    landmass_node_centroid_wx /= landmass_node_count;
    landmass_node_centroid_wy /= landmass_node_count;
    min_delta_distance = 1000;
    landmass_node_index = _ai_landmass_dock_squares_heads[wp][landmass_idx];

    // Step B: walk this landmass's dock squares, find the one closest to that centroid
    while(landmass_node_index != ST_UNDEFINED)
    {
        delta_distance = (
            Delta_XY_With_Wrap(
                _ai_landmass_dock_squares_wx_array[wp][landmass_node_index],
                _ai_landmass_dock_squares_wy_array[wp][landmass_node_index],
                landmass_node_centroid_wx, landmass_node_centroid_wy, WORLD_WIDTH
            )
            + Random(5)
        );
        if(delta_distance < min_delta_distance)
        {
            landmass_node_wx = _ai_landmass_dock_squares_wx_array[wp][landmass_node_index];
            landmass_node_wy = _ai_landmass_dock_squares_wy_array[wp][landmass_node_index];
            if(g_ai_evaluation_map[wp][((landmass_node_wy * WORLD_WIDTH) + landmass_node_wx)] == 0)
            {
                min_delta_distance = delta_distance;
                target_square_wx = landmass_node_wx;
                target_square_wy = landmass_node_wy;
            }
        }
        landmass_node_index = _ai_landmass_dock_squares_lists[wp][landmass_node_index];
    }
    // Step C: write back
    if(min_delta_distance < 1000)
    {
        _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = (uint8_t)target_square_wx;
        _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = (uint8_t)target_square_wy;
    }
}

/* ¿ OGBUG  no EMM_Map_DataH() ? */
```

**Intent:** for landmasses we own decisively, the stage point shouldn't just be near our city center — it should be positioned to ship troops out toward the main war. Step A computes the centroid of dock squares on the main war landmass (the embark destination). Step B picks this landmass's dock square closest to that centroid. Step C writes the result back to `wx_array` / `wy_array`.

### OGBUG-D — Division by zero risk (line 5987)

`landmass_node_centroid_wx /= landmass_node_count` and `landmass_node_centroid_wy /= landmass_node_count`. If the main war landmass has no dock squares, `landmass_node_count` remains 0 and these divisions crash. Sibling `AI_Evaluate_Continents` Phase 9 has the same divide-by-zero OGBUG inline. Preserved.

### Open question on Phase 6 head lookup (line 5978)

Inline comment: `/* ¿ OG BUG  _ai_landmass_dock_squares_heads[][0] -1 on purpose, because _ai_landmass_war_targets[wp][player_idx] could be 0 as in NONE ? */`

When no main war target is set, `_ai_landmass_war_targets[wp][player_idx]` is 0. The lookup then reads `_ai_landmass_dock_squares_heads[wp][0]` (slot 0 = ocean). Slot 0 has no chain (slot-0 was force-set in Phase 4 to `lmt_NoOwnCityAndAllyHasCity` and isn't a real landmass), so the `while` loop exits without entering — `landmass_node_count` stays 0, triggering OGBUG-D's divide-by-zero. Open question on intent; flagged in source.

## Phase cleanup — open question (line 6025)

Inline comment: `/* ¿ OGBUG  no EMM_Map_DataH() ? */`

Unlike sibling [`AI_Evaluate_Continents`](AIMOVE-AI_Evaluate_Continents.md) (which calls `EMM_Map_DataH()` to restore the default EMM mapping), this function ends without restoring the EMM mapping. Caller `AI_Stacks_Stage_Expedition_Forces` does not re-map either. Either:
- The CONTXXX mapping happens to be the correct state for downstream callers in this code path, or
- This is a missing cleanup — preserved-OG behavior.

## Bug catalog summary

| # | Severity | Line | Description |
|---|---|---|---|
| Open question | n/a | 5783 (Phase 2) | "Should clear all non-military units?" — inline `¿ OGBUG ?` |
| OGBUG-A | Medium | 5784 (Phase 2) | No same-plane check on unit landmass lookup |
| OGBUG-B | Low | 5837 (Phase 4a) | Coord sums contaminate cost sums (vars not cleared between Phase 3 and Phase 4) |
| Open question | n/a | 5841 (Phase 4a) | "Should skip all non-military units?" — only Settlers excluded |
| OGBUG-C | Medium | 5847 (Phase 4a) | Same cross-plane lookup issue as OGBUG-A |
| Open question | n/a | 5978 (Phase 6) | `_ai_landmass_dock_squares_heads[][0]` lookup when no main war set — inline `¿ OG BUG ?` |
| OGBUG-D | Known | 5987 (Phase 6) | Division by zero if main war landmass has no dock squares |
| Open question | n/a | 6025 (cleanup) | No `EMM_Map_DataH()` call before return — inline `¿ OGBUG ?` |

All entries are inline OG annotations preserved faithful-to-disassembly.

## Comparison with sibling `AI_Evaluate_Continents`

| Aspect | `AI_Reevaluate_Continent` | [`AI_Evaluate_Continents`](AIMOVE-AI_Evaluate_Continents.md) |
|---|---|---|
| Scope | One specific (player, plane, landmass) | All landmasses on both planes for one player |
| Caller | `AI_Stacks_Stage_Expedition_Forces` on 5% random roll when stage is full | (per-turn; needs caller verification) |
| Zeros type_array before counter use? | Yes (line 5805) | Yes |
| Has unit-cost summing loop? | Yes (lines 5838-5859) | Yes |
| Uses correct `.wp/.wy/.wx` landmass lookup? | Yes | Yes |
| Forces slot-0 sentinel? | Yes (lines 5902-5903) | Yes |
| Has stage-square picker? | Yes (Phases 5-6) | Yes (Phases 6-9) |
| Same-plane check on unit/city lookups? | No (OGBUG-A, OGBUG-C) | No — Phase 3 has cross-plane stale `wp` OGBUG too |
| Coord/cost var sharing? | Yes (OGBUG-B) | No — explicit clearing between Phase 4 and Phase 5 |
| EMM_Map_DataH cleanup? | No (inline open question) | Yes |

The two functions are siblings — `AI_Evaluate_Continents` is the "rebuild everything" full pass, `AI_Reevaluate_Continent` is the "just this one landmass" focused pass. The per-landmass version closely mirrors the full-rebuild structure. The remaining differences are the preserved-OG bugs (cross-plane contamination, coord/cost var sharing) plus the open questions (non-military clear, EMM cleanup).

## Related references

- [AIMOVE-AI_Evaluate_Continents.md](AIMOVE-AI_Evaluate_Continents.md) — full-rebuild counterpart at [AIMOVE.c:6934](../../MoM/src/AIMOVE.c#L6934)
- [AIMOVE-AI_Choose_War_Landmass.md](AIMOVE-AI_Choose_War_Landmass.md) — producer of `_ai_landmass_war_targets[]` read in Phase 6
- [MoM-AI-Landmass-Types.md](MoM-AI-Landmass-Types.md) — `lmt_*` enum state machine; this function's writes feed the Tier-1 classifier outputs
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index entry
