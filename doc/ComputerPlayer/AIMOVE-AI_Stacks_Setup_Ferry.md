AIMOVE-AI_Stacks_Setup_Ferry.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Stacks_Setup_Ferry

---

# AI_Stacks_Setup_Ferry — Walkthrough

**Location:** [MoM/src/AIMOVE.c:2028](../../MoM/src/AIMOVE.c#L2028) (~234 lines, ends 2261).
**WZD overlay:** ovr158, p09
**drake178 name:** `G_AI_RallyOrFerry()` — "gives any unassigned units move orders to the passed tile, gives any adjacent ships a halt order (move to their current location), and tries to move any remaining units in the stack to a ship if there is one adjacent. **full of BUGs and inconsistencies**."

## Purpose

Per-stack workhorse called by [`AI_Stacks_Relocate_Roamers`](AIMOVE-AI_Stacks_Relocate_Roamers.md) for every roamer stack the parent wants to rally. Does three things in one pass:

1. **Register a ferry-pickup request** for an ocean square near the stage point (via [`AI_Stacks_Ferry_Add_Location`](AIMOVE-AI_Stacks_Ferry_Add_Location.md)) — for the next-turn transport dispatcher to pick up.
2. **Issue goto-stage orders** for up to `stage_count_slot_count` units in the stack (capped by the parent's open-slot budget).
3. **Opportunistic embark** — if a transport happens to be parked on an adjacent ocean square right now, redirect the stack's units toward it instead of waiting for next turn.

Sole caller: [`AI_Stacks_Relocate_Roamers`](AIMOVE-AI_Stacks_Relocate_Roamers.md) at [line 1916](../../MoM/src/AIMOVE.c#L1916).

## Why "full of BUGs and inconsistencies"

Drake178's header comment isn't an exaggeration. The function preserves several inline `OGBUG` / `BUG?` annotations (drake178's plus user-added ones) describing things the OG code does wrong. Faithful reconstruction keeps all of them; this walkthrough catalogues them for reference.

The bug catalog (numbered here for cross-reference within this doc; not in source):

| # | Issue | Source annotation | Severity |
|---|---|---|---|
| B1 | Phase 2 scans around the stage point, not around the stack (per the implicit assumption that "vicinity" means stack) | none — code uses `stage_wx`/`stage_wy` | Minor — depends on intent |
| B2 | Phase 2 may not find any ocean — `ocean_wx/wy` default to the stack's land coords | `/* OGBUG  will add the original land tile if there was no ocean around the stack */` | Real — pollutes ferry-request list |
| B3 | Phase 2 doesn't validate world bounds before indexing `_landmasses[]` | `/* OGBUG  should validate coordinates */` | Real — out-of-bounds reads at world edge |
| B4 | Phase 4 doesn't check that pulled units can physically reach the stage point | none — preserved drake178 spirit, not annotated | Real — orphan goto orders for non-reachable destinations |
| B5 | Phase 5 result vars are written but never read (`niu_` prefix signals this) | `/* DEDU  For what might this have been meant to be used? */` + the NOTE pointing to `AI_SendToColonize__WIP` | Dead code — entire phase has no observable effect |
| B6 | Phase 5 stores `itr_stack_wx` (un-wrapped) into `niu_embarkable_wx` instead of the wrapped `wx_offset` | none (my flag) | Dead — would matter only if a consumer existed |
| B7 | Phase 6 unit-position filter doesn't check the unit's plane | `/* OGBUG  should use unit's wp */` | Real — counts units on the other plane if their (wx,wy) lands in the box |
| B8 | Phase 6's `Unit_Has_AirTravel` / `Unit_Has_WaterTravel` checks ignore spell/item-granted travel | none — same pattern as flagged in [AIMOVE-AI_Stacks_Order_To_War_Landmass.md](AIMOVE-AI_Stacks_Order_To_War_Landmass.md) | Real — misses Wind Walking, Flight spell, mobility items |
| B9 | Phase 6 issues a goto order to a transport's own current tile (no-op move) | none — preserved pattern | Quirk — may be intentional "park" marker |
| B10 | Phase 6 tracks `total_boat_capacity` aggregated across all 9 squares but `adjacent_unit_on_ocean_wx/wy` only holds the LAST-found ocean unit | `/* OGBUG  should track coordinates and capacity for where flag was set */` | Real — Phase 7a's destination may not match where the transport capacity actually is |
| B11 | Phase 7a uses `\|\|` for the "lacks travel" decrement; Phase 6 uses `&&` | none (my flag) | Real inconsistency — Phase 7a decrements too aggressively |
| B12 | Phase 7b is unreachable (`total_boat_capacity` is always 0 in the `else` branch) | `/* unreachable - total_boat_capacity is always zero if there was no adjacent ocean unit found */` | Documented dead code — see "Why no community complaints?" below |

Two bugs previously catalogued have been corrected in source and are not listed here:
- The duplicate `Y_Offset` iterator in Phase 2 (now correctly uses `wy_offset` outer / `wx_offset` inner).
- The Phase 6 bounding box `min_wy/max_wy` derived from `stack_wx` (now correctly uses `stack_wy`).

## Signature

```c
void AI_Stacks_Setup_Ferry(
    int16_t stack_idx,                  // index into _ai_own_stack_*[] of the stack being processed
    int16_t landmass_idx,               // landmass we're rallying on (cached as niu_Landmass_idx, never read)
    int16_t wp,                         // plane
    int16_t stage_wx,                   // stage point X (passed through from AI_Stacks_Relocate_Roamers)
    int16_t stage_wy,                   // stage point Y
    int16_t stage_count_slot_count,     // open-slot budget for Phase 4 goto orders
    int16_t player_idx                  // player whose units we're looking at (used in Phase 6)
)
```

## Locals

```c
int16_t unit_type;                          // _UNITS[itr_units].type cache (Phase 6)
int16_t max_wy;                             // bounding-box max Y (Phase 6, around stack)
int16_t min_wy;                             // bounding-box min Y (Phase 6)
int16_t max_wx;                             // bounding-box max X (Phase 6)
int16_t min_wx;                             // bounding-box min X (Phase 6)
int16_t ocean_wy;                           // ocean-tile coords for ferry request (Phase 2 output, Phase 3 input)
int16_t ocean_wx;
int16_t unit_idx;                           // per-iteration unit index cache
int16_t list_unit_count;                    // _ai_own_stack_unit_count[stack_idx] cache (Phase 4)
int16_t stack_wy;                           // _ai_own_stack_wy[stack_idx] cache
int16_t stack_wx;                           // _ai_own_stack_wx[stack_idx] cache
int16_t free_stack_slots_on_square;         // = MAX_STACK; Phase 6 decrements for each ocean unit found
int16_t total_boat_capacity;                // Phase 6 sum of nearby transports' capacity (with Phase 6 / Phase 7a decrements)
int16_t niu_embarkable_wy;                  // Phase 5 dead write (B5) — "niu" = Not In Use
int16_t niu_embarkable_wx;                  // Phase 5 dead write
int16_t niu_Landmass_idx;                   // = landmass_idx, then never read (orphaned cache)
int16_t adjacent_unit_on_ocean_wy;          // position of last-found ocean unit (Phase 6 → Phase 7a destination)
int16_t adjacent_unit_on_ocean_wx;
int16_t Adjacent_X;                         // (legacy declaration; replaced in body by itr_stack_wx for Phase 5)
int16_t Y_Offset;                           // (legacy declaration; replaced in body by itr_stack_wy for Phase 5)
int16_t itr_units;                          // Phase 6 _UNITS[] iterator
int16_t found_adjacent_unit_on_ocean;       // Phase 6 flag → Phase 7 branch selector
int16_t niu_embarkable_found;               // Phase 5 dead write (B5)
int16_t itr_list_units;                     // Phase 4 / Phase 7 _ai_own_stack_unit_list[] iterator
int16_t X_Offset;                           // (legacy declaration; replaced in body by wx_offset)
int16_t wx_offset;                          // Phase 2 inner-loop iterator
int16_t wy_offset;                          // Phase 2 outer-loop iterator
int16_t unit_wx;                            // DNE in Dasm; reuses wx_offset for Phase 6 unit position cache
int16_t unit_wy;                            // DNE in Dasm; reuses wy_offset
int16_t itr_stack_wx;                       // DNE in Dasm; reuses Adjacent_X for Phase 5 inner iterator
int16_t itr_stack_wy;                       // DNE in Dasm; reuses Y_Offset for Phase 5 outer iterator
```

The `niu_` (Not In Use) and `DNE in Dasm` (Does Not Exist in the original disassembly) prefixes are explicit reconstruction-vs-OG annotations. The `niu_` locals are dead writes preserved from the original code; the `DNE` locals are reconstruction conveniences added to make the code more readable while still mapping back to the same register/stack-slot pattern.

## Code walk

### Phase 1 — Cache stack coords + seed `ocean_wx/wy` ([lines 2062-2067](../../MoM/src/AIMOVE.c#L2062-L2067))

```c
/* Phase 1: Init */
niu_Landmass_idx = landmass_idx;
stack_wx = _ai_own_stack_wx[stack_idx];
stack_wy = _ai_own_stack_wy[stack_idx];
ocean_wx = stack_wx;       // Phase 2 default if no ocean found
ocean_wy = stack_wy;
```

Phase 2's loop only updates `ocean_wx/wy` on a hit, so this seed is what gets passed to `AI_Stacks_Ferry_Add_Location` if no ocean is found (B2).

### Phase 2 — Find an ocean tile in 3×3 around the STAGE point ([lines 2070-2084](../../MoM/src/AIMOVE.c#L2070-L2084))

```c
/* Phase 2: Find adjacent Ocean square */
/* OGBUG  should handle no ocean found */
/* OGBUG  should validate coordinates */
/* OGBUG  should early-exit on Ocean found */
for(wy_offset = -1; wy_offset < 2; wy_offset++)
{
    for(wx_offset = -1; wx_offset < 2; wx_offset++)
    {
        if(_landmasses[((wp * WORLD_SIZE) + ((stage_wy + wy_offset) * WORLD_WIDTH) + (stage_wx + wx_offset))] == 0)
        {
            ocean_wx = (stage_wx + wx_offset);
            ocean_wy = (stage_wy + wy_offset);
        }
    }
}
```

Scans the 3×3 area centered on `(stage_wx, stage_wy)` and records the LAST ocean square found (no early-exit on first hit — B/OGBUG marker `should early-exit on Ocean found`).

**B1** — scans around the STAGE point, not the stack. Whether this is the intent depends on whether you read the function as "find an ocean tile that the stage point can ferry-pick up from" (stage-centric, current behavior) or "find an ocean tile adjacent to the stack itself" (stack-centric). Given the function name "RallyOrFerry" and the call context (the stack is at `stack_wx/wy` and wants to embark there), stack-centric would seem more natural. But the code as written is stage-centric.

**B2** — if none of the 9 checked squares is ocean (landmass index 0), `ocean_wx`/`ocean_wy` keep their Phase 1 seed values (the stack's own land coords). Phase 3 then registers the stack's land position with `AI_Stacks_Ferry_Add_Location` as if it were an ocean pickup point.

**B3** — `(stage_wy + wy_offset)` and `(stage_wx + wx_offset)` are not range-checked. If the stage point is near the world edge the indexed `_landmasses[]` position can go out of bounds.

### Phase 3 — Register ferry-pickup request ([line 2089](../../MoM/src/AIMOVE.c#L2089))

```c
/* Phase 3 */
/* OGBUG  will add the original land tile if there was no ocean around the stack */
AI_Stacks_Ferry_Add_Location(ocean_wx, ocean_wy, wp);
```

Adds `(ocean_wx, ocean_wy, wp)` to the ferry-request list — see [AIMOVE-AI_Stacks_Ferry_Add_Location.md](AIMOVE-AI_Stacks_Ferry_Add_Location.md). Subject to B2 above (may be a land square).

This is the **patient-path** half of the function — the request gets serviced next turn by `G_AI_ProcessTransports__WIP` directing an idle transport toward the registered coord.

### Phase 4 — Issue goto-stage orders if stack not at stage ([lines 2092-2115](../../MoM/src/AIMOVE.c#L2092-L2115))

```c
/* Phase 4 */
/* If the stack is not at the staging point, move units to it */
if(
    (stack_wx != stage_wx)
    ||
    (stack_wy != stage_wy)
)
{
    list_unit_count = _ai_own_stack_unit_count[stack_idx];
    for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
    {
        if(stage_count_slot_count <= 0)
        {
            break;
        }
        unit_idx = _ai_own_stack_unit_list[stack_idx][itr_list_units];
        g_ai_set_target_caller = 9;
        AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, stage_wx, stage_wy, stack_idx, itr_list_units);
        stage_count_slot_count--;
    }
}
```

This is the **rally** half. For stacks not already at the stage point:
- Iterate the stack's unit slots up to the stack's unit count, with an explicit `break` when the parent's `stage_count_slot_count` budget hits zero.
- Issue a goto-stage order for each (the order-setter consumes the slot via `ST_UNDEFINED` write).
- Decrement `stage_count_slot_count` per order issued.

**B4** — no path-reachability check. A unit ordered to walk to a stage point it physically can't reach piles up `Move_Failed = TRUE` turn after turn. (The wider AI doesn't have a recovery path for this beyond "try again next turn.")

**No ST_UNDEFINED guard.** The iterator reads `unit_idx` without checking `ST_UNDEFINED` first. If a prior dispatch slot consumed this stack's slots, `unit_idx == -1` gets passed to `AI_Stacks_Order_Attack_Target_Or_Goto_Destination`, which has its own range check that handles the bad case. Robust by virtue of the callee's check; not by this function's own logic.

### Phase 5 — "Find embarkable tile in 3×3 around stack" (DEAD WRITES) ([lines 2118-2148](../../MoM/src/AIMOVE.c#L2118-L2148))

```c
/* Phase 5 */
/* DEDU  For what might this have been meant to be used? */
/* NOTE(JimBalcomb,20260523): this block is in AI_SendToColonize__WIP(), used properly there */
for(itr_stack_wy = (stack_wy - 1); (stack_wy + 2) > itr_stack_wy; itr_stack_wy++)
{
    if(
        (itr_stack_wy > 0)
        &&
        (itr_stack_wy < WORLD_HEIGHT)
    )
    {
        for(itr_stack_wx = (stack_wx - 1); (stack_wx + 2) > itr_stack_wx; itr_stack_wx++)
        {
            wx_offset = itr_stack_wx;
            if(wx_offset < 0)            { wx_offset += WORLD_WIDTH; }   // wrap left
            if(wx_offset >= WORLD_WIDTH) { wx_offset -= WORLD_WIDTH; }   // wrap right
            if(Map_Square_Is_Embarkable(wx_offset, itr_stack_wy, wp) != ST_FALSE)
            {
                niu_embarkable_found = ST_TRUE;
                niu_embarkable_wx = itr_stack_wx;          // ← B6: stores unwrapped
                niu_embarkable_wy = itr_stack_wy;
            }
        }
    }
}
```

**B5 — Dead writes.** `niu_embarkable_found`, `niu_embarkable_wx`, `niu_embarkable_wy` are written here but never read anywhere else in this function. The `niu_` prefix signals "Not In Use" per the project glossary. Phase 5 runs and accomplishes nothing observable.

The inline NOTE points at `AI_SendToColonize__WIP` as a function that contains the same block and DOES use its output. So this Phase 5 is plausibly copy-pasted from there (or was intended to converge), with the consumer left out.

**B6** — `niu_embarkable_wx = itr_stack_wx` stores the un-wrapped value (could be `-1` or `WORLD_WIDTH`). The wrapped value lives in `wx_offset`. Would matter only if a consumer existed.

### Phase 6 — Survey ocean units in 3×3 around STACK ([lines 2151-2212](../../MoM/src/AIMOVE.c#L2151-L2212))

```c
/* Phase 6 */
/* Search all units for a nearby transport on the ocean */
/* OGBUG  should skip center square */
/* OGBUG  should track coordinates and capacity for where flag was set */
found_adjacent_unit_on_ocean = ST_FALSE;
min_wx = (stack_wx - 1);
min_wy = (stack_wy - 1);
max_wx = (stack_wx + 1);
max_wy = (stack_wy + 1);
total_boat_capacity = 0;
free_stack_slots_on_square = MAX_STACK;
for(itr_units = 0; itr_units < _units; itr_units++)
{
    if(_UNITS[itr_units].owner_idx != player_idx) { continue; }
    unit_wx = _UNITS[itr_units].wx;
    unit_wy = _UNITS[itr_units].wy;
    if((unit_wx < min_wx) || (unit_wy < min_wy) ||
       (unit_wx > max_wx) || (unit_wy > max_wy))   { continue; }
    /* OGBUG  should use unit's wp */
    if(_landmasses[((wp * WORLD_SIZE) + (unit_wy * WORLD_WIDTH) + unit_wx)] != 0) { continue; }
    /* found any adjacent unit on an ocean square */
    found_adjacent_unit_on_ocean = ST_TRUE;
    adjacent_unit_on_ocean_wx = _UNITS[itr_units].wx;   // last-seen ocean unit's square
    adjacent_unit_on_ocean_wy = _UNITS[itr_units].wy;
    unit_type = _UNITS[itr_units].type;
    if(_unit_type_table[unit_type].Transport > 0)
    {
        _UNITS[itr_units].dst_wx = _UNITS[itr_units].wx;
        _UNITS[itr_units].dst_wy = _UNITS[itr_units].wy;
        _UNITS[itr_units].Status = us_GOTO;
        total_boat_capacity += _unit_type_table[unit_type].Transport;   // ← B10: aggregated across 9 squares
    }
    else
    {
        if((Unit_Has_AirTravel(itr_units) == ST_FALSE) &&
           (Unit_Has_WaterTravel(itr_units) == ST_FALSE))
        {
            total_boat_capacity--;       // passenger needing ferry: deduct one (correct AND)
        }
    }
    free_stack_slots_on_square--;
}
```

Scans every owned unit in `_UNITS[]`, filters to those whose (wx, wy) lies in the 3×3 box around the stack AND whose square is ocean on plane `wp`.

For each hit:
- Set `found_adjacent_unit_on_ocean = TRUE`
- Update `adjacent_unit_on_ocean_wx/wy` (LAST-found ocean unit's position)
- If it's a transport: park it (no-op goto to own tile, B9) and bump `total_boat_capacity`
- Else if it has neither air nor water travel: decrement `total_boat_capacity` (passenger needing ferry)
- Decrement `free_stack_slots_on_square` regardless of type

**B7 — no plane check on the unit.** `_UNITS[itr_units].wp` is never compared to `wp`. A unit on the other plane that happens to be at coords inside the box gets counted. The `_landmasses[wp * WORLD_SIZE + ...]` check verifies the SQUARE is ocean on plane `wp`, not that the UNIT is on plane `wp`.

**B8 — Air/Water travel check ignores transient sources.** `Unit_Has_AirTravel` and `Unit_Has_WaterTravel` presumably check unit-type base abilities + persistent ability flags. They likely don't account for spell-granted travel (Flight, Water Walking) or item-granted travel, or stack-mate buffs like Wind Walking. Same caveat as flagged in [AIMOVE-AI_Stacks_Order_To_War_Landmass.md](AIMOVE-AI_Stacks_Order_To_War_Landmass.md).

**B9 — transports get a no-op goto to their current tile.** `dst_wx = wx`, `dst_wy = wy`, `Status = us_GOTO`. Plausibly intentional ("park this transport so it doesn't drift off this turn") but the goto-to-self is unusual.

**B10 — aggregated counts across 9 squares.** `total_boat_capacity` sums transport capacity from any ocean square in the box, but Phase 7a's destination is `adjacent_unit_on_ocean_wx/wy` — the position of the LAST-found ocean unit. Capacity may exist at a tile the stack isn't being directed to. Source comment `/* OGBUG  should track coordinates and capacity for where flag was set */` flags this explicitly.

**`free_stack_slots_on_square` is now meaningfully decremented.** Each found ocean unit reduces the budget. This is a real bound for Phase 7a's inner check — if the box already contains 9 own ocean units, no room left for the stack's units to embark.

### Phase 7 — Dispatch based on `found_adjacent_unit_on_ocean` ([lines 2215-2259](../../MoM/src/AIMOVE.c#L2215-L2259))

#### Phase 7a — Opportunistic embark ([lines 2216-2242](../../MoM/src/AIMOVE.c#L2216-L2242))

```c
/* Phase 7: Optimization - join neighbor or actively seek transport */
if(found_adjacent_unit_on_ocean == ST_TRUE)
{
    /* Phase 7a: override the ferry request */
    for(itr_list_units = 0; _ai_own_stack_unit_count[stack_idx] > itr_list_units; itr_list_units++)
    {
        if(itr_list_units < free_stack_slots_on_square)
        {
            if(total_boat_capacity > 0)
            {
                unit_idx = _ai_own_stack_unit_list[stack_idx][itr_list_units];
                g_ai_set_target_caller = 10;
                AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, adjacent_unit_on_ocean_wx, adjacent_unit_on_ocean_wy, stack_idx, itr_list_units);
                if((Unit_Has_AirTravel(unit_idx) == ST_FALSE) ||      // ← B11: || vs Phase 6's &&
                   (Unit_Has_WaterTravel(unit_idx) == ST_FALSE))
                {
                    total_boat_capacity--;
                }
            }
        }
    }
}
```

If at least one own ocean unit was found, redirect each stack unit toward the LAST-found ocean unit's position (subject to `free_stack_slots_on_square` room and `total_boat_capacity > 0`). This **overrides** the Phase 4 goto-stage orders for the same stack — the most recent `AI_Stacks_Order_Attack_Target_Or_Goto_Destination` write wins on the unit's Status/dst fields.

**B11 — `||` vs Phase 6's `&&`.** Phase 6 decrements only when a unit has NEITHER air NOR water travel. Phase 7a decrements when a unit lacks EITHER air OR water travel — meaning it decrements for almost every unit (anything without BOTH). The accounting is inconsistent between the two phases. Almost certainly a typo.

#### Phase 7b — DEAD CODE ([lines 2243-2259](../../MoM/src/AIMOVE.c#L2243-L2259))

```c
else  /* (found_adjacent_unit_on_ocean == ST_FALSE) */
{
    /* Phase 7b: set ferry request */
    /* would have been an optimization, because us_Ferry? otherwise, Phase 3 and 4 still set up the stage move and ferry dispatch */
    for(itr_list_units = 0; _ai_own_stack_unit_count[stack_idx] > itr_list_units; itr_list_units++)
    {
        if(itr_list_units < free_stack_slots_on_square)
        {
            /* unreachable - total_boat_capacity is always zero if there was no adjacent ocean unit found */
            if(total_boat_capacity > 0)
            {
                unit_idx = _ai_own_stack_unit_list[stack_idx][itr_list_units];
                AI_Stacks_Order_Ferry(unit_idx, stack_idx, itr_list_units);
            }
        }
    }
}
```

**B12 — entire inner body is unreachable.** Phase 6 only modifies `total_boat_capacity` inside the `_landmasses[...] != 0 continue` filter (and the surrounding ownership/box filters), and that path also sets `found_adjacent_unit_on_ocean = TRUE` before the modification. So if we're in this `else` branch, `total_boat_capacity` is exactly 0 (never bumped). `if(total_boat_capacity > 0)` is `if(0 > 0)` → always false → [`AI_Stacks_Order_Ferry`](AIMOVE-AI_Stacks_Order_Ferry.md) never executes from this path.

This is documented in source (`/* unreachable ... */`). It looks like the `if(total_boat_capacity > 0)` gate is a copy-paste from Phase 7a where the same gate fits ("only embark if there's spare capacity"). In Phase 7b the intent was probably the opposite — "if NO nearby ocean unit, mark each unit as actively seeking transport via `us_Ferry`" — but the inverted gate makes the body unreachable.

**Why no community complaints despite the dead code?** The patient path (Phase 3 + next-turn `G_AI_ProcessTransports__WIP` + auto-embark when units walk onto a transport's tile) covers the basic ferry need without Phase 7b firing. Phase 7b's job was to mark units with `us_Ferry` status as an additional optimization — players don't notice its absence because the patient path is functionally adequate.

(The function [`AI_Stacks_Order_Ferry`](AIMOVE-AI_Stacks_Order_Ferry.md) itself IS called from elsewhere — lines 1490 and 5127 — so it's not dead, just this caller path is.)

## ASCII summary

```
AI_Stacks_Setup_Ferry(stack_idx, landmass_idx, wp, stage_wx, stage_wy, stage_count_slot_count, player_idx)
  ├─ Phase 1: cache stack coords, seed ocean_wx/wy with stack position
  ├─ Phase 2: scan 3×3 around STAGE point for ocean tile  [B1-B3]
  ├─ Phase 3: AI_Stacks_Ferry_Add_Location(ocean_wx, ocean_wy, wp)
  ├─ Phase 4: if stack not at stage:
  │    └─ for u in stack, capped by stage_count_slot_count:
  │         └─ goto order to (stage_wx, stage_wy)         [caller=9]
  ├─ Phase 5: scan 3×3 for embarkable tile                [B5-B6: dead writes]
  ├─ Phase 6: scan _UNITS[] for own units in 3×3 box on ocean squares:
  │    ├─ set found_adjacent_unit_on_ocean
  │    ├─ track adjacent_unit_on_ocean_wx/wy (last hit)
  │    ├─ transports → stay-put goto, +Transport to total_boat_capacity  [B9]
  │    ├─ non-mobile → -1 to total_boat_capacity
  │    └─ free_stack_slots_on_square-- for each ocean unit found
  └─ Phase 7:
       ├─ if found_adjacent_unit_on_ocean:
       │    └─ for u in stack (bounded by free_stack_slots_on_square):
       │         └─ if total_boat_capacity > 0:
       │              └─ goto order to (adjacent_unit_on_ocean_wx/wy)   [caller=10, B11]
       └─ else: DEAD CODE — would order us_Ferry but total_boat_capacity always 0 [B12]
```

## Net behavior despite the bugs

Despite the bug catalog, the function does the core job on most calls:

- **Phase 3** files a ferry-pickup request (subject to B2 — coord may be wrong but a request lands either way).
- **Phase 4** issues correct goto-stage orders for stacks not at the stage point — the "rally" effect.
- **Phase 7a** correctly redirects to a nearby own ocean unit when one is found — the "opportunistic embark" effect.

The buggy bits (Phase 2's `else if`-vs-`else`, Phase 5's dead writes, Phase 6's plane non-check, Phase 7b's dead branch) mostly fail silently. The game runs; the AI makes mildly suboptimal decisions about pickup points and embark targets. See the discussion of Phase 7b above for why no player complaints have surfaced.

## Related references

- [AIMOVE-AI_Stacks_Relocate_Roamers.md](AIMOVE-AI_Stacks_Relocate_Roamers.md) — sole caller; passes `stack_idx`, `stage_wx/wy`, and `stage_count_slot_count` budget
- [AIMOVE-AI_Stacks_Ferry_Add_Location.md](AIMOVE-AI_Stacks_Ferry_Add_Location.md) — called from Phase 3 (registers pickup request)
- [AIMOVE-AI_Stacks_Order_Ferry.md](AIMOVE-AI_Stacks_Order_Ferry.md) — referenced from Phase 7b (unreachable from this caller)
- [AIMOVE-AI_Stacks_Order_To_War_Landmass.md](AIMOVE-AI_Stacks_Order_To_War_Landmass.md) — shares the `Unit_Has_AirTravel` / `Unit_Has_WaterTravel` B8-equivalent caveat
- `AI_SendToColonize__WIP` (in AIMOVE.c, not yet walked through) — contains the same Phase 5 block, used properly there per the inline NOTE
- [MoX/src/MOM_DAT.h:524](../../MoX/src/MOM_DAT.h#L524) — `us_GOTO` enum value (used by Phase 6 stay-put writes)
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
