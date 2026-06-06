AIMOVE-AI_Stacks_Do_Purify.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Do_Purify.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Order_Purify.asm

AI_Stacks_Do_Purify()
    |-> AI_Stacks_Order_Purify()                                (unit already on corrupted square)
    |-> AI_Stacks_Order_Attack_Target_Or_Goto_Destination()     (unit elsewhere — route to target)

---

# AI_Stacks_Do_Purify && AI_Stacks_Order_Purify — Walkthrough

Two coupled functions covered together: `AI_Stacks_Do_Purify` is the per-landmass dispatcher that decides whether the AI has any worth-purifying corruption and, if so, finds its Purify-capable units and points them at it; `AI_Stacks_Order_Purify` is the 4-line order-issuer it calls to set a purifier's `us_Purify` status when the unit is already standing on a corrupted square.

> **Naming:** the OG/IDA names are `AI_Do_Purify` (o158p24) and `AI_Order_Purify` (o158p31) — the disassembly files on disk keep those names. ReMoM renamed the production functions to `AI_Stacks_Do_Purify` / `AI_Stacks_Order_Purify` to match the `AI_Stacks_Do_Meld` / `AI_Stacks_Order_Meld` sibling convention.

**`AI_Stacks_Do_Purify` location:** [MoM/src/AIMOVE.c:4309](../../MoM/src/AIMOVE.c#L4309) (~106 lines, ends [line 4415](../../MoM/src/AIMOVE.c#L4415); Doxygen header at [lines 4279-4307](../../MoM/src/AIMOVE.c#L4279-L4307)). **WZD overlay:** ovr158, p24. **drake178 name:** `AI_ProcessPurifiers()`. **GEMINI variant:** deleted.

**`AI_Stacks_Order_Purify` location:** [MoM/src/AIMOVE.c:4772](../../MoM/src/AIMOVE.c#L4772) (4 lines, ends [line 4778](../../MoM/src/AIMOVE.c#L4778); Doxygen header at [lines 4750-4771](../../MoM/src/AIMOVE.c#L4750-L4771)). **WZD overlay:** ovr158, p31. **GEMINI variant:** none (never had one).

## Purpose

**Per-landmass PURIFIER dispatch.** Magic/corrupted terrain (`MSF_CORRUPTION`, set by Chaos/Death-realm effects such as *Corruption* and *Raise Volcano*) lowers a square's output. Units with the `UA_PURIFY` ability (the Engineer-class and certain summons) can spend turns standing on a corrupted square to clear the flag. `AI_Stacks_Do_Purify` is the meta-AI's decision to actually use those units.

The function runs in two stages:

1. **Scout** — walk the landmass's land-square list looking for a corrupted square that (a) is not itself already a hostile AI target and (b) sits within a 5×5 box of an `AI_TARGET_SITE`-flagged square (i.e. corruption that threatens something the AI cares about — a city or node area). The first such square becomes the single `target_wx/target_wy`. If none is found, the function returns without doing anything.
2. **Dispatch** — scan own stacks for purifier units. A purifier already standing on a corrupted square is told to `us_Purify` in place; a purifier elsewhere is routed toward `target_wx/target_wy`.

Note the asymmetry baked into the OG design: the scout phase finds **one** target and stops, but the dispatch phase orders **every** idle purifier. Multiple purifiers all converge on the same one square.

## Signatures

```c
void AI_Stacks_Do_Purify(int16_t landmass_idx, int16_t wp);
void AI_Stacks_Order_Purify(int16_t unit_idx, int16_t unit_list_idx, int16_t list_unit_idx);
```

`AI_Stacks_Do_Purify` caller:
- [AIMOVE.c:283](../../MoM/src/AIMOVE.c#L283) — **slot 6** of the per-landmass dispatch inside [`AI_Set_Unit_Orders`](AIMOVE-AI_Set_Unit_Orders.md). Runs once per (player, plane, landmass) → `NUM_PLANES × (NUM_LANDMASSES-1) = 2×98 = 196` invocations per AI player per turn. The scout phase short-circuits to an early return when no worth-purifying corruption exists, so the high call count is cheap.

`AI_Stacks_Order_Purify` caller:
- [AIMOVE.c:4390](../../MoM/src/AIMOVE.c#L4390) — only called from `AI_Stacks_Do_Purify` (the on-square case).

## Inputs (globals read)

| Source | Phase | Used for |
|---|---|---|
| `_ai_landmass_land_squares_heads[wp][landmass_idx]` | P1 | head of the land-square linked list (OG: `CONTX_FirstTiles`) |
| `_ai_landmass_land_squares_lists[wp][node]` | P1 | linked-list "next" pointer (OG: `CONTX_TileChain`) |
| `_ai_landmass_land_squares_wx_array[wp][node]` | P1 | square x at node (OG: `CONTX_TileXs`) |
| `_ai_landmass_land_squares_wy_array[wp][node]` | P1 | square y at node (OG: `CONTX_TileYs`) |
| `_map_square_flags[(wp*WORLD_SIZE)+(wy*WORLD_WIDTH)+wx]` | P1, P3 | `MSF_CORRUPTION` (`0x20`) test |
| `g_ai_evaluation_map[wp][wy*WORLD_WIDTH + wx]` | P1 | `AI_TARGET_STRENGTH_MASK` gate + `AI_TARGET_SITE` 5×5 scan |
| `_ai_own_stack_count` | P3 | outer stack-loop bound |
| `_ai_own_stack_type[itr_stacks]` | P3 | `AISTK_Unknown` (0) stack-type filter |
| `_ai_own_stack_unit_count[itr_stacks]` | P3 | per-stack unit count |
| `_ai_own_stack_unit_list[itr_stacks][list_unit_idx]` | P3 | unit-slot cursor |
| `_UNITS[unit_idx].{type, wx, wy}` | P3 | purifier ID + position |
| `_unit_type_table[type].Abilities` | P3 | `UA_PURIFY` (`0x4000`) test |

## Outputs (side effects)

- **On-square purifier** (unit's own square is corrupted): `AI_Stacks_Order_Purify(unit_idx, itr_stacks, list_unit_idx)` → sets `_UNITS[unit_idx].Status = us_Purify`, `_UNITS[unit_idx].dst_wx = 20`, and marks the stack slot `ST_UNDEFINED`.
- **Off-square purifier**: `g_ai_set_target_caller = 17;` then `AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, target_wx, target_wy, itr_stacks, list_unit_idx)` — routes the unit toward the corruption (writes `Status` us_Move/us_GOTO + `dst_wx`/`dst_wy` + consumes the slot).

## Locals — `AI_Stacks_Do_Purify`

```c
int16_t unit_idx = 0;            /* P3 _ai_own_stack_unit_list cursor */
int16_t list_unit_idx = 0;       /* P3 per-stack unit-list iterator */
int16_t list_unit_count = 0;     /* P3 per-stack unit count cache */
int16_t landmass_node_wy = 0;    /* P1 land-square y (asm: Tile_Y) */
int16_t landmass_node_wx = 0;    /* P1 land-square x (asm: Tile_X) */
int16_t wy_offset = 0;           /* P1 5×5 scan y offset (-2..2) */
int16_t wx_offset = 0;           /* P1 5×5 scan x offset (-2..2) */
int16_t target_wy = 0;           /* P1 chosen corruption y */
int16_t target_wx = 0;           /* P1 chosen corruption x */
int16_t landmass_node_index = 0; /* P1 linked-list cursor */
int16_t InRange_Corruption = 0;  /* P1 found-a-target flag (ST_FALSE/ST_TRUE) */
int16_t unit_wy = 0;             /* P3 purifier y cache */
int16_t unit_wx = 0;             /* P3 purifier x cache */
int16_t itr_stacks = 0;          /* P3 stack iterator (asm: DI) */
```

The asm copies `wp` into SI on entry and uses it throughout; production reads the `wp` parameter directly (never reassigned) — equivalent.

## Code walk — `AI_Stacks_Do_Purify`

### Phase 1 — Scout for worth-purifying corruption ([lines 4326-4357](../../MoM/src/AIMOVE.c#L4326-L4357))

```c
InRange_Corruption = ST_FALSE;
landmass_node_index = _ai_landmass_land_squares_heads[wp][landmass_idx];
while(landmass_node_index != ST_UNDEFINED)
{
    if(InRange_Corruption != ST_FALSE) { break; }

    landmass_node_wx = _ai_landmass_land_squares_wx_array[wp][landmass_node_index];
    landmass_node_wy = _ai_landmass_land_squares_wy_array[wp][landmass_node_index];

    if(   ((_map_square_flags[((wp*WORLD_SIZE)+(landmass_node_wy*WORLD_WIDTH)+landmass_node_wx)] & MSF_CORRUPTION) != 0)
       && ((g_ai_evaluation_map[wp][((landmass_node_wy*WORLD_WIDTH)+landmass_node_wx)] & AI_TARGET_STRENGTH_MASK) == 0) )
    {
        for(wy_offset = -2; wy_offset < 3; wy_offset++)
        {
            for(wx_offset = -2; wx_offset < 3; wx_offset++)
            {
                if(g_ai_evaluation_map[wp][(((wy_offset+landmass_node_wy)*WORLD_WIDTH)+(wx_offset+landmass_node_wx))] == AI_TARGET_SITE)
                {
                    InRange_Corruption = ST_TRUE;
                    target_wx = landmass_node_wx;
                    target_wy = landmass_node_wy;
                }
            }
        }
    }
    landmass_node_index = _ai_landmass_land_squares_lists[wp][landmass_node_index];
}
```

Walks the landmass's land-square linked list (head from `_ai_landmass_land_squares_heads`, chained through `_ai_landmass_land_squares_lists`). The `if(InRange_Corruption) break;` at the top means the loop stops at the **first** qualifying square — at most one target.

**Phase 1a — corruption gate** ([lines 4337-4341](../../MoM/src/AIMOVE.c#L4337-L4341)): take the square only if (a) it carries `MSF_CORRUPTION` **and** (b) `(eval & AI_TARGET_STRENGTH_MASK) == 0` — the eval-map cell has no hostile target-strength assigned, i.e. it is not itself an enemy target the combat dispatcher is already chasing. **IDA-confirmed OG-faithful** ([AI_Do_Purify.asm 62-75](C:/STU/devel/STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr158/AI_Do_Purify.asm): `test [msf], MSF_CORRUPTION; jz skip` then bitwise `test [eval], e_AI_TARGET_FORTRESS; jnz skip`). The asm's second test is bitwise (`test…jnz`), and production reproduces it as `(eval & MASK) == 0` against `AI_TARGET_STRENGTH_MASK` (`0x3FFF`) — i.e. production reads the IDA enum symbol `e_AI_TARGET_FORTRESS` as the strength mask. The operation matches the bytes; the mask identity rests on `e_AI_TARGET_FORTRESS == 0x3FFF` (there is deliberately no `AI_TARGET_FORTRESS` define in ReMoM, consistent with that re-identification).

**Phase 1b — 5×5 site-proximity scan** ([lines 4343-4354](../../MoM/src/AIMOVE.c#L4343-L4354)): if the square is qualifying corruption, scan the 5×5 box centered on it for any cell flagged exactly `AI_TARGET_SITE` (`0x8000`). A hit sets `InRange_Corruption = ST_TRUE` and records the **corruption** square (not the site square) as the target. **IDA-confirmed OG-faithful** (asm `loc_F0856`: `cmp [eval], e_AI_TARGET_SITE; jnz`, equality, and `Target_X/Y = Tile_X/Y`). No early-break inside the 5×5 — it scans all 25 cells; since every hit records the same center corruption square, the missing break is harmless.

### Phase 2 — Early return if nothing to do ([lines 4359-4362](../../MoM/src/AIMOVE.c#L4359-L4362))

```c
if(InRange_Corruption == ST_FALSE) { return; }
```

No qualifying corruption anywhere on the landmass → bail before touching any units. **IDA-confirmed** (asm `loc_F08C5`: `cmp InRange, e_ST_FALSE; jnz proceed; xor ax,ax; jmp Done`).

### Phase 3 — Dispatch purifiers ([lines 4364-4413](../../MoM/src/AIMOVE.c#L4364-L4413))

```c
for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
{
    if(_ai_own_stack_type[itr_stacks] == AISTK_Unknown)
    {
        list_unit_count = _ai_own_stack_unit_count[itr_stacks];
        for(list_unit_idx = 0; list_unit_idx < list_unit_count; list_unit_idx++)
        {
            unit_idx = _ai_own_stack_unit_list[itr_stacks][list_unit_idx];
            if(unit_idx != ST_UNDEFINED)
            {
                if((_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_PURIFY) != 0)
                {
                    unit_wx = _UNITS[unit_idx].wx;
                    unit_wy = _UNITS[unit_idx].wy;
                    if((_map_square_flags[((wp*WORLD_SIZE)+(unit_wy*WORLD_WIDTH)+unit_wx)] & MSF_CORRUPTION) != 0)
                    {
                        AI_Stacks_Order_Purify(unit_idx, itr_stacks, list_unit_idx);
                    }
                    else
                    {
                        g_ai_set_target_caller = 17;
                        AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, target_wx, target_wy, itr_stacks, list_unit_idx);
                    }
                }
            }
        }
    }
}
```

**Phase 3a — stack-type filter** ([line 4367](../../MoM/src/AIMOVE.c#L4367)): only `AISTK_Unknown` (0) stacks — "in transit / purifying / road-building" stacks that haven't been classified as roamers or garrisons. Garrison and roamer stacks are skipped. **IDA-confirmed** (asm `loc_F08D5`: `cmp [stack_type], 0; jz process`).

**Phase 3b — per-unit gates** ([lines 4377-4380](../../MoM/src/AIMOVE.c#L4377-L4380)): skip consumed slots (`unit_idx != ST_UNDEFINED`), then skip non-purifiers (`Abilities & UA_PURIFY`). **IDA-confirmed OG-faithful** (asm 165-167: `cmp unit_idx, e_ST_UNDEFINED; jnz process` — process only when valid; asm 180-181: `test Abilities, UA_PURIFY; jz skip`). Note this is the **correct** ordering — the `ST_UNDEFINED` check precedes the `_UNITS[unit_idx]` deref, so unlike the sibling `AI_Stacks_Do_Meld` Phase 2, there is no out-of-bounds read here.

**Phase 3c — order decision** ([lines 4386-4403](../../MoM/src/AIMOVE.c#L4386-L4403)): if the purifier's own square is corrupted, purify in place (`AI_Stacks_Order_Purify`); otherwise route it toward the scouted target with `g_ai_set_target_caller = 17`. The `else` branch's callee `AI_Stacks_Order_Attack_Target_Or_Goto_Destination` ([defined at AIMOVE.c:4614](../../MoM/src/AIMOVE.c#L4614)) is the ReMoM rename of the OG `AI_Set_Move_Or_Goto_Target` (the OG name survives in-tree only as an `.asm`). The `#ifdef STU_DEBUG` `LOG_DEBUG` trace and the `g_ai_set_target_caller` tag are ReMoM instrumentation with no OG counterpart.

## Code walk — `AI_Stacks_Order_Purify`

```c
void AI_Stacks_Order_Purify(int16_t unit_idx, int16_t unit_list_idx, int16_t list_unit_idx)
{
    if((unit_idx < 0) || (unit_idx >= MAX_UNIT_COUNT)) { return; }
    _UNITS[unit_idx].Status = us_Purify;
    _UNITS[unit_idx].dst_wx = 20;  /* DEDU  timer, like SeekTransport? */
    _ai_own_stack_unit_list[unit_list_idx][list_unit_idx] = ST_UNDEFINED;
}
```

Four lines, **IDA-confirmed OG-faithful** ([AI_Order_Purify.asm](C:/STU/devel/STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr158/AI_Order_Purify.asm)):

1. **Bounds check** — reject `unit_idx` outside `[0, MAX_UNIT_COUNT)`. OG hardcodes `1000` (`or dx,dx; jl ret; cmp dx,1000; jl proceed`); production uses the `MAX_UNIT_COUNT` constant (= 1000).
2. **Set Status** — `_UNITS[unit_idx].Status = us_Purify`. The unit begins purifying; the actual flag-clear plays out over subsequent turns of unit processing.
3. **Set `dst_wx = 20`** — a literal `20` stuffed into `dst_wx` (asm `mov [...dst_wx], 20`). The `dst_wx` field is reused as a countdown/timer here rather than a destination x (compare `AI_Order_SeekTransport`); the Doxygen header calls it "the original game's purify timer", and the inline `DEDU` keeps the question open pending confirmation.
4. **Consume stack slot** — `_ai_own_stack_unit_list[unit_list_idx][list_unit_idx] = ST_UNDEFINED`, so later dispatch slots skip this unit.

OG asm `xor ax, ax` returns 0 (declared `int` in OG, `void` in production — cosmetic).

## Bug catalog

| # | Where | Issue | Severity |
|---|---|---|---|
| B1 | [Line 4308](../../MoM/src/AIMOVE.c#L4308) `OGBUG` comment; bodies at [P1a line 4337](../../MoM/src/AIMOVE.c#L4337) and [P1b line 4343](../../MoM/src/AIMOVE.c#L4343) | Scout phase considers **every** corrupted land-square on the landmass and a full 5×5 box around it, rather than restricting to city catchment areas (excluding corners). The AI will spend purifiers on corruption that does not actually touch a worked city square. **IDA-confirmed OG-faithful** — the asm walks the whole land-square chain and the full 5×5. | OGBUG-faithful; behavioral (purifier targeting wider than catchment) |
| B-quirk | [P1 lines 4326-4357](../../MoM/src/AIMOVE.c#L4326-L4357) vs [P3 lines 4364-4413](../../MoM/src/AIMOVE.c#L4364-L4413) | Scout finds exactly **one** target square (loop breaks on first hit) but dispatch orders **all** idle purifiers — every purifier converges on the same square. IDA-confirmed OG-faithful. | OGBUG-faithful; behavioral (no per-purifier target spread) |

`AI_Stacks_Order_Purify` is OGBUG-clean — bounds check is correct, no OOB, no behavioral surprises.

## ASCII summary

```
AI_Stacks_Do_Purify(landmass_idx, wp)
  ├─ P1: SCOUT — walk landmass land-square list [landmass_node_index]
  │        gate:  MSF_CORRUPTION set  AND  (eval & STRENGTH_MASK) == 0   [B1: whole landmass, not catchment]
  │        5x5 scan around square for  eval == AI_TARGET_SITE
  │           → InRange_Corruption = TRUE; target_wx/wy = THIS square
  │        break on first target (at most ONE target)
  │
  ├─ P2: if InRange_Corruption == FALSE → return
  │
  └─ P3: DISPATCH — for each AISTK_Unknown stack, each unit slot:
           skip if unit_idx == ST_UNDEFINED            [correct order — no OOB]
           skip if NOT (Abilities & UA_PURIFY)
           if own square corrupted:  AI_Stacks_Order_Purify              [purify in place]
                                       └─ bounds-check
                                          _UNITS[u].Status = us_Purify
                                          _UNITS[u].dst_wx = 20    [DEDU: timer]
                                          _ai_own_stack_unit_list[s][u] = ST_UNDEFINED
           else:                      Order_Attack/GOTO  target    [caller=17; all purifiers → same square]
```

## Position in the dispatch chain

```
AI_Set_Unit_Orders(player_idx)
  └─ for wp in [0, 1]:
       └─ for landmass_idx in [1, NUM_LANDMASSES):
            ├─ slot 1: AI_Stacks_Init_Build_Target_Order
            ├─ slot 2: AI_Stacks_Move_Out_NonMilitary_Garrisoned
            ├─ slot 3: AI_Stacks_Survey_Expedition_Forces
            ├─ slot 4: AI_Stacks_Do_Meld     → AI_Stacks_Order_Meld
            ├─ slot 5: AI_Stacks_Do_Settle   → AI_Order_Settle
            ├─ slot 6: AI_Stacks_Do_Purify   → AI_Stacks_Order_Purify     ◄── HERE
            ├─ slot 7: AI_Stacks_Do_RoadBuild       → AI_Stacks_Order_RoadBuild
            └─ ... slots 8-13 ...
```

**Sibling slot-4-through-7 functions** (per-job order-setters). All share the same shape: scan `_ai_own_stack_*` for the relevant ability-bit, find a target, issue the order via the matching order helper (bounds-check + set `Status` + consume slot):
- `AI_Stacks_Do_Meld` → `AI_Stacks_Order_Meld` (`UA_MELD`)
- `AI_Stacks_Do_Settle` → `AI_Order_Settle` (settler ability)
- **`AI_Stacks_Do_Purify`** (this) → `AI_Stacks_Order_Purify` (`UA_PURIFY`)
- `AI_Stacks_Do_RoadBuild` → `AI_Stacks_Order_RoadBuild` (road-build ability)

(`AI_Stacks_Do_Meld`/`AI_Stacks_Do_Purify` carry the `AI_Stacks_` prefix on both halves; `Settle`/`RoadBuild` have not yet been renamed from their OG `AI_Do_*`/`AI_Order_*` form.)

Running purifiers at slot 6 (before the combat dispatcher at slot 9, [`AI_Stacks_Roamers_Target_Or_Deploy`](AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md)) pre-claims them for their dedicated job — the slot-consumption (`ST_UNDEFINED` write) makes later slots skip these units.

## Related references

- [AIMOVE-AI_Set_Unit_Orders.md](AIMOVE-AI_Set_Unit_Orders.md) — dispatcher; calls `AI_Stacks_Do_Purify` as slot 6
- [AIMOVE-AI_Do_Meld.md](AIMOVE-AI_Do_Meld.md) — sibling slot-4 melder dispatcher (same shape; its Phase 2 has the OOB-read OGBUG this one avoids)
- [AIMOVE-AI_Stacks_Do_Settle.md](AIMOVE-AI_Stacks_Do_Settle.md) — sibling slot-5 settler dispatcher
- [MoM-AI-Move-ai_own_stack.md](MoM-AI-Move-ai_own_stack.md) — `_ai_own_stack_*` parallel-arrays + `AISTK_*` type enum (`AISTK_Unknown = 0`)
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Do_Purify.asm` — IDA Pro 5.5 disassembly (ground truth, verified)
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Order_Purify.asm` — IDA Pro 5.5 disassembly (ground truth, verified)
