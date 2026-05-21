AIMOVE-AI_ProcessRoamers__WIP.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_ProcessRoamers__WIP.c

---

# AI_ProcessRoamers__WIP — Walkthrough

**Location:** [MoM/src/AIMOVE.c:1755](../../MoM/src/AIMOVE.c#L1755) (~128 lines, ends 1883). GEMINI variant at [line 1885](../../MoM/src/AIMOVE.c#L1885).
**WZD overlay:** ovr158, p06
**drake178 name:** `AI_ProcessRoamers()`

## Purpose

**Per-(player, plane, landmass) roamer-stack processor.** Two responsibilities, gated by what it finds:

1. **Primary path:** for every roaming stack (`_ai_own_stack_type == 1`) on this landmass, try to assign it a target from the per-landmass list that `AI_Build_Target_List` just built. If a target is found, every unit in that stack gets a move order to those coords via `AI_Set_Move_Or_Goto_Target`.

2. **Abandon path:** if no roamer stack found a target AND there's a large stack (≥8 units) on this landmass AND we have a main-war landmass set elsewhere, retag this landmass `lmt_Abandon` with its stage point set to the dock square closest to the war landmass's dock-square centroid — preparing the large stack to embark and ferry off.

Per the drake178 header comment ([lines 1741-1750](../../MoM/src/AIMOVE.c#L1741-L1750)):

> searches for and assigns targets and corresponding move orders to all roamer stack on the continent or, if there are no targets but at least one roaming stack with at least 7 units, changes the type and stage point of the continent to prepare for troops to move to the action continent instead
>
> WARNING: when setting the leave continent type, the stage point chosen ignores own units, and can't be a tile with a city, tower, or node either

(Note: the OG comment says "at least 7 units" but the production check is `list_unit_count >= 8` at [line 1795](../../MoM/src/AIMOVE.c#L1795). Off-by-one between comment and code — needs disassembly verification per the BUG-B lesson.)

## Signature

```c
void AI_ProcessRoamers__WIP(int16_t landmass_idx, int16_t wp, int16_t player_idx)
```

Called from [`AI_Set_Unit_Orders`](AIMOVE-AI_Set_Unit_Orders.md) Phase 4 dispatch (slot 9), immediately after [`AI_Build_Target_List`](AIMOVE-AI_Build_Target_List.md) (slot 8 — the producer of the target list this function consumes).

## Signature and locals

```c
void AI_ProcessRoamers__WIP(int16_t landmass_idx, int16_t wp, int16_t player_idx)
{
    int16_t Have_Large_Stack = 0;            // set ST_TRUE if any roamer stack has >= 8 units
    int16_t landmass_node_count = 0;         // dock-square count for centroid divisor (Phase 2 Step A)
    int16_t min_delta_distance = 0;          // best dock-square distance to war centroid (Phase 2 Step B); see suspected bug below
    int16_t Best_LoadTile_Y = 0;             // chosen abandon stage Y
    int16_t No_Targets = 0;                  // start ST_TRUE; cleared if any stack finds a target
    int16_t landmass_node_wy = 0;            // current chain node Y
    int16_t landmass_node_wx = 0;            // current chain node X
    int16_t Best_LoadTile_X = 0;             // chosen abandon stage X
    int16_t landmass_node_index = 0;         // chain cursor
    int16_t landmass_node_centroid_wy = 0;   // war landmass dock centroid Y (Phase 2 Step A)
    int16_t landmass_node_centroid_wx = 0;   // war landmass dock centroid X (Phase 2 Step A)
    int16_t Have_Processed_Unit = 0;         // see suspected bug: always 0 here, gate at line 1802 always passes
    int16_t unit_idx = 0;
    int16_t list_unit_count = 0;             // count of units in current stack
    int16_t UU_Have_Landlubbers = 0;         // unused (UU = Unknown/Unused per project convention)
    int16_t itr_list_units = 0;              // see suspected bug: passed to AI_AssignStackTarget__WIP before assigned
    int16_t target_wy = 0;                   // out-param from AI_AssignStackTarget__WIP
    int16_t target_wx = 0;                   // out-param from AI_AssignStackTarget__WIP
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t itr_stacks = 0;
    int16_t delta_distance = 0;              // DNE in Dasm, reuses landmass_node_count
    ...
}
```

## Phase 1 — Iterate roamer stacks, assign targets (lines 1788-1822)

```c
for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
{
    if(_ai_own_stack_type[itr_stacks] == 1)               // only roamer stacks
    {
        list_unit_count = _ai_own_stack_unit_count[itr_stacks];
        stack_wx = _ai_own_stack_wx[itr_stacks];
        stack_wy = _ai_own_stack_wy[itr_stacks];

        if(list_unit_count >= 8)
        {
            Have_Large_Stack = ST_TRUE;
        }

        // ; check for processed units and land only units
        // ; WARNING: the latter is not evaluated properly by the called functions (but is not used anyway)
        if(Have_Processed_Unit == ST_FALSE)               // always-true gate (see Suspected Bug 1 below)
        {
            if(AI_AssignStackTarget__WIP(stack_wx, stack_wy, &target_wx, &target_wy, itr_stacks, itr_list_units) == ST_TRUE)
            {
                // ; clear the no targets indicator, and assign every unit in the stack a move order
                No_Targets = ST_FALSE;
                for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
                {
                    unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
                    g_ai_set_target_caller = 7;           // debug breadcrumb identifying which caller of AI_Set_Move_Or_Goto_Target
                    AI_Set_Move_Or_Goto_Target(unit_idx, target_wx, target_wy, itr_stacks, itr_list_units);
                }
            }
        }
        UU_AI_TargetingVar = 0;
    }
}
```

**Intent:** for each roaming stack (type 1) on this landmass:
- Cache its size + position.
- If it's a large stack (≥8 units), remember that — used as a precondition for the abandon path in Phase 2.
- Try to assign a target via `AI_AssignStackTarget__WIP`. That function reads `_ai_targets_*` (built by [`AI_Build_Target_List`](AIMOVE-AI_Build_Target_List.md) in dispatcher slot 8 immediately before this function), picks the highest-rated reachable target, and zeros that target's value so no other stack claims it.
- If a target was found, issue every unit in the stack a move order to those coords via `AI_Set_Move_Or_Goto_Target`. The shared module-level `g_ai_set_target_caller = 7` is a debug breadcrumb identifying this call site.

`UU_AI_TargetingVar = 0` is a per-stack reset of an unrelated module-level state variable consumed elsewhere — the "UU" prefix in this codebase marks identifiers whose semantics are still uncertain.

## Phase 2 — Find abandon stage tile (lines 1824-1874)

Conditional block — runs only when all four gating conditions hold:

```c
if(
    (Have_Large_Stack == ST_TRUE)
    && (No_Targets == ST_TRUE)
    && (_ai_landmass_war_targets[wp][player_idx] != 0)              // we have a main war landmass
    && (_ai_landmass_war_targets[wp][player_idx] != landmass_idx)   // and we're NOT on it
)
{
    // ; find the nearest empty transport loading tile to the center of the action continent, if any
    // ; WARNING: ignores own units on the tile

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
    landmass_node_centroid_wx /= landmass_node_count;          // suspected bug: divide-by-zero if count == 0
    landmass_node_centroid_wy /= landmass_node_count;
    min_delta_distance = 1000;

    // Step B: walk this landmass's dock squares, find empty one closest to that centroid
    landmass_node_index = _ai_landmass_dock_squares_heads[wp][landmass_idx];
    while(landmass_node_index != ST_UNDEFINED)
    {
        landmass_node_wx = _ai_landmass_dock_squares_wx_array[wp][landmass_node_index];
        landmass_node_wy = _ai_landmass_dock_squares_wy_array[wp][landmass_node_index];
        if(g_ai_evaluation_map[wp][((landmass_node_wy * WORLD_WIDTH) + landmass_node_wx)] == 0)
        {
            delta_distance = Delta_XY_With_Wrap(
                landmass_node_centroid_wx, landmass_node_centroid_wy,
                landmass_node_wx, landmass_node_wy,
                WORLD_WIDTH
            );
            if(delta_distance < min_delta_distance)
            {
                min_delta_distance = delta_distance;
                Best_LoadTile_X = landmass_node_wx;
                Best_LoadTile_Y = landmass_node_wy;
            }
        }
        landmass_node_index = _ai_landmass_dock_squares_lists[wp][landmass_node_index];
    }
}
```

**Intent:** when we have a sizable stack stranded on a landmass with nothing to attack AND there's a main war elsewhere, find the dock square on this landmass that's closest to where the war is happening — so the large stack can embark and ferry over. Step A computes the centroid of the war landmass's dock squares (the embark destination). Step B walks this landmass's own dock squares and picks the empty one closest to that centroid.

"Empty" here means `g_ai_evaluation_map[wp][square] == 0` — no enemy units, no sites. The WARNING in the source comment notes this ignores OWN units on the square, so the chosen embark point may already have one of our stacks on it.

## Phase 3 — Apply lmt_Abandon tag (lines 1876-1881)

```c
if(min_delta_distance != 1000)
{
    _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_Abandon;
    _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = (uint8_t)Best_LoadTile_X;
    _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = (uint8_t)Best_LoadTile_Y;
}
```

**Intent:** if Phase 2 ran AND found an acceptable dock square (`min_delta_distance` was lowered below 1000), tag the landmass `lmt_Abandon` and set its stage point to that dock square. The next per-turn dispatch will see `lmt_Abandon` and route troops toward the embark tile.

`lmt_Abandon` (= 5) is one of two "phase overlay" landmass types (the other is `lmt_NoTargets`); see [MoM-AI-Landmass-Types.md](MoM-AI-Landmass-Types.md). It signals "we're leaving this landmass" — `AI_Choose_War_Landmass` sets `Reevaluate = ST_TRUE` for landmasses in this state so the abandon decision is reconsidered next turn.

## Suspected bugs (need disassembly verification per BUG-B lesson)

### Suspected Bug 1 — `Have_Processed_Unit` always-zero local (line 1802)

`Have_Processed_Unit` is initialized to 0 ([line 1768](../../MoM/src/AIMOVE.c#L1768)) and never written anywhere in the function. The gate `if(Have_Processed_Unit == ST_FALSE)` at line 1802 always passes. Per the comment at [lines 1799-1801](../../MoM/src/AIMOVE.c#L1799-L1801):

> // ; check for processed units and land only units
> // ; WARNING: the latter is not evaluated properly by the called functions (but is not used anyway)

The OG seems to have intended this as a guard against re-processing a stack whose units already got orders earlier this turn. Either the C reconstruction missed an assignment, the OG itself never wrote the flag, or the variable was always meant as a placeholder. Cannot tell from production alone.

### Suspected Bug 2 — `itr_list_units` passed before assignment (line 1804)

`itr_list_units` is initialized to 0 ([line 1772](../../MoM/src/AIMOVE.c#L1772)) and passed to `AI_AssignStackTarget__WIP(..., itr_list_units)` at line 1804 BEFORE the inner `for(itr_list_units = 0; ...)` at line 1809 assigns it. So the first stack's call receives 0; subsequent stacks' calls receive whatever value the inner for-loop left it at on the previous iteration (typically `list_unit_count` from the prior stack).

This argument feeds the `list_unit_idx` parameter of `AI_AssignStackTarget__WIP` — that function uses it for path-finding fudge-factor parameters (`boatrider_count`, `troop_count` per the `Make_Move_Path` callsite). Garbage values may distort which targets the picker considers reachable.

### Suspected Bug 3 — `min_delta_distance != 1000` gate at line 1876 (false-firing Phase 3)

`min_delta_distance` is initialized to 0 ([line 1759](../../MoM/src/AIMOVE.c#L1759)) and only set to 1000 INSIDE Phase 2's outer `if` block ([line 1850](../../MoM/src/AIMOVE.c#L1850)). When Phase 2's four-condition gate is FALSE (no large stack, OR targets were found, OR no main war set, OR we're already on the war landmass), `min_delta_distance` stays at its initializer value 0.

Then Phase 3's check `if(min_delta_distance != 1000)` evaluates `0 != 1000` → TRUE → Phase 3 fires and writes `lmt_Abandon` with stage point `(Best_LoadTile_X, Best_LoadTile_Y)` = `(0, 0)` (their initializer values).

In the OG, local variables aren't necessarily zero-initialized — they hold whatever was on the stack. The C reconstruction's `= 0` initializers may not match OG behavior. If the OG has garbage there, the same gate would still misfire on most stack states, just unpredictably.

A semantically-correct version would either:
- Move the `if(min_delta_distance != 1000) { ... write Abandon ... }` block INSIDE the Phase 2 outer `if`, OR
- Initialize `min_delta_distance` to 1000 at declaration, OR
- Use a separate boolean flag set by Phase 2 to gate Phase 3.

**Needs disassembly verification.** This may be preserved-OG (Phase 3 always fires by design — but then why have the `!= 1000` check at all?), or it may be a translation defect from the C reconstruction adding `= 0` initializers.

### Suspected Bug 4 — divide-by-zero on war landmass with no dock squares (lines 1848-1849)

```c
landmass_node_centroid_wx /= landmass_node_count;
landmass_node_centroid_wy /= landmass_node_count;
```

If `_ai_landmass_dock_squares_heads[wp][_ai_landmass_war_targets[wp][player_idx]]` chain is empty (war landmass has no dock squares), `landmass_node_count` remains 0 → divide-by-zero crash.

Same OGBUG pattern as the sibling [`AI_Reevaluate_All_Continents`](AIMOVE-AI_Reevaluate_All_Continents.md) Phase 9 and [`AI_Reevaluate_Continent`](AIMOVE-AI_Reevaluate_Continent.md) Phase 6 — both have inline `/* OGBUG  possible division by 0 */` annotations. This function does NOT have the inline OGBUG marker; worth adding one if disassembly confirms the same OG behavior.

### Off-by-one between OG comment and code (line 1795)

The drake178 header says "at least 7 units"; the code checks `list_unit_count >= 8`. Either the comment is loose ("more than 7" = "≥ 8") or the OG genuinely had `>= 7`. Comment-vs-code mismatch only; not a behavioral concern by itself.

## Call graph

```
AI_Set_Unit_Orders                                  [AIMOVE.c:131, Phase 4 dispatch]
  └─ per (plane, landmass):
       ├─ ... slots 1-7 ...
       ├─ AI_Build_Target_List(player_idx, landmass_idx, wp)              [slot 8 — PRODUCER]
       │    └─ writes _ai_targets_count, _ai_targets_wx/wy/strength/value
       │
       └─ AI_ProcessRoamers__WIP(landmass_idx, wp, player_idx)             [slot 9 — THIS FUNCTION]
            │
            ├─ Phase 1: per roamer stack (_ai_own_stack_type == 1):
            │   └─ AI_AssignStackTarget__WIP(stack_wx, stack_wy, &target_wx, &target_wy, ...)  [AIMOVE.c:3186]
            │        ↑ READS _ai_targets_*
            │        ↓ zeros _ai_targets_value[picked]
            │        ↓ returns picked target via out-params
            │
            │   if found:
            │   └─ per unit in stack:
            │        └─ AI_Set_Move_Or_Goto_Target(unit_idx, target_wx, target_wy, ...)
            │             ↓ sets g_ai_set_target_caller = 7 first (debug breadcrumb)
            │
            ├─ Phase 2 (CONDITIONAL: Large stack + no targets + war target set + not on war landmass):
            │   └─ walk _ai_landmass_dock_squares_* for war landmass → centroid
            │   └─ walk _ai_landmass_dock_squares_* for this landmass → closest empty
            │
            └─ Phase 3 (CONDITIONAL: min_delta_distance != 1000):
                 └─ write _ai_continents.plane[wp].player[player_idx].{type, wx, wy}_array[landmass_idx] = {lmt_Abandon, Best_LoadTile_X, Best_LoadTile_Y}
```

## Inputs / outputs summary

**Reads:**
- `_ai_own_stack_*` (count, type, unit_count, wx, wy, unit_list) — roamer-stack inventory built earlier in the dispatch
- `_ai_targets_*` (via `AI_AssignStackTarget__WIP`) — per-landmass target list from `AI_Build_Target_List`
- `_ai_landmass_war_targets[wp][player_idx]` — main war landmass set by `AI_Choose_War_Landmass` (last turn)
- `_ai_landmass_dock_squares_*` — per-landmass dock-square chains
- `g_ai_evaluation_map[wp][...]` — per-square evaluation map

**Writes:**
- Unit-level: `AI_Set_Move_Or_Goto_Target` orders for every unit in stacks that found targets
- Module-level: `g_ai_set_target_caller = 7`, `UU_AI_TargetingVar = 0`, `_ai_targets_value[picked] = 0` (via callee)
- Landmass classification: `_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_Abandon` (conditional, see Suspected Bug 3)
- Landmass stage: `wx_array[landmass_idx]` / `wy_array[landmass_idx]` (same conditional)

## Downstream impact

The `lmt_Abandon` tag written in Phase 3 is read by:

- [`AI_Set_Unit_Orders`](AIMOVE-AI_Set_Unit_Orders.md) per-landmass gates (Phase 4):
  - `AI_PullForMainWar__WIP` runs when `>= lmt_Abandon` (includes Abandon + NoTargets)
  - `G_AI_HomeRallyFill__WIP` runs when `>= lmt_Abandon` OR `Own`
  - `AI_FillGarrisons__WIP` runs when `>= lmt_Abandon` OR `Own` OR `Contested`
- [`AI_Choose_War_Landmass`](AIMOVE-AI_Choose_War_Landmass.md): the Phase 2 switch case for `lmt_Abandon` sets `Reevaluate = ST_TRUE` — abandon decisions get re-evaluated next turn.

So Phase 3's write affects: this turn's later dispatch slots (PullForMainWar, HomeRallyFill, FillGarrisons all see the Abandon tag and act accordingly), AND next turn's main-war reselection.

If Suspected Bug 3 is real, Phase 3 spuriously fires for landmasses that never went through Phase 2 — writing `lmt_Abandon` with stage `(0, 0)`. Downstream callees would then treat those landmasses as embark-staging targets and try to route stacks to coordinate (0, 0), which is typically ocean.

## Related references

- [AIMOVE-AI_Build_Target_List.md](AIMOVE-AI_Build_Target_List.md) — producer of `_ai_targets_*` consumed by Phase 1 here
- [AIMOVE-AI_Set_Unit_Orders.md](AIMOVE-AI_Set_Unit_Orders.md) — dispatcher; this function is slot 9
- [AIMOVE-AI_Choose_War_Landmass.md](AIMOVE-AI_Choose_War_Landmass.md) — producer of `_ai_landmass_war_targets[]` consumed by Phase 2
- [AIMOVE-AI_Reevaluate_Continent.md](AIMOVE-AI_Reevaluate_Continent.md) — sibling with the same div-by-zero pattern in its Phase 6
- [AIMOVE-AI_Reevaluate_All_Continents.md](AIMOVE-AI_Reevaluate_All_Continents.md) — sibling with the same div-by-zero pattern in its Phase 9
- [MoM-AI-Landmass-Types.md](MoM-AI-Landmass-Types.md) — `lmt_Abandon` state machine entry; this function is the sole writer of that tag
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
