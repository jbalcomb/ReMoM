AIMOVE-AI_Stacks_Order_Ferry.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Stacks_Order_Ferry

---

# AI_Stacks_Order_Ferry — Walkthrough

**Location:** [MoM/src/AIMOVE.c:4842](../../MoM/src/AIMOVE.c#L4842) (~7 lines, ends 4848).
**WZD overlay:** ovr158, p29
**drake178 name:** `AI_UNIT_SeekTransport()`

## Purpose

Set a single unit's status to `us_Ferry`, prime its `dst_wx` field with a "timer" value of 10, and consume its slot in the AI's per-stack unit list.

The unit will then be re-examined on subsequent turns by the `us_Ferry`-status handler (see [`AI_MoveUnits`](AIMOVE-AI_MoveUnits.md) dispatch) — that handler walks the seek-transport list ([`AI_SeekTransport_*`](AIMOVE-AI_Stacks_Ferry_Add_Location.md)) and tries to send the unit toward a transport pickup point.

## Signature

```c
void AI_Stacks_Order_Ferry(int16_t unit_idx, int16_t unit_list_idx, int16_t list_unit_idx)
```

| Param | Used for |
|---|---|
| `unit_idx` | Index into global `_UNITS[]` of the unit to receive the order |
| `unit_list_idx` | Index into `_ai_own_stack_unit_list[][]` — which AI-stack slot |
| `list_unit_idx` | Sub-index — which slot within the per-stack unit list |

Same shape as the other `AI_Order_*` helpers (`AI_Order_Settle`, `AI_Stacks_Order_RoadBuild`, `AI_Stacks_Order_Meld`, `AI_Stacks_Order_Purify`).

## Inputs (globals read)

| Source | Fields |
|---|---|
| `MAX_UNIT_COUNT` | Constant (1000 per [MoX/src/MOX_DEF.h:706](../../MoX/src/MOX_DEF.h#L706)) — upper bound for `unit_idx` validity check |

Defensive bounds check only — does not read any other state.

## Outputs (side effects)

If `unit_idx` is in `[0, MAX_UNIT_COUNT)`:
- `_UNITS[unit_idx].Status = us_Ferry;` (value `11` per [MoX/src/MOM_DAT.h:524](../../MoX/src/MOM_DAT.h#L524))
- `_UNITS[unit_idx].dst_wx = 10;` — **NOT a coordinate** (see below)
- `_ai_own_stack_unit_list[unit_list_idx][list_unit_idx] = ST_UNDEFINED;` — consume the stack slot (consume-by-sentinel idiom)

Otherwise: no-op (early return).

## Locals

None.

## Code walk ([lines 4842-4848](../../MoM/src/AIMOVE.c#L4842-L4848))

```c
void AI_Stacks_Order_Ferry(int16_t unit_idx, int16_t unit_list_idx, int16_t list_unit_idx)
{
    if((unit_idx < 0) || (unit_idx >= MAX_UNIT_COUNT)) { return; }
    _UNITS[unit_idx].Status = us_Ferry;
    _UNITS[unit_idx].dst_wx = 10;  /* DEDU  drake178: "sets ... a timer of 10" */
    _ai_own_stack_unit_list[unit_list_idx][list_unit_idx] = ST_UNDEFINED;
}
```

### Why `dst_wx = 10` is a "timer", not a coordinate

`dst_wx` normally stores the destination X coordinate (range `[0, WORLD_WIDTH)` = `[0, 60)`). For a unit with `Status = us_Ferry`, the field is **repurposed as a countdown timer** per drake178's annotation.

The mechanism (inferred — needs walkthrough of the `us_Ferry` handler to confirm): each turn the unit fails to find a transport, the timer is presumably decremented; when it reaches 0 the unit either gets re-ordered to a default goto or has its status cleared. The literal `10` here is the initial timer value (10 turns to find a transport before giving up).

The `dst_wy` field is **not** initialized — it presumably gets set later by the seek-transport handler when an actual destination is chosen, or it's just leftover from whatever was there before.

This field-overloading is a common DOS-era memory optimization (no spare bytes for a separate timer field, so reuse an existing one). Worth a code search for `_UNITS[].Status == us_Ferry` to confirm the timer-consumer.

### Bounds check

The `unit_idx < 0 || unit_idx >= MAX_UNIT_COUNT` guard is more defensive than typical for this codebase — most `AI_Order_*` helpers trust the caller. Possibly added because ferry-order is the only `AI_Order_*` that's invoked from contexts where `unit_idx` could be `ST_UNDEFINED` (= `-1`), e.g., from [`AI_Stacks_Setup_Ferry`](AIMOVE-AI_Stacks_Setup_Ferry.md) Phase 7b where the caller doesn't pre-check the slot for `ST_UNDEFINED` (though that path is itself unreachable).

`MAX_UNIT_COUNT` is `1000` per [MoX/src/MOX_DEF.h:706](../../MoX/src/MOX_DEF.h#L706). Note this is distinct from `UNIT_COUNT_MAX` (= 1009, [MoM/src/MoM_DEF.h:89](../../MoX/src/MOM_DEF.h#L89)) — the codebase has both constants, with different values. This function uses the smaller bound.

## Callers

Three call sites:

1. **[AI_Stacks_Ocean_Landmass_Orders at line 1490](../../MoM/src/AIMOVE.c#L1490)** — when iterating own units and finding one with `_unit_type_table[unit_type].Transport > 0`, orders that transport unit to seek ferry. This is structurally odd (a transport ordering itself to seek ferry) — worth flagging as a suspect during walkthrough of `AI_Stacks_Ocean_Landmass_Orders`. Possibly the intent is "this is the transport we want OTHERS to seek; mark it so we don't reprocess it this pass."

2. **[AI_Stacks_Setup_Ferry at line 2261](../../MoM/src/AIMOVE.c#L2261)** — inside Phase 7b (the no-nearby-ocean branch), which is unreachable due to the inverted `if(total_boat_capacity > 0)` gate. So in practice this caller never fires under current data conditions. See [AIMOVE-AI_Stacks_Setup_Ferry.md](AIMOVE-AI_Stacks_Setup_Ferry.md) (bug B12 in that doc) for details on why.

3. **[AI_Stacks_Reorder_Settle_Elsewhere at line 5343](../../MoM/src/AIMOVE.c#L5343)** — when an adjacent ocean transport is NOT found for a colonizing settler/engineer, register a ferry-pickup request at the adjacent ocean square AND order the unit to ferry. Gated by `if(_ai_ferry_count < MAX_AI_FERRIES)` — note this caller pre-checks the ferry-list capacity before appending, paralleling the guard inside [`AI_Stacks_Ferry_Add_Location`](AIMOVE-AI_Stacks_Ferry_Add_Location.md) itself.

## ASCII summary

```
AI_Stacks_Order_Ferry(unit_idx, unit_list_idx, list_unit_idx)
  ├─ if unit_idx out of [0, MAX_UNIT_COUNT) → return
  ├─ _UNITS[unit_idx].Status = us_Ferry
  ├─ _UNITS[unit_idx].dst_wx = 10            (timer, not a coord)
  └─ _ai_own_stack_unit_list[unit_list_idx][list_unit_idx] = ST_UNDEFINED
```

## Related references

- [AIMOVE-AI_Stacks_Relocate_Roamers.md](AIMOVE-AI_Stacks_Relocate_Roamers.md) — entry-point of the chain that includes this function
- [AIMOVE-AI_Stacks_Setup_Ferry.md](AIMOVE-AI_Stacks_Setup_Ferry.md) — one of three callers (in dead code path per drake178)
- [AIMOVE-AI_Stacks_Ferry_Add_Location.md](AIMOVE-AI_Stacks_Ferry_Add_Location.md) — sibling helper that registers a pickup-point request; this function sets a unit's status, that one writes the global request list
- [MoX/src/MOM_DAT.h:524](../../MoX/src/MOM_DAT.h#L524) — `us_Ferry` enum value
- [MoX/src/MOX_DEF.h:706](../../MoX/src/MOX_DEF.h#L706) — `MAX_UNIT_COUNT` (1000) — the bound used by this function
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
