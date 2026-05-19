AIMOVE-AI_Continent_Reeval__WIP.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr162\AI_Continent_Reeval__WIP.c

---

# AI_Continent_Reeval__WIP — Walkthrough (re-review with GEMINI second opinion)

**Location:** [MoM/src/AIMOVE.c:6855](../../MoM/src/AIMOVE.c#L6855) (~624 lines, ends line 7479). GEMINI variant at [line 7481-7779](../../MoM/src/AIMOVE.c#L7481).
**WZD overlay:** ovr162, p35
**drake178 name:** `AI_Continent_Reeval()`

## Purpose

**Full-rebuild classifier for ALL landmasses on BOTH planes for one player**, throttled to ~25-43 turns. The counterpart to per-landmass [`AI_SingleCont_Reeval__WIP`](AIMOVE-AI_SingleCont_Reeval__WIP.md).

Outputs:
1. **`_ai_continents.plane[*][player_idx].type_array[*]`** — `lmt_*` classification per (plane, landmass)
2. **`_ai_continents.plane[*][player_idx].wx_array[*]` / `wy_array[*]`** — stage point per (plane, landmass)
3. **`AI_NewColConts[*][player_idx]` / `AI_NewColTgtXs[*][player_idx]` / `AI_NewColTgtYs[*][player_idx]`** — chosen colonization target per plane

## Re-review headline

**TRANSLATION-BUG-A is fixed** (was: countdown reset used random as array index). Lines 6905-6914 now properly implement the throttle:

```c
if(found_target == ST_TRUE)                                     // ← NEW: Phase 1 force-reeval
{
    _ai_reevaluate_continents_countdown[player_idx] = 0;
}
_ai_reevaluate_continents_countdown[player_idx] -= 1;
if(_ai_reevaluate_continents_countdown[player_idx] > 0)
{
    return ;
}
_ai_reevaluate_continents_countdown[player_idx] = (25 + Random(10) + Random(10));   // ← CORRECT: writes to player slot
```

This also clarifies Phase 1's `found_target` semantic: when Phase 1 detects a NoOwnCity landmass whose stage point has new enemy presence, it forces an immediate reeval by zeroing the countdown.

**GEMINI second opinion reveals many of my prior "preserved OG bug" labels were wrong.** Several discrepancies I'd marked as OG-preserved actually look like translation bugs in the production WIP version — GEMINI implements them in ways that match the OG comments and the sibling [`AI_SingleCont_Reeval__WIP`](AIMOVE-AI_SingleCont_Reeval__WIP.md). I've reclassified the bug catalog accordingly.

## When it's called

Caller not yet verified by grep this turn. The throttle reset to `_ai_reevaluate_continents_countdown[player_idx] = 0` at game load ([LoadScr.c:797-799](../../MoM/src/LoadScr.c#L797)) means it fires immediately on the first AI turn, then on a `25 + 2×Random(10)` turn cadence.

## Phases (re-traced from current source)

| # | Lines | Phase | Production state |
|---|---|---|---|
| 0 | 6887-6891 | EMM page in + clear found_target | OK |
| 1 | 6892-6904 | Scan for stale NoOwnCity colony targets | OK |
| 2 | 6905-6914 | Throttle gate + force-reeval on stale | **FIXED** (was TRANSLATION-BUG-A) |
| 3 | 6923-6935 | Cancel orders of player units moving to empty tiles | Has OGBUG-A, OGBUG-B (cross-plane, settler/transport not excluded) |
| 4 | 6941-6957 | Clear type_array + sum arrays | OK |
| 5 | 6965-6981 | Accumulate city coords + city count | Has stale-`wp` bug (DIVERGES from GEMINI) |
| 6 | 6988-7007 | Compute city centroids | OK (modulo Phase 5 data) |
| 7 | 7012-7023 | Re-clear sum arrays for cost reuse | OK |
| 8 | 7030-7060 | Accumulate own/enemy unit costs | Has stale-`wp` bug (DIVERGES from GEMINI) |
| 9 | 7070-7117 | Classify Own/Contested/NoOwnCity | Has multiple bugs (DIVERGES from GEMINI) |
| 10 | 7123-7151 | Allied-territory promotion + slot force | Has bugs (DIVERGES from GEMINI) |
| 11 | 7157-7235 | Pick new colony continent per plane | Has multiple bugs (DIVERGES from GEMINI) |
| 12 | 7245-7337 | Refine stage point for Own/Contested/AllyLand | Has bugs (DIVERGES from GEMINI) |
| 13 | 7346-7418 | Set stage for NoOwnCity using embark squares | OK (uses renamed g_world_* arrays correctly) |
| 14 | 7424-7475 | Refine Own continent stage toward main-war embark | Has OGBUG-N (DIVERGES from GEMINI) |
| 15 | 7477 | EMM_Map_DataH() to restore default mapping | OK |

## Production vs GEMINI — divergence catalog

This is the meat of the rereview. Each row compares production WIP to the GEMINI second-opinion variant. The OG-truth column reflects which version better matches the OG comments, the sibling `AI_SingleCont_Reeval` formula, and (where applicable) the semantic intent.

| # | Site | Production WIP | GEMINI | Likely OG truth |
|---|---|---|---|---|
| **D-1** | Phase 5 city accumulation owner filter | No filter — adds ALL cities to centroid sums | `if(_CITIES[itr].owner_idx == player_idx)` filter | **GEMINI** — OG comment at 6960 says "owned cities". Production has missing owner filter. |
| **D-2** | Phase 5 `wp` for store | Uses stale `wp` (left at NUM_PLANES from prior loop) | `wp = _CITIES[itr].wp;` per-city | **GEMINI** — per-city `wp` is the only way the per-(plane,landmass) sum can be correct. |
| **D-3** | Phase 8 `wp` for store | Uses stale `wp` | `wp = _UNITS[itr].wp;` per-unit | **GEMINI** — same reasoning as D-2. |
| **D-4** | Phase 9 cost index | `[wp][itr]` where `itr` is leftover from Phase 7 loop (= NUM_LANDMASSES-1) | `[wp][itr]` where `itr` IS the landmass-iterator in GEMINI's outer for-loop | **GEMINI** — the production index `itr` is stale, reading one fixed slot for the cost comparison across all landmasses. |
| **D-5** | **Phase 9 ratio direction** | `own > enemy / 10` (TOO LENIENT — own only needs to be 10% of enemy) | `own > enemy * 10` (CORRECT — own needs to be 10× enemy) | **GEMINI** — matches OG comment "if enemy forces are less than 10% of ours, we own it" AND matches sibling [`AI_SingleCont_Reeval__WIP:6025`](../../MoM/src/AIMOVE.c#L6025) which uses `(enemy * 10) < own`. Production has the divisor on the wrong side. **High-impact bug** — production classifies far too many landmasses as `lmt_Own`. |
| **D-6** | Phase 10 allied check | `_CITIES[itr].owner_idx == NEUTRAL_PLAYER_IDX` (combined with `!= player_idx`) → mutually exclusive with DIPL_Alliance check → branch NEVER fires | `_CITIES[itr].owner_idx != NEUTRAL_PLAYER_IDX` (any non-neutral non-self) | **GEMINI** — allied-promotion is intended to fire for actual ally wizard cities, not neutral villages. Production's `==` is a translation typo. |
| **D-7** | Phase 10 Dipl_Status direction | `_players[_CITIES[itr].owner_idx].Dipl.Dipl_Status[player_idx]` (reads city-owner's status of us) | `_players[player_idx].Dipl.Dipl_Status[_CITIES[itr].owner_idx]` (reads our status of city-owner) | **GEMINI** — the canonical direction in this codebase is "I read my own Dipl table for my view of other players." Production may also be correct if Dipl_Status is symmetric, but GEMINI's form is the standard idiom. Worth checking. |
| **D-8** | Phase 10 slot-0 sentinel force | `type_array[landmass_idx] = ...` (uses STALE landmass_idx, writes to random slot) | `type_array[0] = ...` (correctly forces slot 0 = ocean sentinel) | **GEMINI** — sibling [`AI_SingleCont_Reeval__WIP:6047-6049`](../../MoM/src/AIMOVE.c#L6047) also uses `[0]`. Production is a translation typo. |
| **D-9** | Phase 11 outer loop iterator | `for(itr = 0; itr < NUM_PLANES; itr++)` but body uses `wp` (stale) — both iterations operate on same plane | `for(wp = 0; wp < NUM_PLANES; wp++)` | **GEMINI** — production's loop runs twice but both iterations use the same stale `wp`, so plane 0 gets processed twice and plane 1 not at all. |
| **D-10** | Phase 11 enemy-cost gate index | `sum_enemy[wp][itr] == 0` (stale `itr`) | `Sum_*_NonOwn_UnitCost[wp][landmass_idx] != 0` | **Both wrong, differently.** Production's index is stale; GEMINI's operator is inverted (the colonization-target intent wants empty landmasses, i.e., `== 0`). The OG truth is probably "`landmass_idx`, `== 0`" — needs verification. |
| **D-11** | Phase 11 chain walk condition | `while(((Next_Tile_ChainIndex = CONTX_TileChain[wp][Next_Tile_ChainIndex] != ST_UNDEFINED) && (found_target != ST_FALSE)))` — operator precedence makes the assignment receive a 0/1 boolean | `while(Next_Tile_ChainIndex != ST_UNDEFINED && found_target == ST_FALSE)` | **GEMINI** — production's chain walk is broken by precedence; the assignment-in-condition needs outer parens. AND production has `found_target != ST_FALSE` (skip unless detected) which is inverted from intent. |
| **D-12** | Phase 11 `Target_Tile_Y` source | `Target_Tile_Y = CONTX_TileXs[wp][Next_Tile_ChainIndex]` (X coord!) | `Target_Tile_Y = CONTX_TileYs[wp][Next_Tile_ChainIndex]` | **GEMINI** — production has TileXs/TileYs copy-paste typo. |
| **D-13** | Phase 11 X_2/Y_2 initialization | Uninitialized — distance "from empire" is from (0,0) | Same (uninitialized) | **OG bug** — both versions have this. Not flagged differently. |
| **D-14** | Phase 12 Center_X, Y empty-tile check | `g_ai_evaluation_map[wp][Tile_Linear]` (uses STALE Tile_Linear from Phase 11) | `g_ai_evaluation_map[wp][Center_Y * WORLD_WIDTH + Center_X]` (correctly indexes by Center coords) | **GEMINI** — production reads the wrong tile. |
| **D-15** | **Phase 12 condition polarity** | `if (g_ai_evaluation_map == 0 && _landmasses[...] == landmass_idx)` — runs the search when centroid IS valid (no-op) | `if (g_ai_evaluation_map != 0 \|\| _landmasses[...] != landmass_idx)` — runs the search when centroid is INVALID | **GEMINI** — OG comment at 7238-7243 says "ensure that stage points are empty land tiles on them, or otherwise set them to [0,0]." That's "find a replacement when current is bad" — GEMINI's polarity. Production's polarity makes Phase 12 effectively skip when it should be running. |
| **D-16** | Phase 12 `Target_Tile_Y` source | `Target_Tile_Y = CONTX_TileXs[wp][Next_Tile_ChainIndex]` (X coord!) | `Target_Tile_Y = CONTX_TileYs[wp][Next_Tile_ChainIndex]` | **GEMINI** — same typo pattern as D-12. |
| **D-17** | Phase 12 outer loop iterator | Same `for itr` / use `wp` mismatch as D-9 | `for(wp = 0; ...)` | **GEMINI** — same reasoning. |
| **D-18** | Phase 13 outer loop iterator | Same mismatch | `for(wp = 0; ...)` | **GEMINI** |
| **D-19** | Phase 13 jitter | No jitter | `+ Random(20)` | **GEMINI?** — jitter prevents deterministic tile picks; Phase 11 and Phase 14 have it. Production missing it may be a translation omission. |
| **D-20** | Phase 14 outer loop iterator | Same mismatch | `for(wp = 0; ...)` | **GEMINI** |
| **D-21** | Phase 14 Step A accumulation | `X_2 = ...; Y_2 = ...;` (overwrite) | `X_2 += ...; Y_2 += ...;` (accumulate) | **GEMINI** — needs to be `+=` to compute a centroid; production overwrites and divides last coord by count. Same bug pattern as OGBUG-N which I previously flagged. |
| **D-22** | End-of-function cleanup | `EMM_Map_DataH()` | `EMMDATAH_Map()` (function name typo) | **PRODUCTION** — `EMM_Map_DataH` is the canonical name; GEMINI has a typo here. |

**Tally:** of 22 divergences I traced:
- **18 favor GEMINI** (production has a likely translation bug)
- **1 favors production** (GEMINI has a typo in function call)
- **1 has both wrong differently** (D-10, both indexing and operator)
- **1 is shared OG bug** (D-13, both uninitialized)
- **1 needs further verification** (D-7, Dipl_Status direction)

The pattern is striking: **production WIP has accumulated a lot of small translation errors** that GEMINI doesn't have. If GEMINI's reading is closer to the actual OG bytes (which would need disassembly verification), then production has many fixable translation defects in this function.

## Signature and locals

```c
void AI_Continent_Reeval__WIP(int16_t player_idx)
{
    int16_t sum_own_city_wy__enemy_units_cost[NUM_PLANES][NUM_LANDMASSES] = { ... };
    int16_t sum_own_city_wx__own_units_cost[NUM_PLANES][NUM_LANDMASSES] = { ... };
    int16_t Tile_Linear = 0;
    int16_t found_target = 0;             // ← Phase 1 sets it; Phase 2 uses it to force-reeval
    int16_t Center_Y = 0;
    int16_t Target_Tile_Landmass = 0;
    int16_t Tile_Weight = 0;
    int16_t Target_Tile_Y = 0;
    int16_t Target_Tile_X = 0;
    int16_t Center_X = 0;
    int16_t Best_Tile_Weight = 0;
    int16_t unit_type = 0;
    int16_t Y_2 = 0;                       // ← OGBUG-H: used uninitialized in Phase 11
    int16_t X_2 = 0;                       // ← OGBUG-H: same
    int16_t Next_Tile_ChainIndex = 0;
    int16_t city_count = 0;
    int16_t Tile_Y = 0;
    int16_t Tile_X = 0;
    int16_t itr = 0;
    int16_t landmass_idx = 0;
    int16_t wp = 0;                        // ← reused across phases — see D-2, D-3, D-9, D-17, D-18, D-20
    ...
}
```

The local sum arrays are dual-use: holds city coordinate sums after Phase 5, then re-cleared in Phase 7 and reused for unit-cost sums after Phase 8. The `__` separator in the names captures this dual purpose.

## Phase 1 — Early detection: stale `NoOwnCity` targets with new enemy presence

```c
// AIMOVE.c:6889-6904
// ; check if there's at least one type 3 continent that
// ; has a non-owned unit or structure at its saved tile
found_target = ST_FALSE;
for(wp = 0; wp < NUM_PLANES; wp++)
{
    for(landmass_idx = 0; ((landmass_idx < NUM_LANDMASSES) && (found_target == ST_FALSE)); landmass_idx++)
    {
        if(_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_NoOwnCity)
        {
            if(g_ai_evaluation_map[wp][((wy * WORLD_WIDTH) + wx)] != 0)
            {
                found_target = ST_TRUE;
            }
        }
    }
}
```

**Intent:** scan for any `lmt_NoOwnCity` landmass whose previously-chosen colony stage tile now has enemy presence. If found, set `found_target = TRUE`. Phase 2 uses this flag to bypass the throttle.

## Phase 2 — Throttle gate + force-reeval (FIXED)

```c
// AIMOVE.c:6905-6914
if(found_target == ST_TRUE)
{
    _ai_reevaluate_continents_countdown[player_idx] = 0;     // ← force-reeval if Phase 1 detected stale
}
_ai_reevaluate_continents_countdown[player_idx] -= 1;
if(_ai_reevaluate_continents_countdown[player_idx] > 0)
{
    return;
}
_ai_reevaluate_continents_countdown[player_idx] = (25 + Random(10) + Random(10));
```

**Previously flagged TRANSLATION-BUG-A** (countdown reset used random as array index) is now correctly written: the player's countdown slot is set to a new 25-43 turn period after the decrement check.

## Phase 3 — Cancel orders for player units moving to empty tiles

```c
// AIMOVE.c:6923-6935
// ; cancel the orders of all non-engineer units moving
// ; to a tile with no enemy presence
// ; 
// ; BUG: fails to check the plane of movement
// ; BUG? settlers and transports are not excluded
for(itr = 0; itr < _units; itr++)
{
    if(
        (_UNITS[itr].owner_idx == player_idx)
        && (_unit_type_table[_UNITS[itr].type].Construction == 0)
        && (g_ai_evaluation_map[wp][((_UNITS[itr].dst_wy * WORLD_WIDTH) + _UNITS[itr].dst_wx)] == 0)
    )
    {
        _UNITS[itr].Status = us_Ready;
    }
}
```

### 🐛 OGBUG-A — doesn't check plane of movement

`g_ai_evaluation_map[wp][...]` uses the stale `wp` left at `NUM_PLANES = 2` after Phase 1's loop. Cross-plane false-positive cancellations. **GEMINI has the same bug** — present in the OG too.

### 🐛 OGBUG-B — settlers and transports not excluded

Only engineers are excluded. Settlers and transports get orders cancelled too. **GEMINI same.**

## Phase 5 — Accumulate per-landmass: city coordinates + city count

```c
// AIMOVE.c:6965-6981 (current production)
for(itr = 0; itr < _cities; itr++)
{
    landmass_idx = _landmasses[((_CITIES[itr].wp * WORLD_SIZE) + (_CITIES[itr].wy * WORLD_WIDTH) + _CITIES[itr].wx)];

    if(landmass_idx < NUM_LANDMASSES)
    {
        sum_own_city_wx__own_units_cost[wp][landmass_idx] += _CITIES[itr].wx;       // ← D-2: wp STALE
        sum_own_city_wy__enemy_units_cost[wp][landmass_idx] += _CITIES[itr].wy;     // ← D-2: wp STALE
        _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] += 1;  // ← D-2: wp STALE
    }
}
```

Compare with GEMINI:
```c
for (itr = 0; itr < _cities; itr++) {
    if (_CITIES[itr].owner_idx == player_idx) {                                     // ← D-1: owner filter
        wp = _CITIES[itr].wp;                                                       // ← D-2: per-city wp
        landmass_idx = _landmasses[wp * WORLD_SIZE + _CITIES[itr].wy * WORLD_WIDTH + _CITIES[itr].wx];
        if (landmass_idx < NUM_LANDMASSES) {
            Sum_City_X_Own_UnitCost[wp][landmass_idx] += _CITIES[itr].wx;
            Sum_City_Y_NonOwn_UnitCost[wp][landmass_idx] += _CITIES[itr].wy;
            _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx]++;
        }
    }
}
```

GEMINI correctly filters by owner (D-1) and computes per-city `wp` (D-2). Production has both bugs.

## Phase 9 — Classify: Own / Contested / NoOwnCity

```c
// AIMOVE.c:7077-7113 (current production)
if(
    (type_array[wp][landmass_idx] != lmt_NoTargets)                           // OK
    ||
    (sum_own_city_wy__enemy_units_cost[wp][itr] != 0)                         // ← D-4: itr stale
)
{
    if(type_array[wp][landmass_idx] > lmt_Unevaluated)
    {
        if(sum_own_city_wx__own_units_cost[wp][itr]                           // ← D-4: itr stale
           > (sum_own_city_wy__enemy_units_cost[wp][itr] / 10))               // ← D-5: WRONG DIRECTION
        {
            type_array[wp][landmass_idx] = lmt_Own;
        }
        else
        {
            type_array[wp][landmass_idx] = lmt_Contested;
        }
    }
    else
    {
        type_array[wp][landmass_idx] = lmt_NoOwnCity;
    }
}
```

Compare with GEMINI:
```c
for (itr = 0; itr < NUM_LANDMASSES; itr++) {                                  // ← itr IS landmass index
    ...
    if (Sum_City_X_Own_UnitCost[wp][itr] > Sum_City_Y_NonOwn_UnitCost[wp][itr] * 10) {  // ← * 10, not / 10
        type_array[itr] = lmt_Own;
    } else {
        type_array[itr] = lmt_Contested;
    }
    ...
}
```

**D-5 is the highest-impact divergence in this function.** Production says "we own if our cost > 10% of enemy" (too lenient); GEMINI says "we own if our cost > 10× enemy" (matches OG comment and sibling SingleCont). Production almost certainly mis-classifies far too many landmasses as `lmt_Own`. This has ripple effects through every consumer of the type_array.

## Phase 14 — Refine Own stages toward main-war embark (Step A accumulation bug)

```c
// AIMOVE.c:7437-7445 (current production)
Next_Tile_ChainIndex = g_world_landmass_first_embark_square__load_init[wp][_ai_landmass_war_targets[wp][player_idx]];
while(Next_Tile_ChainIndex != ST_UNDEFINED)
{
    X_2 = g_world_embark_square_wx__load_init[wp][Next_Tile_ChainIndex];     // ← D-21: OVERWRITE, not accumulate
    Y_2 = g_world_embark_square_wy__load_init[wp][Next_Tile_ChainIndex];
    Tile_Linear++;
    Next_Tile_ChainIndex = g_world_embark_square_next__load_init[wp][Next_Tile_ChainIndex];
}
X_2 /= Tile_Linear;     // last X / count, not centroid
Y_2 /= Tile_Linear;
```

GEMINI ([line 7745-7752](../../MoM/src/AIMOVE.c#L7745)) correctly uses `+=`:
```c
while (Next_Tile_ChainIndex != ST_UNDEFINED) {
    X_2 += g_world_embark_square_wx__load_init[wp][Next_Tile_ChainIndex];
    Y_2 += g_world_embark_square_wy__load_init[wp][Next_Tile_ChainIndex];
    Tile_Linear++;
    Next_Tile_ChainIndex = g_world_embark_square_next__load_init[wp][Next_Tile_ChainIndex];
}
X_2 = X_2 / Tile_Linear;
Y_2 = Y_2 / Tile_Linear;
```

Same `+=`-vs-`=` pattern that you fixed elsewhere. **D-21 is a clear-cut translation typo.**

## Updated bug catalog

| # | Severity | Site | Description | Status |
|---|---|---|---|---|
| ~~TRANSLATION-BUG-A~~ | ~~Critical~~ | ~~Line 6929~~ | ~~Countdown reset uses random as array index~~ | **FIXED** in current source |
| OGBUG-A | Medium | Line 6930 (Phase 3) | Cross-plane: uses stale `wp` for tile evaluation lookup | Shared with GEMINI — OG bug |
| OGBUG-B | Low | Line 6921 (Phase 3) | Settlers and transports get orders cancelled | Shared with GEMINI — OG bug |
| OGBUG-H | Known | Lines 7190-7194 (Phase 11) | `X_2`/`Y_2` uninitialized — distance from (0,0) | Shared with GEMINI — OG bug (commented) |
| OGBUG-D | Known | Line 7068 (Phase 9) | 6-own-cities-no-enemies → stuck at `lmt_NoTargets` | Shared with GEMINI — OG bug (commented) |
| **D-1** | High | Phase 5 (line 6965) | Missing `owner_idx == player_idx` filter in city accumulation | **Likely translation bug** (GEMINI has filter) |
| **D-2** | High | Phase 5 (lines 6973-6977) | Stale `wp` for centroid store | **Likely translation bug** (GEMINI uses per-city `wp`) |
| **D-3** | High | Phase 8 (lines 7046, 7052) | Stale `wp` for cost store | **Likely translation bug** |
| **D-4** | High | Phase 9 (lines 7080, 7092) | Stale `itr` for cost index | **Likely translation bug** |
| **D-5** | **Critical** | Phase 9 (line 7092) | Ratio direction `/ 10` instead of `* 10` — over-classifies as Own | **Likely translation bug** — production diverges from both OG comment and sibling SingleCont |
| **D-6** | High | Phase 10 (line 7129) | Allied-promotion `== NEUTRAL` (branch never fires) | **Likely translation bug** |
| **D-7** | Medium | Phase 10 (line 7131) | Dipl_Status direction (city-owner-of-us vs us-of-city-owner) | Verify against OG — may be equivalent |
| **D-8** | High | Phase 10 (lines 7149, 7151) | Slot-0 force uses stale `landmass_idx` instead of `0` | **Likely translation bug** |
| **D-9** | High | Phase 11 (line 7157) | `for itr` but body uses `wp` — same plane processed twice | **Likely translation bug** |
| **D-10** | High | Phase 11 (line 7171) | Enemy-cost gate index AND operator both wrong | **Both wrong differently** — needs OG check |
| **D-11** | High | Phase 11 (line 7177) | Chain walk operator precedence + inverted polarity | **Likely translation bug** |
| **D-12** | High | Phase 11 (line 7204) | `Target_Tile_Y = CONTX_TileXs` (X coord!) | **Likely translation bug** |
| **D-14** | Medium | Phase 12 (line 7265) | Centroid empty-check uses stale `Tile_Linear` | **Likely translation bug** |
| **D-15** | **Critical** | Phase 12 (lines 7264-7268) | Condition polarity inverted — Phase 12 runs when stage IS valid (no-op) | **Likely translation bug** — should run when stage is invalid |
| **D-16** | High | Phase 12 (line 7300) | `Target_Tile_Y = CONTX_TileXs` (X coord!) | **Likely translation bug** |
| **D-17** | High | Phase 12 (line 7245) | Same loop iterator mismatch as D-9 | **Likely translation bug** |
| **D-18** | High | Phase 13 (line 7346) | Same loop iterator mismatch | **Likely translation bug** |
| **D-19** | Low | Phase 13 (line 7374) | Missing `Random(20)` jitter | Possible translation omission (compare to Phases 11, 14) |
| **D-20** | High | Phase 14 (line 7424) | Same loop iterator mismatch | **Likely translation bug** |
| **D-21** | **Critical** | Phase 14 (lines 7441-7442) | `=` overwrite instead of `+=` accumulate — last embark / count, not centroid | **Likely translation bug** |

## Comparison with sibling `AI_SingleCont_Reeval__WIP`

Both functions implement the same general "classify landmasses + pick stage points" algorithm with the same OG-derived structure. The per-landmass sibling has different bugs:

| Pattern | `AI_SingleCont_Reeval__WIP` (per-landmass) | `AI_Continent_Reeval__WIP` (full rebuild) |
|---|---|---|
| Zero type_array before count | Yes (line 5955) | Yes (line 6947) |
| Has unit-cost loop | Yes | Yes |
| Coord/cost var clearing | Has bug (vars not cleared, OGBUG) | Properly cleared (Phase 7) |
| Slot-0 sentinel force | Correct `[0]` | **D-8: stale `landmass_idx`** |
| Ratio direction | Correct `enemy * 10 < own` | **D-5: wrong `own > enemy / 10`** |
| Cross-plane stale `wp` | N/A (per-call wp parameter) | **D-2, D-3, D-9, D-17, D-18, D-20** |
| Centroid accumulation in Phase 14 analog | Correct `+= ` (verified earlier this turn) | **D-21: `=` overwrite** |
| Main-war refinement write-back | BUG-7: missing | Present (write-back works) |

The sibling has fewer translation bugs in the body but is missing the Phase 7 write-back (BUG-7). The full-rebuild has the Phase 14 write-back but many more translation issues elsewhere. **The two functions are likely both translations of the same OG code, done at different times with different defects.**

## Recommendation

Given the GEMINI variant resolves so many of these discrepancies in ways that match OG comments and the sibling function, it's worth verifying each "Likely translation bug" tag against the actual disassembly bytes. The high-impact ones (D-5, D-15, D-21) materially change the function's behavior:

- **D-5** (ratio direction) — over-classifies landmasses as Own, distorting downstream targeting
- **D-15** (Phase 12 polarity) — Phase 12's stage-point refinement effectively never runs
- **D-21** (Phase 14 accumulation) — Phase 14's main-war stage refinement computes garbage centroid

If those three turn out to be translation bugs, fixing them would substantially improve AI behavior compared to current production state.

## Related references

- [AIMOVE-AI_SingleCont_Reeval__WIP.md](AIMOVE-AI_SingleCont_Reeval__WIP.md) — per-landmass sibling; shares Phase 5-7 structure
- [AIMOVE-AI_Choose_War_Landmass](AIMOVE-AI_AI_Choose_War_Landmass consumer of `_ai_landmass_war_targets` (Phase 14 reads here)
- [MoM-AI-Landmass-Types.md](MoM-AI-Landmass-Types.md) — `lmt_*` state machine; this function is a major writer
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
