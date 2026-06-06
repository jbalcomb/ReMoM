AIMOVE-AI_Stacks_Do_RoadBuild.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Do_RoadBuild.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Order_RoadBuild.asm

AI_Stacks_Do_RoadBuild()
    |-> AI_Stacks_Order_RoadBuild()                             (unit at/adjacent to a city — build road)
    |-> AI_Stacks_Order_Attack_Target_Or_Goto_Destination()     (unit elsewhere — route to nearest city)

---

# AI_Stacks_Do_RoadBuild && AI_Stacks_Order_RoadBuild — Walkthrough

Two coupled functions: `AI_Stacks_Do_RoadBuild` is the per-landmass dispatcher that finds road-builder units (Engineers — `Construction > 0`) and points them at city-to-city road work; `AI_Stacks_Order_RoadBuild` is the order-issuer it calls to put a unit into `us_BuildRoad` toward a target city.

> **Naming:** the OG/IDA names are `AI_Do_RoadBuild` (o158p25) and `AI_Order_RoadBuild` (o158p28) — the disassembly files on disk keep those names. ReMoM renamed the production functions to `AI_Stacks_Do_RoadBuild` / `AI_Stacks_Order_RoadBuild` to match the `AI_Stacks_Do_Meld` / `AI_Stacks_Do_Purify` sibling convention.

