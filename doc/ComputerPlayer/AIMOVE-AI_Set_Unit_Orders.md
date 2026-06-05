AIMOVE-AI_Set_Unit_Orders.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_SetUnitOrders.c

first proc in ovr158
...AI_Reevaluate_Continent() is last

---

# AI_Set_Unit_Orders — Walkthrough

**Location:** [MoM/src/AIMOVE.c:203](../../MoM/src/AIMOVE.c#L203) (~121 lines, ends [line 323](../../MoM/src/AIMOVE.c#L323)).
**WZD overlay:** ovr158, p01
**drake178 name:** `AI_SetUnitOrders()`

## Purpose

**Top-level AI orders driver, called once per AI player per turn.** Sets the `ai_human_hostility` flag, performs the global pre-pass (budget, off-plane shift, boat redistribution, opportunity-city), then walks every (plane, landmass) and dispatches the per-landmass AI pipeline. Restores the default EMM mapping before returning.

This function is a **dispatcher** — almost all real logic lives in the callees. Its job is sequencing: which sub-systems run, in what order, gated on which `lmt_*` types, and how the per-landmass globals are set up so callees can find them.

## Side-channel globals it writes

Before iterating each landmass, the dispatcher caches three per-plane pointers into module-scope globals so the per-landmass callees can read the landmass data without taking it as parameters:

```c
cp_landmass_wx_array   = &_ai_continents.plane[wp].player[player_idx].wx_array[0];
cp_landmass_wy_array   = &_ai_continents.plane[wp].player[player_idx].wy_array[0];
cp_landmass_type_array = &_ai_continents.plane[wp].player[player_idx].type_array[0];
```

It also writes `ai_human_hostility` (boolean) and `_ai_expedition_size_threshold` (turn-scaled stack size threshold), and resets `_ai_ferry_count = 0`.

## When it's called

Top-level per-turn AI entry point. Called from outside this file (see callers of `AI_Set_Unit_Orders`).

## Signature and locals

```c
void AI_Set_Unit_Orders(int16_t player_idx)
{
    int16_t wp = 0;           // plane iterator
    int16_t landmass_idx = 0; // landmass iterator
    ...
}
```

## Phase 1 — Hostility flag

```c
// AIMOVE.c:209-221
ai_human_hostility = ST_FALSE;
if(
    (
        (_players[player_idx].Hostility[HUMAN_PLAYER_IDX] >= 3)
        ||
        (_players[player_idx].Dipl.Dipl_Status[HUMAN_PLAYER_IDX] >= DIPL_War)
    )
    &&
    (_players[player_idx].peace_duration[HUMAN_PLAYER_IDX] == 0)
)
{
    ai_human_hostility = ST_TRUE;
}
```

**Intent:** set `ai_human_hostility` to ST_TRUE when this AI player is hostile toward the human (Hostility ≥ 3 = Jihad, OR Dipl_Status ≥ DIPL_War) AND not in a peace duration. Hostility 3 = Jihad per the [`Hostility` OSG scale](MoM-NextTurn-AI.md). This flag is consumed by downstream AI decisions.

## Phase 2 — Per-turn setup

```c
// AIMOVE.c:223-230
EMM_Map_CONTXXX__WIP();
_ai_ferry_count = 0;
_ai_expedition_size_threshold = (2 + (_turn / 30));
if(_ai_expedition_size_threshold > MAX_STACK)
{
    _ai_expedition_size_threshold = MAX_STACK;
}
```

**Intent:**
- Page the CONTXXX data handle into the EMS frame so the `_ai_landmass_*` arrays are accessible.
- Reset the seek-transport counter to 0.
- Compute the minimum attack-stack threshold: grows 1 per 30 turns starting from 2, capped at `MAX_STACK` (9 by turn 240).

## Phase 3 — Global pre-pass (plane-agnostic)

```c
// AIMOVE.c:232-237
AI_Disband_To_Balance_Budget(player_idx);
AI_Shift_Off_Home_Plane(player_idx);
AI_Move_Out_Boats();
/* OGBUG: wp is used here before being initialized to 0 */
AI_Find_Opportunity_City_Target(wp, player_idx);
```

**Intent:** four global passes run once before the per-landmass loops:
1. `AI_Disband_To_Balance_Budget` — disband units if the player is bleeding gold.
2. `AI_Shift_Off_Home_Plane` — move expansion off the home plane.
3. `AI_Move_Out_Boats` — redistribute transports.
4. `AI_Find_Opportunity_City_Target` — look for an opportunity-city to target.

### OGBUG — `wp` passed-before-init (line 237)

`AI_Find_Opportunity_City_Target(wp, player_idx)` is called BEFORE the plane loop starts. At this point `wp` still holds its initializer value `0` (Arcanus). The function only ever evaluates the Arcanus plane this turn — Myrror is never considered for opportunity-city targets in this pass. **IDA-confirmed OG-faithful** — OG asm at `loc_EBE34-` has `mov [bp+wp], 0` AFTER the call (only initializes wp post-call), so the actual OG call passes whatever happened to be on the stack frame at function entry. Production's explicit `int16_t wp = 0;` reproduces the same observable (wp == 0 at the call site) via a different timing. Preserved faithful-to-dasm.

## Phase 4 — Plane × landmass dispatch loop

```c
// AIMOVE.c:240-318
for(wp = 0; wp < NUM_PLANES; wp++)
{
    cp_landmass_wx_array   = &_ai_continents.plane[wp].player[player_idx].wx_array[0];
    cp_landmass_wy_array   = &_ai_continents.plane[wp].player[player_idx].wy_array[0];
    cp_landmass_type_array = &_ai_continents.plane[wp].player[player_idx].type_array[0];

    for(landmass_idx = 1; landmass_idx < NUM_LANDMASSES; landmass_idx++)
    {
        AI_Stacks_Init_Build_Target_Order(player_idx, landmass_idx, wp);
        AI_Stacks_Move_Out_NonMilitary_Garrisoned(wp);
        AI_Stacks_Survey_Expedition_Forces();
        AI_Stacks_Do_Meld(player_idx);
        AI_Stacks_Do_Settle(player_idx, landmass_idx);
        AI_Stacks_Do_Purify(landmass_idx, wp);
        AI_Do_RoadBuild(landmass_idx);
        AI_Build_Target_List(player_idx, landmass_idx, wp);
        AI_Stacks_Roamers_Target_Or_Deploy(landmass_idx, wp, player_idx);

        // Pull units toward main-war landmass — runs for "almost NOT lmt_Contested"
        if(
            (cp_landmass_type_array[landmass_idx] >= lmt_Leaveable)
            || (cp_landmass_type_array[landmass_idx] == lmt_Own)
            || (cp_landmass_type_array[landmass_idx] == lmt_NoOwnCityAndAllyHasCity)
            || (cp_landmass_type_array[landmass_idx] == lmt_NoOwnCity)
        )
        {
            AI_Stacks_Order_To_War_Landmass(player_idx, wp);
        }

        // Home stage fill — only for own / abandoned / no-targets
        if(
            (cp_landmass_type_array[landmass_idx] >= lmt_Leaveable)
            || (cp_landmass_type_array[landmass_idx] == lmt_Own)
        )
        {
            AI_Stacks_Relocate_Roamers(landmass_idx, wp, player_idx);
        }

        AI_Stacks_Stage_Expedition_Forces(landmass_idx, wp, player_idx);  // unconditional

        // Garrison fill — for own / contested / abandoned (NOT NoOwnCity/AllyHasCity)
        if(
            (cp_landmass_type_array[landmass_idx] == lmt_Own)
            || (cp_landmass_type_array[landmass_idx] == lmt_Contested)
            || (cp_landmass_type_array[landmass_idx] >= lmt_Leaveable)
        )
        {
            AI_Stacks_Garrison_Sites(player_idx, wp, landmass_idx);
        }
    }

    // Non-landmass passes
    AI_Stacks_Wartime_Ocean_Movement_And_Cleanup(player_idx, wp);
    AI_Stacks_Ocean_Landmass_Orders(player_idx, wp);
}
```

**Intent:** for each plane, set the side-channel pointers, then for each landmass (slot 1 onward — slot 0 = ocean is skipped) run the per-landmass AI pipeline. After all landmasses on a plane are processed, run two non-landmass passes (ocean units, transports).

### Per-landmass dispatch order (always-on)

| Order | Function | Purpose (brief) |
|---|---|---|
| 1 | `AI_Stacks_Init_Build_Target_Order` | Build the per-landmass target list + stack assignments. |
| 2 | `AI_Stacks_Move_Out_NonMilitary_Garrisoned` | Push garrison builds. |
| 3 | `AI_Stacks_Survey_Expedition_Forces` | Inventory excess units (for later disband). |
| 4 | `AI_Stacks_Do_Meld` | Spirit-meld at nodes. |
| 5 | `AI_Stacks_Do_Settle` | Issue settler-settle orders. |
| 6 | `AI_Stacks_Do_Purify` | Issue purify orders for corrupted tiles. |
| 7 | `AI_Do_RoadBuild` | Issue road-build orders. |
| 8 | `AI_Build_Target_List` | (Re-)build the target list. |
| 9 | `AI_Stacks_Roamers_Target_Or_Deploy` | Move stray "roamer" stacks. |
| 12 | `AI_Stacks_Stage_Expedition_Forces` | Top up the stage point with units (always). |

### Per-landmass conditional gates

| Function | Slot | Gate | Notes |
|---|---|---|---|
| `AI_Stacks_Order_To_War_Landmass` | 10 | `lmt_Leaveable` or `lmt_Own` or `lmt_NoOwnCityAndAllyHasCity` or `lmt_NoOwnCity` | "almost just NOT `lmt_Contested`" — comment in source. Skips contested-only. |
| `AI_Stacks_Relocate_Roamers` | 11 | `lmt_Leaveable` or `lmt_Own` | Home-base stage — only for landmasses we hold or are abandoning. |
| `AI_Stacks_Garrison_Sites` | 13 | `lmt_Own` or `lmt_Contested` or `lmt_Leaveable` | Garrison maintenance — needs a city to garrison. Excludes `lmt_NoOwnCity` / `lmt_NoOwnCityAndAllyHasCity`. |

(Slots 10 / 11 / 13 are gated; slot 12 is unconditional and listed in the table above.)

### Per-plane post-landmass passes

After all landmasses on a plane finish, two non-landmass passes run:
- `AI_Stacks_Wartime_Ocean_Movement_And_Cleanup` — units at sea (not assigned to any landmass).
- `AI_Stacks_Ocean_Landmass_Orders` — transport assignments / loading.

## Phase 5 — Cleanup

```c
// AIMOVE.c:321
EMMDATAH_Map();
```

Restores the default data-segment mapping in the EMS page frame, undoing the `EMM_Map_CONTXXX__WIP()` from Phase 2. Mirrors the cleanup pattern in sibling [`AI_Reevaluate_All_Continents`](AIMOVE-AI_Reevaluate_All_Continents.md).

## Bug catalog summary

| # | Severity | Line | Description |
|---|---|---|---|
| OGBUG | Medium | 237 (Phase 3) | `AI_Find_Opportunity_City_Target(wp, ...)` called with `wp` still at its initializer (0). Only Arcanus is ever evaluated for opportunity-city targets. **IDA-confirmed OG-faithful.** |

All other "many many BUGs" referenced in the function's preamble comment live in the per-landmass callees, not in the dispatcher itself.

## Call graph

```
AI_Set_Unit_Orders(player_idx)
├── EMM_Map_CONTXXX__WIP                     [setup]
├── AI_Disband_To_Balance_Budget             [global pre-pass]
├── AI_Shift_Off_Home_Plane                  [global pre-pass]
├── AI_Move_Out_Boats                        [global pre-pass]
├── AI_Find_Opportunity_City_Target          [global pre-pass; OGBUG: wp=0 stale]
└── for wp in [Arcanus, Myrror]:
    └── for landmass_idx in [1..NUM_LANDMASSES):
        ├── AI_Stacks_Init_Build_Target_Order
        ├── AI_Stacks_Move_Out_NonMilitary_Garrisoned
        ├── AI_Stacks_Survey_Expedition_Forces
        ├── AI_Stacks_Do_Meld
        ├── AI_Stacks_Do_Settle
        ├── AI_Stacks_Do_Purify
        ├── AI_Do_RoadBuild
        ├── AI_Build_Target_List
        ├── AI_Stacks_Roamers_Target_Or_Deploy
        ├── AI_Stacks_Order_To_War_Landmass           [gate: NOT lmt_Contested basically]
        ├── AI_Stacks_Relocate_Roamers          [gate: lmt_Own / lmt_Leaveable+]
        ├── AI_Stacks_Stage_Expedition_Forces              [always]
        │   └── AI_Reevaluate_Continent     [5% roll when stage is full]
        └── AI_Stacks_Garrison_Sites            [gate: lmt_Own / lmt_Contested / lmt_Leaveable+]
    ├── AI_Stacks_Wartime_Ocean_Movement_And_Cleanup                 [per-plane post-pass]
    └── AI_Stacks_Ocean_Landmass_Orders          [per-plane post-pass]
└── EMMDATAH_Map                            [cleanup]
```

## Related references

- [AIMOVE-AI_Reevaluate_Continent.md](AIMOVE-AI_Reevaluate_Continent.md) — invoked via `AI_Stacks_Stage_Expedition_Forces` on 5% roll
- [AIMOVE-AI_Reevaluate_All_Continents.md](AIMOVE-AI_Reevaluate_All_Continents.md) — full-rebuild classifier; **not** called from this dispatcher (runs on a separate per-turn schedule)
- [AIMOVE-AI_Choose_War_Landmass.md](AIMOVE-AI_Choose_War_Landmass.md) — producer of `_ai_landmass_war_targets[]` read by `AI_Stacks_Order_To_War_Landmass`
- [MoM-AI-Landmass-Types.md](MoM-AI-Landmass-Types.md) — `lmt_*` enum used by every conditional gate above
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
