AIMOVE-AI_Stacks_Init_Build_Target_Order

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Stacks_Init_Build_Target_Order

---

# AI_Stacks_Init_Build_Target_Orderthrough

**Location:** [MoM/src/AIMOVE.c:2817](../../MoM/src/AIMOVE.c#L2817) (~218 lines, ends 3035). GEMINI variant at [line 3037](../../MoM/src/AIMOVE.c#L3037).
**WZD overlay:** ovr158, p13
**drake178 name:** `AI_CONTX_Regroup()`

## Purpose

**Producer of every `_ai_own_stack_*` array** that the rest of the per-landmass AI dispatch reads. Called as dispatch slot 1 of [`AI_Set_Unit_Orders`](AIMOVE-AI_Set_Unit_Orders.md) Phase 4 — runs first per (plane, landmass) iteration, rebuilds the stack inventory from scratch, classifies each stack (Roamer / Garrison / FortressGarrison), and for roamer stacks already in `us_Move` status tries to find a nearby enemy stack to attack via [`AI_Stacks_Target_Nearest_Hostile_Stack`](../../MoM/src/AIMOVE.c#L3240).

Per the drake178 header:

> `form up to 80 stacks of own units on the selected continent, mark their types, count the units at or en route to the stage point, and try to move roaming stacks to attack any nearby roaming enemy stacks`

## Signature

```c
void AI_Stacks_Init_Build_Target_Order(int16_t player_idx, int16_t landmass_idx, int16_t wp)
```

Called from [`AI_Set_Unit_Orders`](AIMOVE-AI_Set_Unit_Orders.md) Phase 4 dispatch (slot 1), once per (plane, landmass).

## Outputs (the side-channel arrays this function rebuilds)

Per the comment block at [MoX/src/MOM_DAT.c:2685-2706](../../MoX/src/MOM_DAT.c#L2685-L2706):

| Array | Meaning |
|---|---|
| `_ai_own_stack_count` | Number of stack slots used (0..MAX_AI_STACKS) |
| `_ai_own_stack_wx[s]`, `_ai_own_stack_wy[s]`, `_ai_own_stack_wp[s]` | Stack `s`'s world coordinates and plane |
| `_ai_own_stack_unit_count[s]` | Number of units in stack `s` |
| `_ai_own_stack_unit_list[s][u]` | Unit index of slot `u` in stack `s`, OR `ST_UNDEFINED` if "consumed" (busy/processed). See [AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md](AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md) "Note on sparse `_ai_own_stack_unit_list[][]`" for the consume-by-sentinel idiom. |
| `_ai_own_stack_type[s]` | `AISTK_FortressGarrison` / `AISTK_Garrison` / `AISTK_Roamer` / `AISTK_Unknown` |

Also writes two `cp_*` module-level counters:
- `cp_staged_unit_count` — units already AT the landmass's stage point
- `cp_enroute_unit_count` — units in `us_GOTO`/`us_Purify`/`us_BuildRoad` status whose destination matches the stage point

## Locals

```c
int16_t first_unit_idx = 0;         // _ai_own_stack_unit_list[s][0] cache (Phase 3)
int16_t target_wy = 0;              // out from AI_Stacks_Target_Nearest_Hostile_Stack
int16_t target_wx = 0;              // out from AI_Stacks_Target_Nearest_Hostile_Stack
int16_t stack_idx = 0;              // result of inner stack lookup (Phase 2)
int16_t unit_wp = 0;                // _UNITS[itr].wp cache
int16_t unit_wy = 0;
int16_t unit_wx = 0;
int16_t itr_stacks1 = 0;            // outer loop iterator (init + Phase 3)
int16_t itr_units1 = 0;             // _UNITS[] iterator (Phase 2)
int16_t itr_stacks2 = 0;            // inner stack-lookup iterator (Phase 2)
int16_t itr_units2 = 0;             // DNE in Dasm — reuses itr_stacks2 in OG; split here for Phase 3 unit-list walk
```

## Phase 1 — Reset all `_ai_own_stack_*` arrays (lines 2832-2842)

```c
_ai_own_stack_count = 0;
cp_enroute_unit_count = 0;
cp_staged_unit_count = 0;

for(itr_stacks1 = 0; itr_stacks1 < MAX_AI_STACKS; itr_stacks1++)
{
    _ai_own_stack_wx[itr_stacks1] = ST_UNDEFINED;
    _ai_own_stack_wy[itr_stacks1] = ST_UNDEFINED;
    _ai_own_stack_wp[itr_stacks1] = ST_UNDEFINED;
    _ai_own_stack_unit_count[itr_stacks1] = 0;
}
```

**Intent:** wipe everything that this function is about to repopulate. Note that `_ai_own_stack_type[]` and `_ai_own_stack_unit_list[][]` are NOT explicitly cleared — they get overwritten as stacks are added in Phase 2. Slots beyond `_ai_own_stack_count` retain whatever was there from prior calls or initial allocation. (Not an issue for current Phase 3 because the gate iterates only over `0..count`.)

## Phase 2 — Group units into stacks by (wx, wy, wp) (lines 2845-2983)

```c
for(itr_units1 = 0; itr_units1 < _units; itr_units1++)
{
    if((_ai_own_stack_count >= MAX_AI_STACKS) || (_UNITS[itr_units1].owner_idx != player_idx)) continue;
    unit_wp = _UNITS[itr_units1].wp;
    if(unit_wp != wp) continue;                 // plane filter — guarantees _ai_own_stack_* is plane-scoped per call
    unit_wx = _UNITS[itr_units1].wx;
    unit_wy = _UNITS[itr_units1].wy;

    // Stage-point count: independent of landmass match (see flag below)
    if((cp_landmass_wx_array[landmass_idx] == unit_wx) && (cp_landmass_wy_array[landmass_idx] == unit_wy))
    {
        cp_staged_unit_count++;
    }

    if(_landmasses[((unit_wp * WORLD_SIZE) + (unit_wy * WORLD_WIDTH) + unit_wx)] != landmass_idx) continue;

    // Find an existing stack at this unit's exact (wx, wy, wp)
    stack_idx = ST_UNDEFINED;
    for(itr_stacks2 = 0; itr_stacks2 < _ai_own_stack_count; itr_stacks2++)
    {
        if(_ai_own_stack_wx[itr_stacks2] == unit_wx
           && _ai_own_stack_wy[itr_stacks2] == unit_wy
           && _ai_own_stack_wp[itr_stacks2] == unit_wp)
        {
            stack_idx = itr_stacks2;
        }
    }

/*
    BEGIN:  Existing or New
*/
    if(stack_idx != ST_UNDEFINED)
    {
        // Append unit to existing stack
        if(_UNITS[itr_units1].Status != us_GOTO && != us_Purify && != us_BuildRoad)
        {
            // *free*
            _ai_own_stack_unit_list[stack_idx][_ai_own_stack_unit_count[stack_idx]] = itr_units1;
        }
        else
        {
            // *busy* — consume the slot immediately
            _ai_own_stack_unit_list[stack_idx][_ai_own_stack_unit_count[stack_idx]] = ST_UNDEFINED;
            if(_UNITS[itr_units1].dst_wx == cp_landmass_wx_array[landmass_idx]
               && _UNITS[itr_units1].dst_wy == cp_landmass_wy_array[landmass_idx])
            {
                cp_enroute_unit_count++;
            }
        }
        _ai_own_stack_unit_count[stack_idx]++;
    }
    else
    {
        // Create new stack
        _ai_own_stack_wx[_ai_own_stack_count] = (unsigned char)unit_wx;
        _ai_own_stack_wy[_ai_own_stack_count] = (unsigned char)unit_wy;
        _ai_own_stack_wp[_ai_own_stack_count] = (unsigned char)unit_wp;
        _ai_own_stack_unit_count[_ai_own_stack_count] = 1;
        if(_UNITS[itr_units1].Status != us_GOTO && != us_Purify && != us_BuildRoad)
        {
            _ai_own_stack_unit_list[_ai_own_stack_count][0] = itr_units1;
        }
        else
        {
            _ai_own_stack_unit_list[_ai_own_stack_count][0] = ST_UNDEFINED;
            if(_UNITS[itr_units1].dst_wx == cp_landmass_wx_array[landmass_idx]
               && _UNITS[itr_units1].dst_wy == cp_landmass_wy_array[landmass_idx])
            {
                cp_enroute_unit_count++;
            }
        }

        // Classify the new stack
        /* weird but clever way to check if a stack is effectively a garrison — it's on a City, Node, or Tower */
        if(g_ai_evaluation_map[unit_wp][((unit_wy * WORLD_WIDTH) + unit_wx)] != 0)
        {
            if(_FORTRESSES[player_idx].wx == unit_wx
               && _FORTRESSES[player_idx].wy == unit_wy
               && _FORTRESSES[player_idx].wp == unit_wp)
            {
                _ai_own_stack_type[_ai_own_stack_count] = AISTK_FortressGarrison;
            }
            else
            {
                _ai_own_stack_type[_ai_own_stack_count] = AISTK_Garrison;
            }
        }
        else
        {
            _ai_own_stack_type[_ai_own_stack_count] = AISTK_Roamer;
        }
        _ai_own_stack_count++;
    }
/*
    END:  Existing or New
*/
}
```

**Intent:** for each unit owned by `player_idx` on this `(wp, landmass_idx)`:
1. Increment `cp_staged_unit_count` if the unit sits on the landmass's stage point (regardless of which landmass the unit's actually on — see flag below).
2. Skip units that aren't on this landmass.
3. Look for an existing stack at the unit's exact square. If found, append (and increment the stack's unit_count); if not, create a new stack.
4. Status-classify the unit's slot:
   - **Free** (`us_Ready`, `us_Move`, others) → write `unit_idx` into the slot.
   - **Busy** (`us_GOTO`, `us_Purify`, `us_BuildRoad`) → consume the slot immediately (write `ST_UNDEFINED`), and if the unit's destination matches the stage point, bump `cp_enroute_unit_count`.
5. For NEW stacks only, classify the stack itself by looking at `g_ai_evaluation_map[wp][square]`:
   - **Non-zero** → there's a site here (city/node/tower per the inline comment). Either `AISTK_FortressGarrison` (our fortress) or `AISTK_Garrison` (other site).
   - **Zero** → no site. `AISTK_Roamer`.

The plane-filter at line 2859 (`unit_wp != wp` skip) is what makes `_ai_own_stack_*` **plane-scoped per call** — every downstream dispatch slot (which is also called per-plane) can rely on this scoping.

### Inline notes in source

- Line 2955 — `weird but clever way to check if a stack is effectively a garrison`: the `g_ai_evaluation_map[wp][square] != 0` check is used as a proxy for "this square has a site." Implicit assumption is that own-unit squares have value 0 in the eval map (the eval map encodes hostile presence + site flags only).
- Line 2958: `/* Is the *site* our Fortress City? */` — the FortressGarrison vs Garrison distinction.
- Line 2986: `/* IDA: Ocean Blue color#14 */` — IDA Pro color tag for the Phase 3 block.

## Phase 3 — Order moves for roamer stacks already in `us_Move` (lines 2985-3033)

```c
for(itr_stacks1 = 0; itr_stacks1 < _ai_own_stack_count; itr_stacks1++)
{
    if (_ai_own_stack_type[itr_stacks1] != AISTK_Roamer)
    {
        continue;
    }
    first_unit_idx = _ai_own_stack_unit_list[itr_stacks1][0];
    if(first_unit_idx == ST_UNDEFINED || _UNITS[first_unit_idx].Status != us_Move)
    {
        if((_ai_own_stack_unit_count[itr_stacks1] + 1) < 3)  /* NOTE(JimBalcomb,20260522): double-checked, this really does weirdly test +1<3 */
        {
            _ai_own_stack_type[itr_stacks1] = AISTK_Unknown;
        }
    }
    else  /* (first_unit_idx != ST_UNDEFINED) && (Status == us_Move) */
    {
        // First unit is in us_Move — try to find an enemy to attack
        if(AI_Stacks_Target_Nearest_Hostile_Stack(itr_stacks1, landmass_idx, &target_wx, &target_wy, wp) == ST_TRUE)
        {
            for(itr_units2 = 0; _ai_own_stack_unit_count[itr_stacks1] > itr_units2; itr_units2++)
            {
                g_ai_set_target_caller = 11;
                AI_Stacks_Order_Attack_Target_Or_Goto_Destination(_ai_own_stack_unit_list[itr_stacks1][itr_units2], target_wx, target_wy, itr_stacks1, itr_units2);
            }
        }
        else
        {
            // No target found — release the stack to us_Ready and mark InTransit
            for(itr_units2 = 0; _ai_own_stack_unit_count[itr_stacks1] > itr_units2; itr_units2++)
            {
                _UNITS[_ai_own_stack_unit_list[itr_stacks1][itr_units2]].Status = us_Ready;
            }
            _ai_own_stack_type[itr_stacks1] = AISTK_Unknown;
        }
    }
}
```

**Intent:** for each Roamer stack:
- If its first unit was consumed (`ST_UNDEFINED` in slot 0) or its first unit is NOT in `us_Move`, AND the stack is small (`count + 1 < 3`, i.e. count is 0 or 1), mark it `AISTK_Unknown`. Small stragglers get parked.
- Otherwise (first unit is in `us_Move`), this stack is actively moving — call [`AI_Stacks_Target_Nearest_Hostile_Stack`](../../MoM/src/AIMOVE.c#L3240) to find a nearby hostile free-roaming stack on the same landmass:
  - If found, issue every unit in the stack a move/goto order toward those coords (caller breadcrumb `g_ai_set_target_caller = 11`). `AI_Stacks_Order_Attack_Target_Or_Goto_Destination` then consumes each slot — so post-Phase-3, the affected stack's unit list becomes fully `ST_UNDEFINED`.
  - If no target found, release every unit to `us_Ready` and mark the stack `AISTK_Unknown`.

The `(count + 1) < 3` check is **faithful-to-OG** (verified against disassembly per the inline note at line 3002) — equivalent to `count < 2`. Stacks of 0 or 1 units get parked; larger stacks-without-us_Move-leader stay as Roamer for later dispatch slots to handle.

See [AIMOVE-AI_Stacks_Target_Nearest_Hostile_Stack](../../MoM/src/AIMOVE.c#L3240) for the target-finder's filter (`STRENGTH_MASK != 0 && !NONHOSTILE && !SITE` = hostile free-roaming stack on this landmass).

### Side observations

- **`cp_staged_unit_count` increment is BEFORE the landmass filter ([lines 2865-2873](../../MoM/src/AIMOVE.c#L2865-L2873)).** A unit at the stage coords gets counted even if `_landmasses[unit_square] != landmass_idx`. In practice the stage square IS on this landmass (the stage point is chosen from this landmass's chains), so this is mostly benign — but if landmass classification is mid-update or near a boundary, you could over-count.
- **`MAX_AI_STACKS` cap silently drops units ([line 2851](../../MoM/src/AIMOVE.c#L2851)).** If we hit the stack-count limit, ALL subsequent units in `_UNITS[]` are skipped — including `cp_staged_unit_count` / `cp_enroute_unit_count` increments and stack classification. Overflow protection by truncation. Probably fine because `MAX_AI_STACKS = 80` is large for a per-plane-per-landmass scope.

## Call graph

```
AI_Set_Unit_Orders                                          [AIMOVE.c:131, Phase 4 dispatch]
  └─ per (plane, landmass):
       └─ AI_Stacks_Init_Build_Target_Order_idx, landmass_idx, wp)   [slot 1 — THIS FUNCTION]
            ├─ Phase 1: zero all _ai_own_stack_* arrays
            ├─ Phase 2: per unit in _UNITS[]:
            │    ├─ plane-filter (unit_wp != wp → skip)
            │    ├─ stage-count + landmass-filter
            │    ├─ find or create stack
            │    │    └─ if new stack: classify via g_ai_evaluation_map[wp][square]:
            │    │         ├─ non-zero + own fortress → AISTK_FortressGarrison
            │    │         ├─ non-zero + other site   → AISTK_Garrison
            │    │         └─ zero                    → AISTK_Roamer
            │    └─ append: write unit_idx (or ST_UNDEFINED if busy), increment unit_count
            └─ Phase 3: per Roamer stack:
                 ├─ small/non-Move stack → AISTK_Unknown
                 └─ Move-status stack:
                      └─ AI_Stacks_Target_Nearest_Hostile_Stack
                           ├─ found → AI_Stacks_Order_Attack_Target_Or_Goto_Destination(every_unit, target)  [g_ai_set_target_caller = 11]
                           └─ none  → release all units to us_Ready, mark AISTK_Unknown
```

## Consumers (what reads the arrays this function builds)

Every subsequent slot of `AI_Set_Unit_Orders` Phase 4 dispatch consumes some part of `_ai_own_stack_*`:

| Slot | Function | Reads |
|---|---|---|
| 2 | `AI_Stacks_Move_Out_NonMilitary_Garrisoned` | type, unit_list (for Garrison+ stacks) |
| 3 | `AI_Stacks_Survey_Expedition_Forces` | type, unit_count, unit_list (pools excess units) |
| 4 | `AI_Stacks_Do_Meld` | type, unit_list |
| 5 | `AI_Stacks_Do_Settle` | type, unit_list |
| 6 | `AI_Stacks_Do_Purify` | type, unit_list |
| 7 | `AI_Do_RoadBuild` | type, unit_list |
| 8 | `AI_Build_Target_List` | (doesn't touch stack arrays) |
| 9 | [`AI_Stacks_Roamers_Target_Or_Deploy`](AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md) | type, unit_count, wx/wy, unit_list (for Roamer stacks) |
| 10 | `AI_Stacks_Order_To_War_Landmass` | type, unit_count, unit_list |
| 11 | `AI_Stacks_Relocate_Roamers` | type, wx/wy, unit_list |
| 13 | `AI_Stacks_Stage_Expedition_Forces` | type, unit_count, unit_list; also `cp_staged_unit_count` / `cp_enroute_unit_count` |
| 14 | `AI_Stacks_Garrison_Sites` | type, unit_count, unit_list |

Plus [`AI_Stacks_Assign_Target`](AIMOVE-AI_Stacks_Assign_Target.md) (called transitively from slot 9) reads `_ai_own_stack_unit_list` to sum stack effective strength.

## Related references

- [AIMOVE-AI_Set_Unit_Orders.md](AIMOVE-AI_Set_Unit_Orders.md) — dispatcher; this function is slot 1
- [AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md](AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md) — primary consumer of Roamer stacks
- [AIMOVE-AI_Stacks_Assign_Target.md](AIMOVE-AI_Stacks_Assign_Target.md) — transitively consumes unit_list via `_ai_own_stack_unit_count` and `_ai_own_stack_unit_list`
- [AIMOVE-AI_Build_Target_List.md](AIMOVE-AI_Build_Target_List.md) — separate per-landmass target list (the `_ai_targets_*` arrays), built by slot 8
- [MoX/src/MOM_DAT.c:2685-2706](../../MoX/src/MOM_DAT.c#L2685-L2706) — `_ai_own_stack_*` field comment block, including the "consume" idiom and the seven writers
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
