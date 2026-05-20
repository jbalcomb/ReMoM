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

**⚠️ Important caveat from the BUG-B lesson (sibling `AI_Choose_War_Landmass`):** I previously flagged D-* divergences as "Likely OG truth: GEMINI" when production diverged from GEMINI and from OG comments. **One such claim has since been falsified.** The user verified BUG-B in `AI_Choose_War_Landmass` (the analogous fortress-continent polarity divergence in that function) against the disassembly: **production matched the OG bytes; the comment and GEMINI were both wrong.**

This means **every "Likely OG truth: GEMINI" label in the table below is a hypothesis, not a finding.** GEMINI is a translation too, not OG-truth. The OG comments can be wrong about what the code does. The only authoritative source is the disassembly bytes themselves.

The table below has been updated to reframe each divergence as "Needs disassembly verification" rather than asserting GEMINI is correct. The divergences are still real and worth investigating — but the resolution requires the bytes, not pattern-matching to GEMINI.

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

This is the meat of the rereview. Each row compares production WIP to the GEMINI second-opinion variant. **Per the BUG-B lesson:** the "Resolution" column now indicates what to verify against disassembly, NOT a claim that GEMINI is correct. GEMINI is a translation too. Each divergence is a hypothesis to test, not a conclusion.

| # | Site | Production WIP | GEMINI | Resolution |
|---|---|---|---|---|
| **D-1** | Phase 5 city accumulation owner filter | No filter — adds ALL cities to centroid sums | `if(_CITIES[itr].owner_idx == player_idx)` filter | **Needs disassembly verification.** OG comment at 6965 says "owned cities" — but per BUG-B, the comment can be wrong about what the code does. Production may be intentional (centroid of all cities). |
| **D-2** | Phase 5 `wp` for store | Uses stale `wp` (left at NUM_PLANES from prior loop) | `wp = _CITIES[itr].wp;` per-city | **Needs disassembly verification.** Stale `wp` would mean all per-(plane,landmass) sums get stuffed into one plane's slot — semantically broken if the OG intended per-plane sums. But the OG may have a register-allocation pattern I'm not seeing in the C translation. |
| **D-3** | Phase 8 `wp` for store | Uses stale `wp` | `wp = _UNITS[itr].wp;` per-unit | **Needs disassembly verification** — same reasoning as D-2. |
| **D-4** | Phase 9 cost index | `[wp][itr]` where `itr` is leftover from Phase 7 loop (= NUM_LANDMASSES-1) | `[wp][itr]` where `itr` IS the landmass-iterator in GEMINI's outer for-loop | **Needs disassembly verification.** Production reads one stale slot for the cost comparison across all landmasses — semantically suspicious, but possible the OG dev intentionally folded the check. |
| **D-5** | **Phase 9 ratio direction** | `own > enemy / 10` | `own > enemy * 10` | **Needs disassembly verification.** Sibling [`AI_SingleCont_Reeval__WIP:6014`](../../MoM/src/AIMOVE.c#L6014) uses `(enemy * 10) < own` (equivalent to `own > enemy * 10`). The two forms classify very differently — `/ 10` is much more lenient. If both functions are supposed to share the same classification rule, one of them is wrong; disassembly tells us which. This is the highest-impact divergence to verify — it materially affects which landmasses are classified `lmt_Own`. |
| **D-6** | Phase 10 allied check | `_CITIES[itr].owner_idx == NEUTRAL_PLAYER_IDX` (combined with `!= player_idx`) → mutually exclusive with DIPL_Alliance check → branch effectively never fires for real allies | `_CITIES[itr].owner_idx != NEUTRAL_PLAYER_IDX` (any non-neutral non-self) | **Needs disassembly verification.** Production's `==` reading would make the branch dead-code for the intended ally case, which is suspicious — but possible the OG bytes really have `==` and the dev intended the branch to only handle some edge case. |
| **D-7** | Phase 10 Dipl_Status direction | `_players[_CITIES[itr].owner_idx].Dipl.Dipl_Status[player_idx]` (reads city-owner's status of us) | `_players[player_idx].Dipl.Dipl_Status[_CITIES[itr].owner_idx]` (reads our status of city-owner) | **Needs disassembly verification.** If Dipl_Status is symmetric (both players view each other identically), both forms give the same answer. If asymmetric, only one is right. |
| **D-8** | Phase 10 slot-0 sentinel force | `type_array[landmass_idx] = ...` (uses STALE landmass_idx, writes to random slot) | `type_array[0] = ...` (correctly forces slot 0 = ocean sentinel) | **Needs disassembly verification.** Sibling [`AI_SingleCont_Reeval__WIP:6051-6052`](../../MoM/src/AIMOVE.c#L6051) uses literal `[0]`. If both functions follow the same OG pattern, one of them is wrong. Disassembly resolves it. |
| **D-9** | Phase 11 outer loop iterator | `for(itr = 0; itr < NUM_PLANES; itr++)` but body uses `wp` (stale) — both iterations operate on same plane | `for(wp = 0; wp < NUM_PLANES; wp++)` | **Needs disassembly verification.** Production's loop semantically would do the same thing twice — suspicious — but possible the OG used a non-`wp` iterator name and the C translation kept the wrong one. |
| **D-10** | Phase 11 enemy-cost gate index | `sum_enemy[wp][itr] == 0` (stale `itr`) | `Sum_*_NonOwn_UnitCost[wp][landmass_idx] != 0` | **Needs disassembly verification.** Both forms look suspect — production's index is stale, GEMINI's operator may have inverted polarity. Disassembly check reveals which (if any) matches OG. |
| **D-11** | Phase 11 chain walk condition | `while(((Next_Tile_ChainIndex = CONTX_TileChain[wp][Next_Tile_ChainIndex] != ST_UNDEFINED) && (found_target != ST_FALSE)))` — operator precedence makes the assignment receive a 0/1 boolean | `while(Next_Tile_ChainIndex != ST_UNDEFINED && found_target == ST_FALSE)` | **GEMINI** — production's chain walk is broken by precedence; the assignment-in-condition needs outer parens. AND production has `found_target != ST_FALSE` (skip unless detected) which is inverted from intent. |
| **D-12** | Phase 11 `Target_Tile_Y` source | `Target_Tile_Y = CONTX_TileXs[wp][Next_Tile_ChainIndex]` (X coord!) | `Target_Tile_Y = CONTX_TileYs[wp][Next_Tile_ChainIndex]` | **Needs disassembly verification.** Looks like a TileXs/TileYs copy-paste typo in production, but per BUG-B lesson, the OG may genuinely have this — it could be intentional (using X coord for Y stage point) or a long-standing OG bug. Check the bytes. |
| **D-13** | Phase 11 X_2/Y_2 initialization | Uninitialized — distance "from empire" is from (0,0) | Same (uninitialized) | **OG bug** — both versions have this. Genuinely shared, no divergence. |
| **D-14** | Phase 12 Center_X, Y empty-tile check | `g_ai_evaluation_map[wp][Tile_Linear]` (uses STALE Tile_Linear from Phase 11) | `g_ai_evaluation_map[wp][Center_Y * WORLD_WIDTH + Center_X]` (correctly indexes by Center coords) | **Needs disassembly verification.** Production reads with stale `Tile_Linear` — semantically suspicious — but the OG may have intentionally folded the index. |
| **D-15** | **Phase 12 condition polarity** | `if (g_ai_evaluation_map == 0 && _landmasses[...] == landmass_idx)` — runs the search when centroid IS valid (no-op) | `if (g_ai_evaluation_map != 0 \|\| _landmasses[...] != landmass_idx)` — runs the search when centroid is INVALID | **Needs disassembly verification.** Same pattern as BUG-B in `AI_Choose_War_Landmass` — production polarity contradicts the OG comment's apparent intent. Just because BUG-B turned out to be production-correct doesn't mean this one is, but it's a strong reminder to NOT assume the comment is the truth. Check the bytes. |
| **D-16** | Phase 12 `Target_Tile_Y` source | `Target_Tile_Y = CONTX_TileXs[wp][Next_Tile_ChainIndex]` (X coord!) | `Target_Tile_Y = CONTX_TileYs[wp][Next_Tile_ChainIndex]` | **Needs disassembly verification** — same as D-12. |
| **D-17** | Phase 12 outer loop iterator | Same `for itr` / use `wp` mismatch as D-9 | `for(wp = 0; ...)` | **Needs disassembly verification** — same reasoning as D-9. |
| **D-18** | Phase 13 outer loop iterator | Same mismatch | `for(wp = 0; ...)` | **Needs disassembly verification** — same as D-9. |
| **D-19** | Phase 13 jitter | No jitter | `+ Random(20)` | **Needs disassembly verification.** Phase 11 and Phase 14 have jitter; Phase 13 doesn't. Either's a translation omission or the OG deliberately makes this phase deterministic. |
| **D-20** | Phase 14 outer loop iterator | Same mismatch | `for(wp = 0; ...)` | **Needs disassembly verification** — same as D-9. |
| **D-21** | Phase 14 Step A accumulation | `X_2 = ...; Y_2 = ...;` (overwrite) | `X_2 += ...; Y_2 += ...;` (accumulate) | **Needs disassembly verification.** The user just confirmed in `AI_Choose_War_Landmass` that an analogous `+=`-vs-`=` mistake (BUG-A) WAS a transcription error in production, fixed by switching to `+=`. So the base rate for this kind of pattern is non-zero — but per BUG-B, don't assume; verify. |
| **D-22** | End-of-function cleanup | `EMM_Map_DataH()` | `EMMDATAH_Map()` (function name typo) | **PRODUCTION CORRECT** — `EMM_Map_DataH` is the canonical name in this codebase. GEMINI has a function-name typo. No disassembly check needed for this one — it's a C-language identifier-existence check. |

**Tally:** of 22 divergences traced:
- **20 need disassembly verification** — divergent but no confirmed OG truth
- **1 confirmed shared OG bug** (D-13, both uninitialized)
- **1 confirmed production-correct** (D-22, GEMINI has C-identifier typo)

**Per the BUG-B lesson:** prior wording claimed "18 favor GEMINI." That was wrong. Each divergence is a hypothesis until verified against the bytes. Two pieces of recent evidence from the sibling `AI_Choose_War_Landmass`:
- **BUG-A** (analogous `+=` vs `=` pattern): production WAS wrong, user fixed by switching to `+=`. Suggests transcription errors of this kind do happen in production.
- **BUG-B** (analogous condition polarity): production was RIGHT, OG comment and GEMINI were both wrong. Suggests assuming "comment + GEMINI agree → production wrong" is unsafe.

The base rate cuts both ways. Some of the 20 divergences here probably are production transcription errors (matching the BUG-A pattern); some are probably production-correct (matching the BUG-B pattern). Disassembly is the only way to know which is which.

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
Next_Tile_ChainIndex = _ai_landmass_dock_squares_heads[wp][_ai_landmass_war_targets[wp][player_idx]];
while(Next_Tile_ChainIndex != ST_UNDEFINED)
{
    X_2 = _ai_landmass_dock_squares_wx_array[wp][Next_Tile_ChainIndex];     // ← D-21: OVERWRITE, not accumulate
    Y_2 = _ai_landmass_dock_squares_wy_array[wp][Next_Tile_ChainIndex];
    Tile_Linear++;
    Next_Tile_ChainIndex = _ai_landmass_dock_squares_lists[wp][Next_Tile_ChainIndex];
}
X_2 /= Tile_Linear;     // last X / count, not centroid
Y_2 /= Tile_Linear;
```

GEMINI ([line 7745-7752](../../MoM/src/AIMOVE.c#L7745)) correctly uses `+=`:
```c
while (Next_Tile_ChainIndex != ST_UNDEFINED) {
    X_2 += _ai_landmass_dock_squares_wx_array[wp][Next_Tile_ChainIndex];
    Y_2 += _ai_landmass_dock_squares_wy_array[wp][Next_Tile_ChainIndex];
    Tile_Linear++;
    Next_Tile_ChainIndex = _ai_landmass_dock_squares_lists[wp][Next_Tile_ChainIndex];
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
| **D-1** | Suspected | Phase 5 (line 6970) | Missing `owner_idx == player_idx` filter in city accumulation | **Needs disassembly verification** (GEMINI has filter; OG comment supports it but per BUG-B comments can mislead) |
| **D-2** | Suspected | Phase 5 (lines 6978-6982) | Stale `wp` for centroid store | **Needs disassembly verification** (GEMINI uses per-city `wp`) |
| **D-3** | Suspected | Phase 8 (lines 7036, 7040) | Stale `wp` for cost store | **Needs disassembly verification** |
| **D-4** | Suspected | Phase 9 (lines 7061, 7069) | Stale `itr` for cost index | **Needs disassembly verification** |
| **D-5** | **Suspected (high impact)** | Phase 9 (line 7069) | Ratio direction `/ 10` instead of `* 10` | **Needs disassembly verification** — production diverges from both OG comment and sibling SingleCont's `* 10` form. High impact if production is wrong: over-classifies as Own. |
| **D-6** | Suspected | Phase 10 (line 7092) | Allied-promotion `== NEUTRAL` (branch never fires for real allies) | **Needs disassembly verification** |
| **D-7** | Suspected | Phase 10 (line 7094) | Dipl_Status direction (city-owner-of-us vs us-of-city-owner) | **Needs disassembly verification** — may be equivalent if Dipl_Status is symmetric |
| **D-8** | Suspected | Phase 10 (lines 7105, 7106) | Slot-0 force uses stale `landmass_idx` instead of `0` | **Needs disassembly verification** |
| **D-9** | Suspected | Phase 11 (line 7110) | `for itr` but body uses `wp` — semantically processes one plane twice | **Needs disassembly verification** |
| **D-10** | Suspected | Phase 11 (line 7120) | Enemy-cost gate index AND operator divergent | **Needs disassembly verification** |
| **D-11** | Suspected | Phase 11 (line 7124) | Chain walk operator precedence + polarity divergent | **Needs disassembly verification** |
| **D-12** | Suspected | Phase 11 (line 7142) | `Target_Tile_Y = CONTX_TileXs` (X coord!) | **Needs disassembly verification** |
| **D-14** | Suspected | Phase 12 (line 7181) | Centroid empty-check uses stale `Tile_Linear` | **Needs disassembly verification** |
| **D-15** | **Suspected (high impact)** | Phase 12 (lines 7180-7184) | Condition polarity divergent — Phase 12 runs when stage IS valid (no-op?) | **Needs disassembly verification** — analogous polarity-vs-comment pattern as BUG-B in `AI_Choose_War_Landmass`, where production turned out RIGHT |
| **D-16** | Suspected | Phase 12 (line 7201) | `Target_Tile_Y = CONTX_TileXs` (X coord!) | **Needs disassembly verification** |
| **D-17** | Suspected | Phase 12 (line 7166) | Same loop iterator mismatch as D-9 | **Needs disassembly verification** |
| **D-18** | Suspected | Phase 13 (line 7227) | Same loop iterator mismatch | **Needs disassembly verification** |
| **D-19** | Low | Phase 13 (line 7243) | Missing `Random(20)` jitter | **Needs disassembly verification** — possible translation omission, or possibly intentional |
| **D-20** | Suspected | Phase 14 (line 7272) | Same loop iterator mismatch | **Needs disassembly verification** |
| **D-21** | **Suspected (high impact)** | Phase 14 (lines 7289-7290) | `=` overwrite instead of `+=` accumulate | **Needs disassembly verification** — analogous `+=`-vs-`=` pattern as BUG-A in `AI_Choose_War_Landmass`, where production turned out WRONG (user fixed). Base rate non-zero for this kind. |

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

Given the volume of divergences, it's worth verifying each "Suspected" tag against the actual disassembly bytes. **Per the BUG-B lesson:** don't pre-conclude what the bytes will show. GEMINI's reading is a hypothesis, not OG-truth. The high-impact divergences (D-5, D-15, D-21) materially change the function's behavior IF production turns out wrong:

- **D-5** (ratio direction) — over-classifies landmasses as Own, distorting downstream targeting
- **D-15** (Phase 12 polarity) — Phase 12's stage-point refinement effectively never runs
- **D-21** (Phase 14 accumulation) — Phase 14's main-war stage refinement computes garbage centroid

If those three turn out to be translation bugs, fixing them would substantially improve AI behavior compared to current production state.

## Related references

- [AIMOVE-AI_SingleCont_Reeval__WIP.md](AIMOVE-AI_SingleCont_Reeval__WIP.md) — per-landmass sibling; shares Phase 5-7 structure
- [AIMOVE-AI_Choose_War_Landmass](AIMOVE-AI_AI_Choose_War_Landmass consumer of `_ai_landmass_war_targets` (Phase 14 reads here)
- [MoM-AI-Landmass-Types.md](MoM-AI-Landmass-Types.md) — `lmt_*` state machine; this function is a major writer
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
