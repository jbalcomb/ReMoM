AISPELL-AITP_Enchant_Road.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_EnchantRoad.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_EnchantRoad.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
                |-> Get_Map_Square_Target_For_Spell()
                    switch(spell_idx)
                        case spl_Enchant_Road:
                            |-> AITP_Enchant_Road()

---

# `AITP_Enchant_Road` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Enchant_Road` | [AISPELL.c:5291-5353](../../MoM/src/AISPELL.c#L5291-L5353) | AI target picker for **Enchant Road**: of the caster's own cities, pick the one with the **strongest garrison** that has a road but **not** yet an enchanted road, and return its map square. Returns `ST_TRUE` with `*targeted_wx/_wy/_wp` set, `ST_FALSE` otherwise. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:5291](../../MoM/src/AISPELL.c#L5291)) is a faithful reconstruction of `AITP_EnchantRoad.asm` (production renamed `AITP_EnchantRoad` → `AITP_Enchant_Road`; the asm/`.c` filename keeps the OG `AITP_EnchantRoad`). It compiles and links as part of AISPELL.c / momlib. The strongest-garrison scan, the road/enchanted-road tests, and the return all match — **including the preserved OG bug** where the 5×5 offsets are never applied ([B1](#b1-ogbug--catchment-offsets-never-applied)). The asm is the authority.

## Purpose

