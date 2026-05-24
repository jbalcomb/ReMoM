AIMOVE-AI_Stacks_Order_To_War_Landmass.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Stacks_Order_To_War_Landmass

---

# AI_Stacks_Order_To_War_Landmass — Walkthrough

**Location:** [MoM/src/AIMOVE.c:1925](../../MoM/src/AIMOVE.c#L1925) (~64 lines, ends 1988).
**WZD overlay:** ovr158, p08
**drake178 name:** `AI_PullForMainWar()`

## Purpose

Order mobile (air- or water-traveling) non-melder units to head to the **main war landmass's stage point** for this (player, plane). Operates on stacks pulled from the per-(plane, landmass) `_ai_own_stack_*` inventory.

The "main war landmass" is `_ai_landmass_war_targets[wp][player_idx]` — a single per-(plane, player) value chosen earlier in the turn by [`AI_Choose_War_Landmass`](AIMOVE-AI_Choose_War_Landmass.md). The stage point is the dock/embark coordinate stored in `_ai_continents.plane[wp].player[player_idx].wx_array[war_landmass] / .wy_array[...]`.

Runs as dispatch slot 10 of [`AI_Set_Unit_Orders`](AIMOVE-AI_Set_Unit_Orders.md), immediately after slot 9 [`AI_Stacks_Roamers_Target_Or_Deploy`](AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md). The slot-9 function sets `lmt_Leaveable` on landmasses that the AI wants to evacuate toward the war target — and the comment at [MOM_DAT.h:167](../../MoX/src/MOM_DAT.h#L167) explicitly notes that `lmt_Leaveable` "seems made specifically for AI_Stacks_Order_To_War_Landmass, which runs next."

## Signature

```c
void AI_Stacks_Order_To_War_Landmass(int16_t player_idx, int16_t wp)
```

Called from [`AI_Set_Unit_Orders`](AIMOVE-AI_Set_Unit_Orders.md) at [line 198](../../MoM/src/AIMOVE.c#L198) and [line 304](../../MoM/src/AIMOVE.c#L304) (the two dispatch paths), gated by landmass type. The gate is:

```c
if(
    (cp_landmass_type_array[landmass_idx] >= lmt_Leaveable)        // lmt_Leaveable, lmt_NoTargets
    || (cp_landmass_type_array[landmass_idx] == lmt_Own)
    || (cp_landmass_type_array[landmass_idx] == lmt_NoOwnCityAndAllyHasCity)
    || (cp_landmass_type_array[landmass_idx] == lmt_NoOwnCity)
)
{
    AI_Stacks_Order_To_War_Landmass(player_idx, wp);
}
```

Effectively: runs on every landmass type **except `lmt_Contested`** (the source comment at [line 187](../../MoM/src/AIMOVE.c#L187) reads "almost just NOT lmt_Contested"). The function itself doesn't know `landmass_idx` — that information is implicit through the `_ai_own_stack_*` contents which slot 1 plane-and-landmass-scoped for this call.

## Inputs (globals read)

| Source | Fields |
|---|---|
| `_ai_landmass_war_targets[wp][player_idx]` | The per-plane war target landmass index. Early-out if `0` (no war target). |
| `_ai_own_stack_count` | Outer loop bound. |
| `_ai_own_stack_type[s]` | Filter — only `AISTK_Roamer` (1) or `AISTK_Unknown` (0). |
| `_ai_own_stack_wx[s]`, `_ai_own_stack_wy[s]` | Stack position. |
| `_ai_own_stack_unit_count[s]` | Inner loop bound. |
| `_ai_own_stack_unit_list[s][u]` | Per-stack unit indices. |
| `_landmasses[wp*WORLD_SIZE + wy*WORLD_WIDTH + wx]` | Landmass index at stack's position. Compared against war target. |
| `_UNITS[unit_idx].type` | Lookup key into `_unit_type_table`. |
| `_unit_type_table[type].Abilities` | `UA_MELD` bit test (excluded). |
| `Unit_Has_AirTravel(unit_idx)`, `Unit_Has_WaterTravel(unit_idx)` | Mobility tests (helper functions). |
| `_ai_continents.plane[wp].player[player_idx].wx_array[war_landmass]`, `.wy_array[...]` | Destination — the stage point coordinates of the war landmass. |

## Outputs (side effects)

- For each qualifying unit, calls [`AI_Stacks_Order_Attack_Target_Or_Goto_Destination`](../../MoM/src/AIMOVE.c) with `(unit_idx, stage_wx, stage_wy, itr_stacks, itr_list_units)`.
  - The order-setter writes `_UNITS[unit_idx].Status` (`us_Move` or `us_GOTO` based on the destination's eval bits) and `_UNITS[unit_idx].dst_wx/wy`.
  - It also consumes the `_ai_own_stack_unit_list[s][u]` slot by writing `ST_UNDEFINED` (consume-by-sentinel idiom) — so any subsequent slot (11/13/14) reading the same array will skip this unit.
- Sets `g_ai_set_target_caller = 8` at [line 1984](../../MoM/src/AIMOVE.c#L1984) for the debug-instrumentation trail.

## Locals

```c
int16_t itr_stacks = 0;                       // outer loop iterator (_ai_own_stack_count)
int16_t itr_list_units = 0;                   // inner loop iterator (per-stack unit slots)
int16_t list_unit_count = 0;                  // _ai_own_stack_unit_count[itr_stacks] cache
int16_t unit_idx = 0;                         // _ai_own_stack_unit_list[itr_stacks][itr_list_units] cache
int16_t stack_wx = 0;                         // _ai_own_stack_wx[itr_stacks] cache
int16_t stack_wy = 0;                         // _ai_own_stack_wy[itr_stacks] cache
int16_t war_landmass_idx = 0;                 // DNE in Dasm — _ai_landmass_war_targets[wp][player_idx] cache
int16_t current_landmass_idx = 0;             // DNE in Dasm — _landmasses[stack position] cache
int16_t war_landmass_stage_point_wx = 0;      // DNE in Dasm — destination cache (per-unit)
int16_t war_landmass_stage_point_wy = 0;      // DNE in Dasm — destination cache (per-unit)
```

The four `DNE in Dasm` locals are not in the original disassembly — they're introduced by the reconstruction to make the code more readable. In the OG bytes the corresponding values are recomputed inline (or are register-resident temporaries) at each use site.

## Code walk

### Phase 1 — Early-out if no war target ([lines 1938-1942](../../MoM/src/AIMOVE.c#L1938-L1942))

```c
war_landmass_idx = _ai_landmass_war_targets[wp][player_idx];
if(war_landmass_idx == 0)
{
    return;
}
```

No work to do if `AI_Choose_War_Landmass` declined to pick a war landmass this turn (landmass index `0` is the sentinel for "none" — landmass indices are `1..NUM_LANDMASSES-1`).

### Phase 2 — Per-stack filter ([lines 1943-1961](../../MoM/src/AIMOVE.c#L1943-L1961))

```c
for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
{
    /* Filter stack types (only Non-Garrison and Unknown) */
    if(
        (_ai_own_stack_type[itr_stacks] != AISTK_Roamer)
        &&
        (_ai_own_stack_type[itr_stacks] != AISTK_Unknown)
    )
    {
        continue;
    }
    list_unit_count = _ai_own_stack_unit_count[itr_stacks];
    stack_wx = _ai_own_stack_wx[itr_stacks];
    stack_wy = _ai_own_stack_wy[itr_stacks];
    current_landmass_idx = _landmasses[((wp * WORLD_SIZE) + (stack_wy * WORLD_WIDTH) + stack_wx)];
    if(current_landmass_idx == war_landmass_idx)
    {
        continue;
    }
    ...
```

Two early-continue filters:

1. **Type filter** — only `AISTK_Roamer` (1) and `AISTK_Unknown` (0) stacks are candidates. Garrison stacks (`AISTK_Garrison`, `AISTK_FortressGarrison`) are skipped because they're meant to stay parked on their site/city.
2. **Position filter** — only stacks NOT already on the war target landmass are candidates. Stacks already on the war landmass need no pulling.

The combined effect: this function pulls mobile units from landmasses other than the war target. The dispatcher's gate ([line 187-199](../../MoM/src/AIMOVE.c#L187-L199)) typically calls this function for `lmt_Leaveable`, `lmt_NoTargets`, `lmt_Own`, `lmt_NoOwnCityAndAllyHasCity`, and `lmt_NoOwnCity` landmasses — the `lmt_Leaveable` case being the explicit "evacuate to war target" path per the [MOM_DAT.h:158-170](../../MoX/src/MOM_DAT.h#L158-L170) annotation.

### Phase 3 — Per-unit filter and order ([lines 1962-1986](../../MoM/src/AIMOVE.c#L1962-L1986))

```c
for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
{
    unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
    /* ¿ OGBUG  should check wind-walk and items ? */
    if(
        (Unit_Has_AirTravel(unit_idx) != ST_TRUE)
        &&
        (Unit_Has_WaterTravel(unit_idx) != ST_TRUE)
    )
    {
        continue;
    }
    if((_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_MELD) != 0)
    {
        continue;
    }
    war_landmass_stage_point_wx = _ai_continents.plane[wp].player[player_idx].wx_array[war_landmass_idx];
    war_landmass_stage_point_wy = _ai_continents.plane[wp].player[player_idx].wy_array[war_landmass_idx];
    g_ai_set_target_caller = 8;
    AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, war_landmass_stage_point_wx, war_landmass_stage_point_wy, itr_stacks, itr_list_units);
}
```

Three early-continue filters per unit:

1. **Mobility filter** — skip unless `Unit_Has_AirTravel` OR `Unit_Has_WaterTravel` is true. Units that can't cross water under their own power are left for the ferry/embark flow elsewhere.
2. **Melder exclusion** — skip if the unit has `UA_MELD`. Magic Spirits have a different job (melding nodes) and shouldn't be pulled to a war stage point.
3. **No `ST_UNDEFINED` guard** — the iterator reads `_UNITS[unit_idx]` and calls `Unit_Has_AirTravel(unit_idx)` without first checking `unit_idx != ST_UNDEFINED`. If an earlier slot consumed this unit's slot, the helper calls would index `_UNITS[-1]` — undefined behavior. In practice the earlier slots 4-7 consume slots for settle/meld/purify/build orders and those units don't typically have air/water travel, so the bad case is plausibly rare. Still a real footgun — but as a faithful reconstruction it's preserved.

**Suspected OGBUG at [line 1965](../../MoM/src/AIMOVE.c#L1965)** — marked in source with `/* ¿ OGBUG  should check wind-walk and items ? */`. The mobility check uses `Unit_Has_AirTravel` / `Unit_Has_WaterTravel` which presumably look at the unit type's base abilities and movement-type byte. But a unit could gain air/water travel from the **Wind Walking** spell (UA_WINDWALKING granted to other stack-mates) or from an **enchanted item** (Flight, Water Walking) — neither of which would necessarily be reflected in those helpers. A unit so-enhanced would be skipped here even though it could cross water for the purposes of joining the war landmass. Whether this is fixable depends on what the helpers actually check; not actionable until the helpers are walked through.

`Unit_Has_AirTravel` and `Unit_Has_WaterTravel` are helper functions (not inlined).

## ASCII summary

```
AI_Stacks_Order_To_War_Landmass(player_idx, wp)
  ├─ if no war target → return
  └─ for s in 0.._ai_own_stack_count:
       ├─ if stack[s] is NOT Roamer or Unknown → skip
       ├─ if stack[s] IS on war target landmass → skip
       └─ for u in 0..unit_count[s]:
            └─ if unit has AirTravel|WaterTravel AND not Melder:
                 └─ AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit, stage_wx, stage_wy) [caller=8]
```

## Position in the dispatch chain

| Slot | Function | Notes |
|---|---|---|
| 1 | [`AI_Stacks_Init_Build_Target_Order`](AIMOVE-AI_Stacks_Init_Build_Target_Order.md) | Rebuilds `_ai_own_stack_*` |
| 2 | [`AI_Stacks_Move_Out_NonMilitary_Garrisoned`](AIMOVE-AI_Stacks_Move_Out_NonMilitary_Garrisoned.md) | Push builders out of garrisons |
| 3 | `AI_Stacks_Survey_Expedition_Forces` | Score surplus combat units into `G_Pushout_*` / `G_Seafaring_*` |
| 4-7 | `AI_Do_Meld` / `AI_Do_Settle` / `AI_Do_Purify` / `AI_Do_RoadBuild` | Per-job order-setters |
| 8 | [`AI_Build_Target_List`](AIMOVE-AI_Build_Target_List.md) | Build `_ai_targets_*` |
| 9 | [`AI_Stacks_Roamers_Target_Or_Deploy`](AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md) | Assign targets to roamer stacks; sets `lmt_Leaveable` as needed |
| **10** | **`AI_Stacks_Order_To_War_Landmass`** | **(this function)** — pull mobile non-melder units to war landmass stage point |
| 11 | [`AI_Stacks_Relocate_Roamers`](AIMOVE-AI_Stacks_Relocate_Roamers.md) | Set up ferry for each roamer on leaveable landmasses |
| 13 | `AI_Stacks_Stage_Expedition_Forces` | Continent stage fill from pushout pool |
| 14 | `AI_FillGarrisons__WIP` | Top up garrisons |

The slot-9 → slot-10 sequencing matters: slot 9's `AI_Stacks_Roamers_Target_Or_Deploy` is the only function that sets `lmt_Leaveable`, and the slot-10 gate accepts landmasses tagged `lmt_Leaveable`. So an `lmt_Leaveable` write in slot 9 directly feeds slot 10's `AI_Stacks_Order_To_War_Landmass` on the same (plane, landmass) iteration.

## Related references

- [AIMOVE-AI_Set_Unit_Orders.md](AIMOVE-AI_Set_Unit_Orders.md) — dispatcher; this function is slot 10
- [AIMOVE-AI_Choose_War_Landmass.md](AIMOVE-AI_Choose_War_Landmass.md) — sets `_ai_landmass_war_targets[wp][player_idx]`
- [AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md](AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md) — sole writer of `lmt_Leaveable` (the tag that drives most calls into this function)
- [AIMOVE-AI_Stacks_Init_Build_Target_Order.md](AIMOVE-AI_Stacks_Init_Build_Target_Order.md) — producer of the `_ai_own_stack_*` arrays
- [MoX/src/MOM_DAT.h:102-107](../../MoX/src/MOM_DAT.h#L102-L107) — `AISTK_*` enum values
- [MoX/src/MOM_DAT.h:158-170](../../MoX/src/MOM_DAT.h#L158-L170) — `lmt_Leaveable` setting conditions, including the "made specifically for AI_Stacks_Order_To_War_Landmass" annotation
- [MoX/src/MOM_DEF.h:698](../../MoX/src/MOM_DEF.h#L698) — `UA_MELD` flag bit
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
