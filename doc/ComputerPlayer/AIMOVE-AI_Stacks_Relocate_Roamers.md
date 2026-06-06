AIMOVE-AI_Stacks_Relocate_Roamers.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\G_AI_HomeRallyFill__WIP.c

---

# AI_Stacks_Relocate_Roamers — Walkthrough

**Location:** [MoM/src/AIMOVE.c:1875](../../MoM/src/AIMOVE.c#L1875) (~47 lines, ends 1921).
**WZD overlay:** ovr158, p07
**drake178 name:** `G_AI_HomeRallyFill()`

## Purpose

For a single (player, plane, landmass), set up ferry-departure for every `AISTK_Roamer` stack on the landmass — driver function that iterates the stacks and delegates per-stack work to [`AI_Stacks_Setup_Ferry`](AIMOVE-AI_Stacks_Setup_Ferry.md).

The gate ([AIMOVE.c:200-204](../../MoM/src/AIMOVE.c#L200-L204)) only invokes this function for landmasses where moving roamers OFF makes sense: `lmt_Leaveable` (explicit evacuate), `lmt_NoTargets` (no local work), and `lmt_Own` (securely owned, spare forces available). In every case the implicit intent is: prepare these roamers to be transported elsewhere.

The "Relocate_Roamers" name reflects that intent — not "Home stage," not "Fill" — the function selects roamers for ferry-setup and provides the per-stack delegate with a stage-point fill budget.

Runs as dispatch slot 11 of [`AI_Set_Unit_Orders`](AIMOVE-AI_Set_Unit_Orders.md), immediately after slot 10 [`AI_Stacks_Order_To_War_Landmass`](AIMOVE-AI_Stacks_Order_To_War_Landmass.md).

## Signature

```c
void AI_Stacks_Relocate_Roamers(int16_t landmass_idx, int16_t wp, int16_t player_idx)
```

Called from [`AI_Set_Unit_Orders`](AIMOVE-AI_Set_Unit_Orders.md) at [line 206](../../MoM/src/AIMOVE.c#L206) and [line 310](../../MoM/src/AIMOVE.c#L310) (the two dispatch paths). The gate accepts three landmass types:

```c
if(
    (cp_landmass_type_array[landmass_idx] >= lmt_Leaveable)  // lmt_Leaveable, lmt_NoTargets
    || (cp_landmass_type_array[landmass_idx] == lmt_Own)
)
{
    AI_Stacks_Relocate_Roamers(landmass_idx, wp, player_idx);
}
```

| Gate-matching type | What it means | Why relocate roamers? |
|---|---|---|
| `lmt_Leaveable` | `AI_Stacks_Roamers_Target_Or_Deploy` set this — no roamer targets here, war landmass set elsewhere | Explicit evacuation in progress |
| `lmt_NoTargets` | No military targets on this landmass | Idle roamers; send where they can act |
| `lmt_Own` | Securely owned (own-strength ≥ 10× enemy) | Spare forces; can support elsewhere |

Specifically **NOT** called on `lmt_Contested`, `lmt_NoOwnCity`, or `lmt_NoOwnCityAndAllyHasCity` — those landmasses have their own mechanisms (e.g., contested landmasses get garrison fills via slot 14).

## Inputs (globals read)

| Source | Fields |
|---|---|
| `_ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx]`, `.wy_array[...]` | Stage point coordinates |
| `_ai_own_stack_count` | Outer-loop bound |
| `_ai_own_stack_wx[s]`, `_ai_own_stack_wy[s]` | Stack position (to detect "stack is at stage point") |
| `_ai_own_stack_unit_count[s]` | Used to compute remaining capacity at stage point and to decrement the per-stack budget |
| `_ai_own_stack_type[s]` | Filter — only `AISTK_Roamer` stacks |
| `_ai_own_stack_unit_list[s][0]` | Filter — only stacks whose first slot is not `ST_UNDEFINED` |
| `MAX_STACK` | Capacity for stage-point slot calculation |

## Outputs (side effects)

Does not directly modify `_UNITS[]` — all unit-level side effects happen inside [`AI_Stacks_Setup_Ferry`](AIMOVE-AI_Stacks_Setup_Ferry.md). For each qualifying roamer stack, this function calls the helper with `stage_count_slot_count` as the "how many more units could legally fit at the stage point" budget.

## Locals

```c
int16_t stage_wx = 0;                // _ai_continents.[...].wx_array[landmass_idx] cache
int16_t stage_wy = 0;                // _ai_continents.[...].wy_array[landmass_idx] cache
int16_t stack_wx = 0;                // _ai_own_stack_wx[itr_stacks] cache (per-iter)
int16_t stack_wy = 0;                // _ai_own_stack_wy[itr_stacks] cache (per-iter)
int16_t itr_stacks = 0;              // outer loop iterator (used by both Phase 2 and Phase 3)
int16_t stage_count_slot_count = 0;  // running budget — open slots at stage square
```

`stage_count_slot_count` is computed in Phase 2 and decremented in Phase 3 per processed stack. It's passed by value to [`AI_Stacks_Setup_Ferry`](AIMOVE-AI_Stacks_Setup_Ferry.md), which uses it to cap the number of goto-stage orders it issues for one stack.

## Code walk

### Phase 1 — Cache stage point ([lines 1884-1886](../../MoM/src/AIMOVE.c#L1884-L1886))

```c
/* Phase 1: Init */
stage_wx = _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx];
stage_wy = _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx];
```

Read the stage point coordinates from the continent table once. These get passed to the per-stack helper.

### Phase 2 — Count open stage-point slots ([lines 1888-1902](../../MoM/src/AIMOVE.c#L1888-L1902))

```c
/* Phase 2: Count of Open Stack Slots at Landmass Stage-Point */
stage_count_slot_count = 0;
for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
{
    stack_wx = _ai_own_stack_wx[itr_stacks];
    stack_wy = _ai_own_stack_wy[itr_stacks];
    if(
        (stack_wx == stage_wx)
        &&
        (stack_wy == stage_wy)
    )
    {
        stage_count_slot_count = (MAX_STACK - _ai_own_stack_unit_count[itr_stacks]);
    }
}
```

Look for a stack currently AT the stage point. If found, `stage_count_slot_count` becomes the remaining capacity at that square (`MAX_STACK - present_count`). The budget is then passed to each Phase 3 helper call to cap how many goto-stage orders get issued.

Notable quirks:

- **Last-write-wins.** If multiple stacks share the stage-point square (shouldn't happen — `_ai_own_stack_*` coalesces by coordinate — but if it did), the last-iterated wins. In normal data this is single-assignment.
- **No early-out.** The loop scans all stacks even after finding one at the stage point.
- **No plane check.** `_ai_own_stack_*` is plane-scoped by [`AI_Stacks_Init_Build_Target_Order`](AIMOVE-AI_Stacks_Init_Build_Target_Order.md), so all entries already have `wp` matching. Comparing only `wx/wy` is safe in context.
- **Suspected OG bug — `stage_count_slot_count` defaults to 0 when no stack is at the stage point.** If no stack occupies the stage square, the budget stays at 0 → every Phase 3 helper call gets `stage_count_slot_count = 0` → [`AI_Stacks_Setup_Ferry`](AIMOVE-AI_Stacks_Setup_Ferry.md)'s Phase 4 loop short-circuits immediately (`stage_count_slot_count <= 0` break). No goto-stage orders get issued for the empty-stage-point case, even though the stage square could legitimately fit `MAX_STACK` (9) new units. The reasonable expectation would be initializing to `MAX_STACK` for the empty-stage case. Preserved as-is per faithful-reconstruction.

### Phase 3 — Setup ferry for roamer stacks ([lines 1904-1919](../../MoM/src/AIMOVE.c#L1904-L1919))

```c
/* Phase 3: Setup Ferry for Valid, Roamer Stacks */
for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
{
    if(_ai_own_stack_type[itr_stacks] != AISTK_Roamer)
    {
        continue;
    }
    if(_ai_own_stack_unit_list[itr_stacks][0] == ST_UNDEFINED)
    {
        continue;
    }
    /* NOTE: OON XREF of AI_Stacks_Setup_Ferry() ~ per stack portion of parent function */
    AI_Stacks_Setup_Ferry(itr_stacks, landmass_idx, wp, stage_wx, stage_wy, stage_count_slot_count, player_idx);
    /* Deduct the units in this stack from the destination's projected available space */
    stage_count_slot_count -= _ai_own_stack_unit_count[itr_stacks];
}
```

Two early-continue filters:

1. **Type filter** — only `AISTK_Roamer` stacks proceed. `AISTK_Garrison` / `AISTK_FortressGarrison` stay parked on their site/city; `AISTK_Unknown` and `AISTK_DNE` are also skipped (the filter is a strict `!= AISTK_Roamer`).
2. **Lead-slot filter** — `_ai_own_stack_unit_list[itr_stacks][0] != ST_UNDEFINED`. Cheap "has anything left to dispatch" test — if the lead unit slot got consumed by an earlier dispatch slot (4-10), the whole stack is skipped. The check only inspects slot 0; a stack with slot 0 consumed but later slots intact will still be skipped.

For each matching stack, call [`AI_Stacks_Setup_Ferry`](AIMOVE-AI_Stacks_Setup_Ferry.md) with the current `stage_count_slot_count` budget, then **unconditionally decrement** that budget by the stack's full unit count.

Notable quirks:

- **Unconditional decrement.** `stage_count_slot_count -= _ai_own_stack_unit_count[itr_stacks]` happens whether or not the helper actually issued any orders. So the budget runs negative after the first sufficiently-large stack is processed — terminating the helper's Phase 4 goto-stage loop for all subsequent stacks. The arithmetic is loose: it assumes each stack's full unit count will be ordered, but the helper caps internally at the budget anyway.
- **No stage-point exclusion.** A stack AT the stage point still gets passed to the helper, which then does its own `stack_wx != stage_wx || stack_wy != stage_wy` check and skips the goto-stage phase. Wasted CPU on the embark / ferry-request phases that still run, but not incorrect.
- **OON XREF marker** at [line 1915](../../MoM/src/AIMOVE.c#L1915): the inline comment formally identifies `AI_Stacks_Setup_Ferry` as the "per stack portion of parent function" — structurally, the helper is the inner loop body that was extracted into its own function (probably for readability and stack-frame separation given the helper's 200+ line bulk).

## ASCII summary

```
AI_Stacks_Relocate_Roamers(landmass_idx, wp, player_idx)
  ├─ Phase 1: cache stage_wx/wy from _ai_continents
  ├─ Phase 2: scan _ai_own_stack for stack AT stage → stage_count_slot_count = open slots (else 0)
  └─ Phase 3: for s in 0.._ai_own_stack_count:
       ├─ if stack[s] is not Roamer → skip
       ├─ if stack[s] lead slot is ST_UNDEFINED → skip
       ├─ AI_Stacks_Setup_Ferry(s, landmass_idx, wp, stage_wx, stage_wy, stage_count_slot_count, player_idx)
       └─ stage_count_slot_count -= unit_count[s]   (unconditional; can go negative)
```

## Position in the dispatch chain

| Slot | Function | Notes |
|---|---|---|
| 1 | [`AI_Stacks_Init_Build_Target_Order`](AIMOVE-AI_Stacks_Init_Build_Target_Order.md) | Rebuilds `_ai_own_stack_*` |
| 2 | [`AI_Stacks_Move_Out_NonMilitary_Garrisoned`](AIMOVE-AI_Stacks_Move_Out_NonMilitary_Garrisoned.md) | Push builders out of garrisons |
| 3 | `AI_Stacks_Survey_Expedition_Forces` | Score surplus combat units into pushout pools |
| 4-7 | `AI_Stacks_Do_Meld` / `AI_Stacks_Do_Settle` / `AI_Stacks_Do_Purify` / `AI_Stacks_Do_RoadBuild` | Per-job order-setters |
| 8 | [`AI_Build_Target_List`](AIMOVE-AI_Build_Target_List.md) | Build `_ai_targets_*` |
| 9 | [`AI_Stacks_Roamers_Target_Or_Deploy`](AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md) | Assign targets to roamers; sets `lmt_Leaveable` as needed |
| 10 | [`AI_Stacks_Order_To_War_Landmass`](AIMOVE-AI_Stacks_Order_To_War_Landmass.md) | Send mobile non-melders to war landmass stage |
| **11** | **`AI_Stacks_Relocate_Roamers`** | **(this function)** — set up ferry for each roamer stack on a leaveable landmass |
| 13 | `AI_Stacks_Stage_Expedition_Forces` | Cross-landmass stage fill from pushout pool |
| 14 | `AI_Stacks_Garrison_Sites` | Top up garrisons |

## Call chain

```
AI_Stacks_Relocate_Roamers           (this function — driver, picks roamer stacks)
└─> AI_Stacks_Setup_Ferry            (per-stack helper — 6 phases, several documented bugs)
    ├─> AI_Stacks_Ferry_Add_Location        (register (wx,wy,wp) in the ferry-pickup queue)
    └─> AI_Stacks_Order_Ferry        (unreachable from this path — Phase 7b dead code in Setup_Ferry)
```

See:
- [AIMOVE-AI_Stacks_Setup_Ferry.md](AIMOVE-AI_Stacks_Setup_Ferry.md) — the per-stack workhorse; carries the bulk of the actual work
- [AIMOVE-AI_Stacks_Ferry_Add_Location.md](AIMOVE-AI_Stacks_Ferry_Add_Location.md) — registers a (wx,wy,wp) request in the ferry-pickup queue
- [AIMOVE-AI_Stacks_Order_Ferry.md](AIMOVE-AI_Stacks_Order_Ferry.md) — sets a unit's status to `us_Ferry` and consumes its stack slot (called from elsewhere too)

## Related references

- [AIMOVE-AI_Set_Unit_Orders.md](AIMOVE-AI_Set_Unit_Orders.md) — dispatcher; this function is slot 11
- [AIMOVE-AI_Stacks_Order_To_War_Landmass.md](AIMOVE-AI_Stacks_Order_To_War_Landmass.md) — slot 10 sibling (sends mobile units to the war landmass)
- [AIMOVE-AI_Reevaluate_All_Continents.md](AIMOVE-AI_Reevaluate_All_Continents.md) — establishes the per-(plane, player, landmass) stage-point coordinates read by Phase 1
- [MoX/src/MOM_DAT.h:102-107](../../MoX/src/MOM_DAT.h#L102-L107) — `AISTK_*` enum values
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
