AIMOVE-AI_Stacks_Stage_Expedition_Forces.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Stacks_Stage_Expedition_Forces

AI_Stacks_Survey_Expedition_Forces()
AI_Stacks_Survey_Expedition_Forces_Stack()
AI_Stacks_Stage_Expedition_Forces()


Not land vs ocean — superset/subset
G_Pushout   = top-9 EXCESS COMBAT UNITS across the army, regardless of mobility
G_Seafaring = top-9 EXCESS COMBAT UNITS that ALSO have Unit_Has_AirTravel OR Unit_Has_WaterTravel
G_Seafaring ⊆ G_Pushout semantically. Every water-capable excess unit is also a Pushout candidate; only water-capable ones additionally go into the Seafaring pool.
Overlap: yes
A single unit will appear in BOTH pools simultaneously if it (a) has air/water travel AND (b) ranks in the top 9 by value. The pools' top-9 cutoffs are independent (each maintains its own Lowest_Value), so a unit might be #5 in Pushout but #2 in Seafaring (since the Seafaring pool has fewer candidates competing).
The two pools are not partitioned — they're cached projections of the same underlying ranking, with different inclusion filters.
"Seafaring" is misleading
The flag is (AirTravel || WaterTravel) — flyers AND swimmers.
So the pool is really "water-crossing-capable," not "ships and sea creatures."
The naming reflects intent (units that can be sent over the ocean to attack), not means (whether they sail).

NOTE: Survey processes all stacks and submits any type
per stack, up to the count, which may be the whole stack

...The attack happens via the normal roamer-target-assignment pipeline after the expedition force has been ferried to where the targets are.
...it's the staging buffer of units flagged as available for redeployment to wherever offensive force is needed. 

