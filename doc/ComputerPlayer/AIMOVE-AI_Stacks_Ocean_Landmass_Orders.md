AIMOVE-AI_Stacks_Ocean_Landmass_Orders.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\G_AI_ProcessTransports__WIP.c
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\G_AI_ProcessTransports__WIP.asm

---

# AI_Stacks_Ocean_Landmass_Orders — Walkthrough

**Location:** [MoM/src/AIMOVE.c:1119](../../MoM/src/AIMOVE.c#L1119) (~490 lines, ends [line 1608](../../MoM/src/AIMOVE.c#L1608)).
**WZD overlay:** ovr158, p05
**drake178 name:** `G_AI_ProcessTransports()`

## Purpose

Per-(player, plane) **post-pass** — sibling of [`AI_Stacks_Wartime_Ocean_Movement_And_Cleanup`](AIMOVE-AI_Stacks_Wartime_Ocean_Movement_And_Cleanup.md), runs immediately after it at the same per-plane scope (not a 1-13 slot).

Five phases in source order:

1. **Init flag** — `stack_has_goto = ST_FALSE` (re-initialized per-stack in Phase 5a; this top-level init is effectively dead).
2. **Ferry-queue capacity accounting** — debit each `_ai_ferry_*` queue entry by the stack-slot footprint of any en-route transport heading to it; invalidate entries that now have zero/negative remaining capacity.
3. **Ocean stack rebuild** — call [`AI_Stacks_Init_Build_Target_Order(player_idx, 0, wp)`](AIMOVE-AI_Stacks_Init_Build_Target_Order.md) to repopulate `_ai_own_stack_*` for the OCEAN (landmass 0).
4. **Out-of-band meld** — [`AI_Do_Meld(player_idx)`](../../MoM/src/AIMOVE.c#L4281) (also called in per-landmass slots 4-7; running it here on the ocean-stack context is non-obvious).
5. **Per-ocean-stack transport dispatch** — for each ocean stack containing at least one transport, classify and dispatch via a multi-branch decision tree based on (`stack_has_only_boats`, `stack_has_settler`, `stack_has_goto`) and landing-tile availability.

Consumer of the `_ai_ferry_*` list written by [`AI_Stacks_Ferry_Add_Location`](AIMOVE-AI_Stacks_Ferry_Add_Location.md) via [`AI_Stacks_Setup_Ferry`](AIMOVE-AI_Stacks_Setup_Ferry.md)'s patient path.

## Signature

```c
void AI_Stacks_Ocean_Landmass_Orders(int16_t player_idx, int16_t wp)
```

Called from:
- Production dispatcher: [AIMOVE.c:316](../../MoM/src/AIMOVE.c#L316) — immediately after `AI_Stacks_Wartime_Ocean_Movement_And_Cleanup`
- GEMINI dispatcher: [AIMOVE.c:418](../../MoM/src/AIMOVE.c#L418)

Always called per (player, plane). No early-return gate — always runs at least Phases 1-4. Phase 5 sub-branches gate on `_ai_landmass_war_targets`/`_ai_landmass_settler_targets` internally.

## Inputs (globals read)

| Source | Fields |
|---|---|
| `_units` | Loop bound for Phase 2b unit scan and Phase 5a classification scan |
| `_UNITS[itr].{Status, owner_idx, wp, wx, wy, dst_wx, dst_wy, type}` | Phase 2b en-route filter; Phase 5a stack-composition classification |
| `_unit_type_table[type].{Transport, Abilities}` | Phase 2b transport-capacity lookup; Phase 5a `stack_has_boat`/`stack_has_only_boats`/`stack_has_settler` flags |
| `_ai_ferry_count` | Loop bound for Phase 2b/2c/5c ferry-queue scan |
| `_ai_ferry_wx_array[i]`, `_ai_ferry_wy_array[i]`, `_ai_ferry_wp_array[i]` | Per-ferry-entry pickup-point coords; `wp_array` doubles as validity sentinel (-1 = `ST_UNDEFINED`) |
| `_ai_own_stack_count` | Phase 5 stack loop bound (refreshed by Phase 3's `AI_Stacks_Init_Build_Target_Order` call) |
| `_ai_own_stack_wx[s]`, `_ai_own_stack_wy[s]` | Phase 5a stack position |
| `_ai_own_stack_unit_count[s]`, `_ai_own_stack_unit_list[s][u]` | Phase 5c/5h/5i/5j per-stack unit dispatch |
| `_landmasses[wp*WORLD_SIZE + wy*WORLD_WIDTH + wx]` | Phase 5e adjacent-land scan; Phase 5f war-landmass match; Phase 5g settler-target match; Phase 5i/5j adjacent-ocean search |
| `g_ai_evaluation_map[wp][wy*WORLD_WIDTH + wx]` | Phase 5e adjacent-empty-tile check; Phase 5j dock-square empty check |
| `_cities`, `_CITIES[c].{wp, wx, wy, owner_idx}` | Phase 5f own/allied-city presence check on adjacent landmass |
| `_players[player_idx].Dipl.Dipl_Status[other]` | Phase 5f alliance check (`DIPL_Alliance`) |
| `_ai_landmass_war_targets[wp][player_idx]` | Phase 5f war-target match; Phase 5j war-landing dispatch gate |
| `_ai_landmass_settler_targets[wp][player_idx]`, `_ai_landmass_settler_targets_wx_array`, `_ai_landmass_settler_targets_wy_array` | Phase 5g settler-target match; Phase 5i settler-target dispatch |
| `_ai_landmass_dock_squares_heads[wp][lmt_idx]`, `_ai_landmass_dock_squares_wx_array`, `_ai_landmass_dock_squares_wy_array`, `_ai_landmass_dock_squares_lists` | Phase 5j dock-square selection (linked-list head + per-tile coords + next-link) |
| `AI_Enemy_Unit_In_Range(stack_wx, stack_wy, wp, 5, player_idx, adjacent_landmass_idx)` | Phase 5g enemy-proximity gate on settler landing |
| `Delta_XY_With_Wrap(...)` | Toroidal distance helper (Phase 5c ferry-queue scoring, Phase 5j dock scoring) |
| `MAX_AI_FERRIES` (= 15), `MAX_STACK` (= 9), `WORLD_WIDTH`, `WORLD_SIZE` | Constants |

## Outputs (side effects)

- **Phase 2c** writes `_ai_ferry_wp_array[i] = ST_UNDEFINED` for ferry-queue entries whose capacity reached ≤0 after Phase 2b debiting.
- **Phase 3** calls [`AI_Stacks_Init_Build_Target_Order(player_idx, 0, wp)`](../../MoM/src/AIMOVE.c#L1208) — repopulates `_ai_own_stack_*` for the OCEAN.
- **Phase 4** calls [`AI_Do_Meld(player_idx)`](../../MoM/src/AIMOVE.c#L1212) — out-of-band meld dispatch.
- **Phase 5c** ferry-queue dispatch — `AI_Stacks_Order_Attack_Target_Or_Goto_Destination(...)` with `g_ai_set_target_caller = 3` per packed transport; invalidates the targeted ferry-queue entry when filled.
- **Phase 5h** landing dispatch — transports get [`AI_Stacks_Order_Ferry`](AIMOVE-AI_Stacks_Order_Ferry.md) (writes `Status = us_Ferry`), non-transports get `AI_Stacks_Order_Attack_Target_Or_Goto_Destination` with `caller = 4`.
- **Phase 5i** settler-fallback dispatch — `AI_Stacks_Order_Attack_Target_Or_Goto_Destination` for every unit with `caller = 5`.
- **Phase 5j** war-landing dispatch — `AI_Stacks_Order_Attack_Target_Or_Goto_Destination` for every unit with `caller = 6`.

## Locals

```c
int16_t Transport_Spaces[MAX_AI_FERRIES] = { 0, ... };  /* Phase 2a inited to MAX_STACK per ferry slot; Phase 2b debit accumulator; Phase 5c capacity tracker */
int16_t landmass_has_own_city = 0;      /* Phase 5f adjacent-landmass own-city flag */
int16_t landmass_has_ally_city = 0;     /* Phase 5f adjacent-landmass allied-city flag */
int16_t stack_has_goto = 0;             /* Phase 5a flag — TRUE if any unit in stack has Status == us_GOTO (Phase 1's top-level init is dead — Phase 5a re-inits per-stack) */
int16_t adjacent_landmass_idx = 0;      /* Phase 5e/5f/5g — landmass index of last-found adjacent land tile */
int16_t Transport_Stack_Room = 0;       /* Phase 5c per-ferry-slot remaining capacity */
int16_t Target_Queue_Index = 0;         /* Phase 5c selected ferry-queue entry index */
int16_t unit_idx = 0;                   /* Phase 2b / 5a / 5c/5h/5i/5j inner unit iterator (also re-purposed as cached unit_list slot) */
int16_t itr_list_units = 0;             /* Phase 5c/5h/5i/5j inner unit-list iterator */
int16_t adjacent_landmass_wy = 0;       /* Phase 5e/5h target-tile y (LAND tile adjacent to stack); 5i/5j target-tile y (OCEAN tile adjacent to settler-target or dock) */
int16_t adjacent_landmass_wx = 0;       /* same — x */
int16_t stack_wy = 0;                   /* Phase 5 per-stack y cache */
int16_t stack_wx = 0;                   /* Phase 5 per-stack x cache */
int16_t Landing_Allowed = 0;            /* Phase 5e/5f/5g/5h gating flag */
int16_t unit_type = 0;                  /* Phase 5a / 5h unit-type cache */
int16_t stack_has_settler = 0;          /* Phase 5a flag — TRUE if stack contains any UA_CREATEOUTPOST unit */
int16_t stack_has_only_boats = 0;       /* Phase 5a flag — TRUE if every unit in stack has Transport > 0 */
int16_t stack_has_boat = 0;             /* Phase 5a flag — TRUE if stack contains any transport unit */
int16_t itr2 = 0;                       /* Phase 2b outer unit iterator */
int16_t list_unit_count = 0;            /* Phase 5a _ai_own_stack_unit_count[itr_stacks] cache */
int16_t landmass_node_index = 0;        /* Phase 5j dock-squares linked-list cursor */
int16_t wy_offset = 0;                  /* Phase 5e/5f/5g/5i/5j 3x3 scan offset */
int16_t wx_offset = 0;                  /* same — x */
int16_t delta_distance = 0;             /* Phase 5c/5j candidate distance */
int16_t min_delta_distance = 0;         /* Phase 5c/5j running-min distance */
int16_t target_wy = 0;                  /* Phase 5c/5j selected target y */
int16_t target_wx = 0;                  /* Phase 5c/5j selected target x */
int16_t itr_stacks = 0;                 /* Phase 5 stack iterator */
int16_t itr1 = 0;                       /* Phase 2a init / Phase 2b/c/5c ferry-queue iterator */
```

## Code walk

### Phase 1 — Init flag ([lines 1152-1153](../../MoM/src/AIMOVE.c#L1152-L1153))

```c
/* Phase 1: Init */
stack_has_goto = ST_FALSE;
```

Single statement. The variable is re-initialized per-stack in Phase 5a, so this top-level init is **effectively dead** — only matters if a compiler reads-before-write the uninitialized auto. Trivial; preserved for OG-faithfulness.

### Phase 2 — Ferry-queue capacity accounting ([lines 1156-1203](../../MoM/src/AIMOVE.c#L1156-L1203))

Three sub-passes.

#### Phase 2a — Init Transport_Spaces[] ([lines 1157-1161](../../MoM/src/AIMOVE.c#L1157-L1161))

Treats every ferry-queue entry as starting with `MAX_STACK = 9` stack-slot capacity at its pickup tile.

#### Phase 2b — Debit per en-route unit ([lines 1162-1193](../../MoM/src/AIMOVE.c#L1162-L1193))

For each own-plane us_GOTO unit whose destination matches a valid `_ai_ferry_*` pickup coord, debit `Transport_Spaces[ferry_idx]` by `(Transport + 1)`. The `+1` accounts for the transport's own stack slot at the pickup tile.

**B1 (drake178 OGBUG comment inline at line 1188):** the debit uses `_UNITS[itr1].type` where `itr1` is the FERRY-QUEUE iterator (0..14), not the unit iterator `itr2`. **IDA-confirmed OG-faithful** (asm 122-139: `mov ax, _DI_itr1` — uses itr1 directly). Net OG behavior: decrement by 1 per matching en-route unit (since `_UNITS[0..14].type.Transport` is almost always 0).

#### Phase 2c — Invalidate exhausted entries ([lines 1195-1203](../../MoM/src/AIMOVE.c#L1195-L1203))

Mark any ferry-queue entry whose remaining capacity is exhausted as invalid (`ST_UNDEFINED`). Phase 5c skips these.

### Phase 3 — Ocean stack rebuild ([line 1208](../../MoM/src/AIMOVE.c#L1208))

```c
AI_Stacks_Init_Build_Target_Order(player_idx, 0, wp);
```

Same slot-1 rebuild that [`AI_Stacks_Wartime_Ocean_Movement_And_Cleanup`](AIMOVE-AI_Stacks_Wartime_Ocean_Movement_And_Cleanup.md) Phase 4 already did this turn. This is the SECOND ocean rebuild in the per-plane post-pass — Wartime's Phase 5c Kill_Unit may have culled units that affect this re-snapshot.

### Phase 4 — Out-of-band meld ([line 1212](../../MoM/src/AIMOVE.c#L1212))

```c
AI_Do_Meld(player_idx);
```

`AI_Do_Meld` is also called per-landmass in slots 4-7. Running it here, on the ocean-stack-rebuild context, is non-obvious — melders normally meld nodes (land features). drake178 didn't comment. **Worth investigating during `AI_Do_Meld`'s walkthrough.**

### Phase 5 — Per-ocean-stack transport dispatch ([lines 1215-1597](../../MoM/src/AIMOVE.c#L1215-L1597))

Outer loop iterates every ocean stack:

```c
for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
{ ... }
```

Inside the loop, **10 distinct sub-blocks** — 5a through 5j. Phases 5b and 5d have been flattened to early-`continue` patterns; 5h/5i/5j remain in their original if/else-if/else cascade.

---

#### Phase 5a — Classify the stack ([lines 1219-1256](../../MoM/src/AIMOVE.c#L1219-L1256))

```c
stack_has_boat = ST_FALSE;
stack_has_only_boats = ST_TRUE;
stack_has_settler = ST_FALSE;
stack_has_goto = ST_FALSE;
list_unit_count = _ai_own_stack_unit_count[itr_stacks];
stack_wx = _ai_own_stack_wx[itr_stacks];
stack_wy = _ai_own_stack_wy[itr_stacks];

for(unit_idx = 0; unit_idx < _units; unit_idx++)
{
    if((_UNITS[unit_idx].wx == stack_wx) && (_UNITS[unit_idx].wy == stack_wy) && (_UNITS[unit_idx].wp == wp))
    {
        unit_type = _UNITS[unit_idx].type;
        if(_unit_type_table[unit_type].Transport > 0)
        {
            stack_has_boat = ST_TRUE;
        }
        else
        {
            stack_has_only_boats = ST_FALSE;
            if(_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST)
            {
                stack_has_settler = ST_TRUE;
            }
        }
        if(_UNITS[unit_idx].Status == us_GOTO)
        {
            stack_has_goto = ST_TRUE;
        }
    }
}
```

Reset 4 per-stack flags + cache `list_unit_count`/`stack_wx`/`stack_wy`. Then scan every unit in the world at the stack's (wx, wy, wp) and set:

- `stack_has_boat` — TRUE if any unit has `Transport > 0`
- `stack_has_only_boats` — starts TRUE; set FALSE if any non-transport unit found
- `stack_has_settler` — TRUE if any non-transport unit has `UA_CREATEOUTPOST`
- `stack_has_goto` — TRUE if any unit has `Status == us_GOTO`

Scan iterates `_units` (all world units) rather than `_ai_own_stack_unit_list[itr_stacks]`. Under slot 1's inclusion invariant (owner+plane+landmass only), the two enumerate the same set on the ocean.

---

#### Phase 5b — Gate: skip stacks without a transport ([lines 1259-1263](../../MoM/src/AIMOVE.c#L1259-L1263))

```c
if(stack_has_boat != ST_TRUE)
{
    continue;
}
```

Early-continue: skip the entire rest of the per-stack body for stacks with no transport. Structural invariant: the dispatch tree below only fires on stacks with at least one transport.

---

#### Phase 5c — All-transport stack: route to closest ferry queue ([lines 1265-1308](../../MoM/src/AIMOVE.c#L1265-L1308))

Two-step:

**Step 1 — Find closest valid ferry-queue entry** ([lines 1269-1283](../../MoM/src/AIMOVE.c#L1269-L1283)):

```c
min_delta_distance = 10000;  /* OG sentinel; user-flagged inconsistency with Phase 5j's 1000 */
for(itr1 = 0; itr1 < _ai_ferry_count; itr1++)
{
    if(_ai_ferry_wp_array[itr1] == wp)   /* skip ST_UNDEFINED-marked entries */
    {
        delta_distance = Delta_XY_With_Wrap(stack_wx, stack_wy, _ai_ferry_wx_array[itr1], _ai_ferry_wy_array[itr1], WORLD_WIDTH);
        if(delta_distance < min_delta_distance)
        {
            min_delta_distance = delta_distance;
            target_wx = _ai_ferry_wx_array[itr1];
            target_wy = _ai_ferry_wy_array[itr1];
            Target_Queue_Index = itr1;
        }
    }
}
```

`min_delta_distance = 10000` is the "no candidate" sentinel — toroidal distance maxes far below 10000. **IDA-confirmed OG-faithful** (asm line 1072: `mov [bp+closest_target_distance], 10000`). User has flagged the inconsistency with Phase 5j's `1000` sentinel inline at line 1269.

**Step 2 — Pack transports until capacity exhausted** ([lines 1285-1306](../../MoM/src/AIMOVE.c#L1285-L1306)):

```c
if(min_delta_distance < 10000)
{
    Transport_Stack_Room = Transport_Spaces[Target_Queue_Index];
    for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
    {
        if(Transport_Stack_Room > 0)
        {
            unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
            Transport_Stack_Room -= (_unit_type_table[_UNITS[unit_idx].type].Transport + 1);
            g_ai_set_target_caller = 3;
            AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, target_wx, target_wy, itr_stacks, itr_list_units);
        }
    }
    Transport_Spaces[Target_Queue_Index] = Transport_Stack_Room;
    if(Transport_Stack_Room <= 0)
    {
        _ai_ferry_wp_array[Target_Queue_Index] = ST_UNDEFINED;
    }
}
```

Pack transports from this stack into the chosen ferry slot until either the unit-list is exhausted or `Transport_Stack_Room <= 0`. Persist the new remaining capacity; invalidate the ferry entry if exhausted.

---

#### Phase 5d — Mixed-stack outer gate: skip if already going somewhere ([lines 1309-1316](../../MoM/src/AIMOVE.c#L1309-L1316))

```c
else  /* stack_has_boat but NOT stack_has_only_boats */
{
    if(stack_has_goto != ST_FALSE)
    {
        continue;
    }
    /* ... 5e through 5j ... */
}
```

The mixed-stack branch is `else` of 5c, and the early-`continue` here fires if any unit in the stack already has `Status == us_GOTO`. Presumption: don't disturb a stack already executing orders.

---

#### Phase 5e — Scan adjacent for a LAND tile ([lines 1318-1343](../../MoM/src/AIMOVE.c#L1318-L1343))

```c
adjacent_landmass_wx = 0;
adjacent_landmass_wy = 0;
Landing_Allowed = ST_FALSE;
for(wy_offset = -1; wy_offset < 2; wy_offset++)
{
    for(wx_offset = -1; wx_offset < 2; wx_offset++)
    {
        if(
            (_landmasses[((wp * WORLD_SIZE) + ((stack_wy + wy_offset) * WORLD_WIDTH) + (stack_wx + wx_offset))] != 0)   /* IS land */
            && (g_ai_evaluation_map[wp][(((stack_wy * WORLD_WIDTH) + wy_offset) + (stack_wx + wx_offset))] == 0)         /* empty per eval map */
        )
        {
            adjacent_landmass_idx = _landmasses[...];
            adjacent_landmass_wx = (stack_wx + wx_offset);
            adjacent_landmass_wy = (stack_wy + wy_offset);
            Landing_Allowed = ST_TRUE;
        }
    }
}
```

3x3 search around the stack. Looking for a LAND tile (`_landmasses != 0`) that is also empty per the eval map. If found, capture coords and set `Landing_Allowed = TRUE`. The variable name `adjacent_landmass_idx` now matches usage (was `Adjacent_Landmass`).

**B7 (drake178-flagged inline at lines 1328-1330):** bad arithmetic in `g_ai_evaluation_map` access — `((stack_wy * WORLD_WIDTH) + wy_offset) + (stack_wx + wx_offset)` instead of `((stack_wy + wy_offset) * WORLD_WIDTH) + (stack_wx + wx_offset)`. Computes a different square than intended. **IDA-confirmed OG-faithful** (asm 329-338).

**B8:** no range-check on the 3x3 indices — at world-edge stacks indices can underflow/overflow into adjacent rows or off the array. **IDA-confirmed OG-faithful.**

---

#### Phase 5f — Non-settler: war-landmass check + city check ([lines 1345-1411](../../MoM/src/AIMOVE.c#L1345-L1411))

Two checks in sequence, only if `stack_has_settler != ST_TRUE`:

**Check 1 — Re-scan 3x3 for war-landmass adjacency** ([lines 1355-1369](../../MoM/src/AIMOVE.c#L1355-L1369)):

```c
Landing_Allowed = ST_FALSE;
for(wy_offset = -1; wy_offset < 2; wy_offset++)
{
    for(wx_offset = -1; wx_offset < 2; wx_offset++)
    {
        if(_landmasses[...] == _ai_landmass_war_targets[wp][player_idx])
        {
            adjacent_landmass_idx = _landmasses[...];
            adjacent_landmass_wx = (stack_wx + wx_offset);
            adjacent_landmass_wy = (stack_wy + wy_offset);
            Landing_Allowed = ST_TRUE;
        }
    }
}
```

If a tile adjacent to the stack belongs to the main war landmass, set `Landing_Allowed = TRUE`. Otherwise, `Landing_Allowed` stays FALSE.

**Check 2 — Suppress landing if own/allied city on adjacent landmass** ([lines 1373-1410](../../MoM/src/AIMOVE.c#L1373-L1410)):

```c
if(Landing_Allowed == ST_FALSE)
{
    landmass_has_own_city = ST_FALSE;
    landmass_has_ally_city = ST_FALSE;
    for(itr1 = 0; itr1 < _cities; itr1++)
    {
        if((_CITIES[itr1].wp == wp) && (_landmasses[_CITIES tile] == adjacent_landmass_idx))
        {
            if(_CITIES[itr1].owner_idx == player_idx)         { landmass_has_own_city = ST_TRUE; }
            else if(_players[player_idx].Dipl.Dipl_Status[_CITIES[itr1].owner_idx] == DIPL_Alliance) { landmass_has_ally_city = ST_TRUE; }
        }
    }
}
if((landmass_has_own_city == ST_TRUE) || (landmass_has_ally_city == ST_TRUE))
{
    Landing_Allowed = ST_FALSE;
}
else
{
    Landing_Allowed = ST_TRUE;
}
```

If the war-landmass check failed, scan cities for own/allied presence on the `adjacent_landmass_idx`. Then set `Landing_Allowed` based on results: FALSE if own/allied cities present (don't land where you already have presence), TRUE otherwise.

**IDA-confirmed:** the own/allied city → FALSE gate IS intentional design (asm 686-695 explicitly sets TRUE in the else branch). "Don't waste landing on already-controlled territory" is OG's intent.

---

#### Phase 5g — Settler: settler-target check + enemy-proximity gate ([lines 1412-1451](../../MoM/src/AIMOVE.c#L1412-L1451))

In the `else` of `stack_has_settler != ST_TRUE`, so this fires when `stack_has_settler == ST_TRUE`. Two checks:

**Check 1 — Re-scan 3x3 for settler-target adjacency** ([lines 1418-1433](../../MoM/src/AIMOVE.c#L1418-L1433)):

```c
for(wy_offset = -1; wy_offset < 2; wy_offset++)
{
    for(wx_offset = -1; wx_offset < 2; wx_offset++)
    {
        if((_landmasses[...] == _ai_landmass_settler_targets[wp][player_idx])
           && _ai_landmass_settler_targets[wp][player_idx] != 0)
        {
            adjacent_landmass_idx = _landmasses[...];
            adjacent_landmass_wx = (stack_wx + wx_offset);
            adjacent_landmass_wy = (stack_wy + wy_offset);
        }
    }
}
```

Comparison value is `_ai_landmass_settler_targets` (per IDA asm 412-425 — uses `AI_NewColConts` which is the OG name). Sets coords WITHOUT touching `Landing_Allowed`.

**Check 2 — Enemy-proximity + landmass-type gate** ([lines 1435-1450](../../MoM/src/AIMOVE.c#L1435-L1450)):

```c
if(
    ((AI_Enemy_Unit_In_Range(stack_wx, stack_wy, wp, 5, player_idx, adjacent_landmass_idx) != ST_FALSE)
     || (_ai_continents.plane[wp].player[player_idx].type_array[adjacent_landmass_idx] != lmt_NoOwnCity))
    && (_ai_landmass_settler_targets[wp][player_idx] != adjacent_landmass_idx)
)
{
    Landing_Allowed = ST_FALSE;
}
else
{
    Landing_Allowed = ST_TRUE;
}
```

Set `Landing_Allowed = TRUE` iff "(safe AND on lmt_NoOwnCity landmass) OR (this IS the settler target)." OG-faithful (asm 463-510 verified).

---

#### Phase 5h — Landing dispatch ([lines 1460-1484](../../MoM/src/AIMOVE.c#L1460-L1484))

```c
if(Landing_Allowed == ST_TRUE)
{
    for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
    {
        /* OGBUG  MUST test unit_idx is ST_UNDEFINED */
        unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
        unit_type = _UNITS[unit_idx].type;
        if(_unit_type_table[unit_type].Transport > 0)
        {
            AI_Stacks_Order_Ferry(unit_idx, itr_stacks, itr_list_units);   /* ← B2 SUSPECT */
        }
        else
        {
            g_ai_set_target_caller = 4;
            AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, adjacent_landmass_wx, adjacent_landmass_wy, itr_stacks, itr_list_units);
        }
    }
}
```

For each unit in the stack:
- Transports → [`AI_Stacks_Order_Ferry`](AIMOVE-AI_Stacks_Order_Ferry.md) (writes `Status = us_Ferry`)
- Non-transports → GOTO `adjacent_landmass_wx/wy` (caller = 4)

**B2 (drake178-flagged at line 1465):** *"BUG? setting ships to seek transport?"*. `us_Ferry` semantically means "unit is waiting for / seeking a transport." Applying it to a transport ITSELF is backwards. **IDA-confirmed OG-faithful** (asm 729-735: `call AI_Order_SeekTransport`). Three interpretations possible — real OGBUG, design overload (`us_Ferry` doubles as "transport available next turn"), or unintended next-turn cascade. Worth deciding intent for the rename to `us_Ferry`.

**B11 (drake178-flagged inline at line 1468):** loop reads `_ai_own_stack_unit_list[...]` and dereferences `_UNITS[unit_idx].type` without checking for `ST_UNDEFINED` sentinel. If Phase 4's `AI_Do_Meld` consumed a slot, this reads `_UNITS[-1].type` (out-of-bounds). **IDA-confirmed OG-faithful** (asm 712-719).

---

#### Phase 5i — Settler-fallback dispatch ([lines 1488-1530](../../MoM/src/AIMOVE.c#L1488-L1530))

In the `else` of `Landing_Allowed == ST_TRUE`. Fires when settler-bearing stack couldn't land:

```c
if((stack_has_settler == ST_TRUE) && (_ai_landmass_settler_targets[wp][player_idx] != 0))
{
    /* Find OCEAN tile adjacent to settler-target stage point */
    adjacent_landmass_wx = 0;
    adjacent_landmass_wy = 0;
    for(wy_offset = -1; wy_offset < 2; wy_offset++)
    {
        for(wx_offset = -1; wx_offset < 2; wx_offset++)
        {
            if(_landmasses[settler_target_neighborhood] == 0)   /* IS ocean */
            {
                adjacent_landmass_wx = (wx_offset + _ai_landmass_settler_targets_wx_array[wp][player_idx]);
                adjacent_landmass_wy = (wy_offset + _ai_landmass_settler_targets_wy_array[wp][player_idx]);
                Landing_Allowed = ST_TRUE;   /* OGBUG  bogus line of code; c&p error? */
            }
        }
    }
    for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
    {
        unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
        g_ai_set_target_caller = 5;
        AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, adjacent_landmass_wx, adjacent_landmass_wy, itr_stacks, itr_list_units);
    }
}
```

Settler-bearing stack that couldn't land: find an ocean tile adjacent to the settler-target landmass's stage point, route the WHOLE stack (including transports) there. `caller = 5`.

**B12 (user OGBUG comment inline at line 1517 + dispatch-loop runs unconditionally):**
- The `Landing_Allowed = ST_TRUE` assignment inside the inner scan is **bogus** — `Landing_Allowed` is never read after this point in the function, so the assignment has no effect. drake-style c&p artifact. **IDA-confirmed OG-faithful** (asm 830: `mov [bp+Landing_Allowed], e_ST_TRUE` — preserved).
- The dispatch loop runs **unconditionally** even when the 3x3 scan found nothing — `adjacent_landmass_wx/wy` stay at 0 and the stack gets ordered to (0, 0). **IDA-confirmed OG-faithful** (asm 845-872: no gate on the dispatch loop).

---

#### Phase 5j — War-landing dock dispatch ([lines 1531-1588](../../MoM/src/AIMOVE.c#L1531-L1588))

In the `else` of `(stack_has_settler && settler_target)`. Fires for non-settler stacks (or settler stacks with no settler-target):

```c
if(_ai_landmass_war_targets[wp][player_idx] != 0)
{
    adjacent_landmass_wx = stack_wx;
    adjacent_landmass_wy = stack_wy;
    landmass_node_index = _ai_landmass_dock_squares_heads[wp][_ai_landmass_war_targets[wp][player_idx]];
    min_delta_distance = 1000;
    while(landmass_node_index != ST_UNDEFINED)
    {
        delta_distance = Delta_XY_With_Wrap(_ai_landmass_dock_squares_wx_array[wp][landmass_node_index], _ai_landmass_dock_squares_wy_array[wp][landmass_node_index], adjacent_landmass_wx, adjacent_landmass_wy, WORLD_WIDTH);
        if(delta_distance < min_delta_distance)
        {
            wx_offset = _ai_landmass_dock_squares_wx_array[wp][landmass_node_index];
            wy_offset = _ai_landmass_dock_squares_wy_array[wp][landmass_node_index];
            if(g_ai_evaluation_map[wp][((wy_offset * WORLD_WIDTH) + wx_offset)] == 0)
            {
                min_delta_distance = delta_distance;
                target_wx = wx_offset;
                target_wy = wy_offset;
            }
        }
        landmass_node_index = _ai_landmass_dock_squares_lists[wp][landmass_node_index];
    }

    if(min_delta_distance < 1000)
    {
        for(wy_offset = -1; wy_offset < 2; wy_offset++)
        {
            for(wx_offset = -1; wx_offset < 2; wx_offset++)
            {
                if(_landmasses[((wp * WORLD_SIZE) + ((target_wy + wy_offset) * WORLD_WIDTH) + (target_wx + wx_offset))] == 0)   /* IS ocean */
                {
                    adjacent_landmass_wx = (target_wx + wx_offset);
                    adjacent_landmass_wy = (target_wy + wy_offset);
                }
            }
        }
        for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
        {
            unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
            g_ai_set_target_caller = 6;
            AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, adjacent_landmass_wx, adjacent_landmass_wy, itr_stacks, itr_list_units);
        }
    }
}
```

Route the WHOLE stack toward the closest empty dock square on the main war landmass. `caller = 6`.

**Three sub-steps:**

1. **Pick closest empty dock square** — walk the dock-list chain (head from `_ai_landmass_dock_squares_heads[wp][war_landmass]`, advance via `_ai_landmass_dock_squares_lists[wp][cursor]`), compute distance for each, take the closest one that's empty per the eval map.
2. **Find adjacent ocean tile** — 3x3 around the target dock square; capture coords of any OCEAN tile.
3. **Dispatch all units in the stack** — GOTO with `caller = 6`.

## Bug catalog

All remaining entries are OG-faithful OGBUGs preserved per [feedback_faithful_dasm](../../C:/Users/jbalcomb/.claude/projects/c--STU-devel-ReMoM/memory/feedback_faithful_dasm.md). Reconstruction errors that diverged from OG (the former R1-R5, plus the dock-list `if`/`while` and the variable-collision dups in `AI_Stacks_Setup_Ferry`) have been corrected and removed from this catalog.

| # | Where | Issue | Severity |
|---|---|---|---|
| B1 | [Line 1190](../../MoM/src/AIMOVE.c#L1190) | Phase 2b debits Transport_Spaces using `_UNITS[itr1].type` — `itr1` is the ferry-queue index, not the unit index. Should be `itr2`. drake178 OGBUG comment inline at line 1188. **IDA-confirmed OG-faithful** (asm 122 uses `_DI_itr1`). OG net behavior: debit by 1 per matching en-route unit, type-independent (since `_UNITS[0..14].type.Transport` is almost always 0). | OGBUG-faithful; behavioral — ferry-capacity tracking unreliable |
| B2 | [Line 1473](../../MoM/src/AIMOVE.c#L1473) | Phase 5h lands a mixed stack: cargo → GOTO landing tile, **transports → `AI_Stacks_Order_Ferry` (writes `us_Ferry`)**. Semantically backwards. drake178 inline at line 1465: `BUG? setting ships to seek transport?`. **IDA-confirmed OG-faithful** (asm 729-735: `call AI_Order_SeekTransport`). | OGBUG-faithful; semantically suspect — `us_Ferry` on a transport may have unintended next-turn cascade |
| B7 | [Lines 1328-1334](../../MoM/src/AIMOVE.c#L1328-L1334) | Phase 5e `g_ai_evaluation_map` access uses bad arithmetic: `((stack_wy * WORLD_WIDTH) + wy_offset) + (stack_wx + wx_offset)` instead of `((stack_wy + wy_offset) * WORLD_WIDTH) + (stack_wx + wx_offset)`. **IDA-confirmed OG-faithful** (asm 329-338). | OGBUG-faithful; behavioral |
| B8 | [Lines 1332, 1361, 1423](../../MoM/src/AIMOVE.c#L1332) | Phase 5e/5f/5g 3x3 scans use unranged offsets — at world-edge stacks, indices can underflow/overflow. drake178 flagged multiple instances. **IDA-confirmed OG-faithful.** | OGBUG-faithful; behavioral edge case |
| B11 | [Line 1468](../../MoM/src/AIMOVE.c#L1468) | Phase 5h loop dereferences `_UNITS[unit_idx].type` without `ST_UNDEFINED` check. drake178 OGBUG comment inline. **IDA-confirmed OG-faithful** (asm 712-719). If Phase 4's `AI_Do_Meld` consumed a slot, reads `_UNITS[-1].type`. | OGBUG-faithful; potential OOB read |
| B12 | [Lines 1517, 1521-1529](../../MoM/src/AIMOVE.c#L1517) | Phase 5i: (a) the inner-scan `Landing_Allowed = ST_TRUE` assignment is bogus (never read); (b) the dispatch loop runs unconditionally even when no ocean tile found — `adjacent_landmass_wx/wy` stay 0, stack ordered to (0, 0). **IDA-confirmed OG-faithful** (asm 830, 845-872). User OGBUG comment inline at line 1517. | OGBUG-faithful; (0,0) dispatch on rare path |
| B-sentinel | [Line 1269 vs 1543](../../MoM/src/AIMOVE.c#L1269) | Phase 5c uses `min_delta_distance = 10000` as no-candidate sentinel; Phase 5j uses `1000`. Inconsistent. Both OG-faithful per asm. User flagged inline. | Stylistic; possibly intentional sizing for the different scan domains |

## ASCII summary

```
AI_Stacks_Ocean_Landmass_Orders(player_idx, wp)
  ├─ Phase 1: stack_has_goto = FALSE (top-level dead init)
  │
  ├─ Phase 2: Ferry-queue capacity accounting
  │    ├─ 2a (1157-1161): Transport_Spaces[i] = MAX_STACK for each ferry slot
  │    ├─ 2b (1162-1193): debit per matching en-route unit  [B1 wrong-index OGBUG]
  │    └─ 2c (1195-1203): invalidate exhausted ferry entries (set wp = ST_UNDEFINED)
  │
  ├─ Phase 3 (1208): AI_Stacks_Init_Build_Target_Order(player_idx, 0, wp)  /* OCEAN */
  ├─ Phase 4 (1212): AI_Do_Meld(player_idx)   /* odd — also called per-landmass */
  │
  └─ Phase 5: Per-ocean-stack dispatch (1215-1597)
       for each ocean stack:
       ├─ 5a (1219-1256): classify — stack_has_boat, stack_has_only_boats,
       │                  stack_has_settler, stack_has_goto
       ├─ 5b (1259-1263): if NOT stack_has_boat → continue
       │
       ├─ 5c (1265-1308): if stack_has_only_boats → ferry-queue dispatch  [caller=3]
       │       step 1: find closest valid ferry-queue entry
       │       step 2: pack transports until Transport_Stack_Room exhausted
       │
       └─ else (mixed stack with cargo):
            ├─ 5d (1313-1316): if stack_has_goto → continue
            │
            ├─ 5e (1318-1343): 3x3 scan for adjacent LAND tile  [B7 B8]
            │
            ├─ if Landing_Allowed:
            │    ├─ 5f (1349-1411): if !stack_has_settler:
            │    │       check 1: war-landmass adjacency → Landing_Allowed
            │    │       check 2: own/allied city → FALSE / else TRUE
            │    └─ 5g (1412-1451): if stack_has_settler:
            │            check 1: settler-target adjacency
            │            check 2: enemy-proximity + landmass-type gate
            │
            ├─ 5h (1460-1484): LAND DISPATCH if Landing_Allowed
            │       ├─ transports → AI_Stacks_Order_Ferry  [B2, B11]
            │       └─ others → ...Order_Attack_Target_Or_Goto_Destination  [caller=4]
            │
            └─ else (Landing_Allowed FALSE):
                 ├─ 5i (1488-1530): SETTLER-FALLBACK if stack_has_settler + settler-target
                 │       find ocean tile adjacent to settler-target stage point
                 │       all units → ...Order_Attack_Target_Or_Goto_Destination  [caller=5]
                 │                                                              [B12]
                 └─ 5j (1535-1586): WAR-LANDING (final fallback) if war-target set
                         step 1: walk dock-list chain, pick closest empty dock
                         step 2: 3x3 find OCEAN tile adjacent to dock
                         step 3: all units → ...Order_Attack_Target_Or_Goto_Destination
                                                                            [caller=6]
```

## Position in the dispatch chain

```
AI_Set_Unit_Orders(player_idx)
  └─ for wp in [0, 1]:                                # per-plane outer loop
       ├─ for landmass_idx in [1, NUM_LANDMASSES):     # per-landmass inner loop
       │    └─ slots 1-13 (see AI_Set_Unit_Orders walkthrough)
       │
       ├─ AI_Stacks_Wartime_Ocean_Movement_And_Cleanup(player_idx, wp)   (per-plane post-pass A)
       └─ AI_Stacks_Ocean_Landmass_Orders(player_idx, wp)                     ◄── HERE (per-plane post-pass B)
```

**Pairing with the sibling post-pass:**

- Wartime ([`AI_Stacks_Wartime_Ocean_Movement_And_Cleanup`](AIMOVE-AI_Stacks_Wartime_Ocean_Movement_And_Cleanup.md)) handles **non-transport** ocean units (pulls swimmers toward war stage, drowns stranded non-mobile stacks).
- This function handles **transport** ocean stacks (routes empty transports to ferry pickups, lands mixed stacks).

Both call `AI_Stacks_Init_Build_Target_Order(player_idx, 0, wp)` independently — the first rebuild is inside Wartime's Phase 4, the second is inside this function's Phase 3. Wartime's Phase 5c Kill_Unit may have culled units between the two rebuilds.

**`AI_Do_Meld` is called THREE times per (player, plane)** in the production flow: once via each per-landmass dispatch (slots 4-7 for each landmass), and AGAIN here at Phase 4 for the ocean-stack-rebuild context. The per-landmass pathway is documented; the ocean-rebuild meld call is non-obvious and worth investigating during `AI_Do_Meld`'s walkthrough.

## Related references

- [AIMOVE-AI_Set_Unit_Orders.md](AIMOVE-AI_Set_Unit_Orders.md) — dispatcher; this function is a per-plane post-pass, not a slot in 1-13
- [AIMOVE-AI_Stacks_Wartime_Ocean_Movement_And_Cleanup.md](AIMOVE-AI_Stacks_Wartime_Ocean_Movement_And_Cleanup.md) — sibling per-plane post-pass A
- [AIMOVE-AI_Stacks_Init_Build_Target_Order.md](AIMOVE-AI_Stacks_Init_Build_Target_Order.md) — Phase 3 calls it for ocean rebuild
- [AIMOVE-AI_Stacks_Setup_Ferry.md](AIMOVE-AI_Stacks_Setup_Ferry.md) — upstream producer of `_ai_ferry_*` queue entries via patient path
- [AIMOVE-AI_Stacks_Ferry_Add_Location.md](AIMOVE-AI_Stacks_Ferry_Add_Location.md) — writes `_ai_ferry_*` entries that Phase 2b/2c/5c consume
- [AIMOVE-AI_Stacks_Order_Ferry.md](AIMOVE-AI_Stacks_Order_Ferry.md) — Phase 5h callee for transports (B2 site)
- [AIMOVE-AI_Choose_War_Landmass.md](AIMOVE-AI_Choose_War_Landmass.md) — populates `_ai_landmass_war_targets[wp][player_idx]` that Phase 5f / 5j gate on
- [MoX/src/MOM_DAT.c:967](../../MoX/src/MOM_DAT.c#L967) — `_ai_ferry_wp_array` declaration with the note about `-1` meaning "no capacity"
- [MoM-AI-Move-ai_own_stack.md](MoM-AI-Move-ai_own_stack.md) — `_ai_own_stack_*` area-of-code reference
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\G_AI_ProcessTransports__WIP.asm` — IDA Pro 5.5 disassembly (ground truth, verified)