`Get_Map_Square_Target_For_Spell` routes **Enchant Road** here ([case spl_Enchant_Road](../../MoM/src/AISPELL.c#L5031)). Enchant Road upgrades ordinary roads to enchanted roads (free movement). The AI tries to apply it to the best-defended city: it scans owned cities for the highest `_ai_all_own_garrison_strengths[]` (asm `AI_Own_Garr_Strs`) entry that has a road but no enchanted road yet, and returns that city's coordinates.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Get_Map_Square_Target_For_Spell` | [AISPELL.c:5031](../../MoM/src/AISPELL.c#L5031) | `switch(spell_idx) case spl_Enchant_Road` → here. No separate priority probe; Enchant Road is a map-square spell. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AITP_EnchantRoad.asm`.

```c
best_city_idx = ST_UNDEFINED;  strongest_garrison = 0;                  // asm  di=-1, Strongest_Garrison=0

for(itr_cities = 0; itr_cities < _cities; itr_cities++)                 // asm  cx loop
{
    garrison_strength = _ai_all_own_garrison_strengths[itr_cities];     // asm  AI_Own_Garr_Strs[cx]
    if(garrison_strength > strongest_garrison)                          // asm  jg  (signed; outer gate)
    {
        have_road = ST_FALSE;  enchanted_road = ST_FALSE;
        for(wy_offset = -2; wy_offset < 3; wy_offset++)                 // asm  Coord_Adjust_1 -2..2
        {
            for(wx_offset = -2; wx_offset < 3; wx_offset++)            // asm  Coord_Adjust_2 -2..2
            {
                if(wy_offset == 0 && wx_offset == 0) continue;          // asm  skip the centre

                /* B1: wy_offset / wx_offset are NEVER added — always the city centre */
                if((GET_MAP_SQUARE_FLAG(_CITIES[itr_cities].wx, _CITIES[itr_cities].wy, _CITIES[itr_cities].wp) & MSF_ROAD) != 0)  // asm test ...,MSF_ROAD
                {
                    have_road = ST_TRUE;
                    if((GET_MAP_SQUARE_FLAG(_CITIES[itr_cities].wx, _CITIES[itr_cities].wy, _CITIES[itr_cities].wp) & MSF_EROAD) != 0)  // asm test ...,MSF_EROAD
                        enchanted_road = ST_TRUE;
                }
            }
        }

        if(have_road == ST_TRUE && enchanted_road == ST_FALSE)         // asm  Have_Road==1 && Enchanted_Road==0
        {
            best_city_idx = itr_cities;                                 // asm  di = cx
            strongest_garrison = garrison_strength;
        }
    }
}

if(best_city_idx == ST_UNDEFINED) return ST_FALSE;                      // asm  cmp di,-1 / xor ax,ax
*targeted_wx = _CITIES[best_city_idx].wx;  // .wy, .wp likewise;  return ST_TRUE;   // asm  store city[di].* / ax=1
```

**Faithful 1:1** — the strongest-garrison outer gate (signed `jg`), the 5×5 loop with the centre `continue`, the `MSF_ROAD` then nested `MSF_EROAD` tests reading the **city-centre** flags (B1), the `have_road && !enchanted_road` selection, and the return of `_CITIES[best_city_idx]`'s coordinates all match the asm. `GET_MAP_SQUARE_FLAG(wx, wy, wp)` expands to `_map_square_flags[wp*WORLD_SIZE + wy*WORLD_WIDTH + wx]` — the exact address the asm builds (`wp*0960h + wy*3Ch + wx`).

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `_ai_all_own_garrison_strengths[]` | `AI_Own_Garr_Strs` | `extern int16_t *` ([MOM_DAT.h:3035](../../MoX/src/MOM_DAT.h#L3035)); word-indexed. |
| `GET_MAP_SQUARE_FLAG(wx,wy,wp)` | `_map_square_flags + wp*0960h + wy*3Ch + wx` | macro ([MOX_DEF.h:494](../../MoX/src/MOX_DEF.h#L494)); `_map_square_flags` is `uint8_t *` ([MOM_DAT.h:4076](../../MoX/src/MOM_DAT.h#L4076)); `WORLD_SIZE=2400`, `WORLD_WIDTH=60`. |
| `MSF_ROAD` / `MSF_EROAD` | `MSF_ROAD` / `MSF_EROAD` | `0x08` / `0x10` ([Terrain.h:74-75](../../MoM/src/Terrain.h#L74-L75)). |
| `_CITIES[i]` stride | `imul 72h` | `sizeof(s_CITY) = 114`. |
| `niu_player_index` | `UU_Player_Index` | assigned from `player_idx`, never read — dead local, preserved. |

## B1 (OGBUG) — catchment offsets never applied

The function builds a 5×5 offset loop (`wy_offset`/`wx_offset`, −2..2, skipping the centre) as if to scan the city's catchment for roads — but **the offsets are never added to `wx`/`wy`**: every iteration reads `GET_MAP_SQUARE_FLAG(city.wx, city.wy, city.wp)`, the **city centre**, 24 times. Verified in the asm: `loc_E9862` computes the flags address from `city[cx].wy`/`.wx` with no `Coord_Adjust_*` term (the offsets are only used as loop counters). drake178's note matches: *"ignores actual surrounding roads, and the city itself always has one…"*

Net effect: a city's own centre tile virtually always has a road, so `have_road` is essentially always set, `enchanted_road` reflects only the centre tile, and the picker reduces to *"the strongest-garrison owned city whose centre tile has a road but not an enchanted road."* The surrounding catchment is never consulted. Faithfully preserved — keep the offsets unused. (The in-code comment at [5322](../../MoM/src/AISPELL.c#L5322) documents it.)

## OG quirks (preserve when reconstructing)

- **B1** — the 5×5 offsets are computed but never applied (above).
- **Dead local**: `niu_player_index` (asm `UU_Player_Index`) is set from `player_idx` and never used — preserved.
- **Tie-break**: `> strongest_garrison` is strictly-greater (signed `jg`), so among equal-garrison cities the **lowest city index** wins.

## Sub-functions / external calls

- **`GET_MAP_SQUARE_FLAG(wx, wy, wp)`** — macro read of `_map_square_flags[]` ([MOX_DEF.h:494](../../MoX/src/MOX_DEF.h#L494)).
- Otherwise reads `_CITIES[]`, `_ai_all_own_garrison_strengths[]`, `_cities`; writes through `*targeted_wx`/`*targeted_wy`/`*targeted_wp`.

## Related references

- `…\ovr156\AITP_EnchantRoad.asm` — IDA Pro 5.5 disassembly (the authority; OG name `AITP_EnchantRoad`).
- `…\ovr156\AITP_EnchantRoad.c` — Piethawn IDA-C.
- `Get_Map_Square_Target_For_Spell` ([AISPELL.c:5002](../../MoM/src/AISPELL.c#L5002)) — the map-square target router that calls this for Enchant Road.
- [AISPELL-AITP_Floating_Island.md](AISPELL-AITP_Floating_Island.md) — sibling map-square target picker (also routed via `Get_Map_Square_Target_For_Spell`).
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