¿ ~== attempt to gather the strongest units into the Stage Stack, per Landmass ?
hard-coded minimum garrison threshold  (¿ same as neutral player's ?)
time scaled minimum stack size


Source	                Fields
cp_staged_unit_count	Units currently parked at this landmass's stage point
cp_enroute_unit_count	Units already ordered to head to the stage point but not yet there
cp_drafted_unit_count	Pool size — number of units in G_Pushout_* (populated by AI_Stacks_Survey_Expedition_Forces)
_ai_expedition_size_threshold	    Minimum stack size threshold for triggering stage orders

G_Pushout_CX_IDs[itr]	Pool entry: index into _ai_own_stack_* (the stack this excess unit lives in)
G_Pushout_UL_Indices[itr]	Pool entry: per-stack slot index — into _ai_own_stack_unit_list[CX][UL]
G_Pushout_Unit_Indices[itr]	Pool entry: cached _UNITS[] index (== _ai_own_stack_unit_list[CX][UL])
cp_landmass_wx_array[landmass_idx], cp_landmass_wy_array[landmass_idx]	Stage point coordinates (cached pointers into _ai_continents.[...].wx_array/wy_array)


all part of "CONTXXXX"? ...should be ~ _cont_, _ai_cont_
"army", "navy"?
"walk", "float"?
expedition force
_ai_own_stacks
    cp_staged_unit_count
    cp_enroute_unit_count
    _ai_expedition_size_threshold
    ...
    cp_landmass_wx_array, cp_landmass_wy_array; but, also used by AI_Stacks_Garrison_Sites()
    ...

cp_staged_unit_count
_ai_own_stack_expedition_staged_unit_count

cp_enroute_unit_count
_ai_own_stack_expedition_enroute_unit_count

_ai_expedition_size_threshold
_ai_own_stack_expedition_required_size
    ..._cont_stage_required_size

G_Seafaring_Lowest_Value
navy_weakest
G_Pushout_Lowest_Value
army_weakest

G_Seafaring_Count
navy_drafted_unit_count
cp_drafted_unit_count
army_drafted_unit_count

G_Seafaring_Values
navy_strengths
G_Pushout_Values
army_strengths

G_Seafaring_CX_IDs
G_Pushout_CX_IDs            stack_idx into _ai_own_stack_unit_count, etc.
G_Seafaring_UL_Indices      
G_Pushout_UL_Indices        stack_ul_idx into _ai_own_stack_unit_count[itr_stacks];
G_Seafaring_Unit_Indices
G_Pushout_Unit_Indices      unti_idx 
navy_stack_idx
army_stack_idx
navy_stack_ul_idx
army_stack_ul_idx
navy_unit_list
army_unit_list


---

# AI_Stacks_Stage_Expedition_Forces — Walkthrough

**Location:** [MoM/src/AIMOVE.c:449](../../MoM/src/AIMOVE.c#L449) (~61 lines, ends 509).
**WZD overlay:** ovr158, p02
**drake178 name:** `G_AI_RallyFill()` — "attempts to move enough units to the stage point to create an offensive stack ... I think anyway"

## Purpose

Per-(plane, landmass) helper that **draws from the `G_Pushout_*` pool** — a top-9 secondary index INTO `_ai_own_stack_*` populated by [`AI_Stacks_Survey_Expedition_Forces`](AIMOVE-AI_Stacks_Survey_Expedition_Forces.md) — and orders the referenced units to head to THIS landmass's stage point, gradually assembling a force sized for downstream overseas attacks.

**This is part of the `_ai_own_stack_*` area-of-code, not a separate AI subsystem.** See [MoM-AI-Move-ai_own_stack.md](MoM-AI-Move-ai_own_stack.md) for the full pool structure. In brief: each `G_Pushout_*[i]` entry is a `(stack_idx, list_unit_idx, unit_idx, value)` triple where `stack_idx` and `list_unit_idx` point into `_ai_own_stack_unit_list[stack_idx][list_unit_idx]` — the same per-stack slot the order-issuers in slots 4-11 consume. The pool cherry-picks the top 9 "excess" slots across the whole army.

The "Expedition_Forces" naming is **forward-looking** — neither this function nor the stage point itself does any combat. Units staged here will eventually be ferried (or marched) to the war landmass via the normal roamer-target-assignment pipeline. The "attack" framing reflects the *expected destination-use*, not current-code semantics.

Conceptually the cross-landmass companion to [`AI_Stacks_Relocate_Roamers`](AIMOVE-AI_Stacks_Relocate_Roamers.md):

- **Relocate_Roamers** (slot 11): SEND this landmass's roamers OUT (via ferry)
- **Stage_Expedition_Forces** (slot 12): PULL the top-9 excess slots IN to this landmass's stage

The pushout pool is the connecting buffer between landmasses — `_ai_own_stack_*` slots flagged as excess elsewhere become available here.

Also performs a periodic (5% chance) reevaluate of the continent strategy via [`AI_Reevaluate_Continent`](AIMOVE-AI_Reevaluate_Continent.md) when the staged stack is already full.

## Signature

```c
void AI_Stacks_Stage_Expedition_Forces(int16_t landmass_idx, int16_t wp, int16_t player_idx)
```

Called from [`AI_Set_Unit_Orders`](AIMOVE-AI_Set_Unit_Orders.md) at [line 289](../../MoM/src/AIMOVE.c#L289) and [line 397](../../MoM/src/AIMOVE.c#L397) (the two dispatch paths). **Notably UNGATED** — fires on every (plane, landmass) iteration regardless of landmass type. Every other slot in this dispatch sequence has some kind of landmass-type gate; this one runs unconditionally and applies its own gating logic internally via the Phase 2 sanity checks.

## Inputs (globals read)

| Source | Fields |
|---|---|
| `cp_staged_unit_count` | Units currently parked at this landmass's stage point |
| `cp_enroute_unit_count` | Units already ordered to head to the stage point but not yet there |
| `cp_drafted_unit_count` | Pool size — number of units in `G_Pushout_*` (populated by `AI_Stacks_Survey_Expedition_Forces`) |
| `_ai_expedition_size_threshold` | Minimum stack size threshold for triggering stage orders |
| `MAX_STACK` (= 9) | Per-square stack cap |
| `_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx]` | Landmass type — checked for `lmt_Leaveable` and `lmt_NoOwnCity` |
| `G_Pushout_CX_IDs[itr]` | Pool entry: index into `_ai_own_stack_*` (the stack this excess unit lives in) |
| `G_Pushout_UL_Indices[itr]` | Pool entry: per-stack slot index — into `_ai_own_stack_unit_list[CX][UL]` |
| `G_Pushout_Unit_Indices[itr]` | Pool entry: cached `_UNITS[]` index (== `_ai_own_stack_unit_list[CX][UL]`) |
| `cp_landmass_wx_array[landmass_idx]`, `cp_landmass_wy_array[landmass_idx]` | Stage point coordinates (cached pointers into `_ai_continents.[...].wx_array/wy_array`) |
| `Random(20)` | 5% chance roll for the reevaluate trigger |

## Outputs (side effects)

- **Periodic call** to [`AI_Reevaluate_Continent(player_idx, landmass_idx, wp)`](AIMOVE-AI_Reevaluate_Continent.md) when staged stack is full (5% chance per call).
- For each unit pulled from the pushout pool, calls [`AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, stage_wx, stage_wy, unit_list_idx, list_unit_idx)`](../../MoM/src/AIMOVE.c) with `g_ai_set_target_caller = 1` at [line 505](../../MoM/src/AIMOVE.c#L505).
  - The order-setter writes `_UNITS[unit_idx].Status` (`us_Move` or `us_GOTO`) and `dst_wx/wy`.
  - Consumes the stack slot via `ST_UNDEFINED` write to `_ai_own_stack_unit_list[unit_list_idx][list_unit_idx]`.

Does NOT decrement `cp_drafted_unit_count` or remove entries from `G_Pushout_*` — the pool is read-only here.

## Locals

```c
int16_t unit_list_idx = 0;  // G_Pushout_CX_IDs[itr] cache — index into _ai_own_stack_unit_list
int16_t unit_idx = 0;       // G_Pushout_Unit_Indices[itr] cache — index into _UNITS[]
int16_t list_unit_idx = 0;  // G_Pushout_UL_Indices[itr] cache — per-stack slot
int16_t gap_count = 0;      // MAX_STACK - cp_enroute_unit_count — how many slots to fill
int16_t itr = 0;            // pushout-pool iterator
```

## Code walk

### Phase 1 — Periodic continent reevaluate when staged stack is full ([lines 457-465](../../MoM/src/AIMOVE.c#L457-L465))

```c
if(cp_staged_unit_count == MAX_STACK)
{
    if(Random(20) == 1)  /* 1:20  5% chance */
    {
        AI_Reevaluate_Continent(player_idx, landmass_idx, wp);
    }
}
```

If the staged stack is full (`MAX_STACK = 9` units already at the stage point), there's a 5% chance per call to invoke `AI_Reevaluate_Continent` — refresh the continent's strategic assessment. The throttle prevents continuous re-evaluation while staging is maxed out.

**Why only when full?** The stage is "ready" — the expedition force is at full strength. The reevaluation likely picks a new target or war landmass given the available force. With a 5% chance per AI player per call, and the dispatcher calling this once per (plane, landmass), the effective rate is "once every ~20 dispatch iterations on a fully-staged landmass."

### Phase 2 — Sanity checks (two early-return guards) ([lines 467-485](../../MoM/src/AIMOVE.c#L467-L485))

```c
/* Check if we've already met our quota */
if((cp_staged_unit_count + cp_enroute_unit_count) >= _ai_expedition_size_threshold)
{
    return;
}
/* Not enough available, don't bother */
/* {lmt_Unevaluated, lmt_Own, lmt_Contested, lmt_NoOwnCityAndAllyHasCity} */
/* Alt. if {lmt_NoOwnCity, lmt_Leaveable, lmt_NoTargets}, assemble what you can? */
if(
    (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] < lmt_Leaveable)
    &&
    (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] != lmt_NoOwnCity)
    &&
    ((cp_staged_unit_count + cp_enroute_unit_count + cp_drafted_unit_count) < _ai_expedition_size_threshold)
)
{
    return;
}
```

**Guard A — quota already met.** If `(staged + enroute) >= _ai_expedition_size_threshold`, return immediately. The expedition force at this landmass already has enough committed; no need to pull more.

**Guard B — can't fill anyway AND landmass isn't a high-priority source.** Returns when ALL THREE hold:

1. Landmass type is BELOW `lmt_Leaveable` — i.e., NOT one of the "desperate" types (`lmt_Leaveable` = evacuating, `lmt_NoTargets` = idle units available).
2. Landmass type is NOT `lmt_NoOwnCity` — also a "desperate" type meaning we have no presence here.
3. `(staged + enroute + drafted) < _ai_expedition_size_threshold` — even adding all available drafts wouldn't reach the minimum.

Reading this together: **for the "normal" landmass types (`lmt_Unevaluated`, `lmt_Own`, `lmt_Contested`, `lmt_NoOwnCityAndAllyHasCity`), only commit drafts if they'd actually fill the expedition stack to its threshold.** For the "desperate" types (`lmt_Leaveable`, `lmt_NoTargets`, `lmt_NoOwnCity`), commit whatever drafts are available even if they don't fill it — the source comment phrases this as "assemble what you can?"

### Phase 3 — Pull from pushout pool to stage point ([lines 487-507](../../MoM/src/AIMOVE.c#L487-L507))

```c
/* Determine how many slots are available in the virtual "stack" being built */
gap_count = (MAX_STACK - cp_enroute_unit_count);
for(itr = 0; itr < gap_count; itr++)
{
    /* If we run out of drafted units to send, stop */
    if(itr >= cp_drafted_unit_count)
    {
        break;
    }
    unit_list_idx = G_Pushout_CX_IDs[itr];
    list_unit_idx = G_Pushout_UL_Indices[itr];
    unit_idx = G_Pushout_Unit_Indices[itr];
    g_ai_set_target_caller = 1;
    AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, cp_landmass_wx_array[landmass_idx], cp_landmass_wy_array[landmass_idx], unit_list_idx, list_unit_idx);
}
```

Iterate the pool up to `min(gap_count, cp_drafted_unit_count)` entries. For each, dereference the triple — `stack_idx = G_Pushout_CX_IDs[itr]`, `list_unit_idx = G_Pushout_UL_Indices[itr]`, `unit_idx = G_Pushout_Unit_Indices[itr]` — and order that unit (via its position in `_ai_own_stack_unit_list[stack_idx][list_unit_idx]`) to head to this landmass's stage point.

The `unit_idx` argument to `AI_Stacks_Order_Attack_Target_Or_Goto_Destination` is the cached `_UNITS[]` index; the `stack_idx` and `list_unit_idx` arguments are used by that order-setter to write `ST_UNDEFINED` back into `_ai_own_stack_unit_list[stack_idx][list_unit_idx]`, consuming the slot the same way every other order-issuer in slots 4-11 does. The consume-by-sentinel idiom applies uniformly whether the order originates from inline iteration or from the pushout pool.

**The "top 9" is bogus — see the AoC doc.** The pool entries this loop iterates are NOT actually the 9 strongest excess units across the army. Per the OGBUG flagged at [AIMOVE.c:4034-4042](../../MoM/src/AIMOVE.c#L4034-L4042), the producer assigns pseudo-random values to candidates instead of real strength scores, so the priority queue ranks by garbage. This function draws "9 essentially-random excess units" each time it fires. The dispatcher behaves as if it's pulling carefully-ranked reinforcements; in OG behavior it's pulling random ones. See [MoM-AI-Move-ai_own_stack.md](MoM-AI-Move-ai_own_stack.md) for the full explanation of the producer-side bug.

**`gap_count = MAX_STACK - cp_enroute_unit_count`** — how many more units would fit in the destination stack (capped at 9, minus those already enroute). Note this does NOT account for `cp_staged_unit_count` — only enroute units. So a stack at stage with 5 units already there, 2 enroute, would compute `gap_count = 9 - 2 = 7`, planning to send 7 more — overshooting the stack cap of 9 by 5. Maybe intentional (some enroute won't arrive) or another quirk.

### Why `_ai_expedition_size_threshold` matters here (forward-looking sizing)

The threshold `_ai_expedition_size_threshold = (2 + _turn / 30)` (capped at MAX_STACK) is the **minimum stack size worth committing to an overseas expedition**, NOT the minimum stack size to attack right now. Phase 2's gates use it to decide whether staging more units is worth the opportunity cost (ferry slots, units committed in transit, defenders pulled from home). Early game (low threshold) → easy to qualify; late game (threshold = 9) → only landmasses already close to a full stack qualify, narrowing the stage pipeline.

The attack itself happens turns later: ferried units arrive on the war landmass, get reclassified as `AISTK_Roamer`, and slot 9 `AI_Stacks_Roamers_Target_Or_Deploy` picks attack targets via `AI_Stacks_Assign_Target`. This function only assembles the force; the attack is downstream and contextual.

## ASCII summary

```
AI_Stacks_Stage_Expedition_Forces(landmass_idx, wp, player_idx)
  ├─ Phase 1: if staged == MAX_STACK:
  │    └─ 5% chance → AI_Reevaluate_Continent
  ├─ Phase 2: sanity checks (early returns)
  │    ├─ if (staged + enroute) >= min                                 → return  (quota met)
  │    └─ if NOT (lmt_Leaveable+ OR lmt_NoOwnCity) AND combined < min  → return  (can't fill, not desperate)
  └─ Phase 3: pool draw
       └─ for itr in 0..min(MAX_STACK - enroute, drafted_count):
            └─ pull G_Pushout_*[itr] → order to stage_wx/wy [caller=1]
```

## Position in the dispatch chain

| Slot | Function | Gate |
|---|---|---|
| 1 | [`AI_Stacks_Init_Build_Target_Order`](AIMOVE-AI_Stacks_Init_Build_Target_Order.md) | Always |
| 2 | [`AI_Stacks_Move_Out_NonMilitary_Garrisoned`](AIMOVE-AI_Stacks_Move_Out_NonMilitary_Garrisoned.md) | Always |
| 3 | `AI_Stacks_Survey_Expedition_Forces` | Always — populates `G_Pushout_*` pool |
| 4-7 | `AI_Stacks_Do_Meld` / `AI_Stacks_Do_Settle` / `AI_Stacks_Do_Purify` / `AI_Do_RoadBuild` | Always (per-type internal filters) |
| 8 | [`AI_Build_Target_List`](AIMOVE-AI_Build_Target_List.md) | Always |
| 9 | [`AI_Stacks_Roamers_Target_Or_Deploy`](AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md) | Always |
| 10 | [`AI_Stacks_Order_To_War_Landmass`](AIMOVE-AI_Stacks_Order_To_War_Landmass.md) | NOT `lmt_Contested` |
| 11 | [`AI_Stacks_Relocate_Roamers`](AIMOVE-AI_Stacks_Relocate_Roamers.md) | `lmt_Leaveable` / `lmt_NoTargets` / `lmt_Own` |
| **12** | **`AI_Stacks_Stage_Expedition_Forces`** | **UNGATED — runs for every landmass** |
| 13 | `AI_Stacks_Garrison_Sites` | `lmt_Own` / `lmt_Contested` / `>= lmt_Leaveable` |

The chain forms a "circulate the army" pair across slots 11 and 12:

- **Slot 3** populated `G_Pushout_*` with the top-9 "excess" `_ai_own_stack_*` slots across the army (the pool indexes INTO the AoC's main arrays — see [MoM-AI-Move-ai_own_stack.md](MoM-AI-Move-ai_own_stack.md))
- **Slot 11 SENDS** roamers from leaveable landmasses outward (via ferry setup — separate mechanism, not the pool)
- **Slot 12 PULLS** the top-9 pool entries IN to whatever landmass needs reinforcement

The pushout pool is the cross-landmass bus, riding on top of the `_ai_own_stack_*` AoC.

## Related references

- [AIMOVE-AI_Set_Unit_Orders.md](AIMOVE-AI_Set_Unit_Orders.md) — dispatcher; this function is slot 12 (UNGATED)
- [AIMOVE-AI_Stacks_Relocate_Roamers.md](AIMOVE-AI_Stacks_Relocate_Roamers.md) — slot 11 sibling (sends roamers OUT)
- [AIMOVE-AI_Reevaluate_Continent.md](AIMOVE-AI_Reevaluate_Continent.md) — periodic call target (Phase A)
- [`AI_Stacks_Survey_Expedition_Forces`](AIMOVE-AI_Stacks_Survey_Expedition_Forces.md) (slot 3) — populates the `G_Pushout_*` pool this function consumes
- [MoX/src/MOM_DAT.h:524](../../MoX/src/MOM_DAT.h#L524) — `us_*` Status enum
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
