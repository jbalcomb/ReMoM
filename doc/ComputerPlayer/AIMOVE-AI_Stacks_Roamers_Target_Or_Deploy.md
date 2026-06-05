AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Stacks_Roamers_Target_Or_Deploy.c

---

# AI_Stacks_Roamers_Target_Or_Deploy — Walkthrough

**Location:** [MoM/src/AIMOVE.c:1763](../../MoM/src/AIMOVE.c#L1763) (~157 lines, ends 1920). GEMINI variant has been removed.
**WZD overlay:** ovr158, p06
**drake178 name:** `AI_ProcessRoamers()`

## Purpose

**Per-(player, plane, landmass) roamer-stack processor.** Two responsibilities:

1. **Target assignment:** for every roaming stack (`_ai_own_stack_type == AISTK_Roamer`) on this landmass, walk its unit list to confirm it's well-formed, then try to assign it a target from the per-landmass list that `AI_Build_Target_List` just built. If a target is found, every unit in that stack gets a move order to those coords via `AI_Stacks_Order_Attack_Target_Or_Goto_Destination`.

2. **Deploy (embark-staging):** if no roamer stack found a target AND there's a large stack (≥8 units) on this landmass AND we have a main-war landmass set elsewhere (and we're not on it), retag this landmass `lmt_Leaveable` with its stage point set to this landmass's dock square closest to the war landmass's dock-square centroid — preparing the large stack to embark and ferry off.

## Signature

```c
void AI_Stacks_Roamers_Target_Or_Deploy(int16_t landmass_idx, int16_t wp, int16_t player_idx)
```

Called from [`AI_Set_Unit_Orders`](AIMOVE-AI_Set_Unit_Orders.md) Phase 4 dispatch (slot 9), immediately after [`AI_Build_Target_List`](AIMOVE-AI_Build_Target_List.md) (slot 8 — the producer of the target list this function consumes).

## Locals

```c
int16_t have_large_stack = 0;                // set ST_TRUE if any roamer stack has >= 8 units
int16_t landmass_node_count = 0;             // dock-square count for centroid divisor (Step A)
int16_t min_delta_distance = 0;              // best dock-square distance to war centroid (Step B); init 1000 inside the staging block
int16_t target_square_wy = 0;                // chosen embark-stage Y
int16_t no_target_found_for_any_stack = 0;   // start ST_TRUE; cleared if any roamer stack got a target
int16_t landmass_node_wy = 0;
int16_t landmass_node_wx = 0;
int16_t target_square_wx = 0;                // chosen embark-stage X
int16_t landmass_node_index = 0;             // chain cursor
int16_t landmass_node_centroid_wy = 0;       // war landmass dock centroid Y (Step A)
int16_t landmass_node_centroid_wx = 0;       // war landmass dock centroid X (Step A)
int16_t hit_end_of_stack = 0;                // misnamed: set ST_TRUE when inner unit-list walk encounters a CONSUMED slot (ST_UNDEFINED) — see note below on sparse unit lists
int16_t unit_idx = 0;
int16_t list_unit_count = 0;                 // count of units in current stack
int16_t niu_have_boat_riders = 0;            // set but never read here (NIU = Not In Use)
int16_t itr_list_units = 0;
int16_t target_wy = 0;                       // out-param from AI_Stacks_Assign_Target
int16_t target_wx = 0;                       // out-param from AI_Stacks_Assign_Target
int16_t stack_wy = 0;
int16_t stack_wx = 0;
int16_t itr_stacks = 0;
int16_t delta_distance = 0;                  // DNE in Dasm, reuses landmass_node_count
```

## Phase 1 — Initialize flags (lines 1788-1791)

```c
have_large_stack = ST_FALSE;
no_target_found_for_any_stack = ST_TRUE;
```

Both flags get updated during Phase 2 and are read by the gating chain that follows.

## Phase 2 — Iterate roamer stacks, assign targets (lines 1794-1848)