**`AI_Stacks_Do_RoadBuild` location:** [MoM/src/AIMOVE.c:4444](../../MoM/src/AIMOVE.c#L4444) (~143 lines, ends [line 4587](../../MoM/src/AIMOVE.c#L4587)). **WZD overlay:** ovr158, p25. **drake178 name:** `AI_ProcessRoadBuilds()`. **GEMINI variant:** deleted at done-done.

**`AI_Stacks_Order_RoadBuild` location:** [MoM/src/AIMOVE.c:4707](../../MoM/src/AIMOVE.c#L4707) (7 lines, ends [line 4716](../../MoM/src/AIMOVE.c#L4716)). **WZD overlay:** ovr158, p28. **drake178 name:** `AI_UNIT_RoadBldOrder()`.

## Purpose

**Per-landmass ROAD-BUILDER dispatch.** Engineer-class units (`_unit_type_table[type].Construction > 0`) can lay roads between cities. For each such unit in the AI's own stacks, this routine:

1. finds the **nearest reachable city** on the unit's plane (gated by the stack's land/water mobility);
2. if the unit is **more than 1 square away** from that city, routes it toward the city (a move/goto order);
3. if it is **on or adjacent to** the city (`min_delta_distance <= 1`), finds the closest same-landmass city that is **not yet road-connected** to it and issues a build-road order toward that target.

So roads grow city-by-city: the engineer walks to a city, then builds toward the nearest unconnected neighbor on the same landmass.

## Signatures

```c
void AI_Stacks_Do_RoadBuild(int16_t landmass_idx);
void AI_Stacks_Order_RoadBuild(int16_t unit_idx, int16_t wx, int16_t wy, int16_t unit_list_idx, int16_t list_unit_idx);
```

`AI_Stacks_Do_RoadBuild` caller:
- [AIMOVE.c:284](../../MoM/src/AIMOVE.c#L284) — **slot 7** of the per-landmass dispatch inside [`AI_Set_Unit_Orders`](AIMOVE-AI_Set_Unit_Orders.md). Runs once per (player, plane, landmass).

`AI_Stacks_Order_RoadBuild` caller:
- [AIMOVE.c:4578](../../MoM/src/AIMOVE.c#L4578) — only from `AI_Stacks_Do_RoadBuild` (the at-city case).

## Inputs (globals read)

| Source | Phase | Used for |
|---|---|---|
| `_ai_own_stack_count` / `_ai_own_stack_unit_count[s]` / `_ai_own_stack_unit_list[s][i]` | P1 | stack/unit iteration |
| `_UNITS[unit_idx].{type, wx, wy, wp}` | P1 | road-builder ID + position |
| `_unit_type_table[type].Construction` | P1 | road-builder filter (`> 0`) |
| `_ai_all_own_stacks[itr_stacks].abilities` | P2 | `AICAP_LandOnly` mobility gate |
| `_cities` / `_CITIES[c].{wp, wx, wy, road_connections}` | P2, P4 | city scan + connection bitfield |
| `_landmasses[(wp*WORLD_SIZE)+(wy*WORLD_WIDTH)+wx]` | P2, P4 | landmass id at a city square |
| `Delta_XY_With_Wrap(...)` | P2, P4 | toroidal distance scorer |
| `Test_Bit_Field(bit_idx, field)` | P4 | "already road-connected?" check |
| `MAX_UNIT_COUNT` (= 1000) | Order | bounds check |

## Outputs (side effects)

- **Off-city unit** (`min_delta_distance > 1`): `g_ai_set_target_caller = 18;` then `AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, city.wx, city.wy, …)` — routes the unit toward the nearest city (writes `Status` us_Move/us_GOTO + `dst_wx/wy`, consumes the slot).
- **At-city unit with a road target**: `AI_Stacks_Order_RoadBuild(unit_idx, target.wx, target.wy, …)` — writes `Rd_From_X/Y` (= current square), `Status = us_BuildRoad`, `dst_wx/wy` (= target city), consumes the slot.

## Locals — `AI_Stacks_Do_RoadBuild`

```c
int16_t unused_variable_2 = 0;    /* dead — see note */
int16_t unused_variable_1 = 0;    /* dead — see note */
int16_t unit_wp = 0;             /* P1 unit plane cache */
int16_t unit_idx = 0;            /* P1 stack-slot cursor */
int16_t itr_list_units = 0;      /* P1 per-stack unit iterator */
int16_t list_unit_count = 0;     /* P1 per-stack unit count */
int16_t RoadTo_City = 0;         /* P4 chosen road-target city (sentinel ST_UNDEFINED) */
int16_t nearest_city_idx = 0;    /* P2 nearest city (sentinel ST_UNDEFINED) */
int16_t delta_distance = 0;      /* P2/P4 candidate distance */
int16_t city_landmass_idx = 0;   /* P4 landmass id at the nearest city */
int16_t min_delta_distance = 0;  /* P2/P4 running-min (sentinel 1000) */
int16_t dupe_landmass_idx = 0;   /* P2 copy of landmass_idx param (asm: Landmass_Dup) */
int16_t unit_wy = 0;             /* P1 unit y cache */
int16_t unit_wx = 0;             /* P1 unit x cache */
int16_t itr_stacks = 0;          /* P1 stack iterator */
int16_t city_idx = 0;            /* P2/P4 city iterator (asm: DI) */
```

**Dead `unused_variable_1/2`** ([lines 4471-4472](../../MoM/src/AIMOVE.c#L4471-L4472)) — the OG writes `0xD4EA` and `0xD526` into two adjacent stack words and never reads them. Resolved as **paragraph/segment values** (not data offsets): `0xD4EA:0 = 0xD4EA0` sits inside ovr145's loaded span, `0xD526:0 = 0xD5260` just past the next overlay — i.e. overlay segment addresses / far-pointer fixup leftovers. Written-never-read, so functionally inert; preserved faithfully (drake178 `DEDU could this be meaningful?`).

## Code walk — `AI_Stacks_Do_RoadBuild`

### Phase 1 — Road-builder filter ([lines 4475-4496](../../MoM/src/AIMOVE.c#L4475-L4496))

```c
for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
{
    list_unit_count = _ai_own_stack_unit_count[itr_stacks];
    for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
    {
        unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
        if((unit_idx != ST_UNDEFINED) && (_unit_type_table[_UNITS[unit_idx].type].Construction > 0))
        {
            unit_wx = _UNITS[unit_idx].wx;  unit_wy = _UNITS[unit_idx].wy;  unit_wp = _UNITS[unit_idx].wp;
            dupe_landmass_idx = landmass_idx;
            ...
```

Walks every unit slot in every own-stack; keeps only valid slots that are road-builders (`Construction > 0`). The `&&` short-circuits, so `_UNITS[unit_idx]` is only read when `unit_idx != ST_UNDEFINED` — **no out-of-bounds read** (unlike the sibling `AI_Stacks_Do_Meld` Phase 2). **IDA-confirmed** ([asm `loc_F0A03`/`loc_F0A19`](C:/STU/devel/STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr158/AI_Do_RoadBuild.asm): `cmp unit_idx, e_ST_UNDEFINED; jnz` then `cmp Construction, 0; jg`).

### Phase 2 — Find the nearest reachable city ([lines 4498-4526](../../MoM/src/AIMOVE.c#L4498-L4526))

```c
nearest_city_idx = ST_UNDEFINED;
min_delta_distance = 1000;
for(city_idx = 0; city_idx < _cities; city_idx++)
{
    if(_CITIES[city_idx].wp != unit_wp) { continue; }                       // same plane
    if( ((_ai_all_own_stacks[itr_stacks].abilities & AICAP_LandOnly) == 0)  // walker AND
        && (_landmasses[((unit_wp*WORLD_SIZE)+(_CITIES[city_idx].wy*WORLD_WIDTH)+_CITIES[city_idx].wx)] != dupe_landmass_idx) )  // different landmass
    { continue; }                                                          //   → unreachable, skip
    delta_distance = Delta_XY_With_Wrap(unit_wx, unit_wy, _CITIES[city_idx].wx, _CITIES[city_idx].wy, WORLD_WIDTH);
    if(delta_distance < min_delta_distance) { min_delta_distance = delta_distance; nearest_city_idx = city_idx; }
}
```

Same-plane filter, then a **mobility gate**, then nearest-by-toroidal-distance. **IDA-confirmed** (`loc_F0A90` plane, `loc_F0AAA` mobility, `loc_F0B07` distance).

**The mobility gate and the misleading flag name.** `AICAP_LandOnly` is **named backwards from what it stores**: the bit is **set** for units that can leave land (air/water travel) and **clear** for pure land walkers. It is built by initializing the stack to `AICAP_LandOnly` then XOR-clearing it for non-air/non-water units ([AIDUDES.c:1425](../../MoM/src/AIDUDES.c#L1425) + [1483-1492](../../MoM/src/AIDUDES.c#L1483-L1492)). So `(abilities & AICAP_LandOnly) == 0` reads as **"is a land walker."** The gate therefore means: *skip the city if the unit is a walker AND the city is on a different landmass* — a walker can't cross water, so it's restricted to its own landmass; air/water-capable stacks keep every same-plane city. The inline `¿ OGBUG should be walker && can't walk there ?` at [line 4509](../../MoM/src/AIMOVE.c#L4509) asks exactly this — and the answer is yes, it already is that, once you account for the inverted flag name.

**B1 — parameter mismatch (OG-faithful, [comments at 4502-4503](../../MoM/src/AIMOVE.c#L4502-L4503)).** The landmass lookup indexes `_landmasses` with the **unit's** plane (`unit_wp`) glued onto the **city's** `(wy, wx)`; it should use the city's own plane (`_CITIES[city_idx].wp`). The preceding `wp == unit_wp` guard forces them equal, so it computes the right square anyway — which is also why drake178 calls that plane guard "redundant" (a correct landmass lookup would filter on its own). Preserved as-is; the OG `AI_Do_RoadBuild.asm` does the same (`loc_F0AAA`, `mov al,[es:bx]` of `_landmasses` on `unit_wp`).

### Phase 3 — Bail if no city ([lines 4527-4530](../../MoM/src/AIMOVE.c#L4527-L4530))

```c
if(nearest_city_idx == ST_UNDEFINED) { continue; }
```

No reachable city for this unit → next slot. **IDA-confirmed** (`loc_F0B5D`).

### Phase 4 — Dispatch: walk-to-city or build-road ([lines 4532-4581](../../MoM/src/AIMOVE.c#L4532-L4581))

```c
if(min_delta_distance > 1)            // not yet at the city → go there
{
    g_ai_set_target_caller = 18;
    AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, _CITIES[nearest_city_idx].wx, _CITIES[nearest_city_idx].wy, itr_stacks, itr_list_units);
}
else                                  // on/adjacent to the city → pick a road target
{
    RoadTo_City = ST_UNDEFINED;  min_delta_distance = 1000;
    city_landmass_idx = _landmasses[((unit_wp*WORLD_SIZE)+(_CITIES[nearest_city_idx].wy*WORLD_WIDTH)+_CITIES[nearest_city_idx].wx)];
    for(city_idx = 0; city_idx < _cities; city_idx++)
    {
        if(city_idx == nearest_city_idx) { continue; }                                   // not itself
        if(_CITIES[city_idx].wp != unit_wp) { continue; }                                // same plane
        if(Test_Bit_Field(city_idx, (uint8_t *)&_CITIES[nearest_city_idx].road_connections[0]) != 0) { continue; }  // not already connected
        if(_landmasses[((unit_wp*WORLD_SIZE)+(_CITIES[city_idx].wy*WORLD_WIDTH)+_CITIES[city_idx].wx)] != city_landmass_idx) { continue; }  // same landmass
        delta_distance = Delta_XY_With_Wrap(_CITIES[nearest_city_idx].wx, _CITIES[nearest_city_idx].wy, _CITIES[city_idx].wx, _CITIES[city_idx].wy, WORLD_WIDTH);
        if(delta_distance < min_delta_distance) { min_delta_distance = delta_distance; RoadTo_City = city_idx; }
    }
    if(RoadTo_City != ST_UNDEFINED) { AI_Stacks_Order_RoadBuild(unit_idx, _CITIES[RoadTo_City].wx, _CITIES[RoadTo_City].wy, itr_stacks, itr_list_units); }
}
```

**Phase 4a — off-city** (`> 1`): route toward the nearest city via the shared goto-issuer [`AI_Stacks_Order_Attack_Target_Or_Goto_Destination`](../../MoM/src/AIMOVE.c#L4619) with `g_ai_set_target_caller = 18`. **IDA-confirmed** (`loc_F0B66`, OG calls `AI_Set_Move_Or_Goto_Target` — the OG name of that renamed helper).

**Phase 4b — at-city** (`<= 1`): scan for the nearest **same-landmass, not-yet-connected** city and issue the build order. All four guards are **IDA-confirmed** against the second city loop: `loc_F0C00` (skip the nearest itself), `loc_F0C08` (same plane), `loc_F0C22` (`Test_Bit_Field` → skip if already connected; `or ax,ax; jz proceed`), `loc_F0C48` (same landmass). `Test_Bit_Field(bit_idx, field)` arg order matches the prototype in [Util.h:90](../../MoX/src/Util.h#L90) and the asm push order.

## Code walk — `AI_Stacks_Order_RoadBuild`

```c
void AI_Stacks_Order_RoadBuild(int16_t unit_idx, int16_t wx, int16_t wy, int16_t unit_list_idx, int16_t list_unit_idx)
{
    if((unit_idx < 0) || (unit_idx >= MAX_UNIT_COUNT)) { return; }
    _UNITS[unit_idx].Rd_From_X = _UNITS[unit_idx].wx;   // remember where the road starts
    _UNITS[unit_idx].Rd_From_Y = _UNITS[unit_idx].wy;
    _UNITS[unit_idx].Status = us_BuildRoad;
    _UNITS[unit_idx].dst_wx = (int8_t)wx;               // target city
    _UNITS[unit_idx].dst_wy = (int8_t)wy;
    _ai_own_stack_unit_list[unit_list_idx][list_unit_idx] = ST_UNDEFINED;  // consume slot
}
```

Bounds-check, stamp the road's origin (`Rd_From_X/Y` = the unit's current square), set `us_BuildRoad`, store the target city as the destination, consume the stack slot. **IDA-confirmed OG-faithful** ([AI_Order_RoadBuild.asm](C:/STU/devel/STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr158/AI_Order_RoadBuild.asm): `or si,si; jl ret` + `cmp si,1000; jl proceed`, then the same field writes). OG hardcodes `1000`; production uses `MAX_UNIT_COUNT`.

## Bug catalog

| # | Where | Issue | Severity |
|---|---|---|---|
| B1 | [lines 4502-4503](../../MoM/src/AIMOVE.c#L4502-L4503) comments; gate at [4510-4513](../../MoM/src/AIMOVE.c#L4510-L4513) | Phase-2 landmass lookup uses `unit_wp` with the city's `(wy,wx)` instead of `_CITIES[city_idx].wp`; the explicit `wp == unit_wp` guard makes it land on the right square and is itself redundant given a correct lookup. **IDA-confirmed OG-faithful**, annotated by drake178. | OGBUG-faithful; harmless (guard compensates) |
| Ref | [line 4511](../../MoM/src/AIMOVE.c#L4511) | `AICAP_LandOnly` is **inverted from its name** — set = can leave land, clear = land walker ([AIDUDES.c:1425](../../MoM/src/AIDUDES.c#L1425)+[1483-1492](../../MoM/src/AIDUDES.c#L1483-L1492)). `== 0` is the "is a walker" test. Not a bug here — a naming caveat that has tripped reconstruction before. | reference note |
| Dead | [lines 4471-4472](../../MoM/src/AIMOVE.c#L4471-L4472) | `unused_variable_1/2` = overlay segment addresses (`0xD4EA`/`0xD526`), written never read. Faithful to OG. | inert; preserved |

`AI_Stacks_Order_RoadBuild` is OGBUG-clean.

## ASCII summary

```
AI_Stacks_Do_RoadBuild(landmass_idx)
  └─ for each own-stack, each unit slot:
       keep if  unit_idx != ST_UNDEFINED  AND  Construction > 0   (road-builder; correct order, no OOB)
       ├─ P2: nearest reachable city
       │      skip if  wp != unit_wp
       │      skip if  (walker)  AND  (different landmass)         [AICAP_LandOnly==0 == "is a walker"]
       │      [B1: landmass lookup uses unit_wp, not city.wp — guard compensates]
       │      track min distance → nearest_city_idx
       ├─ P3: if nearest == ST_UNDEFINED → next unit
       └─ P4: if min_dist > 1 :  Order_Attack/GOTO nearest city     [caller=18]
              else (at city)  :  scan for nearest same-landmass, unconnected city
                                   skip self / other-plane / already-connected / other-landmass
                                 → AI_Stacks_Order_RoadBuild(target)
                                     └─ Rd_From_X/Y = here; Status=us_BuildRoad; dst=target; consume slot
```

## Position in the dispatch chain

```
AI_Set_Unit_Orders(player_idx)
  └─ for wp in [0,1]:
       └─ for landmass_idx in [1, NUM_LANDMASSES):
            ├─ slot 4: AI_Stacks_Do_Meld     → AI_Stacks_Order_Meld
            ├─ slot 5: AI_Stacks_Do_Settle   → AI_Order_Settle
            ├─ slot 6: AI_Stacks_Do_Purify   → AI_Stacks_Order_Purify
            ├─ slot 7: AI_Stacks_Do_RoadBuild → AI_Stacks_Order_RoadBuild   ◄── HERE
            └─ ... slots 8-13 ...
```

Sibling per-job order-setter (same scan→find→issue shape as slots 4-6). The order helper consumes the stack slot (`ST_UNDEFINED`) so later slots skip the unit.

## Related references

- [AIMOVE-AI_Set_Unit_Orders.md](AIMOVE-AI_Set_Unit_Orders.md) — dispatcher; calls this as slot 7
- [AIMOVE-AI_Stacks_Do_Purify.md](AIMOVE-AI_Stacks_Do_Purify.md) — sibling slot-6 dispatcher (same shape)
- [AIMOVE-AI_Do_Meld.md](AIMOVE-AI_Do_Meld.md) — sibling slot-4 dispatcher
- [MoM-AI-Move-ai_own_stack.md](MoM-AI-Move-ai_own_stack.md) — `_ai_own_stack_*` / `_ai_all_own_stacks` (`AICAP_*` ability bits)
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Do_RoadBuild.asm` — IDA Pro 5.5 disassembly (ground truth, verified)
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Order_RoadBuild.asm` — IDA Pro 5.5 disassembly (ground truth, verified)
