AIMOVE-AI_Reevaluate_All_Continents.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr162\AI_Continent_Reeval__WIP.c

---

# AI_Reevaluate_All_Continents — Walkthrough

**Location:** [MoM/src/AIMOVE.c:7059](../../MoM/src/AIMOVE.c#L7059) (~498 lines, ends line 7556). No GEMINI variant — tossed.
**WZD overlay:** ovr162, p35
**drake178 name:** `AI_Continent_Reeval()`

## Purpose

**Full-rebuild classifier for every (plane, landmass) for one player**, throttled to ~25-43 turns. The counterpart to the per-landmass [`AI_SingleCont_Reeval__WIP`](AIMOVE-AI_SingleCont_Reeval__WIP.md), which only refreshes one landmass at a time.

Three outputs:
1. `_ai_continents.plane[*][player_idx].type_array[*]` — `lmt_*` classification per (plane, landmass)
2. `_ai_continents.plane[*][player_idx].wx_array[*]` / `wy_array[*]` — stage point per (plane, landmass)
3. `_ai_landmass_settler_targets[*][player_idx]` / `_wx_array[*]` / `_wy_array[*]` — chosen colonization target per plane

## Locals (with semantic intent)

```c
int16_t sum_own_city_wy__enemy_units_cost[NUM_PLANES][NUM_LANDMASSES];  // dual-use: city wy sums in Phase 4, enemy unit costs in Phase 5
int16_t sum_own_city_wx__own_units_cost[NUM_PLANES][NUM_LANDMASSES];    // dual-use: city wx sums in Phase 4, own unit costs in Phase 5
int16_t world_offset;                       // helper: (wy * WORLD_WIDTH) + wx for g_ai_evaluation_map indexing
int16_t force_reevaluation;                 // Phase 2 sets this on stale colony detection → bypasses throttle
int16_t territory_centroid_wx, wy;          // per-landmass geographic centroid of own cities
int16_t target_landmass_idx;                // chosen colony target landmass index
int16_t target_square_wx, wy;               // chosen target square coords
int16_t delta_distance;                     // per-candidate distance to centroid
int16_t min_delta_distance;                 // best-candidate sentinel (init 1000)
int16_t city_count;                         // per-landmass own city count (read from type_array slot used as counter)
int16_t unit_type;                          // _UNITS[itr].type cache
int16_t landmass_node_index;                // chain cursor into _ai_landmass_*_squares_lists
int16_t landmass_node_wx, wy;               // current chain node's coords
int16_t landmass_node_centroid_wx, wy;     // Phase 9: centroid of dock squares on war landmass
int16_t landmass_node_count;                // Phase 9: count for centroid divisor
int16_t square_occupation_value;            // g_ai_evaluation_map read; 0 means unoccupied
int16_t itr_units, itr_cities, itr_landmasses;  // loop iterators (split from the OG `itr` for readability)
int16_t landmass_idx, wp;                   // plane / landmass loop iterators
```

The `sum_own_city_wx__own_units_cost` / `sum_own_city_wy__enemy_units_cost` arrays carry two semantically distinct uses, separated by an explicit re-clear at the start of Phase 5. The `__` separator in the names is a visible cue to the dual purpose.

## Phase 1 — Setup

```c
EMM_Map_CONTXXX__WIP();  /* needs all the landmass data */
```

Pages CONTXXX data into the EMS frame so the `_ai_landmass_*` arrays are accessible.

## Phase 2 — Reevaluation check + throttle gate

```c
force_reevaluation = ST_FALSE;
for(wp = 0; wp < NUM_PLANES; wp++)
{
    for(landmass_idx = 0; ((landmass_idx < NUM_LANDMASSES) && (force_reevaluation == ST_FALSE)); landmass_idx++)
    {
        if(_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_NoOwnCity)
        {
            /* unoccupieable - site or enemy stack */
            if(g_ai_evaluation_map[wp][((wy_array * WORLD_WIDTH) + wx_array)] != 0)
            {
                force_reevaluation = ST_TRUE;
            }
        }
    }
}
if(force_reevaluation == ST_TRUE)
{
    _ai_reevaluate_continents_countdown[player_idx] = 0;
}
_ai_reevaluate_continents_countdown[player_idx] -= 1;
if(_ai_reevaluate_continents_countdown[player_idx] > 0)
{
    return ;
}
_ai_reevaluate_continents_countdown[player_idx] = (25 + Random(10) + Random(10));
```

**Intent:** scan for stale colony targets (`lmt_NoOwnCity` landmasses whose previously-chosen settler-landing square now has site/enemy occupation). If found, force an immediate reeval by zeroing the countdown. Then decrement the countdown; bail if not yet expired. Reset to a fresh 25-43 turn period.

## Phase 3 — Cancel orders for player units moving to empty tiles

```c
/* OGBUG  should check unit wp */
/* OGBUG  settlers and transports (non-military) are not excluded ? */
for(itr_units = 0; itr_units < _units; itr_units++)
{
    if(
        (_UNITS[itr_units].owner_idx == player_idx)
        &&
        (_unit_type_table[_UNITS[itr_units].type].Construction == 0)
        &&
        (g_ai_evaluation_map[wp][((_UNITS[itr_units].dst_wy * WORLD_WIDTH) + _UNITS[itr_units].dst_wx)] == 0)
    )
    {
        _UNITS[itr_units].Status = us_Ready;
    }
}
```

**Intent:** any non-engineer player unit currently moving to a tile with no enemy presence has its movement order cancelled — so Phases 4-9 can re-task them based on the fresh classification.

**Source-flagged OGBUGs** (preserved as faithful-to-disassembly):
- The `g_ai_evaluation_map[wp][...]` lookup uses the stale `wp` from Phase 2's outermost loop. Cross-plane false-positive cancellations possible.
- Only engineers (`Construction != 0`) are excluded — settlers and transports get their orders cancelled too.

## Phase 4 — Geographic centroid of own cities per landmass

```c
// Zero type_array (will be used as city counter) and sum arrays
for(wp = 0; wp < NUM_PLANES; wp++)
{
    for(landmass_idx = 0; landmass_idx < NUM_LANDMASSES; landmass_idx++)
    {
        _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = 0;
        sum_own_city_wx__own_units_cost[wp][landmass_idx] = 0;
        sum_own_city_wy__enemy_units_cost[wp][landmass_idx] = 0;
    }
}

// Accumulate own city coords + count per landmass
for(itr_cities = 0; itr_cities < _cities; itr_cities++)
{
    if(_CITIES[itr_cities].owner_idx == player_idx)
    {
        wp = _CITIES[itr_cities].wp;
        landmass_idx = _landmasses[((wp * WORLD_SIZE) + (_CITIES[itr_cities].wy * WORLD_WIDTH) + _CITIES[itr_cities].wx)];
        if(landmass_idx < NUM_LANDMASSES)
        {
            sum_own_city_wx__own_units_cost[wp][landmass_idx] += _CITIES[itr_cities].wx;
            sum_own_city_wy__enemy_units_cost[wp][landmass_idx] += _CITIES[itr_cities].wy;
            _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] += 1;  // city counter
        }
    }
}

// Divide to get centroid
for(wp = 0; wp < NUM_PLANES; wp++)
{
    for(landmass_idx = 0; landmass_idx < NUM_LANDMASSES; landmass_idx++)
    {
        city_count = _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx];
        if(city_count > 0)
        {
            wx_array[landmass_idx] = (sum_own_city_wx__own_units_cost[wp][landmass_idx] / city_count);
            wy_array[landmass_idx] = (sum_own_city_wy__enemy_units_cost[wp][landmass_idx] / city_count);
        }
    }
}
```

**Intent:** for each (plane, landmass), compute the geographic centroid of owned cities and store in `wx_array`/`wy_array`. The `type_array` slot serves double duty here — it's the city counter accumulator before being overwritten by Phase 5's classification.

## Phase 5 — Classify Own / Contested / NoOwnCity / AllyHasCity

```c
// Re-clear sum arrays (Phase 4 left them with coordinate sums; Phase 5 reuses them for unit costs)
for(itr_landmasses = 0; itr_landmasses < NUM_LANDMASSES; itr_landmasses++)
{
    sum_own_city_wx__own_units_cost[0][itr_landmasses] = 0;
    sum_own_city_wx__own_units_cost[1][itr_landmasses] = 0;
    sum_own_city_wy__enemy_units_cost[0][itr_landmasses] = 0;
    sum_own_city_wy__enemy_units_cost[1][itr_landmasses] = 0;
}

// Accumulate non-settler unit costs per (plane, landmass), split by ownership
/*  ¿ OGBUG  should exclude non-military, rather than just Settlers (or, why exclude anything?) ? */
for(itr_units = 0; itr_units < _units; itr_units++)
{
    unit_type = _UNITS[itr_units].type;
    if((_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) != 0) continue;  // skip settlers
    wp = _UNITS[itr_units].wp;
    landmass_idx = _landmasses[((wp * WORLD_SIZE) + (_UNITS[itr_units].wy * WORLD_WIDTH) + _UNITS[itr_units].wx)];
    if(landmass_idx < NUM_LANDMASSES)
    {
        if(_UNITS[itr_units].owner_idx == player_idx)
            sum_own_city_wx__own_units_cost[wp][landmass_idx] += _unit_type_table[unit_type].cost;
        else
            sum_own_city_wy__enemy_units_cost[wp][landmass_idx] += _unit_type_table[unit_type].cost;
    }
}

// Classify per (plane, landmass)
/* {1:lmt_Own, 2:lmt_Contested, 3:lmt_NoOwnCity} */
for(wp = 0; wp < NUM_PLANES; wp++)
{
    for(landmass_idx = 0; landmass_idx < NUM_LANDMASSES; landmass_idx++)
    {
        // Already lmt_NoTargets with no enemy presence → leave it
        /* OGBUG  checking for landmass status/type, but actually just city count */
        if(
            (type_array[landmass_idx] == lmt_NoTargets)
            &&
            (sum_own_city_wy__enemy_units_cost[wp][landmass_idx] == 0)
        )
        {
            continue;
        }
        if(type_array[landmass_idx] > 0)  // has own cities
        {
            /* Classify lmt_Own when our unit-cost on this landmass is more than 10× the enemy's
               unit-cost (equivalently: enemy unit-cost is < 10% of ours). */
            if(
                sum_own_city_wx__own_units_cost[wp][landmass_idx]
                >
                (sum_own_city_wy__enemy_units_cost[wp][landmass_idx] * 10)  /* ¿ OGBUG  could overflow ? */
            )
                type_array[landmass_idx] = lmt_Own;
            else
                type_array[landmass_idx] = lmt_Contested;
        }
        else  // no own cities
        {
            type_array[landmass_idx] = lmt_NoOwnCity;
        }
    }
}

// Promote lmt_NoOwnCity → lmt_NoOwnCityAndAllyHasCity for landmasses where an ally has a city
for(itr_cities = 0; itr_cities < _cities; itr_cities++)
{
    if(_CITIES[itr_cities].owner_idx == player_idx) continue;
    if(_CITIES[itr_cities].owner_idx == NEUTRAL_PLAYER_IDX) continue;
    if(_players[player_idx].Dipl.Dipl_Status[_CITIES[itr_cities].owner_idx] != DIPL_Alliance) continue;
    wp = _CITIES[itr_cities].wp;
    landmass_idx = _landmasses[((wp * WORLD_SIZE) + (_CITIES[itr_cities].wy * WORLD_WIDTH) + _CITIES[itr_cities].wx)];
    if(type_array[landmass_idx] == lmt_NoOwnCity)
    {
        type_array[landmass_idx] = lmt_NoOwnCityAndAllyHasCity;
    }
}

// Slot-0 (Ocean) sentinel force on both planes
_ai_continents.plane[0].player[player_idx].type_array[0] = lmt_NoOwnCityAndAllyHasCity;
_ai_continents.plane[1].player[player_idx].type_array[0] = lmt_NoOwnCityAndAllyHasCity;
```

**Intent:** the heart of the function. After clearing the sum arrays and re-accumulating them as unit costs (own vs. enemy, excluding settlers), each (plane, landmass) gets classified:
- Already-`lmt_NoTargets` with no enemy presence → stays `lmt_NoTargets`
- City count > 0: `lmt_Own` if own unit-cost > 10× enemy unit-cost, else `lmt_Contested`
- City count == 0: `lmt_NoOwnCity` (then promoted to `lmt_NoOwnCityAndAllyHasCity` if an allied wizard has a city here)
- Slot 0 (the Ocean landmass) is finally pinned to `lmt_NoOwnCityAndAllyHasCity` on both planes — this is the "no targets at sea" sentinel that downstream code (e.g. `AI_Choose_War_Landmass`) relies on.

**Source-flagged OGBUGs:**
- The `type != lmt_NoTargets` short-circuit uses the type_array slot which at this point in the loop is still acting as a city counter — the comparison against `lmt_NoTargets` (= 6) accidentally matches landmasses with exactly 6 own cities.
- The wartime ratio test `enemy × 10` could overflow on large continents (preserved OG comment).

## Phase 6 — Pick colonization target per plane

```c
for(wp = 0; wp < NUM_PLANES; wp++)
{
    min_delta_distance = 1000;
    for(landmass_idx = 1; landmass_idx < NUM_LANDMASSES; landmass_idx++)
    {
        if(type_array[landmass_idx] != lmt_NoOwnCity) continue;
        if(sum_own_city_wy__enemy_units_cost[wp][landmass_idx] != 0) continue;
        landmass_node_index = _ai_landmass_land_squares_heads[wp][landmass_idx];
        while((landmass_node_index != ST_UNDEFINED) && (square_occupation_value == ST_FALSE))
        {
            landmass_node_wx = _ai_landmass_land_squares_wx_array[wp][landmass_node_index];
            landmass_node_wy = _ai_landmass_land_squares_wy_array[wp][landmass_node_index];
            world_offset = ((landmass_node_wy * WORLD_WIDTH) + landmass_node_wx);
            if(g_ai_evaluation_map[wp][world_offset] == 0)
            {
                /* OGBUG  landmass_node_centroid_wx and landmass_node_centroid_wy are uninitialized */
                delta_distance = Random(20) + Delta_XY_With_Wrap(
                    landmass_node_wx, landmass_node_wy,
                    landmass_node_centroid_wx, landmass_node_centroid_wy,
                    WORLD_WIDTH
                );
                if(delta_distance < min_delta_distance)
                {
                    min_delta_distance = delta_distance;
                    target_square_wx = _ai_landmass_land_squares_wx_array[wp][landmass_node_index];
                    target_square_wy = _ai_landmass_land_squares_wy_array[wp][landmass_node_index];
                    target_landmass_idx = landmass_idx;
                }
            }
            landmass_node_index = _ai_landmass_land_squares_lists[wp][landmass_node_index];
        }
    }
    if(min_delta_distance < 1000)
    {
        _ai_landmass_settler_targets[wp][player_idx] = (uint8_t)target_landmass_idx;
        _ai_landmass_settler_targets_wx_array[wp][player_idx] = (uint8_t)target_square_wx;
        _ai_landmass_settler_targets_wy_array[wp][player_idx] = (uint8_t)target_square_wy;
    }
    else
    {
        _ai_landmass_settler_targets[wp][player_idx] = 0;
    }
}
```

**Intent:** for each plane, find the best (closest-to-empire + small random jitter) unoccupied land square on a `lmt_NoOwnCity` landmass that has no enemy presence. Best one across all candidate landmasses on that plane becomes the next colonization target.

**Source-flagged OGBUG:** `landmass_node_centroid_wx`/`wy` are uninitialized when the `Delta_XY_With_Wrap` call uses them in this phase (they're set in Phase 9, but Phase 6 runs first). Distance is computed "from (0, 0)" rather than from any meaningful empire centroid.

## Phase 7 — Refine stage point for Own / Contested / AllyLand landmasses

```c
for(wp = 0; wp < NUM_PLANES; wp++)
{
    for(landmass_idx = 1; landmass_idx < NUM_LANDMASSES; landmass_idx++)
    {
        // Skip landmasses that aren't one of the three target types
        if(
            (type_array[landmass_idx] != lmt_Contested)
            && (type_array[landmass_idx] != lmt_Own)
            && (type_array[landmass_idx] != lmt_NoOwnCityAndAllyHasCity)
        )
        {
            continue;
        }
        territory_centroid_wx = wx_array[landmass_idx];
        territory_centroid_wy = wy_array[landmass_idx];
        // Skip if current centroid is already a valid unoccupied tile on this landmass
        if(
            (g_ai_evaluation_map[wp][((territory_centroid_wy * WORLD_WIDTH) + territory_centroid_wx)] == 0)
            && (_landmasses[((wp * WORLD_SIZE) + (territory_centroid_wy * WORLD_WIDTH) + territory_centroid_wx)] == landmass_idx)
        )
        {
            continue;
        }
        // Otherwise, find the closest unoccupied land square on the landmass
        min_delta_distance = 1000;
        landmass_node_index = _ai_landmass_land_squares_heads[wp][landmass_idx];
        while(landmass_node_index != ST_UNDEFINED)
        {
            /* no jitter, wants exact */
            delta_distance = Delta_XY_With_Wrap(
                _ai_landmass_land_squares_wx_array[wp][landmass_node_index],
                _ai_landmass_land_squares_wy_array[wp][landmass_node_index],
                territory_centroid_wx, territory_centroid_wy, WORLD_WIDTH
            );
            if(delta_distance < min_delta_distance)
            {
                landmass_node_wx = _ai_landmass_land_squares_wx_array[wp][landmass_node_index];
                landmass_node_wy = _ai_landmass_land_squares_wy_array[wp][landmass_node_index];
                square_occupation_value = g_ai_evaluation_map[wp][((landmass_node_wy * WORLD_WIDTH) + landmass_node_wx)];
                if(square_occupation_value == 0)
                {
                    min_delta_distance = delta_distance;
                    target_square_wx = _ai_landmass_land_squares_wx_array[wp][landmass_node_index];
                    target_square_wy = _ai_landmass_land_squares_wy_array[wp][landmass_node_index];
                    target_landmass_idx = landmass_idx;  /* OGBUG  c&p error? target_landmass_idx is not used in this phase */
                }
            }
            landmass_node_index = _ai_landmass_land_squares_lists[wp][landmass_node_index];
        }
        if(min_delta_distance < 1000)
        {
            wx_array[landmass_idx] = (uint8_t)target_square_wx;
            wy_array[landmass_idx] = (uint8_t)target_square_wy;
        }
        else
        {
            wx_array[landmass_idx] = 0;
            wy_array[landmass_idx] = 0;
            type_array[landmass_idx] = lmt_NoOwnCityAndAllyHasCity;
        }
    }
}
```

**Intent:** for landmasses we hold/contest/are-allied-with, ensure the stage point is a valid unoccupied tile on the landmass. If the current centroid is already valid, leave it; otherwise find the closest unoccupied land square. If no acceptable square exists, downgrade to `lmt_NoOwnCityAndAllyHasCity` with stage [0,0].

**Source-flagged OGBUG:** `target_landmass_idx` is assigned but never read in this phase — likely a copy-paste artifact from the surrounding phases.

## Phase 8 — Set stage for NoOwnCity using dock squares

```c
for(wp = 0; wp < NUM_PLANES; wp++)
{
    for(landmass_idx = 1; landmass_idx < NUM_LANDMASSES; landmass_idx++)
    {
        if(type_array[landmass_idx] != lmt_NoOwnCity) continue;
        territory_centroid_wx = wx_array[landmass_idx];
        territory_centroid_wy = wy_array[landmass_idx];
        min_delta_distance = 1000;
        landmass_node_index = _ai_landmass_dock_squares_heads[wp][landmass_idx];
        while(landmass_node_index != ST_UNDEFINED)
        {
            landmass_node_wx = _ai_landmass_dock_squares_wx_array[wp][landmass_node_index];
            landmass_node_wy = _ai_landmass_dock_squares_wy_array[wp][landmass_node_index];
            if(g_ai_evaluation_map[wp][((landmass_node_wy * WORLD_WIDTH) + landmass_node_wx)] == 0)
            {
                /* ¿ OGBUG  no jitter ? */
                delta_distance = Delta_XY_With_Wrap(
                    landmass_node_wx, landmass_node_wy,
                    territory_centroid_wx, territory_centroid_wy, WORLD_WIDTH
                );
                if(delta_distance < min_delta_distance)
                {
                    min_delta_distance = delta_distance;
                    target_square_wx = _ai_landmass_dock_squares_wx_array[wp][landmass_node_index];
                    target_square_wy = _ai_landmass_dock_squares_wy_array[wp][landmass_node_index];
                    target_landmass_idx = landmass_idx;  /* OGBUG  c&p error? target_landmass_idx is not used in this phase */
                }
            }
            landmass_node_index = _ai_landmass_dock_squares_lists[wp][landmass_node_index];
        }
        if(min_delta_distance < 1000)
        {
            wx_array[landmass_idx] = (uint8_t)target_square_wx;
            wy_array[landmass_idx] = (uint8_t)target_square_wy;
        }
        else
        {
            wx_array[landmass_idx] = 0;
            wy_array[landmass_idx] = 0;
            type_array[landmass_idx] = lmt_NoOwnCityAndAllyHasCity;
        }
    }
}
```

**Intent:** for `lmt_NoOwnCity` landmasses, find the closest unoccupied dock (embark) square to the centroid stored by Phase 4. Store as new stage point; if none found, downgrade to `lmt_NoOwnCityAndAllyHasCity`.

**Source-flagged OGBUGs:**
- `target_landmass_idx` assigned but unused (same c&p artifact as Phase 7).
- No `Random(20)` jitter on the weight — Phase 6 has it but Phase 8 doesn't.

## Phase 9 — Refine Own stage toward main-war embark centroid

```c
for(wp = 0; wp < NUM_PLANES; wp++)
{
    if(_ai_landmass_war_targets[wp][player_idx] != 0)  // war effort on this plane?
    {
        for(landmass_idx = 1; landmass_idx < NUM_LANDMASSES; landmass_idx++)
        {
            // Are we secure enough to relocate troops from this landmass?
            if(type_array[landmass_idx] != lmt_Own) continue;
            // Step A: compute centroid of dock squares on the war landmass
            landmass_node_centroid_wx = 0;
            landmass_node_centroid_wy = 0;
            landmass_node_count = 0;
            landmass_node_index = _ai_landmass_dock_squares_heads[wp][_ai_landmass_war_targets[wp][player_idx]];
            while(landmass_node_index != ST_UNDEFINED)
            {
                landmass_node_centroid_wx += _ai_landmass_dock_squares_wx_array[wp][landmass_node_index];
                landmass_node_centroid_wy += _ai_landmass_dock_squares_wy_array[wp][landmass_node_index];
                landmass_node_count++;
                landmass_node_index = _ai_landmass_dock_squares_lists[wp][landmass_node_index];
            }
            /* OGBUG  divide by zero, should `if(landmass_node_count > 0)` */
            landmass_node_centroid_wx /= landmass_node_count;
            landmass_node_centroid_wy /= landmass_node_count;
            // Step B: find this landmass's dock square closest to that centroid
            min_delta_distance = 1000;
            landmass_node_index = _ai_landmass_dock_squares_heads[wp][landmass_idx];
            while(landmass_node_index != ST_UNDEFINED)
            {
                /* ¿ OGBUG  no jitter ? */
                delta_distance = Delta_XY_With_Wrap(
                    _ai_landmass_dock_squares_wx_array[wp][landmass_node_index],
                    _ai_landmass_dock_squares_wy_array[wp][landmass_node_index],
                    landmass_node_centroid_wx, landmass_node_centroid_wy, WORLD_WIDTH
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
            if(min_delta_distance < 1000)
            {
                wx_array[landmass_idx] = (uint8_t)target_square_wx;
                wy_array[landmass_idx] = (uint8_t)target_square_wy;
            }
        }
    }
}
```

**Intent:** for landmasses we hold (`lmt_Own`), if a war target exists on this plane, reposition the stage point toward the war landmass. Step A computes the centroid of the war landmass's dock squares; Step B finds this landmass's dock square closest to that centroid. If found, overwrite the stage point.

**Source-flagged OGBUGs:**
- Divide-by-zero risk: Step A divides by `landmass_node_count` without checking for 0. If the war landmass has no dock squares, divide-by-zero. (Same OGBUG as the sibling Phase in `AI_SingleCont_Reeval__WIP`.)
- No `Random(20)` jitter on the weight.

## Phase 10 — Cleanup

```c
EMM_Map_DataH();
```

Restores the default data-segment mapping in the EMS page frame, undoing the CONTXXX page-in from Phase 1.

## Source-flagged OGBUGs — summary

All preserved as faithful-to-disassembly with inline `/* OGBUG */` markers:

| Phase | Line | OGBUG |
|---|---|---|
| 3 | 7141 | Cross-plane stale `wp` in evaluation-map lookup (`should check unit wp`) |
| 3 | 7142 | Settlers and transports not excluded from order-cancellation |
| 5 | 7214 | Should exclude non-military rather than just Settlers (or, why exclude anything?) |
| 5 | 7242 | `lmt_NoTargets` short-circuit checks against type_array slot while it's still being used as a city counter (accidentally matches 6-city continents) |
| 5 | 7257 | Wartime ratio `enemy × 10` could overflow on large continents |
| 6 | 7331 | `landmass_node_centroid_wx`/`wy` uninitialized at use (set later in Phase 9) |
| 7 | 7413 | `target_landmass_idx` assigned but unused |
| 8 | 7458 | No `Random(20)` jitter on the weight (Phase 6 has it) |
| 8 | 7465 | `target_landmass_idx` assigned but unused |
| 9 | 7513 | Divide by zero — `landmass_node_count` could be 0 if war landmass has no dock squares |
| 9 | 7520 | No `Random(20)` jitter on the weight |

## Related references

- [AIMOVE-AI_SingleCont_Reeval__WIP.md](AIMOVE-AI_SingleCont_Reeval__WIP.md) — per-landmass sibling; shares Phase 4-7 structure
- [AIMOVE-AI_Choose_War_Landmass.md](AIMOVE-AI_Choose_War_Landmass.md) — consumer of `_ai_landmass_war_targets` (which Phase 9 here reads)
- [MoM-AI-Landmass-Types.md](MoM-AI-Landmass-Types.md) — `lmt_*` state machine; this function is a major writer
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
