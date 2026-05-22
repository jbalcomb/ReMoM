AIMOVE-AI_Build_Target_List.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Build_Target_List.c

SEEALSO: AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md

---

# AI_Build_Target_List — Walkthrough

**Location:** [MoM/src/AIMOVE.c:2536](../../MoM/src/AIMOVE.c#L2536) (~345 lines, ends 2880). GEMINI variant at [line 2882](../../MoM/src/AIMOVE.c#L2882).
**WZD overlay:** ovr158, p10
**drake178 name:** (not listed)

## Purpose

**Builds the per-(player, plane, landmass) target list** that drives roamer stack target assignment. Walks fortresses, cities, free-roaming enemy stacks, nodes, lairs, and towers; calls `AI_Add_Target` to push qualified entries into `_ai_targets_*[25]`. If nothing qualified, tags the landmass `lmt_NoTargets`.

## Output

```c
extern int16_t _ai_targets_count;       // 0..25
extern int16_t _ai_targets_wx[25];
extern int16_t _ai_targets_wy[25];
extern int16_t _ai_targets_strength[25];
extern int16_t _ai_targets_value[25];
```

Also writes `_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_NoTargets` when `_ai_targets_count == 0` after the pass — the only writer of that tag in the codebase. See [AIMOVE-AI_Choose_War_Landmass.md](AIMOVE-AI_Choose_War_Landmass.md) "What `lmt_NoTargets` really means" for the downstream timing implications.

## Signature

```c
void AI_Build_Target_List(int16_t player_idx, int16_t landmass_idx, int16_t wp)
```

Called from [`AI_Set_Unit_Orders`](AIMOVE-AI_Set_Unit_Orders.md) Phase 4 dispatch (slot 8), immediately before [`AI_Stacks_Roamers_Target_Or_Deploy`](AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md) (slot 9, the only consumer).

## Phase 1 — Clear the target count (line 2564)

```c
_ai_targets_count = 0;
```

Reset the shared target list. Every per-landmass iteration starts fresh.

## Phase 2 — Fortresses (lines 2568-2605)

```c
for(itr_players = 0; itr_players < _num_players; itr_players++)
{
    if(_FORTRESSES[itr_players].wp != wp) continue;

    target_value_base = (Dipl_Status == DIPL_War) ? 10 : 1;

    if(itr_players == HUMAN_PLAYER_IDX)
        // strength lookup uses fortress_wy/wx BEFORE they're set this iteration
        target_strength = ...(eval & AI_TARGET_STRENGTH_MASK) * 5 / 4;
    else
        target_strength = ...(eval & AI_TARGET_STRENGTH_MASK);

    fortress_wx = _FORTRESSES[itr_players].wx;
    fortress_wy = _FORTRESSES[itr_players].wy;

    if((landmass matches) && (Hostility[itr_players] >= 3))
    {
        AI_Add_Target(fortress_wx, fortress_wy, target_strength, target_value_base * 500);
    }
}
```

**Intent:** add wizard fortresses on this landmass when our `Hostility` toward that wizard is Jihad-level (≥ 3). Value is 500 × (DIPL_War ? 10 : 1) = up to 5000. Human-player fortress gets a 25% strength multiplier (assumed tougher to assault).

**Source-flagged OGBUGs:**

| Line | OGBUG |
|---|---|
| 2570 | `¿ OGBUG  own fortress is not excluded - caught by _players[player_idx].Hostility[] check ?` — the loop iterates `0..num_players` including `player_idx`. Saved in practice by `Hostility[player_idx] >= 3` being effectively impossible. |
| 2575 | `OGBUG  should be >= for {DIPL_War, DIPL_Crusade}` — `Dipl_Status == DIPL_War` misses `DIPL_Crusade` (also a war state). Crusade fortresses get value_base=1 instead of 10. |
| 2587, 2592 | `OGBUG  unset fortress_wx, fortress_wy` — `target_strength` reads `g_ai_evaluation_map[wp][fortress_wy*WORLD_WIDTH + fortress_wx]` BEFORE `fortress_wx`/`fortress_wy` are assigned this iteration, so it reads the PREVIOUS wizard's square (or uninitialized 0,0 on the first iteration). |

## Phase 3 — Enemy Cities (lines 2612-2686)

```c
for(itr_cities = 0; itr_cities < _cities; itr_cities++)
{
    if(_CITIES[itr_cities].wp != wp) continue;

    target_owner_idx = _CITIES[itr_cities].owner_idx;
    city_wx = _CITIES[itr_cities].wx;
    city_wy = _CITIES[itr_cities].wy;

    target_value_base = (non-neutral && DIPL_War) ? 10 : 1;

    if(itr_cities == HUMAN_PLAYER_IDX)                     // OGBUG: c&p typo, see below
        target_strength = ...(city eval & AI_TARGET_STRENGTH_MASK) * 5 / 4;
    else
        target_strength = ...(city eval & AI_TARGET_STRENGTH_MASK);

    if((target_owner_idx == NEUTRAL) && (landmass matches))
    {
        neutral_city_value = (((pop + bldg_cnt) * 10) + 100) * target_value_base;
        AI_Add_Target(city_wx, city_wy, target_strength, neutral_city_value);
    }
    else
    {
        if(target_owner_idx == player_idx) continue;
        if(Hostility[target_owner_idx] < 3) continue;

        city_wx = _CITIES[itr_cities].wx;                  // OGBUG: duplicate of lines 2619-2620
        city_wy = _CITIES[itr_cities].wy;
        if(landmass != landmass_idx) continue;

        if(Hostility[target_owner_idx] >= 3)
            AI_Add_Target(city_wx, city_wy, target_strength, ((pop+bldg)*10 + 100) * target_value_base);
        /* DEAD */ else if(Hostility[target_owner_idx] >= 2) { ... }    // unreachable
    }
}
```

**Intent:** add neutral cities on this landmass (any) and Jihad-hostile (`Hostility >= 3`) wizard cities on this landmass. Value scales with `(population + building_count) × 10 + 100`, multiplied by `target_value_base`.

**Source-flagged OGBUGs:**

| Line | OGBUG |
|---|---|
| 2621 | `OGBUG  should be >= for {DIPL_War, DIPL_Crusade}` — same Crusade omission as Phase 2. |
| 2634 | `OGBUG  should be target_owner_idx, not itr; c&p error from above?` — the `itr_cities == HUMAN_PLAYER_IDX` check is a copy-paste from Phase 2; `itr_cities` is a city index, not a player index. The 5/4 strength multiplier fires for whichever city happens to share an index value with `HUMAN_PLAYER_IDX` (typically index 0). |
| 2663 | `OGBUG  duplicate city_wx,city_wy` — `city_wx`/`city_wy` already set at lines 2619-2620; the reassignment at 2664-2665 is dead code (no intervening writes). |
| 2674-2684 | `/* DEAD */` — the `Hostility >= 2` branch is unreachable because the earlier `Hostility < 3` guard at line 2659 catches the same range. Preserved as a faithful-to-dasm marker. |

## Phase 4 — Free-roaming enemy stacks (lines 2696-2728)

```c
landmass_node_index = _ai_landmass_land_squares_heads[wp][landmass_idx];
while(landmass_node_index != ST_UNDEFINED)
{
    landmass_node_wx = _ai_landmass_land_squares_wx_array[wp][landmass_node_index];
    landmass_node_wy = _ai_landmass_land_squares_wy_array[wp][landmass_node_index];
    target_square_evaluation_value = g_ai_evaluation_map[wp][...];

    if(target_square_evaluation_value != 0)
    {
        if(
            ((eval & AI_TARGET_NONHOSTILE) == 0)
            && ((eval & AI_TARGET_SITE) == 0)
            && ((eval & AI_TARGET_STRENGTH_MASK) != 0)      // strength bits non-zero = at least one unit present
        )
        {
            if(own fortress on this landmass)
                AI_Add_Target(..., strength, strength);         // defensive: full value
            else
                AI_Add_Target(..., strength, strength / 3);     // reduced value
        }
    }

    landmass_node_index = _ai_landmass_land_squares_lists[wp][landmass_node_index];
}
```

**Intent:** walk every land square on this landmass. Add as a target every square that:
- has non-zero evaluation (any units present), AND
- is not flagged `AI_TARGET_NONHOSTILE`, AND
- is not flagged `AI_TARGET_SITE` (sites are handled by their own phases), AND
- has at least one unit's worth of strength on it (`eval & AI_TARGET_STRENGTH_MASK != 0` — note this is the strength-value field; combined with the two flag tests above, this means hostile units present, not on a site).

Value is full strength if our fortress is on this landmass (defensive priority), otherwise one-third.

## Phase 5 — Nodes (lines 2738-2776)

```c
for(itr_nodes = 0; itr_nodes < NUM_NODES; itr_nodes++)
{
    if(_NODES[itr_nodes].wp != wp) continue;
    target_owner_idx = _NODES[itr_nodes].owner_idx;
    if(target_owner_idx == player_idx) continue;

    /* OGBUG  WTF? (target_owner_idx != ST_UNDEFINED) accessing arrays with -1 index? */
    if((Hostility[target_owner_idx] < 2) && (target_owner_idx != ST_UNDEFINED)) continue;

    node_wx = _NODES[itr_nodes].wx;
    node_wy = _NODES[itr_nodes].wy;
    if(landmass != landmass_idx) continue;

    if(target_owner_idx != ST_UNDEFINED)
        AI_Add_Target(node_wx, node_wy, strength, (power * 10) + 50);    // owned by hostile
    else if((eval & AI_TARGET_STRENGTH_MASK) != 0)
        AI_Add_Target(node_wx, node_wy, strength, (power * 10) + 25);    // unowned but defended
}
```

**Intent:** add hostile-owned (`Hostility >= 2`) or unowned-but-defended magic nodes on this landmass. Value = `power × 10 + 50` (owned) or `power × 10 + 25` (unowned, only if guardians remain per `eval & AI_TARGET_STRENGTH_MASK != 0`).

**Source-flagged OGBUG:**

| Line | OGBUG |
|---|---|
| 2749 | `OGBUG  WTF? (target_owner_idx != ST_UNDEFINED) accessing arrays with -1 index?` — `Hostility[target_owner_idx]` is dereferenced BEFORE the `!= ST_UNDEFINED` check, so for unowned nodes (`owner_idx == ST_UNDEFINED == -1`) the array access uses index -1. The short-circuit AND means the `< 2` check has to evaluate first, so the negative index is real. Whatever value lives at `Hostility[-1]` (memory before the array, typically the prior struct field) determines whether unowned nodes are skipped. |

## Phase 6 — Lairs (lines 2786-2803)

```c
for(itr_lairs = 0; itr_lairs < NUM_LAIRS; itr_lairs++)
{
    if(_LAIRS[itr_lairs].wp != wp) continue;
    if(_LAIRS[itr_lairs].intact != ST_TRUE) continue;
    lair_wx = _LAIRS[itr_lairs].wx;
    lair_wy = _LAIRS[itr_lairs].wy;
    if(landmass != landmass_idx) continue;
    AI_Add_Target(lair_wx, lair_wy, strength, 50);
}
```

**Intent:** add intact lairs (encounter sites) on this landmass. Fixed value of 50.

## Phase 7 — Towers (lines 2813-2867)

```c
if(_FORTRESSES[player_idx].wp == wp)        // only when our fortress is on this plane
{
    for(itr_towers = 0; itr_towers < NUM_TOWERS; itr_towers++)
    {
        target_owner_idx = _TOWERS[itr_towers].owner_idx;
        tower_wx = _TOWERS[itr_towers].wx;
        tower_wy = _TOWERS[itr_towers].wy;
        if(landmass != landmass_idx) continue;
        if(AI_Tower_Target_Worthwhile(player_idx, tower_wx, tower_wy, wp) == ST_FALSE) continue;

        if((target_owner_idx == player_idx) && (landmass matches))
        {
            AI_Add_Target(tower_wx, tower_wy, 0, 1);                          // own tower, trivial value
        }
        else
        {
            /* OGBUG  WTF? (target_owner_idx != ST_UNDEFINED) accessing arrays with -1 index? */
            if((Hostility[target_owner_idx] < 2) && (target_owner_idx != ST_UNDEFINED)) continue;

            if((target_owner_idx > ST_UNDEFINED) && (Hostility[target_owner_idx] >= 3))
                AI_Add_Target(tower_wx, tower_wy, strength, 10);              // hostile-occupied
            else if(target_owner_idx == ST_UNDEFINED)
                AI_Add_Target(tower_wx, tower_wy, strength, 150);             // unowned (Chaos Node-style guards)
        }
    }
}
```

**Intent:** when our fortress is on this plane, add Towers of Wizardry on this landmass that lead somewhere worthwhile (per `AI_Tower_Target_Worthwhile`). Value tiers: 150 (unowned, high incentive), 10 (hostile-occupied), 1 (own — placeholder presence).

**Source-flagged OGBUG:**

| Line | OGBUG |
|---|---|
| 2838 | `OGBUG  WTF? (target_owner_idx != ST_UNDEFINED) accessing arrays with -1 index?` — same negative-index hazard as Phase 5 line 2749. |

## Phase 7 (lmt_NoTargets gate) — line 2873-2878

The source labels this `/* Phase 7 */`, duplicating the Towers phase label.

```c
if(_ai_targets_count == 0)
{
    _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_NoTargets;
}
```

The only writer of `lmt_NoTargets` in the codebase. The tag survives across turns via `AI_Evaluate_Continents`'s preservation clause at [AIMOVE.c:6848](../../MoM/src/AIMOVE.c#L6848) and is read by `AI_Choose_War_Landmass` next turn before any refresh — see [AIMOVE-AI_Choose_War_Landmass.md](AIMOVE-AI_Choose_War_Landmass.md) for the full timing analysis.

## Call graph

```
AI_Set_Unit_Orders                                  [AIMOVE.c:131, Phase 4 dispatch loop]
  └─ per (plane, landmass):
       ├─ ... slots 1-7 ...
       ├─ AI_Build_Target_List(player_idx, landmass_idx, wp)              [slot 8 — THIS FUNCTION]
       │    ├─ Phase 1: clear _ai_targets_count
       │    ├─ Phase 2: per-player fortress scan → AI_Add_Target
       │    ├─ Phase 3: per-city scan → AI_Add_Target
       │    ├─ Phase 4: per-land-square chain walk → AI_Add_Target
       │    ├─ Phase 5: per-node scan → AI_Add_Target
       │    ├─ Phase 6: per-lair scan → AI_Add_Target
       │    ├─ Phase 7: per-tower scan → AI_Tower_Target_Worthwhile, AI_Add_Target
       │    └─ Phase 7 (lmt_NoTargets gate): tag landmass if count == 0
       │         ↓ writes _ai_targets_count/wx/wy/strength/value[25]
       │         ↓ writes type_array[landmass_idx] = lmt_NoTargets (conditional)
       │
       └─ AI_Stacks_Roamers_Target_Or_Deploy(landmass_idx, wp, player_idx)             [slot 9 — SOLE CONSUMER]
            └─ per roamer stack (_ai_own_stack_type == 1):
                 └─ AI_Stacks_Assign_Target(...)                          [AIMOVE.c:3186]
                      ↑ reads _ai_targets_count/wx/wy/strength/value
                      ↓ "consumes" by zeroing _ai_targets_value[picked]
```

No other function in `MoM/src/` or `MoX/src/` reads `_ai_targets_*` arrays. The producer/consumer pair is fully scoped to one per-landmass iteration of `AI_Set_Unit_Orders` Phase 4.

## Impact on AI_Stacks_Roamers_Target_Or_Deploy

`AI_Stacks_Roamers_Target_Or_Deploy` ([AIMOVE.c:1755](../../MoM/src/AIMOVE.c#L1755)) iterates roaming stacks (`_ai_own_stack_type[itr_stacks] == 1`) and for each one calls `AI_Stacks_Assign_Target`, which:

1. Iterates `_ai_targets_count` entries.
2. Filters by `value > 0 && strength * 3/4 < effective_stack_strength`.
3. Rates each candidate by `value / (distance * 4)`.
4. Verifies path reachability via `Make_Move_Path`.
5. Picks the highest-rated reachable target.
6. Zeros the picked entry's `_ai_targets_value` so no other roamer in this landmass-iteration can claim the same target.

### What the value tiers mean for roamer behavior

The relative `value` numbers `AI_Build_Target_List` assigns determine which targets roamers prioritize. Sorted high-to-low for a typical war scenario (DIPL_War so `target_value_base = 10`):

| Target type | Value | Where set |
|---|---|---|
| War-state hostile fortress | 5000 | Phase 2 line 2603 |
| Neutral city (size + bldg) × 10 | varies, ~100-2500 | Phase 3 line 2651 |
| War-state hostile city (size + bldg) × 100 | varies, ~1000-25000 | Phase 3 line 2672 |
| Unowned tower | 150 | Phase 7 line 2861 |
| Owned node (high power) | 50 + power×10 | Phase 5 line 2767 |
| Lair | 50 | Phase 6 line 2802 |
| Unowned-but-defended node | 25 + power×10 | Phase 5 line 2773 |
| Hostile-occupied tower | 10 | Phase 7 line 2855 |
| Own tower (placeholder) | 1 | Phase 7 line 2834 |
| Enemy stack (defensive) | full strength | Phase 4 line 2719 |
| Enemy stack (offensive) | strength / 3 | Phase 4 line 2723 |

War-hostile cities dominate the value board, so roamers in a war target enemy cities first. Lairs and nodes are deliberately mid-tier — the AI clears them when it has the strength but won't divert from a city assault. Towers are rare (NUM_TOWERS = 6) and only enter the list when our fortress is on this plane.

### Cascade into `lmt_NoTargets` (next-turn coupling)

When all phases together produce zero entries, Phase 7 (lmt_NoTargets gate) tags the landmass. That tag is read by `AI_Choose_War_Landmass` on the FOLLOWING turn (the per-turn driver in `AIDUDES.c:241-285` runs `AI_Choose_War_Landmass` BEFORE the next `AI_Set_Unit_Orders` dispatch) to exclude the landmass from war-target selection. So this function's per-landmass output controls both:

1. **This turn's roamer targets** on this landmass (via `AI_Stacks_Roamers_Target_Or_Deploy` immediately after).
2. **Next turn's war-landmass selection** (via the `lmt_NoTargets` tag surviving through `AI_Evaluate_Continents`'s preservation clause).

## Related references

- [AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md](AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md) — sole consumer
- [AIMOVE-AI_Set_Unit_Orders.md](AIMOVE-AI_Set_Unit_Orders.md) — dispatcher (slot 8 = this function, slot 9 = consumer)
- [AIMOVE-AI_Choose_War_Landmass.md](AIMOVE-AI_Choose_War_Landmass.md) — reads `lmt_NoTargets` written by Phase 7 above
- [MoM-AI-Landmass-Types.md](MoM-AI-Landmass-Types.md) — `lmt_NoTargets` state machine
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
