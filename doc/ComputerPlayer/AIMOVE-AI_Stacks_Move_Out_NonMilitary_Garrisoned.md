AIMOVE-AI_Stacks_Move_Out_NonMilitary_Garrisoned.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Stacks_Move_Out_NonMilitary_Garrisoned



---

# AI_Stacks_Move_Out_NonMilitary_Garrisoned — Walkthrough

**Location:** [MoM/src/AIMOVE.c:3558](../../MoM/src/AIMOVE.c#L3558) (~84 lines, ends 3641).
**WZD overlay:** ovr158, p19
**drake178 name:** `AI_GarrBuilderPush()`

## Purpose

**Pushes non-combat units out of garrisons.** Settlers, Engineers (Construction > 0), and Melders parked inside a Garrison or FortressGarrison stack get ordered to walk to an adjacent free square so they can do their actual jobs (found new outposts, build roads, meld nodes) instead of warming garrison floors.

Three categories of unit get pushed:
- **Settlers** — units with `UA_CREATEOUTPOST` ability
- **Engineers** — units with `_unit_type_table[type].Construction > 0`
- **Melders** — units with `UA_MELD` ability, **but only if not already on a magic node square** on this plane

Runs as dispatch slot 2 of [`AI_Set_Unit_Orders`](AIMOVE-AI_Set_Unit_Orders.md) Phase 4 — immediately after [`AI_Stacks_Init_Build_Target_Order`](AIMOVE-AI_Stacks_Init_Build_Target_Order.md) populates the `_ai_own_stack_*` arrays.

## Signature

```c
void AI_Stacks_Move_Out_NonMilitary_Garrisoned(int16_t wp)
```

| Param | Used for |
|---|---|
| `wp` | Plane filter for the melder-on-node check (matches `_NODES[].wp`). NOT used for stack iteration — the function walks all `_ai_own_stack_*` entries regardless of plane, relying on the caller having populated those arrays plane-scoped via `AI_Stacks_Init_Build_Target_Order`. |

Called from [`AI_Set_Unit_Orders`](AIMOVE-AI_Set_Unit_Orders.md) at [line 177](../../MoM/src/AIMOVE.c#L177) and [line 307](../../MoM/src/AIMOVE.c#L307) (the two dispatch paths through the (plane, landmass) loop).

## Inputs (globals read)

| Source | Fields |
|---|---|
| `_ai_own_stack_count` | Loop bound |
| `_ai_own_stack_type[s]` | Stack-type filter (only `AISTK_Garrison` / `AISTK_FortressGarrison`) |
| `_ai_own_stack_unit_count[s]` | Inner loop bound |
| `_ai_own_stack_unit_list[s][u]` | Per-stack unit index |
| `_ai_own_stack_wx[s]`, `_ai_own_stack_wy[s]`, `_ai_own_stack_wp[s]` | Stack location (passed to `Adjacent_Free_Square`) |
| `_UNITS[unit_idx].type` | Lookup key into `_unit_type_table` |
| `_UNITS[unit_idx].wx`, `.wy` | Melder location (for node check) |
| `_unit_type_table[type].Abilities` | `UA_CREATEOUTPOST` (0x0020), `UA_MELD` (0x0400) bit tests |
| `_unit_type_table[type].Construction` | Engineer detection (`> 0`) |
| `_NODES[].wx`, `.wy`, `.wp` | Node-location lookup, all `NUM_NODES` entries scanned |

## Outputs (side effects)

- For each pushed unit, calls [`AI_Stacks_Order_Attack_Target_Or_Goto_Destination`](../../MoM/src/AIMOVE.c) with `(unit_idx, adjacent_wx, adjacent_wy, itr_stacks, itr_stack_units)`.
  - The order-setter writes `_UNITS[unit_idx].Status` (`us_Move` if target square has SITE/STRENGTH eval bits; otherwise `us_GOTO`) and `_UNITS[unit_idx].dst_wx/wy`.
  - It also consumes the `_ai_own_stack_unit_list[s][u]` slot by writing `ST_UNDEFINED` (the consume-by-sentinel idiom) — meaning subsequent slot-2..slot-14 consumers in the same dispatch round will skip this slot.
- Sets `g_ai_set_target_caller = 12` (settler/engineer push at [line 3601](../../MoM/src/AIMOVE.c#L3601)) or `13` (melder push at [line 3634](../../MoM/src/AIMOVE.c#L3634)) for the debug-instrumentation trail.

## Locals

```c
int16_t itr_stacks = 0;              // outer loop iterator (_ai_own_stack_count)
int16_t itr_stack_units = 0;         // inner loop iterator (per-stack unit slots)
int16_t stack_unit_count = 0;        // _ai_own_stack_unit_count[itr_stacks] cache
int16_t unit_idx = 0;                // _ai_own_stack_unit_list[itr_stacks][itr_stack_units] cache
int16_t unit_type = 0;               // _UNITS[unit_idx].type cache
int16_t adjacent_wx = 0;             // out from Adjacent_Free_Square
int16_t adjacent_wy = 0;             // out from Adjacent_Free_Square
int16_t unit_wx = 0;                 // _UNITS[unit_idx].wx cache (melder branch)
int16_t unit_wy = 0;                 // _UNITS[unit_idx].wy cache (melder branch)
int16_t unit_is_on_node_square = 0;  // ST_FALSE/ST_TRUE flag
int16_t itr_nodes = 0;               // _NODES[] iterator (melder branch)
```

## Code walk

### Outer loop — per-stack filter ([lines 3574-3579](../../MoM/src/AIMOVE.c#L3574-L3579))

```c
for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
{
    if(_ai_own_stack_type[itr_stacks] < AISTK_Garrison)  /* Site Garrison or Fortress City Garrison */
    {
        continue;
    }
    ...
```

`AISTK_Garrison = 3` and `AISTK_FortressGarrison = 4` per [MoX/src/MOM_DAT.h:102-107](../../MoX/src/MOM_DAT.h#L102-L107). Skipping `< AISTK_Garrison` excludes `AISTK_Unknown` (0), `AISTK_Roamer` (1), and `AISTK_DNE` (2). Only stacks that are physically sitting on a city/site/fortress square get processed — the rationale being that roamers don't need to be "pushed out" of anywhere; they're already mobile.

### Inner loop — per-unit slot ([lines 3580-3588](../../MoM/src/AIMOVE.c#L3580-L3588))

```c
stack_unit_count = _ai_own_stack_unit_count[itr_stacks];
for(itr_stack_units = 0; itr_stack_units < stack_unit_count; itr_stack_units++)
{
    unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_stack_units];
    if(unit_idx == ST_UNDEFINED)
    {
        continue;
    }
    unit_type = _UNITS[unit_idx].type;
    ...
```

The `ST_UNDEFINED` check honors the consume-by-sentinel idiom: any slot already cleared by a prior order-setter (within this same per-(plane, landmass) dispatch round) is skipped. Since `AI_Stacks_Move_Out_NonMilitary_Garrisoned` is dispatch slot 2 — running right after slot 1's `AI_Stacks_Init_Build_Target_Order` rebuilds the arrays — in practice no slots are consumed yet on entry, but the gate is harmless and correct.

### Branch A — Settlers/Engineers ([lines 3590-3604](../../MoM/src/AIMOVE.c#L3590-L3604))

```c
if(
    ((_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) != 0)
    ||
    (_unit_type_table[unit_type].Construction > 0)
)
{
    if(Adjacent_Free_Square(_ai_own_stack_wx[itr_stacks], _ai_own_stack_wy[itr_stacks], _ai_own_stack_wp[itr_stacks], &adjacent_wx, &adjacent_wy) == ST_TRUE)
    {
        g_ai_set_target_caller = 12;
        AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, adjacent_wx, adjacent_wy, itr_stacks, itr_stack_units);
    }
}
```

Note that the disjunction is **inclusive** — a unit with both `UA_CREATEOUTPOST` AND `Construction > 0` is treated the same as either alone. In practice no MoM unit type has both flags set, so the case never triggers; the `||` is structurally redundant.

`Adjacent_Free_Square` returns the first free 8-neighbor square; if it returns `ST_FALSE` (no adjacent free square), the unit is silently left in the garrison — no fallback, no error. The unit just doesn't get pushed this turn.

### Branch B — Melders ([lines 3605-3638](../../MoM/src/AIMOVE.c#L3605-L3638))

```c
else if((_unit_type_table[unit_type].Abilities & UA_MELD) != 0)
{
    unit_wx = _UNITS[unit_idx].wx;
    unit_wy = _UNITS[unit_idx].wy;
    unit_is_on_node_square = ST_FALSE;
    for(itr_nodes = 0; itr_nodes < NUM_NODES; itr_nodes++)
    {
        /* ¿ OGBUG  should check if Nodes is owned ? */
        if(
            (_NODES[itr_nodes].wx == unit_wx)
            &&
            (_NODES[itr_nodes].wy == unit_wy)
            &&
            (_NODES[itr_nodes].wp == wp)
        )
        {
            unit_is_on_node_square = ST_TRUE;
            break;
        }
    }
    if(unit_is_on_node_square == ST_FALSE)
    {
        if(Adjacent_Free_Square(...) == ST_TRUE)
        {
            g_ai_set_target_caller = 13;
            AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, adjacent_wx, adjacent_wy, itr_stacks, itr_stack_units);
        }
    }
}
```

**Suspected OGBUG — node-ownership not checked.** The node-presence check matches purely on `(wx, wy, wp)` coordinates without consulting `_NODES[itr_nodes].owner_idx`. A Magic Spirit sitting on a node owned by a DIFFERENT wizard would still be treated as "on a node square" and left alone — but it isn't actually melding (someone else owns it). The realistic case for this is narrow (you'd usually fight whoever owns the node before parking your Spirit there), but the check arguably ought to gate on ownership too. Marked in source with `/* ¿ OGBUG  should check if Nodes is owned ? */` at [line 3612](../../MoM/src/AIMOVE.c#L3612). Per the OG-faithfulness rule the code is left as-is.

The `else if` means melders that are *also* settlers/engineers fall into Branch A (and never reach Branch B). In practice no unit type has both — Magic/Guardian Spirits are pure melders — so the elif is just a structural choice, not a behavioral one.

The node-presence check uses the `wp` parameter (not the stack's `_ai_own_stack_wp[itr_stacks]`). Since `AI_Stacks_Init_Build_Target_Order` is plane-scoped per call (Phase 2 filters `_UNITS[i].wp != wp continue`), `_ai_own_stack_wp[s]` should equal `wp` for every populated entry, so the two values are interchangeable here. Using the parameter is slightly cheaper (no array indirection).

**Why the node check is melder-only:** Magic Spirits parked on top of an unmelded node are doing exactly what they should be doing — finishing the meld. Pushing them off would interrupt that. Settlers and Engineers have no equivalent "in-progress" state worth detecting.

**Melder destination is not node-aware.** When a Spirit IS pushed out (Spirit in a garrison, no node on its square), the destination is whatever `Adjacent_Free_Square` returns first — a generic neighbor, not chosen for proximity to an unmelded node. The Spirit walks one square and then waits for next turn's slot-4 `AI_Stacks_Do_Meld` pass to assign it a proper meld target. So this function only handles the "extract from garrison" half; it doesn't try to do `AI_Stacks_Do_Meld`'s job.

## `Adjacent_Free_Square` filter semantics

[`Adjacent_Free_Square`](../../MoM/src/AIMOVE.c#L6019) builds a 3×3 area centered on the stack's square, marks the center as not-free, and excludes squares where any of the following are true:

| Filter | Source |
|---|---|
| Not land (`Square_Is_Land == ST_FALSE`) | terrain |
| Occupied by any unit on the same plane (any owner) | `_UNITS[]` |
| Holds an intact lair | `_LAIRS[]` (intact only) |
| Holds a city | `_CITIES[]` |

It does **not** filter `_NODES[]`. A melder could legitimately be pushed onto a node square — harmless, since next turn's `AI_Stacks_Do_Meld` pass would target it for melding anyway. Towers, if they live in `_LAIRS[]`, are filtered via the intact-lair check.

Practical consequence: a settler will NEVER be pushed onto water, onto a hostile lair, onto a city, or onto a stack — so the `us_Move`-classification edge case (where the order-setter would see SITE/STRENGTH bits at the destination and Status the unit as attacking) does not arise from this function's pushes.

## ASCII summary

```
AI_Stacks_Move_Out_NonMilitary_Garrisoned(wp)
  └─ for s in 0.._ai_own_stack_count:
       └─ if type[s] >= AISTK_Garrison (Garrison or FortressGarrison):
            └─ for u in 0..unit_count[s]:
                 ├─ if unit_list[s][u] == ST_UNDEFINED → skip
                 ├─ Settler / Engineer:
                 │    └─ Adjacent_Free_Square found → AI_Stacks_Order_Attack_Target_Or_Goto_Destination [caller=12]
                 └─ Melder:
                      └─ not standing on a node on plane wp:
                           └─ Adjacent_Free_Square found → AI_Stacks_Order_Attack_Target_Or_Goto_Destination [caller=13]
```

## Position in the dispatch chain

| Slot | Function | Notes |
|---|---|---|
| 1 | [`AI_Stacks_Init_Build_Target_Order`](AIMOVE-AI_Stacks_Init_Build_Target_Order.md) | Rebuilds `_ai_own_stack_*` from scratch for this (plane, landmass). |
| **2** | **`AI_Stacks_Move_Out_NonMilitary_Garrisoned`** | **(this function)** — push builders out of garrisons |
| 3 | `AI_Stacks_Survey_Expedition_Forces` | Score surplus combat units into the `G_Pushout_*` / `G_Seafaring_*` global pools |
| 4 | `AI_Stacks_Do_Meld` | Issue meld orders |
| 5 | `AI_Stacks_Do_Settle` | Issue settle orders |
| 6 | `AI_Stacks_Do_Purify` | Issue purify orders |
| 7 | `AI_Stacks_Do_RoadBuild` | Issue road-build orders |
| 8 | `AI_Build_Target_List` | Build `_ai_targets_*` (separate from stack arrays) |
| 9 | [`AI_Stacks_Roamers_Target_Or_Deploy`](AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md) | Assign targets to roamer stacks |
| 10-14 | stage/garrison fill | Consume excess pool, top up garrisons |

By running before slots 4-7 (the per-job order-setters), `AI_Stacks_Move_Out_NonMilitary_Garrisoned` gets first crack at builders that happen to be parked in garrisons. Once it consumes a unit slot (via the order-setter's `ST_UNDEFINED` write), the same unit won't be re-considered by `AI_Stacks_Do_Settle` / `AI_Stacks_Do_RoadBuild` / `AI_Stacks_Do_Meld` later in the same dispatch round — those consumers all read `_ai_own_stack_unit_list[s][u]` and skip undefined slots.

## Related references

- [AIMOVE-AI_Set_Unit_Orders.md](AIMOVE-AI_Set_Unit_Orders.md) — dispatcher; this function is slot 2
- [AIMOVE-AI_Stacks_Init_Build_Target_Order.md](AIMOVE-AI_Stacks_Init_Build_Target_Order.md) — producer of the `_ai_own_stack_*` arrays this function reads
- [AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md](AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md) — "Note on sparse `_ai_own_stack_unit_list[][]`" for the consume-by-sentinel idiom
- [MoX/src/MOM_DAT.h:102-107](../../MoX/src/MOM_DAT.h#L102-L107) — `AISTK_*` enum values
- [MoX/src/MOM_DEF.h:687-703](../../MoX/src/MOM_DEF.h#L687-L703) — `UA_*` unit-ability flag bits
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