```c
for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
{
    if(_ai_own_stack_type[itr_stacks] != AISTK_Roamer)        // skip non-roamer stacks
    {
        continue;
    }
    list_unit_count = _ai_own_stack_unit_count[itr_stacks];
    stack_wx = _ai_own_stack_wx[itr_stacks];
    stack_wy = _ai_own_stack_wy[itr_stacks];

    if(list_unit_count >= 8)        /* ¿ (MAX_STACK - 1) ?*/
    {
        have_large_stack = ST_TRUE;
    }

    // Inner pass: walk unit list, check for end-of-stack sentinel AND track water/air travel coverage
    niu_have_boat_riders = ST_FALSE;
    hit_end_of_stack = ST_FALSE;
    for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
    {
        unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
        if(unit_idx == ST_UNDEFINED)
        {
            hit_end_of_stack = ST_TRUE;
            break;
        }

        if(
            (Unit_Has_AirTravel(unit_idx) == ST_FALSE)
            && (Unit_Has_WaterTravel(unit_idx) == ST_FALSE)
        )
        {
            niu_have_boat_riders = ST_TRUE;        // gathered but never read in this function
        }
    }

    if(hit_end_of_stack == ST_FALSE)
    {
        if(AI_Stacks_Assign_Target(stack_wx, stack_wy, &target_wx, &target_wy, itr_stacks, itr_list_units) == ST_TRUE)
        {
            no_target_found_for_any_stack = ST_FALSE;
            for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
            {
                unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
                g_ai_set_target_caller = 7;
                AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, target_wx, target_wy, itr_stacks, itr_list_units);
            }
        }
    }
    niu_unknown_var = 0;
}
```

**Intent:** for each roaming stack (type `AISTK_Roamer`) on this landmass:
- Cache size + position.
- Note if it's a large stack (≥8 units) — used as a precondition for the embark-staging path.
- Walk the unit-list once:
  - Break out the moment a `ST_UNDEFINED` slot is found (means "this unit's slot was already consumed this turn by an earlier dispatch-slot order-issuer — see note on sparse unit lists below") and remember that with `hit_end_of_stack`.
  - Also count whether any unit in the stack lacks both air and water travel (`niu_have_boat_riders`) — currently gathered but unused.
- **Skip the target call entirely if any unit in this stack was already consumed.** The semantic is "don't split a stack — only assign a unified roamer target if every unit is still available." (`hit_end_of_stack` is misnamed; it really means "stack has at least one consumed slot.")
- Otherwise try `AI_Stacks_Assign_Target`. That function reads `_ai_targets_*` built by [`AI_Build_Target_List`](AIMOVE-AI_Build_Target_List.md), picks the highest-rated reachable target, and consumes the picked target by zeroing its value so no other stack claims it.
- If a target was found, issue every unit in the stack a move order via `AI_Stacks_Order_Attack_Target_Or_Goto_Destination`. The `g_ai_set_target_caller = 7` is a debug breadcrumb identifying this call site.

`niu_unknown_var = 0` is a per-stack reset of an unrelated module-level variable consumed elsewhere — the "NIU" prefix marks identifiers whose semantics are still unknown.

### Note on sparse `_ai_own_stack_unit_list[][]`

