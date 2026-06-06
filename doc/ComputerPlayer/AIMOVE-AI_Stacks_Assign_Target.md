AIMOVE-AI_Stacks_Assign_Target.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Stacks_Assign_Target.c

SEEALSO: AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md

---

# AI_Stacks_Assign_Target — Walkthrough

**Location:** [MoM/src/AIMOVE.c:2830](../../MoM/src/AIMOVE.c#L2830) (~113 lines, ends 2943). GEMINI variant has been removed.
**WZD overlay:** ovr158, p12
**drake178 name:** `AI_AssignStackTarget()`

## Purpose

**Pick the best target from the per-landmass target list for one roamer stack.** Computes the stack's combined effective strength, walks `_ai_targets_*` (built by [`AI_Build_Target_List`](AIMOVE-AI_Build_Target_List.md)), filters out targets too strong for this stack to take, scores remaining candidates by `value / distance`, and returns the highest-rated reachable target via out-parameters. "Consumes" the chosen target by zeroing its value so no other stack can claim it in the same landmass-iteration.

Per the source preamble:

> `; selects a target for the unassigned units in the specified stack, clearing its value and returning 1 if a valid one was found, or returning 0 otherwise`

## Signature

```c
int16_t AI_Stacks_Assign_Target(
    int16_t   stack_wx,
    int16_t   stack_wy,
    int16_t * target_wx,            // out
    int16_t * target_wy,            // out
    int16_t   unit_list_idx,        // index into _ai_own_stack_unit_list[][]
    int16_t   niu_list_unit_idx     // NIU — read only to silence "unused parameter" warning; see Phase 1
);
```

Returns `ST_TRUE` if a target was picked (out-params written), `ST_FALSE` otherwise.

## Locals

```c
int16_t last_move_failed = 0;             // set from LAST unit's _UNITS[].Move_Failed during Phase 2
int16_t player_idx = 0;                   // set from LAST unit's owner_idx during Phase 2
int16_t wp = 0;                           // set from LAST unit's wp during Phase 2
int16_t effective_unit_strength = 0;      // per-unit cache (Effective_Unit_Strength(unit_idx) / 10)
int16_t delta_distance = 0;               // wrapped taxicab distance, floored at 1
int16_t target_weighted_value = 0;        // value / (distance * 4), floored at 1
int16_t targets_idx = 0;                  // picked target index; ST_UNDEFINED until something wins
int16_t highest_target_value = 0;         // running max
int16_t effective_stack_strength = 0;     // accumulator over the stack's units (sum of effective_unit_strength)
int16_t unit_idx = 0;
int16_t list_unit_count = 0;
int16_t itr_list_units = 0;
int16_t itr_targets = 0;
int16_t return_value = 0;                 // DNE in Dasm
```

## Phase 1 — Cache the stack's unit count + touch the dead parameter (lines 2848-2850)

```c
/* DEAD */  itr_list_units = niu_list_unit_idx;
list_unit_count = _ai_own_stack_unit_count[unit_list_idx];
```

The `/* DEAD */` assignment reads `niu_list_unit_idx` once into `itr_list_units` — both to silence "unused parameter" warnings and to mark the parameter visibly as NIU (Not In Use). `itr_list_units` is immediately overwritten by the Phase 2 loop initializer, so the read has no functional effect. The parameter exists in the OG bytes; the production C reconstruction preserves the signature.

## Phase 2 — Sum effective strength + cache last-unit's wp/owner/Move_Failed (lines 2853-2868)

```c
effective_stack_strength = 0;
for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
{
    unit_idx = _ai_own_stack_unit_list[unit_list_idx][itr_list_units];
    if(unit_idx != ST_UNDEFINED)
    {
        wp = _UNITS[unit_idx].wp;                                      // ← overwritten each iter; ends with LAST unit's wp
        player_idx = _UNITS[unit_idx].owner_idx;                       // ← overwritten each iter; ends with LAST unit's owner
        /* ¿ OGBUG  should be once true, always true (ever false), for the whole stack ? */
        last_move_failed = _UNITS[unit_idx].Move_Failed;               // ← overwritten each iter; ends with LAST unit's Move_Failed
        effective_unit_strength = (Effective_Unit_Strength(unit_idx) / 10);
        effective_stack_strength += effective_unit_strength;
    }
}
```

**Intent:** sum the stack's effective combat strength (the comparator for target filtering in Phase 3) and incidentally cache the last unit's `wp`, `owner_idx`, and `Move_Failed` flag for use in Phase 3. The `wp` and `owner_idx` caching is OK in practice — all units in a stack share the same plane and owner. The `Move_Failed` caching is "last unit wins" — see the Source-flagged OGBUG below.

**Why `/ 10`?** The `g_ai_evaluation_map` strength bits (set by `AI_Evaluation_Map`) use the same "tenth of effective strength" scale per unit, so the comparison `_ai_targets_strength[itr] * 3/4 < effective_stack_strength` in Phase 3 is unit-consistent.

**Sparse-list handling.** `_ai_own_stack_unit_list[stack][slot]` becomes sparse during the AI dispatch. Per the comment block at [MoX/src/MOM_DAT.c:2697](../../MoX/src/MOM_DAT.c#L2697), the pattern is the same as Phase 4's `_ai_targets_value[targets_idx] = 0` "consume" idiom below — every per-unit order-issuer (`AI_Stacks_Order_Attack_Target_Or_Goto_Destination` at [AIMOVE.c:5076](../../MoM/src/AIMOVE.c#L5076), `AI_Order_Settle` at [AIMOVE.c:5120](../../MoM/src/AIMOVE.c#L5120), `AI_Stacks_Order_RoadBuild`, `AI_Stacks_Order_Purify`, `AI_Stacks_Order_Meld`, `AI_Stacks_Order_Ferry`) consumes the unit's slot by writing `ST_UNDEFINED`, marking it processed. The `if(unit_idx != ST_UNDEFINED)` check correctly skips consumed slots while still iterating to subsequent valid ones — `effective_stack_strength` and `last_move_failed` reflect only the still-available units. In practice the caller `AI_Stacks_Roamers_Target_Or_Deploy` short-circuits before invoking this function if ANY unit in the stack is consumed, so the typical call sees a fully-dense list; the sparse-handling here is defensive.

The symmetry is exact: `_ai_targets_*` and `_ai_own_stack_unit_list[][]` are both "consume-by-sentinel-write" arrays. `_ai_targets_value[i] = 0` consumes target slot `i`; `_ai_own_stack_unit_list[s][u] = ST_UNDEFINED` consumes unit slot `(s, u)`. Same idiom, different sentinel value (chosen so the consumed value falls outside the valid range — 0 isn't a valid target value, ST_UNDEFINED isn't a valid unit index).

### `Move_Failed` lifecycle (relevant to the OGBUG below)

`_UNITS[].Move_Failed` is **single-turn sticky**, not forever-sticky:

- **Set TRUE** at exactly one site: [UNITSTK.c:385](../../MoM/src/UNITSTK.c#L385), in the post-move cleanup after `Move_Units` — when a unit was ordered to a destination (status `us_GOTO`/`us_Move`), `Move_Units` was called, but the unit's `(wx, wy)` didn't actually change. The status is bounced back to `us_Ready` and `dst_wx/wy` cleared. Causes can be: no path, blocked by another unit, out of moves mid-route. The flag conflates all "tried, didn't budge" outcomes.
- **Reset FALSE** in [AIDUDES.c:297-303](../../MoM/src/AIDUDES.c#L297-L303) at the start of each AI turn, for all non-human units. Also FALSE at unit creation ([NEXTTURN.c:1001](../../MoM/src/NEXTTURN.c#L1001), [MAPGEN.c:6269](../../MoM/src/MAPGEN.c#L6269)).
- **Field definition** at [MoX/src/MOM_DAT.h:1768](../../MoX/src/MOM_DAT.h#L1768): `/* reset to ST_FALSE in AI_Next_Turn(), for all non-human units */`.

So within a single AI turn the flag accumulates (sticky), but it's cleared at the start of every AI turn. `Make_Move_Path` itself is a **pure pathfinding query** ([UnitMove.c:746](../../MoM/src/UnitMove.c#L746)); it doesn't mutate `Move_Failed`. The flag is set ONLY by `Move_Units`'s post-execution check.

### Source-flagged OGBUG (line 2863)

Inline comment: `/* ¿ OGBUG  should be once true, always true (ever false), for the whole stack ? */`

`last_move_failed` reflects only the LAST valid unit in the stack's unit list. If units within a stack have differing `Move_Failed` flags this turn (one unit got stuck, others didn't), only the last one's flag reaches the Phase 3 gate. Combined with the within-turn stickiness above, the value is a weak proxy for "does this stack need a path verified." The OG comment's "for the whole stack" framing captures the concern: a stack-level OR across all units would be more conservative than the current last-unit-only value.

## Phase 3 — Score and pick the best reachable target (lines 2871-2926)

```c
highest_target_value = 0;
targets_idx = ST_UNDEFINED;
for(itr_targets = 0; itr_targets < _ai_targets_count; itr_targets++)
{
    if (_ai_targets_value[itr_targets] <= 0)
    {
        continue;                                                              // target already consumed
    }
    /* Strength Check: Stack must be stronger than 75% of target strength */
    if((_ai_targets_strength[itr_targets] * 3 / 4) >= effective_stack_strength)
    {
        continue;                                                              // target too strong
    }

    delta_distance = Delta_XY_With_Wrap(
        _ai_targets_wx[itr_targets], _ai_targets_wy[itr_targets],
        stack_wx, stack_wy,
        WORLD_WIDTH
    );
    SETMIN(delta_distance, 1);                                                 // floor at 1
    target_weighted_value = (_ai_targets_value[itr_targets] / (delta_distance * 4));
    SETMIN(target_weighted_value, 1);                                          // floor at 1

    if(target_weighted_value > highest_target_value)
    {
        if(
            (last_move_failed == ST_FALSE)                                     // last unit moved fine ⇒ trust the path, skip the check
            || (Make_Move_Path(0, 0, 0, 0, 0, 0,
                               stack_wx, stack_wy,
                               _ai_targets_wx[itr_targets], _ai_targets_wy[itr_targets],
                               wp,
                               &movepath_x_array[2], &movepath_y_array[2], &movepath_cost_array[0],
                               1, 4,
                               list_unit_count, list_unit_count,
                               player_idx) > 0)                                // verify path exists
        )
        {
            highest_target_value = target_weighted_value;
            targets_idx = itr_targets;
        }
    }
}
```

**Intent:** linear scan over the target list. For each candidate:
1. **Skip filters** — `continue` if the target has been consumed (`value <= 0`) or is too strong (`strength * 3/4 >= effective_stack_strength`).
2. **Rating** — `value / (distance × 4)`, both `delta_distance` and `target_weighted_value` floored at 1 via the `SETMIN` macro. Closer + higher-value wins.
3. **Reachability** — either short-circuit (last unit's `Move_Failed` was FALSE → trust the prior result, skip path verification) OR call `Make_Move_Path` with all movement-mode flags zeroed and `UU_moves2 = 4` to verify a route exists.
4. **Track best** — overwrite `highest_target_value` and `targets_idx` if this candidate beat the prior best.

The gate semantics: "trust prior success, verify after failure." A unit/stack that hasn't experienced a `Move_Failed` this turn gets quick acceptance; one that has gets the expensive path check on every candidate.

### Note on the `Make_Move_Path` call

The call uses all six movement-mode flags as 0, `UU_flag = 1`, `UU_moves2 = 4`, and passes `list_unit_count` for both `boatrider_count` and `troop_count`. The MainScr.C caller (for player UI) passes `UU_moves2 = 30`. The 8× smaller value here means the AI's reachability test is effectively limited to short paths; targets that exist but require more move-steps to reach will fail the path check.

## Phase 4 — Write back / return (lines 2929-2941)

```c
if(targets_idx == ST_UNDEFINED)
{
    return_value = ST_FALSE;
}
else
{
    *target_wx = _ai_targets_wx[targets_idx];
    *target_wy = _ai_targets_wy[targets_idx];
    _ai_targets_value[targets_idx] = 0;  /* *consume* the chosen target */
    return_value = ST_TRUE;
}
return return_value;
```

**Intent:** if any candidate passed Phase 3, write its coords through the out-params, zero its value so subsequent calls (other stacks in the same landmass iteration) can't pick the same target, and return ST_TRUE. Otherwise return ST_FALSE.

The zero-the-value step is what couples each call to the next within the [`AI_Stacks_Roamers_Target_Or_Deploy`](AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md) Phase 2 per-stack loop — the target list shrinks (effectively) as stacks claim entries.

## Suspected bugs / open questions (need disassembly verification per BUG-B lesson)

### Suspected Bug 1 — Very small `UU_moves2` (4) in `Make_Move_Path` call (line 2913)

The MainScr.C reference shows player-side code passes `UU_moves2 = 30`. This AI call passes `4`. If `UU_moves2` is the search depth or move-step limit, the AI's reachability test is effectively limited to targets within 4 move-steps — much shorter than the player's 30. Targets just out of that horizon will fail the path check and be skipped, even if they'd be valuable picks.

Either the OG genuinely has a tight search for performance reasons or this is a translation typo. Worth disassembly verification.

## Call graph

```
AI_Set_Unit_Orders                                  [AIMOVE.c:131, Phase 4 dispatch]
  └─ per (plane, landmass):
       ├─ AI_Build_Target_List(player_idx, landmass_idx, wp)              [slot 8]
       │    └─ writes _ai_targets_count, _ai_targets_wx/wy/strength/value
       │
       └─ AI_Stacks_Roamers_Target_Or_Deploy(landmass_idx, wp, player_idx) [slot 9]
            └─ per roamer stack:
                 └─ AI_Stacks_Assign_Target(stack_wx, stack_wy, &tx, &ty, unit_list_idx, niu_list_unit_idx)   [THIS FUNCTION]
                      ├─ Phase 1: list_unit_count = _ai_own_stack_unit_count[unit_list_idx]
                      ├─ Phase 2: per stack unit:
                      │    └─ Effective_Unit_Strength(unit_idx)  →  effective_stack_strength
                      ├─ Phase 3: per target in _ai_targets_*:
                      │    ├─ filter on value/strength
                      │    ├─ Delta_XY_With_Wrap                  →  delta_distance
                      │    └─ Make_Move_Path(0,0,0,0,0,0, stack→target, ..., UU_moves2=4, ...)  →  reachability
                      └─ Phase 4: write *target_wx/wy, zero _ai_targets_value[picked], return ST_TRUE/FALSE
```

`AI_Stacks_Assign_Target` has exactly ONE caller: `AI_Stacks_Roamers_Target_Or_Deploy` at line 1833.

## Inputs / outputs

**Reads:**
- `_ai_own_stack_unit_count[unit_list_idx]`, `_ai_own_stack_unit_list[unit_list_idx][...]` — stack's unit roster
- `_UNITS[unit_idx]` fields: `.wp`, `.owner_idx`, `.Move_Failed`
- `Effective_Unit_Strength(unit_idx)` — per-unit combat strength
- `_ai_targets_count`, `_ai_targets_value[]`, `_ai_targets_strength[]`, `_ai_targets_wx[]`, `_ai_targets_wy[]`
- `movepath_*_array` (via `Make_Move_Path`)

**Writes:**
- `*target_wx`, `*target_wy` (out-params) — chosen target coords when ST_TRUE returned
- `_ai_targets_value[targets_idx] = 0` — "consumes" the picked target so other stacks can't double-claim it

**Returns:** `ST_TRUE` if a target was picked, `ST_FALSE` otherwise.

## Related references

- [AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md](AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md) — sole caller
- [AIMOVE-AI_Build_Target_List.md](AIMOVE-AI_Build_Target_List.md) — producer of the `_ai_targets_*` arrays this function reads
- [AIMOVE-AI_Set_Unit_Orders.md](AIMOVE-AI_Set_Unit_Orders.md) — top-level dispatcher
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
