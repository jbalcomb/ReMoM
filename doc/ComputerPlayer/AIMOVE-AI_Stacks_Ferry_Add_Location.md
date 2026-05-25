AIMOVE-AI_Stacks_Ferry_Add_Location.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Stacks_Ferry_Add_Location

---

# AI_Stacks_Ferry_Add_Location — Walkthrough

**Location:** [MoM/src/AIMOVE.c:5151](../../MoM/src/AIMOVE.c#L5151) (~29 lines, ends 5179).
**WZD overlay:** ovr158, p36
**drake178 name:** `AI_SeekTransportFrom()`

## Purpose

Add a `(wx, wy, wp)` coordinate triple to the global **seek-transport request list** (`_ai_ferry_wx_array/Ys/Ps` parallel arrays), but only if the triple isn't already present. Effectively "register that the AI wants a transport to show up at this location."

The seek-transport list is consumed later by [`AI_Stacks_Ocean_Landmass_Orders`](AIMOVE-AI_Stacks_Ocean_Landmass_Orders.md) (slot 5 in the next turn's dispatch, [AIMOVE.c:1074, 1201, 1207](../../MoM/src/AIMOVE.c#L1074)) — that pass directs idle transport stacks to the registered pickup points.

Called only from [`AI_Stacks_Setup_Ferry`](AIMOVE-AI_Stacks_Setup_Ferry.md) at [line 2089](../../MoM/src/AIMOVE.c#L2089), after that function locates an ocean tile near a rallying stack.

## Signature

```c
void AI_Stacks_Ferry_Add_Location(int16_t wx, int16_t wy, int16_t wp)
```

| Param | Used for |
|---|---|
| `wx`, `wy`, `wp` | The coordinates where a transport pickup is wanted (typically an ocean square adjacent to a land stack that needs ferrying) |

## Inputs (globals read)

| Source | Fields |
|---|---|
| `_ai_ferry_count` | Current list length (loop bound for the dedup scan) |
| `_ai_ferry_wx_array[i]`, `_ai_ferry_wy_array[i]`, `_ai_ferry_wp_array[i]` | Existing entries to compare against (dedup) |

Array capacity: all three are `int16_t [15]` per [MoX/src/MOM_DAT.h:2922-2928](../../MoX/src/MOM_DAT.h#L2922-L2928) and [MoX/src/MOM_DAT.c:965-971](../../MoX/src/MOM_DAT.c#L965-L971). The same bound is also exposed as `MAX_AI_FERRIES` (= 15) per [MoX/src/MOM_DEF.h:161](../../MoX/src/MOM_DEF.h#L161).

## Outputs (side effects)

If `(wx, wy, wp)` is not already in the list:
- `_ai_ferry_wx_array[_ai_ferry_count] = wx;`
- `_ai_ferry_wy_array[_ai_ferry_count] = wy;`
- `_ai_ferry_wp_array[_ai_ferry_count] = wp;`
- `_ai_ferry_count++;`

Otherwise: no-op (early return).

## Locals

```c
int16_t itr = 0;   // dedup-scan iterator
```

## Code walk ([lines 5151-5179](../../MoM/src/AIMOVE.c#L5151-L5179))

```c
void AI_Stacks_Ferry_Add_Location(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t itr = 0;
    /* Search for existing entry */
    for(itr = 0; itr < _ai_ferry_count; itr++)
    {
        if(
            (_ai_ferry_wx_array[itr] == wx)
            &&
            (_ai_ferry_wy_array[itr] == wy)
            &&
            (_ai_ferry_wp_array[itr] == wp)
        )
        {
            /* Found: do nothing and return. */
            return;
        }
    }
    if(_ai_ferry_count >= MAX_AI_FERRIES)
    {
        return;
    }
    _ai_ferry_wx_array[_ai_ferry_count] = wx;
    _ai_ferry_wy_array[_ai_ferry_count] = wy;
    _ai_ferry_wp_array[_ai_ferry_count] = wp;
    _ai_ferry_count++;
}
```

Three-step structure:

1. **Dedup scan** — linear search for an existing match on `(wx, wy, wp)`. If found, early return (no duplicate entry).
2. **Capacity guard** — refuse to append if the list is already at `MAX_AI_FERRIES` (= 15). Silent drop — no error reporting; the caller has no way to know the request was rejected.
3. **Append** — write the triple and increment `_ai_ferry_count`.

### Counter-reset note

This function's writes accumulate within a single turn. `_ai_ferry_count` gets reset at the start of each AI turn — see [AIMOVE.c:139](../../MoM/src/AIMOVE.c#L139) and [AIMOVE.c:254](../../MoM/src/AIMOVE.c#L254) (the two dispatch paths through `AI_Set_Unit_Orders`). So requests don't bleed across turns; each turn's seek list is built from scratch.

## ASCII summary

```
AI_Stacks_Ferry_Add_Location(wx, wy, wp)
  ├─ for i in 0.._ai_ferry_count:
  │    └─ if matches → return (dedup)
  ├─ if _ai_ferry_count >= MAX_AI_FERRIES → return (silent capacity drop)
  └─ append (wx, wy, wp) to _ai_ferry_w?_array, ++_ai_ferry_count
```

## Other callers

- [`AIMOVE.c:5254`](../../MoM/src/AIMOVE.c#L5254) — inside [`AI_SendToColonize__WIP`](../../MoM/src/AIMOVE.c), a direct write to the same arrays (not through this function), pre-gated with the same `if(_ai_ferry_count < 15)` capacity check. Different path to the same global state — could conceivably be refactored to call `AI_Stacks_Ferry_Add_Location` instead, but currently inlines the append.

## Related references

- [AIMOVE-AI_Stacks_Relocate_Roamers.md](AIMOVE-AI_Stacks_Relocate_Roamers.md) — entry-point that drives this chain
- [AIMOVE-AI_Stacks_Setup_Ferry.md](AIMOVE-AI_Stacks_Setup_Ferry.md) — sole caller of this function
- [AIMOVE-AI_Stacks_Ocean_Landmass_Orders.md](AIMOVE-AI_Stacks_Ocean_Landmass_Orders.md) — consumer of the seek-transport list on the next turn
- [MoX/src/MOM_DAT.h:2922-2928](../../MoX/src/MOM_DAT.h#L2922-L2928) — declarations for `_ai_ferry_count` and the three parallel arrays
- [MoX/src/MOM_DEF.h:161](../../MoX/src/MOM_DEF.h#L161) — `MAX_AI_FERRIES` constant
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