`_ai_own_stack_unit_list[stack][slot]` is **NOT a contiguous list terminated by a single ST_UNDEFINED.** Per the comment block at [MoX/src/MOM_DAT.c:2697](../../MoX/src/MOM_DAT.c#L2697), the pattern is the same as the `_ai_targets_value[picked] = 0` "consume" idiom used by `AI_Stacks_Assign_Target` — every per-unit order-issuer in the AI dispatch consumes the unit's slot by writing `ST_UNDEFINED`, marking it processed so subsequent passes skip it. The drake178 header on `AI_Order_Settle` says it plainly: `"replaces its index in the stack with -1, marking it processed"`.

The seven consumers (per the MOM_DAT.c comment):
- `AI_Stacks_Init_Build_Target_Order` ...ial empty state)
- `AI_Stacks_Order_Attack_Target_Or_Goto_Destination` at [AIMOVE.c:5076](../../MoM/src/AIMOVE.c#L5076)
- `AI_Order_Settle` at [AIMOVE.c:5120](../../MoM/src/AIMOVE.c#L5120)
- `AI_Order_RoadBuild`
- `AI_Stacks_Order_Ferry`
- `AI_Stacks_Order_Meld`
- `AI_Stacks_Order_Purify`

By the time this function runs (dispatch slot 9), the stack lists have been sparsified by slots 4-7 (`AI_Stacks_Do_Meld`, `AI_Stacks_Do_Settle`, `AI_Stacks_Do_Purify`, `AI_Do_RoadBuild`) consuming slots as they issue orders to settlers/road-builders/melders. The consumed slots are scattered with valid slots; later slots in the same stack can still hold real unit indices.

That's why the gate works as a whole-stack check: **if ANY slot is `ST_UNDEFINED`, the stack has been partially consumed and should not be re-tasked as a roamer destination.** The `break` is a short-circuit ("I only need to know IF any unit was consumed, not how many").

### Note on `niu_have_boat_riders`

The inner unit-list walk gathers `niu_have_boat_riders` (true if any unit lacks both air and water travel — i.e., "would need a transport to cross water") but doesn't use it. Preserved faithful-to-dasm; the value would presumably gate transport-related logic if the gather were wired up downstream.

Because the loop `break`s on the first `ST_UNDEFINED`, `niu_have_boat_riders` only reflects units BEFORE the first pre-tasked slot. If the gather ever gets wired up, this asymmetry would matter.

## Embark-staging path (lines 1850-1918)

The source frames the rest with a heading-style comment:

```c
/*
¿ prepare large stack to embark and ferry off ?
*/
```

### Gating chain — four early returns (lines 1853-1868)

```c
if(have_large_stack != ST_TRUE)                                   return;     // no large stack on this landmass
if(no_target_found_for_any_stack != ST_TRUE)                      return;     // at least one stack found a target — not stranded
if(_ai_landmass_war_targets[wp][player_idx] == 0)                 return;     // no main war target set
if(_ai_landmass_war_targets[wp][player_idx] == landmass_idx)      return;     // we ARE on the war landmass; embark target would be self
```

All four conditions must hold for the staging logic below to run. The early-return chain has the side benefit that `min_delta_distance` (initialized to 0 at function entry) is GUARANTEED to be set to 1000 in Step B below before the post-loop check at line 1913 — so the `if(min_delta_distance != 1000)` write-back gate is only reachable when Step B actually ran.

### Step A — centroid of dock squares on the war landmass (lines 1873-1886)

```c
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
```

Walk the war landmass's dock-square chain; accumulate coordinates and count; divide to get the centroid. The centroid represents the "where on the war landmass do we want to land" reference point.

**Source-flagged OGBUG (line 1884):** `landmass_node_centroid_wx /= landmass_node_count` divides without a zero-check. If the war landmass has no dock squares (empty chain), `landmass_node_count` stays 0 and the divisions crash. Same pattern as sibling [`AI_Reevaluate_Continent`](AIMOVE-AI_Reevaluate_Continent.md) Phase 6 and [`AI_Reevaluate_All_Continents`](AIMOVE-AI_Reevaluate_All_Continents.md) Phase 9. Preserved faithful-to-dasm.

### Step B — pick this landmass's dock square closest to the war centroid (lines 1887-1910)

```c
min_delta_distance = 1000;
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
            target_square_wx = landmass_node_wx;
            target_square_wy = landmass_node_wy;
        }
    }
    landmass_node_index = _ai_landmass_dock_squares_lists[wp][landmass_node_index];
}
```

Walk THIS landmass's dock-square chain; for each empty (eval == 0) dock, compute wrapped taxicab distance to the war centroid; track the minimum. "Empty" here means no enemy units, no sites — but ignores OWN units on the square (the chosen embark point may already have one of our stacks on it).

### Step C — apply `lmt_Leaveable` tag (lines 1911-1918)

```c
/* NOTE(JimBalcomb,20260521): this is the only place in the whole code-base that sets lmt_Leaveable */
if(min_delta_distance != 1000)
{
    _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_Leaveable;
    _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = (uint8_t)target_square_wx;
    _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = (uint8_t)target_square_wy;
}
```

If Step B found an acceptable dock square (`min_delta_distance` lowered below 1000), tag this landmass `lmt_Leaveable` and overwrite its stage point with that dock square. The next dispatch slot will see `lmt_Leaveable` and route troops toward the embark square.

## Inputs / outputs

**Reads:**
- `_ai_own_stack_*` (count, type, unit_count, wx, wy, unit_list) — roamer-stack inventory
- `_ai_targets_*` (via `AI_Stacks_Assign_Target`) — per-landmass target list from `AI_Build_Target_List`
- `Unit_Has_AirTravel` / `Unit_Has_WaterTravel` (per unit) — gathered but result unused
- `_ai_landmass_war_targets[wp][player_idx]` — main war landmass set by `AI_Choose_War_Landmass`
- `_ai_landmass_dock_squares_*` — per-landmass dock-square chains
- `g_ai_evaluation_map[wp][...]` — per-square evaluation map

**Writes:**
- Unit-level: `AI_Stacks_Order_Attack_Target_Or_Goto_Destination` orders for every unit in stacks that found targets
- Module-level: `g_ai_set_target_caller = 7`, `niu_unknown_var = 0`, `_ai_targets_value[picked] = 0` (via callee)
- Landmass classification: `_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_Leaveable` (conditional)
- Landmass stage: `wx_array[landmass_idx]` / `wy_array[landmass_idx]` (same conditional)

## Call graph

```
AI_Set_Unit_Orders                                  [AIMOVE.c:131, Phase 4 dispatch]
  └─ per (plane, landmass):
       ├─ ... slots 1-7 ...
       ├─ AI_Build_Target_List(player_idx, landmass_idx, wp)              [slot 8 — PRODUCER]
       │    └─ writes _ai_targets_count, _ai_targets_wx/wy/strength/value
       │
       └─ AI_Stacks_Roamers_Target_Or_Deploy(landmass_idx, wp, player_idx) [slot 9 — THIS FUNCTION]
            │
            ├─ Phase 2: per roamer stack (_ai_own_stack_type == AISTK_Roamer):
            │   ├─ inner unit-list walk → Unit_Has_AirTravel, Unit_Has_WaterTravel (results gathered, NIU)
            │   ├─ AI_Stacks_Assign_Target(stack_wx, stack_wy, &target_wx, &target_wy, ...)  [AIMOVE.c:2830]
            │   │    ↑ READS _ai_targets_*
            │   │    ↓ zeros _ai_targets_value[picked]
            │   │    ↓ returns picked target via out-params
            │   └─ if found: per unit in stack:
            │        └─ AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, target_wx, target_wy, ...)
            │
            └─ Embark-staging path (4 early-return gates):
                 ├─ Step A: walk _ai_landmass_dock_squares_* for war landmass → centroid
                 ├─ Step B: walk _ai_landmass_dock_squares_* for this landmass → closest empty
                 └─ Step C: write _ai_continents.{type, wx, wy}_array[landmass_idx] = {lmt_Leaveable, target_square_wx, target_square_wy}
```

## What `lmt_Leaveable` really means

Step C of this function is the **only writer** of `lmt_Leaveable` in the codebase.

### Lifetime — single-turn, unlike `lmt_NoTargets`

`AI_Evaluate_Continents` unconditionally rewrites `type_array` every turn it runs ([AIMOVE.c:6874](../../MoM/src/AIMOVE.c#L6874)). Its preservation clause at [line 6848](../../MoM/src/AIMOVE.c#L6848) only triggers for `lmt_NoTargets`, NOT `lmt_Leaveable`. So:

- Turn N, `AI_Set_Unit_Orders` dispatch slot 9: this function writes `lmt_Leaveable`.
- Turn N, dispatch slots 10/11/14: see the just-written tag and use it.
- Turn N+1, `AI_Next_Turn` driver: `AI_Choose_War_Landmass` reads the tag, then `AI_Evaluate_Continents` **clobbers it** with one of `lmt_Own`/`lmt_Contested`/`lmt_NoOwnCity`/`lmt_NoOwnCityAndAllyHasCity`.
- Turn N+1, dispatch: the previous Abandon decision is gone. To stay tagged, Step C must re-fire — which requires ALL four staging-gate conditions to still hold.

This is a **single-turn assertion** that only persists across one turn boundary (long enough for next turn's `AI_Choose_War_Landmass` to see it before `AI_Evaluate_Continents` runs).

### Read sites and what the tag means at each

| Reader | Test | Timing vs. writer | Meaning of tag at this read |
|---|---|---|---|
| `AI_Stacks_Order_To_War_Landmass` ([line 203](../../MoM/src/AIMOVE.c#L203) gate) | `>= lmt_Leaveable` (in OR-chain that's "almost NOT Contested") | Same turn, just after slot 9 wrote it | Pull units off this landmass toward main war |
| `AI_Stacks_Relocate_Roamers` ([line 220](../../MoM/src/AIMOVE.c#L220) gate) | `>= lmt_Leaveable || == lmt_Own` | Same turn | Stage point is valid — consolidate units there |
| `AI_Stacks_Garrison_Sites` ([line 233](../../MoM/src/AIMOVE.c#L233) gate) | `== lmt_Own || == lmt_Contested || >= lmt_Leaveable` | Same turn | Garrison maintenance allowed |
| `AI_Stacks_Garrison_Sites` internal ([line 380](../../MoM/src/AIMOVE.c#L380)) | `>= lmt_Leaveable` | Same turn | "We're in the abandoning/no-targets group" |
| `AI_Stacks_Garrison_Sites` internal ([line 452](../../MoM/src/AIMOVE.c#L452)) | `< lmt_Leaveable` | Same turn | "We're NOT abandoning" |
| `AI_Stacks_Do_Meld` ([line 4558](../../MoM/src/AIMOVE.c#L4558)) | `>= lmt_Leaveable` (node-meld eligibility) | Same turn, BEFORE slot 9 wrote it (Do_Meld is dispatch slot 4) | In practice, never sees Abandon |
| `AI_Choose_War_Landmass` ([line 7786](../../MoM/src/AIMOVE.c#L7786) switch) | `case lmt_Leaveable:` → `Reevaluate = ST_TRUE` | **Next turn**, before `AI_Evaluate_Continents` clobbers | "We marked this for abandonment last turn — reconsider whether the war target is still right" |

### The real meaning, in one sentence

`lmt_Leaveable` is a **single-turn ferry-staging marker**: "we have a large stack here, nothing to attack here, and a war elsewhere — for the rest of this turn, treat this landmass as having a valid stage point AT THE EMBARK DOCK so later dispatch slots can pull troops off; next turn, reconsider whether the main-war target is still correct."

It is NOT a long-term classification ("we are leaving this place"). It's a within-turn coordination signal between dispatch slot 9 (writer) and slots 10/11/14 (consumers), plus a single-turn-stale hint for next turn's war-landmass reselection.

### Why "Abandon" is a slightly misleading tag name

The function name `AI_Stacks_Roamers_Target_Or_Deploy` is more accurate for what this function actually does — the "Or_Deploy" half captures the embark-staging case. But the tag it writes is still called `lmt_Leaveable`, which suggests a strategic decision that persists. In reality:
- The tag does NOT persist past `AI_Evaluate_Continents` (one turn).
- The tag does NOT prevent re-targeting — slots 10/11/14 still process the landmass actively, just with the embark-stage point.
- The "abandon" is conditional on a large stack being there. A 6-unit roamer on an empty landmass does NOT trigger it — that stack just gets no orders.

A more accurate tag name would be `lmt_EmbarkingLargeStack` or `lmt_FerryStagingThisTurn`.

### Compared to `lmt_NoTargets`

| Aspect | `lmt_NoTargets` | `lmt_Leaveable` |
|---|---|---|
| Writers in production | 1 (`AI_Build_Target_List`) | 1 (this function) |
| Survives `AI_Evaluate_Continents`? | Yes (preservation clause at [line 6848](../../MoM/src/AIMOVE.c#L6848), subject to city-count OGBUG) | No (clobbered every turn) |
| Lifetime | Multi-turn until enemy arrives | Single turn |
| Read by `AI_Choose_War_Landmass`? | Yes (`!= lmt_NoTargets` filter) | Yes (switch `case lmt_Leaveable`) |
| Trigger | "No hostile assets found this turn" | "Large stack stranded, war elsewhere, embark point exists" |
| Effect of misfire | Wrong landmass excluded from war-target weighting | Wrong landmass gets treated as ferry-staging this turn |

## Related references

- [AIMOVE-AI_Build_Target_List.md](AIMOVE-AI_Build_Target_List.md) — producer of `_ai_targets_*` consumed by Phase 2 here
- [AIMOVE-AI_Stacks_Assign_Target.md](AIMOVE-AI_Stacks_Assign_Target.md) — the per-stack target picker called by Phase 2
- [AIMOVE-AI_Set_Unit_Orders.md](AIMOVE-AI_Set_Unit_Orders.md) — dispatcher; this function is slot 9
- [AIMOVE-AI_Choose_War_Landmass.md](AIMOVE-AI_Choose_War_Landmass.md) — producer of `_ai_landmass_war_targets[]` consumed by the staging path
- [AIMOVE-AI_Reevaluate_Continent.md](AIMOVE-AI_Reevaluate_Continent.md) — sibling with the same div-by-zero pattern in its Phase 6
- [AIMOVE-AI_Reevaluate_All_Continents.md](AIMOVE-AI_Reevaluate_All_Continents.md) — sibling with the same div-by-zero pattern in its Phase 9
- [MoM-AI-Landmass-Types.md](MoM-AI-Landmass-Types.md) — `lmt_Leaveable` state machine entry; this function is the sole writer of that tag
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
